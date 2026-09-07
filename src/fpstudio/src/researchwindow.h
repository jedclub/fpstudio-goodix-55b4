#pragma once
#include <QDialog>
#include <QJsonArray>
#include <QProcess>
#include <QTimer>
#include <memory>

class QLabel;
class QPushButton;
class QProgressBar;
class QLockFile;
class QCloseEvent;

namespace fpstudio {
// Owns the capture protocol. MCP starts/observes/stops a session; it never
// times individual touches. This window remains usable without an agent.
class ResearchWindow : public QDialog {
public:
    explicit ResearchWindow(const QString &directory, QWidget *parent = nullptr,
                            const QString &captureProgram = {}, int prepareMs = 5000, int reviewMs = 4000,
                            bool autoRun = true);
    ~ResearchWindow() override;
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    void tick();
    void prepare();
    void capture();
    void finished(int code, QProcess::ExitStatus status);
    void announce(const QString &state, const QString &instruction, const QString &detail);
    void persist();
    void stop();
    QString phase() const;
    QString placement() const;
    QString filename() const;

    QString m_dir, m_state, m_instruction, m_detail, m_path, m_captureProgram;
    QByteArray m_output, m_errors;
    QJsonArray m_samples, m_attempts;
    QProcess m_capture;
    QTimer m_tick;
    std::unique_ptr<QLockFile> m_lock;
    QLabel *m_heading, *m_instructionLabel, *m_detailLabel, *m_preview, *m_quality;
    QPushButton *m_stop;
    QProgressBar *m_progress;
    int m_index = 0, m_retries = 0, m_attempt = 0, m_prepareMs, m_reviewMs;
    bool m_cancelled = false, m_ready = false;
    qint64 m_deadline = 0;
};
}
