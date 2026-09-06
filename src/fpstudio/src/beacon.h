// A one-file status channel between the privileged worker and the window.
//
// The point is the workflow the tool is built for: an agent calls the MCP
// server, the sensor needs a finger, and the person sitting at the machine
// should learn that from the screen rather than from the chat transcript they
// may not be looking at. The MCP and CLI processes run as root and often have
// no terminal at all, so they cannot ask; the GUI can, but it is not the
// process talking to the device.
//
// So the worker publishes what it is doing and what it needs, and any running
// window picks it up. A file rather than D-Bus because the two sides are
// different users (root worker, seat user GUI) and would otherwise be on
// different buses; a single small JSON write is atomic enough via rename.
//
// Nothing depends on a reader existing. If no GUI is running the file is just
// written and ignored.

#pragma once

#include <QString>

namespace fpstudio {

class Beacon {
public:
    // Where both sides meet. World-readable, written by root, read by the
    // seat user - so it carries no secrets, only stage names and prompts.
    static QString path();

    // Two different things, deliberately kept apart:
    //
    //   prompt  what the user should be doing, for the whole operation
    //   status  what the driver is doing right now
    //
    // Collapsing them loses the instruction the moment anything happens - the
    // banner flips from "put your finger down" to "sensor ready" and the user
    // no longer knows what is being asked of them. So begin() sets the standing
    // instruction and progress() only replaces the live line under it.
    //
    // source says who asked - "mcp", "cli" or "gui" - so the window can make
    // clear that a request came from an agent rather than a button.
    static void begin(const QString &source,
                      const QString &stage,
                      const QString &prompt);

    // Updates the live line, keeping whatever prompt begin() set.
    static void progress(const QString &source,
                         const QString &stage,
                         const QString &status);

    // The driver's own measurement of the frame it just captured, carried
    // alongside the prompt so the window can show it while the finger is
    // still on the sensor - which is the only moment the person can act on
    // it. accepted is false when the gate handed the frame back, and the
    // coverage/sharpness pair is then meaningless, so pass -1 for both.
    static void quality(const QString &source, int coverage, int sharpness,
                        bool accepted);

    // Called when a command finishes so a stale "press your finger" does not
    // linger on screen.
    static void clear(const QString &source);
};

} // namespace fpstudio
