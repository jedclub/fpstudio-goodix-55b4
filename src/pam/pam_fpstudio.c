// SPDX-License-Identifier: LGPL-2.1-or-later
#define _GNU_SOURCE
#define PAM_SM_AUTH
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <spawn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include "protocol.h"
#include "messages.h"
#include "terminal.h"

#ifndef FP_WORKER
#define FP_WORKER "/opt/fpstudio-auth/bin/fpstudio-fprint-worker"
#endif

/* Supported C PAM conversations run in an isolated/single-threaded process.
 * KDE's in-process lock-screen authenticator uses its stock parallel services.
 * In particular do not fork a Qt application or change its PAM conversation. */
static bool supported(const char *service)
{
#ifdef FPSTUDIO_TESTING
    (void)service;
    return true;
#else
    const char *names[] = {"sudo", "sudo-i", "polkit-1", "login", "su", "su-l", NULL};
    bool found = false;
    for (int i = 0; names[i]; ++i) if (!strcmp(service, names[i])) found = true;
    if (!found) return false;
    DIR *tasks = opendir("/proc/self/task");
    if (!tasks) return false;
    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(tasks))) if (entry->d_name[0] != '.') ++count;
    closedir(tasks);
    return count == 1;
#endif
}

static bool external_sudo_prompt(const char *service)
{
    if (strcmp(service, "sudo") && strcmp(service, "sudo-i")) return false;
    /* Explicit askpass/stdin/noninteractive modes belong to sudo's own UI.
     * A conservative false positive only retains stock password auth. */
    int fd = open("/proc/self/cmdline", O_RDONLY | O_CLOEXEC);
    if (fd < 0) return true;
    char args[32768];
    ssize_t size = read(fd, args, sizeof(args) - 1); close(fd);
    if (size <= 0) return true;
    args[size] = 0;
    bool external = false;
    for (size_t i = strlen(args) + 1; i < (size_t)size; i += strlen(args + i) + 1) {
        const char *arg = args + i;
        if (!strcmp(arg, "--")) break;
        if (!strcmp(arg, "--askpass") || !strcmp(arg, "--stdin") || !strcmp(arg, "--non-interactive") ||
            (arg[0] == '-' && arg[1] && arg[1] != '-' && strpbrk(arg + 1, "ASn"))) external = true;
    }
    explicit_bzero(args, sizeof(args));
    return external;
}

static void wipe_response(struct pam_response *response)
{
    if (!response) return;
    if (response->resp) {
        explicit_bzero(response->resp, strlen(response->resp));
        free(response->resp);
    }
    free(response);
}

static void feedback(const struct pam_conv *conv, bool terminal, const char *message)
{
    /* A password conversation has no trailing newline. Start terminal status
     * on its own line; never append sensor text to that input prompt. Do not
     * redraw the prompt or issue another password conversation. GUI clients
     * receive plain TEXT_INFO, separately from their password field. */
    char line[512];
    snprintf(line, sizeof(line), "%s%s", terminal ? "\n" : "", message);
    struct pam_message info = {PAM_TEXT_INFO, line};
    const struct pam_message *messages = &info;
    struct pam_response *response = NULL;
    conv->conv(1, &messages, &response, conv->appdata_ptr);
    wipe_response(response);
}

static int sockets(int pair[2])
{
    int raw[2];
    if (socketpair(AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, raw)) return -1;
    pair[0] = fcntl(raw[0], F_DUPFD_CLOEXEC, 10);
    pair[1] = fcntl(raw[1], F_DUPFD_CLOEXEC, 10);
    close(raw[0]); close(raw[1]);
    if (pair[0] >= 0 && pair[1] >= 0) return 0;
    if (pair[0] >= 0) close(pair[0]);
    if (pair[1] >= 0) close(pair[1]);
    return -1;
}

static void reap(pid_t pid)
{
    int status;
    if (pid > 0) while (waitpid(pid, &status, 0) < 0 && errno == EINTR) {}
}

static void stop_worker(pid_t pid, int socket)
{
    if (pid <= 0) return;
    /* Closing this private socket cancels the worker even if the caller dies.
     * The worker bounds D-Bus calls and exits on HUP. */
    shutdown(socket, SHUT_RDWR);
    for (int i = 0; i < 50; ++i) {
        pid_t waited = waitpid(pid, NULL, WNOHANG);
        if (waited == pid || (waited < 0 && errno == ECHILD)) return;
        usleep(10000);
    }
    kill(pid, SIGKILL);
    reap(pid);
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    (void)flags;
    bool fingerprint_only = argc == 1 && !strcmp(argv[0], "fingerprint-only");
    if (argc && !fingerprint_only) return PAM_IGNORE;
    const char *user = NULL, *service = NULL, *remote = NULL, *token = NULL;
    const struct pam_conv *conv = NULL;
    if (pam_get_user(pamh, &user, NULL) != PAM_SUCCESS || !user || !*user)
        return PAM_IGNORE;
    pam_get_item(pamh, PAM_SERVICE, (const void **)&service);
    pam_get_item(pamh, PAM_RHOST, (const void **)&remote);
    pam_get_item(pamh, PAM_AUTHTOK, (const void **)&token);
    if (!service || (fingerprint_only ? strcmp(service, "kde-fingerprint") != 0 : !supported(service)) ||
        (remote && *remote) || (token && *token) || external_sudo_prompt(service))
        return PAM_IGNORE;
    if (pam_get_item(pamh, PAM_CONV, (const void **)&conv) != PAM_SUCCESS || !conv || !conv->conv)
        return PAM_IGNORE;

    int sensor[2], password[2];
    if (sockets(sensor)) return PAM_IGNORE;
    if (sockets(password)) { close(sensor[0]); close(sensor[1]); return PAM_IGNORE; }
    char *worker_args[] = {FP_WORKER, (char *)user, NULL};
    char *worker_env[] = {"PATH=/usr/bin", "LANG=C.UTF-8", NULL};
    posix_spawn_file_actions_t actions;
    posix_spawn_file_actions_init(&actions);
    posix_spawn_file_actions_adddup2(&actions, sensor[1], 3);
    posix_spawn_file_actions_addopen(&actions, 0, "/dev/null", O_RDONLY, 0);
    pid_t scanner = -1;
    int spawned = posix_spawn(&scanner, FP_WORKER, &actions, NULL, worker_args, worker_env);
    posix_spawn_file_actions_destroy(&actions);
    close(sensor[1]);
    if (spawned) {
        close(sensor[0]); close(password[0]); close(password[1]);
        return PAM_IGNORE;
    }

    bool cli = !fingerprint_only && strcmp(service, "polkit-1");
#ifdef FPSTUDIO_TESTING
    /* Non-terminal test conversations represent the GUI helper. */
    cli = !fingerprint_only && isatty(STDIN_FILENO);
#endif
    int terminal = cli ? open("/dev/tty", O_RDWR | O_CLOEXEC | O_NOCTTY) : -1;
    struct termios saved = {0};
    bool restore_terminal = terminal >= 0 && tcgetattr(terminal, &saved) == 0;
    bool terminal_messages = cli && restore_terminal;
    const struct fp_messages *messages = fp_message_catalog();
    struct fp_terminal input = {.master = -1, .slave = -1};
    struct sigaction previous_signals[4];
    bool signals_installed = false;
    if (cli) {
        /* Own echo from BEFORE the first instruction until all cleanup is
         * complete. The native password callback inherits echo-off and cannot
         * restore echo-on while the parent is still handling sensor events.
         * Pipes / sudo -S / background or non-controlling terminals retain the
         * stock password path rather than having their input intercepted. */
        struct termios hidden = saved;
        bool foreground = restore_terminal && isatty(STDIN_FILENO) &&
            tcgetsid(STDIN_FILENO) == getsid(0) && tcgetpgrp(terminal) == getpgrp();
        hidden.c_lflag &= ~(ECHO | ECHONL | ICANON);
        hidden.c_oflag &= ~OPOST;
        hidden.c_cc[VMIN] = 1; hidden.c_cc[VTIME] = 0;
        if (!foreground || terminal_init(&input, terminal, &saved, messages) != 0 ||
            tcsetattr(terminal, TCSANOW, &hidden) != 0) {
            terminal_close(&input);
            if (terminal >= 0) close(terminal);
            stop_worker(scanner, sensor[0]);
            close(sensor[0]); close(password[0]); close(password[1]);
            return PAM_IGNORE;
        }
        signals_installed = terminal_signal_begin(previous_signals) == 0;
    }
    const char *prompt = messages->prompt;
    /* The input label contains no fingerprint instruction. In terminals its
     * trailing newline reserves a separate input row, even if the first
     * sensor event races the child's initial prompt. */
    if (terminal_messages)
        prompt = FP_INPUT_MARKER;
    if (!fingerprint_only && !cli)
        feedback(conv, false, messages->starting);
    pid_t owner = getpid();
    pid_t reader = fingerprint_only || (cli && !signals_installed) ? -1 : fork();
    if (reader == 0) {
        close(sensor[0]); close(password[0]);
        if (terminal >= 0) close(terminal);
        if (prctl(PR_SET_PDEATHSIG, SIGKILL) || getppid() != owner) _exit(1);
        prctl(PR_SET_DUMPABLE, 0);
        if (cli) {
            terminal_signal_restore(previous_signals);
            if (terminal_child(&input)) _exit(1);
        }
        struct pam_message request = {PAM_PROMPT_ECHO_OFF, prompt};
        const struct pam_message *messages = &request;
        struct pam_response *response = NULL;
        int status = conv->conv(1, &messages, &response, conv->appdata_ptr);
        if (status == PAM_SUCCESS && response && response->resp) {
            size_t length = strnlen(response->resp, PAM_MAX_RESP_SIZE);
            if (length < PAM_MAX_RESP_SIZE)
                send(password[1], response->resp, length + 1, MSG_NOSIGNAL);
        }
        wipe_response(response);
        close(password[1]);
        _exit(0);
    }
    close(password[1]);
    if (cli && input.slave >= 0) { close(input.slave); input.slave = -1; }
    int result = fingerprint_only ? PAM_AUTH_ERR : PAM_IGNORE;
    bool matched = false, password_done = false, pending_match = false;
    if (reader < 0 && !fingerprint_only) goto cleanup;

    bool sensor_open = true;
    while (!password_done && !matched) {
        struct pollfd fds[] = {{fingerprint_only ? -1 : password[0], POLLIN, 0},
                               {sensor_open ? sensor[0] : -1, POLLIN, 0},
                               {cli && !input.submitted ? terminal : -1, POLLIN, 0},
                               {cli ? input.master : -1, POLLIN, 0}};
        if (cli && terminal_signal) { result = PAM_AUTH_ERR; break; }
        if (poll(fds, 4, -1) < 0) {
            if (errno == EINTR) continue;
            break;
        }
        /* Keyboard activity is consumed before a simultaneous match event.
         * Native conversation output stays private, even if it echoes. */
        if (cli && fds[2].revents && terminal_read(&input) < 0) {
            result = PAM_AUTH_ERR; terminal_signal = SIGINT; break;
        }
        if (cli && fds[3].revents) terminal_proxy_output(&input);
        if (cli && terminal_forward(&input) < 0) { result = PAM_AUTH_ERR; break; }
        /* An explicitly submitted password takes priority over a simultaneous
         * scan. Never turn a wrong password into success inside this module. */
        if (fds[0].revents) {
            char secret[PAM_MAX_RESP_SIZE];
            ssize_t length = recv(password[0], secret, sizeof(secret), 0);
            password_done = true;
            if (length > 0 && secret[length - 1] == '\0' &&
                strlen(secret) == (size_t)length - 1) {
                if (secret[0])
                    result = pam_set_item(pamh, PAM_AUTHTOK, secret) == PAM_SUCCESS ? PAM_IGNORE : PAM_AUTH_ERR;
                else if (cli && pending_match) {
                    matched = true;
                    result = PAM_SUCCESS;
                } else result = PAM_AUTH_ERR;
            } else result = PAM_AUTH_ERR;
            explicit_bzero(secret, sizeof(secret));
            break;
        }
        if (fds[1].revents) {
            struct fp_message message;
            ssize_t length = recv(sensor[0], &message, sizeof(message), 0);
            if (length != sizeof(message)) {
                sensor_open = false;
                if (cli) terminal_prompt(&input, messages->no_response, false);
                else feedback(conv, false, messages->no_response);
                if (fingerprint_only) break;
            } else if (message.event == FP_MATCH) {
                if (cli) {
                    pending_match = true;
                    sensor_open = false;
                    stop_worker(scanner, sensor[0]); scanner = -1;
                    if (input.key_pressed) terminal_prompt(&input, messages->matched_pending, false);
                } else {
                    matched = true;
                    result = PAM_SUCCESS;
                }
            } else if (message.event == FP_READY) {
                if (!cli) feedback(conv, false, messages->ready);
            } else if (message.event == FP_CONTACT) {
                if (cli) terminal_prompt(&input, messages->contact, false);
                else feedback(conv, false, messages->contact);
            } else if (message.event == FP_PROGRESS) {
                static const char *frames[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
                char text[240];
                snprintf(text, sizeof(text), messages->scanning, frames[message.attempt % 10]);
                if (cli) terminal_progress(&input, text);
                else feedback(conv, false, text);
            } else if (message.event == FP_RETRY) {
                char text[240];
                snprintf(text, sizeof(text), cli ? "(%d/20)" : messages->retry, message.attempt);
                if (cli) terminal_prompt(&input, text, false);
                else feedback(conv, false, text);
            } else {
                sensor_open = false;
                const char *status = message.event == FP_TIMEOUT ? messages->ended : messages->unavailable;
                if (cli) terminal_prompt(&input, status, false);
                else feedback(conv, false, status);
                if (fingerprint_only) break;
            }
        }
        if (cli && pending_match && input.ready && !input.key_pressed) {
            /* Recheck after bounded worker cleanup: keys may have arrived
             * during Release. Never turn such a key into shell typeahead. */
            struct pollfd keyboard = {terminal, POLLIN, 0};
            int available = poll(&keyboard, 1, 0);
            if (available == 0) { matched = true; result = PAM_SUCCESS; }
            else if (available < 0 && errno != EINTR) { result = PAM_AUTH_ERR; break; }
        }
    }
cleanup:
    if (reader > 0) {
        if (!password_done) kill(reader, SIGKILL);
        reap(reader);
    }
    stop_worker(scanner, sensor[0]);
    close(sensor[0]); close(password[0]);
    if (cli) {
        if (matched && !input.submitted) dprintf(terminal, "\r\n");
        terminal_close(&input);
    } else if (matched) feedback(conv, false, messages->authenticated);
    if (restore_terminal) {
        /* Flush buffered input in the same operation that restores echo, and
         * only after worker cleanup and final messages. */
        int restored;
        do { restored = tcsetattr(terminal, TCSAFLUSH, &saved); } while (restored < 0 && errno == EINTR);
        if (restored < 0) result = PAM_AUTH_ERR;
    }
    if (terminal >= 0) close(terminal);
    if (signals_installed) {
        int interrupted = terminal_signal;
        terminal_signal_restore(previous_signals);
        if (interrupted) raise(interrupted);
    }
    return result;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    (void)pamh; (void)flags; (void)argc; (void)argv;
    return PAM_SUCCESS;
}
