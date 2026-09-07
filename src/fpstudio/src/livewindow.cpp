#include "livewindow.h"
#include "frameselector.h"
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QProcessEnvironment>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <QSaveFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHBoxLayout>
#include <QPainter>
#include <cmath>

namespace fpstudio {
LiveWindow::LiveWindow(const QString &directory, QWidget *parent,
                       const QString &program, int prepareMs, const QString &referenceDir, bool differentFinger,
                       const QString &matcherProgram, const QString &fastBank, int targetContacts)
    : QDialog(parent), m_dir(directory),
      m_program(program.isEmpty() ? QCoreApplication::applicationFilePath() : program),
      m_prepareMs(prepareMs)
{
    m_targetContacts=qBound(0,targetContacts,128);
    m_differentFinger=differentFinger;
    m_fingerPrompt=differentFinger?QStringLiteral("방금과 다른 손가락"):
        QStringLiteral("지금까지 사용한 같은 손가락");
    setWindowTitle(QStringLiteral("fpstudio · 실시간 지문 영상"));
    resize(1050, 920);
    setStyleSheet("QDialog {background:#101827;} QLabel {color:#e5edf8;} QPushButton {padding:12px 28px;font-size:18px;}");
    auto *layout = new QVBoxLayout(this);
    m_instruction = new QLabel(QStringLiteral("손가락을 센서에서 떼 주세요. 곧 자동으로 보정합니다."));
    m_instruction->setWordWrap(true);
    m_instruction->setStyleSheet("font-size:25px;font-weight:bold;padding:18px;background:#1d4ed8;border-radius:8px;");
    auto *detail = new QLabel(QStringLiteral("보정 후 같은 손가락을 가볍게 올리고 안내를 따라 주세요. 접촉별 후보를 자동 비교합니다.\n"
        "실제 센서 영상 108 × 88 · 배경 차감/명암 정규화 · 인증 판정 아님 · 최대 3분"));
    detail->setWordWrap(true); detail->setStyleSheet("font-size:16px;");
    if(m_differentFinger)detail->setText(QStringLiteral("다른 손가락 비교 시험입니다. 방금 사용한 손가락은 이번에는 대지 마세요.\n"
        "보정 후 다른 손가락 하나로 가볍게 대었다 떼며 반복하세요. 높은 점수도 인증 승인을 뜻하지 않습니다."));
    m_image = new QLabel(QStringLiteral("보정 대기 중 — 아직 실시간 영상이 없습니다"));
    m_image->setMinimumSize(432, 352); m_image->setAlignment(Qt::AlignCenter);
    m_image->setStyleSheet("background:#050a11;border:1px solid #344155;");
    m_metrics = new QLabel(QStringLiteral("0 FPS · 새 프레임 대기"));
    m_metrics->setStyleSheet("font-size:18px;");
    m_matchLabel = new QLabel(QStringLiteral("GPU 비교: 기준 영상 미지정 · 실시간 수집만 진행"));
    m_matchLabel->setWordWrap(true); m_matchLabel->setStyleSheet("font-size:16px;color:#a5d7ff;");
    m_matchLabel->setMinimumHeight(150);
    m_stop = new QPushButton(QStringLiteral("중단"));
    m_matchImage=new QLabel(QStringLiteral("비교 영상 대기\n초록: 내부 무늬 지지 · 빨강: 근거 부족\n파랑 테두리: 비교 범위\n실시간 영상이 아닌 비교 당시의 정지 영상"));
    m_matchImage->setAlignment(Qt::AlignCenter);m_matchImage->setMinimumSize(432,352);
    m_matchImage->setStyleSheet("background:#050a11;border:1px solid #344155;font-size:15px;");
    auto *images=new QHBoxLayout;images->addWidget(m_image,1);images->addWidget(m_matchImage,1);
    layout->addWidget(m_instruction); layout->addWidget(detail); layout->addLayout(images, 1);
    layout->addWidget(m_metrics); layout->addWidget(m_matchLabel); layout->addWidget(m_stop, 0, Qt::AlignRight);
    connect(m_stop, &QPushButton::clicked, this, [this]{stop();});
    m_lock = std::make_unique<QLockFile>(QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation) + "/fpstudio-research.lock");
    if (!m_lock->tryLock() || !QDir().mkpath(m_dir) ||
        !QFile::setPermissions(m_dir, QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner)) {
        m_instruction->setText(QStringLiteral("시작하지 못했습니다. 다른 센서 세션 또는 저장 폴더 권한을 확인하세요."));
        m_finished = true; return;
    }
    auto env = QProcessEnvironment::systemEnvironment();
    env.insert("FPSTUDIO_STATUS_PATH", m_dir + "/capture-status.json");
    env.insert("GOODIX55X4_PREVIEW_PATH", m_dir + "/live.pgm");
    m_worker.setProcessEnvironment(env);
    // Drain output throughout the stream; biometric images stay in the private PGM.
    connect(&m_worker, &QProcess::readyReadStandardOutput, this, [this] { m_result += m_worker.readAllStandardOutput(); });
    connect(&m_worker, &QProcess::readyReadStandardError, this, [this] { m_worker.readAllStandardError(); });
    connect(&m_worker, &QProcess::errorOccurred, this, [this](QProcess::ProcessError e) {
        if (e == QProcess::FailedToStart) {
            m_finished = true;
            m_instruction->setText(QStringLiteral("캡처 프로그램을 실행하지 못했습니다: ") + m_worker.errorString());
            m_stop->setText(QStringLiteral("닫기"));
            finishMatching();
        }
    });
    connect(&m_worker, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, [this](int, QProcess::ExitStatus) {
        m_finished = true;
        m_instruction->setText(m_stopping ? QStringLiteral("중단했습니다. 센서에서 손을 떼셔도 됩니다.")
             : QStringLiteral("실시간 영상이 종료되었습니다. 센서에서 손을 떼 주세요."));
        m_metrics->setText(QStringLiteral("0 FPS · 종료 · 마지막 영상은 정지 화면입니다 · 총 %1 프레임").arg(m_total));
        m_stop->setText(QStringLiteral("닫기"));
        QSaveFile result(m_dir + "/capture-result.json");
        if (result.open(QIODevice::WriteOnly)) {
            result.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
            result.write(m_result); result.commit();
        }
        finishMatching();
    });
    m_clock.start(); m_windowStart = 0;
    if (!referenceDir.isEmpty()) startMatcher(referenceDir, matcherProgram, fastBank);
    connect(&m_tick, &QTimer::timeout, this, &LiveWindow::tick);
    m_tick.start(30);
}
LiveWindow::~LiveWindow()
{
    if (m_matcher.state() != QProcess::NotRunning) { m_matcher.kill(); m_matcher.waitForFinished(2000); }
    if (m_worker.state() != QProcess::NotRunning) { m_worker.kill(); m_worker.waitForFinished(2000); }
}
void LiveWindow::startMatcher(const QString &referenceDir, const QString &program, const QString &fastBank)
{
    m_matchingEnabled=true;
    m_matchLabel->setText(tr("Vulkan GPU is initializing — it calculates separately from the live stream"));
    connect(&m_matcher, &QProcess::readyReadStandardError, this, [this] { m_matcher.readAllStandardError(); });
    connect(&m_matcher, &QProcess::readyReadStandardOutput, this, [this] {
        m_matchOutput += m_matcher.readAllStandardOutput();
        int end;
        while ((end=m_matchOutput.indexOf('\n'))>=0) {
            auto object=QJsonDocument::fromJson(m_matchOutput.left(end)).object();m_matchOutput.remove(0,end+1);
            if(object.value("ready").toBool()) {
                m_gpuReady=true;
                m_matchLabel->setText(tr("%1 · %2 references ready · waiting for a stable candidate")
                    .arg(object.value("device").toString()).arg(object.value("references").toInt()));
                dispatchMatch();
                continue;
            }
            if(!m_gpuBusy)continue;
            object.insert("contact_group",m_gpuContact);object.insert("sensor_timestamp_us",m_gpuStamp);
            object.insert("selection_kind",m_gpuSelection);
            object.insert("instructed_role",m_differentFinger?"different-finger":"unspecified");
            m_gpuBusy=false;m_gpuStatus=object;
            auto &contact=m_contacts[m_gpuContact];
            contact.insert("completed",contact.value("completed").toInt()+1);
            ++m_gpuCompleted;
            const auto interiorResult=object.value("best").toObject().value("interior").toObject();
            contact.insert("consistent",contact.value("consistent").toBool() ||
                (object.value("ok").toBool() && interiorResult.value("consistent").toBool()));
            contact.insert("last_result",object.value("ok").toBool()?"compared":"error");
            if(!object.value("ok").toBool()) {
                ++m_gpuErrors;contact.insert("error",object.value("error"));
            } else contact.insert("compared",true);
            QSaveFile record(m_dir+QStringLiteral("/gpu-match-%1.json").arg(m_gpuRequest,4,10,QLatin1Char('0')));
            if(record.open(QIODevice::WriteOnly)) {
                record.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner);
                record.write(QJsonDocument(object).toJson());
                if(!record.commit())contact.insert("record_error",true);
            } else contact.insert("record_error",true);
            if(!object.value("ok").toBool()) {
                m_matchLabel->setText(tr("GPU comparison error: ")+object.value("error").toString());
                dispatchMatch();finishMatching();continue;
            }
            const auto best=object.value("best").toObject();
            showMatchOverlay(object);
            m_matchLabel->setText(tr("GPU %1 hypotheses · %2 ms · image correlation %3 · ridge-direction agreement %4\nBest: %5 / %6 — research comparison only; not an authentication decision")
                .arg(object.value("hypotheses").toInteger()).arg(object.value("wall_ms").toDouble(),0,'f',0)
                .arg(best.value("ncc").toDouble(),0,'f',3).arg(best.value("gradient").toDouble(),0,'f',3)
                .arg(QFileInfo(best.value("reference").toString()).fileName()).arg(best.value("method").toString())
                +(best.value("ambiguous").toBool()?tr("\nAlignment is ambiguous: a different position has a similar score"):QString())
                +(best.value("at_search_boundary").toBool()?tr("\nAt the search boundary: move the finger slightly toward the centre"):QString()));
            const auto interior=best.value("interior").toObject();
            if(!interior.isEmpty())m_matchLabel->setText(m_matchLabel->text()+
                tr("\nInterior ridge support %1/%2 regions · %3")
                    .arg(interior.value("supported_tiles").toInt()).arg(interior.value("informative_tiles").toInt())
                    .arg(interior.value("consistent").toBool()?tr("ridges are consistent (identity unverified)"):tr("insufficient match evidence")));
            const auto ridgeRoi=best.value("ridge_roi").toObject();
            if(ridgeRoi.value("binary").toBool())m_matchLabel->setText(m_matchLabel->text()+
                tr(" · 0/1 contact-region overlap %1%")
                    .arg(ridgeRoi.value("overlap").toDouble()*100,0,'f',0));
            if(object.value("wide_rotation").toBool())m_matchLabel->setText(m_matchLabel->text()+tr(" · 360° additional search applied"));
            if(object.contains("full_bank_fallback"))m_matchLabel->setText(m_matchLabel->text()+
                (object.value("full_bank_fallback").toBool()?tr(" · full reference-bank search"):tr(" · representative-reference fast comparison")));
            int compared=0;for(const auto &c:m_contacts)if(c.value("compared").toBool())++compared;
            if(m_targetContacts>0&&compared>=m_targetContacts&&!m_stopping){stop(true);finishMatching();continue;}
            dispatchMatch();finishMatching();
        }
    });
    connect(&m_matcher,&QProcess::errorOccurred,this,[this](QProcess::ProcessError error) {
        // QProcess reports our deliberate kill() as Crashed. It is a
        // cancellation, not an initialization failure or a failed comparison.
        if(m_gpuStopExpected&&error==QProcess::Crashed) {
            m_gpuReady=false;m_gpuBusy=false;finishMatching();return;
        }
        // Preserve a specific error already recorded (e.g. timeout).
        if(m_gpuFailed){m_gpuReady=false;m_gpuBusy=false;finishMatching();return;}
        failPending("matcher-error");
        m_gpuReady=false;m_gpuBusy=false;
        m_matchLabel->setText(tr("GPU comparison cannot run: ")+m_matcher.errorString());
        finishMatching();
    });
    connect(&m_matcher,qOverload<int,QProcess::ExitStatus>(&QProcess::finished),this,[this](int code,QProcess::ExitStatus exit) {
        if(!m_gpuStopExpected&&!m_gpuFailed&&(m_gpuBusy||!m_pending.isEmpty()||!m_finished||code!=0||exit!=QProcess::NormalExit)) {
            failPending("matcher-exited");
            m_matchLabel->setText(QStringLiteral("GPU 비교 작업자가 예기치 않게 종료됐습니다 (코드 %1)").arg(code));
        }
        m_gpuReady=false;m_gpuBusy=false;
        finishMatching();
    });
    QStringList args{"--reference-dir",referenceDir,"--serve"};
    if(!fastBank.isEmpty()) {
        // The fast bank is explicitly a research cascade and is incompatible
        // with the full-bank auth-search bounds. Keep that experiment mode
        // separate rather than silently weakening a requested v8 comparison.
        args<<"--fast-reference-bank"<<fastBank;
    } else {
        // Live full-bank research must exercise the same bounded v8 geometry
        // (contact anchor, rotation and +/-5% uniform scale) shown in its UI.
        // The matcher still emits research_only/null authentication_decision.
        args<<"--auth-search";
    }
    m_matcher.start(program.isEmpty()?QCoreApplication::applicationDirPath()+"/fpstudio-vkmatch":program,args);
    QTimer::singleShot(30000,this,[this] {
        if(!m_gpuReady&&!m_gpuFailed&&m_matcher.state()!=QProcess::NotRunning) {
            failPending("matcher-startup-timeout");m_gpuBusy=false;m_matcher.kill();finishMatching();
        }
    });
}
void LiveWindow::matchFrame(const QImage &frame, const QString &kind)
{
    auto &contact=m_contacts[m_touch];
    if(!m_matchingEnabled){contact.insert("skip_reason","no-reference-bank");return;}
    if(m_gpuFailed){contact.insert("skip_reason","matcher-unavailable");return;}
    // Keep one newest best candidate per contact, including while GPU starts
    // or is busy. Never substitute the current touch ID when dispatching later.
    if(m_gpuRequest>=384) {contact.insert("skip_reason","session-comparison-limit");return;}
    if(contact.value("submitted").toInt()>=3) {contact.insert("skip_reason","contact-comparison-limit");return;}
    m_pending.insert(m_touch,{frame,m_lastStamp,kind});
    m_lastMatchCandidate=frame;
    dispatchMatch();
}
void LiveWindow::dispatchMatch()
{
    if(!m_gpuReady||m_gpuBusy||m_stopping||m_pending.isEmpty())return;
    if(m_gpuRequest>=384){failPending("comparison-limit");return;}
    const auto item=m_pending.begin();
    const int contactId=item.key();const auto pending=item.value();m_pending.erase(item);
    auto &contact=m_contacts[contactId];
    const QString path=m_dir+QStringLiteral("/gpu-probe-%1.png").arg(m_gpuRequest+1,4,10,QLatin1Char('0'));
    QSaveFile file(path);
    if(!file.open(QIODevice::WriteOnly)||!file.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner)||
       !pending.image.save(&file,"PNG")||!file.commit()) {
        contact.insert("skip_reason","probe-save-error");
        QTimer::singleShot(0,this,[this]{dispatchMatch();finishMatching();});return;
    }
    m_gpuBusy=true;const int request=++m_gpuRequest;
    m_gpuContact=contactId;m_gpuStamp=pending.stamp;
    m_gpuSelection=pending.kind;
    contact.insert("submitted",contact.value("submitted").toInt()+1);
    m_matchLabel->setText(tr("Vulkan GPU is comparing reference images, angles, and positions in parallel…"));
    m_matcher.write(QJsonDocument(QJsonObject{{"probe",path}}).toJson(QJsonDocument::Compact)+"\n");
    QTimer::singleShot(30000,this,[this,request] {
        if(m_gpuBusy&&request==m_gpuRequest) {
            failPending("matcher-timeout");m_matcher.kill();m_gpuReady=false;m_gpuBusy=false;
            m_matchLabel->setText(tr("GPU comparison timed out — live collection continues"));
            finishMatching();
        }
    });
}
void LiveWindow::failPending(const QString &reason)
{
    m_gpuFailed=true;
    if(m_gpuBusy) {
        m_contacts[m_gpuContact].insert("error",reason);
        ++m_gpuErrors;
    }
    for(auto i=m_pending.cbegin();i!=m_pending.cend();++i)m_contacts[i.key()].insert("skip_reason",reason);
    m_pending.clear();
}
void LiveWindow::showMatchOverlay(const QJsonObject &result)
{
    const auto best=result.value("best").toObject();
    const auto evidence=best.value("interior").toObject();
    const QImage probe(result.value("probe").toString());
    if(probe.size()!=QSize(108,88)||!evidence.contains("supported_mask"))return;
    QImage overlay(432,410,QImage::Format_RGB32);overlay.fill(QColor("#050a11"));
    QPainter painter(&overlay);painter.drawImage(QRect(0,0,432,352),probe);
    painter.save();painter.scale(4,4);painter.setClipRect(QRectF(8,8,92,72));
    const double angle=best.value("angle").toDouble()*3.141592653589793/180;
    const double c=std::cos(angle),s=std::sin(angle),scale=best.value("scale").toDouble(1);
    const auto ridgeRoi=best.value("ridge_roi").toObject();
    const auto referenceCenter=ridgeRoi.value("reference_center").toArray();
    const auto probeCenter=ridgeRoi.value("probe_center").toArray();
    const bool contactAnchored=best.value("transform_anchor").toString()==QStringLiteral("contact-component-v1")&&
        referenceCenter.size()==2&&probeCenter.size()==2;
    // v7 and older saved results have no anchor declaration, so retain their
    // original image-centre transform when rendering historical overlays.
    const double pivotX=contactAnchored?referenceCenter.at(0).toDouble():53.5;
    const double pivotY=contactAnchored?referenceCenter.at(1).toDouble():43.5;
    const double anchorX=contactAnchored?probeCenter.at(0).toDouble():53.5;
    const double anchorY=contactAnchored?probeCenter.at(1).toDouble():43.5;
    auto point=[&](double x,double y){x-=pivotX;y-=pivotY;
        return QPointF(anchorX+scale*(c*x-s*y)+best.value("dx").toDouble(),
                       anchorY+scale*(s*x+c*y)+best.value("dy").toDouble());};
    auto polygon=[&](double x,double y,double w,double h){return QPolygonF{point(x,y),point(x+w,y),point(x+w,y+h),point(x,y+h)};};
    const int supported=evidence.value("supported_mask").toInt(),informative=evidence.value("informative_mask").toInt();
    // Old saved research records have no explicit foreground mask.  Render
    // those with the earlier all-tiles diagnostic rather than claiming their
    // background has been segmented retrospectively.
    const int contactMask=ridgeRoi.value("binary").toBool() ?
        ridgeRoi.value("reference_mask").toInt() : 0xffff;
    for(int i=0;i<16;++i) {
        QColor color;
        if(!(contactMask&(1<<i))) color=QColor(110,125,145,55);       // 0: background
        else if(!(informative&(1<<i))) color=QColor(255,185,45,65);   // 1, but too flat/noisy
        else color=(supported&(1<<i))?QColor(30,230,100,65):QColor(250,55,55,65);
        painter.setBrush(color);painter.setPen(QPen(QColor(color.red(),color.green(),color.blue(),180),.35));
        painter.drawPolygon(polygon(8+(i%4)*23,8+(i/4)*18,23,18));
    }
    auto cross=[&](const QPointF &at,const QColor &color) {
        painter.setPen(QPen(color,.8));
        painter.drawLine(at+QPointF(-2.4,0),at+QPointF(2.4,0));
        painter.drawLine(at+QPointF(0,-2.4),at+QPointF(0,2.4));
        painter.setBrush(Qt::NoBrush);painter.drawEllipse(at,1.25,1.25);
    };
    if(referenceCenter.size()==2&&probeCenter.size()==2) {
        // These are contact-component centres, not fingerprint core points.
        // Showing both makes the transform's translation intelligible without
        // pretending that a tiny partial image contains the full fingerprint.
        cross(point(referenceCenter.at(0).toDouble(),referenceCenter.at(1).toDouble()),QColor(85,220,255));
        cross(QPointF(probeCenter.at(0).toDouble(),probeCenter.at(1).toDouble()),QColor(255,175,45));
    }
    painter.setClipRect(QRectF(0,0,108,88));painter.setBrush(Qt::NoBrush);painter.setPen(QPen(QColor(50,150,255),.6));
    painter.drawPolygon(polygon(8,8,92,72));painter.restore();
    painter.setPen(Qt::white);painter.setFont(QFont(QString(),10));
    painter.drawText(QRect(5,355,422,52),Qt::AlignCenter|Qt::TextWordWrap,
        tr("Contact %1 · image from comparison %2 (not live)\nGray: background (0) · yellow: contact (1), but weak features · green/red: comparison evidence\nCyan +: transformed reference contact centre · orange +: current probe contact centre · blue: comparison range")
        .arg(m_gpuContact).arg(m_gpuRequest));painter.end();
    m_matchImage->setPixmap(QPixmap::fromImage(overlay));
    QSaveFile file(m_dir+QString("/gpu-overlay-%1.png").arg(m_gpuRequest,4,10,QLatin1Char('0')));
    if(file.open(QIODevice::WriteOnly)) {
        file.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner);
        if(overlay.save(&file,"PNG"))file.commit();
    }
}
void LiveWindow::finishMatching()
{
    if(m_finished) {
        if(m_gpuBusy||!m_pending.isEmpty()) {
            m_instruction->setText(QStringLiteral("촬영 종료 — 손을 떼 주세요. 남은 접촉 %1개를 자동 비교 중입니다").arg(m_pending.size()+(m_gpuBusy?1:0)));
            m_stop->setText(QStringLiteral("중단"));
        } else {
            m_stop->setText(QStringLiteral("닫기"));
            m_instruction->setText(m_targetReached?QStringLiteral("목표 %1접촉 비교를 완료했습니다 — 더 반복하지 마세요. 손을 떼셔도 됩니다.").arg(m_targetContacts):m_stopping?QStringLiteral("중단했습니다. 손을 떼셔도 됩니다."):
                QStringLiteral("촬영과 후보 비교가 종료되었습니다. 아래 접촉별 집계를 확인하세요."));
            m_matcher.closeWriteChannel();
        }
    }
    writeStatus();
}
void LiveWindow::writeStatus()
{
    QJsonArray contacts;int compared=0,consistent=0,noCandidate=0;
    for(auto i=m_contacts.cbegin();i!=m_contacts.cend();++i) {
        auto c=i.value();c.insert("contact_group",i.key());
        const bool candidate=c.value("candidate_saved").toBool();
        if(!candidate&&!c.contains("skip_reason"))c.insert("skip_reason",
            c.value("eligible_frames").toInt()>0?"no-stable-candidate":"no-eligible-frame");
        if(!candidate)++noCandidate;
        if(c.value("compared").toBool())++compared;
        if(c.value("consistent").toBool())++consistent;
        c.insert("pending",m_pending.contains(i.key()));contacts.append(c);
    }
    const qint64 now=m_clock.elapsed();
    const bool draining=m_finished&&(m_gpuBusy||!m_pending.isEmpty());
    const QJsonObject object{{"schema_version",2},{"state",draining?"draining":m_finished?"ended":m_stopping?"stopping":"running"},
        {"research_only",true},{"authentication_decision",QJsonValue::Null},
        {"frames",m_total},{"touches",m_touch},{"candidates",m_candidates},
        {"target_contacts",m_targetContacts},{"target_reached",m_targetReached},
        {"background_ready",m_calibrated},{"contact_on",m_contactOn},{"centered_signal",m_centeredSignal},
        {"raw_signal",m_rawSignal},{"background_noise",m_noise},
        {"gpu_busy",m_gpuBusy},{"gpu_requests",m_gpuRequest},{"gpu_completed",m_gpuCompleted},{"gpu_errors",m_gpuErrors},
        {"gpu_cancelled",m_gpuCancelled},{"gpu_stop_expected",m_gpuStopExpected},{"gpu_message",m_matchLabel->text()},
        {"pending_contacts",m_pending.size()},{"compared_contacts",compared},{"consistent_contacts",consistent},
        {"no_candidate_contacts",noCandidate},{"uncompared_contacts",m_touch-compared},
        {"gpu_best",m_gpuStatus.value("best")},{"contacts",contacts},
        {"fps",!m_finished&&m_lastArrival&&now-m_lastArrival<1500?m_fps:0},
        {"age_ms",m_lastArrival?now-m_lastArrival:-1},{"instruction",m_instruction->text()}};
    QSaveFile file(m_dir+"/live-status.json");
    if(file.open(QIODevice::WriteOnly)) {
        file.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner);
        file.write(QJsonDocument(object).toJson());file.commit();
    }
    if(m_finished)m_metrics->setText(QStringLiteral("촬영 종료 · %1 프레임 · 감지 접촉 %2회\n비교 응답 %3회 · 무늬 일관성 %4회 · 미비교 %5회 (후보 없음 %6회) · 대기 %7회 · 오류 %8건\n선별 자료의 연구 결과이며 인증 성공률이 아닙니다")
        .arg(m_total).arg(m_touch).arg(compared).arg(consistent).arg(m_touch-compared).arg(noCandidate).arg(m_pending.size()).arg(m_gpuErrors));
    m_lastStatusWrite=now;
}
void LiveWindow::start()
{
    m_started = true;
    m_instruction->setText(QStringLiteral("아직 손을 떼고 계세요 — 센서 연결 및 배경 보정 중"));
    m_worker.start(m_program, {"--lang", "ko", "--cli", "capture", "--timeout", "195", "--out", m_dir + "/unused.png"});
}
void LiveWindow::tick()
{
    if (m_finished || m_stopping) return;
    const qint64 now = m_clock.elapsed();
    if (!m_started) {
        m_instruction->setText(QStringLiteral("손가락을 떼 주세요 — %1초 후 배경 보정").arg(qMax<qint64>(1, (m_prepareMs - now + 999) / 1000)));
        if (now >= m_prepareMs) start();
        return;
    }
    if (!m_lastArrival) {
        QFile beacon(m_dir + "/capture-status.json");
        if (beacon.open(QIODevice::ReadOnly)) {
            const auto state = QJsonDocument::fromJson(beacon.readAll()).object();
            if (state.value("stage").toString() == "background-ready")
                m_instruction->setText(QStringLiteral("손을 계속 떼고 계세요 — 연속 영상으로 배경을 추가 수집합니다"));
        }
        if (now > m_prepareMs + 15000)
            m_metrics->setText(QStringLiteral("0 FPS · 첫 영상 대기 중 · %1초").arg((now - m_prepareMs) / 1000));
    }
    QFile file(m_dir + "/live.pgm");
    if (file.open(QIODevice::ReadOnly)) {
        const QByteArray data = file.readAll();
        const auto lines = data.split('\n');
        const auto fields = lines.size() > 1 ? lines[1].split(' ') : QList<QByteArray>();
        if ((fields.size() == 5 || fields.size()==9) && fields[0] == "#") {
            const qint64 stamp = fields[1].toLongLong();
            const QImage frame = QImage::fromData(data, "PGM");
            if (!frame.isNull() && stamp > m_lastStamp) {
                m_lastStamp = stamp; m_lastArrival = now; ++m_frames; ++m_total;
                if (now - m_windowStart >= 1000) {
                    m_fps = m_frames * 1000.0 / (now - m_windowStart);
                    m_frames = 0; m_windowStart = now;
                }
                const bool extended=fields.size()==9;
                m_calibrated=!extended||fields[6]=="1";
                m_centeredSignal=fields[2].toDouble();m_rawSignal=extended?fields[5].toDouble():m_centeredSignal;
                m_noise=extended?fields[8].toDouble():0;
                const bool touch=m_calibrated&&(extended?fields[7]=="1":m_centeredSignal>=100);
                if (touch && (extended?!m_contactOn:m_emptyFrames >= 3)) {
                    ++m_touch; m_bestScore = 0;m_stable=0;m_previous=QImage();m_lastMatchCandidate=QImage();
                    m_contacts[m_touch]=QJsonObject{{"start_timestamp_us",stamp}};
                }
                m_contactOn=touch;
                m_emptyFrames = touch ? 0 : m_emptyFrames + 1;
                const auto quality = assessPreview(frame, m_previous, touch?100:0, fields[3].toInt(), fields[4].toInt());
                m_previous = frame;
                m_stable = quality.eligible ? m_stable + 1 : 0;
                if(touch) {
                    auto &contact=m_contacts[m_touch];
                    contact.insert("frames",contact.value("frames").toInt()+1);
                    contact.insert("last_quality_reason",quality.reason);
                    contact.insert("last_timestamp_us",stamp);
                    if(quality.eligible)contact.insert("eligible_frames",contact.value("eligible_frames").toInt()+1);
                }
                // Keep only the best candidate per contact, not every adjacent
                // frame. An explicit lift is required to start another group.
                if (m_stable >= 3 && quality.score > m_bestScore * 1.05 && (m_bestScore>0 || m_candidates<128)) {
                    const QString path = m_dir + QStringLiteral("/candidate-touch-%1.png").arg(m_touch, 2, 10, QLatin1Char('0'));
                    QSaveFile selected(path);
                    if (selected.open(QIODevice::WriteOnly)) {
                        selected.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
                        if (frame.save(&selected, "PNG") && selected.commit()) {
                            if (m_bestScore == 0) ++m_candidates;
                            m_bestScore = quality.score;
                            m_contacts[m_touch].insert("candidate_saved",true);
                            m_contacts[m_touch].insert("candidate_score",quality.score);
                            matchFrame(frame);
                            QSaveFile metadata(path + ".json");
                            if (metadata.open(QIODevice::WriteOnly)) {
                                metadata.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
                                metadata.write(QJsonDocument(QJsonObject{{"role", "unvalidated-candidate"},
                                    {"finger_label", "unconfirmed"}, {"contact_group", m_touch},
                                    {"sensor_timestamp_us", stamp}, {"ranking_version", 1},
                                    {"score", quality.score}, {"coherence", quality.coherence}, {"motion", quality.motion},
                                    {"signal", fields[2].toDouble()}, {"coverage", fields[3].toInt()},
                                    {"sharpness", fields[4].toInt()}}).toJson());
                                metadata.commit();
                            }
                        }
                    } else m_contacts[m_touch].insert("skip_reason","candidate-save-error");
                }
                if(touch&&m_candidates>=128&&m_bestScore==0)m_contacts[m_touch].insert("skip_reason","candidate-storage-limit");
                // A visually different but equally sharp frame can cover more
                // useful ridges. Retry weak contacts without lowering quality
                // gates, altering the reference bank, or resubmitting duplicates.
                const auto contact=m_contacts.value(m_touch);
                if(m_stable>=3&&m_bestScore>0&&contact.value("compared").toBool()&&
                   !contact.value("consistent").toBool()&&contact.value("submitted").toInt()<3&&
                   !m_pending.contains(m_touch)&&!(m_gpuBusy&&m_gpuContact==m_touch)&&
                   !m_lastMatchCandidate.isNull()&&
                   assessPreview(frame,m_lastMatchCandidate,fields[2].toDouble(),fields[3].toInt(),fields[4].toInt()).motion>=4)
                    matchFrame(frame,"diverse-retry");
                m_image->setPixmap(QPixmap::fromImage(frame).scaled(m_image->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
                QString guidance;
                if(!m_calibrated)guidance=QStringLiteral("손을 떼고 유지해 주세요 — 배경을 여러 프레임으로 수집 중입니다 (지금은 접촉하지 마세요)");
                else if(!touch)guidance=QStringLiteral("손 뗌 상태입니다 — 지금 %1을 센서 중앙에 가볍게 올려 주세요").arg(m_fingerPrompt);
                else if(m_differentFinger&&m_contacts[m_touch].value("compared").toBool())
                    guidance=m_contacts[m_touch].value("consistent").toBool()?
                        QStringLiteral("주의: 다른 손가락에서도 무늬 일치 근거가 나왔습니다. 오인식 의심 기록 완료 — 떼었다 다시 대 주세요"):
                        QStringLiteral("이번 다른 손가락에서는 일치 근거가 없었습니다. 기록 완료 — 손을 완전히 떼었다 다시 대 주세요");
                else if(m_contacts[m_touch].value("consistent").toBool())
                    guidance=QStringLiteral("이번 접촉의 무늬를 확인했습니다 — 손을 완전히 떼고 같은 손가락을 다시 올려 주세요 (인증 판정 아님)");
                else if(m_pending.contains(m_touch)||(m_gpuBusy&&m_gpuContact==m_touch))
                    guidance=QStringLiteral("선명한 영상을 확보했습니다 — 자동 비교 중입니다. 손을 떼셔도 됩니다");
                else if(m_contacts[m_touch].value("compared").toBool())
                    guidance=m_contacts[m_touch].value("submitted").toInt()>=3?
                        QStringLiteral("이번 접촉은 3회 비교해도 근거가 부족합니다 — 완전히 뗀 뒤 같은 손가락을 중앙에 다시 대 주세요"):
                        QStringLiteral("일치 근거가 부족합니다 — 같은 손가락을 중앙으로 조금 옮기고 압력을 낮춰 주세요. 자동 재비교합니다");
                else if(fields[3].toInt()<62)guidance=QStringLiteral("접촉 면적이 작습니다 — 같은 손가락의 지문 면을 센서 중앙에 넓게 대 주세요");
                else if(quality.motion>18)guidance=QStringLiteral("손가락을 잠깐 멈춰 주세요 — 안정된 영상을 자동 선택합니다");
                else if(!quality.eligible)guidance=QStringLiteral("무늬가 불분명합니다 — 힘을 조금 빼고 같은 손가락 위치를 살짝 바꿔 주세요");
                else guidance=QStringLiteral("그대로 잠깐 유지해 주세요 — 선명한 영상을 자동 선택 중입니다");
                if(m_differentFinger)guidance=QStringLiteral("다른 손가락 시험 · ")+guidance.replace(QStringLiteral("같은 손가락"),QStringLiteral("이번 시험 손가락"));
                const auto skipReason=m_contacts.value(m_touch).value("skip_reason").toString();
                if(touch&&(skipReason=="candidate-storage-limit"||skipReason=="session-comparison-limit"))
                    guidance=QStringLiteral("연구 저장·비교 한도에 도달했습니다 — 이 접촉은 추가 비교하지 않습니다. 중단해 주세요");
                if(m_gpuFailed)guidance=QStringLiteral("GPU 비교를 사용할 수 없습니다 — 영상만 표시 중입니다. 중단해 주세요");
                if(m_targetContacts>0) {
                    int completed=0;for(const auto &c:m_contacts)if(c.value("compared").toBool())++completed;
                    guidance=tr("[%1/%2 contacts compared · stops automatically at the target] ").arg(completed).arg(m_targetContacts)+guidance;
                }
                m_instruction->setText(guidance);
                m_metrics->setText(QStringLiteral("%1 FPS · %2 프레임 · 무늬 신호 %3 · 명암 면적 %4% · 선명도 %5\n")
                    .arg(m_fps, 0, 'f', 1).arg(m_total).arg(QString::fromLatin1(fields[2]))
                    .arg(QString::fromLatin1(fields[3])).arg(QString::fromLatin1(fields[4]))
                    + QStringLiteral("%1 · 접촉 %2회 · 후보 %3장 · GPU 완료 %4건 / 대기 %5접촉 (인증용 미검증)")
                        .arg(quality.reason).arg(m_touch).arg(m_candidates).arg(m_gpuCompleted).arg(m_pending.size())+
                    QStringLiteral("\n원본 차이 %1 · 배경 잡음 %2 · %3").arg(m_rawSignal,0,'f',1).arg(m_noise,0,'f',1)
                        .arg(!m_calibrated?QStringLiteral("배경 수집 중"):touch?QStringLiteral("손가락 접촉"):QStringLiteral("손 뗌")));
            }
        }
    }
    if (m_lastArrival && now - m_lastArrival > 1500) {
        m_instruction->setText(QStringLiteral("영상 수신이 멈췄습니다 — 현재 화면은 마지막 프레임입니다"));
        m_metrics->setText(QStringLiteral("0 FPS · %1초 동안 새 영상 없음").arg((now - m_lastArrival) / 1000.0, 0, 'f', 1));
    }
    // Machine-readable progress for monitoring without orchestrating each frame.
    if(now-m_lastStatusWrite>=200)writeStatus();
}
void LiveWindow::stop(bool completed)
{
    if ((m_finished&&!m_gpuBusy&&m_pending.isEmpty()) || !m_started) { reject(); return; }
    if (m_stopping) return;
    m_stopping = true;
    m_targetReached=completed;
    for(auto i=m_pending.cbegin();i!=m_pending.cend();++i)m_contacts[i.key()].insert("skip_reason",completed?"target-reached":"user-stopped");
    m_pending.clear();
    if(m_gpuBusy) {
        m_contacts[m_gpuContact].insert("cancel_reason",completed?"target-reached":"user-stopped");
        ++m_gpuCancelled;
    }
    m_gpuStopExpected=true;
    m_gpuBusy=false;m_gpuReady=false;
    if(completed)m_matcher.closeWriteChannel();
    else m_matcher.kill();
    if(!m_gpuFailed)m_matchLabel->setText(completed?
        QStringLiteral("GPU 비교 완료 · %1건 처리 · 목표 %2접촉 도달로 정상 종료\n결과와 색상 표시 영상은 저장되어 있습니다. 인증 승인 여부를 판정한 것은 아닙니다.").arg(m_gpuCompleted).arg(m_targetContacts):
        QStringLiteral("GPU 비교 중단 · 완료 %1건 · 진행 중 취소 %2건\n사용자 요청으로 중단했습니다. 저장된 결과는 유지됩니다.").arg(m_gpuCompleted).arg(m_gpuCancelled));
    m_instruction->setText(QStringLiteral("중단 중 — 손가락을 떼 주세요"));
    writeStatus();
    if(m_finished)finishMatching();
    QFile marker(m_dir + "/live.pgm.stop");
    if (marker.open(QIODevice::WriteOnly))
        marker.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    QTimer::singleShot(3000, this, [this] { if (m_worker.state() != QProcess::NotRunning) m_worker.terminate(); });
    QTimer::singleShot(5000, this, [this] { if (m_worker.state() != QProcess::NotRunning) m_worker.kill(); });
}
void LiveWindow::closeEvent(QCloseEvent *event)
{
    if (m_started&&(!m_finished||m_gpuBusy||!m_pending.isEmpty())) { stop(); event->ignore(); }
    else event->accept();
}
}
