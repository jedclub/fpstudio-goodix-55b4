#include "livewindow.h"
#include "mapplace.h"

#include "theme.h"
#include "frameselector.h"
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include "enrolselect.h"
#include <QPushButton>
#include <QDateTime>
#include <QMessageBox>
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
    m_fingerPrompt=differentFinger?tr("a different finger from the last one"):
        tr("the same finger used so far");
    setWindowTitle(tr("fpstudio · live fingerprint stream"));
    // 1050x920 assumes a display with room for it. On a 1080p panel, once the
    // title bar and a desktop panel are taken out, it is already close; at any
    // desktop scale above 1 it is past, and what goes over the bottom edge is
    // the stop button - the one control this window has. Clamped to the screen
    // it actually opens on.
    resize(theme::fitToScreen(this, 1050, 920));
    setStyleSheet("QDialog {background:#101827;} QLabel {color:#e5edf8;} QPushButton {padding:12px 28px;font-size:18px;}");
    auto *layout = new QVBoxLayout(this);
    m_instruction = new QLabel(m_differentFinger
        ? tr("\u26a0 Different-finger trial — lift your finger off the sensor. Calibration starts automatically.")
        : tr("Lift your finger off the sensor. Calibration starts automatically."));
    m_instruction->setWordWrap(true);
    // Red for the different-finger run, blue otherwise.
    //
    // These two sessions look identical on screen and produce identically
    // named files, but they mean opposite things: one is what the matcher must
    // accept, the other is what it must reject. A session recorded under the
    // wrong label is worse than no session, because it teaches the exact
    // opposite of the truth and nothing downstream can detect it. The banner
    // is the only thing the person is looking at while they decide which
    // finger to use, so the distinction belongs here rather than in the
    // detail line underneath.
    m_instruction->setStyleSheet(m_differentFinger
        ? "font-size:25px;font-weight:bold;padding:18px;background:#b91c1c;border-radius:8px;color:#ffffff;"
        : "font-size:25px;font-weight:bold;padding:18px;background:#1d4ed8;border-radius:8px;");
    // A word-wrapped label grows taller as the window gets narrower, and this
    // one is 25px bold - three wrapped lines is most of a phone screen. It
    // carries short instructions, but it also carries error text from the
    // capture helper, whose length nothing here controls. Capped so that no
    // message it is handed can take the window's other rows off the bottom.
    m_instruction->setMaximumHeight(110);
    auto *detail = new QLabel(tr("After calibration, rest the same finger lightly on the sensor and follow the prompts. Candidates are compared automatically for each contact.\n"
        "Live sensor image 108 × 88 · background subtracted, contrast normalised · not an authentication decision · up to 3 minutes"));
    detail->setWordWrap(true); detail->setStyleSheet("font-size:16px;");
    detail->setMaximumHeight(72);
    if(m_differentFinger)detail->setText(tr("This is a different-finger comparison trial. Do not use the finger from the previous run this time.\n"
        "After calibration, tap and lift repeatedly with one other finger. Even a high score does not mean authentication was granted."));
    m_image = new QLabel(tr("Waiting for calibration — no live image yet"));
    // Minimum, not preferred: two of these side by side at 352 high, plus the
    // instruction banner, the detail text, the metrics line, the match caption
    // and the button, set a floor the window cannot shrink below. On a short
    // screen that floor is what pushes the button out of reach, so it is stated
    // small and the layout's stretch factor is left to make them large.
    m_image->setMinimumSize(240, 180); m_image->setAlignment(Qt::AlignCenter);
    // These two hold the preview frames, so they should take all the space the
    // layout can give them - but they also display status and error text, and a
    // QLabel's minimum size hint grows with its text. That is how a long error
    // message from the capture helper could raise the window's floor and push
    // the stop button off the bottom. Ignored size policy detaches the layout
    // from the text's hint; the explicit minimum above still applies, so they
    // keep a floor of their own without inheriting one from their contents.
    m_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_image->setWordWrap(true);
    m_image->setStyleSheet("background:#050a11;border:1px solid #344155;");
    m_metrics = new QLabel(tr("0 FPS · waiting for a new frame"));
    m_metrics->setStyleSheet("font-size:18px;");
    m_metrics->setMaximumHeight(44);
    m_matchLabel = new QLabel(tr("GPU comparison: no reference image set · collecting live only"));
    m_matchLabel->setWordWrap(true); m_matchLabel->setStyleSheet("font-size:16px;color:#a5d7ff;");
    // Capped as well as floored: this caption reports per-contact comparison
    // results whose length is not fixed, and an uncapped growing widget in a
    // vertical layout takes its space from whatever sits below it.
    m_matchLabel->setMinimumHeight(110);
    m_matchLabel->setMaximumHeight(150);
    m_stop = new QPushButton(tr("Stop"));
    m_stop->setObjectName(QStringLiteral("stop"));
    // Only offered once the session has stopped and there is something to
    // install: enrolling from a half-finished sweep is how a gallery ends up
    // covering one corner of the finger.
    m_install = new QPushButton(tr("Install this fingerprint as the enrolment"));
    m_install->setObjectName(QStringLiteral("install"));
    m_install->hide();
    m_matchImage=new QLabel(tr("Coverage map — drawn here as contacts accumulate\nThe dark red areas have not been touched yet\nMove your finger toward the empty side\nThis is enrolment guidance, not an authentication decision"));
    m_matchImage->setAlignment(Qt::AlignCenter);m_matchImage->setMinimumSize(240,180);
    m_matchImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_matchImage->setWordWrap(true);
    m_matchImage->setStyleSheet("background:#050a11;border:1px solid #344155;font-size:15px;");
    auto *images=new QHBoxLayout;images->addWidget(m_image,1);images->addWidget(m_matchImage,1);
    layout->addWidget(m_instruction); layout->addWidget(detail); layout->addLayout(images, 1);
    layout->addWidget(m_metrics); layout->addWidget(m_matchLabel);
    auto *actions=new QHBoxLayout;actions->addStretch(1);
    actions->addWidget(m_install);actions->addWidget(m_stop);
    layout->addLayout(actions);
    connect(m_stop, &QPushButton::clicked, this, [this]{stop();});
    connect(m_install, &QPushButton::clicked, this, [this]{installEnrolment();});
    connect(&m_installer, qOverload<int,QProcess::ExitStatus>(&QProcess::finished), this,
            [this](int code,QProcess::ExitStatus status) {
        m_install->setEnabled(true);
        m_stage.reset();
        const bool ok=status==QProcess::NormalExit&&code==0;
        m_instruction->setText(ok?tr("The enrolment is installed. sudo and the lock screen now use this fingerprint.")
                                 :tr("Installing the enrolment failed. The existing enrolment is unchanged."));
        if(!ok) {
            const QString detail=QString::fromLocal8Bit(m_installer.readAllStandardError()).trimmed();
            QMessageBox::warning(this,tr("Installation failed"),
                detail.isEmpty()?tr("The installer exited with code %1.").arg(code)
                                :detail.right(1200));
        }
    });
    m_lock = std::make_unique<QLockFile>(QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation) + "/fpstudio-research.lock");
    if (!m_lock->tryLock() || !QDir().mkpath(m_dir) ||
        !QFile::setPermissions(m_dir, QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner)) {
        m_instruction->setText(tr("Could not start. Check for another sensor session, or the permissions on the save folder."));
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
            m_instruction->setText(tr("Could not run the capture program: ") + m_worker.errorString());
            m_stop->setText(tr("Close"));
            finishMatching();
        }
    });
    connect(&m_worker, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, [this](int, QProcess::ExitStatus) {
        m_finished = true;
        m_instruction->setText(m_stopping ? tr("Stopped. You can lift your finger off the sensor.")
             : tr("The live stream has ended. Lift your finger off the sensor."));
        m_metrics->setText(tr("0 FPS · ended · the last image is a still · %1 frames in total").arg(m_total));
        m_stop->setText(tr("Close"));
        saveMap();
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
            m_matchLabel->setText(tr("The GPU comparison worker exited unexpectedly (code %1)").arg(code));
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
// Where this capture sits relative to everything placed before it.
//
// Brute-force normalised cross-correlation over the search window. It is one
// search per accepted contact - a second or two apart - so the cost of trying
// every offset is irrelevant, and it avoids the trap the video experiment fell
// into: phase correlation whitens every frequency equally, this sensor has a
// fixed pattern that does not move with the finger, and whitened that pattern
// wins every time. It answered (0,0) for every pair while brute force on the
// same pair found the true shift at 0.965.
// Rotation span the GPU search covers. Measured over a 107-capture session:
// no rotation left 7% of contacts unplaced, +-8 degrees left 4%, +-15 left
// 3%, and +-25 left 2% for six times the poses. The knee is at 15.
constexpr double kMapAngleSpan = 15.0;
constexpr double kMapAngleCoarse = 3.0;
constexpr double kMapAngleFine = 0.75;

// Searching the map on the GPU.
//
// The poses are independent of one another, which is the only property that
// matters here. On the CPU, on the thread that reads preview frames, the search
// could afford translation only - every second offset, no rotation - and a
// contact whose finger had rolled a few degrees had no pose that fitted and was
// dropped. Replaying one session with rotation added on the CPU did not finish
// in ten minutes.
//
// The same session through this path: 7% of contacts unplaced becomes 3%, mean
// registration 0.640 becomes 0.722, coverage 4.39x becomes 4.62x, and the
// search costs 32ms of GPU time for 80,000 poses where the narrow CPU one cost
// 11ms of the frame-reading thread for 3,984.
//
// Two passes rather than one: a coarse grid, then a fine one around its winner.
// One pass over the same span would be six times the poses for one more contact
// placed, measured over the same session.
// Where on the map this capture belongs, searched on the GPU.
//
// The window is given by the caller because the two questions are different
// sizes. A contact that has just started could belong anywhere the probe
// fits, and gets swept whole - the old code searched only 60px either side of
// the map's centre, so a region further out than that could not be placed
// correctly at all and was forced into the nearest position the search could
// reach. That is what stacks a session's captures into a few overlapping
// blocks instead of spreading them across the finger. A contact that never
// lifted has moved a few pixels since the last frame and only needs those.
//
// peakGap reports how much better the winner is than the best placement well
// away from it. Ridges repeat, so a wrong placement one period out scores
// nearly as well; without this the maximum is picked regardless and the map is
// corrupted by confident-looking mistakes.
static bool searchOnGpu(vkmap::Placer &placer,const QVector<float> &mapPixel,
                        const QVector<float> &mapScore,const QVector<float> &probe,
                        int mapSize,int w,int h,
                        int fromX,int toX,int fromY,int toY,int step,
                        double angleCentre,double angleSpan,double angleCoarse,
                        int peakRadius,
                        int &bestX,int &bestY,double &bestAngle,
                        double &bestFit,double &peakGap,double &gpuMs)
{
    const std::vector<float> pixels(mapPixel.cbegin(),mapPixel.cend());
    const std::vector<float> weights(mapScore.cbegin(),mapScore.cend());
    const std::vector<float> probeCopy(probe.cbegin(),probe.cend());
    placer.setMap(pixels,weights,uint32_t(mapSize));
    placer.setProbe(probeCopy,uint32_t(w),uint32_t(h));

    const int limitX=mapSize-w,limitY=mapSize-h;
    fromX=std::max(0,fromX);toX=std::min(limitX,toX);
    fromY=std::max(0,fromY);toY=std::min(limitY,toY);
    if(fromX>toX||fromY>toY)return false;

    auto sweep=[&](double angleFrom,double angleTo,double angleStep,
                   int x0,int x1,int y0,int y1,int stride,
                   int &outX,int &outY,double &outAngle,double &outFit,
                   double *gap)->bool {
        std::vector<vkmap::Pose> poses;
        for(double a=angleFrom;a<=angleTo+1e-9;a+=angleStep)
            for(int oy=y0;oy<=y1;oy+=stride)
                for(int ox=x0;ox<=x1;ox+=stride)
                    poses.push_back({float(ox),float(oy),float(a)});
        if(poses.empty())return false;
        const auto scored=placer.run(poses,2);
        gpuMs+=placer.lastWallMs();
        double best=-2;size_t at=0;bool any=false;
        for(size_t i=0;i<scored.size();++i) {
            // A pose that agreed with almost none of the covered map has not
            // been tested, however well the handful of samples happened to line
            // up. The CPU search applied the same floor as a sample count.
            if(scored[i].overlap<0.25f)continue;
            if(scored[i].ncc>best){best=scored[i].ncc;at=i;any=true;}
        }
        if(!any)return false;
        if(gap) {
            // The best placement that is not simply a neighbour of the winner.
            double rival=-2;
            for(size_t i=0;i<scored.size();++i) {
                if(scored[i].overlap<0.25f)continue;
                const double dx=double(poses[i].offsetX)-double(poses[at].offsetX);
                const double dy=double(poses[i].offsetY)-double(poses[at].offsetY);
                if(dx*dx+dy*dy<double(peakRadius)*double(peakRadius))continue;
                if(scored[i].ncc>rival)rival=scored[i].ncc;
            }
            *gap=rival<-1?1.0:best-rival;
        }
        outX=int(poses[at].offsetX);outY=int(poses[at].offsetY);
        outAngle=poses[at].angleDegrees;outFit=best;
        return true;
    };

    int cx=fromX,cy=fromY;double ca=angleCentre,cf=-2;
    peakGap=1.0;
    if(!sweep(angleCentre-angleSpan,angleCentre+angleSpan,angleCoarse,
              fromX,toX,fromY,toY,step,cx,cy,ca,cf,&peakGap))
        return false;
    // Widening the first pass to the whole map made each one about three times
    // the work, which is paid once per contact on the thread that also reads
    // preview frames - enough to start dropping them. A coarse grid followed
    // by a middle pass around its winner costs less than the old narrow search
    // did while reaching everywhere, because resolution is only spent where
    // the answer turned out to be.
    if(step>2) {
        int mx=cx,my=cy;double ma=ca,mf=cf;
        if(sweep(ca-angleCoarse,ca+angleCoarse,angleCoarse,
                 cx-step,cx+step,cy-step,cy+step,2,mx,my,ma,mf,nullptr)&&mf>cf) {
            cx=mx;cy=my;ca=ma;cf=mf;
        }
    }
    int fx=cx,fy=cy;double fa=ca,ff=cf;
    sweep(ca-angleCoarse,ca+angleCoarse,kMapAngleFine,
          cx-3,cx+3,cy-3,cy+3,1,fx,fy,fa,ff,nullptr);
    if(ff<cf){fx=cx;fy=cy;fa=ca;ff=cf;}
    bestX=fx;bestY=fy;bestAngle=fa;bestFit=ff;
    return true;
}

void LiveWindow::placeOnMap(const QImage &frame)
{
    if(frame.size()!=QSize(108,88))return;
    if(m_mapPixel.isEmpty()) {
        m_mapPixel.fill(0.f,kMapSize*kMapSize);
        m_mapScore.fill(0.f,kMapSize*kMapSize);
    }
    const int W=108,H=88;
    QVector<float> pixels(W*H);
    for(int y=0;y<H;++y)for(int x=0;x<W;++x)
        pixels[y*W+x]=qGray(frame.pixel(x,y));

    // Measured first, acted on after the search: a contact that is not going on
    // the map is exactly the one whose position needs showing, so the search
    // runs either way and the window can point at where it landed.
    const double quality=fpstudio::ridgeSharpness(pixels.constData(),W,H);

    int bestX=(kMapSize-W)/2,bestY=(kMapSize-H)/2;
    // The first capture has nothing to register against, so it used to be
    // placed with a perfect score of 1.0 - which no later capture could ever
    // beat, freezing whatever happened to arrive first into the middle of the
    // map for the rest of the session. It gets the same ordinary standing
    // everything else has to earn.
    double fit=kMapSeedFit;
    double bestAngle=0;
    if(m_mapPlaced>0&&!m_placerTried) {
        m_placerTried=true;
        // Built once. A machine with no usable compute queue keeps the CPU
        // search rather than failing the session over a picture that decides
        // nothing.
        QString spirv=QStringLiteral(MAPPLACE_SHADER_PATH);
        if(!QFile::exists(spirv))
            spirv=QCoreApplication::applicationDirPath()+QStringLiteral("/vkmatch/fpstudio-mapplace.spv");
        if(!QFile::exists(spirv))
            spirv=QCoreApplication::applicationDirPath()+QStringLiteral("/fpstudio-mapplace.spv");
        try {
            m_placer=std::make_unique<vkmap::Placer>(spirv.toStdString());
            m_mapFastPath=true;
            qInfo("지도 정합: GPU %s", m_placer->deviceName().c_str());
        } catch(const std::exception &e) {
            m_placer.reset();m_mapFastPath=false;
            qWarning("지도 정합 GPU 사용 불가 (%s) - CPU 탐색으로 진행합니다",e.what());
        }
    }
    if(m_mapPlaced>0&&m_placer) {
        double gpuMs=0,gfit=-2,gap=1.0;int gx=bestX,gy=bestY;double ga=0;
        bool ok=false;
        // Carrying the pose forward is what makes this accurate. Within one
        // contact the finger has moved a few pixels since the last frame, so a
        // small window around where it was is both the right place to look and
        // far cheaper than sweeping the map. Only a contact that has just
        // started has to be found from nothing.
        const bool tracking=m_mapAnchored;
        try {
            if(tracking)
                ok=searchOnGpu(*m_placer,m_mapPixel,m_mapScore,pixels,kMapSize,W,H,
                               m_trackX-kMapTrackRadius,m_trackX+kMapTrackRadius,
                               m_trackY-kMapTrackRadius,m_trackY+kMapTrackRadius,1,
                               m_trackAngle,kMapTrackAngleSpan,kMapAngleFine,
                               kMapPeakRadius,gx,gy,ga,gfit,gap,gpuMs);
            else
                ok=searchOnGpu(*m_placer,m_mapPixel,m_mapScore,pixels,kMapSize,W,H,
                               0,kMapSize-W,0,kMapSize-H,6,
                               0.0,kMapAngleSpan,kMapAngleCoarse,
                               kMapPeakRadius,gx,gy,ga,gfit,gap,gpuMs);
        } catch(const std::exception &e) {
            qWarning("지도 정합 GPU 실패 (%s) - 이번 접촉은 CPU 로 탐색합니다",e.what());
            m_placer.reset();m_mapFastPath=false;
        }
        m_placeMs=gpuMs;
        if(m_placer) {
            if(!ok){m_mapAnchored=false;++m_rejectNoOverlap;++m_mapRejects;
                m_mapOutcome=MapOutcome::NoOverlap;renderMap();return;}
            // A tracked frame is already known to belong here, so it is judged
            // on fit alone; a fresh anchor also has to be the only plausible
            // answer, or the map is about to be corrupted confidently.
            const double floor=tracking?kMapTrackMinFit:kMapMinFit;
            if(gfit<floor||(!tracking&&gap<kMapMinPeakGap)) {
                if(gfit>=floor)++m_rejectAmbiguous; else ++m_rejectUnregistered;
                m_mapAnchored=false;
                m_mapOutcome=MapOutcome::NotRegistered;
                m_mapLastX=gx;m_mapLastY=gy;m_mapLastFit=gfit;
                ++m_mapRejects;renderMap();return;
            }
            bestX=gx;bestY=gy;bestAngle=ga;fit=gfit;
        }
    }
    if(m_mapPlaced>0&&!m_placer) {
        // Only the area already covered can vote, so an offset that hangs off
        // the edge of the map is judged on the part that overlaps rather than
        // being rewarded for overlapping nothing.
        const int centreX=(kMapSize-W)/2,centreY=(kMapSize-H)/2;
        auto scoreAt=[&](int ox,int oy,double &out)->bool {
            if(ox<0||oy<0||ox+W>kMapSize||oy+H>kMapSize)return false;
            double sa=0,sb=0,saa=0,sbb=0,sab=0;int n=0;
            for(int y=0;y<H;y+=2)for(int x=0;x<W;x+=2) {
                const int idx=(oy+y)*kMapSize+ox+x;
                if(m_mapScore[idx]<=0)continue;
                const double a=pixels[y*W+x],b=m_mapPixel[idx];
                sa+=a;sb+=b;saa+=a*a;sbb+=b*b;sab+=a*b;++n;
            }
            if(n<300)return false;                   // too little overlap to judge
            const double va=saa-sa*sa/n,vb=sbb-sb*sb/n;
            if(va<1e-6||vb<1e-6)return false;
            out=(sab-sa*sb/n)/std::sqrt(va*vb);
            return true;
        };
        // Every second offset first, then every offset within one step of the
        // winner. Two is not an arbitrary coarse step: the ridges repeat every
        // 8-10px, so the correlation surface has a side lobe that close to the
        // true peak, and a coarse step that approaches half a ridge period can
        // land on the wrong lobe with no way back. Replayed over a real
        // 101-capture session against the same map, a step of 2 agreed with
        // the exhaustive search to within 1px on 79 of 91 placements for an
        // average score cost of 0.0001, while a step of 3 started missing by a
        // full ridge - 11px - and a step of 4 was worse again.
        //
        // The exhaustive search cost 80ms, on the thread that reads frames,
        // every time a capture was accepted. Frames arrive every 30-40ms.
        double best=-2;
        for(int oy=centreY-60;oy<=centreY+60;oy+=2)
        for(int ox=centreX-60;ox<=centreX+60;ox+=2) {
            double score;
            if(!scoreAt(ox,oy,score))continue;
            if(score>best){best=score;bestX=ox;bestY=oy;}
        }
        if(best<-1){          // nowhere on the map overlapped enough to judge
            m_mapOutcome=MapOutcome::NoOverlap;renderMap();return;
        }
        const int coarseX=bestX,coarseY=bestY;
        for(int oy=coarseY-1;oy<=coarseY+1;++oy)
        for(int ox=coarseX-1;ox<=coarseX+1;++ox) {
            double score;
            if(!scoreAt(ox,oy,score))continue;
            if(score>best){best=score;bestX=ox;bestY=oy;}
        }
        if(best<kMapMinFit) {   // not confidently anywhere; leave the map alone
            ++m_rejectUnregistered;
            m_mapOutcome=MapOutcome::NotRegistered;
            m_mapLastX=bestX;m_mapLastY=bestY;m_mapLastFit=best;
            ++m_mapRejects;renderMap();return;
        }
        fit=best;
    }

    if(quality<kMapMinSharpness) {
        ++m_rejectSmeared;
        m_mapOutcome=MapOutcome::TooSmeared;
        m_mapLastX=bestX;m_mapLastY=bestY;m_mapLastFit=fit;
        ++m_mapRejects;renderMap();return;
    }

    const double angleRadians=bestAngle*M_PI/180.0;
    const double cosA=std::cos(angleRadians),sinA=std::sin(angleRadians);
    for(int y=0;y<H;++y)for(int x=0;x<W;++x) {
        // How much this capture deserves to be believed at this pixel: how
        // well it registered overall, and how far the pixel is from the edge
        // of the sensor, where contact is most marginal and the driver's
        // background subtraction has the least to work with.
        const float tx=std::min(1.f,float(std::min(x,W-1-x))/12.f);
        const float ty=std::min(1.f,float(std::min(y,H-1-y))/12.f);
        // How well it registered, how clear its ridges are, and how far the
        // pixel is from the sensor edge where contact is most marginal.
        //
        // Quality is what makes pressing the same place again worth doing:
        // registration score alone is a measure of agreement with what is
        // already there, so the incumbent always had the advantage and a
        // cleaner second look could not take the pixel back. Over a real
        // session this rule let later captures reclaim 8.7 times a capture's
        // area where the old one managed 7.9, and the finished map went from
        // 0.855 to 0.883 on ridge coherence.
        const float claim=tx*ty*float(fit*quality);
        if(claim<=0)continue;
        // The pose found may include a rotation, so the destination is computed
        // the same way the search scored it: about the capture's own centre.
        const double dx=double(x)-W/2.0,dy=double(y)-H/2.0;
        const int mx=int(std::lround(bestX+(cosA*dx-sinA*dy)+W/2.0));
        const int my=int(std::lround(bestY+(sinA*dx+cosA*dy)+H/2.0));
        if(mx<0||my<0||mx>=kMapSize||my>=kMapSize)continue;
        const int idx=my*kMapSize+mx;
        // A weighted mean, not a winner. Letting one capture own a pixel
        // outright puts a boundary wherever ownership changes, and the captures
        // disagree across it: measured on two real sessions the brightness step
        // across such a boundary was 2.3 to 3.0 times the step inside a single
        // capture, while its low-frequency part was only 1.1 - so the visible
        // cut is the ridges arriving out of phase, not the captures disagreeing
        // about brightness, and no amount of re-registering removes it. Trying
        // harder was measured and did not help: rotation, re-solving every pose
        // against the finished map, and per-block elastic warping all left the
        // step at 2.4 to 2.8 and two of them made the map worse.
        //
        // Skin is not rigid; two presses of one finger genuinely differ. What
        // removes the edge is not having one. Blending costs about a fifth of
        // the ridge contrast and takes ridge coherence from 0.87 to 0.94, above
        // what a single capture scores.
        const float w=claim;
        m_mapPixel[idx]=(m_mapPixel[idx]*m_mapScore[idx]+pixels[y*W+x]*w)/(m_mapScore[idx]+w);
        // Capped, so the map stays answerable to a new contact. A plain running
        // mean is deaf by its tenth - the eleventh moves it by an eleventh - and
        // pressing a place again stops changing anything, which was the
        // complaint that started this.
        m_mapScore[idx]=std::min(kMapWeightCap,m_mapScore[idx]+w);
    }
    ++m_mapPlaced;
    m_mapOutcome=MapOutcome::Placed;
    if(m_mapAnchored)++m_trackedPlacements; else ++m_anchorPlacements;
    m_mapAnchored=true;m_trackX=bestX;m_trackY=bestY;m_trackAngle=bestAngle;
    m_mapLastX=bestX;m_mapLastY=bestY;m_mapLastFit=fit;m_mapLastQuality=quality;m_mapLastAngle=bestAngle;
    renderMap();
}

// Blending rounds off the ridge edges: measured against the captures the
// map was built from, it keeps 4.5 to 7.6 times their power at the ridge
// frequency itself - averaging reinforces what the captures agree on - but
// only 0.61 of it between periods of 3.5 and 7 pixels, which is what makes
// a ridge look like a ridge rather than a stripe. An unsharp mask of this
// radius and strength brings that band back to 0.99 of a single capture's
// and no further, leaving coherence at 0.921 where it was 0.924. It puts
// back what blending took, rather than adding contrast that was never
// there.
constexpr double kMapSharpenSigma = 1.0;
constexpr double kMapSharpenAmount = 0.5;

// Put back the detail blending rounds off.
//
// Uncovered map is not zero here, it is absent: a blur that treated it as black
// would eat into the edge of every covered region and draw a dark rim around
// the whole map. Each pixel is normalised by the weight that actually reached
// it.
static QVector<float> restoreDetail(const QVector<float> &src,const QVector<float> &score,
                                    int w,int h,int stride,int x0,int y0)
{
    const int r=int(std::ceil(3.0*kMapSharpenSigma));
    QVector<float> kernel(2*r+1);
    for(int i=-r;i<=r;++i)
        kernel[i+r]=float(std::exp(-0.5*double(i*i)/(kMapSharpenSigma*
                                                    kMapSharpenSigma)));
    QVector<float> value(w*h,0.f),weight(w*h,0.f),out(w*h,0.f);
    // horizontal
    for(int y=0;y<h;++y)for(int x=0;x<w;++x) {
        float v=0,wt=0;
        for(int i=-r;i<=r;++i) {
            const int sx=x+i;
            if(sx<0||sx>=w)continue;
            const int idx=(y0+y)*stride+x0+sx;
            if(score[idx]<=0)continue;
            v+=src[idx]*kernel[i+r];wt+=kernel[i+r];
        }
        value[y*w+x]=v;weight[y*w+x]=wt;
    }
    // vertical, then the mask itself
    for(int y=0;y<h;++y)for(int x=0;x<w;++x) {
        float v=0,wt=0;
        for(int i=-r;i<=r;++i) {
            const int sy=y+i;
            if(sy<0||sy>=h)continue;
            if(weight[sy*w+x]<=0)continue;
            v+=value[sy*w+x]*kernel[i+r];wt+=weight[sy*w+x]*kernel[i+r];
        }
        const int idx=(y0+y)*stride+x0+x;
        if(score[idx]<=0||wt<=0){out[y*w+x]=score[idx]>0?src[idx]:0.f;continue;}
        const float blurred=v/wt;
        out[y*w+x]=float(std::clamp(double(src[idx])+
            kMapSharpenAmount*(double(src[idx])-double(blurred)),0.0,255.0));
    }
    return out;
}

void LiveWindow::renderMap()
{
    if(m_mapPixel.isEmpty())return;
    int x0=kMapSize,y0=kMapSize,x1=0,y1=0;long covered=0;
    for(int y=0;y<kMapSize;++y)for(int x=0;x<kMapSize;++x)
        if(m_mapScore[y*kMapSize+x]>0){
            ++covered;
            x0=std::min(x0,x);x1=std::max(x1,x);y0=std::min(y0,y);y1=std::max(y1,y);
        }
    if(covered<100)return;
    // A margin of uncovered map around the edge, so a gap at the boundary
    // reads as a gap rather than as the picture simply ending there.
    x0=std::max(0,x0-14);y0=std::max(0,y0-14);
    x1=std::min(kMapSize-1,x1+14);y1=std::min(kMapSize-1,y1+14);
    const int w=x1-x0+1,h=y1-y0+1;
    const QVector<float> shown=restoreDetail(m_mapPixel,m_mapScore,w,h,kMapSize,x0,y0);
    QImage map(w,h,QImage::Format_RGB32);
    for(int y=0;y<h;++y)for(int x=0;x<w;++x) {
        const int idx=(y0+y)*kMapSize+x0+x;
        if(m_mapScore[idx]>0) {
            const int v=std::clamp(int(shown[y*w+x]),0,255);
            map.setPixel(x,y,qRgb(v,v,v));
        } else {
            map.setPixel(x,y,qRgb(28,16,16));      // uncovered: still to fill
        }
    }
    const int side=std::max(w,h);
    QImage panel(side,side+34,QImage::Format_RGB32);panel.fill(QColor("#050a11"));
    QPainter painter(&panel);
    painter.drawImage(QRect((side-w)/2,(side-h)/2,w,h),map);
    painter.setPen(QColor(50,150,255));painter.setBrush(Qt::NoBrush);
    painter.drawRect((side-w)/2,(side-h)/2,w-1,h-1);
    // Where the last contact went. Without it there is no way to tell which
    // part of the map a press just changed, so aiming at a gap is guesswork
    // and a press that was refused looks the same as one that worked.
    if(m_mapLastX>=0) {
        const int lx=(side-w)/2+m_mapLastX-x0,ly=(side-h)/2+m_mapLastY-y0;
        QColor mark;QString note;
        switch(m_mapOutcome) {
        case MapOutcome::Placed:        mark=QColor(80,255,140);note=tr("added");break;
        case MapOutcome::TooSmeared:    mark=QColor(255,80,80); note=tr("smeared · not added");break;
        case MapOutcome::NotRegistered: mark=QColor(255,170,40);note=tr("registration failed · not added");break;
        default:                        mark=QColor(255,170,40);note=tr("too little overlap · not added");break;
        }
        painter.setPen(QPen(mark,2));
        painter.drawRect(lx,ly,108-1,88-1);
        painter.drawLine(lx+54,ly+38,lx+54,ly+50);
        painter.drawLine(lx+48,ly+44,lx+60,ly+44);
        painter.setPen(mark);painter.setFont(QFont(QString(),8));
        painter.drawText(QRect(lx,ly-13,108,12),Qt::AlignCenter,
            tr("last contact · %1").arg(note));
    }
    painter.setPen(Qt::white);painter.setFont(QFont(QString(),9));
    painter.drawText(QRect(0,side,side,34),Qt::AlignCenter|Qt::TextWordWrap,
        tr("Coverage %1× · %2 placed · %3 not added\nDark red is what has not been touched yet · the green outline is what was just added")
            .arg(double(covered)/(108.0*88.0),0,'f',2).arg(m_mapPlaced).arg(m_mapRejects));
    painter.end();
    m_matchImage->setPixmap(QPixmap::fromImage(
        panel.scaled(m_matchImage->width()>32?m_matchImage->width():432,
                     m_matchImage->height()>32?m_matchImage->height():432,
                     Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}


// Which captures become the enrolment.
//
// Ranking by sharpness alone was measured and is not the right rule: on this
// session the sharpest 21 captures recognised 17 of 25 probes where a broader
// 32 recognised 20, because a set chosen purely for sharpness clusters on
// whatever part of the finger happened to press cleanly and has nothing to say
// about the rest. The map knows where each capture sat, so the set is grown by
// what it adds - each step takes the capture contributing the most finger the
// set does not already have, weighted by how clear that capture is. Sharpness
// breaks ties and fills any remaining room.
// Where the installer lives, searched the same way the setup module searches
// for its own resources. Resolved here rather than calling into that module:
// the live window is built into targets that do not link it, and a private
// copy of four paths is cheaper than that coupling.
static QString installerScript()
{
    const QString bin=QFileInfo(QCoreApplication::applicationFilePath()).absolutePath();
    for(const QString &root:{bin+QStringLiteral("/../share/fpstudio/setup"),bin,
                             bin+QStringLiteral("/.."),bin+QStringLiteral("/../../..")}) {
        const QString path=QDir(root).absoluteFilePath(QStringLiteral("tools/auth_install.py"));
        if(QFileInfo::exists(path))return QFileInfo(path).canonicalFilePath();
    }
    return QString();
}

QStringList LiveWindow::selectEnrolment(int limit) const
{
    // Read back what the session recorded rather than what this object still
    // holds: the same function then serves the button, an offline rerun and a
    // test, and all three make the same choice.
    return fpstudio::selectEnrolment(fpstudio::readEnrolCandidates(m_dir),limit,kMapSize);
}

// Hand this session's captures to the authentication store.
//
// Until now the enrolment program produced files and something else had to
// copy them into place, which meant enrolling and being enrolled were separate
// acts and the second one was easy to get wrong or forget. The matcher accepts
// at most 64 references, so the selection above is what decides.
void LiveWindow::installEnrolment()
{
    if(m_installer.state()!=QProcess::NotRunning)return;
    const QStringList chosen=selectEnrolment(56);
    if(chosen.isEmpty()) {
        QMessageBox::warning(this,tr("Cannot install"),
            tr("No capture has been placed on the map. Rest your finger on the sensor, move it slowly, and collect again."));
        return;
    }
    const QString user=qEnvironmentVariable("USER");
    if(user.isEmpty())return;
    if(QMessageBox::question(this,tr("Install fingerprint enrolment"),
        tr("Install %1 captures from this session as the authentication enrolment.\n\n"
           "The existing enrolment is backed up and replaced; fprintd enrolment and PAM settings are left alone.\n"
           "This matcher has not been validated for rejecting other people. Continue?").arg(chosen.size()))
        !=QMessageBox::Yes)return;

    m_stage=std::make_unique<QTemporaryDir>();
    if(!m_stage->isValid())return;
    QDir().mkpath(m_stage->path());
    QFile::setPermissions(m_stage->path(),QFileDevice::ReadOwner|QFileDevice::WriteOwner|QFileDevice::ExeOwner);
    int index=0;
    for(const QString &source:chosen) {
        const QString target=m_stage->path()+QStringLiteral("/candidate-touch-%1.png").arg(index++,2,10,QLatin1Char('0'));
        if(!QFile::copy(source,target))continue;
        QFile::setPermissions(target,QFileDevice::ReadOwner|QFileDevice::WriteOwner);
    }
    // Refresh the gallery where one is already installed for this user, and do
    // the whole installation where none is. Refreshing deliberately leaves the
    // PAM stacks alone: another tool owns them here.
    bool installed=false;
    QFile state(QStringLiteral("/etc/fpstudio-auth.json"));
    if(state.open(QIODevice::ReadOnly)) {
        const auto row=QJsonDocument::fromJson(state.readAll()).object();
        installed=row.value("installed").toBool()&&row.value("username").toString()==user;
    }
    const QString script=installerScript();
    if(script.isEmpty()) {
        QMessageBox::warning(this,tr("Cannot install"),
            tr("Could not find the installer (tools/auth_install.py)."));
        return;
    }
    QStringList args{QStringLiteral("/usr/bin/python"),script};
    if(installed)args<<QStringLiteral("--refresh-gallery");
    args<<QStringLiteral("--user")<<user
        <<QStringLiteral("--reference-dir")<<m_stage->path()
        <<QStringLiteral("--enable-experimental-auth")<<QStringLiteral("--apply");
    m_install->setEnabled(false);
    m_instruction->setText(tr("Installing the enrolment…"));
    m_installer.start(QStringLiteral("/usr/bin/pkexec"),args);
}


// One line per frame, so that a contact can be replayed afterwards. Numbers
// only - no image data - and 0600 like everything else this window writes.
void LiveWindow::traceFrame(bool touch, qint64 stamp, double signal, int coverage,
                            int sharpness, double motion, bool eligible)
{
    if(!m_trace) {
        m_trace=std::make_unique<QFile>(m_dir+"/frame-trace.csv");
        if(!m_trace->open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text)) { m_trace.reset(); return; }
        m_trace->setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner);
        m_trace->write("contact,ms_into_contact,touch,signal,coverage,sharpness,motion,eligible\n");
    }
    if(touch&&!m_contactStart) m_contactStart=stamp;
    if(!touch) m_contactStart=0;
    const qint64 into=m_contactStart?(stamp-m_contactStart)/1000:-1;
    m_trace->write(QStringLiteral("%1,%2,%3,%4,%5,%6,%7,%8\n")
        .arg(m_touch).arg(into).arg(touch?1:0)
        .arg(signal,0,'f',1).arg(coverage).arg(sharpness)
        .arg(motion,0,'f',2).arg(eligible?1:0).toLatin1());
}

void LiveWindow::saveMap()
{
    if(m_mapPixel.isEmpty()||m_mapPlaced<2)return;
    int x0=kMapSize,y0=kMapSize,x1=0,y1=0;long covered=0;
    for(int y=0;y<kMapSize;++y)for(int x=0;x<kMapSize;++x)
        if(m_mapScore[y*kMapSize+x]>0){
            ++covered;
            x0=std::min(x0,x);x1=std::max(x1,x);y0=std::min(y0,y);y1=std::max(y1,y);
        }
    if(covered<100)return;
    const int w=x1-x0+1,h=y1-y0+1;
    QImage map(w,h,QImage::Format_Grayscale8);
    // Written a scanline at a time rather than through setPixel. On a
    // Grayscale8 image setPixel takes a packed colour, not a grey level, so a
    // plain integer v arrives as qGray(0,0,v) = 5*v/32 - the first saved map
    // came out with a maximum of 39 where the captures it was built from reach
    // 255. The bytes are the grey levels here, so write them.
    const QVector<float> shown=restoreDetail(m_mapPixel,m_mapScore,w,h,kMapSize,x0,y0);
    for(int y=0;y<h;++y) {
        uchar *row=map.scanLine(y);
        for(int x=0;x<w;++x) {
            const int idx=(y0+y)*kMapSize+x0+x;
            row[x]=m_mapScore[idx]>0?uchar(std::clamp(int(shown[y*w+x]),0,255)):0;
        }
    }
    QSaveFile file(m_dir+QStringLiteral("/coverage-map.png"));
    if(file.open(QIODevice::WriteOnly)) {
        file.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner);
        if(map.save(&file,"PNG"))file.commit();
    }
    // Beside it, what it is and is not, so the image is never found on its own
    // without the part that says it decides nothing.
    QSaveFile side(m_dir+QStringLiteral("/coverage-map.json"));
    if(side.open(QIODevice::WriteOnly)) {
        side.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner);
        const QJsonObject meta{
            {"research_only",true},{"authentication_decision",QJsonValue::Null},
            {"biometric_security_validated",false},
            {"role","enrolment-coverage-guide"},
            {"note","Assembled from this session's captures to show which parts of "
                    "the finger were reached. Registration between separate touches "
                    "is accurate to about 5.5px on a sensor whose ridges repeat every "
                    "8-10px, which is enough to see coverage and not enough to match "
                    "against - an assembled map scored 0.29 where the raw captures "
                    "scored 0.77. The session's output is the captures, not this."},
            {"fusion","feathered-weighted-mean"},{"detail_restored",true},
            {"registration",m_placer?"gpu-rotation-search":"cpu-translation-search"},
            {"angle_span_degrees",m_placer?QJsonValue(kMapAngleSpan):QJsonValue(0.0)},
            {"restoration","unsharp sigma 1.0 amount 0.5, returning the 3.5-7px band to a single capture's level"},
            {"placed",m_mapPlaced},{"contacts",m_candidates},{"rejected",m_mapRejects},
            {"placed_tracked",m_trackedPlacements},{"placed_anchor",m_anchorPlacements},
            {"rejected_smeared",m_rejectSmeared},{"rejected_unregistered",m_rejectUnregistered},
            {"rejected_ambiguous",m_rejectAmbiguous},{"rejected_no_overlap",m_rejectNoOverlap},
            {"min_fit",kMapMinFit},{"min_peak_gap",kMapMinPeakGap},{"quality_metric","ridge-sharpness"},{"min_ridge_sharpness",kMapMinSharpness},
            {"map_size",QJsonArray{w,h}},
            {"covered_vs_sensor",double(covered)/(108.0*88.0)}};
        side.write(QJsonDocument(meta).toJson());
        side.commit();
    }
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
            m_instruction->setText(tr("Capture finished — lift your finger. %1 remaining contacts are still being compared").arg(m_pending.size()+(m_gpuBusy?1:0)));
            m_stop->setText(tr("Stop"));
        } else {
            m_stop->setText(tr("Close"));
            // The session is over and its captures are on disk, so enrolling
            // from them is now a single button rather than a separate tool.
            if(m_install&&m_installer.state()==QProcess::NotRunning)
                m_install->setVisible(m_mapPlaced>0&&m_candidates>0);
            m_instruction->setText(m_targetReached?tr("The target of %1 contacts has been compared — no need to repeat. You can lift your finger.").arg(m_targetContacts):m_stopping?tr("Stopped. You can lift your finger."):
                tr("Capture and candidate comparison have ended. See the per-contact totals below."));
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
        {"frames",m_total},{"dropped_frames",m_dropped},{"longest_frame_gap",m_longestGap},{"touches",m_touch},{"candidates",m_candidates},
        {"target_contacts",m_targetContacts},{"target_reached",m_targetReached},
        {"scan_seconds_left",m_scanStart&&!m_finished?qMax<qint64>(0,(kScanBudgetMs-(now-m_scanStart))/1000):-1},
        {"stop_reason",m_thermalStop?QJsonValue(QStringLiteral("sensor-duty-limit")):m_targetReached?QJsonValue(QStringLiteral("target-reached")):m_stopping?QJsonValue(QStringLiteral("user-stopped")):QJsonValue(QJsonValue::Null)},
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
    if(m_finished)m_metrics->setText(tr("Capture finished · %1 frames · %2 contacts detected\n%3 comparisons answered · %4 ridge-consistent · %5 not compared (%6 with no candidate) · %7 queued · %8 errors\nResearch results on selected data, not an authentication success rate")
        .arg(m_total).arg(m_touch).arg(compared).arg(consistent).arg(m_touch-compared).arg(noCandidate).arg(m_pending.size()).arg(m_gpuErrors));
    m_lastStatusWrite=now;
}
void LiveWindow::start()
{
    m_started = true;
    m_scanStart = m_clock.elapsed();
    m_instruction->setText(tr("Keep your finger off — connecting to the sensor and calibrating the background"));
    // The child inherits the language already selected by the application.
    // Forcing Korean here made an otherwise localized session switch language
    // as soon as live capture started.
    m_worker.start(m_program, {"--cli", "capture", "--timeout", "195", "--out", m_dir + "/unused.png"});
}
// A pressure gauge, because contact area is the one thing that reliably decides
// whether a capture is usable and the one thing nobody can feel accurately.
//
// Over 540 captures from nine sessions the ridge coherence of the saved image
// falls monotonically with contact area - 0.925 below 76, 0.892 at 77-78, 0.785
// at 79-80, 0.654 above 80 - and the correlation is between -0.78 and -0.94 in
// every session taken separately. Pressing harder puts more of the finger on
// the glass and flattens the ridges into it, so there is more contact and less
// pattern.
//
// Time correlates with nothing: across the same sessions it ranges from -0.82 to
// +0.71 and changes sign. A session that "got worse as it went" was one where
// the pressure crept up, and a session that got better was one where it crept
// down. That is invisible to the person pressing, and it is the whole spread
// between a good session and a bad one, so it gets shown rather than inferred.
static QString pressureGauge(int coverage)
{
    if(coverage<=0)return QString();
    // 70 to 84 covers the whole range the sensor reports in practice.
    const int span=14,lo=70;
    const int filled=std::clamp((coverage-lo)*span/span,0,span);
    QString bar;
    for(int i=0;i<span;++i){
        const int at=lo+i;
        bar+=i<filled?(at>=79?QStringLiteral("\u2588"):at>=78?QStringLiteral("\u2593"):QStringLiteral("\u2592"))
                     :QStringLiteral("\u00b7");
    }
    // A free function, so it cannot use tr(); named into LiveWindow's context
    // so these strings sit with the rest of the window's text.
    const auto T=[](const char *text){return QCoreApplication::translate("LiveWindow",text);};
    const QString verdict=coverage<=77?T("good")
                         :coverage<=78?T("slightly hard")
                         :coverage<=79?T("hard — ease off")
                                      :T("too hard — just rest it on");
    return T("Pressure %1 %2  %3   (target 77 or below)")
        .arg(bar).arg(coverage,2).arg(verdict);
}

void LiveWindow::tick()
{
    if (m_finished || m_stopping) return;
    const qint64 now = m_clock.elapsed();
    if (!m_started) {
        m_instruction->setText(tr("Lift your finger — background calibration in %1 s").arg(qMax<qint64>(1, (m_prepareMs - now + 999) / 1000)));
        if (now >= m_prepareMs) start();
        return;
    }
    if (!m_lastArrival) {
        QFile beacon(m_dir + "/capture-status.json");
        if (beacon.open(QIODevice::ReadOnly)) {
            const auto state = QJsonDocument::fromJson(beacon.readAll()).object();
            if (state.value("stage").toString() == "background-ready")
                m_instruction->setText(tr("Keep your finger off — collecting more background from the live stream"));
        }
        if (now > m_prepareMs + 15000)
            m_metrics->setText(tr("0 FPS · waiting for the first image · %1 s").arg((now - m_prepareMs) / 1000));
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
                if (m_lastStamp && stamp > m_lastStamp + 1) {
                    const qint64 gap = stamp - m_lastStamp - 1;
                    m_dropped += gap;
                    if (gap > m_longestGap) m_longestGap = gap;
                }
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
                    m_mapAnchored=false;
                    m_contacts[m_touch]=QJsonObject{{"start_timestamp_us",stamp}};
                }
                m_contactOn=touch;
                m_emptyFrames = touch ? 0 : m_emptyFrames + 1;
                const auto quality = assessPreview(frame, m_previous, touch?100:0, fields[3].toInt(), fields[4].toInt());
                m_previous = frame;
                traceFrame(touch, stamp, fields[2].toDouble(), fields[3].toInt(),
                           fields[4].toInt(), quality.motion, quality.eligible);
                m_stable = quality.eligible ? m_stable + 1 : 0;
                if(touch) {
                    auto &contact=m_contacts[m_touch];
                    contact.insert("frames",contact.value("frames").toInt()+1);
                    contact.insert("last_quality_reason",quality.reason);
                    contact.insert("last_timestamp_us",stamp);
                    if(quality.eligible)contact.insert("eligible_frames",contact.value("eligible_frames").toInt()+1);
                    // Every usable frame, not one per touch. The map used to be
                    // assembled from the single best frame of each contact,
                    // which means every join in it was made across a lift - the
                    // one place registration is poor. The frames between are
                    // where the finger is moving a few pixels at a time, which
                    // is where it is accurate, and they were being discarded.
                    const qint64 placeEvery=std::max<qint64>(kMapPlaceIntervalMs,m_placeCostMs*2);
                    if(quality.eligible&&m_mapFastPath&&now-m_lastPlaceMs>=placeEvery) {
                        m_lastPlaceMs=now;
                        const qint64 startedAt=QDateTime::currentMSecsSinceEpoch();
                        placeOnMap(frame);
                        m_placeCostMs=QDateTime::currentMSecsSinceEpoch()-startedAt;
                        // Keep a capture every so often along the sweep.
                        //
                        // "The best frame of each contact" was written when a
                        // contact meant one press of one spot. Sliding without
                        // lifting is the whole point now, and it makes a
                        // session of a few contacts - one measured run covered
                        // 7.4 times the sensor and produced ten captures, two
                        // of which had a place on the map. The enrolment set
                        // has to be sampled along the path the finger took,
                        // not once per time it was put down.
                        const int dx=m_mapLastX-m_lastSavedX,dy=m_mapLastY-m_lastSavedY;
                        const bool moved=!m_haveSavedPose||
                            dx*dx+dy*dy>=kSweepSampleStride*kSweepSampleStride;
                        if(m_mapOutcome==MapOutcome::Placed&&moved&&m_saved.size()<128) {
                            const QString path=m_dir+QStringLiteral("/candidate-sweep-%1.png")
                                .arg(m_candidateIndex,3,10,QLatin1Char('0'));
                            QSaveFile sample(path);
                            if(sample.open(QIODevice::WriteOnly)) {
                                sample.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner);
                                if(frame.save(&sample,"PNG")&&sample.commit()) {
                                    ++m_candidateIndex;
                                    m_saved.push_back({path,m_mapLastX,m_mapLastY,m_mapLastQuality});
                                    m_lastSavedX=m_mapLastX;m_lastSavedY=m_mapLastY;
                                    m_haveSavedPose=true;
                                    QSaveFile note(path+QStringLiteral(".json"));
                                    if(note.open(QIODevice::WriteOnly)) {
                                        note.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner);
                                        note.write(QJsonDocument(QJsonObject{
                                            {"role","unvalidated-candidate"},
                                            {"source","sweep"},
                                            {"instructed_role",m_differentFinger?"different-finger":"genuine"},
                                            {"finger_label","unconfirmed"},
                                            {"contact_group",m_touch},
                                            {"sensor_timestamp_us",stamp},
                                            {"map_x",m_mapLastX},{"map_y",m_mapLastY},
                                            {"map_angle",m_mapLastAngle},{"map_fit",m_mapLastFit},
                                            {"map_quality",m_mapLastQuality}}).toJson());
                                        note.commit();
                                    }
                                }
                            }
                        }
                    }
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
                            // Where this capture sits on the finger, kept with
                            // it. Every frame of the contact was just placed,
                            // including this one, so the last pose recorded is
                            // this frame's. Selecting an enrolment set later
                            // needs to know which parts of the finger each
                            // capture speaks for, not merely how sharp it is -
                            // sixty sharp views of one knuckle are worth less
                            // than a dozen ordinary ones that cover the whole
                            // print.
                            // Without the GPU path this is the only placement
                            // the contact gets, so it happens here.
                            if(!m_mapFastPath)placeOnMap(frame);
                            if(m_mapOutcome==MapOutcome::Placed) {
                                m_contacts[m_touch].insert("map_x",m_mapLastX);
                                m_contacts[m_touch].insert("map_y",m_mapLastY);
                                m_contacts[m_touch].insert("map_angle",m_mapLastAngle);
                                m_contacts[m_touch].insert("map_fit",m_mapLastFit);
                                m_contacts[m_touch].insert("map_quality",m_mapLastQuality);
                            }
                            matchFrame(frame);
                            QSaveFile metadata(path + ".json");
                            if (metadata.open(QIODevice::WriteOnly)) {
                                metadata.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
                                metadata.write(QJsonDocument(QJsonObject{{"role", "unvalidated-candidate"},
                                    // Which run this frame came out of, recorded per frame.
                                    //
                                    // A genuine session and a different-finger session write identically
                                    // named files into identically shaped directories, and until now the
                                    // only difference was the flag on a command line that is gone by the
                                    // time anyone looks at the data. A frame whose role has to be
                                    // reconstructed from a directory name and somebody's memory is a frame
                                    // that can be labelled backwards, and a backwards impostor sample
                                    // teaches the exact opposite of the truth with nothing downstream able
                                    // to notice. It costs one field to make that impossible.
                                    //
                                    // This records what the run was *instructed* to be, which is not the
                                    // same as what finger was actually presented - hence finger_label
                                    // staying unconfirmed beside it.
                                    {"instructed_role", m_differentFinger ? "different-finger" : "genuine"},
                                    {"finger_label", "unconfirmed"}, {"contact_group", m_touch},
                                    {"sensor_timestamp_us", stamp}, {"ranking_version", 1},
                                    {"score", quality.score}, {"coherence", quality.coherence}, {"motion", quality.motion},
                                    {"signal", fields[2].toDouble()}, {"coverage", fields[3].toInt()},
                                    {"sharpness", fields[4].toInt()},
                                    // Where it sits on the finger. Written here rather
                                    // than kept in memory so that the enrolment set a
                                    // session would have chosen can still be worked out
                                    // after the window is gone - and so that choosing it
                                    // can be tested without one.
                                    {"map_x", m_mapOutcome==MapOutcome::Placed?QJsonValue(m_mapLastX):QJsonValue()},
                                    {"map_y", m_mapOutcome==MapOutcome::Placed?QJsonValue(m_mapLastY):QJsonValue()},
                                    {"map_angle", m_mapOutcome==MapOutcome::Placed?QJsonValue(m_mapLastAngle):QJsonValue()},
                                    {"map_fit", m_mapOutcome==MapOutcome::Placed?QJsonValue(m_mapLastFit):QJsonValue()},
                                    {"map_quality", m_mapOutcome==MapOutcome::Placed?QJsonValue(m_mapLastQuality):QJsonValue()}}).toJson());
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
                if(!m_calibrated)guidance=tr("Keep your finger off and hold — collecting the background over several frames (do not touch the sensor yet)");
                else if(!touch)guidance=tr("No finger detected — now rest %1 lightly in the centre of the sensor").arg(m_fingerPrompt);
                else if(m_differentFinger&&m_contacts[m_touch].value("compared").toBool())
                    guidance=m_contacts[m_touch].value("consistent").toBool()?
                        tr("Warning: the other finger also produced ridge-match evidence. A suspected false accept has been recorded — lift and touch again"):
                        tr("This other finger produced no match evidence. Recorded — lift completely and touch again");
                else if(m_contacts[m_touch].value("consistent").toBool())
                    guidance=tr("This contact's ridges were confirmed — lift completely and rest %1 on the sensor again (not an authentication decision)").arg(m_fingerPrompt);
                else if(m_pending.contains(m_touch)||(m_gpuBusy&&m_gpuContact==m_touch))
                    guidance=tr("A sharp image was captured — comparing automatically. You can lift your finger");
                else if(m_contacts[m_touch].value("compared").toBool())
                    guidance=m_contacts[m_touch].value("submitted").toInt()>=3?
                        tr("Three comparisons of this contact found too little evidence — lift completely, then place %1 in the centre again").arg(m_fingerPrompt):
                        tr("Too little match evidence — move %1 slightly toward the centre and press more lightly. It will be compared again automatically").arg(m_fingerPrompt);
                else if(fields[3].toInt()<62)guidance=tr("The contact area is small — lay the pad of %1 flat across the centre of the sensor").arg(m_fingerPrompt);
                else if(fields[3].toInt()>=79)guidance=tr("You are pressing hard — ease off and just rest the finger. Quality falls below 0.79 in this range");
                else if(quality.motion>18)guidance=tr("Hold your finger still for a moment — a stable image will be selected automatically");
                else if(!quality.eligible)guidance=tr("The ridges are unclear — ease off a little and shift %1 slightly").arg(m_fingerPrompt);
                else guidance=tr("Hold it there for a moment — selecting a sharp image automatically");
                if(m_differentFinger)guidance=tr("\u26a0 Use a different finger · ")+guidance;
                const auto skipReason=m_contacts.value(m_touch).value("skip_reason").toString();
                if(touch&&(skipReason=="candidate-storage-limit"||skipReason=="session-comparison-limit"))
                    guidance=tr("The research storage and comparison limit has been reached — this contact will not be compared further. Please stop");
                if(m_gpuFailed)guidance=tr("GPU comparison is unavailable — showing the image only. Please stop");
                // Say it where it can still be acted on: the contact is over,
                // but the place it was meant to cover is still empty.
                if(touch&&m_mapOutcome==MapOutcome::TooSmeared)
                    guidance=tr("\u26a0 The ridges were smeared, so this was not added to the map — the red outline on the map is where it would have gone. "
                        "Ease off and press the same spot again")+QStringLiteral(" · ")+guidance;
                else if(touch&&m_mapOutcome==MapOutcome::NotRegistered)
                    guidance=tr("\u26a0 This could not be fitted to the map — press so that it overlaps an already filled area by about half")
                        +QStringLiteral(" · ")+guidance;
                else if(touch&&m_mapOutcome==MapOutcome::NoOverlap)
                    guidance=tr("\u26a0 This does not overlap anything already on the map — continue from the edge of a filled area")
                        +QStringLiteral(" · ")+guidance;
                if(m_targetContacts>0) {
                    int completed=0;for(const auto &c:m_contacts)if(c.value("compared").toBool())++completed;
                    guidance=tr("[%1/%2 contacts compared · stops automatically at the target] ").arg(completed).arg(m_targetContacts)+guidance;
                }
                m_instruction->setText(guidance);
                m_metrics->setText(pressureGauge(fields[3].toInt())+QStringLiteral("\n")
                    +tr("%1 FPS · %2 frames · ridge signal %3 · contrast area %4% · sharpness %5\n")
                    .arg(m_fps, 0, 'f', 1).arg(m_total).arg(QString::fromLatin1(fields[2]))
                    .arg(QString::fromLatin1(fields[3])).arg(QString::fromLatin1(fields[4]))
                    + tr("%1 · %2 contacts · %3 candidates · %4 GPU comparisons done / %5 contacts queued (not validated for authentication)")
                        .arg(quality.reason).arg(m_touch).arg(m_candidates).arg(m_gpuCompleted).arg(m_pending.size())+
                    tr("\nRaw difference %1 · background noise %2 · %3").arg(m_rawSignal,0,'f',1).arg(m_noise,0,'f',1)
                        .arg(!m_calibrated?tr("collecting background"):touch?tr("finger in contact"):tr("no finger")));
            }
        }
    }
    if (m_lastArrival && now - m_lastArrival > 1500) {
        m_instruction->setText(tr("The image stream has stopped — the screen shows the last frame"));
        m_metrics->setText(tr("0 FPS · no new image for %1 s").arg((now - m_lastArrival) / 1000.0, 0, 'f', 1));
    }
    // Spend the thermal budget rather than run into it. See kScanBudgetMs.
    if(m_scanStart&&!m_finished&&!m_stopping) {
        const qint64 left=kScanBudgetMs-(now-m_scanStart);
        if(left<=0) {
            m_instruction->setText(QStringLiteral(
                "센서 연속 사용 한도에 도달해 이번 세션을 정상 종료합니다 — "
                "결과와 지도는 모두 저장됩니다. 약 3분 쉬었다 다시 시작해 주세요"));
            m_thermalStop=true;
            stop(true);
            return;
        }
        if(left<=30000)
            m_metrics->setText(m_metrics->text()+QStringLiteral(
                "\n센서 연속 사용 %1초 남음 — 남은 빈 곳을 먼저 채워 주세요").arg(left/1000));
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
        tr("GPU comparison finished · %1 processed · stopped normally on reaching the target of %2 contacts\nThe results and colour-marked images have been saved. This did not decide whether authentication is granted.").arg(m_gpuCompleted).arg(m_targetContacts):
        tr("GPU comparison stopped · %1 finished · %2 cancelled in progress\nStopped at your request. Saved results are kept.").arg(m_gpuCompleted).arg(m_gpuCancelled));
    m_instruction->setText(tr("Stopping — lift your finger"));
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
