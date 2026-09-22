<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN" sourcelanguage="en">
<context>
    <name>fpstudio</name>
    <message>
        <location filename="../src/engine.cpp" line="86"/>
        <source>Finger detected — hold still</source>
        <translation>检测到手指 — 请保持不动</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="91"/>
        <source>Finger lifted</source>
        <translation>手指已抬起</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="104"/>
        <source>Only part of the sensor was touched — cover more of it, centred</source>
        <translation>只接触到一部分 — 请居中并覆盖更多区域</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="109"/>
        <source>The ridges are faint — press a little harder</source>
        <translation>纹路偏淡 — 请稍微用力按压</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="114"/>
        <source>Frame received</source>
        <translation>已接收帧</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="124"/>
        <location filename="../src/engine.cpp" line="134"/>
        <location filename="../src/headless.cpp" line="482"/>
        <source>Captured — now lift your finger</source>
        <translation>采集完成 — 请抬起手指</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="139"/>
        <source>Matching against the enrolled print…</source>
        <translation>正在与已注册指纹比对…</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="144"/>
        <location filename="../src/setupcheck.cpp" line="210"/>
        <source>Secure channel to the sensor established</source>
        <translation>已与传感器建立安全通道</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="149"/>
        <source>Sensor ready</source>
        <translation>传感器就绪</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="418"/>
        <location filename="../src/engine.cpp" line="499"/>
        <location filename="../src/engine.cpp" line="575"/>
        <source>Gave up after %1 seconds</source>
        <translation>%1 秒内未完成，已中止</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="461"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>注册 %1/%2 — 抬起手指后再次按下</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <location filename="../src/headless.cpp" line="93"/>
        <location filename="../src/headless.cpp" line="442"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>请将手指放在传感器上并保持不动</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="95"/>
        <location filename="../src/headless.cpp" line="443"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>正在注册 — 请反复按下并抬起手指</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="97"/>
        <location filename="../src/headless.cpp" line="444"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>请将已注册的手指放在传感器上</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="65"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>已发现 Goodix 27c6:55b4</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>A different Goodix sensor is present</source>
        <translation>存在其他 Goodix 传感器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="75"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>此构建仅支持 27c6:55b4。检测到 27c6:%1。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="81"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>未找到 Goodix 指纹传感器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="82"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>USB 总线上没有设备响应厂商 ID 27c6。如果本机确实配备读取器，可能已在固件设置中被禁用，或为其他厂商产品。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="101"/>
        <source>Patched driver is active</source>
        <translation>已启用修补版驱动</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="108"/>
        <location filename="../src/setupcheck.cpp" line="116"/>
        <source>Patched driver is not installed</source>
        <translation>未安装修补版驱动</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="109"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint 无法识别此传感器。需要构建并安装修补版分支，但本程序副本未附带相应源码。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="117"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint 无法识别此传感器。修补版分支会添加支持。构建需要几分钟，并会替换系统的 libfprint，因此需要密码。

请之后再安装 fprintd：如果先安装，会拉入官方 libfprint 并撤销此更改。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="122"/>
        <source>Build and install the patched libfprint</source>
        <translation>构建并安装修补版 libfprint</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="152"/>
        <source>Sensor is reachable without a password</source>
        <translation>无需密码即可访问传感器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="158"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>udev 规则已安装但尚未生效</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="159"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>每次传感器操作都会要求输入密码</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="161"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>规则文件已就位，但设备节点仍属于 root。它会在下一次 add 事件时生效，重新插拔读取器或重新加载 udev 即可。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="164"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>没有该规则，libusb 只能以 root 身份打开传感器，因此每次采集都会弹出密码提示。

此规则授予 wheel 组对传感器的读写权限。在单用户笔记本上，这与本来就能通过 pkexec 操作的人相同；在共享机器上则不同。跳过此项是安全的——只是会多几次密码提示。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="174"/>
        <source>Install the udev rule and reload</source>
        <translation>安装 udev 规则并重新加载</translation>
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
        <translation>传感器已应答，但握手未完成</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="218"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>传感器持有的密钥不是本驱动所拥有的。下一步是写入全零密钥，且此操作不可撤销。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="224"/>
        <source>Could not talk to the sensor</source>
        <translation>无法与传感器通信</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="225"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>握手过程未进展到足以说明原因。请先检查上方的驱动和权限步骤。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="238"/>
        <source>The sensor already holds a usable key</source>
        <translation>传感器已持有可用密钥</translation>
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
        <translation>需要向传感器写入全零密钥</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="252"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>此操作不可撤销。

传感器目前持有的密钥无法读回——协议返回的是由密钥派生出的值，而非密钥本身——因此之后没有备份可以恢复。

本机上的 Windows 指纹登录将永久失效。如果你双系统在 Windows 上也使用它，请到此为止。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="260"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>向传感器写入全零 PSK</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="290"/>
        <source>Image quality has not been measured yet</source>
        <translation>尚未测量图像质量</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="291"/>
        <source>Press the button below, then put your finger on the sensor and hold it there for a couple of seconds.</source>
        <translation>按下方按钮，然后将手指放在传感器上并保持几秒钟。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="294"/>
        <source>Test a capture now</source>
        <translation>立即测试采集</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="298"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>接触面 %1%  ·  清晰度 %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="307"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>低于 24 时，即使驱动接受该帧，匹配通常也会失败。手指干燥或传感器持续繁忙都会导致这种情况；休息几分钟并稍微湿润手指通常可解决。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="311"/>
        <source>Try again</source>
        <translation>重试</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="324"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>fprintd 中已注册手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="330"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>fprintd 中未注册任何手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="331"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>本程序单独保存自己的注册数据，系统认证看不到这些数据。通过 fprintd 重新注册后，指纹才能用于解锁。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="335"/>
        <source>Enrol a finger with fprintd</source>
        <translation>通过 fprintd 注册手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="340"/>
        <source>fprintd is not answering</source>
        <translation>fprintd 无响应</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="341"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>无法连接到守护进程。它通常按需自动启动，安装它可能就是唯一缺少的步骤。</translation>
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
        <translation>传感器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="488"/>
        <source>Driver</source>
        <translation>驱动</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="489"/>
        <source>Permissions</source>
        <translation>权限</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="490"/>
        <source>Secure channel</source>
        <translation>安全通道</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="491"/>
        <source>Sensor key</source>
        <translation>传感器密钥</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="492"/>
        <source>Image quality</source>
        <translation>图像质量</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="493"/>
        <source>Enrolment</source>
        <translation>注册</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="494"/>
        <source>Unlocking</source>
        <translation>解锁</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="516"/>
        <source>Not checked - no sensor</source>
        <translation>未检查 — 无传感器</translation>
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
        <translation>尚无图像

使用「采集」从传感器获取图像</translation>
    </message>
</context>
<context>
    <name>fpstudio::LiveWindow</name>
    <message>
        <location filename="../src/livewindow.cpp" line="189"/>
        <source>Vulkan GPU is initializing — it calculates separately from the live stream</source>
        <translation>正在初始化 Vulkan GPU — 它将与实时画面分开计算</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="198"/>
        <source>%1 · %2 references ready · waiting for a stable candidate</source>
        <translation>%1 · 已准备 %2 张参考图像 · 正在等待稳定的候选图像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="225"/>
        <source>GPU comparison error: </source>
        <translation>GPU 比对错误：</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="230"/>
        <source>GPU %1 hypotheses · %2 ms · image correlation %3 · ridge-direction agreement %4
Best: %5 / %6 — research comparison only; not an authentication decision</source>
        <translation>GPU %1 个假设 · %2 毫秒 · 图像相关性 %3 · 纹线方向一致性 %4
最佳：%5 / %6 — 仅供研究比对，并非身份验证决定</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="234"/>
        <source>
Alignment is ambiguous: a different position has a similar score</source>
        <translation>
对齐存在歧义：其他位置也有相近分数</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="235"/>
        <source>
At the search boundary: move the finger slightly toward the centre</source>
        <translation>
已到搜索范围边界：请将手指稍微移向中央</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="238"/>
        <source>
Interior ridge support %1/%2 regions · %3</source>
        <translation>
内部纹线支持 %1/%2 个区域 · %3</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="240"/>
        <source>ridges are consistent (identity unverified)</source>
        <translation>纹线一致（身份尚未验证）</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="240"/>
        <source>insufficient match evidence</source>
        <translation>匹配证据不足</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="243"/>
        <source> · 0/1 contact-region overlap %1%</source>
        <translation> · 0/1 接触区域重叠 %1%</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="245"/>
        <source> · 360° additional search applied</source>
        <translation> · 已应用 360° 额外搜索</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="247"/>
        <source> · full reference-bank search</source>
        <translation> · 全部参考库搜索</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="247"/>
        <source> · representative-reference fast comparison</source>
        <translation> · 代表参考快速比对</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="263"/>
        <source>GPU comparison cannot run: </source>
        <translation>无法运行 GPU 比对：</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="324"/>
        <source>Vulkan GPU is comparing reference images, angles, and positions in parallel…</source>
        <translation>Vulkan GPU 正在并行比对参考图像、角度和位置…</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="329"/>
        <source>GPU comparison timed out — live collection continues</source>
        <translation>GPU 比对超时 — 实时采集继续进行</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1017"/>
        <source>Contact %1 · image from comparison %2 (not live)
Gray: background (0) · yellow: contact (1), but weak features · green/red: comparison evidence
Cyan +: transformed reference contact centre · orange +: current probe contact centre · blue: comparison range</source>
        <translation>接触 %1 · 比对 %2 时的图像（非实时）
灰色：背景 (0) · 黄色：接触 (1)，但特征较弱 · 绿色/红色：比对证据
青色 +：变换后的参考接触中心 · 橙色 +：当前待测接触中心 · 蓝色：比对范围</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1360"/>
        <source>[%1/%2 contacts compared · stops automatically at the target] </source>
        <translation>[%1/%2 次接触已比对 · 达到目标时自动停止] </translation>
    </message>
</context>
<context>
    <name>fpstudio::MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="109"/>
        <location filename="../src/mainwindow.cpp" line="363"/>
        <source>Device scan</source>
        <translation>设备扫描</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="154"/>
        <source>Setup</source>
        <translation>设置</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="155"/>
        <source>Set up fingerprint unlock…</source>
        <translation>设置指纹解锁…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="158"/>
        <source>Language</source>
        <translation>语言</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="195"/>
        <source>fpstudio — fingerprint sensor diagnostics</source>
        <translation>fpstudio — 指纹传感器诊断</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="200"/>
        <source>Looking for a device…</source>
        <translation>正在查找设备…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="204"/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="206"/>
        <source>Rescan</source>
        <translation>重新扫描</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="211"/>
        <source>Capture — show image</source>
        <translation>采集 — 显示图像</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="212"/>
        <location filename="../src/mainwindow.cpp" line="373"/>
        <source>Enrol</source>
        <translation>注册</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="213"/>
        <location filename="../src/mainwindow.cpp" line="379"/>
        <source>Verify</source>
        <translation>验证</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="214"/>
        <location filename="../src/mainwindow.cpp" line="383"/>
        <source>Delete enrolment</source>
        <translation>删除注册</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="219"/>
        <source>Actions</source>
        <translation>操作</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="226"/>
        <source>Progress</source>
        <translation>进度</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="258"/>
        <source>—</source>
        <translation>—</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="295"/>
        <source>Coverage</source>
        <translation>接触面</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="297"/>
        <source>Sharpness</source>
        <translation>清晰度</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Sample</source>
        <translation>样本</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Score</source>
        <translation>分数</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Keypoints</source>
        <translation>特征点</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="314"/>
        <source>Match scores</source>
        <translation>匹配分数</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="324"/>
        <source>Driver log</source>
        <translation>驱动日志</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="366"/>
        <source>→ Put your finger on the sensor</source>
        <translation>→ 请将手指放在传感器上</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="367"/>
        <source>Image capture</source>
        <translation>图像采集</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="370"/>
        <source>→ Enrol: press and lift your finger repeatedly</source>
        <translation>→ 注册：反复按下并抬起手指</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="377"/>
        <source>→ Verify: present the enrolled finger</source>
        <translation>→ 验证：放上已注册的手指</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="394"/>
        <source>in progress…</source>
        <translation>进行中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="394"/>
        <source> in progress…</source>
        <translation> 进行中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="422"/>
        <source>No fingerprint device was found.</source>
        <translation>未找到指纹设备。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="431"/>
        <source>Driver: %1
ID: %2
Scan: %3 · %4 enrol stages
Raw capture: %5</source>
        <translation>驱动: %1
ID: %2
扫描: %3 · %4 个注册阶段
原始采集: %5</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="436"/>
        <source>yes</source>
        <translation>支持</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="436"/>
        <source>no</source>
        <translation>不支持</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="448"/>
        <source>No image was returned</source>
        <translation>未返回图像</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="452"/>
        <source>Captured %1 × %2</source>
        <translation>已采集 %1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="471"/>
        <source>Match   %1 / %2</source>
        <translation>匹配   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="472"/>
        <source>No match   %1 / %2</source>
        <translation>不匹配   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="483"/>
        <source>Failed — %1</source>
        <translation>失败 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484"/>
        <location filename="../src/mainwindow.cpp" line="642"/>
        <location filename="../src/mainwindow.cpp" line="643"/>
        <source>Failed</source>
        <translation>失败</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="485"/>
        <location filename="../src/mainwindow.cpp" line="644"/>
        <source>Error: %1</source>
        <translation>错误: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="489"/>
        <source>Done — %1</source>
        <translation>完成 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="494"/>
        <source>Enrolment complete</source>
        <translation>注册完成</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="496"/>
        <source>Enrolment deleted</source>
        <translation>注册已删除</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="523"/>
        <source>This scan fell below the threshold and was asked for again</source>
        <translation>本次扫描低于阈值，已重新请求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="538"/>
        <source>Coverage %1%  ·  sharpness %2  —  faint. Press a little harder</source>
        <translation>接触面 %1%  ·  清晰度 %2  —  偏淡，请稍微用力</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="540"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>接触面 %1%  ·  清晰度 %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="561"/>
        <source>%1 × %2</source>
        <translation>%1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="617"/>
        <source>Agent request</source>
        <translation>代理请求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="618"/>
        <source>CLI request</source>
        <translation>CLI 请求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="618"/>
        <source>request</source>
        <translation>请求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="634"/>
        <source>Idle</source>
        <translation>空闲</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="635"/>
        <source>▸ %1   ·   %2s elapsed</source>
        <translation>▸ %1   ·   已过 %2 秒</translation>
    </message>
</context>
<context>
    <name>fpstudio::PrivRunner</name>
    <message>
        <location filename="../src/privrunner.cpp" line="32"/>
        <source>Could not run pkexec</source>
        <translation>无法运行 pkexec</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="47"/>
        <source>The operation did not finish in time and was stopped</source>
        <translation>操作未在限定时间内完成，已停止</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="52"/>
        <source>The operation terminated abnormally</source>
        <translation>操作异常终止</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="59"/>
        <source>Authentication was cancelled or refused</source>
        <translation>认证被取消或拒绝</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="84"/>
        <source>Could not parse the result (exit code %1)</source>
        <translation>无法解析结果（退出码 %1）</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="147"/>
        <source>An operation is already running</source>
        <translation>已有操作在运行</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="151"/>
        <source>Could not resolve this program&apos;s own path</source>
        <translation>无法确定本程序自身的路径</translation>
    </message>
</context>
<context>
    <name>fpstudio::SetupWizard</name>
    <message>
        <location filename="../src/setupwizard.cpp" line="405"/>
        <source>Failed — %1</source>
        <translation>失败 — %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="150"/>
        <source>Skip</source>
        <translation>跳过</translation>
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
        <translation>打开诊断工具</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="240"/>
        <source>Live capture preview, the driver log, and MCP status - for a closer look once the checklist here is done.</source>
        <translation>提供实时采集预览、驱动日志和 MCP 状态——用于完成此清单后进一步深入查看。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="274"/>
        <source>Checking the device and installation. Keep your finger off the sensor for now.</source>
        <translation>Checking the device and installation. Keep your finger off the sensor for now.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="350"/>
        <source>This step cannot be undone.</source>
        <translation>此步骤无法撤销。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="366"/>
        <source>This will ask for your password.</source>
        <translation>这将要求输入你的密码。</translation>
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
        <translation>传感器密钥</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="382"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

输入 %2 以确认。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="406"/>
        <source>The enrolment helper was not found</source>
        <translation>找不到注册辅助程序</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="412"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>正在注册 — 请反复按下并抬起手指</translation>
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
        <translation>请将手指放在传感器上并保持不动</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="465"/>
        <source>Could not start the image tool: %1</source>
        <translation>Could not start the image tool: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="529"/>
        <source>in progress…</source>
        <translation>进行中…</translation>
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
        <translation>此步骤未在限定时间内完成，已停止。传感器和您已录入的指纹均未更改，请重试。</translation>
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
