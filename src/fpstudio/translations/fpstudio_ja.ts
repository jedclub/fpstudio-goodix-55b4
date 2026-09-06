<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ja" sourcelanguage="en">
<context>
    <name>fpstudio</name>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <source>Finger detected — hold still</source>
        <translation>指を検出しました — そのまま保持してください</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="84"/>
        <source>Finger lifted</source>
        <translation>指が離れました</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="97"/>
        <source>Only part of the sensor was touched — cover more of it, centred</source>
        <translation>一部しか触れていません — 中央にもっと広く当ててください</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="102"/>
        <source>The ridges are faint — press a little harder</source>
        <translation>指紋の隆線が薄いです — もう少し強く押してください</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="107"/>
        <source>Frame received</source>
        <translation>フレームを受信しました</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="117"/>
        <location filename="../src/engine.cpp" line="127"/>
        <source>Captured — now lift your finger</source>
        <translation>取得完了 — 指を離してください</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="132"/>
        <source>Matching against the enrolled print…</source>
        <translation>登録済みの指紋と照合中…</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="137"/>
        <location filename="../src/setupcheck.cpp" line="195"/>
        <source>Secure channel to the sensor established</source>
        <translation>センサーとの安全な通信が確立しました</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="142"/>
        <source>Sensor ready</source>
        <translation>センサー準備完了</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="403"/>
        <location filename="../src/engine.cpp" line="484"/>
        <location filename="../src/engine.cpp" line="560"/>
        <source>Gave up after %1 seconds</source>
        <translation>%1 秒以内に完了せず中断しました</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="446"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>登録 %1/%2 — 指を離してもう一度置いてください</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="81"/>
        <location filename="../src/headless.cpp" line="379"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>センサーに指を置いて、そのまま保持してください</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="83"/>
        <location filename="../src/headless.cpp" line="380"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>登録中です — 指を置いて離すを繰り返してください</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="85"/>
        <location filename="../src/headless.cpp" line="381"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>登録した指をセンサーに置いてください</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="58"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>Goodix 27c6:55b4 を検出</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="67"/>
        <source>A different Goodix sensor is present</source>
        <translation>別の Goodix センサーが存在します</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="68"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>このビルドは 27c6:55b4 のみ対応しています。検出: 27c6:%1。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>Goodix 指紋センサーが見つかりません</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="75"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>USB バス上にベンダー 27c6 に応答するデバイスがありません。リーダーが搭載されている場合、ファームウェア設定で無効になっているか、別のメーカー製の可能性があります。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="94"/>
        <source>Patched driver is active</source>
        <translation>パッチ済みドライバーが有効です</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="101"/>
        <location filename="../src/setupcheck.cpp" line="109"/>
        <source>Patched driver is not installed</source>
        <translation>パッチ済みドライバーがインストールされていません</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="102"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint がこのセンサーを認識しません。パッチ済みフォークをビルド・インストールする必要がありますが、このプログラムにはそのソースが含まれていません。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="110"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint がこのセンサーを認識しません。パッチ済みフォークがサポートを追加します。ビルドには数分かかり、システムの libfprint を置き換えるためパスワードが必要です。

fprintd はこの後にインストールしてください。先にインストールすると純正の libfprint が入り、この変更が元に戻ります。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="115"/>
        <source>Build and install the patched libfprint</source>
        <translation>パッチ済み libfprint をビルドしてインストール</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="148"/>
        <source>Sensor is reachable without a password</source>
        <translation>パスワードなしでセンサーにアクセスできます</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="154"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>udev ルールはインストール済みですが、まだ有効になっていません</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="155"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>センサーの操作のたびにパスワードが必要になります</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="157"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>ルールファイルは配置済みですが、デバイスノードはまだ root 所有です。次の add イベントで適用されます。リーダーを挿し直すか udev を再読み込みしてください。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="160"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>ルールがないと libusb は root としてしかセンサーを開けず、すべてのキャプチャがパスワード入力を要求します。

このルールは wheel グループにセンサーの読み書き権限を与えます。シングルユーザーのノートPCでは pkexec で既にできたのと同じ人物ですが、共有マシンでは違います。スキップしても安全です — パスワード入力が増えるだけです。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="166"/>
        <source>Install the udev rule and reload</source>
        <translation>udev ルールをインストールして再読み込み</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="203"/>
        <source>The sensor answered, but the handshake did not complete</source>
        <translation>センサーは応答しましたが、ハンドシェイクが完了しませんでした</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="204"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>センサーはこのドライバーが持っていないキーを保持しています。次のステップは全ゼロキーの書き込みで、これは元に戻せません。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="210"/>
        <source>Could not talk to the sensor</source>
        <translation>センサーと通信できませんでした</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="211"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>ハンドシェイクが原因を特定できるほど進みませんでした。まず上の「ドライバー」「権限」の項目を確認してください。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="224"/>
        <source>The sensor already holds a usable key</source>
        <translation>センサーはすでに使用可能なキーを保持しています</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="230"/>
        <source>The sensor needs the all-zero key written</source>
        <translation>センサーに全ゼロキーを書き込む必要があります</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="231"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>この操作は元に戻せません。

センサーが今保持しているキーは読み出せません — プロトコルが返すのはキーから導出された値であり、キーそのものではありません — そのため後で復元するバックアップがありません。

このマシンの Windows 指紋サインインは永久に動作しなくなります。デュアルブートで Windows でも使っている場合は、ここで止めてください。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="238"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>全ゼロ PSK をセンサーに書き込む</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="265"/>
        <source>Image quality has not been measured yet</source>
        <translation>画像品質はまだ測定されていません</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="266"/>
        <source>Present a finger on the next page and the reading will appear here.</source>
        <translation>次のページで指を置くと、ここに測定値が表示されます。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="271"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>接触 %1%  ·  鮮明度 %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="280"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>24 未満だと、ドライバーがフレームを受け入れても照合は失敗しがちです。指先の乾燥やセンサーの酷使が原因のことが多く、数分休ませて指先を少し湿らせると大抵直ります。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="296"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>fprintd に指が登録済みです</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="302"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>fprintd に登録された指がありません</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="303"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>このプログラムは独自の登録データを別に保持しており、システム認証はそれを参照しません。fprintd で再登録することで、指紋がロック解除に使えるようになります。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="307"/>
        <source>Enrol a finger with fprintd</source>
        <translation>fprintd に指を登録</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="312"/>
        <source>fprintd is not answering</source>
        <translation>fprintd が応答しません</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="313"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>デーモンに接続できませんでした。通常は必要時に自動起動するため、インストールするだけで解決することが多いです。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="329"/>
        <source>polkit accepts a fingerprint</source>
        <translation>polkit は指紋を受け付けます</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="337"/>
        <source>pam_fprintd is not installed</source>
        <translation>pam_fprintd がインストールされていません</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="338"/>
        <source>The PAM module that lets authentication use a fingerprint is missing. It usually ships with fprintd.</source>
        <translation>認証で指紋を使えるようにする PAM モジュールがありません。通常 fprintd に同梱されています。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="345"/>
        <source>polkit still asks for a password</source>
        <translation>polkit はまだパスワードを要求します</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="346"/>
        <source>This adds one line to the stack polkit uses, so pkexec and the desktop&apos;s authentication dialog try the fingerprint first.

Login and sudo are deliberately left alone, so a sensor that stops working can never lock you out of the machine. The line is &apos;sufficient&apos;: if the fingerprint fails for any reason, you are asked for the password exactly as before.</source>
        <translation>polkit が使う認証スタックに1行追加します。これにより pkexec とデスクトップの認証ダイアログが最初に指紋を試すようになります。

ログインと sudo はあえて変更していません。センサーが動作しなくなっても、これによりマシンから締め出されることはありません。この行は「sufficient」なので、指紋がどんな理由で失敗しても、以前とまったく同じようにパスワードが求められます。</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="353"/>
        <source>Let polkit accept a fingerprint</source>
        <translation>polkit が指紋を受け付けるようにする</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="378"/>
        <source>Sensor</source>
        <translation>センサー</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="379"/>
        <source>Driver</source>
        <translation>ドライバー</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="380"/>
        <source>Permissions</source>
        <translation>権限</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="381"/>
        <source>Secure channel</source>
        <translation>セキュアチャネル</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="382"/>
        <source>Sensor key</source>
        <translation>センサーキー</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="383"/>
        <source>Image quality</source>
        <translation>画像品質</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="384"/>
        <source>Enrolment</source>
        <translation>登録</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="385"/>
        <source>Unlocking</source>
        <translation>ロック解除</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="404"/>
        <source>Not checked - no sensor</source>
        <translation>未確認 — センサーなし</translation>
    </message>
</context>
<context>
    <name>fpstudio::ImageView</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="57"/>
        <source>No frame yet

Use Capture to pull an image from the sensor</source>
        <translation>まだフレームがありません

キャプチャでセンサーから画像を取得します</translation>
    </message>
</context>
<context>
    <name>fpstudio::MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="108"/>
        <location filename="../src/mainwindow.cpp" line="355"/>
        <source>Device scan</source>
        <translation>デバイス検索</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="153"/>
        <source>Setup</source>
        <translation>セットアップ</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="154"/>
        <source>Set up fingerprint unlock…</source>
        <translation>指紋によるロック解除を設定…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="157"/>
        <source>Language</source>
        <translation>言語</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="194"/>
        <source>fpstudio — fingerprint sensor diagnostics</source>
        <translation>fpstudio — 指紋センサー診断</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="199"/>
        <source>Looking for a device…</source>
        <translation>デバイスを検索中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="203"/>
        <source>Device</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="205"/>
        <source>Rescan</source>
        <translation>再検索</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="210"/>
        <source>Capture — show image</source>
        <translation>キャプチャ — 画像を表示</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="211"/>
        <location filename="../src/mainwindow.cpp" line="364"/>
        <source>Enrol</source>
        <translation>登録</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="212"/>
        <location filename="../src/mainwindow.cpp" line="369"/>
        <source>Verify</source>
        <translation>照合</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="213"/>
        <location filename="../src/mainwindow.cpp" line="373"/>
        <source>Delete enrolment</source>
        <translation>登録を削除</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="218"/>
        <source>Actions</source>
        <translation>操作</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="225"/>
        <source>Progress</source>
        <translation>進行</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="257"/>
        <source>—</source>
        <translation>—</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="294"/>
        <source>Coverage</source>
        <translation>接触</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="296"/>
        <source>Sharpness</source>
        <translation>鮮明度</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Sample</source>
        <translation>サンプル</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Score</source>
        <translation>スコア</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Keypoints</source>
        <translation>特徴点</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="313"/>
        <source>Match scores</source>
        <translation>照合スコア</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="323"/>
        <source>Driver log</source>
        <translation>ドライバーログ</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="358"/>
        <source>→ Put your finger on the sensor</source>
        <translation>→ センサーに指を置いてください</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="359"/>
        <source>Image capture</source>
        <translation>画像キャプチャ</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="362"/>
        <source>→ Enrol: press and lift your finger repeatedly</source>
        <translation>→ 登録: 指を置いて離すを繰り返してください</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="367"/>
        <source>→ Verify: present the enrolled finger</source>
        <translation>→ 照合: 登録した指を置いてください</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source>in progress…</source>
        <translation>実行中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source> in progress…</source>
        <translation> 実行中…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="412"/>
        <source>No fingerprint device was found.</source>
        <translation>指紋デバイスが見つかりませんでした。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="421"/>
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
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>yes</source>
        <translation>対応</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>no</source>
        <translation>非対応</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="438"/>
        <source>No image was returned</source>
        <translation>画像が返りませんでした</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="442"/>
        <source>Captured %1 × %2</source>
        <translation>取得 %1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="461"/>
        <source>Match   %1 / %2</source>
        <translation>一致   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="462"/>
        <source>No match   %1 / %2</source>
        <translation>不一致   %1 / %2</translation>
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
        <translation>エラー: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="479"/>
        <source>Done — %1</source>
        <translation>完了 — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484"/>
        <source>Enrolment complete</source>
        <translation>登録完了</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="486"/>
        <source>Enrolment deleted</source>
        <translation>登録を削除しました</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513"/>
        <source>This scan fell below the threshold and was asked for again</source>
        <translation>このスキャンは基準を下回ったため再要求されました</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="528"/>
        <source>Coverage %1%  ·  sharpness %2  —  faint. Press a little harder</source>
        <translation>接触 %1%  ·  鮮明度 %2  —  薄いです。もう少し強く押してください</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="530"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>接触 %1%  ·  鮮明度 %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="598"/>
        <source>Agent request</source>
        <translation>エージェントからの要求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>CLI request</source>
        <translation>CLI からの要求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>request</source>
        <translation>要求</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="615"/>
        <source>Idle</source>
        <translation>待機中</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="616"/>
        <source>▸ %1   ·   %2s elapsed</source>
        <translation>▸ %1   ·   %2 秒経過</translation>
    </message>
</context>
<context>
    <name>fpstudio::PrivRunner</name>
    <message>
        <location filename="../src/privrunner.cpp" line="22"/>
        <source>Could not run pkexec</source>
        <translation>pkexec を実行できません</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="31"/>
        <source>The operation terminated abnormally</source>
        <translation>処理が異常終了しました</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="38"/>
        <source>Authentication was cancelled or refused</source>
        <translation>認証がキャンセルまたは拒否されました</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="63"/>
        <source>Could not parse the result (exit code %1)</source>
        <translation>結果を解析できませんでした (終了コード %1)</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="108"/>
        <source>An operation is already running</source>
        <translation>すでに処理が実行中です</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="112"/>
        <source>Could not resolve this program&apos;s own path</source>
        <translation>実行ファイルのパスを特定できません</translation>
    </message>
</context>
<context>
    <name>fpstudio::SetupWizard</name>
    <message>
        <location filename="../src/setupwizard.cpp" line="65"/>
        <source>The command did not finish in time</source>
        <translation>コマンドが時間内に終わりませんでした</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="73"/>
        <source>Authentication was cancelled or refused</source>
        <translation>認証がキャンセルまたは拒否されました</translation>
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
        <translation>指紋によるロック解除を設定</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="130"/>
        <source>Skip</source>
        <translation>スキップ</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="131"/>
        <source>Re-check</source>
        <translation>再確認</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="166"/>
        <source>Checking…</source>
        <translation>確認中…</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="226"/>
        <source>This step cannot be undone.</source>
        <translation>この手順は元に戻せません。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="239"/>
        <source>This will ask for your password.</source>
        <translation>パスワードの入力が求められます。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="251"/>
        <source>WRITE</source>
        <translation>WRITE</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="253"/>
        <source>Sensor key</source>
        <translation>センサーキー</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="254"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

確認するには %2 と入力してください。</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="274"/>
        <source>The enrolment helper was not found</source>
        <translation>登録用ヘルパーが見つかりません</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="280"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>登録中です — 指を置いて離すを繰り返してください</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="316"/>
        <source>in progress…</source>
        <translation>実行中…</translation>
    </message>
</context>
</TS>
