"""Execute the production title input decision with the engine's real key masks."""
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]

class TitleControlsTests(unittest.TestCase):
    def test_entry_maintenance_and_idle_priorities(self):
        source = r'''
#include <assert.h>
#include "sc_title_controls.h"
int main(void) {
    assert(ScTitleChooseAction(0, A_BUTTON, 0, 0) == SC_TITLE_ENTER);
    assert(ScTitleChooseAction(0, START_BUTTON, 0, 1) == SC_TITLE_ENTER);
    assert(ScTitleChooseAction(A_BUTTON, 0, 0, 0) == SC_TITLE_WAIT);
    assert(ScTitleChooseAction(0, B_BUTTON | SELECT_BUTTON, 0, 0) == SC_TITLE_WAIT);
    assert(ScTitleChooseAction(B_BUTTON | SELECT_BUTTON | DPAD_UP, A_BUTTON, 1, 1) == SC_TITLE_CLEAR_SAVE);
    assert(ScTitleChooseAction(B_BUTTON | SELECT_BUTTON | DPAD_LEFT, START_BUTTON, 1, 1) == SC_TITLE_RESET_RTC);
    assert(ScTitleChooseAction(B_BUTTON | SELECT_BUTTON | DPAD_LEFT, 0, 0, 0) == SC_TITLE_WAIT);
    assert(ScTitleChooseAction(B_BUTTON | SELECT_BUTTON, 0, 1, 0) == SC_TITLE_WAIT);
    assert(ScTitleChooseAction(0, 0, 0, 1) == SC_TITLE_RESTART);
    assert(ScTitleChooseAction(0, DPAD_DOWN, 1, 0) == SC_TITLE_WAIT);
    return 0;
}
'''
        with tempfile.TemporaryDirectory(prefix='psy-title-controls-') as directory:
            path = Path(directory)
            (path / 'test.c').write_text(source)
            subprocess.run(['cc', '-std=c11', '-Wall', '-Wextra', '-Werror', '-I', str(ROOT / 'engine/include'), str(path / 'test.c'), '-o', str(path / 'test')], check=True)
            subprocess.run([str(path / 'test')], check=True)

if __name__ == '__main__':
    unittest.main()
