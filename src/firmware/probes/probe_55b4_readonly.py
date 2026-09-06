"""Read-only probe for the Goodix 55b4 sensor.

driver_55x4.main() reads three things and then, if the firmware matches
VALID_FIRMWARE, calls erase_firmware() - which wipes the sensor's app
firmware. This script performs only those three reads and stops before the
erase, so nothing is written to the device.

Reads performed (all query commands, no state change):
  device.nop()                 no-op handshake
  device.firmware_version()    current app firmware string
  check_psk(device)            whether the stored PSK hash equals PMK_HASH
  device.get_iap_version(25)   bootloader version

The bootloader version is the gate: driver_55x4 refuses to flash unless it
is exactly MILAN_RTSEC_IAP_10027, and that refusal happens before any
destructive call. This probe reports whether we would clear that gate.
"""

import re
import sys

import driver_55x4
import goodix
import protocol


def main() -> int:
    print("=" * 62)
    print("Goodix 55b4 읽기 전용 프로브 - 장치에 아무것도 쓰지 않습니다")
    print("=" * 62)

    try:
        device = goodix.Device(0x55b4, protocol.USBProtocol)
        device.nop()
    except Exception as exc:  # noqa: BLE001 - report whatever went wrong
        print(f"\n장치 열기 실패: {exc}")
        return 2

    firmware = device.firmware_version()
    print(f"\n  현재 펌웨어    : {firmware}")

    try:
        valid_psk = driver_55x4.check_psk(device)
    except Exception as exc:  # noqa: BLE001
        valid_psk = f"읽기 실패 ({exc})"
    print(f"  PSK 기대값 일치: {valid_psk}")

    if firmware == driver_55x4.IAP_FIRMWARE:
        iap = driver_55x4.IAP_FIRMWARE
    else:
        iap = device.get_iap_version(25)
    print(f"  부트로더(IAP)  : {iap}")

    print("\n" + "-" * 62)
    print("판정")
    print("-" * 62)

    target = re.fullmatch(driver_55x4.TARGET_FIRMWARE, firmware) is not None
    valid = re.fullmatch(driver_55x4.VALID_FIRMWARE, firmware) is not None
    iap_ok = iap == driver_55x4.IAP_FIRMWARE

    print(f"  펌웨어이 이미 목표값인가 : {target}   "
          f"(목표 {driver_55x4.TARGET_FIRMWARE})")
    print(f"  플래시 대상 범위인가     : {valid}   "
          f"(패턴 {driver_55x4.VALID_FIRMWARE})")
    print(f"  부트로더 조건 충족       : {iap_ok}   "
          f"(요구 {driver_55x4.IAP_FIRMWARE})")

    print()
    if target:
        print("  => 이미 목표 펌웨어입니다. 플래시 불필요.")
        verdict = 0
    elif not iap_ok:
        print("  => 부트로더가 요구값과 다릅니다.")
        print("     driver_55x4 는 이 경우 erase 전에 중단합니다. 진행 불가.")
        verdict = 1
    elif valid:
        print("  => 조건 충족. 플래시를 진행하면 다음이 일어납니다:")
        print("       1) 현재 앱 펌웨어 삭제 (mcu_erase_app)")
        print("       2) 부트로더 모드로 재부팅")
        print("       3) PSK 쓰기 (zero 키, white-box 인코딩)")
        print(f"       4) {driver_55x4.TARGET_FIRMWARE} 플래시")
        verdict = 0
    else:
        print("  => 펌웨어가 플래시 대상 범위 밖입니다. 진행 불가.")
        verdict = 1

    print("\n  (이 프로브는 여기서 종료합니다. erase/flash 는 실행하지 않았습니다.)")
    return verdict


if __name__ == "__main__":
    sys.exit(main())
