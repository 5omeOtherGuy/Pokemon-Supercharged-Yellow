#!/usr/bin/env bash
# Real GBA execution; no interactive campaign control.
set -euo pipefail
sc_sim_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../.." && pwd)
sc_sim_log=${SC_SIMULATION_LOG:-$(mktemp "${TMPDIR:-/tmp}/sc-simulation.XXXXXX.log")}
sc_sim_sources='test/test_runner.c test/test_runner_battle.c test/test_runner_args.c test/test_test_runner.c test/sc_simulation/stage_interactions.c'
make -C "$sc_sim_root/engine" BUILD=firered SC_TEST_CAMPAIGN=1 \
    TESTS='SC simulation:*' TEST_SRCS="$sc_sim_sources" check -j2 2>&1 | tee "$sc_sim_log"
python3 - "$sc_sim_log" <<'PY'
import re
import sys
from pathlib import Path
text = re.sub(r'\x1b\[[0-9;]*m', '', Path(sys.argv[1]).read_text())
for label in ('PASSED', 'TOTAL'):
    found = re.findall(r'Tests\s+' + label + r':\s+(\d+)', text)
    if found != ['7']:
        raise SystemExit(f'Expected exactly 7 {label.lower()} groups, got {found}')
if re.search(r'Tests\s+(?:FAILED|EXPECT_FAILING):\s+[1-9]', text):
    raise SystemExit('All simulation groups must pass without expected failures')
print('Verified: 7 native campaign interaction groups passed.')
PY
