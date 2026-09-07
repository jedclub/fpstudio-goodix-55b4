# 이전 로컬 작업 자료 통합

2026-09-06 홈폴더의 `fpstudio/`와 `goodix-55b4-linux-local/`를
현재 프로젝트와 비교했다. 원본 폴더는 삭제하거나 수정하지 않았다.

## fpstudio

이전 폴더에는 CMake 설정, 소스 7개, SIGFM 헤더, 빌드 산출물이 있다.
현재 `src/fpstudio/`와 파일별로 비교한 결과:

- `src/headless.h`와 `thirdparty/sigfm/sigfm.hpp`는 현재 파일과 바이트 단위로 같다.
- `engine`의 장치 열거·열기·캡처·등록·검증·등록본 관리 기능은 현재에도 있다.
  현재 구현은 타임아웃, 진행·품질 콜백, 검증 로그 보존을 추가했다.
- `headless.cpp`의 CLI 및 MCP 기능은 현재에도 있다. 현재 구현에는 설정 진단,
  로그 전달, 실제 드라이버 임계값 사용, 작업 상태 표시가 추가되어 있다.
- 이전 GUI는 같은 프로세스의 작업 스레드에서 장치를 열고 유지한다.
  현재 GUI는 `PrivRunner`를 통해 CLI 자식 프로세스로 작업을 실행한다.
  이전의 별도 열기 버튼과 Worker를 복원하면 현재 권한 분리 구조와 충돌한다.
- 현재 시작점과 CMake 설정은 위저드, 번역, 권한 실행 및 상태 표시 모듈을
  포함한다. 이전 설정으로 교체하지 않는다.

현재 구현에 복원할 독립적인 수정은 확인하지 못했다. 다만 이전 소스 중
일부는 현재 Git 이력에서 동일한 blob을 찾지 못했으므로 비교 자료로 보존한다.
`local-private/imported-home/fpstudio/`에는 소스와 CMake 설정만 복사했다.
옛 경로가 들어 있는 CMake 캐시와 실행 파일 등 `build/`는 가져오지 않았다.
앱을 빌드할 때는 계속 `src/fpstudio/`를 사용한다.

## goodix-55b4-linux-local

| 원본 파일 | 처리 |
|---|---|
| `0004-goodix55x4-accept-gf3208-psk.patch` | 현재 `src/driver/patches/` 파일과 동일. 중복 복사하지 않음 |
| `WbdiUsb.inf` | 원본은 비공개 보관, 확인한 사실은 evidence 문서로 통합 |
| `lenovo-r19gf02w-readme.txt` | 원본은 비공개 보관, 확인한 사실은 evidence 문서로 통합 |
| `device-secrets.txt` | OTP, 쓰기 전 공장 PSK 해시, 전체 머신타입을 비공개 보관 |

제조사 자료에서 추가로 확인한 정보는
[Windows 드라이버 메타데이터](evidence/windows-driver-metadata.md)에 정리했다.
장치 고유 값은 기존 `evidence/device-readings.txt`의 가림 처리를 유지한다.
공장 PSK 해시는 키 원본이나 복원 가능한 백업이 아니다.

## 로컬 보관 위치

`local-private/imported-home/goodix-55b4-linux-local/`에 위 세 원본을 보관한다.
`local-private/`는 `.gitignore`로 제외하며 디렉터리는 소유자만 접근 가능하게,
복사한 파일은 소유자만 읽고 쓸 수 있게 설정한다.
Git clone에는 이 자료가 포함되지 않는다. 필요하면 별도의 비공개 백업에 포함하고,
`git add -f`로 공개 저장소에 넣지 않는다.

검증 범위는 소스 비교, 원본과 복사본의 바이트 일치, Git 제외 및 파일 권한이다.
실행 코드는 변경하지 않았으며 장치 조작이나 인식률 시험은 하지 않았다.
