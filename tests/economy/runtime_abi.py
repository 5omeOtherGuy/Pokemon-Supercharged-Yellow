"""Compile the real ARM layout/constants used by diagnostic RAM observations."""
import argparse,json,subprocess,tempfile,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
FIELDS={'money':'offsetof(struct SaveBlock1, money)','flags':'offsetof(struct SaveBlock1, flags)','encryption_key':'offsetof(struct SaveBlock2, encryptionKey)','badge04':'FLAG_BADGE04_GET','vs_seeker_flag':'FLAG_GOT_VS_SEEKER','vs_seeker_item':'ITEM_VS_SEEKER','key_capacity':'BAG_KEYITEMS_COUNT','focus_sash':'ITEM_FOCUS_SASH','light_ball':'ITEM_LIGHT_BALL','sitrus':'ITEM_SITRUS_BERRY','air_balloon':'ITEM_AIR_BALLOON'}
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--generated-engine',type=Path,required=True);ap.add_argument('--output',type=Path,required=True);a=ap.parse_args()
 with tempfile.TemporaryDirectory(prefix='economy-abi-') as d:
  p=Path(d);source='#include "global.h"\n#include "constants/flags.h"\n#include "constants/items.h"\nconst unsigned economyAbi[] = {'+','.join(FIELDS.values())+'};\n';(p/'abi.c').write_text(source)
  subprocess.run(['arm-none-eabi-gcc','-c','-mabi=apcs-gnu','-mcpu=arm7tdmi','-mthumb','-DFIRERED','-DMODERN=1','-DTESTING=0','-iquote','include','-iquote',str(a.generated_engine.resolve()/'include'),'-iquote',str(a.generated_engine.resolve()/'include/constants'),str(p/'abi.c'),'-o',str(p/'abi.o')],cwd=ROOT/'engine',check=True)
  subprocess.run(['arm-none-eabi-objcopy','-O','binary','--only-section=.rodata',str(p/'abi.o'),str(p/'abi.bin')],check=True)
  values=struct.unpack('<'+'I'*len(FIELDS),(p/'abi.bin').read_bytes());result={'source':source,'values':dict(zip(FIELDS,values))}
  a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result['values']))
if __name__=='__main__':main()
