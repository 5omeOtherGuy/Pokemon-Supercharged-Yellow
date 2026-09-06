"""Host-run the actual Kanto state adapter, with deterministic engine boundary data."""
from pathlib import Path
import re
import subprocess
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[2]

class RematchRuntimeTests(unittest.TestCase):
    def test_actual_adapter_bounds_stage_and_ready_state(self):
        opponents=(ROOT/'engine/include/constants/opponents_frlg.h').read_text()
        definitions={k:int(v) for k,v in re.findall(r'^#define (TRAINER_\w+)\s+(\d+)\s*$',opponents,re.M)}
        rows=re.findall(r'SC_REMATCH\((TRAINER_\w+), (MAP_\w+)\)',(ROOT/'engine/src/data/rematches_frlg.h').read_text())
        maps=list(dict.fromkeys(m for t,m in rows))
        with tempfile.TemporaryDirectory(prefix='sc-rematches-host-') as folder:
            d=Path(folder);(d/'constants').mkdir()
            (d/'sc_rematches.h').write_text((ROOT/'engine/include/sc_rematches.h').read_text())
            (d/'global.h').write_text('''#ifndef TEST_GLOBAL_H
#define TEST_GLOBAL_H
#include <stdint.h>
#include <stddef.h>
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32; typedef int32_t s32; typedef u32 bool32;
#define TRUE 1
#define FALSE 0
#define TRAINER_NONE 0
#define TRAINERS_COUNT 652
#define PARTY_SIZE 6
#define MAX_REMATCH_ENTRIES 100
#define REMATCH_TABLE_ENTRIES 78
#define FLAG_IS_CHAMPION 100
#define FLAG_SYS_VS_SEEKER_CHARGING 101
#define SPECIES_BULBASAUR 1
#define SPECIES_MEW 151
struct SaveBlock1 { struct { u16 mapGroup,mapNum; } location; u32 before; u8 trainerRematches[100]; u32 after; };
extern struct SaveBlock1 *gSaveBlock1Ptr;
#endif
''')
            (d/'battle_setup.h').write_text('''#include "global.h"
struct RematchTrainer { u16 trainerIds[5];u16 mapGroup,mapNum; };
extern const struct RematchTrainer gRematchTable[78];
s32 FirstBattleTrainerIdToRematchTableId(const struct RematchTrainer *,u16);
bool32 HasTrainerBeenFought(u16);
''')
            (d/'data.h').write_text('''#include "global.h"
struct TrainerMon { u16 species; u8 lvl; };
u8 GetTrainerPartySizeFromId(u16);
const struct TrainerMon *GetTrainerPartyFromId(u16);
''')
            (d/'event_data.h').write_text('#include "global.h"\nbool32 FlagGet(u16);\n')
            (d/'item.h').write_text('#include "global.h"\nbool32 CheckBagHasItem(u16,u16);\n')
            (d/'sc_progression.h').write_text('#include "sc_progression_core.h"\nu32 ScGetBadgeCount(void);\n')
            (d/'constants/items.h').write_text('#define ITEM_VS_SEEKER 1\n')
            (d/'constants/maps.h').write_text('')
            defines='\n'.join(f'#define {t} {definitions[t]}' for t,m in rows)+'\n'+'\n'.join(f'#define {m} {i+1}' for i,m in enumerate(maps))
            probe=d/'probe.c'
            probe.write_text(defines+'''
#include "global.h"
#include "battle_setup.h"
#include "data.h"
#include "sc_rematches.h"
#include "sc_progression_core.h"
#include <assert.h>
#include <string.h>
const struct RematchTrainer gRematchTable[78]={
#define SC_REMATCH(t,m) {{t,t,t,t,t},1,m},
#include "data/rematches_frlg.h"
#undef SC_REMATCH
};
static struct SaveBlock1 save;
struct SaveBlock1 *gSaveBlock1Ptr=&save;
static u32 badges,champion,owned,charging,fought[652],size=1;
static struct TrainerMon mon={1,11};
bool32 FlagGet(u16 f){return f==100?champion:charging;}
bool32 CheckBagHasItem(u16 i,u16 q){return owned;}
bool32 HasTrainerBeenFought(u16 t){assert(t<652);return fought[t];}
u32 ScGetBadgeCount(void){return badges;}
u8 GetTrainerPartySizeFromId(u16 t){assert(t<652);return size;}
const struct TrainerMon *GetTrainerPartyFromId(u16 t){assert(t<652);return &mon;}
s32 FirstBattleTrainerIdToRematchTableId(const struct RematchTrainer *t,u16 id){if(!id)return -1;for(u32 i=0;i<78;i++)if(t[i].trainerIds[0]==id)return i;return -1;}
int main(void){
 save.before=0x12345678;save.after=0x87654321;
 owned=charging=1;
 for(u32 i=0;i<78;i++){
  u16 t=gRematchTable[i].trainerIds[0]; if(!t)continue;
  save.location.mapGroup=1;save.location.mapNum=gRematchTable[i].mapNum;
  assert(ScRematchIsMainlandMap(1,save.location.mapNum));
  assert(!ScRematchResolve(t)); assert(!ScRematchSetReady(t));
  fought[t]=1;
  for(badges=0;badges<9;badges++)for(champion=0;champion<2;champion++){
   mon.lvl=ScLevelCap(badges,champion);
   assert(ScRematchResolve(t)==t); assert(ScRematchSetReady(t));
   assert(save.trainerRematches[i]==1);assert(ScRematchIsReady(t));
   mon.lvl++; assert(!ScRematchResolve(t));assert(!ScRematchIsReady(t));
   mon.lvl--; save.trainerRematches[i]=255;assert(!ScRematchIsReady(t));
   ScRematchClearReady(t);assert(save.trainerRematches[i]==0);
  }
  mon.lvl=11;mon.species=152; assert(!ScRematchResolve(t));mon.species=1;
  size=0;assert(!ScRematchResolve(t));size=7;assert(!ScRematchResolve(t));size=1;
  mon.lvl=0;assert(!ScRematchResolve(t));mon.lvl=11;
  owned=0;assert(!ScRematchSetReady(t));owned=1;
  assert(ScRematchSetReady(t));charging=0;assert(!ScRematchIsReady(t));charging=1;
  save.location.mapGroup=2;assert(!ScRematchIsReady(t));save.location.mapGroup=1;
  ScRematchClearReady(t);assert(!ScRematchIsReady(t));fought[t]=0;
 }
 assert(!ScRematchIsMainlandMap(65535,65535));
 for(u32 t=0;t<65536;t++){
  assert(!ScRematchSetReady(t));assert(!ScRematchIsReady(t));ScRematchClearReady(t);
 }
 assert(save.before==0x12345678 && save.after==0x87654321);
 for(u32 i=0;i<100;i++)assert(!save.trainerRematches[i]);
 return 0;
}
''')
            executable=d/'probe'
            subprocess.run(['gcc','-std=c11','-Wall','-Werror','-Wno-unused-parameter','-DFIRERED','-fsanitize=undefined,bounds','-I'+str(d),'-I'+str(ROOT/'engine/include'),'-I'+str(ROOT/'engine/src'),str(probe),str(ROOT/'engine/src/sc_rematches.c'),str(ROOT/'engine/src/sc_progression_core.c'),'-o',str(executable)],check=True,text=True)
            subprocess.run([str(executable)],check=True,text=True)

if __name__=='__main__': unittest.main()
