"""Write the all-zero PSK to the sensor. Firmware is never touched.

Why not just run driver_55x4.main(): its loop branches on the firmware string,
and this sensor reports GF3268_RTSEC_APP_10042 while TARGET_FIRMWARE is
..._10041. re.fullmatch(TARGET) therefore fails, re.fullmatch(VALID) succeeds,
and the next thing it does is erase_firmware(). That is precisely the path we
are avoiding, so this calls write_psk() directly and nothing else.

What this does send: one preset_psk_write (flags 0xbb010003, the 96-byte
white-box encoded PSK), followed by a preset_psk_read to confirm the stored
hash now equals PMK_HASH.

This is a one-way change. The read side returns a hash, not the key, and the
write side takes a differently-formatted 96-byte blob, so the factory PSK
cannot be captured beforehand or put back afterwards. Firmware, and therefore
the device's ability to function at all, is unaffected.
"""

import sys

import driver_55x4
import goodix
import protocol

FLASH_FUNCS = ("erase_firmware", "update_firmware")


def main() -> int:
    print("=" * 64)
    print("Goodix 55b4 — PSK 쓰기 (펌웨어는 건드리지 않음)")
    print("=" * 64)

    device = goodix.Device(0x55b4, protocol.USBProtocol)
    device.nop()

    fw = device.firmware_version()
    print(f"\n  펌웨어      : {fw}")

    reply = device.preset_psk_read(0xbb020007)
    if not reply[0]:
        print("  PSK 읽기 실패 — 중단")
        return 1
    print(f"  현재 PSK해시: {reply[2].hex()}")
    print(f"  기대값      : {driver_55x4.PMK_HASH.hex()}")

    if reply[2] == driver_55x4.PMK_HASH:
        print("\n  이미 zero 키가 들어 있습니다. 쓸 필요 없음.")
        return 0

    print("\n  → preset_psk_write(0xbb010003, 96바이트 white-box PSK) 전송")
    ok = driver_55x4.write_psk(device)
    print(f"  write_psk 결과: {ok}")

    reply2 = device.preset_psk_read(0xbb020007)
    print(f"\n  쓴 뒤 PSK해시: {reply2[2].hex() if reply2[0] else '읽기 실패'}")
    match = reply2[0] and reply2[2] == driver_55x4.PMK_HASH
    print(f"  기대값과 일치: {match}")

    print("\n" + ("  ✅ PSK 설정 완료" if match else "  ❌ 일치하지 않음"))
    print("  (erase_firmware / update_firmware 는 호출되지 않았습니다.)")
    return 0 if match else 1


if __name__ == "__main__":
    # Fail loudly rather than silently flashing if this file is ever edited to
    # pull in more of driver_55x4 than intended.
    src = open(__file__).read()
    for fn in FLASH_FUNCS:
        if f"driver_55x4.{fn}" in src:
            print(f"거부: 이 스크립트는 {fn} 를 호출해서는 안 됩니다")
            sys.exit(2)
    sys.exit(main())
