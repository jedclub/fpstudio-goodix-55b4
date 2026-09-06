// Non-GUI front ends: a JSON-emitting CLI and an MCP stdio server.
//
// Both exist for the same reason. Driving a fingerprint sensor from a chat
// transcript meant reading journalctl and guessing; these give an agent (or a
// script) the same facts the GUI shows, as data rather than prose.

#pragma once

#include <QStringList>

namespace fpstudio {

// Sub-commands: devices, capture, enroll, verify, list, delete.
// Every invocation prints exactly one JSON object on stdout.
int runCli(const QStringList &args);

// JSON-RPC 2.0 over stdin/stdout, newline-delimited. Implements initialize,
// tools/list and tools/call.
int runMcp();

} // namespace fpstudio
