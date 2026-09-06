"""Verify the native wrapper cannot report an empty or fixture-mode run as green."""
from pathlib import Path
import os
import subprocess
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[2]

class NativeWrapperTests(unittest.TestCase):
    def test_counted_wrapper_requires_campaign_mode_and_exact_executed_groups(self):
        with tempfile.TemporaryDirectory(prefix='sc-rematch-runner-') as folder:
            d=Path(folder);fake=d/'make'
            fake.write_text('''#!/usr/bin/env python3
import os,sys
assert 'BUILD=firered' in sys.argv
assert 'SC_TEST_CAMPAIGN=1' in sys.argv
prefix=next(a for a in sys.argv if a.startswith('TESTS='))
expected=4 if prefix=='TESTS=SC rematches:*' else 1
case=os.environ['SC_RUNNER_CASE']
count=0 if case=='empty' else expected
if case=='missing_bag' and expected==1:count=0
print(f'- Tests \\x1b[32mPASSED\\x1b[0m: {count}')
print(f'- Tests \\x1b[34mTOTAL\\x1b[0m: {count}')
if case=='expected_failure':print('- Tests EXPECT_FAILING: 1')
if case=='command_failure':sys.exit(2)
''')
            fake.chmod(0o755)
            for case in ('success','empty','missing_bag','expected_failure','command_failure'):
                with self.subTest(case=case):
                    env=dict(os.environ,PATH=str(d)+os.pathsep+os.environ['PATH'],
                             SC_RUNNER_CASE=case,SC_REMATCH_LOG=str(d/(case+'.log')))
                    run=subprocess.run([str(ROOT/'tests/rematches/run_native.sh')],env=env,text=True,capture_output=True)
                    self.assertEqual(run.returncode==0,case=='success',run.stdout+run.stderr)
                    if case=='success':
                        self.assertIn('Verified: 4 campaign-data native groups passed.',run.stdout)
                        self.assertIn('Verified: 1 campaign-data native groups passed.',run.stdout)

if __name__=='__main__':unittest.main()
