# firmware / 프로토콜 프로브

`goodix-fp-dump`의 프로토콜 구현 위에서 도는 스크립트들. **하나를 제외하고 전부
읽기 전용**이다.

## 준비

이 스크립트들은 `goodix-fp-dump` 체크아웃 안에서 돈다 (`goodix`, `protocol`,
`driver_55x4` 모듈을 임포트한다).

```bash
git clone --recursive https://github.com/goodix-fp-linux-dev/goodix-fp-dump.git
cd goodix-fp-dump
python3 -m venv venv
venv/bin/pip install pyusb crcmod python-periphery spidev pycryptodome crccheck
cp /path/to/src/firmware/probes/*.py .
```

USB 직접 접근에 root가 필요하다:

```bash
pkexec bash -c "cd $PWD && venv/bin/python identify_readonly.py"
```

## 읽기 전용

| 스크립트 | 하는 일 |
|---|---|
| `identify_readonly.py` | 펌웨어 문자열 반복 조회 + IAP 버전 + OTP 덤프 |
| `fwver_once.py` | 펌웨어 문자열 1회 조회. 여러 번 돌려 안정성을 재는 용도 |
| `probe_55b4_readonly.py` | `driver_55x4`의 판정 로직을 **erase 직전까지만** 재현 |
| `backup_55b4_readonly.py` | 펌웨어 리드백 시도 — 이 장치에서는 타임아웃(차단됨) |
| `capture_via_reference_driver.py` | `run_driver()`로 PGM 이미지 3장 저장 |

`probe_55b4_readonly.py`가 중요하다. `driver_55x4.main()`은 펌웨어 문자열을 보고
분기하는데, 이 장치는 `..._10042`를 보고하고 목표는 `..._10041`이라
`re.fullmatch(TARGET)`이 실패하고 `re.fullmatch(VALID)`가 성공해서 **바로
`erase_firmware()`로 간다.** 프로브는 그 직전에 멈춰 조건만 보고한다.

## 쓰기 — 하나뿐

### `write_psk_only.py` ⚠️

장치에 32바이트 zero PSK를 써넣는다. libfprint의 TLS가 그 키를 쓰므로, 이걸
하지 않으면 핸드셰이크가 `cipher operation failed`로 끝난다.

**되돌릴 수 없다.** 읽기는 해시(32B)를 주고 쓰기는 형식이 다른 96B white-box
블롭을 받는다. 공장 키는 사전에 캡처할 수도, 사후에 복원할 수도 없다.

**펌웨어는 건드리지 않는다.** `driver_55x4.main()`을 쓰지 않고 `write_psk()`만
직접 부르며, 스크립트 하단에 자기 소스를 검사해 `erase_firmware` /
`update_firmware` 문자열이 있으면 실행을 거부하는 가드가 있다.

리눅스 전용 기기가 아니라면 하지 말 것 — 이후 Windows Goodix 드라이버는 이
센서로 인증하지 못한다.
