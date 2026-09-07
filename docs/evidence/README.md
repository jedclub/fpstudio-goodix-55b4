# evidence

제조사 드라이버에서 **추출한 사실**만 둔다. 원본 배포 파일(`WbdiUsb.inf`,
Lenovo readme 등)은 저작권이 있는 제3자 배포물이라 재배포하지 않는다.

재현하려면 Lenovo 카탈로그에서 직접 받으면 된다:

```bash
curl -O https://download.lenovo.com/catalog/20U7_Win10.xml     # 머신타입별 패키지 목록
curl -O https://download.lenovo.com/pccbbs/mobiles/r19gf02w.exe  # Goodix 지문 드라이버
sha256sum r19gf02w.exe   # B3071EBE4DEB20866D8EE6CD66CDF9DBA043181C37E9010538E6CE1056A7E3D9
innoextract -s r19gf02w.exe
```

| 파일 | 내용 |
|---|---|
| `Wbdi.dll-sensor-models.txt` | `Wbdi.dll`에서 뽑은 센서 모델·펌웨어 문자열 |
| `device-readings.txt` | 이 기기의 조회 응답 (고유 식별자는 가림) |
| `windows-driver-metadata.md` | 로컬 INF와 Lenovo 설명서에서 확인한 패키지 정보 |

홈폴더의 이전 작업 자료를 통합한 내역은
[`../08-local-materials.md`](../08-local-materials.md)에 기록했다.
제조사 원본과 장치 고유 값은 Git에서 제외되는 `local-private/`에만 보관한다.
