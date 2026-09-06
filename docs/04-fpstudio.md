# fpstudio

libfprint 위에 얹은 진단 도구. C++20 / Qt6.

이 도구가 존재하는 이유는 단순하다 — 이 센서를 `journalctl`로 디버깅하는 것이
실제 병목이었다. 점수가 왜 0인지, 이미지가 지문이긴 한지, 어느 단계에서 막혔는지를
**보여주는** 것이 목적이다.

---

## 빌드

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

필요: Qt 6.5+, CMake 3.21+, C++20 컴파일러, `libfprint-2` (pkg-config), glib2.

> `thirdparty/sigfm/sigfm.hpp`는 벤더링한 사본이다. 설치된 `fp-image.h`가
> `#include "sigfm/sigfm.hpp"`를 하는데 포크의 패키지가 그 헤더를 설치하지 않는다.
> opencv 의존 없는 선언부 97줄이라 사본으로 충분하다.

---

## 권한 구조

가장 빠른 길은 `src/fpstudio/99-fpstudio-goodix.rules` 를 넣는 것이다.
그러면 노드가 `crw-rw---- root wheel` 이 되어 `pkexec` 자체가 필요 없어진다.

```bash
sudo install -Dm644 src/fpstudio/99-fpstudio-goodix.rules \
     /etc/udev/rules.d/99-fpstudio-goodix.rules
sudo udevadm control --reload
sudo udevadm trigger --attr-match=idVendor=27c6
```

GUI 는 노드가 이미 쓰기 가능하면 `pkexec` 를 건너뛴다(`PrivRunner::deviceWritable()`).
규칙을 안 넣었다면 자동으로 `pkexec` 경로로 떨어지므로 둘 다 동작한다.

대가는 분명히 해두는 게 좋다 — wheel 로 도는 아무 프로세스나 센서를 직접
조작할 수 있게 된다(프레임 읽기, 이 드라이버에서는 PSK 쓰기까지). 1인 노트북
이라면 pkexec 로도 어차피 할 수 있던 사람이지만, 공용 기기라면 다르다.

### 왜 uaccess 가 아닌가

USB 직접 접근에는 root가 필요하다. libusb 가 `/dev/bus/usb/...` 에 **쓰기**를
요구하는데 그 노드는 `crw-rw-r-- root:root` 다.

`uaccess` 태그가 정석이다 — 시트에 로그인한 사용자에게만 ACL 을 주고 로그아웃
하면 회수한다. 이 시스템에서는 동작하지 않았다. 규칙은 매칭됐고
(`udevadm test` 로 `CURRENT_TAGS=:uaccess:` 확인), 세션은 seat0 에서 로컬·활성
이었고, unbind/bind 로 실제 add 이벤트까지 만들어 봤지만 systemd-logind 가
ACL 을 붙이지 않았다. 동작하지 않는 규칙을 넣어두느니 그룹을 주는 쪽을 택했다.

**GUI 는 사용자 권한으로 돌고, 장치 작업만 자기 자신을 `--cli` 모드로
`pkexec` 실행한다.** Qt/Wayland 전체를 root 로 끌고 가는 것보다 낫고,
CLI 가 이미 구조화된 결과를 내므로 두 프런트엔드가 갈라지지 않는다.

작업마다 인증을 묻는 건 번거로우므로, polkit 의 `auth_admin_keep` 유예를
켜두면 실질적으로 세션당 한 번이 된다.

```javascript
// /etc/polkit-1/rules.d/49-pkexec-auth-keep.rules
polkit.addRule(function(action, subject) {
    if (action.id == "org.freedesktop.policykit.exec" &&
        subject.isInGroup("wheel") && subject.local && subject.active) {
        return polkit.Result.AUTH_ADMIN_KEEP;
    }
});
```

유지 시간은 `/etc/polkit-1/polkitd.conf` 의 `[Polkitd] ExpirationSeconds`
로 정한다 (기본 300초).

`devices` 와 `delete` 는 장치를 열지 않으므로 `pkexec` 없이 직접 실행된다.

## 세 가지 모드

### GUI

```bash
fpstudio
```

- **이미지** — 캡처한 프레임. 88×108을 확대할 때 **보간을 끈다**. 작은 센서에서
  스무딩은 판단 근거인 융선 자체를 지운다
- **진행 단계** — 드라이버가 도달한 단계를 순서대로
- **매칭 점수** — 등록 샘플별 SIGFM 점수와 임계값(150)
- **드라이버 로그** — 실시간

libfprint의 sync API는 전부 블로킹이라 작업은 워커 스레드에서 돈다. "손가락
기다리는 중"에 창이 얼지 않는다.

### CLI — JSON

한 번 실행에 **JSON 객체 하나**를 stdout으로 낸다.

```bash
fpstudio --cli devices
fpstudio --cli capture [--out FILE] [--no-wait]
fpstudio --cli enroll  [--user U] [--finger N]
fpstudio --cli verify  [--user U] [--finger N]
fpstudio --cli list | delete [--user U]
```

`--finger`는 `FpFinger` 값이고 `7`이 오른쪽 검지다.

```console
$ fpstudio --cli devices
{"devices":[{"driver":"goodixtls55x4","enroll_stages":10,"id":"0",
  "name":"Goodix TLS Fingerprint Sensor 55X4","open":false,
  "scan_type":"press","supports_capture":true,"supports_identify":true}],"ok":true}

$ pkexec fpstudio --cli capture --out /tmp/frame.png
{"height":108,"ok":true,"saved":"/tmp/frame.png","width":88}
```

`--out`을 생략하면 PNG를 base64로 실어 보낸다. 파이프 반대편에 있는 호출자도
프레임을 볼 수 있게 하려는 것이다.

`verify`는 관측된 점수를 전부 낸다:

```json
{"ok":true,"matched":false,"best_score":0,"threshold":150,
 "scores":[{"score":0,"total":72}, ...]}
```

`best_score`가 임계값 근처인지 0인지는 완전히 다른 문제다 — 전자는 매칭 품질,
후자는 이미지가 상관관계를 못 만든다는 뜻이다.

### MCP

```bash
fpstudio --mcp
```

stdio 위의 JSON-RPC 2.0. `initialize`, `tools/list`, `tools/call` 구현.

노출 도구: `list_devices`, `capture_image`, `enroll`, `verify`,
`list_prints`, `delete_prints`.

MCP 클라이언트 등록 예 (설정 형식은 클라이언트마다 다르다):

```json
{
  "mcpServers": {
    "fpstudio": {
      "command": "pkexec",
      "args": ["/path/to/fpstudio", "--mcp"]
    }
  }
}
```

USB 직접 접근에 root가 필요하다. udev 규칙으로 시트에 권한을 주면 `pkexec` 없이
쓸 수 있다.

---

## MCP 지시가 화면에 뜬다

이 도구가 상정하는 흐름은 이렇다 — **에이전트가 MCP로 명령하고, 사용자는 화면만
보고 따라한다.** 대화창을 들여다보고 있지 않아도 된다.

```
에이전트 ──MCP──▶ fpstudio --mcp (root)
                      │
                      ├─▶ 센서 조작
                      └─▶ /tmp/fpstudio-status.json 에 지시문 기록
                                    │
                          fpstudio GUI (사용자 권한) 가 감시
                                    │
                                    ▼
                   ┌────────────────────────────────────┐
                   │ AI 에이전트 요청 · 손가락을 올려주세요 │
                   └────────────────────────────────────┘
```

파일을 쓰는 이유는 두 쪽의 **사용자가 다르기 때문**이다. MCP 서버는 root 로
돌고 GUI 는 시트 사용자로 도니 D-Bus 세션 버스를 공유하지 않는다. 내용은 단계
이름과 안내 문장뿐이라 감출 것이 없고, `QSaveFile` 이 rename 으로 바꿔치므로
읽는 쪽이 반쯤 쓰인 파일을 보는 일은 없다.

배너에는 **요청 출처**가 함께 뜬다(`AI 에이전트 요청` / `CLI 요청`). 사용자가
직접 누르지 않은 요청일수록 어디서 온 것인지가 중요하기 때문이다.

### 지시문과 진행 상태는 분리한다

```
┌────────────────────────────────────────────┐
│ AI 에이전트 요청 · 센서에 손가락을 올려주세요 │  ← 작업 내내 유지
└────────────────────────────────────────────┘
      ▸ 특징점을 추출하는 중…                    ← 계속 바뀜
```

한 줄로 합치면 뭔가 일어나는 순간 지시문이 사라진다. "손가락을 올리세요"가
"센서 준비 완료"로 바뀌면 사용자는 자기가 뭘 해야 하는지 잃는다. 그래서
`Beacon::begin()` 이 서 있는 지시문을 걸고 `Beacon::progress()` 는 그 아래
줄만 갈아끼운다.

상태 줄은 드라이버 로그에서 뽑는다. 특히 손가락 감지 여부가 중요하다 —
그게 없으면 **기다리는 중인 캡처와 멈춰버린 캡처가 화면상 구별되지 않는다.**

| 드라이버가 말하는 것 | 화면에 뜨는 것 |
|---|---|
| `FP_FINGER_STATUS_PRESENT` | 손가락 감지됨 — 그대로 유지해 주세요 |
| `finger status: off` | 손가락을 떼셨습니다 |
| `captured an image` | 프레임을 받았습니다 |
| `sigfm extract completed` | 캡처 완료 — 이제 손가락을 떼주세요 |
| `sigfm score` | 등록된 지문과 대조하는 중… |
| `HANDSHAKE DONE` | 센서와 보안 채널 연결됨 |
| 등록 진행 콜백 | 등록 3/10 — 손가락을 떼었다 다시 올려주세요 |

등록 진행도는 시그널이 아니라 훅으로 직접 전달한다. CLI 에는 블로킹 호출이
도는 동안 이벤트 루프가 없어서, 큐 연결로는 작업이 다 끝난 뒤에야 도착한다.

### 경과 시간과, 시간이 어디로 가는가

상태 줄에는 초 단위 경과가 함께 뜬다. GUI 가 로컬 타이머로 세는데, 드라이버가
손가락을 기다리는 동안에는 로그를 한 줄도 내지 않아 비콘만 보면 숫자가 멈추고
그러면 멈춘 프로그램처럼 보이기 때문이다.

실측한 한 번의 캡처 내역:

```
   248ms  센서 준비 완료
 11506ms  손가락 감지          ← 사용자가 올리기까지
 11546ms  이미지 캡처 완료      ← 감지 후 40ms
 18324ms  손가락 뗌 감지        ← 캡처 후 6.8초
 18598ms  종료
```

**캡처 자체는 40ms 다.** 나머지는 전부 사람을 기다리는 시간이고, 그중
"떼기를 기다리는 6.8초"는 사용자가 뗄 때라는 걸 몰라서 생기는 죽은 시간이었다.
`sigfm extract completed` 를 추출 안내가 아니라 **떼기 안내**로 쓰는 이유가
이것이다 — 추출은 70ms 만에 끝나고 그 직후가 정확히 뗄 시점이다.

읽는 쪽이 없어도 무방하다. GUI 가 안 떠 있으면 파일만 쓰이고 무시된다.

## 구조

```
src/
  engine.{h,cpp}       libfprint 래퍼 + GLib 로그 가로채기
  headless.cpp         CLI 와 MCP (특권 쪽에서 도는 실제 작업)
  privrunner.{h,cpp}   pkexec 로 자기 --cli 를 호출하고 JSON 을 되받음
  mainwindow.{h,cpp}   Qt GUI. 장치 상태를 갖지 않는다
  main.cpp             모드 분기
```

GUI 가 장치 상태를 갖지 않는 것이 설계의 요점이다. 창은 `PrivRunner` 를 통해
명령을 보내고 JSON 을 받아 그릴 뿐이라, GUI 로 보는 것과 CLI/MCP 로 보는 것이
같은 코드 경로에서 나온다.

핵심은 `engine.cpp`의 **로그 탭**이다. 드라이버가 아는 흥미로운 것들 — 도달한
활성화 단계, 샘플별 SIGFM 점수, TLS 실패 이유 — 은 전부 `fp_dbg()`로 나갔다가
버려진다. `g_log_set_writer_func`로 그 스트림을 받아 구조가 있는 줄
(`sigfm score N/M`)을 파싱한다. 사용자가 `journalctl`을 훑는 대신 UI가 숫자를
보여줄 수 있는 이유다.

---

## 저장 위치

등록한 지문은 `~/.local/share/fpstudio/<user>-<finger>.fp`에 둔다.
`fprintd`의 데이터베이스에 쓰지 않는다 — 진단 도구의 결과가 우리가 넣지 않은
상태에 의존하면 안 되기 때문이다.
