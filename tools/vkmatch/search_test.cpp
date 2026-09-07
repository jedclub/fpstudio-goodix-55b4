#include "authpolicy.h"
#include <QCoreApplication>
#include <QImage>
#include <QJsonDocument>
#include <QProcess>
#include <QTemporaryDir>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>

// Analytic, non-biometric fixtures. These catch search/decision regressions,
// NOT false acceptance of another person's finger.
static double texture(double x,double y) {
    return .5+.17*std::sin(.31*x+.17*y)+.13*std::cos(.07*x-.43*y)+.09*std::sin(.011*x*y+.4);
}
static QImage make(const std::function<double(double,double)> &f) {
    QImage image(108,88,QImage::Format_Grayscale8);
    for(int y=0;y<88;++y)for(int x=0;x<108;++x)
        image.scanLine(y)[x]=std::clamp(int(std::lround(255*f(x,y))),0,255);
    return image;
}
int main(int argc,char **argv) {
    QCoreApplication app(argc,argv);if(argc!=2)return 2;
    QTemporaryDir work;if(!work.isValid())return 2;
    const auto reference=work.filePath("reference.png");
    if(!make(texture).save(reference))return 2;
    struct Case {const char *name;QImage image;bool pass;};
    const auto subpixel=make([](double x,double y){
        const double a=12.5*3.141592653589793/180,c=std::cos(a),s=std::sin(a);
        const double dx=x-53.5-3.25,dy=y-43.5+2.75;
        // 4.7% is deliberately near the new 5% authentication bound. The
        // nearest 5% GPU hypothesis must recover it without elastic warping.
        return texture(53.5+(c*dx+s*dy)/1.047,43.5+(-s*dx+c*dy)/1.047);
    });
    uint32_t seed=0x99887766u;
    const auto noise=make([&](double,double){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return double(seed>>8)/16777215.;});
    for(const auto &test:{Case{"subpixel",subpixel,true},Case{"flat",make([](double,double){return .5;}),false},
                          Case{"unrelated-noise",noise,false}}) {
        const auto path=work.filePath(QString::fromLatin1(test.name)+".png");if(!test.image.save(path))return 2;
        QProcess worker;worker.start(QString::fromLocal8Bit(argv[1]),{"--reference",reference,"--probe",path,"--auth-search"});
        if(!worker.waitForFinished(20000)||worker.exitStatus()!=QProcess::NormalExit||worker.exitCode()!=0)return 2;
        const auto row=QJsonDocument::fromJson(worker.readAllStandardOutput()).object();
        const auto scale=row.value("best").toObject().value("scale").toDouble();
        if(row.value("algorithm_version").toInt()!=8||row.value("auth_profile").toString()!=QStringLiteral("auth-v8-contact-anchored-ridge-roi-uniform-scale-5pct")||
           !row.value("auth_search").toBool()||
           std::abs(row.value("auth_scale_min").toDouble()-.95)>1e-5||
           std::abs(row.value("auth_scale_max").toDouble()-1.05)>1e-5||
           scale<.95-1e-5||scale>1.05+1e-5||
           row.value("best").toObject().value("transform_anchor").toString()!=QStringLiteral("contact-component-v1")||
           !row.value("best").toObject().value("ridge_roi").toObject().value("binary").toBool()||
           !row.value("ok").toBool()||authPatternAccepted(row)!=test.pass) {
            std::fprintf(stderr,"Search fixture failed: %s %s\n",test.name,QJsonDocument(row).toJson(QJsonDocument::Compact).constData());return 1;
        }
    }
    return 0;
}
