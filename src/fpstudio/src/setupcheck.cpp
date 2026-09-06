#include "setupcheck.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QRegularExpression>
#include <QStandardPaths>

namespace fpstudio {
namespace {

// Runs a command and returns stdout, or an empty string if it could not run.
// Used only for probing, never for changing anything - every call here is a
// question, so a failure means "unknown", not "broken".
QString ask(const QString &program, const QStringList &args, int timeoutMs = 4000)
{
    QProcess p;
    p.setProcessChannelMode(QProcess::MergedChannels);
    p.start(program, args);
    if (!p.waitForFinished(timeoutMs))
        p.kill();
    return QString::fromUtf8(p.readAll());
}

QString selfPath()
{
    return QFileInfo(QCoreApplication::applicationFilePath()).canonicalFilePath();
}

// The repo's own files, needed by the fixes. Found relative to the binary so a
// build tree and an install both work; empty when neither has them, which is
// what turns a fixable step into a manual one.
QString repoFile(const QString &relative)
{
    const QString bin = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath();
    const QStringList roots = {
        bin + QStringLiteral("/.."),                       // build tree
        bin + QStringLiteral("/../share/fpstudio"),        // installed
        QStringLiteral(FPSTUDIO_SOURCE_DIR),               // configured at build
    };
    for (const QString &root : roots) {
        const QString path = QDir(root).absoluteFilePath(relative);
        if (QFileInfo::exists(path))
            return QFileInfo(path).canonicalFilePath();
    }
    return QString();
}

StepResult device()
{
    StepResult r{StepId::Device};
    const QString out = ask(QStringLiteral("lsusb"), {QStringLiteral("-d"), QStringLiteral("27c6:")});

    if (out.contains(QStringLiteral("27c6:55b4"))) {
        r.state = StepState::Ok;
        r.summary = QCoreApplication::translate("fpstudio", "Goodix 27c6:55b4 found");
        return r;
    }
    if (out.contains(QStringLiteral("27c6:"))) {
        // Another Goodix part. Saying which one matters: the person can then
        // look for a project that covers it instead of fighting this one.
        static const QRegularExpression re(QStringLiteral("27c6:([0-9a-f]{4})"));
        const auto m = re.match(out);
        r.state = StepState::Failed;
        r.summary = QCoreApplication::translate("fpstudio", "A different Goodix sensor is present");
        r.detail = QCoreApplication::translate("fpstudio", "This build only covers 27c6:55b4. Found 27c6:%1.")
                       .arg(m.hasMatch() ? m.captured(1) : QStringLiteral("????"));
        return r;
    }

    r.state = StepState::Failed;
    r.summary = QCoreApplication::translate("fpstudio", "No Goodix fingerprint sensor found");
    r.detail = QCoreApplication::translate("fpstudio", "Nothing on the USB bus answers to vendor 27c6. If the "
                   "machine has a reader, it may be disabled in firmware "
                   "settings, or it may be a different make.");
    return r;
}

StepResult driver()
{
    StepResult r{StepId::Driver};
    r.needsRoot = true;

    // Ask libfprint itself rather than the package manager: what matters is
    // which driver is loaded, not which package claims to have installed it.
    // A source build, a different distribution, or a stale package all give
    // the same honest answer this way.
    const QString out = ask(selfPath(), {QStringLiteral("--cli"), QStringLiteral("devices")}, 15000);

    if (out.contains(QStringLiteral("goodixtls55x4"))) {
        r.state = StepState::Ok;
        r.summary = QCoreApplication::translate("fpstudio", "Patched driver is active");
        return r;
    }

    const QString pkgbuild = repoFile(QStringLiteral("../driver/PKGBUILD"));
    if (pkgbuild.isEmpty()) {
        r.state = StepState::Manual;
        r.summary = QCoreApplication::translate("fpstudio", "Patched driver is not installed");
        r.detail = QCoreApplication::translate("fpstudio", "libfprint does not recognise this sensor. The patched "
                       "fork has to be built and installed; this copy of the "
                       "program does not ship the sources to do it.");
        return r;
    }

    r.state = StepState::Missing;
    r.summary = QCoreApplication::translate("fpstudio", "Patched driver is not installed");
    r.detail = QCoreApplication::translate("fpstudio", "libfprint does not recognise this sensor. The patched fork "
                   "adds it. Building takes a few minutes and replaces the "
                   "system libfprint, which is why it needs a password.\n\n"
                   "Install fprintd afterwards, not before: installing it "
                   "first pulls in the stock libfprint and undoes this.");
    r.action = QCoreApplication::translate("fpstudio", "Build and install the patched libfprint");
    r.commands = {QStringLiteral("makepkg -f -D %1").arg(QFileInfo(pkgbuild).absolutePath()),
                  QStringLiteral("pacman -U <built package>")};
    return r;
}

StepResult udevRule()
{
    StepResult r{StepId::UdevRule};
    r.needsRoot = true;

    const QString installed = QStringLiteral("/etc/udev/rules.d/99-fpstudio-goodix.rules");
    const bool present = QFileInfo::exists(installed);

    // The rule existing is not the same as it working - it applies on the next
    // add event, so a freshly written rule does nothing until a replug or a
    // trigger. Check the node itself, which is the thing that matters.
    bool writable = false;
    QDir busRoot(QStringLiteral("/dev/bus/usb"));
    for (const QString &bus : busRoot.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir d(busRoot.filePath(bus));
        for (const QString &dev : d.entryList(QDir::Files | QDir::System)) {
            QFile f(d.filePath(dev));
            if (f.open(QIODevice::ReadWrite)) {
                f.close();
                if (QFileInfo(d.filePath(dev)).group() == QLatin1String("wheel"))
                    writable = true;
            }
        }
    }

    if (writable) {
        r.state = StepState::Ok;
        r.summary = QCoreApplication::translate("fpstudio", "Sensor is reachable without a password");
        return r;
    }

    const QString rule = repoFile(QStringLiteral("99-fpstudio-goodix.rules"));
    r.state = rule.isEmpty() ? StepState::Manual : StepState::Missing;
    r.summary = present ? QCoreApplication::translate("fpstudio", "udev rule is installed but has not taken effect")
                        : QCoreApplication::translate("fpstudio", "Every sensor operation will ask for a password");
    r.detail = present
        ? QCoreApplication::translate("fpstudio", "The rule file is in place but the device node still belongs to "
              "root. It applies on the next add event; replugging the reader "
              "or reloading udev will do it.")
        : QCoreApplication::translate("fpstudio", "Without a rule, libusb cannot open the sensor except as root, "
              "so every capture goes through a password prompt.\n\n"
              "This grants the wheel group read and write on the sensor. On a "
              "single-user laptop that is the same person who could already do "
              "it through pkexec; on a shared machine it is not. Skipping this "
              "is safe - it only means more prompts.");
    // Only when there is a script to run - a Manual result with a populated
    // action would show a button (or a CLI "action" field) that promises
    // something this copy of the program cannot actually do.
    if (r.state == StepState::Missing) {
        r.action = QCoreApplication::translate("fpstudio", "Install the udev rule and reload");
        r.commands = {QStringLiteral("install -Dm644 %1 %2").arg(rule, installed),
                      QStringLiteral("udevadm control --reload"),
                      QStringLiteral("udevadm trigger --attr-match=idVendor=27c6")};
    }
    return r;
}

// Both TLS and PSK are decided by one capture attempt, because that is the
// only thing that actually exercises the handshake. Run once, read twice.
QString handshakeProbe()
{
    return ask(selfPath(),
               {QStringLiteral("--cli"), QStringLiteral("capture"),
                QStringLiteral("--timeout"), QStringLiteral("2")},
               30000);
}

StepResult tlsSession(const QString &probe)
{
    StepResult r{StepId::TlsSession};

    // Not "HANDSHAKE DONE": that line is a debug log, and the CLI drops its
    // log array on the timeout path this probe deliberately takes. What does
    // reach stdout are the driver's own g_print markers, and the scan states
    // only run after the handshake has completed - so reaching one of them is
    // the same proof, from output that survives.
    if (probe.contains(QStringLiteral("SWITCH TO FDT MODE")) ||
        probe.contains(QStringLiteral("HANDSHAKE DONE"))) {
        r.state = StepState::Ok;
        r.summary = QCoreApplication::translate("fpstudio", "Secure channel to the sensor established");
        return r;
    }
    // Activation got as far as reading the sensor's key-derived value and
    // then stopped, which is what a key mismatch looks like from here.
    if (probe.contains(QStringLiteral("Checking PSK")) ||
        probe.contains(QStringLiteral("Device PSK"))) {
        r.state = StepState::Failed;
        r.summary = QCoreApplication::translate("fpstudio", "The sensor answered, but the handshake did not complete");
        r.detail = QCoreApplication::translate("fpstudio", "The sensor holds a key this driver does not have. "
                       "Writing the all-zero key is the next step, and it "
                       "cannot be undone.");
        return r;
    }
    r.state = StepState::Failed;
    r.summary = QCoreApplication::translate("fpstudio", "Could not talk to the sensor");
    r.detail = QCoreApplication::translate("fpstudio", "The handshake did not get far enough to say why. Check the "
                   "driver and permission steps above first.");
    return r;
}

StepResult psk(const StepResult &tls)
{
    StepResult r{StepId::Psk};
    r.needsRoot = true;
    r.irreversible = true;

    if (tls.state == StepState::Ok) {
        r.state = StepState::Ok;
        r.summary = QCoreApplication::translate("fpstudio", "The sensor already holds a usable key");
        return r;
    }

    const QString script = repoFile(QStringLiteral("../firmware/probes/write_psk_only.py"));
    r.state = script.isEmpty() ? StepState::Manual : StepState::Missing;
    r.summary = QCoreApplication::translate("fpstudio", "The sensor needs the all-zero key written");
    r.detail = QCoreApplication::translate("fpstudio", "This cannot be undone.\n\n"
                   "The key the sensor holds now cannot be read back - what "
                   "the protocol returns is a value derived from it, not the "
                   "key - so there is no backup to restore afterwards.\n\n"
                   "Windows fingerprint sign-in will stop working on this "
                   "machine, permanently. If you dual-boot and use it there, "
                   "stop here.");
    if (r.state == StepState::Missing) {
        r.action = QCoreApplication::translate("fpstudio", "Write the all-zero PSK to the sensor");
        r.commands = {QStringLiteral("python %1").arg(script)};
    }
    return r;
}

StepResult capture(const QString &probe)
{
    StepResult r{StepId::Capture};

    static const QRegularExpression re(
        QStringLiteral("coverage (\\d+)%, sharpness (\\d+)"));
    const auto m = re.globalMatch(probe);
    int coverage = -1, sharpness = -1;
    QRegularExpressionMatch last;
    auto it = m;
    while (it.hasNext())
        last = it.next();
    if (last.hasMatch()) {
        coverage = last.captured(1).toInt();
        sharpness = last.captured(2).toInt();
    }

    if (coverage < 0) {
        // No finger was presented during this probe. During probeAll() that is
        // the normal case - the probe uses a two-second timeout precisely so
        // it does not demand one - but either way the person looking at this
        // step needs a next action, not a description of why there is no
        // number yet. testCapture() below is that action.
        r.state = StepState::Unknown;
        r.summary = QCoreApplication::translate("fpstudio", "Image quality has not been measured yet");
        r.detail = QCoreApplication::translate("fpstudio",
                       "Press the button below, then put your finger on the "
                       "sensor and hold it there for a couple of seconds.");
        r.action = QCoreApplication::translate("fpstudio", "Test a capture now");
        return r;
    }

    r.summary = QCoreApplication::translate("fpstudio", "Coverage %1%  ·  sharpness %2").arg(coverage).arg(sharpness);
    if (sharpness >= 24) {
        r.state = StepState::Ok;
        return r;
    }
    // 24 is not the driver's own floor (16) but where matching starts to
    // succeed: measured, a sharpness of 21 scored 23 against a threshold of
    // 72, while 28 and 30 scored 1349 and 1686.
    r.state = StepState::Failed;
    r.detail = QCoreApplication::translate("fpstudio", "Below about 24, matching tends to fail even though the "
                   "driver accepts the frame. Dry fingertips and a sensor that "
                   "has been busy both do this; a few minutes' rest and a "
                   "little moisture usually fix it.");
    r.action = QCoreApplication::translate("fpstudio", "Try again");
    return r;
}

StepResult enrolment()
{
    StepResult r{StepId::Enrolment};

    const QString out = ask(QStringLiteral("fprintd-list"),
                            {qEnvironmentVariable("USER")}, 15000);

    if (out.contains(QStringLiteral("-finger"))) {
        r.state = StepState::Ok;
        r.summary = QCoreApplication::translate("fpstudio", "A finger is enrolled with fprintd");
        return r;
    }
    if (out.contains(QStringLiteral("no fingers enrolled")) ||
        out.contains(QStringLiteral("found 1 device"))) {
        r.state = StepState::Missing;
        r.summary = QCoreApplication::translate("fpstudio", "No finger is enrolled with fprintd");
        r.detail = QCoreApplication::translate("fpstudio", "This program keeps its own enrolments separately, and "
                       "system authentication does not see those. Enrolling "
                       "again through fprintd is what makes the fingerprint "
                       "usable for unlocking.");
        r.action = QCoreApplication::translate("fpstudio", "Enrol a finger with fprintd");
        return r;
    }

    r.state = StepState::Failed;
    r.summary = QCoreApplication::translate("fpstudio", "fprintd is not answering");
    r.detail = QCoreApplication::translate("fpstudio", "The daemon could not be reached. It is usually started on "
                   "demand; installing it may be all that is missing.");
    return r;
}

StepResult pamPolkit()
{
    StepResult r{StepId::PamPolkit};
    r.needsRoot = true;

    const QString installed = QStringLiteral("/etc/pam.d/polkit-1");
    QFile f(installed);
    if (f.open(QIODevice::ReadOnly)) {
        const QString text = QString::fromUtf8(f.readAll());
        if (text.contains(QStringLiteral("pam_fprintd"))) {
            r.state = StepState::Ok;
            r.summary = QCoreApplication::translate("fpstudio", "polkit accepts a fingerprint");
            return r;
        }
    }

    if (!QFileInfo::exists(QStringLiteral("/usr/lib/security/pam_fprintd.so")) &&
        !QFileInfo::exists(QStringLiteral("/lib/x86_64-linux-gnu/security/pam_fprintd.so"))) {
        r.state = StepState::Manual;
        r.summary = QCoreApplication::translate("fpstudio", "pam_fprintd is not installed");
        r.detail = QCoreApplication::translate("fpstudio", "The PAM module that lets authentication use a "
                       "fingerprint is missing. It usually ships with fprintd.");
        return r;
    }

    const QString stack = repoFile(QStringLiteral("pam/polkit-1"));
    r.state = stack.isEmpty() ? StepState::Manual : StepState::Missing;
    r.summary = QCoreApplication::translate("fpstudio", "polkit still asks for a password");
    r.detail = QCoreApplication::translate("fpstudio", "This adds one line to the stack polkit uses, so pkexec and "
                   "the desktop's authentication dialog try the fingerprint "
                   "first.\n\n"
                   "Login and sudo are deliberately left alone, so a sensor "
                   "that stops working can never lock you out of the machine. "
                   "The line is 'sufficient': if the fingerprint fails for any "
                   "reason, you are asked for the password exactly as before.");
    if (r.state == StepState::Missing) {
        r.action = QCoreApplication::translate("fpstudio", "Let polkit accept a fingerprint");
        r.commands = {QStringLiteral("install -Dm644 %1 %2").arg(stack, installed)};
    }
    return r;
}

StepResult pamSudo()
{
    StepResult r{StepId::PamSudo};
    r.needsRoot = true;

    QFile f(QStringLiteral("/etc/pam.d/sudo"));
    if (f.open(QIODevice::ReadOnly)) {
        const QString text = QString::fromUtf8(f.readAll());
        if (text.contains(QStringLiteral("pam_fprintd"))) {
            r.state = StepState::Ok;
            r.summary = QCoreApplication::translate("fpstudio", "sudo accepts a fingerprint");
            return r;
        }
    }

    if (!QFileInfo::exists(QStringLiteral("/usr/lib/security/pam_fprintd.so")) &&
        !QFileInfo::exists(QStringLiteral("/lib/x86_64-linux-gnu/security/pam_fprintd.so"))) {
        r.state = StepState::Manual;
        r.summary = QCoreApplication::translate("fpstudio", "pam_fprintd is not installed");
        r.detail = QCoreApplication::translate("fpstudio", "The PAM module that lets authentication use a "
                       "fingerprint is missing. It usually ships with fprintd.");
        return r;
    }

    // Opt-in and deliberately excluded from allReady() - sudo is usually the
    // way back in when something else on the machine breaks, so this asks
    // rather than assumes. See the detail text below for why extending
    // fingerprint auth to it is still safe.
    const QString stack = repoFile(QStringLiteral("pam/sudo"));
    r.state = stack.isEmpty() ? StepState::Manual : StepState::Missing;
    r.summary = QCoreApplication::translate("fpstudio", "Terminal sudo still asks for a password only");
    r.detail = QCoreApplication::translate("fpstudio", "Optional, and a step further than the polkit rule "
                   "above: sudo is usually the way back in when something "
                   "else on the machine is broken, so extending fingerprint "
                   "auth to it is worth doing deliberately rather than by "
                   "default.\n\n"
                   "The safety net is the same either way. The line added is "
                   "'sufficient', so a failing fingerprint falls back to the "
                   "password exactly as before. sudo itself never stops "
                   "working - only the fingerprint shortcut can.");
    if (r.state == StepState::Missing) {
        r.action = QCoreApplication::translate("fpstudio", "Let sudo accept a fingerprint");
        r.commands = {QStringLiteral("install -Dm644 %1 /etc/pam.d/sudo").arg(stack)};
    }
    return r;
}

} // namespace

QString stepKey(StepId id)
{
    switch (id) {
    case StepId::Device:     return QStringLiteral("device");
    case StepId::Driver:     return QStringLiteral("driver");
    case StepId::UdevRule:   return QStringLiteral("udev");
    case StepId::TlsSession: return QStringLiteral("tls");
    case StepId::Psk:        return QStringLiteral("psk");
    case StepId::Capture:    return QStringLiteral("capture");
    case StepId::Enrolment:  return QStringLiteral("enrolment");
    case StepId::PamPolkit:  return QStringLiteral("pam");
    case StepId::PamSudo:    return QStringLiteral("pam_sudo");
    }
    return QString();
}

QString stepTitle(StepId id)
{
    switch (id) {
    case StepId::Device:     return QCoreApplication::translate("fpstudio", "Sensor");
    case StepId::Driver:     return QCoreApplication::translate("fpstudio", "Driver");
    case StepId::UdevRule:   return QCoreApplication::translate("fpstudio", "Permissions");
    case StepId::TlsSession: return QCoreApplication::translate("fpstudio", "Secure channel");
    case StepId::Psk:        return QCoreApplication::translate("fpstudio", "Sensor key");
    case StepId::Capture:    return QCoreApplication::translate("fpstudio", "Image quality");
    case StepId::Enrolment:  return QCoreApplication::translate("fpstudio", "Enrolment");
    case StepId::PamPolkit:  return QCoreApplication::translate("fpstudio", "Unlocking");
    case StepId::PamSudo:    return QCoreApplication::translate("fpstudio", "Terminal sudo (optional)");
    }
    return QString();
}

QVector<StepResult> probeAll()
{
    QVector<StepResult> out;
    const StepResult dev = device();
    out << dev;

    // A missing sensor makes every later probe meaningless, and running them
    // anyway would produce a page of red that says nothing useful.
    if (dev.state != StepState::Ok) {
        for (StepId id : {StepId::Driver, StepId::UdevRule, StepId::TlsSession,
                          StepId::Psk, StepId::Capture, StepId::Enrolment,
                          StepId::PamPolkit, StepId::PamSudo}) {
            StepResult r{id};
            r.state = StepState::Unknown;
            r.summary = QCoreApplication::translate("fpstudio", "Not checked - no sensor");
            out << r;
        }
        return out;
    }

    out << driver();
    out << udevRule();

    // One capture attempt answers three questions, so it runs once here rather
    // than once per step. It is also the slowest probe by far.
    const QString probe = handshakeProbe();
    const StepResult tls = tlsSession(probe);
    out << tls;
    out << psk(tls);
    out << capture(probe);
    out << enrolment();
    out << pamPolkit();
    out << pamSudo();
    return out;
}

// Turns one capture attempt's stdout into a result. Shared by the passive,
// short probe inside probeAll() and by the wizard's "test now" button, which
// runs a longer attempt a person can actually act on - so a pass means the
// same thing either way.
StepResult parseCaptureOutput(const QString &output)
{
    return capture(output);
}

bool allReady(const QVector<StepResult> &steps)
{
    for (const StepResult &r : steps) {
        if (r.id == StepId::UdevRule || r.id == StepId::Capture || r.id == StepId::PamSudo)
            continue;
        if (r.state != StepState::Ok && r.state != StepState::Skipped)
            return false;
    }
    return true;
}

StepResult testCapture(int timeoutSecs)
{
    const QString out = ask(selfPath(),
                            {QStringLiteral("--cli"), QStringLiteral("capture"),
                             QStringLiteral("--timeout"), QString::number(timeoutSecs)},
                            (timeoutSecs + 5) * 1000);
    return capture(out);
}

StepResult probe(StepId id)
{
    switch (id) {
    case StepId::Device:   return device();
    case StepId::Driver:   return driver();
    case StepId::UdevRule: return udevRule();
    case StepId::Enrolment: return enrolment();
    case StepId::PamPolkit: return pamPolkit();
    case StepId::PamSudo: return pamSudo();
    case StepId::TlsSession: return tlsSession(handshakeProbe());
    case StepId::Psk: {
        const QString p = handshakeProbe();
        return psk(tlsSession(p));
    }
    case StepId::Capture: return capture(handshakeProbe());
    }
    return StepResult{};
}

} // namespace fpstudio
