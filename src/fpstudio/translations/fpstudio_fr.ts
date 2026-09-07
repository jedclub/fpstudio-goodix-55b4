<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr" sourcelanguage="en">
<context>
    <name>fpstudio</name>
    <message>
        <location filename="../src/engine.cpp" line="86"/>
        <source>Finger detected — hold still</source>
        <translation>Doigt détecté — ne bougez pas</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="91"/>
        <source>Finger lifted</source>
        <translation>Doigt retiré</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="104"/>
        <source>Only part of the sensor was touched — cover more of it, centred</source>
        <translation>Le capteur n&apos;a été touché qu&apos;en partie — couvrez-le davantage, au centre</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="109"/>
        <source>The ridges are faint — press a little harder</source>
        <translation>Les crêtes sont peu marquées — appuyez un peu plus fort</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="114"/>
        <source>Frame received</source>
        <translation>Image reçue</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="124"/>
        <location filename="../src/engine.cpp" line="134"/>
        <location filename="../src/headless.cpp" line="476"/>
        <source>Captured — now lift your finger</source>
        <translation>Capturé — levez le doigt</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="139"/>
        <source>Matching against the enrolled print…</source>
        <translation>Comparaison avec l&apos;empreinte enregistrée…</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="144"/>
        <location filename="../src/setupcheck.cpp" line="209"/>
        <source>Secure channel to the sensor established</source>
        <translation>Canal sécurisé établi avec le capteur</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="149"/>
        <source>Sensor ready</source>
        <translation>Capteur prêt</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="417"/>
        <location filename="../src/engine.cpp" line="498"/>
        <location filename="../src/engine.cpp" line="574"/>
        <source>Gave up after %1 seconds</source>
        <translation>Abandon après %1 secondes</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="460"/>
        <source>Enrol %1/%2 — lift your finger and press again</source>
        <translation>Enregistrement %1/%2 — levez le doigt puis reposez-le</translation>
    </message>
    <message>
        <location filename="../src/engine.cpp" line="79"/>
        <location filename="../src/headless.cpp" line="87"/>
        <location filename="../src/headless.cpp" line="436"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>Posez votre doigt sur le capteur et maintenez-le</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="89"/>
        <location filename="../src/headless.cpp" line="437"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>Enregistrement — posez et levez le doigt plusieurs fois</translation>
    </message>
    <message>
        <location filename="../src/headless.cpp" line="91"/>
        <location filename="../src/headless.cpp" line="438"/>
        <source>Put the enrolled finger on the sensor</source>
        <translation>Posez le doigt enregistré sur le capteur</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="64"/>
        <source>Goodix 27c6:55b4 found</source>
        <translation>Goodix 27c6:55b4 détecté</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="73"/>
        <source>A different Goodix sensor is present</source>
        <translation>Un autre capteur Goodix est présent</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="74"/>
        <source>This build only covers 27c6:55b4. Found 27c6:%1.</source>
        <translation>Cette version ne prend en charge que 27c6:55b4. Trouvé : 27c6:%1.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="80"/>
        <source>No Goodix fingerprint sensor found</source>
        <translation>Aucun capteur d&apos;empreintes Goodix trouvé</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="81"/>
        <source>Nothing on the USB bus answers to vendor 27c6. If the machine has a reader, it may be disabled in firmware settings, or it may be a different make.</source>
        <translation>Rien sur le bus USB ne répond au fabricant 27c6. Si la machine possède un lecteur, il peut être désactivé dans les paramètres du micrologiciel, ou être d&apos;une autre marque.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="100"/>
        <source>Patched driver is active</source>
        <translation>Le pilote corrigé est actif</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="107"/>
        <location filename="../src/setupcheck.cpp" line="115"/>
        <source>Patched driver is not installed</source>
        <translation>Le pilote corrigé n&apos;est pas installé</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="108"/>
        <source>libfprint does not recognise this sensor. The patched fork has to be built and installed; this copy of the program does not ship the sources to do it.</source>
        <translation>libfprint ne reconnaît pas ce capteur. Le fork corrigé doit être compilé et installé ; cette copie du programme n&apos;en fournit pas les sources.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="116"/>
        <source>libfprint does not recognise this sensor. The patched fork adds it. Building takes a few minutes and replaces the system libfprint, which is why it needs a password.

Install fprintd afterwards, not before: installing it first pulls in the stock libfprint and undoes this.</source>
        <translation>libfprint ne reconnaît pas ce capteur. Le fork corrigé ajoute la prise en charge. La compilation prend quelques minutes et remplace le libfprint du système, d&apos;où le mot de passe demandé.

Installez fprintd après, pas avant : l&apos;installer d&apos;abord ramène le libfprint officiel et annule ceci.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="121"/>
        <source>Build and install the patched libfprint</source>
        <translation>Compiler et installer le libfprint corrigé</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="151"/>
        <source>Sensor is reachable without a password</source>
        <translation>Le capteur est accessible sans mot de passe</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="157"/>
        <source>udev rule is installed but has not taken effect</source>
        <translation>La règle udev est installée mais pas encore appliquée</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="158"/>
        <source>Every sensor operation will ask for a password</source>
        <translation>Chaque opération du capteur demandera un mot de passe</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="160"/>
        <source>The rule file is in place but the device node still belongs to root. It applies on the next add event; replugging the reader or reloading udev will do it.</source>
        <translation>Le fichier de règles est en place, mais le nœud du périphérique appartient encore à root. Elle s&apos;applique au prochain évènement add ; débrancher/rebrancher le lecteur ou recharger udev suffit.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="163"/>
        <source>Without a rule, libusb cannot open the sensor except as root, so every capture goes through a password prompt.

This grants the wheel group read and write on the sensor. On a single-user laptop that is the same person who could already do it through pkexec; on a shared machine it is not. Skipping this is safe - it only means more prompts.</source>
        <translation>Sans règle, libusb ne peut ouvrir le capteur qu&apos;en tant que root, donc chaque capture passe par une demande de mot de passe.

Ceci accorde au groupe wheel la lecture et l&apos;écriture sur le capteur. Sur un portable mono-utilisateur, c&apos;est la même personne qui pourrait déjà le faire via pkexec ; sur une machine partagée, non. Ignorer cette étape est sans risque - cela signifie seulement plus de mots de passe demandés.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="173"/>
        <source>Install the udev rule and reload</source>
        <translation>Installer la règle udev et recharger</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="216"/>
        <source>The sensor answered, but the handshake did not complete</source>
        <translation>Le capteur a répondu, mais la négociation ne s&apos;est pas terminée</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="217"/>
        <source>The sensor holds a key this driver does not have. Writing the all-zero key is the next step, and it cannot be undone.</source>
        <translation>Le capteur détient une clé que ce pilote n&apos;a pas. La prochaine étape est d&apos;écrire la clé entièrement à zéro, et c&apos;est irréversible.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="223"/>
        <source>Could not talk to the sensor</source>
        <translation>Impossible de communiquer avec le capteur</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="224"/>
        <source>The handshake did not get far enough to say why. Check the driver and permission steps above first.</source>
        <translation>La négociation n&apos;est pas allée assez loin pour en dire la raison. Vérifiez d&apos;abord les étapes Pilote et Autorisations ci-dessus.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="237"/>
        <source>The sensor already holds a usable key</source>
        <translation>Le capteur possède déjà une clé utilisable</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="250"/>
        <source>The sensor needs the all-zero key written</source>
        <translation>Le capteur a besoin que la clé à zéro soit écrite</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="251"/>
        <source>This cannot be undone.

The key the sensor holds now cannot be read back - what the protocol returns is a value derived from it, not the key - so there is no backup to restore afterwards.

Windows fingerprint sign-in will stop working on this machine, permanently. If you dual-boot and use it there, stop here.</source>
        <translation>Ceci est irréversible.

La clé que le capteur détient actuellement ne peut pas être relue - ce que le protocole renvoie est une valeur qui en est dérivée, pas la clé elle-même - il n&apos;y a donc aucune sauvegarde à restaurer ensuite.

La connexion par empreinte sous Windows cessera définitivement de fonctionner sur cette machine. Si vous êtes en double amorçage et l&apos;utilisez là-bas, arrêtez-vous ici.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="259"/>
        <source>Write the all-zero PSK to the sensor</source>
        <translation>Écrire la clé PSK entièrement à zéro sur le capteur</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="289"/>
        <source>Image quality has not been measured yet</source>
        <translation>La qualité de l&apos;image n&apos;a pas encore été mesurée</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="290"/>
        <source>Press the button below, then put your finger on the sensor and hold it there for a couple of seconds.</source>
        <translation>Appuyez sur le bouton ci-dessous, puis posez votre doigt sur le capteur et maintenez-le quelques secondes.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="293"/>
        <source>Test a capture now</source>
        <translation>Tester une capture maintenant</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="297"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>Couverture %1%  ·  netteté %2</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="306"/>
        <source>Below about 24, matching tends to fail even though the driver accepts the frame. Dry fingertips and a sensor that has been busy both do this; a few minutes&apos; rest and a little moisture usually fix it.</source>
        <translation>En dessous d&apos;environ 24, la correspondance échoue généralement même si le pilote accepte l&apos;image. Des doigts secs et un capteur sollicité en sont tous deux la cause ; quelques minutes de repos et un peu d&apos;humidité règlent généralement cela.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="310"/>
        <source>Try again</source>
        <translation>Réessayer</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="323"/>
        <source>A finger is enrolled with fprintd</source>
        <translation>Un doigt est enregistré auprès de fprintd</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="329"/>
        <source>No finger is enrolled with fprintd</source>
        <translation>Aucun doigt n&apos;est enregistré auprès de fprintd</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="330"/>
        <source>This program keeps its own enrolments separately, and system authentication does not see those. Enrolling again through fprintd is what makes the fingerprint usable for unlocking.</source>
        <translation>Ce programme conserve ses propres enregistrements séparément, et l&apos;authentification système ne les voit pas. Se réenregistrer via fprintd est ce qui rend l&apos;empreinte utilisable pour le déverrouillage.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="334"/>
        <source>Enrol a finger with fprintd</source>
        <translation>Enregistrer un doigt auprès de fprintd</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="339"/>
        <source>fprintd is not answering</source>
        <translation>fprintd ne répond pas</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="340"/>
        <source>The daemon could not be reached. It is usually started on demand; installing it may be all that is missing.</source>
        <translation>Impossible de joindre le démon. Il démarre généralement à la demande ; l&apos;installer peut être tout ce qui manque.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="387"/>
        <source>polkit accepts a fingerprint</source>
        <translation>polkit accepte une empreinte</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="395"/>
        <location filename="../src/setupcheck.cpp" line="438"/>
        <source>pam_fprintd is not installed</source>
        <translation>pam_fprintd n&apos;est pas installé</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="396"/>
        <location filename="../src/setupcheck.cpp" line="439"/>
        <source>The PAM module that lets authentication use a fingerprint is missing. It usually ships with fprintd.</source>
        <translation>Le module PAM qui permet à l&apos;authentification d&apos;utiliser une empreinte est manquant. Il est généralement fourni avec fprintd.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="404"/>
        <source>polkit still asks for a password</source>
        <translation>polkit demande encore un mot de passe</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="405"/>
        <source>This adds one line to the stack polkit uses, so pkexec and the desktop&apos;s authentication dialog try the fingerprint first.

Login and sudo are deliberately left alone, so a sensor that stops working can never lock you out of the machine. The line is &apos;sufficient&apos;: if the fingerprint fails for any reason, you are asked for the password exactly as before.</source>
        <translation>Ceci ajoute une ligne à la pile utilisée par polkit, afin que pkexec et la boîte de dialogue d&apos;authentification du bureau essaient d&apos;abord l&apos;empreinte.

La connexion et sudo sont volontairement laissés intacts, afin qu&apos;un capteur qui cesse de fonctionner ne puisse jamais vous verrouiller hors de la machine. La ligne est &apos;sufficient&apos; : si l&apos;empreinte échoue pour une raison quelconque, le mot de passe vous sera demandé exactement comme avant.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="413"/>
        <source>Let polkit accept a fingerprint</source>
        <translation>Permettre à polkit d&apos;accepter une empreinte</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="430"/>
        <source>sudo accepts a fingerprint</source>
        <translation>sudo accepte une empreinte</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="451"/>
        <source>Terminal sudo still asks for a password only</source>
        <translation>Le sudo du terminal ne demande encore que le mot de passe</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="452"/>
        <source>Optional, and a step further than the polkit rule above: sudo is usually the way back in when something else on the machine is broken, so extending fingerprint auth to it is worth doing deliberately rather than by default.

The safety net is the same either way. The line added is &apos;sufficient&apos;, so a failing fingerprint falls back to the password exactly as before. sudo itself never stops working - only the fingerprint shortcut can.</source>
        <translation>Facultatif, et une étape de plus que la règle polkit ci-dessus : sudo est généralement le moyen de revenir en arrière quand autre chose sur la machine est cassé, donc étendre l&apos;authentification par empreinte à sudo mérite d&apos;être décidé délibérément plutôt que par défaut.

Le filet de sécurité est le même dans tous les cas. La ligne ajoutée est &apos;sufficient&apos;, donc une empreinte en échec revient au mot de passe exactement comme avant. sudo lui-même ne cesse jamais de fonctionner - seul le raccourci de l&apos;empreinte le peut.</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="462"/>
        <source>Let sudo accept a fingerprint</source>
        <translation>Permettre à sudo d&apos;accepter une empreinte</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="492"/>
        <source>Sensor</source>
        <translation>Capteur</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="493"/>
        <source>Driver</source>
        <translation>Pilote</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="494"/>
        <source>Permissions</source>
        <translation>Autorisations</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="495"/>
        <source>Secure channel</source>
        <translation>Canal sécurisé</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="496"/>
        <source>Sensor key</source>
        <translation>Clé du capteur</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="497"/>
        <source>Image quality</source>
        <translation>Qualité de l&apos;image</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="498"/>
        <source>Enrolment</source>
        <translation>Enregistrement</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="499"/>
        <source>Unlocking</source>
        <translation>Déverrouillage</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="500"/>
        <source>Terminal sudo (optional)</source>
        <translation>sudo du terminal (facultatif)</translation>
    </message>
    <message>
        <location filename="../src/setupcheck.cpp" line="521"/>
        <source>Not checked - no sensor</source>
        <translation>Non vérifié - aucun capteur</translation>
    </message>
</context>
<context>
    <name>fpstudio::ImageView</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="57"/>
        <source>No frame yet

Use Capture to pull an image from the sensor</source>
        <translation>Aucune image pour l&apos;instant

Utilisez Capturer pour obtenir une image du capteur</translation>
    </message>
</context>
<context>
    <name>fpstudio::LiveWindow</name>
    <message>
        <location filename="../src/livewindow.cpp" line="110"/>
        <source>Vulkan GPU is initializing — it calculates separately from the live stream</source>
        <translation>Initialisation du GPU Vulkan — calcul séparé du flux en direct</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="119"/>
        <source>%1 · %2 references ready · waiting for a stable candidate</source>
        <translation>%1 · %2 références prêtes · en attente d’un candidat stable</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="146"/>
        <source>GPU comparison error: </source>
        <translation>Erreur de comparaison GPU : </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="151"/>
        <source>GPU %1 hypotheses · %2 ms · image correlation %3 · ridge-direction agreement %4
Best: %5 / %6 — research comparison only; not an authentication decision</source>
        <translation>GPU : %1 hypothèses · %2 ms · corrélation d’image %3 · concordance de direction des crêtes %4
Meilleur : %5 / %6 — comparaison de recherche uniquement ; ce n’est pas une décision d’authentification</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="155"/>
        <source>
Alignment is ambiguous: a different position has a similar score</source>
        <translation>
L’alignement est ambigu : une autre position a un score similaire</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="156"/>
        <source>
At the search boundary: move the finger slightly toward the centre</source>
        <translation>
À la limite de recherche : déplacez légèrement le doigt vers le centre</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="159"/>
        <source>
Interior ridge support %1/%2 regions · %3</source>
        <translation>
Soutien interne des crêtes : %1/%2 régions · %3</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="161"/>
        <source>ridges are consistent (identity unverified)</source>
        <translation>crêtes cohérentes (identité non vérifiée)</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="161"/>
        <source>insufficient match evidence</source>
        <translation>preuves de correspondance insuffisantes</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="164"/>
        <source> · 0/1 contact-region overlap %1%</source>
        <translation> · recouvrement de région de contact 0/1 : %1%</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="166"/>
        <source> · 360° additional search applied</source>
        <translation> · recherche supplémentaire à 360° appliquée</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="168"/>
        <source> · full reference-bank search</source>
        <translation> · recherche dans toute la banque de références</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="168"/>
        <source> · representative-reference fast comparison</source>
        <translation> · comparaison rapide avec des références représentatives</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="184"/>
        <source>GPU comparison cannot run: </source>
        <translation>Impossible d’exécuter la comparaison GPU : </translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="245"/>
        <source>Vulkan GPU is comparing reference images, angles, and positions in parallel…</source>
        <translation>Le GPU Vulkan compare en parallèle les images de référence, les angles et les positions…</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="250"/>
        <source>GPU comparison timed out — live collection continues</source>
        <translation>Délai de comparaison GPU dépassé — la collecte en direct continue</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="322"/>
        <source>Contact %1 · image from comparison %2 (not live)
Gray: background (0) · yellow: contact (1), but weak features · green/red: comparison evidence
Cyan +: transformed reference contact centre · orange +: current probe contact centre · blue: comparison range</source>
        <translation>Contact %1 · image de la comparaison %2 (pas en direct)
Gris : arrière-plan (0) · jaune : contact (1), mais caractéristiques faibles · vert/rouge : preuves de comparaison
+ cyan : centre de contact de référence transformé · + orange : centre du contact testé · bleu : plage de comparaison</translation>
    </message>
    <message>
        <location filename="../src/livewindow.cpp" line="508"/>
        <source>[%1/%2 contacts compared · stops automatically at the target] </source>
        <translation>[%1/%2 contacts comparés · arrêt automatique à l’objectif] </translation>
    </message>
</context>
<context>
    <name>fpstudio::MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="108"/>
        <location filename="../src/mainwindow.cpp" line="355"/>
        <source>Device scan</source>
        <translation>Recherche de périphériques</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="153"/>
        <source>Setup</source>
        <translation>Configuration</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="154"/>
        <source>Set up fingerprint unlock…</source>
        <translation>Configurer le déverrouillage par empreinte…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="157"/>
        <source>Language</source>
        <translation>Langue</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="194"/>
        <source>fpstudio — fingerprint sensor diagnostics</source>
        <translation>fpstudio — diagnostic du capteur d&apos;empreintes</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="199"/>
        <source>Looking for a device…</source>
        <translation>Recherche d&apos;un périphérique…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="203"/>
        <source>Device</source>
        <translation>Périphérique</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="205"/>
        <source>Rescan</source>
        <translation>Rechercher à nouveau</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="210"/>
        <source>Capture — show image</source>
        <translation>Capturer — afficher l&apos;image</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="211"/>
        <location filename="../src/mainwindow.cpp" line="364"/>
        <source>Enrol</source>
        <translation>Enregistrer</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="212"/>
        <location filename="../src/mainwindow.cpp" line="369"/>
        <source>Verify</source>
        <translation>Vérifier</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="213"/>
        <location filename="../src/mainwindow.cpp" line="373"/>
        <source>Delete enrolment</source>
        <translation>Supprimer l&apos;enregistrement</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="218"/>
        <source>Actions</source>
        <translation>Actions</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="225"/>
        <source>Progress</source>
        <translation>Progression</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="257"/>
        <source>—</source>
        <translation>—</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="294"/>
        <source>Coverage</source>
        <translation>Couverture</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="296"/>
        <source>Sharpness</source>
        <translation>Netteté</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Sample</source>
        <translation>Échantillon</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Score</source>
        <translation>Score</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="309"/>
        <source>Keypoints</source>
        <translation>Points caractéristiques</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="313"/>
        <source>Match scores</source>
        <translation>Scores de correspondance</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="323"/>
        <source>Driver log</source>
        <translation>Journal du pilote</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="358"/>
        <source>→ Put your finger on the sensor</source>
        <translation>→ Posez votre doigt sur le capteur</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="359"/>
        <source>Image capture</source>
        <translation>Capture d&apos;image</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="362"/>
        <source>→ Enrol: press and lift your finger repeatedly</source>
        <translation>→ Enregistrer : posez et levez le doigt plusieurs fois</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="367"/>
        <source>→ Verify: present the enrolled finger</source>
        <translation>→ Vérifier : posez le doigt enregistré</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source>in progress…</source>
        <translation>en cours…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="384"/>
        <source> in progress…</source>
        <translation> en cours…</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="412"/>
        <source>No fingerprint device was found.</source>
        <translation>Aucun lecteur d&apos;empreintes trouvé.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="421"/>
        <source>Driver: %1
ID: %2
Scan: %3 · %4 enrol stages
Raw capture: %5</source>
        <translation>Pilote : %1
ID : %2
Scan : %3 · %4 étapes d&apos;enregistrement
Capture brute : %5</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>yes</source>
        <translation>oui</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="426"/>
        <source>no</source>
        <translation>non</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="438"/>
        <source>No image was returned</source>
        <translation>Aucune image renvoyée</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="442"/>
        <source>Captured %1 × %2</source>
        <translation>Capturé %1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="461"/>
        <source>Match   %1 / %2</source>
        <translation>Correspondance   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="462"/>
        <source>No match   %1 / %2</source>
        <translation>Aucune correspondance   %1 / %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="473"/>
        <source>Failed — %1</source>
        <translation>Échec — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="474"/>
        <location filename="../src/mainwindow.cpp" line="632"/>
        <location filename="../src/mainwindow.cpp" line="633"/>
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="475"/>
        <location filename="../src/mainwindow.cpp" line="634"/>
        <source>Error: %1</source>
        <translation>Erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="479"/>
        <source>Done — %1</source>
        <translation>Terminé — %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484"/>
        <source>Enrolment complete</source>
        <translation>Enregistrement terminé</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="486"/>
        <source>Enrolment deleted</source>
        <translation>Enregistrement supprimé</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513"/>
        <source>This scan fell below the threshold and was asked for again</source>
        <translation>Ce scan est passé sous le seuil et a été redemandé</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="528"/>
        <source>Coverage %1%  ·  sharpness %2  —  faint. Press a little harder</source>
        <translation>Couverture %1%  ·  netteté %2  —  faible. Appuyez un peu plus</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="530"/>
        <source>Coverage %1%  ·  sharpness %2</source>
        <translation>Couverture %1%  ·  netteté %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="551"/>
        <source>%1 × %2</source>
        <translation>%1 × %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="607"/>
        <source>Agent request</source>
        <translation>Requête de l&apos;agent</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="608"/>
        <source>CLI request</source>
        <translation>Requête CLI</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="608"/>
        <source>request</source>
        <translation>requête</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="624"/>
        <source>Idle</source>
        <translation>Inactif</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="625"/>
        <source>▸ %1   ·   %2s elapsed</source>
        <translation>▸ %1   ·   %2 s écoulées</translation>
    </message>
</context>
<context>
    <name>fpstudio::PrivRunner</name>
    <message>
        <location filename="../src/privrunner.cpp" line="22"/>
        <source>Could not run pkexec</source>
        <translation>Impossible d&apos;exécuter pkexec</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="31"/>
        <source>The operation terminated abnormally</source>
        <translation>L&apos;opération s&apos;est terminée anormalement</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="38"/>
        <source>Authentication was cancelled or refused</source>
        <translation>Authentification annulée ou refusée</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="63"/>
        <source>Could not parse the result (exit code %1)</source>
        <translation>Impossible d&apos;analyser le résultat (code de sortie %1)</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="108"/>
        <source>An operation is already running</source>
        <translation>Une opération est déjà en cours</translation>
    </message>
    <message>
        <location filename="../src/privrunner.cpp" line="112"/>
        <source>Could not resolve this program&apos;s own path</source>
        <translation>Impossible de déterminer le chemin du programme</translation>
    </message>
</context>
<context>
    <name>fpstudio::SetupWizard</name>
    <message>
        <location filename="../src/setupwizard.cpp" line="355"/>
        <source>Failed — %1</source>
        <translation>Échec — %1</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="125"/>
        <source>Skip</source>
        <translation>Ignorer</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="140"/>
        <source>Finish</source>
        <translation>Terminer</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="187"/>
        <source>Open diagnostics tool</source>
        <translation>Ouvrir l&apos;outil de diagnostic</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="190"/>
        <source>Live capture preview, the driver log, and MCP status - for a closer look once the checklist here is done.</source>
        <translation>Aperçu de capture en direct, journal du pilote et état MCP - pour un examen plus approfondi une fois cette liste terminée.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="300"/>
        <source>This step cannot be undone.</source>
        <translation>Cette étape est irréversible.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="316"/>
        <source>This will ask for your password.</source>
        <translation>Ceci demandera votre mot de passe.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="329"/>
        <source>WRITE</source>
        <translation>WRITE</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="331"/>
        <source>Sensor key</source>
        <translation>Clé du capteur</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="332"/>
        <source>%1

Type %2 to confirm.</source>
        <translation>%1

Tapez %2 pour confirmer.</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="356"/>
        <source>The enrolment helper was not found</source>
        <translation>L&apos;assistant d&apos;enregistrement est introuvable</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="362"/>
        <source>Enrolling — press and lift your finger repeatedly</source>
        <translation>Enregistrement — posez et levez le doigt plusieurs fois</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="402"/>
        <source>Put your finger on the sensor and hold it there</source>
        <translation>Posez votre doigt sur le capteur et maintenez-le</translation>
    </message>
    <message>
        <location filename="../src/setupwizard.cpp" line="472"/>
        <source>in progress…</source>
        <translation>en cours…</translation>
    </message>
</context>
</TS>
