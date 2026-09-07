#pragma once
#include <QJsonObject>
#include <cmath>

// Experimental deployment policy, deliberately distinct from the research
// overlay. These bounds have NOT been calibrated as a biometric FAR claim.
inline bool authCandidateAccepted(const QJsonObject &b) {
    const auto e=b.value("interior").toObject();
    auto atLeast=[](const QJsonObject &o,const char *key,double minimum) {
        const auto v=o.value(key);
        return v.isDouble()&&std::isfinite(v.toDouble())&&v.toDouble()>=minimum&&v.toDouble()<=1.;
    };
    return b.value("ambiguous").isBool()&&!b.value("ambiguous").toBool()&&
        b.value("at_search_boundary").isBool()&&!b.value("at_search_boundary").toBool()&&
        e.value("consistent").toBool()&&atLeast(e,"ncc",.70)&&atLeast(e,"gradient",.65)&&
        atLeast(e,"overlap",.50)&&e.value("supported_tiles").toInt()>=8&&
        e.value("supported_tiles").toInt()<=16&&e.value("probe_tiles").toInt()>=8&&
        e.value("probe_tiles").toInt()<=16;
}

inline bool authPatternAccepted(const QJsonObject &result) {
    return result.value("ok").toBool() && authCandidateAccepted(result.value("best").toObject());
}

// Ranking is not an acceptance test: a higher aggregate score can have an
// ambiguous alignment or inadequate coverage while another candidate passes.
// Apply the SAME policy to each bounded search candidate before tie-breaking.
inline bool preferAuthCandidate(const QJsonObject &candidate,const QJsonObject &incumbent) {
    if(candidate.isEmpty())return false;
    if(incumbent.isEmpty())return true;
    const bool a=authCandidateAccepted(candidate),b=authCandidateAccepted(incumbent);
    if(a!=b)return a;
    return candidate.value("evidence_rank").toDouble(-10)>incumbent.value("evidence_rank").toDouble(-10);
}
