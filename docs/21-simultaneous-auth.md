# 지문 또는 비밀번호 동시 입력

기존 `pam_fprintd.so` → `system-auth` 순서는 지문 처리가 끝나야 비밀번호를
묻는다. `timeout=30`만 지정하면 동시 입력이 되지 않으며, 표준 모듈의 시간
제한은 재시도마다 다시 시작할 수 있다.

`src/pam`의 `pam_fpstudio.so`는 sudo, sudo-i, Polkit, 로컬 TTY login,
su, su-l의 지원되는 단일 프로세스 PAM 대화에서 비밀번호 프롬프트와 센서 검사를
함께 진행한다. 입력한 비밀번호는 PAM_AUTHTOK으로 기존 비밀번호 모듈에 전달하고,
검증은 그 모듈이 수행한다. 비밀번호 제출이 지문 성공과 겹치면 비밀번호 검증을
우선한다. 지문 일치가 없는 빈 입력·취소·잘못된 메시지는 지문 성공으로 처리하지 않는다.

CLI에서는 비밀번호 입력 도중 지문 성공으로 입력을 강제로 끝내지 않는다.
키 입력이 없으면 지문 일치 즉시 완료한다. 키를 한 번이라도 누르면 입력을
지우거나 방향키만 눌렀어도 **Enter로 완료**하는 상태를 유지한다.
지문 일치 후 입력 내용이 비어 있으면 Enter로 지문 인증을 완료한다.
입력 내용이 있으면 지문 일치와 무관하게 기존 비밀번호 검증을 수행한다.
이 구분 없이 자동 종료하면 이어서 입력한 비밀번호 뒷부분이 셸에 노출될 수 있다.
터미널 echo는 첫 안내 전부터 마지막 정리까지 끄고, 복원 시 입력 버퍼를 비운다.
정상 전경 제어 터미널이 아닌 파이프·`sudo -S` 등은 기존 비밀번호 경로를 쓴다.
`sudo -n`과 `sudo -A`도 sudo의 기존 경로를 유지한다. CLI는 빨간
`[Admin 인증]` 아래 `패스워드 or 지문입력:`을 표시하고 입력을 `*`로 가린다.
부모 프로세스만 실제 터미널 입력을 읽는다. 기본 PAM 비밀번호 대화는 사설
PTY에서 실행하며 그 출력을 실제 화면으로 전달하지 않는다. 입력은 Enter 뒤에만
그 대화로 전달한다. 비대화식 요청 거절과 기존 비밀번호 검증은 유지한다.
색상 미지원 터미널 또는 `NO_COLOR` 환경에서는 일반 텍스트를 사용한다.
KDE GUI는 숨김 입력을 처리하는 별도 대화창이므로 지문 자동 완료를 유지한다.

센서 작업자는 고정된 실행 파일로 별도 실행한다. 비밀번호는 전달하지 않으며,
시스템 fprintd의 등록 사용자와 VerifyStatus 결과만 사용한다. 전체 작업에
30초·최대 20회 제한을 적용한다. 준비·재시도·센서 오류·종료·성공 메시지를
PAM 정보 메시지로 보낸다. 호출자가 종료되면 사설 소켓 종료로 센서를 해제한다.

KDE 잠금 화면의 `kde-fingerprint`에는 같은 모듈의 `fingerprint-only` 모드를
사용한다. 별도 `kde` 비밀번호 서비스는 유지한다. 다중 스레드 앱의 비밀번호
대화를 fork하지 않으며, 지원 목록 밖 서비스와 원격 인증은 기존 경로로 넘긴다.
KDE 관리자 작업은 Polkit 경로를 사용한다. 그래픽 로그인 화면의 plasmalogin,
SSH, 디스크 암호화 및 KWallet 복호화는 이 설치 대상이 아니다.

데스크톱 적용은 `bash tools/activate_auth.sh` 또는 위저드의 기본 KDE Polkit
인증창을 사용한다. Konsole을 인증 UI로 실행하지 않는다. 비밀번호 필드의
라벨과 지문 안내는 분리하고, 터미널 상태 메시지는 독립된 줄에 출력한다.
11개 언어 중 메시지 로케일에 맞는 하나만 표시한다. PAM 안내는
`LC_ALL` → `LC_MESSAGES` → `LANG` 순서이며 미지원 언어는 영어로 표시한다.
GUI의 수동 언어 선택과 시스템 인증 언어는 별도 설정이다. 일부 고급 위저드의
기존 한국어 고정 문구는 아직 번역 대상에 편입되지 않았다.

## 빌드·시험·설치

```sh
cmake -S src/fpstudio -B src/fpstudio/build -DCMAKE_BUILD_TYPE=Release
cmake --build src/fpstudio/build --parallel
ctest --test-dir src/fpstudio/build --output-on-failure
python src/pam/test_tty.py src/fpstudio/build/pam
python src/pam/test_worker_bus.py src/fpstudio/build/pam/fpstudio-fprint-worker
python tools/dual_auth_install.py
sudo python tools/dual_auth_install.py --apply
```

기본 실행은 변경 차이만 출력한다. 적용은 지원하는 기존 PAM 구조를 확인하고,
모듈·작업자를 먼저 배치한 뒤 서비스 파일을 교체한다. 비밀번호·계정·세션
검사는 보존하며 지문보다 먼저 계정 잠금 여부를 확인한다. 원본 파일은
`/var/backups/fpstudio-dual-auth/install-*`에 보관하고 실패 시 되돌린다.

```sh
sudo python tools/dual_auth_install.py --rollback /var/backups/fpstudio-dual-auth/install-EXACT
```

마법사 PAM 적용 버튼도 이 설치 경로를 사용한다. 이미 열린 인증창은 기존
스택을 사용하므로 닫은 뒤 새 요청으로 확인한다. 자동 시험은 합성 자격 증명과
사설 D-Bus만 사용한다. 실제 사용자 지문·비밀번호 및 KDE 대화창에서의 성공은
배포 후 별도로 확인해야 한다. 매칭 임계값과 학습 데이터는 이 변경에 포함되지 않는다.

## 잠금 화면 확인

KDE 6.7.4의 `kde-fingerprint` 경로는 비밀번호 프롬프트 없이 지문 일치로
완료한다. 별도 스레드가 실행 중인 합성 PAM 시험에서 일치·센서 사용 불가·
시간 초과·잘못된 메시지를 검사하며 비밀번호 요청 횟수가 0인지 확인한다.

2026-09-09 배포 후 기본 KDE greeter의 `--testing --immediateLock` 모드에서
실제 센서 `verify-match`와 greeter의 `Unlocked` 출력을 확인했다. 이는 실제
PAM/센서 경로 시험이며, 테스트 모드이므로 세션 자체를 잠근 전체 주기의 시험과는
구분한다. 실제 잠금은 작업을 저장한 뒤 Meta+L로 별도 확인할 수 있다.
