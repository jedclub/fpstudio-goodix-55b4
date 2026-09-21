// Replay a real session's captures onto the coverage map with the GPU placer,
// and measure what the wider search buys.
#include "mapplace.h"
#include <QImage>
#include <QDir>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>
constexpr uint32_t kMap=300,W=108,H=88;
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
    printf("== %s : %zu장 ==\n",qPrintable(dir.dirName()),caps.size());
    vkmap::Placer placer(argv[2]);
    printf("GPU: %s\n\n",placer.deviceName().c_str());

    struct Cfg{const char*tag;double span,step;int range,coarse;};
    Cfg cfgs[]={
        {"현재와 동일 (회전 없음)",0,1,60,2},
        {"회전 ±8도 / 2도",8,2,60,2},
        {"회전 ±15도 / 1.5도",15,1.5,60,2},
        {"회전 ±25도 / 1.5도 + 전수이동",25,1.5,60,1},
    };
    for(auto&cf:cfgs){
        std::vector<float> mp(size_t(kMap)*kMap,0.f),ms(size_t(kMap)*kMap,0.f);
        int placed=0,rejected=0; double fitsum=0,gpuMs=0,wallMs=0; long poseTotal=0;
        const int cx=(int(kMap)-int(W))/2, cy=(int(kMap)-int(H))/2;
        for(size_t i=0;i<caps.size();++i){
            float bx=float(cx),by=float(cy),bd=0; double fit=0.70;
            if(placed>0){
                placer.setMap(mp,ms,kMap);
                placer.setProbe(caps[i],W,H);
                std::vector<vkmap::Pose> poses;
                for(double a=-cf.span;a<=cf.span+1e-9;a+=(cf.span>0?cf.step:1e9))
                    for(int oy=-cf.range;oy<=cf.range;oy+=cf.coarse)
                        for(int ox=-cf.range;ox<=cf.range;ox+=cf.coarse)
                            poses.push_back({float(cx+ox),float(cy+oy),float(a)});
                const auto r=placer.run(poses,2);
                poseTotal+=long(poses.size());
                gpuMs+=placer.lastGpuMs()>0?placer.lastGpuMs():0;
                wallMs+=placer.lastWallMs();
                double best=-2; size_t bi=0;
                for(size_t k=0;k<r.size();++k)
                    if(r[k].overlap>0.25f && r[k].ncc>best){best=r[k].ncc;bi=k;}
                if(best<0.30){++rejected;continue;}
                bx=poses[bi].offsetX;by=poses[bi].offsetY;bd=poses[bi].angleDegrees;fit=best;
            }
            const double a=double(bd)*M_PI/180.0,c=std::cos(a),s=std::sin(a);
            for(uint32_t y=0;y<H;++y)for(uint32_t x=0;x<W;++x){
                const float tx=std::min(1.f,float(std::min(x,W-1-x))/12.f);
                const float ty=std::min(1.f,float(std::min(y,H-1-y))/12.f);
                const float w=tx*ty*float(fit);
                if(w<=0)continue;
                const double dx=double(x)-W/2.0,dy=double(y)-H/2.0;
                const int mx=int(std::lround(bx+(c*dx-s*dy)+W/2.0));
                const int my=int(std::lround(by+(s*dx+c*dy)+H/2.0));
                if(mx<0||my<0||mx>=int(kMap)||my>=int(kMap))continue;
                const size_t idx=size_t(my)*kMap+size_t(mx);
                mp[idx]=(mp[idx]*ms[idx]+caps[i][size_t(y)*W+x]*w)/(ms[idx]+w);
                ms[idx]=std::min(3.f,ms[idx]+w);
            }
            ++placed; fitsum+=fit;
        }
        long covered=0; for(size_t i=0;i<ms.size();++i) if(ms[i]>0) ++covered;
        printf("%-30s 배치 %3d  미반영 %2d (%2.0f%%)  평균정합 %.3f  덮인 %.2f배\n",
            cf.tag,placed,rejected,100.0*rejected/double(caps.size()),
            fitsum/std::max(1,placed),double(covered)/(108.0*88.0));
        printf("%-30s 자세 %ld개/장   GPU %.1f ms/장   전체 %.1f ms/장\n\n","",
            poseTotal/std::max(1,placed),gpuMs/std::max(1,placed),wallMs/std::max(1,placed));
    }
    return 0;
}
