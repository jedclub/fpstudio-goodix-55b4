#pragma once

#include "vkmatch.h"
#include "profile.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>

namespace vkmatch {

// A contact-region mask is deliberately distinct from a ridge template.  It
// answers only whether a pixel belongs to the finger-bearing portion of this
// tiny sensor frame (1) or to the calibrated background (0).  The actual
// identity evidence remains intensity and ridge-direction agreement.
struct RidgeRegion {
    uint32_t width = 0, height = 0;
    std::vector<uint8_t> pixels; // binary: 0 = background, 1 = finger region
    uint32_t contactPixels = 0;
    uint32_t contactMask = 0;    // 4x4 summary, suitable for diagnostic UI
    double coverage = 0;
    // Geometric centre of the binary contact component.  This is deliberately
    // not a claimed fingerprint core/delta: a 108x88 partial sensor normally
    // sees only a small ridge fragment.
    double centerX = 0, centerY = 0;
    bool usable = false;

    bool at(int x, int y) const {
        return x >= 0 && y >= 0 && x < int(width) && y < int(height) &&
               pixels[size_t(y) * width + x] != 0;
    }
};

// The Goodix driver makes calibrated untouched pixels black before it writes
// PNG/PGM research frames.  A 24/255 floor matches that driver's contact
// metric.  Ridges may themselves dip below the floor, therefore we first use
// local support and then retain the largest connected contact component rather
// than treating every dark ridge pixel as background.
inline RidgeRegion extractRidgeRegion(const Image &image) {
    FPSTUDIO_PROFILE_SCOPE("ridge/extractRidgeRegion");
    RidgeRegion out;
    if (image.width < 5 || image.height < 5 ||
        image.pixels.size() != size_t(image.width) * image.height)
        return out;
    out.width = image.width;
    out.height = image.height;
    const int w = int(image.width), h = int(image.height);
    const size_t count = size_t(w) * h;
    std::vector<uint8_t> live(count, 0), supported(count, 0);
    constexpr float kDeadFloor = 24.f / 255.f;
    for (size_t i = 0; i < count; ++i)
        live[i] = image.pixels[i] > kDeadFloor;

    // A 5x5 support count bridges dark valleys inside an otherwise contacted
    // patch, while an isolated hot/background pixel cannot become foreground.
    //
    // Counted separably rather than by re-reading the 25-pixel window for every
    // output pixel: a horizontal 5-wide running count per row, then a vertical
    // running count over five of those rows. 25 reads per pixel become two.
    //
    // Only the five rows inside the window are kept - about 2 KB, so the whole
    // working set stays in L1 - and, exactly as in the minutiae window, the row
    // arriving and the row leaving share a ring slot, so the departing row is
    // subtracted before the arriving one is written over it.
    //
    // These are counts of 0/1 bytes, so unlike the floating-point window in
    // minutiae.h this reassociation is not merely equivalent to the direct form
    // but bit-identical to it: integer addition is exact and associative.
    {
        constexpr int radius = 2, window = 2 * radius + 1;
        std::vector<int> ring(size_t(window) * w, 0), column(size_t(w), 0);
        auto fillRow = [&](int r) {
            const uint8_t *row = live.data() + size_t(r) * w;
            int *out = ring.data() + size_t(r % window) * w;
            int running = 0;
            for (int x = 0; x <= std::min(w - 1, radius); ++x)
                running += row[x];
            for (int x = 0; x < w; ++x) {
                out[x] = running;
                const int add = x + radius + 1, drop = x - radius;
                if (add < w) running += row[add];
                if (drop >= 0) running -= row[drop];
            }
        };
        for (int y = 0; y <= std::min(h - 1, radius); ++y) {
            fillRow(y);
            const int *slot = ring.data() + size_t(y % window) * w;
            for (int x = 0; x < w; ++x) column[x] += slot[x];
        }
        for (int y = 0; y < h; ++y) {
            uint8_t *out = supported.data() + size_t(y) * w;
            for (int x = 0; x < w; ++x) out[x] = column[x] >= 7;
            const int drop = y - radius, add = y + radius + 1;
            if (drop >= 0) {
                const int *slot = ring.data() + size_t(drop % window) * w;
                for (int x = 0; x < w; ++x) column[x] -= slot[x];
            }
            if (add < h) {
                fillRow(add);                   // reuses the slot just vacated
                const int *slot = ring.data() + size_t(add % window) * w;
                for (int x = 0; x < w; ++x) column[x] += slot[x];
            }
        }
    }

    // Keep one physical contact.  Separating disconnected noise here is
    // important because a sensor-edge artefact otherwise looks like a second
    // matching island.  Eight-neighbour connectivity preserves diagonal ridges.
    std::vector<int> label(count, -1);
    std::vector<uint32_t> sizes;
    // One buffer for every component instead of a fresh std::queue per
    // component. std::queue is deque-backed, so each component used to mean a
    // round of chunk allocations for a frontier that never exceeds the frame;
    // this reserves the worst case once and rewinds. Still first-in-first-out,
    // so the traversal order - and therefore the labelling - is unchanged.
    std::vector<int> pending;
    pending.reserve(count);
    int component = 0;
    for (int start = 0; start < int(count); ++start) {
        if (!supported[start] || label[start] != -1)
            continue;
        pending.clear();
        size_t head = 0;
        pending.push_back(start);
        label[start] = component;
        uint32_t size = 0;
        while (head < pending.size()) {
            const int p = pending[head++];
            ++size;
            const int x = p % w, y = p / w;
            for (int yy = std::max(0, y - 1); yy <= std::min(h - 1, y + 1); ++yy)
                for (int xx = std::max(0, x - 1); xx <= std::min(w - 1, x + 1); ++xx) {
                    const int q = yy * w + xx;
                    if (supported[q] && label[q] == -1) {
                        label[q] = component;
                        pending.push_back(q);
                    }
                }
        }
        sizes.push_back(size);
        ++component;
    }
    if (sizes.empty())
        return out;
    const int largest = int(std::max_element(sizes.begin(), sizes.end()) - sizes.begin());
    out.pixels.assign(count, 0);
    double sumX = 0, sumY = 0;
    for (size_t i = 0; i < count; ++i) {
        if (label[i] == largest) {
            out.pixels[i] = 1;
            ++out.contactPixels;
            sumX += i % w;
            sumY += i / w;
        }
    }
    out.coverage = double(out.contactPixels) / count;
    if (out.contactPixels) {
        out.centerX = sumX / out.contactPixels;
        out.centerY = sumY / out.contactPixels;
    }
    // The driver already rejects below 62% contact.  Keep this lower floor in
    // the matcher so research can diagnose a marginal capture without turning
    // a local speckle into a plausible region.
    out.usable = out.coverage >= .20;

    constexpr int grid = 4;
    for (int gy = 0; gy < grid; ++gy) {
        for (int gx = 0; gx < grid; ++gx) {
            const int x0 = gx * w / grid, x1 = (gx + 1) * w / grid;
            const int y0 = gy * h / grid, y1 = (gy + 1) * h / grid;
            int active = 0;
            for (int y = y0; y < y1; ++y)
                for (int x = x0; x < x1; ++x)
                    active += out.at(x, y);
            if (active * 2 >= (x1 - x0) * (y1 - y0))
                out.contactMask |= uint32_t(1) << (gy * grid + gx);
        }
    }
    return out;
}

// Bilinear positions need a mask decision too.  Requiring a majority of the
// four source pixels prevents a transformed sample from leaking across a
// contact boundary while still permitting fractional alignment.
inline bool regionContains(const RidgeRegion &region, double x, double y) {
    const int ix = int(x), iy = int(y);
    const double fx = x - ix, fy = y - iy;
    const double weight = (1 - fx) * (1 - fy) * region.at(ix, iy) +
        fx * (1 - fy) * region.at(ix + 1, iy) +
        (1 - fx) * fy * region.at(ix, iy + 1) + fx * fy * region.at(ix + 1, iy + 1);
    return weight >= .60;
}

} // namespace vkmatch
