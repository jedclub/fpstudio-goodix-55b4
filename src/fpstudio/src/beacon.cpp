#include "beacon.h"

#include <QDateTime>
#include <QFile>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QJsonObject>
#include <QSaveFile>

#include <sys/stat.h>

namespace fpstudio {

QString Beacon::path()
{
    // Fixed path so a GUI started before or after the worker finds the same
    // file. /tmp rather than /run because the worker may run before any
    // /run/fpstudio has been created, and this is not state worth persisting.
    return QStringLiteral("/tmp/fpstudio-status.json");
}

namespace {
// The standing instruction for the operation in flight. progress() re-publishes
// it so a status update never erases what the user was asked to do.
QString g_prompt;
qint64  g_startedAt = 0;   // begin() 시각. 화면에 경과를 띄우기 위한 것
}

static int g_coverage = -1;
static int g_sharpness = -1;
static bool g_accepted = true;

static void write(const QString &source, const QString &stage,
                  const QString &prompt, const QString &status)
{
    const QJsonObject o{
        {QStringLiteral("source"), source},
        {QStringLiteral("stage"), stage},
        {QStringLiteral("prompt"), prompt},
        {QStringLiteral("status"), status},
        {QStringLiteral("started_at"), g_startedAt},
        {QStringLiteral("ts"), QDateTime::currentMSecsSinceEpoch()},
        // Who to ask whether this is still live. The timestamp alone cannot
        // answer that: the sensor logs nothing at all while it waits for a
        // finger, so a perfectly healthy enrolment goes minutes without
        // touching this file - exactly the stretch during which the user most
        // needs the instruction on screen. A reader that treats silence as
        // death hides the prompt precisely then. The pid does not go stale.
        {QStringLiteral("pid"), static_cast<qint64>(QCoreApplication::applicationPid())},
        {QStringLiteral("coverage"), g_coverage},
        {QStringLiteral("sharpness"), g_sharpness},
        {QStringLiteral("quality_ok"), g_accepted},
    };

    // QSaveFile renames into place, so a reader never sees a half-written file.
    QSaveFile f(Beacon::path());
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    f.write(QJsonDocument(o).toJson(QJsonDocument::Compact));
    f.write("\n");
    if (!f.commit())
        return;

    // Written by root, read by the seat user: make that possible. The content
    // is stage names and prompts, nothing worth protecting.
    ::chmod(Beacon::path().toLocal8Bit().constData(), 0644);
}

void Beacon::begin(const QString &source, const QString &stage,
                   const QString &prompt)
{
    g_prompt = prompt;
    g_startedAt = QDateTime::currentMSecsSinceEpoch();
    // A reading from the previous run says nothing about this one, and left
    // on screen it would look like a measurement of a finger that is not
    // there yet.
    g_coverage = -1;
    g_sharpness = -1;
    g_accepted = true;
    write(source, stage, prompt, QString());
}

void Beacon::progress(const QString &source, const QString &stage,
                      const QString &status)
{
    write(source, stage, g_prompt, status);
}

void Beacon::quality(const QString &source, int coverage, int sharpness,
                     bool accepted)
{
    g_coverage = coverage;
    g_sharpness = sharpness;
    g_accepted = accepted;
    // Same stage and status as whatever is currently showing: this only adds
    // numbers to the picture, it does not change what the person is being
    // asked to do.
    write(source, QStringLiteral("quality"), g_prompt, QString());
}

void Beacon::clear(const QString &source)
{
    g_prompt.clear();
    g_startedAt = 0;
    // The quality reading deliberately survives. A prompt goes stale the
    // moment the run ends - nobody should still be told to press a finger -
    // but the numbers describe the scan that just happened, and that is
    // exactly when someone wants to look at them. begin() clears them for the
    // next run.
    write(source, QStringLiteral("idle"), QString(), QString());
}

} // namespace fpstudio
