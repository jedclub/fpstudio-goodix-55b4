#!/usr/bin/env python3
"""Capture real images from the sensor using goodix-fp-dump's own driver path.

Two things this answers that libfprint cannot:

  1. The chip ID, read straight out of sensor register 0x0000. Every identity
     we have so far is a firmware string the device reports about itself, and
     it has answered GF3208 in some sessions and GF3268 in others. A register
     read is the die talking.

  2. What the captured image actually looks like. When libfprint's matcher
     scores 0/72 against every enrolled sample, that says the frames are
     unusable but not why. run_driver() writes clear-0.pgm, clear-1.pgm and
     fingerprint.pgm at the reference geometry (88x108), so the picture can be
     inspected directly instead of inferred.

Capture only: reset, register/OTP reads, a TLS session, the volatile MCU
config upload, and image reads. erase_firmware/update_firmware are never
reached - main() is not used, because its firmware-string branch takes the
erase path on a device whose firmware is not exactly TARGET_FIRMWARE.

Run from inside a goodix-fp-dump checkout, as root (raw USB), with fprintd
stopped so the sensor is free:

    pkexec bash -c "cd /path/to/goodix-fp-dump && venv/bin/python capture_via_reference_driver.py"

Needs the openssl binary: run_driver starts `openssl s_server` as the TLS peer.
Set GOODIX_FP_DUMP / FPCAP_OUT to override the paths below.
"""

import os
import sys

# Default to the current directory, which is where a goodix-fp-dump checkout
# puts the modules this imports. Overridable so the script can live elsewhere.
GFP = os.environ.get("GOODIX_FP_DUMP", os.getcwd())
OUT = os.environ.get("FPCAP_OUT", os.path.join(os.getcwd(), "captures"))

sys.path.insert(0, GFP)
os.makedirs(OUT, exist_ok=True)
os.chdir(OUT)          # run_driver writes the .pgm files into the cwd

import driver_55x4     # noqa: E402
import goodix          # noqa: E402
import protocol        # noqa: E402


def main() -> int:
    print("=" * 60)
    print("Goodix 55b4 - image capture (no flashing)")
    print("=" * 60)

    device = goodix.Device(0x55b4, protocol.USBProtocol)
    device.nop()
    print(f"\n  firmware: {device.firmware_version()}")

    print("\n  Press the sensor when asked.\n")
    driver_55x4.run_driver(device)

    print("\n" + "=" * 60)
    for name in ("clear-0.pgm", "clear-1.pgm", "fingerprint.pgm"):
        p = os.path.join(OUT, name)
        if os.path.exists(p):
            print(f"  wrote:   {p}  ({os.path.getsize(p):,} bytes)")
        else:
            print(f"  missing: {name}")
    return 0


if __name__ == "__main__":
    # Refuse to run if this file is ever edited to reach the flashing calls.
    src = open(__file__).read()
    for fn in ("erase_firmware", "update_firmware"):
        if f"driver_55x4.{fn}" in src:
            print(f"refusing to run: this script must not call {fn}")
            sys.exit(2)
    sys.exit(main())
