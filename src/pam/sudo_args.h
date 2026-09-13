// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static bool fp_sudo_env_assignment(const char *arg)
{
    const char *equals = strchr(arg, '=');
    if (!equals || equals == arg) return false;
    for (const unsigned char *p = (const unsigned char *)arg; p < (const unsigned char *)equals; ++p) {
        if (p == (const unsigned char *)arg) {
            if (!( (*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') || *p == '_' )) return false;
        } else if (!( (*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') ||
                      (*p >= '0' && *p <= '9') || *p == '_' )) return false;
    }
    return true;
}

static bool fp_sudo_long_takes_argument(const char *arg)
{
    static const char *options[] = {
        "--chdir", "--close-from", "--command-timeout", "--group", "--host",
        "--prompt", "--role", "--type", "--user", "--other-user", NULL
    };
    if (strchr(arg, '=')) return false;
    for (size_t i = 0; options[i]; ++i)
        if (!strcmp(arg, options[i])) return true;
    return false;
}

/* Parse only sudo's own option prefix. Once the command name is reached, its
 * arguments are opaque: `sudo pacman -S ...` must not be mistaken for sudo's
 * `-S` password-from-stdin option. The input is /proc/self/cmdline bytes. */
static bool fp_sudo_external_prompt_bytes(const char *args, size_t size)
{
    if (!args || !size) return true;
    size_t offset = strnlen(args, size);
    if (offset == size) return true;
    offset++;
    while (offset < size) {
        const char *arg = args + offset;
        const size_t remaining = size - offset;
        const size_t length = strnlen(arg, remaining);
        if (length == remaining) return true;
        offset += length + 1;
        if (!length) continue;
        if (!strcmp(arg, "--")) break;
        if (arg[0] != '-') {
            if (fp_sudo_env_assignment(arg)) continue;
            break;
        }
        if (arg[1] == '-') {
            if (!strcmp(arg, "--askpass") || !strcmp(arg, "--stdin") ||
                !strcmp(arg, "--non-interactive")) return true;
            if (fp_sudo_long_takes_argument(arg) && offset < size) {
                size_t skipped = strnlen(args + offset, size - offset);
                if (skipped == size - offset) return true;
                offset += skipped + 1;
            }
            continue;
        }
        for (size_t i = 1; arg[i]; ++i) {
            if (arg[i] == 'A' || arg[i] == 'S' || arg[i] == 'n') return true;
            if (strchr("CDghpRrtTuU", arg[i])) {
                if (!arg[i + 1] && offset < size) {
                    size_t skipped = strnlen(args + offset, size - offset);
                    if (skipped == size - offset) return true;
                    offset += skipped + 1;
                }
                break;
            }
        }
    }
    return false;
}
