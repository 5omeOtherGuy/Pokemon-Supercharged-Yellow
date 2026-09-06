"""Real libmgba integration checks; requires a locally built GBA ROM."""
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
BINARY = ROOT / 'build/emulator/headless'
ROM = os.environ.get('EMULATOR_TEST_ROM')


class HarnessTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        BINARY.parent.mkdir(parents=True, exist_ok=True)
        subprocess.run(['cc', '-std=c11', '-Wall', '-Wextra', '-Werror', '-O2',
                        '-o', str(BINARY), str(ROOT / 'tools/emulator/headless.c'),
                        '-lmgba'], check=True)
        if not ROM:
            raise RuntimeError('Set EMULATOR_TEST_ROM to a locally built GBA ROM')

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(dir=BINARY.parent)
        self.path = Path(self.temp.name)
        self.start()

    def start(self):
        self.proc = subprocess.Popen([str(BINARY), ROM, str(self.path / 'test.sav')],
                                     stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE, text=True)
        self.ready = json.loads(self.proc.stdout.readline())
        self.assertTrue(self.ready['ok'], self.ready)

    def command(self, command):
        self.proc.stdin.write(command + '\n')
        self.proc.stdin.flush()
        return json.loads(self.proc.stdout.readline())

    def stop(self):
        self.assertTrue(self.command('quit')['ok'])
        _, errors = self.proc.communicate(timeout=10)
        self.assertEqual(self.proc.returncode, 0, errors)

    def tearDown(self):
        if self.proc.poll() is None:
            self.proc.kill()
            self.proc.communicate()
        self.temp.cleanup()

    def test_real_frames_keys_png_and_reset(self):
        self.assertEqual(self.ready['provenance'], 'fresh-input-only')
        first = self.command('run 180 0')
        self.assertEqual(first['frames'], 180)
        first_path = self.path / 'first image.png'
        self.assertTrue(self.command(f'screenshot {first_path}')['ok'])
        png = first_path.read_bytes()
        self.assertEqual(png[:8], b'\x89PNG\r\n\x1a\n')
        self.assertEqual(struct.unpack('>II', png[16:24]), (240, 160))
        self.assertEqual(self.command('run 2 1')['keys'], 1)
        # Actual emulated hardware key register, active low.
        self.assertEqual(self.command('read 0x04000130 2')['hex'], 'fe03')
        self.assertEqual(self.command('release')['keys'], 0)
        self.assertEqual(self.command('read 0x04000130 2')['hex'], 'ff03')
        self.assertEqual(self.command('reset')['frames'], 0)
        self.command('run 180 0')
        second_path = self.path / 'reset.png'
        self.command(f'screenshot {second_path}')
        self.assertEqual(hashlib.sha256(png).digest(),
                         hashlib.sha256(second_path.read_bytes()).digest())
        self.stop()
        self.assertTrue((self.path / 'test.sav').exists())
        self.start()
        self.assertEqual(self.ready['provenance'], 'existing-save-unverified')
        self.command('run 180 0')
        self.command(f'screenshot {second_path}')
        self.assertEqual(png, second_path.read_bytes())

    def test_rejects_invalid_commands_without_advancing(self):
        for cmd in ['run -1 0', 'run 1 1024', 'run 1', 'run 1 0 junk',
                    'read 0xffffffff 2', 'read 0 0', 'read 0 4097',
                    'read -1 1', 'read 0x10000000 1', 'write 0x02000000 0',
                    'reset junk', 'release junk', 'screenshot /missing/dir/test.png',
                    'unknown', 'x' * 9000]:
            with self.subTest(cmd=cmd[:80]):
                self.assertFalse(self.command(cmd)['ok'])
        self.assertEqual(self.command('status')['frames'], 0)
        self.assertEqual(self.command('run 0 0')['frames'], 0)


if __name__ == '__main__':
    unittest.main()
