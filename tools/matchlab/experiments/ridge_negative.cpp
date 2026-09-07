// Synthetic artifact controls; no authentication acceptance rule is defined.
// Reuse the exact diagnostic routines, without editing geometry_match.cpp.
#define main geometry_command_main
#include "geometry_match.cpp"
#undef main
#include <filesystem>

static cv::Mat stripes(double period, double phase, double noise, int seed) {
    cv::Mat image(88,108,CV_8U); cv::RNG rng(seed);
    for(int y=0;y<image.rows;++y) for(int x=0;x<image.cols;++x) {
        const double edge=std::min({double(x-3),double(104-x),double(y-3),double(84-y)});
        const double envelope=std::clamp(edge/3.,0.,1.);
        const double wave=std::sin(2*CV_PI*(y+.16*x+1.8*std::sin(x/25.))/period+phase);
        const double gray=(125+110*std::tanh(2*wave))*envelope+rng.gaussian(noise);
        image.at<unsigned char>(y,x)=cv::saturate_cast<unsigned char>(gray);
    }
    return image;
}
static Features features(std::string name, cv::Mat image, int mode) {
    Features f;f.path=std::move(name);f.image=image;f.mode=mode;
    cv::Mat prepared=image.clone();
    if(mode) {
        cv::Mat pixels,mean,variance;
        image.convertTo(pixels,CV_32F);cv::GaussianBlur(pixels,mean,cv::Size(),5);
        cv::GaussianBlur(pixels.mul(pixels),variance,cv::Size(),5);
        variance-=mean.mul(mean);cv::max(variance,100,variance);cv::sqrt(variance,variance);
        pixels=(pixels-mean)/variance;pixels.convertTo(prepared,CV_8U,45,128);
    }
    cv::SIFT::create()->detectAndCompute(prepared,cv::noArray(),f.keys,f.descriptors);
    return f;
}
int main(int argc,char**argv) {
    if(argc!=2)return 2;
    const std::string dir=argv[1];if(!std::filesystem::is_directory(dir))return 3;
    cv::setNumThreads(1);
    std::vector<std::pair<std::string,cv::Mat>> controls;
    controls.emplace_back("period6_noise4_a",stripes(6,0,4,11));
    controls.emplace_back("period6_noise4_b",stripes(6,0,4,93));
    controls.emplace_back("period6_phase1",stripes(6,1,4,31));
    controls.emplace_back("period6p2_noise4",stripes(6.2,0,4,43));
    controls.emplace_back("period6p5_noise4",stripes(6.5,0,4,53));
    controls.emplace_back("period7_noise4",stripes(7,0,4,63));
    controls.emplace_back("period6_noise15",stripes(6,0,15,73));
    cv::Mat noise(88,108,CV_8U);cv::RNG rng(82);rng.fill(noise,cv::RNG::NORMAL,128,35);
    controls.emplace_back("independent_noise_a",noise.clone());rng.fill(noise,cv::RNG::NORMAL,128,35);
    controls.emplace_back("independent_noise_b",noise.clone());
    for(auto& [name,image]:controls)cv::imwrite(dir+"/"+name+".png",image);
    std::puts("mode,reference,probe,ratio,kp_ref,kp_probe,ratio_pass,mutual_unique,inliers,rmse,spread,scale,angle,overlap,ncc_highpass");
    for(int mode=0;mode<2;++mode) {
        std::vector<Features> f;for(auto& [name,image]:controls)f.push_back(features(name,image,mode));
        for(double ratio:{.75,.8,.85})for(size_t i=0;i<f.size();++i)for(size_t j=i+1;j<f.size();++j)compare(f[i],f[j],ratio);
    }
}
