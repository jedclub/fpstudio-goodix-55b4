// SPDX-License-Identifier: LGPL-2.1-or-later
#ifndef FPSTUDIO_TERMINAL_H
#define FPSTUDIO_TERMINAL_H
#include <ctype.h>

#define FP_INPUT_MARKER "FPSTUDIO_INPUT_READY"
static volatile sig_atomic_t terminal_signal;
static volatile sig_atomic_t terminal_signal_source;
static volatile sig_atomic_t terminal_signal_sender;
static volatile sig_atomic_t terminal_signal_sender_uid;
static volatile sig_atomic_t terminal_signal_code;
static volatile sig_atomic_t terminal_abort_reason;
static volatile sig_atomic_t terminal_abort_code;
static sigset_t terminal_previous_mask;
/* These handlers are installed only in the isolated input child. The sudo PAM
 * process keeps its own job-control handlers and signal mask untouched. Kitty
 * and fish can generate a job-control SIGHUP while sudo remains valid; the
 * child explicitly ignores that signal below. */
static const int terminal_signals[] = {SIGINT, SIGTERM, SIGQUIT};
#define TERMINAL_SIGNAL_COUNT (sizeof(terminal_signals) / sizeof(terminal_signals[0]))
static void terminal_signalled(int number, siginfo_t *info, void *context)
{
    (void)context;
    terminal_signal = number;
    terminal_signal_source = 1;
    if (info) {
        terminal_signal_sender = info->si_pid;
        terminal_signal_sender_uid = info->si_uid;
        terminal_signal_code = info->si_code;
    }
}
static void terminal_signal_restore(struct sigaction *previous)
{
    for (size_t i = 0; i < TERMINAL_SIGNAL_COUNT; ++i)
        sigaction(terminal_signals[i], &previous[i], NULL);
    sigprocmask(SIG_SETMASK, &terminal_previous_mask, NULL);
}
static int terminal_signal_begin(struct sigaction *previous)
{
    struct sigaction action = {0};
    action.sa_sigaction = terminal_signalled;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);
    terminal_signal = 0; terminal_signal_source = 0;
    terminal_signal_sender = 0; terminal_signal_sender_uid = 0; terminal_signal_code = 0;
    for (size_t i = 0; i < TERMINAL_SIGNAL_COUNT; ++i) {
        if (sigaction(terminal_signals[i], &action, &previous[i])) {
            while (i) { --i; sigaction(terminal_signals[i], &previous[i], NULL); }
            return -1;
        }
    }
    sigset_t managed;
    sigemptyset(&managed);
    for (size_t i = 0; i < TERMINAL_SIGNAL_COUNT; ++i)
        sigaddset(&managed, terminal_signals[i]);
    if (sigprocmask(SIG_UNBLOCK, &managed, &terminal_previous_mask)) {
        for (size_t i = 0; i < TERMINAL_SIGNAL_COUNT; ++i)
            sigaction(terminal_signals[i], &previous[i], NULL);
        return -1;
    }
    return 0;
}

/* Only the PAM process reads the controlling terminal. GUI conversations use
 * their native callback; CLI input is masked here and handed to pam_unix as
 * PAM_AUTHTOK without ever being printed or sent to the sensor worker. */
struct fp_terminal {
    int fd;
    bool ready, key_pressed, submitted, invalid, colour;
    unsigned escape;
    size_t length;
    char secret[PAM_MAX_RESP_SIZE];
    const struct fp_messages *messages;
};

enum fp_input_event {
    FP_INPUT_ACTIVITY = 1,
    FP_INPUT_SUBMIT,
    FP_INPUT_INVALID,
    FP_INPUT_ABORT,
    FP_INPUT_MATCH_ACCEPTED,
    FP_INPUT_MATCH_PENDING
};

struct fp_input_message {
    int event;
    int reason;
    int code;
    int process_group;
    int foreground_group;
    size_t length;
    char secret[PAM_MAX_RESP_SIZE];
};

static const char *terminal_choice(const struct fp_messages *m)
{
    static const char *choices[] = {
        "Password or fingerprint: ", "패스워드 or 지문입력: ", "パスワード または 指紋: ",
        "密码或指纹: ", "密碼或指紋: ", "Contraseña o huella: ",
        "Passwort oder Fingerabdruck: ", "Mot de passe ou empreinte : ",
        "Пароль или отпечаток: ", "Password o impronta: ", "Palavra-passe ou impressão digital: "
    };
    return choices[m - fp_catalog];
}

static size_t terminal_stars(const struct fp_terminal *t)
{
    size_t n = 0;
    for (size_t i = 0; i < t->length; ++i)
        if (((unsigned char)t->secret[i] & 0xc0) != 0x80) ++n;
    return n;
}

static void terminal_prompt(struct fp_terminal *t, const char *status, bool initial)
{
    if (!t->ready || (t->submitted && !initial)) return;
    if (initial) {
        const char *end = strchr(t->messages->prompt, ']');
        int width = end ? (int)(end - t->messages->prompt + 1) : 0;
        dprintf(t->fd, "%s%.*s%s\r\n", t->colour ? "\033[31m" : "", width,
                t->messages->prompt, t->colour ? "\033[0m" : "");
        if (t->colour) dprintf(t->fd, "\r\n"); /* reserved status row */
    } else if (status && *status) {
        if (!t->colour) return; /* no scrolling status on a dumb terminal */
        dprintf(t->fd, "\r\033[1A\033[2K%s\r\n", status);
    }
    dprintf(t->fd, "%s", t->colour ? "\r\033[2K" : "\r");
    dprintf(t->fd, "%s", terminal_choice(t->messages));
    for (size_t i = 0; i < terminal_stars(t); ++i) dprintf(t->fd, "*");
    if (t->submitted) dprintf(t->fd, "\r\n");
}

/* Keep the spinner in place: a new line every quarter-second makes sudo and
 * TTY history unreadable. Do not animate a dumb terminal or after the user
 * starts typing, because the password prompt then has priority. */
static void terminal_progress(struct fp_terminal *t, const char *status)
{
    if (!t->ready || t->submitted || t->key_pressed || !t->colour) return;
    terminal_prompt(t, status, false);
}

static void terminal_finish(struct fp_terminal *t, const char *status)
{
    if (!t->ready) return;
    if (t->colour)
        dprintf(t->fd, "\r\033[2K\033[1A\r\033[2K%s\r\n", status);
    else dprintf(t->fd, "\r\n%s\r\n", status);
}

static void terminal_prepare(struct fp_terminal *t, int fd,
                             const struct fp_messages *messages)
{
    memset(t, 0, sizeof(*t));
    t->fd = fd; t->messages = messages;
    const char *term = getenv("TERM");
    t->colour = term && *term && strcmp(term, "dumb") && !getenv("NO_COLOR");
}

static void terminal_direct_init(struct fp_terminal *t, int fd,
                                 const struct fp_messages *messages)
{
    terminal_prepare(t, fd, messages);
    t->ready = true;
}

static void terminal_delete(struct fp_terminal *t)
{
    if (!t->length) return;
    --t->length;
    while (t->length && ((unsigned char)t->secret[t->length] & 0xc0) == 0x80) --t->length;
    explicit_bzero(t->secret + t->length, sizeof(t->secret) - t->length);
}

static int terminal_read(struct fp_terminal *t)
{
    unsigned char input[64];
    errno = 0;
    ssize_t n = read(t->fd, input, sizeof(input));
    if (n < 0) return errno == EINTR || errno == EAGAIN ? 0 : -1;
    if (!n) return -1;
    t->key_pressed = true;  // sticky, including backspace and escape sequences
    int result = 0;
    for (ssize_t i = 0; i < n && !t->submitted; ++i) {
        unsigned char c = input[i];
        if (c == 3 || c == 4 || c == 26) {
            terminal_signal_source = 2;
            result = -2;
            break;
        }
        if (t->escape) {
            if (t->escape == 1 && (c == '[' || c == 'O')) t->escape = 2;
            else if (t->escape == 1 || (c >= 0x40 && c <= 0x7e)) t->escape = 0;
            continue;
        }
        if (c == 27) { t->escape = 1; continue; }
        if (c == '\n' || c == '\r') {
            terminal_prompt(t, NULL, false);
            t->submitted = true;
            if (t->ready) dprintf(t->fd, "\r\n");
            break;
        }
        if (c == 127 || c == 8) terminal_delete(t);
        else if (c == 21) { explicit_bzero(t->secret, sizeof(t->secret)); t->length = 0; }
        else if (c == 23) {
            while (t->length && t->secret[t->length - 1] == ' ') terminal_delete(t);
            while (t->length && t->secret[t->length - 1] != ' ') terminal_delete(t);
        } else if (c >= 32) {
            if (t->length + 1 >= sizeof(t->secret)) t->invalid = true;
            else t->secret[t->length++] = (char)c;
        }
    }
    explicit_bzero(input, sizeof(input));
    if (result == 0 && !t->submitted) terminal_prompt(t, NULL, false);
    return result;
}

static void terminal_close(struct fp_terminal *t)
{
    explicit_bzero(t->secret, sizeof(t->secret));
}

static int terminal_input_send(int socket, int event, const struct fp_terminal *input)
{
    struct fp_input_message message = {.event = event};
    if (event == FP_INPUT_ABORT) {
        message.reason = terminal_abort_reason;
        message.code = terminal_abort_code;
        message.process_group = (int)getpgrp();
        message.foreground_group = (int)tcgetpgrp(input->fd);
    }
    if (event == FP_INPUT_SUBMIT) {
        message.length = input->length;
        memcpy(message.secret, input->secret, input->length);
    }
    ssize_t sent;
    do { sent = send(socket, &message, sizeof(message), MSG_NOSIGNAL); }
    while (sent < 0 && errno == EINTR);
    explicit_bzero(&message, sizeof(message));
    return sent == (ssize_t)sizeof(message) ? 0 : -1;
}

/* Run all termios changes, terminal reads and signal handling outside sudo's
 * PAM process. The parent exchanges only fixed-size messages with this child,
 * so a Kitty/fish job-control HUP cannot corrupt sudo's terminal state. */
static int terminal_input_child(int terminal, int socket,
                                const struct fp_messages *messages)
{
    struct termios saved;
    pid_t owner_pgrp;
    if (terminal < 0 || tcgetattr(terminal, &saved) != 0 ||
        !isatty(STDIN_FILENO) || tcgetsid(STDIN_FILENO) != getsid(0) ||
        (owner_pgrp = tcgetpgrp(terminal)) != getpgrp()) return 2;

    struct sigaction previous[TERMINAL_SIGNAL_COUNT];
    struct sigaction ignore_hup = {0}, previous_hup, previous_ttou;
    ignore_hup.sa_handler = SIG_IGN;
    sigemptyset(&ignore_hup.sa_mask);
    if (sigaction(SIGHUP, &ignore_hup, &previous_hup) != 0) return 2;
    if (sigaction(SIGTTOU, &ignore_hup, &previous_ttou) != 0) {
        sigaction(SIGHUP, &previous_hup, NULL);
        return 2;
    }
    if (terminal_signal_begin(previous) != 0) {
        sigaction(SIGTTOU, &previous_ttou, NULL);
        sigaction(SIGHUP, &previous_hup, NULL);
        return 2;
    }
    /* Interactive shells can move sudo's group while PAM is running. Give
     * the input child a stable foreground group, then hand the terminal back
     * to the original sudo group during every cleanup path. */
    if (setpgid(0, 0) != 0 || tcsetpgrp(terminal, getpgrp()) != 0) {
        terminal_signal_restore(previous);
        sigaction(SIGTTOU, &previous_ttou, NULL);
        sigaction(SIGHUP, &previous_hup, NULL);
        return 2;
    }
    struct termios hidden = saved;
    hidden.c_lflag &= ~(ECHO | ECHONL | ICANON);
    hidden.c_cc[VMIN] = 1;
    hidden.c_cc[VTIME] = 0;
    if (tcsetattr(terminal, TCSANOW, &hidden) != 0) {
        tcsetpgrp(terminal, owner_pgrp);
        terminal_signal_restore(previous);
        sigaction(SIGTTOU, &previous_ttou, NULL);
        sigaction(SIGHUP, &previous_hup, NULL);
        return 2;
    }

    struct fp_terminal input;
    terminal_direct_init(&input, terminal, messages);
    terminal_prompt(&input, NULL, true);
    bool activity_sent = false;
    bool match_pending = false;
    bool wait_for_parent = false;
    bool finished = false;
    while (!finished) {
        struct pollfd fds[] = {{terminal, POLLIN, 0}, {socket, POLLIN, 0}};
        if (terminal_signal) {
            terminal_abort_reason = 1;
            terminal_abort_code = terminal_signal;
            wait_for_parent = terminal_input_send(socket, FP_INPUT_ABORT, &input) == 0;
            break;
        }
        int ready = poll(fds, 2, -1);
        if (ready < 0) {
            if (errno == EINTR) continue;
            terminal_abort_reason = 2;
            terminal_abort_code = errno;
            wait_for_parent = terminal_input_send(socket, FP_INPUT_ABORT, &input) == 0;
            break;
        }
        /* Keyboard readiness wins over a simultaneous match command. */
        if (fds[0].revents) {
            int status = terminal_read(&input);
            if (input.key_pressed && !activity_sent && !input.submitted) {
                activity_sent = true;
                terminal_input_send(socket, FP_INPUT_ACTIVITY, &input);
            }
            if (status < 0) {
                terminal_abort_reason = 3;
                terminal_abort_code = errno;
                wait_for_parent = terminal_input_send(socket, FP_INPUT_ABORT, &input) == 0;
                break;
            }
            if (input.submitted) {
                if (!input.invalid && !input.length && match_pending)
                    terminal_finish(&input, messages->authenticated);
                wait_for_parent = terminal_input_send(
                    socket, input.invalid ? FP_INPUT_INVALID : FP_INPUT_SUBMIT, &input) == 0;
                break;
            }
        }
        if (fds[1].revents) {
            struct fp_message command;
            ssize_t length = recv(socket, &command, sizeof(command), 0);
            if (length != sizeof(command)) break;
            if (command.event == FP_UI_STOP) break;
            if (command.event == FP_UI_MATCH) {
                if (input.key_pressed) {
                    match_pending = true;
                    terminal_prompt(&input, messages->matched_pending, false);
                    terminal_input_send(socket, FP_INPUT_MATCH_PENDING, &input);
                } else {
                    terminal_finish(&input, messages->authenticated);
                    wait_for_parent = terminal_input_send(
                        socket, FP_INPUT_MATCH_ACCEPTED, &input) == 0;
                    finished = true;
                }
            } else if (command.event == FP_READY) {
                terminal_prompt(&input, messages->ready, false);
            } else if (command.event == FP_CONTACT) {
                terminal_prompt(&input, messages->contact, false);
            } else if (command.event == FP_PROGRESS) {
                static const char *frames[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
                char text[240];
                snprintf(text, sizeof(text), messages->scanning, frames[command.attempt % 10]);
                terminal_progress(&input, text);
            } else if (command.event == FP_RETRY) {
                char text[64];
                snprintf(text, sizeof(text), "(%d/20)", command.attempt);
                terminal_prompt(&input, text, false);
            } else if (command.event == FP_TIMEOUT) {
                terminal_prompt(&input, messages->ended, false);
            } else if (command.event == FP_UNAVAILABLE) {
                terminal_prompt(&input, messages->unavailable, false);
            }
        }
    }

    if (wait_for_parent) {
        struct pollfd acknowledgement = {socket, POLLIN, 0};
        if (poll(&acknowledgement, 1, 1000) > 0) {
            struct fp_message ignored;
            recv(socket, &ignored, sizeof(ignored), 0);
        }
    }

    terminal_close(&input);
    int restored;
    do { restored = tcsetattr(terminal, TCSAFLUSH, &saved); }
    while (restored < 0 && errno == EINTR);
    if (tcsetpgrp(terminal, owner_pgrp) != 0) restored = -1;
    terminal_signal_restore(previous);
    sigaction(SIGTTOU, &previous_ttou, NULL);
    sigaction(SIGHUP, &previous_hup, NULL);
    return restored == 0 ? 0 : 2;
}
#endif
