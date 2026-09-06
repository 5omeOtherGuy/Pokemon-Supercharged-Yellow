"""Interactive logged diagnostic session; each command has exactly one response.

The test save must be absent. Bootstrap derives ordinary introduction/house
inputs from existing service evidence, then stops just after the new Mom warp.
"""
import argparse
import json
from pathlib import Path
import subprocess
import sys
ROOT=Path(__file__).resolve().parents[2]

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--rom',type=Path,required=True);ap.add_argument('--output',type=Path,required=True);ap.add_argument('--commands',type=Path,help='Replay a complete recorded trace instead of bootstrapping interactively');args=ap.parse_args()
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    if any((out/name).exists() for name in ('fresh.sav','commands.txt','responses.jsonl')):ap.error('use a fresh diagnostic output directory')
    driver=ROOT/'build/services/fixture-driver'
    if not driver.exists():ap.error('run tests/services/build_fixture_driver.py first')
    process=subprocess.Popen([str(driver),str(args.rom.resolve()),str(out/'fresh.sav')],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=(out/'stderr.log').open('w'),text=True,bufsize=1)
    (out/'process.json').write_text(json.dumps({'pid':process.pid,'provenance':'isolated diagnostic emulator'})+'\n')
    with (out/'commands.txt').open('w',buffering=1) as commands,(out/'responses.jsonl').open('w',buffering=1) as responses:
        def receive():
            response=process.stdout.readline()
            if not response:raise RuntimeError('emulator ended without response')
            responses.write(response);print(response,end='',flush=True)
            if not json.loads(response).get('ok'):raise RuntimeError('emulator command failed')
        def send(command):
            commands.write(command+'\n');process.stdin.write(command+'\n');process.stdin.flush();receive()
        try:
            receive()
            if args.commands:
                for command in args.commands.read_text().splitlines():
                    if command.startswith('screenshot '):
                        command='screenshot '+str(out/Path(command.split(' ',1)[1]).name)
                    send(command)
                    if command=='quit':break
                return
            for command in (ROOT/'tests/services/evidence/195bd43e/commands.txt').read_text().splitlines():
                if 'center-arrival.png' in command:break
                if not command.startswith('screenshot '):send(command)
            send('screenshot '+str(out/'arrival.png'))
            for line in sys.stdin:
                command=line.strip()
                if not command:continue
                send(command)
                if command=='quit':break
        finally:
            process.stdin.close();process.wait(timeout=10)

if __name__=='__main__':main()
