#!/usr/bin/env python3
"""Looks for contamination that is common to every capture rather than
specific to one.

The question this answers is not "is this image good" - the diagnostics window
already reports coverage and sharpness per capture. It is whether something
fixed is riding on top of every capture: sensor fixed-pattern noise that the
background subtraction did not remove, a column or row bias, or a dead area
that is being stretched into visible structure.

Why it matters for the match score specifically: anything present in the same
place in every image produces SIFT keypoints in the same place in every image.
Those keypoints match between *any* two captures, including two different
fingers. They inflate impostor scores, and they consume the correspondence
budget that the real ridges need. So common-mode structure is worse than
noise - it is noise that looks like signal to the matcher.
"""
import sys
import numpy as np
from PIL import Image


def load(paths):
    imgs = []
    for p in paths:
        a = np.asarray(Image.open(p).convert("L"), dtype=np.float64)
        imgs.append(a)
    return np.stack(imgs)


def main(paths):
    st = load(paths)
    n, h, w = st.shape
    print(f"{n} captures, {w}x{h}\n")

    print("per-capture:")
    print(f"  {'file':<28} {'mean':>6} {'std':>6} {'p1':>4} {'p50':>4} {'p99':>4} {'sharp':>6}")
    for p, a in zip(paths, st):
        # Same sharpness proxy the driver's quality gate uses: mean absolute
        # Laplacian, so the numbers here are comparable to what it logs.
        lap = np.abs(4 * a[1:-1, 1:-1] - a[:-2, 1:-1] - a[2:, 1:-1]
                     - a[1:-1, :-2] - a[1:-1, 2:])
        print(f"  {p.split('/')[-1]:<28} {a.mean():6.1f} {a.std():6.1f} "
              f"{np.percentile(a,1):4.0f} {np.percentile(a,50):4.0f} "
              f"{np.percentile(a,99):4.0f} {lap.mean():6.1f}")

    mean = st.mean(axis=0)
    # A per-pixel mean over captures whose finger positions differ should be
    # close to flat: the ridges move, so they average out. Whatever survives is
    # common to every capture, i.e. not the finger.
    resid = mean - mean.mean()
    print(f"\ncommon-mode image (mean of all captures, then de-meaned):")
    print(f"  std                 {resid.std():6.2f}  (of a 0-255 range)")
    print(f"  range               {resid.min():6.1f} .. {resid.max():6.1f}")

    col = resid.mean(axis=0)
    row = resid.mean(axis=1)
    print(f"  column-bias std     {col.std():6.2f}   max |bias| {np.abs(col).max():5.1f}")
    print(f"  row-bias std        {row.std():6.2f}   max |bias| {np.abs(row).max():5.1f}")

    # Column-to-column alternation is the signature of an interleaved readout
    # or a per-column amplifier offset - the striping this project chased
    # before. A high value here means the sensor's own pattern is still in the
    # image the matcher sees.
    alt_c = np.abs(np.diff(col)).mean()
    alt_r = np.abs(np.diff(row)).mean()
    print(f"  adjacent-column step {alt_c:5.2f}   adjacent-row step {alt_r:5.2f}")

    # How much of any single capture is explained by that common image. If this
    # is large, the matcher is looking at the sensor as much as at the finger.
    frac = []
    for a in st:
        d = a - a.mean()
        r = resid
        denom = (d * d).sum()
        frac.append(float((d * r).sum() ** 2 / (denom * (r * r).sum())) if denom else 0.0)
    print(f"  variance explained  {100*np.mean(frac):5.1f}% of a capture, on average")

    # Saturation: the percentile stretch clips deliberately, but if a large
    # part of the frame is pinned at 0 or 255 there is no gradient there and
    # SIFT can find nothing.
    lo = float((st <= 0).mean() * 100)
    hi = float((st >= 255).mean() * 100)
    print(f"\nclipping: {lo:.1f}% of pixels at 0, {hi:.1f}% at 255")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.exit("usage: analyse.py <capture.png> ...")
    main(sys.argv[1:])
