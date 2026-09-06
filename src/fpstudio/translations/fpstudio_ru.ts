<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru" sourcelanguage="en">
<context>
    <name>fpstudio</name>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <source>Finger detected — hold still</source>
        <translation>Палец обнаружен — не двигайте</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="84"/>
        <source>Finger lifted</source>
        <translation>Палец убран</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="97"/>
        <source>Only part of the sensor was touched — cover more of it, centred</source>
        <translation>Задета лишь часть сканера — приложите шире, по центру</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="102"/>
        <source>The ridges are faint — press a little harder</source>
        <translation>Линии слабые — нажмите чуть сильнее</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="107"/>
        <source>Frame received</source>
        <translation>Кадр получен</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="117"/>
        <location filename="../src/engine.cpp" line="127"/>
        <source>Captured — now lift your finger</source>
        <translation>Снято — уберите палец</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="132"/>
        <source>Matching against the enrolled print…</source>
        <translation>Сравнение с зарегистрированным отпечатком…</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="137"/>
        <location filename="../src/setupcheck.cpp" line="200"/>
        <source>Secure channel to the sensor established</source>
        <translation>Установлен защищённый канал с датчиком</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="142"/>
        <source>Sensor ready</source>
        <translation>Сканер готов</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="403"/>
        <location filename="../src/engine.cpp" line="484"/>
        <location filename="../src/engine.cpp" line="560"/>
        <source>Gave up after %1 seconds</source>
        <translation>Прервано через %1 с</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="446"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>Регистрация %1/%2 — уберите палец и приложите снова</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="81"/>
        <location filename="../src/headless.cpp" line="373"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>Приложите палец к сканеру и удерживайте</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="83"/>
        <location filename="../src/headless.cpp" line="374"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>Регистрация — прикладывайте и убирайте палец</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="85"/>
        <location filename="../src/headless.cpp" line="375"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>Приложите зарегистрированный палец к сканеру</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="58"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>Найден Goodix 27c6:55b4</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="67"/>
        <source>A different Goodix sensor is present</source>
        <translation>Обнаружен другой датчик Goodix</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="68"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>Эта сборка поддерживает только 27c6:55b4. Обнаружено: 27c6:%1.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>Датчик отпечатков Goodix не найден</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="75"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>На шине USB нет устройств с производителем 27c6. Если у машины есть сканер, он может быть отключён в настройках прошивки или быть другой марки.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="94"/>
        <source>Patched driver is active</source>
        <translation>Патченый драйвер активен</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="101"/>
        <location filename="../src/setupcheck.cpp" line="109"/>
        <source>Patched driver is not installed</source>
        <translation>Патченый драйвер не установлен</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="102"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint не распознаёт этот датчик. Патченый форк нужно собрать и установить; эта копия программы не содержит исходный код для этого.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="110"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint не распознаёт этот датчик. Патченый форк добавляет поддержку. Сборка займёт пару минут и заменит системный libfprint, поэтому нужен пароль.

Установите fprintd после, а не до: если установить его сначала, подтянется официальный libfprint и это отменится.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="115"/>
        <source>Build and install the patched libfprint</source>
        <translation>Собрать и установить патченый libfprint</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="148"/>
        <source>Sensor is reachable without a password</source>
        <translation>Датчик доступен без пароля</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="154"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>Правило udev установлено, но ещё не вступило в силу</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="155"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>Каждая операция с датчиком будет запрашивать пароль</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="157"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>Файл правила на месте, но узел устройства всё ещё принадлежит root. Оно вступит в силу при следующем событии add; переподключите сканер или перезагрузите udev.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="160"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>Без правила libusb может открыть датчик только от имени root, поэтому каждый снимок требует пароль.

Это даёт группе wheel чтение и запись датчика. На однопользовательском ноутбуке это тот же человек, что уже мог бы сделать это через pkexec; на общей машине - нет. Пропустить это безопасно - будет просто больше запросов пароля.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="170"/>
        <source>Install the udev rule and reload</source>
        <translation>Установить правило udev и перезагрузить</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="208"/>
        <source>The sensor answered, but the handshake did not complete</source>
        <translation>Датчик ответил, но рукопожатие не завершилось</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="209"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>У датчика есть ключ, которого нет у этого драйвера. Следующий шаг - запись нулевого ключа, и его нельзя отменить.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="215"/>
        <source>Could not talk to the sensor</source>
        <translation>Не удалось связаться с датчиком</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="216"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>Рукопожатие не продвинулось достаточно, чтобы понять причину. Сначала проверьте шаги «Драйвер» и «Права доступа» выше.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="229"/>
        <source>The sensor already holds a usable key</source>
        <translation>У датчика уже есть пригодный ключ</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="235"/>
        <source>The sensor needs the all-zero key written</source>
        <translation>Датчику нужно записать нулевой ключ</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="236"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>Это нельзя отменить.

Ключ, которым сейчас владеет датчик, нельзя прочитать обратно - протокол возвращает производное от него значение, а не сам ключ - поэтому восстановить резервную копию потом не получится.

Вход по отпечатку в Windows на этой машине перестанет работать навсегда. Если у вас двойная загрузка и вы пользуетесь этим в Windows, остановитесь здесь.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="244"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>Записать нулевой PSK на датчик</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="274"/>
        <source>Image quality has not been measured yet</source>
        <translation>Качество изображения ещё не измерено</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="275"/>
        <source>Press the button below, then put your finger on the sensor and hold it there for a couple of seconds.</source>
        <translation>Нажмите кнопку ниже, затем приложите палец к сканеру и подержите пару секунд.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="278"/>
        <source>Test a capture now</source>
        <translation>Проверить снимок сейчас</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="282"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>Покрытие %1%  ·  чёткость %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="291"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>Ниже примерно 24 сравнение обычно не проходит, даже если драйвер принимает кадр. Причина - сухие пальцы или перегруженный датчик; несколько минут отдыха и немного влаги обычно помогают.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="295"/>
        <source>Try again</source>
        <translation>Повторить</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="308"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>В fprintd зарегистрирован палец</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="314"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>В fprintd не зарегистрирован ни один палец</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="315"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>Эта программа хранит свои регистрации отдельно, и системная аутентификация их не видит. Повторная регистрация через fprintd делает отпечаток пригодным для разблокировки.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="319"/>
        <source>Enrol a finger with fprintd</source>
        <translation>Зарегистрировать палец в fprintd</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="324"/>
        <source>fprintd is not answering</source>
        <translation>fprintd не отвечает</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="325"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>Не удалось связаться с демоном. Обычно он запускается по требованию; возможно, достаточно его установить.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="341"/>
        <source>polkit accepts a fingerprint</source>
        <translation>polkit принимает отпечаток</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="349"/>
        <location filename="../src/setupcheck.cpp" line="390"/>
        <source>pam_fprintd is not installed</source>
        <translation>pam_fprintd не установлен</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="350"/>
        <location filename="../src/setupcheck.cpp" line="391"/>
        <source>The PAM module that lets authentication use a fingerprint is missing. It usually ships with fprintd.</source>
        <translation>Отсутствует модуль PAM, позволяющий использовать отпечаток при аутентификации. Обычно поставляется с fprintd.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="357"/>
        <source>polkit still asks for a password</source>
        <translation>polkit по-прежнему запрашивает пароль</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="358"/>
        <source>This adds one line to the stack polkit uses, so pkexec and the desktop&apos;s authentication dialog try the fingerprint first.

Login and sudo are deliberately left alone, so a sensor that stops working can never lock you out of the machine. The line is &apos;sufficient&apos;: if the fingerprint fails for any reason, you are asked for the password exactly as before.</source>
        <translation>Это добавляет одну строку в стек, который использует polkit, чтобы pkexec и диалог аутентификации рабочего стола сначала пробовали отпечаток.

Вход в систему и sudo намеренно оставлены без изменений, чтобы неисправный датчик никогда не мог заблокировать вам доступ к машине. Строка 'sufficient': если отпечаток по какой-либо причине не сработает, пароль будет запрошен точно так же, как раньше.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="366"/>
        <source>Let polkit accept a fingerprint</source>
        <translation>Разрешить polkit принимать отпечаток</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="382"/>
        <source>sudo accepts a fingerprint</source>
        <translation>sudo принимает отпечаток</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="402"/>
        <source>Terminal sudo still asks for a password only</source>
        <translation>Терминальный sudo пока принимает только пароль</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="403"/>
        <source>Optional, and a step further than the polkit rule above: sudo is usually the way back in when something else on the machine is broken, so extending fingerprint auth to it is worth doing deliberately rather than by default.

The safety net is the same either way. The line added is &apos;sufficient&apos;, so a failing fingerprint falls back to the password exactly as before. sudo itself never stops working - only the fingerprint shortcut can.</source>
        <translation>Необязательно, и это шаг дальше правила polkit выше: sudo обычно является путём назад, когда что-то ещё на машине сломано, поэтому расширение аутентификации по отпечатку на него стоит решать осознанно, а не по умолчанию.

Страховка та же в любом случае. Добавленная строка - 'sufficient', поэтому неудачный отпечаток откатывается к паролю точно как раньше. Сам sudo никогда не перестаёт работать - может перестать работать только ярлык с отпечатком.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="413"/>
        <source>Let sudo accept a fingerprint</source>
        <translation>Разрешить sudo принимать отпечаток</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="440"/>
        <source>Sensor</source>
        <translation>Датчик</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="441"/>
        <source>Driver</source>
        <translation>Драйвер</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="442"/>
        <source>Permissions</source>
        <translation>Права доступа</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="443"/>
        <source>Secure channel</source>
        <translation>Защищённый канал</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="444"/>
        <source>Sensor key</source>
        <translation>Ключ датчика</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="445"/>
        <source>Image quality</source>
        <translation>Качество изображения</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="446"/>
        <source>Enrolment</source>
        <translation>Регистрация</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="447"/>
        <source>Unlocking</source>
        <translation>Разблокировка</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="448"/>
        <source>Terminal sudo (optional)</source>
        <translation>Терминальный sudo (необязательно)</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="467"/>
        <source>Not checked - no sensor</source>
        <translation>Не проверено - нет датчика</translation>
    </message>
</context>
<context>
    <name>fpstudio::ImageView</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="57"/>
        <source>No frame yet

Use Capture to pull an image from the sensor</source>
        <translation>Кадра пока нет

Нажмите «Снимок», чтобы получить изображение</translation>
    </message>
</context>
<context>
    <name>fpstudio::MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="108"/>
        <location filename="../src/mainwindow.cpp" line="355"/>
        <source>Device scan</source>
        <translation>Поиск устройств</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="153"/>
        <source>Setup</source>
        <translation>Настройка</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="154"/>
        <source>Set up fingerprint unlock…</source>
        <translation>Настроить разблокировку по отпечатку…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="157"/>
        <source>Language</source>
        <translation>Язык</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="194"/>
        <source>fpstudio — fingerprint sensor diagnostics</source>
        <translation>fpstudio — диагностика сканера отпечатков</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="199"/>
        <source>Looking for a device…</source>
        <translation>Поиск устройства…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="203"/>
        <source>Device</source>
        <translation>Устройство</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="205"/>
        <source>Rescan</source>
        <translation>Искать снова</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="210"/>
        <source>Capture — show image</source>
        <translation>Снимок — показать изображение</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="211"/>
        <location filename="../src/mainwindow.cpp" line="364"/>
        <source>Enrol</source>
        <translation>Регистрация</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="212"/>
        <location filename="../src/mainwindow.cpp" line="369"/>
        <source>Verify</source>
        <translation>Проверить</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="213"/>
        <location filename="../src/mainwindow.cpp" line="373"/>
        <source>Delete enrolment</source>
        <translation>Удалить регистрацию</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="218"/>
        <source>Actions</source>
        <translation>Действия</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="225"/>
        <source>Progress</source>
        <translation>Ход выполнения</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="257"/>
        <source>—</source>
        <translation>—</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="294"/>
        <source>Coverage</source>
        <translation>Покрытие</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="296"/>
        <source>Sharpness</source>
        <translation>Чёткость</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Sample</source>
        <translation>Образец</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Score</source>
        <translation>Балл</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Keypoints</source>
        <translation>Ключевые точки</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="313"/>
        <source>Match scores</source>
        <translation>Баллы совпадения</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="323"/>
        <source>Driver log</source>
        <translation>Журнал драйвера</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="358"/>
        <source>→ Put your finger on the sensor</source>
        <translation>→ Приложите палец к сканеру</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="359"/>
        <source>Image capture</source>
        <translation>Снимок изображения</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="362"/>
        <source>→ Enrol: press and lift your finger repeatedly</source>
        <translation>→ Регистрация: прикладывайте и убирайте палец</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="367"/>
        <source>→ Verify: present the enrolled finger</source>
        <translation>→ Проверка: приложите зарегистрированный палец</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source>in progress…</source>
        <translation>выполняется…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source> in progress…</source>
        <translation> выполняется…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="412"/>
        <source>No fingerprint device was found.</source>
        <translation>Сканер отпечатков не найден.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="421"/>
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
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>yes</source>
        <translation>да</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>no</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="438"/>
        <source>No image was returned</source>
        <translation>Изображение не получено</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="442"/>
        <source>Captured %1 × %2</source>
        <translation>Снято %1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="461"/>
        <source>Match   %1 / %2</source>
        <translation>Совпадение   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="462"/>
        <source>No match   %1 / %2</source>
        <translation>Нет совпадения   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="473"/>
        <source>Failed — %1</source>
        <translation>Ошибка — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="474"/>
        <location filename="../src/mainwindow.cpp" line="623"/>
        <location filename="../src/mainwindow.cpp" line="624"/>
        <source>Failed</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="475"/>
        <location filename="../src/mainwindow.cpp" line="625"/>
        <source>Error: %1</source>
        <translation>Ошибка: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="479"/>
        <source>Done — %1</source>
        <translation>Готово — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484"/>
        <source>Enrolment complete</source>
        <translation>Регистрация завершена</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="486"/>
        <source>Enrolment deleted</source>
        <translation>Регистрация удалена</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513"/>
        <source>This scan fell below the threshold and was asked for again</source>
        <translation>Скан не достиг порога и был запрошен снова</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="528"/>
        <source>Coverage %1%  ·  sharpness %2  —  faint. Press a little harder</source>
        <translation>Покрытие %1%  ·  чёткость %2  —  слабо. Нажмите сильнее</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="530"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>Покрытие %1%  ·  чёткость %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="598"/>
        <source>Agent request</source>
        <translation>Запрос агента</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>CLI request</source>
        <translation>Запрос из CLI</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>request</source>
        <translation>запрос</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="615"/>
        <source>Idle</source>
        <translation>Ожидание</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="616"/>
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
        <location filename="../src/setupwizard.cpp" line="67"/>
        <source>The command did not finish in time</source>
        <translation>Команда не завершилась вовремя</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="75"/>
        <source>Authentication was cancelled or refused</source>
        <translation>Аутентификация отменена или отклонена</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="76"/>
        <location filename="../src/setupwizard.cpp" line="347"/>
        <location filename="../src/setupwizard.cpp" line="443"/>
        <source>Failed — %1</source>
        <translation>Ошибка — %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="114"/>
        <source>Set up fingerprint unlock</source>
        <translation>Настроить разблокировку по отпечатку</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="145"/>
        <source>Skip</source>
        <translation>Пропустить</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="146"/>
        <source>Re-check</source>
        <translation>Проверить снова</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="157"/>
        <source>Fingerprint unlock is ready.</source>
        <translation>Разблокировка по отпечатку готова.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="159"/>
        <source>Finish</source>
        <translation>Готово</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="194"/>
        <source>Open diagnostics tool</source>
        <translation>Открыть инструмент диагностики</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="197"/>
        <source>Live capture preview, the driver log, and MCP status - for a closer look once the checklist here is done.</source>
        <translation>Предпросмотр снимка вживую, журнал драйвера и статус MCP - для более пристального взгляда после завершения этого списка.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="229"/>
        <source>Checking…</source>
        <translation>Проверка…</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="294"/>
        <source>This step cannot be undone.</source>
        <translation>Этот шаг нельзя отменить.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="310"/>
        <source>This will ask for your password.</source>
        <translation>Это запросит ваш пароль.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="322"/>
        <source>WRITE</source>
        <translation>WRITE</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="324"/>
        <source>Sensor key</source>
        <translation>Ключ датчика</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="325"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

Введите %2 для подтверждения.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="348"/>
        <source>The enrolment helper was not found</source>
        <translation>Помощник регистрации не найден</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="354"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>Регистрация — прикладывайте и убирайте палец</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="378"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>Приложите палец к сканеру и удерживайте</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="427"/>
        <source>in progress…</source>
        <translation>выполняется…</translation>
    </message>
</context>
</TS>
