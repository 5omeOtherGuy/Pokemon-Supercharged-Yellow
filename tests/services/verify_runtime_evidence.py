"""Check recorded diagnostic observations; does not replay or prove the campaign."""
import hashlib
import json
from pathlib import Path
import struct
from fixture_state import crc

ROOT=Path(__file__).resolve().parent/'evidence'/'195bd43e'

def main():
    manifest=json.loads((ROOT/'manifest.json').read_text())
    for name,digest in manifest['files'].items():
        assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==digest,name
    records=[json.loads(x) for x in (ROOT/'responses.jsonl').read_text().splitlines()]
    commands=(ROOT/'commands.txt').read_text().splitlines()
    assert len(records)==len(commands)+1
    assert all(x['ok'] for x in records)
    observations={};frame=0
    for record in records:
        frame=record.get('frames',frame)
        if 'address' in record:
            observations[(frame,record['address'])]=bytes.fromhex(record['hex'])
    def pokemon(frame):
        mon=observations[(frame,0x02031c7c)]
        personality,ot=struct.unpack_from('<II',mon)
        assert personality%24==7 # The recorded fixture's authoritative substructure permutation.
        plain=struct.pack('<12I',*[v^personality^ot for v in struct.unpack_from('<12I',mon,32)])
        assert sum(struct.unpack('<24H',plain))&0xffff==struct.unpack_from('<H',mon,28)[0]
        assert not mon[19]&1
        ivs=struct.unpack_from('<I',plain,28)[0]
        return dict(raw=mon,ivs=[(ivs>>(5*i))&31 for i in range(6)],hp=struct.unpack_from('<HH',mon,86),
            nature=(personality%25+(mon[18]>>3))%25,ability=(struct.unpack_from('<I',plain,32)[0]>>29)&3,
            evs=list(plain[36:42]),earned=struct.unpack_from('<H',plain,42)[0],active=struct.unpack_from('<H',plain,44)[0],focus=plain[46],
            moves=[struct.unpack_from('<H',plain,2*i)[0]&0x7ff for i in range(4)],pps=[v&127 for v in plain[8:12]])
    initial=pokemon(17347);improved=pokemon(20099);relearned=pokemon(26971)
    assert initial['ivs']==[15]*6 and improved['ivs']==[16,15,15,15,15,15]
    assert improved['hp']==(16,19) and improved['nature']==initial['nature']==3
    assert improved['ability']==2 and improved['evs']==[0]*6
    assert (improved['earned'],improved['active'],improved['focus'])==(4095,3,1)
    assert pokemon(23581)['raw']==pokemon(23683)['raw'] # A on field effect never assigns.
    assert pokemon(23581)['moves'][0]==0 and pokemon(23581)['pps'][0]==0
    assert relearned['moves']==initial['moves'] and relearned['pps']==initial['pps']
    trainers={frame:record for (frame,address),record in observations.items() if address==0x020073c0 and len(record)==48}
    assert struct.unpack_from('<I',trainers[20099],16)[0]==1980
    for record in trainers.values():assert crc(record)==struct.unpack_from('<H',record,30)[0]
    assert struct.unpack_from('<I',trainers[27895],12)[0]==1
    assert struct.unpack_from('<3H',trainers[28399],20)==(28,0,0) and trainers[28399][26:29]==bytes([2,0,0])
    assert struct.unpack_from('<3H',trainers[28525],20)==(28,43,0) and trainers[28525][26:29]==bytes([2,1,0])
    assert trainers[28567][20:29]==bytes(9)
    assert observations[(28525,0x020104c4)]==bytes.fromhex('1c0057da2b0056da210055da')
    print(f'{len(commands)} successful diagnostic commands; {manifest["png_count"]} pinned native screenshots; observed IV, focus, effect, relearn, checksum and supply invariants passed.')

if __name__=='__main__':main()
