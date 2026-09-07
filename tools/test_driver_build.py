import unittest
from driver_build import pinned_recipe


class DriverBuildTest(unittest.TestCase):
    def test_no_moving_branch_or_forced_pull(self):
        source = 'source=("x::git+https://github.com/TheWeirdDev/libfprint.git")\n    git switch \'55b4-experimental\'\n    git pull -f origin 55b4-experimental\n    patch -Np1 -i sample.patch\n'
        result = pinned_recipe(source)
        self.assertIn("#commit=c1937b9", result)
        self.assertNotIn("git pull", result)
        self.assertNotIn("git switch", result)
        self.assertIn("patch -Np1", result)


if __name__ == "__main__":
    unittest.main()
