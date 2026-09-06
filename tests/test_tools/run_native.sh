#!/usr/bin/env bash
set -euo pipefail
sc_root=$(cd "$(dirname "$0")/../.." && pwd)
cd "$sc_root"
mkdir -p build/owner-test
sc_sources='test/test_runner.c test/test_runner_battle.c test/test_runner_args.c test/test_test_runner.c test/sc_cheats/editor.c test/sc_supplies/action_commit.c'
for sc_tools in 1 0; do
    sc_log="build/owner-test/native-tools-${sc_tools}.log"
    make -C engine BUILD=firered SC_TEST_CAMPAIGN=1 SC_TEST_TOOLS="$sc_tools" \
        TESTS='SC *' TEST_SRCS="$sc_sources" check -j2 > "$sc_log" 2>&1
    python3 - "$sc_log" "$sc_tools" <<'PY'
from pathlib import Path
import re,sys
text=re.sub(r'\x1b\[[0-9;]*m','',Path(sys.argv[1]).read_text())
expected='10' if sys.argv[2]=='1' else '3'
for label in ('PASSED','TOTAL'):
    if re.findall(r'Tests\s+'+label+r':\s+(\d+)',text) != [expected]:
        raise SystemExit('Unexpected native count; see '+sys.argv[1])
if re.search(r'Tests\s+(FAILED|SKIPPED|EXPECT_FAILING|TO_DO|KNOWN_FAILING):\s+[1-9]',text):
    raise SystemExit('Unexpected native result; see '+sys.argv[1])
print('Verified tools='+sys.argv[2]+': '+expected+' native groups passed.')
PY
done
