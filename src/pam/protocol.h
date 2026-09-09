// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
/* Private socket between one PAM invocation and its own sensor worker.
 * No password ever travels to the sensor worker. */
enum fp_event { FP_READY = 1, FP_CONTACT, FP_PROGRESS, FP_RETRY, FP_MATCH, FP_UNAVAILABLE, FP_TIMEOUT };
struct fp_message { int event; int attempt; };
#define FP_MAX_TRIES 20
