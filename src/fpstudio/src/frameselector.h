#pragma once
#include <QImage>
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
    if (input.size() != QSize(108, 88)) { q.reason = "영상 크기 오류"; return q; }
    const QImage im = input.convertToFormat(QImage::Format_Grayscale8);
    const QImage prev = previous.convertToFormat(QImage::Format_Grayscale8);
    int blocks = 0; double coherence = 0;
    for (int by = 1; by + 8 < im.height(); by += 8) {
        for (int bx = 1; bx + 8 < im.width(); bx += 8) {
            double xx = 0, yy = 0, xy = 0;
            for (int y = by; y < by + 8; ++y) {
                for (int x = bx; x < bx + 8; ++x) {
                    const double dx = im.constScanLine(y)[x+1] - im.constScanLine(y)[x-1];
                    const double dy = im.constScanLine(y+1)[x] - im.constScanLine(y-1)[x];
                    xx += dx*dx; yy += dy*dy; xy += dx*dy;
                }
            }
            if (xx + yy > 6400) { coherence += std::hypot(xx - yy, 2*xy)/(xx + yy); ++blocks; }
        }
    }
    q.coherence = blocks ? coherence / blocks : 0;
    if (prev.size() == im.size()) {
        for (int y = 0; y < im.height(); ++y)
            for (int x = 0; x < im.width(); ++x)
                q.motion += std::abs(int(im.constScanLine(y)[x]) - int(prev.constScanLine(y)[x]));
        q.motion /= im.width() * im.height();
    } else q.motion = 255;
    if (signal < 100) q.reason = "손가락 대기";
    else if (coverage < 62) q.reason = "접촉 면적 부족";
    else if (sharpness < 16) q.reason = "무늬 대비 부족";
    else if (blocks < 20 || q.coherence < .45) q.reason = "일관된 무늬 부족";
    else if (q.motion > 18) q.reason = "움직임 안정 대기";
    else { q.eligible = true; q.reason = "연구 후보"; }
    q.score = q.coherence * qMin(sharpness, 50) * qMin(coverage, 80) / 80.0;
    return q;
}
}
