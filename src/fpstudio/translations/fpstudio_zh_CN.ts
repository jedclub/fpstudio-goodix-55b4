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
        <location filename="../src/livewindow.cpp" line="39"/>
        <source>a different finger from the last one</source>
        <translation>与刚才不同的手指</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="40"/>
        <source>the same finger used so far</source>
        <translation>目前一直使用的同一根手指</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="41"/>
        <source>fpstudio · live fingerprint stream</source>
        <translation>fpstudio · 指纹实时图像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="51"/>
        <source>⚠ Different-finger trial — lift your finger off the sensor. Calibration starts automatically.</source>
        <translation>⚠ 不同手指测试 — 请把手指从传感器上移开。稍后会自动校准。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="52"/>
        <source>Lift your finger off the sensor. Calibration starts automatically.</source>
        <translation>请把手指从传感器上移开。稍后会自动校准。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="73"/>
        <source>After calibration, rest the same finger lightly on the sensor and follow the prompts. Candidates are compared automatically for each contact.
Live sensor image 108 × 88 · background subtracted, contrast normalised · not an authentication decision · up to 3 minutes</source>
        <translation>校准后请把同一根手指轻轻放在传感器上，并按提示操作。每次接触都会自动比对候选图。
实时传感器图像 108 × 88 · 已减去背景、对比度归一化 · 不是认证判定 · 最长 3 分钟</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="77"/>
        <source>This is a different-finger comparison trial. Do not use the finger from the previous run this time.
After calibration, tap and lift repeatedly with one other finger. Even a high score does not mean authentication was granted.</source>
        <translation>这是与其他手指的比对测试。这次请勿使用上一轮用过的手指。
校准后请用另一根手指反复轻触并抬起。分数高也不代表认证已通过。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="79"/>
        <source>Waiting for calibration — no live image yet</source>
        <translation>等待校准 — 尚无实时图像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="96"/>
        <source>0 FPS · waiting for a new frame</source>
        <translation>0 FPS · 等待新帧</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="99"/>
        <source>GPU comparison: no reference image set · collecting live only</source>
        <translation>GPU 比对：未指定参考图像 · 仅进行实时采集</translation>
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
        <translation>将此指纹安装为登记数据</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="114"/>
        <source>Coverage map — drawn here as contacts accumulate
The dark red areas have not been touched yet
Move your finger toward the empty side
This is enrolment guidance, not an authentication decision</source>
        <translation>覆盖图 — 随着接触累积在此绘制
深红色区域是尚未触及的位置
请把手指移向空白的一侧
这是登记引导，不是认证判定</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="132"/>
        <source>The enrolment is installed. sudo and the lock screen now use this fingerprint.</source>
        <translation>登记数据已安装。sudo 与锁屏现在会使用此指纹。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="133"/>
        <source>Installing the enrolment failed. The existing enrolment is unchanged.</source>
        <translation>登记数据安装失败。原有登记数据保持不变。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="136"/>
        <source>Installation failed</source>
        <translation>安装失败</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="137"/>
        <source>The installer exited with code %1.</source>
        <translation>安装工具以代码 %1 结束。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="144"/>
        <source>Could not start. Check for another sensor session, or the permissions on the save folder.</source>
        <translation>无法启动。请检查是否有其他传感器会话，或保存文件夹的权限。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="157"/>
        <source>Could not run the capture program: </source>
        <translation>无法运行采集程序：</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="158"/>
        <location filename="../src/livewindow.cpp" line="167"/>
        <location filename="../src/livewindow.cpp" line="1054"/>
        <source>Close</source>
        <translation>关闭</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="164"/>
        <source>Stopped. You can lift your finger off the sensor.</source>
        <translation>已停止。可以把手指从传感器上移开。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="165"/>
        <source>The live stream has ended. Lift your finger off the sensor.</source>
        <translation>实时图像已结束。请把手指从传感器上移开。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="166"/>
        <source>0 FPS · ended · the last image is a still · %1 frames in total</source>
        <translation>0 FPS · 已结束 · 最后一张是静止画面 · 共 %1 帧</translation>
    </message>
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
        <location filename="../src/livewindow.cpp" line="269"/>
        <source>The GPU comparison worker exited unexpectedly (code %1)</source>
        <translation>GPU 比对进程意外退出（代码 %1）</translation>
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
        <location filename="../src/livewindow.cpp" line="783"/>
        <source>added</source>
        <translation>已采纳</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="784"/>
        <source>smeared · not added</source>
        <translation>模糊 · 未采纳</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="785"/>
        <source>registration failed · not added</source>
        <translation>配准失败 · 未采纳</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="786"/>
        <source>too little overlap · not added</source>
        <translation>重叠不足 · 未采纳</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="794"/>
        <source>last contact · %1</source>
        <translation>最后一次接触 · %1</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="798"/>
        <source>Coverage %1× · %2 placed · %3 not added
Dark red is what has not been touched yet · the green outline is what was just added</source>
        <translation>覆盖范围 %1 倍 · 已放置 %2 个 · 未采纳 %3 个
深红色是尚未触及的位置 · 绿色边框是刚刚采纳的位置</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="852"/>
        <location filename="../src/livewindow.cpp" line="885"/>
        <source>Cannot install</source>
        <translation>无法安装</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="853"/>
        <source>No capture has been placed on the map. Rest your finger on the sensor, move it slowly, and collect again.</source>
        <translation>没有任何采集图被放入覆盖图。请把手指放在传感器上缓慢移动后重新采集。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="858"/>
        <source>Install fingerprint enrolment</source>
        <translation>安装指纹登记数据</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="859"/>
        <source>Install %1 captures from this session as the authentication enrolment.

The existing enrolment is backed up and replaced; fprintd enrolment and PAM settings are left alone.
This matcher has not been validated for rejecting other people. Continue?</source>
        <translation>将本次会话的 %1 张采集图安装为认证登记数据。

原有登记数据会先备份再替换；fprintd 登记与 PAM 设置保持不变。
此匹配器尚未验证能否拒绝他人。是否继续？</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="886"/>
        <source>Could not find the installer (tools/auth_install.py).</source>
        <translation>找不到安装工具（tools/auth_install.py）。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="895"/>
        <source>Installing the enrolment…</source>
        <translation>正在安装登记数据…</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1038"/>
        <source>Contact %1 · image from comparison %2 (not live)
Gray: background (0) · yellow: contact (1), but weak features · green/red: comparison evidence
Cyan +: transformed reference contact centre · orange +: current probe contact centre · blue: comparison range</source>
        <translation>接触 %1 · 比对 %2 时的图像（非实时）
灰色：背景 (0) · 黄色：接触 (1)，但特征较弱 · 绿色/红色：比对证据
青色 +：变换后的参考接触中心 · 橙色 +：当前待测接触中心 · 蓝色：比对范围</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1051"/>
        <source>Capture finished — lift your finger. %1 remaining contacts are still being compared</source>
        <translation>采集结束 — 请抬起手指。剩余 %1 次接触仍在比对中</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1059"/>
        <source>The target of %1 contacts has been compared — no need to repeat. You can lift your finger.</source>
        <translation>已完成目标的 %1 次接触比对 — 无需重复。可以抬起手指。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1059"/>
        <source>Stopped. You can lift your finger.</source>
        <translation>已停止。可以抬起手指。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1060"/>
        <source>Capture and candidate comparison have ended. See the per-contact totals below.</source>
        <translation>采集与候选比对已结束。请查看下方按接触统计的结果。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1101"/>
        <source>Capture finished · %1 frames · %2 contacts detected
%3 comparisons answered · %4 ridge-consistent · %5 not compared (%6 with no candidate) · %7 queued · %8 errors
Research results on selected data, not an authentication success rate</source>
        <translation>采集结束 · %1 帧 · 检测到接触 %2 次
比对应答 %3 次 · 纹线一致 %4 次 · 未比对 %5 次（其中无候选 %6 次）· 等待 %7 次 · 错误 %8 个
这是对筛选数据的研究结果，不是认证成功率</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1109"/>
        <source>Keep your finger off — connecting to the sensor and calibrating the background</source>
        <translation>请继续抬着手指 — 正在连接传感器并校准背景</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1158"/>
        <source>Lift your finger — background calibration in %1 s</source>
        <translation>请抬起手指 — %1 秒后校准背景</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1167"/>
        <source>Keep your finger off — collecting more background from the live stream</source>
        <translation>请继续抬着手指 — 正在从实时图像中补充采集背景</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1170"/>
        <source>0 FPS · waiting for the first image · %1 s</source>
        <translation>0 FPS · 正在等待第一张图像 · %1 秒</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1349"/>
        <source>Keep your finger off and hold — collecting the background over several frames (do not touch the sensor yet)</source>
        <translation>请抬着手指等待 — 正在用多帧采集背景（此时请勿触碰传感器）</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1350"/>
        <source>No finger detected — now rest %1 lightly in the centre of the sensor</source>
        <translation>未检测到手指 — 现在请把%1轻轻放在传感器中央</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1353"/>
        <source>Warning: the other finger also produced ridge-match evidence. A suspected false accept has been recorded — lift and touch again</source>
        <translation>注意：其他手指也出现了纹线吻合的迹象。已记录为疑似误识 — 请抬起后重新放置</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1354"/>
        <source>This other finger produced no match evidence. Recorded — lift completely and touch again</source>
        <translation>这根其他手指没有出现吻合迹象。已记录 — 请完全抬起后重新放置</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1356"/>
        <source>This contact&apos;s ridges were confirmed — lift completely and rest %1 on the sensor again (not an authentication decision)</source>
        <translation>已确认本次接触的纹线 — 请完全抬起，然后把%1重新放在传感器上（不是认证判定）</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1358"/>
        <source>A sharp image was captured — comparing automatically. You can lift your finger</source>
        <translation>已取得清晰图像 — 正在自动比对。可以抬起手指</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1361"/>
        <source>Three comparisons of this contact found too little evidence — lift completely, then place %1 in the centre again</source>
        <translation>本次接触比对 3 次仍证据不足 — 请完全抬起，然后把%1重新放到中央</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1362"/>
        <source>Too little match evidence — move %1 slightly toward the centre and press more lightly. It will be compared again automatically</source>
        <translation>吻合证据不足 — 请把%1稍微移向中央并减轻力度。系统会自动重新比对</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1363"/>
        <source>The contact area is small — lay the pad of %1 flat across the centre of the sensor</source>
        <translation>接触面积偏小 — 请把%1的指腹平放在传感器中央</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1364"/>
        <source>You are pressing hard — ease off and just rest the finger. Quality falls below 0.79 in this range</source>
        <translation>按压力度偏大 — 请放松力度，像轻放一样。在此区间品质会降到 0.79 以下</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1365"/>
        <source>Hold your finger still for a moment — a stable image will be selected automatically</source>
        <translation>请让手指稍微静止 — 系统会自动选取稳定的图像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1366"/>
        <source>The ridges are unclear — ease off a little and shift %1 slightly</source>
        <translation>纹线不清晰 — 请稍微放松力度，并把%1的位置略作调整</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1367"/>
        <source>Hold it there for a moment — selecting a sharp image automatically</source>
        <translation>请保持一下 — 正在自动选取清晰图像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1368"/>
        <source>⚠ Use a different finger · </source>
        <translation>⚠ 请换一根手指 · </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1371"/>
        <source>The research storage and comparison limit has been reached — this contact will not be compared further. Please stop</source>
        <translation>已达到研究用存储与比对上限 — 本次接触不再继续比对。请停止</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1372"/>
        <source>GPU comparison is unavailable — showing the image only. Please stop</source>
        <translation>无法使用 GPU 比对 — 仅显示图像。请停止</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1376"/>
        <source>⚠ The ridges were smeared, so this was not added to the map — the red outline on the map is where it would have gone. Ease off and press the same spot again</source>
        <translation>⚠ 纹线模糊，因此未加入覆盖图 — 图上的红色边框就是该位置。请放松力度并在同一位置重新按压</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1379"/>
        <source>⚠ This could not be fitted to the map — press so that it overlaps an already filled area by about half</source>
        <translation>⚠ 无法与覆盖图对齐 — 请按压时与已填满的区域重叠约一半</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1382"/>
        <source>⚠ This does not overlap anything already on the map — continue from the edge of a filled area</source>
        <translation>⚠ 与覆盖图上已有区域没有任何重叠 — 请从已填满区域的边缘继续</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1386"/>
        <source>[%1/%2 contacts compared · stops automatically at the target] </source>
        <translation>[%1/%2 次接触已比对 · 达到目标时自动停止] </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1390"/>
        <source>%1 FPS · %2 frames · ridge signal %3 · contrast area %4% · sharpness %5
</source>
        <translation>%1 FPS · %2 帧 · 纹线信号 %3 · 对比面积 %4% · 清晰度 %5
</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1393"/>
        <source>%1 · %2 contacts · %3 candidates · %4 GPU comparisons done / %5 contacts queued (not validated for authentication)</source>
        <translation>%1 · 接触 %2 次 · 候选 %3 张 · GPU 完成 %4 件 / 等待 %5 次接触（未针对认证验证）</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1395"/>
        <source>
Raw difference %1 · background noise %2 · %3</source>
        <translation>
原始差值 %1 · 背景噪声 %2 · %3</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1396"/>
        <source>collecting background</source>
        <translation>正在采集背景</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1396"/>
        <source>finger in contact</source>
        <translation>手指接触中</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1396"/>
        <source>no finger</source>
        <translation>无手指</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1401"/>
        <source>The image stream has stopped — the screen shows the last frame</source>
        <translation>图像接收已停止 — 屏幕显示的是最后一帧</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1402"/>
        <source>0 FPS · no new image for %1 s</source>
        <translation>0 FPS · 已有 %1 秒没有新图像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1439"/>
        <source>GPU comparison finished · %1 processed · stopped normally on reaching the target of %2 contacts
The results and colour-marked images have been saved. This did not decide whether authentication is granted.</source>
        <translation>GPU 比对完成 · 已处理 %1 件 · 达到 %2 次接触的目标后正常结束
结果与彩色标注图像均已保存。这并未判定认证是否通过。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1440"/>
        <source>GPU comparison stopped · %1 finished · %2 cancelled in progress
Stopped at your request. Saved results are kept.</source>
        <translation>GPU 比对已停止 · 完成 %1 件 · 进行中取消 %2 件
已按您的要求停止。已保存的结果予以保留。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1441"/>
        <source>Stopping — lift your finger</source>
        <translation>正在停止 — 请抬起手指</translation>
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
    <name>fpstudio::ResearchWindow</name>
    <message>
        <location filename="../src/researchwindow.cpp" line="43"/>
        <source>fpstudio · fingerprint research session</source>
        <translation>fpstudio · 指纹研究会话</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="55"/>
        <source>The captured fingerprint image will appear here</source>
        <translation>采集到的指纹图像将显示在这里</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="58"/>
        <source>No image · nothing captured yet</source>
        <translation>无图像 · 尚未采集</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="60"/>
        <source>Stop</source>
        <translation>停止</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="68"/>
        <source>Another research session is already running</source>
        <translation>已有另一个研究会话在运行</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="69"/>
        <location filename="../src/researchwindow.cpp" line="230"/>
        <location filename="../src/researchwindow.cpp" line="248"/>
        <source>Close</source>
        <translation>关闭</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="74"/>
        <source>Could not create the save folder</source>
        <translation>无法创建保存文件夹</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="88"/>
        <source>Could not start the capture program</source>
        <translation>无法启动采集程序</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="94"/>
        <source>Get your right index finger ready. Do not touch the sensor yet.</source>
        <translation>请准备好右手食指。暂时不要触碰传感器。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="95"/>
        <source>Plan: 6 reference images → 4 verification images of the same finger → 4 comparison images of the right middle finger.
No buttons to press. Follow the finger and position prompts on screen. You can stop at any time.</source>
        <translation>计划：基准图 6 张 → 同一手指的验证图 4 张 → 右手中指的比对图 4 张。
无需按任何按钮。请按屏幕上关于手指与位置的提示操作。随时可以停止。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="107"/>
        <source>Reference images</source>
        <translation>基准图像</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="107"/>
        <source>Separate verification images</source>
        <translation>单独验证图像</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="107"/>
        <source>Different-finger comparison</source>
        <translation>与其他手指的比对</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="111"/>
        <source>the broad pad of your right middle finger, centred</source>
        <translation>右手中指的宽指腹，居中</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="112"/>
        <source>your right index finger, a touch left of centre</source>
        <translation>右手食指，略偏左于中央</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="113"/>
        <source>your right index finger, a touch right of centre</source>
        <translation>右手食指，略偏右于中央</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="114"/>
        <source>the broad pad of your right index finger, centred</source>
        <translation>右手食指的宽指腹，居中</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="125"/>
        <source>%1 · saved %2 / %3</source>
        <translation>%1 · 已保存 %2 / %3</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="143"/>
        <source>① Lift your finger clear of the sensor · 5 s</source>
        <translation>① 请把手指完全移离传感器 · 5 秒</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="144"/>
        <source>The empty sensor is measured first. Wait until this changes to &apos;Touch now&apos;.</source>
        <translation>先测量空的传感器。请等到此处变为“现在放置”。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="149"/>
        <source>① Keep it off and wait · preparing the sensor</source>
        <translation>① 请继续移开并等待 · 正在准备传感器</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="150"/>
        <source>This changes to &apos;Touch now&apos; automatically once the background measurement is done.</source>
        <translation>背景测量完成后，此处会自动变为“现在放置”。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="164"/>
        <source>① Lift your finger clear · %1 s</source>
        <translation>① 请完全抬起手指 · %1 秒</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="168"/>
        <source>%1 · sensor ready in %2 s</source>
        <translation>%1 · %2 秒后传感器就绪</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="173"/>
        <source>6 reference images done. Starting the separate verification.</source>
        <translation>6 张基准图像已完成。开始单独验证。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="174"/>
        <source>Now switch to your right middle finger.</source>
        <translation>现在请换成右手中指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="175"/>
        <source>Four more images with the same right index finger, placed afresh. These are evaluated separately from the reference images.</source>
        <translation>用同一根右手食指重新放置再采集 4 张。这些图像与基准图像分开评估。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="176"/>
        <source>Four images to check whether a different finger is wrongly accepted. Do not touch the sensor yet.</source>
        <translation>这 4 张用于确认是否会误接受其他手指。暂时不要触碰传感器。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="188"/>
        <source>② Touch now — %1</source>
        <translation>② 现在放置 — %1</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="189"/>
        <source>Cover the whole sensor, press lightly and hold. Lift when the prompt says so, after the image appears.</source>
        <translation>请覆盖整个传感器，轻轻按压并保持。图像出现后，按提示抬起即可。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="191"/>
        <source>③ Hold your finger still · capturing</source>
        <translation>③ 请保持手指不动 · 正在采集</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="192"/>
        <source>Contact confirmed. Do not move your finger.</source>
        <translation>已确认接触。请不要移动手指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="194"/>
        <source>④ Captured. Lift your finger clear.</source>
        <translation>④ 已采集。请完全抬起手指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="195"/>
        <source>The image and its quality will be shown in a moment.</source>
        <translation>稍后会显示采集图像及其品质。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="221"/>
        <source>Image just captured · %1×%2 · contact %3% · sharpness %4 · background difference %5</source>
        <translation>刚刚采集的图像 · %1×%2 · 接触率 %3% · 清晰度 %4 · 背景差值 %5</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="228"/>
        <source>All 14 images collected — you can rest your hand.</source>
        <translation>14 张已全部采集完成 — 可以让手休息了。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="229"/>
        <source>The reference, verification and different-finger images were saved separately. The matching results are analysed next. Finishing collection is not a decision that authentication succeeded.</source>
        <translation>基准、验证与其他手指的图像已分开保存。接下来将分析匹配结果。采集完成并不代表认证成功。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="233"/>
        <source>④ Saved · %1/14 — lift your finger.</source>
        <translation>④ 已保存 · %1/14 — 请抬起手指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="234"/>
        <source>Below is the image just captured. The next capture is prepared automatically in 4 seconds.</source>
        <translation>下方是刚刚采集的实际图像。4 秒后会自动进入下一次采集准备。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="239"/>
        <source>The capture failed — lift your finger.</source>
        <translation>采集失败 — 请抬起手指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="240"/>
        <source>The same sample will be retried automatically. Do not touch the sensor yet.</source>
        <translation>将自动重试同一样本。暂时不要放上手指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="241"/>
        <source>Three failures in a row, so it has paused for diagnosis. Rest your hand. Collection resumes automatically once the cause is identified.</source>
        <translation>连续 3 次失败，已暂停以便诊断。请让手休息。确认原因后会自动恢复采集。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="249"/>
        <source>Stopped. Lift your finger.</source>
        <translation>已停止。请抬起手指。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="249"/>
        <source>Images already saved have been kept.</source>
        <translation>已保存的图像已予保留。</translation>
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
