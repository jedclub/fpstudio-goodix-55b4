<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ko" sourcelanguage="en">
<context>
    <name>fpstudio</name>
    <message>
        <location filename="../src/engine.cpp" line="86"/>
        <source>Finger detected — hold still</source>
        <translation>손가락 감지됨 — 그대로 유지해 주세요</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="91"/>
        <source>Finger lifted</source>
        <translation>손가락을 떼셨습니다</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="104"/>
        <source>Only part of the sensor was touched — cover more of it, centred</source>
        <translation>일부만 닿았습니다 — 손가락을 센서 중앙에 더 넓게 대주세요</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="109"/>
        <source>The ridges are faint — press a little harder</source>
        <translation>지문 결이 흐립니다 — 조금 더 힘주어 눌러 주세요</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="114"/>
        <source>Frame received</source>
        <translation>프레임을 받았습니다</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="124"/>
        <location filename="../src/engine.cpp" line="134"/>
        <location filename="../src/headless.cpp" line="476"/>
        <source>Captured — now lift your finger</source>
        <translation>캡처 완료 — 이제 손가락을 떼주세요</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="139"/>
        <source>Matching against the enrolled print…</source>
        <translation>등록된 지문과 대조하는 중…</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="144"/>
        <location filename="../src/setupcheck.cpp" line="209"/>
        <source>Secure channel to the sensor established</source>
        <translation>센서와 보안 채널 연결됨</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="149"/>
        <source>Sensor ready</source>
        <translation>센서 준비 완료</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="417"/>
        <location filename="../src/engine.cpp" line="498"/>
        <location filename="../src/engine.cpp" line="574"/>
        <source>Gave up after %1 seconds</source>
        <translation>%1초 안에 끝나지 않아 중단했습니다</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="460"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>등록 %1/%2 — 손가락을 떼었다 다시 올려주세요</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <location filename="../src/headless.cpp" line="87"/>
        <location filename="../src/headless.cpp" line="436"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>센서에 손가락을 올리고 그대로 유지해 주세요</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="89"/>
        <location filename="../src/headless.cpp" line="437"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>등록 중입니다 — 손가락을 올렸다 떼기를 반복해 주세요</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="91"/>
        <location filename="../src/headless.cpp" line="438"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>등록한 손가락을 센서에 올려 주세요</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="64"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>Goodix 27c6:55b4 발견됨</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="73"/>
        <source>A different Goodix sensor is present</source>
        <translation>다른 Goodix 센서가 있습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>이 빌드는 27c6:55b4 만 지원합니다. 발견된 것: 27c6:%1.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="80"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>Goodix 지문 센서를 찾지 못했습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="81"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>USB 버스에서 벤더 27c6 에 응답하는 장치가 없습니다. 리더가 있는 기기라면 펌웨어 설정에서 비활성화되어 있거나 다른 제조사일 수 있습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="100"/>
        <source>Patched driver is active</source>
        <translation>패치된 드라이버가 활성화됨</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="107"/>
        <location filename="../src/setupcheck.cpp" line="115"/>
        <source>Patched driver is not installed</source>
        <translation>패치된 드라이버가 설치되어 있지 않습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="108"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint 이 이 센서를 인식하지 못합니다. 패치된 포크를 직접 빌드해서 설치해야 하는데, 이 프로그램 배포본에는 그 소스가 들어있지 않습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="116"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint 이 이 센서를 인식하지 못합니다. 패치된 포크가 지원을 추가합니다. 빌드는 몇 분 걸리고 시스템 libfprint 를 교체하므로 비밀번호가 필요합니다.

fprintd 는 이후에 설치하세요. 먼저 설치하면 공식 libfprint 가 딸려와 이 작업을 되돌립니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="121"/>
        <source>Build and install the patched libfprint</source>
        <translation>패치된 libfprint 빌드 및 설치</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="151"/>
        <source>Sensor is reachable without a password</source>
        <translation>비밀번호 없이 센서에 접근 가능합니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="157"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>udev 규칙이 설치되었지만 아직 적용되지 않았습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="158"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>모든 센서 작업이 비밀번호를 요구합니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="160"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>규칙 파일은 있지만 장치 노드가 여전히 root 소유입니다. 다음 add 이벤트에 적용되므로, 리더를 다시 꽂거나 udev 를 재시작하면 됩니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="163"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>규칙이 없으면 libusb 는 root 로만 센서를 열 수 있어, 모든 캡처가 비밀번호 창을 거칩니다.

이 규칙은 wheel 그룹에 센서 읽기/쓰기 권한을 줍니다. 1인용 노트북에서는 어차피 pkexec 로 할 수 있던 것과 같은 사람이지만, 공용 기계에서는 다릅니다. 건너뛰어도 안전합니다 — 비밀번호를 더 물을 뿐입니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="173"/>
        <source>Install the udev rule and reload</source>
        <translation>udev 규칙 설치 및 재적용</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="216"/>
        <source>The sensor answered, but the handshake did not complete</source>
        <translation>센서가 응답했지만 핸드셰이크가 완료되지 않았습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="217"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>센서가 이 드라이버에 없는 키를 갖고 있습니다. 다음 단계는 영(0) 키를 쓰는 것이고, 되돌릴 수 없습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="223"/>
        <source>Could not talk to the sensor</source>
        <translation>센서와 통신할 수 없습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="224"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>핸드셰이크가 원인을 알 만큼 진행되지 않았습니다. 먼저 위의 드라이버·권한 단계를 확인하세요.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="237"/>
        <source>The sensor already holds a usable key</source>
        <translation>센서가 이미 사용 가능한 키를 갖고 있습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="250"/>
        <source>The sensor needs the all-zero key written</source>
        <translation>센서에 영(0) 키를 써야 합니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="251"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>이 작업은 되돌릴 수 없습니다.

센서가 지금 갖고 있는 키는 읽어올 수 없습니다 — 프로토콜이 돌려주는 것은 키에서 파생된 값이지 키 자체가 아닙니다 — 그래서 나중에 복원할 백업이 없습니다.

이 기기의 윈도우 지문 로그인이 영구히 동작하지 않게 됩니다. 듀얼부팅으로 윈도우에서도 쓰고 있다면 여기서 멈추세요.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="259"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>영(0) PSK 를 센서에 쓰기</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="289"/>
        <source>Image quality has not been measured yet</source>
        <translation>이미지 품질을 아직 측정하지 않았습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="290"/>
        <source>Press the button below, then put your finger on the sensor and hold it there for a couple of seconds.</source>
        <translation>아래 버튼을 누른 뒤 손가락을 센서에 올리고 몇 초간 유지하세요.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="293"/>
        <source>Test a capture now</source>
        <translation>지금 캡처 테스트</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="297"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>접촉 %1%  ·  선명도 %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="306"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>24 미만이면 드라이버는 프레임을 받아도 매칭은 대체로 실패합니다. 손끝이 건조하거나 센서가 계속 사용 중이었을 때 이렇게 되며, 몇 분 쉬고 손끝에 습기를 주면 대개 해결됩니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="310"/>
        <source>Try again</source>
        <translation>다시 시도</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="323"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>fprintd 에 지문이 등록되어 있습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="329"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>fprintd 에 등록된 지문이 없습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="330"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>이 프로그램은 자체 등록본을 별도로 보관하며, 시스템 인증은 그것을 보지 못합니다. fprintd 로 다시 등록해야 지문으로 잠금 해제가 가능해집니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="334"/>
        <source>Enrol a finger with fprintd</source>
        <translation>fprintd 에 지문 등록</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="339"/>
        <source>fprintd is not answering</source>
        <translation>fprintd 가 응답하지 않습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="340"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>데몬에 접근할 수 없습니다. 보통은 필요할 때 자동으로 시작되므로, 설치만 하면 되는 경우가 많습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="387"/>
        <source>polkit accepts a fingerprint</source>
        <translation>polkit 이 지문을 받습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="395"/>
        <location filename="../src/setupcheck.cpp" line="438"/>
        <source>pam_fprintd is not installed</source>
        <translation>pam_fprintd 가 설치되어 있지 않습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="396"/>
        <location filename="../src/setupcheck.cpp" line="439"/>
        <source>The PAM module that lets authentication use a fingerprint is missing. It usually ships with fprintd.</source>
        <translation>인증에 지문을 쓸 수 있게 하는 PAM 모듈이 없습니다. 보통 fprintd 와 함께 제공됩니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="404"/>
        <source>polkit still asks for a password</source>
        <translation>polkit 이 여전히 비밀번호를 요구합니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="405"/>
        <source>This adds one line to the stack polkit uses, so pkexec and the desktop&apos;s authentication dialog try the fingerprint first.

Login and sudo are deliberately left alone, so a sensor that stops working can never lock you out of the machine. The line is &apos;sufficient&apos;: if the fingerprint fails for any reason, you are asked for the password exactly as before.</source>
        <translation>polkit 이 쓰는 인증 스택에 한 줄을 추가합니다. 그러면 pkexec 와 데스크톱 인증 창이 지문을 먼저 시도합니다.

로그인과 sudo 는 일부러 건드리지 않았습니다. 그래야 센서가 고장 나도 기계에서 잠기지 않습니다. 이 줄은 &apos;sufficient&apos; 이므로 지문이 어떤 이유로든 실패하면 예전과 똑같이 비밀번호를 묻습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="413"/>
        <source>Let polkit accept a fingerprint</source>
        <translation>polkit 이 지문을 받게 하기</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="430"/>
        <source>sudo accepts a fingerprint</source>
        <translation>sudo 가 지문을 받습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="451"/>
        <source>Terminal sudo still asks for a password only</source>
        <translation>터미널 sudo 는 아직 비밀번호만 받습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="452"/>
        <source>Optional, and a step further than the polkit rule above: sudo is usually the way back in when something else on the machine is broken, so extending fingerprint auth to it is worth doing deliberately rather than by default.

The safety net is the same either way. The line added is &apos;sufficient&apos;, so a failing fingerprint falls back to the password exactly as before. sudo itself never stops working - only the fingerprint shortcut can.</source>
        <translation>선택 사항이며, 위 polkit 규칙보다 한 걸음 더 나아간 것입니다. sudo 는 기계의 다른 무언가가 고장 났을 때 되돌아가는 통로인 경우가 많아서, 여기에 지문 인증을 확장하는 건 기본값이 아니라 신중하게 결정할 일입니다.

안전장치는 똑같습니다. 추가되는 줄은 &apos;sufficient&apos; 이므로, 지문이 실패하면 예전처럼 비밀번호로 넘어갑니다. sudo 자체는 절대 멈추지 않습니다 — 지문이라는 지름길만 멈출 수 있습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="462"/>
        <source>Let sudo accept a fingerprint</source>
        <translation>sudo 가 지문을 받게 하기</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="492"/>
        <source>Sensor</source>
        <translation>센서</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="493"/>
        <source>Driver</source>
        <translation>드라이버</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="494"/>
        <source>Permissions</source>
        <translation>권한</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="495"/>
        <source>Secure channel</source>
        <translation>보안 채널</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="496"/>
        <source>Sensor key</source>
        <translation>센서 키</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="497"/>
        <source>Image quality</source>
        <translation>이미지 품질</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="498"/>
        <source>Enrolment</source>
        <translation>등록</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="499"/>
        <source>Unlocking</source>
        <translation>잠금 해제</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="500"/>
        <source>Terminal sudo (optional)</source>
        <translation>터미널 sudo (선택 사항)</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="521"/>
        <source>Not checked - no sensor</source>
        <translation>확인 안 됨 — 센서 없음</translation>
    </message>
</context>
<context>
    <name>fpstudio::ImageView</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="57"/>
        <source>No frame yet

Use Capture to pull an image from the sensor</source>
        <translation>아직 프레임 없음

Capture 로 센서 이미지를 가져옵니다</translation>
    </message>
</context>
<context>
    <name>fpstudio::LiveWindow</name>
    <message>
        <location filename="../src/livewindow.cpp" line="110"/>
        <source>Vulkan GPU is initializing — it calculates separately from the live stream</source>
        <translation>Vulkan GPU를 초기화하는 중 — 실시간 영상과 별도로 계산합니다</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="119"/>
        <source>%1 · %2 references ready · waiting for a stable candidate</source>
        <translation>%1 · 기준 %2장 준비 · 안정된 후보 영상 대기</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="146"/>
        <source>GPU comparison error: </source>
        <translation>GPU 비교 오류: </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="151"/>
        <source>GPU %1 hypotheses · %2 ms · image correlation %3 · ridge-direction agreement %4
Best: %5 / %6 — research comparison only; not an authentication decision</source>
        <translation>GPU %1개 조건 · %2 ms · 영상 상관 %3 · 융선 방향 일치 %4
최상위: %5 / %6 — 연구용 비교이며 인증 판정이 아닙니다</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="155"/>
        <source>
Alignment is ambiguous: a different position has a similar score</source>
        <translation>
정렬 모호함: 다른 위치에도 비슷한 점수가 있습니다</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="156"/>
        <source>
At the search boundary: move the finger slightly toward the centre</source>
        <translation>
탐색 범위 경계: 손가락을 중앙 쪽으로 조금 옮겨 보세요</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="159"/>
        <source>
Interior ridge support %1/%2 regions · %3</source>
        <translation>
내부 융선 지지 %1/%2 영역 · %3</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="161"/>
        <source>ridges are consistent (identity unverified)</source>
        <translation>융선 일관성 있음 (신원 미검증)</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="161"/>
        <source>insufficient match evidence</source>
        <translation>일치 근거 부족</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="164"/>
        <source> · 0/1 contact-region overlap %1%</source>
        <translation> · 0/1 접촉영역 교집합 %1%</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="166"/>
        <source> · 360° additional search applied</source>
        <translation> · 360° 추가 탐색 적용</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="168"/>
        <source> · full reference-bank search</source>
        <translation> · 전체 기준군 탐색</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="168"/>
        <source> · representative-reference fast comparison</source>
        <translation> · 대표 기준 빠른 비교</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="184"/>
        <source>GPU comparison cannot run: </source>
        <translation>GPU 비교를 실행할 수 없습니다: </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="245"/>
        <source>Vulkan GPU is comparing reference images, angles, and positions in parallel…</source>
        <translation>Vulkan GPU가 기준 영상·각도·위치를 병렬 비교 중입니다…</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="250"/>
        <source>GPU comparison timed out — live collection continues</source>
        <translation>GPU 비교 시간이 초과되었습니다 — 실시간 수집은 계속됩니다</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="322"/>
        <source>Contact %1 · image from comparison %2 (not live)
Gray: background (0) · yellow: contact (1), but weak features · green/red: comparison evidence
Cyan +: transformed reference contact centre · orange +: current probe contact centre · blue: comparison range</source>
        <translation>접촉 %1 · 비교 %2 당시 영상 (실시간 아님)
회색: 배경(0) · 노랑: 접촉(1)이지만 특징 부족 · 초록/빨강: 비교 근거
하늘색 +: 변환된 기준 접촉중심 · 주황 +: 현재 검사 접촉중심 · 파랑: 비교 범위</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="508"/>
        <source>[%1/%2 contacts compared · stops automatically at the target] </source>
        <translation>[%1/%2접촉 비교 완료 · 목표에 도달하면 자동 종료] </translation>
    </message>
</context>
<context>
    <name>fpstudio::MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="108"/>
        <location filename="../src/mainwindow.cpp" line="355"/>
        <source>Device scan</source>
        <translation>장치 검색</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="153"/>
        <source>Setup</source>
        <translation>설정</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="154"/>
        <source>Set up fingerprint unlock…</source>
        <translation>지문 잠금 해제 설정…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="157"/>
        <source>Language</source>
        <translation>언어</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="194"/>
        <source>fpstudio — fingerprint sensor diagnostics</source>
        <translation>fpstudio — 지문 센서 진단</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="199"/>
        <source>Looking for a device…</source>
        <translation>장치를 찾는 중…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="203"/>
        <source>Device</source>
        <translation>장치</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="205"/>
        <source>Rescan</source>
        <translation>다시 찾기</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="210"/>
        <source>Capture — show image</source>
        <translation>Capture — 이미지 보기</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="211"/>
        <location filename="../src/mainwindow.cpp" line="364"/>
        <source>Enrol</source>
        <translation>등록</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="212"/>
        <location filename="../src/mainwindow.cpp" line="369"/>
        <source>Verify</source>
        <translation>검증</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="213"/>
        <location filename="../src/mainwindow.cpp" line="373"/>
        <source>Delete enrolment</source>
        <translation>등록 삭제</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="218"/>
        <source>Actions</source>
        <translation>동작</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="225"/>
        <source>Progress</source>
        <translation>진행</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="257"/>
        <source>—</source>
        <translation>—</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="294"/>
        <source>Coverage</source>
        <translation>접촉</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="296"/>
        <source>Sharpness</source>
        <translation>선명도</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Sample</source>
        <translation>샘플</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Score</source>
        <translation>점수</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Keypoints</source>
        <translation>특징점</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="313"/>
        <source>Match scores</source>
        <translation>매칭 점수</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="323"/>
        <source>Driver log</source>
        <translation>드라이버 로그</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="358"/>
        <source>→ Put your finger on the sensor</source>
        <translation>→ 센서에 손가락을 올려주세요</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="359"/>
        <source>Image capture</source>
        <translation>이미지 캡처</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="362"/>
        <source>→ Enrol: press and lift your finger repeatedly</source>
        <translation>→ 등록: 손가락을 올렸다 떼기를 반복해 주세요</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="367"/>
        <source>→ Verify: present the enrolled finger</source>
        <translation>→ 검증: 등록한 손가락을 올려주세요</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source>in progress…</source>
        <translation>진행 중…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source> in progress…</source>
        <translation> 진행 중…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="412"/>
        <source>No fingerprint device was found.</source>
        <translation>지문 장치를 찾지 못했습니다.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="421"/>
        <source>Driver: %1
ID: %2
Scan: %3 · %4 enrol stages
Raw capture: %5</source>
        <translation>드라이버: %1
ID: %2
스캔: %3 · 등록 %4단계
원본 캡처: %5</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>yes</source>
        <translation>지원</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>no</source>
        <translation>미지원</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="438"/>
        <source>No image was returned</source>
        <translation>이미지를 받지 못했습니다</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="442"/>
        <source>Captured %1 × %2</source>
        <translation>캡처 %1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="461"/>
        <source>Match   %1 / %2</source>
        <translation>일치   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="462"/>
        <source>No match   %1 / %2</source>
        <translation>불일치   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="473"/>
        <source>Failed — %1</source>
        <translation>실패 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="474"/>
        <location filename="../src/mainwindow.cpp" line="632"/>
        <location filename="../src/mainwindow.cpp" line="633"/>
        <source>Failed</source>
        <translation>실패</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="475"/>
        <location filename="../src/mainwindow.cpp" line="634"/>
        <source>Error: %1</source>
        <translation>오류: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="479"/>
        <source>Done — %1</source>
        <translation>완료 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484"/>
        <source>Enrolment complete</source>
        <translation>등록 완료</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="486"/>
        <source>Enrolment deleted</source>
        <translation>등록 삭제됨</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513"/>
        <source>This scan fell below the threshold and was asked for again</source>
        <translation>이 스캔은 기준 미달로 다시 요청되었습니다</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="528"/>
        <source>Coverage %1%  ·  sharpness %2  —  faint. Press a little harder</source>
        <translation>접촉 %1%  ·  선명도 %2  —  흐립니다. 조금 더 눌러 주세요</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="530"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>접촉 %1%  ·  선명도 %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="551"/>
        <source>%1 × %2</source>
        <translation>%1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="607"/>
        <source>Agent request</source>
        <translation>AI 에이전트 요청</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="608"/>
        <source>CLI request</source>
        <translation>CLI 요청</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="608"/>
        <source>request</source>
        <translation>요청</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="624"/>
        <source>Idle</source>
        <translation>대기 중</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="625"/>
        <source>▸ %1   ·   %2s elapsed</source>
        <translation>▸ %1   ·   %2초 경과</translation>
    </message>
</context>
<context>
    <name>fpstudio::PrivRunner</name>
    <message>
        <location filename="../src/privrunner.cpp" line="22"/>
        <source>Could not run pkexec</source>
        <translation>pkexec 를 실행할 수 없습니다</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="31"/>
        <source>The operation terminated abnormally</source>
        <translation>작업이 비정상 종료했습니다</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="38"/>
        <source>Authentication was cancelled or refused</source>
        <translation>인증이 취소되었거나 거부되었습니다</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="63"/>
        <source>Could not parse the result (exit code %1)</source>
        <translation>결과를 파싱하지 못했습니다 (종료 코드 %1)</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="108"/>
        <source>An operation is already running</source>
        <translation>이미 작업이 실행 중입니다</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="112"/>
        <source>Could not resolve this program&apos;s own path</source>
        <translation>실행 파일 경로를 확인할 수 없습니다</translation>
    </message>
</context>
<context>
    <name>fpstudio::SetupWizard</name>
    <message>
        <location filename="../src/setupwizard.cpp" line="355"/>
        <source>Failed — %1</source>
        <translation>실패 — %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="125"/>
        <source>Skip</source>
        <translation>건너뛰기</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="140"/>
        <source>Finish</source>
        <translation>마침</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="187"/>
        <source>Open diagnostics tool</source>
        <translation>진단 도구 열기</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="190"/>
        <source>Live capture preview, the driver log, and MCP status - for a closer look once the checklist here is done.</source>
        <translation>실시간 캡처 미리보기, 드라이버 로그, MCP 상태를 볼 수 있습니다 — 이 체크리스트를 마친 뒤 더 자세히 볼 때 씁니다.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="300"/>
        <source>This step cannot be undone.</source>
        <translation>이 단계는 되돌릴 수 없습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="316"/>
        <source>This will ask for your password.</source>
        <translation>비밀번호를 물어봅니다.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="329"/>
        <source>WRITE</source>
        <translation>WRITE</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="331"/>
        <source>Sensor key</source>
        <translation>센서 키</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="332"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

확인하려면 %2 를 입력하세요.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="356"/>
        <source>The enrolment helper was not found</source>
        <translation>등록 도우미를 찾지 못했습니다</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="362"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>등록 중입니다 — 손가락을 올렸다 떼기를 반복해 주세요</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="402"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>센서에 손가락을 올리고 그대로 유지해 주세요</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="472"/>
        <source>in progress…</source>
        <translation>진행 중…</translation>
    </message>
</context>
</TS>
