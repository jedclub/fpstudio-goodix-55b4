#include "engine.h"

#include <QCoreApplication>

#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QMutex>
#include <QMutexLocker>
#include <QRegularExpression>

// Qt defines `signals` as a macro expanding to `public:`, and GLib's
// GDBusInterfaceInfo has a field of that name. Including libfprint (which
// pulls in gio) after any Qt header therefore fails to parse. engine.h has
// already been read by this point, so dropping the macro here is safe; it is
// restored afterwards for the rest of the translation unit.
#undef signals
#include <libfprint-2/fprint.h>
#define signals Q_SIGNALS

#include <atomic>

namespace fpstudio {

QString stageName(Stage s)
{
    switch (s) {
    case Stage::Idle:             return QStringLiteral("idle");
    case Stage::Enumerating:      return QStringLiteral("enumerating");
    case Stage::Opening:          return QStringLiteral("opening");
    case Stage::Activating:       return QStringLiteral("activating");
    case Stage::WaitingForFinger: return QStringLiteral("waiting-for-finger");
    case Stage::Capturing:        return QStringLiteral("capturing");
    case Stage::Extracting:       return QStringLiteral("extracting");
    case Stage::Matching:         return QStringLiteral("matching");
    case Stage::Done:             return QStringLiteral("done");
    case Stage::Failed:           return QStringLiteral("failed");
    }
    return QStringLiteral("unknown");
}

QJsonObject DeviceInfo::toJson() const
{
    return QJsonObject{
        {QStringLiteral("id"), id},
        {QStringLiteral("name"), name},
        {QStringLiteral("driver"), driver},
        {QStringLiteral("scan_type"), scanType},
        {QStringLiteral("enroll_stages"), enrollStages},
        {QStringLiteral("supports_capture"), supportsCapture},
        {QStringLiteral("supports_identify"), supportsIdentify},
        {QStringLiteral("open"), open},
    };
}

// ---------------------------------------------------------------- log tap

namespace {

QMutex          g_logMutex;
QList<LogLine>  g_logBuffer;
Engine         *g_logSink = nullptr;      // only for the signal; guarded above
QElapsedTimer   g_logClock;

Engine::ProgressHook g_progressHook;
Engine::QualityHook  g_qualityHook;

// Maps the driver's own chatter onto something worth putting in front of a
// person. Only lines that change what the user should do or expect are
// listed; everything else stays in the log pane.
//
// The finger-status lines are the important ones: without them a capture that
// is waiting looks identical to a capture that has hung.
bool humaniseLine(const QString &text, QString *stage, QString *human)
{
    if (text.contains(QLatin1String("SCAN_EMPTY_NUM completed successfully"))) {
        *stage = QStringLiteral("background-ready");
        *human = QCoreApplication::translate("fpstudio", "Put your finger on the sensor and hold it there");
        return true;
    }
    // A PRESENT-only status also occurs after AWAIT_FINGER_OFF when the
    // NEEDED flag is cleared. It must not overwrite "lift your finger".
    if (text.contains(QLatin1String("Image device reported finger status: on"))) {
        *stage = QStringLiteral("finger-on");
        *human = QCoreApplication::translate("fpstudio", "Finger detected — hold still");
        return true;
    }
    if (text.contains(QLatin1String("finger status: off"))) {
        *stage = QStringLiteral("finger-off");
        *human = QCoreApplication::translate("fpstudio", "Finger lifted");
        return true;
    }
    // The driver's quality gate hands a scan back instead of spending an
    // enrolment stage on it, and the person at the sensor is the only one who
    // can fix what it objected to. Saying which of the two checks failed is
    // the difference between a useful instruction and "try again".
    //
    // These arrive through GLib's log writer because the driver emits them
    // with g_message; g_print, which the rest of that driver uses, bypasses
    // the writer and would never reach here.
    if (text.contains(QLatin1String("Image rejected: partial contact"))) {
        *stage = QStringLiteral("rejected-partial");
        *human = QCoreApplication::translate("fpstudio", "Only part of the sensor was touched — cover more of it, centred");
        return true;
    }
    if (text.contains(QLatin1String("Image rejected: no ridge detail"))) {
        *stage = QStringLiteral("rejected-smeared");
        *human = QCoreApplication::translate("fpstudio", "The ridges are faint — press a little harder");
        return true;
    }
    if (text.contains(QLatin1String("captured an image"))) {
        *stage = QStringLiteral("captured");
        *human = QCoreApplication::translate("fpstudio", "Frame received");
        return true;
    }
    // The driver spends several seconds here polling for the finger to leave,
    // and the user has no way to know that is what it wants. Measured on this
    // sensor: the capture itself takes ~40ms after detection, then ~7s waiting
    // for release - so this line turns most of a run's dead time into
    // something the user can act on.
    if (text.contains(QLatin1String("AWAIT_FINGER_OFF"))) {
        *stage = QStringLiteral("await-finger-off");
        *human = QCoreApplication::translate("fpstudio", "Captured — now lift your finger");
        return true;
    }
    // Extraction takes ~70ms; what the user then waits on is the driver
    // polling for the finger to leave, which takes seconds. Reporting the
    // extraction as the current activity is accurate for a blink and
    // misleading for the rest, so this line announces the wait instead - it
    // arrives right when the release becomes the thing to do.
    if (text.contains(QLatin1String("sigfm extract completed"))) {
        *stage = QStringLiteral("await-finger-off");
        *human = QCoreApplication::translate("fpstudio", "Captured — now lift your finger");
        return true;
    }
    if (text.startsWith(QLatin1String("sigfm score"))) {
        *stage = QStringLiteral("matching");
        *human = QCoreApplication::translate("fpstudio", "Matching against the enrolled print…");
        return true;
    }
    if (text.contains(QLatin1String("HANDSHAKE DONE"))) {
        *stage = QStringLiteral("tls-ready");
        *human = QCoreApplication::translate("fpstudio", "Secure channel to the sensor established");
        return true;
    }
    if (text.contains(QLatin1String("Image device activation completed"))) {
        *stage = QStringLiteral("activated");
        *human = QCoreApplication::translate("fpstudio", "Sensor ready");
        return true;
    }
    return false;
}

// GLib hands structured fields; MESSAGE is the human line, GLIB_DOMAIN the
// emitter. Everything is kept - filtering is a front-end decision, and a line
// that looks like noise now is often the one that explains the next failure.
GLogWriterOutput logWriter(GLogLevelFlags, const GLogField *fields, gsize n_fields, gpointer)
{
    LogLine line;
    for (gsize i = 0; i < n_fields; ++i) {
        const QString key = QString::fromUtf8(fields[i].key);
        if (fields[i].length != -1)
            continue;                       // non-string payload, skip
        const QString val = QString::fromUtf8(static_cast<const char *>(fields[i].value));
        if (key == QLatin1String("MESSAGE"))
            line.text = val;
        else if (key == QLatin1String("GLIB_DOMAIN"))
            line.domain = val;
    }
    if (line.text.isEmpty())
        return G_LOG_WRITER_HANDLED;

    {
        QMutexLocker lock(&g_logMutex);
        if (!g_logClock.isValid())
            g_logClock.start();
        line.monotonicMs = g_logClock.elapsed();
        g_logBuffer.append(line);
        // Keep the buffer bounded; a long verify is chatty.
        if (g_logBuffer.size() > 5000)
            g_logBuffer.removeFirst();
    }

    // Outside the lock: the hooks write a file and must not hold it.
    if (g_qualityHook) {
        // "Image quality: coverage 76%, sharpness 29" - the driver's own
        // measurement of the frame it just took. Parsed rather than
        // recomputed here because the numbers have to be the ones the gate
        // actually judged, or the display would disagree with the decision.
        static const QRegularExpression re(
            QStringLiteral("Image quality: coverage (\\d+)%, sharpness (\\d+)"));
        const auto m = re.match(line.text);
        if (m.hasMatch())
            g_qualityHook(m.captured(1).toInt(), m.captured(2).toInt(), true);
        else if (line.text.contains(QLatin1String("Image rejected")))
            g_qualityHook(-1, -1, false);
    }

    if (g_progressHook) {
        QString stage, human;
        if (humaniseLine(line.text, &stage, &human))
            g_progressHook(stage, human);
    }
    return G_LOG_WRITER_HANDLED;
}

// A GCancellable that trips after n seconds. libfprint checks it between
// protocol steps, so the call unwinds cleanly rather than being killed.
struct Deadline {
    GCancellable *c = nullptr;
    guint         id = 0;

    explicit Deadline(int secs)
    {
        c = g_cancellable_new();
        if (secs > 0)
            id = g_timeout_add_seconds(secs, [](gpointer p) -> gboolean {
                g_cancellable_cancel(static_cast<GCancellable *>(p));
                return G_SOURCE_REMOVE;
            }, c);
    }
    ~Deadline()
    {
        if (id && g_main_context_find_source_by_id(nullptr, id)) g_source_remove(id);
        if (c) g_object_unref(c);
    }
    bool tripped() const { return c && g_cancellable_is_cancelled(c); }
};

const QRegularExpression &scoreRe()
{
    static const QRegularExpression re(QStringLiteral(R"(sigfm score\s+(\d+)\s*/\s*(\d+))"));
    return re;
}

} // namespace

// ---------------------------------------------------------------- Priv

struct Engine::Priv {
    FpContext *ctx = nullptr;
    FpDevice  *dev = nullptr;              // borrowed from ctx, not owned
    QList<DeviceInfo> infos;
    QList<FpDevice *> handles;
    int currentIndex = -1;
    QList<MatchScore> scores;
    QList<LogLine> lastRunLog;

    ~Priv()
    {
        if (dev && fp_device_is_open(dev))
            fp_device_close_sync(dev, nullptr, nullptr);
        if (ctx)
            g_object_unref(ctx);
    }
};

Engine::Engine(QObject *parent)
    : QObject(parent), d(std::make_unique<Priv>())
{
    qRegisterMetaType<fpstudio::Stage>("fpstudio::Stage");
    qRegisterMetaType<fpstudio::LogLine>("fpstudio::LogLine");

    QMutexLocker lock(&g_logMutex);
    g_logSink = this;
    if (!g_logClock.isValid())
        g_logClock.start();
}

Engine::~Engine()
{
    QMutexLocker lock(&g_logMutex);
    if (g_logSink == this)
        g_logSink = nullptr;
}

void Engine::setProgressHook(ProgressHook hook)
{
    g_progressHook = std::move(hook);
}

void Engine::setQualityHook(QualityHook hook)
{
    g_qualityHook = std::move(hook);
}

void Engine::enableVerboseLogging()
{
    // libfprint gates fp_dbg on these. Set them in-process so a front end
    // works without the caller having to remember an env var.
    g_setenv("G_MESSAGES_DEBUG", "all", TRUE);
    g_setenv("LIBFPRINT_DEBUG", "3", FALSE);
    g_log_set_writer_func(logWriter, nullptr, nullptr);
    // The Goodix fork also uses g_print for protocol and background traces.
    // Route these through the same collector so stdout remains one JSON
    // result and failed captures retain their diagnostic evidence.
    g_set_print_handler([](const gchar *message) {
        g_log_structured("goodix-stdout", G_LOG_LEVEL_DEBUG,
                         "MESSAGE", "%s", message);
    });
}

QList<LogLine> Engine::takeLog()
{
    QMutexLocker lock(&g_logMutex);
    QList<LogLine> out = g_logBuffer;
    g_logBuffer.clear();
    return out;
}

QList<MatchScore> Engine::lastScores() const { return d->scores; }

QList<LogLine> Engine::lastRunLog() const { return d->lastRunLog; }

bool Engine::enumerate(QString *error)
{
    emit stageChanged(Stage::Enumerating, {});

    if (!d->ctx)
        d->ctx = fp_context_new();
    fp_context_enumerate(d->ctx);

    d->infos.clear();
    d->handles.clear();

    GPtrArray *devs = fp_context_get_devices(d->ctx);
    if (!devs) {
        if (error) *error = QStringLiteral("fp_context_get_devices returned nothing");
        emit stageChanged(Stage::Failed, *error);
        return false;
    }

    for (guint i = 0; i < devs->len; ++i) {
        auto *dev = static_cast<FpDevice *>(g_ptr_array_index(devs, i));
        DeviceInfo info;
        info.id      = QString::fromUtf8(fp_device_get_device_id(dev));
        info.name    = QString::fromUtf8(fp_device_get_name(dev));
        info.driver  = QString::fromUtf8(fp_device_get_driver(dev));
        info.scanType = fp_device_get_scan_type(dev) == FP_SCAN_TYPE_PRESS
                          ? QStringLiteral("press") : QStringLiteral("swipe");
        info.enrollStages    = fp_device_get_nr_enroll_stages(dev);
        info.supportsCapture = fp_device_supports_capture(dev);
        info.supportsIdentify = fp_device_supports_identify(dev);
        info.open   = fp_device_is_open(dev);
        d->infos.append(info);
        d->handles.append(dev);
    }

    emit stageChanged(Stage::Done, QStringLiteral("%1 device(s)").arg(d->infos.size()));
    return true;
}

QList<DeviceInfo> Engine::devices() const { return d->infos; }

bool Engine::open(int index, QString *error)
{
    if (index < 0 || index >= d->handles.size()) {
        if (error) *error = QStringLiteral("device index out of range");
        return false;
    }
    emit stageChanged(Stage::Opening, d->infos[index].name);

    FpDevice *dev = d->handles[index];
    GError *err = nullptr;
    if (!fp_device_open_sync(dev, nullptr, &err)) {
        if (error) *error = QString::fromUtf8(err ? err->message : "open failed");
        if (err) g_error_free(err);
        emit stageChanged(Stage::Failed, error ? *error : QString());
        return false;
    }
    d->dev = dev;
    d->currentIndex = index;
    d->infos[index].open = true;
    emit stageChanged(Stage::Done, QStringLiteral("opened"));
    return true;
}

void Engine::close()
{
    if (d->dev && fp_device_is_open(d->dev))
        fp_device_close_sync(d->dev, nullptr, nullptr);
    if (d->currentIndex >= 0 && d->currentIndex < d->infos.size())
        d->infos[d->currentIndex].open = false;
    d->dev = nullptr;
    d->currentIndex = -1;
    emit stageChanged(Stage::Idle, {});
}

bool Engine::isOpen() const { return d->dev && fp_device_is_open(d->dev); }

DeviceInfo Engine::currentDevice() const
{
    if (d->currentIndex >= 0 && d->currentIndex < d->infos.size())
        return d->infos[d->currentIndex];
    return {};
}

bool Engine::captureImage(bool waitForFinger, int timeoutSecs, QImage *out, QString *error)
{
    if (!isOpen()) {
        if (error) *error = QStringLiteral("no device open");
        return false;
    }
    if (!fp_device_supports_capture(d->dev)) {
        if (error) *error = QStringLiteral("driver does not support raw capture");
        return false;
    }

    emit stageChanged(waitForFinger ? Stage::WaitingForFinger : Stage::Capturing, {});

    Deadline dl(timeoutSecs);
    GError *err = nullptr;
    FpImage *img = fp_device_capture_sync(d->dev, waitForFinger, dl.c, &err);
    if (!img) {
        if (error) *error = dl.tripped()
            ? QCoreApplication::translate("fpstudio", "Gave up after %1 seconds").arg(timeoutSecs)
            : QString::fromUtf8(err ? err->message : "capture failed");
        if (err) g_error_free(err);
        emit stageChanged(Stage::Failed, error ? *error : QString());
        return false;
    }

    const int w = fp_image_get_width(img);
    const int h = fp_image_get_height(img);
    gsize len = 0;
    const guchar *data = fp_image_get_data(img, &len);

    // libfprint hands 8-bit greyscale, one byte per pixel, tightly packed.
    QImage qi(w, h, QImage::Format_Grayscale8);
    if (data && len >= static_cast<gsize>(w) * h) {
        for (int y = 0; y < h; ++y)
            memcpy(qi.scanLine(y), data + static_cast<gsize>(y) * w, w);
    } else {
        qi.fill(0);
    }
    g_object_unref(img);

    if (out) *out = qi;
    emit imageReady(qi);
    emit stageChanged(Stage::Done, QStringLiteral("%1x%2").arg(w).arg(h));
    return true;
}

namespace {

struct EnrollCtx {
    Engine *engine;
    int total;
};

void enrollProgressCb(FpDevice *, gint completed, FpPrint *, gpointer user, GError *)
{
    auto *c = static_cast<EnrollCtx *>(user);
    // Reported directly rather than through the signal: the CLI has no running
    // event loop while the blocking enroll call is in progress, so a queued
    // connection would only deliver after the whole thing finished.
    if (g_progressHook)
        g_progressHook(QStringLiteral("enroll-progress"),
                       QCoreApplication::translate("fpstudio", "Enrol %1/%2 — lift your finger and press again")
                           .arg(completed).arg(c->total));
    QMetaObject::invokeMethod(c->engine, [c, completed] {
        emit c->engine->enrollProgress(completed, c->total);
    }, Qt::QueuedConnection);
}

QString printPath(const QString &username, int finger)
{
    // Kept beside the tool rather than in fprintd's store: this is a
    // diagnostic instrument, and writing into the daemon's database would
    // make its results depend on state we did not put there.
    return QStringLiteral("%1/.local/share/fpstudio/%2-%3.fp")
        .arg(QDir::homePath()).arg(username).arg(finger);
}

} // namespace

bool Engine::enroll(const QString &username, int finger, int timeoutSecs, QString *error)
{
    if (!isOpen()) {
        if (error) *error = QStringLiteral("no device open");
        return false;
    }

    emit stageChanged(Stage::Activating, QStringLiteral("enroll"));

    FpPrint *tmpl = fp_print_new(d->dev);
    fp_print_set_finger(tmpl, static_cast<FpFinger>(finger));
    fp_print_set_username(tmpl, username.toUtf8().constData());

    EnrollCtx ctx{this, fp_device_get_nr_enroll_stages(d->dev)};
    Deadline dl(timeoutSecs);
    GError *err = nullptr;
    FpPrint *result = fp_device_enroll_sync(d->dev, tmpl, dl.c,
                                            enrollProgressCb, &ctx, &err);
    if (!result) {
        if (error) *error = dl.tripped()
            ? QCoreApplication::translate("fpstudio", "Gave up after %1 seconds").arg(timeoutSecs)
            : QString::fromUtf8(err ? err->message : "enroll failed");
        if (err) g_error_free(err);
        emit stageChanged(Stage::Failed, error ? *error : QString());
        return false;
    }

    guchar *blob = nullptr;
    gsize   blobLen = 0;
    bool ok = fp_print_serialize(result, &blob, &blobLen, &err);
    if (ok) {
        const QString path = printPath(username, finger);
        QDir().mkpath(QFileInfo(path).absolutePath());
        QFile f(path);
        if (f.open(QIODevice::WriteOnly)) {
            f.write(reinterpret_cast<const char *>(blob), blobLen);
            f.close();
        } else {
            ok = false;
            if (error) *error = QStringLiteral("cannot write %1").arg(path);
        }
        g_free(blob);
    } else {
        if (error) *error = QString::fromUtf8(err ? err->message : "serialize failed");
        if (err) g_error_free(err);
    }
    g_object_unref(result);

    emit stageChanged(ok ? Stage::Done : Stage::Failed, {});
    return ok;
}

namespace {

void matchCb(FpDevice *, FpPrint *, FpPrint *, gpointer, GError *) { /* scores come via the log */ }

} // namespace

bool Engine::verify(const QString &username, int finger, int timeoutSecs,
                    bool *matched, QString *error)
{
    if (!isOpen()) {
        if (error) *error = QStringLiteral("no device open");
        return false;
    }

    const QString path = printPath(username, finger);
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        if (error) *error = QStringLiteral("no enrolled print at %1").arg(path);
        return false;
    }
    const QByteArray blob = f.readAll();
    f.close();

    GError *err = nullptr;
    FpPrint *enrolled = fp_print_deserialize(
        reinterpret_cast<const guchar *>(blob.constData()), blob.size(), &err);
    if (!enrolled) {
        if (error) *error = QString::fromUtf8(err ? err->message : "deserialize failed");
        if (err) g_error_free(err);
        return false;
    }

    d->scores.clear();
    d->lastRunLog.clear();
    takeLog();                                   // start from a clean slate
    emit stageChanged(Stage::Matching, {});

    Deadline dl(timeoutSecs);
    gboolean match = FALSE;
    FpPrint *scanned = nullptr;
    const bool ok = fp_device_verify_sync(d->dev, enrolled, dl.c,
                                          matchCb, nullptr, &match, &scanned, &err);
    if (!ok) {
        if (error) *error = dl.tripped()
            ? QCoreApplication::translate("fpstudio", "Gave up after %1 seconds").arg(timeoutSecs)
            : QString::fromUtf8(err ? err->message : "verify failed");
        if (err) g_error_free(err);
    }

    // Harvest the scores the driver printed while matching. The lines are
    // kept in lastRunLog because draining the buffer here would otherwise
    // discard exactly the output a caller most wants to see.
    d->lastRunLog = takeLog();
    for (const LogLine &l : d->lastRunLog) {
        auto m = scoreRe().match(l.text);
        if (m.hasMatch())
            d->scores.append({m.captured(1).toInt(), m.captured(2).toInt()});
        emit logged(l);
    }

    if (scanned) g_object_unref(scanned);
    g_object_unref(enrolled);

    if (matched) *matched = match;
    emit stageChanged(ok ? Stage::Done : Stage::Failed, match ? QStringLiteral("match") : QStringLiteral("no-match"));
    return ok;
}

bool Engine::listPrints(const QString &username, QStringList *out, QString *error)
{
    QDir dir(QFileInfo(printPath(username, 0)).absolutePath());
    if (!dir.exists()) {
        if (out) out->clear();
        return true;
    }
    QStringList names;
    for (const QString &n : dir.entryList({username + QStringLiteral("-*.fp")}, QDir::Files))
        names << n;
    if (out) *out = names;
    Q_UNUSED(error);
    return true;
}

bool Engine::deletePrints(const QString &username, QString *error)
{
    QDir dir(QFileInfo(printPath(username, 0)).absolutePath());
    if (!dir.exists())
        return true;
    bool ok = true;
    for (const QString &n : dir.entryList({username + QStringLiteral("-*.fp")}, QDir::Files))
        ok = dir.remove(n) && ok;
    if (!ok && error) *error = QStringLiteral("could not remove every print");
    return ok;
}

} // namespace fpstudio
