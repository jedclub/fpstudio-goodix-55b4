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
        <location filename="../src/headless.cpp" line="476"/>
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
        <location filename="../src/setupcheck.cpp" line="209"/>
        <source>Secure channel to the sensor established</source>
        <translation>已與感測器建立安全通道</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="149"/>
        <source>Sensor ready</source>
        <translation>感測器就緒</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="417"/>
        <location filename="../src/engine.cpp" line="498"/>
        <location filename="../src/engine.cpp" line="574"/>
        <source>Gave up after %1 seconds</source>
        <translation>%1 秒內未完成，已中止</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="460"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>註冊 %1/%2 — 抬起手指後再次按下</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <location filename="../src/headless.cpp" line="87"/>
        <location filename="../src/headless.cpp" line="436"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>請將手指放在感測器上並保持不動</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="89"/>
        <location filename="../src/headless.cpp" line="437"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>正在註冊 — 請反覆按下並抬起手指</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="91"/>
        <location filename="../src/headless.cpp" line="438"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>請將已註冊的手指放在感測器上</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="64"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>已發現 Goodix 27c6:55b4</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="73"/>
        <source>A different Goodix sensor is present</source>
        <translation>存在其他 Goodix 感測器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>此組建僅支援 27c6:55b4。偵測到 27c6:%1。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="80"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>找不到 Goodix 指紋感測器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="81"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>USB 匯流排上沒有裝置回應廠商 ID 27c6。若本機確實配備讀取器，可能已在韌體設定中停用，或為其他廠牌產品。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="100"/>
        <source>Patched driver is active</source>
        <translation>已啟用修補版驅動程式</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="107"/>
        <location filename="../src/setupcheck.cpp" line="115"/>
        <source>Patched driver is not installed</source>
        <translation>未安裝修補版驅動程式</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="108"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint 無法辨識此感測器。需要建置並安裝修補版分支，但本程式副本未附帶相應原始碼。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="116"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint 無法辨識此感測器。修補版分支會加入支援。建置需要幾分鐘，並會取代系統的 libfprint，因此需要密碼。

請之後再安裝 fprintd：若先安裝，會引入官方 libfprint 並復原此變更。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="121"/>
        <source>Build and install the patched libfprint</source>
        <translation>建置並安裝修補版 libfprint</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="151"/>
        <source>Sensor is reachable without a password</source>
        <translation>無需密碼即可存取感測器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="157"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>udev 規則已安裝但尚未生效</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="158"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>每次感測器操作都會要求輸入密碼</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="160"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>規則檔已就位，但裝置節點仍屬於 root。它會在下一次 add 事件時生效，重新插拔讀取器或重新載入 udev 即可。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="163"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>沒有該規則，libusb 只能以 root 身分開啟感測器，因此每次擷取都會跳出密碼提示。

此規則授予 wheel 群組對感測器的讀寫權限。在單人筆電上，這與原本就能透過 pkexec 操作的人相同；在共用機器上則不同。略過此項是安全的——只是會多幾次密碼提示。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="173"/>
        <source>Install the udev rule and reload</source>
        <translation>安裝 udev 規則並重新載入</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="216"/>
        <source>The sensor answered, but the handshake did not complete</source>
        <translation>感測器已回應，但交握未完成</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="217"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>感測器持有的金鑰不是本驅動所擁有的。下一步是寫入全零金鑰，且此操作不可復原。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="223"/>
        <source>Could not talk to the sensor</source>
        <translation>無法與感測器通訊</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="224"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>交握過程未進展到足以說明原因。請先檢查上方的驅動與權限步驟。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="237"/>
        <source>The sensor already holds a usable key</source>
        <translation>感測器已持有可用金鑰</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="250"/>
        <source>The sensor needs the all-zero key written</source>
        <translation>需要向感測器寫入全零金鑰</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="251"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>此操作不可復原。

感測器目前持有的金鑰無法讀回——通訊協定回傳的是由金鑰衍生出的值，而非金鑰本身——因此之後沒有備份可以還原。

本機上的 Windows 指紋登入將永久失效。若你雙系統在 Windows 上也使用它，請到此為止。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="259"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>向感測器寫入全零 PSK</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="289"/>
        <source>Image quality has not been measured yet</source>
        <translation>尚未測量影像品質</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="290"/>
        <source>Press the button below, then put your finger on the sensor and hold it there for a couple of seconds.</source>
        <translation>按下方按鈕，然後將手指放在感測器上並保持幾秒鐘。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="293"/>
        <source>Test a capture now</source>
        <translation>立即測試擷取</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="297"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>接觸面 %1%  ·  清晰度 %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="306"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>低於 24 時，即使驅動接受該畫面，比對通常也會失敗。手指乾燥或感測器持續忙碌都會導致此情況；休息幾分鐘並稍微濕潤手指通常可解決。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="310"/>
        <source>Try again</source>
        <translation>重試</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="323"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>fprintd 中已註冊手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="329"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>fprintd 中未註冊任何手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="330"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>本程式單獨保存自己的註冊資料，系統驗證看不到這些資料。透過 fprintd 重新註冊後，指紋才能用於解鎖。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="334"/>
        <source>Enrol a finger with fprintd</source>
        <translation>透過 fprintd 註冊手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="339"/>
        <source>fprintd is not answering</source>
        <translation>fprintd 無回應</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="340"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>無法連接到守護行程。它通常會依需求自動啟動，安裝它可能就是唯一缺少的步驟。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="387"/>
        <source>polkit accepts a fingerprint</source>
        <translation>polkit 接受指紋</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="395"/>
        <location filename="../src/setupcheck.cpp" line="438"/>
        <source>pam_fprintd is not installed</source>
        <translation>未安裝 pam_fprintd</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="396"/>
        <location filename="../src/setupcheck.cpp" line="439"/>
        <source>The PAM module that lets authentication use a fingerprint is missing. It usually ships with fprintd.</source>
        <translation>缺少讓身分驗證使用指紋的 PAM 模組。它通常隨 fprintd 一起提供。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="404"/>
        <source>polkit still asks for a password</source>
        <translation>polkit 仍要求輸入密碼</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="405"/>
        <source>This adds one line to the stack polkit uses, so pkexec and the desktop&apos;s authentication dialog try the fingerprint first.

Login and sudo are deliberately left alone, so a sensor that stops working can never lock you out of the machine. The line is &apos;sufficient&apos;: if the fingerprint fails for any reason, you are asked for the password exactly as before.</source>
        <translation>這會在 polkit 使用的驗證堆疊中加入一行，讓 pkexec 和桌面驗證對話框優先嘗試指紋。

登入和 sudo 刻意保持不變，這樣即使感測器停止運作也不會把你鎖在機器之外。這一行是 &apos;sufficient&apos;：若指紋因任何原因失敗，仍會像以前一樣要求輸入密碼。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="413"/>
        <source>Let polkit accept a fingerprint</source>
        <translation>讓 polkit 接受指紋</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="430"/>
        <source>sudo accepts a fingerprint</source>
        <translation>sudo 接受指紋</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="451"/>
        <source>Terminal sudo still asks for a password only</source>
        <translation>終端機 sudo 目前仍只接受密碼</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="452"/>
        <source>Optional, and a step further than the polkit rule above: sudo is usually the way back in when something else on the machine is broken, so extending fingerprint auth to it is worth doing deliberately rather than by default.

The safety net is the same either way. The line added is &apos;sufficient&apos;, so a failing fingerprint falls back to the password exactly as before. sudo itself never stops working - only the fingerprint shortcut can.</source>
        <translation>這是可選項，比上面的 polkit 規則更進一步：sudo 通常是機器上其他東西出問題時的退路，因此為其擴充指紋驗證值得慎重決定，而非預設開啟。

安全網是一樣的。新增的一行是 &apos;sufficient&apos;，因此指紋失敗時會像以前一樣退回密碼。sudo 本身永遠不會失效——只有指紋這條捷徑可能失效。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="462"/>
        <source>Let sudo accept a fingerprint</source>
        <translation>讓 sudo 接受指紋</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="492"/>
        <source>Sensor</source>
        <translation>感測器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="493"/>
        <source>Driver</source>
        <translation>驅動程式</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="494"/>
        <source>Permissions</source>
        <translation>權限</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="495"/>
        <source>Secure channel</source>
        <translation>安全通道</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="496"/>
        <source>Sensor key</source>
        <translation>感測器金鑰</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="497"/>
        <source>Image quality</source>
        <translation>影像品質</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="498"/>
        <source>Enrolment</source>
        <translation>註冊</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="499"/>
        <source>Unlocking</source>
        <translation>解鎖</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="500"/>
        <source>Terminal sudo (optional)</source>
        <translation>終端機 sudo（可選）</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="521"/>
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
    <name>fpstudio::LiveWindow</name>
    <message>
        <location filename="../src/livewindow.cpp" line="110"/>
        <source>Vulkan GPU is initializing — it calculates separately from the live stream</source>
        <translation>正在初始化 Vulkan GPU — 它會與即時影像分開計算</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="119"/>
        <source>%1 · %2 references ready · waiting for a stable candidate</source>
        <translation>%1 · 已備妥 %2 張參考影像 · 正在等候穩定候選影像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="146"/>
        <source>GPU comparison error: </source>
        <translation>GPU 比對錯誤：</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="151"/>
        <source>GPU %1 hypotheses · %2 ms · image correlation %3 · ridge-direction agreement %4
Best: %5 / %6 — research comparison only; not an authentication decision</source>
        <translation>GPU %1 個假設 · %2 毫秒 · 影像相關性 %3 · 紋線方向一致性 %4
最佳：%5 / %6 — 僅供研究比對，並非身分驗證決定</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="155"/>
        <source>
Alignment is ambiguous: a different position has a similar score</source>
        <translation>
對齊存在歧義：其他位置也有相近分數</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="156"/>
        <source>
At the search boundary: move the finger slightly toward the centre</source>
        <translation>
已到搜尋範圍邊界：請將手指稍微移向中央</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="159"/>
        <source>
Interior ridge support %1/%2 regions · %3</source>
        <translation>
內部紋線支援 %1/%2 個區域 · %3</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="161"/>
        <source>ridges are consistent (identity unverified)</source>
        <translation>紋線一致（身分尚未驗證）</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="161"/>
        <source>insufficient match evidence</source>
        <translation>匹配證據不足</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="164"/>
        <source> · 0/1 contact-region overlap %1%</source>
        <translation> · 0/1 接觸區域重疊 %1%</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="166"/>
        <source> · 360° additional search applied</source>
        <translation> · 已套用 360° 額外搜尋</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="168"/>
        <source> · full reference-bank search</source>
        <translation> · 全部參考庫搜尋</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="168"/>
        <source> · representative-reference fast comparison</source>
        <translation> · 代表參考快速比對</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="184"/>
        <source>GPU comparison cannot run: </source>
        <translation>無法執行 GPU 比對：</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="245"/>
        <source>Vulkan GPU is comparing reference images, angles, and positions in parallel…</source>
        <translation>Vulkan GPU 正在並行比對參考影像、角度和位置…</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="250"/>
        <source>GPU comparison timed out — live collection continues</source>
        <translation>GPU 比對逾時 — 即時收集繼續進行</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="322"/>
        <source>Contact %1 · image from comparison %2 (not live)
Gray: background (0) · yellow: contact (1), but weak features · green/red: comparison evidence
Cyan +: transformed reference contact centre · orange +: current probe contact centre · blue: comparison range</source>
        <translation>接觸 %1 · 比對 %2 時的影像（非即時）
灰色：背景 (0) · 黃色：接觸 (1)，但特徵較弱 · 綠色/紅色：比對證據
青色 +：轉換後的參考接觸中心 · 橘色 +：目前待測接觸中心 · 藍色：比對範圍</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="508"/>
        <source>[%1/%2 contacts compared · stops automatically at the target] </source>
        <translation>[%1/%2 次接觸已比對 · 達到目標時自動停止] </translation>
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
        <location filename="../src/mainwindow.cpp" line="632"/>
        <location filename="../src/mainwindow.cpp" line="633"/>
        <source>Failed</source>
        <translation>失敗</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="475"/>
        <location filename="../src/mainwindow.cpp" line="634"/>
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
        <location filename="../src/mainwindow.cpp" line="551"/>
        <source>%1 × %2</source>
        <translation>%1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="607"/>
        <source>Agent request</source>
        <translation>代理請求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="608"/>
        <source>CLI request</source>
        <translation>CLI 請求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="608"/>
        <source>request</source>
        <translation>請求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="624"/>
        <source>Idle</source>
        <translation>閒置</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="625"/>
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
        <location filename="../src/setupwizard.cpp" line="355"/>
        <source>Failed — %1</source>
        <translation>失敗 — %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="125"/>
        <source>Skip</source>
        <translation>略過</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="140"/>
        <source>Finish</source>
        <translation>完成</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="187"/>
        <source>Open diagnostics tool</source>
        <translation>開啟診斷工具</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="190"/>
        <source>Live capture preview, the driver log, and MCP status - for a closer look once the checklist here is done.</source>
        <translation>提供即時擷取預覽、驅動記錄與 MCP 狀態——用於完成此清單後進一步深入查看。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="300"/>
        <source>This step cannot be undone.</source>
        <translation>此步驟無法復原。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="316"/>
        <source>This will ask for your password.</source>
        <translation>這將要求輸入你的密碼。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="329"/>
        <source>WRITE</source>
        <translation>WRITE</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="331"/>
        <source>Sensor key</source>
        <translation>感測器金鑰</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="332"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

輸入 %2 以確認。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="356"/>
        <source>The enrolment helper was not found</source>
        <translation>找不到註冊輔助程式</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="362"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>正在註冊 — 請反覆按下並抬起手指</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="402"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>請將手指放在感測器上並保持不動</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="472"/>
        <source>in progress…</source>
        <translation>進行中…</translation>
    </message>
</context>
</TS>
