<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="de" sourcelanguage="en">
<context>
    <name>fpstudio</name>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <source>Finger detected — hold still</source>
        <translation>Finger erkannt — bitte still halten</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="84"/>
        <source>Finger lifted</source>
        <translation>Finger abgehoben</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="97"/>
        <source>Only part of the sensor was touched — cover more of it, centred</source>
        <translation>Nur ein Teil des Sensors wurde berührt — mittiger und großflächiger auflegen</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="102"/>
        <source>The ridges are faint — press a little harder</source>
        <translation>Die Papillarlinien sind schwach — etwas fester drücken</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="107"/>
        <source>Frame received</source>
        <translation>Bild empfangen</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="117"/>
        <location filename="../src/engine.cpp" line="127"/>
        <source>Captured — now lift your finger</source>
        <translation>Aufgenommen — Finger jetzt abheben</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="132"/>
        <source>Matching against the enrolled print…</source>
        <translation>Abgleich mit dem registrierten Abdruck…</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="137"/>
        <location filename="../src/setupcheck.cpp" line="200"/>
        <source>Secure channel to the sensor established</source>
        <translation>Sicherer Kanal zum Sensor hergestellt</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="142"/>
        <source>Sensor ready</source>
        <translation>Sensor bereit</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="403"/>
        <location filename="../src/engine.cpp" line="484"/>
        <location filename="../src/engine.cpp" line="560"/>
        <source>Gave up after %1 seconds</source>
        <translation>Nach %1 Sekunden abgebrochen</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="446"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>Registrierung %1/%2 — Finger abheben und erneut auflegen</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="81"/>
        <location filename="../src/headless.cpp" line="373"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>Finger auf den Sensor legen und dort halten</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="83"/>
        <location filename="../src/headless.cpp" line="374"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>Registrierung läuft — Finger wiederholt auflegen und abheben</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="85"/>
        <location filename="../src/headless.cpp" line="375"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>Registrierten Finger auf den Sensor legen</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="58"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>Goodix 27c6:55b4 gefunden</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="67"/>
        <source>A different Goodix sensor is present</source>
        <translation>Ein anderer Goodix-Sensor ist vorhanden</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="68"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>Dieser Build unterstützt nur 27c6:55b4. Gefunden: 27c6:%1.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>Kein Goodix-Fingerabdrucksensor gefunden</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="75"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>Am USB-Bus antwortet nichts auf Hersteller 27c6. Falls das Gerät einen Sensor hat, ist er möglicherweise in den Firmware-Einstellungen deaktiviert oder stammt von einem anderen Hersteller.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="94"/>
        <source>Patched driver is active</source>
        <translation>Gepatchter Treiber ist aktiv</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="101"/>
        <location filename="../src/setupcheck.cpp" line="109"/>
        <source>Patched driver is not installed</source>
        <translation>Gepatchter Treiber ist nicht installiert</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="102"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint erkennt diesen Sensor nicht. Der gepatchte Fork muss gebaut und installiert werden; diese Programmkopie enthält dafür keinen Quellcode.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="110"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint erkennt diesen Sensor nicht. Der gepatchte Fork fügt Unterstützung hinzu. Der Build dauert einige Minuten und ersetzt das System-libfprint, daher wird ein Passwort benötigt.

fprintd danach installieren, nicht vorher: Wird es zuerst installiert, zieht es das offizielle libfprint nach und macht dies rückgängig.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="115"/>
        <source>Build and install the patched libfprint</source>
        <translation>Gepatchtes libfprint bauen und installieren</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="148"/>
        <source>Sensor is reachable without a password</source>
        <translation>Sensor ist ohne Passwort erreichbar</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="154"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>udev-Regel ist installiert, aber noch nicht wirksam</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="155"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>Jede Sensoraktion fragt nach einem Passwort</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="157"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>Die Regel-Datei ist vorhanden, aber der Gerätenode gehört noch root. Sie greift beim nächsten Add-Ereignis; den Leser neu einstecken oder udev neu laden genügt.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="160"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>Ohne Regel kann libusb den Sensor nur als root öffnen, sodass jede Aufnahme ein Passwort verlangt.

Dies gewährt der Gruppe wheel Lese- und Schreibzugriff auf den Sensor. Auf einem Einzelbenutzer-Laptop ist das dieselbe Person, die es ohnehin über pkexec könnte; auf einer gemeinsam genutzten Maschine nicht. Dies zu überspringen ist sicher - es bedeutet nur mehr Passwortabfragen.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="170"/>
        <source>Install the udev rule and reload</source>
        <translation>udev-Regel installieren und neu laden</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="208"/>
        <source>The sensor answered, but the handshake did not complete</source>
        <translation>Der Sensor hat geantwortet, aber der Handshake wurde nicht abgeschlossen</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="209"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>Der Sensor besitzt einen Schlüssel, den dieser Treiber nicht hat. Der nächste Schritt ist das Schreiben des Nullschlüssels, und das kann nicht rückgängig gemacht werden.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="215"/>
        <source>Could not talk to the sensor</source>
        <translation>Kommunikation mit dem Sensor fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="216"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>Der Handshake kam nicht weit genug, um den Grund zu nennen. Prüfen Sie zuerst die Schritte „Treiber“ und „Berechtigungen“ oben.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="229"/>
        <source>The sensor already holds a usable key</source>
        <translation>Der Sensor besitzt bereits einen brauchbaren Schlüssel</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="235"/>
        <source>The sensor needs the all-zero key written</source>
        <translation>Der Sensor benötigt das Schreiben des Nullschlüssels</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="236"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>Dies kann nicht rückgängig gemacht werden.

Der Schlüssel, den der Sensor jetzt besitzt, kann nicht ausgelesen werden - was das Protokoll zurückgibt, ist ein davon abgeleiteter Wert, nicht der Schlüssel selbst - daher gibt es danach kein Backup zum Wiederherstellen.

Die Windows-Fingerabdruckanmeldung wird auf diesem Gerät dauerhaft nicht mehr funktionieren. Wenn Sie Dual-Boot nutzen und es dort brauchen, brechen Sie hier ab.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="244"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>Den Null-PSK auf den Sensor schreiben</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="274"/>
        <source>Image quality has not been measured yet</source>
        <translation>Bildqualität wurde noch nicht gemessen</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="275"/>
        <source>Press the button below, then put your finger on the sensor and hold it there for a couple of seconds.</source>
        <translation>Drücken Sie die Schaltfläche unten und legen Sie dann den Finger einige Sekunden auf den Sensor.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="278"/>
        <source>Test a capture now</source>
        <translation>Jetzt eine Aufnahme testen</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="282"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>Abdeckung %1%  ·  Schärfe %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="291"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>Unter etwa 24 schlägt der Abgleich meist fehl, obwohl der Treiber das Bild annimmt. Trockene Fingerspitzen und ein stark beanspruchter Sensor verursachen das beide; ein paar Minuten Pause und etwas Feuchtigkeit beheben es meist.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="295"/>
        <source>Try again</source>
        <translation>Erneut versuchen</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="308"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>Ein Finger ist bei fprintd registriert</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="314"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>Kein Finger ist bei fprintd registriert</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="315"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>Dieses Programm führt seine eigenen Registrierungen separat, und die Systemauthentifizierung sieht diese nicht. Erneutes Registrieren über fprintd macht den Fingerabdruck zum Entsperren nutzbar.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="319"/>
        <source>Enrol a finger with fprintd</source>
        <translation>Einen Finger bei fprintd registrieren</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="324"/>
        <source>fprintd is not answering</source>
        <translation>fprintd antwortet nicht</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="325"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>Der Dienst war nicht erreichbar. Er wird normalerweise bei Bedarf gestartet; ihn zu installieren ist oft alles, was fehlt.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="341"/>
        <source>polkit accepts a fingerprint</source>
        <translation>polkit akzeptiert einen Fingerabdruck</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="349"/>
        <location filename="../src/setupcheck.cpp" line="390"/>
        <source>pam_fprintd is not installed</source>
        <translation>pam_fprintd ist nicht installiert</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="350"/>
        <location filename="../src/setupcheck.cpp" line="391"/>
        <source>The PAM module that lets authentication use a fingerprint is missing. It usually ships with fprintd.</source>
        <translation>Das PAM-Modul, das Authentifizierung per Fingerabdruck ermöglicht, fehlt. Es wird meist mit fprintd ausgeliefert.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="357"/>
        <source>polkit still asks for a password</source>
        <translation>polkit fragt weiterhin nach einem Passwort</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="358"/>
        <source>This adds one line to the stack polkit uses, so pkexec and the desktop&apos;s authentication dialog try the fingerprint first.

Login and sudo are deliberately left alone, so a sensor that stops working can never lock you out of the machine. The line is &apos;sufficient&apos;: if the fingerprint fails for any reason, you are asked for the password exactly as before.</source>
        <translation>Dies fügt der von polkit verwendeten Stapelkonfiguration eine Zeile hinzu, damit pkexec und der Anmeldedialog der Desktop-Umgebung zuerst den Fingerabdruck versuchen.

Anmeldung und sudo bleiben absichtlich unangetastet, damit ein ausfallender Sensor Sie niemals aus dem System aussperren kann. Die Zeile ist &apos;sufficient&apos;: Schlägt der Fingerabdruck aus irgendeinem Grund fehl, werden Sie genau wie zuvor nach dem Passwort gefragt.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="366"/>
        <source>Let polkit accept a fingerprint</source>
        <translation>polkit einen Fingerabdruck akzeptieren lassen</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="382"/>
        <source>sudo accepts a fingerprint</source>
        <translation>sudo akzeptiert einen Fingerabdruck</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="402"/>
        <source>Terminal sudo still asks for a password only</source>
        <translation>Terminal-sudo verlangt weiterhin nur ein Passwort</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="403"/>
        <source>Optional, and a step further than the polkit rule above: sudo is usually the way back in when something else on the machine is broken, so extending fingerprint auth to it is worth doing deliberately rather than by default.

The safety net is the same either way. The line added is &apos;sufficient&apos;, so a failing fingerprint falls back to the password exactly as before. sudo itself never stops working - only the fingerprint shortcut can.</source>
        <translation>Optional, und ein Schritt weiter als die obige polkit-Regel: sudo ist meist der Weg zurück, wenn etwas anderes auf dem Rechner kaputt ist, daher sollte man die Fingerabdruck-Authentifizierung darauf bewusst und nicht standardmäßig ausweiten.

Das Sicherheitsnetz ist in jedem Fall dasselbe. Die hinzugefügte Zeile ist &apos;sufficient&apos;, sodass ein fehlschlagender Fingerabdruck genau wie zuvor auf das Passwort zurückfällt. sudo selbst hört nie auf zu funktionieren - nur die Fingerabdruck-Abkürzung kann das.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="413"/>
        <source>Let sudo accept a fingerprint</source>
        <translation>sudo einen Fingerabdruck akzeptieren lassen</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="440"/>
        <source>Sensor</source>
        <translation>Sensor</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="441"/>
        <source>Driver</source>
        <translation>Treiber</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="442"/>
        <source>Permissions</source>
        <translation>Berechtigungen</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="443"/>
        <source>Secure channel</source>
        <translation>Sicherer Kanal</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="444"/>
        <source>Sensor key</source>
        <translation>Sensorschlüssel</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="445"/>
        <source>Image quality</source>
        <translation>Bildqualität</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="446"/>
        <source>Enrolment</source>
        <translation>Registrierung</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="447"/>
        <source>Unlocking</source>
        <translation>Entsperren</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="448"/>
        <source>Terminal sudo (optional)</source>
        <translation>Terminal-sudo (optional)</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="467"/>
        <source>Not checked - no sensor</source>
        <translation>Nicht geprüft - kein Sensor</translation>
    </message>
</context>
<context>
    <name>fpstudio::ImageView</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="57"/>
        <source>No frame yet

Use Capture to pull an image from the sensor</source>
        <translation>Noch kein Bild

Mit „Aufnehmen“ ein Bild vom Sensor holen</translation>
    </message>
</context>
<context>
    <name>fpstudio::MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="108"/>
        <location filename="../src/mainwindow.cpp" line="355"/>
        <source>Device scan</source>
        <translation>Gerätesuche</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="153"/>
        <source>Setup</source>
        <translation>Einrichtung</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="154"/>
        <source>Set up fingerprint unlock…</source>
        <translation>Fingerabdruck-Entsperrung einrichten…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="157"/>
        <source>Language</source>
        <translation>Sprache</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="194"/>
        <source>fpstudio — fingerprint sensor diagnostics</source>
        <translation>fpstudio — Fingerabdrucksensor-Diagnose</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="199"/>
        <source>Looking for a device…</source>
        <translation>Gerät wird gesucht…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="203"/>
        <source>Device</source>
        <translation>Gerät</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="205"/>
        <source>Rescan</source>
        <translation>Erneut suchen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="210"/>
        <source>Capture — show image</source>
        <translation>Aufnehmen — Bild anzeigen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="211"/>
        <location filename="../src/mainwindow.cpp" line="364"/>
        <source>Enrol</source>
        <translation>Registrieren</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="212"/>
        <location filename="../src/mainwindow.cpp" line="369"/>
        <source>Verify</source>
        <translation>Prüfen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="213"/>
        <location filename="../src/mainwindow.cpp" line="373"/>
        <source>Delete enrolment</source>
        <translation>Registrierung löschen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="218"/>
        <source>Actions</source>
        <translation>Aktionen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="225"/>
        <source>Progress</source>
        <translation>Fortschritt</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="257"/>
        <source>—</source>
        <translation>—</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="294"/>
        <source>Coverage</source>
        <translation>Abdeckung</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="296"/>
        <source>Sharpness</source>
        <translation>Schärfe</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Sample</source>
        <translation>Probe</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Score</source>
        <translation>Wert</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Keypoints</source>
        <translation>Merkmalspunkte</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="313"/>
        <source>Match scores</source>
        <translation>Übereinstimmungswerte</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="323"/>
        <source>Driver log</source>
        <translation>Treiberprotokoll</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="358"/>
        <source>→ Put your finger on the sensor</source>
        <translation>→ Finger auf den Sensor legen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="359"/>
        <source>Image capture</source>
        <translation>Bildaufnahme</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="362"/>
        <source>→ Enrol: press and lift your finger repeatedly</source>
        <translation>→ Registrieren: Finger wiederholt auflegen und abheben</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="367"/>
        <source>→ Verify: present the enrolled finger</source>
        <translation>→ Prüfen: registrierten Finger auflegen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source>in progress…</source>
        <translation>läuft…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source> in progress…</source>
        <translation> läuft…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="412"/>
        <source>No fingerprint device was found.</source>
        <translation>Kein Fingerabdruckgerät gefunden.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="421"/>
        <source>Driver: %1
ID: %2
Scan: %3 · %4 enrol stages
Raw capture: %5</source>
        <translation>Treiber: %1
ID: %2
Scan: %3 · %4 Registrierungsschritte
Rohaufnahme: %5</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>yes</source>
        <translation>ja</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>no</source>
        <translation>nein</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="438"/>
        <source>No image was returned</source>
        <translation>Kein Bild zurückgegeben</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="442"/>
        <source>Captured %1 × %2</source>
        <translation>Aufgenommen %1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="461"/>
        <source>Match   %1 / %2</source>
        <translation>Übereinstimmung   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="462"/>
        <source>No match   %1 / %2</source>
        <translation>Keine Übereinstimmung   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="473"/>
        <source>Failed — %1</source>
        <translation>Fehlgeschlagen — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="474"/>
        <location filename="../src/mainwindow.cpp" line="623"/>
        <location filename="../src/mainwindow.cpp" line="624"/>
        <source>Failed</source>
        <translation>Fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="475"/>
        <location filename="../src/mainwindow.cpp" line="625"/>
        <source>Error: %1</source>
        <translation>Fehler: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="479"/>
        <source>Done — %1</source>
        <translation>Fertig — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484"/>
        <source>Enrolment complete</source>
        <translation>Registrierung abgeschlossen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="486"/>
        <source>Enrolment deleted</source>
        <translation>Registrierung gelöscht</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513"/>
        <source>This scan fell below the threshold and was asked for again</source>
        <translation>Dieser Scan lag unter dem Schwellwert und wurde erneut angefordert</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="528"/>
        <source>Coverage %1%  ·  sharpness %2  —  faint. Press a little harder</source>
        <translation>Abdeckung %1%  ·  Schärfe %2  —  schwach. Etwas fester drücken</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="530"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>Abdeckung %1%  ·  Schärfe %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="598"/>
        <source>Agent request</source>
        <translation>Agent-Anfrage</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>CLI request</source>
        <translation>CLI-Anfrage</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>request</source>
        <translation>Anfrage</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="615"/>
        <source>Idle</source>
        <translation>Bereit</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="616"/>
        <source>▸ %1   ·   %2s elapsed</source>
        <translation>▸ %1   ·   %2 s vergangen</translation>
    </message>
</context>
<context>
    <name>fpstudio::PrivRunner</name>
    <message>
        <location filename="../src/privrunner.cpp" line="22"/>
        <source>Could not run pkexec</source>
        <translation>pkexec konnte nicht gestartet werden</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="31"/>
        <source>The operation terminated abnormally</source>
        <translation>Der Vorgang wurde abnormal beendet</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="38"/>
        <source>Authentication was cancelled or refused</source>
        <translation>Authentifizierung abgebrochen oder verweigert</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="63"/>
        <source>Could not parse the result (exit code %1)</source>
        <translation>Ergebnis konnte nicht gelesen werden (Exit-Code %1)</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="108"/>
        <source>An operation is already running</source>
        <translation>Es läuft bereits ein Vorgang</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="112"/>
        <source>Could not resolve this program&apos;s own path</source>
        <translation>Eigener Programmpfad konnte nicht ermittelt werden</translation>
    </message>
</context>
<context>
    <name>fpstudio::SetupWizard</name>
    <message>
        <location filename="../src/setupwizard.cpp" line="67"/>
        <source>The command did not finish in time</source>
        <translation>Der Befehl wurde nicht rechtzeitig beendet</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="75"/>
        <source>Authentication was cancelled or refused</source>
        <translation>Authentifizierung abgebrochen oder verweigert</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="76"/>
        <location filename="../src/setupwizard.cpp" line="347"/>
        <location filename="../src/setupwizard.cpp" line="443"/>
        <source>Failed — %1</source>
        <translation>Fehlgeschlagen — %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="114"/>
        <source>Set up fingerprint unlock</source>
        <translation>Fingerabdruck-Entsperrung einrichten</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="145"/>
        <source>Skip</source>
        <translation>Überspringen</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="146"/>
        <source>Re-check</source>
        <translation>Erneut prüfen</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="157"/>
        <source>Fingerprint unlock is ready.</source>
        <translation>Fingerabdruck-Entsperrung ist bereit.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="159"/>
        <source>Finish</source>
        <translation>Fertigstellen</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="194"/>
        <source>Open diagnostics tool</source>
        <translation>Diagnosewerkzeug öffnen</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="197"/>
        <source>Live capture preview, the driver log, and MCP status - for a closer look once the checklist here is done.</source>
        <translation>Live-Aufnahmevorschau, Treiberprotokoll und MCP-Status - für einen genaueren Blick, sobald diese Liste erledigt ist.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="229"/>
        <source>Checking…</source>
        <translation>Wird geprüft…</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="294"/>
        <source>This step cannot be undone.</source>
        <translation>Dieser Schritt kann nicht rückgängig gemacht werden.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="310"/>
        <source>This will ask for your password.</source>
        <translation>Dies fragt nach Ihrem Passwort.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="322"/>
        <source>WRITE</source>
        <translation>WRITE</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="324"/>
        <source>Sensor key</source>
        <translation>Sensorschlüssel</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="325"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

Geben Sie %2 ein, um zu bestätigen.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="348"/>
        <source>The enrolment helper was not found</source>
        <translation>Registrierungshelfer wurde nicht gefunden</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="354"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>Registrierung läuft — Finger wiederholt auflegen und abheben</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="378"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>Finger auf den Sensor legen und dort halten</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="427"/>
        <source>in progress…</source>
        <translation>läuft…</translation>
    </message>
</context>
</TS>
