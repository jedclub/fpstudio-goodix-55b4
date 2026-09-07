import unittest
from admin_notice import add_notice, set_retry_limit, LINE


class AdminNoticeTest(unittest.TestCase):
    def test_only_notice_changes(self):
        source = "#%PAM-1.0\nauth sufficient pam_fprintd.so max-tries=20 timeout=90\nauth include system-auth\naccount include system-auth\n"
        after = add_notice(source)
        self.assertEqual(after.replace(LINE + "\n", ""), source)
        self.assertEqual(add_notice(after), after)
        self.assertIn(LINE + "\nauth sufficient pam_fprintd.so", after)

    def test_unsafe_or_ambiguous_stacks_rejected(self):
        for source in ("auth include system-auth\n",
                       "auth sufficient pam_fprintd.so\nauth sufficient pam_fprintd.so\n",
                       "auth [success=1 default=ignore] pam_test.so\nauth sufficient pam_fprintd.so\n",
                       LINE + "\nauth optional pam_test.so\nauth sufficient pam_fprintd.so\n"):
            with self.assertRaises(ValueError):
                add_notice(source)

    def test_existing_templates_idempotent(self):
        from pathlib import Path
        folder = Path(__file__).resolve().parents[1] / "src/fpstudio/pam"
        for service in ("sudo", "polkit-1"):
            text = (folder / service).read_text()
            self.assertEqual(add_notice(text), text)
            self.assertEqual(set_retry_limit(text, 10), text)

    def test_retry_limit_preserves_fallback_and_other_options(self):
        source = "# max-tries=20 historical comment\nauth\tsufficient\tpam_fprintd.so max-tries=20 timeout=90 # keep\nauth include system-auth\naccount include system-auth\n"
        expected = source.replace("pam_fprintd.so max-tries=20", "pam_fprintd.so max-tries=10")
        self.assertEqual(set_retry_limit(source, 10), expected)
        self.assertEqual(set_retry_limit(expected, 10), expected)

    def test_retry_option_inserted_when_absent(self):
        source = "auth sufficient /usr/lib/security/pam_fprintd.so timeout=90\nauth include system-auth\n"
        self.assertEqual(set_retry_limit(source, 10), source.replace("timeout=90", "timeout=90 max-tries=10"))

    def test_retry_limit_rejects_unsafe_stacks(self):
        good = "auth sufficient pam_fprintd.so max-tries=20 timeout=90\nauth include system-auth\n"
        for source in (good.replace("sufficient", "required"),
                       good.replace("include system-auth", "required pam_deny.so"),
                       good.replace("max-tries=20", "max-tries=20 max-tries=3"),
                       good.replace("max-tries=20", "max-tries=oops"),
                       good + "auth sufficient pam_permit.so\n",
                       good.replace("auth include", "auth sufficient pam_fprintd.so\nauth include"),
                       good.replace("timeout=90", "timeout=90 " + chr(92))):
            with self.assertRaises(ValueError):
                set_retry_limit(source, 10)
        for limit in (0, -1, 101):
            with self.assertRaises(ValueError):
                set_retry_limit(good, limit)


if __name__ == "__main__":
    unittest.main()
