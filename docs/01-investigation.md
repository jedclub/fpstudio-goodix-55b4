# 조사 기록

ThinkPad L15 Gen 1 (20U7) · Goodix `27c6:55b4` · 2026-09-05 ~ 06

이 문서는 결론뿐 아니라 **틀렸던 판단과 그것을 뒤집은 증거**도 함께 남긴다.
같은 함정을 다시 밟지 않는 것이 목적이다.

---

## 1. 장치 식별

| 항목 | 값 |
|---|---|
| USB | `27C6:55B4`, Vendor Specific(255), EP in `0x82` / out `0x1` |
| 부트로더 (IAP) | `MILAN_RTSEC_IAP_10027` |
| 앱 펌웨어 | `GF3208_RTSEC_APP_10042` **또는** `GF3268_RTSEC_APP_10042` |
| 센서 배열 | 88 × 108, 8bit |

### 펌웨어 문자열이 두 가지로 나온다

이 장치는 세션에 따라 자기를 다르게 보고한다. 한 기간에는 20/20 전부 `GF3268`,
다른 기간에는 8/8 전부 `GF3208`. **기간 안에서는 안정적이고 기간 사이에 바뀐다.**

전송 오류가 아니다. `goodix-fp-dump`는 패킷 체크섬을 실제로 검증하며
(`decode_message_protocol`), 두 응답 모두 체크섬을 통과했다. libfprint 쪽 디코더는
`// TODO implement checksum` 상태라 검증하지 않는다 — 이 차이가 판별에 결정적이었다.

원인은 아직 모른다. 실용적으로는 **두 문자열을 모두 허용 목록에 넣으면 된다**
(패치 `0003`).

### 다이 확정 시도와 그 한계

`GF3208`이 펌웨어 이름인지 실제 다이명인지 확인하려고 제조사 드라이버를 뜯었다:

```
Lenovo 카탈로그   https://download.lenovo.com/catalog/20U7_Win10.xml
  └ r19gf02w      Goodix FingerPrint Driver 3.1.55.600
      ├ WbdiUsb.inf   USB\Vid_27C6&Pid_55B4 지원 명시
      └ Wbdi.dll      센서 모델 테이블이 평문
```

`Wbdi.dll`에는 `GF3206/GF3208/GF3258/GF3268` 각각에 대한 `_RTSEC_APP_10042`
문자열이 **별개 항목으로** 들어 있다. 즉 `GF32xx`는 다이, `_100xx`는 리비전이다.
다만 그 사실이 우리 장치가 둘 중 무엇인지까지 확정해 주지는 못했다.

> 패키지는 SHA-256을 Lenovo 공시값과 대조한 뒤 열었고, 실행하지 않았다.
> InnoSetup 형식이라 `innoextract`로 풀었다.

---

## 2. 틀렸던 판단들

정직하게 남긴다. 각각이 시간을 잡아먹었다.

### ❌ "다른 칩이라 config 블롭이 안 맞아서 크래시한다"

MCU 설정 업로드에서 `*** buffer overflow detected ***`가 나기에 GF3268용
캘리브레이션 블롭이 GF3208에 맞지 않는다고 결론냈다.

**실제로는** `err_from_ssl()`의 off-by-one이었다:

```c
err->message = malloc (strlen (msg));   // NUL 자리 없음
strcpy (err->message, msg);             // strlen+1 바이트를 씀
```

이 함수는 `SSL_accept`가 **실패했을 때만** 호출된다. 즉 진짜 사건은 "TLS 실패"였고,
그것을 보고하려다 fortify에 걸려 죽은 것이다. 한 바이트 고치니(패치 `0005`)
config 업로드가 정상 통과했고, 진짜 원인 `cipher operation failed`가 드러났다.

**교훈**: 크래시 지점과 원인 지점을 같다고 보지 말 것. 백트레이스를 먼저 뜰 것.

### ❌ "제조사 DLL에서 GF3208용 config를 뽑으면 된다"

`?? 11 60 71` 시그니처로 패키지 전체를 훑었다. 55x4 형태(`24 95 2c c1`)는
GF3268용 **하나뿐**이었고 나머지 4개는 511 계열이었다. 게다가 이 드라이버는
Intel SGX 엔클레이브를 쓴다(`WbdiEnclave.signed.dll`). 정적 분석으로 뚫을
대상이 아니다.

### ❌ "칩이 같다" (한 번의 관측으로 뒤집은 판단)

프로브가 한 번 `GF3268`로 읽자 앞선 결론을 뒤집었다. 이어서 8회 연속 `GF3208`이
나왔다. **1회 관측으로 결론을 뒤집지 말 것.**

---

## 3. 실제로 막혔던 관문과 해법

```
1. Read and NO OP / Enable Chip / NOP     통과
2. Checking FW      문자열 불일치          → 패치 0003
3. Checking PSK     해시 불일치            → 패치 0004 + PSK 쓰기
4. Reset / IDLE                           통과
5. Uploading Device Config                → 패치 0005 (크래시는 무관한 버그였음)
6. TLS handshake    cipher operation failed → PSK 쓰기로 해결
7. 등록                                    ✅ 10단계 완료
8. 검증             sigfm score 0/72       ❌ 미해결
```

### PSK 쓰기

libfprint의 TLS는 32바이트 zero 키를 쓴다(`tls_server_psk_server_callback`에서
`OPENSSL_hexstr2buf("00…0")`). 장치는 공장 PSK를 갖고 있어 협상이 성립하지 않았다.

`preset_psk_write(0xbb010003, 96바이트 white-box PSK)` 한 번으로 해결됐다.
**펌웨어는 건드리지 않는다** — `driver_55x4.main()`을 쓰지 않은 이유가 이것이다.
그 함수는 현재 펌웨어(`..._10042`)가 목표(`..._10041`)와 달라
`erase_firmware()` 분기로 빠진다.

되돌릴 수 없다: 읽기는 해시(32B)를 주고 쓰기는 형식이 다른 96B를 받는다.

---

## 4. 남은 문제 — 검증이 0/72

기하값을 88×108로 바로잡고(패치 `0006`) 재등록했으나 점수는 여전히 0이다.

**확인된 것**
- 캡처는 실제 내용을 담는다: 88×108, 고유 픽셀값 253개, 전 계조 사용
- 특징점은 추출된다 (72개)
- 등록 10단계가 모두 통과한다

**추정 가능한 방향**
- 이미지가 지문이 아닌 다른 무언가(잔상·배경)일 가능성 — `captures/`의 프레임을
  눈으로 확인하는 것이 다음 단계
- 프레임 조립 순서(행 방향·시작 오프셋)가 여전히 어긋났을 가능성
- SIGFM이 이 해상도에서 필요한 대비를 못 얻는 경우

`fpstudio`를 만든 이유가 여기에 있다. 이 판단은 로그가 아니라 **이미지를 봐야**
할 수 있다.

---

## 5. 배제된 경로

| 시도 | 결과 |
|---|---|
| 공식 `libfprint` | 55xx 대역 지원 0건 (MOC 계열만) |
| 펌웨어 백업 | `read_firmware`가 타임아웃 — 리드백 차단됨 |
| GF3268 펌웨어 플래시 | 하지 않음. 백업 불가 + 다이 불일치 가능성 |
| Windows 드라이버 이식 | 불가 — WBDI 스택 + SGX 엔클레이브 |
| `fox8091/Goodix-GF3208` | 안드로이드 SPI용 미완성 코드, 저자가 폐기 선언 |

---

## 참고

- [goodix-fp-dump](https://github.com/goodix-fp-linux-dev/goodix-fp-dump)
- [TheWeirdDev/libfprint `55b4-experimental`](https://github.com/TheWeirdDev/libfprint/tree/55b4-experimental)
- [SIGFM 매칭 알고리즘](https://github.com/goodix-fp-linux-dev/sigfm)
- [linux-hardware.org `27c6:55b4`](https://linux-hardware.org/index.php?id=usb%3A27c6-55b4)
