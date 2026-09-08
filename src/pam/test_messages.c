// SPDX-License-Identifier: LGPL-2.1-or-later
#define _GNU_SOURCE
#include "messages.h"
#include <stdio.h>

int main(void)
{
    unsetenv("LC_ALL"); unsetenv("LC_MESSAGES");
    const char *locales[] = {"en_US.UTF-8", "ko_KR.UTF-8", "ja_JP.UTF-8", "zh_CN.UTF-8",
        "zh_TW.UTF-8", "es_ES.UTF-8", "de_DE.UTF-8", "fr_FR.UTF-8", "ru_RU.UTF-8", "it_IT.UTF-8", "pt_PT.UTF-8"};
    for (size_t i = 0; i < sizeof(fp_catalog) / sizeof(fp_catalog[0]); ++i) {
        setenv("LANG", locales[i], 1);
        const struct fp_messages *m = fp_message_catalog();
        if (m != &fp_catalog[i]) return 1;
        const char *fields[] = {m->prompt, m->prompt_tty, m->starting, m->no_response,
            m->matched_pending, m->ready, m->retry, m->ended, m->unavailable, m->authenticated};
        for (size_t j = 0; j < sizeof(fields) / sizeof(fields[0]); ++j)
            if (!fields[j] || !*fields[j] || strlen(fields[j]) >= 240) return 2;
        const char *format = strchr(m->retry, '%');
        if (!format || format[1] != 'd' || strchr(format + 1, '%')) return 3;
    }
    setenv("LANG", "ko_KR.UTF-8", 1); setenv("LC_MESSAGES", "ja_JP.UTF-8", 1);
    if (strcmp(fp_message_catalog()->language, "ja")) return 4;
    setenv("LC_ALL", "C.UTF-8", 1);
    if (strcmp(fp_message_catalog()->language, "en")) return 5;
    const char *traditional[] = {"zh-Hant-TW", "zh_HK.UTF-8", "zh-MO"};
    for (size_t i = 0; i < 3; ++i) {
        setenv("LC_ALL", traditional[i], 1);
        if (strcmp(fp_message_catalog()->language, "zh_TW")) return 6;
    }
    setenv("LC_ALL", "unknown_LOCALE", 1);
    if (strcmp(fp_message_catalog()->language, "en")) return 7;
    puts("11 PAM languages, format safety, locale precedence and fallback passed");
    return 0;
}
