// Offline diagnostics only. No authentication decisions or threshold tuning.
// g++ -O2 -std=c++17 geometry_match.cpp -o /private/path/geometry-match $(pkg-config --cflags --libs opencv5)
#include <opencv2/opencv.hpp>
#include <opencv2/geometry/3d.hpp>
#include <opencv2/geometry/2d.hpp>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

struct Features { std::string path; int mode = 0; cv::Mat image, descriptors; std::vector<cv::KeyPoint> keys; };
struct Fit { cv::Mat transform, inliers; int count = 0; double rmse = 0, spread = 0; };
static Fit fit(const std::vector<cv::Point2f> &a, const std::vector<cv::Point2f> &b, double area) {
    Fit f;
    if (a.size() < 3) return f;
    cv::setRNGSeed(42);
    f.transform = cv::estimateAffinePartial2D(a, b, f.inliers, cv::RANSAC, 2.5, 2000, .995, 10);
    if (f.transform.empty()) return f;
    std::vector<cv::Point2f> projected, in;
    cv::transform(a, projected, f.transform);
    for (size_t i=0; i<a.size(); ++i) if (f.inliers.at<unsigned char>(int(i))) {
        ++f.count; const auto d = projected[i] - b[i]; f.rmse += d.dot(d); in.push_back(a[i]);
    }
    f.rmse = f.count ? std::sqrt(f.rmse / f.count) : 0;
    if (in.size() >= 3) { std::vector<cv::Point2f> hull; cv::convexHull(in, hull); f.spread = cv::contourArea(hull) / area; }
    return f;
}
static double correlation(const Features &a, const Features &b, const Fit &fit, int &pixels) {
    pixels = 0;
    if (fit.transform.empty()) return 0;
    cv::Mat warped, mask, af, bf, blurA, blurB;
    cv::warpAffine(a.image, warped, fit.transform, b.image.size());
    cv::warpAffine(cv::Mat(a.image.size(), CV_8U, cv::Scalar(255)), mask, fit.transform, b.image.size(), cv::INTER_NEAREST);
    cv::erode(mask, mask, cv::Mat(), cv::Point(-1,-1), 3);
    warped.convertTo(af, CV_32F); b.image.convertTo(bf, CV_32F);
    cv::GaussianBlur(af, blurA, cv::Size(), 3); cv::GaussianBlur(bf, blurB, cv::Size(), 3);
    mask &= blurA > 15; mask &= blurB > 15;
    af -= blurA; bf -= blurB;
    pixels = cv::countNonZero(mask);
    if (pixels < 100) return 0;
    const double ma = cv::mean(af, mask)[0], mb = cv::mean(bf, mask)[0];
    double aa=0,bb=0,ab=0;
    for (int y=0;y<af.rows;++y) for(int x=0;x<af.cols;++x) if(mask.at<unsigned char>(y,x)) {
        const double av=af.at<float>(y,x)-ma, bv=bf.at<float>(y,x)-mb;
        aa+=av*av; bb+=bv*bv; ab+=av*bv;
    }
    return aa>0 && bb>0 ? ab/std::sqrt(aa*bb) : 0;
}
static void compare(const Features &a, const Features &b, double ratio) {
    std::vector<std::vector<cv::DMatch>> forward, reverse;
    std::vector<cv::Point2f> pa,pb;
    int ratioPass=0;
    if(a.descriptors.rows>=2 && b.descriptors.rows>=2) {
        cv::BFMatcher matcher(cv::NORM_L2);
        matcher.knnMatch(a.descriptors,b.descriptors,forward,2);
        matcher.knnMatch(b.descriptors,a.descriptors,reverse,2);
        for(const auto &row:forward) {
            if(row.size()<2 || row[0].distance >= ratio*row[1].distance) continue;
            ++ratioPass;
            const auto &m=row[0]; const auto &back=reverse[m.trainIdx];
            if(back.size()<2 || back[0].trainIdx!=m.queryIdx || back[0].distance>=ratio*back[1].distance)continue;
            const auto p=a.keys[m.queryIdx].pt, q=b.keys[m.trainIdx].pt;
            bool duplicate=false;
            for(size_t i=0;i<pa.size();++i) if(cv::norm(pa[i]-p)<1 || cv::norm(pb[i]-q)<1) {duplicate=true;break;}
            if(!duplicate) {pa.push_back(p);pb.push_back(q);}
        }
    }
    const auto f=fit(pa,pb,a.image.total()); int overlap=0;
    const double ncc=correlation(a,b,f,overlap);
    double scale=0, angle=0;
    if(!f.transform.empty()) {scale=std::hypot(f.transform.at<double>(0,0),f.transform.at<double>(1,0)); angle=std::atan2(f.transform.at<double>(1,0),f.transform.at<double>(0,0))*180/CV_PI;}
    std::printf("%d,%s,%s,%.2f,%zu,%zu,%d,%zu,%d,%.3f,%.4f,%.4f,%.2f,%d,%.4f\n",a.mode,a.path.c_str(),b.path.c_str(),ratio,a.keys.size(),b.keys.size(),ratioPass,pa.size(),f.count,f.rmse,f.spread,scale,angle,overlap,ncc);
}
static int selftest() {
    std::vector<cv::Point2f>a,b;
    const double theta=.6,c=std::cos(theta),s=std::sin(theta);
    for(int y=0;y<3;++y)for(int x=0;x<4;++x) {
        cv::Point2f p(x*12+5,y*15+5);a.push_back(p);b.emplace_back(c*p.x-s*p.y+8,s*p.x+c*p.y-4);
    }
    auto f=fit(a,b,108*88);
    if(f.count!=12 || f.rmse>.01) return 1;
    for(int i=0;i<12;++i) b[i] += cv::Point2f((i/4)*70, (i/4)*(i/4)*50);
    f=fit(a,b,108*88);
    if(f.count>5) return 2;
    std::puts("PASS: known rotation/translation; incompatible local groups rejected by global fit");return 0;
}
int main(int argc,char**argv) {
    if(argc==2 && std::string(argv[1])=="--selftest")return selftest();
    if(argc<3) {std::fprintf(stderr,"usage: geometry-match IMAGE IMAGE [IMAGE ...] | --selftest\n");return 2;}
    cv::setNumThreads(1);
    std::puts("mode,reference,probe,ratio,kp_ref,kp_probe,ratio_pass,mutual_unique,inliers,rmse,spread,scale,angle,overlap,ncc_highpass");
    for (int mode=0;mode<2;++mode) {
    std::vector<Features> images;
    for(int i=1;i<argc;++i) {
        Features f;f.path=argv[i];f.image=cv::imread(f.path,cv::IMREAD_GRAYSCALE);
        if(f.image.empty())return 3;
        f.path = f.path.substr(f.path.find_last_of('/') + 1); f.mode = mode;
        cv::Mat prepared=f.image.clone();
        if(mode==1) {
            cv::Mat pixels,mean,variance;
            f.image.convertTo(pixels,CV_32F);cv::GaussianBlur(pixels,mean,cv::Size(),5);
            cv::GaussianBlur(pixels.mul(pixels),variance,cv::Size(),5);
            variance-=mean.mul(mean);cv::max(variance,100,variance);cv::sqrt(variance,variance);
            pixels=(pixels-mean)/variance;pixels.convertTo(prepared,CV_8U,45,128);
        }
        cv::SIFT::create()->detectAndCompute(prepared,cv::noArray(),f.keys,f.descriptors);images.push_back(std::move(f));
    }
    for(double ratio:{.75,.8,.85}) for(size_t i=0;i<images.size();++i)for(size_t j=i+1;j<images.size();++j)compare(images[i],images[j],ratio);
    }
}
