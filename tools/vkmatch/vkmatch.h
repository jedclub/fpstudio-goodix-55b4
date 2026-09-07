#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace vkmatch {
// Research scores only. No biometric acceptance decision is exposed.
struct Image { uint32_t width = 0, height = 0; std::vector<float> pixels; };
// Warp reference coordinates about a per-job pivot into probe coordinates.
// The -1 defaults preserve image-centre rotation for generic callers. The
// fingerprint matcher supplies an explicit contact pivot and anchor for every
// reference/probe pair; neither coordinate is retained across frames.
struct Job {
    uint32_t reference = 0, probe = 0;
    float angleDegrees = 0, dx = 0, dy = 0, scale = 1;
    float pivotX = -1, pivotY = -1;
    float anchorX = -1, anchorY = -1;
};
struct TransformAnchor {
    float pivotX = 0, pivotY = 0, anchorX = 0, anchorY = 0;
};
inline bool usesImageCentreAnchor(const Job &job) {
    return job.pivotX == -1 && job.pivotY == -1 && job.anchorX == -1 && job.anchorY == -1;
}
inline TransformAnchor resolveTransformAnchor(const Job &job,uint32_t width,uint32_t height) {
    if(usesImageCentreAnchor(job)) {
        const float x=(width-1)*.5f,y=(height-1)*.5f;
        return {x,y,x,y};
    }
    return {job.pivotX,job.pivotY,job.anchorX,job.anchorY};
}
struct Score {
    float ncc = 0;        // signed normalized intensity correlation [-1,1]
    float gradient = 0;   // signed normalized gradient dot product [-1,1]
    float overlap = 0;    // valid samples / sampled reference pixels
    float mae = 0;        // mean absolute intensity difference [0,1]
};
struct Batch {
    std::vector<Score> scores;
    double wallMs = 0;
    double gpuMs = -1;    // -1 if timestamps unsupported
};
class Matcher {
public:
    explicit Matcher(const std::string &spirvPath);
    ~Matcher();
    Matcher(const Matcher &) = delete;
    Matcher &operator=(const Matcher &) = delete;
    std::string deviceName() const;
    // Upload once, dispatch many batches. All images share dimensions.
    void setImages(const std::vector<Image> &images);
    Batch run(const std::vector<Job> &jobs, uint32_t stride = 2);
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
Score cpuScore(const Image &reference, const Image &probe, const Job &, uint32_t stride = 2);
}
