// matchlab - offline A/B bench for the SIGFM matcher on real captures.
//
// Why this exists: the driver's match score is not "how many keypoints
// agree". Reading libfprint/sigfm/sigfm.cpp, the returned number is the count
// of *pairs of pairs*: correspondences are paired into `angles` when their
// two connecting vectors have the same length, and the score counts pairs of
// those that also agree in orientation. So the score grows roughly with the
// fourth power of the number of good correspondences, which is why the
// measured passes span 77-2920 while every failure sits at 0 or 7. Any change
// has to be judged on real captures, at the real threshold, against real
// impostors - a proxy metric would be meaningless at that exponent.
//
// The baseline variant is a line-by-line reimplementation of sigfm.cpp and is
// checked by the separate selftest executable against the bundled reference
// source. selftest-fixed checks the fixed variant against supplied source.

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include "upstream/img-info.hpp"

#include <chrono>
#include <map>
#include <set>
#include <string>
#include <vector>

// ---------------------------------------------------------------- knobs ----

struct Variant {
    std::string name;

    // matcher
    bool   dedup_fix   = false; // fix the std::set comparator (see note below)
    double ratio       = 0.75;  // Lowe ratio test
    int    min_match   = 5;
    double length_tol  = 0.05;
    double angle_tol   = 0.05;
    int    max_matches = 0;     // 0 = no cap; keeps the best N by Lowe ratio
    bool   circular_angle = false; // experimental radians, never the baseline

    // preprocessing, applied before SIFT
    int    upscale     = 1;     // linear factor, cubic interpolation
    double unsharp     = 0.0;   // amount; 0 disables
    double clahe_clip  = 0.0;   // 0 disables

    // SIFT
    int    octave_layers = 3;
    double contrast_thr  = 0.04;
    double edge_thr      = 10.0;
    double sigma         = 1.6;
};

// ------------------------------------------------------------ extraction ----

struct Info {
    std::vector<cv::KeyPoint> kp;
    cv::Mat desc;
};

static cv::Mat preprocess(const cv::Mat &src, const Variant &v)
{
    cv::Mat img = src;

    if (v.clahe_clip > 0.0) {
        cv::Mat out;
        cv::createCLAHE(v.clahe_clip, cv::Size(8, 8))->apply(img, out);
        img = out;
    }
    if (v.unsharp > 0.0) {
        cv::Mat blur, out;
        cv::GaussianBlur(img, blur, cv::Size(0, 0), 1.5);
        cv::addWeighted(img, 1.0 + v.unsharp, blur, -v.unsharp, 0, out);
        img = out;
    }
    if (v.upscale > 1) {
        cv::Mat out;
        cv::resize(img, out, cv::Size(), v.upscale, v.upscale, cv::INTER_CUBIC);
        img = out;
    }
    return img;
}

static Info extract(const cv::Mat &raw, const Variant &v)
{
    const cv::Mat img = preprocess(raw, v);
    const cv::Mat roi = cv::Mat::ones(img.size(), CV_8UC1);
    Info info;
    cv::SIFT::create(0, v.octave_layers, v.contrast_thr, v.edge_thr, v.sigma)
        ->detectAndCompute(img, roi, info.kp, info.desc);
    return info;
}

// ---------------------------------------------------------------- score ----

namespace {

struct Match {
    cv::Point2i p1, p2;
    float strength = 0.f; // Lowe distance ratio; lower is a better match
};

// Upstream's comparator is
//     return (p1.y < r.p1.y) || ((p1.y < r.p1.y) && p1.x < r.p1.x);
// where the second clause repeats the first instead of testing equality, so
// the whole thing reduces to "order by p1.y". That is a valid strict weak
// ordering, so std::set accepts it - and then treats every correspondence
// sharing a row of the query image as a duplicate and keeps only the first.
// On an 88-row image that silently discards genuine correspondences, and at a
// fourth-power score it discards them where it hurts most. `dedup_fix` uses
// the ordering the struct's own operator== implies.
struct LessBuggy {
    bool operator()(const Match &a, const Match &b) const
    {
        return (a.p1.y < b.p1.y) || ((a.p1.y < b.p1.y) && a.p1.x < b.p1.x);
    }
};
struct LessFixed {
    bool operator()(const Match &a, const Match &b) const
    {
        return std::tie(a.p1.y, a.p1.x, a.p2.y, a.p2.x) <
               std::tie(b.p1.y, b.p1.x, b.p2.y, b.p2.x);
    }
};

struct Angle {
    double cos, sin;
};

} // namespace

struct ScoreTrace {
    int ratio = 0, unique = 0, edges = 0, nonfinite = 0;
};

static int score(const Info &frame, const Info &enrolled, const Variant &v,
                 ScoreTrace *trace = nullptr)
{
    if (frame.desc.empty() || enrolled.desc.empty())
        return 0;

    std::vector<std::vector<cv::DMatch>> pts;
    cv::BFMatcher::create()->knnMatch(frame.desc, enrolled.desc, pts, 2);

    std::vector<Match> matches;
    {
        std::set<Match, LessBuggy> uniq_buggy;
        std::set<Match, LessFixed> uniq_fixed;
        int nb_matched = 0;
        for (const auto &p : pts) {
            if (p.size() < 2)
                continue;
            const cv::DMatch &m = p.at(0);
            if (m.distance < v.ratio * p.at(1).distance) {
                const Match mm{frame.kp.at(m.queryIdx).pt,
                               enrolled.kp.at(m.trainIdx).pt,
                               p.at(1).distance > 0.f
                                   ? m.distance / p.at(1).distance : 0.f};
                if (v.dedup_fix) uniq_fixed.insert(mm);
                else             uniq_buggy.insert(mm);
                nb_matched++;
            }
        }
        if (trace) trace->ratio = nb_matched;
        if (trace) trace->unique = v.dedup_fix ? uniq_fixed.size() : uniq_buggy.size();
        if (nb_matched < v.min_match)
            return 0;
        if (v.dedup_fix) matches.assign(uniq_fixed.begin(), uniq_fixed.end());
        else             matches.assign(uniq_buggy.begin(), uniq_buggy.end());
    }

    // Both loops below are quadratic in what they are given, so the score is
    // quartic in the number of correspondences. Upstream is bounded only by
    // accident - its comparator keeps at most one match per image row. Once
    // that is fixed the count has to be bounded on purpose, and keeping the
    // strongest correspondences is a better bound than keeping whichever one
    // happened to land on a row first.
    if (v.max_matches > 0 && (int)matches.size() > v.max_matches) {
        std::nth_element(matches.begin(), matches.begin() + v.max_matches,
                         matches.end(), [](const Match &a, const Match &b) {
                             return a.strength < b.strength;
                         });
        matches.resize(v.max_matches);
    }

    std::vector<Angle> angles;
    for (std::size_t j = 0; j < matches.size(); j++) {
        for (std::size_t k = j + 1; k < matches.size(); k++) {
            const double v1[2] = {double(matches[j].p1.x - matches[k].p1.x),
                                  double(matches[j].p1.y - matches[k].p1.y)};
            const double v2[2] = {double(matches[j].p2.x - matches[k].p2.x),
                                  double(matches[j].p2.y - matches[k].p2.y)};
            const double l1 = std::sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
            const double l2 = std::sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
            if (v.circular_angle && (l1 == 0 || l2 == 0)) continue;
            if (1 - std::min(l1, l2) / std::max(l1, l2) <= v.length_tol) {
                const double prod = l1 * l2;
                if (v.circular_angle) {
                    angles.push_back({std::atan2(v1[0] * v2[1] - v1[1] * v2[0],
                                                v1[0] * v2[0] + v1[1] * v2[1]), 0});
                    continue;
                }
                angles.push_back({M_PI / 2 + std::asin((v1[0] * v2[0] + v1[1] * v2[1]) / prod),
                                  std::acos((v1[0] * v2[1] - v1[1] * v2[0]) / prod)});
            }
        }
    }
    if (trace) {
        trace->edges = angles.size();
        for (const auto &a : angles)
            if (!std::isfinite(a.cos) || !std::isfinite(a.sin)) ++trace->nonfinite;
    }
    if ((int)angles.size() < v.min_match)
        return 0;

    int count = 0;
    for (std::size_t j = 0; j < angles.size(); j++) {
        for (std::size_t k = j + 1; k < angles.size(); k++) {
            if (v.circular_angle) {
                if (std::abs(std::remainder(angles[j].cos - angles[k].cos, 2 * M_PI))
                    <= 5 * M_PI / 180.) ++count;
                continue;
            }
            if (1 - std::min(angles[j].sin, angles[k].sin) /
                        std::max(angles[j].sin, angles[k].sin) <= v.angle_tol &&
                1 - std::min(angles[j].cos, angles[k].cos) /
                        std::max(angles[j].cos, angles[k].cos) <= v.angle_tol)
                count++;
        }
    }
    return count;
}

// ----------------------------------------------------------------- main ----

// The driver's own gate, so a variant cannot look good by counting scores
// against frames the enrol path would have thrown away.
static const int THRESHOLD = 72;

struct Sample {
    std::string path, label;
    cv::Mat img;                 // empty when the sample came from a print
    bool from_print = false;
    bool reference = false;
    Info fixed;                  // set when from_print: extraction already done
};

bool load_print_stages(const std::string &path, std::vector<SigfmImgInfo> *out,
                       std::string *error);

[[maybe_unused]] static void usage()
{
    std::fprintf(stderr,
        "usage: matchlab <label>=<file> [<label>=<file> ...]\n"
        "       matchlab --audit-print FILE.fp\n"
        "       matchlab --heldout ref:index=FILE probe:index=FILE probe:middle=FILE ...\n"
        "  <file> is a .png capture, or a .fp enrolled print - a print\n"
        "  expands into one sample per enrol stage.\n"
        "  Same label = same finger (genuine pairs); different = impostor.\n"
        "\n"
        "  Prints carry keypoints and descriptors, not pixels, so variants\n"
        "  that change preprocessing or SIFT can only be measured on .png\n"
        "  captures. Matcher-side variants work on both.\n");
}

#ifndef MATCHLAB_NO_MAIN
int main(int argc, char **argv)
{
    if (argc == 3 && std::string(argv[1]) == "--audit-print") {
        std::vector<SigfmImgInfo> stages;
        std::string error;
        if (!load_print_stages(argv[2], &stages, &error) || stages.size() < 2) {
            std::fprintf(stderr, "cannot audit: %s\n", error.c_str());
            return 1;
        }
        for (bool fixed : {false, true}) {
            Variant v;
            v.dedup_fix = fixed; v.max_matches = fixed ? 64 : 0;
            int total = 0, early = 0, length = 0, low = 0, passed = 0, nan = 0;
            int maxRatio = 0, maxUnique = 0;
            for (std::size_t i = 0; i < stages.size(); ++i)
                for (std::size_t j = 0; j < stages.size(); ++j) {
                    if (i == j) continue;
                    ScoreTrace trace;
                    int result = score(Info{stages[i].keypoints, stages[i].descriptors},
                                       Info{stages[j].keypoints, stages[j].descriptors}, v, &trace);
                    ++total;
                    maxRatio = std::max(maxRatio, trace.ratio);
                    maxUnique = std::max(maxUnique, trace.unique);
                    if (trace.nonfinite) ++nan;
                    if (trace.ratio < 5) ++early;
                    else if (trace.edges < 5) ++length;
                    else if (result < THRESHOLD) ++low;
                    else ++passed;
                }
            std::printf("%s: pairs=%d descriptor_gate=%d length_gate=%d low_score=%d pass=%d nonfinite_pairs=%d max_ratio_matches=%d max_unique=%d\n",
                        fixed ? "fixed-cap64" : "upstream", total, early, length, low,
                        passed, nan, maxRatio, maxUnique);
        }
        return 0;
    }
    if (argc < 2) { usage(); return 2; }

    const bool heldout = std::string(argv[1]) == "--heldout";
    std::vector<Sample> samples;
    for (int i = heldout ? 2 : 1; i < argc; i++) {
        const std::string a = argv[i];
        const auto eq = a.find('=');
        if (eq == std::string::npos) { usage(); return 2; }
        std::string label = a.substr(0, eq);
        const bool reference = heldout && label.rfind("ref:", 0) == 0;
        if (heldout) {
            if (reference) label.erase(0, 4);
            else if (label.rfind("probe:", 0) == 0) label.erase(0, 6);
            else { usage(); return 2; }
        }
        const std::string path  = a.substr(eq + 1);

        if (path.size() > 3 && path.compare(path.size() - 3, 3, ".fp") == 0) {
            std::vector<SigfmImgInfo> stages;
            std::string err;
            if (!load_print_stages(path, &stages, &err)) {
                std::fprintf(stderr, "%s: %s\n", path.c_str(), err.c_str());
                return 1;
            }
            for (std::size_t k = 0; k < stages.size(); k++) {
                Sample s;
                s.label = label;
                s.path  = path + ":" + std::to_string(k);
                s.from_print = true;
                s.reference = reference;
                s.fixed.kp   = stages[k].keypoints;
                s.fixed.desc = stages[k].descriptors;
                samples.push_back(std::move(s));
            }
            continue;
        }

        Sample s;
        s.label = label;
        s.path  = path;
        s.reference = reference;
        s.img   = cv::imread(path, cv::IMREAD_GRAYSCALE);
        if (s.img.empty()) {
            std::fprintf(stderr, "cannot read %s\n", path.c_str());
            return 1;
        }
        samples.push_back(std::move(s));
    }

    std::vector<Variant> variants;
    {
        Variant base;       base.name = "upstream (before fixes)";
        variants.push_back(base);

        Variant d = base;   d.name = "dedup-fix, cap 64";
        d.dedup_fix = true; d.max_matches = 64;
        variants.push_back(d);

        Variant d2 = d;     d2.name = "dedup-fix, cap 96";  d2.max_matches = 96;
        variants.push_back(d2);

        Variant angle = d; angle.name = "EXPERIMENT: angle delta 5deg";
        angle.circular_angle = true;
        variants.push_back(angle);

        Variant r = d;      r.name = "  + ratio .80";       r.ratio = 0.80;
        variants.push_back(r);

        Variant c = d;      c.name = "  + contrast .03";    c.contrast_thr = 0.03;
        variants.push_back(c);

        Variant u = d;      u.name = "  + upscale 2x";      u.upscale = 2;
        variants.push_back(u);

        Variant sh = d;     sh.name = "  + unsharp .8";     sh.unsharp = 0.8;
        variants.push_back(sh);

        Variant cl = d;     cl.name = "  + clahe 2.0";      cl.clahe_clip = 2.0;
        variants.push_back(cl);

        Variant a = d;      a.name = "  + upscale 2x + unsharp .8";
        a.upscale = 2;      a.unsharp = 0.8;
        variants.push_back(a);
    }

    // A print stores the keypoints and descriptors the driver already
    // extracted, so anything upstream of extraction cannot be re-run on it.
    // Silently reporting those variants would report the matcher-side change
    // twice under two names.
    const bool have_prints =
        std::any_of(samples.begin(), samples.end(),
                    [](const Sample &s) { return s.from_print; });
    if (have_prints) {
        const std::size_t before = variants.size();
        variants.erase(std::remove_if(variants.begin(), variants.end(),
                                      [](const Variant &v) {
                                          return v.upscale > 1 || v.unsharp > 0 ||
                                                 v.clahe_clip > 0 ||
                                                 v.contrast_thr != 0.04 ||
                                                 v.octave_layers != 3;
                                      }),
                       variants.end());
        std::printf("note: input includes enrolled prints, which carry "
                    "descriptors rather than pixels -\n      skipping %zu "
                    "variant(s) that change extraction.\n\n",
                    before - variants.size());
    }

    if (heldout) {
        const auto references = std::count_if(samples.begin(), samples.end(), [](const Sample &s) { return s.reference; });
        if (!references || references == (int)samples.size()) {
            std::fputs("held-out evaluation needs both reference and probe samples\n", stderr);
            return 2;
        }
        std::printf("Held-out evaluation: %zu references, %zu probes; threshold %d\n",
                    (std::size_t)references, samples.size() - references, THRESHOLD);
        std::puts("No probe is ever used as a reference. Small-sample rates are not population accuracy.");
        std::printf("%-32s %14s %14s %10s\n", "variant", "genuine pass", "other accepted", "other max");
        for (const auto &v : variants) {
            std::vector<Info> infos;
            for (const auto &s : samples) infos.push_back(s.from_print ? s.fixed : extract(s.img, v));
            int genuine = 0, passed = 0, other = 0, falseAccept = 0, maxOther = -1;
            for (std::size_t i = 0; i < samples.size(); ++i) {
                if (samples[i].reference) continue;
                const bool known = std::any_of(samples.begin(), samples.end(), [&](const Sample &s) {
                    return s.reference && s.label == samples[i].label;
                });
                int best = 0;
                for (std::size_t j = 0; j < samples.size(); ++j) {
                    if (!samples[j].reference || (known && samples[j].label != samples[i].label)) continue;
                    best = std::max(best, score(infos[j], infos[i], v));
                }
                if (known) { ++genuine; if (best >= THRESHOLD) ++passed; }
                else { ++other; if (best >= THRESHOLD) ++falseAccept; maxOther = std::max(maxOther, best); }
            }
            std::printf("%-32s %6d/%-7d %6d/%-7d %10d\n", v.name.c_str(), passed, genuine, falseAccept, other, maxOther);
        }
        return 0;
    }

    int n_gen = 0, n_imp = 0;
    for (std::size_t i = 0; i < samples.size(); i++)
        for (std::size_t j = 0; j < samples.size(); j++)
            if (i != j) (samples[i].label == samples[j].label ? n_gen : n_imp)++;

    std::printf("%d samples: %d genuine pairs, %d impostor pairs\n",
                (int)samples.size(), n_gen, n_imp);
    std::puts("Leave-one-out internal consistency only; not held-out authentication accuracy.");
    std::printf("shipping threshold %d\n\n", THRESHOLD);

    // Two accept rates per variant, because a change that multiplies every
    // score would look like an improvement against a fixed threshold while
    // separating nothing. TPR@72 is what the driver would do today, unchanged.
    // TPR@0FP is what it could do if the threshold were retuned to the point
    // where no impostor pair passes - that is the one that measures whether a
    // variant actually separates the two distributions better.
    std::printf("%-32s %5s %7s %7s %8s %8s %7s\n",
                "variant", "kp", "pairmed", "imp.max", "accept", "@0FP", "ms/cmp");
    std::printf("%s\n", std::string(80, '-').c_str());

    for (const Variant &v : variants) {
        std::vector<Info> infos;
        double kp_total = 0;
        for (const Sample &s : samples) {
            infos.push_back(s.from_print ? s.fixed : extract(s.img, v));
            kp_total += infos.back().kp.size();
        }

        // The driver accepts if ANY enrolled stage clears the threshold
        // (fpi_print_sigfm_match returns on the first one), so the unit that
        // matters is a probe against a whole enrolment, not a pair. Scored
        // leave-one-out: each sample plays the probe once, against every
        // other sample of its own label.
        std::vector<int> gen_best, imp_best, all_pairs;
        const auto t0 = std::chrono::steady_clock::now();
        std::size_t n_cmp = 0;

        for (std::size_t i = 0; i < samples.size(); i++) {
            int best_same = 0, best_other = 0;
            for (std::size_t j = 0; j < samples.size(); j++) {
                if (i == j) continue;
                // fpi_print_sigfm_match calls score(enrolled_stage, probe).
                // Lowe's directed nearest-neighbour test is not symmetric.
                const int sc = score(infos[j], infos[i], v);
                n_cmp++;
                all_pairs.push_back(sc);
                if (samples[i].label == samples[j].label)
                    best_same = std::max(best_same, sc);
                else
                    best_other = std::max(best_other, sc);
            }
            gen_best.push_back(best_same);
            if (std::any_of(samples.begin(), samples.end(),
                            [&](const Sample &s) { return s.label != samples[i].label; }))
                imp_best.push_back(best_other);
        }
        const double ms = std::chrono::duration<double, std::milli>(
                              std::chrono::steady_clock::now() - t0).count() /
                          std::max<std::size_t>(1, n_cmp);

        std::sort(all_pairs.begin(), all_pairs.end());
        const int pair_med = all_pairs.empty() ? 0 : all_pairs[all_pairs.size() / 2];

        std::vector<int> imp_sorted = imp_best;
        std::sort(imp_sorted.begin(), imp_sorted.end());
        const int imp_max = imp_sorted.empty() ? -1 : imp_sorted.back();

        const int acc72 = std::count_if(gen_best.begin(), gen_best.end(),
                                        [](int s) { return s >= THRESHOLD; });
        const int acc0fp = imp_sorted.empty() ? -1
            : (int) std::count_if(gen_best.begin(), gen_best.end(),
                                  [&](int s) { return s > imp_max; });

        char tpr0[16];
        if (acc0fp < 0) std::snprintf(tpr0, sizeof tpr0, "%7s", "n/a");
        else std::snprintf(tpr0, sizeof tpr0, "%6.1f%%",
                           100.0 * acc0fp / gen_best.size());
        char impcol[16];
        if (imp_max < 0) std::snprintf(impcol, sizeof impcol, "%7s", "n/a");
        else std::snprintf(impcol, sizeof impcol, "%7d", imp_max);

        std::printf("%-32s %5.0f %7d %s %7.1f%% %s %7.2f\n",
                    v.name.c_str(), kp_total / samples.size(), pair_med, impcol,
                    100.0 * acc72 / gen_best.size(), tpr0, ms);
        std::fflush(stdout);
    }

    // Per-sample view, on the shipping matcher. An enrolled stage that no
    // other stage matches is either a placement nothing else overlaps, or a
    // frame that carries no usable structure at all - and the second kind is
    // a wasted slot that can never contribute to an unlock.
    {
        const Variant v = variants.front();
        std::vector<Info> infos;
        for (const Sample &s : samples)
            infos.push_back(s.from_print ? s.fixed : extract(s.img, v));

        std::printf("\nper-sample, %s:\n", v.name.c_str());
        std::printf("  %-24s %5s %7s %7s\n", "sample", "kp", "best", "matches");
        for (std::size_t i = 0; i < samples.size(); i++) {
            int best = 0, n = 0;
            for (std::size_t j = 0; j < samples.size(); j++) {
                if (i == j || samples[i].label != samples[j].label) continue;
                const int sc = score(infos[j], infos[i], v);
                best = std::max(best, sc);
                if (sc >= THRESHOLD) n++;
            }
            const char *tag = best >= THRESHOLD ? "" : "   <- matches nothing";
            std::printf("  %-24s %5zu %7d %7d%s\n",
                        samples[i].path.substr(samples[i].path.find_last_of('/') + 1).c_str(),
                        infos[i].kp.size(), best, n, tag);
        }
    }
    return 0;
}
#endif

// Exposed for selftest.cpp, which compares this against the shipping matcher.
int matchlab_baseline_score(const cv::Mat &a, const cv::Mat &b)
{
    Variant v; // defaults == upstream
    return score(extract(a, v), extract(b, v), v);
}

int matchlab_print_score(const SigfmImgInfo &a, const SigfmImgInfo &b, bool fixed)
{
    Variant v;
    v.dedup_fix = fixed;
    v.max_matches = fixed ? 64 : 0;
    return score(Info{a.keypoints, a.descriptors}, Info{b.keypoints, b.descriptors}, v);
}

int matchlab_circular_score(const SigfmImgInfo &a, const SigfmImgInfo &b)
{
    Variant v;
    v.dedup_fix = true; v.max_matches = 64; v.circular_angle = true;
    return score(Info{a.keypoints, a.descriptors}, Info{b.keypoints, b.descriptors}, v);
}
