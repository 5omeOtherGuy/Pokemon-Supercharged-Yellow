"""Run the actual adapter, including execution-time target checks, on host fixtures."""
from pathlib import Path
import subprocess
import tempfile
import unittest
from test_adapter import FIXTURE, ROOT

TARGET_FIXTURE = FIXTURE.replace(
    'struct {struct {u16 originalItem;} itemLost[4][6];}',
    'struct {u8 itemPartyIndex[4],itemMoveIndex[4];struct {u16 originalItem;} itemLost[4][6];}'
).replace('boost[7]={0}', 'boost[7]={[1]=1}').replace(
    'static bool32 TargetHasEffect(enum BattlerId actor,enum Item item){return targetUseful;}', '') + r'''
#define MAX_MON_MOVES 4
#define MOVE_NONE 0
#define NUM_BATTLE_STATS 8
#define MAX_STAT_STAGE 12
#define STAT_ATK 1
#define STAT_SPDEF 5
#define SIDE_STATUS_MIST 1
#define MON_DATA_HP 0
#define MON_DATA_MAX_HP 1
#define MON_DATA_STATUS 2
#define MON_DATA_MOVE1 3
#define MON_DATA_PP1 7
#define MON_DATA_PP_BONUSES 11
struct Pokemon {u32 data[12];} party[2][6];
struct {struct {u32 embargoTimer,confusionTimer,infatuation,focusEnergy,dragonCheer;} volatiles;u8 statStages[8];} gBattleMons[4];
u32 gBattlersCount=4,gBattlerPartyIndexes[4]={0,0,1,1},gSideStatuses[2];
struct Pokemon *GetBattlerParty(enum BattlerId actor){return party[actor&1];}
u32 GetMonData(struct Pokemon *mon,u32 attr){return mon->data[attr];}
u32 GetItemStatus1Mask(enum Item item){return item==ITEM_FULL_HEAL?255:0;}
u32 CalculatePPWithBonus(u32 move,u32 bonus,u32 slot){return 10;}
'''
TARGET_CASES = r'''
static void prepare(enum Item item){
 ScSuppliesEndBattle();
 u16 items[3]={item,0,0};u8 qty[3]={1,0,0};
 inventory[item]=1;assert(ScSuppliesSetSelection(items,qty));
 gBattleTypeFlags=BATTLE_TYPE_TRAINER;assert(ScSuppliesBeginBattle());
 assert(ScSuppliesReserve(PLAYER_LEFT,item));
}
static void denied(enum Item item){assert(!ScSuppliesCommit(PLAYER_LEFT,item));assert(inventory[item]==1);}
static void spent(enum Item item){assert(ScSuppliesCommit(PLAYER_LEFT,item));assert(inventory[item]==0);}
int main(void){
 ScInitTrainerProgress(&save.sc);
 struct Pokemon *mon=&party[0][0];mon->data[MON_DATA_MAX_HP]=100;
 prepare(ITEM_POTION);mon->data[MON_DATA_HP]=100;denied(ITEM_POTION); /* earlier actor healed */
 prepare(ITEM_POTION);mon->data[MON_DATA_HP]=0;denied(ITEM_POTION); /* never revive */
 prepare(ITEM_POTION);mon->data[MON_DATA_HP]=50;gBattleMons[0].volatiles.embargoTimer=1;denied(ITEM_POTION);
 gBattleMons[0].volatiles.embargoTimer=0;prepare(ITEM_POTION);spent(ITEM_POTION);
 prepare(ITEM_POTION);bs.itemPartyIndex[0]=6;denied(ITEM_POTION);bs.itemPartyIndex[0]=0;
 prepare(ITEM_ETHER);denied(ITEM_ETHER); /* empty move slot */
 mon->data[MON_DATA_MOVE1]=1;mon->data[MON_DATA_PP1]=10;prepare(ITEM_ETHER);denied(ITEM_ETHER);
 mon->data[MON_DATA_PP1]=0;prepare(ITEM_ETHER);bs.itemMoveIndex[0]=1;denied(ITEM_ETHER);
 bs.itemMoveIndex[0]=0;prepare(ITEM_ETHER);spent(ITEM_ETHER);
 prepare(ITEM_FULL_HEAL);denied(ITEM_FULL_HEAL);
 mon->data[MON_DATA_STATUS]=1;prepare(ITEM_FULL_HEAL);spent(ITEM_FULL_HEAL);mon->data[MON_DATA_STATUS]=0;
 gBattleMons[0].volatiles.confusionTimer=3;prepare(ITEM_FULL_HEAL);spent(ITEM_FULL_HEAL);gBattleMons[0].volatiles.confusionTimer=0;
 gBattleMons[0].volatiles.infatuation=1;prepare(ITEM_FULL_HEAL);spent(ITEM_FULL_HEAL);gBattleMons[0].volatiles.infatuation=0;
 gBattleMons[0].statStages[STAT_ATK]=12;prepare(ITEM_X_ATTACK);denied(ITEM_X_ATTACK);
 gBattleMons[0].statStages[STAT_ATK]=6;prepare(ITEM_X_ATTACK);spent(ITEM_X_ATTACK);
 /* Bench cures are allowed but an X item cannot target a bench mon. */
 party[0][2].data[MON_DATA_HP]=50;party[0][2].data[MON_DATA_STATUS]=1;
 bs.itemPartyIndex[0]=2;prepare(ITEM_FULL_HEAL);spent(ITEM_FULL_HEAL);
 prepare(ITEM_X_ATTACK);denied(ITEM_X_ATTACK);
 return 0;
}
'''
class TargetTests(unittest.TestCase):
    def test_actual_target_state_is_rechecked_before_spending(self):
        source='\n'.join(l for l in (ROOT/'engine/src/sc_supplies.c').read_text().splitlines() if not l.startswith('#include'))
        with tempfile.TemporaryDirectory(prefix='sc-supply-targets-') as d:
            p=Path(d);(p/'test.c').write_text(TARGET_FIXTURE+source+TARGET_CASES)
            result=subprocess.run(['cc','-std=c11','-Werror','-I'+str(ROOT/'engine/include'),str(p/'test.c'),str(ROOT/'engine/src/sc_supplies_core.c'),str(ROOT/'engine/src/sc_progression_core.c'),'-o',str(p/'test')],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)
            result=subprocess.run([str(p/'test')],capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)
if __name__=='__main__':unittest.main()
