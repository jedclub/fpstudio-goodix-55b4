import json
from pathlib import Path
import tempfile
import unittest
from auth_install import kde_stack, source_images


class AuthInstallTest(unittest.TestCase):
    def test_kde_preserves_controls_and_other_lines(self):
        text = "auth required pam_shells.so\n-auth required pam_fprintd.so\nauth required pam_env.so\n"
        updated = kde_stack(text)
        self.assertEqual(updated, text.replace("pam_fprintd.so", "pam_fprintd.so max-tries=10 timeout=90"))
        self.assertEqual(kde_stack(updated), updated)

    def test_kde_refuses_unknown_control(self):
        for text in ("auth sufficient pam_fprintd.so\n", "auth required pam_fprintd.so max-tries=oops\n", "auth required pam_fprintd.so\nauth required pam_fprintd.so\n"):
            with self.assertRaises(ValueError):
                kde_stack(text)

    def test_import_omits_failed_contacts_and_deduplicates(self):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder)
            for index, data in ((1, b"one"), (2, b"failed"), (3, b"one")):
                (path / f"candidate-touch-{index:02d}.png").write_bytes(b"\x89PNG\r\n\x1a\n" + data)
            (path / "live-status.json").write_text(json.dumps(dict(state="ended", compared_contacts=3,
                contacts=[dict(contact_group=i, consistent=i != 2) for i in range(1, 4)])))
            self.assertEqual(len(source_images([path])), 1)

    def test_import_refuses_symlink(self):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder)
            (path / "source.png").write_bytes(b"\x89PNG\r\n\x1a\n")
            (path / "candidate-touch-01.png").symlink_to(path / "source.png")
            with self.assertRaises(ValueError):
                source_images([path])


if __name__ == "__main__":
    unittest.main()
