#include "setupwizard.h"

#include "mainwindow.h"

#include <QFileInfo>
#include <QCoreApplication>
#include <QApplication>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QProcess>
#include <QProgressBar>
#include <QPushButton>
#include <QSplitter>
#include <QTextBrowser>
#include <QVBoxLayout>

namespace fpstudio {
namespace {

// The mark in front of each row. Text rather than icons because the states
// mean different things and a theme's icon set does not distinguish them:
// "can't do this for you" is not a kind of failure, and "not measured" is not
// a kind of success.
QString mark(StepState s)
{
    switch (s) {
    case StepState::Ok:      return QStringLiteral("✓");
    case StepState::Missing: return QStringLiteral("•");
    case StepState::Manual:  return QStringLiteral("✋");
    case StepState::Failed:  return QStringLiteral("✕");
    case StepState::Skipped: return QStringLiteral("–");
    case StepState::Unknown: return QStringLiteral("?");
    }
    return QString();
}

QString colour(StepState s)
{
    switch (s) {
    case StepState::Ok:      return QStringLiteral("#3a8f3a");
    case StepState::Missing: return QStringLiteral("#c9a227");
    case StepState::Manual:  return QStringLiteral("#8a6d1f");
    case StepState::Failed:  return QStringLiteral("#b03030");
    case StepState::Skipped: return QStringLiteral("#888888");
    case StepState::Unknown: return QStringLiteral("#888888");
    }
    return QString();
}

// Runs one privileged command list under a single pkexec, so a fix that needs
// three commands asks once rather than three times. The shell is what makes
// that possible; the commands come from setupcheck.cpp and never from user
// input, which is what makes it safe.
bool runAsRoot(const QStringList &commands, QString *error)
{
    QProcess p;
    p.setProcessChannelMode(QProcess::MergedChannels);
    p.start(QStringLiteral("pkexec"),
            {QStringLiteral("sh"), QStringLiteral("-c"),
             QStringLiteral("set -e\n") + commands.join(QLatin1Char('\n'))});
    if (!p.waitForFinished(600000)) {
        p.kill();
        *error = SetupWizard::tr("The command did not finish in time");
        return false;
    }
    if (p.exitCode() != 0) {
        const QString out = QString::fromUtf8(p.readAll()).trimmed();
        // 126 and 127 are pkexec saying the prompt was dismissed or refused,
        // which is a choice rather than a fault and should not read as one.
        *error = (p.exitCode() == 126 || p.exitCode() == 127)
                     ? SetupWizard::tr("Authentication was cancelled or refused")
                     : (out.isEmpty() ? SetupWizard::tr("Failed — %1").arg(p.exitCode())
                                      : out.right(600));
        return false;
    }
    return true;
}

// The wrapper that puts fprintd's progress on the banner. Beside the binary in
// a build tree, under share/ once installed.
QString enrolmentScript()
{
    const QString bin = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath();
    for (const QString &c : {bin + QStringLiteral("/../fprintd-beacon.sh"),
                             bin + QStringLiteral("/../share/fpstudio/fprintd-beacon.sh"),
                             QStringLiteral(FPSTUDIO_SOURCE_DIR "/fprintd-beacon.sh")}) {
        if (QFileInfo(c).isExecutable())
            return QFileInfo(c).canonicalFilePath();
    }
    return QString();
}

} // namespace

// Opens the diagnostics window as a second, independent top-level window -
// this dialog stays open, since someone who reaches for the deep tool from
// here has usually not finished with the checklist yet.
void SetupWizard::openDiagnostics()
{
    // MainWindow is not owned by this dialog and outlives it; giving it no
    // parent is what makes it a real second window rather than something tied
    // to this one's lifetime.
    auto *w = new MainWindow(nullptr);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

SetupWizard::SetupWizard(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Set up fingerprint unlock"));
    resize(760, 520);

    m_list = new QListWidget;
    m_list->setFixedWidth(230);
    connect(m_list, &QListWidget::currentRowChanged, this, &SetupWizard::selectRow);

    m_title = new QLabel;
    m_title->setStyleSheet(QStringLiteral("font-size:15px; font-weight:bold;"));
    m_title->setWordWrap(true);

    m_detail = new QTextBrowser;
    m_detail->setOpenExternalLinks(true);

    // The exact commands, always visible rather than behind a disclosure.
    // Someone who would rather run them by hand should not have to hunt, and
    // someone who would not should still see what they are agreeing to.
    m_cmds = new QLabel;
    m_cmds->setWordWrap(true);
    m_cmds->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_cmds->setStyleSheet(QStringLiteral(
        "font-family:monospace; font-size:11px; color:#555;"
        "background:#f4f4f4; border:1px solid #ddd; padding:6px;"));

    m_busy = new QProgressBar;
    m_busy->setRange(0, 0);
    m_busy->setTextVisible(false);
    m_busy->setFixedHeight(6);
    m_busy->hide();

    m_fix    = new QPushButton;
    m_skip   = new QPushButton(tr("Skip"));
    m_rescan = new QPushButton(tr("Re-check"));
    connect(m_fix,    &QPushButton::clicked, this, &SetupWizard::runCurrentFix);
    connect(m_skip,   &QPushButton::clicked, this, &SetupWizard::skipCurrent);
    connect(m_rescan, &QPushButton::clicked, this, &SetupWizard::rescan);

    m_verdict = new QLabel;
    m_verdict->setWordWrap(true);

    // A quiet checklist of green marks does not say "you are done" on its
    // own - this does, and Finish is the one button in the dialog whose whole
    // job is to end the wizard on purpose.
    m_completionText = new QLabel(tr("Fingerprint unlock is ready."));
    m_completionText->setWordWrap(true);
    m_finish = new QPushButton(tr("Finish"));
    connect(m_finish, &QPushButton::clicked, this, &QDialog::accept);

    auto *compLay = new QHBoxLayout;
    compLay->addWidget(m_completionText, 1);
    compLay->addWidget(m_finish);

    m_completion = new QWidget;
    m_completion->setLayout(compLay);
    // Background and text set together, on every widget in this box, by type
    // selector rather than colour alone - a solid background with only its
    // own colour set is exactly how this box shipped invisible the first
    // time: light green with no text colour of its own inherits the app's
    // palette, which on a dark desktop theme is light text on a light box.
    // A solid dark green with white text stays readable either way, matching
    // how the blue instruction banner above the image view already does it.
    m_completion->setStyleSheet(QStringLiteral(
        "QWidget#completion { background:#2e7d32; border-radius:4px; }"
        "QWidget#completion QLabel { color:white; font-weight:bold; padding:8px; background:transparent; }"
        "QWidget#completion QPushButton { color:white; background:#1b5e20; border:1px solid white;"
        "                                 border-radius:4px; padding:4px 14px; font-weight:bold; }"
        "QWidget#completion QPushButton:hover { background:#245c27; }"));
    m_completion->setObjectName(QStringLiteral("completion"));
    m_completion->hide();

    auto *btns = new QHBoxLayout;
    btns->addWidget(m_rescan);
    btns->addStretch(1);
    btns->addWidget(m_skip);
    btns->addWidget(m_fix);

    // A way into the deep tool - live capture, driver log, MCP status - but
    // only when this dialog IS the application, not when the diagnostics
    // window opened it. Otherwise this would be a second door back into a
    // room the person is already standing in.
    m_diagnostics = new QPushButton(tr("Open diagnostics tool"));
    m_diagnostics->setFlat(true);
    m_diagnostics->setToolTip(
        tr("Live capture preview, the driver log, and MCP status - for a "
           "closer look once the checklist here is done."));
    m_diagnostics->setVisible(!parent);
    connect(m_diagnostics, &QPushButton::clicked, this, &SetupWizard::openDiagnostics);

    auto *footer = new QHBoxLayout;
    footer->addWidget(m_diagnostics);
    footer->addStretch(1);

    auto *right = new QVBoxLayout;
    right->addWidget(m_completion);
    right->addWidget(m_title);
    right->addWidget(m_detail, 1);
    right->addWidget(m_cmds);
    right->addWidget(m_busy);
    right->addWidget(m_verdict);
    right->addLayout(btns);
    right->addLayout(footer);

    auto *rightBox = new QWidget;
    rightBox->setLayout(right);

    auto *lay = new QHBoxLayout(this);
    lay->addWidget(m_list);
    lay->addWidget(rightBox, 1);

    rescan();
}

void SetupWizard::rescan()
{
    m_busy->show();
    m_verdict->setText(tr("Checking…"));
    // Let the label paint before probeAll blocks: it takes seconds, and a
    // window that freezes with no explanation is indistinguishable from one
    // that has crashed.
    QApplication::processEvents();

    m_steps = probeAll();
    m_busy->hide();
    m_verdict->clear();
    render();

    m_completion->setVisible(allReady(m_steps));

    // Land on the first thing that needs attention rather than the top, so
    // the wizard opens on the work instead of on eight ticks. When there is
    // nothing left, staying on the first row is fine too - the banner above
    // it is what actually announces "done", not which row happens to be lit.
    int first = 0;
    for (int i = 0; i < m_steps.size(); ++i) {
        if (m_steps[i].state != StepState::Ok && m_steps[i].state != StepState::Skipped) {
            first = i;
            break;
        }
    }
    m_list->setCurrentRow(first);
}

void SetupWizard::render()
{
    const int keep = m_list->currentRow();
    m_list->clear();
    for (const StepResult &r : m_steps) {
        auto *item = new QListWidgetItem(
            QStringLiteral("%1  %2").arg(mark(r.state), stepTitle(r.id)));
        item->setForeground(QColor(colour(r.state)));
        m_list->addItem(item);
    }
    if (keep >= 0 && keep < m_list->count())
        m_list->setCurrentRow(keep);
}

void SetupWizard::selectRow(int row)
{
    if (row < 0 || row >= m_steps.size())
        return;
    m_current = row;
    showStep(row);
}

void SetupWizard::showStep(int index)
{
    const StepResult &r = m_steps[index];

    m_title->setText(stepTitle(r.id));

    QString body = QStringLiteral("<p><b>%1</b></p>").arg(r.summary.toHtmlEscaped());
    if (!r.detail.isEmpty()) {
        QString d = r.detail.toHtmlEscaped();
        d.replace(QStringLiteral("\n\n"), QStringLiteral("</p><p>"));
        d.replace(QLatin1Char('\n'), QStringLiteral("<br>"));
        body += QStringLiteral("<p>%1</p>").arg(d);
    }
    if (r.irreversible) {
        body += QStringLiteral(
                    "<p style='color:#b03030'><b>%1</b></p>")
                    .arg(tr("This step cannot be undone.").toHtmlEscaped());
    }
    m_detail->setHtml(body);

    m_cmds->setVisible(!r.commands.isEmpty());
    m_cmds->setText(r.commands.join(QLatin1Char('\n')));

    const bool interactive = r.id == StepId::Enrolment || r.id == StepId::Capture;
    const bool fixable = !r.action.isEmpty() &&
        (r.state == StepState::Missing ||
         (interactive && r.state != StepState::Ok && r.state != StepState::Skipped));
    m_fix->setVisible(fixable);
    m_fix->setText(r.action);
    m_skip->setVisible(r.state == StepState::Missing || r.state == StepState::Manual);

    if (r.needsRoot && fixable)
        m_verdict->setText(tr("This will ask for your password."));
    else
        m_verdict->clear();
}

bool SetupWizard::confirmIrreversible(const StepResult &r)
{
    // A checkbox or a second OK is not enough here. The cost is permanent and
    // silent - nothing breaks today, and Windows fingerprint sign-in is simply
    // gone the next time someone boots it. Typing the word is a deliberate
    // speed bump, and the phrase to type is one nobody hits by reflex.
    bool ok = false;
    const QString want = tr("WRITE");
    const QString got = QInputDialog::getText(
        this, tr("Sensor key"),
        tr("%1\n\nType %2 to confirm.").arg(r.detail, want),
        QLineEdit::Normal, QString(), &ok);
    return ok && got.trimmed().compare(want, Qt::CaseInsensitive) == 0;
}

void SetupWizard::applyResult(int index, const StepResult &r)
{
    m_steps[index] = r;
    render();
    showStep(index);
    // A single fix can be the last thing standing between here and "ready" -
    // the banner should not wait for the next full rescan to notice that.
    m_completion->setVisible(allReady(m_steps));
}

// Runs the enrolment wrapper and re-probes when it exits. Detached from the
// dialog's own event loop by QProcess rather than blocking, because this one
// takes minutes and the banner it drives has to keep updating while it does.
void SetupWizard::runEnrolment()
{
    const QString script = enrolmentScript();
    if (script.isEmpty()) {
        m_verdict->setText(tr("Failed — %1")
                               .arg(tr("The enrolment helper was not found")));
        return;
    }

    m_busy->show();
    m_fix->setEnabled(false);
    m_verdict->setText(tr("Enrolling — press and lift your finger repeatedly"));

    auto *p = new QProcess(this);
    p->setProcessChannelMode(QProcess::MergedChannels);
    connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [this, p](int, QProcess::ExitStatus) {
        m_busy->hide();
        m_fix->setEnabled(true);
        applyResult(m_current, probe(StepId::Enrolment));
        p->deleteLater();
    });
    p->start(script, {QStringLiteral("right-index-finger")});
}

// Mirrors runEnrolment(): a real capture attempt is a person doing something
// at the sensor, not a command whose exit code is the whole story, so it runs
// asynchronously with its own instruction on screen rather than through the
// blocking pkexec path below.
void SetupWizard::runCaptureTest()
{
    m_busy->show();
    m_fix->setEnabled(false);
    // The same sentence the CLI's own capture command shows, so the
    // instruction reads the same wherever it appears.
    m_verdict->setText(tr("Put your finger on the sensor and hold it there"));

    auto *p = new QProcess(this);
    p->setProcessChannelMode(QProcess::MergedChannels);
    connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [this, p](int, QProcess::ExitStatus) {
        const QString out = QString::fromUtf8(p->readAllStandardOutput());
        m_busy->hide();
        m_fix->setEnabled(true);
        m_verdict->clear();
        applyResult(m_current, parseCaptureOutput(out));
        p->deleteLater();
    });
    p->start(QFileInfo(QCoreApplication::applicationFilePath()).canonicalFilePath(),
             {QStringLiteral("--cli"), QStringLiteral("capture"),
              QStringLiteral("--timeout"), QStringLiteral("8")});
}


void SetupWizard::runCurrentFix()
{
    StepResult r = m_steps[m_current];

    // Enrolment is the one step whose fix is not a command to run but a person
    // to guide: it needs fifteen presentations of a finger, with feedback
    // after each. The wrapper beside this binary drives fprintd and publishes
    // that feedback to the banner, so hand off to it rather than reimplement
    // the loop here.
    if (r.id == StepId::Enrolment) {
        runEnrolment();
        return;
    }

    // Same reasoning as enrolment: this is a person doing something in front
    // of the sensor, not a command to run, so it gets its own interactive flow
    // instead of the pkexec path below.
    if (r.id == StepId::Capture) {
        runCaptureTest();
        return;
    }

    if (r.commands.isEmpty())
        return;

    if (r.irreversible && !confirmIrreversible(r))
        return;

    m_busy->show();
    m_fix->setEnabled(false);
    m_verdict->setText(tr("in progress…"));
    QApplication::processEvents();

    QString error;
    const bool ran = runAsRoot(r.commands, &error);

    // Re-probe either way. A fix that reported success can still leave the
    // machine unchanged - a udev rule that has not taken effect is exactly
    // that - and a fix that reported failure sometimes did most of the work.
    // What the machine says now is the only answer worth showing.
    StepResult after = probe(r.id);
    m_busy->hide();
    m_fix->setEnabled(true);
    applyResult(m_current, after);

    if (!ran && after.state != StepState::Ok)
        m_verdict->setText(tr("Failed — %1").arg(error));
}

void SetupWizard::skipCurrent()
{
    StepResult r = m_steps[m_current];
    r.state = StepState::Skipped;
    applyResult(m_current, r);

    for (int i = m_current + 1; i < m_steps.size(); ++i) {
        if (m_steps[i].state != StepState::Ok && m_steps[i].state != StepState::Skipped) {
            m_list->setCurrentRow(i);
            return;
        }
    }
}

} // namespace fpstudio
