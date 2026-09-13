# 시스템 연동 — 지문과 비밀번호 동시 인증

드라이버가 동작하고 fpstudio 로 등록·검증이 되는 것과, 실제로 비밀번호를
대체하는 것은 별개다. 이 문서는 그 간극을 메우는 두 단계를 다룬다.

## 무엇이 어디에 저장되는가

fpstudio 는 libfprint 를 직접 쓰고 등록본을 `~/.local/share/fpstudio/` 에
둔다. 시스템 인증이 보는 것은 `fprintd` 데몬이고, 그쪽 저장소는
`/var/lib/fprint/<user>/<driver>/<device>/` 다. **두 곳은 공유되지 않는다.**
fpstudio 로 아무리 잘 등록해도 pkexec 는 그것을 모른다.

그래서 `fprintd-enroll` 로 한 번 더 등록해야 한다. 같은 드라이버를 쓰지만
데몬을 거치는 경로라 별도로 확인할 가치도 있다.

```
fprintd-delete "$USER"                    # 옛 등록본 정리
fprintd-enroll -f right-index-finger      # 드라이버가 정한 단계 수만큼
fprintd-verify                            # 데몬 경로 확인
```

`fprintd-list "$USER"` 로 장치와 등록 상태를 볼 수 있다.

## PAM

표준 `pam_fprintd`를 비밀번호 모듈 앞에 직렬로 두면 지문 작업이 끝날 때까지
비밀번호를 입력할 수 없다. FPStudio는 이 구성을 직접 복사하지 않고, 지원하는
PAM 원본 형태를 검사한 다음 동시입력 모듈을 트랜잭션으로 설치한다.

```
python tools/dual_auth_install.py
sudo python tools/dual_auth_install.py --apply
```

```
첫 명령은 읽기 전용 미리보기다. 적용 시 sudo/sudo-i, Polkit, 로컬 TTY login,
su/su-l 및 KDE 지문 서비스를 함께 검사한다. 각 원본은
`/var/backups/fpstudio-dual-auth/` 아래에 보존되며 하나라도 예상 구조와 다르면
아무 설정도 추측해서 고치지 않는다. 내부 오류는 성공으로 간주하지 않는
fail-closed 제어를 사용하면서 기존 `system-auth` 비밀번호 검증을 유지한다.

터미널에서는 키를 누르기 전까지 지문 일치가 즉시 인증을 완료한다. 키 입력을
시작하면 입력은 `*`로 가려지고 Enter를 눌러 기존 비밀번호 검증기로 제출된다.
KDE 잠금 화면은 별도의 비밀번호 서비스와 fingerprint-only 서비스를 병렬로
유지한다. SSH, 원격 로그인, 디스크 암호화 및 KWallet은 변경하지 않는다.

## 온도

`fprintd-enroll` 이 `enroll-retry-scan` 과 `enroll-finger-not-centered` 를
반복하면 화질이 실제로 나쁜 것이다. 드라이버가 매 캡처마다 수치를 남기므로
확인할 수 있다.

```
journalctl -n 2000 --no-pager | grep -oE "coverage [0-9]+%, sharpness [0-9]+"
```

한 번 관측된 사례:

```
coverage 51~71%,  sharpness 10~14    반복 실패하던 중
coverage 73~79%,  sharpness 24~35    직전에 성공했을 때
```

같은 시각 저널에 `FP_TEMPERATURE_HOT` 이 찍혀 있었다. libfprint 는 장치별
발열 모델을 돌리는데(`DEFAULT_TEMP_HOT_SECONDS` 3분,
`DEFAULT_TEMP_COLD_SECONDS` 9분), 이것은 과열 방지를 위한 **소프트웨어
추정치이지 온도계가 아니다.** 그러니 발열이 화질 저하의 원인이라는 것은
상관관계이지 확인된 인과가 아니다. 손끝이 건조해도 신호는 약해진다.

실용적으로는 둘 다 걷어내면 된다 — 몇 분 쉬고, 손끝에 습기를 준 뒤 다시.
비율 0.73 에서 WARM(0.5)까지 약 3.5분, COLD(0.27)까지 약 9분이다.
