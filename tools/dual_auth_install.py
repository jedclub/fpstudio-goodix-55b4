#!/usr/bin/env python3
"""Install simultaneous password/fingerprint input on supported local PAM paths.

Default is a read-only diff. Build src/pam in build-pam first. --apply requires
an ordinary administrator authentication; this tool never obtains credentials.
"""
import argparse
import difflib
import json
import os
from pathlib import Path
import re
import stat
import tempfile

from admin_notice import LINE as OLD_NOTICE_LINE, atomic_write, read_root_file, set_fingerprint_limits
from auth_install import REPO, safe_parent, verified_kde_password_profile

MODULE = Path('/opt/fpstudio-auth/lib/pam_fpstudio.so')
WORKER = Path('/opt/fpstudio-auth/bin/fpstudio-fprint-worker')
BACKUPS = Path('/var/backups/fpstudio-dual-auth')
SERVICES = ('sudo', 'sudo-i', 'polkit-1', 'login', 'su', 'su-l', 'kde-fingerprint')
TARGETS = {MODULE, WORKER, *(Path('/etc/pam.d') / s for s in SERVICES)}
DUAL = f'auth sufficient {MODULE}\n'
LOCK = 'auth requisite pam_faillock.so preauth\n'


def auth_entries(text):
    if any('\\' in line.split('#', 1)[0] for line in text.splitlines()):
        raise ValueError('PAM continuations require manual review')
    return [line.split('#', 1)[0].split() for line in text.splitlines()
            if re.match(r'^-?auth\s', line)]


def dual_stack(text, service):
    # The old display-only banner describes serial password fallback. Our
    # module supplies the localized, simultaneous-input notice instead.
    text = ''.join(line for line in text.splitlines(keepends=True)
                   if line.strip() != OLD_NOTICE_LINE)
    entries = auth_entries(text)
    if str(MODULE) in text:
        expected = (f'-auth required {MODULE} fingerprint-only' if service == 'kde-fingerprint'
                    else DUAL.strip())
        active = [' '.join(fields) for fields in entries]
        if active.count(expected) != 1 or any('pam_fprintd.so' in x for x in active):
            raise ValueError('Conflicting simultaneous fingerprint configuration')
        if service != 'kde-fingerprint' and 'auth requisite pam_faillock.so preauth' not in active:
            raise ValueError('Missing account lockout precheck')
        return text
    if service == 'kde-fingerprint':
        matches = [e for e in entries if 'pam_fprintd.so' in e]
        if len(matches) != 1 or matches[0][:3] != ['-auth', 'required', 'pam_fprintd.so']:
            raise ValueError('Unsupported KDE fingerprint service')
        return re.sub(r'(?m)^-auth\s+required\s+pam_fprintd\.so[^\n]*',
                      f'-auth required {MODULE} fingerprint-only', text)
    if service in ('sudo', 'polkit-1'):
        if any('pam_fprintd.so' in e for e in entries):
            # Existing helper validates sufficient control and the immediate
            # system-auth password fallback before we replace the module.
            set_fingerprint_limits(text, 10, 30)
            text = re.sub(r'(?m)^auth\s+sufficient\s+pam_fprintd\.so[^\n]*\n', '', text)
        remaining = auth_entries(text)
        if [e for e in remaining if 'pam_echo.so' not in e] != [['auth', 'include', 'system-auth']]:
            raise ValueError(f'Unsupported {service} authentication stack')
        return re.sub(r'(?m)^(auth\s+include\s+system-auth\s*)$',
                      lambda m: LOCK + DUAL + m[1], text)
    if service == 'login':
        if entries != [['auth', 'requisite', 'pam_nologin.so'],
                       ['auth', 'include', 'system-local-login']]:
            raise ValueError('Unsupported local console login stack')
        return re.sub(r'(?m)^(auth\s+include\s+system-local-login\s*)$',
                      lambda m: 'auth required pam_shells.so\n' + LOCK + DUAL + m[1], text)
    if service in ('su', 'su-l'):
        if entries != [['auth', 'sufficient', 'pam_rootok.so'], ['auth', 'required', 'pam_unix.so']]:
            raise ValueError(f'Unsupported {service} authentication stack')
        return re.sub(r'(?m)^auth\s+required\s+pam_unix\.so\s*$',
                      LOCK + DUAL + 'auth required pam_unix.so try_first_pass\n', text)
    raise ValueError(f'Unknown authentication service: {service}')


def make_plans():
    verified_kde_password_profile()
    plans = {}
    build = REPO / 'src/fpstudio/build/pam'
    if not build.is_dir():
        build = REPO / 'build/pam'
    if not build.is_dir():
        build = REPO / 'build-pam'
    for source, target in ((build / 'pam_fpstudio.so', MODULE),
                           (build / 'fpstudio-fprint-worker', WORKER)):
        if source.is_symlink() or not source.is_file():
            raise ValueError(f'Build the production PAM target first: {source}')
        data = source.read_bytes()
        if not data.startswith(b'\x7fELF') or b'synthetic-test-password' in data or b'fake-fprint-worker' in data:
            raise ValueError('Refusing invalid or test authentication artifact')
        if target == MODULE and str(WORKER).encode() not in data:
            raise ValueError('PAM module does not use the installed production sensor worker')
        before = read_root_file(target) if target.exists() or target.is_symlink() else None
        plans[target] = (before, data, 0o755)
    for service in SERVICES:
        path = Path('/etc/pam.d') / service
        if service == 'sudo-i' and not path.exists() and not path.is_symlink():
            after = b'#%PAM-1.0\nauth include sudo\naccount include sudo\nsession include sudo\n'
            plans[path] = (None, after, 0o644)
            continue
        if service == 'sudo-i':
            before = read_root_file(path)
            if auth_entries(before.decode()) != [['auth', 'include', 'sudo']]:
                raise ValueError('Custom sudo-i PAM profile needs review')
            continue
        source = path if path.exists() or path.is_symlink() else Path('/usr/lib/pam.d') / service
        original = read_root_file(source)
        after = dual_stack(original.decode(), service).encode()
        plans[path] = (original if source == path else None, after, 0o644)
    return {path: record for path, record in plans.items() if record[0] != record[1]}


def rollback(backup):
    backup = backup.resolve(strict=True)
    if backup.parent != BACKUPS:
        raise ValueError('Expected an exact simultaneous-auth backup directory')
    safe_parent(backup / 'transaction.json')
    records = json.loads(read_root_file(backup / 'transaction.json'))
    if any(Path(r['path']) not in TARGETS for r in records):
        raise ValueError('Unexpected rollback target')
    # Restore PAM first, then remove unused module/worker files recoverably.
    for i in reversed(range(len(records))):
        record = records[i]; path = Path(record['path'])
        safe_parent(path)
        if record['existed']:
            atomic_write(path, read_root_file(backup / f'old-{i:03d}'), record['mode'])
        elif path.exists():
            read_root_file(path)
            os.replace(path, backup / f'removed-{i:03d}')
    print(f'Restored authentication settings; retained replaced files in {backup}')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply', action='store_true')
    parser.add_argument('--rollback', type=Path)
    args = parser.parse_args()
    if (args.apply or args.rollback) and os.geteuid() != 0:
        parser.error('Administrator authentication is required to change system PAM')
    if args.rollback:
        rollback(args.rollback); return
    plans = make_plans()
    for path, (before, after, _) in plans.items():
        if path in (MODULE, WORKER):
            print(f'Install {path} ({len(after)} bytes)')
        else:
            print(''.join(difflib.unified_diff((before or b'').decode().splitlines(True),
                       after.decode().splitlines(True), fromfile=str(path), tofile=str(path))), end='')
    if not args.apply:
        print('Read-only preview; system settings unchanged.'); return
    if not plans:
        print('Simultaneous authentication already installed.'); return
    safe_parent(BACKUPS / 'transaction.json')
    for path, (before, _, _) in plans.items():
        safe_parent(path)
        current = read_root_file(path) if path.exists() or path.is_symlink() else None
        if current != before:
            raise ValueError(f'Concurrent configuration change: {path}')
    BACKUPS.mkdir(mode=0o700, parents=True, exist_ok=True)
    if stat.S_IMODE(BACKUPS.stat().st_mode) != 0o700:
        raise ValueError('Backup directory must have mode 0700')
    backup = Path(tempfile.mkdtemp(prefix='install-', dir=BACKUPS))
    records = []
    for i, (path, (before, _, mode)) in enumerate(plans.items()):
        oldmode = stat.S_IMODE(path.stat().st_mode) if before is not None else mode
        records.append(dict(path=str(path), existed=before is not None, mode=oldmode))
        if before is not None: atomic_write(backup / f'old-{i:03d}', before, 0o600)
    atomic_write(backup / 'transaction.json', json.dumps(records).encode(), 0o600)
    try:
        for path, (_, after, mode) in plans.items():
            path.parent.mkdir(parents=True, exist_ok=True)
            safe_parent(path)
            atomic_write(path, after, mode)
    except Exception:
        rollback(backup)
        raise
    print(f'Installed simultaneous authentication. Backup: {backup}')
    print('PAM uses this configuration for NEW requests. Existing dialogs must be closed.')


if __name__ == '__main__':
    main()
