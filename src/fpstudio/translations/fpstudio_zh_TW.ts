<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_TW" sourcelanguage="en">
<context>
    <name>fpstudio</name>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <source>Finger detected — hold still</source>
        <translation>偵測到手指 — 請保持不動</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="84"/>
        <source>Finger lifted</source>
        <translation>手指已抬起</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="97"/>
        <source>Only part of the sensor was touched — cover more of it, centred</source>
        <translation>只接觸到一部分 — 請置中並覆蓋更多區域</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="102"/>
        <source>The ridges are faint — press a little harder</source>
        <translation>紋路偏淡 — 請稍微用力按壓</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="107"/>
        <source>Frame received</source>
        <translation>已接收畫面</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="117"/>
        <location filename="../src/engine.cpp" line="127"/>
        <source>Captured — now lift your finger</source>
        <translation>擷取完成 — 請抬起手指</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="132"/>
        <source>Matching against the enrolled print…</source>
        <translation>正在與已註冊指紋比對…</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="137"/>
        <location filename="../src/setupcheck.cpp" line="195"/>
        <source>Secure channel to the sensor established</source>
        <translation>已與感測器建立安全通道</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="142"/>
        <source>Sensor ready</source>
        <translation>感測器就緒</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="403"/>
        <location filename="../src/engine.cpp" line="484"/>
        <location filename="../src/engine.cpp" line="560"/>
        <source>Gave up after %1 seconds</source>
        <translation>%1 秒內未完成，已中止</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="446"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>註冊 %1/%2 — 抬起手指後再次按下</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="81"/>
        <location filename="../src/headless.cpp" line="379"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>請將手指放在感測器上並保持不動</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="83"/>
        <location filename="../src/headless.cpp" line="380"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>正在註冊 — 請反覆按下並抬起手指</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="85"/>
        <location filename="../src/headless.cpp" line="381"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>請將已註冊的手指放在感測器上</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="58"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>已發現 Goodix 27c6:55b4</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="67"/>
        <source>A different Goodix sensor is present</source>
        <translation>存在其他 Goodix 感測器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="68"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>此組建僅支援 27c6:55b4。偵測到 27c6:%1。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>找不到 Goodix 指紋感測器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="75"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>USB 匯流排上沒有裝置回應廠商 ID 27c6。若本機確實配備讀取器，可能已在韌體設定中停用，或為其他廠牌產品。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="94"/>
        <source>Patched driver is active</source>
        <translation>已啟用修補版驅動程式</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="101"/>
        <location filename="../src/setupcheck.cpp" line="109"/>
        <source>Patched driver is not installed</source>
        <translation>未安裝修補版驅動程式</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="102"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint 無法辨識此感測器。需要建置並安裝修補版分支，但本程式副本未附帶相應原始碼。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="110"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint 無法辨識此感測器。修補版分支會加入支援。建置需要幾分鐘，並會取代系統的 libfprint，因此需要密碼。

請之後再安裝 fprintd：若先安裝，會引入官方 libfprint 並復原此變更。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="115"/>
        <source>Build and install the patched libfprint</source>
        <translation>建置並安裝修補版 libfprint</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="148"/>
        <source>Sensor is reachable without a password</source>
        <translation>無需密碼即可存取感測器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="154"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>udev 規則已安裝但尚未生效</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="155"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>每次感測器操作都會要求輸入密碼</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="157"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>規則檔已就位，但裝置節點仍屬於 root。它會在下一次 add 事件時生效，重新插拔讀取器或重新載入 udev 即可。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="160"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>沒有該規則，libusb 只能以 root 身分開啟感測器，因此每次擷取都會跳出密碼提示。

此規則授予 wheel 群組對感測器的讀寫權限。在單人筆電上，這與原本就能透過 pkexec 操作的人相同；在共用機器上則不同。略過此項是安全的——只是會多幾次密碼提示。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="166"/>
        <source>Install the udev rule and reload</source>
        <translation>安裝 udev 規則並重新載入</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="203"/>
        <source>The sensor answered, but the handshake did not complete</source>
        <translation>感測器已回應，但交握未完成</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="204"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>感測器持有的金鑰不是本驅動所擁有的。下一步是寫入全零金鑰，且此操作不可復原。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="210"/>
        <source>Could not talk to the sensor</source>
        <translation>無法與感測器通訊</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="211"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>交握過程未進展到足以說明原因。請先檢查上方的驅動與權限步驟。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="224"/>
        <source>The sensor already holds a usable key</source>
        <translation>感測器已持有可用金鑰</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="230"/>
        <source>The sensor needs the all-zero key written</source>
        <translation>需要向感測器寫入全零金鑰</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="231"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>此操作不可復原。

感測器目前持有的金鑰無法讀回——通訊協定回傳的是由金鑰衍生出的值，而非金鑰本身——因此之後沒有備份可以還原。

本機上的 Windows 指紋登入將永久失效。若你雙系統在 Windows 上也使用它，請到此為止。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="238"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>向感測器寫入全零 PSK</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="265"/>
        <source>Image quality has not been measured yet</source>
        <translation>尚未測量影像品質</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="266"/>
        <source>Present a finger on the next page and the reading will appear here.</source>
        <translation>在下一頁放上手指後，讀數會顯示在這裡。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="271"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>接觸面 %1%  ·  清晰度 %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="280"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>低於 24 時，即使驅動接受該畫面，比對通常也會失敗。手指乾燥或感測器持續忙碌都會導致此情況；休息幾分鐘並稍微濕潤手指通常可解決。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="296"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>fprintd 中已註冊手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="302"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>fprintd 中未註冊任何手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="303"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>本程式單獨保存自己的註冊資料，系統驗證看不到這些資料。透過 fprintd 重新註冊後，指紋才能用於解鎖。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="307"/>
        <source>Enrol a finger with fprintd</source>
        <translation>透過 fprintd 註冊手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="312"/>
        <source>fprintd is not answering</source>
        <translation>fprintd 無回應</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="313"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>無法連接到守護行程。它通常會依需求自動啟動，安裝它可能就是唯一缺少的步驟。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="329"/>
        <source>polkit accepts a fingerprint</source>
        <translation>polkit 接受指紋</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="337"/>
        <source>pam_fprintd is not installed</source>
        <translation>未安裝 pam_fprintd</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="338"/>
        <source>The PAM module that lets authentication use a fingerprint is missing. It usually ships with fprintd.</source>
        <translation>缺少讓身分驗證使用指紋的 PAM 模組。它通常隨 fprintd 一起提供。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="345"/>
        <source>polkit still asks for a password</source>
        <translation>polkit 仍要求輸入密碼</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="346"/>
        <source>This adds one line to the stack polkit uses, so pkexec and the desktop&apos;s authentication dialog try the fingerprint first.

Login and sudo are deliberately left alone, so a sensor that stops working can never lock you out of the machine. The line is &apos;sufficient&apos;: if the fingerprint fails for any reason, you are asked for the password exactly as before.</source>
        <translation>這會在 polkit 使用的驗證堆疊中加入一行，讓 pkexec 和桌面驗證對話框優先嘗試指紋。

登入和 sudo 刻意保持不變，這樣即使感測器停止運作也不會把你鎖在機器之外。這一行是 'sufficient'：若指紋因任何原因失敗，仍會像以前一樣要求輸入密碼。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="353"/>
        <source>Let polkit accept a fingerprint</source>
        <translation>讓 polkit 接受指紋</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="378"/>
        <source>Sensor</source>
        <translation>感測器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="379"/>
        <source>Driver</source>
        <translation>驅動程式</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="380"/>
        <source>Permissions</source>
        <translation>權限</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="381"/>
        <source>Secure channel</source>
        <translation>安全通道</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="382"/>
        <source>Sensor key</source>
        <translation>感測器金鑰</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="383"/>
        <source>Image quality</source>
        <translation>影像品質</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="384"/>
        <source>Enrolment</source>
        <translation>註冊</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="385"/>
        <source>Unlocking</source>
        <translation>解鎖</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="404"/>
        <source>Not checked - no sensor</source>
        <translation>未檢查 — 無感測器</translation>
    </message>
</context>
<context>
    <name>fpstudio::ImageView</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="57"/>
        <source>No frame yet

Use Capture to pull an image from the sensor</source>
        <translation>尚無影像

使用「擷取」從感測器取得影像</translation>
    </message>
</context>
<context>
    <name>fpstudio::MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="108"/>
        <location filename="../src/mainwindow.cpp" line="355"/>
        <source>Device scan</source>
        <translation>裝置掃描</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="153"/>
        <source>Setup</source>
        <translation>設定</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="154"/>
        <source>Set up fingerprint unlock…</source>
        <translation>設定指紋解鎖…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="157"/>
        <source>Language</source>
        <translation>語言</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="194"/>
        <source>fpstudio — fingerprint sensor diagnostics</source>
        <translation>fpstudio — 指紋感測器診斷</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="199"/>
        <source>Looking for a device…</source>
        <translation>正在尋找裝置…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="203"/>
        <source>Device</source>
        <translation>裝置</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="205"/>
        <source>Rescan</source>
        <translation>重新掃描</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="210"/>
        <source>Capture — show image</source>
        <translation>擷取 — 顯示影像</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="211"/>
        <location filename="../src/mainwindow.cpp" line="364"/>
        <source>Enrol</source>
        <translation>註冊</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="212"/>
        <location filename="../src/mainwindow.cpp" line="369"/>
        <source>Verify</source>
        <translation>驗證</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="213"/>
        <location filename="../src/mainwindow.cpp" line="373"/>
        <source>Delete enrolment</source>
        <translation>刪除註冊</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="218"/>
        <source>Actions</source>
        <translation>操作</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="225"/>
        <source>Progress</source>
        <translation>進度</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="257"/>
        <source>—</source>
        <translation>—</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="294"/>
        <source>Coverage</source>
        <translation>接觸面</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="296"/>
        <source>Sharpness</source>
        <translation>清晰度</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Sample</source>
        <translation>樣本</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Score</source>
        <translation>分數</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Keypoints</source>
        <translation>特徵點</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="313"/>
        <source>Match scores</source>
        <translation>比對分數</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="323"/>
        <source>Driver log</source>
        <translation>驅動程式記錄</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="358"/>
        <source>→ Put your finger on the sensor</source>
        <translation>→ 請將手指放在感測器上</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="359"/>
        <source>Image capture</source>
        <translation>影像擷取</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="362"/>
        <source>→ Enrol: press and lift your finger repeatedly</source>
        <translation>→ 註冊：反覆按下並抬起手指</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="367"/>
        <source>→ Verify: present the enrolled finger</source>
        <translation>→ 驗證：放上已註冊的手指</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source>in progress…</source>
        <translation>進行中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source> in progress…</source>
        <translation> 進行中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="412"/>
        <source>No fingerprint device was found.</source>
        <translation>找不到指紋裝置。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="421"/>
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
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>yes</source>
        <translation>支援</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>no</source>
        <translation>不支援</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="438"/>
        <source>No image was returned</source>
        <translation>未回傳影像</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="442"/>
        <source>Captured %1 × %2</source>
        <translation>已擷取 %1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="461"/>
        <source>Match   %1 / %2</source>
        <translation>符合   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="462"/>
        <source>No match   %1 / %2</source>
        <translation>不符合   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="473"/>
        <source>Failed — %1</source>
        <translation>失敗 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="474"/>
        <location filename="../src/mainwindow.cpp" line="623"/>
        <location filename="../src/mainwindow.cpp" line="624"/>
        <source>Failed</source>
        <translation>失敗</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="475"/>
        <location filename="../src/mainwindow.cpp" line="625"/>
        <source>Error: %1</source>
        <translation>錯誤: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="479"/>
        <source>Done — %1</source>
        <translation>完成 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484"/>
        <source>Enrolment complete</source>
        <translation>註冊完成</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="486"/>
        <source>Enrolment deleted</source>
        <translation>註冊已刪除</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513"/>
        <source>This scan fell below the threshold and was asked for again</source>
        <translation>本次掃描低於門檻，已重新要求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="528"/>
        <source>Coverage %1%  ·  sharpness %2  —  faint. Press a little harder</source>
        <translation>接觸面 %1%  ·  清晰度 %2  —  偏淡，請稍微用力</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="530"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>接觸面 %1%  ·  清晰度 %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="598"/>
        <source>Agent request</source>
        <translation>代理請求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>CLI request</source>
        <translation>CLI 請求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>request</source>
        <translation>請求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="615"/>
        <source>Idle</source>
        <translation>閒置</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="616"/>
        <source>▸ %1   ·   %2s elapsed</source>
        <translation>▸ %1   ·   已過 %2 秒</translation>
    </message>
</context>
<context>
    <name>fpstudio::PrivRunner</name>
    <message>
        <location filename="../src/privrunner.cpp" line="22"/>
        <source>Could not run pkexec</source>
        <translation>無法執行 pkexec</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="31"/>
        <source>The operation terminated abnormally</source>
        <translation>作業異常終止</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="38"/>
        <source>Authentication was cancelled or refused</source>
        <translation>驗證被取消或拒絕</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="63"/>
        <source>Could not parse the result (exit code %1)</source>
        <translation>無法解析結果（結束碼 %1）</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="108"/>
        <source>An operation is already running</source>
        <translation>已有作業在執行</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="112"/>
        <source>Could not resolve this program&apos;s own path</source>
        <translation>無法確定本程式自身的路徑</translation>
    </message>
</context>
<context>
    <name>fpstudio::SetupWizard</name>
    <message>
        <location filename="../src/setupwizard.cpp" line="65"/>
        <source>The command did not finish in time</source>
        <translation>命令未在規定時間內完成</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="73"/>
        <source>Authentication was cancelled or refused</source>
        <translation>驗證被取消或拒絕</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="74"/>
        <location filename="../src/setupwizard.cpp" line="273"/>
        <location filename="../src/setupwizard.cpp" line="332"/>
        <source>Failed — %1</source>
        <translation>失敗 — %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="99"/>
        <source>Set up fingerprint unlock</source>
        <translation>設定指紋解鎖</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="130"/>
        <source>Skip</source>
        <translation>略過</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="131"/>
        <source>Re-check</source>
        <translation>重新檢查</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="166"/>
        <source>Checking…</source>
        <translation>正在檢查…</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="226"/>
        <source>This step cannot be undone.</source>
        <translation>此步驟無法復原。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="239"/>
        <source>This will ask for your password.</source>
        <translation>這將要求輸入你的密碼。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="251"/>
        <source>WRITE</source>
        <translation>WRITE</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="253"/>
        <source>Sensor key</source>
        <translation>感測器金鑰</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="254"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

輸入 %2 以確認。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="274"/>
        <source>The enrolment helper was not found</source>
        <translation>找不到註冊輔助程式</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="280"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>正在註冊 — 請反覆按下並抬起手指</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="316"/>
        <source>in progress…</source>
        <translation>進行中…</translation>
    </message>
</context>
</TS>
