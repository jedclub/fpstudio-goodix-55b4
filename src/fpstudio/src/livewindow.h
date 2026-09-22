#pragma once

#include <memory>

namespace vkmap { class Placer; }
#include <QDialog>
#include <QElapsedTimer>
#include <QProcess>
#include <QTemporaryDir>
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
    Q_OBJECT
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

    // The coverage map shown on the right, and the one thing it is for.
    //
    // This is not the matcher's map. An earlier attempt to assemble captures
    // into one image and authenticate against it failed, and the reason is
    // physical rather than fixable: registering two captures taken either side
    // of a lift leaves a median error of 5.5 px on a sensor whose ridges repeat
    // every 8-10 px, so merging cancels the ridges instead of reinforcing them.
    // The assembled map scored 0.29 against probes where the raw gallery
    // scored 0.77.
    //
    // None of that matters here, because nobody is matching against this. It
    // exists so the person can see which parts of their finger the session has
    // actually reached and go and press the parts it has not. A placement that
    // is five pixels out is still in the right corner of the map, and the right
    // corner is the whole question. What the session produces is still the
    // individual captures - this only makes their coverage visible while there
    // is still time to improve it.
    void placeOnMap(const QImage &frame);
    // Which of this session's captures should become the enrolment, and
    // installing them. The map is what decides: a capture earns its place by
    // covering part of the finger nothing else covers, not by being sharp.
    QStringList selectEnrolment(int limit) const;
    void installEnrolment();
    void renderMap();

    // Write the coverage map out when the session ends.
    //
    // It only existed on screen before, which meant a session's coverage could
    // not be reviewed after the fact, compared against the next one, or looked
    // at at all once the window closed. It is research material like every
    // other capture here and is written the same way: 0600, beside them, and
    // labelled for what it is rather than mistaken later for a template.
    void traceFrame(bool touch, qint64 stamp, double signal, int coverage,
                    int sharpness, double motion, bool eligible);
    void saveMap();
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
    bool m_thermalStop = false;
    enum class MapOutcome { None, Placed, TooSmeared, NotRegistered, NoOverlap };
    MapOutcome m_mapOutcome = MapOutcome::None;
    int m_mapLastX = -1, m_mapLastY = -1;
    double m_mapLastFit = 0, m_mapLastQuality = 0, m_mapLastAngle = 0;
    int m_mapRejects = 0;
    // Split by reason. A single total cannot tell "the finger was smeared"
    // from "the placement was not trusted", and those call for opposite
    // changes - one to the capture gate, one to the registration floor.
    int m_rejectSmeared = 0, m_rejectUnregistered = 0, m_rejectAmbiguous = 0, m_rejectNoOverlap = 0;
    int m_trackedPlacements = 0, m_anchorPlacements = 0;
    // Built on first use and kept for the session. A machine without a usable
    // Vulkan compute queue falls back to the CPU search, which is the same
    // arithmetic over a narrower set of poses.
    std::unique_ptr<vkmap::Placer> m_placer;
    bool m_placerTried = false;
    double m_placeMs = 0;
    int m_gpuCompleted = 0, m_gpuErrors = 0, m_gpuCancelled = 0;
    qint64 m_lastStatusWrite = -1000;
    qint64 m_scanStart = 0;
    // libfprint models the sensor's temperature by counting the seconds it
    // spends scanning, with no reading from the device: three minutes of
    // activity flips it to FP_TEMPERATURE_HOT and it refuses to scan until it
    // has cooled, at a third of the rate it heated. A live preview is
    // continuously active, so a collection session reaches that ceiling and
    // ends on "Device disabled to prevent overheating" - which is what ended
    // the 2026-09-20 session at 180.01 seconds, exactly on the limit.
    //
    // Stopping ten seconds early turns that into an ordinary finish: the
    // captures, the map and the session record are all written by the normal
    // path, and the device is left cool enough to start again.
    static constexpr qint64 kScanBudgetMs = 170000;
    QTimer m_tick;
    QElapsedTimer m_clock;
    // Per-frame record of what the sensor reported, kept for one question the
    // saved candidates cannot answer: within a single contact, is the finger
    // still arriving or already leaving? Only the chosen frame of a contact is
    // saved as a candidate, so the shape of the contact is lost. The driver's
    // settle rule picks among frames 100ms apart and has to know which way they
    // are going.
    std::unique_ptr<QFile> m_trace;
    qint64 m_contactStart=0;
    // Frames that arrived while this window was busy elsewhere. The preview is
    // a single file the capture side overwrites, so falling behind does not
    // queue - it loses. A whole contact fits in the gap, and the per-contact
    // totals this window exists to produce would then simply be short, with
    // nothing on screen or on disk saying so.
    qint64 m_dropped=0;
    // The total says how much was lost; this says whether a contact could
    // have been lost inside it. A contact is several frames of touch, so a
    // gap shorter than that cannot hide one - and a gap longer than that is
    // the difference between a slow session and a wrong count.
    qint64 m_longestGap=0;
    std::unique_ptr<QLockFile> m_lock;
    qint64 m_lastStamp = 0, m_lastArrival = 0, m_windowStart = 0;
    int m_prepareMs, m_frames = 0, m_total = 0;
    double m_fps = 0;
    bool m_calibrated = false, m_contactOn = false;
    double m_centeredSignal = 0, m_rawSignal = 0, m_noise = 0;
    QImage m_previous;
    // Best sample per pixel, not a mean of every sample.
    //
    // A mean was the obvious thing and it is the wrong thing here. Two captures
    // of the same patch taken either side of a lift register to within about
    // 5.5 px of each other, and the ridges repeat every 8-10, so they disagree
    // by most of a ridge. Averaging them cancels what they were supposed to
    // reinforce, and averaging more of them does not help - pressing the same
    // spot repeatedly kept the blur exactly where it was, which is what
    // prompted this. The blur was not a shortage of samples; it was the mean.
    //
    // So each pixel keeps the single best contribution it has been offered and
    // the score that won it. A later contact that registers more confidently
    // replaces what is there; a worse one is ignored rather than mixed in. The
    // same measurement that showed the mean blurring showed this holding the
    // ridges: 1.43 of a raw capture's contrast against the mean's 0.64.
    QPushButton *m_install = nullptr;
    QProcess m_installer;
    std::unique_ptr<QTemporaryDir> m_stage;
    QVector<float> m_mapPixel, m_mapScore;
    // Pose carried across the frames of one uninterrupted contact.
    bool m_mapAnchored = false;
    // Per-frame placement is only affordable on the GPU. The CPU fallback
    // sweeps the map for every frame it is given, which at preview rate is
    // tens of times the work it was written for and stalls the window; on that
    // path the map goes back to one placement per contact.
    bool m_mapFastPath = false;
    // Placements are paced, not taken from every frame that arrives. Preview
    // frames come about 65ms apart, so this changes nothing in use; what it
    // bounds is a caller that supplies frames as fast as it can, where placing
    // each one is both pointless - the finger has not moved - and unbounded
    // work.
    qint64 m_lastPlaceMs = 0;
    // Placement runs on the thread that reads preview frames, so its cost has
    // to be paid out of that thread's budget or contacts are missed. A fixed
    // interval only works if placement is fast, which is an assumption about
    // the machine: CI has a software Vulkan device where the same call is an
    // order of magnitude slower, and it dropped 4 of 33 contacts. Waiting at
    // least as long again as the last placement took keeps frame reading ahead
    // of map building wherever this runs, at the cost of a sparser map on a
    // slow device - which is the right thing to give up.
    qint64 m_placeCostMs = 0;
    // Captures kept for enrolment, with where each one sits on the finger.
    // Keyed by nothing - a sliding finger is one contact, so "the best frame of
    // each contact" is one capture for a whole sweep.
    struct SavedCapture { QString path; int x=0,y=0; double quality=0; };
    QVector<SavedCapture> m_saved;
    int m_candidateIndex = 0;
    int m_lastSavedX = 0, m_lastSavedY = 0;
    bool m_haveSavedPose = false;
    // How far the finger must have travelled before another capture is worth
    // keeping. Two views 10px apart are nearly the same view; the sensor is
    // 108px wide, so this samples a sweep about four deep.
    static constexpr int kSweepSampleStride = 24;
    static constexpr qint64 kMapPlaceIntervalMs = 50;
    int m_trackX = 0, m_trackY = 0;
    double m_trackAngle = 0;
    int m_mapPlaced = 0;
    static constexpr int kMapSize = 300;
    // Ridge sharpness a capture must reach to go on the map. The measure this
    // replaces - structure-tensor coherence at 0.85 - kept 62% of a session and
    // the third it discarded was the whorl core, because coherence falls where
    // ridges curve and rises when an image is blurred. This keeps about 90%,
    // and what it now drops is actually soft.
    static constexpr double kMapMinSharpness = 0.357;
    static constexpr double kMapSeedFit = 0.70;
    static constexpr float kMapWeightCap = 3.0f;
    // How well a capture must agree with the map before it is allowed to
    // change it.
    //
    // This was 0.30, which rejected nothing: measured by placing ten captures
    // of a DIFFERENT finger on a finished map, the wrong finger still scored
    // up to 0.575. A floor beneath that is not a floor. Placements that
    // survive it are fused in and corrupt the map for every later placement,
    // which is why a session degrades as it goes rather than failing outright.
    static constexpr double kMapMinFit = 0.62;
    // Re-anchoring a contact has to be unambiguous, not merely best. Ridges
    // repeat every 8-10px, so a placement one ridge out scores nearly as well
    // as the right one and picking the maximum picks wrong about as often as
    // right. A candidate is only used if no placement at least kMapPeakRadius
    // away comes within kMapMinPeakGap of it.
    // A frame that never lifted is not asking "does this belong here" - the
    // finger was here a sixteenth of a second ago. It only has to still agree,
    // and the map it is agreeing with is a blend of several presses, so it
    // scores lower than a fresh capture would against another fresh capture.
    // Holding it to the anchoring floor rejected 777 of 987 frames in a
    // measured session, most of them frames that were tracking correctly.
    static constexpr double kMapTrackMinFit = 0.45;
    static constexpr double kMapMinPeakGap = 0.06;
    static constexpr int kMapPeakRadius = 6;
    // Tracking a contact that never lifted is a different problem from finding
    // where a new contact belongs. Between preview frames the finger moves a
    // few pixels, and registration at that distance is accurate to hundredths
    // of a pixel, where registration across a lift is accurate to about 5.5 -
    // most of a ridge. So the pose is carried forward while contact holds and
    // only searched for again when it breaks.
    static constexpr int kMapTrackRadius = 5;
    static constexpr double kMapTrackAngleSpan = 2.0;
    QImage m_lastMatchCandidate;
    double m_bestScore = 0;
    int m_stable = 0, m_touch = 0, m_emptyFrames = 3, m_candidates = 0;
    bool m_started = false, m_stopping = false, m_finished = false;
};
}
