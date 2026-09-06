"""Reject empty, skipped or failed native runs and verify the isolation sequence."""
from pathlib import Path
import re
import sys

log = re.sub(r'\x1b\[[0-9;]*m', '', Path(sys.argv[1]).read_text())
for result in re.finditer(r'^\[\d+\] (.*): ([A-Z_]+)$', log, re.M):
    name, outcome = result.groups()
    expected_crash = name in ('fatalf counts as CRASH', 'Tests resume after CRASH') and outcome == 'CRASH'
    assert outcome in ('PASS', 'EXPECTED_FAIL') or expected_crash, result[0]
assert not re.search(r'Tests (FAILED|SKIPPED|TO_DO|KNOWN_FAILING):', log), log
for title in (
    'SC runtime effects: Quick Start changes actual order only on the entry turn',
    'SC runtime effects: delayed hit keeps original capability after a switch',
    'SC runtime effects: wild battles retain player recovery and neutral wild effects',
    'SC AI: public physical defense changes damage estimates and chosen category',
    'SC AI: pending commands, exact player stats and battle RNG cannot change observations',
    "SC AI: the player's hidden supply reservation cannot change observations",
):
    assert f'{title}: PASS' in log, title
trigger = log.index('SC isolation: an expected failure leaves campaign opt-ins enabled: EXPECTED_FAIL')
check = log.index('SC isolation: teardown resets opt-ins after a failed test: PASS')
assert trigger < check, 'Teardown fixture did not execute in the required serial order'
total = re.search(r'Tests TOTAL:\s+(\d+)', log)
assert total and int(total[1]) >= 80, 'Missing selected native tests'
print(f'Native regression validated: {total[1]} groups, including the deliberate failure/teardown pair.')
