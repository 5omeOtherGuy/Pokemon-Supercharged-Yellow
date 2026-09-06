"""Run the exact portable progression arithmetic used by the GBA adapter."""
from pathlib import Path
import subprocess
import unittest

ROOT = Path(__file__).resolve().parents[2]
BUILD = ROOT / "build" / "progression"


class ProgressionCoreTests(unittest.TestCase):
    def test_persistent_growth_and_reward_rules(self):
        BUILD.mkdir(parents=True, exist_ok=True)
        for profile in BUILD.glob("core_test-*.gcda"):
            profile.unlink()
        subprocess.run(
            ["cc", "-std=c11", "-Wall", "-Wextra", "-Werror", "--coverage", "-O0",
             "-I", str(ROOT / "engine" / "include"),
             str(ROOT / "tests" / "progression" / "core_test.c"),
             str(ROOT / "engine" / "src" / "sc_progression_core.c"),
             "-o", str(BUILD / "core_test")],
            cwd=BUILD, check=True,
        )
        subprocess.run([str(BUILD / "core_test")], cwd=BUILD, check=True)


if __name__ == "__main__":
    unittest.main()
