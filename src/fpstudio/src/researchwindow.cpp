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
    setWindowTitle(QStringLiteral("fpstudio · 지문 연구 세션"));
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
    m_preview = new QLabel(QStringLiteral("촬영 후 실제 지문 영상이 여기에 표시됩니다"));
    m_preview->setAlignment(Qt::AlignCenter); m_preview->setMinimumSize(432, 352);
    m_preview->setStyleSheet("background:#060b12;border:1px solid #344155;border-radius:8px;");
    m_quality = new QLabel(QStringLiteral("영상 없음 · 아직 촬영하지 않았습니다"));
    m_quality->setAlignment(Qt::AlignCenter); m_quality->setStyleSheet("font-size:16px;");
    m_stop = new QPushButton(QStringLiteral("중단"));
    auto *buttons = new QHBoxLayout; buttons->addStretch(); buttons->addWidget(m_stop);
    layout->addWidget(m_heading); layout->addWidget(m_progress); layout->addWidget(m_instructionLabel);
    layout->addWidget(m_detailLabel); layout->addWidget(m_preview, 1); layout->addWidget(m_quality); layout->addLayout(buttons);

    m_lock = std::make_unique<QLockFile>(QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation)
                                        + QStringLiteral("/fpstudio-research.lock"));
    if (!m_lock->tryLock()) {
        m_instructionLabel->setText(QStringLiteral("다른 연구 세션이 실행 중입니다"));
        m_stop->setText(QStringLiteral("닫기"));
        connect(m_stop, &QPushButton::clicked, this, &QDialog::reject);
        return;
    }
    if (!QDir().mkpath(m_dir)) {
        m_instructionLabel->setText(QStringLiteral("저장 폴더를 만들지 못했습니다"));
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
            announce("paused", QStringLiteral("캡처 프로그램을 시작하지 못했습니다"), m_capture.errorString());
        }
    });
    connect(m_stop, &QPushButton::clicked, this, &ResearchWindow::stop);
    connect(&m_tick, &QTimer::timeout, this, &ResearchWindow::tick);
    m_tick.start(150);
    announce("ready", QStringLiteral("오른쪽 검지를 준비하세요. 아직 센서에 대지 마세요."),
             QStringLiteral("계획: 기준 6장 → 같은 손가락 검증 4장 → 오른쪽 중지 비교 4장.\n"
                            "버튼을 누를 필요 없습니다. 화면의 손가락·위치 안내를 따라가세요. 중단은 언제든 가능합니다."));
    if (autoRun) QTimer::singleShot(qMin(m_prepareMs, 1500), this, &ResearchWindow::prepare);
}

ResearchWindow::~ResearchWindow()
{
    if (m_capture.state() != QProcess::NotRunning) { m_capture.kill(); m_capture.waitForFinished(2000); }
}

QString ResearchWindow::phase() const
{
    return m_index < 6 ? QStringLiteral("기준 영상") : m_index < 10 ? QStringLiteral("별도 검증 영상") : QStringLiteral("다른 손가락 비교");
}
QString ResearchWindow::placement() const
{
    if (m_index >= 10) return QStringLiteral("오른쪽 중지의 넓은 면을 중앙에");
    if (m_index == 4 || m_index == 8) return QStringLiteral("오른쪽 검지를 중앙에서 아주 조금 왼쪽으로");
    if (m_index == 5 || m_index == 9) return QStringLiteral("오른쪽 검지를 중앙에서 아주 조금 오른쪽으로");
    return QStringLiteral("오른쪽 검지의 넓은 면을 중앙에");
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
    m_heading->setText(QStringLiteral("%1 · 저장 %2 / %3").arg(phase()).arg(m_index).arg(total));
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
    announce("prepare", QStringLiteral("① 손가락을 센서에서 완전히 떼세요 · 5초"),
             QStringLiteral("빈 센서를 먼저 측정합니다. '지금 대세요'로 바뀔 때까지 기다리세요."));
}
void ResearchWindow::capture()
{
    m_path = filename(); m_output.clear(); m_errors.clear(); ++m_attempt;
    announce("calibrating", QStringLiteral("① 그대로 떼고 기다리세요 · 센서 준비 중"),
             QStringLiteral("배경 측정이 끝나면 자동으로 '지금 대세요'로 바뀝니다."));
    m_capture.start(m_captureProgram,
                    {"--lang", "ko", "--cli", "capture", "--timeout", "30", "--out", m_path});
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
        m_instructionLabel->setText(QStringLiteral("① 손가락을 완전히 떼세요 · %1초").arg(seconds));
        if (!seconds) capture();
    } else if (m_state == "phase-ready") {
        const int seconds = qMax(0, int((m_deadline - now + 999) / 1000));
        m_instructionLabel->setText(QStringLiteral("%1 · %2초 뒤 센서 준비").arg(m_instruction).arg(seconds));
        if (!seconds) prepare();
    } else if (m_state == "review" || m_state == "retry") {
        if (now >= m_deadline) {
            if (m_state == "review" && (m_index == 6 || m_index == 10)) {
                announce("phase-ready", m_index == 6 ? QStringLiteral("기준 영상 6장 완료. 별도 검증을 시작합니다.")
                                                      : QStringLiteral("이제 오른쪽 중지로 바꿔 주세요."),
                         m_index == 6 ? QStringLiteral("같은 오른쪽 검지를 새로 대어 4장을 촬영합니다. 이 영상은 기준 영상과 분리해 평가합니다.")
                                      : QStringLiteral("다른 손가락을 잘못 받아들이는지 확인할 4장입니다. 아직 센서에는 대지 마세요."));
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
            announce("await-touch", QStringLiteral("② 지금 대세요 — %1").arg(placement()),
                     QStringLiteral("센서 전체를 덮고 가볍게 눌러 유지하세요. 영상이 나온 뒤 '떼세요' 안내에 따라 떼면 됩니다."));
        } else if ((stage == "finger-on" || stage == "captured") && m_state != "hold") {
            announce("hold", QStringLiteral("③ 손가락을 그대로 유지하세요 · 촬영 중"),
                     QStringLiteral("실제 접촉이 확인됐습니다. 손가락을 움직이지 마세요."));
        } else if (stage == "await-finger-off" && m_state != "release") {
            announce("release", QStringLiteral("④ 촬영했습니다. 손가락을 완전히 떼세요."),
                     QStringLiteral("곧 촬영 영상과 품질을 표시합니다."));
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
        m_quality->setText(QStringLiteral("방금 촬영한 영상 · %1×%2 · 접촉률 %3% · 선명도 %4 · 배경 차이 %5")
                           .arg(image.width()).arg(image.height()).arg(coverage).arg(sharpness).arg(background, 0, 'f', 1));
        m_samples.append(QJsonObject{{"path", m_path}, {"role", m_index < 6 ? "reference" : "probe"},
                                    {"finger", m_index < 10 ? "index" : "middle"},
                                    {"coverage", coverage}, {"sharpness", sharpness}, {"background_diff", background}});
        ++m_index; m_retries = 0;
        if (m_index == total) {
            announce("complete", QStringLiteral("14장 수집 완료 — 손을 쉬세요."),
                     QStringLiteral("기준·검증·다른 손가락 영상이 분리 저장됐습니다. 이제 매칭 결과를 분석합니다. 수집 완료는 인증 성공 판정이 아닙니다."));
            m_stop->setText(QStringLiteral("닫기")); return;
        }
        m_deadline = QDateTime::currentMSecsSinceEpoch() + m_reviewMs;
        announce("review", QStringLiteral("④ 저장 완료 · %1/14 — 손가락을 떼세요.").arg(m_index),
                 QStringLiteral("아래는 방금 촬영한 실제 영상입니다. 4초 뒤 다음 촬영 준비로 자동 이동합니다."));
    } else {
        ++m_retries;
        const QString error = result.value("error").toString(QString::fromUtf8(m_errors).right(250));
        m_deadline = QDateTime::currentMSecsSinceEpoch() + m_reviewMs;
        announce(m_retries < 3 ? "retry" : "paused", QStringLiteral("촬영하지 못했습니다 — 손가락을 떼세요."),
                 QStringLiteral("%1\n%2").arg(error, m_retries < 3 ? QStringLiteral("같은 샘플을 자동으로 다시 시도합니다. 아직 손가락을 대지 마세요.")
                                                                             : QStringLiteral("3회 연속 실패하여 진단을 위해 멈췄습니다. 손을 쉬세요. 원인을 확인한 뒤 자동 수집을 재개합니다.")));
    }
}
void ResearchWindow::stop()
{
    if (m_state == "complete" || m_state == "cancelled") { accept(); return; }
    m_cancelled = true;
    m_stop->setText(QStringLiteral("닫기"));
    announce("cancelled", QStringLiteral("중단했습니다. 손가락을 떼세요."), QStringLiteral("이미 저장한 영상은 보존했습니다."));
    if (m_capture.state() != QProcess::NotRunning) m_capture.terminate();
}
void ResearchWindow::closeEvent(QCloseEvent *event)
{
    if (m_state != "complete") stop();
    event->accept();
}
}
