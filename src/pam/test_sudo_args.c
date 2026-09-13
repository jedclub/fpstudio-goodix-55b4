// SPDX-License-Identifier: LGPL-2.1-or-later
#include "sudo_args.h"
#include <stdio.h>

#define CHECK(bytes, expected) do { \
    const char value[] = bytes; \
    if (fp_sudo_external_prompt_bytes(value, sizeof(value) - 1) != (expected)) return 1; \
} while (0)

int main(void)
{
    CHECK("sudo\0true\0", false);
    CHECK("sudo\0pacman\0-Syu\0package\0", false);
    CHECK("sudo\0/usr/bin/true\0-S\0", false);
    CHECK("sudo\0systemctl\0--no-pager\0status\0", false);
    CHECK("sudo\0NAME=value\0pacman\0-S\0", false);
    CHECK("sudo\0-S\0true\0", true);
    CHECK("sudo\0-An\0true\0", true);
    CHECK("sudo\0--stdin\0true\0", true);
    CHECK("sudo\0--askpass\0true\0", true);
    CHECK("sudo\0-u\0root\0-S\0true\0", true);
    CHECK("sudo\0--user=root\0pacman\0-Syu\0", false);
    CHECK("sudo\0--\0-S-command\0", false);
    puts("sudo option boundary tests passed");
    return 0;
}
