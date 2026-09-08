// SPDX-License-Identifier: LGPL-2.1-or-later
#ifndef FPSTUDIO_TERMINAL_H
#define FPSTUDIO_TERMINAL_H
#include <pty.h>
#include <sys/ioctl.h>
#include <ctype.h>

#define FP_INPUT_MARKER "FPSTUDIO_INPUT_READY"
static volatile sig_atomic_t terminal_signal;
static sigset_t terminal_previous_mask;
static const int terminal_signals[] = {SIGINT, SIGTERM, SIGHUP, SIGQUIT};
static void terminal_signalled(int number) { terminal_signal = number; }
static void terminal_signal_restore(struct sigaction *previous)
{
    for (size_t i = 0; i < 4; ++i) sigaction(terminal_signals[i], &previous[i], NULL);
    sigprocmask(SIG_SETMASK, &terminal_previous_mask, NULL);
}
static int terminal_signal_begin(struct sigaction *previous)
{
    struct sigaction action = {0}; action.sa_handler = terminal_signalled;
    sigemptyset(&action.sa_mask); terminal_signal = 0;
    for (size_t i = 0; i < 4; ++i) {
        if (sigaction(terminal_signals[i], &action, &previous[i])) {
            while (i) { --i; sigaction(terminal_signals[i], &previous[i], NULL); }
            return -1;
        }
    }
    sigset_t managed;
    sigemptyset(&managed);
    for (size_t i = 0; i < 4; ++i) sigaddset(&managed, terminal_signals[i]);
    if (sigprocmask(SIG_UNBLOCK, &managed, &terminal_previous_mask)) {
        for (size_t i = 0; i < 4; ++i) sigaction(terminal_signals[i], &previous[i], NULL);
        return -1;
    }
    return 0;
}

/* Only the parent reads the real terminal. The caller's native password
 * conversation runs on a private PTY, preserving its noninteractive refusal
 * and validation contract. No PTY output is relayed to the real terminal. */
struct fp_terminal {
    int master, slave, fd;
    bool ready, key_pressed, submitted, forwarded, invalid, colour;
    unsigned escape;
    size_t length, marker_length;
    char secret[PAM_MAX_RESP_SIZE];
    char marker[sizeof(FP_INPUT_MARKER)];
    const struct fp_messages *messages;
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
    } else dprintf(t->fd, "%s", t->colour ? "\r\033[2K" : "\r\n");
    if (status && *status) dprintf(t->fd, "%s\r\n", status);
    dprintf(t->fd, "%s", terminal_choice(t->messages));
    for (size_t i = 0; i < terminal_stars(t); ++i) dprintf(t->fd, "*");
    if (t->submitted) dprintf(t->fd, "\r\n");
}

static int terminal_init(struct fp_terminal *t, int fd, const struct termios *saved,
                         const struct fp_messages *messages)
{
    memset(t, 0, sizeof(*t));
    t->master = t->slave = -1; t->fd = fd; t->messages = messages;
    const char *term = getenv("TERM");
    t->colour = term && *term && strcmp(term, "dumb") && !getenv("NO_COLOR");
    struct termios proxy = *saved;
    cfmakeraw(&proxy);
    if (openpty(&t->master, &t->slave, NULL, &proxy, NULL)) return -1;
    fcntl(t->master, F_SETFD, FD_CLOEXEC); fcntl(t->slave, F_SETFD, FD_CLOEXEC);
    fcntl(t->master, F_SETFL, O_NONBLOCK);
    return 0;
}

static int terminal_child(struct fp_terminal *t)
{
    close(t->master);
    if (setsid() < 0 || ioctl(t->slave, TIOCSCTTY, 0) < 0) return -1;
    for (int fd = 0; fd < 3; ++fd) if (dup2(t->slave, fd) < 0) return -1;
    if (t->slave > 2) close(t->slave);
    return 0;
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
    ssize_t n = read(t->fd, input, sizeof(input));
    if (n < 0) return errno == EINTR || errno == EAGAIN ? 0 : -1;
    if (!n) return -1;
    t->key_pressed = true;  // sticky, including backspace and escape sequences
    int result = 0;
    for (ssize_t i = 0; i < n && !t->submitted; ++i) {
        unsigned char c = input[i];
        if (c == 3 || c == 4 || c == 26) { result = -1; break; }
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

static int terminal_proxy_output(struct fp_terminal *t)
{
    char discard[256];
    ssize_t n = read(t->master, discard, sizeof(discard));
    if (n <= 0) return n < 0 && (errno == EAGAIN || errno == EINTR) ? 0 : -1;
    for (ssize_t i = 0; i < n && !t->ready; ++i) {
        if (discard[i] == FP_INPUT_MARKER[t->marker_length]) ++t->marker_length;
        else t->marker_length = discard[i] == FP_INPUT_MARKER[0] ? 1 : 0;
        if (t->marker_length == strlen(FP_INPUT_MARKER)) {
            t->ready = true;
            terminal_prompt(t, NULL, true);
        }
    }
    explicit_bzero(discard, sizeof(discard));
    return 0;
}

static int terminal_forward(struct fp_terminal *t)
{
    if (!t->ready || !t->submitted || t->forwarded) return 0;
    t->forwarded = true;
    if (t->invalid) return -1;
    char line[PAM_MAX_RESP_SIZE + 1];
    memcpy(line, t->secret, t->length); line[t->length] = '\n';
    ssize_t n;
    do { n = write(t->master, line, t->length + 1); } while (n < 0 && errno == EINTR);
    explicit_bzero(line, sizeof(line));
    explicit_bzero(t->secret, sizeof(t->secret));
    return n == (ssize_t)t->length + 1 ? 0 : -1;
}

static void terminal_close(struct fp_terminal *t)
{
    if (t->master >= 0) close(t->master);
    if (t->slave >= 0) close(t->slave);
    explicit_bzero(t->secret, sizeof(t->secret));
}
#endif
