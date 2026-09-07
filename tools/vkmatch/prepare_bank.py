#!/usr/bin/env python3
"""Select a compact research gallery from training scores only, never holdout."""
import argparse
import hashlib
import json
import os
from pathlib import Path


def choose_gallery(coverage, quality, limit):
    selected, covered = [], set()
    remaining = set(coverage)
    while remaining and len(selected) < limit:
        # Deterministic tie-breaking, independent of filesystem ordering.
        chosen = min(remaining, key=lambda ref: (
            -len(coverage[ref] - covered), -quality[ref], str(ref)))
        if not coverage[chosen] - covered:
            break
        selected.append(chosen)
        covered.update(coverage[chosen])
        remaining.remove(chosen)
    return selected, covered


def exclusive_write(path, data):
    fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
    with os.fdopen(fd, "wb") as stream:
        stream.write(data)


def prepare(reference_dir, training_dir, output, limit=8):
    reference_dir = reference_dir.resolve(strict=True)
    training_dir = training_dir.resolve(strict=True)
    if reference_dir == training_dir:
        raise ValueError("Reference and training capture sessions must differ")
    label = json.loads((training_dir / "user-label.json").read_text())
    if not label.get("same_finger_throughout") or label.get("detected_contact_count_invalid"):
        raise ValueError("Training provenance is unconfirmed or contact counts invalid")
    records = sorted(training_dir.glob("v3-match-*.json"))
    if not records:
        raise ValueError("No v3 training scores")
    coverage, quality, provenance, all_groups = {}, {}, [], set()
    for path in records:
        data = path.read_bytes()
        row = json.loads(data)
        if not row.get("ok") or row.get("authentication_decision") is not None:
            raise ValueError("Invalid research record")
        probe = Path(row["probe"]).resolve(strict=True)
        if probe.parent != training_dir:
            raise ValueError("Training probe belongs to another session")
        group = row["contact_group"]
        all_groups.add(group)
        provenance.append(dict(path=str(path), sha256=hashlib.sha256(data).hexdigest(),
                               probe_sha256=hashlib.sha256(probe.read_bytes()).hexdigest()))
        for result in row["results"] + [row["best"]]:
            ref = Path(result["reference"]).resolve(strict=True)
            if ref.parent != reference_dir:
                raise ValueError("Training score used another reference bank")
            coverage.setdefault(ref, set())
            quality.setdefault(ref, 0.)
            # Existing interior criteria; no label inferred from UI instructions.
            if result["interior"]["consistent"] and not result["ambiguous"]:
                coverage[ref].add(group)
                quality[ref] += result["evidence_rank"]
    selected, covered = choose_gallery(coverage, quality, limit)
    if not selected:
        raise ValueError("No supported reference candidates")
    # New directory only: never replace an enrollment or previous bank.
    output.mkdir(mode=0o700, parents=False, exist_ok=False)
    entries, hashes = [], set()
    for index, ref in enumerate(selected, 1):
        data = ref.read_bytes()
        digest = hashlib.sha256(data).hexdigest()
        if digest in hashes:
            raise ValueError("Duplicate source content; refusing silently duplicated templates")
        hashes.add(digest)
        filename = f"reference-index-{index:02d}.png"
        exclusive_write(output / filename, data)
        entries.append(dict(file=filename, source=str(ref), sha256=digest,
                            supported_training_groups=sorted(coverage[ref])))
    manifest = dict(schema_version=1, research_only=True, authentication_validated=False,
                    selection="greedy-training-contact-coverage-v1", reference_session=str(reference_dir),
                    training_session=str(training_dir), training_records=provenance,
                    evaluated_training_groups=len(all_groups), covered_training_groups=len(covered),
                    uncovered_training_groups=sorted(all_groups - covered), references=entries,
                    note="No holdout data consulted. Internal consistency is not biometric acceptance. This is not installed in fprintd or PAM.")
    exclusive_write(output / "manifest.json", (json.dumps(manifest, ensure_ascii=False, indent=2)+"\n").encode())
    return dict(output=str(output), selected=len(entries), training_groups=len(all_groups),
                covered=len(covered), uncovered=sorted(all_groups - covered))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reference-dir", required=True, type=Path)
    parser.add_argument("--training-dir", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--limit", type=int, default=8)
    args = parser.parse_args()
    if not 1 <= args.limit <= 64:
        parser.error("limit must be 1..64")
    print(json.dumps(prepare(args.reference_dir, args.training_dir, args.output, args.limit), indent=2))


if __name__ == "__main__":
    main()
