#!/usr/bin/env python3
"""Score a capture against a composite map, and against the gallery it replaces.

Research tool. Emits similarity numbers and no authentication decision.

This is a CPU reference implementation, written to answer one question before
anything is built on the GPU: does matching a probe against one large fused map
find the finger more reliably than matching it against the many small captures
the map was built from?

The comparison is deliberately like-for-like. Both sides use the same
similarity - normalised cross-correlation over the overlapping region, with the
same minimum overlap - so the only thing that differs is what the probe is being
compared against. The gallery side takes the best score over all references,
which is what the shipped matcher does.

Speed is not the point here and this is not the shipped matcher; the GPU path
searches far more transforms than this does. A finding here is a reason to build
that, not a substitute for it.
"""
import argparse
import glob
import json
import math
import os
from pathlib import Path

import numpy as np
from PIL import Image

WIDTH, HEIGHT = 108, 88
MIN_OVERLAP = 0.50          # same floor the shipped matcher applies


def load_gray(path):
    return np.asarray(Image.open(path).convert("L"), dtype=np.float32)


def sample(target, valid, xs, ys):
    """Nearest-neighbour lookup with an explicit validity mask."""
    h, w = target.shape
    inside = (xs >= 0) & (xs < w) & (ys >= 0) & (ys < h)
    xi = np.clip(xs, 0, w - 1).astype(np.int32)
    yi = np.clip(ys, 0, h - 1).astype(np.int32)
    return target[yi, xi], inside & valid[yi, xi]


def ncc(a, b):
    """Normalised cross-correlation of two equal-length vectors."""
    if a.size < 32:
        return 0.0
    a = a - a.mean()
    b = b - b.mean()
    da, db = math.sqrt(float(a @ a)), math.sqrt(float(b @ b))
    if da < 1e-6 or db < 1e-6:
        return 0.0
    return float(a @ b) / (da * db)


def search(target, valid, probe, angles, dxs, dys, stride=2):
    """Best NCC of the probe placed anywhere on the target.

    The probe is the thing being moved, so its sampled pixels are fixed and
    only the target lookups change - which is what makes this affordable in
    numpy without writing a shader for it.
    """
    py, px = np.mgrid[0:HEIGHT:stride, 0:WIDTH:stride]
    px = (px - WIDTH / 2.0).ravel()
    py = (py - HEIGHT / 2.0).ravel()
    values = probe[0:HEIGHT:stride, 0:WIDTH:stride].ravel()
    live = values > 24                      # the driver's dead-pixel floor
    px, py, values = px[live], py[live], values[live]
    if values.size < 32:
        return 0.0, None

    cx, cy = target.shape[1] / 2.0, target.shape[0] / 2.0
    best, where = 0.0, None
    for angle in angles:
        c, s = math.cos(math.radians(angle)), math.sin(math.radians(angle))
        rx = px * c - py * s
        ry = px * s + py * c
        for dy in dys:
            for dx in dxs:
                xs = np.rint(rx + cx + dx).astype(np.int32)
                ys = np.rint(ry + cy + dy).astype(np.int32)
                got, ok = sample(target, valid, xs, ys)
                if ok.sum() < values.size * MIN_OVERLAP:
                    continue
                score = ncc(values[ok], got[ok])
                if score > best:
                    best, where = score, (dx, dy, angle)
    return best, where


def coarse_to_fine(target, valid, probe, span_x, span_y, angle_span=20):
    coarse, _ = None, None
    best, where = search(target, valid, probe,
                         range(-angle_span, angle_span + 1, 5),
                         range(-span_x, span_x + 1, 6),
                         range(-span_y, span_y + 1, 6), stride=3)
    if where is None:
        return best, where
    dx0, dy0, a0 = where
    return search(target, valid, probe,
                  [a0 + d for d in range(-4, 5, 2)],
                  range(dx0 - 5, dx0 + 6, 1),
                  range(dy0 - 5, dy0 + 6, 1), stride=2)


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n", 1)[0])
    parser.add_argument("--map", required=True, type=Path)
    parser.add_argument("--gallery", required=True, type=Path,
                        help="directory of the reference captures the map replaces")
    parser.add_argument("--probes", required=True, nargs="+", type=Path)
    parser.add_argument("--label", default="genuine",
                        help="what these probes are, for the report only")
    options = parser.parse_args()

    composite = load_gray(options.map)
    covered = composite > 0
    refs = sorted(glob.glob(str(options.gallery / "*.png")))
    gallery = [load_gray(p) for p in refs]
    gallery_valid = [g > 0 for g in gallery]

    span_x = max(8, (composite.shape[1] - WIDTH) // 2 + 6)
    span_y = max(8, (composite.shape[0] - HEIGHT) // 2 + 6)
    print(f"지도 {composite.shape[1]}x{composite.shape[0]}, 갤러리 {len(gallery)}장, "
          f"probe {len(options.probes)}개, 라벨 {options.label}")
    print(f"{'probe':<26}{'지도':>8}{'갤러리최고':>11}{'차이':>8}")

    rows = []
    for path in options.probes:
        probe = load_gray(path)
        map_score, _ = coarse_to_fine(composite, covered, probe, span_x, span_y)
        best_ref = 0.0
        for g, v in zip(gallery, gallery_valid):
            score, _ = coarse_to_fine(g, v, probe, 24, 24)
            best_ref = max(best_ref, score)
        rows.append((os.path.basename(path), map_score, best_ref))
        print(f"{os.path.basename(path):<26}{map_score:>8.3f}{best_ref:>11.3f}"
              f"{map_score - best_ref:>+8.3f}")

    m = sorted(r[1] for r in rows)
    g = sorted(r[2] for r in rows)
    print()
    print(f"{'':<26}{'지도':>8}{'갤러리':>11}")
    print(f"{'중앙값':<26}{m[len(m)//2]:>8.3f}{g[len(g)//2]:>11.3f}")
    print(f"{'최솟값':<26}{m[0]:>8.3f}{g[0]:>11.3f}")
    wins = sum(1 for _, a, b in rows if a > b)
    print(f"\n지도가 더 높은 경우: {wins}/{len(rows)}")
    print("연구용 수치이며 인증 판정이 아니다.")


if __name__ == "__main__":
    main()
