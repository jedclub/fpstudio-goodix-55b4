#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

// Placing a capture on the coverage map, on the GPU.
//
// Research tool. It answers "how well does this capture agree with the map at
// this pose", nothing else; no acceptance decision of any kind is exposed.
//
// The CPU search this replaces could afford translation only, every second
// offset, because it ran on the thread that reads preview frames. Adding
// rotation there was measured and abandoned: a +-25 degree search at 1.5 degree
// steps is about 300 million sample operations per capture, and a replay of one
// session did not finish in ten minutes. The poses are independent, so the work
// belongs somewhere wide rather than somewhere fast.
namespace vkmap {

struct Pose {
    float offsetX = 0, offsetY = 0;   // probe's top-left corner on the map
    float angleDegrees = 0;           // rotation about the probe's own centre
};
struct Result {
    float ncc = -2;      // correlation over the covered overlap, -2 if unusable
    float overlap = 0;   // covered samples / sampled probe pixels
};

class Placer {
public:
    explicit Placer(const std::string &spirvPath);
    ~Placer();
    Placer(const Placer &) = delete;
    Placer &operator=(const Placer &) = delete;

    std::string deviceName() const;
    // The map is uploaded once per change; weights are its validity mask, where
    // anything at or below zero means "nothing has been placed here yet".
    void setMap(const std::vector<float> &pixels, const std::vector<float> &weights, uint32_t size);
    void setProbe(const std::vector<float> &pixels, uint32_t width, uint32_t height);
    std::vector<Result> run(const std::vector<Pose> &poses, uint32_t stride = 2);
    double lastWallMs() const;
    double lastGpuMs() const;   // -1 when the device has no usable timestamps

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

// Same arithmetic on the CPU, for parity testing and as the fallback when no
// usable GPU is present.
Result cpuScore(const std::vector<float> &mapPixels, const std::vector<float> &mapWeights,
                uint32_t mapSize, const std::vector<float> &probe, uint32_t width,
                uint32_t height, const Pose &pose, uint32_t stride = 2);
}
