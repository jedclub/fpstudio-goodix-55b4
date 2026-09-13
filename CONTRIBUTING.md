# Contributing to FPStudio

Thanks for helping Linux users with the Goodix `27c6:55b4` fingerprint reader.
Small, reproducible fixes are especially valuable because firmware, kernel,
libfprint, desktop and PAM behaviour all affect the result.

## Before opening a report

1. Confirm the USB ID with `lsusb -d 27c6:55b4`.
2. Follow the current [setup guide](docs/00-setup.md).
3. Run the privacy-safe authentication summary when relevant:

   ```bash
   /opt/fpstudio-auth/bin/fpstudio-auth-report --since "30 minutes ago"
   ```

4. Search existing issues, then use the repository's structured issue form.

Never upload fingerprint images, templates, passwords, PSKs, device keys,
authentication backups, local research sessions, home-directory paths or
unredacted system logs. Report security vulnerabilities privately according to
[SECURITY.md](SECURITY.md).

## Development workflow

```bash
cmake -S src/fpstudio -B build -G Ninja -DBUILD_TESTING=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
python src/pam/test_tty.py build/pam
python src/pam/test_worker_bus.py build/pam/fpstudio-fprint-worker
```

Keep password fallback fail-safe, do not weaken PAM return handling, and add a
regression test for authentication or matcher changes. User-visible GUI and PAM
text must remain localisable. Commits should be focused and explain the user
impact; pull requests should list the tested device, distro, kernel and desktop.

Driver patches derived from libfprint must preserve upstream copyright and
LGPL notices. Do not add proprietary firmware or redistribution-restricted
binary blobs.
