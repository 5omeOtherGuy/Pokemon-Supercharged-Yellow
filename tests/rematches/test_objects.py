"""Exercise the actual scanner functions with missing and maximum visible objects."""
from pathlib import Path
import re
import subprocess
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[2]

def function(source,name):
    start=re.search(r'(?:static )?[\w *]+ '+name+r'\([^;]*?\)\n\{',source).start()
    brace=source.index('{',start);depth=1;end=brace+1
    while depth:
        depth += (source[end]=='{')-(source[end]=='}');end+=1
    return source[start:end]

class RematchObjectTests(unittest.TestCase):
    def test_actual_scanner_rejects_missing_objects_and_bounds_full_screen(self):
        source=(ROOT/'engine/src/vs_seeker.c').read_text()
        types='\n'.join(re.search(r'struct '+n+r'\n\{.*?\n\};',source,re.S)[0] for n in ('VsSeekerTrainerInfo','VsSeekerStruct'))
        code='''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
typedef uint8_t u8;typedef uint16_t u16;typedef uint32_t u32;typedef int16_t s16;typedef int32_t s32;typedef u8 bool8;typedef u32 bool32;
#define TRUE 1
#define FALSE 0
#define OBJECT_EVENTS_COUNT 16
#define MAX_SPRITES 64
#define MAP_OFFSET 7
#define ARRAY_COUNT(a) (sizeof(a)/sizeof((a)[0]))
#define TRAINER_NONE 0
#define TRAINER_TYPE_NONE 0
#define I_SHOW_NO_ID_TRAINER 0
#define END_TRAINER_INFO 255
#define SCR_OP_TRAINERBATTLE 92
struct ObjectEventTemplate {u8 localId;u8 trainerType;u16 graphicsId;const u8 *script;};
struct ObjectEvent {u8 active,spriteId,localId,mapGroup,mapNum;struct {s16 x,y;} currentCoords;};
struct {struct {u16 mapGroup,mapNum;} location;struct ObjectEventTemplate objectEventTemplates[64];} save,*gSaveBlock1Ptr=&save;
struct {u8 objectEventCount;} events;
struct {void *unused;typeof(events) *events;} gMapHeader={NULL,&events};
struct ObjectEvent gObjectEvents[16];struct {s16 data[8];} gSprites[64];
static void NativeVsSeekerRematchId(void){}
static bool32 Script_MatchesCallNative(const u8 *p,void (*f)(void),bool32 t){return p[0]==1;}
static bool32 TryGetObjectEventIdByLocalIdAndMap(u8 local,u16 num,u16 group,u8 *id){
 if(local==99){*id=255;return 0;}
 if(local<100||local>=116){*id=16;return 1;}
 *id=local-100;return 0;
}
'''+types+'''
static struct VsSeekerStruct state,*sVsSeeker=&state;
'''+function(source,'ObjectEventIdIsSane')+'\n'+function(source,'GetTrainerFlagFromScript')+'\n'+function(source,'GatherNearbyTrainerInfo')+'''
int main(void){
 static const u8 trainer[]={SCR_OP_TRAINERBATTLE,0,0,126,1};
 static const u8 native[]={1,0,0,0,0,126,1};
 save.location.mapGroup=3;save.location.mapNum=2;
 assert(!ObjectEventIdIsSane(16));assert(!ObjectEventIdIsSane(255));
 assert(!GetTrainerFlagFromScript(NULL));
 assert(GetTrainerFlagFromScript(trainer)==382);assert(GetTrainerFlagFromScript(native)==382);
 events.objectEventCount=64;
 // Offscreen templates precede every live object; local IDs need not be <= count.
 for(u32 i=0;i<64;i++){save.objectEventTemplates[i].localId=1;save.objectEventTemplates[i].script=trainer;}
 save.objectEventTemplates[0].localId=99;
 for(u32 i=0;i<16;i++){
  gObjectEvents[i].active=1;gObjectEvents[i].spriteId=i;gObjectEvents[i].localId=100+i;
  gObjectEvents[i].mapGroup=3;gObjectEvents[i].mapNum=2;gSprites[i].data[0]=i;
  save.objectEventTemplates[48+i].localId=100+i;save.objectEventTemplates[48+i].graphicsId=600+i;
 }
 GatherNearbyTrainerInfo();
 for(u32 i=0;i<16;i++){assert(state.trainerInfo[i].localId==100+i);assert(state.trainerInfo[i].graphicsId==600+i);assert(state.trainerInfo[i].trainerIdx==382);}
 assert(state.trainerInfo[16].localId==END_TRAINER_INFO);
 // Dead, stale-map and invalid sprite objects cannot enter the scan.
 gObjectEvents[0].active=0;gObjectEvents[1].mapNum=9;gObjectEvents[2].spriteId=255;
 memset(&state,0,sizeof(state));GatherNearbyTrainerInfo();assert(state.trainerInfo[13].localId==END_TRAINER_INFO);
 // Corrupt template count still cannot exceed the actual template array.
 events.objectEventCount=255;GatherNearbyTrainerInfo();assert(state.trainerInfo[13].localId==END_TRAINER_INFO);
 return 0;
}
'''
        with tempfile.TemporaryDirectory(prefix='sc-rematch-objects-') as folder:
            d=Path(folder);p=d/'probe.c';p.write_text(code)
            subprocess.run(['gcc','-std=gnu17','-Wall','-Werror','-Wno-unused-parameter','-fsanitize=undefined,bounds',str(p),'-o',str(d/'probe')],check=True)
            subprocess.run([str(d/'probe')],check=True)

if __name__=='__main__':unittest.main()
