// SPDX-License-Identifier: LGPL-2.1-or-later
#define _GNU_SOURCE
#include <systemd/sd-bus.h>
#include <sys/socket.h>
#include <poll.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include "protocol.h"

#define DEST "net.reactivated.Fprint"
#define IFACE "net.reactivated.Fprint.Device"
struct state { bool matched, ended, failed; int attempts; };

static long long milliseconds(void)
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (long long)time.tv_sec * 1000 + time.tv_nsec / 1000000;
}
static bool report(int event, int attempt)
{
    struct fp_message message = {event, attempt};
    return send(3, &message, sizeof(message), MSG_NOSIGNAL) == sizeof(message);
}
static int status_received(sd_bus_message *message, void *data, sd_bus_error *error)
{
    (void)error;
    struct state *state = data;
    const char *status;
    int done;
    if (sd_bus_message_read(message, "sb", &status, &done) < 0) {
        state->failed = true;
        return 0;
    }
    if (state->matched) return 0;
    if (!strcmp(status, "verify-match") && done) state->matched = true;
    else if (!strcmp(status, "verify-no-match") ||
             !strcmp(status, "verify-retry-scan") ||
             !strcmp(status, "verify-swipe-too-short") ||
             !strcmp(status, "verify-finger-not-centered") ||
             !strcmp(status, "verify-remove-and-retry")) {
        ++state->attempts;
        report(FP_RETRY, state->attempts);
        state->ended = done;
    } else state->failed = true;
    return 0;
}
static int start_finished(sd_bus_message *message, void *data, sd_bus_error *error)
{
    (void)error;
    struct state *state = data;
    if (sd_bus_message_is_method_error(message, NULL)) state->failed = true;
    else report(FP_READY, state->attempts);
    return 0;
}

int main(int argc, char **argv)
{
    /* This binary is not setuid and accepts no device path, bus address,
     * simulation flag, password or override of the authentication result. */
    if (argc != 2 || !argv[1][0] || strlen(argv[1]) > 256) return 2;
    sd_bus *bus = NULL;
    sd_bus_message *reply = NULL;
    sd_bus_slot *slot = NULL, *start = NULL;
    char *device = NULL;
    bool claimed = false, verifying = false;
    struct state state = {0};
    long long deadline = milliseconds() + 30000;
    int event = FP_UNAVAILABLE;
    if (sd_bus_open_system(&bus) < 0) goto finish;
    /* Cold daemon activation and USB Claim can exceed 400 ms. Password input
     * runs independently; cancellation still closes our socket immediately. */
    sd_bus_set_method_call_timeout(bus, 5000000);
    if (sd_bus_call_method(bus, DEST, "/net/reactivated/Fprint/Manager",
                          "net.reactivated.Fprint.Manager", "GetDevices", NULL,
                          &reply, NULL) < 0) goto finish;
    if (sd_bus_message_enter_container(reply, 'a', "o") < 0) goto finish;
    const char *path;
    while (sd_bus_message_read(reply, "o", &path) > 0) {
        sd_bus_message *prints = NULL;
        const char *finger;
        if (sd_bus_call_method(bus, DEST, path, IFACE, "ListEnrolledFingers", NULL,
                              &prints, "s", argv[1]) >= 0 &&
            sd_bus_message_enter_container(prints, 'a', "s") >= 0 &&
            sd_bus_message_read(prints, "s", &finger) > 0) device = strdup(path);
        sd_bus_message_unref(prints);
        if (device) break;
    }
    if (!device) goto finish;
    if (sd_bus_call_method(bus, DEST, device, IFACE, "Claim", NULL, NULL, "s", argv[1]) < 0)
        goto finish;
    claimed = true;
    if (sd_bus_match_signal(bus, &slot, DEST, device, IFACE, "VerifyStatus", status_received, &state) < 0)
        goto finish;
    while (milliseconds() < deadline && state.attempts < FP_MAX_TRIES && !state.failed && !state.matched) {
        if (!verifying) {
            state.ended = false;
            sd_bus_slot_unref(start); start = NULL;
            if (sd_bus_call_method_async(bus, &start, DEST, device, IFACE, "VerifyStart",
                                        start_finished, &state, "s", "any") < 0) break;
            verifying = true;
        }
        struct pollfd fds[] = {{3, POLLIN, 0}, {sd_bus_get_fd(bus), POLLIN, 0}};
        int result = poll(fds, 2, 50);
        if (result < 0 && errno != EINTR) break;
        if (fds[0].revents) goto cancelled;
        while ((result = sd_bus_process(bus, NULL)) > 0 && !state.matched && !state.failed) {}
        if (result < 0) break;
        if (state.matched) { event = FP_MATCH; break; }
        if (state.ended) {
            sd_bus_call_method(bus, DEST, device, IFACE, "VerifyStop", NULL, NULL, NULL);
            verifying = false;
        }
    }
    if (state.matched) event = FP_MATCH;
    else if (milliseconds() >= deadline || state.attempts >= FP_MAX_TRIES) event = FP_TIMEOUT;
finish:
    report(event, state.attempts);
cancelled:
    if (bus) sd_bus_set_method_call_timeout(bus, 400000);
    if (verifying) sd_bus_call_method(bus, DEST, device, IFACE, "VerifyStop", NULL, NULL, NULL);
    if (claimed) sd_bus_call_method(bus, DEST, device, IFACE, "Release", NULL, NULL, NULL);
    sd_bus_slot_unref(start); sd_bus_slot_unref(slot); sd_bus_message_unref(reply);
    sd_bus_flush_close_unref(bus);
    free(device);
    return 0;
}
