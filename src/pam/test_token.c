// SPDX-License-Identifier: LGPL-2.1-or-later
/* Test-only password verifier. Not part of any installation target. */
#define PAM_SM_AUTH
#include <security/pam_modules.h>
#include <string.h>
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    (void)flags; (void)argc; (void)argv;
    const char *token = NULL;
    pam_get_item(pamh, PAM_AUTHTOK, (const void **)&token);
    return token && !strcmp(token, "synthetic-test-password") ? PAM_SUCCESS : PAM_AUTH_ERR;
}
