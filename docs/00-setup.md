# 처음부터 끝까지 — 지문으로 비밀번호 대체하기

이 문서는 아무것도 안 된 상태에서 pkexec 가 지문을 받기까지의 전체 순서다.
각 단계마다 **무엇을 확인하고, 무엇을 하고, 됐는지 어떻게 아는지**를 적었다.
fpstudio 의 설정 마법사가 하는 일이 정확히 이것이므로, 이 문서는 그 명세이기도
하다.

되돌릴 수 없는 단계가 하나 있다(4단계 PSK). 거기만 주의하면 나머지는 전부
되돌릴 수 있다.

---

## 0. 장치 확인

```
lsusb -d 27c6:
```

`27c6:55b4` 가 보여야 한다. 다른 제품 ID 라면 이 저장소는 맞지 않는다.

칩 식별은 `docs/02-device.md` 를 보라. 요약하면 이 보드는 **GF3208** 이고,
상류 드라이버는 GF3268 을 상대로 쓰였다. 그 차이가 3·4번 패치의 이유다.

이 센서는 펌웨어 버전 문자열을 **두 가지로 번갈아 답한다** —
`GF3268_RTSEC_APP_10042` 와 `GF3208_RTSEC_APP_10042`. 둘 다 프로토콜
체크섬을 통과하므로 전송 오류가 아니다. 패치는 양쪽을 다 받아들인다.

## 1. 드라이버

공식 `libfprint` 에는 이 센서 지원이 없다. 이 저장소의 패치 12개를 얹은
포크를 빌드해서 넣는다.

```
cd src/driver
makepkg -f
sudo pacman -U libfprint-goodixtls-55x4-fixed-*.pkg.tar.zst
sudo pacman -S fprintd            # 드라이버 설치 후에
```

**순서가 중요하다.** `fprintd` 를 먼저 넣으면 공식 `libfprint` 가 딸려온다.

패치가 무엇을 고치는지는 `docs/03-driver.md` 와 `docs/05-geometry-regression.md`
에 있다. 셋만 짚으면:

- `0005` 는 상류 버그다. `err_from_ssl()` 이 1바이트 덜 잡아서 SSL 오류를
  보고하려는 순간 프로세스가 죽는다. 이걸 고쳐야 진짜 오류가 보인다.
- `0009` 는 정규화다. 접촉하지 않은 영역이 흑점을 잡아 능선을 위쪽 절반에
  눌러놓고 있었다. 이것 하나로 매칭 점수가 5에서 63으로 올랐다.
- `0011` 은 등록 품질 게이트다. 반쯤 닿은 프레임이 등록본 한 칸을 차지하고
  영원히 기여하지 못하는 것을 막는다.

확인:

```
fpstudio --cli devices
```

`"driver":"goodixtls55x4"` 가 나오면 된다.

## 2. 장치 권한

libusb 는 쓰기 권한이 필요한데 기본은 `root:root 0664` 다. 매 작업마다
pkexec 를 거치지 않으려면 규칙을 넣는다.

```
sudo install -Dm644 src/fpstudio/99-fpstudio-goodix.rules /etc/udev/rules.d/
sudo udevadm control --reload
sudo udevadm trigger --attr-match=idVendor=27c6
```

`TAG+="uaccess"` 가 원래 맞는 방법이지만 이 시스템에서는 동작하지 않았다 —
규칙은 매칭되고 세션도 seat0 에서 활성인데 logind 가 ACL 을 적용하지 않았다.
그래서 그룹 부여로 대체했다. 자세한 것은 규칙 파일의 주석에 있다.

이것을 넣으면 wheel 그룹 프로세스가 센서를 직접 조작할 수 있다. 1인용
노트북에서는 pkexec 로 이미 할 수 있던 것과 같은 사람이지만, 공용 기계에서는
다르다. CLI/MCP 만 pkexec 아래서 쓸 거라면 이 단계는 건너뛰어도 된다.

## 3. TLS 세션

드라이버는 센서와 TLS-PSK 로 통신한다. 핸드셰이크는 32바이트 영(0) 키를
쓰는데, 센서가 그 키를 갖고 있지 않으면 실패한다.

```
fpstudio --cli capture --timeout 5
```

`HANDSHAKE DONE` 까지 가면 이미 되는 것이니 4단계는 필요 없다.

## 4. PSK 쓰기 — 되돌릴 수 없다

3단계가 실패하면 센서에 영 키를 써야 한다. **이 단계는 되돌릴 수 없다.**

센서가 현재 갖고 있는 키는 읽을 수 없다. `PRESET_PSK_READ` 가 돌려주는 것은
키가 아니라 키에서 파생된 값이다. 그래서 덮어쓰기 전에 백업할 방법이 없고,
한 번 쓰면 **윈도우의 지문 인증이 더는 동작하지 않는다.**

윈도우를 쓸 계획이 있다면 여기서 멈춰야 한다.

절차는 `src/firmware/README.md` 에 있다.

## 5. 진단

```
fpstudio                      # 창을 열고 Capture
```

정상이면 대각선 능선이 선명한 108×88 이미지가 나온다. 이미지 아래 막대가
접촉률과 선명도를 보여준다.

**선명도가 성공을 예측한다.** 실측에서 21은 검증에 실패했고 28~30은
임계값 72를 20배 넘겨 통과했다. 24 미만이면 창이 경고한다.

줄무늬가 보이거나 능선이 없으면 `docs/05-geometry-regression.md` 를 보라.
거기에 무엇을 재서 무엇을 알아냈는지가 있다.

## 6. 등록

fpstudio 와 fprintd 는 **저장소를 공유하지 않는다.** fpstudio 는
`~/.local/share/fpstudio/` 에, fprintd 는 `/var/lib/fprint/` 에 둔다.
시스템 인증이 보는 것은 후자이므로, 최종 목적이 비밀번호 대체라면
fprintd 쪽에 등록해야 한다.

```
src/fpstudio/fprintd-beacon.sh right-index-finger
```

`fprintd-enroll` 을 그대로 쓰면 터미널에만 안내가 나오고 센서 앞 화면에는
아무것도 안 뜬다. 이 래퍼가 fpstudio 의 배너에 안내를 띄운다.

단계 수는 드라이버가 정한다(현재 15). 품질 미달 스캔은 단계를 소비하지 않고
다시 요청된다.

## 7. PAM

```
sudo install -Dm644 src/fpstudio/pam/polkit-1 /etc/pam.d/polkit-1
```

polkit 에만 넣는다. 로그인과 sudo 를 건드리지 않으므로 센서가 죽어도 기계에서
잠기지 않는다. `sufficient` 이므로 지문이 실패하면 예전처럼 비밀번호를 묻는다.

되돌리기는 `sudo rm /etc/pam.d/polkit-1` 하나다.

### 선택 — 터미널 sudo 도 지문으로

```
sudo install -Dm644 src/fpstudio/pam/sudo /etc/pam.d/sudo
```

기본값에는 포함하지 않았다. sudo 는 기계의 다른 무언가가 고장 났을 때
되돌아가는 통로인 경우가 많아서, 여기까지 확장하는 건 신중히 결정할 일이라고
봤다. 안전장치는 polkit 과 동일하다 — `sufficient` 이므로 지문이 실패하면
예전처럼 비밀번호를 묻는다. sudo 자체가 멈추는 일은 없다.

되돌리기는 `sudo rm /etc/pam.d/sudo` 하나다.

### 재시도 횟수

두 파일 모두 `pam_fprintd.so max-tries=20 timeout=90` 을 쓴다. 기본값은
3회/30초인데, 이 센서의 실측 성공률(7회 중 5회, §5 참고)로는 3회면 실패하는
경우가 드물지 않다. 20회로 올리면 매번 성공률은 그대로여도 전부 실패할
확률이 0.3^20 수준으로 떨어져 사실상 항상 성공한다.

### 터미널에서 보이는 것 — 정상 흐름

`pam_fprintd` 는 자체 진행률 표시가 없다. 정확히 알아야 답답하지 않다.

```
1. "Place your finger on the fingerprint reader" 가 한 번 뜬다
2. 손가락을 올렸는데 인식에 실패하면 -> 새 줄로 재시도 문구가 뜬다
   (Swipe your finger again / try touching the sensor again 등)
3. 아무것도 안 뜨는 동안은 "입력을 기다리는 중"이다 - 화면이 멈춘 게 아니다
4. 20번 다 실패하거나 90초가 지나면 -> 비밀번호 프롬프트로 넘어간다
```

경과 시간이나 "20번 중 3번째" 같은 카운터는 없다. `pam_fprintd` 는 이
프로젝트가 만든 것이 아니라 배포판이 관리하는 시스템 인증 모듈이라, 여기에
진행률 UI를 넣으려면 그 모듈 자체를 패치해서 다시 빌드해야 한다. 인증 핵심
모듈은 실수의 파급이 커서 이 프로젝트의 범위 밖으로 남겨뒀다. fpstudio 가
직접 그리는 실시간 막대·경과 시간은 이 프로젝트가 전체를 통제하는
등록/검증 화면(§5, §6)에서만 가능한 것이고, sudo/pkexec 의 인증 대화 자체는
그 모듈의 몫이다.

## 8. 확인

```
busctl call org.freedesktop.PolicyKit1 /org/freedesktop/PolicyKit1/Authority \
  org.freedesktop.PolicyKit1.Authority RevokeTemporaryAuthorizations \
  "(sa{sv})" "unix-session" 1 "session-id" s "$XDG_SESSION_ID"
pkexec /usr/bin/id -u
```

먼저 임시 인증을 취소해야 의미가 있다. polkit 은 한 번 인증하면 일정 시간
다시 묻지 않으므로, 취소하지 않으면 지문이 동작하는지 알 수 없다.

인증 창에서 손가락을 올려 `0` 이 나오면 끝이다. 저널에서 경로를 확인할 수
있다:

```
journalctl -n 200 --since "-2min" | grep -E "fprintd.*verify|pam_unix\(polkit"
```

---

## 잘 안 될 때

**등록이 계속 거부된다.** 화질이 실제로 나쁜 것이다. 확인:

```
journalctl -n 2000 | grep -oE "coverage [0-9]+%, sharpness [0-9]+"
```

정상은 접촉 73~79% / 선명도 24~35다. 10~14 가 나오면 두 가지를 의심한다 —
손끝이 건조하거나, 센서가 달아올랐거나. libfprint 는 발열 모델을 돌리고
(`FP_TEMPERATURE_HOT`) 냉각 시상수는 9분이다. 다만 그것은 과열 방지용
소프트웨어 추정치이지 온도계가 아니므로, 발열이 원인이라는 것은 확인된
인과가 아니다. 몇 분 쉬고 손끝에 습기를 주면 두 가설을 한 번에 걷어낸다.

**검증이 되다 안 되다 한다.** 정상이다. 실측 성공률은 70~80% 수준이고, 창이
5.5 × 4.5mm 로 작아 등록본 어디와도 겹치지 않는 위치로 올라오면 0점이 된다.
점수 분포가 극단적인 것도 그 때문이다 — 성공은 77~2920, 실패는 0이나 7이다.
실패해도 비밀번호로 폴백되므로 잠기지 않는다.

**화질을 더 올리려면.** 남은 것은 칩별 OTP 보정(DAC/tcode)이다. 상류는 읽기
호출을 주석 처리한 채 콜백을 쓴 적이 없고, 우리가 구현해 읽어보니 명령
`0xa6` 이 주는 32바이트는 MCU 자신의 OTP 였다. 아날로그 보정이 든 것은
벤더가 SPI 로 읽는 64바이트 쪽이고, 센서 레지스터 공간을 `0x8000` 까지
훑어도 거기에는 없다. 벤더의 비트뱅잉 SPI 시퀀스를 해독해야 한다.
`docs/05-geometry-regression.md` 의 OTP 절에 근거가 있다.
