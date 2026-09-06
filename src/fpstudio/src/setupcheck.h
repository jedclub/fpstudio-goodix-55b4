// What the machine currently looks like, and what to do about it.
//
// This is the part an agent used to supply. Getting the sensor working takes
// eight steps that each depend on the one before, and the failure modes are
// not self-announcing: a missing udev rule looks like a permissions error, a
// missing PSK looks like a TLS error, the wrong driver looks like no device at
// all. Someone who has not read the docs cannot tell those apart, and someone
// who has still has to run the checks by hand.
//
// So the checks live here, one per step, each answering three things: is it
// done, can this program do it, and what does the person need to know before
// it does. The wizard renders that; the CLI prints it. Neither decides
// anything - the decisions are here, in one place, testable without a screen.
//
// Nothing in this header acts. Probing is separate from changing so that a
// report can be produced on a machine nobody intends to modify.

#pragma once

#include <QString>
#include <QStringList>
#include <QVector>

namespace fpstudio {

enum class StepId {
    Device,        // is a supported sensor plugged in
    Driver,        // is the patched libfprint installed
    UdevRule,      // can this user open the device node
    TlsSession,    // does the handshake complete
    Psk,           // does the sensor need the all-zero key written
    Capture,       // does a capture produce a usable image
    Enrolment,     // is a finger enrolled with fprintd
    PamPolkit,     // does polkit accept a fingerprint
    PamSudo,       // does terminal sudo accept a fingerprint (opt-in)
};

enum class StepState {
    Unknown,   // not probed yet
    Ok,        // done, nothing to do
    Missing,   // not done, and this program can do it
    Manual,    // not done, and the person has to do it themselves
    Failed,    // probed and something is wrong beyond a missing step
    Skipped,   // deliberately not done - optional, or declined
};

struct StepResult {
    StepId    id;
    StepState state = StepState::Unknown;

    // One line for the list, and the detail for the panel underneath. Both
    // translated: this is the surface the person reads.
    QString   summary;
    QString   detail;

    // What running the fix would do, in plain words, shown before it runs.
    // Empty when there is nothing to run.
    QString   action;

    // True when the fix cannot be undone. Exactly one step is - writing the
    // PSK - and it deserves more than a colour: the sensor's existing key
    // cannot be read back, so there is no backup to restore, and Windows
    // fingerprint login stops working for good. The wizard makes the person
    // type their agreement rather than click past it.
    bool      irreversible = false;

    // True when the fix needs root. The wizard warns that a password prompt
    // is coming, which matters because on a half-configured machine that
    // prompt is the very thing being removed.
    bool      needsRoot = false;

    // Command lines the fix would run, verbatim. Shown on request and copied
    // into the report, so someone who would rather run them by hand can, and
    // someone reporting a bug can say exactly what was attempted.
    QStringList commands;
};

// Probes every step in order, stopping at nothing - a later step's result is
// still worth knowing even when an earlier one failed, because it tells the
// person how much of the path is already in place.
QVector<StepResult> probeAll();

// One step, for re-checking after a fix without redoing the rest.
StepResult probe(StepId id);

// Whether unlocking works end to end - so gated on the sensor, the driver,
// the secure channel, the sensor's key, enrolment and PAM, but not on the
// udev rule (a convenience: skipping it only costs more password prompts) or
// image quality (a reading, not something to configure). One definition of
// "done" shared by the wizard's completion banner and `--cli setup`'s "ready"
// field, so the two can never disagree about it.
bool allReady(const QVector<StepResult> &steps);

// Runs a real, longer capture attempt and reports what it measured. Used by
// the wizard's "test now" action - the two-second window probeAll() uses is
// enough to tell whether the handshake still works, but not long enough for a
// person to read an instruction and then act on it.
StepResult testCapture(int timeoutSecs = 8);

// Turns one capture attempt's stdout into a result, without running anything.
// Shared by the passive probe above and by the wizard, which runs its own
// asynchronous process for the "test now" button rather than blocking the UI
// thread the way testCapture() does.
StepResult parseCaptureOutput(const QString &output);

// Stable names for the CLI and for JSON. Not translated: these are keys.
QString stepKey(StepId id);

// The title shown in the wizard. Translated.
QString stepTitle(StepId id);

} // namespace fpstudio
