#!/usr/bin/env bash
# Production Kanto rules AND the real authored trainer database are required.
set -euo pipefail
sc_rematch_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../.." && pwd)
sc_rematch_log=${SC_REMATCH_LOG:-$(mktemp "${TMPDIR:-/tmp}/sc-rematches-native.XXXXXX.log")}
sc_rematch_sources='test/test_runner.c test/test_runner_battle.c test/test_runner_args.c test/test_test_runner.c test/sc_rematches/vs_seeker.c test/sc_supplies/selection.c'
sc_run_native_group() {
    local sc_expected=$1 sc_prefix=$2 sc_log=$3
    printf 'Native campaign log: %s\n' "$sc_log"
    make -C "$sc_rematch_root/engine" BUILD=firered SC_TEST_CAMPAIGN=1 \
        TESTS="$sc_prefix" TEST_SRCS="$sc_rematch_sources" check -j2 2>&1 | tee "$sc_log"
    python3 - "$sc_log" "$sc_expected" <<'PY'
import re
import sys
from pathlib import Path
text=re.sub(r'\x1b\[[0-9;]*m','',Path(sys.argv[1]).read_text())
expected=sys.argv[2]
for label in ('PASSED','TOTAL'):
    found=re.findall(r'Tests\s+'+label+r':\s+(\d+)',text)
    if found != [expected]:
        raise SystemExit(f'Expected exactly {expected} native {label.lower()} groups, got {found}')
if re.search(r'Tests\s+(?:FAILED|EXPECT_FAILING):\s+[1-9]',text):
    raise SystemExit('Campaign checks require ordinary passes, with no expected failures')
print(f'Verified: {expected} campaign-data native groups passed.')
PY
}
sc_run_native_group 4 'SC rematches:*' "$sc_rematch_log"
sc_run_native_group 1 'SC supplies: every authored opponent bag*' "${sc_rematch_log%.log}.bags.log"
