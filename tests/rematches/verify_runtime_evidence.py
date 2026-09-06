"""Check pinned diagnostic traces; this is not organic journey/balance evidence."""
from pathlib import Path
import hashlib
import json
import struct

EVIDENCE = Path(__file__).with_name('evidence') / 'd2dd72da'
ROM = 'f98b9b8d02ddd0d9a35f6baf2a81750a42c4c89cf525e8411c9dfe927f556acd'
ELF = '8cfa881f978aa3b1ff35494d91928272adc9a2a4e4b29ba3771711583554edfe'


def records(scene):
    folder = EVIDENCE / scene
    commands = (folder / 'commands.txt').read_text().splitlines()
    responses = [json.loads(x) for x in (folder / 'responses.jsonl').read_text().splitlines()]
    assert len(responses) == len(commands) + 1
    assert all(x['ok'] for x in responses)
    assert commands[-1] == 'quit'
    epoch = frame = 0
    reads = {}
    for command, response in zip(commands, responses[1:]):
        op = command.split()[0]
        assert op in ('run', 'release', 'reset', 'status', 'read', 'screenshot', 'quit')
        if op == 'reset':
            epoch += 1
            frame = 0
        if op == 'run':
            frame += int(command.split()[1])
            assert response['frames'] == frame
        if op == 'read':
            address, size = (int(x, 0) for x in command.split()[1:])
            value = bytes.fromhex(response['hex'])
            assert response['address'] == address and len(value) == size
            reads[epoch, frame, address, size] = value
        if op == 'screenshot':
            data = (folder / Path(command.split(' ', 1)[1]).name).read_bytes()
            assert data[:8] == b'\x89PNG\r\n\x1a\n'
            assert struct.unpack('>II', data[16:24]) == (240, 160)
    fixture = json.loads((folder / 'fixture.json').read_text())
    assert fixture['source_sha256'] == ROM and fixture['elf_sha256'] == ELF
    assert fixture['source_revision'] == 'd2dd72dae3be97c094e7f7d5be80456b505aae49'
    assert len(bytes.fromhex(fixture['patch_bytes'])) == len(bytes.fromhex(fixture['original_bytes']))
    assert len(bytes.fromhex(fixture['patch_bytes'])) <= 89
    return reads, len(commands)


def verify_gift(reads):
    def raw(epoch, frame, address, size=4):
        return reads[epoch, frame, address, size]
    def state(epoch, frame, address, packed, ready):
        value = raw(epoch, frame, address, 102)
        assert int.from_bytes(value[:2], 'little') == packed
        assert value[5] == ready  # table slot3 follows the two counter bytes
        assert sum(value[2:]) == ready
    assert raw(0, 19713, 0x0201123e, 1) == b'\x04'  # real gift flag
    assert raw(0, 34469, 0x0201125d, 1) == b'\x02'  # Eddie first victory
    state(0, 37088, 0x020108dc, 99, 0)
    state(0, 37847, 0x020108dc, 100, 0)
    state(0, 38328, 0x020108dc, 0, 1)
    state(0, 39154, 0x020108dc, 0, 1)
    state(1, 2164, 0x02010924, 0, 1)  # ordinary save/reset/Continue
    assert raw(1, 2164, 0x020112cc, 1) == b'\x02'
    assert raw(1, 3972, 0x02000934, 32)[:4] == bytes.fromhex('26000900')
    assert raw(1, 3972, 0x020000cc) == bytes.fromhex('0c000000')
    state(1, 9033, 0x020108e0, 0x0505, 0)  # actual repeat victory clears readiness
    def money(epoch, frame, address, key):
        return int.from_bytes(raw(epoch, frame, address), 'little') ^ int.from_bytes(raw(epoch, frame, key), 'little')
    assert money(0, 34469, 0x020103dc, 0x02013e04) == 3000 + 336
    assert money(1, 9033, 0x020103a8, 0x02013dd0) == 3000 + 2 * 336
    state(1, 10814, 0x020108e0, 100, 0)
    state(1, 11295, 0x020108e0, 0, 1)
    state(1, 12899, 0x020108e0, 0x6363, 1)
    state(1, 13005, 0x020108e0, 100, 0)
    state(1, 13486, 0x020108e0, 0, 1)
    state(1, 13922, 0x020108e0, 16, 0)
    assert raw(1, 13922, 0x0200ff18, 12)[4:6] == bytes((37, 5))  # Vermilion


def verify_doubles_red(reads):
    def raw(frame, address, size):
        return reads[0, frame, address, size]
    assert raw(19110, 0x02000934, 32)[:4] == bytes.fromhex('25007801')
    assert raw(19110, 0x020000cc, 4) == bytes.fromhex('0d000000')
    assert raw(36462, 0x02011243, 1) == b'\x01'
    assert int.from_bytes(raw(17041, 0x02031c7c, 200)[186:188], 'little') == 19
    assert int.from_bytes(raw(36462, 0x02031c7c, 200)[186:188], 'little') == 0
    assert raw(39202, 0x02031c7c, 200) == raw(36462, 0x02031c7c, 200)
    for frame in (39202, 39601):
        assert raw(frame, 0x02000934, 32)[:4] == bytes.fromhex('27007801')
    for frame in (39601, 41019):
        state = raw(frame, 0x02010934, 102)
        assert state[:2] == b'\x01\x01' and state[64] == 1
        assert sum(state[2:]) == 1
    assert raw(41019, 0x0200ff6c, 12)[:4] == bytes.fromhex('28000400')


def main():
    manifest = json.loads((EVIDENCE / 'manifest.json').read_text())
    for name, digest in manifest['sha256'].items():
        assert hashlib.sha256((EVIDENCE / name).read_bytes()).hexdigest() == digest, name
    gift, count = records('gift')
    verify_gift(gift)
    print(f'Gift-to-rematch diagnostic: {count} commands; charge, ready, first/repeat payout, save/reload, expiry and transition assertions PASS.')
    red, red_count = records('doubles-red')
    verify_doubles_red(red)
    print(f'Historical double-refusal RED reproduced: {red_count} commands, actual ID376 and fainted partner; retained readiness, repeated refusal and unchanged position.')
    print('All runtime actions use ordinary emulator input; setup is an explicit separate ROM fixture. No human balance or Android claim.')

if __name__ == '__main__':
    main()
