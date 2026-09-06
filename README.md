# goodix-55b4-linux

Getting a Goodix `27c6:55b4` fingerprint sensor working on Linux, and the
tooling built along the way to make that possible to debug at all.

Hardware: **ThinkPad L15 Gen 1 (20U7)**, CachyOS / KDE Plasma 6 (Wayland).

---

## Status

| | |
|---|---|
| Device recognised | ✅ `Goodix TLS Fingerprint Sensor 55X4`, driver `goodixtls55x4` |
| TLS session established | ✅ after writing the all-zero PSK |
| MCU config upload | ✅ after fixing an unrelated crash in the driver |
| Raw image capture | ✅ 108×88, 8-bit, real ridge detail |
| Enrolment | ✅ 15 stages, with a quality gate that hands poor scans back instead of spending one |
| **Verification** | ✅ **works** — passing scores 77 to 2920 against a threshold of 72 |
| Capture quality | ✅ clean ridge detail once patch `0006` was reverted |

The sensor enrols and verifies. Getting there took undoing one of this repo's
own patches and then fixing a second thing underneath it.

**The row length.** Patch `0006` had changed it from 108 to 88, on the strength
of two sources that agree — `goodix-fp-dump` and the vendor's `Wbdi.dll` table —
but that both describe the die's *physical* geometry. The sensor emits whatever
the uploaded MCU config asks it to scan, and that config is the GF3268's (251 of
its 256 bytes are identical to a constant inside `Wbdi.dll`). Reverting the patch
took the row/column deviation ratio from 11:1 to about 1:1 and produced clean
diagonal ridge detail.

**The normalisation.** That alone still scored 0/72. `squash_frame_linear`
stretched min-to-max, so the part of the window the finger never touched pinned
the black point and the ridges were squeezed into the top half of the range —
every capture had a 1st percentile of 0 and a median of 122-189. SIFT's detector
drops low-contrast extrema, so that was a direct loss of usable keypoints.
Stretching the 2nd-98th percentile of the live area instead (patch `0009`) took
the score on two overlapping captures from 5 to 63, and made real verification
work.

Measured over seven attempts: five matched. Passing scores ran 77 to 2920
against a threshold of 72, while the two failures scored 0 and 7 — the matcher
counts agreeing pairs of pairs, so it either finds the overlap decisively or not
at all. The failures are placement misses: the window is about 5.5 x 4.5 mm, and
a print that lands away from all ten enrolled samples has nothing to agree with.

See [docs/05-geometry-regression.md](docs/05-geometry-regression.md) for the
measurement trail, including the wrong theory chased first, and
[docs/01-investigation.md](docs/01-investigation.md) for how the device was
identified.

---

## What is here

```
docs/          how the device was identified, what was tried, what was ruled out
  evidence/    facts extracted from the vendor's Windows driver (not the files themselves)
src/
  fpstudio/    C++/Qt6 tool: GUI, JSON CLI, and an MCP server over one engine
    pam/       a polkit stack that accepts a fingerprint, password still the fallback
  driver/      patches against the libfprint goodixtls fork, plus a PKGBUILD
  firmware/    read-only probes and the one write (PSK) this needed
captures/      a frame straight off the sensor
```

### `src/fpstudio` — the instrument

Debugging this by reading `journalctl` and guessing was the real bottleneck.
`fpstudio` links libfprint directly and surfaces what the driver knows:

- the captured frame, drawn unsmoothed so ridge detail survives
- each activation stage as the driver reaches it
- every SIGFM score, next to the threshold it is judged against
- the driver's own log, live

One binary, three front ends. The GUI stays unprivileged and runs its own
`--cli` mode under `pkexec` for the operations that touch the device, so the
window and the agent see identical results:

```bash
fpstudio                                  # Qt window
fpstudio --cli capture --out frame.png    # one JSON object on stdout
fpstudio --mcp                            # JSON-RPC over stdio, for an agent
```

The CLI and MCP modes need no display, so they work over ssh and inside a tool
call.

**An MCP request shows up on screen.** When an agent asks for a capture, the
running window raises a banner saying who asked and what the sensor needs —
so the person at the machine follows the screen rather than the transcript.
See [docs/04-fpstudio.md](docs/04-fpstudio.md).

### `src/driver` — five patches

Two come from the AUR fork, one is device-specific, and two are plain bugs
that anyone on this driver hits:

| | |
|---|---|
| `0001` | host-side finger detection (from the AUR fork) |
| `0002` | opencv5 build fix (from the AUR fork) |
| `0003` | accept this sensor's firmware strings |
| `0005` | **`err_from_ssl()` off-by-one** — under-allocates by one byte, so reporting *any* SSL failure aborts the process before the reason is printed |
| `0006` | **sensor geometry 108×88 → 88×108** — the row length was wrong |

`0005` is worth calling out. It turned every diagnosable TLS failure into
`*** buffer overflow detected ***` with no detail, which is what made this look
like a hardware incompatibility for most of the investigation.

### `src/firmware` — probes

Read-only unless the filename says otherwise. `write_psk_only.py` is the single
write this project performs, and it deliberately avoids
`driver_55x4.main()`, whose firmware-string branch would erase the sensor's
firmware on this device.

---

## Quick start

```bash
# build the tool
cd src/fpstudio && cmake -S . -B build -G Ninja && cmake --build build

# see what the sensor is
./build/fpstudio --cli devices

# get a frame (needs USB access, hence pkexec)
pkexec ./build/fpstudio --cli capture --out /tmp/frame.png
```

To rebuild the patched driver, see [docs/03-driver.md](docs/03-driver.md).

---

## Requirements

- `libfprint` built from the goodixtls 55x4 fork with the patches in `src/driver`
- Qt 6.5+, CMake 3.21+, a C++20 compiler
- `pyusb crcmod python-periphery spidev pycryptodome crccheck` for the probes,
  plus a checkout of [goodix-fp-dump](https://github.com/goodix-fp-linux-dev/goodix-fp-dump)

---

## A warning about the PSK

This sensor ships with a factory PSK. libfprint's TLS uses an all-zero key, so
the two cannot negotiate until the device is re-provisioned. `write_psk_only.py`
does that, and **it cannot be undone**: the read side returns a hash, the write
side takes a differently formatted blob, so the original key can be neither
captured beforehand nor restored after.

Firmware is untouched, so the device keeps working either way — but a Windows
Goodix driver will no longer authenticate against it. Only do this on a machine
you do not intend to run Windows fingerprint auth on.

---

## Licence

The patches under `src/driver/patches` are derived from libfprint and the
goodixtls fork and inherit **LGPL-2.1**. `src/fpstudio` and the probes are
**MIT** — see [LICENSE](LICENSE).

Not affiliated with or endorsed by Shenzhen Goodix Technology or Lenovo.
