#pragma once

// Colours for the setup dialog, chosen against the desktop's own palette
// rather than written into each stylesheet.
//
// The dialog used to name seventeen literal hex colours, and the bug that
// causes is already recorded in setupwizard.cpp: a box given a light green
// background and no text colour of its own inherits the application palette,
// which on a dark desktop is light text - so the box shipped unreadable. The
// fix applied at the time was to pin both colours on that one widget. That
// works, and it does not scale: every new panel has to remember to do it, and
// a palette pinned for a dark desktop is then wrong on a light one.
//
// So the colours live here, once, and each is a pair - a surface and something
// legible on top of it - because a background without a matching foreground is
// exactly the shape of the original bug. The dark variants are not the light
// ones dimmed: a saturated fill that reads as emphasis on white reads as a
// glare on a dark background, so those are darkened and the text lightened,
// keeping contrast on the text rather than on the fill.
//
// Status colours (the checklist ticks) stay recognisable across themes - green
// is progress, amber is attention, red is a problem - but are lightened for
// dark backgrounds, where mid-tone text on near-black falls below a readable
// contrast ratio.

#include <QApplication>
#include <QColor>
#include <QPalette>
#include <QString>
#include <QGuiApplication>
#include <QScreen>
#include <QStyleHints>
#include <QWidget>

namespace fpstudio {
namespace theme {

// True when the desktop is running a dark colour scheme.
//
// Qt 6.5 added styleHints()->colorScheme(), which reports what the desktop
// asked for. It is not always answered - a plain X11 session, or a style that
// sets a palette without publishing a scheme, both report Unknown - so the
// window's own palette decides in that case. Reading the actual window colour
// is the more reliable of the two signals; the hint is preferred only because
// it matches what the user chose rather than what a style happened to apply.
inline bool isDark(const QWidget *widget = nullptr)
{
    if (const QStyleHints *hints = QGuiApplication::styleHints()) {
        const Qt::ColorScheme scheme = hints->colorScheme();
        if (scheme == Qt::ColorScheme::Dark)
            return true;
        if (scheme == Qt::ColorScheme::Light)
            return false;
    }
    const QPalette palette = widget ? widget->palette() : QApplication::palette();
    const QColor window = palette.color(QPalette::Window);
    // Rec. 601 luma. The exact coefficients matter less than the threshold
    // being taken on perceived brightness rather than on any single channel:
    // a saturated blue window is dark, and its blue channel alone says light.
    return (window.red() * 299 + window.green() * 587 + window.blue() * 114) / 1000 < 128;
}

struct Pair {
    QString background;
    QString foreground;
};

// The blue instruction banner that carries the current verdict.
inline Pair banner(bool dark)
{
    return dark ? Pair{QStringLiteral("#15314d"), QStringLiteral("#dce9f7")}
                : Pair{QStringLiteral("#123e68"), QStringLiteral("#ffffff")};
}

// The primary action button.
inline Pair accent(bool dark)
{
    return dark ? Pair{QStringLiteral("#2b6fb8"), QStringLiteral("#ffffff")}
                : Pair{QStringLiteral("#1769c2"), QStringLiteral("#ffffff")};
}

// The same button with nothing to do.
inline Pair accentDisabled(bool dark)
{
    return dark ? Pair{QStringLiteral("#3a4149"), QStringLiteral("#8b9299")}
                : Pair{QStringLiteral("#a0a8b0"), QStringLiteral("#e8e8e8")};
}

// The "everything is set up" box.
inline Pair success(bool dark)
{
    return dark ? Pair{QStringLiteral("#1e5122"), QStringLiteral("#e6f4e7")}
                : Pair{QStringLiteral("#2e7d32"), QStringLiteral("#ffffff")};
}

inline Pair successButton(bool dark)
{
    return dark ? Pair{QStringLiteral("#143a17"), QStringLiteral("#e6f4e7")}
                : Pair{QStringLiteral("#1b5e20"), QStringLiteral("#ffffff")};
}

// The literal commands a step will run, shown rather than hidden.
inline Pair code(bool dark)
{
    return dark ? Pair{QStringLiteral("#22262b"), QStringLiteral("#c4ccd4")}
                : Pair{QStringLiteral("#f4f4f4"), QStringLiteral("#555555")};
}

inline QString codeBorder(bool dark)
{
    return dark ? QStringLiteral("#3a4149") : QStringLiteral("#dddddd");
}

// A warning that a step cannot be undone.
inline QString danger(bool dark)
{
    return dark ? QStringLiteral("#f08a8a") : QStringLiteral("#b03030");
}

// Checklist status marks.
inline QString statusOk(bool dark)      { return dark ? QStringLiteral("#63c163") : QStringLiteral("#3a8f3a"); }
inline QString statusMissing(bool dark) { return dark ? QStringLiteral("#e0bc4a") : QStringLiteral("#c9a227"); }
inline QString statusManual(bool dark)  { return dark ? QStringLiteral("#c0a04a") : QStringLiteral("#8a6d1f"); }
inline QString statusFailed(bool dark)  { return dark ? QStringLiteral("#f08a8a") : QStringLiteral("#b03030"); }
inline QString statusMuted(bool dark)   { return dark ? QStringLiteral("#9aa0a6") : QStringLiteral("#888888"); }

// Fit a window to the screen it is opening on.
//
// The dialog asked for 1050x740 and the live view for 1050x920, both written
// for a display that has room for them. On a smaller panel, or at a desktop
// scale factor above 1, the request survives but the window does not fit, and
// what falls off the bottom is whatever the layout put last - here, the row of
// buttons, which is the part the person needs to carry on. Qt will not shrink
// a window below its layout's minimum, so the clamp has to happen before the
// window is shown, against the space actually available.
//
// The margin covers the title bar and any panel the desktop reserves; the
// available geometry already excludes reserved struts on most setups, and
// taking a little more costs nothing and covers the ones where it does not.
inline QSize fitToScreen(const QWidget *widget, int preferredWidth, int preferredHeight)
{
    const QScreen *screen = widget && widget->screen() ? widget->screen()
                                                       : QGuiApplication::primaryScreen();
    if (!screen)
        return QSize(preferredWidth, preferredHeight);
    const QRect available = screen->availableGeometry();
    return QSize(qMin(preferredWidth, available.width() - 40),
                 qMin(preferredHeight, available.height() - 80));
}

} // namespace theme
} // namespace fpstudio
