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
