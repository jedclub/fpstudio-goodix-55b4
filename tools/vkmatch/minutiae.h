#pragma once

#include "vkmatch.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <vector>

namespace vkmatch {

// Research-only ISO/INCITS-style minutiae evidence. A point is retained only
// if it is well inside the acquired patch and survives binarisation/thinning.
// It is deliberately not an authentication gate until genuine/impostor data
// calibrates its false-accept and false-reject behaviour for this sensor.
enum class MinutiaType { Ending, Bifurcation };
struct Minutia {
    float x=0, y=0, direction=0, quality=0;
    MinutiaType type=MinutiaType::Ending;
};
struct Minutiae {
    std::vector<Minutia> points;
    int rawEndings=0,rawBifurcations=0;
    bool usable=false;
};
struct MinutiaEvidence {
    int referenceCount=0,probeCount=0,paired=0,typeAgreement=0;
    double coverage=0,orientationAgreement=0;
    bool consistent=false;
};

inline constexpr float minutiaePi=3.14159265358979323846f;
inline float minutiaeOrientationDifference(float a,float b) {
    // Ridge orientation is axial: theta and theta + pi denote the same ridge.
    float d=std::abs(std::remainder(a-b,minutiaePi));
    return std::min(d,minutiaePi-d);
}

inline int minutiaeIndex(int x,int y,int w) { return y*w+x; }

inline int minutiaeNeighbours(const std::vector<uint8_t> &bits,int x,int y,int w) {
    int n=0;
    for(int dy=-1;dy<=1;++dy)for(int dx=-1;dx<=1;++dx)
        if((dx||dy)&&bits[minutiaeIndex(x+dx,y+dy,w)])++n;
    return n;
}

inline int minutiaeCrossingNumber(const std::vector<uint8_t> &bits,int x,int y,int w) {
    const int dx[]={0,1,1,1,0,-1,-1,-1};
    const int dy[]={-1,-1,0,1,1,1,0,-1};
    int transitions=0;
    for(int i=0;i<8;++i) {
        const int a=bits[minutiaeIndex(x+dx[i],y+dy[i],w)];
        const int b=bits[minutiaeIndex(x+dx[(i+1)%8],y+dy[(i+1)%8],w)];
        transitions+=a!=b;
    }
    return transitions/2;
}

// Zhang-Suen thinning, operating on a bounded 108x88 binary ridge image.
inline void thinRidges(std::vector<uint8_t> &bits,int w,int h) {
    std::vector<int> remove;
    for(int iteration=0;iteration<96;++iteration) {
        bool changed=false;
        for(int phase=0;phase<2;++phase) {
            remove.clear();
            for(int y=1;y<h-1;++y)for(int x=1;x<w-1;++x) {
                if(!bits[minutiaeIndex(x,y,w)])continue;
                const int p2=bits[minutiaeIndex(x,y-1,w)],p3=bits[minutiaeIndex(x+1,y-1,w)];
                const int p4=bits[minutiaeIndex(x+1,y,w)],p5=bits[minutiaeIndex(x+1,y+1,w)];
                const int p6=bits[minutiaeIndex(x,y+1,w)],p7=bits[minutiaeIndex(x-1,y+1,w)];
                const int p8=bits[minutiaeIndex(x-1,y,w)],p9=bits[minutiaeIndex(x-1,y-1,w)];
                const int n=p2+p3+p4+p5+p6+p7+p8+p9;
                const int ring[]={p2,p3,p4,p5,p6,p7,p8,p9};int transitions=0;
                for(int i=0;i<8;++i)transitions+=ring[i]==0&&ring[(i+1)%8]==1;
                if(n<2||n>6||transitions!=1)continue;
                if((phase==0&&((p2&&p4&&p6)||(p4&&p6&&p8)))||
                   (phase==1&&((p2&&p4&&p8)||(p2&&p6&&p8))))continue;
                remove.push_back(minutiaeIndex(x,y,w));
            }
            for(int i:remove)bits[i]=0;
            changed|=!remove.empty();
        }
        if(!changed)break;
    }
}

inline double minutiaeCoherence(const Image &im,int x,int y,float *direction) {
    const int w=int(im.width),h=int(im.height);double re=0,imag=0,energy=0;
    for(int yy=std::max(1,y-4);yy<=std::min(h-2,y+4);++yy)
        for(int xx=std::max(1,x-4);xx<=std::min(w-2,x+4);++xx) {
            const double gx=im.pixels[yy*w+xx+1]-im.pixels[yy*w+xx-1];
            const double gy=im.pixels[(yy+1)*w+xx]-im.pixels[(yy-1)*w+xx];
            const double e=gx*gx+gy*gy;if(e<1e-8)continue;
            const double theta=std::atan2(gy,gx)+minutiaePi*.5;
            re+=e*std::cos(2*theta);imag+=e*std::sin(2*theta);energy+=e;
        }
    if(energy<=1e-8){*direction=0;return 0;}
    *direction=float(.5*std::atan2(imag,re));
    return std::clamp(std::hypot(re,imag)/energy,0.,1.);
}

inline Minutiae extractMinutiae(const Image &im) {
    Minutiae result;
    const int w=int(im.width),h=int(im.height);
    if(w<32||h<32||im.pixels.size()!=size_t(w)*h)return result;
    std::vector<uint8_t> ridges(size_t(w)*h);
    std::vector<double> localSd(size_t(w)*h);
    // Local thresholding makes this invariant to a modest contact brightness
    // change. Pixels darker than their 11x11 neighbourhood are ridge seeds.
    for(int y=0;y<h;++y)for(int x=0;x<w;++x) {
        double sum=0,sq=0;int count=0;
        for(int yy=std::max(0,y-5);yy<=std::min(h-1,y+5);++yy)
            for(int xx=std::max(0,x-5);xx<=std::min(w-1,x+5);++xx) {
                const double v=im.pixels[yy*w+xx];sum+=v;sq+=v*v;++count;
            }
        const double mean=sum/count,sd=std::sqrt(std::max(0.,sq/count-mean*mean));
        localSd[minutiaeIndex(x,y,w)]=sd;
        ridges[minutiaeIndex(x,y,w)]=(sd>=.022&&im.pixels[y*w+x]<mean-.006)?1:0;
    }
    thinRidges(ridges,w,h);
    constexpr int margin=12;std::vector<Minutia> candidates;
    for(int y=margin;y<h-margin;++y)for(int x=margin;x<w-margin;++x) {
        if(!ridges[minutiaeIndex(x,y,w)])continue;
        const int crossing=minutiaeCrossingNumber(ridges,x,y,w);
        const auto type=crossing==1?MinutiaType::Ending:(crossing==3?MinutiaType::Bifurcation:MinutiaType::Ending);
        if(crossing!=1&&crossing!=3)continue;
        if(crossing==1)++result.rawEndings;else ++result.rawBifurcations;
        float direction=0;const double coherence=minutiaeCoherence(im,x,y,&direction);
        const float quality=float(.55*coherence+.45*std::min(1.,localSd[minutiaeIndex(x,y,w)]/.10));
        // Low-coherence points are normally binarisation/skeleton artefacts.
        if(quality<.45f)continue;
        candidates.push_back({float(x),float(y),direction,quality,type});
    }
    std::sort(candidates.begin(),candidates.end(),[](const auto&a,const auto&b){return a.quality>b.quality;});
    // One unstable ridge break often creates a cluster of nearby crossings;
    // retain only its best representative. Edge points were discarded above.
    for(const auto &point:candidates) {
        const bool nearby=std::any_of(result.points.begin(),result.points.end(),[&](const auto &old){
            return std::hypot(point.x-old.x,point.y-old.y)<4.5f;
        });
        if(!nearby)result.points.push_back(point);
        if(result.points.size()==64)break;
    }
    result.usable=result.points.size()>=3;
    return result;
}

inline MinutiaEvidence assessMinutiae(const Minutiae &reference,const Minutiae &probe,const Job &job,
                                      uint32_t width,uint32_t height) {
    MinutiaEvidence out;out.referenceCount=reference.points.size();out.probeCount=probe.points.size();
    if(!reference.usable||!probe.usable||width<2||height<2)return out;
    struct Edge {int reference,probe;double cost,angle;bool sameType;};std::vector<Edge> edges;
    const double angle=job.angleDegrees*minutiaePi/180,c=std::cos(angle),s=std::sin(angle);
    const auto anchor=resolveTransformAnchor(job,width,height);
    for(size_t i=0;i<reference.points.size();++i) {
        const auto &a=reference.points[i];const double dx=a.x-anchor.pivotX,dy=a.y-anchor.pivotY;
        const double x=anchor.anchorX+job.scale*(c*dx-s*dy)+job.dx,y=anchor.anchorY+job.scale*(s*dx+c*dy)+job.dy;
        const float transformedDirection=float(a.direction+angle);
        for(size_t j=0;j<probe.points.size();++j) {
            const auto &b=probe.points[j];const double distance=std::hypot(x-b.x,y-b.y);
            const double direction=minutiaeOrientationDifference(transformedDirection,b.direction);
            if(distance>3.2||direction>35.*minutiaePi/180)continue;
            edges.push_back({int(i),int(j),distance+.8*direction,direction,a.type==b.type});
        }
    }
    std::sort(edges.begin(),edges.end(),[](const auto&a,const auto&b){return a.cost<b.cost;});
    std::vector<bool> usedReference(reference.points.size()),usedProbe(probe.points.size());double directionSum=0;
    for(const auto &edge:edges) {
        if(usedReference[edge.reference]||usedProbe[edge.probe])continue;
        usedReference[edge.reference]=usedProbe[edge.probe]=true;++out.paired;
        out.typeAgreement+=edge.sameType;directionSum+=edge.angle;
    }
    out.coverage=out.paired/double(std::min(reference.points.size(),probe.points.size()));
    out.orientationAgreement=out.paired?1.-directionSum/out.paired/(35.*minutiaePi/180):0.;
    out.consistent=out.paired>=3&&out.coverage>=.28&&out.orientationAgreement>=.5;
    return out;
}

} // namespace vkmatch
