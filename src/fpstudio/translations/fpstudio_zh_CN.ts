<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN" sourcelanguage="en">
<context>
    <name>fpstudio</name>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <source>Finger detected — hold still</source>
        <translation>检测到手指 — 请保持不动</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="84"/>
        <source>Finger lifted</source>
        <translation>手指已抬起</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="97"/>
        <source>Only part of the sensor was touched — cover more of it, centred</source>
        <translation>只接触到一部分 — 请居中并覆盖更多区域</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="102"/>
        <source>The ridges are faint — press a little harder</source>
        <translation>纹路偏淡 — 请稍微用力按压</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="107"/>
        <source>Frame received</source>
        <translation>已接收帧</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="117"/>
        <location filename="../src/engine.cpp" line="127"/>
        <source>Captured — now lift your finger</source>
        <translation>采集完成 — 请抬起手指</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="132"/>
        <source>Matching against the enrolled print…</source>
        <translation>正在与已注册指纹比对…</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="137"/>
        <location filename="../src/setupcheck.cpp" line="200"/>
        <source>Secure channel to the sensor established</source>
        <translation>已与传感器建立安全通道</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="142"/>
        <source>Sensor ready</source>
        <translation>传感器就绪</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="403"/>
        <location filename="../src/engine.cpp" line="484"/>
        <location filename="../src/engine.cpp" line="560"/>
        <source>Gave up after %1 seconds</source>
        <translation>%1 秒内未完成，已中止</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="446"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>注册 %1/%2 — 抬起手指后再次按下</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="81"/>
        <location filename="../src/headless.cpp" line="373"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>请将手指放在传感器上并保持不动</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="83"/>
        <location filename="../src/headless.cpp" line="374"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>正在注册 — 请反复按下并抬起手指</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="85"/>
        <location filename="../src/headless.cpp" line="375"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>请将已注册的手指放在传感器上</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="58"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>已发现 Goodix 27c6:55b4</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="67"/>
        <source>A different Goodix sensor is present</source>
        <translation>存在其他 Goodix 传感器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="68"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>此构建仅支持 27c6:55b4。检测到 27c6:%1。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>未找到 Goodix 指纹传感器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="75"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>USB 总线上没有设备响应厂商 ID 27c6。如果本机确实配备读取器，可能已在固件设置中被禁用，或为其他厂商产品。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="94"/>
        <source>Patched driver is active</source>
        <translation>已启用修补版驱动</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="101"/>
        <location filename="../src/setupcheck.cpp" line="109"/>
        <source>Patched driver is not installed</source>
        <translation>未安装修补版驱动</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="102"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint 无法识别此传感器。需要构建并安装修补版分支，但本程序副本未附带相应源码。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="110"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint 无法识别此传感器。修补版分支会添加支持。构建需要几分钟，并会替换系统的 libfprint，因此需要密码。

请之后再安装 fprintd：如果先安装，会拉入官方 libfprint 并撤销此更改。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="115"/>
        <source>Build and install the patched libfprint</source>
        <translation>构建并安装修补版 libfprint</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="148"/>
        <source>Sensor is reachable without a password</source>
        <translation>无需密码即可访问传感器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="154"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>udev 规则已安装但尚未生效</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="155"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>每次传感器操作都会要求输入密码</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="157"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>规则文件已就位，但设备节点仍属于 root。它会在下一次 add 事件时生效，重新插拔读取器或重新加载 udev 即可。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="160"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>没有该规则，libusb 只能以 root 身份打开传感器，因此每次采集都会弹出密码提示。

此规则授予 wheel 组对传感器的读写权限。在单用户笔记本上，这与本来就能通过 pkexec 操作的人相同；在共享机器上则不同。跳过此项是安全的——只是会多几次密码提示。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="170"/>
        <source>Install the udev rule and reload</source>
        <translation>安装 udev 规则并重新加载</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="208"/>
        <source>The sensor answered, but the handshake did not complete</source>
        <translation>传感器已应答，但握手未完成</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="209"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>传感器持有的密钥不是本驱动所拥有的。下一步是写入全零密钥，且此操作不可撤销。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="215"/>
        <source>Could not talk to the sensor</source>
        <translation>无法与传感器通信</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="216"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>握手过程未进展到足以说明原因。请先检查上方的驱动和权限步骤。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="229"/>
        <source>The sensor already holds a usable key</source>
        <translation>传感器已持有可用密钥</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="235"/>
        <source>The sensor needs the all-zero key written</source>
        <translation>需要向传感器写入全零密钥</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="236"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>此操作不可撤销。

传感器目前持有的密钥无法读回——协议返回的是由密钥派生出的值，而非密钥本身——因此之后没有备份可以恢复。

本机上的 Windows 指纹登录将永久失效。如果你双系统在 Windows 上也使用它，请到此为止。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="244"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>向传感器写入全零 PSK</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="274"/>
        <source>Image quality has not been measured yet</source>
        <translation>尚未测量图像质量</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="275"/>
        <source>Press the button below, then put your finger on the sensor and hold it there for a couple of seconds.</source>
        <translation>按下方按钮，然后将手指放在传感器上并保持几秒钟。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="278"/>
        <source>Test a capture now</source>
        <translation>立即测试采集</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="282"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>接触面 %1%  ·  清晰度 %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="291"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>低于 24 时，即使驱动接受该帧，匹配通常也会失败。手指干燥或传感器持续繁忙都会导致这种情况；休息几分钟并稍微湿润手指通常可解决。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="295"/>
        <source>Try again</source>
        <translation>重试</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="308"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>fprintd 中已注册手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="314"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>fprintd 中未注册任何手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="315"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>本程序单独保存自己的注册数据，系统认证看不到这些数据。通过 fprintd 重新注册后，指纹才能用于解锁。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="319"/>
        <source>Enrol a finger with fprintd</source>
        <translation>通过 fprintd 注册手指</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="324"/>
        <source>fprintd is not answering</source>
        <translation>fprintd 无响应</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="325"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>无法连接到守护进程。它通常按需自动启动，安装它可能就是唯一缺少的步骤。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="341"/>
        <source>polkit accepts a fingerprint</source>
        <translation>polkit 接受指纹</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="349"/>
        <location filename="../src/setupcheck.cpp" line="390"/>
        <source>pam_fprintd is not installed</source>
        <translation>未安装 pam_fprintd</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="350"/>
        <location filename="../src/setupcheck.cpp" line="391"/>
        <source>The PAM module that lets authentication use a fingerprint is missing. It usually ships with fprintd.</source>
        <translation>缺少让身份验证使用指纹的 PAM 模块。它通常随 fprintd 一起提供。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="357"/>
        <source>polkit still asks for a password</source>
        <translation>polkit 仍要求输入密码</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="358"/>
        <source>This adds one line to the stack polkit uses, so pkexec and the desktop&apos;s authentication dialog try the fingerprint first.

Login and sudo are deliberately left alone, so a sensor that stops working can never lock you out of the machine. The line is &apos;sufficient&apos;: if the fingerprint fails for any reason, you are asked for the password exactly as before.</source>
        <translation>这会在 polkit 使用的认证栈中添加一行，让 pkexec 和桌面认证对话框优先尝试指纹。

登录和 sudo 特意保持不变，这样即使传感器停止工作也不会把你锁在机器之外。这一行是 &apos;sufficient&apos;：如果指纹因任何原因失败，仍会像以前一样要求输入密码。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="366"/>
        <source>Let polkit accept a fingerprint</source>
        <translation>让 polkit 接受指纹</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="382"/>
        <source>sudo accepts a fingerprint</source>
        <translation>sudo 接受指纹</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="402"/>
        <source>Terminal sudo still asks for a password only</source>
        <translation>终端 sudo 目前仍只接受密码</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="403"/>
        <source>Optional, and a step further than the polkit rule above: sudo is usually the way back in when something else on the machine is broken, so extending fingerprint auth to it is worth doing deliberately rather than by default.

The safety net is the same either way. The line added is &apos;sufficient&apos;, so a failing fingerprint falls back to the password exactly as before. sudo itself never stops working - only the fingerprint shortcut can.</source>
        <translation>这是可选项，比上面的 polkit 规则更进一步：sudo 通常是机器上其他东西出问题时的退路，因此为其扩展指纹认证值得慎重决定，而非默认开启。

安全网是一样的。新增的一行是 &apos;sufficient&apos;，因此指纹失败时会像以前一样回退到密码。sudo 本身永远不会失效——只有指纹这条捷径可能失效。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="413"/>
        <source>Let sudo accept a fingerprint</source>
        <translation>让 sudo 接受指纹</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="440"/>
        <source>Sensor</source>
        <translation>传感器</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="441"/>
        <source>Driver</source>
        <translation>驱动</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="442"/>
        <source>Permissions</source>
        <translation>权限</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="443"/>
        <source>Secure channel</source>
        <translation>安全通道</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="444"/>
        <source>Sensor key</source>
        <translation>传感器密钥</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="445"/>
        <source>Image quality</source>
        <translation>图像质量</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="446"/>
        <source>Enrolment</source>
        <translation>注册</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="447"/>
        <source>Unlocking</source>
        <translation>解锁</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="448"/>
        <source>Terminal sudo (optional)</source>
        <translation>终端 sudo（可选）</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="467"/>
        <source>Not checked - no sensor</source>
        <translation>未检查 — 无传感器</translation>
    </message>
</context>
<context>
    <name>fpstudio::ImageView</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="57"/>
        <source>No frame yet

Use Capture to pull an image from the sensor</source>
        <translation>尚无图像

使用「采集」从传感器获取图像</translation>
    </message>
</context>
<context>
    <name>fpstudio::MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="108"/>
        <location filename="../src/mainwindow.cpp" line="355"/>
        <source>Device scan</source>
        <translation>设备扫描</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="153"/>
        <source>Setup</source>
        <translation>设置</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="154"/>
        <source>Set up fingerprint unlock…</source>
        <translation>设置指纹解锁…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="157"/>
        <source>Language</source>
        <translation>语言</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="194"/>
        <source>fpstudio — fingerprint sensor diagnostics</source>
        <translation>fpstudio — 指纹传感器诊断</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="199"/>
        <source>Looking for a device…</source>
        <translation>正在查找设备…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="203"/>
        <source>Device</source>
        <translation>设备</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="205"/>
        <source>Rescan</source>
        <translation>重新扫描</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="210"/>
        <source>Capture — show image</source>
        <translation>采集 — 显示图像</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="211"/>
        <location filename="../src/mainwindow.cpp" line="364"/>
        <source>Enrol</source>
        <translation>注册</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="212"/>
        <location filename="../src/mainwindow.cpp" line="369"/>
        <source>Verify</source>
        <translation>验证</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="213"/>
        <location filename="../src/mainwindow.cpp" line="373"/>
        <source>Delete enrolment</source>
        <translation>删除注册</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="218"/>
        <source>Actions</source>
        <translation>操作</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="225"/>
        <source>Progress</source>
        <translation>进度</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="257"/>
        <source>—</source>
        <translation>—</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="294"/>
        <source>Coverage</source>
        <translation>接触面</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="296"/>
        <source>Sharpness</source>
        <translation>清晰度</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Sample</source>
        <translation>样本</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Score</source>
        <translation>分数</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Keypoints</source>
        <translation>特征点</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="313"/>
        <source>Match scores</source>
        <translation>匹配分数</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="323"/>
        <source>Driver log</source>
        <translation>驱动日志</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="358"/>
        <source>→ Put your finger on the sensor</source>
        <translation>→ 请将手指放在传感器上</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="359"/>
        <source>Image capture</source>
        <translation>图像采集</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="362"/>
        <source>→ Enrol: press and lift your finger repeatedly</source>
        <translation>→ 注册：反复按下并抬起手指</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="367"/>
        <source>→ Verify: present the enrolled finger</source>
        <translation>→ 验证：放上已注册的手指</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source>in progress…</source>
        <translation>进行中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source> in progress…</source>
        <translation> 进行中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="412"/>
        <source>No fingerprint device was found.</source>
        <translation>未找到指纹设备。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="421"/>
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
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>yes</source>
        <translation>支持</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>no</source>
        <translation>不支持</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="438"/>
        <source>No image was returned</source>
        <translation>未返回图像</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="442"/>
        <source>Captured %1 × %2</source>
        <translation>已采集 %1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="461"/>
        <source>Match   %1 / %2</source>
        <translation>匹配   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="462"/>
        <source>No match   %1 / %2</source>
        <translation>不匹配   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="473"/>
        <source>Failed — %1</source>
        <translation>失败 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="474"/>
        <location filename="../src/mainwindow.cpp" line="623"/>
        <location filename="../src/mainwindow.cpp" line="624"/>
        <source>Failed</source>
        <translation>失败</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="475"/>
        <location filename="../src/mainwindow.cpp" line="625"/>
        <source>Error: %1</source>
        <translation>错误: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="479"/>
        <source>Done — %1</source>
        <translation>完成 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484"/>
        <source>Enrolment complete</source>
        <translation>注册完成</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="486"/>
        <source>Enrolment deleted</source>
        <translation>注册已删除</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513"/>
        <source>This scan fell below the threshold and was asked for again</source>
        <translation>本次扫描低于阈值，已重新请求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="528"/>
        <source>Coverage %1%  ·  sharpness %2  —  faint. Press a little harder</source>
        <translation>接触面 %1%  ·  清晰度 %2  —  偏淡，请稍微用力</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="530"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>接触面 %1%  ·  清晰度 %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="598"/>
        <source>Agent request</source>
        <translation>代理请求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>CLI request</source>
        <translation>CLI 请求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>request</source>
        <translation>请求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="615"/>
        <source>Idle</source>
        <translation>空闲</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="616"/>
        <source>▸ %1   ·   %2s elapsed</source>
        <translation>▸ %1   ·   已过 %2 秒</translation>
    </message>
</context>
<context>
    <name>fpstudio::PrivRunner</name>
    <message>
        <location filename="../src/privrunner.cpp" line="22"/>
        <source>Could not run pkexec</source>
        <translation>无法运行 pkexec</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="31"/>
        <source>The operation terminated abnormally</source>
        <translation>操作异常终止</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="38"/>
        <source>Authentication was cancelled or refused</source>
        <translation>认证被取消或拒绝</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="63"/>
        <source>Could not parse the result (exit code %1)</source>
        <translation>无法解析结果（退出码 %1）</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="108"/>
        <source>An operation is already running</source>
        <translation>已有操作在运行</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="112"/>
        <source>Could not resolve this program&apos;s own path</source>
        <translation>无法确定本程序自身的路径</translation>
    </message>
</context>
<context>
    <name>fpstudio::SetupWizard</name>
    <message>
        <location filename="../src/setupwizard.cpp" line="67"/>
        <source>The command did not finish in time</source>
        <translation>命令未在规定时间内完成</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="75"/>
        <source>Authentication was cancelled or refused</source>
        <translation>认证被取消或拒绝</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="76"/>
        <location filename="../src/setupwizard.cpp" line="347"/>
        <location filename="../src/setupwizard.cpp" line="443"/>
        <source>Failed — %1</source>
        <translation>失败 — %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="114"/>
        <source>Set up fingerprint unlock</source>
        <translation>设置指纹解锁</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="145"/>
        <source>Skip</source>
        <translation>跳过</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="146"/>
        <source>Re-check</source>
        <translation>重新检查</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="157"/>
        <source>Fingerprint unlock is ready.</source>
        <translation>指纹解锁已就绪。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="159"/>
        <source>Finish</source>
        <translation>完成</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="194"/>
        <source>Open diagnostics tool</source>
        <translation>打开诊断工具</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="197"/>
        <source>Live capture preview, the driver log, and MCP status - for a closer look once the checklist here is done.</source>
        <translation>提供实时采集预览、驱动日志和 MCP 状态——用于完成此清单后进一步深入查看。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="229"/>
        <source>Checking…</source>
        <translation>正在检查…</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="294"/>
        <source>This step cannot be undone.</source>
        <translation>此步骤无法撤销。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="310"/>
        <source>This will ask for your password.</source>
        <translation>这将要求输入你的密码。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="322"/>
        <source>WRITE</source>
        <translation>WRITE</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="324"/>
        <source>Sensor key</source>
        <translation>传感器密钥</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="325"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

输入 %2 以确认。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="348"/>
        <source>The enrolment helper was not found</source>
        <translation>找不到注册辅助程序</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="354"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>正在注册 — 请反复按下并抬起手指</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="378"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>请将手指放在传感器上并保持不动</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="427"/>
        <source>in progress…</source>
        <translation>进行中…</translation>
    </message>
</context>
</TS>
