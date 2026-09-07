#include "setupwizard.h"

#include "mainwindow.h"

#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
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
    setWindowTitle(QStringLiteral("FPStudio · 지문 인증 통합 설정"));
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
    m_rescan = new QPushButton(QStringLiteral("상태 다시 확인"));
    connect(m_fix,    &QPushButton::clicked, this, &SetupWizard::runCurrentFix);
    connect(m_skip,   &QPushButton::clicked, this, &SetupWizard::skipCurrent);
    connect(m_rescan, &QPushButton::clicked, this, &SetupWizard::rescan);

    m_verdict = new QLabel;
    m_verdict->setWordWrap(true);
    m_verdict->setStyleSheet(QStringLiteral("background:#123e68;color:white;padding:10px;border-radius:5px;font-size:13px;"));

    // A quiet checklist of green marks does not say "you are done" on its
    // own - this does, and Finish is the one button in the dialog whose whole
    // job is to end the wizard on purpose.
    m_completionText = new QLabel(QStringLiteral("설정 단계 완료 · 실제 sudo/KDE 인증과 비밀번호 복구 시험은 별도 확인이 필요합니다."));
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
    m_stopOperation=new QPushButton(QStringLiteral("중단"));
    m_stopOperation->hide();
    connect(m_stopOperation,&QPushButton::clicked,this,[this]{
        if(m_operation){m_verdict->setText(QStringLiteral("지문 시험을 중단합니다. 손을 떼셔도 됩니다."));m_operation->terminate();}
    });
    btns->addWidget(m_stopOperation);
    m_recover=new QPushButton(QStringLiteral("인증 설정 복구"));
    m_verify=new QPushButton(QStringLiteral("시스템 지문 시험"));
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
    m_verdict->setText(QStringLiteral("장치와 설치 상태 확인 중입니다. 손가락은 아직 대지 마세요."));
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
        m_verdict->setText(r.state==StepState::Ok?QStringLiteral("이 단계는 확인됐습니다. 다음 필요한 단계로 진행하세요."):
            !r.action.isEmpty()?QStringLiteral("다음 행동: ")+r.action:QStringLiteral("안내를 확인하세요. 아직 완료로 판정하지 않은 단계입니다."));
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
        m_verdict->setText(QStringLiteral("등록 진행 · 안내에 따라 같은 손가락을 대고 떼세요\n")+out.right(500));
    });
    connect(p,&QProcess::errorOccurred,this,[this,p](QProcess::ProcessError error){
        if(error!=QProcess::FailedToStart||m_operation!=p)return;
        m_operation=nullptr;m_diagnostics->setEnabled(true);
        setProperty("scanResultMessage",QStringLiteral("등록 도구를 시작하지 못했습니다: ")+p->errorString());
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
        setProperty("scanResultMessage",QStringLiteral("영상 도구를 시작하지 못했습니다: ")+p->errorString());
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
        runManagedAction({QStringLiteral("/usr/bin/python"),QStringLiteral(FPSTUDIO_SOURCE_DIR "/../../tools/driver_build.py")},false);return;
    }
    if(r.id==StepId::GpuAuth||r.id==StepId::PamKde||r.id==StepId::PamPolkit||r.id==StepId::PamSudo) {
        runGpuInstall();return;
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
    if(m_operation){m_verdict->setText(QStringLiteral("작업이 끝나거나 인증 요청이 취소될 때까지 기다려 주세요. 설정 기록과 복구 정보를 보호하고 있습니다."));return;}
    QDialog::reject();
}
void SetupWizard::done(int result) {
    if(m_operation){m_verdict->setText(QStringLiteral("실행 중인 설정 작업을 완료하거나 인증 요청을 취소한 뒤 닫아 주세요."));return;}
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
        setProperty("scanResultMessage",ok?QStringLiteral("설정 작업 완료. 시스템 지문 시험으로 실제 경로를 확인하세요."):QStringLiteral("설정 미완료·취소: ")+output);
        rescan();
        p->deleteLater();
    };
    connect(p,qOverload<int,QProcess::ExitStatus>(&QProcess::finished),this,[finish](int code,QProcess::ExitStatus status){finish(status==QProcess::NormalExit&&code==0);});
    connect(p,&QProcess::errorOccurred,this,[finish](QProcess::ProcessError error){if(error==QProcess::FailedToStart)finish(false);});
    m_verdict->setText(privileged?QStringLiteral("관리자 권한 요청 후 자동 설치·검사합니다. 인증 연결 설치가 실패하면 원래 설정을 복구합니다."):
        QStringLiteral("일반 사용자 권한으로 드라이버를 빌드합니다. 의존성과 패키지 설치 때만 관리자 권한을 요청합니다."));
    if(privileged)p->start(QStringLiteral("/usr/bin/pkexec"),arguments);
    else p->start(arguments.first(),arguments.mid(1));
}

void SetupWizard::runGpuInstall() {
    QStringList directories;
    const auto options=QCoreApplication::arguments();
    for(int i=0;i+1<options.size();++i)if(options[i]=="--auth-reference-dir")directories<<options[++i];
    if(directories.isEmpty()) {
        const QString directory=QFileDialog::getExistingDirectory(this,QStringLiteral("본인의 저장 지문 세션 선택"),QStringLiteral(FPSTUDIO_SOURCE_DIR "/../../local-private/recognition"));
        if(directory.isEmpty())return;
        directories<<directory;
    }
    if(QMessageBox::question(this,QStringLiteral("실험적 GPU 인증 연결"),
        QStringLiteral("선택한 폴더의 지문이 현재 사용자 본인의 것인지 확인하세요.\n%1\n\n기존 등록은 보존하고 sudo·KDE 인증에 GPU 비교를 사용합니다. 최대 10회/90초 후 비밀번호 경로를 유지합니다. 다른 지문 거절 성능은 미검증입니다. 설정 백업 후 적용할까요?").arg(directories.join('\n')))!=QMessageBox::Yes)return;
    QStringList args{QStringLiteral("/usr/bin/python"),QStringLiteral(FPSTUDIO_SOURCE_DIR "/../../tools/auth_install.py"),"--user",qEnvironmentVariable("USER"),"--enable-experimental-auth","--apply"};
    for(const auto &dir:directories)args<<"--reference-dir"<<dir;
    runManagedAction(args);
}

void SetupWizard::runRecovery() {
    QFile file(QStringLiteral("/etc/fpstudio-auth.json"));if(!file.open(QIODevice::ReadOnly))return;
    const auto path=QJsonDocument::fromJson(file.readAll()).object().value("backup").toString();
    if(path.isEmpty())return;
    if(QMessageBox::question(this,QStringLiteral("인증 설정 복구"),QStringLiteral("설치 전 sudo·KDE·fprintd 설정을 복구할까요? 기존 지문 등록은 삭제하지 않습니다."))!=QMessageBox::Yes)return;
    runManagedAction({QStringLiteral("/usr/bin/python"),QStringLiteral(FPSTUDIO_SOURCE_DIR "/../../tools/auth_install.py"),"--rollback",path});
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
        m_verdict->setText(matched?QStringLiteral("지문 비교 성공. 손가락을 계속 대고 있어도 결과가 이미 전달됐습니다. sudo/KDE 대화창과 비밀번호 폴백은 별도 시험이 필요합니다."):QStringLiteral("시스템 지문 시험 미완료: ")+output.right(1000));p->deleteLater();
    };
    connect(p,qOverload<int,QProcess::ExitStatus>(&QProcess::finished),this,[finish](int code,QProcess::ExitStatus status){finish(code==0&&status==QProcess::NormalExit);});
    connect(p,&QProcess::errorOccurred,this,[finish](QProcess::ProcessError error){if(error==QProcess::FailedToStart)finish(false);});
    connect(p,&QProcess::readyReadStandardOutput,this,[this,p]{
        const auto output=QString::fromUtf8(p->readAllStandardOutput());
        p->setProperty("transcript",(p->property("transcript").toString()+output).right(4000));
        if(output.contains(QStringLiteral("verify-match"))) {
            // Show the actual match as soon as fprintd emits it, rather than
            // making the person wait for the sensor's cleanup/release phase.
            m_verdict->setText(QStringLiteral("[시스템 지문 시험] 일치했습니다. 손가락을 유지한 상태에서 결과가 확인됐습니다.\n")+output.right(700));
        } else if(output.contains(QStringLiteral("verify-no-match"))) {
            m_verdict->setText(QStringLiteral("[시스템 지문 시험] 일치하지 않습니다. 이 접촉은 기록·등록하지 않았습니다.\n")+output.right(700));
        } else {
            m_verdict->setText(QStringLiteral("[시스템 지문 시험] 손가락을 센서 중앙에 올린 뒤 결과가 표시될 때까지 그대로 유지하세요. 떼는 동작은 필요 없습니다.\n")+output.right(700));
        }
    });
    QTimer::singleShot(95000,p,[p]{if(p->state()!=QProcess::NotRunning)p->kill();});
    m_verdict->setText(QStringLiteral("[시스템 지문 시험] 처음에는 센서에서 손을 떼고, 지문 요청이 뜨면 등록한 같은 손가락을 올린 뒤 결과가 표시될 때까지 유지하세요. 떼는 동작은 필요 없습니다."));
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
