# 장치 실측값

모두 이 기기에서 직접 읽은 값이다. 원본 응답은 [`evidence/`](evidence/)에 있다.

> 개체마다 다른 값(OTP, 공장 PSK 해시)과 머신타입 전체 표기는 저장소에 두지
> 않았다. 자리표시자로 표시된 곳이 그것이다.

## USB

```
Bus 001 Port 004 Dev 006
  idVendor        0x27C6   Shenzhen Goodix Technology
  idProduct       0x55B4   Fingerprint Reader
  iProduct        "Goodix FingerPrint Device"
  bNumInterfaces  1
  bInterfaceClass 255 (Vendor Specific)
  EP in  0x82 / EP out 0x1
```

커널 드라이버는 붙지 않는다(`Driver=[none]`). libfprint가 usbfs로 직접 잡는다.

## 조회 응답

| 명령 | 응답 |
|---|---|
| `firmware_version()` | `GF3208_RTSEC_APP_10042` 또는 `GF3268_RTSEC_APP_10042` — [세션마다 다름](01-investigation.md#펌웨어-문자열이-두-가지로-나온다) |
| `get_iap_version(25)` | `MILAN_RTSEC_IAP_10027` |
| `preset_psk_read(0xbb020007)` | 공장: `<redacted - per-device factory PSK hash>` |
| | zero 키 기록 후: `81b8ff49 0612022a 121a9449 ee3aad27 92f32b9f 3141182c d0101994 5ee50361` |
| `read_firmware(0, 1024)` | **타임아웃** — 리드백 차단 |
| `read_otp()` | 32바이트, 아래 참조 |

### OTP

```
0000  <redacted - per-device OTP, see local notes>
0010  <redacted>
```

공백이 아닌 실데이터지만 ASCII 모델명은 없다. Goodix의 OTP 레이아웃이
비공개라 이 바이트만으로 다이를 판별하지 못했다.

## 센서 기하

`88 × 108`, 8bit 그레이스케일, 픽셀 9,504개.

세 출처가 일치한다:

| 출처 | 값 |
|---|---|
| `goodix-fp-dump` `driver_55x4.py` | `SENSOR_WIDTH=88`, `SENSOR_HEIGHT=108` |
| 제조사 `Wbdi.dll` 파라미터 테이블 | `(800 dpi, 88, 108)` |
| `fpstudio --cli capture` 실측 | `{"width":88,"height":108}` |

libfprint 포크만 `108 × 88`로 반대였다 → 패치 `0006`.

### Wbdi.dll 의 센서 파라미터 테이블

32바이트 레코드가 나열돼 있다 (`0x1febc0`부터):

```
        dpi  가로  세로   ?   #
        800   88  108   0   1
        800   64  176   6   2
        800   54  176   7   3     ← GF3206 (공개 자료의 54×176과 일치)
        400  112  132   2   4
        600   60  128   1   5
        600   88  108   8   6
        600   64  176   4   7
        600   68  118  63   8
        300   96   96  62   9
        800   88  108   0  10
        800   64   80  10  11
```

`88×108`이 세 번 나온다 — 여러 모델이 같은 배열을 공유한다.

## 하드웨어 사양

- ThinkPad L15 Gen 1, 머신타입 `20U7`
- BIOS `R19ET56W 1.40` — 지문 센서와 **무관**. 이 프로젝트는 BIOS/UEFI를
  건드리지 않는다
- 센서는 USB 버스의 독립 모듈이며 자체 MCU와 플래시를 갖는다
