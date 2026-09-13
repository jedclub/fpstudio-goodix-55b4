<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru" sourcelanguage="en">
<context>
    <name>fpstudio</name>
    <message>
        <location filename="../src/engine.cpp" line="86"/>
        <source>Finger detected — hold still</source>
        <translation>Палец обнаружен — не двигайте</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="91"/>
        <source>Finger lifted</source>
        <translation>Палец убран</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="104"/>
        <source>Only part of the sensor was touched — cover more of it, centred</source>
        <translation>Задета лишь часть сканера — приложите шире, по центру</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="109"/>
        <source>The ridges are faint — press a little harder</source>
        <translation>Линии слабые — нажмите чуть сильнее</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="114"/>
        <source>Frame received</source>
        <translation>Кадр получен</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="124"/>
        <location filename="../src/engine.cpp" line="134"/>
        <location filename="../src/headless.cpp" line="482"/>
        <source>Captured — now lift your finger</source>
        <translation>Снято — уберите палец</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="139"/>
        <source>Matching against the enrolled print…</source>
        <translation>Сравнение с зарегистрированным отпечатком…</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="144"/>
        <location filename="../src/setupcheck.cpp" line="210"/>
        <source>Secure channel to the sensor established</source>
        <translation>Установлен защищённый канал с датчиком</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="149"/>
        <source>Sensor ready</source>
        <translation>Сканер готов</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="418"/>
        <location filename="../src/engine.cpp" line="499"/>
        <location filename="../src/engine.cpp" line="575"/>
        <source>Gave up after %1 seconds</source>
        <translation>Прервано через %1 с</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="461"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>Регистрация %1/%2 — уберите палец и приложите снова</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <location filename="../src/headless.cpp" line="93"/>
        <location filename="../src/headless.cpp" line="442"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>Приложите палец к сканеру и удерживайте</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="95"/>
        <location filename="../src/headless.cpp" line="443"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>Регистрация — прикладывайте и убирайте палец</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="97"/>
        <location filename="../src/headless.cpp" line="444"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>Приложите зарегистрированный палец к сканеру</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="65"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>Найден Goodix 27c6:55b4</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>A different Goodix sensor is present</source>
        <translation>Обнаружен другой датчик Goodix</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="75"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>Эта сборка поддерживает только 27c6:55b4. Обнаружено: 27c6:%1.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="81"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>Датчик отпечатков Goodix не найден</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="82"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>На шине USB нет устройств с производителем 27c6. Если у машины есть сканер, он может быть отключён в настройках прошивки или быть другой марки.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="101"/>
        <source>Patched driver is active</source>
        <translation>Патченый драйвер активен</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="108"/>
        <location filename="../src/setupcheck.cpp" line="116"/>
        <source>Patched driver is not installed</source>
        <translation>Патченый драйвер не установлен</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="109"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint не распознаёт этот датчик. Патченый форк нужно собрать и установить; эта копия программы не содержит исходный код для этого.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="117"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint не распознаёт этот датчик. Патченый форк добавляет поддержку. Сборка займёт пару минут и заменит системный libfprint, поэтому нужен пароль.

Установите fprintd после, а не до: если установить его сначала, подтянется официальный libfprint и это отменится.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="122"/>
        <source>Build and install the patched libfprint</source>
        <translation>Собрать и установить патченый libfprint</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="152"/>
        <source>Sensor is reachable without a password</source>
        <translation>Датчик доступен без пароля</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="158"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>Правило udev установлено, но ещё не вступило в силу</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="159"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>Каждая операция с датчиком будет запрашивать пароль</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="161"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>Файл правила на месте, но узел устройства всё ещё принадлежит root. Оно вступит в силу при следующем событии add; переподключите сканер или перезагрузите udev.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="164"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>Без правила libusb может открыть датчик только от имени root, поэтому каждый снимок требует пароль.

Это даёт группе wheel чтение и запись датчика. На однопользовательском ноутбуке это тот же человек, что уже мог бы сделать это через pkexec; на общей машине - нет. Пропустить это безопасно - будет просто больше запросов пароля.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="174"/>
        <source>Install the udev rule and reload</source>
        <translation>Установить правило udev и перезагрузить</translation>
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
        <translation>Датчик ответил, но рукопожатие не завершилось</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="218"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>У датчика есть ключ, которого нет у этого драйвера. Следующий шаг - запись нулевого ключа, и его нельзя отменить.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="224"/>
        <source>Could not talk to the sensor</source>
        <translation>Не удалось связаться с датчиком</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="225"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>Рукопожатие не продвинулось достаточно, чтобы понять причину. Сначала проверьте шаги «Драйвер» и «Права доступа» выше.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="238"/>
        <source>The sensor already holds a usable key</source>
        <translation>У датчика уже есть пригодный ключ</translation>
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
        <translation>Датчику нужно записать нулевой ключ</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="252"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>Это нельзя отменить.

Ключ, которым сейчас владеет датчик, нельзя прочитать обратно - протокол возвращает производное от него значение, а не сам ключ - поэтому восстановить резервную копию потом не получится.

Вход по отпечатку в Windows на этой машине перестанет работать навсегда. Если у вас двойная загрузка и вы пользуетесь этим в Windows, остановитесь здесь.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="260"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>Записать нулевой PSK на датчик</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="290"/>
        <source>Image quality has not been measured yet</source>
        <translation>Качество изображения ещё не измерено</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="291"/>
        <source>Press the button below, then put your finger on the sensor and hold it there for a couple of seconds.</source>
        <translation>Нажмите кнопку ниже, затем приложите палец к сканеру и подержите пару секунд.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="294"/>
        <source>Test a capture now</source>
        <translation>Проверить снимок сейчас</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="298"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>Покрытие %1%  ·  чёткость %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="307"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>Ниже примерно 24 сравнение обычно не проходит, даже если драйвер принимает кадр. Причина - сухие пальцы или перегруженный датчик; несколько минут отдыха и немного влаги обычно помогают.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="311"/>
        <source>Try again</source>
        <translation>Повторить</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="324"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>В fprintd зарегистрирован палец</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="330"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>В fprintd не зарегистрирован ни один палец</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="331"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>Эта программа хранит свои регистрации отдельно, и системная аутентификация их не видит. Повторная регистрация через fprintd делает отпечаток пригодным для разблокировки.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="335"/>
        <source>Enrol a finger with fprintd</source>
        <translation>Зарегистрировать палец в fprintd</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="340"/>
        <source>fprintd is not answering</source>
        <translation>fprintd не отвечает</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="341"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>Не удалось связаться с демоном. Обычно он запускается по требованию; возможно, достаточно его установить.</translation>
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
        <translation>Датчик</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="488"/>
        <source>Driver</source>
        <translation>Драйвер</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="489"/>
        <source>Permissions</source>
        <translation>Права доступа</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="490"/>
        <source>Secure channel</source>
        <translation>Защищённый канал</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="491"/>
        <source>Sensor key</source>
        <translation>Ключ датчика</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="492"/>
        <source>Image quality</source>
        <translation>Качество изображения</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="493"/>
        <source>Enrolment</source>
        <translation>Регистрация</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="494"/>
        <source>Unlocking</source>
        <translation>Разблокировка</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="516"/>
        <source>Not checked - no sensor</source>
        <translation>Не проверено - нет датчика</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="20"/>
        <source>Unexpected image size</source>
        <translation>Unexpected image size</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="52"/>
        <source>Waiting for a finger</source>
        <translation>Waiting for a finger</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="53"/>
        <source>Insufficient contact area</source>
        <translation>Insufficient contact area</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="54"/>
        <source>Insufficient ridge contrast</source>
        <translation>Insufficient ridge contrast</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="55"/>
        <source>Insufficient consistent ridge pattern</source>
        <translation>Insufficient consistent ridge pattern</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="56"/>
        <source>Waiting for motion to settle</source>
        <translation>Waiting for motion to settle</translation>
    </message>
    <message>
        <location filename="../src/frameselector.h" line="57"/>
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
        <translation>Кадра пока нет

Нажмите «Снимок», чтобы получить изображение</translation>
    </message>
</context>
<context>
    <name>fpstudio::LiveWindow</name>
    <message>
        <location filename="../src/livewindow.cpp" line="110"/>
        <source>Vulkan GPU is initializing — it calculates separately from the live stream</source>
        <translation>Инициализация Vulkan GPU — вычисления выполняются отдельно от потока в реальном времени</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="119"/>
        <source>%1 · %2 references ready · waiting for a stable candidate</source>
        <translation>%1 · подготовлено эталонов: %2 · ожидание стабильного кандидата</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="146"/>
        <source>GPU comparison error: </source>
        <translation>Ошибка сравнения GPU: </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="151"/>
        <source>GPU %1 hypotheses · %2 ms · image correlation %3 · ridge-direction agreement %4
Best: %5 / %6 — research comparison only; not an authentication decision</source>
        <translation>GPU: гипотез %1 · %2 мс · корреляция изображения %3 · согласованность направления гребней %4
Лучший: %5 / %6 — только исследовательское сравнение, не решение об аутентификации</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="155"/>
        <source>
Alignment is ambiguous: a different position has a similar score</source>
        <translation>
Выравнивание неоднозначно: другая позиция имеет похожий результат</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="156"/>
        <source>
At the search boundary: move the finger slightly toward the centre</source>
        <translation>
На границе поиска: немного переместите палец к центру</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="159"/>
        <source>
Interior ridge support %1/%2 regions · %3</source>
        <translation>
Поддержка внутренних гребней: %1/%2 областей · %3</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="161"/>
        <source>ridges are consistent (identity unverified)</source>
        <translation>гребни согласованы (личность не подтверждена)</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="161"/>
        <source>insufficient match evidence</source>
        <translation>недостаточно доказательств совпадения</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="164"/>
        <source> · 0/1 contact-region overlap %1%</source>
        <translation> · перекрытие области контакта 0/1: %1%</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="166"/>
        <source> · 360° additional search applied</source>
        <translation> · применён дополнительный поиск на 360°</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="168"/>
        <source> · full reference-bank search</source>
        <translation> · поиск по всему банку эталонов</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="168"/>
        <source> · representative-reference fast comparison</source>
        <translation> · быстрое сравнение с представительными эталонами</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="184"/>
        <source>GPU comparison cannot run: </source>
        <translation>Невозможно выполнить сравнение GPU: </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="245"/>
        <source>Vulkan GPU is comparing reference images, angles, and positions in parallel…</source>
        <translation>Vulkan GPU параллельно сравнивает эталонные изображения, углы и позиции…</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="250"/>
        <source>GPU comparison timed out — live collection continues</source>
        <translation>Время сравнения GPU истекло — сбор в реальном времени продолжается</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="322"/>
        <source>Contact %1 · image from comparison %2 (not live)
Gray: background (0) · yellow: contact (1), but weak features · green/red: comparison evidence
Cyan +: transformed reference contact centre · orange +: current probe contact centre · blue: comparison range</source>
        <translation>Контакт %1 · изображение из сравнения %2 (не в реальном времени)
Серый: фон (0) · жёлтый: контакт (1), но признаки слабые · зелёный/красный: доказательства сравнения
Голубой +: преобразованный центр эталонного контакта · оранжевый +: центр текущего проверяемого контакта · синий: область сравнения</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="511"/>
        <source>[%1/%2 contacts compared · stops automatically at the target] </source>
        <translation>[%1/%2 контактов сравнено · остановка автоматически при достижении цели] </translation>
    </message>
</context>
<context>
    <name>fpstudio::MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="109"/>
        <location filename="../src/mainwindow.cpp" line="356"/>
        <source>Device scan</source>
        <translation>Поиск устройств</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="154"/>
        <source>Setup</source>
        <translation>Настройка</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="155"/>
        <source>Set up fingerprint unlock…</source>
        <translation>Настроить разблокировку по отпечатку…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="158"/>
        <source>Language</source>
        <translation>Язык</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="195"/>
        <source>fpstudio — fingerprint sensor diagnostics</source>
        <translation>fpstudio — диагностика сканера отпечатков</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="200"/>
        <source>Looking for a device…</source>
        <translation>Поиск устройства…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="204"/>
        <source>Device</source>
        <translation>Устройство</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="206"/>
        <source>Rescan</source>
        <translation>Искать снова</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="211"/>
        <source>Capture — show image</source>
        <translation>Снимок — показать изображение</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="212"/>
        <location filename="../src/mainwindow.cpp" line="365"/>
        <source>Enrol</source>
        <translation>Регистрация</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="213"/>
        <location filename="../src/mainwindow.cpp" line="370"/>
        <source>Verify</source>
        <translation>Проверить</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="214"/>
        <location filename="../src/mainwindow.cpp" line="374"/>
        <source>Delete enrolment</source>
        <translation>Удалить регистрацию</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="219"/>
        <source>Actions</source>
        <translation>Действия</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="226"/>
        <source>Progress</source>
        <translation>Ход выполнения</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="258"/>
        <source>—</source>
        <translation>—</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="295"/>
        <source>Coverage</source>
        <translation>Покрытие</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="297"/>
        <source>Sharpness</source>
        <translation>Чёткость</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Sample</source>
        <translation>Образец</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Score</source>
        <translation>Балл</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="310"/>
        <source>Keypoints</source>
        <translation>Ключевые точки</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="314"/>
        <source>Match scores</source>
        <translation>Баллы совпадения</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="324"/>
        <source>Driver log</source>
        <translation>Журнал драйвера</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="359"/>
        <source>→ Put your finger on the sensor</source>
        <translation>→ Приложите палец к сканеру</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="360"/>
        <source>Image capture</source>
        <translation>Снимок изображения</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="363"/>
        <source>→ Enrol: press and lift your finger repeatedly</source>
        <translation>→ Регистрация: прикладывайте и убирайте палец</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="368"/>
        <source>→ Verify: present the enrolled finger</source>
        <translation>→ Проверка: приложите зарегистрированный палец</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="385"/>
        <source>in progress…</source>
        <translation>выполняется…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="385"/>
        <source> in progress…</source>
        <translation> выполняется…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="413"/>
        <source>No fingerprint device was found.</source>
        <translation>Сканер отпечатков не найден.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="422"/>
        <source>Driver: %1
ID: %2
Scan: %3 · %4 enrol stages
Raw capture: %5</source>
        <translation>Драйвер: %1
ID: %2
Сканирование: %3 · этапов регистрации: %4
Сырой снимок: %5</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="427"/>
        <source>yes</source>
        <translation>да</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="427"/>
        <source>no</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="439"/>
        <source>No image was returned</source>
        <translation>Изображение не получено</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="443"/>
        <source>Captured %1 × %2</source>
        <translation>Снято %1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="462"/>
        <source>Match   %1 / %2</source>
        <translation>Совпадение   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="463"/>
        <source>No match   %1 / %2</source>
        <translation>Нет совпадения   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="474"/>
        <source>Failed — %1</source>
        <translation>Ошибка — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="475"/>
        <location filename="../src/mainwindow.cpp" line="633"/>
        <location filename="../src/mainwindow.cpp" line="634"/>
        <source>Failed</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="476"/>
        <location filename="../src/mainwindow.cpp" line="635"/>
        <source>Error: %1</source>
        <translation>Ошибка: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="480"/>
        <source>Done — %1</source>
        <translation>Готово — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="485"/>
        <source>Enrolment complete</source>
        <translation>Регистрация завершена</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="487"/>
        <source>Enrolment deleted</source>
        <translation>Регистрация удалена</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="514"/>
        <source>This scan fell below the threshold and was asked for again</source>
        <translation>Скан не достиг порога и был запрошен снова</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="529"/>
        <source>Coverage %1%  ·  sharpness %2  —  faint. Press a little harder</source>
        <translation>Покрытие %1%  ·  чёткость %2  —  слабо. Нажмите сильнее</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="531"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>Покрытие %1%  ·  чёткость %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="552"/>
        <source>%1 × %2</source>
        <translation>%1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="608"/>
        <source>Agent request</source>
        <translation>Запрос агента</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="609"/>
        <source>CLI request</source>
        <translation>Запрос из CLI</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="609"/>
        <source>request</source>
        <translation>запрос</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="625"/>
        <source>Idle</source>
        <translation>Ожидание</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="626"/>
        <source>▸ %1   ·   %2s elapsed</source>
        <translation>▸ %1   ·   прошло %2 с</translation>
    </message>
</context>
<context>
    <name>fpstudio::PrivRunner</name>
    <message>
        <location filename="../src/privrunner.cpp" line="22"/>
        <source>Could not run pkexec</source>
        <translation>Не удалось запустить pkexec</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="31"/>
        <source>The operation terminated abnormally</source>
        <translation>Операция завершилась аварийно</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="38"/>
        <source>Authentication was cancelled or refused</source>
        <translation>Аутентификация отменена или отклонена</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="63"/>
        <source>Could not parse the result (exit code %1)</source>
        <translation>Не удалось разобрать результат (код выхода %1)</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="108"/>
        <source>An operation is already running</source>
        <translation>Операция уже выполняется</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="112"/>
        <source>Could not resolve this program&apos;s own path</source>
        <translation>Не удалось определить путь к самой программе</translation>
    </message>
</context>
<context>
    <name>fpstudio::SetupWizard</name>
    <message>
        <location filename="../src/setupwizard.cpp" line="350"/>
        <source>Failed — %1</source>
        <translation>Ошибка — %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="120"/>
        <source>Skip</source>
        <translation>Пропустить</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="87"/>
        <source>FPStudio · Integrated fingerprint authentication setup</source>
        <translation>FPStudio · Integrated fingerprint authentication setup</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="121"/>
        <source>Check status again</source>
        <translation>Check status again</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="133"/>
        <source>Setup steps complete · Verify real sudo/KDE authentication and password recovery separately.</source>
        <translation>Setup steps complete · Verify real sudo/KDE authentication and password recovery separately.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="135"/>
        <source>Finish</source>
        <translation>Готово</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="161"/>
        <source>Stop</source>
        <translation>Stop</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="164"/>
        <source>Stopping the fingerprint test. You may lift your finger.</source>
        <translation>Stopping the fingerprint test. You may lift your finger.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="167"/>
        <location filename="../src/setupwizard.cpp" line="548"/>
        <source>Restore authentication settings</source>
        <translation>Restore authentication settings</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="168"/>
        <source>Test system fingerprint</source>
        <translation>Test system fingerprint</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="182"/>
        <source>Open diagnostics tool</source>
        <translation>Открыть инструмент диагностики</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="185"/>
        <source>Live capture preview, the driver log, and MCP status - for a closer look once the checklist here is done.</source>
        <translation>Предпросмотр снимка вживую, журнал драйвера и статус MCP - для более пристального взгляда после завершения этого списка.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="219"/>
        <source>Checking the device and installation. Keep your finger off the sensor for now.</source>
        <translation>Checking the device and installation. Keep your finger off the sensor for now.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="295"/>
        <source>This step cannot be undone.</source>
        <translation>Этот шаг нельзя отменить.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="311"/>
        <source>This will ask for your password.</source>
        <translation>Это запросит ваш пароль.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="313"/>
        <source>This step is confirmed. Continue to the next required step.</source>
        <translation>This step is confirmed. Continue to the next required step.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="314"/>
        <source>Next action: %1</source>
        <translation>Next action: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="314"/>
        <source>Review the guidance. This step is not confirmed yet.</source>
        <translation>Review the guidance. This step is not confirmed yet.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="324"/>
        <source>WRITE</source>
        <translation>WRITE</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="326"/>
        <source>Sensor key</source>
        <translation>Ключ датчика</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="327"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

Введите %2 для подтверждения.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="351"/>
        <source>The enrolment helper was not found</source>
        <translation>Помощник регистрации не найден</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="357"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>Регистрация — прикладывайте и убирайте палец</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="365"/>
        <source>Enrolment in progress · Present and lift the same finger as instructed
</source>
        <translation>Enrolment in progress · Present and lift the same finger as instructed
</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="370"/>
        <source>Could not start the enrolment tool: %1</source>
        <translation>Could not start the enrolment tool: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="397"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>Приложите палец к сканеру и удерживайте</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="406"/>
        <source>Could not start the image tool: %1</source>
        <translation>Could not start the image tool: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="468"/>
        <source>in progress…</source>
        <translation>выполняется…</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="475"/>
        <source>Wait for the operation to finish or cancel its authentication request. Setup records and recovery information are being protected.</source>
        <translation>Wait for the operation to finish or cancel its authentication request. Setup records and recovery information are being protected.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="479"/>
        <source>Finish the running setup operation or cancel its authentication request before closing.</source>
        <translation>Finish the running setup operation or cancel its authentication request before closing.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="495"/>
        <source>Setup operation complete. Use the system fingerprint test to verify the real path.</source>
        <translation>Setup operation complete. Use the system fingerprint test to verify the real path.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="495"/>
        <source>Setup incomplete or cancelled: %1</source>
        <translation>Setup incomplete or cancelled: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="501"/>
        <source>After administrator authorization, installation and checks run automatically. Authentication settings are restored if installation fails.</source>
        <translation>After administrator authorization, installation and checks run automatically. Authentication settings are restored if installation fails.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="502"/>
        <source>The driver is built as the current user. Administrator authorization is requested only for dependencies and package installation.</source>
        <translation>The driver is built as the current user. Administrator authorization is requested only for dependencies and package installation.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="512"/>
        <source>Update GPU authentication engine</source>
        <translation>Update GPU authentication engine</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="513"/>
        <source>Existing fingerprint references, fprintd enrolment and PAM settings will not change. Only the matcher, shader and driver bridge are replaced, followed by reference self-checks. Changed files are backed up separately. Apply the update?</source>
        <translation>Existing fingerprint references, fprintd enrolment and PAM settings will not change. Only the matcher, shader and driver bridge are replaced, followed by reference self-checks. Changed files are backed up separately. Apply the update?</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="524"/>
        <source>Select your saved fingerprint session</source>
        <translation>Select your saved fingerprint session</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="528"/>
        <source>Experimental GPU authentication</source>
        <translation>Experimental GPU authentication</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="529"/>
        <source>Confirm that the selected folders contain this user&apos;s fingerprints.
%1

Existing enrolment is preserved and GPU comparison is used for sudo and KDE authentication. After at most 20 failed contacts or 60 seconds, the password path remains available. Rejection of other fingers is not fully validated. Back up the settings and apply?</source>
        <translation>Confirm that the selected folders contain this user&apos;s fingerprints.
%1

Existing enrolment is preserved and GPU comparison is used for sudo and KDE authentication. After at most 20 failed contacts or 60 seconds, the password path remains available. Rejection of other fingers is not fully validated. Back up the settings and apply?</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="548"/>
        <source>Restore the sudo, KDE and fprintd settings from before installation? Existing fingerprint enrolment is not deleted.</source>
        <translation>Restore the sudo, KDE and fprintd settings from before installation? Existing fingerprint enrolment is not deleted.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="574"/>
        <source>Fingerprint match succeeded. The result was delivered while the finger remained in place. Test the sudo/KDE dialog and password fallback separately.</source>
        <translation>Fingerprint match succeeded. The result was delivered while the finger remained in place. Test the sudo/KDE dialog and password fallback separately.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="575"/>
        <source>Sensor thermal protection stopped the test. Lift your finger, let it cool briefly, then try again.</source>
        <translation>Sensor thermal protection stopped the test. Lift your finger, let it cool briefly, then try again.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="576"/>
        <source>No finger was detected during this 30-second one-shot test. Clear the sensor, then place the enrolled finger in the centre for one or two seconds when prompted.</source>
        <translation>No finger was detected during this 30-second one-shot test. Clear the sensor, then place the enrolled finger in the centre for one or two seconds when prompted.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="577"/>
        <source>System fingerprint test incomplete: %1</source>
        <translation>System fingerprint test incomplete: %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="587"/>
        <source>[System fingerprint test] Match confirmed while the finger remained in place.
</source>
        <translation>[System fingerprint test] Match confirmed while the finger remained in place.
</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="589"/>
        <source>[System fingerprint test] No match. This contact was not recorded or enrolled.
</source>
        <translation>[System fingerprint test] No match. This contact was not recorded or enrolled.
</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="591"/>
        <source>[System fingerprint test] Place the finger in the centre and hold it until a result appears. You do not need to lift it.
</source>
        <translation>[System fingerprint test] Place the finger in the centre and hold it until a result appears. You do not need to lift it.
</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="606"/>
        <source>[System fingerprint test] Start with the sensor clear. When prompted, place the same enrolled finger and hold it until the result appears. You do not need to lift it.</source>
        <translation>[System fingerprint test] Start with the sensor clear. When prompted, place the same enrolled finger and hold it until the result appears. You do not need to lift it.</translation>
    </message>
</context>
</TS>
