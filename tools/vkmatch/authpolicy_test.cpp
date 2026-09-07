#include "authpolicy.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <cstdio>
int main(int argc,char **argv) {
    QCoreApplication app(argc,argv);
    QJsonObject good{{"ok",true},{"best",QJsonObject{{"ambiguous",false},{"at_search_boundary",false},
        {"interior",QJsonObject{{"consistent",true},{"ncc",.8},{"gradient",.8},{"overlap",.6},
            {"supported_tiles",10},{"probe_tiles",10}}}}}};
    if(!authPatternAccepted(good)||authPatternAccepted({}))return 1;
    for(const auto *key:{"ncc","gradient","overlap"}) {
        auto b=good["best"].toObject();auto e=b["interior"].toObject();e[key]=.1;b["interior"]=e;
        auto bad=good;bad["best"]=b;if(authPatternAccepted(bad))return 1;
        e.remove(key);b["interior"]=e;bad["best"]=b;if(authPatternAccepted(bad))return 1;
    }
    for(const auto *key:{"ambiguous","at_search_boundary"}) {
        auto bad=good;auto b=bad["best"].toObject();b[key]=true;bad["best"]=b;
        if(authPatternAccepted(bad))return 1;
        b.remove(key);bad["best"]=b;if(authPatternAccepted(bad))return 1;
    }
    auto passing=good["best"].toObject();passing["evidence_rank"]=.75;
    auto misleading=passing;misleading["ambiguous"]=true;misleading["evidence_rank"]=.95;
    if(!preferAuthCandidate(passing,misleading)||preferAuthCandidate(misleading,passing))return 1;
    auto sparse=passing;auto interior=sparse["interior"].toObject();interior["supported_tiles"]=7;
    sparse["interior"]=interior;sparse["evidence_rank"]=.99;
    if(!preferAuthCandidate(passing,sparse)||authCandidateAccepted(sparse))return 1;
    if(preferAuthCandidate({},passing)||!preferAuthCandidate(passing,{}))return 1;
    auto better=passing;better["evidence_rank"]=.85;
    if(!preferAuthCandidate(better,passing)||preferAuthCandidate(passing,better))return 1;
    for(const auto *key:{"supported_tiles","probe_tiles"}) {
        for(int n:{-1,7,17}) {
            auto b=passing;auto e=b["interior"].toObject();e[key]=n;b["interior"]=e;
            if(authCandidateAccepted(b))return 1;
        }
    }
    // Optional read-only replay of recorded GPU results through this policy.
    for(int i=1;i<argc;++i){QFile f(QString::fromLocal8Bit(argv[i]));if(!f.open(QIODevice::ReadOnly))return 2;
        const auto row=QJsonDocument::fromJson(f.readAll()).object();
        std::printf("contact=%d accepted=%d\n",row["contact_group"].toInt(),authPatternAccepted(row));}
    return 0;
}
