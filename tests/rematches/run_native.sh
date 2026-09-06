#!/usr/bin/env bash
# Production Kanto rules AND the real authored trainer database are required.
set -euo pipefail
sc_rematch_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../.." && pwd)
sc_rematch_log=${SC_REMATCH_LOG:-$(mktemp "${TMPDIR:-/tmp}/sc-rematches-native.XXXXXX.log")}
printf 'Native rematch log: %s\n' "$sc_rematch_log"
make -C "$sc_rematch_root/engine" BUILD=firered SC_TEST_CAMPAIGN=1 \
    TESTS='SC rematches:*' check -j2 2>&1 | tee "$sc_rematch_log"
python3 - "$sc_rematch_log" <<'PY'
import re
import sys
from pathlib import Path
text=re.sub(r'\x1b\[[0-9;]*m','',Path(sys.argv[1]).read_text())
for label in ('PASSED','TOTAL'):
    found=re.findall(r'Tests\s+'+label+r':\s+(\d+)',text)
    if found != ['4']:
        raise SystemExit(f'Expected exactly four native {label.lower()} groups, got {found}')
if re.search(r'Tests\s+(?:FAILED|EXPECT_FAILING):\s+[1-9]',text):
    raise SystemExit('Rematch checks require four ordinary passes, with no expected failures')
print('Verified: four campaign-data native rematch groups passed.')
PY
