// fpstudio — one binary, three front ends over the same libfprint engine.
//
//   fpstudio                 Qt window: live frame, stages, scores, driver log
//   fpstudio --cli <cmd>     one JSON object on stdout, for scripts
//   fpstudio --mcp           JSON-RPC over stdio, for an LLM agent
//
// The GUI needs a display; --cli and --mcp deliberately do not, so they work
// over ssh and inside a tool call.

#include <QApplication>
#include <QCoreApplication>
#include <QStringList>

#include <cstdio>

#include "engine.h"
#include "i18n.h"
#include "headless.h"
#include "mainwindow.h"
#include "setupwizard.h"

namespace {

void printUsage()
{
    std::fputs(
        "fpstudio — fingerprint sensor studio\n"
        "\n"
        "  fpstudio                       setup wizard - what most people want\n"
        "  fpstudio --setup               same thing, spelled out\n"
        "  fpstudio --diagnostics         the full tool: live capture, driver "
        "log, MCP status\n"
        "  fpstudio --cli setup           what is configured, what is not\n"
        "  fpstudio --cli devices\n"
        "  fpstudio --cli capture [--out FILE] [--timeout N]\n"
        "  fpstudio --cli enroll  [--user U] [--finger N]\n"
        "  fpstudio --cli verify  [--user U] [--finger N]\n"
        "  fpstudio --cli list|delete [--user U]\n"
        "  fpstudio --mcp                 MCP stdio server\n"
        "\n"
        "  --lang TAG                     en ko ja zh_CN zh_TW es de fr ru it pt\n"
        "                                 default: the system locale, else en\n"
        "  --tr STRING                    translate one string and print it\n"
        "\n"
        "  --finger takes an FpFinger value; 7 is the right index finger.\n"
        "  Raw USB access needs root, so run the capture/enroll/verify paths\n"
        "  under pkexec unless a udev rule grants the seat.\n",
        stderr);
}

} // namespace

int main(int argc, char **argv)
{
    // Must precede any libfprint use: it installs the log writer the whole
    // tool depends on for stage and score reporting.
    fpstudio::Engine::enableVerboseLogging();

    QStringList args;
    for (int i = 1; i < argc; ++i)
        args << QString::fromLocal8Bit(argv[i]);

    // Pull --lang out before anything branches: all three front ends produce
    // text, and the CLI's very first line can be an error message. Taking it
    // here also keeps it out of each front end's own option parsing, where
    // three copies would drift.
    QString lang;
    const int langAt = args.indexOf(QStringLiteral("--lang"));
    if (langAt >= 0) {
        if (langAt + 1 >= args.size()) {
            std::fputs("--lang needs a language tag\n", stderr);
            return 2;
        }
        lang = args.at(langAt + 1);
        args.removeAt(langAt + 1);
        args.removeAt(langAt);
    }

    if (args.contains(QStringLiteral("--help")) || args.contains(QStringLiteral("-h"))) {
        printUsage();
        return 0;
    }

    // The organisation and application names decide where QSettings keeps the
    // remembered language, so they have to be set before i18n::install reads
    // it - in every front end, not just the GUI.
    QCoreApplication::setApplicationName(QStringLiteral("fpstudio"));
    QCoreApplication::setOrganizationName(QStringLiteral("fpstudio"));

    // Lets a shell script reuse this catalogue instead of carrying eleven
    // copies of the same sentences that would then drift from these ones.
    // Prints the key unchanged when there is no translation, which is the same
    // thing tr() does and keeps the caller's output readable either way.
    const int trAt = args.indexOf(QStringLiteral("--tr"));
    if (trAt >= 0) {
        if (trAt + 1 >= args.size()) {
            std::fputs("--tr needs a string\n", stderr);
            return 2;
        }
        QCoreApplication app(argc, argv);
        fpstudio::i18n::install(lang);
        const QByteArray key = args.at(trAt + 1).toUtf8();
        std::fputs(QCoreApplication::translate("fpstudio", key.constData())
                       .toUtf8().constData(), stdout);
        return 0;
    }

    if (args.contains(QStringLiteral("--mcp"))) {
        QCoreApplication app(argc, argv);
        fpstudio::i18n::install(lang);
        return fpstudio::runMcp();
    }

    const int cliAt = args.indexOf(QStringLiteral("--cli"));
    if (cliAt >= 0) {
        QCoreApplication app(argc, argv);
        fpstudio::i18n::install(lang);
        return fpstudio::runCli(args.mid(cliAt + 1));
    }

    QApplication app(argc, argv);
    fpstudio::i18n::install(lang);

    // Two different tools live in this one binary, and showing both windows
    // together is what made them confusing to tell apart - a checklist for
    // getting the sensor working, and a deep diagnostics window for capture
    // images, the driver log, and MCP status. So only one opens by default.
    //
    // --diagnostics is the escape hatch for someone who already knows this
    // tool and wants the window they are used to. Everyone else - including a
    // bare `fpstudio`, which is what a desktop launcher runs - gets the
    // wizard, and it has its own button into diagnostics for when the
    // checklist is not enough.
    if (args.contains(QStringLiteral("--diagnostics"))) {
        fpstudio::MainWindow *w = new fpstudio::MainWindow;
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
        w->maybeOfferSetup();
    } else {
        auto *wiz = new fpstudio::SetupWizard(nullptr);
        wiz->setAttribute(Qt::WA_DeleteOnClose);
        wiz->show();
    }
    return app.exec();
}
