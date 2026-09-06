from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[2]
class SupplyTests(unittest.TestCase):
    def test_portable_units_categories_cancel_and_doubles(self):
        with tempfile.TemporaryDirectory(prefix='sc-supplies-') as d:
            exe=Path(d)/'test'
            result=subprocess.run(['cc','-std=c11','-Wall','-Wextra','-Werror','-I'+str(ROOT/'engine/include'),str(ROOT/'tests/supplies/test_supplies_core.c'),str(ROOT/'engine/src/sc_supplies_core.c'),'-o',str(exe)],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)
            result=subprocess.run([str(exe)],capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)

if __name__=='__main__':unittest.main()
