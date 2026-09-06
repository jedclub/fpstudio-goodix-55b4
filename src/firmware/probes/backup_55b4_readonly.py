"""Read-only firmware backup for the Goodix 55b4 sensor.

Uses COMMAND_READ_FIRMWARE, the same query dumper_53x5.py uses to dump other
Goodix models. Nothing is erased and nothing is written; if the sensor's
current firmware refuses readback the first page simply fails and this exits.

Why a backup is worth attempting: after flashing, the sensor's PSK is the
known all-zero key, and update_firmware() computes the verification HMAC
from that key over whatever image it is given. So an image we hold the bytes
for can be written back and will verify - the HMAC is not tied to the
factory firmware. That makes a successful dump a real restore path, not just
a souvenir.

Probes one page first, then dumps until the device stops answering.
"""

import hashlib
import sys

import goodix
import protocol

PAGE = 0x400
MAX_BYTES = 256 * 1024  # generous ceiling; we stop as soon as a read fails
OUT = "GF3268_RTSEC_APP_10042_factory_backup.bin"


def main() -> int:
    print("=" * 62)
    print("Goodix 55b4 펌웨어 백업 (읽기 전용)")
    print("=" * 62)

    device = goodix.Device(0x55b4, protocol.USBProtocol)
    device.nop()

    firmware = device.firmware_version()
    print(f"\n  현재 펌웨어: {firmware}")

    print(f"\n  1페이지 시험 읽기 (offset 0, {PAGE} bytes)...")
    try:
        first = device.read_firmware(0, PAGE)
    except Exception as exc:  # noqa: BLE001
        print(f"\n  ❌ 읽기 거부됨: {exc}")
        print("  이 펌웨어는 리드백을 막고 있습니다. 백업 불가.")
        return 1

    print(f"  ✅ 성공 ({len(first)} bytes)")

    dump = bytearray(first)
    offset = PAGE
    while offset < MAX_BYTES:
        try:
            dump += device.read_firmware(offset, PAGE)
        except Exception as exc:  # noqa: BLE001
            print(f"\n  offset 0x{offset:06x} 에서 읽기 종료: {exc}")
            break
        offset += PAGE

    data = bytes(dump)
    print(f"\n  덤프 크기 : {len(data):,} bytes")
    print(f"  SHA-256   : {hashlib.sha256(data).hexdigest()}")

    blank = data.count(0xFF) + data.count(0x00)
    print(f"  비어있는 바이트(0x00/0xFF) 비율: {blank * 100 // max(len(data), 1)}%")
    if blank == len(data):
        print("\n  ⚠️  전부 공백입니다 - 실제 펌웨어가 아닙니다. 백업으로 쓸 수 없습니다.")
        return 1

    with open(OUT, "wb") as f:
        f.write(data)
    print(f"\n  저장: {OUT}")
    print("\n  (쓰기 명령은 하나도 실행하지 않았습니다.)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
