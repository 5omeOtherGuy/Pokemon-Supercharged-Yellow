"""Make a checksum-pinned diagnostic ROM copy: Mom gives a test Pikachu and warps.

The test Pokémon is explicitly level5, nature3, male, all IV15. The destination
is the real Viridian Center 2F; its production lounge script opens the services.
Neither the source ROM nor a save is modified. This is not organic progression.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ('rom','elf','output'):parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--sha256',required=True)
    args=parser.parse_args()
    original=args.rom.read_bytes()
    if hashlib.sha256(original).hexdigest()!=args.sha256:parser.error('source checksum mismatch')
    if args.output.exists():parser.error('output must be an absent disposable path')
    symbols={}
    for line in subprocess.check_output(['arm-none-eabi-nm','-n',str(args.elf)],text=True).splitlines():
        fields=line.split()
        if len(fields)==3:symbols[fields[2]]=int(fields[0],16)
    script=symbols['PalletTown_PlayersHouse_1F_EventScript_Mom']
    limit=symbols['PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouMale']
    special=symbols['SPECIAL_ScShowCenterServices']
    function=struct.unpack_from('<I',original,symbols['gSpecials']-0x08000000+special*4)[0]
    if function & ~1 != symbols['ScShowCenterServices'] & ~1:parser.error('ROM and ELF do not match')
    # Exact encoding from engine/asm/macros/event.inc: givemon with explicit
    # nature/gender/IVs; warp map group39,num5, x6,y4; waitstate; releaseall; end.
    patch=b'\x69\x23'+struct.pack('<I',(symbols['ScrCmd_createmon']|1)+0x02000000)
    patch+=struct.pack('<BBHHI',0,6,25,5,0x1f814)
    patch+=struct.pack('<HH6H',3,0,*([15]*6))
    patch+=b'\x39'+struct.pack('<BBBHH',39,5,255,6,4)+b'\x27\x6b\x02'
    offset=script-0x08000000
    if limit-script<len(patch):parser.error('diagnostic script exceeds its target extent')
    modified=bytearray(original);modified[offset:offset+len(patch)]=patch
    with args.output.open('xb') as out:out.write(modified)
    print(json.dumps({'provenance':'diagnostic ROM fixture; NOT organic campaign progress',
        'source_sha256':args.sha256,'elf_sha256':hashlib.sha256(args.elf.read_bytes()).hexdigest(),
        'fixture_sha256':hashlib.sha256(modified).hexdigest(),'script_address':hex(script),
        'original_bytes':original[offset:offset+len(patch)].hex(),'fixture_bytes':patch.hex(),
        'test_mon':{'species':25,'level':5,'nature':3,'gender':0,'ivs':[15]*6},
        'destination':{'mapGroup':39,'mapNum':5,'x':6,'y':4}},indent=2))

if __name__=='__main__':main()
