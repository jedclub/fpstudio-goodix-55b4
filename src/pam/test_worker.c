// SPDX-License-Identifier: LGPL-2.1-or-later
/* Linked into a separate test executable; never installed or selected by the
 * production module. Test scenarios are not runtime authentication options. */
#include "protocol.h"
#include <sys/socket.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char **argv)
{
    if (argc != 2) return 1;
    struct fp_message message = {FP_READY, 0};
    send(3, &message, sizeof(message), MSG_NOSIGNAL);
    struct pollfd p = {3, POLLIN, 0};
    if (poll(&p, 1, 100) > 0) return 0;
    if (!strcmp(argv[1], "tty-retry-password")) {
        message.event = FP_RETRY; message.attempt = 1;
        send(3, &message, sizeof(message), MSG_NOSIGNAL);
        poll(&p, 1, 5000);
        return 0;
    }
    if (!strcmp(argv[1], "match") || !strcmp(argv[1], "repeat") || !strncmp(argv[1], "tty-match", 9) || !strcmp(argv[1], "kde-match")) message.event = FP_MATCH;
    else if (!strcmp(argv[1], "unavailable") || !strcmp(argv[1], "kde-unavailable")) message.event = FP_UNAVAILABLE;
    else if (!strcmp(argv[1], "timeout") || !strcmp(argv[1], "kde-timeout")) message.event = FP_TIMEOUT;
    else if (!strcmp(argv[1], "malformed") || !strcmp(argv[1], "kde-malformed")) { send(3, "x", 1, MSG_NOSIGNAL); return 0; }
    else { poll(&p, 1, 5000); return 0; }
    send(3, &message, sizeof(message), MSG_NOSIGNAL);
    return 0;
}
