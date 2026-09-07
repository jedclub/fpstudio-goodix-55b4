#include "vkmatch.h"
#include "interior.h"
#include "ridge_roi.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
constexpr uint32_t size = 65;

void require(bool ok, const std::string &message) {
    if (!ok) throw std::runtime_error(message);
}

void near(float actual, float expected, float tolerance, const std::string &message) {
    require(std::isfinite(actual) && std::isfinite(expected), message + ": non-finite score");
    require(std::abs(actual - expected) <= tolerance,
            message + ": got " + std::to_string(actual) + ", expected " + std::to_string(expected));
}

void rejects(const std::function<void()> &action, const std::string &message) {
    bool rejected = false;
    try { action(); } catch (const std::exception &) { rejected = true; }
    require(rejected, message + ": invalid input was accepted");
}

// Analytic, asymmetric, non-biometric texture; values remain strictly in [0,1].
float texture(float x, float y) {
    return 0.5f + 0.18f * std::sin(0.31f * x + 0.17f * y)
                + 0.13f * std::cos(0.07f * x - 0.43f * y)
                + 0.09f * std::sin(0.011f * x * y + 0.4f);
}

vkmatch::Image makeImage(const std::function<float(float, float)> &f) {
    vkmatch::Image image;
    image.width = image.height = size;
    image.pixels.reserve(size * size);
    for (uint32_t y = 0; y < size; ++y)
        for (uint32_t x = 0; x < size; ++x)
            image.pixels.push_back(f(static_cast<float>(x), static_cast<float>(y)));
    return image;
}

vkmatch::Image noise(uint32_t seed) {
    return makeImage([&](float, float) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        return static_cast<float>(seed >> 8) / 16777215.0f;
    });
}

void bounded(const vkmatch::Score &score, const std::string &label) {
    require(std::isfinite(score.ncc) && std::abs(score.ncc) <= 1.0001f, label + ": NCC bounds");
    require(std::isfinite(score.gradient) && std::abs(score.gradient) <= 1.0001f, label + ": gradient bounds");
    require(std::isfinite(score.overlap) && score.overlap >= 0 && score.overlap <= 1.0001f,
            label + ": overlap bounds");
    require(std::isfinite(score.mae) && score.mae >= 0 && score.mae <= 1.0001f, label + ": MAE bounds");
}

void parity(const vkmatch::Score &gpu, const vkmatch::Score &cpu, const std::string &label) {
    bounded(gpu, label + " GPU");
    bounded(cpu, label + " CPU");
    near(gpu.ncc, cpu.ncc, 2e-3f, label + " NCC");
    // Vulkan implementations may contract multiply-adds and choose a
    // different legal reduction schedule. That is most visible when the
    // expected signed gradient correlation is almost zero; retain a bounded
    // cross-driver tolerance without relaxing the score-range checks.
    near(gpu.gradient, cpu.gradient, 1.5e-2f, label + " gradient");
    near(gpu.overlap, cpu.overlap, 2e-3f, label + " overlap");
    near(gpu.mae, cpu.mae, 2e-3f, label + " MAE");
}

void matchingTests(vkmatch::Matcher &matcher) {
    const auto reference = makeImage(texture);
    const auto fullRegion=vkmatch::extractRidgeRegion(reference);
    require(fullRegion.usable&&fullRegion.coverage>.99,"non-background fixture has a full contact region");
    const auto interiorIdentity=vkmatch::assessInterior(reference,reference,{0,0,0,0,0,1});
    require(interiorIdentity.consistent,"interior structure identity");
    require(interiorIdentity.supportedTiles>=12,"identity spatial tile coverage");
    auto borderA=noise(0x99887766u),borderB=noise(0x11223344u);
    for(uint32_t y=0;y<size;++y)for(uint32_t x=0;x<size;++x) {
        if(x<8||y<8||x>=size-8||y>=size-8) {
            const float p=((x/2+y/2)%2)?1.f:0.f;
            borderA.pixels[y*size+x]=borderB.pixels[y*size+x]=p;
        }
    }
    const auto borderEvidence=vkmatch::assessInterior(borderA,borderB,{0,0,0,0,0,1});
    require(!borderEvidence.consistent,"identical borders cannot establish interior consistency");
    require(std::abs(borderEvidence.ncc)<.2,"interior ignores shared outer border");
    auto contactOnly=makeImage([](float,float){return 0.f;});
    for(uint32_t y=14;y<52;++y)for(uint32_t x=18;x<50;++x)
        contactOnly.pixels[y*size+x]=((x+y)%7==0)?.02f:.72f;
    const auto contactRegion=vkmatch::extractRidgeRegion(contactOnly);
    require(contactRegion.usable,"connected finger contact is segmented");
    require(contactRegion.at(32,30)&&contactRegion.at(25,21),"dark ridge valleys remain inside contact mask");
    require(!contactRegion.at(3,3),"calibrated background remains zero in contact mask");
    require(contactRegion.contactMask!=0&&contactRegion.contactMask!=0xffff,"contact mask is spatially selective");
    const auto noContact=vkmatch::extractRidgeRegion(makeImage([](float,float){return 0.f;}));
    require(!noContact.usable,"black background has no contact region");
    const auto shifted = makeImage([](float x, float y) { return texture(x - 4, y + 3); });
    // For a +90 degree reference-to-probe rotation, inverse warp is (y, 64-x).
    const auto rotated = makeImage([](float x, float y) { return texture(y, size - 1.0f - x); });
    // This warp is deliberately anchored away from the image centre. It
    // catches a shader/CPU regression where only the old centre pivot works.
    constexpr float pivotX=17.0f,pivotY=25.0f,anchorX=43.0f,anchorY=35.0f,anchorAngle=17.0f,anchorScale=1.02f;
    const auto anchored=makeImage([=](float x,float y) {
        constexpr float pi=3.14159265358979323846f;
        const float c=std::cos(anchorAngle*pi/180.0f),s=std::sin(anchorAngle*pi/180.0f);
        const float dx=(x-anchorX)/anchorScale,dy=(y-anchorY)/anchorScale;
        return texture(pivotX+c*dx+s*dy,pivotY-s*dx+c*dy);
    });
    const auto flat = makeImage([](float, float) { return 0.25f; });
    const auto otherFlat = makeImage([](float, float) { return 0.75f; });
    const auto inverted = makeImage([](float x, float y) { return 1.0f - texture(x, y); });
    const std::vector<vkmatch::Image> images = {
        reference, shifted, rotated, flat, otherFlat, noise(0x1ac38f71u), noise(0x76fd28a9u), inverted, anchored
    };
    matcher.setImages(images);
    std::vector<vkmatch::Job> checks = {
        {0, 0, 0, 0, 0, 1}, {0, 1, 0, 4, -3, 1}, {0, 2, 90, 0, 0, 1},
        {3, 3, 0, 0, 0, 1}, {3, 4, 0, 0, 0, 1}, {5, 6, 0, 0, 0, 1},
        {0, 7, 0, 0, 0, 1}, {0, 0, 0, 200, 0, 1}
    };
    vkmatch::Job anchoredJob{0,8,anchorAngle,0,0,anchorScale};
    anchoredJob.pivotX=pivotX;anchoredJob.pivotY=pivotY;anchoredJob.anchorX=anchorX;anchoredJob.anchorY=anchorY;
    checks.push_back(anchoredJob);
    const auto batch = matcher.run(checks, 1);
    require(batch.scores.size() == checks.size(), "score count preserves job count");
    require(std::isfinite(batch.wallMs) && batch.wallMs >= 0, "finite nonnegative wall timing");
    require(std::isfinite(batch.gpuMs) && (batch.gpuMs == -1 || batch.gpuMs >= 0), "valid optional GPU timing");
    for (size_t i = 0; i < checks.size(); ++i)
        parity(batch.scores[i], vkmatch::cpuScore(images[checks[i].reference], images[checks[i].probe], checks[i], 1),
               "known job " + std::to_string(i));
    for (size_t i = 0; i < 3; ++i) {
        near(batch.scores[i].ncc, 1, 5e-4f, "known warp NCC " + std::to_string(i));
        near(batch.scores[i].gradient, 1, 5e-4f, "known warp gradient " + std::to_string(i));
        near(batch.scores[i].mae, 0, 5e-4f, "known warp MAE " + std::to_string(i));
    }
    near(batch.scores[0].overlap, 1, 1e-6f, "identity overlap");
    near(batch.scores[1].overlap, (59.0f * 60.0f) / (63.0f * 63.0f), 1e-6f, "translation overlap");
    near(batch.scores[3].ncc, 0, 1e-6f, "constant image has no correlation evidence");
    near(batch.scores[3].gradient, 0, 1e-6f, "constant image has no gradient evidence");
    near(batch.scores[4].ncc, 0, 1e-6f, "different constant images have no correlation evidence");
    near(batch.scores[4].gradient, 0, 1e-6f, "different constant images have no gradient evidence");
    near(batch.scores[4].mae, 0.5f, 1e-6f, "constant image MAE");
    const auto textureAgainstFlat=matcher.run({{0,4,0,0,0,1}},1).scores.at(0);
    parity(textureAgainstFlat,vkmatch::cpuScore(reference,otherFlat,{0,4,0,0,0,1},1),"texture against flat probe");
    near(textureAgainstFlat.gradient,0,1e-6f,"flat probe has no gradient evidence");
    require(std::abs(batch.scores[5].ncc) < 0.1f && std::abs(batch.scores[5].gradient) < 0.1f,
            "independent deterministic noise should have low signed correlation");
    near(batch.scores[6].ncc, -1, 5e-4f, "inverted image signed NCC");
    near(batch.scores[6].gradient, -1, 5e-4f, "inverted image signed gradient");
    near(batch.scores[7].overlap, 0, 1e-6f, "no-overlap warp");
    near(batch.scores[8].ncc, 1, 5e-4f, "off-centre anchored warp NCC");
    near(batch.scores[8].gradient, 1, 5e-4f, "off-centre anchored warp gradient");
    near(batch.scores[8].mae, 0, 3e-3f, "off-centre anchored warp MAE");

    // Multiple source/probe pairs and subpixel transforms exercise indexing,
    // interpolation, scale, rotation and many independent workgroups.
    std::vector<vkmatch::Job> many;
    for (uint32_t i = 0; i < 513; ++i) {
        many.push_back({i % 8, (i / 8) % 8,
                       static_cast<float>(static_cast<int>(i % 37) - 18) * 2.7f,
                       static_cast<float>(static_cast<int>(i % 17) - 8) * 0.73f,
                       static_cast<float>(static_cast<int>(i % 13) - 6) * 0.61f,
                       0.85f + static_cast<float>(i % 7) * 0.05f});
    }
    for (uint32_t stride : {1u, 2u, 4u}) {
        const auto result = matcher.run(many, stride);
        require(result.scores.size() == many.size(), "large batch output count");
        for (size_t i = 0; i < many.size(); ++i) {
            const auto &job = many[i];
            parity(result.scores[i], vkmatch::cpuScore(images[job.reference], images[job.probe], job, stride),
                   "batch stride " + std::to_string(stride) + " job " + std::to_string(i));
        }
    }

    // Re-upload verifies old image data cannot leak into later dispatches.
    matcher.setImages({flat, otherFlat});
    const auto replaced = matcher.run({{0, 1, 0, 0, 0, 1}}, 2);
    require(replaced.scores.size() == 1, "re-upload output count");
    near(replaced.scores[0].mae, 0.5f, 1e-6f, "re-upload replaces storage");
    // Equal-size allocation reuse must upload changed pixels, not keep the old bank.
    matcher.setImages({reference, reference});
    const auto reused = matcher.run({{0, 1, 0, 0, 0, 1}}, 2);
    near(reused.scores[0].ncc, 1, 5e-4f, "same-size re-upload changes pixel contents");
    near(reused.scores[0].mae, 0, 5e-4f, "same-size re-upload identical images");

    // Non-square dimensions guard against accidental width/height interchange.
    vkmatch::Image rectangular;
    rectangular.width = 47;
    rectangular.height = 29;
    for (uint32_t y = 0; y < rectangular.height; ++y)
        for (uint32_t x = 0; x < rectangular.width; ++x)
            rectangular.pixels.push_back(texture(static_cast<float>(x), static_cast<float>(y)));
    matcher.setImages({rectangular});
    const std::vector<vkmatch::Job> rectangleJobs = {{0, 0, 0, 0, 0, 1}, {0, 0, -17.5f, 1.2f, -2.1f, 1.03f}};
    const auto rectangleResult = matcher.run(rectangleJobs, 3);
    require(rectangleResult.scores.size() == rectangleJobs.size(), "rectangular batch size");
    for (size_t i = 0; i < rectangleJobs.size(); ++i)
        parity(rectangleResult.scores[i], vkmatch::cpuScore(rectangular, rectangular, rectangleJobs[i], 3),
               "rectangular job " + std::to_string(i));
}

void chunkBoundaryTests(vkmatch::Matcher &matcher) {
    std::vector<vkmatch::Image> bank;
    bank.reserve(128);
    for (uint32_t i = 0; i < 128; ++i)
        bank.push_back(makeImage([i](float x, float y) {
            return texture(x + static_cast<float>(i) * 0.37f, y - static_cast<float>(i) * 0.23f);
        }));
    matcher.setImages(bank);
    require(matcher.run({}, 4).scores.empty(), "empty jobs return empty scores");
    std::vector<vkmatch::Job> jobs;
    jobs.reserve(2053);
    // The final five jobs exercise the second 2,048-job dispatch chunk.
    for (uint32_t i = 0; i < 2053; ++i)
        jobs.push_back({i % 128, (i * 19 + 7) % 128,
                        static_cast<float>(static_cast<int>(i % 37) - 18) * 2.7f,
                        static_cast<float>(static_cast<int>(i % 17) - 8) * 0.73f,
                        static_cast<float>(static_cast<int>(i % 13) - 6) * 0.61f,
                        0.85f + static_cast<float>(i % 7) * 0.05f});
    const auto gpu = matcher.run(jobs, 4);
    require(gpu.scores.size() == jobs.size(), "multi-chunk output count");
    std::vector<vkmatch::Score> cpu;
    cpu.reserve(jobs.size());
    const auto cpuStart = std::chrono::steady_clock::now();
    for (const auto &job : jobs)
        cpu.push_back(vkmatch::cpuScore(bank[job.reference], bank[job.probe], job, 4));
    const double cpuMs = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - cpuStart).count();
    for (size_t i = 0; i < jobs.size(); ++i)
        parity(gpu.scores[i], cpu[i], "multi-chunk job " + std::to_string(i));
    std::cout << "BENCHMARK (informational, no speed pass/fail): 128 synthetic 65x65 images, "
                 "2053 jobs, stride 4; GPU dispatch wall_ms=" << gpu.wallMs
              << ", gpu_ms=" << gpu.gpuMs << ", CPU reference loop wall_ms=" << cpuMs << '\n';
    std::cout << "GPU timing excludes initial image-bank upload. CPU is the numerical reference "
                 "implementation, not an optimized CPU matcher. Images are not the sensor's 108x88 frames.\n";
}

void invalidInputTests(vkmatch::Matcher &matcher) {
    const auto valid = makeImage(texture);
    matcher.setImages({valid});
    rejects([&] { matcher.run({{1, 0, 0, 0, 0, 1}}); }, "reference index");
    rejects([&] { matcher.run({{0, 1, 0, 0, 0, 1}}); }, "probe index");
    rejects([&] { matcher.run({{0, 0, 0, 0, 0, 1}}, 0); }, "zero stride");
    rejects([&] { matcher.run({{0, 0, 0, 0, 0, 0}}); }, "zero scale");
    rejects([&] { matcher.run({{0, 0, 0, 0, 0, -1}}); }, "negative scale");
    rejects([&] { matcher.run({{0, 0, std::numeric_limits<float>::quiet_NaN(), 0, 0, 1}}); }, "NaN angle");
    rejects([&] { matcher.run({{0, 0, 0, std::numeric_limits<float>::infinity(), 0, 1}}); }, "infinite translation");
    vkmatch::Job partialAnchor{0,0,0,0,0,1};partialAnchor.pivotX=4;
    rejects([&] { matcher.run({partialAnchor}); }, "partial transform anchor");
    rejects([&] { matcher.setImages({vkmatch::Image{}}); }, "zero dimensions");
    auto bad = valid;
    bad.pixels.pop_back();
    rejects([&] { matcher.setImages({bad}); }, "short pixel array");
    bad = valid;
    bad.width = 2;
    bad.pixels.resize(bad.width * bad.height);
    rejects([&] { matcher.setImages({bad}); }, "dimensions too small for gradients");
    bad = valid;
    bad.width -= 1;
    bad.pixels.resize(bad.width * bad.height);
    rejects([&] { matcher.setImages({valid, bad}); }, "mixed dimensions");
    bad = valid;
    bad.pixels[10] = std::numeric_limits<float>::quiet_NaN();
    rejects([&] { matcher.setImages({bad}); }, "non-finite pixel");
    bad.pixels[10] = 1.1f;
    rejects([&] { matcher.setImages({bad}); }, "pixel outside normalized range");
    matcher.setImages({valid});
    near(matcher.run({{0, 0, 0, 0, 0, 1}}, 2).scores.at(0).ncc, 1, 5e-4f,
         "valid dispatch after invalid inputs");
}
}

int main(int argc, char **argv) {
    try {
        require(argc == 2, "usage: vkmatch-test matcher.comp.spv");
        vkmatch::Matcher matcher(argv[1]);
        require(!matcher.deviceName().empty(), "device name");
        matchingTests(matcher);
        chunkBoundaryTests(matcher);
        invalidInputTests(matcher);
        std::cout << "PASS: synthetic warp, batch, input validation and CPU/GPU score parity on "
                  << matcher.deviceName() << '\n';
        std::cout << "These are numerical tests, not fingerprint recognition or security validation.\n";
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
