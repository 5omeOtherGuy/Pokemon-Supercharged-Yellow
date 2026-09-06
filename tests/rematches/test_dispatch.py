"""Run actual dispatch function against script-stack boundary spies."""
from pathlib import Path
import re
import subprocess
import tempfile
import unittest
from test_objects import function, ROOT

class RematchDispatchTests(unittest.TestCase):
    def test_first_battle_and_declined_double_rematch_have_distinct_safe_paths(self):
        body=function((ROOT/'engine/src/battle_setup.c').read_text(),'BattleSetup_ConfigureTrainerBattle')
        events=sorted(set(re.findall(r'EventSnippet_\w+',body)))
        code='''
#include <assert.h>
#include <stdint.h>
#include <string.h>
typedef uint16_t u16;typedef uint32_t bool32;
#define FIRERED 1
#define FREE_MATCH_CALL 0
#define TRAINER_BATTLE_TYPE_DOUBLES 1
struct ScriptStack {int unused;};
typedef struct {struct {unsigned skipFlagCheck,isRematch,isDoubleBattle,playMusicA,facePlayer,continueScript;u16 opponentA,opponentB;void *introTextA;} params;} TrainerBattleParameter;
static int defeated,ready,twoMons,doubleParty,gNoOfApproachingTrainers;
static unsigned emitted[32],count,resolutions;
static void SetMapVarsToTrainerA(void){}
static int GetTrainerFlag(void){return defeated;}
static int IsTrainerReadyForRematch(void){return ready;}
static int HasEnoughMonsForDoubleBattle2(void){return twoMons;}
static int GetTrainerBattleType(u16 id){return doubleParty;}
static u16 GetRematchTrainerId(u16 id){resolutions++;return id;}
static void emit(unsigned e){assert(count<32);emitted[count++]=e;}
#define PUSH(e) emit(e);
#define PUSH_IF_SET(e,c) if(c)emit(e);
#define PUSH_IF_ELSE(a,b,c) emit((c)?(a):(b));
'''+ 'enum {'+','.join(events)+'};\n'+body+'''
static int contains(unsigned e){for(unsigned i=0;i<count;i++)if(emitted[i]==e)return 1;return 0;}
int main(void){
 TrainerBattleParameter p={0};struct ScriptStack stack={0};p.params.opponentA=382;
 // The first battle opcode keeps the ordinary path; rematch readiness cannot replace it.
 ready=1;twoMons=1;BattleSetup_ConfigureTrainerBattle(&p,&stack);
 assert(contains(EventSnippet_DoTrainerBattle));assert(!contains(EventSnippet_DoRematchTrainerBattle));assert(!resolutions);
 count=0;defeated=1;BattleSetup_ConfigureTrainerBattle(&p,&stack);
 assert(contains(EventSnippet_GotoPostBattleScript));assert(!contains(EventSnippet_DoTrainerBattle));
 // Actual inherited rematch macro supplies isDoubleBattle=0; team data corrects it.
 p.params.isRematch=1;p.params.isDoubleBattle=0;doubleParty=1;twoMons=0;count=0;
 BattleSetup_ConfigureTrainerBattle(&p,&stack);
 assert(contains(EventSnippet_NotEnoughMonsForDoubleBattle));assert(!contains(EventSnippet_DoRematchTrainerBattle));
 assert(!resolutions && ready); // Decline preserves readiness for a valid retry.
 twoMons=1;count=0;BattleSetup_ConfigureTrainerBattle(&p,&stack);
 assert(contains(EventSnippet_DoRematchTrainerBattle));assert(resolutions==1);assert(p.params.opponentA==382);
 ready=0;count=0;BattleSetup_ConfigureTrainerBattle(&p,&stack);
 assert(contains(EventSnippet_GotoPostBattleScript));assert(!contains(EventSnippet_DoRematchTrainerBattle));assert(resolutions==1);
 return 0;
}
'''
        with tempfile.TemporaryDirectory(prefix='sc-rematch-dispatch-') as folder:
            d=Path(folder);p=d/'probe.c';p.write_text(code)
            subprocess.run(['gcc','-std=gnu17','-Wall','-Werror','-Wno-unused-parameter',str(p),'-o',str(d/'probe')],check=True)
            subprocess.run([str(d/'probe')],check=True)

if __name__=='__main__':unittest.main()
