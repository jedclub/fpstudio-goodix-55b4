#include "setupcheck.h"
#include <QCoreApplication>
int main(int argc,char **argv) {
    QCoreApplication app(argc,argv);
    using namespace fpstudio;
    if(allReady({}))return 1;
    QVector<StepResult> steps;
    for(auto id:{StepId::Device,StepId::Driver,StepId::UdevRule,StepId::TlsSession,StepId::Psk,
        StepId::Capture,StepId::Enrolment,StepId::GpuAuth,StepId::PamSudo,StepId::PamPolkit,StepId::PamKde}) {
        StepResult row{id};row.state=StepState::Ok;steps<<row;
    }
    if(!allReady(steps))return 1;
    steps[8].state=StepState::Skipped;if(allReady(steps))return 1;
    parseCaptureOutput("USB error: Input/Output Error; Checking PSK");
    const auto unknown=probe(StepId::Psk);
    if(unknown.irreversible||!unknown.action.isEmpty()||unknown.state!=StepState::Unknown)return 1;
    parseCaptureOutput("SWITCH TO FDT MODE");
    if(probe(StepId::Psk).state!=StepState::Ok||probe(StepId::TlsSession).state!=StepState::Ok)return 1;
    return 0;
}
