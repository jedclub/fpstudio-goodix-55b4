# 인증 관측성과 장애 분석

FPStudio 인증 로깅은 한 번의 sudo/KDE/TTY 인증을 세 계층에서 관측한다.

1. `pam_fpstudio`: 요청 진입, 안전상 건너뛴 이유, 워커 실행, 터미널 입력 모드,
   접촉/재시도/일치, 비밀번호 PAM 전달 및 종료 결과
2. `fpstudio-fprint-worker`: system bus 연결, 등록 지문 확인, 센서 Claim,
   VerifyStart/VerifyStatus, 손가락 접촉·해제, 재초기화와 종료 단계
3. `fprintd` 매처: 캡처 coverage/sharpness, GPU 수치 판정, 새 Vulkan helper로
   재초기화, SIGFM 폴백과 점수

PAM과 워커는 `fpstudio_auth session=<opaque-id>` 형식의 동일 ID를 사용한다.
fprintd는 시스템 데몬이라 PAM 환경을 받지 않으므로 리포터가 활성 인증 시간
구간에 속하는 매처·드라이버 레코드를 결합한다. 동시에 여러 인증이 겹치면 가장
가까운 시작 시각에 배정하며, 확실히 배정할 수 없는 레코드는 `unscoped`로 센다.

## 개인정보 경계

로그에는 상태 이름, 시도 횟수, 소요 시간, 정수/부동소수 품질 지표와 오류 단계만
기록한다. 다음 항목은 기록하지 않는다.

- 비밀번호 및 입력 길이
- 사용자명과 홈 디렉터리
- 지문 원본/가공 영상
- 템플릿, 특징점 및 갤러리 파일명·경로

리포터도 원본 journal 메시지를 그대로 재출력하지 않고 허용된 필드만 파싱한다.

## 사용

```sh
fpstudio-auth-report --since "30 minutes ago"
fpstudio-auth-report --since today --json
journalctl --since today -o json | fpstudio-auth-report --input - --json
```

사람이 읽는 기본 출력은 세션별 결과, 접촉/재시도 수, 캡처 품질과 진단 항목을
보여준다. JSON 출력은 장기간의 실패 유형 집계나 이슈 첨부 자동화에 사용한다.
시스템 journal 읽기 권한이 없는 계정은 관리자 권한 또는 `systemd-journal`
그룹 정책이 필요할 수 있다.
