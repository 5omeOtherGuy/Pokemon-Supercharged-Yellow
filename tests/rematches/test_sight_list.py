"""Execute the real trainer sight-list function at empty and full boundaries."""
from pathlib import Path
import subprocess
import tempfile
import unittest
from test_objects import function, ROOT

class TrainerSightListTests(unittest.TestCase):
    def test_empty_sparse_and_full_lists_never_visit_an_extra_object(self):
        body = function((ROOT/'engine/src/trainer_see.c').read_text(), 'CheckForTrainersWantingBattle')
        code = '''
#include <assert.h>
#include <stdint.h>
#include <string.h>
typedef uint8_t u8; typedef uint32_t u32; typedef u8 bool8;
#define FALSE 0
#define TRUE 1
#define OBJECT_EVENTS_COUNT 16
#define OW_FLAG_NO_TRAINER_SEE 0
#define TRAINER_TYPE_NORMAL 1
#define TRAINER_TYPE_SEE_ALL_DIRECTIONS 2
#define TRAINER_TYPE_BURIED 3
#define PLAYER_HAS_TWO_USABLE_MONS 2
struct Object {u8 active,trainerType,localId;} gObjectEvents[16];
struct Approach {u32 objectEventId;const u8 *trainerScriptPtr;} gApproachingTrainers[2];
static u8 gNoOfApproachingTrainers,gApproachingTrainerId,gSelectedObjectEvent,gSpecialVar_LastTalked,gTrainerApproachedPlayer;
static int disabled,visited;static u8 previousLocal;
static const u8 EventScript_ObjectApproachPlayer[1];
static int FlagGet(int id){return disabled;}
static u8 CheckTrainer(u8 id){
 assert(id<16 && gObjectEvents[id].active && gObjectEvents[id].trainerType);
 assert(gObjectEvents[id].localId>previousLocal);previousLocal=gObjectEvents[id].localId;visited++;return 0;
}
static const u8 *GetObjectEventScriptPointerByObjectEventId(u32 id){return 0;}
static void ScriptContext_SetupScript(const u8 *s){}
static void LockPlayerFieldControls(void){}
static int GetMonsStateToDoubles_2(void){return PLAYER_HAS_TWO_USABLE_MONS;}
static int InBattlePyramid(void){return 0;}
static int InTrainerHillChallenge(void){return 0;}
static void ConfigureApproachingFacilityTrainerBattle(void *p){}
static void ConfigureApproachingTrainerBattle(void *p){}
''' + body + '''
static void check(unsigned expected){visited=previousLocal=0;assert(!CheckForTrainersWantingBattle());assert(visited==expected);}
int main(void){
 // Full capacity sorts backwards local IDs without reading index16.
 for(unsigned i=0;i<16;i++)gObjectEvents[i]=(struct Object){1,1,16-i};
 check(16);
 memset(gObjectEvents,0,sizeof(gObjectEvents));check(0);
 gObjectEvents[7]=(struct Object){1,1,42};check(1);
 // Active player/non-trainer objects are not appended or accidentally visited.
 gObjectEvents[0]=(struct Object){1,0,1};check(1);
 disabled=1;check(0);
 return 0;
}
'''
        with tempfile.TemporaryDirectory(prefix='sc-sight-list-') as folder:
            d=Path(folder);(d/'probe.c').write_text(code)
            subprocess.run(['gcc','-std=gnu17','-Wall','-Werror','-Wno-unused-parameter',
                            '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                            str(d/'probe.c'),'-o',str(d/'probe')],check=True)
            subprocess.run([str(d/'probe')],check=True)

if __name__ == '__main__':unittest.main()
