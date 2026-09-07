#include "mainwindow.h"

#include "i18n.h"
#include "setupwizard.h"

#include <signal.h>
#include <cerrno>

#include <QApplication>
#include <QByteArray>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>
#include <QGroupBox>
#include <QHeaderView>
#include <QJsonArray>
#include <QLabel>
#include <QListWidget>
#include <QPainter>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QSplitter>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QFileSystemWatcher>
#include <QFile>
#include <QJsonDocument>
#include <QTimer>
#include <QDateTime>

#include "beacon.h"

namespace fpstudio {

// Nearest-neighbour on purpose: this sensor is 88x108, and smoothing a frame
// that small hides exactly the ridge detail the tool exists to show.
class ImageView : public QWidget {
public:
    explicit ImageView(QWidget *parent = nullptr) : QWidget(parent)
    {
        setMinimumSize(300, 360);
    }

    void setImage(const QImage &img) { m_image = img; update(); }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.fillRect(rect(), QColor(24, 24, 27));

        if (m_image.isNull()) {
            p.setPen(QColor(130, 130, 140));
            p.drawText(rect(), Qt::AlignCenter,
                       tr("No frame yet\n\nUse Capture to pull an image from the sensor"));
            return;
        }

        const QSize target = m_image.size().scaled(size() - QSize(20, 20), Qt::KeepAspectRatio);
        const QRect where(QPoint((width() - target.width()) / 2,
                                 (height() - target.height()) / 2), target);
        p.setRenderHint(QPainter::SmoothPixmapTransform, false);
        p.drawImage(where, m_image);

        p.setPen(QColor(90, 90, 100));
        p.drawRect(where.adjusted(-1, -1, 0, 0));
        p.setPen(QColor(170, 170, 180));
        p.drawText(rect().adjusted(10, 8, -10, -8), Qt::AlignTop | Qt::AlignLeft,
                   QStringLiteral("%1 × %2").arg(m_image.width()).arg(m_image.height()));
    }

private:
    QImage m_image;
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    m_runner = new PrivRunner(this);
    connect(m_runner, &PrivRunner::finished, this, &MainWindow::onFinished);
    connect(m_runner, &PrivRunner::failed,   this, &MainWindow::onFailed);

    buildUi();
    buildLanguageMenu();
    wireActions();

    // QFileSystemWatcher drops the path when the file is replaced by rename,
    // which is exactly how Beacon writes it - so re-add on every change, and
    // poll slowly as a backstop for the window between unlink and create.
    m_beaconWatch = new QFileSystemWatcher(this);
    m_beaconWatch->addPath(Beacon::path());
    connect(m_beaconWatch, &QFileSystemWatcher::fileChanged,
            this, &MainWindow::onBeaconChanged);
    auto *poll = new QTimer(this);
    connect(poll, &QTimer::timeout, this, &MainWindow::onBeaconChanged);
    poll->start(700);

    // The elapsed counter ticks locally rather than waiting for the next
    // beacon write. A driver that is simply waiting for a finger emits nothing
    // for many seconds, and a frozen number reads as a frozen program.
    auto *tick = new QTimer(this);
    connect(tick, &QTimer::timeout, this, &MainWindow::tickElapsed);
    tick->start(500);

    // Enumeration only reads sysfs, so it needs no prompt.
    m_pending = QStringLiteral("devices");
    setBusy(true, tr("Device scan"));
    m_runner->run({QStringLiteral("devices")}, /*direct=*/true);
}

// A menu rather than a combo box in the toolbar: language is set once and
// then forgotten, and a control that is used once should not occupy space
// that the image and the quality bars can use instead.
//
// Switching rebuilds the window. Qt can retranslate a live widget tree via
// changeEvent(QEvent::LanguageChange), but only for text the widgets still
// own - every string this window computed and handed over (the verdict line,
// the stage list, the quality caption) would keep its old language until the
// next run produced it again. Rebuilding is a few milliseconds and leaves
// nothing half-translated.
void MainWindow::openSetupWizard()
{
    SetupWizard w(this);
    w.exec();
    // The wizard can change what the device list should say - a driver
    // installed, a permission granted - so do not leave a stale view behind.
    m_runner->run({QStringLiteral("devices")}, true);
}

void MainWindow::maybeOfferSetup()
{
    // Probing takes seconds and blocks, so it must not happen before the
    // window is on screen: a program that shows nothing for five seconds
    // after launch looks broken, whatever it is doing.
    QTimer::singleShot(0, this, [this] {
        // Only the steps that stand between here and unlocking count. The
        // udev rule is a convenience and image quality is a measurement, and
        // opening a wizard over either would be nagging rather than helping.
        for (const StepResult &r : probeAll()) {
            if (r.id == StepId::UdevRule || r.id == StepId::Capture)
                continue;
            if (r.state == StepState::Ok || r.state == StepState::Skipped)
                continue;
            openSetupWizard();
            return;
        }
    });
}

void MainWindow::buildLanguageMenu()
{
    QMenu *setup = menuBar()->addMenu(tr("Setup"));
    setup->addAction(tr("Set up fingerprint unlock…"), this,
                     &MainWindow::openSetupWizard);

    m_langMenu = menuBar()->addMenu(tr("Language"));

    auto *group = new QActionGroup(this);
    group->setExclusive(true);

    const QString now = i18n::current();
    for (const QString &tag : i18n::available()) {
        QAction *a = m_langMenu->addAction(i18n::displayName(tag));
        a->setCheckable(true);
        a->setChecked(tag == now);
        a->setData(tag);
        group->addAction(a);

        connect(a, &QAction::triggered, this, [this, tag] {
            if (tag == i18n::current())
                return;
            // Remember first: install() consults the stored value when asked
            // for an empty tag, and a crash between the two should leave the
            // choice the user just made, not the one before it.
            i18n::remember(tag);
            i18n::install(tag);

            // The banner and any in-flight result belong to the old language.
            // A rebuilt window starts empty, which is honest - it says nothing
            // rather than saying something stale.
            auto *fresh = new MainWindow;
            fresh->resize(size());
            fresh->move(pos());
            fresh->show();
            close();
            deleteLater();
        });
    }
}

void MainWindow::buildUi()
{
    setWindowTitle(tr("fpstudio — fingerprint sensor diagnostics"));
    resize(1200, 740);

    m_deviceList = new QListWidget;
    m_deviceList->setMaximumHeight(90);
    m_deviceInfo = new QLabel(tr("Looking for a device…"));
    m_deviceInfo->setWordWrap(true);
    m_deviceInfo->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto *devBox = new QGroupBox(tr("Device"));
    auto *devLay = new QVBoxLayout(devBox);
    m_btnRefresh = new QPushButton(tr("Rescan"));
    devLay->addWidget(m_deviceList);
    devLay->addWidget(m_deviceInfo);
    devLay->addWidget(m_btnRefresh);

    m_btnCapture = new QPushButton(tr("Capture — show image"));
    m_btnEnroll  = new QPushButton(tr("Enrol"));
    m_btnVerify  = new QPushButton(tr("Verify"));
    m_btnDelete  = new QPushButton(tr("Delete enrolment"));
    m_progress = new QProgressBar;
    m_progress->setRange(0, 0);            // indeterminate while a call runs
    m_progress->hide();

    auto *actBox = new QGroupBox(tr("Actions"));
    auto *actLay = new QVBoxLayout(actBox);
    for (QPushButton *b : {m_btnCapture, m_btnEnroll, m_btnVerify, m_btnDelete})
        actLay->addWidget(b);
    actLay->addWidget(m_progress);

    m_stageList = new QListWidget;
    auto *stageBox = new QGroupBox(tr("Progress"));
    auto *stageLay = new QVBoxLayout(stageBox);
    stageLay->addWidget(m_stageList);

    auto *left = new QWidget;
    auto *leftLay = new QVBoxLayout(left);
    leftLay->addWidget(devBox);
    leftLay->addWidget(actBox);
    leftLay->addWidget(stageBox, 1);
    left->setMaximumWidth(340);

    // The instruction strip. Hidden until something asks for the user, then
    // impossible to miss - this is the whole point of the beacon.
    m_banner = new QLabel;
    m_banner->setAlignment(Qt::AlignCenter);
    m_banner->setWordWrap(true);
    m_banner->setMinimumHeight(52);
    m_banner->setStyleSheet(QStringLiteral(
        "QLabel { background:#1d4ed8; color:white; border-radius:6px;"
        "         padding:10px; font-size:15px; font-weight:bold; }"));
    m_banner->hide();

    // The live line sits under the instruction rather than replacing it. A
    // capture that is waiting and a capture that has hung look identical
    // without this, which was the complaint that prompted it.
    m_liveStatus = new QLabel;
    m_liveStatus->setAlignment(Qt::AlignCenter);
    m_liveStatus->setStyleSheet(QStringLiteral(
        "QLabel { color:#93c5fd; padding:2px; font-size:13px; }"));
    m_liveStatus->hide();

    m_imageView = new ImageView;
    m_verdict = new QLabel(tr("—"));
    m_verdict->setAlignment(Qt::AlignCenter);
    QFont vf = m_verdict->font();
    vf.setPointSize(vf.pointSize() + 3);
    vf.setBold(true);
    m_verdict->setFont(vf);

    auto *centre = new QWidget;
    auto *centreLay = new QVBoxLayout(centre);
    // Quality readout, under the image it describes. Two bars rather than two
    // numbers because the useful question is "is this one good enough", which
    // a filled bar answers at a glance and a bare integer does not.
    //
    // The ranges are the driver's, not arbitrary: coverage cannot exceed about
    // 83% because the percentile stretch clips the darkest sixth of the frame
    // to zero, so scaling it to 100 would make every good scan look
    // three-quarters full. Sharpness has no ceiling of its own; 40 is simply
    // above everything observed (24-35 across a full enrolment) and keeps the
    // useful range in the middle of the bar.
    m_qualityBox = new QWidget;
    auto *qLay = new QGridLayout(m_qualityBox);
    qLay->setContentsMargins(0, 4, 0, 0);
    qLay->setHorizontalSpacing(8);

    m_barCoverage = new QProgressBar;
    m_barCoverage->setRange(0, 83);
    m_barCoverage->setTextVisible(false);
    m_barCoverage->setFixedHeight(10);

    m_barSharpness = new QProgressBar;
    m_barSharpness->setRange(0, 40);
    m_barSharpness->setTextVisible(false);
    m_barSharpness->setFixedHeight(10);

    m_qualityText = new QLabel;
    m_qualityText->setStyleSheet(QStringLiteral("color:#666; font-size:11px;"));

    qLay->addWidget(new QLabel(tr("Coverage")),   0, 0);
    qLay->addWidget(m_barCoverage,           0, 1);
    qLay->addWidget(new QLabel(tr("Sharpness")), 1, 0);
    qLay->addWidget(m_barSharpness,          1, 1);
    qLay->addWidget(m_qualityText,           2, 0, 1, 2);
    qLay->setColumnStretch(1, 1);
    m_qualityBox->hide();

    centreLay->addWidget(m_banner);
    centreLay->addWidget(m_liveStatus);
    centreLay->addWidget(m_imageView, 1);
    centreLay->addWidget(m_qualityBox);
    centreLay->addWidget(m_verdict);

    m_scoreTable = new QTableWidget(0, 3);
    m_scoreTable->setHorizontalHeaderLabels({tr("Sample"), tr("Score"), tr("Keypoints")});
    m_scoreTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_scoreTable->verticalHeader()->hide();
    m_scoreTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    auto *scoreBox = new QGroupBox(tr("Match scores"));
    auto *scoreLay = new QVBoxLayout(scoreBox);
    scoreLay->addWidget(m_scoreTable);

    m_log = new QPlainTextEdit;
    m_log->setReadOnly(true);
    m_log->setMaximumBlockCount(5000);
    QFont lf(QStringLiteral("monospace"));
    lf.setStyleHint(QFont::TypeWriter);
    m_log->setFont(lf);
    auto *logBox = new QGroupBox(tr("Driver log"));
    auto *logLay = new QVBoxLayout(logBox);
    logLay->addWidget(m_log);

    auto *right = new QSplitter(Qt::Vertical);
    right->addWidget(scoreBox);
    right->addWidget(logBox);
    right->setStretchFactor(1, 2);

    auto *split = new QSplitter(Qt::Horizontal);
    split->addWidget(left);
    split->addWidget(centre);
    split->addWidget(right);
    split->setStretchFactor(1, 2);
    split->setStretchFactor(2, 2);
    setCentralWidget(split);
}

void MainWindow::wireActions()
{
    const QString user = qEnvironmentVariable("USER", QStringLiteral("user"));

    auto start = [this](const QString &tag, const QStringList &args,
                        const QString &label, bool direct = false) {
        m_pending = tag;
        addStage(label);
        setBusy(true, label);
        m_runner->run(args, direct);
    };

    connect(m_btnRefresh, &QPushButton::clicked, this, [this, start] {
        start(QStringLiteral("devices"), {QStringLiteral("devices")},
              tr("Device scan"), true);
    });
    connect(m_btnCapture, &QPushButton::clicked, this, [this, start] {
        appendLog(tr("→ Put your finger on the sensor"));
        start(QStringLiteral("capture"), {QStringLiteral("capture")}, tr("Image capture"));
    });
    connect(m_btnEnroll, &QPushButton::clicked, this, [this, start, user] {
        appendLog(tr("→ Enrol: press and lift your finger repeatedly"));
        start(QStringLiteral("enroll"),
              {QStringLiteral("enroll"), QStringLiteral("--user"), user}, tr("Enrol"));
    });
    connect(m_btnVerify, &QPushButton::clicked, this, [this, start, user] {
        appendLog(tr("→ Verify: present the enrolled finger"));
        start(QStringLiteral("verify"),
              {QStringLiteral("verify"), QStringLiteral("--user"), user}, tr("Verify"));
    });
    connect(m_btnDelete, &QPushButton::clicked, this, [this, start, user] {
        start(QStringLiteral("delete"),
              {QStringLiteral("delete"), QStringLiteral("--user"), user}, tr("Delete enrolment"), true);
    });
}

void MainWindow::setBusy(bool busy, const QString &what)
{
    for (QPushButton *b : {m_btnRefresh, m_btnCapture, m_btnEnroll, m_btnVerify, m_btnDelete})
        if (b) b->setEnabled(!busy);
    m_progress->setVisible(busy);
    m_progress->setFormat(what);
    if (busy)
        m_verdict->setText(what.isEmpty() ? tr("in progress…") : what + tr(" in progress…"));
}

void MainWindow::appendLog(const QString &text) { m_log->appendPlainText(text); }

void MainWindow::addStage(const QString &text)
{
    m_stageList->addItem(text);
    m_stageList->scrollToBottom();
}

void MainWindow::applyLog(const QJsonObject &result)
{
    const QJsonArray lines = result.value(QStringLiteral("log")).toArray();
    for (const QJsonValue &v : lines) {
        const QJsonObject o = v.toObject();
        const QString dom = o.value(QStringLiteral("domain")).toString();
        appendLog(QStringLiteral("[%1] %2")
                      .arg(dom.isEmpty() ? QStringLiteral("fp") : dom,
                           o.value(QStringLiteral("text")).toString()));
    }
}

void MainWindow::showDevices(const QJsonObject &result)
{
    const QJsonArray devs = result.value(QStringLiteral("devices")).toArray();
    m_deviceList->clear();
    if (devs.isEmpty()) {
        m_deviceInfo->setText(tr("No fingerprint device was found."));
        return;
    }
    for (const QJsonValue &v : devs)
        m_deviceList->addItem(v.toObject().value(QStringLiteral("name")).toString());
    m_deviceList->setCurrentRow(0);

    const QJsonObject d = devs.first().toObject();
    m_deviceInfo->setText(
        tr("Driver: %1\nID: %2\nScan: %3 · %4 enrol stages\nRaw capture: %5")
            .arg(d.value(QStringLiteral("driver")).toString(),
                 d.value(QStringLiteral("id")).toString(),
                 d.value(QStringLiteral("scan_type")).toString())
            .arg(d.value(QStringLiteral("enroll_stages")).toInt())
            .arg(d.value(QStringLiteral("supports_capture")).toBool() ? tr("yes") : tr("no")));
    m_verdict->setText(d.value(QStringLiteral("name")).toString());
}

void MainWindow::showCapture(const QJsonObject &result)
{
    const QString b64 = result.value(QStringLiteral("png_base64")).toString();
    QImage img;
    if (!b64.isEmpty())
        img.loadFromData(QByteArray::fromBase64(b64.toLatin1()), "PNG");

    if (img.isNull()) {
        m_verdict->setText(tr("No image was returned"));
        return;
    }
    m_imageView->setImage(img);
    m_verdict->setText(tr("Captured %1 × %2").arg(img.width()).arg(img.height()));
}

void MainWindow::showVerify(const QJsonObject &result)
{
    const QJsonArray scores = result.value(QStringLiteral("scores")).toArray();
    m_scoreTable->setRowCount(scores.size());
    for (int i = 0; i < scores.size(); ++i) {
        const QJsonObject s = scores[i].toObject();
        m_scoreTable->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        m_scoreTable->setItem(i, 1, new QTableWidgetItem(
            QString::number(s.value(QStringLiteral("score")).toInt())));
        m_scoreTable->setItem(i, 2, new QTableWidgetItem(
            QString::number(s.value(QStringLiteral("total")).toInt())));
    }

    const int best = result.value(QStringLiteral("best_score")).toInt();
    const int thr  = result.value(QStringLiteral("threshold")).toInt(0);
    const bool matched = result.value(QStringLiteral("matched")).toBool();
    m_verdict->setText(matched ? tr("Match   %1 / %2").arg(best).arg(thr)
                               : tr("No match   %1 / %2").arg(best).arg(thr));
}

void MainWindow::onFinished(const QJsonObject &result)
{
    setBusy(false);
    applyLog(result);

    const bool ok = result.value(QStringLiteral("ok")).toBool();
    if (!ok) {
        const QString err = result.value(QStringLiteral("error")).toString();
        addStage(tr("Failed — %1").arg(err));
        m_verdict->setText(tr("Failed"));
        appendLog(tr("Error: %1").arg(err));
        return;
    }

    addStage(tr("Done — %1").arg(m_pending));

    if      (m_pending == QLatin1String("devices")) showDevices(result);
    else if (m_pending == QLatin1String("capture")) showCapture(result);
    else if (m_pending == QLatin1String("verify"))  showVerify(result);
    else if (m_pending == QLatin1String("enroll"))  m_verdict->setText(tr("Enrolment complete"));
    else if (m_pending == QLatin1String("delete")) {
        m_verdict->setText(tr("Enrolment deleted"));
        m_scoreTable->setRowCount(0);
    }
}

// Colours the bars by the driver's own thresholds so the display and the
// accept/reject decision cannot disagree. Kept in sync by hand with
// GOODIX55X4_QUALITY_MIN_* - there is no header shared across the driver
// boundary, and inventing one for two integers would be worse.
void MainWindow::showQuality(int coverage, int sharpness, bool accepted)
{
    if (coverage < 0 && accepted) {
        m_qualityBox->hide();
        return;
    }

    static const char *good = "QProgressBar{border:1px solid #ccc;border-radius:5px;"
                              "background:#f0f0f0;}"
                              "QProgressBar::chunk{background:#3a8f3a;border-radius:4px;}";
    static const char *weak = "QProgressBar{border:1px solid #ccc;border-radius:5px;"
                              "background:#f0f0f0;}"
                              "QProgressBar::chunk{background:#c9a227;border-radius:4px;}";

    if (!accepted) {
        // The gate handed the frame back and did not say what it measured;
        // showing the previous scan's numbers here would credit this attempt
        // with a reading it never produced.
        m_qualityText->setText(tr("This scan fell below the threshold and was asked for again"));
        m_qualityBox->show();
        return;
    }

    m_barCoverage->setValue(coverage);
    m_barSharpness->setValue(sharpness);
    m_barCoverage->setStyleSheet(QLatin1String(coverage >= 62 ? good : weak));
    m_barSharpness->setStyleSheet(QLatin1String(sharpness >= 16 ? good : weak));

    // Sharpness below the low twenties has failed to match in every run so
    // far, well above the driver's own floor of 16, so it earns a warning
    // even though the frame was accepted.
    m_qualityText->setText(
        sharpness < 24
            ? tr("Coverage %1%  ·  sharpness %2  —  faint. Press a little harder")
                  .arg(coverage).arg(sharpness)
            : tr("Coverage %1%  ·  sharpness %2").arg(coverage).arg(sharpness));
    m_qualityBox->show();
}

void MainWindow::onBeaconChanged()
{
    if (!m_beaconWatch->files().contains(Beacon::path()))
        m_beaconWatch->addPath(Beacon::path());

    QFile f(Beacon::path());
    if (!f.open(QIODevice::ReadOnly)) {
        m_banner->hide();
        return;
    }
    const QJsonObject o = QJsonDocument::fromJson(f.readAll()).object();
    const QString previewPath = o.value(QStringLiteral("image_path")).toString();
    if (!previewPath.isEmpty() && previewPath != m_lastPreview) {
        const QImage preview(previewPath);
        if (!preview.isNull()) {
            m_imageView->setImage(preview);
            m_lastPreview = previewPath;
            m_verdict->setText(tr("%1 × %2").arg(preview.width()).arg(preview.height()));
        }
    }
    showQuality(o.value(QStringLiteral("coverage")).toInt(-1),
                o.value(QStringLiteral("sharpness")).toInt(-1),
                o.value(QStringLiteral("quality_ok")).toBool(true));
    const QString prompt = o.value(QStringLiteral("prompt")).toString();
    const QString status = o.value(QStringLiteral("status")).toString();
    const QString source = o.value(QStringLiteral("source")).toString();

    if (prompt.isEmpty()) {
        // Clear the clock too. Leaving it set made tickElapsed re-show the
        // line every 500ms while this hid it every 700ms - a visible flicker,
        // and a stale "capture complete" long after the run had ended.
        m_startedAt = 0;
        m_statusText.clear();
        m_banner->hide();
        m_liveStatus->hide();
        return;
    }

    // A worker that was killed leaves its last line behind, so the prompt has
    // to be retired somehow. Age is the wrong test: the driver logs nothing
    // while the sensor waits for a finger, so a healthy enrolment can sit
    // minutes between writes - the exact stretch where the instruction matters
    // most. An age cutoff hides it right then, which is what happened here.
    //
    // Ask about the process instead. kill(pid, 0) is a liveness probe, not a
    // signal, and the worker and the window run as the same user often enough
    // that ESRCH means gone rather than not-permitted. Fall back to the age
    // rule only when there is no pid to ask about - beacons written by an
    // older build, mostly.
    const qint64 pid = o.value(QStringLiteral("pid")).toVariant().toLongLong();
    const qint64 age = QDateTime::currentMSecsSinceEpoch()
                       - o.value(QStringLiteral("ts")).toVariant().toLongLong();
    const bool gone = pid > 0 ? (::kill(static_cast<pid_t>(pid), 0) != 0 && errno == ESRCH)
                              : (age > 15000);
    if (gone) {
        m_startedAt = 0;
        m_statusText.clear();
        m_banner->hide();
        m_liveStatus->hide();
        return;
    }

    m_startedAt = o.value(QStringLiteral("started_at")).toVariant().toLongLong();
    if (status != m_statusText) {
        m_statusText = status;
        if (!status.isEmpty())
            appendLog(QStringLiteral("▸ %1").arg(status));
    }
    tickElapsed();

    // Say where the request came from. A prompt the user did not click for is
    // the case that most needs explaining.
    const QString who = source == QLatin1String("mcp")
                            ? tr("Agent request")
                            : (source == QLatin1String("cli") ? tr("CLI request") : tr("request"));
    const QString text = QStringLiteral("%1 · %2").arg(who, prompt);
    if (m_banner->text() != text) {
        m_banner->setText(text);
        appendLog(QStringLiteral("[%1] %2").arg(who, prompt));
    }
    m_banner->show();
}

void MainWindow::tickElapsed()
{
    if (m_startedAt <= 0) {
        m_liveStatus->hide();
        return;
    }
    const qint64 secs = (QDateTime::currentMSecsSinceEpoch() - m_startedAt) / 1000;
    const QString what = m_statusText.isEmpty() ? tr("Idle") : m_statusText;
    m_liveStatus->setText(tr("▸ %1   ·   %2s elapsed").arg(what).arg(secs));
    m_liveStatus->show();
}

void MainWindow::onFailed(const QString &error)
{
    setBusy(false);
    addStage(tr("Failed"));
    m_verdict->setText(tr("Failed"));
    appendLog(tr("Error: %1").arg(error));
}

} // namespace fpstudio
