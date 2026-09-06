# 시스템 연동 — 비밀번호 대신 지문

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

polkit 은 `/usr/lib/pam.d/polkit-1` 을 쓰고, 내용은 `system-auth` 를 네 번
include 하는 것이 전부다. Arch 에서는 `/etc/pam.d/` 가 이를 덮으므로 거기에
한 줄 얹은 사본을 두면 된다. `src/fpstudio/pam/polkit-1` 이 그 파일이다.

```
auth       sufficient   pam_fprintd.so
auth       include      system-auth
account    include      system-auth
password   include      system-auth
session    include      system-auth
```

```
sudo install -Dm644 src/fpstudio/pam/polkit-1 /etc/pam.d/polkit-1
```

되돌리는 것은 `sudo rm /etc/pam.d/polkit-1` 하나다. 데몬 재시작도 필요 없다 —
PAM 은 인증할 때마다 스택을 읽는다.

### 왜 polkit 에만 넣는가

없애고 싶은 비밀번호는 pkexec 와 KDE 인증 창이 띄우는 것들이다. 로그인과
sudo 까지 건드릴 이유가 없고, 건드리지 않으면 **센서가 죽어도 기계에서
잠기지 않는다.** 커널 업데이트로 드라이버가 깨지든, 리더를 뽑든, 이 저장소의
패치를 다시 빌드하다 실수하든, 로그인 경로는 영향을 받지 않는다.

`sufficient` 도 같은 이유다. `pam_fprintd` 가 어떤 이유로든 실패하면 PAM 은
`system-auth` 로 넘어가 예전과 똑같이 비밀번호를 묻는다. 되던 인증이 안 되게
만드는 경우가 없다.

등록된 지문이 없어도 마찬가지다 — 그냥 비밀번호를 묻는다. 그래서 PAM 파일을
먼저 깔고 등록을 나중에 해도 순서 문제가 없다.

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
