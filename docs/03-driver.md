# 드라이버 패치

기반: [TheWeirdDev/libfprint](https://github.com/TheWeirdDev/libfprint) `55b4-experimental`
브랜치 (커밋 `c1937b9`), AUR `libfprint-goodixtls-55x4-fixed` 패키징.

`0001`·`0002`는 AUR 패키지가 원래 갖고 있던 것이고, `0003`·`0005`·`0006`이 이
프로젝트에서 추가한 것이다. `0004`는 폐기했다 — 아래 참조.

---

## 0001 — host-side finger detect *(기존)*

이 펌웨어의 MCU는 FDT-down 응답이 실제 접촉까지 블록하지 않고 즉시 돌아온다.
프레임을 폴링해 캘리브레이션 배경과의 차이로 접촉을 판정한다.

## 0002 — opencv5 pkgconfig *(기존)*

`meson`이 `opencv4`만 찾던 것을 `opencv4 → opencv5 → opencv` 순으로 시도하게 한다.

---

## 0003 — 펌웨어 문자열 허용 목록

원본은 `GF3268_RTSEC_APP_10041` 하나와만 정확히 일치할 것을 요구한다.
이 장치는 `_10042`를 보고하고, 그것도 `GF3208`/`GF3268` 두 가지로 오간다.

```c
static const gchar *const goodix_55x4_known_firmware[] = {
    "GF3268_RTSEC_APP_10041",   /* 원저자 하드웨어 */
    "GF3268_RTSEC_APP_10042",   /* 같은 다이, 한 리비전 위 */
    "GF3208_RTSEC_APP_10042",   /* 이 센서가 때때로 답하는 값 */
    NULL,
};
```

검사 자체는 부작용 없는 문자열 비교다. 잘못 넣어도 위험하지 않다 —
실제 검증은 몇 단계 뒤 TLS 핸드셰이크가 한다.

## 0004 — 없음 (폐기)

한때 공장 PSK 해시를 허용 목록에 넣는 패치가 있었다. **잘못된 방향이었다.**

`goodix_55x4_psk_0`는 TLS 키가 아니라 **장치가 zero 키를 보유할 때 보고하는
파생값**이다. 공장 PSK를 가진 장치를 통과시켜 봐야, 몇 단계 뒤 TLS 핸드셰이크가
`cipher operation failed`로 끝난다 — 키가 실제로 다르기 때문이다.

해결책은 검사를 우회하는 것이 아니라
[PSK를 써넣는 것](../src/firmware/probes/write_psk_only.py)이다. 그러면 장치가
표준값을 보고하므로 **원래 검사가 그대로 통과한다.** 패치가 필요 없다.

> 남기지 않은 또 다른 이유: 그 패치에는 이 개체의 공장 PSK 해시가 코드로 박혀
> 있었다. 기기마다 다른 값이라 남의 저장소에 있을 이유가 없다.

## 0005 — `err_from_ssl()` off-by-one ⚠️

**장치와 무관한 순수 버그.** 이 드라이버를 쓰는 누구나 밟는다.

```diff
-  err->message = malloc (strlen (msg));
+  if (msg == NULL)
+    msg = "unknown SSL error";
+  err->message = malloc (strlen (msg) + 1);
   strcpy (err->message, msg);
```

`strcpy`는 `strlen+1` 바이트를 쓰므로 종결자가 할당 밖으로 나간다.
glibc의 `_FORTIFY_SOURCE`가 이를 잡아 프로세스를 abort시킨다.

이 함수는 `SSL_accept` 실패 시에만 불린다. 따라서 **모든 진단 가능한 TLS 오류가
이유 없는 `*** buffer overflow detected ***`로 바뀌어** 있었다. 이 프로젝트가
오래 헤맨 직접적 원인이다.

업스트림에 보고할 가치가 있다.

## 0006 — 센서 기하 ⚠️

```diff
-#define GOODIX55X4_WIDTH 108
-#define GOODIX55X4_HEIGHT 88
-#define GOODIX55X4_SCAN_WIDTH 108
+#define GOODIX55X4_WIDTH 88
+#define GOODIX55X4_HEIGHT 108
+#define GOODIX55X4_SCAN_WIDTH 88
```

`goodix-fp-dump`와 제조사 `Wbdi.dll` 모두 88×108이라고 한다([근거](02-device.md#센서-기하)).

픽셀 총수가 같으므로(`88*108 == 108*88`) `RAW_FRAME_SIZE`와 USB 전송 길이는
변하지 않는다. 바뀌는 것은 **선형 픽셀 스트림을 어디서 행으로 자르느냐**뿐이다.
88에서 잘라야 할 것을 108에서 자르면 행마다 조금씩 밀려 이미지가 어긋난다.

> 이 패치 이후에도 검증 점수는 0/72다. 기하가 유일한 원인은 아니었다.

---

## 빌드

```bash
git clone https://aur.archlinux.org/libfprint-goodixtls-55x4-fixed.git
cd libfprint-goodixtls-55x4-fixed
cp /path/to/src/driver/patches/000*.patch .
cp /path/to/src/driver/PKGBUILD .          # 패치 5개가 편입된 버전
makepkg -f
sudo pacman -U libfprint-goodixtls-55x4-fixed-*.pkg.tar.zst
sudo pacman -S fprintd                      # 드라이버 설치 후에
```

설치 순서가 중요하다. `fprintd`를 먼저 넣으면 공식 `libfprint`가 딸려온다.

### 디버그 심볼

크래시 위치를 잡아야 한다면 `PKGBUILD`에 추가한다:

```bash
options=('!strip' 'debug')
```

`--buildtype=release`가 `-g`를 덮으므로 DWARF까지는 안 남지만, 심볼 테이블만
있어도 백트레이스에 함수 이름은 나온다. `0005`를 찾은 것이 그 방법이었다.
