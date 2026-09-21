#!/usr/bin/env python3
"""Assemble many partial captures into one composite fingerprint map.

Research tool. Produces no authentication decision and replaces no enrolment.

Why this exists
---------------
This sensor images about 25 mm^2 - roughly 5.5 x 4.5 mm. A finger is several
times that, so every touch sees a different patch and most pairs of touches do
not overlap at all: measured over 3540 ordered pairs of real captures from this
device, 43% registered against each other and the median pair scored zero.

The system's answer so far has been to keep 44 separate references and hope one
of them overlaps whatever is presented. That works, and it costs 44 comparisons
per authentication - measured at 9.2 ms each, which is most of the time an
authentication takes.

The alternative is to stop treating the captures as independent. Pairs that do
not overlap each other are still connected through pairs that do, so the whole
set can be placed in one coordinate frame and merged into a single larger
image. Measured on 60 captures from this device: 93% of them land in one
connected component, and the merged result covers 3.58x the sensor area with no
holes in it.

What it does not do
-------------------
Nothing here decides whether the map recognises better than the 44 references
it would replace. That is a separate measurement and it has not been made. The
map is written where it can be inspected and compared; it is not installed.

Privacy
-------
A composite is a more complete fingerprint than the scattered partials it is
built from - that is the whole point of it, and it is also the reason it is
written 0600 into a 0700 directory, like every other biometric artefact in this
repository, and never to a shared or tracked path.
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


# ----------------------------------------------------------------- pose graph

def compose(pose, dx, dy, dangle):
    """Apply a relative transform to a pose, in the pose's own frame."""
    x, y, a = pose
    c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
    return (x + dx * c - dy * s, y + dx * s + dy * c, a + dangle)


def invert(dx, dy, dangle):
    """The same relationship seen from the other end."""
    c, s = math.cos(math.radians(dangle)), math.sin(math.radians(dangle))
    return (-(dx * c + dy * s), -(-dx * s + dy * c), -dangle)


def spanning_layout(count, edges):
    """A first guess: walk the graph and chain the transforms.

    Good enough to start from and not good enough to keep - error accumulates
    along every chain, so a patch twenty hops from the root carries twenty
    registrations' worth of drift. optimise() below is what removes that.
    """
    neighbours = {}
    for i, j, dx, dy, angle, weight in edges:
        neighbours.setdefault(i, []).append((j, dx, dy, angle))
        neighbours.setdefault(j, []).append((i, *invert(dx, dy, angle)))
    best = {}
    for root in range(count):
        if root in best:
            continue
        poses = {root: (0.0, 0.0, 0.0)}
        queue = [root]
        while queue:
            node = queue.pop(0)
            for other, dx, dy, angle in neighbours.get(node, ()):
                if other in poses:
                    continue
                poses[other] = compose(poses[node], dx, dy, angle)
                queue.append(other)
        if len(poses) > len(best):
            best = poses
    return best


def optimise(poses, edges, rounds=400):
    """Spread the registration error over every constraint instead of the chain.

    Every edge is a measurement of where one patch sits relative to another,
    and there are far more of them than there are unknowns - 1539 edges for 56
    patches in the run this was written against. The chain layout satisfies the
    handful it happened to walk through and ignores the rest; this moves each
    patch to the position its neighbours collectively predict, repeatedly, until
    nothing moves. It is Gauss-Seidel relaxation on the pose graph.

    Angles are averaged as unit vectors. Averaging degrees directly is how a
    patch at 179 degrees and one at -179 degrees - two degrees apart - end up
    agreeing on zero.

    Edge weight is the interior correlation the matcher reported, so a
    confident registration pulls harder than a marginal one.
    """
    poses = dict(poses)
    incident = {}
    for i, j, dx, dy, angle, weight in edges:
        if i in poses and j in poses:
            incident.setdefault(j, []).append((i, dx, dy, angle, weight))
            incident.setdefault(i, []).append((j, *invert(dx, dy, angle), weight))

    for _ in range(rounds):
        moved = 0.0
        for node, links in incident.items():
            sx = sy = sc = ss = total = 0.0
            for other, dx, dy, angle, weight in links:
                px, py, pa = compose(poses[other], dx, dy, angle)
                sx += weight * px
                sy += weight * py
                sc += weight * math.cos(math.radians(pa))
                ss += weight * math.sin(math.radians(pa))
                total += weight
            if total <= 0:
                continue
            nx, ny = sx / total, sy / total
            na = math.degrees(math.atan2(ss / total, sc / total))
            moved = max(moved, abs(nx - poses[node][0]), abs(ny - poses[node][1]))
            poses[node] = (nx, ny, na)
        if moved < 1e-3:
            break
    return poses


def reject_outliers(poses, edges, factor=2.5, passes=4):
    """Drop the registrations that disagree with everything else, then re-solve.

    A pose graph is a least-squares fit, and least squares has no defence
    against a wrong measurement - one confidently mis-registered pair drags
    every patch near it out of place, and the error is spread thinly enough
    that no single patch looks obviously wrong. On the first real run the
    residuals ran to 80 px against a median of 6.6, which is nine ridges of
    disagreement from something the matcher reported as consistent.

    So the fit is repeated with the worst edges removed. The cutoff is a
    multiple of the median absolute residual rather than a fixed pixel count,
    because the median is what the honest edges agree on and is not itself
    moved by the outliers - a fixed threshold would have to be retuned for
    every gallery.

    Edges are never dropped below a floor, so a sparse graph cannot be pruned
    into disconnection.
    """
    kept = list(edges)
    for _ in range(passes):
        poses = optimise(poses, kept)
        values = residuals(poses, kept)
        if len(values) < 8:
            break
        median = sorted(values)[len(values) // 2]
        limit = max(factor * median, 3.0)
        survivors = [e for e, r in zip(kept, values) if r <= limit]
        if len(survivors) < max(8, len(edges) // 4) or len(survivors) == len(kept):
            break
        kept = survivors
    return optimise(poses, kept), kept


def residuals(poses, edges):
    """How far each edge is from being satisfied, in pixels."""
    out = []
    for i, j, dx, dy, angle, weight in edges:
        if i not in poses or j not in poses:
            continue
        px, py, _ = compose(poses[i], dx, dy, angle)
        qx, qy, _ = poses[j]
        out.append(math.hypot(px - qx, py - qy))
    return out


# --------------------------------------------------------------------- fusion

def render_mosaic(samples, poses, feather=0.25):
    """Take each pixel from whichever patch saw it best, instead of averaging.

    Averaging assumes the patches agree about where the ridges are. They do
    not, and not because the fit is bad: the residual sits at 5.5 px after
    outlier rejection, which is most of a ridge period, and skin is the reason.
    A fingertip is elastic, so pressing at a different angle stretches the
    pattern, and no rotation and translation can undo a stretch. Blending two
    correctly-placed patches that disagree by half a ridge cancels the ridges
    between them - which is why the centre of the averaged map, where the most
    patches overlap, came out blurrier than its edges.

    So this does not blend. Every output pixel is taken whole from the single
    patch whose centre is nearest to it, which is the patch with the most
    contact and the least of the sensor's edge falloff at that point. Ridges
    stay as sharp as they were captured; the cost is a visible seam wherever
    the winner changes, and a seam is a local artefact rather than a loss of
    the signal everywhere.
    """
    xs = [poses[k][0] for k in poses]
    ys = [poses[k][1] for k in poses]
    pad = max(WIDTH, HEIGHT)
    ox, oy = min(xs) - pad / 2, min(ys) - pad / 2
    gw = int(max(xs) - min(xs) + pad) + 1
    gh = int(max(ys) - min(ys) + pad) + 1

    out = np.zeros((gh, gw), dtype=np.float64)
    champion = np.zeros((gh, gw), dtype=np.float64)

    px, py = np.meshgrid(np.arange(WIDTH) - WIDTH / 2.0,
                         np.arange(HEIGHT) - HEIGHT / 2.0)
    fx = np.clip((1 - np.abs(px) / (WIDTH / 2.0)) / feather, 0, 1)
    fy = np.clip((1 - np.abs(py) / (HEIGHT / 2.0)) / feather, 0, 1)
    taper = fx * fy

    for index, path in enumerate(samples):
        if index not in poses:
            continue
        cx, cy, angle = poses[index]
        pixels = np.asarray(Image.open(path).convert("L"), dtype=np.float64)
        if pixels.shape != (HEIGHT, WIDTH):
            continue
        c, s = math.cos(math.radians(angle)), math.sin(math.radians(angle))
        X = np.rint(cx + px * c - py * s - ox).astype(int)
        Y = np.rint(cy + px * s + py * c - oy).astype(int)
        ok = (X >= 0) & (X < gw) & (Y >= 0) & (Y < gh) & (taper > 0)
        yy, xx, tt, vv = Y[ok], X[ok], taper[ok], pixels[ok]
        better = tt > champion[yy, xx]
        out[yy[better], xx[better]] = vv[better]
        champion[yy[better], xx[better]] = tt[better]

    return out, champion > 0, (ox, oy)


def render(samples, poses, feather=0.25):
    """Merge the placed patches into one image.

    Weighted by distance from each patch's own centre. The sensor's edge rows
    are where contact is most marginal and where the driver's background
    subtraction has the least to work with, so a patch should have the least say
    exactly there - otherwise every seam in the map sits on the least reliable
    pixels either side of it.
    """
    xs = [poses[k][0] for k in poses]
    ys = [poses[k][1] for k in poses]
    pad = max(WIDTH, HEIGHT)
    ox, oy = min(xs) - pad / 2, min(ys) - pad / 2
    gw = int(max(xs) - min(xs) + pad) + 1
    gh = int(max(ys) - min(ys) + pad) + 1

    total = np.zeros((gh, gw), dtype=np.float64)
    weight = np.zeros((gh, gw), dtype=np.float64)

    px, py = np.meshgrid(np.arange(WIDTH) - WIDTH / 2.0,
                         np.arange(HEIGHT) - HEIGHT / 2.0)
    # 1 in the middle, falling to 0 at the border.
    fx = np.clip((1 - np.abs(px) / (WIDTH / 2.0)) / feather, 0, 1)
    fy = np.clip((1 - np.abs(py) / (HEIGHT / 2.0)) / feather, 0, 1)
    taper = fx * fy

    for index, path in enumerate(samples):
        if index not in poses:
            continue
        cx, cy, angle = poses[index]
        pixels = np.asarray(Image.open(path).convert("L"), dtype=np.float64)
        if pixels.shape != (HEIGHT, WIDTH):
            continue
        c, s = math.cos(math.radians(angle)), math.sin(math.radians(angle))
        X = cx + px * c - py * s - ox
        Y = cy + px * s + py * c - oy
        xi = np.rint(X).astype(int)
        yi = np.rint(Y).astype(int)
        inside = (xi >= 0) & (xi < gw) & (yi >= 0) & (yi < gh) & (taper > 0)
        np.add.at(total, (yi[inside], xi[inside]), (pixels * taper)[inside])
        np.add.at(weight, (yi[inside], xi[inside]), taper[inside])

    covered = weight > 0
    out = np.zeros_like(total)
    out[covered] = total[covered] / weight[covered]
    return out, covered, (ox, oy)


# ----------------------------------------------------------------------- main

def load_edges(path):
    return [tuple(row) for row in json.load(open(path))]


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n", 1)[0])
    parser.add_argument("--edges", required=True, type=Path,
                        help="pairwise registrations, as written by scan_pairs")
    parser.add_argument("--samples", required=True, type=Path,
                        help="JSON list of capture paths, in edge index order")
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--rounds", type=int, default=400)
    parser.add_argument("--fusion", choices=("mosaic", "blend"), default="mosaic",
                        help="mosaic keeps ridges sharp; blend averages and softens them")
    options = parser.parse_args()

    samples = json.load(open(options.samples))
    edges = load_edges(options.edges)
    print(f"표본 {len(samples)}개, 정합 간선 {len(edges)}개")

    start = spanning_layout(len(samples), edges)
    before = residuals(start, edges)
    plain = optimise(start, edges, options.rounds)
    mid = residuals(plain, edges)
    poses, kept = reject_outliers(start, edges)
    after = residuals(poses, kept)

    def summary(values):
        values = sorted(values)
        return (f"중앙 {values[len(values)//2]:.1f}px  "
                f"90% {values[int(len(values)*.9)]:.1f}px  최대 {values[-1]:.1f}px")

    print(f"배치 {len(poses)}/{len(samples)}개")
    print(f"  사슬 누적 잔차 : {summary(before)}")
    print(f"  최적화 후 잔차 : {summary(mid)}")
    print(f"  이상치 제거 후 : {summary(after)}   (간선 {len(edges)} -> {len(kept)})")

    renderer = render_mosaic if options.fusion == "mosaic" else render
    image, covered, origin = renderer(samples, poses)
    options.out.parent.mkdir(mode=0o700, parents=True, exist_ok=True)
    Image.fromarray(np.clip(image, 0, 255).astype(np.uint8)).save(options.out)
    os.chmod(options.out, 0o600)

    area = int(covered.sum())
    meta = {
        "research_only": True, "fusion": options.fusion,
        "authentication_decision": None,
        "biometric_security_validated": False,
        "samples": len(samples), "placed": len(poses),
        "edges": len(edges), "edges_kept": len(kept),
        "map_size": [image.shape[1], image.shape[0]],
        "covered_pixels": area,
        "covered_vs_sensor": round(area / (WIDTH * HEIGHT), 3),
        "fill_ratio": round(area / image.size, 3),
        "residual_median_px": round(sorted(after)[len(after)//2], 2),
        "origin": [round(origin[0], 2), round(origin[1], 2)],
        "poses": {str(k): [round(v, 3) for v in poses[k]] for k in sorted(poses)},
    }
    side = options.out.with_suffix(".json")
    side.write_text(json.dumps(meta, indent=2))
    os.chmod(side, 0o600)

    print(f"지도 {image.shape[1]} x {image.shape[0]} px, "
          f"덮인 면적 {meta['covered_vs_sensor']}배, 채움 {100*meta['fill_ratio']:.0f}%")
    print(f"저장: {options.out}")


if __name__ == "__main__":
    main()
