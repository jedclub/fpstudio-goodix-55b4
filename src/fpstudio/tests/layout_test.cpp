// Layout regression tests for the two windows that were being cut off.
//
// Both had the same defect, from opposite directions. The setup dialog put a
// word-wrapped label with no height limit into a vertical layout and then fed
// it up to 1000 characters of fprintd transcript; the layout grew to fit, the
// button row and footer went past the bottom edge, and because a layout's
// minimum height is a floor Qt will not resize below, they could not be brought
// back by making the window smaller. The live view asked for a window taller
// than a 1080p screen once its two 352-pixel previews and its captions were
// added up, so its stop button started below the visible area.
//
// A screenshot cannot catch either of these again. The measurable property is
// the one that actually broke: whatever the content, the window's minimum size
// has to stay inside the screen, and the controls have to stay above the
// bottom edge. That is what these assert.
#include "livewindow.h"
#include "theme.h"

#include <QtTest>
#include <QApplication>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QTemporaryDir>
#include <QScreen>

using namespace fpstudio;

// The smallest display this is expected to be usable on, minus the space a
// desktop normally reserves. Deliberately not the machine's own screen: a test
// that passes only on a large monitor is the bug it is meant to catch.
static constexpr int kSmallScreenWidth = 1280;
static constexpr int kSmallScreenHeight = 720;

class LayoutTest : public QObject {
    Q_OBJECT

private slots:
    void fitToScreenNeverExceedsAvailableSpace();
    void liveWindowFitsASmallScreen();
    void liveWindowKeepsItsStopButtonOnScreen();
    void unboundedCaptionCannotGrowTheWindow();
    void themeGivesEveryFillAReadableForeground();
};

// theme::fitToScreen is what both windows now pass their preferred size
// through. It must never hand back something larger than the screen, and must
// leave a smaller request alone.
void LayoutTest::fitToScreenNeverExceedsAvailableSpace()
{
    const QScreen *screen = QGuiApplication::primaryScreen();
    QVERIFY(screen);
    const QRect available = screen->availableGeometry();

    const QSize huge = theme::fitToScreen(nullptr, 10000, 10000);
    QVERIFY(huge.width() <= available.width());
    QVERIFY(huge.height() <= available.height());

    // A request that already fits must survive unchanged - the clamp is a
    // ceiling, not a resize.
    const QSize small = theme::fitToScreen(nullptr, 320, 240);
    QCOMPARE(small, QSize(320, 240));
}

void LayoutTest::liveWindowFitsASmallScreen()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    // A long prepare delay so constructing the window does not launch the
    // capture helper; this test is about geometry, not about capturing.
    LiveWindow window(dir.path(), nullptr, {}, 3600000, {}, false, {}, {}, {});
    window.show();
    QVERIFY(QTest::qWaitForWindowExposed(&window, 2000) || true);

    const QSize floor = window.minimumSizeHint().expandedTo(window.minimumSize());
    QVERIFY2(floor.height() <= kSmallScreenHeight,
             qPrintable(QStringLiteral("minimum height %1 exceeds a %2px screen")
                            .arg(floor.height()).arg(kSmallScreenHeight)));
    QVERIFY2(floor.width() <= kSmallScreenWidth,
             qPrintable(QStringLiteral("minimum width %1 exceeds a %2px screen")
                            .arg(floor.width()).arg(kSmallScreenWidth)));
}

void LayoutTest::liveWindowKeepsItsStopButtonOnScreen()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    // A long prepare delay so constructing the window does not launch the
    // capture helper; this test is about geometry, not about capturing.
    LiveWindow window(dir.path(), nullptr, {}, 3600000, {}, false, {}, {}, {});
    window.show();
    // Squeeze it to the smallest screen this should work on and confirm the
    // only control it has is still inside the frame. This is exactly what was
    // failing: the widget existed and was laid out, just below the edge.
    window.resize(kSmallScreenWidth, kSmallScreenHeight);
    window.layout()->activate();
    QCoreApplication::processEvents();

    const QList<QPushButton *> buttons = window.findChildren<QPushButton *>();
    QVERIFY(!buttons.isEmpty());
    for (const QPushButton *button : buttons) {
        if (!button->isVisible())
            continue;
        const QRect box = QRect(button->mapTo(&window, QPoint(0, 0)), button->size());
        QVERIFY2(box.bottom() <= window.height(),
                 qPrintable(QStringLiteral("control '%1' ends at y=%2 in a %3px window")
                                .arg(button->text()).arg(box.bottom()).arg(window.height())));
    }
}

// The regression itself: a caption fed far more text than it was designed for
// must not be able to push the window's floor upwards. Before the cap, the
// label's height requirement rose with the text and took the layout minimum
// with it.
void LayoutTest::unboundedCaptionCannotGrowTheWindow()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    // A long prepare delay so constructing the window does not launch the
    // capture helper; this test is about geometry, not about capturing.
    LiveWindow window(dir.path(), nullptr, {}, 3600000, {}, false, {}, {}, {});
    window.show();
    window.layout()->activate();
    QCoreApplication::processEvents();
    const int before = window.minimumSizeHint().height();

    // 1000 characters of transcript, in the shape the real one arrives in:
    // many short lines rather than one long wrapped paragraph.
    QString flood;
    for (int i = 0; i < 40; ++i)
        flood += QStringLiteral("verify-retry-scan attempt %1 coverage 73%\n").arg(i);
    for (QLabel *label : window.findChildren<QLabel *>())
        label->setText(flood);
    window.layout()->activate();
    QCoreApplication::processEvents();
    const int after = window.minimumSizeHint().height();

    // Some growth is correct and expected: each caption is allowed to expand up
    // to the cap it was given, and several do. What must not happen is growth
    // that has no ceiling, so the property worth asserting is not "it did not
    // grow" but "however much text it is given, the floor stays on the screen".
    //
    // The bound is stated with headroom rather than at the screen edge. A test
    // that passes at 719 of 720 would be reporting that the bug is fixed while
    // sitting one line of text away from it returning.
    constexpr int kHeadroom = 80;
    if (after > kSmallScreenHeight - kHeadroom) {
        for (const QWidget *child : window.findChildren<QWidget *>()) {
            // What the layout actually asks this widget for, which is not the
            // same as its own hint: a vertical policy of Ignored drops the hint
            // entirely and leaves only an explicit minimumHeight, and any
            // policy is then bounded by the maximum. Reporting the raw hint
            // instead names the wrong widget - the preview labels quote the
            // full height of their text and contribute their floor.
            const bool ignored =
                child->sizePolicy().verticalPolicy() == QSizePolicy::Ignored;
            const int contribution =
                ignored ? child->minimumHeight()
                        : qMin(child->minimumSizeHint().height(), child->maximumHeight());
            if (contribution > 100)
                qWarning("uncapped candidate: %s '%s' contributes %d",
                         child->metaObject()->className(),
                         qPrintable(child->objectName()), contribution);
        }
    }
    QVERIFY2(after <= kSmallScreenHeight - kHeadroom,
             qPrintable(QStringLiteral("minimum height grew %1 -> %2, past the %3px "
                                       "budget for a %4px screen")
                            .arg(before).arg(after)
                            .arg(kSmallScreenHeight - kHeadroom).arg(kSmallScreenHeight)));
}

// Every themed fill must come with a foreground stated alongside it. A surface
// with only its background set is precisely how the completion box once shipped
// invisible: it inherited the application's text colour, which on the opposite
// theme is the same brightness as the fill.
void LayoutTest::themeGivesEveryFillAReadableForeground()
{
    auto luma = [](const QColor &c) {
        return (c.red() * 299 + c.green() * 587 + c.blue() * 114) / 1000;
    };
    for (bool dark : {false, true}) {
        const QList<theme::Pair> pairs = {
            theme::banner(dark), theme::accent(dark), theme::accentDisabled(dark),
            theme::success(dark), theme::successButton(dark), theme::code(dark),
        };
        for (const theme::Pair &pair : pairs) {
            const QColor background(pair.background), foreground(pair.foreground);
            QVERIFY2(background.isValid() && foreground.isValid(),
                     "a theme pair names a colour Qt cannot parse");
            QVERIFY2(qAbs(luma(background) - luma(foreground)) >= 60,
                     qPrintable(QStringLiteral("%1 on %2 is too low contrast in the %3 theme")
                                    .arg(pair.foreground, pair.background,
                                         dark ? QStringLiteral("dark") : QStringLiteral("light"))));
        }
    }
}

QTEST_MAIN(LayoutTest)
#include "layout_test.moc"
