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
#include "researchwindow.h"
#include "livewindow.h"
#include <QDir>
#include <QDateTime>
#include <QLockFile>
#include <QStandardPaths>
#include <QMessageBox>
#include <QTimer>
#include <QListWidget>
#include <QSaveFile>
#include <QFileInfo>

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
        "  fpstudio --research            guided research capture session\n"
        "  fpstudio --live                continuous diagnostic preview (local preview driver required)\n"
        "    --match-reference-dir DIR    compare stable frames against prior reference images using Vulkan\n"
        "    --different-finger-test      explicitly guide a different-finger research trial\n"
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
        QLockFile serverLock(QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation)
                             + QStringLiteral("/fpstudio-mcp.lock"));
        if (!serverLock.tryLock()) { std::fputs("fpstudio MCP is already running\n", stderr); return 3; }
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
    QLockFile guiLock(QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation)
                      + QStringLiteral("/fpstudio-gui.lock"));
    if (!guiLock.tryLock()) {
        std::fputs("fpstudio GUI is already running\n", stderr);
        return 3;
    }
    if (args.contains(QStringLiteral("--live"))) {
        const QString directory = QDir::cleanPath(QStringLiteral(FPSTUDIO_SOURCE_DIR "/../../local-private/recognition/live-")
                             + QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss-zzz"));
        const int refAt=args.indexOf(QStringLiteral("--match-reference-dir"));
        const QString references=refAt>=0&&refAt+1<args.size()?args.at(refAt+1):QString();
        const int fastAt=args.indexOf(QStringLiteral("--match-fast-bank"));
        const QString fastBank=fastAt>=0&&fastAt+1<args.size()?args.at(fastAt+1):QString();
        const bool different=args.contains(QStringLiteral("--different-finger-test"));
        const int targetAt=args.indexOf(QStringLiteral("--target-contacts"));
        bool valid=true;
        const int target=targetAt>=0?args.value(targetAt+1).toInt(&valid):(references.isEmpty()?0:different?10:20);
        if(!valid||target<0||target>128) {std::fputs("--target-contacts must be 0..128\n",stderr);return 2;}
        fpstudio::LiveWindow window(directory,nullptr,{},5000,references,different,{},fastBank,target);
        window.show();
        return app.exec();
    }
    if (args.contains(QStringLiteral("--research"))) {
        const int at = args.indexOf(QStringLiteral("--session-dir"));
        const QString directory = at >= 0 && at + 1 < args.size() ? args.at(at + 1)
            : QDir::cleanPath(QStringLiteral(FPSTUDIO_SOURCE_DIR "/../../local-private/recognition/session-")
                             + QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss-zzz"));
        fpstudio::ResearchWindow window(directory, nullptr, {}, 5000, 4000,
                                         !args.contains(QStringLiteral("--preview")));
        window.show();
        return app.exec();
    }

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
        if(args.contains(QStringLiteral("--system-verify"))) {
            auto *start=new QTimer(wiz);
            QObject::connect(start,&QTimer::timeout,wiz,[wiz,start]{
                auto *list=wiz->findChild<QListWidget *>();
                if(!list||list->count()==0||!list->isEnabled())return;
                start->stop();QMetaObject::invokeMethod(wiz,"runSystemVerify",Qt::QueuedConnection);
            });
            start->start(100);
        }
        // Read-only UI regression capture; never invokes a setup action.
        const int snapshotAt=args.indexOf(QStringLiteral("--wizard-snapshot"));
        if(snapshotAt>=0) {
            const QString path=args.value(snapshotAt+1);
            if(path.isEmpty()||QFileInfo::exists(path))return 2;
            auto *timer=new QTimer(wiz);
            QObject::connect(timer,&QTimer::timeout,wiz,[wiz,path,&app]{
                auto *list=wiz->findChild<QListWidget *>();
                if(!list||list->count()==0||!list->isEnabled())return;
                QSaveFile file(path);
                if(!file.open(QIODevice::WriteOnly)){app.exit(2);return;}
                file.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner);
                const bool ok=wiz->grab().save(&file,"PNG")&&file.commit();app.exit(ok?0:2);
            });
            timer->start(100);QTimer::singleShot(25000,&app,[&app]{app.exit(2);});
        }
    }
    return app.exec();
}
