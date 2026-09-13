import contextlib
import io
import unittest

from auth_log_report import correlate, parse_record, print_human, summarise


def record(at, message):
    return {"__REALTIME_TIMESTAMP": str(at), "MESSAGE": message}


class AuthLogReportTest(unittest.TestCase):
    def report(self, messages):
        events = [parse_record(record(at, text)) for at, text in messages]
        sessions, unscoped = correlate([event for event in events if event])
        return summarise(sessions, unscoped)

    def test_correlates_pam_worker_quality_and_gpu_without_sensitive_text(self):
        report = self.report([
            (1_000_000, "fpstudio_auth session=abc-10 component=pam service=sudo event=request_started"),
            (1_100_000, "fpstudio_auth session=abc-10 component=pam event=worker_spawned"),
            (1_200_000, "fpstudio_auth session=abc-10 component=worker event=bus_connected"),
            (1_300_000, "fpstudio_auth session=abc-10 component=worker event=enrollment_found"),
            (1_400_000, "fpstudio_auth session=abc-10 component=worker event=device_claimed"),
            (1_500_000, "fpstudio_auth session=abc-10 component=pam event=finger_contact"),
            (1_600_000, "Image quality: coverage 76%, sharpness 39"),
            (1_700_000, "fpstudio_auth component=matcher event=gpu_metrics accepted=1 ncc=.9 ms=4.2"),
            (1_800_000, "fpstudio_auth component=matcher event=gpu_result result=1"),
            (1_900_000, "fpstudio_auth session=abc-10 component=pam event=fingerprint_match"),
            (2_000_000, "fpstudio_auth session=abc-10 component=pam event=request_finished outcome=fingerprint_success pam_result=0"),
        ])
        session = report["sessions"][0]
        self.assertEqual(session["service"], "sudo")
        self.assertEqual(session["outcome"], "fingerprint_success")
        self.assertEqual(session["gpu_results"], ["1"])
        self.assertEqual(session["gpu_metrics"][0]["accepted"], 1)
        self.assertEqual(session["gpu_metrics"][0]["ncc"], .9)
        self.assertEqual(session["qualities"], [{"coverage": 76, "sharpness": 39}])
        self.assertFalse(session["findings"])
        self.assertEqual(report["finding_counts"], {})
        self.assertNotIn("jedclub", str(report))

    def test_diagnoses_skip_gpu_reset_driver_error_and_incomplete_session(self):
        report = self.report([
            (1_000_000, "fpstudio_auth session=def-20 component=pam event=request_started"),
            (1_100_000, "fpstudio_auth session=def-20 component=pam event=request_skipped reason=terminal_unsupported"),
            (3_000_000, "fpstudio_auth session=fed-21 component=pam event=request_started"),
            (3_100_000, "fpstudio_auth session=fed-21 component=pam event=worker_spawned"),
            (3_200_000, "fpstudio_auth session=fed-21 component=worker event=bus_connected"),
            (3_300_000, "fpstudio_auth session=fed-21 component=worker event=enrollment_found"),
            (3_400_000, "fpstudio_auth session=fed-21 component=worker event=device_claimed"),
            (3_500_000, "fpstudio_auth session=fed-21 component=pam event=finger_contact"),
            (3_600_000, "fpstudio_auth component=matcher event=gpu_reinitialise reason=helper_failure"),
            (3_700_000, "failed to scan: Device 27c6:55b4 has not been opened (code: 5)"),
        ])
        self.assertIn("module_skipped:terminal_unsupported", report["sessions"][0]["findings"])
        findings = report["sessions"][1]["findings"]
        self.assertIn("gpu_helper_was_reinitialised", findings)
        self.assertIn("driver_error:device_not_open", findings)
        self.assertIn("authentication_session_incomplete", findings)

    def test_rejects_untrusted_session_id(self):
        parsed = parse_record(record(1, "fpstudio_auth session=../../secret component=pam event=request_started"))
        self.assertIsNone(parsed["session"])

    def test_accepts_nanosecond_session_id(self):
        parsed = parse_record(record(1, "fpstudio_auth session=abc-123-def component=pam service=sudo event=request_started"))
        self.assertEqual(parsed["session"], "abc-123-def")
        self.assertEqual(parsed["service"], "sudo")

    def test_human_report_prints_repeated_findings(self):
        report = self.report([
            (1, "fpstudio_auth session=abc-10 component=pam service=sudo event=request_started"),
            (2, "fpstudio_auth session=abc-10 component=pam service=sudo event=request_skipped reason=terminal_unsupported"),
        ])
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            print_human(report)
        self.assertIn("module_skipped:terminal_unsupported=1", output.getvalue())


if __name__ == "__main__":
    unittest.main()
