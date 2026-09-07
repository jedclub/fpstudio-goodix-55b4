#include "headless.h"

#include "setupcheck.h"
#include "engine.h"
#include "beacon.h"

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QBuffer>
#include <QTextStream>
#include <QImage>
#include <QHash>
#include <QProcess>
#include <QDir>
#include <QDateTime>
#include <QSaveFile>
#include <QLockFile>
#include <QStandardPaths>

#include <iostream>

namespace fpstudio {
namespace {

void emitJson(const QJsonObject &o)
{
    std::cout << QJsonDocument(o).toJson(QJsonDocument::Compact).toStdString()
              << std::endl;
}

QJsonObject fail(const QString &msg)
{
    return QJsonObject{{QStringLiteral("ok"), false},
                       {QStringLiteral("error"), msg}};
}

QJsonArray scoresToJson(const QList<MatchScore> &scores)
{
    QJsonArray a;
    for (const MatchScore &s : scores)
        a.append(QJsonObject{{QStringLiteral("score"), s.score},
                             {QStringLiteral("total"), s.total}});
    return a;
}

// The driver's log is the interesting half of any result. The GUI runs these
// commands through pkexec and has no other way to see what the privileged
// process observed, so every command carries its log lines back.
void attachLog(QJsonObject *o, Engine &e)
{
    QJsonArray a;
    QList<LogLine> lines = e.lastRunLog();      // what verify() already consumed
    lines += e.takeLog();                       // plus anything still buffered
    for (const LogLine &l : lines)
        a.append(QJsonObject{{QStringLiteral("t"), l.monotonicMs},
                             {QStringLiteral("domain"), l.domain},
                             {QStringLiteral("text"), l.text}});
    if (!a.isEmpty())
        o->insert(QStringLiteral("log"), a);
}

// Images go out as a base64 PNG so a caller that cannot read the filesystem -
// an agent on the other end of a pipe - still gets to see the frame.
QString imageToBase64Png(const QImage &img)
{
    QByteArray bytes;
    QBuffer buf(&bytes);
    buf.open(QIODevice::WriteOnly);
    img.save(&buf, "PNG");
    return QString::fromLatin1(bytes.toBase64());
}

// The sentence a person at the machine should act on. Kept next to the
// command dispatch so a new command cannot quietly ship without one.
//
// No stage count here on purpose. This runs before the device is opened, so
// the number would have to be hardcoded, and it was - it still said ten after
// the driver moved to fifteen. The live line under the banner already carries
// the real "등록 N/M", read from the device, and a rejected scan does not
// advance N, so the count on screen stays honest while a fixed one would not.
QString promptFor(const QString &cmd)
{
    if (cmd == QLatin1String("capture"))
        return QCoreApplication::translate("fpstudio", "Put your finger on the sensor and hold it there");
    if (cmd == QLatin1String("enroll"))
        return QCoreApplication::translate("fpstudio", "Enrolling — press and lift your finger repeatedly");
    if (cmd == QLatin1String("verify"))
        return QCoreApplication::translate("fpstudio", "Put the enrolled finger on the sensor");
    return QString();
}

// Opens the first device unless an index is given. Most callers have exactly
// one sensor and should not have to say so.
bool prepare(Engine &e, int index, QJsonObject *err)
{
    QString msg;
    if (!e.enumerate(&msg)) { *err = fail(msg); return false; }
    if (e.devices().isEmpty()) { *err = fail(QStringLiteral("no fingerprint device found")); return false; }
    if (index < 0) index = 0;
    if (!e.open(index, &msg)) { *err = fail(msg); return false; }
    return true;
}

// The setup report, as JSON. Same checks the wizard runs - the wizard renders
// them, this prints them, and neither owns the judgement.
QJsonObject cmdSetup()
{
    static const QHash<StepState, QString> names{
        {StepState::Unknown, QStringLiteral("unknown")},
        {StepState::Ok,      QStringLiteral("ok")},
        {StepState::Missing, QStringLiteral("missing")},
        {StepState::Manual,  QStringLiteral("manual")},
        {StepState::Failed,  QStringLiteral("failed")},
        {StepState::Skipped, QStringLiteral("skipped")},
    };

    const QVector<StepResult> results = probeAll();

    QJsonArray steps;
    for (const StepResult &r : results) {
        QJsonObject o{
            {QStringLiteral("step"),    stepKey(r.id)},
            {QStringLiteral("title"),   stepTitle(r.id)},
            {QStringLiteral("state"),   names.value(r.state)},
            {QStringLiteral("summary"), r.summary},
        };
        if (!r.detail.isEmpty())   o.insert(QStringLiteral("detail"), r.detail);
        if (!r.action.isEmpty())   o.insert(QStringLiteral("action"), r.action);
        if (r.irreversible)        o.insert(QStringLiteral("irreversible"), true);
        if (r.needsRoot)           o.insert(QStringLiteral("needs_root"), true);
        if (!r.commands.isEmpty()) {
            QJsonArray c;
            for (const QString &line : r.commands) c.append(line);
            o.insert(QStringLiteral("commands"), c);
        }
        steps.append(o);
    }

    return QJsonObject{
        {QStringLiteral("ok"), true},
        {QStringLiteral("ready"), allReady(results)},
        {QStringLiteral("steps"), steps},
    };
}

QJsonObject cmdDevices()
{
    Engine e;
    QString msg;
    if (!e.enumerate(&msg))
        return fail(msg);
    QJsonArray arr;
    for (const DeviceInfo &d : e.devices())
        arr.append(d.toJson());
    return QJsonObject{{QStringLiteral("ok"), true},
                       {QStringLiteral("devices"), arr}};
}

QJsonObject cmdCapture(int index, bool waitForFinger, const QString &savePath, int timeoutSecs)
{
    Engine e;
    QJsonObject err;
    if (!prepare(e, index, &err)) return err;

    QImage img;
    QString msg;
    if (!e.captureImage(waitForFinger, timeoutSecs, &img, &msg)) {
        QJsonObject result = fail(msg);
        attachLog(&result, e);
        return result;
    }

    QJsonObject o{{QStringLiteral("ok"), true},
                  {QStringLiteral("width"), img.width()},
                  {QStringLiteral("height"), img.height()}};
    attachLog(&o, e);
    if (!savePath.isEmpty()) {
        if (!img.save(savePath))
            return fail(QStringLiteral("captured but could not write %1").arg(savePath));
        o.insert(QStringLiteral("saved"), savePath);
    } else {
        o.insert(QStringLiteral("png_base64"), imageToBase64Png(img));
    }
    return o;
}

QJsonObject cmdEnroll(int index, const QString &user, int finger, int timeoutSecs)
{
    Engine e;
    QJsonObject err;
    if (!prepare(e, index, &err)) return err;

    QString msg;
    const bool ok = e.enroll(user, finger, timeoutSecs, &msg);
    QJsonObject o{{QStringLiteral("ok"), ok},
                  {QStringLiteral("username"), user},
                  {QStringLiteral("finger"), finger}};
    if (!ok) o.insert(QStringLiteral("error"), msg);
    attachLog(&o, e);
    return o;
}

QJsonObject cmdVerify(int index, const QString &user, int finger, int timeoutSecs)
{
    Engine e;
    QJsonObject err;
    if (!prepare(e, index, &err)) return err;

    bool matched = false;
    QString msg;
    const bool ok = e.verify(user, finger, timeoutSecs, &matched, &msg);
    const auto scores = e.lastScores();

    int best = 0;
    // Every score line carries the driver's threshold; take it from there
    // rather than hardcoding one, so the number reported is the one actually
    // applied. Zero when nothing scored - the caller can tell that apart.
    int threshold = 0;
    for (const MatchScore &s : scores) {
        best = std::max(best, s.score);
        if (s.total > 0) threshold = s.total;
    }

    QJsonObject o{{QStringLiteral("ok"), ok},
                  {QStringLiteral("matched"), matched},
                  {QStringLiteral("best_score"), best},
                  {QStringLiteral("threshold"), threshold},
                  {QStringLiteral("scores"), scoresToJson(scores)}};
    if (!ok) o.insert(QStringLiteral("error"), msg);
    attachLog(&o, e);
    return o;
}

QJsonObject cmdList(const QString &user)
{
    Engine e;
    QStringList names;
    QString msg;
    if (!e.listPrints(user, &names, &msg))
        return fail(msg);
    QJsonArray a;
    for (const QString &n : names) a.append(n);
    return QJsonObject{{QStringLiteral("ok"), true}, {QStringLiteral("prints"), a}};
}

QJsonObject cmdDelete(const QString &user)
{
    Engine e;
    QString msg;
    if (!e.deletePrints(user, &msg))
        return fail(msg);
    return QJsonObject{{QStringLiteral("ok"), true}};
}

int optInt(const QStringList &a, const QString &key, int def)
{
    const int i = a.indexOf(key);
    return (i >= 0 && i + 1 < a.size()) ? a[i + 1].toInt() : def;
}

QString optStr(const QStringList &a, const QString &key, const QString &def)
{
    const int i = a.indexOf(key);
    return (i >= 0 && i + 1 < a.size()) ? a[i + 1] : def;
}

} // namespace

int runCli(const QStringList &args)
{
    if (args.isEmpty()) {
        emitJson(fail(QStringLiteral(
            "usage: fpstudio --cli <devices|capture|enroll|verify|list|delete> [opts]")));
        return 2;
    }

    const QString cmd = args.first();
    const int     dev = optInt(args, QStringLiteral("--device"), -1);
    const QString user = optStr(args, QStringLiteral("--user"),
                                qEnvironmentVariable("USER", QStringLiteral("user")));
    const int  finger = optInt(args, QStringLiteral("--finger"), 7);   // right index
    const QString out = optStr(args, QStringLiteral("--out"), {});
    const bool   wait = !args.contains(QStringLiteral("--no-wait"));
    // Bounded by default. A capture is 40ms of work wrapped in an unbounded
    // wait for a person; enrolment asks for ten of those, so it gets longer.
    const int  toCap = optInt(args, QStringLiteral("--timeout"), 45);
    const int  toEnr = optInt(args, QStringLiteral("--timeout"), 180);

    const QString prompt = promptFor(cmd);
    const QString source = qEnvironmentVariable("FPSTUDIO_SOURCE",
                                                QStringLiteral("cli"));
    if (!prompt.isEmpty()) {
        Beacon::begin(source, cmd, prompt);
        // Keep the screen current while the blocking call runs. Without this
        // the banner says "put your finger down" and then never changes, so a
        // working capture and a hung one look the same.
        Engine::setProgressHook([source](const QString &stage, const QString &human) {
            Beacon::progress(source, stage, human);
        });
        Engine::setQualityHook([source](int cov, int sharp, bool ok) {
            Beacon::quality(source, cov, sharp, ok);
        });
    }

    QJsonObject result;
    if      (cmd == QLatin1String("setup"))   result = cmdSetup();
    else if (cmd == QLatin1String("devices")) result = cmdDevices();
    else if (cmd == QLatin1String("capture")) result = cmdCapture(dev, wait, out, toCap);
    else if (cmd == QLatin1String("enroll"))  result = cmdEnroll(dev, user, finger, toEnr);
    else if (cmd == QLatin1String("verify"))  result = cmdVerify(dev, user, finger, toCap);
    else if (cmd == QLatin1String("list"))    result = cmdList(user);
    else if (cmd == QLatin1String("delete"))  result = cmdDelete(user);
    else result = fail(QStringLiteral("unknown command: %1").arg(cmd));

    if (!prompt.isEmpty()) {
        Engine::setProgressHook(nullptr);
        Engine::setQualityHook(nullptr);
        Beacon::clear(source);
    }

    emitJson(result);
    return result.value(QStringLiteral("ok")).toBool() ? 0 : 1;
}

// ---------------------------------------------------------------- MCP

namespace {

QJsonObject toolDef(const QString &name, const QString &desc, const QJsonObject &props,
                    const QJsonArray &required = {})
{
    return QJsonObject{
        {QStringLiteral("name"), name},
        {QStringLiteral("description"), desc},
        {QStringLiteral("inputSchema"), QJsonObject{
            {QStringLiteral("type"), QStringLiteral("object")},
            {QStringLiteral("properties"), props},
            {QStringLiteral("required"), required}}}};
}

QJsonArray mcpTools()
{
    const QJsonObject userProp{
        {QStringLiteral("username"), QJsonObject{{QStringLiteral("type"), QStringLiteral("string")}}},
        {QStringLiteral("finger"), QJsonObject{
            {QStringLiteral("type"), QStringLiteral("integer")},
            {QStringLiteral("description"), QStringLiteral("FpFinger value; 7 = right index")}}}};

    QJsonArray tools;
    tools.append(toolDef(QStringLiteral("start_collection"),
                         QStringLiteral("Open and automatically run the 14-sample research workflow, including finger-change countdowns. The only user button is Stop."), {}));
    tools.append(toolDef(QStringLiteral("collection_status"),
                         QStringLiteral("Read the research workflow state, saved sample paths and quality measurements."), {}));
    tools.append(toolDef(QStringLiteral("stop_collection"),
                         QStringLiteral("Stop the active research workflow, preserving captured samples."), {}));
    tools.append(toolDef(QStringLiteral("resume_collection"),
                         QStringLiteral("Resume a workflow paused after repeated capture errors, after diagnosing the cause."), {}));
    tools.append(toolDef(QStringLiteral("show_guidance"),
                         QStringLiteral("Show an instruction or result on the diagnostics banner until the next operation."),
                         QJsonObject{{QStringLiteral("message"), QJsonObject{{QStringLiteral("type"), QStringLiteral("string")}}},
                                     {QStringLiteral("image_path"), QJsonObject{{QStringLiteral("type"), QStringLiteral("string")}}}},
                         QJsonArray{QStringLiteral("message")}));
    tools.append(toolDef(QStringLiteral("list_devices"),
                         QStringLiteral("Enumerate fingerprint devices and their capabilities."),
                         {}));
    tools.append(toolDef(QStringLiteral("capture_image"),
                         QStringLiteral("Capture one raw frame. Returns width, height and a base64 PNG, "
                                        "or writes a file if save_path is given. This is the call that "
                                        "shows what the sensor actually sees."),
                         QJsonObject{
                             {QStringLiteral("instruction"), QJsonObject{{QStringLiteral("type"), QStringLiteral("string")}}},
                             {QStringLiteral("timeout_secs"), QJsonObject{{QStringLiteral("type"), QStringLiteral("integer")}}},
                             {QStringLiteral("wait_for_finger"), QJsonObject{{QStringLiteral("type"), QStringLiteral("boolean")}}},
                             {QStringLiteral("save_path"), QJsonObject{{QStringLiteral("type"), QStringLiteral("string")}}}}));
    tools.append(toolDef(QStringLiteral("enroll"),
                         QStringLiteral("Run a full enrolment and store the print for later verify calls."),
                         userProp));
    tools.append(toolDef(QStringLiteral("verify"),
                         QStringLiteral("Verify against a stored print. Returns matched plus every SIGFM "
                                        "score observed, so a near miss can be told from no correlation."),
                         userProp));
    tools.append(toolDef(QStringLiteral("list_prints"),
                         QStringLiteral("List stored prints for a user."), userProp));
    tools.append(toolDef(QStringLiteral("delete_prints"),
                         QStringLiteral("Delete stored prints for a user."), userProp));
    return tools;
}

QJsonObject mcpCall(const QString &name, const QJsonObject &a)
{
    static QString collectionDir;
    if (name == QLatin1String("start_collection")) {
        QLockFile gui(QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation) + "/fpstudio-gui.lock");
        if (!gui.tryLock()) return fail(QStringLiteral("A fpstudio GUI is already running. Use that window or close it before starting research."));
        gui.unlock();
        collectionDir = QDir::cleanPath(QStringLiteral(FPSTUDIO_SOURCE_DIR "/../../local-private/recognition/session-")
                                        + QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss-zzz"));
        if (!QDir().mkpath(collectionDir)) return fail(QStringLiteral("Cannot create private session directory"));
        QFile::setPermissions(collectionDir, QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner);
        qint64 pid = 0;
        if (!QProcess::startDetached(QCoreApplication::applicationFilePath(),
                                     {"--research", "--lang", "ko", "--session-dir", collectionDir}, QString(), &pid))
            return fail(QStringLiteral("Could not open research window"));
        return QJsonObject{{"ok", true}, {"pid", pid}, {"session_dir", collectionDir}, {"state", "launching"}};
    }
    if (name == QLatin1String("collection_status") || name == QLatin1String("stop_collection") || name == QLatin1String("resume_collection")) {
        if (collectionDir.isEmpty()) return fail(QStringLiteral("No collection was started by this MCP server"));
        if (name == QLatin1String("stop_collection") || name == QLatin1String("resume_collection")) {
            QSaveFile stop(collectionDir + (name == QLatin1String("stop_collection") ? "/stop" : "/resume"));
            if (!stop.open(QIODevice::WriteOnly) || !stop.commit()) return fail(QStringLiteral("Cannot request stop"));
            return QJsonObject{{"ok", true}};
        }
        QFile file(collectionDir + "/session.json");
        if (!file.open(QIODevice::ReadOnly)) return QJsonObject{{"ok", true}, {"state", "launching"}, {"session_dir", collectionDir}};
        QJsonObject status = QJsonDocument::fromJson(file.readAll()).object();
        status.insert("ok", true); status.insert("session_dir", collectionDir);
        return status;
    }
    if (name == QLatin1String("capture_image") || name == QLatin1String("enroll") || name == QLatin1String("verify")) {
        QLockFile research(QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation) + "/fpstudio-research.lock");
        if (!research.tryLock()) return fail(QStringLiteral("Research workflow owns capture timing. Use collection_status or stop_collection."));
    }
    if (name == QLatin1String("show_guidance")) {
        Beacon::begin(QStringLiteral("mcp"), QStringLiteral("guidance"),
                      a.value(QStringLiteral("message")).toString());
        const QString path = a.value(QStringLiteral("image_path")).toString();
        if (!path.isEmpty()) Beacon::preview(QStringLiteral("mcp"), path);
        return QJsonObject{{QStringLiteral("ok"), true}};
    }
    // Everything an agent triggers is announced on screen, so the person at
    // the machine can follow the guide instead of the transcript.
    static const QHash<QString, QString> prompts{
        {QStringLiteral("capture_image"), QCoreApplication::translate("fpstudio", "Put your finger on the sensor and hold it there")},
        {QStringLiteral("enroll"),        QCoreApplication::translate("fpstudio", "Enrolling — press and lift your finger repeatedly")},
        {QStringLiteral("verify"),        QCoreApplication::translate("fpstudio", "Put the enrolled finger on the sensor")},
    };
    const QString prompt = a.value(QStringLiteral("instruction")).toString(prompts.value(name));
    if (!prompt.isEmpty()) {
        Beacon::begin(QStringLiteral("mcp"), name, prompt);
        Engine::setQualityHook([](int cov, int sharp, bool ok) {
            Beacon::quality(QStringLiteral("mcp"), cov, sharp, ok);
        });
        Engine::setProgressHook([](const QString &stage, const QString &human) {
            Beacon::progress(QStringLiteral("mcp"), stage, human);
        });
    }
    struct Clear {
        QString p;
        bool keepResult;
        ~Clear() {
            if (p.isEmpty()) return;
            Engine::setProgressHook(nullptr);
            Engine::setQualityHook(nullptr);
            if (!keepResult) Beacon::clear(QStringLiteral("mcp"));
        }
    } clearer{prompt, name == QLatin1String("capture_image")};

    const QString user = a.value(QStringLiteral("username")).toString(
        qEnvironmentVariable("USER", QStringLiteral("user")));
    const int finger = a.value(QStringLiteral("finger")).toInt(7);

    if (name == QLatin1String("list_devices"))  return cmdDevices();
    if (name == QLatin1String("capture_image")) {
        const QJsonObject result = cmdCapture(-1, a.value(QStringLiteral("wait_for_finger")).toBool(true),
                          a.value(QStringLiteral("save_path")).toString(),
                          a.value(QStringLiteral("timeout_secs")).toInt(45));
        if (result.value(QStringLiteral("ok")).toBool() &&
            !a.value(QStringLiteral("save_path")).toString().isEmpty())
            Beacon::preview(QStringLiteral("mcp"), a.value(QStringLiteral("save_path")).toString());
        Beacon::progress(QStringLiteral("mcp"),
                         result.value(QStringLiteral("ok")).toBool() ? QStringLiteral("done") : QStringLiteral("failed"),
                         result.value(QStringLiteral("ok")).toBool()
                             ? QCoreApplication::translate("fpstudio", "Captured — now lift your finger")
                             : result.value(QStringLiteral("error")).toString());
        return result;
    }
    if (name == QLatin1String("enroll"))
        return cmdEnroll(-1, user, finger, a.value(QStringLiteral("timeout_secs")).toInt(180));
    if (name == QLatin1String("verify"))
        return cmdVerify(-1, user, finger, a.value(QStringLiteral("timeout_secs")).toInt(45));
    if (name == QLatin1String("list_prints"))   return cmdList(user);
    if (name == QLatin1String("delete_prints")) return cmdDelete(user);
    return fail(QStringLiteral("unknown tool: %1").arg(name));
}

void reply(const QJsonValue &id, const QJsonObject &result)
{
    emitJson(QJsonObject{{QStringLiteral("jsonrpc"), QStringLiteral("2.0")},
                         {QStringLiteral("id"), id},
                         {QStringLiteral("result"), result}});
}

} // namespace

int runMcp()
{
    QTextStream in(stdin);
    QString line;
    while (!(line = in.readLine()).isNull()) {
        if (line.trimmed().isEmpty())
            continue;

        QJsonParseError perr{};
        const QJsonObject req = QJsonDocument::fromJson(line.toUtf8(), &perr).object();
        if (perr.error != QJsonParseError::NoError)
            continue;                       // notifications and junk are ignored

        const QString method = req.value(QStringLiteral("method")).toString();
        const QJsonValue id  = req.value(QStringLiteral("id"));
        if (!req.contains(QStringLiteral("id"))) continue; // JSON-RPC notifications have no response

        if (method == QLatin1String("initialize")) {
            reply(id, QJsonObject{
                {QStringLiteral("protocolVersion"), QStringLiteral("2024-11-05")},
                {QStringLiteral("capabilities"), QJsonObject{
                    {QStringLiteral("tools"), QJsonObject{}}}},
                {QStringLiteral("serverInfo"), QJsonObject{
                    {QStringLiteral("name"), QStringLiteral("fpstudio")},
                    {QStringLiteral("version"), QStringLiteral("0.2.2")}}}});
        } else if (method == QLatin1String("tools/list")) {
            reply(id, QJsonObject{{QStringLiteral("tools"), mcpTools()}});
        } else if (method == QLatin1String("tools/call")) {
            const QJsonObject p = req.value(QStringLiteral("params")).toObject();
            const QJsonObject r = mcpCall(p.value(QStringLiteral("name")).toString(),
                                          p.value(QStringLiteral("arguments")).toObject());
            reply(id, QJsonObject{
                {QStringLiteral("content"), QJsonArray{QJsonObject{
                    {QStringLiteral("type"), QStringLiteral("text")},
                    {QStringLiteral("text"), QString::fromUtf8(
                        QJsonDocument(r).toJson(QJsonDocument::Compact))}}}},
                {QStringLiteral("isError"), !r.value(QStringLiteral("ok")).toBool()}});
        } else if (!id.isNull()) {
            emitJson(QJsonObject{
                {QStringLiteral("jsonrpc"), QStringLiteral("2.0")},
                {QStringLiteral("id"), id},
                {QStringLiteral("error"), QJsonObject{
                    {QStringLiteral("code"), -32601},
                    {QStringLiteral("message"), QStringLiteral("method not found: %1").arg(method)}}}});
        }
    }
    return 0;
}

} // namespace fpstudio
