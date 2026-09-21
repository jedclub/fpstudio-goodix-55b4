#pragma once
#include <QJsonObject>
#include <cmath>

// Experimental deployment policy, deliberately distinct from the research
// overlay. These bounds have NOT been calibrated as a biometric FAR claim.

// Correlation floor for accepting a placement.
//
// This was .70, which is well inside the range other fingers reach. Measured
// on a 44-reference gallery with 25 probes of the enrolled finger and 10 of
// other fingers of the same hand, the two groups do not overlap: the enrolled
// finger never scored below .874 and the other fingers never above .842. A
// floor anywhere in .855-.870 took every enrolled probe and no other finger;
// .70 took 6 of the 10 other fingers as well. .86 is the middle of that band.
//
// Raising the gradient floor alongside it would only cost recognition: on the
// same data the enrolled finger's gradient runs as low as .794 while other
// fingers reach .841, so that axis does not separate and is left at .65.
//
// Ten other fingers of one person is not a false-accept rate and the band is
// only .032 wide, so this is a bound that held on the data measured, not a
// validated FAR. Re-measure it when the gallery is rebuilt.
inline constexpr double kAuthMinNcc = .86;
inline bool authCandidateAccepted(const QJsonObject &b) {
    const auto e=b.value("interior").toObject();
    auto atLeast=[](const QJsonObject &o,const char *key,double minimum) {
        const auto v=o.value(key);
        return v.isDouble()&&std::isfinite(v.toDouble())&&v.toDouble()>=minimum&&v.toDouble()<=1.;
    };
    return b.value("ambiguous").isBool()&&!b.value("ambiguous").toBool()&&
        b.value("at_search_boundary").isBool()&&!b.value("at_search_boundary").toBool()&&
        e.value("consistent").toBool()&&atLeast(e,"ncc",kAuthMinNcc)&&atLeast(e,"gradient",.65)&&
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
