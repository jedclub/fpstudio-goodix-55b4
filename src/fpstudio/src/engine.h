// Thin, blocking wrapper around libfprint plus a tap on GLib's log stream.
//
// libfprint's sync API spins its own GMainLoop, so every call here blocks and
// must be run off the GUI thread. The GUI drives it from a worker; the CLI and
// MCP front ends call it directly on their only thread.
//
// The log tap is the point of the exercise. Everything interesting the driver
// knows - which activation stage it reached, the SIGFM score for each enrolled
// sample, why a TLS handshake failed - it says through fp_dbg() and then throws
// away. Installing a GLib writer keeps those lines and parses the ones with
// structure, so the UI can show a score instead of a user squinting at
// journalctl.

#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QJsonObject>
#include <QJsonArray>

#include <functional>
#include <memory>

struct _FpContext;
struct _FpDevice;
struct _FpPrint;

namespace fpstudio {

// One line lifted from libfprint's log stream.
struct LogLine {
    QString domain;
    QString text;
    qint64  monotonicMs = 0;
};

// A parsed "sigfm score N/M" line.
//
// N is the score: the number of match pairs whose relative geometry agrees,
// counted over every pair of pairs, so it grows quadratically and its range is
// enormous - two identical frames score in the millions. M is not a count at
// all but the driver's acceptance threshold, printed on every line by
// fpi_print_sigfm_match. For this sensor that is bz3_threshold = 24 * 3 = 72.
//
// So compare score against total, and take the threshold from the data rather
// than assuming one: a different driver prints a different M.
//
// A run that peaks just under the threshold is a placement problem - too
// little overlap between probe and enrolled sample. All-zero scores mean
// something structural instead: sigfm_match_score bails early and returns
// exactly 0 when fewer than five descriptors match, or when fewer than five
// of those pairs agree on length.
struct MatchScore {
    int score = 0;
    int total = 0;
};

struct DeviceInfo {
    QString id;
    QString name;
    QString driver;
    QString scanType;       // "press" or "swipe"
    int     enrollStages = 0;
    bool    supportsCapture = false;
    bool    supportsIdentify = false;
    bool    open = false;

    QJsonObject toJson() const;
};

// Emitted as a run progresses so a front end can show stages rather than a
// spinner. Ordering is the driver's, not ours: we report what we observe.
enum class Stage {
    Idle,
    Enumerating,
    Opening,
    Activating,
    WaitingForFinger,
    Capturing,
    Extracting,
    Matching,
    Done,
    Failed,
};

QString stageName(Stage s);

class Engine : public QObject {
    Q_OBJECT

public:
    explicit Engine(QObject *parent = nullptr);
    ~Engine() override;

    // Called the moment a notable line appears in the driver's log, so a front
    // end can report progress while an operation is still blocked rather than
    // only after it returns. stage is a token, human is the sentence to show.
    // Set before any operation; it is process-wide, like the log writer.
    using ProgressHook = std::function<void(const QString &stage, const QString &human)>;
    static void setProgressHook(ProgressHook hook);

    // The driver measures every captured frame before deciding whether to
    // spend an enrolment stage on it, and prints what it saw. Those two
    // numbers are the only direct read on how well the finger is landing, so
    // they are worth putting in front of the person doing the landing rather
    // than leaving in a log. accepted says whether the driver kept the frame.
    using QualityHook = std::function<void(int coverage, int sharpness, bool accepted)>;
    static void setQualityHook(QualityHook hook);

    // Turns on libfprint's own debug output for this process. Without it the
    // driver stays quiet and the log tap has nothing to catch. Call before
    // any other method.
    static void enableVerboseLogging();

    bool        enumerate(QString *error);
    QList<DeviceInfo> devices() const;

    // Index into devices(). Opening is what runs the activation sequence, so
    // this is where a device that cannot be talked to fails.
    bool open(int index, QString *error);
    void close();
    bool isOpen() const;
    DeviceInfo currentDevice() const;

    // A raw frame, no enrolment or matching. This is the call that answers
    // "what does the sensor actually see", which is the question a score of
    // 0/72 cannot answer on its own.
    // timeoutSecs bounds the whole call. libfprint's sync API waits forever on
    // a NULL cancellable, and the states this driver can sit in - waiting for a
    // finger, and then waiting for it to leave - are both driven by a human who
    // may simply walk away. Measured: sensor ready 0.3s, capture 40ms after
    // detection; everything beyond that is waiting on a person.
    bool captureImage(bool waitForFinger, int timeoutSecs, QImage *out, QString *error);

    // finger is an FpFinger value; username scopes the stored print.
    bool enroll(const QString &username, int finger, int timeoutSecs, QString *error);
    bool verify(const QString &username, int finger, int timeoutSecs,
                bool *matched, QString *error);
    bool listPrints(const QString &username, QStringList *out, QString *error);
    bool deletePrints(const QString &username, QString *error);

    // Scores parsed out of the log during the most recent verify.
    QList<MatchScore> lastScores() const;

    // verify() has to drain the log to find the scores, so the lines it
    // consumed are kept here rather than lost. Callers that want everything
    // should read this as well as takeLog().
    QList<LogLine>    lastRunLog() const;

    QList<LogLine>    takeLog();          // drains the shared buffer

signals:
    void stageChanged(fpstudio::Stage stage, const QString &detail);
    void enrollProgress(int completed, int total);
    void imageReady(const QImage &image);
    void logged(const fpstudio::LogLine &line);

private:
    struct Priv;
    std::unique_ptr<Priv> d;
};

} // namespace fpstudio

Q_DECLARE_METATYPE(fpstudio::Stage)
Q_DECLARE_METATYPE(fpstudio::LogLine)
