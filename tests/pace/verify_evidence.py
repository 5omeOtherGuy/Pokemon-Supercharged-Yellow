"""Check retained actual-runtime timing evidence; does not replay the game."""
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent / 'evidence/57d08b70'
manifest = json.loads((HERE / 'manifest.json').read_text())
for name, expected in manifest['files'].items():
    assert hashlib.sha256((HERE / name).read_bytes()).hexdigest() == expected, name
results = []
for index, name in enumerate(('norm', 'fast', 'quick')):
    folder = HERE / name
    result = json.loads((folder / 'measurement.json').read_text())
    assert result['saved_pace'] == 1 << index and result['outcome'] == 1
    assert result['end_frame'] - result['begin_frame'] == result['measured_frames']
    responses = [json.loads(line) for line in (folder / 'responses.jsonl').read_text().splitlines()]
    commands = (folder / 'commands.txt').read_text().splitlines()
    assert len(responses) == len(commands) + 1 and all(r['ok'] for r in responses)
    frames = sum(int(c.split()[1]) for c in commands if c.startswith('run '))
    assert frames == result['end_frame']
    results.append(result)
assert results[0]['measured_frames'] > results[1]['measured_frames'] > results[2]['measured_frames']
assert len({r['party_before'] for r in results}) == len({r['party_after'] for r in results}) == 1
records = []
for name in ('responses.jsonl', 'restart.jsonl'):
    responses = [json.loads(line) for line in (HERE / 'quick-save' / name).read_text().splitlines()]
    assert all(r['ok'] for r in responses)
    records.append([r['hex'] for r in responses if r.get('address') == 0x020073c0 and len(r.get('hex', '')) == 96][-1])
assert records[0] == records[1] and bytes.fromhex(records[1])[29] == 4
print('Pinned actual timings:', [r['measured_frames'] for r in results], '; identical resulting party data; QUICK restored after Continue.')
