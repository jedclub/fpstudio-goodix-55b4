#pragma once
#include <QImage>
#include <QCoreApplication>
#include <QString>
#include <cmath>

namespace fpstudio {
// Conservative research-only ranking, not a trained fingerprint detector or
// authentication decision. Scores must be validated on held-out re-touches.
struct FrameQuality {
    bool eligible = false;
    double coherence = 0, motion = 0, score = 0;
    QString reason;
};
inline FrameQuality assessPreview(const QImage &input, const QImage &previous,
                                  double signal, int coverage, int sharpness)
{
    FrameQuality q;
    if (input.size() != QSize(108, 88)) {
        q.reason = QCoreApplication::translate("fpstudio", "Unexpected image size");
        return q;
    }
    const QImage im = input.convertToFormat(QImage::Format_Grayscale8);
    const QImage prev = previous.convertToFormat(QImage::Format_Grayscale8);
    int blocks = 0; double coherence = 0;
    for (int by = 1; by + 8 < im.height(); by += 8) {
        for (int bx = 1; bx + 8 < im.width(); bx += 8) {
            double xx = 0, yy = 0, xy = 0;
            for (int y = by; y < by + 8; ++y) {
                const uchar *row = im.constScanLine(y);
                const uchar *above = im.constScanLine(y - 1);
                const uchar *below = im.constScanLine(y + 1);
                for (int x = bx; x < bx + 8; ++x) {
                    const double dx = row[x+1] - row[x-1];
                    const double dy = below[x] - above[x];
                    xx += dx*dx; yy += dy*dy; xy += dx*dy;
                }
            }
            if (xx + yy > 6400) { coherence += std::hypot(xx - yy, 2*xy)/(xx + yy); ++blocks; }
        }
    }
    q.coherence = blocks ? coherence / blocks : 0;
    if (prev.size() == im.size()) {
        for (int y = 0; y < im.height(); ++y) {
            const uchar *row = im.constScanLine(y);
            const uchar *old = prev.constScanLine(y);
            for (int x = 0; x < im.width(); ++x)
                q.motion += std::abs(int(row[x]) - int(old[x]));
        }
        q.motion /= im.width() * im.height();
    } else q.motion = 255;
    if (signal < 100) q.reason = QCoreApplication::translate("fpstudio", "Waiting for a finger");
    else if (coverage < 62) q.reason = QCoreApplication::translate("fpstudio", "Insufficient contact area");
    else if (sharpness < 16) q.reason = QCoreApplication::translate("fpstudio", "Insufficient ridge contrast");
    else if (blocks < 20 || q.coherence < .45) q.reason = QCoreApplication::translate("fpstudio", "Insufficient consistent ridge pattern");
    else if (q.motion > 18) q.reason = QCoreApplication::translate("fpstudio", "Waiting for motion to settle");
    else { q.eligible = true; q.reason = QCoreApplication::translate("fpstudio", "Research candidate"); }
    q.score = q.coherence * qMin(sharpness, 50) * qMin(coverage, 80) / 80.0;
    return q;
}
}
