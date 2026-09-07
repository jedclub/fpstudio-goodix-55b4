#pragma once
#include <QDialog>
#include <QElapsedTimer>
#include <QProcess>
#include <QTimer>
#include <QLockFile>
#include <QImage>
#include <QJsonObject>
#include <QMap>
#include <memory>
class QLabel;
class QPushButton;
namespace fpstudio {
class LiveWindow : public QDialog {
public:
    explicit LiveWindow(const QString &directory, QWidget *parent = nullptr,
                        const QString &program = {}, int prepareMs = 5000,
                        const QString &referenceDir = {}, bool differentFinger = false,
                        const QString &matcherProgram = {}, const QString &fastBank = {}, int targetContacts = 0);
    ~LiveWindow() override;
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    void start();
    void tick();
    void stop(bool completed = false);
    void startMatcher(const QString &referenceDir, const QString &program, const QString &fastBank);
    void matchFrame(const QImage &frame, const QString &kind = "quality-best");
    void dispatchMatch();
    void writeStatus();
    void failPending(const QString &reason);
    void finishMatching();
    void showMatchOverlay(const QJsonObject &result);
    QString m_dir, m_program;
    QString m_fingerPrompt;
    bool m_differentFinger = false;
    QLabel *m_instruction, *m_image, *m_metrics;
    QLabel *m_matchLabel;
    QLabel *m_matchImage;
    QPushButton *m_stop;
    QProcess m_worker;
    QByteArray m_result;
    QProcess m_matcher;
    QByteArray m_matchOutput;
    bool m_gpuReady = false, m_gpuBusy = false;
    bool m_gpuStopExpected = false;
    int m_gpuRequest = 0;
    int m_gpuContact = 0;
    qint64 m_gpuStamp = 0;
    QString m_gpuSelection;
    QJsonObject m_gpuStatus;
    struct PendingFrame { QImage image; qint64 stamp; QString kind; };
    QMap<int, PendingFrame> m_pending;
    QMap<int, QJsonObject> m_contacts;
    bool m_matchingEnabled = false, m_gpuFailed = false;
    int m_targetContacts = 0;
    bool m_targetReached = false;
    int m_gpuCompleted = 0, m_gpuErrors = 0, m_gpuCancelled = 0;
    qint64 m_lastStatusWrite = -1000;
    QTimer m_tick;
    QElapsedTimer m_clock;
    std::unique_ptr<QLockFile> m_lock;
    qint64 m_lastStamp = 0, m_lastArrival = 0, m_windowStart = 0;
    int m_prepareMs, m_frames = 0, m_total = 0;
    double m_fps = 0;
    bool m_calibrated = false, m_contactOn = false;
    double m_centeredSignal = 0, m_rawSignal = 0, m_noise = 0;
    QImage m_previous;
    QImage m_lastMatchCandidate;
    double m_bestScore = 0;
    int m_stable = 0, m_touch = 0, m_emptyFrames = 3, m_candidates = 0;
    bool m_started = false, m_stopping = false, m_finished = false;
};
}
