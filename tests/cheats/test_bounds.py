"""Compile and exercise the same request bounds used by the cartridge editor."""
import pathlib
import subprocess
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[2]

class CheatBounds(unittest.TestCase):
    def test_untrusted_editor_requests(self):
        source = r'''
#include <assert.h>
#include <stdint.h>
#include "sc_debug_core.h"
int main(void) {
  assert(ScDebugValidEdit(SC_EDIT_SPECIES, 0, 1, 1000, 1000));
  assert(ScDebugValidEdit(SC_EDIT_SPECIES, 0, 151, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_SPECIES, 0, 0, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_SPECIES, 0, 152, 1000, 1000));
  assert(ScDebugValidEdit(SC_EDIT_LEVEL, 0, 100, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_LEVEL, 0, 0, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_LEVEL, 0, 101, 1000, 1000));
  for (unsigned s=0; s<6; ++s) {
    assert(ScDebugValidEdit(SC_EDIT_IV, s, 31, 1000, 1000));
    assert(!ScDebugValidEdit(SC_EDIT_IV, s, 32, 1000, 1000));
    assert(ScDebugValidEdit(SC_EDIT_TRAINING, s, 255, 1000, 1000));
    assert(!ScDebugValidEdit(SC_EDIT_TRAINING, s, 256, 1000, 1000));
  }
  assert(!ScDebugValidEdit(SC_EDIT_IV, 6, 31, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_TRAINING, UINT32_MAX, 0, 1000, 1000));
  assert(ScDebugValidEdit(SC_EDIT_MOVE, 3, 999, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_MOVE, 4, 1, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_MOVE, 0, 1000, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_ITEM, 0, 1000, 1000, 1000));
  assert(ScDebugValidEdit(SC_EDIT_NATURE, 0, 24, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_NATURE, 0, 25, 1000, 1000));
  assert(ScDebugValidEdit(SC_EDIT_GENDER, 0, 2, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_GENDER, 0, 3, 1000, 1000));
  assert(ScDebugValidEdit(SC_EDIT_ABILITY, 0, 2, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_ABILITY, 0, 3, 1000, 1000));
  assert(ScDebugValidEdit(SC_EDIT_SHINY, 0, 1, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_SHINY, 0, 2, 1000, 1000));
  assert(ScDebugValidEdit(SC_EDIT_FRIENDSHIP, 0, 255, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_FRIENDSHIP, 0, 256, 1000, 1000));
  assert(ScDebugValidEdit(SC_EDIT_PP, 3, 99, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_PP, 4, 1, 1000, 1000));
  assert(!ScDebugValidEdit(SC_EDIT_COUNT, 0, 0, 1000, 1000));
  assert(!ScDebugValidEdit(UINT32_MAX, 0, 0, 1000, 1000));
  assert(ScDebugAdjust(1, 1, 100, -10) == 1);
  assert(ScDebugAdjust(95, 1, 100, 10) == 100);
  assert(ScDebugAdjust(100, 1, 100, -10) == 90);
  assert(ScDebugAdjust(0, 0, 999, 100) == 100);
}
'''
        with tempfile.TemporaryDirectory() as tmp:
            path = pathlib.Path(tmp)
            (path / 'bounds.c').write_text(source)
            subprocess.run(['cc', '-std=c11', '-Wall', '-Werror', '-I', str(ROOT/'engine/include'), str(path/'bounds.c'), '-o', str(path/'bounds')], check=True)
            subprocess.run([str(path/'bounds')], check=True)

if __name__ == '__main__':
    unittest.main()
