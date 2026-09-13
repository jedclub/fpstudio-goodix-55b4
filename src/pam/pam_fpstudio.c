// SPDX-License-Identifier: LGPL-2.1-or-later
#define _GNU_SOURCE
#define PAM_SM_AUTH
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <systemd/sd-journal.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <spawn.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include "protocol.h"
#include "messages.h"
#include "sudo_args.h"
#include "terminal.h"

#ifdef FPSTUDIO_TESTING
static char fpstudio_test_audit_buffer[4096];
static size_t fpstudio_test_audit_length;
const char *fpstudio_test_audit_trace(void) { return fpstudio_test_audit_buffer; }
#endif

#ifndef FP_WORKER
#define FP_WORKER "/opt/fpstudio-auth/bin/fpstudio-fprint-worker"
#endif

/* One opaque identifier joins PAM and worker records without recording a
 * username, password, fingerprint image or template.  PID plus monotonic time
 * is unique enough for local journal correlation and has no authentication
 * value of its own. */
static void make_session_id(char output[48])
{
    struct timespec now = {0};
    clock_gettime(CLOCK_MONOTONIC, &now);
    snprintf(output, 48, "%llx-%lx-%lx", (unsigned long long)now.tv_sec,
             (unsigned long)now.tv_nsec, (unsigned long)getpid());
}

#ifndef FPSTUDIO_TESTING
static bool safe_log_token(const char *value)
{
    if (!value || !*value || strlen(value) > 48) return false;
    for (const unsigned char *p = (const unsigned char *)value; *p; ++p)
        if (!( (*p >= 'a' && *p <= 'z') || (*p >= '0' && *p <= '9') || *p == '-' )) return false;
    return true;
}
#endif

static void audit_event(pam_handle_t *pamh, const char *session,
                        const char *event, const char *detail)
{
#ifndef FPSTUDIO_TESTING
    const char *service = NULL;
    pam_get_item(pamh, PAM_SERVICE, (const void **)&service);
    if (!safe_log_token(service)) service = "unknown";
    sd_journal_send("MESSAGE=fpstudio_auth session=%s component=pam service=%s event=%s%s%s",
                    session, service, event, detail && *detail ? " " : "",
                    detail && *detail ? detail : "", "PRIORITY=5",
                    "SYSLOG_IDENTIFIER=fpstudio-auth", "FPSTUDIO_SESSION=%s", session,
                    "FPSTUDIO_EVENT=%s", event, "FPSTUDIO_COMPONENT=pam",
                    "FPSTUDIO_SERVICE=%s", service, NULL);
#else
    (void)pamh;
    size_t left = sizeof(fpstudio_test_audit_buffer) - fpstudio_test_audit_length;
    if (left > 1) {
        int written = snprintf(fpstudio_test_audit_buffer + fpstudio_test_audit_length, left,
                               "%s%s%s;", event, detail && *detail ? " " : "",
                               detail && *detail ? detail : "");
        if (written > 0) fpstudio_test_audit_length +=
            (size_t)written < left ? (size_t)written : left - 1;
    }
    if (getenv("FPSTUDIO_TEST_AUDIT"))
        dprintf(STDERR_FILENO, "fpstudio_test session=%s event=%s%s%s\n", session, event,
                detail && *detail ? " " : "", detail && *detail ? detail : "");
#endif
}

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
    bool external = fp_sudo_external_prompt_bytes(args, (size_t)size);
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

static void feedback(const struct pam_conv *conv, bool terminal, int style,
                     const char *message)
{
    /* A password conversation has no trailing newline. Start terminal status
     * on its own line; never append sensor text to that input prompt. Do not
     * redraw the prompt or issue another password conversation. GUI clients
     * receive plain TEXT_INFO, separately from their password field. */
    char line[512];
    snprintf(line, sizeof(line), "%s%s", terminal ? "\n" : "", message);
    struct pam_message info = {style, line};
    const struct pam_message *messages = &info;
    struct pam_response *response = NULL;
    conv->conv(1, &messages, &response, conv->appdata_ptr);
    wipe_response(response);
}

static void sensor_feedback(const struct pam_conv *conv, bool terminal,
                            bool fingerprint_only, const char *message)
{
    /* Plasma 6.7 forwards non-interactive PAM_TEXT_INFO as
     * `noninteractiveInfo`, but its shipped lock-screen QML only renders
     * `noninteractiveError`. Use that display channel for the dedicated KDE
     * fingerprint service. This affects presentation only; the PAM return
     * value below remains the sole authentication decision. */
    feedback(conv, terminal, fingerprint_only ? PAM_ERROR_MSG : PAM_TEXT_INFO,
             message);
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
     * Cleanup can make two bounded 400 ms D-Bus calls (VerifyStop, Release).
     * Give both calls room to finish; the former 500 ms grace could kill the
     * worker between them and leave fprintd's device claim stuck. */
    shutdown(socket, SHUT_RDWR);
    for (int i = 0; i < 150; ++i) {
        pid_t waited = waitpid(pid, NULL, WNOHANG);
        if (waited == pid || (waited < 0 && errno == ECHILD)) return;
        usleep(10000);
    }
    kill(pid, SIGKILL);
    reap(pid);
}

static void notify_input(int socket, int event, int attempt)
{
    struct fp_message message = {event, attempt};
    send(socket, &message, sizeof(message), MSG_NOSIGNAL | MSG_DONTWAIT);
}

static void stop_reader(pid_t pid, int socket, bool cli, bool completed)
{
    if (pid <= 0) return;
    if (cli) {
        notify_input(socket, FP_UI_STOP, 0);
        for (int i = 0; i < 50; ++i) {
            pid_t waited = waitpid(pid, NULL, WNOHANG);
            if (waited == pid || (waited < 0 && errno == ECHILD)) return;
            usleep(10000);
        }
        kill(pid, SIGTERM);
        for (int i = 0; i < 20; ++i) {
            pid_t waited = waitpid(pid, NULL, WNOHANG);
            if (waited == pid || (waited < 0 && errno == ECHILD)) return;
            usleep(10000);
        }
    } else if (!completed) {
        kill(pid, SIGKILL);
    }
    if (!cli || completed) reap(pid);
    else { kill(pid, SIGKILL); reap(pid); }
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    (void)flags;
    char session[48];
    make_session_id(session);
    audit_event(pamh, session, "request_started", NULL);
    bool fingerprint_only = argc == 1 && !strcmp(argv[0], "fingerprint-only");
    if (argc && !fingerprint_only) {
        audit_event(pamh, session, "request_skipped", "reason=invalid_arguments");
        return PAM_IGNORE;
    }
    const char *user = NULL, *service = NULL, *remote = NULL, *token = NULL;
    const struct pam_conv *conv = NULL;
    if (pam_get_user(pamh, &user, NULL) != PAM_SUCCESS || !user || !*user) {
        audit_event(pamh, session, "request_skipped", "reason=user_unavailable");
        return PAM_IGNORE;
    }
    pam_get_item(pamh, PAM_SERVICE, (const void **)&service);
    pam_get_item(pamh, PAM_RHOST, (const void **)&remote);
    pam_get_item(pamh, PAM_AUTHTOK, (const void **)&token);
    if (!service) {
        audit_event(pamh, session, "request_skipped", "reason=service_unavailable");
        return PAM_IGNORE;
    }
    if (fingerprint_only ? strcmp(service, "kde-fingerprint") != 0 : !supported(service)) {
        audit_event(pamh, session, "request_skipped", "reason=unsupported_service");
        return PAM_IGNORE;
    }
    if (remote && *remote) {
        audit_event(pamh, session, "request_skipped", "reason=remote_request");
        return PAM_IGNORE;
    }
    if (token && *token) {
        audit_event(pamh, session, "request_skipped", "reason=existing_password_token");
        return PAM_IGNORE;
    }
    if (external_sudo_prompt(service)) {
        audit_event(pamh, session, "request_skipped", "reason=sudo_external_prompt");
        return PAM_IGNORE;
    }
    if (pam_get_item(pamh, PAM_CONV, (const void **)&conv) != PAM_SUCCESS || !conv || !conv->conv) {
        audit_event(pamh, session, "request_skipped", "reason=conversation_unavailable");
        return PAM_IGNORE;
    }

    int sensor[2], password[2];
    if (sockets(sensor)) {
        audit_event(pamh, session, "request_failed", "stage=sensor_socket");
        if (fingerprint_only)
            sensor_feedback(conv, false, true, fp_message_catalog()->unavailable);
        return PAM_IGNORE;
    }
    if (sockets(password)) {
        audit_event(pamh, session, "request_failed", "stage=password_socket");
        if (fingerprint_only)
            sensor_feedback(conv, false, true, fp_message_catalog()->unavailable);
        close(sensor[0]); close(sensor[1]); return PAM_IGNORE;
    }
    char *worker_args[] = {FP_WORKER, (char *)user, NULL};
    char session_env[80];
    snprintf(session_env, sizeof(session_env), "FPSTUDIO_AUTH_SESSION=%s", session);
    char *worker_env[] = {"PATH=/usr/bin", "LANG=C.UTF-8", session_env, NULL};
    posix_spawn_file_actions_t actions;
    posix_spawnattr_t attributes;
    posix_spawn_file_actions_init(&actions);
    posix_spawn_file_actions_adddup2(&actions, sensor[1], 3);
    posix_spawn_file_actions_addopen(&actions, 0, "/dev/null", O_RDONLY, 0);
    posix_spawn_file_actions_addopen(&actions, 1, "/dev/null", O_WRONLY, 0);
    posix_spawn_file_actions_addopen(&actions, 2, "/dev/null", O_WRONLY, 0);
    int attr_status = posix_spawnattr_init(&attributes);
#ifdef POSIX_SPAWN_SETSID
    if (!attr_status)
        attr_status = posix_spawnattr_setflags(&attributes, POSIX_SPAWN_SETSID);
#else
    attr_status = ENOTSUP;
#endif
    pid_t scanner = -1;
    int spawned = attr_status ? attr_status :
        posix_spawn(&scanner, FP_WORKER, &actions, &attributes, worker_args, worker_env);
    if (!attr_status) posix_spawnattr_destroy(&attributes);
    posix_spawn_file_actions_destroy(&actions);
    close(sensor[1]);
    if (spawned) {
        char detail[64];
        snprintf(detail, sizeof(detail), "stage=worker_spawn code=%d", spawned);
        audit_event(pamh, session, "request_failed", detail);
        if (fingerprint_only)
            sensor_feedback(conv, false, true, fp_message_catalog()->unavailable);
        close(sensor[0]); close(password[0]); close(password[1]);
        return PAM_IGNORE;
    }
    audit_event(pamh, session, "worker_spawned", NULL);

    bool cli = !fingerprint_only && strcmp(service, "polkit-1");
#ifdef FPSTUDIO_TESTING
    /* Non-terminal test conversations represent the GUI helper. */
    cli = !fingerprint_only && isatty(STDIN_FILENO);
#endif
    int terminal = cli ? open("/dev/tty", O_RDWR | O_CLOEXEC | O_NOCTTY) : -1;
    struct termios saved = {0};
    bool terminal_valid = terminal >= 0 && tcgetattr(terminal, &saved) == 0;
    const struct fp_messages *messages = fp_message_catalog();
    if (cli) {
        /* Validate the controlling terminal without changing it. All termios,
         * reads and masking happen in the isolated input child below. */
        bool foreground = terminal_valid && isatty(STDIN_FILENO) &&
            tcgetsid(STDIN_FILENO) == getsid(0) && tcgetpgrp(terminal) == getpgrp();
        if (!foreground) {
            audit_event(pamh, session, "request_skipped", "reason=terminal_unsupported");
            if (terminal >= 0) close(terminal);
            stop_worker(scanner, sensor[0]);
            close(sensor[0]); close(password[0]); close(password[1]);
            return PAM_IGNORE;
        }
    }
    const char *prompt = messages->prompt;
    if (!cli)
        sensor_feedback(conv, false, fingerprint_only, messages->starting);
    pid_t owner = getpid();
    pid_t reader = fingerprint_only ? -1 : fork();
    if (reader == 0) {
        close(sensor[0]); close(password[0]);
        if (prctl(PR_SET_PDEATHSIG, cli ? SIGTERM : SIGKILL) || getppid() != owner) _exit(1);
        prctl(PR_SET_DUMPABLE, 0);
        if (cli) {
            int status = terminal_input_child(terminal, password[1], messages);
            close(password[1]);
            if (terminal >= 0) close(terminal);
            _exit(status);
        }
        if (terminal >= 0) close(terminal);
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
    if (terminal >= 0) { close(terminal); terminal = -1; }
    if (reader > 0) audit_event(pamh, session, "password_listener_started", NULL);
    int result = fingerprint_only ? PAM_AUTH_ERR : PAM_IGNORE;
    bool matched = false, password_done = false, pending_match = false;
    bool keyboard_started = false;
    if (reader < 0 && !fingerprint_only) {
        audit_event(pamh, session, "request_failed", "stage=password_listener");
        goto cleanup;
    }

    bool sensor_open = true;
    const char *loop_exit = "condition";
    int loop_errno = 0;
    while (!password_done && !matched) {
        struct pollfd fds[] = {{fingerprint_only ? -1 : password[0], POLLIN, 0},
                               {sensor_open ? sensor[0] : -1, POLLIN, 0}};
        if (poll(fds, 2, -1) < 0) {
            if (errno == EINTR) continue;
            loop_exit = "poll_error";
            loop_errno = errno;
            break;
        }
        /* An explicitly submitted password takes priority over a simultaneous
         * scan. The isolated CLI child also reports the first keypress so it
         * can resolve a racing match without leaking typeahead to the shell. */
        if (fds[0].revents) {
            if (cli) {
                struct fp_input_message input;
                ssize_t length = recv(password[0], &input, sizeof(input), 0);
                if (length != sizeof(input)) {
                    password_done = true;
                    result = PAM_IGNORE;
                    loop_exit = "input_child_closed";
                    audit_event(pamh, session, "password_listener_ended", NULL);
                } else if (input.event == FP_INPUT_ACTIVITY) {
                    keyboard_started = true;
                    audit_event(pamh, session, "keyboard_input_started", NULL);
                } else if (input.event == FP_INPUT_MATCH_PENDING) {
                    pending_match = true;
                    keyboard_started = true;
                    audit_event(pamh, session, "fingerprint_pending_password", NULL);
                } else if (input.event == FP_INPUT_MATCH_ACCEPTED) {
                    matched = true;
                    result = PAM_SUCCESS;
                    loop_exit = "fingerprint_accepted";
                } else if (input.event == FP_INPUT_SUBMIT) {
                    password_done = true;
                    if (input.length >= sizeof(input.secret)) result = PAM_AUTH_ERR;
                    else if (input.length) {
                        input.secret[input.length] = '\0';
                        result = pam_set_item(pamh, PAM_AUTHTOK, input.secret) == PAM_SUCCESS ?
                            PAM_IGNORE : PAM_AUTH_ERR;
                    } else if (pending_match) {
                        matched = true;
                        result = PAM_SUCCESS;
                    } else result = PAM_AUTH_ERR;
                    loop_exit = "terminal_submitted";
                    audit_event(pamh, session, result == PAM_IGNORE ? "password_selected" :
                                matched ? "fingerprint_selected" : "input_rejected", NULL);
                } else if (input.event == FP_INPUT_INVALID) {
                    password_done = true;
                    result = PAM_AUTH_ERR;
                    loop_exit = "input_invalid";
                    audit_event(pamh, session, "input_rejected", NULL);
                } else if (input.event == FP_INPUT_ABORT) {
                    password_done = true;
                    result = PAM_ABORT;
                    loop_exit = "input_cancelled";
                    char detail[160];
                    snprintf(detail, sizeof(detail),
                             "reason=%d code=%d process_group=%d foreground_group=%d",
                             input.reason, input.code, input.process_group, input.foreground_group);
                    audit_event(pamh, session, "input_cancelled", detail);
                }
                explicit_bzero(&input, sizeof(input));
            } else {
                char secret[PAM_MAX_RESP_SIZE];
                ssize_t length = recv(password[0], secret, sizeof(secret), 0);
                password_done = true;
                if (length > 0 && secret[length - 1] == '\0' &&
                    strlen(secret) == (size_t)length - 1) {
                    if (secret[0]) {
                        int token_status = pam_set_item(pamh, PAM_AUTHTOK, secret);
                        result = token_status == PAM_SUCCESS ? PAM_IGNORE : PAM_AUTH_ERR;
                        if (token_status != PAM_SUCCESS) {
                            char detail[64];
                            snprintf(detail, sizeof(detail), "stage=password_token code=%d", token_status);
                            audit_event(pamh, session, "request_failed", detail);
                        }
                    }
                    else result = PAM_AUTH_ERR;
                } else {
                    result = PAM_AUTH_ERR;
                    char detail[96];
                    snprintf(detail, sizeof(detail),
                             "stage=password_packet length=%ld terminated=%d",
                             (long)length, length > 0 && secret[length - 1] == '\0');
                    audit_event(pamh, session, "request_failed", detail);
                }
                explicit_bzero(secret, sizeof(secret));
                audit_event(pamh, session, result == PAM_IGNORE ? "password_selected" :
                            "input_rejected", NULL);
                loop_exit = "conversation_submitted";
            }
        }
        if (password_done || matched) break;
        if (fds[1].revents) {
            struct fp_message message;
            ssize_t length = recv(sensor[0], &message, sizeof(message), 0);
            if (length != sizeof(message)) {
                sensor_open = false;
                audit_event(pamh, session, "sensor_ended", "reason=worker_channel_closed");
                stop_worker(scanner, sensor[0]); scanner = -1;
                if (cli) notify_input(password[0], FP_UNAVAILABLE, 0);
                else sensor_feedback(conv, false, fingerprint_only, messages->no_response);
                if (fingerprint_only) break;
            } else if (message.event == FP_MATCH) {
                audit_event(pamh, session, "fingerprint_match", NULL);
                loop_exit = "fingerprint_match";
                if (cli) {
                    sensor_open = false;
                    stop_worker(scanner, sensor[0]); scanner = -1;
                    notify_input(password[0], FP_UI_MATCH, message.attempt);
                } else {
                    matched = true;
                    result = PAM_SUCCESS;
                }
            } else if (message.event == FP_READY) {
                audit_event(pamh, session, "sensor_ready", NULL);
                if (cli) notify_input(password[0], message.event, message.attempt);
                else sensor_feedback(conv, false, fingerprint_only, messages->ready);
            } else if (message.event == FP_CONTACT) {
                audit_event(pamh, session, "finger_contact", NULL);
                if (cli) notify_input(password[0], message.event, message.attempt);
                else sensor_feedback(conv, false, fingerprint_only, messages->contact);
            } else if (message.event == FP_PROGRESS) {
                if (cli) notify_input(password[0], message.event, message.attempt);
                else {
                    static const char *frames[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
                    char text[240];
                    snprintf(text, sizeof(text), messages->scanning, frames[message.attempt % 10]);
                    sensor_feedback(conv, false, fingerprint_only, text);
                }
            } else if (message.event == FP_RETRY) {
                char detail[64];
                snprintf(detail, sizeof(detail), "attempt=%d", message.attempt);
                audit_event(pamh, session, "fingerprint_retry", detail);
                if (cli) notify_input(password[0], message.event, message.attempt);
                else {
                    char text[240];
                    snprintf(text, sizeof(text), messages->retry, message.attempt);
                    sensor_feedback(conv, false, fingerprint_only, text);
                }
            } else {
                sensor_open = false;
                char detail[64];
                snprintf(detail, sizeof(detail), "reason=%s attempts=%d",
                         message.event == FP_TIMEOUT ? "limit" : "unavailable", message.attempt);
                audit_event(pamh, session, "sensor_ended", detail);
                stop_worker(scanner, sensor[0]); scanner = -1;
                const char *status = message.event == FP_TIMEOUT ? messages->ended : messages->unavailable;
                if (cli) notify_input(password[0], message.event, message.attempt);
                else sensor_feedback(conv, false, fingerprint_only, status);
                if (fingerprint_only) break;
            }
        }
    }
cleanup:
    {
        char detail[192];
        snprintf(detail, sizeof(detail),
                 "reason=%s result=%d errno=%d sensor_open=%d matched=%d password_done=%d keyboard_started=%d",
                 loop_exit, result, loop_errno, sensor_open, matched, password_done, keyboard_started);
        audit_event(pamh, session, "loop_finished", detail);
    }
    stop_reader(reader, password[0], cli, password_done);
    stop_worker(scanner, sensor[0]);
    close(sensor[0]); close(password[0]);
    if (!cli && matched)
        sensor_feedback(conv, false, fingerprint_only, messages->authenticated);
    if (result == PAM_ABORT) {
        audit_event(pamh, session, "request_finished", "outcome=cancelled source=input-child");
        return PAM_ABORT;
    }
    char final_detail[96];
    snprintf(final_detail, sizeof(final_detail), "outcome=%s pam_result=%d",
             matched ? "fingerprint_success" : result == PAM_IGNORE ? "password_handoff" : "failure",
             result);
    audit_event(pamh, session, "request_finished", final_detail);
    return result;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    (void)pamh; (void)flags; (void)argc; (void)argv;
    return PAM_SUCCESS;
}
