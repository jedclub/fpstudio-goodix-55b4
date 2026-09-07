#!/usr/bin/env python3
"""Paired local replay with a frozen gallery. Development evidence, not FAR.

Never adds test probes to the gallery; skips byte-identical gallery images.
Does not access the sensor, PAM, root gallery or network.
"""
import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import statistics
import subprocess
import tempfile
from evaluate import private_json


def accepted(row):
    b = row.get("best", {})
    e = b.get("interior", {})
    return (row.get("ok") is True and b.get("ambiguous") is False
            and b.get("at_search_boundary") is False and e.get("consistent") is True
            and all(type(e.get(k)) in (int, float) and math.isfinite(e[k]) and lo <= e[k] <= 1
                    for k, lo in (("ncc", .70), ("gradient", .65), ("overlap", .50)))
            and all(type(e.get(k)) is int and 8 <= e[k] <= 16
                    for k in ("supported_tiles", "probe_tiles")))


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run_worker(binary, references, probes, auth_search):
    command = [str(binary), "--serve"]
    for path in references:
        command += ["--reference", str(path)]
    if auth_search:
        command.append("--auth-search")
    run = subprocess.run(command, input="".join(json.dumps({"probe": str(p)}) + "\n" for p in probes),
                         text=True, capture_output=True, check=True, timeout=30 + 10 * len(probes))
    rows = [json.loads(line) for line in run.stdout.splitlines()]
    if not rows.pop(0).get("ready") or len(rows) != len(probes):
        raise RuntimeError("Incomplete matcher output")
    for row, path in zip(rows, probes):
        if row.get("ok") is not True or row.get("probe") != str(path):
            raise RuntimeError("Invalid/mismatched result")
        if auth_search and (row.get("algorithm_version") != 7 or row.get("auth_search") is not True):
            raise RuntimeError("Candidate did not execute v7 auth search")
    return rows


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--baseline", type=Path, required=True)
    parser.add_argument("--candidate", type=Path, required=True)
    parser.add_argument("--reference-dir", type=Path, required=True)
    parser.add_argument("--session", type=Path, action="append", required=True)
    parser.add_argument("--output-parent", type=Path, required=True)
    args = parser.parse_args()
    refs = sorted(args.reference_dir.resolve(strict=True).glob("candidate-touch-*.png"))
    if not 1 <= len(refs) <= 64:
        parser.error("Need 1..64 frozen references")
    binaries = [args.baseline.resolve(strict=True), args.candidate.resolve(strict=True)]
    hashes = {digest(p) for p in refs}
    out = Path(tempfile.mkdtemp(prefix="auth-v7-replay-", dir=args.output_parent.resolve(strict=True)))
    os.chmod(out, 0o700)
    print(f"Private results: {out}", flush=True)
    summary = dict(research_only=True, biometric_security_validated=False,
                   note="Reused same-finger development data, not independent FRR/FAR validation.",
                   binaries=[dict(path=str(p), sha256=digest(p)) for p in binaries],
                   gallery=[dict(path=str(p), sha256=digest(p)) for p in refs], sessions=[])
    for session in args.session:
        session = session.resolve(strict=True)
        originals = [json.loads(p.read_text()) for p in sorted(session.glob("gpu-match-*.json"))]
        pairs = [(r, Path(r["probe"]).resolve(strict=True)) for r in originals]
        pairs = [(r, p) for r, p in pairs if digest(p) not in hashes]
        if not pairs:
            raise RuntimeError("No independent-from-gallery probe files")
        if any(p.parent != session for _, p in pairs):
            raise RuntimeError("Probe outside selected session")
        probes = [p for _, p in pairs]
        before, after = [run_worker(binary, refs, probes, index == 1) for index, binary in enumerate(binaries)]
        groups = [set(), set()]
        for index, rows in enumerate((before, after)):
            for i, (row, (original, _)) in enumerate(zip(rows, pairs)):
                group = original["contact_group"]
                row["contact_group"] = group
                if accepted(row):
                    groups[index].add(group)
                private_json(out / f"{session.name}-{'before' if index == 0 else 'after'}-{i:04d}.json", row)
        old, new = list(map(accepted, before)), list(map(accepted, after))
        entry = dict(session=session.name, probes=len(pairs), excluded_identical=len(originals)-len(pairs),
                     contacts=len({r["contact_group"] for r, _ in pairs}),
                     before_pass=sum(old), after_pass=sum(new),
                     before_contacts=len(groups[0]), after_contacts=len(groups[1]),
                     recovered=[p.name for p, a, b in zip(probes, old, new) if b and not a],
                     regressed=[p.name for p, a, b in zip(probes, old, new) if a and not b],
                     precision_recovered=sum(accepted(r) and not accepted(dict(ok=True, best=r["before_precision"])) for r in after),
                     candidate_mean_ms=statistics.mean(r["wall_ms"] for r in after),
                     candidate_max_ms=max(r["wall_ms"] for r in after))
        summary["sessions"].append(entry)
        print(json.dumps(entry, ensure_ascii=False), flush=True)
    private_json(out / "summary.json", summary)


if __name__ == "__main__":
    main()
