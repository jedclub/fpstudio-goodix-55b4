// Offline-only preprocessing diagnostic; no biometric labels or acceptance claims.
// Build: g++ -O2 -std=c++17 ridge_preprocessing.cpp $(pkg-config --cflags --libs opencv5) -o <private-output>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

struct Features { std::vector<cv::KeyPoint> points; cv::Mat descriptors; };
cv::Mat prepare(const cv::Mat& input, int mode) {
    cv::Mat image = input.clone();
    if (mode == 1) image = input(cv::Rect(7,7,input.cols-14,input.rows-14)).clone();
    if (mode == 2) {
        cv::Mat f, mean, variance, squared;
        input.convertTo(f,CV_32F); cv::GaussianBlur(f,mean,cv::Size(),5.0);
        squared=f.mul(f); cv::GaussianBlur(squared,variance,cv::Size(),5.0);
        variance-=mean.mul(mean); cv::max(variance,100,variance); cv::sqrt(variance,variance);
        f=(f-mean)/variance; f.convertTo(image,CV_8U,45,128);
    }
    if (mode == 3) cv::createCLAHE(2.0,cv::Size(4,4))->apply(input,image);
    return image;
}
Features extract(const cv::Mat& image, bool root=false) {
    Features f; cv::SIFT::create()->detectAndCompute(image,cv::noArray(),f.points,f.descriptors);
    if(root) for(int r=0;r<f.descriptors.rows;++r) {
        auto row=f.descriptors.row(r); row/=cv::sum(row)[0]+1e-8; cv::sqrt(row,row);
    }
    return f;
}
int matches(const Features& a,const Features& b) {
    if(a.descriptors.empty() || b.descriptors.rows<2)return 0;
    std::vector<std::vector<cv::DMatch>> nearest; cv::BFMatcher matcher;
    matcher.knnMatch(a.descriptors,b.descriptors,nearest,2);
    int n=0; for(auto& pair:nearest) if(pair.size()==2 && pair[0].distance<.75*pair[1].distance) ++n;
    return n;
}
int main(int argc,char** argv) {
    std::vector<cv::Mat> images;
    for(int i=1;i<argc;++i) { auto img=cv::imread(argv[i],cv::IMREAD_GRAYSCALE);if(img.empty())return 2;images.push_back(img); }
    const char* modes[]={"raw","crop7","local-normalize","clahe4"};
    std::puts("kind,mode,root,i,j,kp_i,kp_j,matches,inverse_matches");
    for(int mode=0;mode<4;++mode)for(bool root:{false,true}) {
        std::vector<Features> f, inverse;
        for(auto& image:images){auto p=prepare(image,mode);f.push_back(extract(p,root)); inverse.push_back(extract(255-p,root));}
        for(size_t i=0;i<f.size();++i)for(size_t j=0;j<f.size();++j)
            std::printf("%s,%s,%d,%zu,%zu,%zu,%zu,%d,%d\n",i==j?"polarity-control":"unlabeled-pair",modes[mode],root,i+1,j+1,f[i].points.size(),f[j].points.size(),matches(f[i],f[j]),matches(f[i],inverse[j]));
    }
    // Edge concentration and ridge direction coherence are diagnostics, NOT identity evidence.
    std::fprintf(stderr,"image,keypoints,near_border,gradient_coherence\n");
    for(size_t i=0;i<images.size();++i) {
        auto f=extract(images[i]);int border=0;
        for(auto& p:f.points)if(p.pt.x<8 || p.pt.y<8 || p.pt.x>images[i].cols-9 || p.pt.y>images[i].rows-9)++border;
        cv::Mat dx,dy;cv::Sobel(images[i],dx,CV_32F,1,0);cv::Sobel(images[i],dy,CV_32F,0,1);
        double xx=cv::sum(dx.mul(dx))[0], yy=cv::sum(dy.mul(dy))[0],xy=cv::sum(dx.mul(dy))[0];
        std::fprintf(stderr,"%zu,%zu,%d,%.4f\n",i+1,f.points.size(),border,std::hypot(xx-yy,2*xy)/(xx+yy+1e-9));
    }
}
