"""Release sources must not contain or embed complete ROM images."""

from pathlib import Path
import re
import subprocess
import unittest


ROOT = Path(__file__).resolve().parents[2]


class SourcePolicyTests(unittest.TestCase):
    def test_no_tracked_rom_images(self):
        paths = subprocess.check_output(
            ["git", "ls-files", "-z"], cwd=ROOT, text=True
        ).split("\0")
        self.assertFalse(
            [path for path in paths if Path(path).suffix.lower() in (".gba", ".gb", ".gbc")]
        )

    def test_engine_does_not_require_embedded_rom_images(self):
        sources = list((ROOT / "engine" / "data").rglob("*.s"))
        self.assertTrue(sources, "engine assembly sources must be present")
        dependencies = []
        for source in sources:
            for number, line in enumerate(source.read_text().splitlines(), 1):
                if re.search(r'\.incbin\s+"[^\"]+\.(?:gba|gbc|gb)"', line):
                    dependencies.append(f"{source.relative_to(ROOT)}:{number}: {line.strip()}")
        self.assertEqual(dependencies, [], "release build must work without bundled ROMs")


if __name__ == "__main__":
    unittest.main()
