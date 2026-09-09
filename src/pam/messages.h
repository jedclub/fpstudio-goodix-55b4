// SPDX-License-Identifier: LGPL-2.1-or-later
#ifndef FPSTUDIO_PAM_MESSAGES_H
#define FPSTUDIO_PAM_MESSAGES_H
#include <stdlib.h>
#include <string.h>
#include <strings.h>

struct fp_messages {
    const char *language, *prompt, *prompt_tty, *starting, *no_response;
    const char *matched_pending, *ready, *contact, *scanning, *retry, *ended, *unavailable, *authenticated;
};

/* Compiled display-only text: no user files or global locale changes in PAM.
 * Retry has exactly one %d argument in every language. */
static const struct fp_messages fp_catalog[] = {
    {"en", "[Admin Auth] Password: ", "[Admin Auth] Password (input is hidden):\n",
     "[Fingerprint] Starting sensor.", "[Fingerprint] No response. Use password.",
     "[Fingerprint] Matched. Enter to finish.", "[Fingerprint] Touch sensor.", "[Fingerprint] Detected. Scanning.", "[Fingerprint] Scanning %s",
     "[Fingerprint] Retry %d/20. Lift and touch.", "[Fingerprint] Ended. Use password.",
     "[Fingerprint] Unavailable. Use password.", "[Fingerprint] Authenticated."},
    {"ko", "[Admin 인증] 비밀번호: ", "[Admin 인증] 비밀번호 (입력 내용은 표시되지 않음):\n",
     "[지문] 준비 중", "[지문] 응답 없음 · 비밀번호 입력",
     "[지문] 일치 · Enter로 완료", "[지문] 손가락을 대세요", "[지문] 감지됨 · 스캔 중", "[지문] 스캔 중 %s",
     "[지문] 떼고 다시 대세요 (%d/20)", "[지문] 대기 종료 · 비밀번호 입력",
     "[지문] 사용 불가 · 비밀번호 입력", "[지문] 인증 완료"},
    {"ja", "[管理者認証] パスワード: ", "[管理者認証] パスワード（入力は非表示）:\n",
     "[指紋] 準備中", "[指紋] 応答なし・パスワードを入力",
     "[指紋] 一致・Enterで完了", "[指紋] 指を置いてください", "[指紋] 検出・スキャン中", "[指紋] スキャン中 %s",
     "[指紋] 離して再度置いてください（%d/20）", "[指紋] 待機終了・パスワードを入力",
     "[指紋] 利用不可・パスワードを入力", "[指紋] 認証完了"},
    {"zh_CN", "[管理员认证] 密码: ", "[管理员认证] 密码（输入不可见）:\n",
     "[指纹] 准备中", "[指纹] 无响应，请输入密码",
     "[指纹] 匹配，按 Enter 完成", "[指纹] 请放上手指", "[指纹] 已检测到，正在扫描", "[指纹] 扫描中 %s",
     "[指纹] 请抬起后重试（%d/20）", "[指纹] 等待结束，请输入密码",
     "[指纹] 不可用，请输入密码", "[指纹] 认证完成"},
    {"zh_TW", "[管理員驗證] 密碼: ", "[管理員驗證] 密碼（輸入不顯示）:\n",
     "[指紋] 準備中", "[指紋] 無回應，請輸入密碼",
     "[指紋] 相符，按 Enter 完成", "[指紋] 請放上手指", "[指紋] 已偵測到，正在掃描", "[指紋] 掃描中 %s",
     "[指紋] 請抬起後重試（%d/20）", "[指紋] 等待結束，請輸入密碼",
     "[指紋] 無法使用，請輸入密碼", "[指紋] 驗證完成"},
    {"es", "[Aut. admin] Contraseña: ", "[Aut. admin] Contraseña (entrada oculta):\n",
     "[Huella] Preparando sensor.", "[Huella] Sin respuesta. Use la contraseña.",
     "[Huella] Coincide. Pulse Enter.", "[Huella] Ponga el dedo.", "[Huella] Detectada. Escaneando.", "[Huella] Escaneando %s",
     "[Huella] Reintento %d/20. Levante y apoye el dedo.", "[Huella] Finalizado. Use la contraseña.",
     "[Huella] No disponible. Use la contraseña.", "[Huella] Autenticación completada."},
    {"de", "[Admin-Anmeldung] Passwort: ", "[Admin-Anmeldung] Passwort (Eingabe verborgen):\n",
     "[Fingerabdruck] Sensor startet.", "[Fingerabdruck] Keine Antwort. Passwort verwenden.",
     "[Fingerabdruck] Erkannt. Enter drücken.", "[Fingerabdruck] Finger auflegen.", "[Fingerabdruck] Erfasst. Scannen läuft.", "[Fingerabdruck] Scannen %s",
     "[Fingerabdruck] Versuch %d/20. Finger abheben und auflegen.", "[Fingerabdruck] Beendet. Passwort verwenden.",
     "[Fingerabdruck] Nicht verfügbar. Passwort verwenden.", "[Fingerabdruck] Authentifiziert."},
    {"fr", "[Auth. admin] Mot de passe : ", "[Auth. admin] Mot de passe (saisie masquée) :\n",
     "[Empreinte] Démarrage du capteur.", "[Empreinte] Sans réponse. Saisissez le mot de passe.",
     "[Empreinte] Reconnue. Appuyez sur Entrée.", "[Empreinte] Posez le doigt.", "[Empreinte] Détectée. Numérisation en cours.", "[Empreinte] Numérisation %s",
     "[Empreinte] Essai %d/20. Levez puis reposez le doigt.", "[Empreinte] Terminé. Saisissez le mot de passe.",
     "[Empreinte] Indisponible. Saisissez le mot de passe.", "[Empreinte] Authentification réussie."},
    {"ru", "[Администратор] Пароль: ", "[Администратор] Пароль (ввод скрыт):\n",
     "[Отпечаток] Запуск датчика.", "[Отпечаток] Нет ответа. Введите пароль.",
     "[Отпечаток] Совпадение. Нажмите Enter.", "[Отпечаток] Приложите палец.", "[Отпечаток] Обнаружен. Сканирование.", "[Отпечаток] Сканирование %s",
     "[Отпечаток] Попытка %d/20. Уберите и приложите палец.", "[Отпечаток] Ожидание завершено. Введите пароль.",
     "[Отпечаток] Недоступен. Введите пароль.", "[Отпечаток] Вход подтверждён."},
    {"it", "[Aut. admin] Password: ", "[Aut. admin] Password (digitazione nascosta):\n",
     "[Impronta] Avvio del sensore.", "[Impronta] Nessuna risposta. Inserire la password.",
     "[Impronta] Corrisponde. Premere Invio.", "[Impronta] Appoggiare il dito.", "[Impronta] Rilevata. Scansione in corso.", "[Impronta] Scansione %s",
     "[Impronta] Tentativo %d/20. Sollevare e riappoggiare il dito.", "[Impronta] Attesa terminata. Inserire la password.",
     "[Impronta] Non disponibile. Inserire la password.", "[Impronta] Autenticazione completata."},
    {"pt", "[Aut. admin] Palavra-passe: ", "[Aut. admin] Palavra-passe (entrada oculta):\n",
     "[Impressão digital] A iniciar sensor.", "[Impressão digital] Sem resposta. Introduza a palavra-passe.",
     "[Impressão digital] Corresponde. Prima Enter.", "[Impressão digital] Coloque o dedo.", "[Impressão digital] Detetada. A digitalizar.", "[Impressão digital] A digitalizar %s",
     "[Impressão digital] Tentativa %d/20. Levante e recoloque o dedo.", "[Impressão digital] Terminado. Introduza a palavra-passe.",
     "[Impressão digital] Indisponível. Introduza a palavra-passe.", "[Impressão digital] Autenticação concluída."}
};

static const struct fp_messages *fp_message_catalog(void)
{
    const char *locale = getenv("LC_ALL");
    if (!locale || !*locale) locale = getenv("LC_MESSAGES");
    if (!locale || !*locale) locale = getenv("LANG");
    if (!locale || strnlen(locale, 128) >= 128) return &fp_catalog[0];
    const char *tag = locale;
    if (!strncasecmp(locale, "zh", 2) &&
        (!locale[2] || strchr("_.-@", locale[2]))) {
        tag = (!strncasecmp(locale + 2, "_TW", 3) || !strncasecmp(locale + 2, "-TW", 3) ||
               !strncasecmp(locale + 2, "_HK", 3) || !strncasecmp(locale + 2, "-HK", 3) ||
               !strncasecmp(locale + 2, "_MO", 3) || !strncasecmp(locale + 2, "-MO", 3) ||
               !strncasecmp(locale + 2, "-Hant", 5)) ? "zh_TW" : "zh_CN";
    }
    for (size_t i = 0; i < sizeof(fp_catalog) / sizeof(fp_catalog[0]); ++i) {
        size_t length = strlen(fp_catalog[i].language);
        if (!strncasecmp(tag, fp_catalog[i].language, length) &&
            (!tag[length] || strchr("_.-@", tag[length]))) return &fp_catalog[i];
    }
    return &fp_catalog[0];
}
#endif
