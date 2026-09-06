"""Measure an actual scripted-wild fixture at all three Options presets.

Uses ordinary controls after an explicitly patched ROM setup. Does not inject
RAM, simulate combat, measure audio, or claim organic campaign progress.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess

ROOT = Path(__file__).resolve().parents[2]


def run(args, name, selection, symbols):
    out = args.output / name
    out.mkdir()
    with (out / 'emulator.log').open('w') as errors, (out / 'commands.txt').open('w') as commands, (out / 'responses.jsonl').open('w') as responses:
        process = subprocess.Popen([str(ROOT / 'build/emulator/headless'), str(args.rom), str(out / 'game.sav')], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=errors, text=True, bufsize=1)
        def receive():
            line = process.stdout.readline()
            responses.write(line)
            responses.flush()
            result = json.loads(line)
            if not result.get('ok'):
                raise RuntimeError(result)
            return result
        def send(command):
            commands.write(command + '\n')
            commands.flush()
            process.stdin.write(command + '\n')
            process.stdin.flush()
            return receive()
        def read(address, length):
            return bytes.fromhex(send(f'read {address:#x} {length}')['hex'])
        def snap(label):
            send(f'screenshot {out}/{label}.png')
        def tap(mask, after=30):
            send(f'run 12 {mask}')
            send(f'run {after} 0')
        receive()
        try:
            # Provenance of the ordinary new-game navigation is the committed
            # service UI trace. Stop before its first Mom interaction.
            seen = False
            for command in (ROOT / 'tests/services/evidence/195bd43e/commands.txt').read_text().splitlines():
                if seen and command == 'run 12 1':
                    break
                if command.startswith('run '):
                    send(command)
                if command == 'run 16 32':
                    seen = True
            else:
                raise RuntimeError('navigation checkpoint absent')
            send('run 60 0')  # Finish the final walking tile before opening Start.
            tap(8, 60)
            snap('start-menu')
            tap(128)
            tap(1, 120)
            snap('options-open')
            tap(128, 20)
            tap(128, 20)
            for i in range(2):
                tap(16 if i < selection else 0, 20)
            snap('pace-selected')
            tap(2, 120)
            tap(2, 60)
            trainer = struct.unpack('<I', read(symbols['gSaveBlock3Ptr'], 4))[0]
            record = read(trainer, 48)
            if record[29] != 1 << selection:
                raise RuntimeError(f'Options did not select {name}: {record.hex()}')
            tap(1, 1200)
            tap(1, 630)
            snap('choose-action')
            tap(1)
            tap(128)  # The actual level-5 learnset lists Thunder Shock third.
            snap('choose-thunder-shock')
            before = read(symbols['gParties'], 100)
            begin = send('status')['frames']
            send('run 1 1')
            for frame in range(2400):
                send(f'run 1 {1 if frame % 30 == 29 else 0}')
                if frame % 30 == 0:
                    snap(f'battle-{frame:04d}')
                if not read(symbols['gMain'] + 0x439, 1)[0] & 2:
                    break
            else:
                raise RuntimeError('battle mode did not exit within 2400 frames')
            end = send('status')['frames']
            outcome = read(symbols['gBattleOutcome'], 1)[0]
            after = read(symbols['gParties'], 100)
            snap('battle-mode-exit')
            if outcome != 1:
                raise RuntimeError(f'battle outcome was {outcome}, expected victory')
            result = {'preset': name, 'saved_pace': record[29], 'begin_frame': begin,
                'end_frame': end, 'measured_frames': end - begin,
                'scope': 'move acceptance until gMain.inBattle clears; field redraw/decision/intro excluded',
                'input_policy': 'one-frame A pulse every30 frames advances messages',
                'outcome': outcome, 'party_before': before.hex(), 'party_after': after.hex()}
            (out / 'measurement.json').write_text(json.dumps(result, indent=2) + '\n')
            print(json.dumps(result), flush=True)
        finally:
            if process.poll() is None:
                send('quit')
            process.wait()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('rom', 'elf', 'fixture_manifest', 'output'):
        parser.add_argument('--' + name.replace('_', '-'), type=Path, required=True)
    args = parser.parse_args()
    fixture = json.loads(args.fixture_manifest.read_text())
    if hashlib.sha256(args.rom.read_bytes()).hexdigest() != fixture['fixture_sha256']:
        parser.error('fixture ROM checksum mismatch')
    if hashlib.sha256(args.elf.read_bytes()).hexdigest() != fixture['elf_sha256']:
        parser.error('source ELF checksum mismatch')
    args.output.mkdir()
    args.output = args.output.resolve()
    symbols = {}
    for line in subprocess.check_output(['arm-none-eabi-nm', '-n', str(args.elf)], text=True).splitlines():
        fields = line.split()
        if len(fields) == 3:
            symbols[fields[2]] = int(fields[0], 16)
    for selection, name in enumerate(('norm', 'fast', 'quick')):
        run(args, name, selection, symbols)


if __name__ == '__main__':
    main()
