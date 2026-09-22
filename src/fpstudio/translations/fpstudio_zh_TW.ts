<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_TW" sourcelanguage="en">
<context>
    <name>fpstudio</name>
    <message>
        <location filename="../src/engine.cpp" line="86"/>
        <source>Finger detected — hold still</source>
        <translation>偵測到手指 — 請保持不動</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="91"/>
        <source>Finger lifted</source>
        <translation>手指已抬起</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="104"/>
        <source>Only part of the sensor was touched — cover more of it, centred</source>
        <translation>只接觸到一部分 — 請置中並覆蓋更多區域</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="109"/>
        <source>The ridges are faint — press a little harder</source>
        <translation>紋路偏淡 — 請稍微用力按壓</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="114"/>
        <source>Frame received</source>
        <translation>已接收畫面</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="124"/>
        <location filename="../src/engine.cpp" line="134"/>
        <location filename="../src/headless.cpp" line="482"/>
        <source>Captured — now lift your finger</source>
        <translation>擷取完成 — 請抬起手指</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="139"/>
        <source>Matching against the enrolled print…</source>
        <translation>正在與已註冊指紋比對…</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="144"/>
        <location filename="../src/setupcheck.cpp" line="210"/>
        <source>Secure channel to the sensor established</source>
        <translation>已與感測器建立安全通道</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="149"/>
        <source>Sensor ready</source>
        <translation>感測器就緒</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="418"/>
        <location filename="../src/engine.cpp" line="499"/>
        <location filename="../src/engine.cpp" line="575"/>
        <source>Gave up after %1 seconds</source>
        <translation>%1 秒內未完成，已中止</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="461"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>註冊 %1/%2 — 抬起手指後再次按下</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <location filename="../src/headless.cpp" line="93"/>
        <location filename="../src/headless.cpp" line="442"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>請將手指放在感測器上並保持不動</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="95"/>
        <location filename="../src/headless.cpp" line="443"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>正在註冊 — 請反覆按下並抬起手指</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="97"/>
        <location filename="../src/headless.cpp" line="444"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>請將已註冊的手指放在感測器上</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="65"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>已發現 Goodix 27c6:55b4</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>A different Goodix sensor is present</source>
        <translation>存在其他 Goodix 感測器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="75"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>此組建僅支援 27c6:55b4。偵測到 27c6:%1。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="81"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>找不到 Goodix 指紋感測器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="82"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>USB 匯流排上沒有裝置回應廠商 ID 27c6。若本機確實配備讀取器，可能已在韌體設定中停用，或為其他廠牌產品。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="101"/>
        <source>Patched driver is active</source>
        <translation>已啟用修補版驅動程式</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="108"/>
        <location filename="../src/setupcheck.cpp" line="116"/>
        <source>Patched driver is not installed</source>
        <translation>未安裝修補版驅動程式</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="109"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint 無法辨識此感測器。需要建置並安裝修補版分支，但本程式副本未附帶相應原始碼。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="117"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint 無法辨識此感測器。修補版分支會加入支援。建置需要幾分鐘，並會取代系統的 libfprint，因此需要密碼。

請之後再安裝 fprintd：若先安裝，會引入官方 libfprint 並復原此變更。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="122"/>
        <source>Build and install the patched libfprint</source>
        <translation>建置並安裝修補版 libfprint</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="152"/>
        <source>Sensor is reachable without a password</source>
        <translation>無需密碼即可存取感測器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="158"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>udev 規則已安裝但尚未生效</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="159"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>每次感測器操作都會要求輸入密碼</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="161"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>規則檔已就位，但裝置節點仍屬於 root。它會在下一次 add 事件時生效，重新插拔讀取器或重新載入 udev 即可。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="164"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>沒有該規則，libusb 只能以 root 身分開啟感測器，因此每次擷取都會跳出密碼提示。

此規則授予 wheel 群組對感測器的讀寫權限。在單人筆電上，這與原本就能透過 pkexec 操作的人相同；在共用機器上則不同。略過此項是安全的——只是會多幾次密碼提示。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="174"/>
        <source>Install the udev rule and reload</source>
        <translation>安裝 udev 規則並重新載入</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="196"/>
        <source>Device communication has not been tested yet</source>
        <translation>Device communication has not been tested yet</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="197"/>
        <source>Close other authentication requests, then run the image test. Opening or refreshing the wizard never claims the sensor automatically.</source>
        <translation>Close other authentication requests, then run the image test. Opening or refreshing the wizard never claims the sensor automatically.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="198"/>
        <source>Test sensor communication and image capture</source>
        <translation>Test sensor communication and image capture</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="217"/>
        <source>The sensor answered, but the handshake did not complete</source>
        <translation>感測器已回應，但交握未完成</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="218"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>感測器持有的金鑰不是本驅動所擁有的。下一步是寫入全零金鑰，且此操作不可復原。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="224"/>
        <source>Could not talk to the sensor</source>
        <translation>無法與感測器通訊</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="225"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>交握過程未進展到足以說明原因。請先檢查上方的驅動與權限步驟。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="238"/>
        <source>The sensor already holds a usable key</source>
        <translation>感測器已持有可用金鑰</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="244"/>
        <source>Key status is unknown — automatic writing is disabled</source>
        <translation>Key status is unknown — automatic writing is disabled</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="245"/>
        <source>A USB error or a busy sensor is not treated as a key mismatch. Complete the communication test first.</source>
        <translation>A USB error or a busy sensor is not treated as a key mismatch. Complete the communication test first.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="251"/>
        <source>The sensor needs the all-zero key written</source>
        <translation>需要向感測器寫入全零金鑰</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="252"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>此操作不可復原。

感測器目前持有的金鑰無法讀回——通訊協定回傳的是由金鑰衍生出的值，而非金鑰本身——因此之後沒有備份可以還原。

本機上的 Windows 指紋登入將永久失效。若你雙系統在 Windows 上也使用它，請到此為止。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="260"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>向感測器寫入全零 PSK</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="290"/>
        <source>Image quality has not been measured yet</source>
        <translation>尚未測量影像品質</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="291"/>
        <source>Press the button below, then put your finger on the sensor and hold it there for a couple of seconds.</source>
        <translation>按下方按鈕，然後將手指放在感測器上並保持幾秒鐘。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="294"/>
        <source>Test a capture now</source>
        <translation>立即測試擷取</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="298"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>接觸面 %1%  ·  清晰度 %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="307"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>低於 24 時，即使驅動接受該畫面，比對通常也會失敗。手指乾燥或感測器持續忙碌都會導致此情況；休息幾分鐘並稍微濕潤手指通常可解決。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="311"/>
        <source>Try again</source>
        <translation>重試</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="324"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>fprintd 中已註冊手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="330"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>fprintd 中未註冊任何手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="331"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>本程式單獨保存自己的註冊資料，系統驗證看不到這些資料。透過 fprintd 重新註冊後，指紋才能用於解鎖。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="335"/>
        <source>Enrol a finger with fprintd</source>
        <translation>透過 fprintd 註冊手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="340"/>
        <source>fprintd is not answering</source>
        <translation>fprintd 無回應</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="341"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>無法連接到守護行程。它通常會依需求自動啟動，安裝它可能就是唯一缺少的步驟。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="358"/>
        <source>GPU authentication installed · %1 references</source>
        <translation>GPU authentication installed · %1 references</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="359"/>
        <source>This is experimental authentication. A valid installation does not by itself prove successful sudo/KDE authentication. Recovery location: %1</source>
        <translation>This is experimental authentication. A valid installation does not by itself prove successful sudo/KDE authentication. Recovery location: %1</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="363"/>
        <source>GPU authentication engine needs an update</source>
        <translation>GPU authentication engine needs an update</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="364"/>
        <source>Deploy the latest GPU authentication engine</source>
        <translation>Deploy the latest GPU authentication engine</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="365"/>
        <source>The existing %1 references, fprintd enrolment and PAM settings are preserved. Only the matcher, shader and driver bridge are replaced, and the update is applied only after reference self-checks pass.</source>
        <translation>The existing %1 references, fprintd enrolment and PAM settings are preserved. Only the matcher, shader and driver bridge are replaced, and the update is applied only after reference self-checks pass.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="369"/>
        <source>Connect saved fingerprints and the GPU engine to system authentication</source>
        <translation>Connect saved fingerprints and the GPU engine to system authentication</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="370"/>
        <source>Import saved fingerprints and connect sudo/KDE</source>
        <translation>Import saved fingerprints and connect sudo/KDE</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="371"/>
        <source>Select a folder containing your own saved fingerprints. Existing fprintd enrolment is preserved and the references are imported into root-only storage. sudo, administrator dialogs, TTY login and the KDE lock screen are connected with a 20-attempt/60-second limit and a password path. Rejection of other fingers remains experimental.</source>
        <translation>Select a folder containing your own saved fingerprints. Existing fprintd enrolment is preserved and the references are imported into root-only storage. sudo, administrator dialogs, TTY login and the KDE lock screen are connected with a 20-attempt/60-second limit and a password path. Rejection of other fingers remains experimental.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="382"/>
        <source>KDE fingerprint and password paths are configured in parallel</source>
        <translation>KDE fingerprint and password paths are configured in parallel</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="382"/>
        <source>Simultaneous fingerprint and password input needs configuration</source>
        <translation>Simultaneous fingerprint and password input needs configuration</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="383"/>
        <source>Fingerprint scanning runs for up to 60 seconds or 20 failed contacts and reports ready, retry and final states. KDE&apos;s separate password field remains available. Confirm actual lock-screen unlocking separately.</source>
        <translation>Fingerprint scanning runs for up to 60 seconds or 20 failed contacts and reports ready, retry and final states. KDE&apos;s separate password field remains available. Confirm actual lock-screen unlocking separately.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="384"/>
        <source>Enable simultaneous fingerprint and password input</source>
        <translation>Enable simultaneous fingerprint and password input</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="399"/>
        <source>Administrator dialogs accept fingerprint and password simultaneously</source>
        <translation>Administrator dialogs accept fingerprint and password simultaneously</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="406"/>
        <source>Administrator dialogs are not connected to simultaneous authentication</source>
        <translation>Administrator dialogs are not connected to simultaneous authentication</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="407"/>
        <source>The guarded PAM module scans a fingerprint while keeping the normal password path available. It backs up every changed file, validates the KDE password service first, and fails closed on module errors. The sensor is limited to 60 seconds or 20 failed contacts.</source>
        <translation>The guarded PAM module scans a fingerprint while keeping the normal password path available. It backs up every changed file, validates the KDE password service first, and fails closed on module errors. The sensor is limited to 60 seconds or 20 failed contacts.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="409"/>
        <source>Connect administrator dialogs to fingerprint and password</source>
        <translation>Connect administrator dialogs to fingerprint and password</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="425"/>
        <source>sudo accepts fingerprint and password simultaneously</source>
        <translation>sudo accepts fingerprint and password simultaneously</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="432"/>
        <source>Terminal sudo is not connected to simultaneous authentication</source>
        <translation>Terminal sudo is not connected to simultaneous authentication</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="433"/>
        <source>The terminal keeps input hidden and accepts either a completed fingerprint match or an explicitly submitted password. Typing selects the password path; otherwise a fingerprint match completes immediately. The original PAM files are backed up before the guarded module is installed.</source>
        <translation>The terminal keeps input hidden and accepts either a completed fingerprint match or an explicitly submitted password. Typing selects the password path; otherwise a fingerprint match completes immediately. The original PAM files are backed up before the guarded module is installed.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="435"/>
        <source>Connect sudo to fingerprint and password</source>
        <translation>Connect sudo to fingerprint and password</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="495"/>
        <source>Terminal sudo</source>
        <translation>Terminal sudo</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="496"/>
        <source>Saved fingerprints · GPU authentication</source>
        <translation>Saved fingerprints · GPU authentication</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="497"/>
        <source>KDE lock screen</source>
        <translation>KDE lock screen</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="487"/>
        <source>Sensor</source>
        <translation>感測器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="488"/>
        <source>Driver</source>
        <translation>驅動程式</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="489"/>
        <source>Permissions</source>
        <translation>權限</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="490"/>
        <source>Secure channel</source>
        <translation>安全通道</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="491"/>
        <source>Sensor key</source>
        <translation>感測器金鑰</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="492"/>
        <source>Image quality</source>
        <translation>影像品質</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="493"/>
        <source>Enrolment</source>
        <translation>註冊</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="494"/>
        <source>Unlocking</source>
        <translation>解鎖</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="516"/>
        <source>Not checked - no sensor</source>
        <translation>未檢查 — 無感測器</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="102"/>
        <source>Unexpected image size</source>
        <translation>Unexpected image size</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="134"/>
        <source>Waiting for a finger</source>
        <translation>Waiting for a finger</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="135"/>
        <source>Insufficient contact area</source>
        <translation>Insufficient contact area</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="136"/>
        <source>Insufficient ridge contrast</source>
        <translation>Insufficient ridge contrast</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="137"/>
        <source>Insufficient consistent ridge pattern</source>
        <translation>Insufficient consistent ridge pattern</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="138"/>
        <source>Waiting for motion to settle</source>
        <translation>Waiting for motion to settle</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="139"/>
        <source>Research candidate</source>
        <translation>Research candidate</translation>
    </message>
</context>
<context>
    <name>fpstudio::ImageView</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="58"/>
        <source>No frame yet

Use Capture to pull an image from the sensor</source>
        <translation>尚無影像

使用「擷取」從感測器取得影像</translation>
    </message>
</context>
<context>
    <name>fpstudio::LiveWindow</name>
    <message>
        <location filename="../src/livewindow.cpp" line="39"/>
        <source>a different finger from the last one</source>
        <translation>與剛才不同的手指</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="40"/>
        <source>the same finger used so far</source>
        <translation>目前一直使用的同一根手指</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="41"/>
        <source>fpstudio · live fingerprint stream</source>
        <translation>fpstudio · 指紋即時影像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="51"/>
        <source>⚠ Different-finger trial — lift your finger off the sensor. Calibration starts automatically.</source>
        <translation>⚠ 不同手指測試 — 請將手指從感測器上移開。稍後會自動校正。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="52"/>
        <source>Lift your finger off the sensor. Calibration starts automatically.</source>
        <translation>請將手指從感測器上移開。稍後會自動校正。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="73"/>
        <source>After calibration, rest the same finger lightly on the sensor and follow the prompts. Candidates are compared automatically for each contact.
Live sensor image 108 × 88 · background subtracted, contrast normalised · not an authentication decision · up to 3 minutes</source>
        <translation>校正後請將同一根手指輕輕放在感測器上，並依提示操作。每次接觸都會自動比對候選影像。
即時感測器影像 108 × 88 · 已扣除背景、對比度正規化 · 不是認證判定 · 最長 3 分鐘</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="77"/>
        <source>This is a different-finger comparison trial. Do not use the finger from the previous run this time.
After calibration, tap and lift repeatedly with one other finger. Even a high score does not mean authentication was granted.</source>
        <translation>這是與其他手指的比對測試。這次請勿使用上一輪用過的手指。
校正後請用另一根手指反覆輕觸並抬起。分數高也不代表認證已通過。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="79"/>
        <source>Waiting for calibration — no live image yet</source>
        <translation>等待校正 — 尚無即時影像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="96"/>
        <source>0 FPS · waiting for a new frame</source>
        <translation>0 FPS · 等待新影格</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="99"/>
        <source>GPU comparison: no reference image set · collecting live only</source>
        <translation>GPU 比對：未指定參考影像 · 僅進行即時擷取</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="106"/>
        <location filename="../src/livewindow.cpp" line="1052"/>
        <source>Stop</source>
        <translation>停止</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="111"/>
        <source>Install this fingerprint as the enrolment</source>
        <translation>將此指紋安裝為登錄資料</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="114"/>
        <source>Coverage map — drawn here as contacts accumulate
The dark red areas have not been touched yet
Move your finger toward the empty side
This is enrolment guidance, not an authentication decision</source>
        <translation>覆蓋圖 — 隨著接觸累積在此繪製
深紅色區域是尚未觸及的位置
請將手指移向空白的一側
這是登錄引導，不是認證判定</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="132"/>
        <source>The enrolment is installed. sudo and the lock screen now use this fingerprint.</source>
        <translation>登錄資料已安裝。sudo 與鎖定畫面現在會使用此指紋。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="133"/>
        <source>Installing the enrolment failed. The existing enrolment is unchanged.</source>
        <translation>登錄資料安裝失敗。原有登錄資料維持不變。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="136"/>
        <source>Installation failed</source>
        <translation>安裝失敗</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="137"/>
        <source>The installer exited with code %1.</source>
        <translation>安裝工具以代碼 %1 結束。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="144"/>
        <source>Could not start. Check for another sensor session, or the permissions on the save folder.</source>
        <translation>無法啟動。請檢查是否有其他感測器工作階段，或儲存資料夾的權限。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="157"/>
        <source>Could not run the capture program: </source>
        <translation>無法執行擷取程式：</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="158"/>
        <location filename="../src/livewindow.cpp" line="167"/>
        <location filename="../src/livewindow.cpp" line="1054"/>
        <source>Close</source>
        <translation>關閉</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="164"/>
        <source>Stopped. You can lift your finger off the sensor.</source>
        <translation>已停止。可以將手指從感測器上移開。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="165"/>
        <source>The live stream has ended. Lift your finger off the sensor.</source>
        <translation>即時影像已結束。請將手指從感測器上移開。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="166"/>
        <source>0 FPS · ended · the last image is a still · %1 frames in total</source>
        <translation>0 FPS · 已結束 · 最後一張是靜止畫面 · 共 %1 個影格</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="189"/>
        <source>Vulkan GPU is initializing — it calculates separately from the live stream</source>
        <translation>正在初始化 Vulkan GPU — 它會與即時影像分開計算</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="198"/>
        <source>%1 · %2 references ready · waiting for a stable candidate</source>
        <translation>%1 · 已備妥 %2 張參考影像 · 正在等候穩定候選影像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="225"/>
        <source>GPU comparison error: </source>
        <translation>GPU 比對錯誤：</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="230"/>
        <source>GPU %1 hypotheses · %2 ms · image correlation %3 · ridge-direction agreement %4
Best: %5 / %6 — research comparison only; not an authentication decision</source>
        <translation>GPU %1 個假設 · %2 毫秒 · 影像相關性 %3 · 紋線方向一致性 %4
最佳：%5 / %6 — 僅供研究比對，並非身分驗證決定</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="234"/>
        <source>
Alignment is ambiguous: a different position has a similar score</source>
        <translation>
對齊存在歧義：其他位置也有相近分數</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="235"/>
        <source>
At the search boundary: move the finger slightly toward the centre</source>
        <translation>
已到搜尋範圍邊界：請將手指稍微移向中央</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="238"/>
        <source>
Interior ridge support %1/%2 regions · %3</source>
        <translation>
內部紋線支援 %1/%2 個區域 · %3</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="240"/>
        <source>ridges are consistent (identity unverified)</source>
        <translation>紋線一致（身分尚未驗證）</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="240"/>
        <source>insufficient match evidence</source>
        <translation>匹配證據不足</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="243"/>
        <source> · 0/1 contact-region overlap %1%</source>
        <translation> · 0/1 接觸區域重疊 %1%</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="245"/>
        <source> · 360° additional search applied</source>
        <translation> · 已套用 360° 額外搜尋</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="247"/>
        <source> · full reference-bank search</source>
        <translation> · 全部參考庫搜尋</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="247"/>
        <source> · representative-reference fast comparison</source>
        <translation> · 代表參考快速比對</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="263"/>
        <source>GPU comparison cannot run: </source>
        <translation>無法執行 GPU 比對：</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="269"/>
        <source>The GPU comparison worker exited unexpectedly (code %1)</source>
        <translation>GPU 比對處理程序意外結束（代碼 %1）</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="324"/>
        <source>Vulkan GPU is comparing reference images, angles, and positions in parallel…</source>
        <translation>Vulkan GPU 正在並行比對參考影像、角度和位置…</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="329"/>
        <source>GPU comparison timed out — live collection continues</source>
        <translation>GPU 比對逾時 — 即時收集繼續進行</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="783"/>
        <source>added</source>
        <translation>已採納</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="784"/>
        <source>smeared · not added</source>
        <translation>模糊 · 未採納</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="785"/>
        <source>registration failed · not added</source>
        <translation>對位失敗 · 未採納</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="786"/>
        <source>too little overlap · not added</source>
        <translation>重疊不足 · 未採納</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="794"/>
        <source>last contact · %1</source>
        <translation>最後一次接觸 · %1</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="798"/>
        <source>Coverage %1× · %2 placed · %3 not added
Dark red is what has not been touched yet · the green outline is what was just added</source>
        <translation>覆蓋範圍 %1 倍 · 已放置 %2 個 · 未採納 %3 個
深紅色是尚未觸及的位置 · 綠色外框是剛剛採納的位置</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="852"/>
        <location filename="../src/livewindow.cpp" line="885"/>
        <source>Cannot install</source>
        <translation>無法安裝</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="853"/>
        <source>No capture has been placed on the map. Rest your finger on the sensor, move it slowly, and collect again.</source>
        <translation>沒有任何擷取影像被放入覆蓋圖。請將手指放在感測器上緩慢移動後重新擷取。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="858"/>
        <source>Install fingerprint enrolment</source>
        <translation>安裝指紋登錄資料</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="859"/>
        <source>Install %1 captures from this session as the authentication enrolment.

The existing enrolment is backed up and replaced; fprintd enrolment and PAM settings are left alone.
This matcher has not been validated for rejecting other people. Continue?</source>
        <translation>將本次工作階段的 %1 張擷取影像安裝為認證登錄資料。

原有登錄資料會先備份再取代；fprintd 登錄與 PAM 設定維持不變。
此比對器尚未驗證能否拒絕他人。是否繼續？</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="886"/>
        <source>Could not find the installer (tools/auth_install.py).</source>
        <translation>找不到安裝工具（tools/auth_install.py）。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="895"/>
        <source>Installing the enrolment…</source>
        <translation>正在安裝登錄資料…</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1038"/>
        <source>Contact %1 · image from comparison %2 (not live)
Gray: background (0) · yellow: contact (1), but weak features · green/red: comparison evidence
Cyan +: transformed reference contact centre · orange +: current probe contact centre · blue: comparison range</source>
        <translation>接觸 %1 · 比對 %2 時的影像（非即時）
灰色：背景 (0) · 黃色：接觸 (1)，但特徵較弱 · 綠色/紅色：比對證據
青色 +：轉換後的參考接觸中心 · 橘色 +：目前待測接觸中心 · 藍色：比對範圍</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1051"/>
        <source>Capture finished — lift your finger. %1 remaining contacts are still being compared</source>
        <translation>擷取結束 — 請抬起手指。剩餘 %1 次接觸仍在比對中</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1059"/>
        <source>The target of %1 contacts has been compared — no need to repeat. You can lift your finger.</source>
        <translation>已完成目標的 %1 次接觸比對 — 不需重複。可以抬起手指。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1059"/>
        <source>Stopped. You can lift your finger.</source>
        <translation>已停止。可以抬起手指。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1060"/>
        <source>Capture and candidate comparison have ended. See the per-contact totals below.</source>
        <translation>擷取與候選比對已結束。請查看下方依接觸統計的結果。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1101"/>
        <source>Capture finished · %1 frames · %2 contacts detected
%3 comparisons answered · %4 ridge-consistent · %5 not compared (%6 with no candidate) · %7 queued · %8 errors
Research results on selected data, not an authentication success rate</source>
        <translation>擷取結束 · %1 個影格 · 偵測到接觸 %2 次
比對回應 %3 次 · 紋線一致 %4 次 · 未比對 %5 次（其中無候選 %6 次）· 等待 %7 次 · 錯誤 %8 個
這是對篩選資料的研究結果，不是認證成功率</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1109"/>
        <source>Keep your finger off — connecting to the sensor and calibrating the background</source>
        <translation>請繼續抬著手指 — 正在連線至感測器並校正背景</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1158"/>
        <source>Lift your finger — background calibration in %1 s</source>
        <translation>請抬起手指 — %1 秒後校正背景</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1167"/>
        <source>Keep your finger off — collecting more background from the live stream</source>
        <translation>請繼續抬著手指 — 正在從即時影像中補充擷取背景</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1170"/>
        <source>0 FPS · waiting for the first image · %1 s</source>
        <translation>0 FPS · 正在等待第一張影像 · %1 秒</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1354"/>
        <source>Keep your finger off and hold — collecting the background over several frames (do not touch the sensor yet)</source>
        <translation>請抬著手指等待 — 正在以多個影格擷取背景（此時請勿觸碰感測器）</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1355"/>
        <source>No finger detected — now rest %1 lightly in the centre of the sensor</source>
        <translation>未偵測到手指 — 現在請將%1輕輕放在感測器中央</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1358"/>
        <source>Warning: the other finger also produced ridge-match evidence. A suspected false accept has been recorded — lift and touch again</source>
        <translation>注意：其他手指也出現了紋線吻合的跡象。已記錄為疑似誤判 — 請抬起後重新放置</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1359"/>
        <source>This other finger produced no match evidence. Recorded — lift completely and touch again</source>
        <translation>這根其他手指沒有出現吻合跡象。已記錄 — 請完全抬起後重新放置</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1361"/>
        <source>This contact&apos;s ridges were confirmed — lift completely and rest %1 on the sensor again (not an authentication decision)</source>
        <translation>已確認本次接觸的紋線 — 請完全抬起，然後將%1重新放在感測器上（不是認證判定）</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1363"/>
        <source>A sharp image was captured — comparing automatically. You can lift your finger</source>
        <translation>已取得清晰影像 — 正在自動比對。可以抬起手指</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1366"/>
        <source>Three comparisons of this contact found too little evidence — lift completely, then place %1 in the centre again</source>
        <translation>本次接觸比對 3 次仍證據不足 — 請完全抬起，然後將%1重新放到中央</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1367"/>
        <source>Too little match evidence — move %1 slightly toward the centre and press more lightly. It will be compared again automatically</source>
        <translation>吻合證據不足 — 請將%1稍微移向中央並減輕力道。系統會自動重新比對</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1368"/>
        <source>The contact area is small — lay the pad of %1 flat across the centre of the sensor</source>
        <translation>接觸面積偏小 — 請將%1的指腹平放在感測器中央</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1369"/>
        <source>You are pressing hard — ease off and just rest the finger. Quality falls below 0.79 in this range</source>
        <translation>按壓力道偏大 — 請放鬆力道，像輕放一樣。在此區間品質會降到 0.79 以下</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1370"/>
        <source>Hold your finger still for a moment — a stable image will be selected automatically</source>
        <translation>請讓手指稍微靜止 — 系統會自動選取穩定的影像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1371"/>
        <source>The ridges are unclear — ease off a little and shift %1 slightly</source>
        <translation>紋線不清晰 — 請稍微放鬆力道，並將%1的位置略作調整</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1372"/>
        <source>Hold it there for a moment — selecting a sharp image automatically</source>
        <translation>請保持一下 — 正在自動選取清晰影像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1373"/>
        <source>⚠ Use a different finger · </source>
        <translation>⚠ 請換一根手指 · </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1376"/>
        <source>The research storage and comparison limit has been reached — this contact will not be compared further. Please stop</source>
        <translation>已達到研究用儲存與比對上限 — 本次接觸不再繼續比對。請停止</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1377"/>
        <source>GPU comparison is unavailable — showing the image only. Please stop</source>
        <translation>無法使用 GPU 比對 — 僅顯示影像。請停止</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1381"/>
        <source>⚠ The ridges were smeared, so this was not added to the map — the red outline on the map is where it would have gone. Ease off and press the same spot again</source>
        <translation>⚠ 紋線模糊，因此未加入覆蓋圖 — 圖上的紅色外框就是該位置。請放鬆力道並在同一位置重新按壓</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1384"/>
        <source>⚠ This could not be fitted to the map — press so that it overlaps an already filled area by about half</source>
        <translation>⚠ 無法與覆蓋圖對齊 — 請按壓時與已填滿的區域重疊約一半</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1387"/>
        <source>⚠ This does not overlap anything already on the map — continue from the edge of a filled area</source>
        <translation>⚠ 與覆蓋圖上既有區域沒有任何重疊 — 請從已填滿區域的邊緣繼續</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1391"/>
        <source>[%1/%2 contacts compared · stops automatically at the target] </source>
        <translation>[%1/%2 次接觸已比對 · 達到目標時自動停止] </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1395"/>
        <source>%1 FPS · %2 frames · ridge signal %3 · contrast area %4% · sharpness %5
</source>
        <translation>%1 FPS · %2 個影格 · 紋線訊號 %3 · 對比面積 %4% · 清晰度 %5
</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1398"/>
        <source>%1 · %2 contacts · %3 candidates · %4 GPU comparisons done / %5 contacts queued (not validated for authentication)</source>
        <translation>%1 · 接觸 %2 次 · 候選 %3 張 · GPU 完成 %4 件 / 等待 %5 次接觸（未針對認證驗證）</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1400"/>
        <source>
Raw difference %1 · background noise %2 · %3</source>
        <translation>
原始差值 %1 · 背景雜訊 %2 · %3</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1401"/>
        <source>collecting background</source>
        <translation>正在擷取背景</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1401"/>
        <source>finger in contact</source>
        <translation>手指接觸中</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1401"/>
        <source>no finger</source>
        <translation>無手指</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1406"/>
        <source>The image stream has stopped — the screen shows the last frame</source>
        <translation>影像接收已停止 — 螢幕顯示的是最後一個影格</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1407"/>
        <source>0 FPS · no new image for %1 s</source>
        <translation>0 FPS · 已有 %1 秒沒有新影像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1444"/>
        <source>GPU comparison finished · %1 processed · stopped normally on reaching the target of %2 contacts
The results and colour-marked images have been saved. This did not decide whether authentication is granted.</source>
        <translation>GPU 比對完成 · 已處理 %1 件 · 達到 %2 次接觸的目標後正常結束
結果與彩色標註影像均已儲存。這並未判定認證是否通過。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1445"/>
        <source>GPU comparison stopped · %1 finished · %2 cancelled in progress
Stopped at your request. Saved results are kept.</source>
        <translation>GPU 比對已停止 · 完成 %1 件 · 進行中取消 %2 件
已依您的要求停止。已儲存的結果予以保留。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1446"/>
        <source>Stopping — lift your finger</source>
        <translation>正在停止 — 請抬起手指</translation>
    </message>
</context>
<context>
    <name>fpstudio::MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="109"/>
        <location filename="../src/mainwindow.cpp" line="363"/>
        <source>Device scan</source>
        <translation>裝置掃描</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="154"/>
        <source>Setup</source>
        <translation>設定</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="155"/>
        <source>Set up fingerprint unlock…</source>
        <translation>設定指紋解鎖…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="158"/>
        <source>Language</source>
        <translation>語言</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="195"/>
        <source>fpstudio — fingerprint sensor diagnostics</source>
        <translation>fpstudio — 指紋感測器診斷</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="200"/>
        <source>Looking for a device…</source>
        <translation>正在尋找裝置…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="204"/>
        <source>Device</source>
        <translation>裝置</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="206"/>
        <source>Rescan</source>
        <translation>重新掃描</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="211"/>
        <source>Capture — show image</source>
        <translation>擷取 — 顯示影像</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="212"/>
        <location filename="../src/mainwindow.cpp" line="373"/>
        <source>Enrol</source>
        <translation>註冊</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="213"/>
        <location filename="../src/mainwindow.cpp" line="379"/>
        <source>Verify</source>
        <translation>驗證</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="214"/>
        <location filename="../src/mainwindow.cpp" line="383"/>
        <source>Delete enrolment</source>
        <translation>刪除註冊</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="219"/>
        <source>Actions</source>
        <translation>操作</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="226"/>
        <source>Progress</source>
        <translation>進度</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="258"/>
        <source>—</source>
        <translation>—</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="295"/>
        <source>Coverage</source>
        <translation>接觸面</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="297"/>
        <source>Sharpness</source>
        <translation>清晰度</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Sample</source>
        <translation>樣本</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Score</source>
        <translation>分數</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Keypoints</source>
        <translation>特徵點</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="314"/>
        <source>Match scores</source>
        <translation>比對分數</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="324"/>
        <source>Driver log</source>
        <translation>驅動程式記錄</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="366"/>
        <source>→ Put your finger on the sensor</source>
        <translation>→ 請將手指放在感測器上</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="367"/>
        <source>Image capture</source>
        <translation>影像擷取</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="370"/>
        <source>→ Enrol: press and lift your finger repeatedly</source>
        <translation>→ 註冊：反覆按下並抬起手指</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="377"/>
        <source>→ Verify: present the enrolled finger</source>
        <translation>→ 驗證：放上已註冊的手指</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="394"/>
        <source>in progress…</source>
        <translation>進行中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="394"/>
        <source> in progress…</source>
        <translation> 進行中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="422"/>
        <source>No fingerprint device was found.</source>
        <translation>找不到指紋裝置。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="431"/>
        <source>Driver: %1
ID: %2
Scan: %3 · %4 enrol stages
Raw capture: %5</source>
        <translation>驅動程式: %1
ID: %2
掃描: %3 · %4 個註冊階段
原始擷取: %5</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="436"/>
        <source>yes</source>
        <translation>支援</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="436"/>
        <source>no</source>
        <translation>不支援</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="448"/>
        <source>No image was returned</source>
        <translation>未回傳影像</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="452"/>
        <source>Captured %1 × %2</source>
        <translation>已擷取 %1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="471"/>
        <source>Match   %1 / %2</source>
        <translation>符合   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="472"/>
        <source>No match   %1 / %2</source>
        <translation>不符合   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="483"/>
        <source>Failed — %1</source>
        <translation>失敗 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484"/>
        <location filename="../src/mainwindow.cpp" line="642"/>
        <location filename="../src/mainwindow.cpp" line="643"/>
        <source>Failed</source>
        <translation>失敗</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="485"/>
        <location filename="../src/mainwindow.cpp" line="644"/>
        <source>Error: %1</source>
        <translation>錯誤: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="489"/>
        <source>Done — %1</source>
        <translation>完成 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="494"/>
        <source>Enrolment complete</source>
        <translation>註冊完成</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="496"/>
        <source>Enrolment deleted</source>
        <translation>註冊已刪除</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="523"/>
        <source>This scan fell below the threshold and was asked for again</source>
        <translation>本次掃描低於門檻，已重新要求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="538"/>
        <source>Coverage %1%  ·  sharpness %2  —  faint. Press a little harder</source>
        <translation>接觸面 %1%  ·  清晰度 %2  —  偏淡，請稍微用力</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="540"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>接觸面 %1%  ·  清晰度 %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="561"/>
        <source>%1 × %2</source>
        <translation>%1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="617"/>
        <source>Agent request</source>
        <translation>代理請求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="618"/>
        <source>CLI request</source>
        <translation>CLI 請求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="618"/>
        <source>request</source>
        <translation>請求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="634"/>
        <source>Idle</source>
        <translation>閒置</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="635"/>
        <source>▸ %1   ·   %2s elapsed</source>
        <translation>▸ %1   ·   已過 %2 秒</translation>
    </message>
</context>
<context>
    <name>fpstudio::PrivRunner</name>
    <message>
        <location filename="../src/privrunner.cpp" line="32"/>
        <source>Could not run pkexec</source>
        <translation>無法執行 pkexec</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="47"/>
        <source>The operation did not finish in time and was stopped</source>
        <translation>操作未在限定時間內完成，已停止</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="52"/>
        <source>The operation terminated abnormally</source>
        <translation>作業異常終止</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="59"/>
        <source>Authentication was cancelled or refused</source>
        <translation>驗證被取消或拒絕</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="84"/>
        <source>Could not parse the result (exit code %1)</source>
        <translation>無法解析結果（結束碼 %1）</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="147"/>
        <source>An operation is already running</source>
        <translation>已有作業在執行</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="151"/>
        <source>Could not resolve this program&apos;s own path</source>
        <translation>無法確定本程式自身的路徑</translation>
    </message>
</context>
<context>
    <name>fpstudio::ResearchWindow</name>
    <message>
        <location filename="../src/researchwindow.cpp" line="43"/>
        <source>fpstudio · fingerprint research session</source>
        <translation>fpstudio · 指紋研究工作階段</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="55"/>
        <source>The captured fingerprint image will appear here</source>
        <translation>擷取到的指紋影像將顯示在這裡</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="58"/>
        <source>No image · nothing captured yet</source>
        <translation>無影像 · 尚未擷取</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="60"/>
        <source>Stop</source>
        <translation>停止</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="68"/>
        <source>Another research session is already running</source>
        <translation>已有另一個研究工作階段在執行</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="69"/>
        <location filename="../src/researchwindow.cpp" line="230"/>
        <location filename="../src/researchwindow.cpp" line="248"/>
        <source>Close</source>
        <translation>關閉</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="74"/>
        <source>Could not create the save folder</source>
        <translation>無法建立儲存資料夾</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="88"/>
        <source>Could not start the capture program</source>
        <translation>無法啟動擷取程式</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="94"/>
        <source>Get your right index finger ready. Do not touch the sensor yet.</source>
        <translation>請準備好右手食指。暫時不要觸碰感測器。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="95"/>
        <source>Plan: 6 reference images → 4 verification images of the same finger → 4 comparison images of the right middle finger.
No buttons to press. Follow the finger and position prompts on screen. You can stop at any time.</source>
        <translation>計畫：基準影像 6 張 → 同一手指的驗證影像 4 張 → 右手中指的比對影像 4 張。
不需按任何按鈕。請依畫面上關於手指與位置的提示操作。隨時可以停止。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="107"/>
        <source>Reference images</source>
        <translation>基準影像</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="107"/>
        <source>Separate verification images</source>
        <translation>單獨驗證影像</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="107"/>
        <source>Different-finger comparison</source>
        <translation>與其他手指的比對</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="111"/>
        <source>the broad pad of your right middle finger, centred</source>
        <translation>右手中指的寬指腹，置中</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="112"/>
        <source>your right index finger, a touch left of centre</source>
        <translation>右手食指，略偏左於中央</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="113"/>
        <source>your right index finger, a touch right of centre</source>
        <translation>右手食指，略偏右於中央</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="114"/>
        <source>the broad pad of your right index finger, centred</source>
        <translation>右手食指的寬指腹，置中</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="125"/>
        <source>%1 · saved %2 / %3</source>
        <translation>%1 · 已儲存 %2 / %3</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="143"/>
        <source>① Lift your finger clear of the sensor · 5 s</source>
        <translation>① 請將手指完全移離感測器 · 5 秒</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="144"/>
        <source>The empty sensor is measured first. Wait until this changes to &apos;Touch now&apos;.</source>
        <translation>先測量空的感測器。請等到此處變為「現在放置」。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="149"/>
        <source>① Keep it off and wait · preparing the sensor</source>
        <translation>① 請繼續移開並等待 · 正在準備感測器</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="150"/>
        <source>This changes to &apos;Touch now&apos; automatically once the background measurement is done.</source>
        <translation>背景測量完成後，此處會自動變為「現在放置」。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="164"/>
        <source>① Lift your finger clear · %1 s</source>
        <translation>① 請完全抬起手指 · %1 秒</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="168"/>
        <source>%1 · sensor ready in %2 s</source>
        <translation>%1 · %2 秒後感測器就緒</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="173"/>
        <source>6 reference images done. Starting the separate verification.</source>
        <translation>6 張基準影像已完成。開始單獨驗證。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="174"/>
        <source>Now switch to your right middle finger.</source>
        <translation>現在請換成右手中指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="175"/>
        <source>Four more images with the same right index finger, placed afresh. These are evaluated separately from the reference images.</source>
        <translation>用同一根右手食指重新放置再擷取 4 張。這些影像與基準影像分開評估。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="176"/>
        <source>Four images to check whether a different finger is wrongly accepted. Do not touch the sensor yet.</source>
        <translation>這 4 張用於確認是否會誤接受其他手指。暫時不要觸碰感測器。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="188"/>
        <source>② Touch now — %1</source>
        <translation>② 現在放置 — %1</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="189"/>
        <source>Cover the whole sensor, press lightly and hold. Lift when the prompt says so, after the image appears.</source>
        <translation>請覆蓋整個感測器，輕輕按壓並保持。影像出現後，依提示抬起即可。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="191"/>
        <source>③ Hold your finger still · capturing</source>
        <translation>③ 請保持手指不動 · 正在擷取</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="192"/>
        <source>Contact confirmed. Do not move your finger.</source>
        <translation>已確認接觸。請不要移動手指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="194"/>
        <source>④ Captured. Lift your finger clear.</source>
        <translation>④ 已擷取。請完全抬起手指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="195"/>
        <source>The image and its quality will be shown in a moment.</source>
        <translation>稍後會顯示擷取影像及其品質。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="221"/>
        <source>Image just captured · %1×%2 · contact %3% · sharpness %4 · background difference %5</source>
        <translation>剛剛擷取的影像 · %1×%2 · 接觸率 %3% · 清晰度 %4 · 背景差值 %5</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="228"/>
        <source>All 14 images collected — you can rest your hand.</source>
        <translation>14 張已全部擷取完成 — 可以讓手休息了。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="229"/>
        <source>The reference, verification and different-finger images were saved separately. The matching results are analysed next. Finishing collection is not a decision that authentication succeeded.</source>
        <translation>基準、驗證與其他手指的影像已分開儲存。接下來將分析比對結果。擷取完成並不代表認證成功。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="233"/>
        <source>④ Saved · %1/14 — lift your finger.</source>
        <translation>④ 已儲存 · %1/14 — 請抬起手指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="234"/>
        <source>Below is the image just captured. The next capture is prepared automatically in 4 seconds.</source>
        <translation>下方是剛剛擷取的實際影像。4 秒後會自動進入下一次擷取準備。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="239"/>
        <source>The capture failed — lift your finger.</source>
        <translation>擷取失敗 — 請抬起手指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="240"/>
        <source>The same sample will be retried automatically. Do not touch the sensor yet.</source>
        <translation>將自動重試同一樣本。暫時不要放上手指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="241"/>
        <source>Three failures in a row, so it has paused for diagnosis. Rest your hand. Collection resumes automatically once the cause is identified.</source>
        <translation>連續 3 次失敗，已暫停以便診斷。請讓手休息。確認原因後會自動恢復擷取。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="249"/>
        <source>Stopped. Lift your finger.</source>
        <translation>已停止。請抬起手指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="249"/>
        <source>Images already saved have been kept.</source>
        <translation>已儲存的影像已予保留。</translation>
    </message>
</context>
<context>
    <name>fpstudio::SetupWizard</name>
    <message>
        <location filename="../src/setupwizard.cpp" line="405"/>
        <source>Failed — %1</source>
        <translation>失敗 — %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="150"/>
        <source>Skip</source>
        <translation>略過</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="92"/>
        <source>FPStudio · Integrated fingerprint authentication setup</source>
        <translation>FPStudio · Integrated fingerprint authentication setup</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="151"/>
        <source>Check status again</source>
        <translation>Check status again</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="183"/>
        <source>Setup steps complete · Verify real sudo/KDE authentication and password recovery separately.</source>
        <translation>Setup steps complete · Verify real sudo/KDE authentication and password recovery separately.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="185"/>
        <source>Finish</source>
        <translation>完成</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="216"/>
        <source>Stop</source>
        <translation>Stop</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="219"/>
        <source>Stopping the fingerprint test. You may lift your finger.</source>
        <translation>Stopping the fingerprint test. You may lift your finger.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="222"/>
        <location filename="../src/setupwizard.cpp" line="632"/>
        <source>Restore authentication settings</source>
        <translation>Restore authentication settings</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="223"/>
        <source>Test system fingerprint</source>
        <translation>Test system fingerprint</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="237"/>
        <source>Open diagnostics tool</source>
        <translation>開啟診斷工具</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="240"/>
        <source>Live capture preview, the driver log, and MCP status - for a closer look once the checklist here is done.</source>
        <translation>提供即時擷取預覽、驅動記錄與 MCP 狀態——用於完成此清單後進一步深入查看。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="274"/>
        <source>Checking the device and installation. Keep your finger off the sensor for now.</source>
        <translation>Checking the device and installation. Keep your finger off the sensor for now.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="350"/>
        <source>This step cannot be undone.</source>
        <translation>此步驟無法復原。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="366"/>
        <source>This will ask for your password.</source>
        <translation>這將要求輸入你的密碼。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="368"/>
        <source>This step is confirmed. Continue to the next required step.</source>
        <translation>This step is confirmed. Continue to the next required step.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="369"/>
        <source>Next action: %1</source>
        <translation>Next action: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="369"/>
        <source>Review the guidance. This step is not confirmed yet.</source>
        <translation>Review the guidance. This step is not confirmed yet.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="379"/>
        <source>WRITE</source>
        <translation>WRITE</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="381"/>
        <source>Sensor key</source>
        <translation>感測器金鑰</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="382"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

輸入 %2 以確認。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="406"/>
        <source>The enrolment helper was not found</source>
        <translation>找不到註冊輔助程式</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="412"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>正在註冊 — 請反覆按下並抬起手指</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="420"/>
        <source>Enrolment in progress · Present and lift the same finger as instructed
</source>
        <translation>Enrolment in progress · Present and lift the same finger as instructed
</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="425"/>
        <source>Could not start the enrolment tool: %1</source>
        <translation>Could not start the enrolment tool: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="456"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>請將手指放在感測器上並保持不動</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="465"/>
        <source>Could not start the image tool: %1</source>
        <translation>Could not start the image tool: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="529"/>
        <source>in progress…</source>
        <translation>進行中…</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="536"/>
        <source>Wait for the operation to finish or cancel its authentication request. Setup records and recovery information are being protected.</source>
        <translation>Wait for the operation to finish or cancel its authentication request. Setup records and recovery information are being protected.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="540"/>
        <source>Finish the running setup operation or cancel its authentication request before closing.</source>
        <translation>Finish the running setup operation or cancel its authentication request before closing.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="554"/>
        <source>This step did not finish in time and was stopped. The sensor and your enrolment are unchanged - try it again.</source>
        <translation>此步驟未在限定時間內完成，已停止。感測器與您已註冊的指紋均未變更，請再試一次。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="575"/>
        <source>Setup operation complete. Use the system fingerprint test to verify the real path.</source>
        <translation>Setup operation complete. Use the system fingerprint test to verify the real path.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="575"/>
        <source>Setup incomplete or cancelled: %1</source>
        <translation>Setup incomplete or cancelled: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="581"/>
        <source>After administrator authorization, installation and checks run automatically. Authentication settings are restored if installation fails.</source>
        <translation>After administrator authorization, installation and checks run automatically. Authentication settings are restored if installation fails.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="582"/>
        <source>The driver is built as the current user. Administrator authorization is requested only for dependencies and package installation.</source>
        <translation>The driver is built as the current user. Administrator authorization is requested only for dependencies and package installation.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="596"/>
        <source>Update GPU authentication engine</source>
        <translation>Update GPU authentication engine</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="597"/>
        <source>Existing fingerprint references, fprintd enrolment and PAM settings will not change. Only the matcher, shader and driver bridge are replaced, followed by reference self-checks. Changed files are backed up separately. Apply the update?</source>
        <translation>Existing fingerprint references, fprintd enrolment and PAM settings will not change. Only the matcher, shader and driver bridge are replaced, followed by reference self-checks. Changed files are backed up separately. Apply the update?</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="608"/>
        <source>Select your saved fingerprint session</source>
        <translation>Select your saved fingerprint session</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="612"/>
        <source>Experimental GPU authentication</source>
        <translation>Experimental GPU authentication</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="613"/>
        <source>Confirm that the selected folders contain this user&apos;s fingerprints.
%1

Existing enrolment is preserved and GPU comparison is used for sudo and KDE authentication. After at most 20 failed contacts or 60 seconds, the password path remains available. Rejection of other fingers is not fully validated. Back up the settings and apply?</source>
        <translation>Confirm that the selected folders contain this user&apos;s fingerprints.
%1

Existing enrolment is preserved and GPU comparison is used for sudo and KDE authentication. After at most 20 failed contacts or 60 seconds, the password path remains available. Rejection of other fingers is not fully validated. Back up the settings and apply?</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="632"/>
        <source>Restore the sudo, KDE and fprintd settings from before installation? Existing fingerprint enrolment is not deleted.</source>
        <translation>Restore the sudo, KDE and fprintd settings from before installation? Existing fingerprint enrolment is not deleted.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="658"/>
        <source>Fingerprint match succeeded. The result was delivered while the finger remained in place. Test the sudo/KDE dialog and password fallback separately.</source>
        <translation>Fingerprint match succeeded. The result was delivered while the finger remained in place. Test the sudo/KDE dialog and password fallback separately.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="659"/>
        <source>Sensor thermal protection stopped the test. Lift your finger, let it cool briefly, then try again.</source>
        <translation>Sensor thermal protection stopped the test. Lift your finger, let it cool briefly, then try again.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="660"/>
        <source>No finger was detected during this 30-second one-shot test. Clear the sensor, then place the enrolled finger in the centre for one or two seconds when prompted.</source>
        <translation>No finger was detected during this 30-second one-shot test. Clear the sensor, then place the enrolled finger in the centre for one or two seconds when prompted.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="661"/>
        <source>System fingerprint test incomplete: %1</source>
        <translation>System fingerprint test incomplete: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="671"/>
        <source>[System fingerprint test] Match confirmed while the finger remained in place.
</source>
        <translation>[System fingerprint test] Match confirmed while the finger remained in place.
</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="673"/>
        <source>[System fingerprint test] No match. This contact was not recorded or enrolled.
</source>
        <translation>[System fingerprint test] No match. This contact was not recorded or enrolled.
</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="675"/>
        <source>[System fingerprint test] Place the finger in the centre and hold it until a result appears. You do not need to lift it.
</source>
        <translation>[System fingerprint test] Place the finger in the centre and hold it until a result appears. You do not need to lift it.
</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="690"/>
        <source>[System fingerprint test] Start with the sensor clear. When prompted, place the same enrolled finger and hold it until the result appears. You do not need to lift it.</source>
        <translation>[System fingerprint test] Start with the sensor clear. When prompted, place the same enrolled finger and hold it until the result appears. You do not need to lift it.</translation>
    </message>
</context>
</TS>
