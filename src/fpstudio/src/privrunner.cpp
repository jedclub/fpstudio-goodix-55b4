#include "privrunner.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFileInfo>
#include <QDir>
#include <QFile>

namespace fpstudio {

PrivRunner::PrivRunner(QObject *parent) : QObject(parent)
{
    // pkexec refuses a relative path and will not follow a symlink, so resolve
    // the running binary once.
    m_selfPath = QFileInfo(QCoreApplication::applicationFilePath()).canonicalFilePath();

    m_proc.setProcessChannelMode(QProcess::SeparateChannels);

    connect(&m_proc, &QProcess::errorOccurred, this, [this](QProcess::ProcessError e) {
        if (e == QProcess::FailedToStart)
            emit failed(tr("Could not run pkexec"));
    });

    connect(&m_proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int code, QProcess::ExitStatus status) {
        const QByteArray out = m_proc.readAllStandardOutput();
        const QByteArray err = m_proc.readAllStandardError();

        if (status != QProcess::NormalExit) {
            emit failed(tr("The operation terminated abnormally"));
            return;
        }
        // pkexec returns 126 when the user dismisses the prompt and 127 when
        // authorisation is refused. Both mean "no result", not "device error",
        // and deserve a message that says so.
        if (out.isEmpty() && (code == 126 || code == 127)) {
            emit failed(tr("Authentication was cancelled or refused"));
            return;
        }

        // The CLI prints exactly one JSON object, but libusb and the driver
        // write to stderr and can interleave; take the last complete line that
        // parses.
        QJsonObject result;
        bool got = false;
        const QList<QByteArray> lines = out.split('\n');
        for (const QByteArray &line : lines) {
            if (line.trimmed().isEmpty())
                continue;
            QJsonParseError pe{};
            const QJsonDocument doc = QJsonDocument::fromJson(line, &pe);
            if (pe.error == QJsonParseError::NoError && doc.isObject()) {
                result = doc.object();
                got = true;
            }
        }

        if (!got) {
            QString detail = QString::fromUtf8(err).trimmed();
            if (detail.size() > 400) detail = detail.left(400) + QStringLiteral("…");
            emit failed(detail.isEmpty()
                            ? tr("Could not parse the result (exit code %1)").arg(code)
                            : detail);
            return;
        }
        emit finished(result);
    });
}

// True when this user can already open a Goodix node for writing.
bool PrivRunner::deviceWritable()
{
    QDir busRoot(QStringLiteral("/dev/bus/usb"));
    const auto buses = busRoot.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &bus : buses) {
        QDir d(busRoot.filePath(bus));
        for (const QString &dev : d.entryList(QDir::Files | QDir::System)) {
            const QString path = d.filePath(dev);
            QFile f(path);
            if (f.open(QIODevice::ReadWrite)) {
                f.close();
                // Writable, but is it ours? Cheap check: only the sensor's
                // node should have been opened up by the rule, and a false
                // positive only costs one skipped prompt that then fails.
                if (QFileInfo(path).group() == QLatin1String("wheel"))
                    return true;
            }
        }
    }
    return false;
}

bool PrivRunner::busy() const
{
    return m_proc.state() != QProcess::NotRunning;
}

void PrivRunner::cancel()
{
    if (busy())
        m_proc.kill();
}

void PrivRunner::run(const QStringList &args, bool direct)
{
    if (busy()) {
        emit failed(tr("An operation is already running"));
        return;
    }
    if (m_selfPath.isEmpty()) {
        emit failed(tr("Could not resolve this program's own path"));
        return;
    }

    QStringList full;
    full << QStringLiteral("--cli") << args;

    // Skip pkexec when the device node is already writable - the udev rule in
    // this directory grants the group, and prompting for a password we do not
    // need is the single most tiresome part of using the tool. Falls back
    // automatically when the rule is not installed.
    if (direct || deviceWritable())
        m_proc.start(m_selfPath, full);
    else
        m_proc.start(QStringLiteral("pkexec"), QStringList{m_selfPath} << full);
}

} // namespace fpstudio
