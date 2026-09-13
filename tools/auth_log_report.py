#!/usr/bin/env python3
"""Summarise privacy-safe FPStudio authentication records from journald.

The report never emits usernames, passwords, image/template paths or raw
fingerprint data.  Matcher records do not carry a PAM session ID, so they are
attached to the authentication session whose time window contains them.
"""
from __future__ import annotations

import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shlex
import subprocess
import sys


MARKER = "fpstudio_auth "
SESSION_RE = re.compile(r"^[a-f0-9]+(?:-[a-f0-9]+)+$")
QUALITY_RE = re.compile(r"Image quality: coverage (\d+)%, sharpness (\d+)")
LEGACY_GPU_RE = re.compile(
    r"fpstudio GPU v8 accepted=(\d+) ncc=([0-9.]+) gradient=([0-9.]+) "
    r"overlap=([0-9.]+) roi=([0-9.]+).*?ms=([0-9.]+)"
)
SERVICE_RE = re.compile(r"pam_fpstudio\(([^:()]+):")


def timestamp(record: dict) -> int:
    try:
        return int(record.get("__REALTIME_TIMESTAMP", 0))
    except (TypeError, ValueError):
        return 0


def key_values(text: str) -> dict[str, str]:
    fields: dict[str, str] = {}
    try:
        tokens = shlex.split(text)
    except ValueError:
        return fields
    for token in tokens:
        if "=" not in token:
            continue
        key, value = token.split("=", 1)
        if re.fullmatch(r"[a-z_]+", key):
            fields[key] = value[:160]
    return fields


def parse_record(record: dict) -> dict | None:
    message = record.get("MESSAGE")
    if not isinstance(message, str):
        return None
    at = timestamp(record)
    if MARKER in message:
        fields = key_values(message.split(MARKER, 1)[1])
        event = fields.pop("event", "unknown")
        component = fields.pop("component", "unknown")
        session = fields.pop("session", None)
        service = fields.pop("service", None)
        if session and not SESSION_RE.fullmatch(session):
            session = None
        if not service:
            match = SERVICE_RE.search(message)
            if match:
                service = match.group(1)
        return {"time": at, "session": session, "component": component,
                "event": event, "fields": fields, "service": service}

    quality = QUALITY_RE.search(message)
    if quality:
        return {"time": at, "session": None, "component": "driver",
                "event": "image_quality",
                "fields": {"coverage": quality.group(1), "sharpness": quality.group(2)}}
    legacy = LEGACY_GPU_RE.search(message)
    if legacy:
        return {"time": at, "session": None, "component": "matcher",
                "event": "gpu_metrics", "fields": dict(zip(
                    ("accepted", "ncc", "gradient", "overlap", "roi", "ms"), legacy.groups()))}
    if "GPU attempt failed" in message:
        return {"time": at, "session": None, "component": "matcher",
                "event": "gpu_reinitialise", "fields": {"reason": "helper_failure"}}
    if "GPU unavailable" in message:
        return {"time": at, "session": None, "component": "matcher",
                "event": "gpu_fallback", "fields": {"reason": "helper_unavailable"}}
    if "failed to scan:" in message:
        reason = "device_not_open" if "has not been opened" in message else "scan_failure"
        return {"time": at, "session": None, "component": "driver",
                "event": "driver_error", "fields": {"reason": reason}}
    return None


def correlate(events: list[dict]) -> tuple[list[dict], list[dict]]:
    sessions: dict[str, dict] = {}
    unscoped: list[dict] = []
    for event in sorted(events, key=lambda item: item["time"]):
        session_id = event["session"]
        if session_id:
            session = sessions.setdefault(session_id, {"id": session_id, "events": [],
                                                        "start": event["time"], "end": None,
                                                        "service": None})
            session["events"].append(event)
            session["start"] = min(session["start"], event["time"])
            session["service"] = session["service"] or event.get("service")
            if event["event"] == "request_finished":
                session["end"] = event["time"]
            continue
        unscoped.append(event)

    ordered = sorted(sessions.values(), key=lambda item: item["start"])
    remaining: list[dict] = []
    for event in unscoped:
        candidates = [session for session in ordered
                      if session["start"] - 2_000_000 <= event["time"] <=
                      (session["end"] or session["start"] + 180_000_000) + 2_000_000]
        if candidates:
            nearest = min(candidates, key=lambda item: abs(event["time"] - item["start"]))
            nearest["events"].append(event)
        else:
            remaining.append(event)
    for session in ordered:
        session["events"].sort(key=lambda item: item["time"])
    return ordered, remaining


def diagnose(session: dict) -> list[str]:
    events = session["events"]
    names = [item["event"] for item in events]
    findings: list[str] = []
    if "worker_spawned" not in names:
        skipped = next((item["fields"].get("reason") for item in events
                        if item["event"] == "request_skipped"), None)
        findings.append("module_skipped:" + (skipped or "worker_not_spawned"))
    elif "bus_connected" not in names:
        findings.append("worker_cannot_connect_system_bus")
    elif "enrollment_found" not in names:
        findings.append("no_enrolled_fingerprint_or_device")
    elif "device_claimed" not in names:
        findings.append("sensor_claim_failed_or_busy")
    if "finger_contact" not in names and "worker_spawned" in names:
        findings.append("no_contact_detected")
    if "gpu_reinitialise" in names:
        findings.append("gpu_helper_was_reinitialised")
    if "gpu_fallback" in names:
        findings.append("gpu_failed_and_sigfm_fallback_was_used")
    if any(item["event"] == "driver_error" for item in events):
        reasons = sorted({item["fields"].get("reason", "unknown") for item in events
                          if item["event"] == "driver_error"})
        findings.append("driver_error:" + ",".join(reasons))
    qualities = [item for item in events if item["event"] == "image_quality"]
    if qualities:
        sharpness = [int(item["fields"]["sharpness"]) for item in qualities]
        coverage = [int(item["fields"]["coverage"]) for item in qualities]
        if max(sharpness) < 24:
            findings.append("all_captures_low_sharpness")
        elif max(coverage) >= 60 and max(sharpness) >= 24 and "fingerprint_match" not in names:
            findings.append("good_capture_but_no_match_check_gallery_or_placement")
    if "sensor_ended" in names and "fingerprint_match" not in names:
        findings.append("sensor_ended_without_match")
    if "password_selected" in names:
        findings.append("password_handed_to_system_pam")
    if not any(item["event"] == "request_finished" for item in events):
        findings.append("authentication_session_incomplete")
    return findings


def summarise(sessions: list[dict], unscoped: list[dict]) -> dict:
    output = []
    outcomes = Counter()
    finding_counts = Counter()
    for session in sessions:
        finish = next((item for item in reversed(session["events"])
                       if item["event"] == "request_finished"), None)
        outcome = finish["fields"].get("outcome", "incomplete") if finish else "incomplete"
        outcomes[outcome] += 1
        retries = [int(item["fields"].get("attempt", 0)) for item in session["events"]
                   if item["event"] == "fingerprint_retry"]
        qualities = [{key: int(item["fields"][key]) for key in ("coverage", "sharpness")}
                     for item in session["events"] if item["event"] == "image_quality"]
        findings = diagnose(session)
        finding_counts.update(findings)
        metrics = []
        for item in session["events"]:
            if item["event"] != "gpu_metrics":
                continue
            numeric = {}
            for key in ("accepted", "ambiguous", "boundary", "refined"):
                if key in item["fields"]:
                    try:
                        numeric[key] = int(item["fields"][key])
                    except ValueError:
                        pass
            if "tiles" in item["fields"]:
                try:
                    supported, probe = item["fields"]["tiles"].split("/", 1)
                    numeric["tiles_supported"] = int(supported)
                    numeric["tiles_probe"] = int(probe)
                except ValueError:
                    pass
            for key in ("ncc", "gradient", "overlap", "roi", "ms"):
                if key in item["fields"]:
                    try:
                        numeric[key] = float(item["fields"][key])
                    except ValueError:
                        pass
            metrics.append(numeric)
        output.append({"session": session["id"], "service": session["service"] or "unknown",
                       "duration_ms": ((session["end"] - session["start"]) // 1000
                                       if session["end"] else None),
                       "outcome": outcome,
                       "contacts": sum(item["event"] == "finger_contact" for item in session["events"]),
                       "retries": max(retries, default=0), "qualities": qualities,
                       "gpu_results": [item["fields"].get("result") for item in session["events"]
                                       if item["event"] == "gpu_result"],
                       "gpu_metrics": metrics, "findings": findings})
    return {"session_count": len(output), "outcomes": dict(outcomes),
            "finding_counts": dict(finding_counts), "sessions": output,
            "unscoped_driver_events": len(unscoped)}


def read_records(path: Path | None, since: str) -> list[dict]:
    if path:
        stream = sys.stdin if str(path) == "-" else path.open(encoding="utf-8")
        try:
            return [json.loads(line) for line in stream if line.strip()]
        finally:
            if stream is not sys.stdin:
                stream.close()
    # Filter by the small set of processes that can produce authentication or
    # driver evidence before serialising. Multiple identifier matches are ORed
    # by journalctl and avoid scanning unrelated desktop/application messages.
    identifiers = ("fpstudio-auth", "fpstudio-fprint-worker", "fprintd")
    command = ["journalctl", "--since", since, "--output=json", "--no-pager"]
    for identifier in identifiers:
        command.extend(("--identifier", identifier))
    result = subprocess.run(command,
                            check=False, text=True, capture_output=True)
    if result.returncode:
        raise RuntimeError(result.stderr.strip() or "journalctl failed")
    return [json.loads(line) for line in result.stdout.splitlines() if line.strip()]


def print_human(report: dict) -> None:
    print(f"FPStudio authentication sessions: {report['session_count']}")
    if report["outcomes"]:
        print("Outcomes: " + ", ".join(f"{key}={value}" for key, value in report["outcomes"].items()))
    if report["finding_counts"]:
        print("Repeated findings: " + ", ".join(
            f"{key}={value}" for key, value in
            sorted(report["finding_counts"].items(), key=lambda item: (-item[1], item[0]))))
    for session in report["sessions"]:
        duration = "open" if session["duration_ms"] is None else f"{session['duration_ms']}ms"
        print(f"- {session['session']} service={session['service']} outcome={session['outcome']} "
              f"contacts={session['contacts']} retries={session['retries']} duration={duration}")
        if session["qualities"]:
            values = ", ".join(f"{q['coverage']}%/{q['sharpness']}" for q in session["qualities"])
            print(f"  quality coverage/sharpness: {values}")
        if session["gpu_metrics"]:
            values = ", ".join(
                f"accepted={m.get('accepted', '?')} ncc={m.get('ncc', '?')} "
                f"roi={m.get('roi', '?')} ms={m.get('ms', '?')}"
                for m in session["gpu_metrics"])
            print(f"  GPU metrics: {values}")
        for finding in session["findings"]:
            print(f"  finding: {finding}")
    if report["unscoped_driver_events"]:
        print(f"Unscoped driver events: {report['unscoped_driver_events']}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--since", default="24 hours ago", help="journalctl time expression")
    parser.add_argument("--input", type=Path, help="read journalctl JSON lines from a file or -")
    parser.add_argument("--json", action="store_true", help="print machine-readable JSON")
    args = parser.parse_args()
    try:
        parsed = [event for record in read_records(args.input, args.since)
                  if (event := parse_record(record)) is not None]
        sessions, unscoped = correlate(parsed)
        report = summarise(sessions, unscoped)
    except (OSError, ValueError, json.JSONDecodeError, RuntimeError) as error:
        print(f"fpstudio-auth-report: {error}", file=sys.stderr)
        return 1
    if args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    else:
        print_human(report)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
