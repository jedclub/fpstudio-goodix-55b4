# FPStudio for Goodix 27c6:55b4

[![CI](https://github.com/jedclub/fpstudio-goodix-55b4/actions/workflows/ci.yml/badge.svg)](https://github.com/jedclub/fpstudio-goodix-55b4/actions/workflows/ci.yml)
[![Latest release](https://img.shields.io/github/v/release/jedclub/fpstudio-goodix-55b4?display_name=tag)](https://github.com/jedclub/fpstudio-goodix-55b4/releases/latest)
[![License: LGPL-2.1-or-later](https://img.shields.io/badge/License-LGPL--2.1--or--later-blue.svg)](LICENSE)
[![Platform: Arch Linux](https://img.shields.io/badge/platform-Arch%20Linux-1793D1?logo=arch-linux&logoColor=white)](https://archlinux.org/)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=c%2B%2B)](https://isocpp.org/)

**FPStudio** is an open-source Linux fingerprint driver toolkit and setup wizard
for the **Goodix 27c6:55b4** reader found in several ThinkPad L14/L15 Gen 1
systems. It combines a patched `libfprint`/`fprintd` build recipe with a Qt 6
application for guided device checks, enrolment and reversible integration with
KDE Plasma lock screen, sudo, Polkit, TTY/PAM and optional Vulkan GPU matching.

If FPStudio makes your `27c6:55b4` reader usable, please star the repository so
other affected Linux users can find the tested path more easily.

> [!WARNING]
> This is hardware-specific, experimental software. A failing fingerprint
> attempt must always retain a password path. Never publish fingerprint images,
> templates, device secrets, local research sessions or system-authentication
> backups.

## At a glance

| Guided setup | Local diagnostics |
|---|---|
| <img src="docs/images/setup-wizard.png" alt="FPStudio fingerprint unlock setup wizard" width="430"> | <img src="docs/images/diagnostics.png" alt="FPStudio diagnostics window" width="500"> |
| Check the sensor, patched driver, permissions and enrolment path step by step. | Inspect device state, invoke safe diagnostics and choose the UI language. |

The screenshots contain no biometric samples. Real capture data is deliberately
kept outside version control.

## What it provides

- **One setup wizard** for sensor discovery, driver prerequisites, permissions,
  standard enrolment and opt-in system integration.
- **Live collection guidance** that derives stable candidates from the video
  stream rather than treating one arbitrary frame as a result.
- **Vulkan-assisted research matching** with contact anchors, ridge-region
  checks, bounded rotation and ±5% uniform-scale search. The coloured preview
  makes comparison evidence and uncertain alignment visible.
- **Privacy-local research sessions**. Installed builds store biometric frames
  and templates under `~/.local/share/fpstudio/recognition/`; source-tree test
  material under `local-private/` remains excluded from Git.
- **Eleven UI languages**: English, Korean, Japanese, Simplified/Traditional
  Chinese, Spanish, German, French, Russian, Italian and Portuguese.
- **Safe authentication boundary**: password fallback remains available;
  GPU-assisted system authentication is experimental and must be explicitly
  installed and verified by the machine owner.
- **Local authentication integration** for supported sudo, Polkit, TTY login
  and KDE lock-screen PAM profiles, with concise sensor feedback. See
  [simultaneous-input setup and limitations](docs/21-simultaneous-auth.md).

## Requirements

- Arch Linux or an Arch-derived distribution for the driver package recipe.
- Goodix `27c6:55b4` hardware (`lsusb -d 27c6:`).
- Qt 6.5+, CMake, Ninja, Vulkan headers/loader and `glslangValidator` to build
  FPStudio.
- PAM and libsystemd development files for the authentication module/worker.
- A patched `libfprint` package from this repository; install `fprintd` only
  after the driver package.

## Quick start

```bash
git clone https://github.com/jedclub/fpstudio-goodix-55b4.git
cd fpstudio-goodix-55b4

# 1. Build and install the pinned, patched libfprint package.
cd src/driver
makepkg -s --needed -f
sudo pacman -U libfprint-goodixtls-55x4-fixed-*.pkg.tar.zst
sudo pacman -S --needed fprintd
cd ../..

# 2. Build FPStudio and run its test suite.
cmake -S src/fpstudio -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure

# 3. Install the Release application and its self-contained wizard helpers.
sudo cmake --install build --prefix /usr

# 4. Open the setup wizard.
fpstudio
```

The GUI resolves its language from `--lang`, a remembered user choice, then
the system locale. For example, start in English with:

```bash
./build/fpstudio --lang en
```

## Security and privacy model

CLI fingerprint authentication completes automatically only if no key has been
pressed. Any key (including erase or arrow keys) makes Enter mandatory for that
request. Typed input is masked with `*`; submitted passwords use the existing
PAM validator. KDE fingerprint authentication still completes automatically.
CLI uses a red Admin header where terminal colour is supported and a `(1/20)`
retry counter. Sensor activity is bounded to 20 retry events or 60 seconds.
FPStudio integrates at PAM and never replaces `sudo` with a shell function,
alias or nested pseudo-terminal. Legacy `sudo` output wrappers must be removed:
they can detach the controlling terminal and break both masked password input
and fingerprint completion.

Every installed authentication request also writes a privacy-safe, structured
timeline to the system journal. It records module routing, sensor readiness,
contact transitions, capture quality, GPU result/reinitialisation/fallback,
retry count, password handoff and final outcome. It never records passwords,
usernames, fingerprint images, templates or gallery paths. Summarise recent
sessions and likely failure points with:

```bash
/opt/fpstudio-auth/bin/fpstudio-auth-report --since "2 hours ago"
/opt/fpstudio-auth/bin/fpstudio-auth-report --since today --json
```

The GUI and setup/recovery wizard route their shipped messages through the Qt
catalogues. Korean and English setup text is complete; the other catalogues
use an English fallback for newly added advanced setup details. PAM notices
cover all eleven languages and follow `LC_ALL`, `LC_MESSAGES`, `LANGUAGE`, then
`LANG`, independently of the GUI language selector. Colon-separated gettext
preferences such as `LANGUAGE=ko:en` are supported.

- **No biometrics in this repository or release assets.** The ignore rules
  exclude local research material, PGM frames, fingerprint templates, build
  trees, credentials and local coding-agent state.
- **No firmware blob is shipped.** The repository contains source probes and
  build instructions only.
- **System changes are explicit and reversible.** The wizard separates checks
  from installation and preserves the password fallback. Local TTY login and
  `su` are supported only when their PAM profiles match the guarded patterns;
  remote login, SSH and disk unlock are never modified.
- **Research is not a security claim.** A high matcher score or consistent
  ridge pattern is not proof of identity, FAR, FRR or production readiness.

## Release contents

Each tagged release publishes source and Arch Linux x86-64 application bundles
with SHA-256 checksums. It does not publish a prebuilt fingerprint driver:
recipients build the pinned source with the supplied patches on their own Arch
system. The application bundle contains translations, built authentication
helpers and pinned setup recipes, so the installed wizard does not depend on
the source checkout. This keeps the driver, runtime dependencies and local
security policy visible and reviewable.

The current driver source revision is
[`c1937b99ec3db5abca05f619a95d2e37496d8810`](https://github.com/TheWeirdDev/libfprint/commit/c1937b99ec3db5abca05f619a95d2e37496d8810).

## Development and CI

```bash
cmake -S src/fpstudio -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

Pull requests and `main` changes run privacy checks, driver package builds,
translation checks, a full CTest run and a release-readiness check. Pushing a
`v*` tag rebuilds and tests the project, publishes source and application
bundles with checksums, and uses the matching note from `docs/releases/`.

## Help and contributing

Start with the [setup guide](docs/00-setup.md). For a reproducible problem, use
the structured [bug report](https://github.com/jedclub/fpstudio-goodix-55b4/issues/new?template=bug_report.yml);
for improvements, see [CONTRIBUTING.md](CONTRIBUTING.md). Never attach biometric
images, templates, passwords, device keys or raw private research sessions.
Security-sensitive reports belong in a private GitHub security advisory, as
described in [SECURITY.md](SECURITY.md).

## Documentation

- [Setup guide](docs/00-setup.md)
- [Driver implementation notes](docs/03-driver.md)
- [FPStudio architecture](docs/04-fpstudio.md)
- [Recognition and research workflow](docs/07-recognition.md)
- [Vulkan matching design](docs/11-vulkan-matching.md)
- [Contact-anchored v8 matcher](docs/20-contact-anchored-rotation-v8.md)
- [fprintd runaway loop: cause and fix](docs/23-fprintd-runaway.md)
- [Matcher CPU optimisation](docs/24-optimisation.md)
- [v0.3.0 release notes](docs/releases/v0.3.0.md)

## Licence and third-party code

FPStudio and its Goodix/libfprint-derived driver patches are licensed under
the **GNU Lesser General Public License, version 2.1 or later**. See
[LICENSE](LICENSE) and [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

The driver build recipe retrieves the LGPL `libfprint` Goodix TLS fork at a
pinned revision. If you redistribute a modified driver binary, you must also
provide the corresponding source, patches, licence text and copyright notices.
