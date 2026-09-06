"""Verify recorded diagnostic emulator transactions, without claiming human balance."""
import hashlib
import json
from pathlib import Path
import struct

EVIDENCE = Path(__file__).with_name('evidence') / '9d479bc5'
SCENES = ('celadon', 'berries', 'gift', 'rick', 'ethan', 'indigo')
ROM = '73759e33b3fe975f887e9b6286002ad0b00ef6a0bf8abd8da821fade2025afe3'
ELF = '763258ce59c9a7002bbd59638cc347c22896eaf47d94003a455fea2f5512cbf7'


def records(scene):
    folder = EVIDENCE / scene
    commands = (folder / 'commands.txt').read_text().splitlines()
    responses = [json.loads(x) for x in (folder / 'responses.jsonl').read_text().splitlines()]
    assert len(responses) == len(commands) + 1
    assert all(x['ok'] for x in responses)
    assert commands[-1] == 'quit'
    frame = 0
    reads = {}
    writes = []
    for command, response in zip(commands, responses[1:]):
        if command.startswith('run '):
            frame += int(command.split()[1])
            assert response['frames'] == frame
        if command.startswith('fixture32 '):
            writes.append((frame, *map(lambda x: int(x, 0), command.split()[1:])))
        if command.startswith('read '):
            address, size = (int(x, 0) for x in command.split()[1:])
            value = bytes.fromhex(response['hex'])
            assert response['address'] == address and len(value) == size
            reads.setdefault((frame, address, size), []).append(value)
        if command.startswith('screenshot '):
            image = folder / Path(command.split(' ', 1)[1]).name
            data = image.read_bytes()
            assert data[:8] == b'\x89PNG\r\n\x1a\n'
            assert struct.unpack('>II', data[16:24]) == (240, 160)
    fixture = json.loads((folder / 'fixture.json').read_text())
    assert fixture['source_revision'] == '9d479bc5'
    assert fixture['source_sha256'] == ROM and fixture['elf_sha256'] == ELF
    assert len(bytes.fromhex(fixture['patch_bytes'])) == len(bytes.fromhex(fixture['original_bytes']))
    return reads, writes, len(commands)


def main():
    manifest = json.loads((EVIDENCE / 'manifest.json').read_text())
    for name, digest in manifest['sha256'].items():
        assert hashlib.sha256((EVIDENCE / name).read_bytes()).hexdigest() == digest, name
    logs = {scene: records(scene) for scene in SCENES}
    def raw(scene, frame, address, size=4):
        return logs[scene][0][frame, address, size][-1]
    def word(scene, frame, address):
        return int.from_bytes(raw(scene, frame, address), 'little')
    def money(scene, frame, cash, key, expected):
        assert word(scene, frame, cash) ^ word(scene, frame, key) == expected
    def bag(scene, frame, address, size, key):
        return [(item, quantity ^ key) for item, quantity in struct.iter_unpack('<HH', raw(scene, frame, address, size)) if item]

    money('celadon', 18926, 0x20103d4, 0x2013dfc, 12500)
    money('celadon', 20756, 0x20103d4, 0x2013dfc, 7500)
    assert bag('celadon', 18926, 0x20104a4, 8, 0xbf0b) == [(481, 1)]
    assert bag('celadon', 20756, 0x20104a4, 16, 0xbf0b) == [(481, 1), (392, 1)]
    assert logs['celadon'][1] == [(18926, 0x20112b8, 0x4000000f)]
    assert word('celadon', 18926, 0x20112b8) == 0x40000000
    money('berries', 22092, 0x20103b4, 0x2013ddc, 12600)
    assert bag('berries', 22092, 0x20106b4, 12, 0xc0c7) == [(523, 2)]
    money('indigo', 20031, 0x20103b8, 0x2013de0, 12700)
    assert bag('indigo', 20031, 0x2010488, 8, 0xb1e5) == [(497, 1)]

    for scene, data in {
        'rick': [(17303, 0x20103dc, 0x2013e04, 0), (24524, 0x20103b8, 0x2013de0, 72), (34846, 0x20103a4, 0x2013dcc, 144)],
        'ethan': [(17423, 0x20103e0, 0x2013e08, 0), (22477, 0x20103f0, 0x2013e18, 360), (29969, 0x20103f0, 0x2013e18, 720)],
    }.items():
        for values in data:
            money(scene, *values)
    for scene, before, after, cash in [('rick', 36080, 36201, 0x20103a4), ('ethan', 23832, 23953, 0x20103f0)]:
        assert raw(scene, before, cash) == raw(scene, after, cash)
        assert raw(scene, before, 0x2031c7c, 100) == raw(scene, after, 0x2031c7c, 100)
        assert any(raw(scene, before, 0x2031c7c, 100))
    for scene in ('berries', 'rick', 'ethan', 'indigo'):
        assert not logs[scene][1], scene

    expected_ids = [x for x in range(706, 737) if x != 714]
    assert logs['gift'][1] == [(17041, 0x2010500 + i * 4, 0xb1e40000 + item) for i, item in enumerate(expected_ids)] + [(18988, 0x2010574, 0xb1e50000)]
    full = bag('gift', 18988, 0x2010500, 120, 0xb1e5)
    assert full == [(x, 1) for x in expected_ids]
    assert word('gift', 18867, 0x20111e8) == word('gift', 18988, 0x20111e8) == 0
    delivered = bag('gift', 21515, 0x2010500, 120, 0xb1e5)
    assert delivered == full[:-1] + [(714, 1)]
    assert word('gift', 21515, 0x20111e8) == word('gift', 23681, 0x20111e8) == 0x40000
    assert bag('gift', 23681, 0x2010500, 120, 0xb1e5) == delivered
    assert word('gift', 20793, 0x20111e8) == 0  # Claim occurs after the receipt message completes.
    print(f'6 diagnostic scenarios; {sum(x[2] for x in logs.values())} successful commands; purchase/payout/decline/gift state assertions PASS.')
    print('Rendered stock, free exits and sight-line behavior require the documented screenshot/trace inspection; no human balance or Android claim.')


if __name__ == '__main__':
    main()
