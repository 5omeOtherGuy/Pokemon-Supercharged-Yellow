"""Compile an explicit diagnostic Mom-warp fixture against a frozen ROM/ELF.

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
SCENES={
 'celadon':('MAP_CELADON_CITY_DEPARTMENT_STORE_5F',3,6,'CeladonCity_DepartmentStore_5F_EventScript_ClerkVitamins'),
 'berries':('MAP_CERULEAN_CITY_HOUSE5',8,4,'CeruleanCity_House5_EventScript_BerryPowderMan'),
 'gift':('MAP_VERMILION_CITY_POKEMON_CENTER_1F',6,5,'VermilionCity_PokemonCenter_1F_EventScript_VSSeekerWoman'),
 'rick':('MAP_VIRIDIAN_FOREST',47,46,'ViridianForest_EventScript_Rick'),
 'ethan':('MAP_ROUTE24',12,20,'ScEconomy_Route24_Ethan'),
 'indigo':('MAP_INDIGO_PLATEAU_POKEMON_CENTER_1F',2,7,'IndigoPlateau_PokemonCenter_1F_EventScript_Clerk'),
}

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    for name in ('rom','elf','output','generated-engine'):ap.add_argument('--'+name,type=Path,required=True)
    ap.add_argument('--sha256',required=True);ap.add_argument('--elf-sha256',required=True)
    ap.add_argument('--scenario',choices=SCENES,required=True);ap.add_argument('--source-revision',required=True)
    args=ap.parse_args();original=args.rom.read_bytes();elf=args.elf.read_bytes()
    if hashlib.sha256(original).hexdigest()!=args.sha256 or hashlib.sha256(elf).hexdigest()!=args.elf_sha256:ap.error('frozen artifact checksum mismatch')
    if args.output.exists() or args.output.with_suffix('.json').exists():ap.error('output ROM and manifest must be absent')
    symbols={}
    for line in subprocess.check_output(['arm-none-eabi-nm','-n',str(args.elf)],text=True).splitlines():
        fields=line.split()
        if len(fields)==3:symbols[fields[2]]=int(fields[0],16)
    scene=SCENES[args.scenario];target=symbols[scene[3]];offset=target-0x08000000
    if not 0<=offset<len(original):ap.error('target script outside ROM')
    # Confirm a canonical function-table entry pairs the supplied ROM and ELF.
    special=symbols['SPECIAL_ScShowCenterServices'];entry=struct.unpack_from('<I',original,symbols['gSpecials']-0x08000000+4*special)[0]
    if entry&~1 != symbols['ScShowCenterServices']&~1:ap.error('ROM/ELF function-table mismatch')
    battle=args.scenario in ('rick','ethan')
    code='\tlockall\n'
    if battle:
        code+='\tremovemoney 999999\n\tgivemon SPECIES_MEWTWO, 15, nature=NATURE_MODEST, gender=0, hpIv=15, atkIv=15, defIv=15, speedIv=15, spAtkIv=15, spDefIv=15\n\tsetflag FLAG_SYS_POKEMON_GET\n'
    else:code+='\taddmoney 10000\n'
    code+=f'\twarp {scene[0]}, 255, {scene[1]}, {scene[2]}\n\twaitstate\n\treleaseall\n\tend\n'
    engine=ROOT/'engine';shared=args.generated_engine.resolve();preproc=str(shared/'tools/preproc/preproc')
    prefix=(engine/'data/event_scripts.s').read_text().split('gSpecialVars::')[0]
    with tempfile.TemporaryDirectory(prefix='sc-economy-fixture-') as folder:
        tmp=Path(folder);wrapper=tmp/'fixture.s'
        wrapper.write_text(prefix+'\n\t.section economy_fixture, "a", %progbits\n'+f'\t.set ScrCmd_createmon, {symbols["ScrCmd_createmon"]|1}\n'+code)
        first=subprocess.run([preproc,'-s',str(wrapper),'charmap.txt'],cwd=engine,stdout=subprocess.PIPE,check=True)
        cpp=subprocess.run(['arm-none-eabi-cpp','-I','include','-I',str(shared/'include'),'-I',str(shared/'include/constants'),'-I','.','-DMODERN=1','-DTESTING=0','-DFIRERED','-std=gnu17','-'],cwd=engine,input=first.stdout,stdout=subprocess.PIPE,check=True)
        second=subprocess.run([preproc,'-ie',str(wrapper),'charmap.txt'],cwd=engine,input=cpp.stdout,stdout=subprocess.PIPE,check=True)
        subprocess.run(['arm-none-eabi-as','-mcpu=arm7tdmi','-march=armv4t','-meabi=5','--defsym','MODERN=1','--defsym','FIRERED=1','-o',str(tmp/'fixture.o'),'-'],cwd=engine,input=second.stdout,check=True)
        reloc=subprocess.check_output(['arm-none-eabi-readelf','-r',str(tmp/'fixture.o')],text=True)
        if '.releconomy_fixture' in reloc:ap.error('unresolved fixture operand')
        subprocess.run(['arm-none-eabi-objcopy','-O','binary','--only-section=economy_fixture',str(tmp/'fixture.o'),str(tmp/'fixture.bin')],check=True)
        patch=(tmp/'fixture.bin').read_bytes()
    start=symbols['PalletTown_PlayersHouse_1F_EventScript_Mom'];limit=symbols['PalletTown_PlayersHouse_1F_EventScript_TV']
    if not patch or len(patch)>limit-start:ap.error('patch exceeds isolated Mom interaction block')
    at=start-0x08000000;modified=bytearray(original);modified[at:at+len(patch)]=patch
    args.output.parent.mkdir(parents=True,exist_ok=True)
    args.output.write_bytes(modified)
    manifest=dict(provenance='Diagnostic ROM fixture; NOT organic progress or balance evidence',source_revision=args.source_revision,source_sha256=args.sha256,elf_sha256=args.elf_sha256,fixture_sha256=hashlib.sha256(modified).hexdigest(),scenario=args.scenario,target_script=scene[3],target_address=hex(target),patch_address=hex(start),patch_assembly=code,original_bytes=original[at:at+len(patch)].hex(),patch_bytes=patch.hex(),setup=dict(cash='zero' if battle else 'initial plus10000',pokemon='Mewtwo Lv15, Modest, IV15 in all stats, legal default moves' if battle else 'none',map=scene[0],x=scene[1],y=scene[2]),generated_map_header_sha256=hashlib.sha256((shared/'include/constants/map_groups.h').read_bytes()).hexdigest())
    args.output.with_suffix('.json').write_text(json.dumps(manifest,indent=2)+'\n');print(args.output.with_suffix('.json'))

if __name__=='__main__':main()
