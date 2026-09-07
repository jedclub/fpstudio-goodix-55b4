import unittest
from prepare_bank import choose_gallery


class GalleryTest(unittest.TestCase):
    def test_coverage_and_determinism(self):
        coverage = {"a": {1, 2, 3}, "b": {1, 2}, "c": {4, 5}, "d": {5}}
        quality = {key: 1 for key in coverage}
        selected, covered = choose_gallery(coverage, quality, 8)
        self.assertEqual(selected, ["a", "c"])
        self.assertEqual(covered, {1, 2, 3, 4, 5})
        self.assertEqual(choose_gallery(dict(reversed(list(coverage.items()))), quality, 8)[0], selected)

    def test_limit_and_empty(self):
        self.assertEqual(choose_gallery({"a": {1}, "b": {2}}, {"a": 2, "b": 1}, 1), (["a"], {1}))
        self.assertEqual(choose_gallery({"a": set()}, {"a": 1}, 5), ([], set()))


if __name__ == "__main__":
    unittest.main()
