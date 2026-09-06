from pathlib import Path
import subprocess
import unittest

ROOT = Path(__file__).resolve().parents[2]
BUILD = ROOT / "build" / "progression"


class PublicEffectTests(unittest.TestCase):
    def test_public_conditions_and_multiplicative_effects(self):
        BUILD.mkdir(parents=True, exist_ok=True)
        for profile in BUILD.glob("effects_test-*.gcda"):
            profile.unlink()
        subprocess.run(
            ["cc", "-std=c11", "-Wall", "-Wextra", "-Werror", "--coverage", "-O0",
             "-I", str(ROOT / "engine" / "include"),
             str(ROOT / "tests" / "progression" / "effects_test.c"),
             str(ROOT / "engine" / "src" / "sc_effects.c"),
             "-o", str(BUILD / "effects_test")], check=True, cwd=BUILD)
        subprocess.run([str(BUILD / "effects_test")], check=True, cwd=BUILD)


if __name__ == "__main__":
    unittest.main()
