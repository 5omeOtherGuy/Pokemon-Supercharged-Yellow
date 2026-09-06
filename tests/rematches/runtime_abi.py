"""Compile the real ARM layout/constants used by diagnostic RAM observations."""
import argparse,json,subprocess,tempfile,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
FIELDS = {
 'money':'offsetof(struct SaveBlock1, money)',
 'flags':'offsetof(struct SaveBlock1, flags)',
 'charge':'offsetof(struct SaveBlock1, trainerRematchStepCounter)',
 'readiness':'offsetof(struct SaveBlock1, trainerRematches)',
 'key_items':'offsetof(struct SaveBlock1, bag.keyItems)',
 'key_capacity':'BAG_KEYITEMS_COUNT',
 'encryption_key':'offsetof(struct SaveBlock2, encryptionKey)',
 'gift_flag':'FLAG_GOT_VS_SEEKER',
 'charging_flag':'FLAG_SYS_VS_SEEKER_CHARGING',
 'eddie_flag':'TRAINER_FLAGS_START + TRAINER_YOUNGSTER_EDDIE',
 'vs_seeker_item':'ITEM_VS_SEEKER',
 'party_mon_size':'sizeof(struct Pokemon)',
}

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--generated-engine',type=Path,required=True);ap.add_argument('--output',type=Path,required=True);a=ap.parse_args()
 with tempfile.TemporaryDirectory(prefix='rematches-abi-') as d:
  p=Path(d);source='#include "global.h"\n#include "constants/flags.h"\n#include "constants/items.h"\n#include "constants/opponents.h"\n#include "pokemon.h"\nconst unsigned rematchesAbi[] = {'+','.join(FIELDS.values())+'};\n';(p/'abi.c').write_text(source)
  subprocess.run(['arm-none-eabi-gcc','-c','-mabi=apcs-gnu','-mcpu=arm7tdmi','-mthumb','-DFIRERED','-DMODERN=1','-DTESTING=0','-iquote','include','-iquote',str(a.generated_engine.resolve()/'include'),'-iquote',str(a.generated_engine.resolve()/'include/constants'),str(p/'abi.c'),'-o',str(p/'abi.o')],cwd=a.generated_engine.resolve(),check=True)
  subprocess.run(['arm-none-eabi-objcopy','-O','binary','--only-section=.rodata',str(p/'abi.o'),str(p/'abi.bin')],check=True)
  values=struct.unpack('<'+'I'*len(FIELDS),(p/'abi.bin').read_bytes());result={'source':source,'values':dict(zip(FIELDS,values))}
  a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result['values']))
if __name__=='__main__':main()
