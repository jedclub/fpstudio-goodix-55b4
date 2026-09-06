"""Read-only identification of the Goodix sensor on this board.

The chip identity so far rests on one thing: the app firmware's own name
string. That is the firmware's self-report, not the silicon's. OTP is
one-time-programmable memory written at manufacture, so whatever is in there
describes the part itself and cannot have been changed by a firmware update.

Everything here is a query command. driver_55x4 itself calls read_otp()
during a normal run, so this is nothing the sensor does not see routinely.
"""

import sys

import goodix
import protocol

READS = 3


def hexdump(data: bytes, width: int = 16) -> None:
    for off in range(0, len(data), width):
        chunk = data[off:off + width]
        hexpart = " ".join(f"{b:02x}" for b in chunk)
        asciipart = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
        print(f"    {off:04x}  {hexpart:<{width * 3}} |{asciipart}|")


def main() -> int:
    device = goodix.Device(0x55b4, protocol.USBProtocol)
    device.nop()

    print("\n" + "=" * 64)
    print("펌웨어 문자열 (자기 보고)")
    print("=" * 64)
    versions = []
    for i in range(READS):
        v = device.firmware_version()
        versions.append(v)
        print(f"    {i + 1}: {v}")
    stable = len(set(versions)) == 1
    print(f"    → {'일관됨' if stable else '불일치 ⚠️'}")

    print("\n" + "=" * 64)
    print("부트로더 (IAP)")
    print("=" * 64)
    try:
        print(f"    {device.get_iap_version(25)}")
    except Exception as exc:  # noqa: BLE001
        print(f"    읽기 실패: {exc}")

    print("\n" + "=" * 64)
    print("OTP - 제조 시 각인, 펌웨어로 바뀌지 않음")
    print("=" * 64)
    try:
        otp = device.read_otp()
        print(f"    길이: {len(otp)} bytes\n")
        hexdump(bytes(otp))
        printable = "".join(
            chr(b) if 32 <= b < 127 else "" for b in otp)
        if printable.strip():
            print(f"\n    포함된 문자열: {printable!r}")
        print(f"\n    전부 0x00/0xFF 인가: "
              f"{set(otp) <= {0x00, 0xFF}}")
    except Exception as exc:  # noqa: BLE001
        print(f"    읽기 실패: {exc}")

    print("\n(쓰기 명령은 하나도 실행하지 않았습니다.)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
