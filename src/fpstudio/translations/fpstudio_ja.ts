<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ja" sourcelanguage="en">
<context>
    <name>fpstudio</name>
    <message>
        <location filename="../src/engine.cpp" line="86"/>
        <source>Finger detected — hold still</source>
        <translation>指を検出しました — そのまま保持してください</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="91"/>
        <source>Finger lifted</source>
        <translation>指が離れました</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="104"/>
        <source>Only part of the sensor was touched — cover more of it, centred</source>
        <translation>一部しか触れていません — 中央にもっと広く当ててください</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="109"/>
        <source>The ridges are faint — press a little harder</source>
        <translation>指紋の隆線が薄いです — もう少し強く押してください</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="114"/>
        <source>Frame received</source>
        <translation>フレームを受信しました</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="124"/>
        <location filename="../src/engine.cpp" line="134"/>
        <location filename="../src/headless.cpp" line="482"/>
        <source>Captured — now lift your finger</source>
        <translation>取得完了 — 指を離してください</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="139"/>
        <source>Matching against the enrolled print…</source>
        <translation>登録済みの指紋と照合中…</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="144"/>
        <location filename="../src/setupcheck.cpp" line="210"/>
        <source>Secure channel to the sensor established</source>
        <translation>センサーとの安全な通信が確立しました</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="149"/>
        <source>Sensor ready</source>
        <translation>センサー準備完了</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="418"/>
        <location filename="../src/engine.cpp" line="499"/>
        <location filename="../src/engine.cpp" line="575"/>
        <source>Gave up after %1 seconds</source>
        <translation>%1 秒以内に完了せず中断しました</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="461"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>登録 %1/%2 — 指を離してもう一度置いてください</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <location filename="../src/headless.cpp" line="93"/>
        <location filename="../src/headless.cpp" line="442"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>センサーに指を置いて、そのまま保持してください</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="95"/>
        <location filename="../src/headless.cpp" line="443"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>登録中です — 指を置いて離すを繰り返してください</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="97"/>
        <location filename="../src/headless.cpp" line="444"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>登録した指をセンサーに置いてください</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="65"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>Goodix 27c6:55b4 を検出</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>A different Goodix sensor is present</source>
        <translation>別の Goodix センサーが存在します</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="75"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>このビルドは 27c6:55b4 のみ対応しています。検出: 27c6:%1。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="81"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>Goodix 指紋センサーが見つかりません</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="82"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>USB バス上にベンダー 27c6 に応答するデバイスがありません。リーダーが搭載されている場合、ファームウェア設定で無効になっているか、別のメーカー製の可能性があります。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="101"/>
        <source>Patched driver is active</source>
        <translation>パッチ済みドライバーが有効です</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="108"/>
        <location filename="../src/setupcheck.cpp" line="116"/>
        <source>Patched driver is not installed</source>
        <translation>パッチ済みドライバーがインストールされていません</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="109"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint がこのセンサーを認識しません。パッチ済みフォークをビルド・インストールする必要がありますが、このプログラムにはそのソースが含まれていません。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="117"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint がこのセンサーを認識しません。パッチ済みフォークがサポートを追加します。ビルドには数分かかり、システムの libfprint を置き換えるためパスワードが必要です。

fprintd はこの後にインストールしてください。先にインストールすると純正の libfprint が入り、この変更が元に戻ります。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="122"/>
        <source>Build and install the patched libfprint</source>
        <translation>パッチ済み libfprint をビルドしてインストール</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="152"/>
        <source>Sensor is reachable without a password</source>
        <translation>パスワードなしでセンサーにアクセスできます</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="158"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>udev ルールはインストール済みですが、まだ有効になっていません</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="159"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>センサーの操作のたびにパスワードが必要になります</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="161"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>ルールファイルは配置済みですが、デバイスノードはまだ root 所有です。次の add イベントで適用されます。リーダーを挿し直すか udev を再読み込みしてください。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="164"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>ルールがないと libusb は root としてしかセンサーを開けず、すべてのキャプチャがパスワード入力を要求します。

このルールは wheel グループにセンサーの読み書き権限を与えます。シングルユーザーのノートPCでは pkexec で既にできたのと同じ人物ですが、共有マシンでは違います。スキップしても安全です — パスワード入力が増えるだけです。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="174"/>
        <source>Install the udev rule and reload</source>
        <translation>udev ルールをインストールして再読み込み</translation>
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
        <translation>センサーは応答しましたが、ハンドシェイクが完了しませんでした</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="218"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>センサーはこのドライバーが持っていないキーを保持しています。次のステップは全ゼロキーの書き込みで、これは元に戻せません。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="224"/>
        <source>Could not talk to the sensor</source>
        <translation>センサーと通信できませんでした</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="225"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>ハンドシェイクが原因を特定できるほど進みませんでした。まず上の「ドライバー」「権限」の項目を確認してください。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="238"/>
        <source>The sensor already holds a usable key</source>
        <translation>センサーはすでに使用可能なキーを保持しています</translation>
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
        <translation>センサーに全ゼロキーを書き込む必要があります</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="252"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>この操作は元に戻せません。

センサーが今保持しているキーは読み出せません — プロトコルが返すのはキーから導出された値であり、キーそのものではありません — そのため後で復元するバックアップがありません。

このマシンの Windows 指紋サインインは永久に動作しなくなります。デュアルブートで Windows でも使っている場合は、ここで止めてください。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="260"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>全ゼロ PSK をセンサーに書き込む</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="290"/>
        <source>Image quality has not been measured yet</source>
        <translation>画像品質はまだ測定されていません</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="291"/>
        <source>Press the button below, then put your finger on the sensor and hold it there for a couple of seconds.</source>
        <translation>下のボタンを押してから、指をセンサーに置いて数秒間そのままにしてください。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="294"/>
        <source>Test a capture now</source>
        <translation>今すぐキャプチャをテスト</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="298"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>接触 %1%  ·  鮮明度 %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="307"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>24 未満だと、ドライバーがフレームを受け入れても照合は失敗しがちです。指先の乾燥やセンサーの酷使が原因のことが多く、数分休ませて指先を少し湿らせると大抵直ります。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="311"/>
        <source>Try again</source>
        <translation>再試行</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="324"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>fprintd に指が登録済みです</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="330"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>fprintd に登録された指がありません</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="331"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>このプログラムは独自の登録データを別に保持しており、システム認証はそれを参照しません。fprintd で再登録することで、指紋がロック解除に使えるようになります。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="335"/>
        <source>Enrol a finger with fprintd</source>
        <translation>fprintd に指を登録</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="340"/>
        <source>fprintd is not answering</source>
        <translation>fprintd が応答しません</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="341"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>デーモンに接続できませんでした。通常は必要時に自動起動するため、インストールするだけで解決することが多いです。</translation>
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
        <translation>センサー</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="488"/>
        <source>Driver</source>
        <translation>ドライバー</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="489"/>
        <source>Permissions</source>
        <translation>権限</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="490"/>
        <source>Secure channel</source>
        <translation>セキュアチャネル</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="491"/>
        <source>Sensor key</source>
        <translation>センサーキー</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="492"/>
        <source>Image quality</source>
        <translation>画像品質</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="493"/>
        <source>Enrolment</source>
        <translation>登録</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="494"/>
        <source>Unlocking</source>
        <translation>ロック解除</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="516"/>
        <source>Not checked - no sensor</source>
        <translation>未確認 — センサーなし</translation>
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
        <translation>まだフレームがありません

キャプチャでセンサーから画像を取得します</translation>
    </message>
</context>
<context>
    <name>fpstudio::LiveWindow</name>
    <message>
        <location filename="../src/livewindow.cpp" line="39"/>
        <source>a different finger from the last one</source>
        <translation>直前とは違う指</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="40"/>
        <source>the same finger used so far</source>
        <translation>これまで使ってきたのと同じ指</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="41"/>
        <source>fpstudio · live fingerprint stream</source>
        <translation>fpstudio · 指紋ライブ映像</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="51"/>
        <source>⚠ Different-finger trial — lift your finger off the sensor. Calibration starts automatically.</source>
        <translation>⚠ 別の指のテスト — センサーから指を離してください。まもなく自動で補正します。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="52"/>
        <source>Lift your finger off the sensor. Calibration starts automatically.</source>
        <translation>センサーから指を離してください。まもなく自動で補正します。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="73"/>
        <source>After calibration, rest the same finger lightly on the sensor and follow the prompts. Candidates are compared automatically for each contact.
Live sensor image 108 × 88 · background subtracted, contrast normalised · not an authentication decision · up to 3 minutes</source>
        <translation>補正後、同じ指を軽く置いて案内に従ってください。接触ごとに候補を自動比較します。
実センサー映像 108 × 88 · 背景差分・コントラスト正規化 · 認証判定ではありません · 最長 3 分</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="77"/>
        <source>This is a different-finger comparison trial. Do not use the finger from the previous run this time.
After calibration, tap and lift repeatedly with one other finger. Even a high score does not mean authentication was granted.</source>
        <translation>別の指との比較テストです。今回は直前に使った指は使わないでください。
補正後、別の指 1 本で軽く触れては離すのを繰り返してください。スコアが高くても認証が許可されたことにはなりません。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="79"/>
        <source>Waiting for calibration — no live image yet</source>
        <translation>補正待ち — まだライブ映像はありません</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="96"/>
        <source>0 FPS · waiting for a new frame</source>
        <translation>0 FPS · 新しいフレームを待機中</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="99"/>
        <source>GPU comparison: no reference image set · collecting live only</source>
        <translation>GPU 比較: 基準画像が未指定 · ライブ収集のみ</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="106"/>
        <location filename="../src/livewindow.cpp" line="1031"/>
        <source>Stop</source>
        <translation>中止</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="111"/>
        <source>Install this fingerprint as the enrolment</source>
        <translation>この指紋を登録データとして導入</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="114"/>
        <source>Coverage map — drawn here as contacts accumulate
The dark red areas have not been touched yet
Move your finger toward the empty side
This is enrolment guidance, not an authentication decision</source>
        <translation>カバレッジマップ — 接触が溜まるとここに描画されます
暗い赤の部分はまだ触れていない場所です
空いている側へ指を動かしてください
認証判定ではなく登録のための案内です</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="132"/>
        <source>The enrolment is installed. sudo and the lock screen now use this fingerprint.</source>
        <translation>登録データを導入しました。sudo とロック画面でこの指紋が使われます。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="133"/>
        <source>Installing the enrolment failed. The existing enrolment is unchanged.</source>
        <translation>登録データの導入に失敗しました。既存の登録データはそのままです。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="136"/>
        <source>Installation failed</source>
        <translation>導入に失敗</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="137"/>
        <source>The installer exited with code %1.</source>
        <translation>導入ツールはコード %1 で終了しました。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="144"/>
        <source>Could not start. Check for another sensor session, or the permissions on the save folder.</source>
        <translation>開始できませんでした。別のセンサーセッション、または保存フォルダーの権限を確認してください。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="157"/>
        <source>Could not run the capture program: </source>
        <translation>キャプチャープログラムを実行できませんでした: </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="158"/>
        <location filename="../src/livewindow.cpp" line="167"/>
        <location filename="../src/livewindow.cpp" line="1033"/>
        <source>Close</source>
        <translation>閉じる</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="164"/>
        <source>Stopped. You can lift your finger off the sensor.</source>
        <translation>中止しました。センサーから指を離して構いません。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="165"/>
        <source>The live stream has ended. Lift your finger off the sensor.</source>
        <translation>ライブ映像が終了しました。センサーから指を離してください。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="166"/>
        <source>0 FPS · ended · the last image is a still · %1 frames in total</source>
        <translation>0 FPS · 終了 · 最後の映像は静止画です · 合計 %1 フレーム</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="189"/>
        <source>Vulkan GPU is initializing — it calculates separately from the live stream</source>
        <translation>Vulkan GPU を初期化中 — ライブ映像とは別に計算します</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="198"/>
        <source>%1 · %2 references ready · waiting for a stable candidate</source>
        <translation>%1 · 基準 %2 枚を準備済み · 安定した候補映像を待機中</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="225"/>
        <source>GPU comparison error: </source>
        <translation>GPU 比較エラー: </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="230"/>
        <source>GPU %1 hypotheses · %2 ms · image correlation %3 · ridge-direction agreement %4
Best: %5 / %6 — research comparison only; not an authentication decision</source>
        <translation>GPU 条件 %1 件 · %2 ms · 画像相関 %3 · 隆線方向一致 %4
最上位: %5 / %6 — 研究用の比較であり、認証判定ではありません</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="234"/>
        <source>
Alignment is ambiguous: a different position has a similar score</source>
        <translation>
位置合わせが曖昧です: 別の位置に近いスコアがあります</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="235"/>
        <source>
At the search boundary: move the finger slightly toward the centre</source>
        <translation>
探索範囲の境界です: 指を少し中央寄りに動かしてください</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="238"/>
        <source>
Interior ridge support %1/%2 regions · %3</source>
        <translation>
内部隆線の支持 %1/%2 領域 · %3</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="240"/>
        <source>ridges are consistent (identity unverified)</source>
        <translation>隆線は一貫しています（本人性は未検証）</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="240"/>
        <source>insufficient match evidence</source>
        <translation>一致の根拠が不足しています</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="243"/>
        <source> · 0/1 contact-region overlap %1%</source>
        <translation> · 0/1 接触領域の重なり %1%</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="245"/>
        <source> · 360° additional search applied</source>
        <translation> · 360° 追加探索を適用</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="247"/>
        <source> · full reference-bank search</source>
        <translation> · 基準バンク全体を探索</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="247"/>
        <source> · representative-reference fast comparison</source>
        <translation> · 代表基準による高速比較</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="263"/>
        <source>GPU comparison cannot run: </source>
        <translation>GPU 比較を実行できません: </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="269"/>
        <source>The GPU comparison worker exited unexpectedly (code %1)</source>
        <translation>GPU 比較ワーカーが予期せず終了しました (コード %1)</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="324"/>
        <source>Vulkan GPU is comparing reference images, angles, and positions in parallel…</source>
        <translation>Vulkan GPU が基準映像・角度・位置を並列比較中です…</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="329"/>
        <source>GPU comparison timed out — live collection continues</source>
        <translation>GPU 比較がタイムアウトしました — ライブ収集は継続します</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="783"/>
        <source>added</source>
        <translation>反映済み</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="784"/>
        <source>smeared · not added</source>
        <translation>にじみ · 未反映</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="785"/>
        <source>registration failed · not added</source>
        <translation>位置合わせ失敗 · 未反映</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="786"/>
        <source>too little overlap · not added</source>
        <translation>重なり不足 · 未反映</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="794"/>
        <source>last contact · %1</source>
        <translation>最後の接触 · %1</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="798"/>
        <source>Coverage %1× · %2 placed · %3 not added
Dark red is what has not been touched yet · the green outline is what was just added</source>
        <translation>カバー範囲 %1 倍 · 配置 %2 件 · 未反映 %3 件
暗い赤はまだ触れていない場所 · 緑の枠は今反映された位置</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="852"/>
        <location filename="../src/livewindow.cpp" line="885"/>
        <source>Cannot install</source>
        <translation>導入できません</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="853"/>
        <source>No capture has been placed on the map. Rest your finger on the sensor, move it slowly, and collect again.</source>
        <translation>マップに配置された撮影がありません。センサーに指を置き、ゆっくり動かしてもう一度収集してください。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="858"/>
        <source>Install fingerprint enrolment</source>
        <translation>指紋登録データの導入</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="859"/>
        <source>Install %1 captures from this session as the authentication enrolment.

The existing enrolment is backed up and replaced; fprintd enrolment and PAM settings are left alone.
This matcher has not been validated for rejecting other people. Continue?</source>
        <translation>このセッションの撮影 %1 枚を認証用の登録データとして導入します。

既存の登録データはバックアップのうえ置き換えられ、fprintd の登録と PAM 設定はそのままにします。
この照合器は他人を拒否できるか検証されていません。続行しますか?</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="886"/>
        <source>Could not find the installer (tools/auth_install.py).</source>
        <translation>導入ツール (tools/auth_install.py) が見つかりませんでした。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="895"/>
        <source>Installing the enrolment…</source>
        <translation>登録データを導入しています…</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1017"/>
        <source>Contact %1 · image from comparison %2 (not live)
Gray: background (0) · yellow: contact (1), but weak features · green/red: comparison evidence
Cyan +: transformed reference contact centre · orange +: current probe contact centre · blue: comparison range</source>
        <translation>接触 %1 · 比較 %2 時の映像（ライブではありません）
灰色: 背景 (0) · 黄色: 接触 (1) だが特徴が弱い · 緑/赤: 比較根拠
水色 +: 変換後の基準接触中心 · 橙 +: 現在の検査接触中心 · 青: 比較範囲</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1030"/>
        <source>Capture finished — lift your finger. %1 remaining contacts are still being compared</source>
        <translation>撮影終了 — 指を離してください。残り %1 件の接触を比較中です</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1038"/>
        <source>The target of %1 contacts has been compared — no need to repeat. You can lift your finger.</source>
        <translation>目標の %1 接触の比較が完了しました — 繰り返す必要はありません。指を離して構いません。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1038"/>
        <source>Stopped. You can lift your finger.</source>
        <translation>中止しました。指を離して構いません。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1039"/>
        <source>Capture and candidate comparison have ended. See the per-contact totals below.</source>
        <translation>撮影と候補比較が終了しました。下の接触ごとの集計を確認してください。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1080"/>
        <source>Capture finished · %1 frames · %2 contacts detected
%3 comparisons answered · %4 ridge-consistent · %5 not compared (%6 with no candidate) · %7 queued · %8 errors
Research results on selected data, not an authentication success rate</source>
        <translation>撮影終了 · %1 フレーム · 検出接触 %2 回
比較応答 %3 件 · 隆線一致 %4 件 · 未比較 %5 件 (候補なし %6 件) · 待機 %7 件 · エラー %8 件
選別データに対する研究結果であり、認証成功率ではありません</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1088"/>
        <source>Keep your finger off — connecting to the sensor and calibrating the background</source>
        <translation>まだ指を離したままにしてください — センサー接続と背景補正中</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1137"/>
        <source>Lift your finger — background calibration in %1 s</source>
        <translation>指を離してください — %1 秒後に背景補正</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1146"/>
        <source>Keep your finger off — collecting more background from the live stream</source>
        <translation>指を離したままにしてください — ライブ映像から背景を追加収集しています</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1149"/>
        <source>0 FPS · waiting for the first image · %1 s</source>
        <translation>0 FPS · 最初の映像を待機中 · %1 秒</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1326"/>
        <source>Keep your finger off and hold — collecting the background over several frames (do not touch the sensor yet)</source>
        <translation>指を離したまま待ってください — 複数フレームで背景を収集中です (まだセンサーに触れないでください)</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1327"/>
        <source>No finger detected — now rest %1 lightly in the centre of the sensor</source>
        <translation>指が検出されていません — いま %1 をセンサー中央に軽く置いてください</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1330"/>
        <source>Warning: the other finger also produced ridge-match evidence. A suspected false accept has been recorded — lift and touch again</source>
        <translation>注意: 別の指でも隆線一致の根拠が出ました。誤認識の疑いとして記録しました — 離してもう一度置いてください</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1331"/>
        <source>This other finger produced no match evidence. Recorded — lift completely and touch again</source>
        <translation>この別の指では一致の根拠はありませんでした。記録しました — 完全に離してもう一度置いてください</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1333"/>
        <source>This contact&apos;s ridges were confirmed — lift completely and rest %1 on the sensor again (not an authentication decision)</source>
        <translation>この接触の隆線を確認しました — 完全に離してから %1 をもう一度置いてください (認証判定ではありません)</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1335"/>
        <source>A sharp image was captured — comparing automatically. You can lift your finger</source>
        <translation>鮮明な映像を取得しました — 自動比較中です。指を離して構いません</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1338"/>
        <source>Three comparisons of this contact found too little evidence — lift completely, then place %1 in the centre again</source>
        <translation>この接触は 3 回比較しても根拠が不足しています — 完全に離してから %1 を中央に置き直してください</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1339"/>
        <source>Too little match evidence — move %1 slightly toward the centre and press more lightly. It will be compared again automatically</source>
        <translation>一致の根拠が不足しています — %1 を中央へ少し寄せ、圧力を弱めてください。自動で再比較します</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1340"/>
        <source>The contact area is small — lay the pad of %1 flat across the centre of the sensor</source>
        <translation>接触面積が小さいです — %1 の指紋面をセンサー中央に広く当ててください</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1341"/>
        <source>You are pressing hard — ease off and just rest the finger. Quality falls below 0.79 in this range</source>
        <translation>強く押しています — 力を抜いて軽く置くようにしてください。この範囲では品質が 0.79 を下回ります</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1342"/>
        <source>Hold your finger still for a moment — a stable image will be selected automatically</source>
        <translation>指を少し止めてください — 安定した映像を自動で選びます</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1343"/>
        <source>The ridges are unclear — ease off a little and shift %1 slightly</source>
        <translation>隆線が不明瞭です — 少し力を抜いて %1 の位置をわずかにずらしてください</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1344"/>
        <source>Hold it there for a moment — selecting a sharp image automatically</source>
        <translation>そのまま少し保持してください — 鮮明な映像を自動選択中です</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1345"/>
        <source>⚠ Use a different finger · </source>
        <translation>⚠ 別の指を置いてください · </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1348"/>
        <source>The research storage and comparison limit has been reached — this contact will not be compared further. Please stop</source>
        <translation>研究用の保存・比較の上限に達しました — この接触はこれ以上比較しません。中止してください</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1349"/>
        <source>GPU comparison is unavailable — showing the image only. Please stop</source>
        <translation>GPU 比較を利用できません — 映像のみ表示中です。中止してください</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1353"/>
        <source>⚠ The ridges were smeared, so this was not added to the map — the red outline on the map is where it would have gone. Ease off and press the same spot again</source>
        <translation>⚠ 隆線がにじんだためマップに入れませんでした — マップの赤い枠がその位置です。力を抜いて同じ場所をもう一度押してください</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1356"/>
        <source>⚠ This could not be fitted to the map — press so that it overlaps an already filled area by about half</source>
        <translation>⚠ マップに合わせられませんでした — すでに埋まっている場所と半分ほど重なるように押してください</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1359"/>
        <source>⚠ This does not overlap anything already on the map — continue from the edge of a filled area</source>
        <translation>⚠ マップ上の既存領域と重なる部分がありません — 埋まっている場所の端から続けてください</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1363"/>
        <source>[%1/%2 contacts compared · stops automatically at the target] </source>
        <translation>[%1/%2 接触を比較済み · 目標に達すると自動終了] </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1367"/>
        <source>%1 FPS · %2 frames · ridge signal %3 · contrast area %4% · sharpness %5
</source>
        <translation>%1 FPS · %2 フレーム · 隆線信号 %3 · コントラスト面積 %4 % · 鮮明度 %5
</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1370"/>
        <source>%1 · %2 contacts · %3 candidates · %4 GPU comparisons done / %5 contacts queued (not validated for authentication)</source>
        <translation>%1 · 接触 %2 回 · 候補 %3 枚 · GPU 完了 %4 件 / 待機 %5 接触 (認証用には未検証)</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1372"/>
        <source>
Raw difference %1 · background noise %2 · %3</source>
        <translation>
元データ差分 %1 · 背景ノイズ %2 · %3</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1373"/>
        <source>collecting background</source>
        <translation>背景収集中</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1373"/>
        <source>finger in contact</source>
        <translation>指が接触</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1373"/>
        <source>no finger</source>
        <translation>指なし</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1378"/>
        <source>The image stream has stopped — the screen shows the last frame</source>
        <translation>映像の受信が止まりました — 画面は最後のフレームです</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1379"/>
        <source>0 FPS · no new image for %1 s</source>
        <translation>0 FPS · %1 秒間新しい映像がありません</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1416"/>
        <source>GPU comparison finished · %1 processed · stopped normally on reaching the target of %2 contacts
The results and colour-marked images have been saved. This did not decide whether authentication is granted.</source>
        <translation>GPU 比較完了 · %1 件処理 · 目標 %2 接触に到達して正常終了
結果と色分け表示の映像は保存されています。認証の可否を判定したものではありません。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1417"/>
        <source>GPU comparison stopped · %1 finished · %2 cancelled in progress
Stopped at your request. Saved results are kept.</source>
        <translation>GPU 比較中止 · 完了 %1 件 · 進行中の取り消し %2 件
要求により中止しました。保存済みの結果は保持されます。</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="1418"/>
        <source>Stopping — lift your finger</source>
        <translation>中止中 — 指を離してください</translation>
    </message>
</context>
<context>
    <name>fpstudio::MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="109"/>
        <location filename="../src/mainwindow.cpp" line="363"/>
        <source>Device scan</source>
        <translation>デバイス検索</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="154"/>
        <source>Setup</source>
        <translation>セットアップ</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="155"/>
        <source>Set up fingerprint unlock…</source>
        <translation>指紋によるロック解除を設定…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="158"/>
        <source>Language</source>
        <translation>言語</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="195"/>
        <source>fpstudio — fingerprint sensor diagnostics</source>
        <translation>fpstudio — 指紋センサー診断</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="200"/>
        <source>Looking for a device…</source>
        <translation>デバイスを検索中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="204"/>
        <source>Device</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="206"/>
        <source>Rescan</source>
        <translation>再検索</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="211"/>
        <source>Capture — show image</source>
        <translation>キャプチャ — 画像を表示</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="212"/>
        <location filename="../src/mainwindow.cpp" line="373"/>
        <source>Enrol</source>
        <translation>登録</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="213"/>
        <location filename="../src/mainwindow.cpp" line="379"/>
        <source>Verify</source>
        <translation>照合</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="214"/>
        <location filename="../src/mainwindow.cpp" line="383"/>
        <source>Delete enrolment</source>
        <translation>登録を削除</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="219"/>
        <source>Actions</source>
        <translation>操作</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="226"/>
        <source>Progress</source>
        <translation>進行</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="258"/>
        <source>—</source>
        <translation>—</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="295"/>
        <source>Coverage</source>
        <translation>接触</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="297"/>
        <source>Sharpness</source>
        <translation>鮮明度</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Sample</source>
        <translation>サンプル</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Score</source>
        <translation>スコア</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Keypoints</source>
        <translation>特徴点</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="314"/>
        <source>Match scores</source>
        <translation>照合スコア</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="324"/>
        <source>Driver log</source>
        <translation>ドライバーログ</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="366"/>
        <source>→ Put your finger on the sensor</source>
        <translation>→ センサーに指を置いてください</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="367"/>
        <source>Image capture</source>
        <translation>画像キャプチャ</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="370"/>
        <source>→ Enrol: press and lift your finger repeatedly</source>
        <translation>→ 登録: 指を置いて離すを繰り返してください</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="377"/>
        <source>→ Verify: present the enrolled finger</source>
        <translation>→ 照合: 登録した指を置いてください</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="394"/>
        <source>in progress…</source>
        <translation>実行中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="394"/>
        <source> in progress…</source>
        <translation> 実行中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="422"/>
        <source>No fingerprint device was found.</source>
        <translation>指紋デバイスが見つかりませんでした。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="431"/>
        <source>Driver: %1
ID: %2
Scan: %3 · %4 enrol stages
Raw capture: %5</source>
        <translation>ドライバー: %1
ID: %2
スキャン: %3 · 登録 %4 段階
生キャプチャ: %5</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="436"/>
        <source>yes</source>
        <translation>対応</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="436"/>
        <source>no</source>
        <translation>非対応</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="448"/>
        <source>No image was returned</source>
        <translation>画像が返りませんでした</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="452"/>
        <source>Captured %1 × %2</source>
        <translation>取得 %1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="471"/>
        <source>Match   %1 / %2</source>
        <translation>一致   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="472"/>
        <source>No match   %1 / %2</source>
        <translation>不一致   %1 / %2</translation>
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
        <translation>エラー: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="489"/>
        <source>Done — %1</source>
        <translation>完了 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="494"/>
        <source>Enrolment complete</source>
        <translation>登録完了</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="496"/>
        <source>Enrolment deleted</source>
        <translation>登録を削除しました</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="523"/>
        <source>This scan fell below the threshold and was asked for again</source>
        <translation>このスキャンは基準を下回ったため再要求されました</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="538"/>
        <source>Coverage %1%  ·  sharpness %2  —  faint. Press a little harder</source>
        <translation>接触 %1%  ·  鮮明度 %2  —  薄いです。もう少し強く押してください</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="540"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>接触 %1%  ·  鮮明度 %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="561"/>
        <source>%1 × %2</source>
        <translation>%1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="617"/>
        <source>Agent request</source>
        <translation>エージェントからの要求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="618"/>
        <source>CLI request</source>
        <translation>CLI からの要求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="618"/>
        <source>request</source>
        <translation>要求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="634"/>
        <source>Idle</source>
        <translation>待機中</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="635"/>
        <source>▸ %1   ·   %2s elapsed</source>
        <translation>▸ %1   ·   %2 秒経過</translation>
    </message>
</context>
<context>
    <name>fpstudio::PrivRunner</name>
    <message>
        <location filename="../src/privrunner.cpp" line="32"/>
        <source>Could not run pkexec</source>
        <translation>pkexec を実行できません</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="47"/>
        <source>The operation did not finish in time and was stopped</source>
        <translation>操作が制限時間内に完了しなかったため停止しました</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="52"/>
        <source>The operation terminated abnormally</source>
        <translation>処理が異常終了しました</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="59"/>
        <source>Authentication was cancelled or refused</source>
        <translation>認証がキャンセルまたは拒否されました</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="84"/>
        <source>Could not parse the result (exit code %1)</source>
        <translation>結果を解析できませんでした (終了コード %1)</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="147"/>
        <source>An operation is already running</source>
        <translation>すでに処理が実行中です</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="151"/>
        <source>Could not resolve this program&apos;s own path</source>
        <translation>実行ファイルのパスを特定できません</translation>
    </message>
</context>
<context>
    <name>fpstudio::ResearchWindow</name>
    <message>
        <location filename="../src/researchwindow.cpp" line="43"/>
        <source>fpstudio · fingerprint research session</source>
        <translation>fpstudio · 指紋研究セッション</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="55"/>
        <source>The captured fingerprint image will appear here</source>
        <translation>撮影した指紋映像がここに表示されます</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="58"/>
        <source>No image · nothing captured yet</source>
        <translation>映像なし · まだ撮影していません</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="60"/>
        <source>Stop</source>
        <translation>中止</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="68"/>
        <source>Another research session is already running</source>
        <translation>別の研究セッションが実行中です</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="69"/>
        <location filename="../src/researchwindow.cpp" line="230"/>
        <location filename="../src/researchwindow.cpp" line="248"/>
        <source>Close</source>
        <translation>閉じる</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="74"/>
        <source>Could not create the save folder</source>
        <translation>保存フォルダーを作成できませんでした</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="88"/>
        <source>Could not start the capture program</source>
        <translation>キャプチャープログラムを開始できませんでした</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="94"/>
        <source>Get your right index finger ready. Do not touch the sensor yet.</source>
        <translation>右手の人差し指を準備してください。まだセンサーに触れないでください。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="95"/>
        <source>Plan: 6 reference images → 4 verification images of the same finger → 4 comparison images of the right middle finger.
No buttons to press. Follow the finger and position prompts on screen. You can stop at any time.</source>
        <translation>計画: 基準 6 枚 → 同じ指の検証 4 枚 → 右手中指の比較 4 枚。
ボタンを押す必要はありません。画面の指と位置の案内に従ってください。いつでも中止できます。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="107"/>
        <source>Reference images</source>
        <translation>基準映像</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="107"/>
        <source>Separate verification images</source>
        <translation>別途検証映像</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="107"/>
        <source>Different-finger comparison</source>
        <translation>別の指との比較</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="111"/>
        <source>the broad pad of your right middle finger, centred</source>
        <translation>右手中指の広い面を中央に</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="112"/>
        <source>your right index finger, a touch left of centre</source>
        <translation>右手の人差し指を中央からほんの少し左に</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="113"/>
        <source>your right index finger, a touch right of centre</source>
        <translation>右手の人差し指を中央からほんの少し右に</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="114"/>
        <source>the broad pad of your right index finger, centred</source>
        <translation>右手の人差し指の広い面を中央に</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="125"/>
        <source>%1 · saved %2 / %3</source>
        <translation>%1 · 保存 %2 / %3</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="143"/>
        <source>① Lift your finger clear of the sensor · 5 s</source>
        <translation>① センサーから指を完全に離してください · 5 秒</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="144"/>
        <source>The empty sensor is measured first. Wait until this changes to &apos;Touch now&apos;.</source>
        <translation>先に空のセンサーを測定します。「今置いてください」に変わるまでお待ちください。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="149"/>
        <source>① Keep it off and wait · preparing the sensor</source>
        <translation>① そのまま離して待ってください · センサー準備中</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="150"/>
        <source>This changes to &apos;Touch now&apos; automatically once the background measurement is done.</source>
        <translation>背景測定が終わると自動的に「今置いてください」に変わります。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="164"/>
        <source>① Lift your finger clear · %1 s</source>
        <translation>① 指を完全に離してください · %1 秒</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="168"/>
        <source>%1 · sensor ready in %2 s</source>
        <translation>%1 · %2 秒後にセンサー準備</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="173"/>
        <source>6 reference images done. Starting the separate verification.</source>
        <translation>基準映像 6 枚が完了しました。別途検証を開始します。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="174"/>
        <source>Now switch to your right middle finger.</source>
        <translation>ここで右手の中指に変えてください。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="175"/>
        <source>Four more images with the same right index finger, placed afresh. These are evaluated separately from the reference images.</source>
        <translation>同じ右手の人差し指を置き直して 4 枚撮影します。この映像は基準映像とは分けて評価します。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="176"/>
        <source>Four images to check whether a different finger is wrongly accepted. Do not touch the sensor yet.</source>
        <translation>別の指を誤って受け入れないか確認する 4 枚です。まだセンサーには触れないでください。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="188"/>
        <source>② Touch now — %1</source>
        <translation>② 今置いてください — %1</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="189"/>
        <source>Cover the whole sensor, press lightly and hold. Lift when the prompt says so, after the image appears.</source>
        <translation>センサー全体を覆い、軽く押して保持してください。映像が出たあと、案内に従って離してください。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="191"/>
        <source>③ Hold your finger still · capturing</source>
        <translation>③ 指をそのまま保持してください · 撮影中</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="192"/>
        <source>Contact confirmed. Do not move your finger.</source>
        <translation>接触を確認しました。指を動かさないでください。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="194"/>
        <source>④ Captured. Lift your finger clear.</source>
        <translation>④ 撮影しました。指を完全に離してください。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="195"/>
        <source>The image and its quality will be shown in a moment.</source>
        <translation>まもなく撮影映像と品質を表示します。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="221"/>
        <source>Image just captured · %1×%2 · contact %3% · sharpness %4 · background difference %5</source>
        <translation>いま撮影した映像 · %1×%2 · 接触率 %3 % · 鮮明度 %4 · 背景差分 %5</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="228"/>
        <source>All 14 images collected — you can rest your hand.</source>
        <translation>14 枚の収集が完了しました — 手を休めてください。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="229"/>
        <source>The reference, verification and different-finger images were saved separately. The matching results are analysed next. Finishing collection is not a decision that authentication succeeded.</source>
        <translation>基準・検証・別の指の映像は分けて保存しました。次に照合結果を分析します。収集完了は認証成功の判定ではありません。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="233"/>
        <source>④ Saved · %1/14 — lift your finger.</source>
        <translation>④ 保存完了 · %1/14 — 指を離してください。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="234"/>
        <source>Below is the image just captured. The next capture is prepared automatically in 4 seconds.</source>
        <translation>下はいま撮影した実際の映像です。4 秒後に次の撮影準備へ自動で進みます。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="239"/>
        <source>The capture failed — lift your finger.</source>
        <translation>撮影できませんでした — 指を離してください。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="240"/>
        <source>The same sample will be retried automatically. Do not touch the sensor yet.</source>
        <translation>同じサンプルを自動で再試行します。まだ指を置かないでください。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="241"/>
        <source>Three failures in a row, so it has paused for diagnosis. Rest your hand. Collection resumes automatically once the cause is identified.</source>
        <translation>3 回連続で失敗したため診断のために停止しました。手を休めてください。原因を確認したあと自動収集を再開します。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="249"/>
        <source>Stopped. Lift your finger.</source>
        <translation>中止しました。指を離してください。</translation>
    </message>
    <message>
        <location filename="../src/researchwindow.cpp" line="249"/>
        <source>Images already saved have been kept.</source>
        <translation>すでに保存した映像は保持しました。</translation>
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
        <translation>スキップ</translation>
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
        <translation>完了</translation>
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
        <translation>診断ツールを開く</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="240"/>
        <source>Live capture preview, the driver log, and MCP status - for a closer look once the checklist here is done.</source>
        <translation>ライブキャプチャのプレビュー、ドライバーログ、MCP ステータスを表示します — このチェックリストが終わった後、より詳しく調べるためのものです。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="274"/>
        <source>Checking the device and installation. Keep your finger off the sensor for now.</source>
        <translation>Checking the device and installation. Keep your finger off the sensor for now.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="350"/>
        <source>This step cannot be undone.</source>
        <translation>この手順は元に戻せません。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="366"/>
        <source>This will ask for your password.</source>
        <translation>パスワードの入力が求められます。</translation>
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
        <translation>センサーキー</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="382"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

確認するには %2 と入力してください。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="406"/>
        <source>The enrolment helper was not found</source>
        <translation>登録用ヘルパーが見つかりません</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="412"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>登録中です — 指を置いて離すを繰り返してください</translation>
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
        <translation>センサーに指を置いて、そのまま保持してください</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="465"/>
        <source>Could not start the image tool: %1</source>
        <translation>Could not start the image tool: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="529"/>
        <source>in progress…</source>
        <translation>実行中…</translation>
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
        <translation>このステップが制限時間内に完了しなかったため停止しました。センサーと登録済みの指紋はそのままです。もう一度お試しください。</translation>
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
