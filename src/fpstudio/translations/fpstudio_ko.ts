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
        <location filename="../src/headless.cpp" line="482"/>
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
        <location filename="../src/setupcheck.cpp" line="210"/>
        <source>Secure channel to the sensor established</source>
        <translation>센서와 보안 채널 연결됨</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="149"/>
        <source>Sensor ready</source>
        <translation>센서 준비 완료</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="418"/>
        <location filename="../src/engine.cpp" line="499"/>
        <location filename="../src/engine.cpp" line="575"/>
        <source>Gave up after %1 seconds</source>
        <translation>%1초 안에 끝나지 않아 중단했습니다</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="461"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>등록 %1/%2 — 손가락을 떼었다 다시 올려주세요</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <location filename="../src/headless.cpp" line="93"/>
        <location filename="../src/headless.cpp" line="442"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>센서에 손가락을 올리고 그대로 유지해 주세요</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="95"/>
        <location filename="../src/headless.cpp" line="443"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>등록 중입니다 — 손가락을 올렸다 떼기를 반복해 주세요</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="97"/>
        <location filename="../src/headless.cpp" line="444"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>등록한 손가락을 센서에 올려 주세요</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="65"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>Goodix 27c6:55b4 발견됨</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>A different Goodix sensor is present</source>
        <translation>다른 Goodix 센서가 있습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="75"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>이 빌드는 27c6:55b4 만 지원합니다. 발견된 것: 27c6:%1.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="81"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>Goodix 지문 센서를 찾지 못했습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="82"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>USB 버스에서 벤더 27c6 에 응답하는 장치가 없습니다. 리더가 있는 기기라면 펌웨어 설정에서 비활성화되어 있거나 다른 제조사일 수 있습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="101"/>
        <source>Patched driver is active</source>
        <translation>패치된 드라이버가 활성화됨</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="108"/>
        <location filename="../src/setupcheck.cpp" line="116"/>
        <source>Patched driver is not installed</source>
        <translation>패치된 드라이버가 설치되어 있지 않습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="109"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint 이 이 센서를 인식하지 못합니다. 패치된 포크를 직접 빌드해서 설치해야 하는데, 이 프로그램 배포본에는 그 소스가 들어있지 않습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="117"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint 이 이 센서를 인식하지 못합니다. 패치된 포크가 지원을 추가합니다. 빌드는 몇 분 걸리고 시스템 libfprint 를 교체하므로 비밀번호가 필요합니다.

fprintd 는 이후에 설치하세요. 먼저 설치하면 공식 libfprint 가 딸려와 이 작업을 되돌립니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="122"/>
        <source>Build and install the patched libfprint</source>
        <translation>패치된 libfprint 빌드 및 설치</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="152"/>
        <source>Sensor is reachable without a password</source>
        <translation>비밀번호 없이 센서에 접근 가능합니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="158"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>udev 규칙이 설치되었지만 아직 적용되지 않았습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="159"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>모든 센서 작업이 비밀번호를 요구합니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="161"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>규칙 파일은 있지만 장치 노드가 여전히 root 소유입니다. 다음 add 이벤트에 적용되므로, 리더를 다시 꽂거나 udev 를 재시작하면 됩니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="164"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>규칙이 없으면 libusb 는 root 로만 센서를 열 수 있어, 모든 캡처가 비밀번호 창을 거칩니다.

이 규칙은 wheel 그룹에 센서 읽기/쓰기 권한을 줍니다. 1인용 노트북에서는 어차피 pkexec 로 할 수 있던 것과 같은 사람이지만, 공용 기계에서는 다릅니다. 건너뛰어도 안전합니다 — 비밀번호를 더 물을 뿐입니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="174"/>
        <source>Install the udev rule and reload</source>
        <translation>udev 규칙 설치 및 재적용</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="196"/>
        <source>Device communication has not been tested yet</source>
        <translation>장치 통신은 아직 시험하지 않았습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="197"/>
        <source>Close other authentication requests, then run the image test. Opening or refreshing the wizard never claims the sensor automatically.</source>
        <translation>다른 인증 요청을 닫고 영상 시험을 실행하세요. 위저드를 열거나 새로고침할 때 센서를 자동 점유하지 않습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="198"/>
        <source>Test sensor communication and image capture</source>
        <translation>센서 통신·영상 시험</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="217"/>
        <source>The sensor answered, but the handshake did not complete</source>
        <translation>센서가 응답했지만 핸드셰이크가 완료되지 않았습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="218"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>센서가 이 드라이버에 없는 키를 갖고 있습니다. 다음 단계는 영(0) 키를 쓰는 것이고, 되돌릴 수 없습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="224"/>
        <source>Could not talk to the sensor</source>
        <translation>센서와 통신할 수 없습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="225"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>핸드셰이크가 원인을 알 만큼 진행되지 않았습니다. 먼저 위의 드라이버·권한 단계를 확인하세요.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="238"/>
        <source>The sensor already holds a usable key</source>
        <translation>센서가 이미 사용 가능한 키를 갖고 있습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="244"/>
        <source>Key status is unknown — automatic writing is disabled</source>
        <translation>키 변경 필요 여부 미확인 — 자동 쓰기 금지</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="245"/>
        <source>A USB error or a busy sensor is not treated as a key mismatch. Complete the communication test first.</source>
        <translation>USB 오류나 센서 점유를 키 불일치로 간주하지 않습니다. 먼저 통신 시험을 완료하세요.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="251"/>
        <source>The sensor needs the all-zero key written</source>
        <translation>센서에 영(0) 키를 써야 합니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="252"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>이 작업은 되돌릴 수 없습니다.

센서가 지금 갖고 있는 키는 읽어올 수 없습니다 — 프로토콜이 돌려주는 것은 키에서 파생된 값이지 키 자체가 아닙니다 — 그래서 나중에 복원할 백업이 없습니다.

이 기기의 윈도우 지문 로그인이 영구히 동작하지 않게 됩니다. 듀얼부팅으로 윈도우에서도 쓰고 있다면 여기서 멈추세요.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="260"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>영(0) PSK 를 센서에 쓰기</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="290"/>
        <source>Image quality has not been measured yet</source>
        <translation>이미지 품질을 아직 측정하지 않았습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="291"/>
        <source>Press the button below, then put your finger on the sensor and hold it there for a couple of seconds.</source>
        <translation>아래 버튼을 누른 뒤 손가락을 센서에 올리고 몇 초간 유지하세요.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="294"/>
        <source>Test a capture now</source>
        <translation>지금 캡처 테스트</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="298"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>접촉 %1%  ·  선명도 %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="307"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>24 미만이면 드라이버는 프레임을 받아도 매칭은 대체로 실패합니다. 손끝이 건조하거나 센서가 계속 사용 중이었을 때 이렇게 되며, 몇 분 쉬고 손끝에 습기를 주면 대개 해결됩니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="311"/>
        <source>Try again</source>
        <translation>다시 시도</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="324"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>fprintd 에 지문이 등록되어 있습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="330"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>fprintd 에 등록된 지문이 없습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="331"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>이 프로그램은 자체 등록본을 별도로 보관하며, 시스템 인증은 그것을 보지 못합니다. fprintd 로 다시 등록해야 지문으로 잠금 해제가 가능해집니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="335"/>
        <source>Enrol a finger with fprintd</source>
        <translation>fprintd 에 지문 등록</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="340"/>
        <source>fprintd is not answering</source>
        <translation>fprintd 가 응답하지 않습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="341"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>데몬에 접근할 수 없습니다. 보통은 필요할 때 자동으로 시작되므로, 설치만 하면 되는 경우가 많습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="358"/>
        <source>GPU authentication installed · %1 references</source>
        <translation>GPU 인증 연결 설치됨 · 기준 %1장</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="359"/>
        <source>This is experimental authentication. A valid installation does not by itself prove successful sudo/KDE authentication. Recovery location: %1</source>
        <translation>실험적 인증입니다. 설치가 정상이어도 실제 sudo/KDE 인증 성공은 별도로 확인해야 합니다. 복구 위치: %1</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="363"/>
        <source>GPU authentication engine needs an update</source>
        <translation>GPU 인증 엔진 업데이트 필요</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="364"/>
        <source>Deploy the latest GPU authentication engine</source>
        <translation>최신 GPU 인증 엔진 배포</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="365"/>
        <source>The existing %1 references, fprintd enrolment and PAM settings are preserved. Only the matcher, shader and driver bridge are replaced, and the update is applied only after reference self-checks pass.</source>
        <translation>기존 기준 %1장·fprintd 등록·PAM 설정은 보존합니다. 최신 매처·셰이더·드라이버 브리지만 교체하고 기준 영상 자기 검사가 통과할 때만 적용합니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="369"/>
        <source>Connect saved fingerprints and the GPU engine to system authentication</source>
        <translation>저장 지문과 GPU 엔진을 시스템 인증에 연결</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="370"/>
        <source>Import saved fingerprints and connect sudo/KDE</source>
        <translation>저장 지문 가져오기 · sudo/KDE 연결</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="371"/>
        <source>Select a folder containing your own saved fingerprints. Existing fprintd enrolment is preserved and the references are imported into root-only storage. sudo, administrator dialogs, TTY login and the KDE lock screen are connected with a 20-attempt/60-second limit and a password path. Rejection of other fingers remains experimental.</source>
        <translation>본인의 저장 지문 폴더를 선택합니다. 기존 fprintd 등록은 보존하고 기준 데이터는 root 전용 저장소로 가져옵니다. sudo·관리자 창·TTY 로그인·KDE 잠금 화면을 최대 20회 실패/60초 제한 및 비밀번호 경로와 함께 연결합니다. 다른 손가락 거절 성능은 아직 실험적입니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="382"/>
        <source>KDE fingerprint and password paths are configured in parallel</source>
        <translation>KDE 지문·비밀번호 병렬 경로 설정됨</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="382"/>
        <source>Simultaneous fingerprint and password input needs configuration</source>
        <translation>지문·비밀번호 동시 입력 설정 필요</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="383"/>
        <source>Fingerprint scanning runs for up to 60 seconds or 20 failed contacts and reports ready, retry and final states. KDE&apos;s separate password field remains available. Confirm actual lock-screen unlocking separately.</source>
        <translation>지문 검사는 최대 60초 또는 실패 접촉 20회까지 실행하며 준비·재시도·종료 상태를 표시합니다. KDE의 별도 비밀번호 입력란은 계속 사용할 수 있습니다. 실제 잠금 해제는 별도로 확인하세요.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="384"/>
        <source>Enable simultaneous fingerprint and password input</source>
        <translation>지문·비밀번호 동시 입력 적용</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="399"/>
        <source>Administrator dialogs accept fingerprint and password simultaneously</source>
        <translation>관리자 창 지문·비밀번호 동시 입력 설정됨</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="406"/>
        <source>Administrator dialogs are not connected to simultaneous authentication</source>
        <translation>관리자 창이 동시 인증에 연결되지 않았습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="407"/>
        <source>The guarded PAM module scans a fingerprint while keeping the normal password path available. It backs up every changed file, validates the KDE password service first, and fails closed on module errors. The sensor is limited to 60 seconds or 20 failed contacts.</source>
        <translation>보호된 PAM 모듈이 일반 비밀번호 경로를 유지하면서 지문을 검사합니다. 변경 파일을 모두 백업하고 KDE 비밀번호 서비스를 먼저 검증하며 모듈 오류 시 안전하게 거부합니다. 센서는 최대 60초 또는 실패 접촉 20회로 제한됩니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="409"/>
        <source>Connect administrator dialogs to fingerprint and password</source>
        <translation>관리자 창에 지문·비밀번호 연결</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="425"/>
        <source>sudo accepts fingerprint and password simultaneously</source>
        <translation>sudo 지문·비밀번호 동시 입력 설정됨</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="432"/>
        <source>Terminal sudo is not connected to simultaneous authentication</source>
        <translation>터미널 sudo가 동시 인증에 연결되지 않았습니다</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="433"/>
        <source>The terminal keeps input hidden and accepts either a completed fingerprint match or an explicitly submitted password. Typing selects the password path; otherwise a fingerprint match completes immediately. The original PAM files are backed up before the guarded module is installed.</source>
        <translation>터미널 입력은 숨겨지며 완료된 지문 일치 또는 명시적으로 제출한 비밀번호를 받습니다. 키 입력을 시작하면 비밀번호 경로를 선택하고, 입력하지 않으면 지문 일치 즉시 완료됩니다. 보호 모듈 설치 전에 기존 PAM 파일을 백업합니다.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="435"/>
        <source>Connect sudo to fingerprint and password</source>
        <translation>sudo에 지문·비밀번호 연결</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="495"/>
        <source>Terminal sudo</source>
        <translation>터미널 sudo</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="496"/>
        <source>Saved fingerprints · GPU authentication</source>
        <translation>저장 지문 · GPU 인증 연결</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="497"/>
        <source>KDE lock screen</source>
        <translation>KDE 잠금 화면</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="487"/>
        <source>Sensor</source>
        <translation>센서</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="488"/>
        <source>Driver</source>
        <translation>드라이버</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="489"/>
        <source>Permissions</source>
        <translation>권한</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="490"/>
        <source>Secure channel</source>
        <translation>보안 채널</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="491"/>
        <source>Sensor key</source>
        <translation>센서 키</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="492"/>
        <source>Image quality</source>
        <translation>이미지 품질</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="493"/>
        <source>Enrolment</source>
        <translation>등록</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="494"/>
        <source>Unlocking</source>
        <translation>잠금 해제</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="516"/>
        <source>Not checked - no sensor</source>
        <translation>확인 안 됨 — 센서 없음</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="102"/>
        <source>Unexpected image size</source>
        <translation>영상 크기 오류</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="134"/>
        <source>Waiting for a finger</source>
        <translation>손가락 대기</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="135"/>
        <source>Insufficient contact area</source>
        <translation>접촉 면적 부족</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="136"/>
        <source>Insufficient ridge contrast</source>
        <translation>무늬 대비 부족</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="137"/>
        <source>Insufficient consistent ridge pattern</source>
        <translation>일관된 무늬 부족</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="138"/>
        <source>Waiting for motion to settle</source>
        <translation>움직임 안정 대기</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="139"/>
        <source>Research candidate</source>
        <translation>연구 후보</translation>
    </message>
</context>
<context>
    <name>fpstudio::ImageView</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="58"/>
        <source>No frame yet

Use Capture to pull an image from the sensor</source>
        <translation>아직 프레임 없음

Capture 로 센서 이미지를 가져옵니다</translation>
    </message>
</context>
<context>
    <name>fpstudio::LiveWindow</name>
    <message>
        <location filename="../src/livewindow.cpp" line="189"/>
        <source>Vulkan GPU is initializing — it calculates separately from the live stream</source>
        <translation>Vulkan GPU를 초기화하는 중 — 실시간 영상과 별도로 계산합니다</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="198"/>
        <source>%1 · %2 references ready · waiting for a stable candidate</source>
        <translation>%1 · 기준 %2장 준비 · 안정된 후보 영상 대기</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="225"/>
        <source>GPU comparison error: </source>
        <translation>GPU 비교 오류: </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="230"/>
        <source>GPU %1 hypotheses · %2 ms · image correlation %3 · ridge-direction agreement %4
Best: %5 / %6 — research comparison only; not an authentication decision</source>
        <translation>GPU %1개 조건 · %2 ms · 영상 상관 %3 · 융선 방향 일치 %4
최상위: %5 / %6 — 연구용 비교이며 인증 판정이 아닙니다</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="234"/>
        <source>
Alignment is ambiguous: a different position has a similar score</source>
        <translation>
정렬 모호함: 다른 위치에도 비슷한 점수가 있습니다</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="235"/>
        <source>
At the search boundary: move the finger slightly toward the centre</source>
        <translation>
탐색 범위 경계: 손가락을 중앙 쪽으로 조금 옮겨 보세요</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="238"/>
        <source>
Interior ridge support %1/%2 regions · %3</source>
        <translation>
내부 융선 지지 %1/%2 영역 · %3</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="240"/>
        <source>ridges are consistent (identity unverified)</source>
        <translation>융선 일관성 있음 (신원 미검증)</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="240"/>
        <source>insufficient match evidence</source>
        <translation>일치 근거 부족</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="243"/>
        <source> · 0/1 contact-region overlap %1%</source>
        <translation> · 0/1 접촉영역 교집합 %1%</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="245"/>
        <source> · 360° additional search applied</source>
        <translation> · 360° 추가 탐색 적용</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="247"/>
        <source> · full reference-bank search</source>
        <translation> · 전체 기준군 탐색</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="247"/>
        <source> · representative-reference fast comparison</source>
        <translation> · 대표 기준 빠른 비교</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="263"/>
        <source>GPU comparison cannot run: </source>
        <translation>GPU 비교를 실행할 수 없습니다: </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="324"/>
        <source>Vulkan GPU is comparing reference images, angles, and positions in parallel…</source>
        <translation>Vulkan GPU가 기준 영상·각도·위치를 병렬 비교 중입니다…</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="329"/>
        <source>GPU comparison timed out — live collection continues</source>
        <translation>GPU 비교 시간이 초과되었습니다 — 실시간 수집은 계속됩니다</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1017"/>
        <source>Contact %1 · image from comparison %2 (not live)
Gray: background (0) · yellow: contact (1), but weak features · green/red: comparison evidence
Cyan +: transformed reference contact centre · orange +: current probe contact centre · blue: comparison range</source>
        <translation>접촉 %1 · 비교 %2 당시 영상 (실시간 아님)
회색: 배경(0) · 노랑: 접촉(1)이지만 특징 부족 · 초록/빨강: 비교 근거
하늘색 +: 변환된 기준 접촉중심 · 주황 +: 현재 검사 접촉중심 · 파랑: 비교 범위</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1360"/>
        <source>[%1/%2 contacts compared · stops automatically at the target] </source>
        <translation>[%1/%2접촉 비교 완료 · 목표에 도달하면 자동 종료] </translation>
    </message>
</context>
<context>
    <name>fpstudio::MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="109"/>
        <location filename="../src/mainwindow.cpp" line="363"/>
        <source>Device scan</source>
        <translation>장치 검색</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="154"/>
        <source>Setup</source>
        <translation>설정</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="155"/>
        <source>Set up fingerprint unlock…</source>
        <translation>지문 잠금 해제 설정…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="158"/>
        <source>Language</source>
        <translation>언어</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="195"/>
        <source>fpstudio — fingerprint sensor diagnostics</source>
        <translation>fpstudio — 지문 센서 진단</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="200"/>
        <source>Looking for a device…</source>
        <translation>장치를 찾는 중…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="204"/>
        <source>Device</source>
        <translation>장치</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="206"/>
        <source>Rescan</source>
        <translation>다시 찾기</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="211"/>
        <source>Capture — show image</source>
        <translation>Capture — 이미지 보기</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="212"/>
        <location filename="../src/mainwindow.cpp" line="373"/>
        <source>Enrol</source>
        <translation>등록</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="213"/>
        <location filename="../src/mainwindow.cpp" line="379"/>
        <source>Verify</source>
        <translation>검증</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="214"/>
        <location filename="../src/mainwindow.cpp" line="383"/>
        <source>Delete enrolment</source>
        <translation>등록 삭제</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="219"/>
        <source>Actions</source>
        <translation>동작</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="226"/>
        <source>Progress</source>
        <translation>진행</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="258"/>
        <source>—</source>
        <translation>—</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="295"/>
        <source>Coverage</source>
        <translation>접촉</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="297"/>
        <source>Sharpness</source>
        <translation>선명도</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Sample</source>
        <translation>샘플</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Score</source>
        <translation>점수</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Keypoints</source>
        <translation>특징점</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="314"/>
        <source>Match scores</source>
        <translation>매칭 점수</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="324"/>
        <source>Driver log</source>
        <translation>드라이버 로그</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="366"/>
        <source>→ Put your finger on the sensor</source>
        <translation>→ 센서에 손가락을 올려주세요</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="367"/>
        <source>Image capture</source>
        <translation>이미지 캡처</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="370"/>
        <source>→ Enrol: press and lift your finger repeatedly</source>
        <translation>→ 등록: 손가락을 올렸다 떼기를 반복해 주세요</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="377"/>
        <source>→ Verify: present the enrolled finger</source>
        <translation>→ 검증: 등록한 손가락을 올려주세요</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="394"/>
        <source>in progress…</source>
        <translation>진행 중…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="394"/>
        <source> in progress…</source>
        <translation> 진행 중…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="422"/>
        <source>No fingerprint device was found.</source>
        <translation>지문 장치를 찾지 못했습니다.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="431"/>
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
        <location filename="../src/mainwindow.cpp" line="436"/>
        <source>yes</source>
        <translation>지원</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="436"/>
        <source>no</source>
        <translation>미지원</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="448"/>
        <source>No image was returned</source>
        <translation>이미지를 받지 못했습니다</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="452"/>
        <source>Captured %1 × %2</source>
        <translation>캡처 %1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="471"/>
        <source>Match   %1 / %2</source>
        <translation>일치   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="472"/>
        <source>No match   %1 / %2</source>
        <translation>불일치   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="483"/>
        <source>Failed — %1</source>
        <translation>실패 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484"/>
        <location filename="../src/mainwindow.cpp" line="642"/>
        <location filename="../src/mainwindow.cpp" line="643"/>
        <source>Failed</source>
        <translation>실패</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="485"/>
        <location filename="../src/mainwindow.cpp" line="644"/>
        <source>Error: %1</source>
        <translation>오류: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="489"/>
        <source>Done — %1</source>
        <translation>완료 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="494"/>
        <source>Enrolment complete</source>
        <translation>등록 완료</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="496"/>
        <source>Enrolment deleted</source>
        <translation>등록 삭제됨</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="523"/>
        <source>This scan fell below the threshold and was asked for again</source>
        <translation>이 스캔은 기준 미달로 다시 요청되었습니다</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="538"/>
        <source>Coverage %1%  ·  sharpness %2  —  faint. Press a little harder</source>
        <translation>접촉 %1%  ·  선명도 %2  —  흐립니다. 조금 더 눌러 주세요</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="540"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>접촉 %1%  ·  선명도 %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="561"/>
        <source>%1 × %2</source>
        <translation>%1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="617"/>
        <source>Agent request</source>
        <translation>AI 에이전트 요청</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="618"/>
        <source>CLI request</source>
        <translation>CLI 요청</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="618"/>
        <source>request</source>
        <translation>요청</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="634"/>
        <source>Idle</source>
        <translation>대기 중</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="635"/>
        <source>▸ %1   ·   %2s elapsed</source>
        <translation>▸ %1   ·   %2초 경과</translation>
    </message>
</context>
<context>
    <name>fpstudio::PrivRunner</name>
    <message>
        <location filename="../src/privrunner.cpp" line="32"/>
        <source>Could not run pkexec</source>
        <translation>pkexec 를 실행할 수 없습니다</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="47"/>
        <source>The operation did not finish in time and was stopped</source>
        <translation>작업이 제한 시간 안에 끝나지 않아 중단했습니다</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="52"/>
        <source>The operation terminated abnormally</source>
        <translation>작업이 비정상 종료했습니다</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="59"/>
        <source>Authentication was cancelled or refused</source>
        <translation>인증이 취소되었거나 거부되었습니다</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="84"/>
        <source>Could not parse the result (exit code %1)</source>
        <translation>결과를 파싱하지 못했습니다 (종료 코드 %1)</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="147"/>
        <source>An operation is already running</source>
        <translation>이미 작업이 실행 중입니다</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="151"/>
        <source>Could not resolve this program&apos;s own path</source>
        <translation>실행 파일 경로를 확인할 수 없습니다</translation>
    </message>
</context>
<context>
    <name>fpstudio::SetupWizard</name>
    <message>
        <location filename="../src/setupwizard.cpp" line="405"/>
        <source>Failed — %1</source>
        <translation>실패 — %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="150"/>
        <source>Skip</source>
        <translation>건너뛰기</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="92"/>
        <source>FPStudio · Integrated fingerprint authentication setup</source>
        <translation>FPStudio · 지문 인증 통합 설정</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="151"/>
        <source>Check status again</source>
        <translation>상태 다시 확인</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="183"/>
        <source>Setup steps complete · Verify real sudo/KDE authentication and password recovery separately.</source>
        <translation>설정 단계 완료 · 실제 sudo/KDE 인증과 비밀번호 복구 시험은 별도 확인이 필요합니다.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="185"/>
        <source>Finish</source>
        <translation>마침</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="216"/>
        <source>Stop</source>
        <translation>중단</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="219"/>
        <source>Stopping the fingerprint test. You may lift your finger.</source>
        <translation>지문 시험을 중단합니다. 손을 떼셔도 됩니다.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="222"/>
        <location filename="../src/setupwizard.cpp" line="632"/>
        <source>Restore authentication settings</source>
        <translation>인증 설정 복구</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="223"/>
        <source>Test system fingerprint</source>
        <translation>시스템 지문 시험</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="237"/>
        <source>Open diagnostics tool</source>
        <translation>진단 도구 열기</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="240"/>
        <source>Live capture preview, the driver log, and MCP status - for a closer look once the checklist here is done.</source>
        <translation>실시간 캡처 미리보기, 드라이버 로그, MCP 상태를 볼 수 있습니다 — 이 체크리스트를 마친 뒤 더 자세히 볼 때 씁니다.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="274"/>
        <source>Checking the device and installation. Keep your finger off the sensor for now.</source>
        <translation>장치와 설치 상태 확인 중입니다. 손가락은 아직 대지 마세요.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="350"/>
        <source>This step cannot be undone.</source>
        <translation>이 단계는 되돌릴 수 없습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="366"/>
        <source>This will ask for your password.</source>
        <translation>비밀번호를 물어봅니다.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="368"/>
        <source>This step is confirmed. Continue to the next required step.</source>
        <translation>이 단계는 확인됐습니다. 다음 필요한 단계로 진행하세요.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="369"/>
        <source>Next action: %1</source>
        <translation>다음 행동: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="369"/>
        <source>Review the guidance. This step is not confirmed yet.</source>
        <translation>안내를 확인하세요. 아직 완료로 판정하지 않은 단계입니다.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="379"/>
        <source>WRITE</source>
        <translation>WRITE</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="381"/>
        <source>Sensor key</source>
        <translation>센서 키</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="382"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

확인하려면 %2 를 입력하세요.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="406"/>
        <source>The enrolment helper was not found</source>
        <translation>등록 도우미를 찾지 못했습니다</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="412"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>등록 중입니다 — 손가락을 올렸다 떼기를 반복해 주세요</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="420"/>
        <source>Enrolment in progress · Present and lift the same finger as instructed
</source>
        <translation>등록 진행 · 안내에 따라 같은 손가락을 대고 떼세요
</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="425"/>
        <source>Could not start the enrolment tool: %1</source>
        <translation>등록 도구를 시작하지 못했습니다: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="456"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>센서에 손가락을 올리고 그대로 유지해 주세요</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="465"/>
        <source>Could not start the image tool: %1</source>
        <translation>영상 도구를 시작하지 못했습니다: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="529"/>
        <source>in progress…</source>
        <translation>진행 중…</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="536"/>
        <source>Wait for the operation to finish or cancel its authentication request. Setup records and recovery information are being protected.</source>
        <translation>작업이 끝나거나 인증 요청이 취소될 때까지 기다려 주세요. 설정 기록과 복구 정보를 보호하고 있습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="540"/>
        <source>Finish the running setup operation or cancel its authentication request before closing.</source>
        <translation>실행 중인 설정 작업을 완료하거나 인증 요청을 취소한 뒤 닫아 주세요.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="554"/>
        <source>This step did not finish in time and was stopped. The sensor and your enrolment are unchanged - try it again.</source>
        <translation>이 단계가 제한 시간 안에 끝나지 않아 중단했습니다. 센서와 등록된 지문은 그대로이니 다시 시도하세요.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="575"/>
        <source>Setup operation complete. Use the system fingerprint test to verify the real path.</source>
        <translation>설정 작업 완료. 시스템 지문 시험으로 실제 경로를 확인하세요.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="575"/>
        <source>Setup incomplete or cancelled: %1</source>
        <translation>설정 미완료 또는 취소: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="581"/>
        <source>After administrator authorization, installation and checks run automatically. Authentication settings are restored if installation fails.</source>
        <translation>관리자 인증 후 설치와 검사를 자동 실행합니다. 인증 연결 설치가 실패하면 원래 설정을 복구합니다.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="582"/>
        <source>The driver is built as the current user. Administrator authorization is requested only for dependencies and package installation.</source>
        <translation>드라이버는 일반 사용자 권한으로 빌드합니다. 의존성과 패키지 설치 때만 관리자 인증을 요청합니다.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="596"/>
        <source>Update GPU authentication engine</source>
        <translation>GPU 인증 엔진 업데이트</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="597"/>
        <source>Existing fingerprint references, fprintd enrolment and PAM settings will not change. Only the matcher, shader and driver bridge are replaced, followed by reference self-checks. Changed files are backed up separately. Apply the update?</source>
        <translation>기존 지문 기준·fprintd 등록·PAM 설정은 바꾸지 않습니다. 최신 매처·셰이더·드라이버 브리지만 교체한 뒤 기준 영상 자기 검사를 실행합니다. 변경 전 파일은 별도로 백업합니다. 업데이트할까요?</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="608"/>
        <source>Select your saved fingerprint session</source>
        <translation>본인의 저장 지문 세션 선택</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="612"/>
        <source>Experimental GPU authentication</source>
        <translation>실험적 GPU 인증</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="613"/>
        <source>Confirm that the selected folders contain this user&apos;s fingerprints.
%1

Existing enrolment is preserved and GPU comparison is used for sudo and KDE authentication. After at most 20 failed contacts or 60 seconds, the password path remains available. Rejection of other fingers is not fully validated. Back up the settings and apply?</source>
        <translation>선택한 폴더의 지문이 현재 사용자 본인의 것인지 확인하세요.
%1

기존 등록은 보존하고 sudo·KDE 인증에 GPU 비교를 사용합니다. 최대 실패 접촉 20회 또는 60초 후에도 비밀번호 경로는 유지됩니다. 다른 손가락 거절 성능은 완전히 검증되지 않았습니다. 설정을 백업하고 적용할까요?</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="632"/>
        <source>Restore the sudo, KDE and fprintd settings from before installation? Existing fingerprint enrolment is not deleted.</source>
        <translation>설치 전 sudo·KDE·fprintd 설정을 복구할까요? 기존 지문 등록은 삭제하지 않습니다.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="658"/>
        <source>Fingerprint match succeeded. The result was delivered while the finger remained in place. Test the sudo/KDE dialog and password fallback separately.</source>
        <translation>지문 비교 성공. 손가락을 계속 대고 있는 동안 결과가 전달됐습니다. sudo/KDE 대화창과 비밀번호 폴백은 별도로 시험하세요.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="659"/>
        <source>Sensor thermal protection stopped the test. Lift your finger, let it cool briefly, then try again.</source>
        <translation>센서 과열 보호가 시험을 중단했습니다. 잠시 손을 떼고 식힌 뒤 다시 시도하세요.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="660"/>
        <source>No finger was detected during this 30-second one-shot test. Clear the sensor, then place the enrolled finger in the centre for one or two seconds when prompted.</source>
        <translation>30초 단발 시험 동안 손가락이 감지되지 않았습니다. 센서를 비운 뒤 안내가 뜨면 등록한 손가락을 중앙에 1~2초 올려 보세요.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="661"/>
        <source>System fingerprint test incomplete: %1</source>
        <translation>시스템 지문 시험 미완료: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="671"/>
        <source>[System fingerprint test] Match confirmed while the finger remained in place.
</source>
        <translation>[시스템 지문 시험] 손가락을 유지한 상태에서 일치를 확인했습니다.
</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="673"/>
        <source>[System fingerprint test] No match. This contact was not recorded or enrolled.
</source>
        <translation>[시스템 지문 시험] 일치하지 않습니다. 이 접촉은 기록하거나 등록하지 않았습니다.
</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="675"/>
        <source>[System fingerprint test] Place the finger in the centre and hold it until a result appears. You do not need to lift it.
</source>
        <translation>[시스템 지문 시험] 손가락을 센서 중앙에 올리고 결과가 표시될 때까지 유지하세요. 떼는 동작은 필요 없습니다.
</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="690"/>
        <source>[System fingerprint test] Start with the sensor clear. When prompted, place the same enrolled finger and hold it until the result appears. You do not need to lift it.</source>
        <translation>[시스템 지문 시험] 처음에는 센서를 비워 두세요. 지문 요청이 뜨면 등록한 같은 손가락을 올리고 결과가 표시될 때까지 유지하세요. 떼는 동작은 필요 없습니다.</translation>
    </message>
</context>
</TS>
