// The GPU placer must agree with the CPU one, and must find a placement that
// was put there on purpose.
//
// Parity matters more here than speed: the whole reason for moving this to the
// GPU is to search poses the CPU cannot afford, so nothing else will be
// checking the answers.
#include "mapplace.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <vector>

namespace {
int failures = 0;
void expect(bool ok, const char *what) {
    if (!ok) { std::printf("FAIL: %s\n", what); ++failures; }
    else std::printf("  ok: %s\n", what);
}

constexpr uint32_t kMap = 200, kW = 108, kH = 88;

// A synthetic print: curved ridges, so rotation is distinguishable and a wrong
// offset by one ridge period does not score the same as the right one.
float ridge(float x, float y) {
    const float r = std::sqrt((x - 40) * (x - 40) + (y + 30) * (y + 30));
    return 128.f + 90.f * std::sin(r * 0.72f);
}
}

int main(int argc, char **argv) {
    if (argc < 2) { std::printf("usage: mapplace-test <spirv>\n"); return 2; }
    std::vector<float> mapPixels(size_t(kMap) * kMap, 0.f), mapWeights(size_t(kMap) * kMap, 0.f);
    const int placedX = 46, placedY = 52;
    for (uint32_t y = 0; y < kH; ++y)
        for (uint32_t x = 0; x < kW; ++x) {
            const size_t i = size_t(placedY + y) * kMap + placedX + x;
            mapPixels[i] = ridge(float(x), float(y));
            mapWeights[i] = 1.f;
        }
    std::vector<float> probe(size_t(kW) * kH);
    for (uint32_t y = 0; y < kH; ++y)
        for (uint32_t x = 0; x < kW; ++x) probe[size_t(y) * kW + x] = ridge(float(x), float(y));

    std::vector<vkmap::Pose> poses;
    for (int dy = -12; dy <= 12; dy += 2)
        for (int dx = -12; dx <= 12; dx += 2)
            for (float a : {-6.f, -3.f, 0.f, 3.f, 6.f})
                poses.push_back({float(placedX + dx), float(placedY + dy), a});

    std::unique_ptr<vkmap::Placer> placer;
    try {
        placer = std::make_unique<vkmap::Placer>(argv[1]);
    } catch (const std::exception &e) {
        std::printf("GPU unavailable (%s) - checking the CPU path only\n", e.what());
        const auto exact = vkmap::cpuScore(mapPixels, mapWeights, kMap, probe, kW, kH,
                                           {float(placedX), float(placedY), 0.f});
        expect(exact.ncc > 0.99f, "CPU finds the planted placement");
        return failures ? 1 : 0;
    }
    std::printf("GPU: %s\n", placer->deviceName().c_str());
    placer->setMap(mapPixels, mapWeights, kMap);
    placer->setProbe(probe, kW, kH);
    const auto gpu = placer->run(poses, 2);
    expect(gpu.size() == poses.size(), "one result per pose");

    double worst = 0;
    size_t bestIndex = 0;
    for (size_t i = 0; i < poses.size(); ++i) {
        const auto cpu = vkmap::cpuScore(mapPixels, mapWeights, kMap, probe, kW, kH, poses[i], 2);
        worst = std::max(worst, std::fabs(double(cpu.ncc) - double(gpu[i].ncc)));
        if (gpu[i].ncc > gpu[bestIndex].ncc) bestIndex = i;
    }
    std::printf("  최대 GPU/CPU 차이 %.6f\n", worst);
    expect(worst < 2e-3, "GPU agrees with CPU on every pose");
    const auto &best = poses[bestIndex];
    std::printf("  최고 pose: (%.0f,%.0f) %.1f도  ncc %.4f\n",
                best.offsetX, best.offsetY, best.angleDegrees, gpu[bestIndex].ncc);
    expect(std::fabs(best.offsetX - placedX) <= 1 && std::fabs(best.offsetY - placedY) <= 1,
           "best pose is where the capture was planted");
    expect(std::fabs(best.angleDegrees) < 1e-3, "best pose has no spurious rotation");
    expect(gpu[bestIndex].ncc > 0.99f, "planted placement scores near 1");
    expect(gpu[bestIndex].overlap > 0.9f, "planted placement overlaps the covered map");

    // Nothing placed anywhere: every pose must be unusable rather than lucky.
    std::vector<float> empty(size_t(kMap) * kMap, 0.f);
    placer->setMap(empty, empty, kMap);
    const auto none = placer->run(poses, 2);
    bool anyUsable = false;
    for (const auto &r : none) if (r.ncc > -1.5f) anyUsable = true;
    expect(!anyUsable, "an empty map yields no usable pose");

    std::printf(failures ? "\n%d FAILED\n" : "\nall passed\n", failures);
    return failures ? 1 : 0;
}
