// Does a fused map identify the finger as well as the captures it was made of?
//
// The honest form of the question needs probes the enrolment never saw, so the
// captures are split: the earlier ones build the map and act as the gallery,
// the later ones are the probes. Both sides are then asked the same question
// with the same search, so the only thing that differs is what is being matched
// against.
//
// Both halves of the question are asked: how readily the owner is accepted, and
// how readily a different finger is. Acceptance alone decides nothing - an
// enrolment that accepted everything would win that comparison outright - so
// the number that matters is the separation between the two.
//
// The impostor set is this one person's other fingers. It bounds nothing about
// false accepts across different people, and is not evidence that either
// enrolment is safe to authenticate with.
#include "mapplace.h"
#include <QImage>
#include <QDir>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>
#include <cstdlib>
constexpr uint32_t kMap=320,W=108,H=88;
// Keeps 90% of a session, where the coherence gate kept 62% and threw away the core.
static double kQualityFloor=0.357;
static int kGalleryCap=0;
// 0 = feathered blend (what the window shows), 1 = best sample per pixel
static int kFusion=0;
// Cap on accumulated weight per pixel. Low keeps the map answerable to the most
// recent contacts; high lets every contact that ever landed there keep a say.
static float kWeightCap=3.0f;

// How sharp is this capture?
//
// The gate this replaces used structure-tensor coherence, which measures how
// locally parallel the ridges are - and that is not sharpness. Blurring a
// capture *raises* coherence, because smoothing removes the isotropic noise
// that was diluting the dominant orientation: measured over 88 captures,
// coherence went from 0.877 to 0.890 under a 1.5px blur. It also correlates
// -0.935 with how much the ridge orientation varies, so it scored the whorl
// core - where the core, the delta and most of the identifying detail live -
// as the worst part of the finger. A third of every session was being thrown
// out of the map for being interesting.
//
// This measures what blurring actually destroys: the harmonics that make a
// ridge edge an edge, relative to the ridge fundamental itself. Both bands come
// from the same image, so the ratio does not care about contrast, and neither
// band cares which way the ridges run. Under the same 1.5px blur it separates
// at 1.9 standard deviations, and its correlation with orientation variation is
// positive rather than negative - it does not punish a core.
static void blur(const std::vector<float>&in,std::vector<float>&out,int w,int h,double sigma)
{
    const int r=std::max(1,int(std::ceil(3*sigma)));
    std::vector<float> k(size_t(2*r+1));
    double sum=0;
    for(int i=-r;i<=r;++i){k[size_t(i+r)]=float(std::exp(-0.5*double(i*i)/(sigma*sigma)));sum+=k[size_t(i+r)];}
    for(auto&v:k)v=float(double(v)/sum);
    std::vector<float> tmp(size_t(w)*h,0.f);
    for(int y=0;y<h;++y)for(int x=0;x<w;++x){
        double a=0;
        for(int i=-r;i<=r;++i)a+=double(in[size_t(y)*w+std::clamp(x+i,0,w-1)])*k[size_t(i+r)];
        tmp[size_t(y)*w+x]=float(a);
    }
    out.assign(size_t(w)*h,0.f);
    for(int y=0;y<h;++y)for(int x=0;x<w;++x){
        double a=0;
        for(int i=-r;i<=r;++i)a+=double(tmp[size_t(std::clamp(y+i,0,h-1))*w+x])*k[size_t(i+r)];
        out[size_t(y)*w+x]=float(a);
    }
}
static double harmonicRatio(const std::vector<float>&px)
{
    std::vector<float> g08,g16,g30;
    blur(px,g08,int(W),int(H),0.8); blur(px,g16,int(W),int(H),1.6); blur(px,g30,int(W),int(H),3.0);
    double ridge=0,harm=0;long n=0;
    for(uint32_t y=6;y<H-6;++y)for(uint32_t x=6;x<W-6;++x){
        const size_t i=size_t(y)*W+x;
        const double r=double(g16[i])-double(g30[i]);   // period 7-13px
        const double a=double(g08[i])-double(g16[i]);   // period 3.5-6px
        ridge+=r*r; harm+=a*a; ++n;
    }
    if(!n||ridge<=1e-9)return 0.0;
    return harm/ridge;
}

static double best(vkmap::Placer&p,const std::vector<float>&mp,const std::vector<float>&ms,
                   uint32_t size,const std::vector<float>&probe,double span,double step,int range){
    p.setMap(mp,ms,size); p.setProbe(probe,W,H);
    const int cx=(int(size)-int(W))/2,cy=(int(size)-int(H))/2;
    std::vector<vkmap::Pose> poses;
    for(double a=-span;a<=span+1e-9;a+=step)
        for(int oy=-range;oy<=range;oy+=2)for(int ox=-range;ox<=range;ox+=2)
            poses.push_back({float(cx+ox),float(cy+oy),float(a)});
    const auto r=p.run(poses,2);
    double b=-2;
    for(const auto&s:r) if(s.overlap>0.35f) b=std::max(b,double(s.ncc));
    return b;
}
int main(int argc,char**argv){
    QDir dir(argv[1]);
    const QStringList files=dir.entryList(QStringList()<<"candidate-touch-*.png",QDir::Files,QDir::Name);
    std::vector<std::vector<float>> caps;
    for(const QString&n:files){
        QImage f(dir.filePath(n));
        if(f.size()!=QSize(int(W),int(H)))continue;
        std::vector<float> px(size_t(W)*H);
        for(uint32_t y=0;y<H;++y)for(uint32_t x=0;x<W;++x)px[y*W+x]=float(qGray(f.pixel(int(x),int(y))));
        caps.push_back(std::move(px));
    }
    if(argc>4)kQualityFloor=std::atof(argv[4]);
    if(argc>5)kGalleryCap=std::atoi(argv[5]);
    if(argc>6)kFusion=std::atoi(argv[6]);
    if(argc>7)kWeightCap=float(std::atof(argv[7]));
    printf("품질 하한 %.3f · 융합 %s · 가중치 상한 %.1f\n",kQualityFloor,kFusion?"최선 선택":"깃털 혼합",kWeightCap);
    const size_t split=caps.size()*7/10;
    printf("캡처 %zu장 → 등록용 %zu장 / 검증용 %zu장\n",caps.size(),split,caps.size()-split);
    vkmap::Placer placer(argv[2]);
    printf("GPU: %s\n\n",placer.deviceName().c_str());

    // build the map from the enrolment half only
    std::vector<float> mp(size_t(kMap)*kMap,0.f),ms(size_t(kMap)*kMap,0.f);
    int placed=0;
    for(size_t i=0;i<split;++i){
        const double q=harmonicRatio(caps[i]);
        if(q<kQualityFloor)continue;
        float bx=float((kMap-W)/2),by=float((kMap-H)/2),ba=0; double fit=0.70;
        if(placed>0){
            placer.setMap(mp,ms,kMap); placer.setProbe(caps[i],W,H);
            const int cx=(int(kMap)-int(W))/2,cy=(int(kMap)-int(H))/2;
            std::vector<vkmap::Pose> poses;
            for(double a=-15;a<=15.001;a+=3)
                for(int oy=-66;oy<=66;oy+=3)for(int ox=-66;ox<=66;ox+=3)
                    poses.push_back({float(cx+ox),float(cy+oy),float(a)});
            const auto r=placer.run(poses,2);
            double b=-2; size_t at=0;
            for(size_t k=0;k<r.size();++k) if(r[k].overlap>0.25f&&r[k].ncc>b){b=r[k].ncc;at=k;}
            if(b<0.30)continue;
            bx=poses[at].offsetX;by=poses[at].offsetY;ba=poses[at].angleDegrees;fit=b;
        }
        const double a=double(ba)*M_PI/180.0,c=std::cos(a),s=std::sin(a);
        for(uint32_t y=0;y<H;++y)for(uint32_t x=0;x<W;++x){
            const float tx=std::min(1.f,float(std::min(x,W-1-x))/12.f);
            const float ty=std::min(1.f,float(std::min(y,H-1-y))/12.f);
            const float w=tx*ty*float(fit*q);
            if(w<=0)continue;
            const double dx=double(x)-W/2.0,dy=double(y)-H/2.0;
            const int mx=int(std::lround(bx+(c*dx-s*dy)+W/2.0));
            const int my=int(std::lround(by+(s*dx+c*dy)+H/2.0));
            if(mx<0||my<0||mx>=int(kMap)||my>=int(kMap))continue;
            const size_t idx=size_t(my)*kMap+size_t(mx);
            if(kFusion==1){
                // Whichever capture has the strongest claim here owns the
                // pixel outright. This is what the window used to draw, and it
                // was replaced because the seams were visible - but a seam only
                // matters to a person looking at it, and the thing doing the
                // matching has never been asked which it prefers.
                if(w>ms[idx]){mp[idx]=caps[i][size_t(y)*W+x];ms[idx]=w;}
            } else {
                mp[idx]=(mp[idx]*ms[idx]+caps[i][size_t(y)*W+x]*w)/(ms[idx]+w);
                ms[idx]=std::min(kWeightCap,ms[idx]+w);
            }
        }
        ++placed;
    }
    long covered=0; for(size_t i=0;i<ms.size();++i) if(ms[i]>0)++covered;
    printf("지도: %d장 배치, 덮인 %.2f배\n\n",placed,double(covered)/(108.0*88.0));

    // each held-out probe against the map, and against the same captures as a gallery
    // the enrolment captures as a gallery, each on its own canvas, built once
    std::vector<std::vector<float>> galleryPixels,galleryWeights;
    for(size_t k=0;k<split;++k){
        if(harmonicRatio(caps[k])<kQualityFloor)continue;
        std::vector<float> one(size_t(kMap)*kMap,0.f),onew(size_t(kMap)*kMap,0.f);
        const int ox=(int(kMap)-int(W))/2,oy=(int(kMap)-int(H))/2;
        for(uint32_t y=0;y<H;++y)for(uint32_t x=0;x<W;++x){
            const size_t idx=size_t(oy+y)*kMap+size_t(ox+x);
            one[idx]=caps[k][size_t(y)*W+x]; onew[idx]=1.f;
        }
        galleryPixels.push_back(std::move(one)); galleryWeights.push_back(std::move(onew));
    }
    // Authentication pays for every reference it searches, so the gallery is
    // capped and the sharpest captures are the ones kept.
    if(kGalleryCap>0&&galleryPixels.size()>size_t(kGalleryCap)){
        std::vector<std::pair<double,size_t>> rank;
        for(size_t k=0,g=0;k<split;++k){
            if(harmonicRatio(caps[k])<kQualityFloor)continue;
            rank.push_back({harmonicRatio(caps[k]),g++});
        }
        std::sort(rank.begin(),rank.end(),[](auto&a,auto&b){return a.first>b.first;});
        rank.resize(size_t(kGalleryCap));
        std::vector<std::vector<float>> gp,gw;
        for(auto&r:rank){gp.push_back(galleryPixels[r.second]);gw.push_back(galleryWeights[r.second]);}
        galleryPixels.swap(gp); galleryWeights.swap(gw);
    }
    printf("갤러리 참조 %zu장\n\n",galleryPixels.size());

    auto scoreAgainstGallery=[&](const std::vector<float>&probe){
        double g=-2;
        for(size_t k=0;k<galleryPixels.size();++k)
            g=std::max(g,best(placer,galleryPixels[k],galleryWeights[k],kMap,probe,15,3,30));
        return g;
    };

    std::vector<std::vector<float>> impostors;
    if(argc>3){
        QDir idir(argv[3]);
        for(const QString&n:idir.entryList(QStringList()<<"candidate-touch-*.png",QDir::Files,QDir::Name)){
            QImage f(idir.filePath(n));
            if(f.size()!=QSize(int(W),int(H)))continue;
            std::vector<float> px(size_t(W)*H);
            for(uint32_t y=0;y<H;++y)for(uint32_t x=0;x<W;++x)px[y*W+x]=float(qGray(f.pixel(int(x),int(y))));
            impostors.push_back(std::move(px));
        }
        printf("다른 손가락 %zu장\n\n",impostors.size());
    }

    std::vector<double> mapScores,galleryScores,mapImp,galImp;
    for(size_t i=split;i<caps.size();++i){
        mapScores.push_back(best(placer,mp,ms,kMap,caps[i],15,3,66));
        galleryScores.push_back(scoreAgainstGallery(caps[i]));
    }
    for(const auto&im:impostors){
        mapImp.push_back(best(placer,mp,ms,kMap,im,15,3,66));
        galImp.push_back(scoreAgainstGallery(im));
    }
    auto stats=[](const std::vector<double>&v,double&mean,double&var){
        mean=0; for(double x:v)mean+=x; mean/=double(std::max<size_t>(1,v.size()));
        var=0; for(double x:v)var+=(x-mean)*(x-mean); var/=double(std::max<size_t>(1,v.size()));
    };
    auto report=[&](const char*tag,std::vector<double> gen,std::vector<double> imp){
        double gm,gv,im_=0,iv=0; stats(gen,gm,gv);
        std::vector<double> gs=gen; std::sort(gs.begin(),gs.end());
        printf("== %s ==\n",tag);
        printf("  같은 손가락 %2zu장: 중앙 %.3f  평균 %.3f  최소 %.3f\n",
            gen.size(),gs[gs.size()/2],gm,gs.front());
        if(!imp.empty()){
            stats(imp,im_,iv);
            std::vector<double> is_=imp; std::sort(is_.begin(),is_.end());
            printf("  다른 손가락 %2zu장: 중앙 %.3f  평균 %.3f  최대 %.3f\n",
                imp.size(),is_[is_.size()/2],im_,is_.back());
            const double d=(gm-im_)/std::sqrt(0.5*(gv+iv)+1e-12);
            printf("  분리도 d' = %.3f\n",d);
            printf("  %8s %10s %10s\n","임계","정답률","오수락률");
            for(double th:{0.30,0.40,0.50,0.60,0.70}){
                size_t a=0,b=0;
                for(double x:gen) if(x>=th)++a;
                for(double x:imp) if(x>=th)++b;
                printf("  %8.2f %9.0f%% %9.0f%%\n",th,
                    100.0*double(a)/double(gen.size()),100.0*double(b)/double(imp.size()));
            }
        }
        printf("\n");
    };
    report("지도",mapScores,mapImp);
    report("갤러리",galleryScores,galImp);
    printf("다른 손가락은 같은 사람의 다른 손가락입니다. 타인에 대한 오수락률은\n");
    printf("이 측정으로 전혀 알 수 없고, 어느 쪽도 인증에 안전하다는 근거가 아닙니다.\n");
    return 0;
}
