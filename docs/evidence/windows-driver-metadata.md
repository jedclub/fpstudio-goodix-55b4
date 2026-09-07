# Windows 드라이버 패키지 확인 기록

2026-09-06, 기존 로컬 자료 `WbdiUsb.inf`와
`lenovo-r19gf02w-readme.txt`를 직접 대조했다. 다음은 해당 버전의 기록이며,
현재 배포되는 최신 버전이나 Linux 호환성을 의미하지 않는다.

| 항목 | 확인한 값 | 근거 |
|---|---|---|
| 패키지 | r19gf02w, Goodix FingerPrint Driver 3.1.55.600 | Lenovo 설명서 |
| INF 드라이버 날짜 | 2021-07-22 | `DriverVer` |
| 지원 USB 장치 | `27C6:55B4` | `MyDevice_Install` 장치 매핑 |
| 지원 모델 | ThinkPad L14 Gen 1: 20U5, 20U6 / L15 Gen 1: 20U7, 20U8 | Lenovo 설명서 |
| 명시된 OS | Windows 10 64-bit 1809 이상, Windows 11 21H2 이상 | Lenovo 설명서 |
| 해당 릴리스의 변경점 | Windows 11 지원 추가 | Lenovo 설명서 |
| 카탈로그 | `WbdiUsb.cat` | `CatalogFile` |
| 지문 드라이버 바이너리 | `UMDF/wbdi.dll` | `ServiceBinary` |
| 관련 서비스 바이너리 | `WUDFRd.sys`, `SessionService.exe`, `WinUSB.sys` | `ServiceBinary` |

이 INF는 해당 USB ID의 Windows 드라이버 지원 근거다. GF3208/GF3268 중
실제 다이를 판별하는 근거로 사용하지 않는다.
원본 재취득 절차는 [evidence 안내](README.md)에 있다.
