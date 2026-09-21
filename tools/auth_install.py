#!/usr/bin/env python3
"""Preview/install experimental GPU authentication for this Goodix/AMD host.

No enrollment replacement, no firmware writes, no system library overwrite.
Default is a read-only plan. Root --apply backs up and rolls back on failure.
"""
import argparse
import ctypes
import hashlib
import json
import os
from pathlib import Path
import pwd
import re
import stat
import subprocess
import tempfile
from admin_notice import (add_notice, set_fingerprint_limits, atomic_write,
                          read_root_file)

REPO = Path(__file__).resolve().parents[1]
PREFIX = Path("/opt/fpstudio-auth")
GALLERIES = Path("/var/lib/fprint/fpstudio-gpu")
BACKUPS = Path("/var/backups/fpstudio-gpu-auth")
DROPIN = Path("/etc/systemd/system/fprintd.service.d/60-fpstudio-gpu.conf")
# Caps the journal flood a misbehaving driver can produce, kept separate from
# the GPU drop-in above so that rolling the matcher back does not also remove
# it. See src/fpstudio/systemd/ and docs/23-fprintd-watchdog.md.
GUARD_DROPIN = Path("/etc/systemd/system/fprintd.service.d/61-fpstudio-guard.conf")
NOTICE = Path("/etc/security/fpstudio-admin-auth.txt")
STATE = Path("/etc/fpstudio-auth.json")
ENABLED = Path("/etc/fpstudio-auth-users")
MAX_TRIES = 10
FINGERPRINT_TIMEOUT_SECONDS = 60
PAM_REPAIR_BACKUPS = Path("/var/backups/fpstudio-pam-repair")
MATCHER_PROFILE = "auth-v8-contact-anchored-ridge-roi-uniform-scale-5pct-ncc86"


def kde_stack(text, attempts=MAX_TRIES, timeout=FINGERPRINT_TIMEOUT_SECONDS):
    if not 1 <= attempts <= 100:
        raise ValueError("Fingerprint attempt limit must be between 1 and 100")
    if not 5 <= timeout <= 60:
        raise ValueError("Fingerprint timeout must be between 5 and 60 seconds")
    lines = text.splitlines(keepends=True)
    indices = [i for i, line in enumerate(lines)
               if re.match(r"^-?auth\s+required\s+pam_fprintd\.so(?:\s|$)", line)]
    if len(indices) != 1 or sum("pam_fprintd.so" in line.split("#", 1)[0] for line in lines) != 1:
        raise ValueError("Unsupported KDE fingerprint PAM stack")
    i = indices[0]
    body, marker, comment = lines[i].partition("#")
    if any(token.startswith(("max-tries", "timeout")) and not re.fullmatch(r"(?:max-tries|timeout)=-?\d+", token)
           for token in body.split()):
        raise ValueError("Malformed KDE fingerprint options")
    body = re.sub(r"\s+(?:max-tries|timeout)=-?\d+", "", body).rstrip()
    lines[i] = body + f" max-tries={attempts} timeout={timeout}" + (" #" + comment if marker else "\n")
    return "".join(lines)


def verified_kde_password_profile():
    """Confirm that KScreenLocker still has its separate password service.

    `kde-fingerprint` is deliberately non-interactive.  Adding system-auth to
    it would make the fingerprint worker ask for a password and can hide the
    lock-screen password field.  KDE runs this `kde` profile alongside it.
    """
    override = Path("/etc/pam.d/kde")
    path = override if override.exists() or override.is_symlink() else Path("/usr/lib/pam.d/kde")
    text = read_root_file(path).decode("utf-8")
    if not re.search(r"(?m)^auth\s+include\s+system-local-login\s*$", text):
        raise ValueError("KDE password PAM profile is not recognised; refusing to alter fingerprint PAM")


def pam_repair_plans():
    """Prepare a bounded repair for all supported admin fingerprint paths."""
    verified_kde_password_profile()
    plans = {}
    for service in ("sudo", "polkit-1"):
        path = Path("/etc/pam.d") / service
        before = read_root_file(path)
        after = set_fingerprint_limits(add_notice(before.decode("utf-8")), MAX_TRIES,
                                        FINGERPRINT_TIMEOUT_SECONDS).encode()
        if after != before:
            plans[path] = (before, after, stat.S_IMODE(path.stat().st_mode))

    kde = Path("/etc/pam.d/kde-fingerprint")
    kde_source = kde if kde.exists() or kde.is_symlink() else Path("/usr/lib/pam.d/kde-fingerprint")
    before = read_root_file(kde_source)
    after = kde_stack(before.decode("utf-8")).encode()
    if after != before or kde_source != kde:
        plans[kde] = (before if kde_source == kde else None, after,
                      stat.S_IMODE(kde_source.stat().st_mode))

    notice = (REPO / "src/fpstudio/pam/admin-auth-notice.txt").read_bytes()
    before_notice = read_root_file(NOTICE) if NOTICE.exists() or NOTICE.is_symlink() else None
    if before_notice != notice:
        plans = {NOTICE: (before_notice, notice, 0o644), **plans}
    return plans


def repair_pam(apply):
    """Preview or atomically apply the safe PAM timing repair."""
    plans = pam_repair_plans()
    for path, (before, after, _) in plans.items():
        print("".join(__import__("difflib").unified_diff(
            (before or b"").decode().splitlines(True), after.decode().splitlines(True),
            fromfile=str(path), tofile=str(path))), end="")
    if not apply:
        print("Read-only PAM repair preview. No system settings changed.")
        return
    if not plans:
        print("Admin PAM paths already use the safe 10-attempt, 60-second fingerprint window.")
        return
    for path, (before, _, _) in plans.items():
        safe_parent(path)
        current = read_root_file(path) if path.exists() or path.is_symlink() else None
        if current != before:
            raise RuntimeError(f"Configuration changed during preflight: {path}")
    PAM_REPAIR_BACKUPS.mkdir(mode=0o700, parents=True, exist_ok=True)
    info = PAM_REPAIR_BACKUPS.lstat()
    if not stat.S_ISDIR(info.st_mode) or info.st_uid != 0 or info.st_mode & 0o077:
        raise RuntimeError("Unsafe PAM repair backup directory")
    backup = Path(tempfile.mkdtemp(prefix="repair-", dir=PAM_REPAIR_BACKUPS))
    records = []
    for index, (path, (before, _, mode)) in enumerate(plans.items()):
        records.append(dict(path=str(path), existed=before is not None, mode=mode))
        if before is not None:
            atomic_write(backup / f"old-{index:03d}", before, 0o600)
    atomic_write(backup / "transaction.json", json.dumps(records, indent=2).encode(), 0o600)
    for path, (_, after, mode) in plans.items():
        atomic_write(path, after, mode)
    print(f"Repaired sudo, polkit and KDE fingerprint PAM timing. Backups: {backup}")


def source_images(directories):
    images, seen = [], set()
    for directory in directories:
        directory = directory.resolve(strict=True)
        # Never import the failed contacts as "learned" successes by default.
        status = directory / "live-status.json"
        accepted = None
        if status.is_file():
            row = json.loads(status.read_text())
            if row.get("state") != "ended":
                raise ValueError("Finish capture before importing its data")
            if row.get("compared_contacts", 0):
                accepted = {int(c["contact_group"]) for c in row.get("contacts", []) if c.get("consistent")}
        # Two kinds of capture come out of a session: the best frame of each
        # contact, and samples taken along an uninterrupted sweep. Both are
        # captures of this finger; only the first is grouped by contact, so
        # only the first can be filtered by which contacts were consistent.
        sweep = sorted(directory.glob("candidate-sweep-*.png"))
        for path in sorted(directory.glob("candidate-touch-*.png")) + sweep:
            if path.is_symlink() or not path.is_file():
                raise ValueError("Reference must be a regular, non-symlink file")
            number = re.fullmatch(r"candidate-touch-(\d+)\.png", path.name)
            if number is None and not re.fullmatch(r"candidate-sweep-\d+\.png", path.name):
                continue
            if number and accepted is not None and int(number[1]) not in accepted:
                continue
            data = path.read_bytes()
            if len(data) > 1024*1024 or not data.startswith(b"\x89PNG\r\n\x1a\n"):
                raise ValueError("Invalid or oversized PNG reference")
            digest = hashlib.sha256(data).hexdigest()
            if digest not in seen:
                seen.add(digest)
                images.append((path, data, digest))
    if not 1 <= len(images) <= 64:
        raise ValueError("Select 1..64 unique reference images")
    return images


def safe_parent(path):
    """Validate every existing parent; only root may control installed paths."""
    for parent in reversed(path.parents):
        if parent.exists() or parent.is_symlink():
            st = parent.lstat()
            if not stat.S_ISDIR(st.st_mode) or st.st_uid != 0 or st.st_mode & 0o022:
                raise ValueError(f"Unsafe system parent: {parent}")


def enrolled_user(username):
    # Use the public deserializer; do not assume binary template offsets.
    lib = ctypes.CDLL("/usr/lib/libfprint-2.so.2")
    lib.fp_print_deserialize.argtypes = [ctypes.c_void_p, ctypes.c_size_t, ctypes.c_void_p]
    lib.fp_print_deserialize.restype = ctypes.c_void_p
    lib.fp_print_get_username.argtypes = [ctypes.c_void_p]
    lib.fp_print_get_username.restype = ctypes.c_char_p
    objects = ctypes.CDLL("/usr/lib/libgobject-2.0.so.0")
    objects.g_object_unref.argtypes = [ctypes.c_void_p]
    root = Path("/var/lib/fprint") / username / "goodixtls55x4"
    matches = 0
    for path in root.glob("*/*"):
        if path.is_symlink() or not path.is_file():
            continue
        data = read_root_file(path)
        if len(data) > 16*1024*1024:
            raise ValueError("Oversized enrolled template")
        buf = ctypes.create_string_buffer(data)
        obj = lib.fp_print_deserialize(buf, len(data), None)
        if obj:
            try:
                if lib.fp_print_get_username(obj) != username.encode():
                    raise ValueError("Existing template username mismatch; refusing a partial engine switch")
                matches += 1
            finally:
                objects.g_object_unref(obj)
    if not matches:
        raise ValueError("No existing Goodix fprintd enrollment with matching username; enroll through wizard first")


def command(*args):
    return subprocess.run(args, check=True, text=True, capture_output=True, timeout=30)


def experimental_artifacts():
    """Read and validate the exact root-owned artifacts fprintd will load."""
    build = REPO / "src/fpstudio/build"
    if not build.is_dir():
        build = REPO / "build"
    library = REPO / "src/driver/src/libfprint-goodixtls-55x4/build/libfprint/libfprint-2.so.2.0.0"
    if not library.is_file() or library.is_symlink():
        # Installed release bundles deliberately do not carry a second copy
        # of the distribution's libfprint.  Once the wizard has confirmed the
        # patched driver is active, stage the exact regular file backing the
        # system SONAME instead.  Resolve first, then retain the existing
        # regular-file and ELF validation below.
        candidates = [Path("/usr/lib/libfprint-2.so.2"),
                      Path("/usr/lib64/libfprint-2.so.2"),
                      Path("/usr/lib/x86_64-linux-gnu/libfprint-2.so.2")]
        resolved = []
        for candidate in candidates:
            try:
                target = candidate.resolve(strict=True)
            except FileNotFoundError:
                continue
            if target not in resolved:
                resolved.append(target)
        library = next((path for path in resolved
                        if path.is_file() and not path.is_symlink()), library)
    sources = [(build / "fpstudio-auth-match", PREFIX / "bin/fpstudio-auth-match", 0o755),
               (build / "fpstudio-vkmatch", PREFIX / "bin/fpstudio-vkmatch", 0o755),
               (build / "vkmatch/fpstudio-match.spv", PREFIX / "bin/fpstudio-match.spv", 0o644),
               (library, PREFIX / "lib/libfprint-2.so.2", 0o644)]
    artifacts = []
    for source, target, mode in sources:
        if source.is_symlink() or not source.is_file():
            raise ValueError(f"Build artifact missing or symlink: {source}")
        data = source.read_bytes()
        if target == PREFIX / "lib/libfprint-2.so.2" and b"goodixtls55x4" not in data:
            raise ValueError("Active libfprint does not contain the Goodix 55x4 driver")
        artifacts.append((target, data, mode))
    library_data = next(data for target, data, _ in artifacts
                        if target == PREFIX / "lib/libfprint-2.so.2")
    # This fixed event name is both the build marker and the runtime journal
    # contract.  Do not key installation safety to human-readable log text.
    if b"fpstudio_auth component=matcher event=gpu_result" not in library_data:
        raise ValueError("Driver was not built with the GPU bridge")
    return artifacts


def guard_artifacts():
    """The drop-in that limits how much journal a runaway driver can destroy.

    Validated the same way as everything else installed here - read from the
    checkout, never copied from an already-installed path - but kept out of
    experimental_artifacts() because it is not a build product and carries no
    GPU bridge marker for that function to insist on.
    """
    sources = [(REPO / "src/fpstudio/systemd/61-fpstudio-guard.conf",
                GUARD_DROPIN, 0o644)]
    artifacts = []
    for source, target, mode in sources:
        if source.is_symlink() or not source.is_file():
            raise ValueError(f"Guard drop-in missing or symlink: {source}")
        artifacts.append((target, source.read_bytes(), mode))
    return artifacts


def artifact_hashes(artifacts):
    return {str(path.relative_to(PREFIX)): hashlib.sha256(data).hexdigest()
            for path, data, _ in artifacts}


def check_bridge_preconditions(username):
    """Assert what the driver bridge demands before it will run the matcher.

    gpu_auth_bridge.h refuses silently: it returns an error the driver logs as
    a transport failure and then falls back to SIGFM, which scores these images
    zero. A gallery directory installed at 0755 therefore looks exactly like a
    broken GPU rather than a broken install, and stays broken for every
    authentication until someone reads the bridge source. Check it here, where
    the install can still say so.
    """
    bank = GALLERIES / username
    marker = ENABLED / username
    for path, directory in ((bank, True), (marker, False), (PREFIX / "bin/fpstudio-auth-match", False)):
        info = path.lstat()
        kind = stat.S_ISDIR if directory else stat.S_ISREG
        if not kind(info.st_mode) or info.st_uid != 0:
            raise RuntimeError(f"Bridge precondition failed: {path} is not a root-owned "
                               f"{'directory' if directory else 'file'}")
        forbidden = 0o077 if directory else 0o022
        if info.st_mode & forbidden:
            raise RuntimeError(
                f"Bridge precondition failed: {path} is {stat.S_IMODE(info.st_mode):04o}; "
                f"the driver bridge requires no {'group or other' if directory else 'group or other write'} "
                f"bits, and will fall back to a matcher that rejects every capture")


def run_gpu_self_test(username):
    """Exercise only the installed transport with a reference self-comparison."""
    result = command("/usr/bin/systemd-run", "--quiet", "--wait", "--pipe", "--collect",
        f"--unit=fpstudio-gpu-selftest-{os.getpid()}",
        "-p", "ProtectSystem=strict", "-p", "ProtectHome=true", "-p", "PrivateTmp=true",
        "-p", "MemoryDenyWriteExecute=true", "-p", "NoNewPrivileges=true",
        "-p", "RestrictAddressFamilies=AF_UNIX AF_LOCAL AF_NETLINK",
        "-p", "SystemCallFilter=@system-service", "-p", "DevicePolicy=closed",
        "-p", "DeviceAllow=char-drm rw",
        str(PREFIX / "bin/fpstudio-auth-match"), "--self-test", username)
    if result.stdout != "FPSTUDIO_GPU_MATCH_V1\n":
        raise RuntimeError("GPU transport self-test failed")
    check_bridge_preconditions(username)


def installed_gpu_state():
    raw = read_root_file(STATE)
    try:
        state = json.loads(raw)
    except json.JSONDecodeError as error:
        raise ValueError("Installed FPStudio state is not valid JSON") from error
    username = state.get("username")
    if (not state.get("installed") or not isinstance(username, str) or
            not re.fullmatch(r"[a-z_][a-z0-9_-]{0,31}", username)):
        raise ValueError("No supported installed FPStudio authentication state to refresh")
    account = pwd.getpwnam(username)
    if account.pw_uid == 0:
        raise ValueError("Refusing to refresh an authentication configuration for root")
    marker = ENABLED / username
    if read_root_file(marker) != b"experimental-gpu-v1\n":
        raise ValueError("Experimental authentication marker is missing or changed")
    dropin = read_root_file(DROPIN)
    if b"LD_LIBRARY_PATH=/opt/fpstudio-auth/lib" not in dropin:
        raise ValueError("fprintd is not configured to load the FPStudio library")
    return state, username


def refresh_experimental_auth(apply):
    """Refresh binaries only; preserve gallery, enrollment and PAM settings."""
    state, username = installed_gpu_state()
    artifacts = experimental_artifacts()
    plans = {}
    # The guard drop-in ships alongside the matcher, so a refresh that left an
    # old copy behind would quietly keep an old rate limit. It may not be there
    # at all on a system installed before it existed, so absence is a normal
    # starting state here rather than an error - `before` is then None, and the
    # rollback record below remembers that the file has to be removed, not
    # restored.
    for path, data, mode in artifacts + guard_artifacts():
        before = read_root_file(path) if path.exists() or path.is_symlink() else None
        if before != data:
            plans[path] = (before, data, mode)
    refreshed = dict(state)
    refreshed["artifact_sha256"] = artifact_hashes(artifacts)
    refreshed["matcher_profile"] = MATCHER_PROFILE
    state_data = json.dumps(refreshed, ensure_ascii=False, indent=2).encode()
    before_state = read_root_file(STATE)
    if state_data != before_state:
        plans[STATE] = (before_state, state_data, stat.S_IMODE(STATE.stat().st_mode))
    for path, (before, after, _) in plans.items():
        if path == STATE:
            print("FPStudio state will record the deployed matcher profile and artifact hashes.")
        elif before is None:
            print(f"{path}: (new) -> {hashlib.sha256(after).hexdigest()[:12]}")
        else:
            print(f"{path}: {hashlib.sha256(before).hexdigest()[:12]} -> "
                  f"{hashlib.sha256(after).hexdigest()[:12]}")
    if not apply:
        print("Read-only GPU refresh preview. Gallery, enrollment and PAM settings are unchanged.")
        return
    if not plans:
        print("Installed GPU matcher already matches this build.")
        return
    for path, (before, _, _) in plans.items():
        safe_parent(path)
        current = read_root_file(path) if path.exists() or path.is_symlink() else None
        if current != before:
            raise RuntimeError(f"Configuration changed during preflight: {path}")
    BACKUPS.mkdir(mode=0o700, parents=True, exist_ok=True)
    backup_info = BACKUPS.lstat()
    if not stat.S_ISDIR(backup_info.st_mode) or backup_info.st_uid != 0 or backup_info.st_mode & 0o077:
        raise RuntimeError("Unsafe GPU authentication backup directory")
    backup = Path(tempfile.mkdtemp(prefix="refresh-", dir=BACKUPS))
    records = []
    for index, (path, (before, _, mode)) in enumerate(plans.items()):
        records.append(dict(path=str(path), existed=before is not None, mode=mode))
        if before is not None:
            atomic_write(backup / f"old-{index:03d}", before, 0o600)
    atomic_write(backup / "transaction.json", json.dumps(records).encode(), 0o600)
    command("/usr/bin/systemctl", "stop", "fprintd.service")
    try:
        for path, (_, after, mode) in plans.items():
            ensure_parent(path)
            safe_parent(path)
            atomic_write(path, after, mode)
        command("/usr/bin/systemctl", "daemon-reload")
        command("/usr/bin/systemctl", "start", "fprintd.service")
        run_gpu_self_test(username)
    except Exception:
        restore(backup)
        raise
    print(f"Refreshed GPU matcher only. Gallery, enrollment and PAM were preserved. Backup: {backup}")


def ensure_parent(path):
    """Create the directory a target lives in, and enforce its mode.

    mkdir(mode=...) only applies the mode when it creates the directory, so a
    gallery directory left over from an earlier install keeps whatever mode it
    had. That is not cosmetic: the driver bridge refuses a gallery directory
    with any group or other bit set (gpu_auth_bridge.h), so a 0755 directory
    surviving here silently disables GPU matching for every authentication and
    falls back to a matcher that cannot score these images at all. Set the mode
    explicitly on the way through.
    """
    gallery = GALLERIES in path.parents
    mode = 0o700 if gallery else 0o755
    path.parent.mkdir(parents=True, mode=mode, exist_ok=True)
    if gallery:
        os.chmod(path.parent, mode)
        GALLERIES.mkdir(parents=True, mode=0o755, exist_ok=True)


def refresh_gallery(username, directories, apply):
    """Replace only the enrolled gallery; preserve binaries, PAM and enrollment.

    The full install rewrites the PAM stacks, which is wrong once a different
    tool owns them, and the binary refresh deliberately will not touch the
    gallery. Re-enrolling had no path between the two, so the only way to
    correct a bad gallery was a rollback followed by a fresh install of
    everything. That is a large, risky operation to fix a set of images.

    References that are no longer part of the gallery are removed rather than
    left behind: they are biometric data, and a stale file that no manifest
    lists is data kept for no reason.
    """
    state, installed = installed_gpu_state()
    if username != installed:
        raise ValueError(f"Installed gallery belongs to {installed!r}, not {username!r}")
    account = pwd.getpwnam(username)
    images = source_images(directories)
    if not images or len(images) > 64:
        raise ValueError("Need 1..64 reference images")
    bank = GALLERIES / username
    plans, entries = {}, []
    for index, (source, data, digest) in enumerate(images):
        name = f"reference-index-{index:03d}.png"
        entries.append(dict(file=name, sha256=digest, source=str(source)))
        path = bank / name
        before = read_root_file(path) if path.exists() or path.is_symlink() else None
        if before != data:
            plans[path] = (before, data, 0o600)
    keep = {entry["file"] for entry in entries}
    for path in sorted(bank.glob("reference-index-*.png")):
        if path.name not in keep:
            plans[path] = (read_root_file(path), None, 0o600)
    config = dict(schema_version=1, username=username, uid=account.pw_uid,
                  policy="experimental-gpu-v1", experimental_auth_enabled=True,
                  biometric_security_validated=False, matcher_profile=MATCHER_PROFILE,
                  references=entries)
    manifest = bank / "manifest.json"
    manifest_data = json.dumps(config, ensure_ascii=False, indent=2).encode()
    before = read_root_file(manifest) if manifest.exists() or manifest.is_symlink() else None
    if before != manifest_data:
        plans[manifest] = (before, manifest_data, 0o600)
    refreshed = dict(state)
    refreshed["references"] = len(entries)
    refreshed["matcher_profile"] = MATCHER_PROFILE
    state_data = json.dumps(refreshed, ensure_ascii=False, indent=2).encode()
    before_state = read_root_file(STATE)
    if state_data != before_state:
        plans[STATE] = (before_state, state_data, stat.S_IMODE(STATE.stat().st_mode))

    print(json.dumps(dict(user=username, references=len(entries),
        replaced=sum(1 for _, after, _ in plans.values() if after is not None),
        removed=sum(1 for _, after, _ in plans.values() if after is None),
        matcher_profile=MATCHER_PROFILE,
        preserves_enrollment=True, preserves_pam=True, preserves_binaries=True,
        biometric_security_validated=False), ensure_ascii=False, indent=2))
    if not apply:
        print("Read-only gallery preview. Nothing was changed.")
        return
    if not plans:
        print("Gallery already matches these references.")
        return

    BACKUPS.mkdir(mode=0o700, parents=True, exist_ok=True)
    backup_info = BACKUPS.lstat()
    if not stat.S_ISDIR(backup_info.st_mode) or backup_info.st_uid != 0 or backup_info.st_mode & 0o077:
        raise RuntimeError("Unsafe GPU authentication backup directory")
    backup = Path(tempfile.mkdtemp(prefix="gallery-", dir=BACKUPS))
    records = []
    for index, (path, (before, _, mode)) in enumerate(plans.items()):
        records.append(dict(path=str(path), existed=before is not None, mode=mode))
        if before is not None:
            atomic_write(backup / f"old-{index:03d}", before, 0o600)
    atomic_write(backup / "transaction.json", json.dumps(records).encode(), 0o600)
    command("/usr/bin/systemctl", "stop", "fprintd.service")
    try:
        for path, (_, after, mode) in plans.items():
            ensure_parent(path)
            safe_parent(path)
            if after is None:
                os.unlink(path)
            else:
                atomic_write(path, after, mode)
        command("/usr/bin/systemctl", "start", "fprintd.service")
        run_gpu_self_test(username)
    except Exception:
        restore(backup)
        raise
    print(f"Replaced the gallery with {len(entries)} references. Enrollment, PAM and "
          f"binaries were preserved. Backup: {backup}")


def restore(backup):
    backup = backup.resolve(strict=True)
    if backup.parent != BACKUPS:
        raise ValueError("Rollback must name an exact FPStudio backup directory")
    records = json.loads(read_root_file(backup / "transaction.json"))
    command("/usr/bin/systemctl", "stop", "fprintd.service")
    for index, record in reversed(list(enumerate(records))):
        path = Path(record["path"])
        if not (path in (DROPIN, GUARD_DROPIN, NOTICE, STATE)
                or path in [Path("/etc/pam.d") / s for s in ("sudo", "polkit-1", "kde-fingerprint")]
                or PREFIX in path.parents or GALLERIES in path.parents or path.parent == ENABLED):
            raise ValueError("Unexpected rollback target")
        safe_parent(path)
        if path.exists() or path.is_symlink():
            read_root_file(path)
            # Retain removed experimental files, including imported biometrics.
            os.replace(path, backup / f"removed-{index:03d}")
        if record["existed"]:
            atomic_write(path, read_root_file(backup / f"old-{index:03d}"), record["mode"])
    command("/usr/bin/systemctl", "daemon-reload")
    command("/usr/bin/systemctl", "start", "fprintd.service")
    print(f"Restored original settings. Removed experimental files are recoverable in {backup}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--user")
    parser.add_argument("--reference-dir", action="append", type=Path, default=[])
    parser.add_argument("--enable-experimental-auth", action="store_true")
    parser.add_argument("--repair-pam", action="store_true",
                        help="repair only sudo, polkit and KDE PAM timing; no gallery import")
    parser.add_argument("--refresh-experimental-auth", action="store_true",
                        help="refresh only installed GPU matcher binaries; preserve gallery, enrollment and PAM")
    parser.add_argument("--refresh-gallery", action="store_true",
                        help="replace only the enrolled gallery; preserve binaries, enrollment and PAM")
    parser.add_argument("--apply", action="store_true")
    parser.add_argument("--rollback", type=Path)
    args = parser.parse_args()
    if (args.apply or args.rollback) and os.geteuid() != 0:
        parser.error("Root required; no authentication dialog or system change was attempted")
    if args.repair_pam:
        if (args.rollback or args.refresh_experimental_auth or args.user or args.reference_dir or
                args.enable_experimental_auth):
            parser.error("--repair-pam cannot be combined with gallery installation options")
        repair_pam(args.apply)
        return
    if args.refresh_gallery:
        if args.rollback or args.refresh_experimental_auth or not args.user or not args.reference_dir:
            parser.error("--refresh-gallery needs --user and --reference-dir, and no other mode")
        if args.apply and not args.enable_experimental_auth:
            parser.error("This matcher is not FAR validated; explicit --enable-experimental-auth required")
        refresh_gallery(args.user, args.reference_dir, args.apply)
        return
    if args.refresh_experimental_auth:
        if args.rollback or args.user or args.reference_dir or args.enable_experimental_auth:
            parser.error("--refresh-experimental-auth cannot be combined with gallery installation options")
        refresh_experimental_auth(args.apply)
        return
    if args.rollback:
        restore(args.rollback)
        return
    if not args.user or not re.fullmatch(r"[a-z_][a-z0-9_-]{0,31}", args.user):
        parser.error("Explicit local --user required")
    account = pwd.getpwnam(args.user)
    if account.pw_uid == 0:
        parser.error("Do not import a gallery for root")
    if args.apply and not args.enable_experimental_auth:
        parser.error("This matcher is not FAR validated; explicit --enable-experimental-auth required")
    images = source_images(args.reference_dir)
    plans = {}
    artifacts = experimental_artifacts()
    for target, data, mode in artifacts:
        plans[target] = (data, mode)
    bank = GALLERIES / args.user
    entries = []
    for index, (source, data, digest) in enumerate(images):
        name = f"reference-index-{index:03d}.png"
        plans[bank / name] = (data, 0o600)
        entries.append(dict(file=name, sha256=digest, source=str(source)))
    config = dict(schema_version=1, username=args.user, uid=account.pw_uid,
                  policy="experimental-gpu-v1", experimental_auth_enabled=True,
                  biometric_security_validated=False, matcher_profile=MATCHER_PROFILE,
                  references=entries)
    plans[bank / "manifest.json"] = (json.dumps(config, ensure_ascii=False, indent=2).encode(), 0o600)
    plans[ENABLED / args.user] = (b"experimental-gpu-v1\n", 0o644)
    for service in ("sudo", "polkit-1"):
        path = Path("/etc/pam.d") / service
        plans[path] = (set_fingerprint_limits(add_notice(read_root_file(path).decode()), MAX_TRIES,
                                               FINGERPRINT_TIMEOUT_SECONDS).encode(), 0o644)
    kde = Path("/etc/pam.d/kde-fingerprint")
    kde_source = kde if kde.exists() or kde.is_symlink() else Path("/usr/lib/pam.d/kde-fingerprint")
    plans[kde] = (kde_stack(read_root_file(kde_source).decode()).encode(), 0o644)
    plans[NOTICE] = ((REPO / "src/fpstudio/pam/admin-auth-notice.txt").read_bytes(), 0o644)
    plans[DROPIN] = (b"[Service]\nEnvironment=LD_LIBRARY_PATH=/opt/fpstudio-auth/lib\nDeviceAllow=char-drm rw\n", 0o644)
    for target, data, mode in guard_artifacts():
        plans[target] = (data, mode)
    print(json.dumps(dict(experimental=True, username=args.user, references=len(entries),
        services=["sudo", "polkit-1", "kde-fingerprint"], max_tries=MAX_TRIES,
        fingerprint_timeout_seconds=FINGERPRINT_TIMEOUT_SECONDS,
        preserves_enrollment=True, firmware_write=False,
        targets=list(map(str, plans)), root_preflight_pending=os.geteuid()!=0), ensure_ascii=False, indent=2))
    if not args.apply:
        print("Read-only plan. No system settings changed.")
        return
    enrolled_user(args.user)
    # Account data must exist before enabling PAM; validate graphics dependency too.
    read_root_file(Path("/usr/share/vulkan/icd.d/radeon_icd.json"))
    if STATE.exists():
        raise ValueError("An installation exists; use its rollback before replacing this experimental version")
    for path in [*plans, STATE, BACKUPS / "placeholder"]:
        safe_parent(path)
        if path.exists() or path.is_symlink():
            read_root_file(path)
    BACKUPS.mkdir(mode=0o700, parents=True, exist_ok=True)
    if BACKUPS.stat().st_mode & 0o077:
        raise ValueError("Unsafe backup permissions")
    backup = Path(tempfile.mkdtemp(prefix="install-", dir=BACKUPS))
    state = dict(schema_version=1, username=args.user, references=len(entries),
                 policy="experimental-gpu-v1", backup=str(backup), max_tries=MAX_TRIES,
                 fingerprint_timeout_seconds=FINGERPRINT_TIMEOUT_SECONDS,
                 artifact_sha256=artifact_hashes(artifacts), matcher_profile=MATCHER_PROFILE,
                 installed=True, biometric_security_validated=False)
    plans[STATE] = (json.dumps(state, indent=2).encode(), 0o644)
    records = []
    for index, (path, _) in enumerate(plans.items()):
        existed = path.exists()
        mode = stat.S_IMODE(path.stat().st_mode) if existed else 0o644
        records.append(dict(path=str(path), existed=existed, mode=mode))
        if existed:
            atomic_write(backup / f"old-{index:03d}", read_root_file(path), 0o600)
    atomic_write(backup / "transaction.json", json.dumps(records).encode(), 0o600)
    command("/usr/bin/systemctl", "stop", "fprintd.service")
    try:
        for path, (data, mode) in plans.items():
            ensure_parent(path)
            safe_parent(path)
            atomic_write(path, data, mode)
        command("/usr/bin/systemctl", "daemon-reload")
        command("/usr/bin/systemctl", "start", "fprintd.service")
        # Reference self-comparison checks transport/GPU only, not biometric security.
        run_gpu_self_test(args.user)
        command("/usr/bin/busctl", "--system", "call", "net.reactivated.Fprint",
                "/net/reactivated/Fprint/Manager", "net.reactivated.Fprint.Manager", "GetDevices")
    except Exception:
        restore(backup)
        raise
    print(f"Installed experimental GPU path; real sudo/KDE authentication still requires user verification. Backup: {backup}")


if __name__ == "__main__":
    main()
