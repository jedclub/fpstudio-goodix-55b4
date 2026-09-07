#!/usr/bin/env python3
"""Install a display-only notice before existing sudo/polkit fingerprint PAM.

Default: read-only diff. --apply requires root but never invokes sudo or PAM.
Existing authentication modules and password fallback are preserved.
Optional --max-tries changes only the fingerprint attempt limit.
"""
import argparse
import difflib
import os
from pathlib import Path
import re
import stat
import tempfile
import time

NOTICE = Path("/etc/security/fpstudio-admin-auth.txt")
SERVICES = ("sudo", "polkit-1")
LINE = f"auth [default=ignore] pam_echo.so file={NOTICE}"
SOURCE = Path(__file__).resolve().parents[1] / "src/fpstudio/pam/admin-auth-notice.txt"


def add_notice(text):
    lines = text.splitlines(keepends=True)
    fingerprint = re.compile(r"^auth\s+(?:\[[^\]]+\]|\S+)\s+(?:/\S*/)?pam_fprintd\.so(?:\s|$)")
    indices = [i for i, line in enumerate(lines) if fingerprint.match(line)]
    if len(indices) != 1:
        raise ValueError("Expected exactly one existing auth pam_fprintd entry; refusing to guess")
    existing = [i for i, line in enumerate(lines) if line.strip() == LINE]
    if existing:
        if len(existing) != 1 or existing[0] != indices[0] - 1:
            raise ValueError("Existing notice is not immediately before pam_fprintd")
        return text
    if any(str(NOTICE) in line and not line.lstrip().startswith("#") for line in lines):
        raise ValueError("Conflicting notice configuration")
    # Numeric PAM jumps refer to positions: adding a module can change them.
    # Refuse such a stack rather than silently change its authentication flow.
    if any(re.search(r"=\d+\b", line.split("#", 1)[0]) for line in lines if "[" in line):
        raise ValueError("PAM numeric jumps need manual review before insertion")
    lines.insert(indices[0], LINE + "\n")
    return "".join(lines)


def read_root_file(path):
    info = path.lstat()
    if not stat.S_ISREG(info.st_mode) or info.st_uid != 0 or info.st_mode & 0o022:
        raise ValueError(f"Refusing symlink/non-root-owned/writable configuration: {path}")
    return path.read_bytes()


def set_retry_limit(text, attempts):
    """Only edit a known sufficient-fingerprint/password-fallback stack."""
    if not 1 <= attempts <= 100:
        raise ValueError("Fingerprint attempt limit must be between 1 and 100")
    lines = text.splitlines(keepends=True)
    if any("\\" in line.split("#", 1)[0] for line in lines):
        raise ValueError("PAM continuations need manual review")
    auth = [(i, line.split("#", 1)[0].split()) for i, line in enumerate(lines)
            if re.match(r"^-?auth\s", line)]
    entries = [(pos, i, fields) for pos, (i, fields) in enumerate(auth)
               if any(Path(field).name == "pam_fprintd.so" for field in fields)]
    if len(entries) != 1:
        raise ValueError("Expected exactly one fingerprint authentication entry")
    pos, index, fields = entries[0]
    if fields[0:2] != ["auth", "sufficient"] or Path(fields[2]).name != "pam_fprintd.so":
        raise ValueError("Fingerprint control must remain sufficient")
    if pos + 1 != len(auth) - 1 or auth[pos + 1][1] != ["auth", "include", "system-auth"]:
        raise ValueError("Expected existing system-auth password fallback immediately after fingerprint")
    body, marker, comment = lines[index].partition("#")
    options = [token for token in fields[3:] if token.startswith("max-tries")]
    if len(options) > 1 or (options and not re.fullmatch(r"max-tries=-?\d+", options[0])):
        raise ValueError("Ambiguous fingerprint attempt options")
    if options:
        body = re.sub(r"(?<!\S)max-tries=-?\d+(?!\S)", f"max-tries={attempts}", body)
    else:
        trailing = body[len(body.rstrip()):]
        body = body.rstrip() + f" max-tries={attempts}" + trailing
    lines[index] = body + marker + comment
    return "".join(lines)


def atomic_write(path, data, mode=0o644):
    fd, temporary = tempfile.mkstemp(prefix=".fpstudio-notice-", dir=path.parent)
    try:
        with os.fdopen(fd, "wb") as stream:
            os.fchmod(stream.fileno(), mode)
            stream.write(data)
            stream.flush()
            os.fsync(stream.fileno())
        os.replace(temporary, path)
    finally:
        if os.path.exists(temporary):
            os.unlink(temporary)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apply", action="store_true", help="apply after backup; root required")
    parser.add_argument("--max-tries", type=int, help="also set fingerprint attempts (1..100); keep password fallback")
    args = parser.parse_args()
    if args.max_tries is not None and not 1 <= args.max_tries <= 100:
        parser.error("--max-tries must be between 1 and 100")
    if args.apply and os.geteuid() != 0:
        parser.error("Root required. No authentication was requested and no files were changed.")
    if not Path("/usr/lib/security/pam_echo.so").is_file():
        parser.error("pam_echo.so is unavailable; nothing changed")
    notice = SOURCE.read_bytes()
    plans = {}
    for service in SERVICES:
        path = Path("/etc/pam.d") / service
        before = read_root_file(path)
        updated = add_notice(before.decode("utf-8"))
        if args.max_tries is not None:
            updated = set_retry_limit(updated, args.max_tries)
        after = updated.encode("utf-8")
        if after != before:
            plans[path] = (before, after, stat.S_IMODE(path.stat().st_mode))
    before_notice = read_root_file(NOTICE) if NOTICE.exists() or NOTICE.is_symlink() else None
    if before_notice != notice:
        plans = {NOTICE: (before_notice, notice, 0o644), **plans}
    for path, (before, after, _) in plans.items():
        print("".join(difflib.unified_diff((before or b"").decode().splitlines(True),
            after.decode().splitlines(True), fromfile=str(path), tofile=str(path))), end="")
    if not args.apply:
        print("Read-only preview. No system settings changed.")
        return
    if not plans:
        print("Requested Admin authentication settings are already installed.")
        return
    # Abort on concurrent edits. Backup is created before the first replacement.
    for path, (before, _, _) in plans.items():
        current = read_root_file(path) if path.exists() or path.is_symlink() else None
        if current != before:
            raise RuntimeError(f"Configuration changed during preflight: {path}")
    backup_root = Path("/var/backups/fpstudio-admin-notice")
    backup_root.mkdir(mode=0o700, exist_ok=True)
    if backup_root.is_symlink() or backup_root.stat().st_uid != 0 or backup_root.stat().st_mode & 0o077:
        raise RuntimeError("Unsafe backup directory")
    backup = Path(tempfile.mkdtemp(prefix=time.strftime("%Y%m%d-%H%M%S-"), dir=backup_root))
    for path, (before, _, _) in plans.items():
        if before is not None:
            atomic_write(backup / path.name, before, 0o600)
    # Message first. Preserve each service's existing password fallback.
    for path, (_, after, mode) in plans.items():
        atomic_write(path, after, mode)
    print(f"Installed Admin notice and requested retry settings. Backups: {backup}")


if __name__ == "__main__":
    main()
