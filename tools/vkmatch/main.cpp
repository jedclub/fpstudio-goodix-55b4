#include "vkmatch.h"
#include "interior.h"
#include "authpolicy.h"
#include "minutiae.h"
#include "ridge_roi.h"
#include <QCoreApplication>
#include <QDir>
#include <QElapsedTimer>
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace {
vkmatch::Image loadImage(const QString &path) {
    QImage q(path);
    if (q.size()!=QSize(108,88)) throw std::runtime_error(("Expected 108x88 image: " + path).toStdString());
    q=q.convertToFormat(QImage::Format_Grayscale8);
    vkmatch::Image im{108,88,{}};im.pixels.reserve(108*88);
    for(int y=0;y<88;++y)for(int x=0;x<108;++x)im.pixels.push_back(q.constScanLine(y)[x]/255.f);
    return im;
}
vkmatch::Image normalize(const vkmatch::Image &im) {
    // Integral-image local contrast normalization. Never modifies the source.
    const int w=im.width,h=im.height,pitch=w+1;
    std::vector<double> sum((w+1)*(h+1)),sq(sum.size());
    for(int y=0;y<h;++y)for(int x=0;x<w;++x) {
        const double v=im.pixels[y*w+x];const int k=(y+1)*pitch+x+1;
        sum[k]=v+sum[k-1]+sum[k-pitch]-sum[k-pitch-1];
        sq[k]=v*v+sq[k-1]+sq[k-pitch]-sq[k-pitch-1];
    }
    auto out=im;
    for(int y=0;y<h;++y)for(int x=0;x<w;++x) {
        const int x0=std::max(0,x-5),x1=std::min(w,x+6),y0=std::max(0,y-5),y1=std::min(h,y+6);
        auto rect=[&](const std::vector<double>&a){return a[y1*pitch+x1]-a[y1*pitch+x0]-a[y0*pitch+x1]+a[y0*pitch+x0];};
        const double n=(x1-x0)*(y1-y0),mean=rect(sum)/n,sd=std::sqrt(std::max(0.,rect(sq)/n-mean*mean));
        out.pixels[y*w+x]=std::clamp(float(.5+.176*(im.pixels[y*w+x]-mean)/std::max(.04,sd)),0.f,1.f);
    }
    return out;
}
double rankScore(const vkmatch::Score &s) {
    if(!std::isfinite(s.ncc)||!std::isfinite(s.gradient)||s.overlap<.55f)return -2;
    return .65*s.ncc+.35*s.gradient-.15*(1-s.overlap)-.1*s.mae;
}
struct Ranked { vkmatch::Job job; vkmatch::Score score; };
bool boundary(const vkmatch::Job &j,float shift=24,float angle=30) {
    return std::abs(j.dx)>=shift||std::abs(j.dy)>=shift||std::abs(j.angleDegrees)>=angle;
}
bool distinct(const vkmatch::Job &a,const vkmatch::Job &b) {
    return std::abs(std::remainder(a.angleDegrees-b.angleDegrees,360.f))>=5 || std::hypot(a.dx-b.dx,a.dy-b.dy)>=5;
}
QJsonObject describe(const Ranked &r) {
    return {{"rank",rankScore(r.score)},{"ncc",r.score.ncc},{"gradient",r.score.gradient},
        {"overlap",r.score.overlap},{"mae",r.score.mae},{"angle",r.job.angleDegrees},
        {"dx",r.job.dx},{"dy",r.job.dy},{"scale",r.job.scale},
        {"transform_anchor",usesImageCentreAnchor(r.job)?QStringLiteral("image-centre-v1"):QStringLiteral("contact-component-v1")},
        {"rotation_pivot",QJsonArray{r.job.pivotX,r.job.pivotY}},
        {"rotation_anchor",QJsonArray{r.job.anchorX,r.job.anchorY}}};
}
QJsonObject evaluate(vkmatch::Matcher &gpu,const QStringList &paths,const std::vector<vkmatch::Image> &references,
                     const QString &probePath, bool refineScale, bool authSearch) {
    QElapsedTimer wall;wall.start();
    // Authentication permits only a uniform, bounded contact-size change.
    // This accommodates pressure/contact variation without allowing an
    // arbitrary aspect-ratio or elastic deformation.
    constexpr float authScaleMin=.95f,authScaleMax=1.05f;
    // Retain every v6 3% hypothesis while adding 5% endpoints. That makes
    // expanding this search monotonic: a previously tested transform is not
    // silently lost merely because the scale grid became wider.
    const std::vector<float> fineScales=authSearch?std::vector<float>{.95f,.97f,.975f,1.f,1.025f,1.03f,1.05f}:
        std::vector<float>{.97f,1.f,1.03f};
    auto bank=references;const auto probe=loadImage(probePath);
    bank.push_back(probe);bank.push_back(normalize(probe));gpu.setImages(bank);
    std::vector<vkmatch::RidgeRegion> referenceRegions;
    referenceRegions.reserve(references.size()/2);
    for(size_t i=0;i<references.size();i+=2)referenceRegions.push_back(vkmatch::extractRidgeRegion(references[i]));
    const auto probeRegion=vkmatch::extractRidgeRegion(probe);
    std::vector<vkmatch::Minutiae> referenceMinutiae;
    for(size_t i=0;i<references.size();i+=2)referenceMinutiae.push_back(vkmatch::extractMinutiae(references[i]));
    const auto probeMinutiae=vkmatch::extractMinutiae(probe);
    // The contact components are recomputed for THIS pair.  They are not
    // fingerprint core points and no pivot is learned from the first capture.
    // Anchoring removes the rotational arc created when an off-centre contact
    // is rotated about the physical image centre; dx/dy remain a bounded
    // residual search around that fresh pair-specific alignment.
    auto contactAnchoredJob=[&](uint32_t ref,float angle,float dx,float dy,float scale) {
        vkmatch::Job job{ref,uint32_t(references.size())+ref%2,angle,dx,dy,scale};
        const auto &referenceRegion=referenceRegions.at(ref/2);
        if(referenceRegion.usable&&probeRegion.usable) {
            job.pivotX=float(referenceRegion.centerX);job.pivotY=float(referenceRegion.centerY);
            job.anchorX=float(probeRegion.centerX);job.anchorY=float(probeRegion.centerY);
        }
        return job;
    };
    // Refinement begins from serialized selected candidates. Rehydrate their
    // pair-specific pivot/anchor rather than silently falling back to the
    // image centre after the first search pass.
    auto serializedJob=[&](uint32_t ref,const QJsonObject &row,float angle,float dx,float dy,float scale) {
        auto job=contactAnchoredJob(ref,angle,dx,dy,scale);
        if(row.value("transform_anchor").toString()!=QStringLiteral("contact-component-v1"))return job;
        const auto pivot=row.value("rotation_pivot").toArray(),anchor=row.value("rotation_anchor").toArray();
        if(pivot.size()==2&&anchor.size()==2) {
            job.pivotX=float(pivot.at(0).toDouble());job.pivotY=float(pivot.at(1).toDouble());
            job.anchorX=float(anchor.at(0).toDouble());job.anchorY=float(anchor.at(1).toDouble());
        }
        return job;
    };
    std::vector<vkmatch::Job> coarse;
    for(uint32_t ref=0;ref<references.size();++ref)
        for(int angle=-30;angle<=30;angle+=5)for(int dy=-24;dy<=24;dy+=4)for(int dx=-24;dx<=24;dx+=4)
            coarse.push_back(contactAnchoredJob(ref,float(angle),float(dx),float(dy),1));
    auto first=gpu.run(coarse,4);
    std::vector<std::vector<Ranked>> groups(references.size());
    for(size_t i=0;i<coarse.size();++i)groups[coarse[i].reference].push_back({coarse[i],first.scores[i]});
    std::vector<vkmatch::Job> fine;
    for(auto &group:groups) {
        std::sort(group.begin(),group.end(),[](const auto&a,const auto&b){return rankScore(a.score)>rankScore(b.score);});
        std::vector<Ranked> seeds;
        for(const auto &r:group) {
            if(rankScore(r.score)<=-2)continue;
            if(std::all_of(seeds.begin(),seeds.end(),[&](const auto&s){return distinct(s.job,r.job);}))seeds.push_back(r);
            if(seeds.size()==3)break;
        }
        for(const auto &r:seeds)for(int a=-2;a<=2;++a)for(int y=-2;y<=2;++y)for(int x=-2;x<=2;++x)for(float scale:fineScales) {
            auto j=r.job;j.angleDegrees+=a;j.dx+=x;j.dy+=y;j.scale=scale;fine.push_back(j);
        }
    }
    auto second=gpu.run(fine,2);
    for(auto &g:groups)g.clear();
    for(size_t i=0;i<fine.size();++i)groups[fine[i].reference].push_back({fine[i],second.scores[i]});
    // Only extend groups whose current best hits the original search boundary.
    std::vector<vkmatch::Job> extension;
    std::vector<bool> extended(groups.size(),false);
    for(size_t g=0;g<groups.size();++g) {
        const auto &rows=groups[g];if(rows.empty())continue;
        const auto best=std::max_element(rows.begin(),rows.end(),[](const auto&a,const auto&b){return rankScore(a.score)<rankScore(b.score);});
        if(!boundary(best->job))continue;
        extended[g]=true;
        for(int a=-8;a<=8;a+=4)for(int y=-8;y<=8;y+=4)for(int x=-8;x<=8;x+=4) {
            auto j=best->job;j.angleDegrees=std::clamp(j.angleDegrees+a,-40.f,40.f);
            j.dx=std::clamp(j.dx+x,-36.f,36.f);j.dy=std::clamp(j.dy+y,-36.f,36.f);extension.push_back(j);
        }
    }
    auto third=gpu.run(extension,2);
    for(size_t i=0;i<extension.size();++i)groups[extension[i].reference].push_back({extension[i],third.scores[i]});
    QJsonArray results;
    QJsonObject best;
    bool wideRotation=false;
    auto prefer=[&](const QJsonObject &a,const QJsonObject &b) {
        return authSearch?preferAuthCandidate(a,b):
            a.value("evidence_rank").toDouble(-10)>b.value("evidence_rank").toDouble(-10);
    };
    auto summarize=[&] {
    results=QJsonArray();best=QJsonObject();
    for(size_t g=0;g<groups.size();++g) {
        auto &rows=groups[g];std::sort(rows.begin(),rows.end(),[](const auto&a,const auto&b){return rankScore(a.score)>rankScore(b.score);});
        if(rows.empty())continue;
        // Confirm several distinct high-scoring alignments using interior tiles.
        QJsonObject selected;std::vector<vkmatch::Job> checked;
        for(const auto&r:rows) {
            if(!checked.empty()&&!std::all_of(checked.begin(),checked.end(),[&](const auto&j){return distinct(j,r.job);}))continue;
            const auto e=vkmatch::assessInterior(references[g-g%2],referenceRegions[g/2],probe,probeRegion,r.job);
            const double value=.55*rankScore(r.score)+.45*std::min(e.ncc,e.gradient)-.15*(1-e.support);
            QJsonObject item=describe(r);
            item.insert("interior",QJsonObject{{"ncc",e.ncc},{"gradient",e.gradient},
                {"overlap",e.support},{"informative_tiles",e.informativeTiles},
                {"supported_tiles",e.supportedTiles},{"probe_tiles",e.probeTiles},{"consistent",e.consistent},
                {"informative_mask",int(e.informativeMask)},{"supported_mask",int(e.supportedMask)}});
            QJsonObject ridgeRoi{{"version",1},{"binary",true},
                {"reference_coverage",e.referenceRoiCoverage},{"probe_coverage",e.probeRoiCoverage},
                {"overlap",e.roiOverlap},{"reference_mask",int(e.referenceRoiMask)},
                {"probe_mask",int(e.probeRoiMask)},{"overlap_mask",int(e.roiOverlapMask)},
                {"reference_center",QJsonArray{e.referenceRoiCenterX,e.referenceRoiCenterY}},
                {"probe_center",QJsonArray{e.probeRoiCenterX,e.probeRoiCenterY}}};
            item.insert("ridge_roi",ridgeRoi);
            const auto m=vkmatch::assessMinutiae(referenceMinutiae[g/2],probeMinutiae,r.job,probe.width,probe.height);
            item.insert("minutiae",QJsonObject{{"research_only",true},{"reference_count",m.referenceCount},
                {"probe_count",m.probeCount},{"matched",m.paired},{"type_agreement",m.typeAgreement},
                {"coverage",m.coverage},{"orientation_agreement",m.orientationAgreement},{"consistent",m.consistent}});
            item.insert("evidence_rank",value);item.insert("search_extended",bool(extended[g]));
            item.insert("reference",paths[int(g/2)]);item.insert("method",g%2?"local-normalized":"raw");
            item.insert("independent_peak_gap",QJsonValue::Null);
            for(size_t i=0;i<rows.size();++i)if(distinct(r.job,rows[i].job)) {
                item.insert("independent_peak_gap",rankScore(r.score)-rankScore(rows[i].score));break;
            }
            item.insert("ambiguous",item.value("independent_peak_gap").isNull()||item.value("independent_peak_gap").toDouble()<.02);
            item.insert("at_search_boundary",boundary(r.job,extended[g]?34:24,wideRotation?181:(extended[g]?38:30)));
            if(prefer(item,selected))selected=item;
            checked.push_back(r.job);if(checked.size()==3)break;
        }
        if(prefer(selected,best))best=selected;
        results.append(selected);
    }
    };
    summarize();
    const auto fastBest=best;
    const auto interior=best.value("interior").toObject();
    vkmatch::Batch rotationCoarse,rotationFine,rotationPolish;
    qint64 rotationHypotheses=0;
    // Fast path remains unchanged for strong, unambiguous interior evidence.
    // Weak/ambiguous candidates receive a bounded full-circle search instead
    // of accepting the best result from an inadequate +/-40 degree window.
    if(authSearch?!authCandidateAccepted(best):(!interior.value("consistent").toBool()||interior.value("gradient").toDouble()<.65||best.value("ambiguous").toBool())) {
        wideRotation=true;
        std::vector<vkmatch::Job> sweep;
        for(uint32_t ref=0;ref<references.size();++ref)
            for(int angle=-180;angle<180;angle+=10)for(int dy=-24;dy<=24;dy+=8)for(int dx=-24;dx<=24;dx+=8)
                sweep.push_back(contactAnchoredJob(ref,float(angle),float(dx),float(dy),1));
        rotationCoarse=gpu.run(sweep,4);
        std::vector<std::vector<Ranked>> wideGroups(references.size());
        for(size_t i=0;i<sweep.size();++i)wideGroups[sweep[i].reference].push_back({sweep[i],rotationCoarse.scores[i]});
        std::vector<vkmatch::Job> refine;
        for(auto &rows:wideGroups) {
            std::sort(rows.begin(),rows.end(),[](const auto&a,const auto&b){return rankScore(a.score)>rankScore(b.score);});
            std::vector<Ranked> seeds;
            for(const auto&r:rows) {
                if(rankScore(r.score)<=-2)continue;
                if(std::all_of(seeds.begin(),seeds.end(),[&](const auto&s){return distinct(s.job,r.job);}))seeds.push_back(r);
                if(seeds.size()==2)break;
            }
            for(const auto&r:seeds)for(int angle=-5;angle<=5;++angle)for(int y=-4;y<=4;y+=2)for(int x=-4;x<=4;x+=2)for(float scale:fineScales) {
                auto j=r.job;j.angleDegrees=std::remainder(j.angleDegrees+angle,360.f);j.dx+=x;j.dy+=y;j.scale=scale;refine.push_back(j);
            }
        }
        rotationFine=gpu.run(refine,2);
        for(auto &g:wideGroups)g.clear();
        for(size_t i=0;i<refine.size();++i) {
            const Ranked r{refine[i],rotationFine.scores[i]};
            groups[refine[i].reference].push_back(r);wideGroups[refine[i].reference].push_back(r);
        }
        // Recover subpixel precision after the deliberately coarse global sweep.
        std::vector<vkmatch::Job> polish;
        for(const auto&rows:wideGroups) {
            if(rows.empty())continue;
            const auto r=*std::max_element(rows.begin(),rows.end(),[](const auto&a,const auto&b){return rankScore(a.score)<rankScore(b.score);});
            for(int angle=-2;angle<=2;++angle)for(int y=-2;y<=2;++y)for(int x=-2;x<=2;++x) {
                auto j=r.job;j.angleDegrees=std::remainder(j.angleDegrees+angle*.5f,360.f);j.dx+=x*.5f;j.dy+=y*.5f;polish.push_back(j);
            }
        }
        rotationPolish=gpu.run(polish,2);
        for(size_t i=0;i<polish.size();++i)groups[polish[i].reference].push_back({polish[i],rotationPolish.scores[i]});
        rotationHypotheses=qint64(sweep.size()+refine.size()+polish.size());
        summarize();
        // Preserve the already verified incumbent if global GPU top-three
        // pruning would otherwise discard better interior evidence.
        if(prefer(fastBest,best) || (!authSearch &&
           (fastBest.value("interior").toObject().value("consistent").toBool() &&
            !best.value("interior").toObject().value("consistent").toBool()))) {
            best=fastBest;best.insert("retained_fast_candidate",true);
        }
    }
    // Optional bounded scale refinement on the strongest existing alignments.
    // Changes the geometric search, never the interior acceptance thresholds.
    bool scaleRefined=false;vkmatch::Batch scaleBatch;QJsonObject beforeScale=best;
    qint64 scaleHypotheses=0;
    if(refineScale && (!best.value("interior").toObject().value("consistent").toBool() ||
                      best.value("interior").toObject().value("gradient").toDouble()<.65)) {
        scaleRefined=true;
        std::vector<QJsonObject> seeds;
        for(const auto &r:results)seeds.push_back(r.toObject());
        // Include an incumbent retained from the fast path as well.
        seeds.push_back(best);
        std::sort(seeds.begin(),seeds.end(),[](const auto&a,const auto&b){return a.value("evidence_rank").toDouble()>b.value("evidence_rank").toDouble();});
        std::vector<vkmatch::Job> jobs;std::vector<int> used;
        for(const auto &r:seeds) {
            const int ri=paths.indexOf(r.value("reference").toString());if(ri<0)continue;
            const int group=2*ri+(r.value("method").toString()=="raw"?0:1);
            if(std::find(used.begin(),used.end(),group)!=used.end())continue;
            used.push_back(group);
            for(int sc=0;sc<=12;++sc)for(int a=-2;a<=2;++a)for(int y=-2;y<=2;++y)for(int x=-2;x<=2;++x)
                jobs.push_back(serializedJob(uint32_t(group),r,
                    float(std::remainder(r.value("angle").toDouble()+a,360.)),
                    float(r.value("dx").toDouble()+x),float(r.value("dy").toDouble()+y),.85f+.025f*sc));
            if(used.size()==6)break;
        }
        scaleBatch=gpu.run(jobs,2);scaleHypotheses=jobs.size();
        for(size_t i=0;i<jobs.size();++i)groups[jobs[i].reference].push_back({jobs[i],scaleBatch.scores[i]});
        summarize();
        if(prefer(beforeScale,best) || (!authSearch &&
           (beforeScale.value("interior").toObject().value("consistent").toBool() &&
            !best.value("interior").toObject().value("consistent").toBool()))) {
            best=beforeScale;best.insert("retained_prescale_candidate",true);
        }
    }
    // A bounded two-level, subpixel geometric retry. No local elastic
    // warps, synthetic ridges, threshold reduction, or learning from rejects.
    // Preserve any already-passing candidate across pruning/re-summarization.
    const auto beforePrecision=best;
    qint64 precisionHypotheses=0;double precisionGpu=0;
    for(int level=0;authSearch&&!authCandidateAccepted(best)&&level<2;++level) {
        std::vector<QJsonObject> seeds;
        for(const auto &r:results)seeds.push_back(r.toObject());
        seeds.push_back(best);
        std::sort(seeds.begin(),seeds.end(),preferAuthCandidate);
        std::vector<vkmatch::Job> jobs;std::vector<int> used;
        const float step=level==0?.5f:.25f,scaleStep=level==0?.02f:.005f;
        for(const auto &r:seeds) {
            const int ri=paths.indexOf(r.value("reference").toString());if(ri<0)continue;
            const int group=2*ri+(r.value("method").toString()=="raw"?0:1);
            if(std::find(used.begin(),used.end(),group)!=used.end())continue;
            used.push_back(group);
            for(int sc=-2;sc<=2;++sc)for(int a=-2;a<=2;++a)for(int y=-2;y<=2;++y)for(int x=-2;x<=2;++x)
                jobs.push_back(serializedJob(uint32_t(group),r,
                    float(std::remainder(r.value("angle").toDouble()+a*step,360.)),
                    float(r.value("dx").toDouble()+x*step),float(r.value("dy").toDouble()+y*step),
                    authSearch?std::clamp(float(r.value("scale").toDouble(1)+sc*scaleStep),authScaleMin,authScaleMax):
                    float(r.value("scale").toDouble(1)+sc*scaleStep)));
            if(used.size()==8)break;
        }
        // Existing scores used stride 2; rescore the bounded retry at stride 2
        // too so peak-gap comparisons have identical sampling semantics.
        const auto batch=gpu.run(jobs,2);precisionHypotheses+=jobs.size();
        precisionGpu=precisionGpu>=0&&batch.gpuMs>=0?precisionGpu+batch.gpuMs:-1;
        for(size_t i=0;i<jobs.size();++i)groups[jobs[i].reference].push_back({jobs[i],batch.scores[i]});
        const auto incumbent=best;summarize();
        if(prefer(incumbent,best))best=incumbent;
    }
    const double rotationGpu=wideRotation?(rotationCoarse.gpuMs>=0&&rotationFine.gpuMs>=0&&rotationPolish.gpuMs>=0?rotationCoarse.gpuMs+rotationFine.gpuMs+rotationPolish.gpuMs:-1):0;
    return {{"ok",true},{"algorithm_version",authSearch?8:(refineScale?4:3)},
        {"auth_profile",authSearch?QStringLiteral("auth-v8-contact-anchored-ridge-roi-uniform-scale-5pct"):QString()},
        {"research_only",true},{"authentication_decision",QJsonValue::Null},
        {"auth_search",authSearch},{"auth_scale_min",authSearch?QJsonValue(authScaleMin):QJsonValue::Null},
        {"auth_scale_max",authSearch?QJsonValue(authScaleMax):QJsonValue::Null},
        {"minutiae_pipeline",QStringLiteral("research-v1-not-an-authentication-gate")},
        {"precision_hypotheses",precisionHypotheses},{"before_precision",beforePrecision},
        {"device",QString::fromStdString(gpu.deviceName())},{"probe",probePath},{"references",paths.size()},
        {"hypotheses",qint64(coarse.size()+fine.size()+extension.size())+rotationHypotheses+scaleHypotheses+precisionHypotheses},{"extension_hypotheses",qint64(extension.size())},
        {"scale_refined",scaleRefined},{"scale_hypotheses",scaleHypotheses},{"before_scale",beforeScale},
        {"wide_rotation",wideRotation},{"rotation_hypotheses",rotationHypotheses},{"fast_best",fastBest},{"wall_ms",wall.nsecsElapsed()/1e6},
        {"gpu_ms",first.gpuMs>=0&&second.gpuMs>=0&&third.gpuMs>=0&&rotationGpu>=0&&precisionGpu>=0&&(!scaleRefined||scaleBatch.gpuMs>=0)?first.gpuMs+second.gpuMs+third.gpuMs+rotationGpu+precisionGpu+(scaleRefined?scaleBatch.gpuMs:0):-1},
        {"best",best},{"results",results}};
}
void emitJson(const QJsonObject &o) {std::cout<<QJsonDocument(o).toJson(QJsonDocument::Compact).constData()<<std::endl;}
}
int main(int argc,char**argv) {
    QCoreApplication app(argc,argv);
    try {
        QStringList refs,probes;bool serve=false,refineScale=false,authSearch=false;QString fastBank;QString shader=QCoreApplication::applicationDirPath()+"/fpstudio-match.spv";
        if(!QFileInfo::exists(shader))shader=QStringLiteral(VKMATCH_SHADER_PATH);
        const auto args=app.arguments();
        for(int i=1;i<args.size();++i) {
            if(args[i]=="--serve")serve=true;
            else if(args[i]=="--scale-refinement")refineScale=true;
            else if(args[i]=="--auth-search")authSearch=true;
            else if(args[i]=="--fast-reference-bank"&&i+1<args.size())fastBank=args[++i];
            else if(args[i]=="--reference"&&i+1<args.size())refs<<args[++i];
            else if(args[i]=="--probe"&&i+1<args.size())probes<<args[++i];
            else if(args[i]=="--shader"&&i+1<args.size())shader=args[++i];
            else if(args[i]=="--reference-dir"&&i+1<args.size()) {
                QDir dir(args[++i]);for(const auto &file:dir.entryInfoList({"candidate-touch-*.png","reference-index-*.png"},QDir::Files,QDir::Name))refs<<file.absoluteFilePath();
            } else throw std::runtime_error("usage: fpstudio-vkmatch --reference FILE [--reference FILE ...] --probe FILE | --reference-dir DIR --serve [--shader FILE]");
        }
        refs.removeDuplicates();
        if(authSearch&&!fastBank.isEmpty())throw std::runtime_error("Auth search requires the full bank, not the research fast cascade");
        if(refs.isEmpty()||refs.size()>64)throw std::runtime_error("Need 1..64 reference images; split larger studies into batches");
        if(probes.size()>64)throw std::runtime_error("At most 64 probes per CLI batch");
        std::vector<vkmatch::Image> bank;
        for(auto &path:refs) {
            path=QFileInfo(path).canonicalFilePath();if(path.isEmpty())throw std::runtime_error("Reference does not exist");
            const auto im=loadImage(path);bank.push_back(im);bank.push_back(normalize(im));
        }
        QStringList fastPaths;std::vector<vkmatch::Image> fastImages;
        QString fastManifestHash;
        if(!fastBank.isEmpty()) {
            QDir directory(fastBank);QFile manifest(directory.filePath("manifest.json"));
            if(!manifest.open(QIODevice::ReadOnly)||manifest.size()>2*1024*1024)throw std::runtime_error("Cannot read bounded fast-bank manifest");
            const auto bytes=manifest.readAll();const auto metadata=QJsonDocument::fromJson(bytes).object();
            if(metadata.value("schema_version").toInt()!=1||!metadata.value("research_only").toBool())throw std::runtime_error("Unsupported fast-bank manifest");
            fastManifestHash=QString::fromLatin1(QCryptographicHash::hash(bytes,QCryptographicHash::Sha256).toHex());
            const auto entries=metadata.value("references").toArray();
            if(entries.isEmpty()||entries.size()>64)throw std::runtime_error("Fast bank needs 1..64 entries");
            auto digest=[](const QString &path){QFile file(path);if(!file.open(QIODevice::ReadOnly)||file.size()>1024*1024)throw std::runtime_error("Cannot hash reference");return QString::fromLatin1(QCryptographicHash::hash(file.readAll(),QCryptographicHash::Sha256).toHex());};
            QStringList hashes;for(const auto &path:refs)hashes<<digest(path);
            for(const auto &entry:entries) {
                const auto item=entry.toObject();const auto name=item.value("file").toString();
                if(name.isEmpty()||name=="."||name==".."||name.contains('/')||name.contains('\\'))throw std::runtime_error("Unsafe fast-bank filename");
                const QFileInfo file(directory.filePath(name));
                if(file.isSymLink()||!file.isFile())throw std::runtime_error("Fast-bank image must be a regular file");
                const auto hash=digest(file.absoluteFilePath());
                if(hash!=item.value("sha256").toString())throw std::runtime_error("Fast-bank checksum mismatch");
                const int index=hashes.indexOf(hash);
                if(index<0||fastPaths.contains(refs[index]))throw std::runtime_error("Fast bank must be a unique subset of the full bank");
                fastPaths<<refs[index];fastImages.push_back(bank[2*index]);fastImages.push_back(bank[2*index+1]);
            }
        }
        vkmatch::Matcher matcher(shader.toStdString());
        auto run=[&](const QString &path) {
            const auto canonical=QFileInfo(path).canonicalFilePath();
            if(canonical.isEmpty()||refs.contains(canonical))throw std::runtime_error("Probe must exist and must not be a reference file");
            if(fastPaths.isEmpty())return evaluate(matcher,refs,bank,canonical,refineScale,authSearch);
            QElapsedTimer elapsed;elapsed.start();
            auto fast=evaluate(matcher,fastPaths,fastImages,canonical,refineScale,false);
            const auto best=fast.value("best").toObject(),evidence=best.value("interior").toObject();
            const bool strong=evidence.value("consistent").toBool()&&evidence.value("gradient").toDouble()>=.65&&
                !best.value("ambiguous").toBool()&&!best.value("at_search_boundary").toBool();
            auto result=strong?fast:evaluate(matcher,refs,bank,canonical,refineScale,false);
            result.insert("algorithm_version",5);result.insert("search_policy","training-cascade-v1");
            result.insert("full_bank_fallback",!strong);result.insert("fast_bank_size",fastPaths.size());
            result.insert("full_bank_size",refs.size());result.insert("fast_bank_manifest_sha256",fastManifestHash);
            result.insert("fast_wall_ms",fast.value("wall_ms"));
            result.insert("wall_ms",elapsed.nsecsElapsed()/1e6);
            if(!strong) {
                result.insert("fast_best",fast.value("best"));
                result.insert("hypotheses",result.value("hypotheses").toInteger()+fast.value("hypotheses").toInteger());
                const double a=fast.value("gpu_ms").toDouble(-1),b=result.value("gpu_ms").toDouble(-1);
                result.insert("gpu_ms",a>=0&&b>=0?a+b:-1);
            }
            return result;
        };
        if(serve) {
            emitJson({{"ready",true},{"device",QString::fromStdString(matcher.deviceName())},{"references",refs.size()},{"fast_references",fastPaths.size()}});
            std::string line;
            while(std::getline(std::cin,line)) {
                try {
                    QJsonParseError error;const auto doc=QJsonDocument::fromJson(QByteArray::fromStdString(line),&error);
                    if(error.error!=QJsonParseError::NoError||!doc.isObject())throw std::runtime_error("Expected JSON object with probe path");
                    emitJson(run(doc.object().value("probe").toString()));
                } catch(const std::exception&e) {emitJson({{"ok",false},{"error",e.what()}});}
            }
        } else {
            if(probes.isEmpty())throw std::runtime_error("Need --probe FILE");
            for(const auto &probe:probes)emitJson(run(probe));
        }
    } catch(const std::exception&e){emitJson({{"ok",false},{"error",e.what()}});return 1;}
}
