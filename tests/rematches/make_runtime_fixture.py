"""Compile a diagnostic Vs. Seeker setup against a frozen ROM/ELF.

Derived from tests/economy/make_runtime_fixture.py; Vs. Seeker and battle behavior are unmodified.
Both setups grant explicit test Pokémon and one badge. The gift scene stops
before the real free Vermilion gift; the doubles scene grants the device and one
Revive. Defeated flags and rematch state are never injected.

Only the disposable output ROM is written. Exact source, patch, metadata and
checksums go into its adjacent JSON manifest. Never use this as organic play.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import tempfile
ROOT=Path(__file__).resolve().parents[2]


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    for name in ('rom','elf','output','generated-engine'):ap.add_argument('--'+name,type=Path,required=True)
    ap.add_argument('--sha256',required=True);ap.add_argument('--elf-sha256',required=True)
    ap.add_argument('--source-revision',required=True)
    ap.add_argument('--scenario',choices=('gift','doubles'),default='gift')
    args=ap.parse_args();original=args.rom.read_bytes();elf=args.elf.read_bytes()
    if hashlib.sha256(original).hexdigest()!=args.sha256 or hashlib.sha256(elf).hexdigest()!=args.elf_sha256:ap.error('frozen artifact checksum mismatch')
    if args.output.exists() or args.output.with_suffix('.json').exists():ap.error('output ROM and manifest must be absent')
    symbols={}
    for line in subprocess.check_output(['arm-none-eabi-nm','-n',str(args.elf)],text=True).splitlines():
        fields=line.split()
        if len(fields)==3:symbols[fields[2]]=int(fields[0],16)
    scene = (('MAP_VERMILION_CITY_POKEMON_CENTER_1F',6,5,'VermilionCity_PokemonCenter_1F_EventScript_VSSeekerWoman') if args.scenario == 'gift' else ('MAP_ROUTE8',40,4,'Route8_EventScript_Eli'))
    target=symbols[scene[3]];offset=target-0x08000000
    if not 0<=offset<len(original):ap.error('target script outside ROM')
    # Confirm a canonical function-table entry pairs the supplied ROM and ELF.
    special=symbols['SPECIAL_ScShowCenterServices'];entry=struct.unpack_from('<I',original,symbols['gSpecials']-0x08000000+4*special)[0]
    if entry&~1 != symbols['ScShowCenterServices']&~1:ap.error('ROM/ELF function-table mismatch')
    code = ("\tlockall\n" if args.scenario == "gift" else "") + "\tsetflag FLAG_BADGE01_GET\n"
    party = [('MEWTWO', 21), ('PIKACHU', 21 if args.scenario == 'gift' else 5)]
    for species, level in party:
        code += (f"\tgivemon SPECIES_{species}, {level}, nature=NATURE_MODEST, gender=0, "
                 "hpIv=15, atkIv=15, defIv=15, speedIv=15, spAtkIv=15, spDefIv=15\n")
    if args.scenario == "doubles":
        code += "\tadditem ITEM_VS_SEEKER, 1\n\tadditem ITEM_REVIVE, 1\n"
    code += ("\tsetflag FLAG_SYS_POKEMON_GET\n"
             f"\twarp {scene[0]}, 255, {scene[1]}, {scene[2]}\n"
             "\twaitstate\n\treleaseall\n\tend\n")
    # Read the same source/config and generated map headers as the frozen build.
    engine=args.generated_engine.resolve();shared=engine;preproc=str(shared/'tools/preproc/preproc')
    prefix=(engine/'data/event_scripts.s').read_text().split('gSpecialVars::')[0]
    with tempfile.TemporaryDirectory(prefix='sc-rematches-fixture-') as folder:
        tmp=Path(folder);wrapper=tmp/'fixture.s'
        wrapper.write_text(prefix+'\n\t.section rematches_fixture, "a", %progbits\n'+f'\t.set ScrCmd_createmon, {symbols["ScrCmd_createmon"]|1}\n'+code)
        first=subprocess.run([preproc,'-s',str(wrapper),'charmap.txt'],cwd=engine,stdout=subprocess.PIPE,check=True)
        cpp=subprocess.run(['arm-none-eabi-cpp','-I','include','-I',str(shared/'include'),'-I',str(shared/'include/constants'),'-I','.','-DMODERN=1','-DTESTING=0','-DFIRERED','-std=gnu17','-'],cwd=engine,input=first.stdout,stdout=subprocess.PIPE,check=True)
        second=subprocess.run([preproc,'-ie',str(wrapper),'charmap.txt'],cwd=engine,input=cpp.stdout,stdout=subprocess.PIPE,check=True)
        subprocess.run(['arm-none-eabi-as','-mcpu=arm7tdmi','-march=armv4t','-meabi=5','--defsym','MODERN=1','--defsym','FIRERED=1','-o',str(tmp/'fixture.o'),'-'],cwd=engine,input=second.stdout,check=True)
        reloc=subprocess.check_output(['arm-none-eabi-readelf','-r',str(tmp/'fixture.o')],text=True)
        if '.relrematches_fixture' in reloc:ap.error('unresolved fixture operand')
        subprocess.run(['arm-none-eabi-objcopy','-O','binary','--only-section=rematches_fixture',str(tmp/'fixture.o'),str(tmp/'fixture.bin')],check=True)
        patch=(tmp/'fixture.bin').read_bytes()
    start=symbols['PalletTown_PlayersHouse_1F_EventScript_Mom'];limit=symbols['PalletTown_PlayersHouse_1F_EventScript_TV']
    if not patch or len(patch)>limit-start:ap.error(f'patch exceeds isolated Mom interaction block: {len(patch)} > {limit-start}')
    at=start-0x08000000;modified=bytearray(original);modified[at:at+len(patch)]=patch
    args.output.parent.mkdir(parents=True,exist_ok=True)
    args.output.write_bytes(modified)
    manifest=dict(provenance='Diagnostic ROM fixture; NOT organic progress or balance evidence',source_revision=args.source_revision,source_sha256=args.sha256,elf_sha256=args.elf_sha256,fixture_sha256=hashlib.sha256(modified).hexdigest(),scenario=args.scenario,target_script=scene[3],target_address=hex(target),patch_address=hex(start),patch_assembly=code,original_bytes=original[at:at+len(patch)].hex(),patch_bytes=patch.hex(),setup=dict(cash='unchanged',pokemon=[dict(species=species,level=level,nature='Modest',ivs=[15]*6,moves='current level defaults') for species,level in party],badges=['Boulder'],rematch_state='unchanged: no trainer victory flags, charge or readiness injected',inventory='unchanged' if args.scenario=='gift' else 'Vs. Seeker and one Revive explicitly granted',map=scene[0],x=scene[1],y=scene[2]),generated_map_header_sha256=hashlib.sha256((shared/'include/constants/map_groups.h').read_bytes()).hexdigest())
    args.output.with_suffix('.json').write_text(json.dumps(manifest,indent=2)+'\n');print(args.output.with_suffix('.json'))

if __name__=='__main__':main()
