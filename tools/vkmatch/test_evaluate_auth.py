import copy
import unittest
from evaluate_auth import accepted


class ReplayPolicyTest(unittest.TestCase):
    def setUp(self):
        self.good = dict(ok=True, best=dict(ambiguous=False, at_search_boundary=False,
            interior=dict(consistent=True, ncc=.8, gradient=.8, overlap=.6,
                          supported_tiles=10, probe_tiles=10)))

    def test_accepts_known_policy_result(self):
        self.assertTrue(accepted(self.good))

    def test_incomplete_result_is_rejected(self):
        for value in ({}, dict(ok=True), dict(best=self.good['best'])):
            self.assertFalse(accepted(value))
        for key in ('ambiguous', 'at_search_boundary'):
            value = copy.deepcopy(self.good)
            del value['best'][key]
            self.assertFalse(accepted(value))

    def test_invalid_scores_are_rejected(self):
        for key in ('ncc', 'gradient', 'overlap'):
            for score in (None, True, float('nan'), float('inf'), -.1, 1.1, '0.9'):
                value = copy.deepcopy(self.good)
                value['best']['interior'][key] = score
                self.assertFalse(accepted(value))

    def test_invalid_coverage_is_rejected(self):
        for key in ('supported_tiles', 'probe_tiles'):
            for tiles in (None, 7, 17, True, '10'):
                value = copy.deepcopy(self.good)
                value['best']['interior'][key] = tiles
                self.assertFalse(accepted(value))


if __name__ == '__main__':
    unittest.main()
