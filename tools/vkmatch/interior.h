#pragma once
#include "vkmatch.h"
#include "ridge_roi.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>

namespace vkmatch {
struct InteriorEvidence {
    double ncc=0, gradient=0, support=0;
    int informativeTiles=0, supportedTiles=0, probeTiles=0;
    uint32_t informativeMask=0, supportedMask=0;
    // Contact is a binary foreground/background segmentation, not an
    // identity claim.  Only its overlap is eligible for ridge comparison.
    double referenceRoiCoverage=0, probeRoiCoverage=0, roiOverlap=0;
    double referenceRoiCenterX=0, referenceRoiCenterY=0;
    double probeRoiCenterX=0, probeRoiCenterY=0;
    uint32_t referenceRoiMask=0, probeRoiMask=0, roiOverlapMask=0;
    bool consistent=false;
};
// Research-only confirmation on original pixels: exclude physical image edges
// and require spatially distributed local intensity AND gradient agreement.
// Repeating non-biometric patterns can still pass; this is not identity proof.
inline InteriorEvidence assessInterior(const Image &a,const RidgeRegion &regionA,
                                      const Image &b,const RidgeRegion &regionB,const Job &job) {
    InteriorEvidence out;
    if(a.width!=b.width||a.height!=b.height||a.width<24||a.height<24||
       a.pixels.size()!=size_t(a.width)*a.height||b.pixels.size()!=a.pixels.size()||
       regionA.width!=a.width||regionA.height!=a.height||regionB.width!=b.width||regionB.height!=b.height||
       !regionA.usable||!regionB.usable)return out;
    struct Stats {double n=0,sa=0,sb=0,aa=0,bb=0,ab=0,gg=0,ga=0,gb=0;uint32_t probeMask=0;};
    std::array<Stats,16> tiles{};Stats all;
    const int w=a.width,h=a.height,margin=8;
    const double angle=job.angleDegrees*3.141592653589793/180,c=std::cos(angle),s=std::sin(angle);
    const auto anchor=resolveTransformAnchor(job,w,h);
    auto bilinear=[&](double x,double y){
        const int ix=int(x),iy=int(y);const double fx=x-ix,fy=y-iy;
        return (1-fy)*((1-fx)*b.pixels[iy*w+ix]+fx*b.pixels[iy*w+ix+1])
            +fy*((1-fx)*b.pixels[(iy+1)*w+ix]+fx*b.pixels[(iy+1)*w+ix+1]);
    };
    int total=0;
    for(int y=margin;y<h-margin;y+=2)for(int x=margin;x<w-margin;x+=2) {
        ++total;
        const int ti=(y-margin)*4/(h-2*margin)*4+(x-margin)*4/(w-2*margin);
        if(!regionA.at(x,y))continue;
        out.referenceRoiMask|=uint32_t(1)<<ti;
        const double dx=x-anchor.pivotX,dy=y-anchor.pivotY;
        const double u=anchor.anchorX+job.scale*(c*dx-s*dy)+job.dx,v=anchor.anchorY+job.scale*(s*dx+c*dy)+job.dy;
        if(!(u>=margin&&u<w-margin&&v>=margin&&v<h-margin))continue;
        const int pi=int(v-margin)*4/(h-2*margin)*4+int(u-margin)*4/(w-2*margin);
        if(!regionContains(regionB,u,v))continue;
        out.probeRoiMask|=uint32_t(1)<<pi;out.roiOverlapMask|=uint32_t(1)<<ti;
        const double av=a.pixels[y*w+x],bv=bilinear(u,v);
        const double ax=a.pixels[y*w+x+1]-a.pixels[y*w+x-1],ay=a.pixels[(y+1)*w+x]-a.pixels[(y-1)*w+x];
        const double px=bilinear(u+1,v)-bilinear(u-1,v),py=bilinear(u,v+1)-bilinear(u,v-1);
        const double bx=c*px+s*py,by=-s*px+c*py;
        auto add=[&](Stats &t){++t.n;t.sa+=av;t.sb+=bv;t.aa+=av*av;t.bb+=bv*bv;t.ab+=av*bv;
            t.gg+=ax*bx+ay*by;t.ga+=ax*ax+ay*ay;t.gb+=bx*bx+by*by;t.probeMask|=uint32_t(1)<<pi;};
        add(tiles[ti]);add(all);
    }
    auto corr=[](const Stats&t){
        if(t.n<8)return 0.;
        const double va=t.aa-t.sa*t.sa/t.n,vb=t.bb-t.sb*t.sb/t.n;
        if(va<t.n*.0001||vb<t.n*.0001)return 0.;
        return std::clamp((t.ab-t.sa*t.sb/t.n)/std::sqrt(va*vb),-1.,1.);
    };
    auto grad=[](const Stats&t){return t.ga*t.gb>1e-12?std::clamp(t.gg/std::sqrt(t.ga*t.gb),-1.,1.):0.;};
    uint32_t mask=0;
    for(size_t index=0;index<tiles.size();++index) {
        const auto &t=tiles[index];
        if(t.n<8||t.aa-t.sa*t.sa/t.n<t.n*.0001||t.bb-t.sb*t.sb/t.n<t.n*.0001)continue;
        ++out.informativeTiles;
        out.informativeMask|=uint32_t(1)<<index;
        if(corr(t)>=.5&&grad(t)>=.45){++out.supportedTiles;mask|=t.probeMask;out.supportedMask|=uint32_t(1)<<index;}
    }
    for(int i=0;i<16;++i)if(mask&(uint32_t(1)<<i))++out.probeTiles;
    out.ncc=corr(all);out.gradient=grad(all);out.support=total?all.n/total:0;
    out.referenceRoiCoverage=regionA.coverage;
    out.probeRoiCoverage=regionB.coverage;
    out.referenceRoiCenterX=regionA.centerX;out.referenceRoiCenterY=regionA.centerY;
    out.probeRoiCenterX=regionB.centerX;out.probeRoiCenterY=regionB.centerY;
    out.roiOverlap=out.support;
    out.consistent=out.support>=.4&&out.ncc>=.55&&out.gradient>=.5&&
        out.supportedTiles>=4&&out.probeTiles>=4&&out.supportedTiles*2>=out.informativeTiles;
    return out;
}
inline InteriorEvidence assessInterior(const Image &a,const Image &b,const Job &job) {
    const auto regionA=extractRidgeRegion(a),regionB=extractRidgeRegion(b);
    return assessInterior(a,regionA,b,regionB,job);
}
}
