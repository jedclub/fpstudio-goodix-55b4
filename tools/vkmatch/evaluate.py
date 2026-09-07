#!/usr/bin/env python3
"""Re-run immutable probes against a fixed bank. Local research, not FAR/FRR."""
import argparse
import json
import os
from pathlib import Path
import statistics
import subprocess


def private_json(path, value):
    # Refuse overwrites: old evidence and biometric records belong to the user.
    fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
    with os.fdopen(fd, "w") as stream:
        json.dump(value, stream, ensure_ascii=False, indent=2)
        stream.write("\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--matcher", type=Path, required=True)
    parser.add_argument("--reference-dir", type=Path, required=True)
    parser.add_argument("--extra-reference-dir", type=Path, action="append", default=[])
    parser.add_argument("--session", type=Path, required=True)
    parser.add_argument("--prefix", default="scale-eval")
    parser.add_argument("--scale-refinement", action="store_true")
    parser.add_argument("--fast-reference-bank", type=Path)
    args = parser.parse_args()
    if not args.prefix or Path(args.prefix).name != args.prefix or args.prefix in (".", ".."):
        parser.error("prefix must be a simple filename prefix")
    session = args.session.resolve(strict=True)
    refs = args.reference_dir.resolve(strict=True)
    extra_refs = [path.resolve(strict=True) for path in args.extra_reference_dir]
    if session in [refs] + extra_refs:
        parser.error("reference bank and probe session must differ")
    records = sorted(session.glob("gpu-match-*.json"))
    if not records:
        parser.error("no recorded probes")
    originals = [json.loads(path.read_text()) for path in records]
    probes = [Path(row["probe"]).resolve(strict=True) for row in originals]
    if any(path.parent != session for path in probes):
        parser.error("probe paths must belong to the selected session")
    destinations = [session / f"{args.prefix}-{i:04d}.json" for i in range(1, len(records) + 1)]
    summary_path = session / f"{args.prefix}-summary.json"
    if any(path.exists() for path in destinations + [summary_path]):
        parser.error("output exists; use a new prefix to preserve prior evidence")
    command = [str(args.matcher.resolve(strict=True)), "--reference-dir", str(refs), "--serve"]
    for directory in extra_refs:
        command += ["--reference-dir", str(directory)]
    if args.scale_refinement:
        command.append("--scale-refinement")
    if args.fast_reference_bank:
        command += ["--fast-reference-bank", str(args.fast_reference_bank.resolve(strict=True))]
    requests = "".join(json.dumps({"probe": str(path)}) + "\n" for path in probes)
    # communicate drains output while sending input; no pipe deadlock, bounded run.
    run = subprocess.run(command, input=requests, text=True, capture_output=True,
                         timeout=60 + 35 * len(records), check=True)
    rows = [json.loads(line) for line in run.stdout.splitlines()]
    if not rows or not rows.pop(0).get("ready") or len(rows) != len(originals):
        raise RuntimeError("Incomplete worker output")
    consistent = lambda row: bool(row.get("ok") and row.get("best", {}).get("interior", {}).get("consistent"))
    recovered, regressed, old_groups, new_groups = [], [], set(), set()
    for before, after, probe, destination in zip(originals, rows, probes, destinations):
        if not after.get("ok") or Path(after.get("probe", "")) != probe:
            raise RuntimeError("Worker error or mismatched probe; no summary accepted")
        if after.get("authentication_decision") is not None:
            raise RuntimeError("Research worker unexpectedly produced identity decision")
        group = before["contact_group"]
        after.update(contact_group=group, sensor_timestamp_us=before.get("sensor_timestamp_us"))
        private_json(destination, after)
        old, new = consistent(before), consistent(after)
        if old: old_groups.add(group)
        if new: new_groups.add(group)
        if new and not old: recovered.append(probe.name)
        if old and not new: regressed.append(probe.name)
    status_path = session / "live-status.json"
    status = json.loads(status_path.read_text()) if status_path.exists() else {}
    summary = dict(research_only=True, authentication_validation=False,
                   note="Repeated development data. Internal pattern consistency is not authentication accuracy. No impostor labels inferred.",
                   source_session=str(session), reference_session=str(refs),
                   extra_reference_sessions=list(map(str, extra_refs)),
                   comparisons=len(rows), detected_contacts=status.get("touches"),
                   evaluated_contacts=len({r["contact_group"] for r in originals}),
                   old_consistent=sum(map(consistent, originals)), new_consistent=sum(map(consistent, rows)),
                   old_consistent_contacts=len(old_groups), new_consistent_contacts=len(new_groups),
                   recovered=recovered, regressed=regressed,
                   pre_scale_consistent=sum(bool(r.get("before_scale", {}).get("interior", {}).get("consistent")) for r in rows),
                   scale_only_recovered=[Path(r["probe"]).name for r in rows if consistent(r) and not r.get("before_scale", {}).get("interior", {}).get("consistent")],
                   mean_ms=statistics.mean(r["wall_ms"] for r in rows),
                   scale_refined=sum(r.get("scale_refined", False) for r in rows))
    summary["full_bank_fallbacks"] = sum(r.get("full_bank_fallback", False) for r in rows)
    private_json(summary_path, summary)
    print(json.dumps(summary, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
