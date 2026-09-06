// Runs fpstudio's own --cli mode under pkexec and parses the JSON back.
//
// Why not just do the work in-process: libusb needs write access to
// /dev/bus/usb/..., which is root-only here. uaccess udev tagging is the
// tidy answer but did not take on this system, and running a Qt/Wayland GUI
// as root to work around that is worse than the problem - it drags the whole
// toolkit, the compositor connection and every plugin across the privilege
// boundary.
//
// So the window stays unprivileged and shells out for the few operations that
// actually touch the device. The privileged half is the same binary in --cli
// mode, which already returns structured results, and now carries the driver
// log back so the GUI can still show what happened inside.
//
// One prompt per call would be tedious; polkit's auth_admin_keep retention
// (see the project docs) collapses a working session into a single prompt.

#pragma once

#include <QObject>
#include <QJsonObject>
#include <QStringList>
#include <QProcess>

namespace fpstudio {

class PrivRunner : public QObject {
    Q_OBJECT
public:
    explicit PrivRunner(QObject *parent = nullptr);

    // args are the --cli sub-command and its options, e.g. {"capture","--out",…}.
    // Set direct when the caller already has device access and pkexec would
    // only add a pointless prompt (device enumeration needs no write access).
    void run(const QStringList &args, bool direct = false);

    // Whether the sensor node is already writable by this user, in which case
    // the pkexec hop is skipped. See the udev rule beside this source.
    static bool deviceWritable();
    bool busy() const;
    void cancel();

signals:
    void finished(const QJsonObject &result);
    void failed(const QString &error);

private:
    QProcess m_proc;
    QString  m_selfPath;
};

} // namespace fpstudio
