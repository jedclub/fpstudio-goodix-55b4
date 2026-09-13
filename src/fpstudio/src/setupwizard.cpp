#include "setupwizard.h"

#include "mainwindow.h"

#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
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

// The wrapper that puts fprintd's progress on the banner. Beside the binary in
// a build tree, under share/ once installed.
QString enrolmentScript()
{
    const QString script = setupResource(QStringLiteral("src/fpstudio/fprintd-beacon.sh"));
    return QFileInfo(script).isExecutable() ? script : QString();
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
    setWindowTitle(tr("FPStudio · Integrated fingerprint authentication setup"));
    resize(1050, 740);

    m_list = new QListWidget;
    m_list->setFixedWidth(270);
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
    m_fix->setMinimumHeight(40);
    m_fix->setStyleSheet(QStringLiteral("QPushButton {background:#1769c2;color:white;border:0;border-radius:6px;padding:8px 16px;font-weight:bold;} QPushButton:disabled {background:#a0a8b0;color:#e8e8e8;}"));
    m_skip   = new QPushButton(tr("Skip"));
    m_rescan = new QPushButton(tr("Check status again"));
    connect(m_fix,    &QPushButton::clicked, this, &SetupWizard::runCurrentFix);
    connect(m_skip,   &QPushButton::clicked, this, &SetupWizard::skipCurrent);
    connect(m_rescan, &QPushButton::clicked, this, &SetupWizard::rescan);

    m_verdict = new QLabel;
    m_verdict->setWordWrap(true);
    m_verdict->setStyleSheet(QStringLiteral("background:#123e68;color:white;padding:10px;border-radius:5px;font-size:13px;"));

    // A quiet checklist of green marks does not say "you are done" on its
    // own - this does, and Finish is the one button in the dialog whose whole
    // job is to end the wizard on purpose.
    m_completionText = new QLabel(tr("Setup steps complete · Verify real sudo/KDE authentication and password recovery separately."));
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
    m_stopOperation=new QPushButton(tr("Stop"));
    m_stopOperation->hide();
    connect(m_stopOperation,&QPushButton::clicked,this,[this]{
        if(m_operation){m_verdict->setText(tr("Stopping the fingerprint test. You may lift your finger."));m_operation->terminate();}
    });
    btns->addWidget(m_stopOperation);
    m_recover=new QPushButton(tr("Restore authentication settings"));
    m_verify=new QPushButton(tr("Test system fingerprint"));
    connect(m_recover,&QPushButton::clicked,this,&SetupWizard::runRecovery);
    connect(m_verify,&QPushButton::clicked,this,&SetupWizard::runSystemVerify);
    btns->addWidget(m_recover);
    btns->addWidget(m_verify);
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
    if(m_operation||m_scanning)return;
    m_scanning=true;
    m_busy->show();
    m_verdict->setText(tr("Checking the device and installation. Keep your finger off the sensor for now."));
    m_fix->setEnabled(false);m_rescan->setEnabled(false);m_list->setEnabled(false);
    m_skip->setEnabled(false);m_verify->setEnabled(false);m_recover->setEnabled(false);
    auto *watcher=new QFutureWatcher<QVector<StepResult>>(this);
    connect(watcher,&QFutureWatcher<QVector<StepResult>>::finished,this,[this,watcher]{
    m_steps=watcher->result();watcher->deleteLater();m_scanning=false;
    m_fix->setEnabled(true);m_rescan->setEnabled(true);m_list->setEnabled(true);
    m_skip->setEnabled(true);m_verify->setEnabled(true);
    m_busy->hide();
    m_verdict->clear();
    render();

    m_completion->setVisible(allReady(m_steps));
    m_recover->setEnabled(QFileInfo::exists(QStringLiteral("/etc/fpstudio-auth.json")));

    // Land on the first thing that needs attention rather than the top, so
    // the wizard opens on the work instead of on eight ticks. When there is
    // nothing left, staying on the first row is fine too - the banner above
    // it is what actually announces "done", not which row happens to be lit.
    selectNextStep();
    const QString message=property("scanResultMessage").toString();
    if(!message.isEmpty()){m_verdict->setText(message);setProperty("scanResultMessage",QString());}
    });
    watcher->setFuture(QtConcurrent::run([]{return probeAll();}));
}

void SetupWizard::selectNextStep() {
    int first = 0;
    for (int i = 0; i < m_steps.size(); ++i) {
        if (m_steps[i].state != StepState::Ok && m_steps[i].state != StepState::Skipped) {
            first = i;
            break;
        }
    }
    m_list->setCurrentRow(first);
    if(first>=0&&first<m_steps.size())showStep(first);
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

    m_title->setText(QStringLiteral("%1 / %2 · %3").arg(index+1).arg(m_steps.size()).arg(stepTitle(r.id)));

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

    const bool interactive = r.id == StepId::Enrolment || r.id == StepId::Capture || r.id == StepId::TlsSession;
    const bool fixable = !r.action.isEmpty() &&
        (r.state == StepState::Missing ||
         (interactive && r.state != StepState::Ok && r.state != StepState::Skipped));
    m_fix->setVisible(fixable);
    m_fix->setText(r.action);
    m_skip->setVisible(r.id==StepId::UdevRule&&(r.state == StepState::Missing || r.state == StepState::Manual));

    if (r.needsRoot && fixable)
        m_verdict->setText(tr("This will ask for your password."));
    else
        m_verdict->setText(r.state==StepState::Ok?tr("This step is confirmed. Continue to the next required step."):
            !r.action.isEmpty()?tr("Next action: %1").arg(r.action):tr("Review the guidance. This step is not confirmed yet."));
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
    if(m_operation)return;
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
    m_operation=p;m_list->setEnabled(false);m_rescan->setEnabled(false);m_skip->setEnabled(false);
    m_recover->setEnabled(false);m_verify->setEnabled(false);m_diagnostics->setEnabled(false);
    p->setProcessChannelMode(QProcess::MergedChannels);
    connect(p,&QProcess::readyReadStandardOutput,this,[this,p]{
        const auto out=QString::fromUtf8(p->readAllStandardOutput());
        m_verdict->setText(tr("Enrolment in progress · Present and lift the same finger as instructed\n")+out.right(500));
    });
    connect(p,&QProcess::errorOccurred,this,[this,p](QProcess::ProcessError error){
        if(error!=QProcess::FailedToStart||m_operation!=p)return;
        m_operation=nullptr;m_diagnostics->setEnabled(true);
        setProperty("scanResultMessage",tr("Could not start the enrolment tool: %1").arg(p->errorString()));
        p->deleteLater();rescan();
    });
    connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [this, p](int, QProcess::ExitStatus) {
        m_busy->hide();
        m_operation=nullptr;m_list->setEnabled(true);m_rescan->setEnabled(true);m_skip->setEnabled(true);
        m_verify->setEnabled(true);m_diagnostics->setEnabled(true);
        m_recover->setEnabled(QFileInfo::exists(QStringLiteral("/etc/fpstudio-auth.json")));
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
    if(m_operation)return;
    m_busy->show();
    m_fix->setEnabled(false);
    // The same sentence the CLI's own capture command shows, so the
    // instruction reads the same wherever it appears.
    m_verdict->setText(tr("Put your finger on the sensor and hold it there"));

    auto *p = new QProcess(this);
    m_operation=p;m_list->setEnabled(false);m_rescan->setEnabled(false);m_skip->setEnabled(false);
    m_recover->setEnabled(false);m_verify->setEnabled(false);m_diagnostics->setEnabled(false);
    p->setProcessChannelMode(QProcess::MergedChannels);
    connect(p,&QProcess::errorOccurred,this,[this,p](QProcess::ProcessError error){
        if(error!=QProcess::FailedToStart||m_operation!=p)return;
        m_operation=nullptr;m_diagnostics->setEnabled(true);
        setProperty("scanResultMessage",tr("Could not start the image tool: %1").arg(p->errorString()));
        p->deleteLater();rescan();
    });
    connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [this, p](int, QProcess::ExitStatus) {
        const QString out = QString::fromUtf8(p->readAllStandardOutput());
        m_busy->hide();
        m_operation=nullptr;m_list->setEnabled(true);m_rescan->setEnabled(true);m_skip->setEnabled(true);
        m_verify->setEnabled(true);m_diagnostics->setEnabled(true);
        m_recover->setEnabled(QFileInfo::exists(QStringLiteral("/etc/fpstudio-auth.json")));
        m_fix->setEnabled(true);
        m_verdict->clear();
        parseCaptureOutput(out);
        for(int i=0;i<m_steps.size();++i)if(m_steps[i].id==StepId::TlsSession||m_steps[i].id==StepId::Psk||m_steps[i].id==StepId::Capture)
            m_steps[i]=probe(m_steps[i].id);
        render();selectNextStep();showStep(m_current);
        p->deleteLater();
    });
    p->start(QFileInfo(QCoreApplication::applicationFilePath()).canonicalFilePath(),
             {QStringLiteral("--cli"), QStringLiteral("capture"),
              QStringLiteral("--timeout"), QStringLiteral("8")});
}


void SetupWizard::runCurrentFix()
{
    if(m_operation||m_scanning)return;
    StepResult r = m_steps[m_current];
    if(r.id==StepId::Driver) {
        runManagedAction({QStringLiteral("/usr/bin/python"),setupResource(QStringLiteral("tools/driver_build.py"))},false);return;
    }
    if(r.id==StepId::GpuAuth) { runGpuInstall();return; }
    if(r.id==StepId::PamKde||r.id==StepId::PamPolkit||r.id==StepId::PamSudo) {
        runPamRepair();return;
    }

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
    if (r.id == StepId::Capture || r.id == StepId::TlsSession) {
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

    runManagedAction({QStringLiteral("/usr/bin/sh"),QStringLiteral("-c"),QStringLiteral("set -e\n")+r.commands.join('\n')});
}

void SetupWizard::reject() {
    if(m_operation){m_verdict->setText(tr("Wait for the operation to finish or cancel its authentication request. Setup records and recovery information are being protected."));return;}
    QDialog::reject();
}
void SetupWizard::done(int result) {
    if(m_operation){m_verdict->setText(tr("Finish the running setup operation or cancel its authentication request before closing."));return;}
    QDialog::done(result);
}

void SetupWizard::runManagedAction(const QStringList &arguments,bool privileged) {
    if(m_operation)return;
    auto *p=new QProcess(this);m_operation=p;
    m_busy->show();m_fix->setEnabled(false);m_rescan->setEnabled(false);
    m_skip->setEnabled(false);m_recover->setEnabled(false);m_verify->setEnabled(false);
    m_list->setEnabled(false);m_diagnostics->setEnabled(false);
    p->setProcessChannelMode(QProcess::MergedChannels);
    auto finish=[this,p](bool ok) {
        if(m_operation!=p)return;
        const QString output=QString::fromUtf8(p->readAll()).right(2400);
        m_operation=nullptr;m_busy->hide();m_fix->setEnabled(true);m_rescan->setEnabled(true);
        m_skip->setEnabled(true);m_verify->setEnabled(true);m_list->setEnabled(true);m_diagnostics->setEnabled(true);
        setProperty("scanResultMessage",ok?tr("Setup operation complete. Use the system fingerprint test to verify the real path."):tr("Setup incomplete or cancelled: %1").arg(output));
        rescan();
        p->deleteLater();
    };
    connect(p,qOverload<int,QProcess::ExitStatus>(&QProcess::finished),this,[finish](int code,QProcess::ExitStatus status){finish(status==QProcess::NormalExit&&code==0);});
    connect(p,&QProcess::errorOccurred,this,[finish](QProcess::ProcessError error){if(error==QProcess::FailedToStart)finish(false);});
    m_verdict->setText(privileged?tr("After administrator authorization, installation and checks run automatically. Authentication settings are restored if installation fails."):
        tr("The driver is built as the current user. Administrator authorization is requested only for dependencies and package installation."));
    if(privileged)p->start(QStringLiteral("/usr/bin/pkexec"),QStringList{QStringLiteral("--disable-internal-agent")}+arguments);
    else p->start(arguments.first(),arguments.mid(1));
}

void SetupWizard::runGpuInstall() {
    QFile state(QStringLiteral("/etc/fpstudio-auth.json"));
    if(state.open(QIODevice::ReadOnly)) {
        const auto installed=QJsonDocument::fromJson(state.readAll()).object();
        if(installed.value("installed").toBool()&&installed.value("username").toString()==qEnvironmentVariable("USER")) {
            if(QMessageBox::question(this,tr("Update GPU authentication engine"),
                tr("Existing fingerprint references, fprintd enrolment and PAM settings will not change. Only the matcher, shader and driver bridge are replaced, followed by reference self-checks. Changed files are backed up separately. Apply the update?"))!=QMessageBox::Yes)return;
            runManagedAction({QStringLiteral("/usr/bin/python"),
                              setupResource(QStringLiteral("tools/auth_install.py")),
                              QStringLiteral("--refresh-experimental-auth"),QStringLiteral("--apply")});
            return;
        }
    }
    QStringList directories;
    const auto options=QCoreApplication::arguments();
    for(int i=0;i+1<options.size();++i)if(options[i]=="--auth-reference-dir")directories<<options[++i];
    if(directories.isEmpty()) {
        const QString directory=QFileDialog::getExistingDirectory(this,tr("Select your saved fingerprint session"),QDir::homePath());
        if(directory.isEmpty())return;
        directories<<directory;
    }
    if(QMessageBox::question(this,tr("Experimental GPU authentication"),
        tr("Confirm that the selected folders contain this user's fingerprints.\n%1\n\nExisting enrolment is preserved and GPU comparison is used for sudo and KDE authentication. After at most 20 failed contacts or 60 seconds, the password path remains available. Rejection of other fingers is not fully validated. Back up the settings and apply?").arg(directories.join('\n')))!=QMessageBox::Yes)return;
    QStringList args{QStringLiteral("/usr/bin/python"),setupResource(QStringLiteral("tools/auth_install.py")),"--user",qEnvironmentVariable("USER"),"--enable-experimental-auth","--apply"};
    for(const auto &dir:directories)args<<"--reference-dir"<<dir;
    runManagedAction(args);
}

void SetupWizard::runPamRepair() {
    // Do not re-import the private gallery for an authentication repair. This
    // installs the supported local PAM paths and first checks KDE's
    // separate password service, which keeps its password field available.
    runManagedAction({QStringLiteral("/usr/bin/python"),
                      setupResource(QStringLiteral("tools/dual_auth_install.py")),
                      QStringLiteral("--apply")});
}

void SetupWizard::runRecovery() {
    QFile file(QStringLiteral("/etc/fpstudio-auth.json"));if(!file.open(QIODevice::ReadOnly))return;
    const auto path=QJsonDocument::fromJson(file.readAll()).object().value("backup").toString();
    if(path.isEmpty())return;
    if(QMessageBox::question(this,tr("Restore authentication settings"),tr("Restore the sudo, KDE and fprintd settings from before installation? Existing fingerprint enrolment is not deleted."))!=QMessageBox::Yes)return;
    runManagedAction({QStringLiteral("/usr/bin/python"),setupResource(QStringLiteral("tools/auth_install.py")),"--rollback",path});
}

void SetupWizard::runSystemVerify() {
    if(m_operation)return;
    auto *p=new QProcess(this);m_operation=p;
    m_stopOperation->show();
    m_busy->show();m_fix->setEnabled(false);m_verify->setEnabled(false);m_rescan->setEnabled(false);
    m_recover->setEnabled(false);m_list->setEnabled(false);m_skip->setEnabled(false);m_diagnostics->setEnabled(false);
    p->setProcessChannelMode(QProcess::MergedChannels);
    auto finish=[this,p](bool ok) {
        if(m_operation!=p)return;
        const QString output=p->property("transcript").toString()+QString::fromUtf8(p->readAll());
        // fprintd publishes VerifyStatus before it winds the imaging device
        // down. fprintd-verify can then stop/release the device and return a
        // non-zero process status even though the authentication result was
        // already a match. The observed status is authoritative for this
        // one-shot test; process completion only closes the transport.
        const bool matched=output.contains(QStringLiteral("verify-match"));
        m_operation=nullptr;m_stopOperation->hide();m_busy->hide();m_fix->setEnabled(true);m_verify->setEnabled(true);m_rescan->setEnabled(true);
        m_list->setEnabled(true);m_skip->setEnabled(true);m_diagnostics->setEnabled(true);
        m_recover->setEnabled(QFileInfo::exists(QStringLiteral("/etc/fpstudio-auth.json")));
        const bool thermal=output.contains(QStringLiteral("overheating"),Qt::CaseInsensitive)||
                           output.contains(QStringLiteral("prevent overheating"),Qt::CaseInsensitive);
        const bool timedOut=p->property("fpstudioTimedOut").toBool();
        m_verdict->setText(matched?tr("Fingerprint match succeeded. The result was delivered while the finger remained in place. Test the sudo/KDE dialog and password fallback separately."):
            thermal?tr("Sensor thermal protection stopped the test. Lift your finger, let it cool briefly, then try again."):
            timedOut?tr("No finger was detected during this 30-second one-shot test. Clear the sensor, then place the enrolled finger in the centre for one or two seconds when prompted."):
            tr("System fingerprint test incomplete: %1").arg(output.right(1000)));p->deleteLater();
    };
    connect(p,qOverload<int,QProcess::ExitStatus>(&QProcess::finished),this,[finish](int code,QProcess::ExitStatus status){finish(code==0&&status==QProcess::NormalExit);});
    connect(p,&QProcess::errorOccurred,this,[finish](QProcess::ProcessError error){if(error==QProcess::FailedToStart)finish(false);});
    connect(p,&QProcess::readyReadStandardOutput,this,[this,p]{
        const auto output=QString::fromUtf8(p->readAllStandardOutput());
        p->setProperty("transcript",(p->property("transcript").toString()+output).right(4000));
        if(output.contains(QStringLiteral("verify-match"))) {
            // Show the actual match as soon as fprintd emits it, rather than
            // making the person wait for the sensor's cleanup/release phase.
            m_verdict->setText(tr("[System fingerprint test] Match confirmed while the finger remained in place.\n")+output.right(700));
        } else if(output.contains(QStringLiteral("verify-no-match"))) {
            m_verdict->setText(tr("[System fingerprint test] No match. This contact was not recorded or enrolled.\n")+output.right(700));
        } else {
            m_verdict->setText(tr("[System fingerprint test] Place the finger in the centre and hold it until a result appears. You do not need to lift it.\n")+output.right(700));
        }
    });
    // A no-touch verification used to run for 95 seconds. On this sensor that
    // is long enough to enter its own thermal protection, turning a missed
    // prompt into a misleading "disconnected" result. This diagnostic is a
    // shorter one-shot test than the 60-second PAM window, and releases
    // fprintd cleanly after 30 seconds.
    QTimer::singleShot(30000,p,[p]{
        if(p->state()!=QProcess::NotRunning) {
            p->setProperty("fpstudioTimedOut",true);
            p->terminate();
            QTimer::singleShot(2000,p,[p]{if(p->state()!=QProcess::NotRunning)p->kill();});
        }
    });
    m_verdict->setText(tr("[System fingerprint test] Start with the sensor clear. When prompted, place the same enrolled finger and hold it until the result appears. You do not need to lift it."));
    p->start(QStringLiteral("/usr/bin/fprintd-verify"),{qEnvironmentVariable("USER")});
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
