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
from admin_notice import add_notice, set_retry_limit, atomic_write, read_root_file

REPO = Path(__file__).resolve().parents[1]
PREFIX = Path("/opt/fpstudio-auth")
GALLERIES = Path("/var/lib/fprint/fpstudio-gpu")
BACKUPS = Path("/var/backups/fpstudio-gpu-auth")
DROPIN = Path("/etc/systemd/system/fprintd.service.d/60-fpstudio-gpu.conf")
NOTICE = Path("/etc/security/fpstudio-admin-auth.txt")
STATE = Path("/etc/fpstudio-auth.json")
ENABLED = Path("/etc/fpstudio-auth-users")


def kde_stack(text):
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
    lines[i] = body + " max-tries=10 timeout=90" + (" #" + comment if marker else "\n")
    return "".join(lines)


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
        for path in sorted(directory.glob("candidate-touch-*.png")):
            if path.is_symlink() or not path.is_file():
                raise ValueError("Reference must be a regular, non-symlink file")
            number = re.fullmatch(r"candidate-touch-(\d+)\.png", path.name)
            if not number or (accepted is not None and int(number[1]) not in accepted):
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


def restore(backup):
    backup = backup.resolve(strict=True)
    if backup.parent != BACKUPS:
        raise ValueError("Rollback must name an exact FPStudio backup directory")
    records = json.loads(read_root_file(backup / "transaction.json"))
    command("/usr/bin/systemctl", "stop", "fprintd.service")
    for index, record in reversed(list(enumerate(records))):
        path = Path(record["path"])
        if not (path in (DROPIN, NOTICE, STATE) or path in [Path("/etc/pam.d") / s for s in ("sudo", "polkit-1", "kde-fingerprint")]
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
    print(f"Restored original settings. Removed experimental files are recoverable in {backup}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--user")
    parser.add_argument("--reference-dir", action="append", type=Path, default=[])
    parser.add_argument("--enable-experimental-auth", action="store_true")
    parser.add_argument("--apply", action="store_true")
    parser.add_argument("--rollback", type=Path)
    args = parser.parse_args()
    if (args.apply or args.rollback) and os.geteuid() != 0:
        parser.error("Root required; no authentication dialog or system change was attempted")
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
    build = REPO / "src/fpstudio/build"
    library = REPO / "src/driver/src/libfprint-goodixtls-55x4/build/libfprint/libfprint-2.so.2.0.0"
    plans = {}
    for source, target, mode in [(build / "fpstudio-auth-match", PREFIX / "bin/fpstudio-auth-match", 0o755),
            (build / "fpstudio-vkmatch", PREFIX / "bin/fpstudio-vkmatch", 0o755),
            (build / "vkmatch/fpstudio-match.spv", PREFIX / "bin/fpstudio-match.spv", 0o644),
            (library, PREFIX / "lib/libfprint-2.so.2", 0o644)]:
        if source.is_symlink() or not source.is_file():
            raise ValueError(f"Build artifact missing or symlink: {source}")
        plans[target] = (source.read_bytes(), mode)
    if b"fpstudio GPU authentication result" not in plans[PREFIX / "lib/libfprint-2.so.2"][0]:
        raise ValueError("Driver was not built with the GPU bridge")
    bank = GALLERIES / args.user
    entries = []
    for index, (source, data, digest) in enumerate(images):
        name = f"reference-index-{index:03d}.png"
        plans[bank / name] = (data, 0o600)
        entries.append(dict(file=name, sha256=digest, source=str(source)))
    config = dict(schema_version=1, username=args.user, uid=account.pw_uid,
                  policy="experimental-gpu-v1", experimental_auth_enabled=True,
                  biometric_security_validated=False, references=entries)
    plans[bank / "manifest.json"] = (json.dumps(config, ensure_ascii=False, indent=2).encode(), 0o600)
    plans[ENABLED / args.user] = (b"experimental-gpu-v1\n", 0o644)
    for service in ("sudo", "polkit-1"):
        path = Path("/etc/pam.d") / service
        plans[path] = (set_retry_limit(add_notice(read_root_file(path).decode()), 10).encode(), 0o644)
    kde = Path("/etc/pam.d/kde-fingerprint")
    kde_source = kde if kde.exists() or kde.is_symlink() else Path("/usr/lib/pam.d/kde-fingerprint")
    plans[kde] = (kde_stack(read_root_file(kde_source).decode()).encode(), 0o644)
    plans[NOTICE] = ((REPO / "src/fpstudio/pam/admin-auth-notice.txt").read_bytes(), 0o644)
    plans[DROPIN] = (b"[Service]\nEnvironment=LD_LIBRARY_PATH=/opt/fpstudio-auth/lib\nDeviceAllow=char-drm rw\n", 0o644)
    print(json.dumps(dict(experimental=True, username=args.user, references=len(entries),
        services=["sudo", "polkit-1", "kde-fingerprint"], max_tries=10,
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
                 policy="experimental-gpu-v1", backup=str(backup), max_tries=10,
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
            path.parent.mkdir(parents=True, mode=0o700 if GALLERIES in path.parents else 0o755, exist_ok=True)
            safe_parent(path)
            atomic_write(path, data, mode)
        # Reference self-comparison checks transport/GPU only, not biometric security.
        result = command("/usr/bin/systemd-run", "--quiet", "--wait", "--pipe", "--collect",
            f"--unit=fpstudio-gpu-selftest-{os.getpid()}",
            "-p", "ProtectSystem=strict", "-p", "ProtectHome=true", "-p", "PrivateTmp=true",
            "-p", "MemoryDenyWriteExecute=true", "-p", "NoNewPrivileges=true",
            "-p", "RestrictAddressFamilies=AF_UNIX AF_LOCAL AF_NETLINK",
            "-p", "SystemCallFilter=@system-service", "-p", "DevicePolicy=closed",
            "-p", "DeviceAllow=char-drm rw",
            str(PREFIX / "bin/fpstudio-auth-match"), "--self-test", args.user)
        if result.stdout != "FPSTUDIO_GPU_MATCH_V1\n":
            raise RuntimeError("GPU transport self-test failed")
        command("/usr/bin/systemctl", "daemon-reload")
        command("/usr/bin/systemctl", "start", "fprintd.service")
        command("/usr/bin/busctl", "--system", "call", "net.reactivated.Fprint",
                "/net/reactivated/Fprint/Manager", "net.reactivated.Fprint.Manager", "GetDevices")
    except Exception:
        restore(backup)
        raise
    print(f"Installed experimental GPU path; real sudo/KDE authentication still requires user verification. Backup: {backup}")


if __name__ == "__main__":
    main()
