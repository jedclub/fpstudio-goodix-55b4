// Compare matchlab against the reference source linked by build.sh, not the
// installed shared library. --print also supports the patched reference.
#include "upstream/sigfm.hpp"
#include "upstream/img-info.hpp"
#include <opencv2/opencv.hpp>
#include <cstdio>
#include <vector>
#include <string>

int matchlab_baseline_score(const cv::Mat &a, const cv::Mat &b); // from matchlab.cpp
int matchlab_print_score(const SigfmImgInfo &, const SigfmImgInfo &, bool);
int matchlab_circular_score(const SigfmImgInfo &, const SigfmImgInfo &);
bool load_print_stages(const std::string &, std::vector<SigfmImgInfo> *, std::string *);

int main(int argc, char **argv)
{
    if (argc == 2 && std::string(argv[1]) == "--geometry") {
        // Exact descriptor correspondences isolate geometry from image quality
        // and SIFT extraction. These are synthetic points, not fingerprints.
        SigfmImgInfo a;
        a.descriptors = cv::Mat::zeros(8, 128, CV_32F);
        for (int i = 0; i < 8; ++i) {
            a.keypoints.emplace_back(float((i * 11) % 71), float((i * 17) % 59), 1.f);
            a.descriptors.at<float>(i, i) = 100.f;
        }
        for (int degrees : {0, 5, 15, 45, 89, 90, 91, 135, 180}) {
            SigfmImgInfo b = a;
            const double angle = degrees * CV_PI / 180.;
            for (auto &kp : b.keypoints) {
                const auto p = kp.pt;
                kp.pt = cv::Point2f(100 + std::cos(angle) * p.x - std::sin(angle) * p.y,
                                    100 + std::sin(angle) * p.x + std::cos(angle) * p.y);
            }
            std::printf("exact correspondences, rotation %3d deg: reference %d, circular experiment %d\n",
                        degrees, sigfm_match_score(&a, &b), matchlab_circular_score(a, b));
        }
        // Three groups with different translations. There is no common
        // transform supported by five correspondences, but pairwise angle
        // votes can pool evidence from all three groups.
        SigfmImgInfo fragmented;
        fragmented.descriptors = cv::Mat::zeros(12, 128, CV_32F);
        for (int i = 0; i < 12; ++i) {
            fragmented.keypoints.emplace_back(float((i % 4) * 10), float(i * 10), 1.f);
            fragmented.descriptors.at<float>(i, i) = 100.f;
        }
        SigfmImgInfo moved = fragmented;
        for (int i = 0; i < 12; ++i)
            moved.keypoints[i].pt += cv::Point2f(float(1000 * (i / 4)), 0.f);
        std::printf("three incompatible translations (4 correspondences each): reference %d, circular experiment %d\n",
                    sigfm_match_score(&fragmented, &moved), matchlab_circular_score(fragmented, moved));
        return 0;
    }
    if (argc == 3 && std::string(argv[1]) == "--print") {
        std::vector<SigfmImgInfo> stages;
        std::string error;
        if (!load_print_stages(argv[2], &stages, &error) || stages.size() < 2) {
            std::fprintf(stderr, "cannot test print: %s\n", error.c_str());
            return 1;
        }
        int bad = 0, pairs = 0;
        for (std::size_t i = 0; i < stages.size(); ++i)
            for (std::size_t j = 0; j < stages.size(); ++j) {
                if (i == j) continue;
#ifdef MATCHLAB_TEST_FIXED
                constexpr bool fixed = true;
#else
                constexpr bool fixed = false;
#endif
                const int reference = sigfm_match_score(&stages[i], &stages[j]);
                const int actual = matchlab_print_score(stages[i], stages[j], fixed);
                ++pairs;
                if (reference != actual) {
                    std::printf("MISMATCH %zu->%zu: reference %d, matchlab %d\n",
                                i, j, reference, actual);
                    ++bad;
                }
            }
        std::printf("print selftest: %d pairs, %d mismatches\n", pairs, bad);
        return bad ? 1 : 0;
    }
#ifdef MATCHLAB_TEST_FIXED
    std::fputs("usage: selftest-fixed --print FILE.fp\n", stderr);
    return 2;
#endif
    std::vector<cv::Mat> imgs;
    for (int i = 1; i < argc; i++) {
        cv::Mat m = cv::imread(argv[i], cv::IMREAD_GRAYSCALE);
        if (m.empty()) { std::fprintf(stderr, "cannot read %s\n", argv[i]); return 1; }
        if (!m.isContinuous()) m = m.clone();
        imgs.push_back(m);
    }
    if (imgs.size() < 2) { std::fprintf(stderr, "need >= 2 images\n"); return 2; }

    std::vector<SigfmImgInfo *> infos;
    for (const auto &m : imgs)
        infos.push_back(sigfm_extract(m.data, m.cols, m.rows));

    int bad = 0;
    for (std::size_t i = 0; i < imgs.size(); i++)
        for (std::size_t j = 0; j < imgs.size(); j++) {
            if (i == j) continue;
            const int up = sigfm_match_score(infos[i], infos[j]);
            const int mine = matchlab_baseline_score(imgs[i], imgs[j]);
            if (up != mine) {
                std::printf("MISMATCH %zu->%zu: upstream %d, matchlab %d\n", i, j, up, mine);
                bad++;
            }
        }
    for (auto *p : infos) sigfm_free_info(p);
    std::printf(bad ? "SELFTEST FAILED (%d mismatches)\n" : "selftest ok: %d pairs identical\n",
                bad ? bad : int(imgs.size() * (imgs.size() - 1)));
    return bad ? 1 : 0;
}
