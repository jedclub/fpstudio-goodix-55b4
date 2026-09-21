#pragma once

#include "vkmatch.h"
#include "profile.h"
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

/* Zhang-Suen's per-pixel test, precomputed for all 256 neighbourhoods.
 *
 * The test reads the eight neighbours, counts them, counts the 0->1
 * transitions around the ring, and checks two phase-specific triples. All of
 * that depends only on which of the eight are set - 256 possibilities - so it
 * is a property of an 8-bit number and can be answered by looking it up.
 *
 * Written out per pixel it was eight index calculations, an eight-element
 * array built on the stack, and a wrap-around loop to count transitions, for
 * every foreground pixel on every one of up to 96 passes. Packed, the same
 * decision is one byte and one load.
 *
 * Bit order is the ring order, so that transitions are adjacent bits and the
 * wrap is between bit 7 and bit 0:
 *
 *   bit 0 = N   bit 1 = NE  bit 2 = E   bit 3 = SE
 *   bit 4 = S   bit 5 = SW  bit 6 = W   bit 7 = NW
 *
 * The tables hold the same booleans the inline code computed, so the thinned
 * output is identical, not merely equivalent.
 */
struct MinutiaeThinTable { uint8_t removable[2][256]; };

inline const MinutiaeThinTable &minutiaeThinTable() {
    static const MinutiaeThinTable table = [] {
        MinutiaeThinTable t{};
        for(int code=0;code<256;++code) {
            int b[8];
            for(int i=0;i<8;++i)b[i]=(code>>i)&1;
            int n=0,transitions=0;
            for(int i=0;i<8;++i)n+=b[i];
            for(int i=0;i<8;++i)transitions+=b[i]==0&&b[(i+1)&7]==1;
            const int p2=b[0],p4=b[2],p6=b[4],p8=b[6];
            const bool base=n>=2&&n<=6&&transitions==1;
            t.removable[0][code]=base&&!((p2&&p4&&p6)||(p4&&p6&&p8));
            t.removable[1][code]=base&&!((p2&&p4&&p8)||(p2&&p6&&p8));
        }
        return t;
    }();
    return table;
}

// Zhang-Suen thinning, operating on a bounded 108x88 binary ridge image.
inline void thinRidges(std::vector<uint8_t> &bits,int w,int h) {
    FPSTUDIO_PROFILE_SCOPE("minutiae/thinRidges");
    const MinutiaeThinTable &table=minutiaeThinTable();
    std::vector<int> remove;
    // Every pixel could in principle be removed in one pass, and this runs up
    // to 192 times; reserving once keeps the vector from regrowing on each.
    remove.reserve(size_t(w)*h);
    for(int iteration=0;iteration<96;++iteration) {
        bool changed=false;
        for(int phase=0;phase<2;++phase) {
            remove.clear();
            const uint8_t *const decide=table.removable[phase];
            for(int y=1;y<h-1;++y) {
                // Three row pointers instead of eight index calculations. The
                // neighbourhood is three consecutive rows, so this is also the
                // access pattern the prefetcher expects.
                const uint8_t *up=bits.data()+size_t(y-1)*w;
                const uint8_t *mid=bits.data()+size_t(y)*w;
                const uint8_t *down=bits.data()+size_t(y+1)*w;
                for(int x=1;x<w-1;++x) {
                    if(!mid[x])continue;
                    // bits[] is strictly 0 or 1 - it is written from a boolean
                    // and only ever cleared - so each neighbour contributes
                    // exactly one bit and no masking is needed.
                    const unsigned code=
                        unsigned(up[x])          |
                        unsigned(up[x+1])   <<1  |
                        unsigned(mid[x+1])  <<2  |
                        unsigned(down[x+1]) <<3  |
                        unsigned(down[x])   <<4  |
                        unsigned(down[x-1]) <<5  |
                        unsigned(mid[x-1])  <<6  |
                        unsigned(up[x-1])   <<7;
                    if(decide[code])remove.push_back(int(size_t(y)*w+x));
                }
            }
            for(int i:remove)bits[i]=0;
            changed|=!remove.empty();
        }
        if(!changed)break;
    }
}

// The accumulator here is a structure tensor, written without the polar
// detour it used to take.
//
// It used to convert every neighbour's gradient to an angle and back:
//   theta = atan2(gy, gx) + pi/2
//   re   += e * cos(2*theta)
//   imag += e * sin(2*theta)          with e = gx^2 + gy^2
//
// That is three transcendental calls per neighbour pixel, up to 81 of them per
// candidate. None of them are needed. Doubling the angle is exactly what the
// standard identities undo:
//
//   cos(2*theta) = cos(2*phi + pi) = -cos(2*phi) = -(gx^2 - gy^2) / e
//   sin(2*theta) = sin(2*phi + pi) = -sin(2*phi) = -(2*gx*gy)     / e
//
// and both are being multiplied by e, so e cancels and what is left is
// polynomial:
//
//   e * cos(2*theta) = gy^2 - gx^2
//   e * sin(2*theta) = -2*gx*gy
//
// This is an algebraic identity, not an approximation: the same quantity by a
// shorter route, differing only in floating-point rounding. Two atan2 calls
// remain per candidate - one for the reported direction, one inside hypot -
// rather than three per neighbour.
inline double minutiaeCoherence(const Image &im,int x,int y,float *direction) {
    FPSTUDIO_PROFILE_SCOPE("minutiae/coherence");
    const int w=int(im.width),h=int(im.height);double re=0,imag=0,energy=0;
    const int ylo=std::max(1,y-4),yhi=std::min(h-2,y+4);
    const int xlo=std::max(1,x-4),xhi=std::min(w-2,x+4);
    for(int yy=ylo;yy<=yhi;++yy) {
        const float *row=im.pixels.data()+size_t(yy)*w;
        const float *above=row-w,*below=row+w;
        for(int xx=xlo;xx<=xhi;++xx) {
            const double gx=double(row[xx+1])-row[xx-1];
            const double gy=double(below[xx])-above[xx];
            const double gxx=gx*gx,gyy=gy*gy;
            const double e=gxx+gyy;if(e<1e-8)continue;
            re+=gyy-gxx;imag-=2*gx*gy;energy+=e;
        }
    }
    if(energy<=1e-8){*direction=0;return 0;}
    *direction=float(.5*std::atan2(imag,re));
    return std::clamp(std::hypot(re,imag)/energy,0.,1.);
}

inline Minutiae extractMinutiae(const Image &im) {
    FPSTUDIO_PROFILE_SCOPE("minutiae/extractMinutiae");
    Minutiae result;
    const int w=int(im.width),h=int(im.height);
    if(w<32||h<32||im.pixels.size()!=size_t(w)*h)return result;
    std::vector<uint8_t> ridges(size_t(w)*h);
    std::vector<double> localSd(size_t(w)*h);
    // Local thresholding makes this invariant to a modest contact brightness
    // change. Pixels darker than their 11x11 neighbourhood are ridge seeds.
    //
    // The window is a rectangle, so its sum separates into a horizontal pass
    // and a vertical one, and each pass slides rather than re-adding. The
    // direct form read 121 pixels per output pixel - 1,149,984 multiply-adds
    // for one 108x88 frame, which profiling showed was most of the cost of
    // extracting minutiae at all. This reads two.
    //
    // It computes the same sum over the same clamped rectangle, so the result
    // is identical bar floating-point ordering; both accumulators stay double
    // precisely because sd is a difference of two nearly equal quantities and
    // is then compared against a fixed 0.022.
    //
    // The vertical pass carries one row of running totals (w doubles, under a
    // kilobyte, L1-resident) and streams the row tables sequentially, rather
    // than walking each column with a stride of w - same arithmetic, but an
    // access pattern the prefetcher can follow.
    constexpr int radius=5,window=2*radius+1;
    // Only the 11 rows currently inside the vertical window are ever needed, so
    // only 11 are kept. Holding the whole frame's row sums instead cost 152 KB
    // of scratch for a 32 KB L1, and it showed: the first version of this was
    // 3.9x faster than the direct loop but took L1 misses from 12.5M to 44.0M
    // in doing it. A ring of 11 rows is 19 KB, and with the two column
    // accumulators the whole working set is about 21 KB - inside L1, and
    // streamed in order rather than walked down columns with a stride of w.
    //
    // The slot arithmetic has one trap worth stating: the row entering the
    // window and the row leaving it are exactly `window` apart, so they share a
    // ring slot. The departing row must therefore be subtracted from the column
    // totals *before* the arriving row is written over it.
    std::vector<double> ring(size_t(window)*w*2);
    double *ringSum=ring.data(),*ringSq=ring.data()+size_t(window)*w;
    auto fillRow=[&](int r) {
        const float *row=im.pixels.data()+size_t(r)*w;
        const size_t slot=size_t(r%window)*w;
        double *outSum=ringSum+slot,*outSq=ringSq+slot;
        double sum=0,sq=0;
        // Prime the window with [0, radius]; each step adds one column on the
        // right and drops one on the left, clamped at both edges.
        for(int x=0;x<=std::min(w-1,radius);++x) { sum+=row[x];sq+=double(row[x])*row[x]; }
        for(int x=0;x<w;++x) {
            outSum[x]=sum;outSq[x]=sq;
            const int add=x+radius+1,drop=x-radius;
            if(add<w) { sum+=row[add];sq+=double(row[add])*row[add]; }
            if(drop>=0) { sum-=row[drop];sq-=double(row[drop])*row[drop]; }
        }
    };
    std::vector<double> colSum(size_t(w),0.),colSq(size_t(w),0.);
    for(int y=0;y<=std::min(h-1,radius);++y) {
        fillRow(y);
        const size_t slot=size_t(y%window)*w;
        for(int x=0;x<w;++x) { colSum[x]+=ringSum[slot+x];colSq[x]+=ringSq[slot+x]; }
    }
    for(int y=0;y<h;++y) {
        // The window is clipped at the frame edge, so the divisor is the size
        // of the rectangle actually summed, exactly as the direct form's
        // ++count made it.
        const int ylo=std::max(0,y-radius),yhi=std::min(h-1,y+radius);
        const double rows=double(yhi-ylo+1);
        const float *row=im.pixels.data()+size_t(y)*w;
        double *sdRow=localSd.data()+size_t(y)*w;
        uint8_t *ridgeRow=ridges.data()+size_t(y)*w;
        for(int x=0;x<w;++x) {
            const int xlo=std::max(0,x-radius),xhi=std::min(w-1,x+radius);
            const double n=rows*(xhi-xlo+1);
            const double mean=colSum[x]/n;
            const double sd=std::sqrt(std::max(0.,colSq[x]/n-mean*mean));
            sdRow[x]=sd;
            ridgeRow[x]=(sd>=.022&&row[x]<mean-.006)?1:0;
        }
        const int drop=y-radius,add=y+radius+1;
        if(drop>=0) {
            const size_t slot=size_t(drop%window)*w;
            for(int x=0;x<w;++x) { colSum[x]-=ringSum[slot+x];colSq[x]-=ringSq[slot+x]; }
        }
        if(add<h) {
            fillRow(add);                       // reuses the slot just vacated
            const size_t slot=size_t(add%window)*w;
            for(int x=0;x<w;++x) { colSum[x]+=ringSum[slot+x];colSq[x]+=ringSq[slot+x]; }
        }
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
