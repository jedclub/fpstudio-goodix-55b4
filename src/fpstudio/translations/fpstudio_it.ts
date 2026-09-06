<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="it" sourcelanguage="en">
<context>
    <name>fpstudio</name>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <source>Finger detected — hold still</source>
        <translation>Dito rilevato — resta fermo</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="84"/>
        <source>Finger lifted</source>
        <translation>Dito sollevato</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="97"/>
        <source>Only part of the sensor was touched — cover more of it, centred</source>
        <translation>Toccata solo una parte del sensore — coprilo di più, al centro</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="102"/>
        <source>The ridges are faint — press a little harder</source>
        <translation>Le creste sono deboli — premi un po&apos; più forte</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="107"/>
        <source>Frame received</source>
        <translation>Frame ricevuto</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="117"/>
        <location filename="../src/engine.cpp" line="127"/>
        <source>Captured — now lift your finger</source>
        <translation>Acquisito — solleva il dito</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="132"/>
        <source>Matching against the enrolled print…</source>
        <translation>Confronto con l&apos;impronta registrata…</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="137"/>
        <location filename="../src/setupcheck.cpp" line="200"/>
        <source>Secure channel to the sensor established</source>
        <translation>Canale sicuro con il sensore stabilito</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="142"/>
        <source>Sensor ready</source>
        <translation>Sensore pronto</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="403"/>
        <location filename="../src/engine.cpp" line="484"/>
        <location filename="../src/engine.cpp" line="560"/>
        <source>Gave up after %1 seconds</source>
        <translation>Interrotto dopo %1 secondi</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="446"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>Registrazione %1/%2 — solleva il dito e riappoggialo</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="81"/>
        <location filename="../src/headless.cpp" line="373"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>Appoggia il dito sul sensore e tienilo fermo</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="83"/>
        <location filename="../src/headless.cpp" line="374"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>Registrazione in corso — appoggia e solleva il dito</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="85"/>
        <location filename="../src/headless.cpp" line="375"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>Appoggia sul sensore il dito registrato</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="58"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>Trovato Goodix 27c6:55b4</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="67"/>
        <source>A different Goodix sensor is present</source>
        <translation>È presente un sensore Goodix diverso</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="68"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>Questa build supporta solo 27c6:55b4. Trovato 27c6:%1.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>Nessun sensore di impronte Goodix trovato</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="75"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>Nessun dispositivo sul bus USB risponde al vendor 27c6. Se la macchina ha un lettore, potrebbe essere disabilitato nel firmware oppure di marca diversa.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="94"/>
        <source>Patched driver is active</source>
        <translation>Il driver con patch è attivo</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="101"/>
        <location filename="../src/setupcheck.cpp" line="109"/>
        <source>Patched driver is not installed</source>
        <translation>Il driver con patch non è installato</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="102"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint non riconosce questo sensore. Bisogna compilare e installare il fork con patch; questa copia del programma non include i sorgenti per farlo.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="110"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint non riconosce questo sensore. Il fork con patch aggiunge il supporto. La compilazione richiede pochi minuti e sostituisce il libfprint di sistema, perciò serve la password.

Installa fprintd dopo, non prima: installandolo prima si scarica il libfprint ufficiale e questo viene annullato.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="115"/>
        <source>Build and install the patched libfprint</source>
        <translation>Compila e installa il libfprint con patch</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="148"/>
        <source>Sensor is reachable without a password</source>
        <translation>Il sensore è raggiungibile senza password</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="154"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>La regola udev è installata ma non ancora attiva</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="155"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>Ogni operazione del sensore richiederà una password</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="157"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>Il file della regola è a posto, ma il nodo del dispositivo appartiene ancora a root. Si applica al prossimo evento add; basta ricollegare il lettore o ricaricare udev.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="160"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>Senza una regola, libusb può aprire il sensore solo come root, quindi ogni acquisizione richiede una password.

Questo concede al gruppo wheel lettura e scrittura sul sensore. Su un portatile monoutente è la stessa persona che potrebbe già farlo tramite pkexec; su una macchina condivisa no. Saltare questo passaggio è sicuro: significa solo più richieste di password.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="170"/>
        <source>Install the udev rule and reload</source>
        <translation>Installa la regola udev e ricarica</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="208"/>
        <source>The sensor answered, but the handshake did not complete</source>
        <translation>Il sensore ha risposto, ma l&apos;handshake non si è completato</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="209"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>Il sensore possiede una chiave che questo driver non ha. Il prossimo passo è scrivere la chiave tutta a zero, ed è irreversibile.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="215"/>
        <source>Could not talk to the sensor</source>
        <translation>Impossibile comunicare con il sensore</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="216"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>L&apos;handshake non è arrivato abbastanza lontano da spiegarne il motivo. Controlla prima i passaggi Driver e Permessi sopra.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="229"/>
        <source>The sensor already holds a usable key</source>
        <translation>Il sensore possiede già una chiave utilizzabile</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="235"/>
        <source>The sensor needs the all-zero key written</source>
        <translation>Il sensore necessita della scrittura della chiave a zero</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="236"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>Questa operazione è irreversibile.

La chiave attualmente posseduta dal sensore non può essere riletta - ciò che il protocollo restituisce è un valore derivato da essa, non la chiave stessa - quindi non c&apos;è backup da ripristinare in seguito.

L&apos;accesso con impronta di Windows smetterà di funzionare su questa macchina, permanentemente. Se usi il dual boot e lo utilizzi lì, fermati qui.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="244"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>Scrivi il PSK tutto a zero sul sensore</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="274"/>
        <source>Image quality has not been measured yet</source>
        <translation>La qualità dell&apos;immagine non è ancora stata misurata</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="275"/>
        <source>Press the button below, then put your finger on the sensor and hold it there for a couple of seconds.</source>
        <translation>Premi il pulsante qui sotto, poi appoggia il dito sul sensore e tienilo per un paio di secondi.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="278"/>
        <source>Test a capture now</source>
        <translation>Prova un&apos;acquisizione ora</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="282"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>Copertura %1%  ·  nitidezza %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="291"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>Sotto circa 24, la corrispondenza tende a fallire anche se il driver accetta il frame. Polpastrelli asciutti e un sensore sotto sforzo ne sono entrambi la causa; qualche minuto di riposo e un po&apos; di umidità di solito risolvono.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="295"/>
        <source>Try again</source>
        <translation>Riprova</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="308"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>Un dito è registrato con fprintd</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="314"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>Nessun dito è registrato con fprintd</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="315"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>Questo programma mantiene le proprie registrazioni separatamente, e l&apos;autenticazione di sistema non le vede. Registrarsi di nuovo tramite fprintd è ciò che rende l&apos;impronta utilizzabile per lo sblocco.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="319"/>
        <source>Enrol a finger with fprintd</source>
        <translation>Registra un dito con fprintd</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="324"/>
        <source>fprintd is not answering</source>
        <translation>fprintd non risponde</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="325"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>Impossibile contattare il demone. Di solito si avvia su richiesta; installarlo potrebbe essere l&apos;unica cosa mancante.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="341"/>
        <source>polkit accepts a fingerprint</source>
        <translation>polkit accetta un&apos;impronta</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="349"/>
        <location filename="../src/setupcheck.cpp" line="390"/>
        <source>pam_fprintd is not installed</source>
        <translation>pam_fprintd non è installato</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="350"/>
        <location filename="../src/setupcheck.cpp" line="391"/>
        <source>The PAM module that lets authentication use a fingerprint is missing. It usually ships with fprintd.</source>
        <translation>Manca il modulo PAM che permette all&apos;autenticazione di usare un&apos;impronta. Di solito è incluso con fprintd.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="357"/>
        <source>polkit still asks for a password</source>
        <translation>polkit richiede ancora una password</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="358"/>
        <source>This adds one line to the stack polkit uses, so pkexec and the desktop&apos;s authentication dialog try the fingerprint first.

Login and sudo are deliberately left alone, so a sensor that stops working can never lock you out of the machine. The line is &apos;sufficient&apos;: if the fingerprint fails for any reason, you are asked for the password exactly as before.</source>
        <translation>Questo aggiunge una riga allo stack usato da polkit, così pkexec e la finestra di autenticazione del desktop provano prima l&apos;impronta.

Login e sudo restano volutamente invariati, così un sensore che smette di funzionare non può mai bloccarti fuori dalla macchina. La riga è &apos;sufficient&apos;: se l&apos;impronta fallisce per qualsiasi motivo, ti verrà chiesta la password esattamente come prima.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="366"/>
        <source>Let polkit accept a fingerprint</source>
        <translation>Consenti a polkit di accettare un&apos;impronta</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="382"/>
        <source>sudo accepts a fingerprint</source>
        <translation>sudo accetta un&apos;impronta</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="402"/>
        <source>Terminal sudo still asks for a password only</source>
        <translation>Il sudo da terminale accetta ancora solo la password</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="403"/>
        <source>Optional, and a step further than the polkit rule above: sudo is usually the way back in when something else on the machine is broken, so extending fingerprint auth to it is worth doing deliberately rather than by default.

The safety net is the same either way. The line added is &apos;sufficient&apos;, so a failing fingerprint falls back to the password exactly as before. sudo itself never stops working - only the fingerprint shortcut can.</source>
        <translation>Facoltativo, e un passo oltre la regola polkit sopra: sudo è di solito la via di ritorno quando qualcos&apos;altro sulla macchina è rotto, quindi estendere l&apos;autenticazione con impronta ad esso merita di essere deciso deliberatamente, non di default.

La rete di sicurezza è la stessa in ogni caso. La riga aggiunta è &apos;sufficient&apos;, quindi un&apos;impronta fallita ripiega sulla password esattamente come prima. sudo stesso non smette mai di funzionare - solo la scorciatoia dell&apos;impronta può farlo.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="413"/>
        <source>Let sudo accept a fingerprint</source>
        <translation>Consenti a sudo di accettare un&apos;impronta</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="440"/>
        <source>Sensor</source>
        <translation>Sensore</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="441"/>
        <source>Driver</source>
        <translation>Driver</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="442"/>
        <source>Permissions</source>
        <translation>Permessi</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="443"/>
        <source>Secure channel</source>
        <translation>Canale sicuro</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="444"/>
        <source>Sensor key</source>
        <translation>Chiave del sensore</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="445"/>
        <source>Image quality</source>
        <translation>Qualità dell&apos;immagine</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="446"/>
        <source>Enrolment</source>
        <translation>Registrazione</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="447"/>
        <source>Unlocking</source>
        <translation>Sblocco</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="448"/>
        <source>Terminal sudo (optional)</source>
        <translation>sudo da terminale (opzionale)</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="467"/>
        <source>Not checked - no sensor</source>
        <translation>Non controllato - nessun sensore</translation>
    </message>
</context>
<context>
    <name>fpstudio::ImageView</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="57"/>
        <source>No frame yet

Use Capture to pull an image from the sensor</source>
        <translation>Nessun frame

Usa Acquisisci per ottenere un&apos;immagine dal sensore</translation>
    </message>
</context>
<context>
    <name>fpstudio::MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="108"/>
        <location filename="../src/mainwindow.cpp" line="355"/>
        <source>Device scan</source>
        <translation>Ricerca dispositivi</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="153"/>
        <source>Setup</source>
        <translation>Configurazione</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="154"/>
        <source>Set up fingerprint unlock…</source>
        <translation>Configura sblocco con impronta…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="157"/>
        <source>Language</source>
        <translation>Lingua</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="194"/>
        <source>fpstudio — fingerprint sensor diagnostics</source>
        <translation>fpstudio — diagnostica sensore impronte</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="199"/>
        <source>Looking for a device…</source>
        <translation>Ricerca dispositivo…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="203"/>
        <source>Device</source>
        <translation>Dispositivo</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="205"/>
        <source>Rescan</source>
        <translation>Cerca di nuovo</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="210"/>
        <source>Capture — show image</source>
        <translation>Acquisisci — mostra immagine</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="211"/>
        <location filename="../src/mainwindow.cpp" line="364"/>
        <source>Enrol</source>
        <translation>Registra</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="212"/>
        <location filename="../src/mainwindow.cpp" line="369"/>
        <source>Verify</source>
        <translation>Verifica</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="213"/>
        <location filename="../src/mainwindow.cpp" line="373"/>
        <source>Delete enrolment</source>
        <translation>Elimina registrazione</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="218"/>
        <source>Actions</source>
        <translation>Azioni</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="225"/>
        <source>Progress</source>
        <translation>Avanzamento</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="257"/>
        <source>—</source>
        <translation>—</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="294"/>
        <source>Coverage</source>
        <translation>Copertura</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="296"/>
        <source>Sharpness</source>
        <translation>Nitidezza</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Sample</source>
        <translation>Campione</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Score</source>
        <translation>Punteggio</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Keypoints</source>
        <translation>Punti chiave</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="313"/>
        <source>Match scores</source>
        <translation>Punteggi di corrispondenza</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="323"/>
        <source>Driver log</source>
        <translation>Log del driver</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="358"/>
        <source>→ Put your finger on the sensor</source>
        <translation>→ Appoggia il dito sul sensore</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="359"/>
        <source>Image capture</source>
        <translation>Acquisizione immagine</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="362"/>
        <source>→ Enrol: press and lift your finger repeatedly</source>
        <translation>→ Registra: appoggia e solleva il dito più volte</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="367"/>
        <source>→ Verify: present the enrolled finger</source>
        <translation>→ Verifica: appoggia il dito registrato</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source>in progress…</source>
        <translation>in corso…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source> in progress…</source>
        <translation> in corso…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="412"/>
        <source>No fingerprint device was found.</source>
        <translation>Nessun dispositivo per impronte trovato.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="421"/>
        <source>Driver: %1
ID: %2
Scan: %3 · %4 enrol stages
Raw capture: %5</source>
        <translation>Driver: %1
ID: %2
Scansione: %3 · %4 fasi di registrazione
Acquisizione grezza: %5</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>yes</source>
        <translation>sì</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>no</source>
        <translation>no</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="438"/>
        <source>No image was returned</source>
        <translation>Nessuna immagine restituita</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="442"/>
        <source>Captured %1 × %2</source>
        <translation>Acquisito %1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="461"/>
        <source>Match   %1 / %2</source>
        <translation>Corrispondenza   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="462"/>
        <source>No match   %1 / %2</source>
        <translation>Nessuna corrispondenza   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="473"/>
        <source>Failed — %1</source>
        <translation>Non riuscito — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="474"/>
        <location filename="../src/mainwindow.cpp" line="623"/>
        <location filename="../src/mainwindow.cpp" line="624"/>
        <source>Failed</source>
        <translation>Non riuscito</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="475"/>
        <location filename="../src/mainwindow.cpp" line="625"/>
        <source>Error: %1</source>
        <translation>Errore: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="479"/>
        <source>Done — %1</source>
        <translation>Fatto — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484"/>
        <source>Enrolment complete</source>
        <translation>Registrazione completata</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="486"/>
        <source>Enrolment deleted</source>
        <translation>Registrazione eliminata</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513"/>
        <source>This scan fell below the threshold and was asked for again</source>
        <translation>Questa scansione è sotto la soglia ed è stata richiesta di nuovo</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="528"/>
        <source>Coverage %1%  ·  sharpness %2  —  faint. Press a little harder</source>
        <translation>Copertura %1%  ·  nitidezza %2  —  debole. Premi un po&apos; più forte</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="530"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>Copertura %1%  ·  nitidezza %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="598"/>
        <source>Agent request</source>
        <translation>Richiesta dell&apos;agente</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>CLI request</source>
        <translation>Richiesta CLI</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>request</source>
        <translation>richiesta</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="615"/>
        <source>Idle</source>
        <translation>Inattivo</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="616"/>
        <source>▸ %1   ·   %2s elapsed</source>
        <translation>▸ %1   ·   %2 s trascorsi</translation>
    </message>
</context>
<context>
    <name>fpstudio::PrivRunner</name>
    <message>
        <location filename="../src/privrunner.cpp" line="22"/>
        <source>Could not run pkexec</source>
        <translation>Impossibile eseguire pkexec</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="31"/>
        <source>The operation terminated abnormally</source>
        <translation>L&apos;operazione è terminata in modo anomalo</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="38"/>
        <source>Authentication was cancelled or refused</source>
        <translation>Autenticazione annullata o rifiutata</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="63"/>
        <source>Could not parse the result (exit code %1)</source>
        <translation>Impossibile interpretare il risultato (codice di uscita %1)</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="108"/>
        <source>An operation is already running</source>
        <translation>Un&apos;operazione è già in corso</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="112"/>
        <source>Could not resolve this program&apos;s own path</source>
        <translation>Impossibile determinare il percorso del programma</translation>
    </message>
</context>
<context>
    <name>fpstudio::SetupWizard</name>
    <message>
        <location filename="../src/setupwizard.cpp" line="67"/>
        <source>The command did not finish in time</source>
        <translation>Il comando non è terminato in tempo</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="75"/>
        <source>Authentication was cancelled or refused</source>
        <translation>Autenticazione annullata o rifiutata</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="76"/>
        <location filename="../src/setupwizard.cpp" line="347"/>
        <location filename="../src/setupwizard.cpp" line="443"/>
        <source>Failed — %1</source>
        <translation>Non riuscito — %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="114"/>
        <source>Set up fingerprint unlock</source>
        <translation>Configura sblocco con impronta</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="145"/>
        <source>Skip</source>
        <translation>Salta</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="146"/>
        <source>Re-check</source>
        <translation>Ricontrolla</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="157"/>
        <source>Fingerprint unlock is ready.</source>
        <translation>Lo sblocco con impronta è pronto.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="159"/>
        <source>Finish</source>
        <translation>Fine</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="194"/>
        <source>Open diagnostics tool</source>
        <translation>Apri strumento diagnostico</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="197"/>
        <source>Live capture preview, the driver log, and MCP status - for a closer look once the checklist here is done.</source>
        <translation>Anteprima dell&apos;acquisizione dal vivo, log del driver e stato MCP - per un&apos;analisi più approfondita una volta completato questo elenco.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="229"/>
        <source>Checking…</source>
        <translation>Verifica in corso…</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="294"/>
        <source>This step cannot be undone.</source>
        <translation>Questo passaggio è irreversibile.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="310"/>
        <source>This will ask for your password.</source>
        <translation>Questo richiederà la tua password.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="322"/>
        <source>WRITE</source>
        <translation>WRITE</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="324"/>
        <source>Sensor key</source>
        <translation>Chiave del sensore</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="325"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

Digita %2 per confermare.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="348"/>
        <source>The enrolment helper was not found</source>
        <translation>Assistente di registrazione non trovato</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="354"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>Registrazione in corso — appoggia e solleva il dito</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="378"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>Appoggia il dito sul sensore e tienilo fermo</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="427"/>
        <source>in progress…</source>
        <translation>in corso…</translation>
    </message>
</context>
</TS>
