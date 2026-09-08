import unittest
from dual_auth_install import dual_stack, MODULE
from admin_notice import LINE as OLD_NOTICE_LINE


class DualAuthInstallTest(unittest.TestCase):
    def test_obsolete_serial_notice_is_removed_without_touching_custom_notice(self):
        source = OLD_NOTICE_LINE + '\nauth include system-auth\n'
        changed = dual_stack(source, 'polkit-1')
        self.assertNotIn(OLD_NOTICE_LINE, changed)
        self.assertNotIn(OLD_NOTICE_LINE, dual_stack(OLD_NOTICE_LINE + '\n' + changed, 'polkit-1'))
        custom = 'auth optional pam_echo.so file=/etc/security/site-notice\n'
        self.assertIn(custom, dual_stack(custom + source, 'sudo'))

    def test_existing_password_and_account_policies_are_preserved(self):
        source = ('auth sufficient pam_fprintd.so max-tries=10 timeout=90\n'
                  'auth include system-auth\naccount required pam_unix.so\n'
                  'session include system-auth\n')
        changed = dual_stack(source, 'sudo')
        self.assertIn('auth requisite pam_faillock.so preauth\n', changed)
        self.assertIn(f'auth sufficient {MODULE}\nauth include system-auth\n', changed)
        self.assertEqual(source.split('account', 1)[1], changed.split('account', 1)[1])
        self.assertEqual(dual_stack(changed, 'sudo'), changed)

    def test_unknown_stacks_are_rejected(self):
        for control in ('required', '[success=2 default=ignore]'):
            source = f'auth {control} pam_fprintd.so\nauth include system-auth\n'
            with self.assertRaises(ValueError): dual_stack(source, 'sudo')
        with self.assertRaises(ValueError):
            dual_stack('auth sufficient pam_permit.so\nauth include system-auth\n', 'polkit-1')

    def test_console_keeps_shell_nologin_and_password_checks(self):
        source = 'auth requisite pam_nologin.so\nauth include system-local-login\naccount include system-local-login\n'
        changed = dual_stack(source, 'login')
        for check in ('pam_nologin.so', 'pam_shells.so', 'pam_faillock.so preauth', 'include system-local-login'):
            self.assertIn(check, changed)
        self.assertEqual(dual_stack(changed, 'login'), changed)

    def test_kde_worker_is_noninteractive(self):
        source = '-auth required pam_fprintd.so max-tries=10 timeout=90\naccount include system-local-login\n'
        changed = dual_stack(source, 'kde-fingerprint')
        self.assertIn(f'-auth required {MODULE} fingerprint-only\n', changed)
        self.assertEqual(dual_stack(changed, 'kde-fingerprint'), changed)

    def test_su_validates_submitted_password_once(self):
        source = 'auth sufficient pam_rootok.so\nauth required pam_unix.so\naccount required pam_unix.so\n'
        changed = dual_stack(source, 'su')
        self.assertTrue(changed.startswith('auth sufficient pam_rootok.so\n'))
        self.assertIn('auth required pam_unix.so try_first_pass\n', changed)


if __name__ == '__main__': unittest.main()
