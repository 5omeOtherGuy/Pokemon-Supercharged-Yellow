"""Assemble every changed economy map through the real game script pipeline.

Uses only a prepared engine's generated headers/preprocessor read-only.
Does not link a ROM or establish runtime correctness.
"""
import argparse
from pathlib import Path
import subprocess
import tempfile
ROOT=Path(__file__).resolve().parents[2]

def main():
    parser=argparse.ArgumentParser();parser.add_argument('--generated-engine',type=Path,required=True);args=parser.parse_args()
    shared=args.generated_engine.resolve();engine=ROOT/'engine'
    # Stable task base identifies the owned changes even after the GREEN commit.
    changed=subprocess.check_output(['git','diff','--name-only','4389a899','--','engine/data/maps'],cwd=ROOT,text=True).splitlines()
    scripts=sorted(x.removeprefix('engine/') for x in changed if x.endswith('/scripts.inc'))
    preproc=str(shared/'tools/preproc/preproc')
    prefix=(engine/'data/event_scripts.s').read_text().split('gSpecialVars::')[0]
    with tempfile.TemporaryDirectory(prefix='sc-economy-asm-') as folder:
        wrapper=Path(folder)/'economy.s'
        wrapper.write_text(prefix+'\n'+''.join('\t.include "'+x+'"\n' for x in scripts))
        first=subprocess.run([preproc,'-s',str(wrapper),'charmap.txt'],cwd=engine,stdout=subprocess.PIPE,check=True)
        cpp=subprocess.run(['arm-none-eabi-cpp','-I','include','-I',str(shared/'include'),'-I',str(shared/'include/constants'),'-I','.','-DMODERN=1','-DTESTING=0','-DFIRERED','-std=gnu17','-'],cwd=engine,input=first.stdout,stdout=subprocess.PIPE,check=True)
        second=subprocess.run([preproc,'-ie',str(wrapper),'charmap.txt'],cwd=engine,input=cpp.stdout,stdout=subprocess.PIPE,check=True)
        subprocess.run(['arm-none-eabi-as','-mcpu=arm7tdmi','-march=armv4t','-meabi=5','--defsym','MODERN=1','--defsym','FIRERED=1','-o',str(Path(folder)/'economy.o'),'-'],cwd=engine,input=second.stdout,check=True)
        print(f'{len(scripts)} changed map scripts assembled through real text/macros/ARM pipeline.')

if __name__=='__main__':main()
