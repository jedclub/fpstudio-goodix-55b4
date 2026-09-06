#pragma once

#include <QMainWindow>
#include <QImage>
#include <QJsonObject>

#include "privrunner.h"

class QFileSystemWatcher;

class QLabel;
class QListWidget;
class QPlainTextEdit;
class QPushButton;
class QMenu;
class QProgressBar;
class QTableWidget;

namespace fpstudio {

class ImageView;

// The window owns no device state. Everything that touches the sensor goes
// through PrivRunner, which runs this same binary in --cli mode under pkexec
// and hands back the JSON. That keeps the GUI unprivileged and means the two
// front ends cannot drift apart - they are the same code path.
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    // Called by main() after show(). Not from the constructor: probing blocks
    // for seconds and the window has to be visible first, or the program looks
    // hung at the moment someone starts it.
    void maybeOfferSetup();

    // Opens the checklist on demand - the Setup menu, and `--setup`.
    void openSetupWizard();

private slots:
    void onFinished(const QJsonObject &result);
    void onFailed(const QString &error);
    void onBeaconChanged();
    void tickElapsed();

private:
    void buildUi();
    void buildLanguageMenu();
    void wireActions();
    void setBusy(bool busy, const QString &what = {});
    void appendLog(const QString &text);
    void applyLog(const QJsonObject &result);
    void showDevices(const QJsonObject &result);
    void showCapture(const QJsonObject &result);
    void showVerify(const QJsonObject &result);
    void addStage(const QString &text);

    PrivRunner *m_runner = nullptr;
    QString     m_pending;              // which command is in flight

    // Watches the status file the privileged worker writes, so a request an
    // agent made over MCP shows up here as an instruction the user can act on.
    QFileSystemWatcher *m_beaconWatch = nullptr;
    QLabel             *m_banner      = nullptr;   // 서 있는 지시문
    QLabel             *m_liveStatus  = nullptr;   // 지금 벌어지는 일

    // The driver's reading of the frame it just took. Worth its own corner of
    // the window rather than a log line: sharpness tracks whether the match
    // will succeed closely enough to act on - measured over three verifies,
    // 21 scored 23 and failed while 28 and 30 scored 1349 and 1686 - and the
    // person can only act on it while the finger is still going down.
    QLabel      *m_qualityText = nullptr;
    QProgressBar *m_barCoverage = nullptr;
    QProgressBar *m_barSharpness = nullptr;
    QWidget     *m_qualityBox = nullptr;
    void showQuality(int coverage, int sharpness, bool accepted);
    qint64              m_startedAt   = 0;         // 비콘이 알려준 시작 시각
    QString             m_statusText;              // 경과를 뺀 상태 문구

    // Kept so the menu can re-check the right entry after a switch, and so
    // the whole window can be rebuilt in the new language without hunting for
    // the action that triggered it.
    QMenu          *m_langMenu   = nullptr;

    QListWidget    *m_deviceList = nullptr;
    QLabel         *m_deviceInfo = nullptr;
    ImageView      *m_imageView  = nullptr;
    QListWidget    *m_stageList  = nullptr;
    QTableWidget   *m_scoreTable = nullptr;
    QPlainTextEdit *m_log        = nullptr;
    QProgressBar   *m_progress   = nullptr;
    QLabel         *m_verdict    = nullptr;

    QPushButton *m_btnRefresh = nullptr;
    QPushButton *m_btnCapture = nullptr;
    QPushButton *m_btnEnroll  = nullptr;
    QPushButton *m_btnVerify  = nullptr;
    QPushButton *m_btnDelete  = nullptr;
};

} // namespace fpstudio
