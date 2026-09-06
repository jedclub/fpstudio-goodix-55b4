#include "i18n.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QLibraryInfo>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

namespace fpstudio {
namespace i18n {
namespace {

// One translator, replaced rather than added to. Qt searches installed
// translators newest-first, so stacking them would leave an old language
// answering for any string the new one happens not to cover - a half-switched
// UI, which is worse than either language on its own.
QTranslator *g_translator = nullptr;
QString      g_current = QStringLiteral("en");

// Where the .qm files are. Installed builds put them beside the binary under
// share/; a build tree has them in the build directory. Checking both means
// running from either works without an install step, which is what anyone
// developing a translation will do.
QStringList searchPaths()
{
    const QString bin = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath();
    return {
        bin + QStringLiteral("/translations"),
        bin + QStringLiteral("/../share/fpstudio/translations"),
        QStringLiteral(FPSTUDIO_TRANSLATIONS_DIR),
    };
}

QSettings settings()
{
    return QSettings(QStringLiteral("fpstudio"), QStringLiteral("fpstudio"));
}

} // namespace

QStringList available()
{
    // en has no .qm of its own - it is the source language, so an empty
    // translator is exactly right for it.
    return {
        QStringLiteral("en"),    QStringLiteral("ko"),    QStringLiteral("ja"),
        QStringLiteral("zh_CN"), QStringLiteral("zh_TW"), QStringLiteral("es"),
        QStringLiteral("de"),    QStringLiteral("fr"),    QStringLiteral("ru"),
        QStringLiteral("it"),    QStringLiteral("pt"),
    };
}

QString displayName(const QString &tag)
{
    static const QHash<QString, QString> names{
        {QStringLiteral("en"),    QStringLiteral("English")},
        {QStringLiteral("ko"),    QStringLiteral("한국어")},
        {QStringLiteral("ja"),    QStringLiteral("日本語")},
        {QStringLiteral("zh_CN"), QStringLiteral("简体中文")},
        {QStringLiteral("zh_TW"), QStringLiteral("繁體中文")},
        {QStringLiteral("es"),    QStringLiteral("Español")},
        {QStringLiteral("de"),    QStringLiteral("Deutsch")},
        {QStringLiteral("fr"),    QStringLiteral("Français")},
        {QStringLiteral("ru"),    QStringLiteral("Русский")},
        {QStringLiteral("it"),    QStringLiteral("Italiano")},
        {QStringLiteral("pt"),    QStringLiteral("Português")},
    };
    return names.value(tag, tag);
}

QString install(const QString &tag)
{
    QString want = tag;

    if (want.isEmpty())
        want = settings().value(QStringLiteral("language")).toString();

    if (want.isEmpty()) {
        // QLocale::name() gives "ko_KR"; the shipped tags are either a bare
        // language ("ko") or language_REGION where the region matters
        // ("zh_CN" vs "zh_TW"). Try the full name first so Chinese lands on
        // the right script, then fall back to the language alone.
        const QString full = QLocale::system().name();
        const QString lang = full.section(QLatin1Char('_'), 0, 0);
        if (available().contains(full))
            want = full;
        else if (available().contains(lang))
            want = lang;
    }

    if (!available().contains(want))
        want = QStringLiteral("en");

    if (g_translator) {
        QCoreApplication::removeTranslator(g_translator);
        delete g_translator;
        g_translator = nullptr;
    }

    // English needs no catalogue - the sources are English.
    if (want != QStringLiteral("en")) {
        auto *t = new QTranslator;
        bool loaded = false;
        for (const QString &dir : searchPaths()) {
            if (t->load(QStringLiteral("fpstudio_") + want, dir)) {
                loaded = true;
                break;
            }
        }
        if (loaded) {
            QCoreApplication::installTranslator(t);
            g_translator = t;
        } else {
            // The catalogue is missing - a build that did not run lrelease, or
            // an install that dropped share/. Report English rather than
            // claiming a language whose strings will not appear.
            delete t;
            want = QStringLiteral("en");
        }
    }

    g_current = want;
    return want;
}

QString current()
{
    return g_current;
}

void remember(const QString &tag)
{
    settings().setValue(QStringLiteral("language"), tag);
}

} // namespace i18n
} // namespace fpstudio
