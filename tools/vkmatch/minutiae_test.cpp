#include "minutiae.h"
#include <cmath>
#include <cstdio>
#include <stdexcept>

static void require(bool value,const char *message) {if(!value)throw std::runtime_error(message);}
static void dark(vkmatch::Image &image,int x,int y) {
    for(int yy=y-1;yy<=y+1;++yy)for(int xx=x-1;xx<=x+1;++xx)
        if(xx>=0&&yy>=0&&xx<int(image.width)&&yy<int(image.height))image.pixels[yy*image.width+xx]=.08f;
}
static void line(vkmatch::Image &image,int x0,int y0,int x1,int y1) {
    const int steps=std::max(std::abs(x1-x0),std::abs(y1-y0));
    for(int i=0;i<=steps;++i)dark(image,int(std::lround(x0+(x1-x0)*i/double(steps))),int(std::lround(y0+(y1-y0)*i/double(steps))));
}
int main() {
    try {
        vkmatch::Image interior{108,88,std::vector<float>(108*88,.88f)};
        line(interior,54,44,54,19);line(interior,54,44,27,68);line(interior,54,44,80,68);
        const auto points=vkmatch::extractMinutiae(interior);
        require(points.usable,"interior Y supplies enough stable minutiae");
        require(std::any_of(points.points.begin(),points.points.end(),[](const auto&p){
            return p.type==vkmatch::MinutiaType::Bifurcation&&std::hypot(p.x-54,p.y-44)<6;
        }),"interior bifurcation is retained");
        const auto evidence=vkmatch::assessMinutiae(points,points,{0,0,0,0,0,1},108,88);
        require(evidence.consistent&&evidence.paired>=3,"identity geometry preserves internal minutiae");
        vkmatch::Image edge{108,88,std::vector<float>(108*88,.88f)};
        line(edge,6,8,6,25);line(edge,6,8,20,8);line(edge,6,8,18,21);
        const auto edgePoints=vkmatch::extractMinutiae(edge);
        require(std::none_of(edgePoints.points.begin(),edgePoints.points.end(),[](const auto&p){return p.x<12||p.y<12;}),
                "outer sensor boundary minutiae are excluded");
    } catch(const std::exception &error) {std::fprintf(stderr,"%s\n",error.what());return 1;}
    return 0;
}
