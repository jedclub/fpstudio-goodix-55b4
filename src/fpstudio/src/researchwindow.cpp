#include "researchwindow.h"
#include "beacon.h"
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLockFile>
#include <QPixmap>
#include <QProgressBar>
#include <QProcessEnvironment>
#include <QPushButton>
#include <QRegularExpression>
#include <QSaveFile>
#include <QStandardPaths>
#include <QVBoxLayout>

namespace fpstudio {
namespace {
constexpr int total = 14; // 6 references, 4 held-out genuine, 4 other-finger probes
bool saveJson(const QString &path, const QJsonObject &object)
{
    QSaveFile f(path);
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    if (f.write(QJsonDocument(object).toJson()) < 0) return false;
    return f.commit();
}
}

ResearchWindow::ResearchWindow(const QString &directory, QWidget *parent,
                               const QString &captureProgram, int prepareMs, int reviewMs, bool autoRun)
    : QDialog(parent), m_dir(directory),
      m_captureProgram(captureProgram.isEmpty() ? QCoreApplication::applicationFilePath() : captureProgram),
      m_prepareMs(prepareMs), m_reviewMs(reviewMs)
{
    setWindowTitle(tr("fpstudio · fingerprint research session"));
    resize(920, 720);
    setStyleSheet(QStringLiteral("QDialog {background:#101827;color:#e5edf8;} QLabel {color:#e5edf8;}"
                                 "QPushButton {padding:12px 24px;font-size:16px;}"));
    auto *layout = new QVBoxLayout(this);
    m_heading = new QLabel; m_heading->setStyleSheet("font-size:20px;font-weight:bold;");
    m_progress = new QProgressBar; m_progress->setRange(0, total);
    m_instructionLabel = new QLabel; m_instructionLabel->setWordWrap(true);
    m_instructionLabel->setMinimumHeight(90);
    m_instructionLabel->setStyleSheet("font-size:26px;font-weight:bold;padding:16px;background:#1d4ed8;border-radius:8px;");
    m_detailLabel = new QLabel; m_detailLabel->setWordWrap(true);
    m_detailLabel->setMinimumHeight(58); m_detailLabel->setStyleSheet("font-size:17px;color:#b9cbe5;");
    m_preview = new QLabel(tr("The captured fingerprint image will appear here"));
    m_preview->setAlignment(Qt::AlignCenter); m_preview->setMinimumSize(432, 352);
    m_preview->setStyleSheet("background:#060b12;border:1px solid #344155;border-radius:8px;");
    m_quality = new QLabel(tr("No image · nothing captured yet"));
    m_quality->setAlignment(Qt::AlignCenter); m_quality->setStyleSheet("font-size:16px;");
    m_stop = new QPushButton(tr("Stop"));
    auto *buttons = new QHBoxLayout; buttons->addStretch(); buttons->addWidget(m_stop);
    layout->addWidget(m_heading); layout->addWidget(m_progress); layout->addWidget(m_instructionLabel);
    layout->addWidget(m_detailLabel); layout->addWidget(m_preview, 1); layout->addWidget(m_quality); layout->addLayout(buttons);

    m_lock = std::make_unique<QLockFile>(QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation)
                                        + QStringLiteral("/fpstudio-research.lock"));
    if (!m_lock->tryLock()) {
        m_instructionLabel->setText(tr("Another research session is already running"));
        m_stop->setText(tr("Close"));
        connect(m_stop, &QPushButton::clicked, this, &QDialog::reject);
        return;
    }
    if (!QDir().mkpath(m_dir)) {
        m_instructionLabel->setText(tr("Could not create the save folder"));
        connect(m_stop, &QPushButton::clicked, this, &QDialog::reject);
        return;
    }
    QFile::setPermissions(m_dir, QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner);
    m_capture.setProcessChannelMode(QProcess::SeparateChannels);
    auto environment = QProcessEnvironment::systemEnvironment();
    environment.insert("FPSTUDIO_STATUS_PATH", m_dir + "/capture-status.json");
    m_capture.setProcessEnvironment(environment);
    connect(&m_capture, &QProcess::readyReadStandardOutput, this, [this] { m_output += m_capture.readAllStandardOutput(); });
    connect(&m_capture, &QProcess::readyReadStandardError, this, [this] { m_errors += m_capture.readAllStandardError(); });
    connect(&m_capture, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &ResearchWindow::finished);
    connect(&m_capture, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
        if (error == QProcess::FailedToStart) {
            announce("paused", tr("Could not start the capture program"), m_capture.errorString());
        }
    });
    connect(m_stop, &QPushButton::clicked, this, &ResearchWindow::stop);
    connect(&m_tick, &QTimer::timeout, this, &ResearchWindow::tick);
    m_tick.start(150);
    announce("ready", tr("Get your right index finger ready. Do not touch the sensor yet."),
             tr("Plan: 6 reference images → 4 verification images of the same finger → 4 comparison images of the right middle finger.\n"
                "No buttons to press. Follow the finger and position prompts on screen. You can stop at any time."));
    if (autoRun) QTimer::singleShot(qMin(m_prepareMs, 1500), this, &ResearchWindow::prepare);
}

ResearchWindow::~ResearchWindow()
{
    if (m_capture.state() != QProcess::NotRunning) { m_capture.kill(); m_capture.waitForFinished(2000); }
}

QString ResearchWindow::phase() const
{
    return m_index < 6 ? tr("Reference images") : m_index < 10 ? tr("Separate verification images") : tr("Different-finger comparison");
}
QString ResearchWindow::placement() const
{
    if (m_index >= 10) return tr("the broad pad of your right middle finger, centred");
    if (m_index == 4 || m_index == 8) return tr("your right index finger, a touch left of centre");
    if (m_index == 5 || m_index == 9) return tr("your right index finger, a touch right of centre");
    return tr("the broad pad of your right index finger, centred");
}
QString ResearchWindow::filename() const
{
    const QString role = m_index < 6 ? "reference-index" : m_index < 10 ? "probe-index" : "probe-middle";
    return QStringLiteral("%1/%2-%3.png").arg(m_dir, role).arg(m_index + 1, 2, 10, QLatin1Char('0'));
}
void ResearchWindow::announce(const QString &state, const QString &instruction, const QString &detail)
{
    m_state = state; m_instruction = instruction; m_detail = detail;
    m_instructionLabel->setText(instruction); m_detailLabel->setText(detail);
    m_heading->setText(tr("%1 · saved %2 / %3").arg(phase()).arg(m_index).arg(total));
    m_progress->setValue(m_index);
    persist();
}
void ResearchWindow::persist()
{
    if (!m_lock || !m_lock->isLocked()) return;
    saveJson(m_dir + "/session.json", QJsonObject{
        {"pid", QCoreApplication::applicationPid()}, {"state", m_state}, {"instruction", m_instruction},
        {"detail", m_detail}, {"completed", m_index}, {"total", total},
        {"samples", m_samples}, {"attempts", m_attempts},
        {"updated_at", QDateTime::currentDateTimeUtc().toString(Qt::ISODate)}});
}
void ResearchWindow::prepare()
{
    if (m_cancelled || m_index >= total || m_capture.state() != QProcess::NotRunning) return;
    m_ready = false;
    m_deadline = QDateTime::currentMSecsSinceEpoch() + m_prepareMs;
    announce("prepare", tr("① Lift your finger clear of the sensor · 5 s"),
             tr("The empty sensor is measured first. Wait until this changes to 'Touch now'."));
}
void ResearchWindow::capture()
{
    m_path = filename(); m_output.clear(); m_errors.clear(); ++m_attempt;
    announce("calibrating", tr("① Keep it off and wait · preparing the sensor"),
             tr("This changes to 'Touch now' automatically once the background measurement is done."));
    m_capture.start(m_captureProgram,
                    {"--cli", "capture", "--timeout", "30", "--out", m_path});
}
void ResearchWindow::tick()
{
    if (QFileInfo::exists(m_dir + "/stop") && !m_cancelled) { stop(); return; }
    if (m_state == "paused" && QFileInfo::exists(m_dir + "/resume")) {
        QFile::remove(m_dir + "/resume");
        m_retries = 0; prepare(); return;
    }
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (m_state == "prepare") {
        const int seconds = qMax(0, int((m_deadline - now + 999) / 1000));
        m_instructionLabel->setText(tr("① Lift your finger clear · %1 s").arg(seconds));
        if (!seconds) capture();
    } else if (m_state == "phase-ready") {
        const int seconds = qMax(0, int((m_deadline - now + 999) / 1000));
        m_instructionLabel->setText(tr("%1 · sensor ready in %2 s").arg(m_instruction).arg(seconds));
        if (!seconds) prepare();
    } else if (m_state == "review" || m_state == "retry") {
        if (now >= m_deadline) {
            if (m_state == "review" && (m_index == 6 || m_index == 10)) {
                announce("phase-ready", m_index == 6 ? tr("6 reference images done. Starting the separate verification.")
                                                      : tr("Now switch to your right middle finger."),
                         m_index == 6 ? tr("Four more images with the same right index finger, placed afresh. These are evaluated separately from the reference images.")
                                      : tr("Four images to check whether a different finger is wrongly accepted. Do not touch the sensor yet."));
                m_deadline = now + m_prepareMs;
            } else prepare();
        }
    } else if (m_capture.state() != QProcess::NotRunning && !m_cancelled) {
        QFile f(m_dir + "/capture-status.json");
        if (!f.open(QIODevice::ReadOnly)) return;
        const auto b = QJsonDocument::fromJson(f.readAll()).object();
        if (b.value("pid").toInteger() != m_capture.processId()) return;
        const QString stage = b.value("stage").toString();
        if (stage == "background-ready" && !m_ready) {
            m_ready = true;
            announce("await-touch", tr("② Touch now — %1").arg(placement()),
                     tr("Cover the whole sensor, press lightly and hold. Lift when the prompt says so, after the image appears."));
        } else if ((stage == "finger-on" || stage == "captured") && m_state != "hold") {
            announce("hold", tr("③ Hold your finger still · capturing"),
                     tr("Contact confirmed. Do not move your finger."));
        } else if (stage == "await-finger-off" && m_state != "release") {
            announce("release", tr("④ Captured. Lift your finger clear."),
                     tr("The image and its quality will be shown in a moment."));
        }
    }
}
void ResearchWindow::finished(int code, QProcess::ExitStatus exitStatus)
{
    m_output += m_capture.readAllStandardOutput(); m_errors += m_capture.readAllStandardError();
    QJsonObject result = QJsonDocument::fromJson(m_output).object();
    const bool ok = code == 0 && exitStatus == QProcess::NormalExit && result.value("ok").toBool();
    saveJson(m_dir + QStringLiteral("/attempt-%1.json").arg(m_attempt, 3, 10, QLatin1Char('0')), result);
    int coverage = -1, sharpness = -1; double background = -1;
    const QRegularExpression quality("Image quality: coverage (\\d+)%, sharpness (\\d+)");
    const QRegularExpression diff("diff vs background = ([0-9.]+)");
    for (const auto &line : result.value("log").toArray()) {
        const QString text = line.toObject().value("text").toString();
        const auto q = quality.match(text); const auto d = diff.match(text);
        if (q.hasMatch()) { coverage = q.captured(1).toInt(); sharpness = q.captured(2).toInt(); }
        if (d.hasMatch()) background = d.captured(1).toDouble();
    }
    m_attempts.append(QJsonObject{{"sample", m_index + 1}, {"ok", ok}, {"error", result.value("error")},
                                 {"coverage", coverage}, {"sharpness", sharpness}, {"background_diff", background}});
    if (m_cancelled) { persist(); return; }
    const QImage image(m_path);
    if (ok && !image.isNull()) {
        QFile::setPermissions(m_path, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
        m_preview->setPixmap(QPixmap::fromImage(image).scaled(540, 440, Qt::KeepAspectRatio, Qt::FastTransformation));
        m_quality->setText(tr("Image just captured · %1×%2 · contact %3% · sharpness %4 · background difference %5")
                           .arg(image.width()).arg(image.height()).arg(coverage).arg(sharpness).arg(background, 0, 'f', 1));
        m_samples.append(QJsonObject{{"path", m_path}, {"role", m_index < 6 ? "reference" : "probe"},
                                    {"finger", m_index < 10 ? "index" : "middle"},
                                    {"coverage", coverage}, {"sharpness", sharpness}, {"background_diff", background}});
        ++m_index; m_retries = 0;
        if (m_index == total) {
            announce("complete", tr("All 14 images collected — you can rest your hand."),
                     tr("The reference, verification and different-finger images were saved separately. The matching results are analysed next. Finishing collection is not a decision that authentication succeeded."));
            m_stop->setText(tr("Close")); return;
        }
        m_deadline = QDateTime::currentMSecsSinceEpoch() + m_reviewMs;
        announce("review", tr("④ Saved · %1/14 — lift your finger.").arg(m_index),
                 tr("Below is the image just captured. The next capture is prepared automatically in 4 seconds."));
    } else {
        ++m_retries;
        const QString error = result.value("error").toString(QString::fromUtf8(m_errors).right(250));
        m_deadline = QDateTime::currentMSecsSinceEpoch() + m_reviewMs;
        announce(m_retries < 3 ? "retry" : "paused", tr("The capture failed — lift your finger."),
                 QStringLiteral("%1\n%2").arg(error, m_retries < 3 ? tr("The same sample will be retried automatically. Do not touch the sensor yet.")
                                                                             : tr("Three failures in a row, so it has paused for diagnosis. Rest your hand. Collection resumes automatically once the cause is identified.")));
    }
}
void ResearchWindow::stop()
{
    if (m_state == "complete" || m_state == "cancelled") { accept(); return; }
    m_cancelled = true;
    m_stop->setText(tr("Close"));
    announce("cancelled", tr("Stopped. Lift your finger."), tr("Images already saved have been kept."));
    if (m_capture.state() != QProcess::NotRunning) m_capture.terminate();
}
void ResearchWindow::closeEvent(QCloseEvent *event)
{
    if (m_state != "complete") stop();
    event->accept();
}
}
