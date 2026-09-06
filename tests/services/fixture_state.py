"""Prepare explicit diagnostic writes from actual emulator snapshots.

Input JSON: party_address, party_hex (100 bytes), trainer_address, trainer_hex
(48 bytes). Verifies Pokémon encryption/checksum and the trainer save schema.
Only grants test capabilities/passives, 2000 Practice Points and 3 HP damage.
Writes are never organic game rewards. Execute while the emulator is paused.
"""
import argparse
import json
import struct
from pathlib import Path

SUBSTRUCT0=[0,0,0,0,0,0,1,1,2,3,2,3,1,1,2,3,2,3,1,1,2,3,2,3]
SUBSTRUCT2=[2,3,1,1,3,2,2,3,1,1,3,2,0,0,0,0,0,0,3,2,3,2,1,1]


def crc(data):
    value=0xffff
    for i,byte in enumerate(data):
        if i in (30,31):continue
        value^=byte<<8
        for _ in range(8):value=((value<<1)^0x1021 if value&0x8000 else value<<1)&0xffff
    return value


def prepare(snapshot):
    before_mon=bytes.fromhex(snapshot['party_hex']);before_trainer=bytes.fromhex(snapshot['trainer_hex'])
    if len(before_mon)!=100 or len(before_trainer)!=48:raise ValueError('exact current ABI snapshots required')
    mon=bytearray(before_mon);trainer=bytearray(before_trainer)
    personality,ot=struct.unpack_from('<II',mon)
    key=personality^ot
    words=[word^key for word in struct.unpack_from('<12I',mon,32)]
    plain=bytearray(struct.pack('<12I',*words))
    if sum(struct.unpack('<24H',plain))&0xffff!=struct.unpack_from('<H',mon,28)[0]:raise ValueError('invalid source Pokémon checksum')
    if struct.unpack_from('<H',plain,12*SUBSTRUCT0[personality%24])[0]!=25:raise ValueError('expected diagnostic Pikachu')
    struct.pack_into('<H',plain,12*SUBSTRUCT2[personality%24]+6,0xfff)
    struct.pack_into('<H',mon,28,sum(struct.unpack('<24H',plain))&0xffff)
    struct.pack_into('<12I',mon,32,*[word^key for word in struct.unpack('<12I',plain)])
    max_hp=struct.unpack_from('<H',mon,88)[0]
    if max_hp<=3:raise ValueError('invalid source HP')
    struct.pack_into('<H',mon,86,max_hp-3)
    magic,version,size=struct.unpack_from('<IHH',trainer)
    if (magic,version,size)!=(0x53435931,1,48):raise ValueError('unexpected trainer schema')
    struct.pack_into('<I',trainer,8,255)
    struct.pack_into('<I',trainer,16,2000)
    struct.pack_into('<H',trainer,30,crc(trainer))
    changes=[]
    for label,before,after,address in [('party',before_mon,mon,int(snapshot['party_address'],0)),('trainer',before_trainer,trainer,int(snapshot['trainer_address'],0))]:
        if address&3:raise ValueError('unaligned snapshot')
        for i in range(0,len(after),4):
            if before[i:i+4]!=after[i:i+4]:
                changes.append({'record':label,'address':hex(address+i),'before':before[i:i+4].hex(),'after':after[i:i+4].hex(),'command':f'fixture32 {hex(address+i)} {hex(struct.unpack_from("<I",after,i)[0])}'})
    return {'provenance':'diagnostic-state-injected; NOT organic rewards/progression','changes':changes,'party_after':mon.hex(),'trainer_after':trainer.hex()}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('snapshot',type=Path)
    args=parser.parse_args()
    print(json.dumps(prepare(json.loads(args.snapshot.read_text())),indent=2))

if __name__=='__main__':main()
