// SPDX-License-Identifier: LGPL-2.1-or-later
#define _GNU_SOURCE
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
static int *prompts;

static void *background_thread(void *data)
{
    char done;
    while (read(*(int *)data, &done, 1) < 0 && errno == EINTR) {}
    return NULL;
}

static int conversation(int count, const struct pam_message **messages,
                        struct pam_response **result, void *data)
{
    const char *mode = data;
    if (!strncmp(mode, "tty-", 4)) {
        for (int i = 0; i < count; ++i)
            if (messages[i]->msg_style == PAM_PROMPT_ECHO_OFF) {
                ++*prompts;
                if (!strcmp(mode, "tty-noninteractive")) return PAM_CONV_ERR;
                if (!strcmp(mode, "tty-startup-password")) usleep(300000);
            }
        return misc_conv(count, messages, result, NULL);
    }
    *result = calloc((size_t)count, sizeof(**result));
    for (int i = 0; i < count; ++i) {
        if (messages[i]->msg_style != PAM_PROMPT_ECHO_OFF) continue;
        ++*prompts;
        if (strcmp(mode, "password") == 0 || strcmp(mode, "failure") == 0) usleep(20000);
        else if (!strcmp(mode, "match") || !strcmp(mode, "repeat")) sleep(4);
        else usleep(200000);
        if (!strcmp(mode, "cancel")) { free(*result); *result = NULL; return PAM_CONV_ERR; }
        (*result)[i].resp = strdup("synthetic-test-password");
    }
    return PAM_SUCCESS;
}
int main(int argc, char **argv)
{
    if (argc != 4) return 1;
    if (!strcmp(argv[2], "tty-blocked-cancel")) {
        sigset_t mask; sigemptyset(&mask); sigaddset(&mask, SIGINT);
        sigprocmask(SIG_BLOCK, &mask, NULL);
    }
    int kde = !strncmp(argv[2], "kde-", 4);
    const char *service = kde ? "kde-fingerprint" : "fpstudio-test";
    int gate[2]; pthread_t thread;
    if (kde && (pipe(gate) || pthread_create(&thread, NULL, background_thread, &gate[0]))) return 1;
    prompts = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (prompts == MAP_FAILED) return 1;
    char directory[] = "/tmp/fpstudio-pam-test-XXXXXX";
    if (!mkdtemp(directory)) return 1;
    char path[256]; snprintf(path, sizeof(path), "%s/%s", directory, service);
    FILE *config = fopen(path, "w");
    if (!config) return 1;
    /* A deny module after ours proves no password/error/timeout/no-sensor can
     * be mistaken for a fingerprint success. A second test module checks the
     * password token actually reaches the fallback, without using PAM users. */
    int reject = !strcmp(argv[2], "failure") || !strcmp(argv[2], "cancel") ||
                 !strcmp(argv[2], "tty-match-wrong-password") || !strcmp(argv[2], "tty-noninteractive") ||
                 !strcmp(argv[2], "tty-overflow") || (kde && strcmp(argv[2], "kde-match"));
    int deny = kde || !strcmp(argv[2], "failure") || !strcmp(argv[2], "cancel");
    fprintf(config, "auth sufficient %s%s\nauth required %s\n", argv[1],
            kde ? " fingerprint-only" : "", deny ? "pam_deny.so" : argv[3]);
    fclose(config);
    struct pam_conv conv = {conversation, argv[2]};
    int iterations = !strcmp(argv[2], "repeat") ? 8 : 1;
    int fail = 0;
    for (int i = 0; i < iterations; ++i) {
        pam_handle_t *handle = NULL;
        int status = pam_start_confdir(service, argv[2], &conv, directory, &handle);
        if (status == PAM_SUCCESS) status = pam_authenticate(handle, 0);
        if ((status == PAM_SUCCESS) == reject) fail = 1;
        pam_end(handle, status);
        if (waitpid(-1, NULL, WNOHANG) != -1 || errno != ECHILD) fail = 1;
    }
    unlink(path); rmdir(directory);
    if (*prompts != (kde ? 0 : iterations)) fail = 1;
    if (kde) { close(gate[1]); pthread_join(thread, NULL); close(gate[0]); }
    munmap(prompts, sizeof(int));
    if (!strcmp(argv[2], "tty-blocked-cancel")) {
        sigset_t mask; sigemptyset(&mask); sigaddset(&mask, SIGINT);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
    }
    return fail;
}
