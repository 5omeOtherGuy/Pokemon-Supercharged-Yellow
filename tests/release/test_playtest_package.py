from pathlib import Path
import importlib.util
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location("build_playtest", ROOT / "tools/build_playtest.py")
package = importlib.util.module_from_spec(spec)
spec.loader.exec_module(package)


class PlaytestPackage(unittest.TestCase):
    def test_header_validation_accepts_a_correct_image_and_rejects_corruption(self):
        # Synthetic bytes, not a commercial ROM or a game build.
        data = bytearray(1024 * 1024)
        data[0xB2] = 0x96
        data[0xBD] = (-sum(data[0xA0:0xBD]) - 0x19) & 255
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "synthetic.gba"
            path.write_bytes(data)
            self.assertEqual(package.check_rom(path)["bytes"], len(data))
            self.assertEqual(package.check_rom(path)["sha256"], package.sha256(path))
            data[0xA0] ^= 1
            path.write_bytes(data)
            with self.assertRaisesRegex(RuntimeError, "header"):
                package.check_rom(path)

    def test_packager_rejects_an_incomplete_output(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "incomplete.gba"
            path.write_bytes(b"unfinished")
            with self.assertRaisesRegex(RuntimeError, "size"):
                package.check_rom(path)


if __name__ == "__main__":
    unittest.main()
