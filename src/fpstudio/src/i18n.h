// Language selection, shared by the window, the CLI and the MCP server.
//
// The three front ends are one binary, and the person reading the banner is
// often not the one who started the operation: an agent calls over MCP, the
// worker runs under pkexec, and the instruction has to appear on the screen of
// whoever is sitting at the sensor. So the language cannot be a property of
// the GUI - every entry point has to resolve it the same way, before it says
// anything.
//
// Resolution order, most specific first:
//
//   1. --lang on the command line, or the GUI's selector
//   2. the value remembered from the GUI's last run
//   3. the system locale, if it is one of the languages shipped here
//   4. English
//
// English is both the source language and the last fallback, so a missing or
// partial translation degrades to readable English rather than to blanks.

#pragma once

#include <QString>
#include <QStringList>

namespace fpstudio {
namespace i18n {

// Language tags as they appear on the command line and in the .ts filenames.
// Kept in one place because three different things index by them: the loader,
// the GUI's menu, and the CLI's --lang validation.
QStringList available();

// Human-readable name for a tag, in that language - "Deutsch" rather than
// "German". Someone looking for their own language recognises it written the
// way they write it, and does not necessarily read the current one.
QString displayName(const QString &tag);

// Installs the translator for `tag`, or resolves one per the order above when
// tag is empty. Returns the tag actually applied, which is what the GUI shows
// as selected - asking for a language that is not shipped gets English, and
// the caller should see that rather than assume it got what it asked for.
//
// Safe to call more than once; a previously installed translator is removed
// first, so switching languages at runtime does not stack them.
QString install(const QString &tag = QString());

// The tag currently installed.
QString current();

// Remembers the choice for the next run. Only the GUI calls this: a --lang on
// one CLI invocation should not silently change what the window does later.
void remember(const QString &tag);

} // namespace i18n
} // namespace fpstudio
