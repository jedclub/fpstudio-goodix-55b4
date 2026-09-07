// The wizard that gets a machine from nothing to a working fingerprint unlock.
//
// Everything it does was, until now, something a person had to do by reading
// eight documents and running a dozen commands in the right order - or
// something an agent did for them, deciding at each turn what had gone wrong
// and what to try next. Those decisions are in setupcheck.h now, and this is
// the surface over them.
//
// Three principles it holds to, because each one was learned the hard way in
// getting this sensor working at all:
//
//   Never act without saying what will happen. Every fix shows its exact
//   commands before it runs. One step cannot be undone, and that one asks for
//   more than a click.
//
//   Never claim success it has not checked. After each fix it re-probes and
//   shows what the machine now says, not what the fix intended.
//
//   Never require the whole path. Half-configured is a legitimate place to
//   stop - the udev rule and the PAM stack are both optional, and skipping
//   them costs password prompts, not function.

#pragma once

#include <QDialog>

#include "setupcheck.h"

class QLabel;
class QListWidget;
class QProgressBar;
class QPushButton;
class QStackedWidget;
class QTextBrowser;
class QProcess;

namespace fpstudio {

class SetupWizard : public QDialog {
    Q_OBJECT
public:
    explicit SetupWizard(QWidget *parent = nullptr);
    void reject() override;
    void done(int result) override;

private slots:
    void rescan();
    void runCurrentFix();
    void runEnrolment();
    void runCaptureTest();
    void skipCurrent();
    void selectRow(int row);
    void openDiagnostics();
    void runGpuInstall();
    void runRecovery();
    void runSystemVerify();

private:
    void render();
    void showStep(int index);
    bool confirmIrreversible(const StepResult &r);
    void applyResult(int index, const StepResult &r);
    void runManagedAction(const QStringList &arguments,bool privileged=true);
    QProcess *m_operation=nullptr;
    bool m_scanning=false;
    void selectNextStep();
    QPushButton *m_recover=nullptr;
    QPushButton *m_verify=nullptr;
    QPushButton *m_stopOperation=nullptr;

    QVector<StepResult> m_steps;
    int                 m_current = 0;

    QListWidget    *m_list    = nullptr;
    QLabel         *m_title   = nullptr;
    QTextBrowser   *m_detail  = nullptr;
    QLabel         *m_cmds    = nullptr;
    QPushButton    *m_fix     = nullptr;
    QPushButton    *m_skip    = nullptr;
    QPushButton    *m_rescan  = nullptr;
    QLabel         *m_verdict = nullptr;
    QProgressBar   *m_busy    = nullptr;

    // Only present when this wizard is the application's own top-level window
    // rather than something MainWindow opened over itself - opened from
    // diagnostics, offering a way back into diagnostics would just be a second
    // way to reach the window already open behind it.
    QPushButton    *m_diagnostics = nullptr;

    // Shown above the step list once every gating step is Ok or Skipped -
    // a wizard that ends by quietly leaving eight green checks on screen
    // still leaves the person guessing whether they are done. This says so,
    // and Finish is the deliberate way out rather than the window's own
    // close button.
    QWidget        *m_completion  = nullptr;
    QLabel         *m_completionText = nullptr;
    QPushButton    *m_finish      = nullptr;
};

} // namespace fpstudio
