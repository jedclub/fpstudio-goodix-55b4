#include "ridge_roi.h"

#include <cstdio>
#include <stdexcept>

static void require(bool value,const char *message) {if(!value)throw std::runtime_error(message);}

int main() {
    try {
        vkmatch::Image image{108,88,std::vector<float>(108*88,0.f)};
        // A contacted rectangle contains deliberately dark, ridge-like gaps.
        for(int y=18;y<71;++y)for(int x=21;x<89;++x)
            image.pixels[y*108+x]=((x+2*y)%9<2)?.02f:.72f;
        const auto region=vkmatch::extractRidgeRegion(image);
        require(region.usable,"finger component is usable");
        require(region.coverage>.34&&region.coverage<.55,"foreground area is bounded");
        require(region.at(54,44),"ridge valley stays foreground");
        require(!region.at(4,4),"background stays zero");
        require(region.centerX>50&&region.centerX<59&&region.centerY>40&&region.centerY<49,
                "contact component centre is stable");
        require(region.contactMask!=0&&region.contactMask!=0xffff,"4x4 diagnostic mask is selective");
        require(!vkmatch::regionContains(region,4.2,4.2),"background interpolation rejects");
        require(vkmatch::regionContains(region,54.3,44.7),"contact interpolation accepts");
        vkmatch::Image blank{108,88,std::vector<float>(108*88,0.f)};
        require(!vkmatch::extractRidgeRegion(blank).usable,"blank image has no foreground component");
    } catch(const std::exception &error) {std::fprintf(stderr,"%s\n",error.what());return 1;}
    return 0;
}
