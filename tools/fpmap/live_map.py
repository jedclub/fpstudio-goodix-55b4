#!/usr/bin/env python3
"""Build a composite fingerprint map from the live preview, as the finger moves.

Research tool. Produces no authentication decision and replaces no enrolment.

Why this and not the per-contact builder
----------------------------------------
build_map.py assembles separate touches, and it does not work: registering two
captures taken either side of a lift leaves a median error of 5.5 px, which on
a sensor whose ridges repeat every 8-10 px is most of a ridge. Merging at that
accuracy cancels the ridges instead of reinforcing them - the assembled map
scored 0.29 against probes where the raw gallery scored 0.77.

That 5.5 px is not the sensor and not the matcher. Measured on this device over
512 consecutive preview frames of a finger rolling without lifting:

    inter-frame motion      registration error
         0-2 px                  0.011 px
         2-5 px                  0.268 px
        5-10 px                  0.536 px
       10-20 px                  1.262 px
        20+ px                  23.251 px

Keeping the finger down is worth two to three orders of magnitude. The error
that made the composite impossible is the cost of lifting, and this never lifts.

The envelope is the last row: past about 20 px between frames the registration
stops being meaningful, so at the ~10 FPS this preview runs the finger has to
move slower than roughly 5 mm/s. Frames that break that are dropped rather than
merged, because a wrong placement is worse than a missing one.

Drift
-----
Motion is measured against the previous frame and drift against the map, and it
has to be that way round. Two frames 90 ms apart are nearly the same picture and
correlate at 0.95; the map is a mean of everything that has crossed it, so it is
smoother than any single frame and correlates with none of them well. Asking the
map for the motion directly was the first version of this, and it threw away 83%
of the frames because it could not answer.

But frame-to-frame alone is a random walk - each step is a fraction of a pixel
and hundreds of steps are not. So wherever a frame lands on ground the map
already covers, the map is allowed to correct the track, and only by a few
pixels: a larger disagreement than that is not drift being corrected, it is the
map and the track telling different stories, and nothing here establishes which
one is right.

Privacy
-------
Frames are read from the preview file, registered and merged in memory, and
dropped. No video is written. What lands on disk is one map, 0600 in a 0700
directory, the same handling every other biometric artefact here gets - and it
is one artefact where the per-contact path leaves hundreds.
"""
import argparse
import json
import math
import os
import re
import sys
import time
from pathlib import Path

import numpy as np
from PIL import Image

WIDTH, HEIGHT = 108, 88
HEADER = re.compile(rb"P5\n#\s*([^\n]*)\n(\d+)\s+(\d+)\n(\d+)\n")


def read_preview(path):
    """One frame plus the driver's own metrics, or None if it is not readable.

    The driver replaces this file whole for every frame and stamps the header
    with the sensor timestamp, the contact signal and whether it considers a
    finger present. Those are read rather than re-derived: the driver is the
    thing that knows.
    """
    try:
        raw = path.read_bytes()
    except OSError:
        return None
    match = HEADER.match(raw)
    if not match:
        return None
    fields = match.group(1).split()
    width, height = int(match.group(2)), int(match.group(3))
    if (width, height) != (WIDTH, HEIGHT):
        return None
    body = raw[match.end():match.end() + width * height]
    if len(body) < width * height:
        return None
    pixels = np.frombuffer(body, dtype="u1").astype(np.float64).reshape(height, width)
    stamp = int(fields[0]) if fields else 0
    touching = len(fields) >= 7 and fields[6] == b"1"
    signal = float(fields[1]) if len(fields) > 1 else 0.0
    return stamp, pixels, touching, signal


WINDOW = np.outer(np.hanning(HEIGHT), np.hanning(WIDTH))


def normalise(frame):
    """Mean-removed and unit-norm, so a correlation peak is directly an NCC."""
    out = (frame - frame.mean()) * WINDOW
    return out / (math.sqrt(float((out * out).sum())) + 1e-9)


def rotate(frame, degrees):
    """Rotate about the frame centre, bilinear, keeping the same footprint."""
    if abs(degrees) < 1e-3:
        return frame
    return np.asarray(
        Image.fromarray(frame).rotate(degrees, resample=Image.BILINEAR,
                                      fillcolor=float(frame.mean())),
        dtype=np.float64)


def align_rigid(reference, frame_raw, limit, base_angle, span=2.0, step=0.5):
    """Translation and rotation together, by trying a few angles.

    Rolling a finger turns it. The first version of this tracked translation
    only and produced a map whose centre was visibly smeared even though every
    frame correlated with its predecessor at 0.998 - a rotation that is never
    estimated is a rotation that accumulates, and 600 frames of it is a lot.
    Per-contact data from this finger spanned -12 to 44 degrees, so the total
    is not small; the per-frame increment is, which is why a narrow search
    around the angle already being carried is enough.

    Brute force over a handful of angles, because at 108x88 a rotation and an
    FFT are both cheap and the alternative - recovering rotation analytically
    through a log-polar transform - is a great deal of machinery for an
    increment that never exceeds a couple of degrees.
    """
    best = None
    offsets = np.arange(-span, span + 1e-9, step)
    for offset in offsets:
        angle = base_angle + offset
        candidate = normalise(rotate(frame_raw, angle - base_angle))
        dx, dy, peak = align(reference, candidate, limit)
        if best is None or peak > best[2]:
            best = (dx, dy, peak, angle)
    return best


def align(reference, frame, limit):
    """Best translation of `frame` onto `reference`, to sub-pixel.

    Plain cross-correlation, deliberately not phase correlation. Phase
    correlation whitens every frequency equally, and this sensor has a fixed
    pattern that does not move with the finger; whitened, that stationary
    pattern wins and the answer is always zero. It was always zero here, and
    the consistency check agreed with itself because three zeros are
    consistent. Brute-force NCC on the same pair found the true 8 px shift at
    0.965, which is what this uses.
    """
    correlation = np.fft.irfft2(
        np.fft.rfft2(reference) * np.conj(np.fft.rfft2(frame)), s=reference.shape)
    correlation = np.fft.fftshift(correlation)
    cy, cx = reference.shape[0] // 2, reference.shape[1] // 2
    patch = correlation[cy - limit:cy + limit + 1, cx - limit:cx + limit + 1]
    iy, ix = np.unravel_index(np.argmax(patch), patch.shape)
    peak = float(patch[iy, ix])

    def parabola(middle, low, high):
        curve = low - 2 * middle + high
        if curve == 0:
            return 0.0
        step = (low - high) / (2 * curve)
        return step if abs(step) < 1 else 0.0

    dy = parabola(patch[iy, ix], patch[max(iy - 1, 0), ix],
                  patch[min(iy + 1, patch.shape[0] - 1), ix])
    dx = parabola(patch[iy, ix], patch[iy, max(ix - 1, 0)],
                  patch[iy, min(ix + 1, patch.shape[1] - 1)])
    return (ix - limit + dx), (iy - limit + dy), peak


class Canvas:
    """The map as it accumulates: a weighted running mean plus its weights."""

    def __init__(self, size):
        self.total = np.zeros((size, size), dtype=np.float64)
        self.weight = np.zeros((size, size), dtype=np.float64)
        self.size = size
        px, py = np.meshgrid(np.arange(WIDTH) - WIDTH / 2.0,
                             np.arange(HEIGHT) - HEIGHT / 2.0)
        self.px, self.py = px, py
        # Least say at the frame's edge, where contact is most marginal.
        taper = np.clip((1 - np.abs(px) / (WIDTH / 2.0)) / 0.3, 0, 1) * \
                np.clip((1 - np.abs(py) / (HEIGHT / 2.0)) / 0.3, 0, 1)
        self.taper = taper

    def image(self):
        out = np.zeros_like(self.total)
        seen = self.weight > 0
        out[seen] = self.total[seen] / self.weight[seen]
        return out, seen

    def window(self, cx, cy):
        """The map under a frame placed at (cx, cy), for aligning against."""
        x0, y0 = int(round(cx - WIDTH / 2)), int(round(cy - HEIGHT / 2))
        x0 = max(0, min(self.size - WIDTH, x0))
        y0 = max(0, min(self.size - HEIGHT, y0))
        current, seen = self.image()
        return current[y0:y0 + HEIGHT, x0:x0 + WIDTH], \
               seen[y0:y0 + HEIGHT, x0:x0 + WIDTH], x0, y0

    def add(self, frame, cx, cy, angle, gain=1.0):
        c, s = math.cos(math.radians(angle)), math.sin(math.radians(angle))
        X = np.rint(cx + self.px * c - self.py * s).astype(int)
        Y = np.rint(cy + self.px * s + self.py * c).astype(int)
        ok = (X >= 0) & (X < self.size) & (Y >= 0) & (Y < self.size) & (self.taper > 0)
        w = self.taper[ok] * gain
        np.add.at(self.total, (Y[ok], X[ok]), frame[ok] * w)
        np.add.at(self.weight, (Y[ok], X[ok]), w)

    def covered(self):
        return int((self.weight > 0).sum())


def newest_session():
    import glob
    sessions = sorted(glob.glob(os.path.expanduser(
        "~/.local/share/fpstudio/recognition/live-*")), key=os.path.getmtime)
    return Path(sessions[-1]) if sessions else None


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n", 1)[0])
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--seconds", type=float, default=60)
    parser.add_argument("--size", type=int, default=360)
    parser.add_argument("--max-step", type=float, default=18.0,
                        help="pixels of inter-frame motion past which a frame is dropped")
    parser.add_argument("--min-peak", type=float, default=0.55,
                        help="correlation below this is not a placement, it is a guess")
    options = parser.parse_args()

    session = newest_session()
    if not session or not (session / "live.pgm").exists():
        print("프리뷰 세션을 찾지 못했습니다. 먼저 --live 창을 띄우세요.")
        return 1
    live = session / "live.pgm"
    print(f"세션 {session.name}")

    canvas = Canvas(options.size)
    centre = options.size / 2.0
    pose = [centre, centre, 0.0]
    velocity = [0.0, 0.0]
    seen_stamps = set()
    merged = dropped = skipped = 0
    previous = None
    peaks = []
    deadline = time.time() + options.seconds
    started = False

    while time.time() < deadline:
        got = read_preview(live)
        if got is None:
            time.sleep(0.005)
            continue
        stamp, pixels, touching, signal = got
        if stamp in seen_stamps:
            time.sleep(0.005)
            continue
        seen_stamps.add(stamp)
        if not touching:
            skipped += 1
            velocity = [0.0, 0.0]        # contact broke; no motion to carry over
            continue

        frame = normalise(pixels)
        if not started:
            canvas.add(pixels, pose[0], pose[1], 0.0)
            previous = normalise(pixels)
            started = True
            merged += 1
            continue

        # Step one: where did the finger go since the last frame?
        #
        # Against the previous frame this is the easy question - two frames 90
        # ms apart are nearly the same picture, and the correlation peak sits
        # at 0.95. Against the map it is a much harder one: the map is a mean
        # of everything that has passed over it, so it is smoother than any
        # single frame and correlates with none of them especially well. The
        # first version asked the map directly and was answered badly enough to
        # throw away 83% of the frames.
        #
        # So the motion comes from the previous frame, which knows it, and the
        # map is consulted afterwards only to correct where that motion has
        # drifted to - which is the one thing the previous frame cannot know.
        dx, dy, peak, rel = align_rigid(previous, pixels, 24, 0.0)
        peaks.append(peak)
        if peak < options.min_peak:
            dropped += 1
            velocity = [0.0, 0.0]
            previous = normalise(pixels)
            continue

        new_angle = pose[2] + rel
        new_x = pose[0] - dx
        new_y = pose[1] - dy

        # Step two: drift. Frame-to-frame error is tiny but it is a random walk,
        # and over hundreds of frames a tiny random walk is not tiny. Wherever
        # the frame lands on ground the map already covers well, the map gets to
        # correct it - and only by a little, because a large correction here
        # means the map disagrees with the track, and at that point which of
        # them is wrong is not established.
        target, seen, x0, y0 = canvas.window(new_x, new_y)
        if seen.mean() > 0.75:
            mdx, mdy, mpeak = align(normalise(target),
                                    normalise(rotate(pixels, -new_angle)), limit=6)
            if mpeak >= options.min_peak:
                cx = x0 + WIDTH / 2.0 - mdx
                cy = y0 + HEIGHT / 2.0 - mdy
                if math.hypot(cx - new_x, cy - new_y) <= 4.0:
                    new_x, new_y = cx, cy
        step = math.hypot(new_x - pose[0], new_y - pose[1])
        if step > options.max_step:
            # Past the envelope the registration stops meaning anything. A
            # frame placed wrongly corrupts every later alignment that matches
            # against it, so it is thrown away instead.
            dropped += 1
            velocity = [0.0, 0.0]
            continue

        velocity = [new_x - pose[0], new_y - pose[1]]
        pose = [new_x, new_y, new_angle]
        canvas.add(rotate(pixels, -new_angle), new_x, new_y, 0.0)
        previous = normalise(pixels)
        merged += 1
        if merged % 25 == 0:
            print(f"  병합 {merged}  버림 {dropped}  "
                  f"커버리지 {canvas.covered()/(WIDTH*HEIGHT):.2f}배  "
                  f"정점 {np.median(peaks[-25:]):.2f}", end="\r", flush=True)

    print()
    image, seen = canvas.image()
    ys, xs = np.nonzero(seen)
    if len(xs) == 0:
        print("병합된 프레임이 없습니다.")
        return 1
    crop = image[ys.min():ys.max() + 1, xs.min():xs.max() + 1]

    options.out.parent.mkdir(mode=0o700, parents=True, exist_ok=True)
    Image.fromarray(np.clip(crop, 0, 255).astype(np.uint8)).save(options.out)
    os.chmod(options.out, 0o600)
    meta = {
        "research_only": True, "authentication_decision": None,
        "biometric_security_validated": False,
        "source": "live preview stream; no frames retained",
        "merged": merged, "dropped": dropped, "no_contact": skipped,
        "map_size": [int(crop.shape[1]), int(crop.shape[0])],
        "covered_vs_sensor": round(canvas.covered() / (WIDTH * HEIGHT), 3),
        "median_peak": round(float(np.median(peaks)), 3) if peaks else None,
        "final_angle": round(pose[2], 2),
    }
    side = options.out.with_suffix(".json")
    side.write_text(json.dumps(meta, indent=2))
    os.chmod(side, 0o600)
    print(f"병합 {merged} / 버림 {dropped} / 비접촉 {skipped}")
    print(f"지도 {crop.shape[1]} x {crop.shape[0]} px, "
          f"커버리지 {meta['covered_vs_sensor']}배, 정점 중앙 {meta['median_peak']}")
    print(f"저장: {options.out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
