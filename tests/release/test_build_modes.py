"""Ask GNU make for resolved outputs; ensure cheat objects cannot leak into normal builds."""
from pathlib import Path
import subprocess
import unittest

ROOT = Path(__file__).resolve().parents[2]


def config(*settings):
    command = ["make", "-s", "-C", str(ROOT / "engine"), "BUILD=firered", "NODEP=1", "SETUP_PREREQS=0", *settings,
               "--eval=sc-config-probe:;@echo SC_CONFIG $(ROM_NAME) $(OBJ_DIR) $(TESTELF) $(CPPFLAGS)", "sc-config-probe"]
    result = subprocess.run(command, capture_output=True, text=True)
    if result.returncode:
        raise AssertionError(result.stderr)
    return next(line.split()[1:] for line in result.stdout.splitlines() if line.startswith("SC_CONFIG "))


class BuildModes(unittest.TestCase):
    def test_owner_objects_and_binary_have_distinct_names_and_macro(self):
        normal, owner = config("SC_TEST_TOOLS=0"), config("SC_TEST_TOOLS=1")
        self.assertNotEqual(normal[0], owner[0], "cheat build overwrites normal ROM")
        self.assertNotEqual(normal[1], owner[1], "cheat build reuses normal objects")
        self.assertIn("-DSC_TEST_TOOLS=0", normal)
        self.assertIn("-DSC_TEST_TOOLS=1", owner)

    def test_campaign_and_fixture_native_modes_are_also_isolated(self):
        variants = [config("SC_TEST_TOOLS=" + str(cheats), "TEST=1", "SC_TEST_CAMPAIGN=" + str(campaign))
                    for cheats in (0, 1) for campaign in (0, 1)]
        self.assertEqual(len({variant[1] for variant in variants}), 4)
        self.assertEqual(len({variant[2] for variant in variants}), 4)


if __name__ == "__main__":
    unittest.main()
