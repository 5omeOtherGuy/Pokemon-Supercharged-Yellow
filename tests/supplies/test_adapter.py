from pathlib import Path
import re
import subprocess
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[2]
FIXTURE=r'''
#include <assert.h>
#include <string.h>
#include "sc_supplies_core.h"
typedef unsigned char u8;typedef unsigned short u16;typedef unsigned int u32;typedef unsigned int bool32;
#include "constants/items.h"
#include "constants/item_effects.h"
#include "constants/opponents_frlg.h"
#define EWRAM_DATA
#define P_SC_KANTO_RULES 1
#define TRUE 1
#define FALSE 0
#define MAX_BATTLE_TRAINERS 4
#define MAX_BATTLERS_COUNT 4
#define MAX_TRAINER_ITEMS 4
#define PARTY_SIZE 6
#define TRAINERS_COUNT 652
#define BATTLE_TYPE_TRAINER 1
#define B_SIDE_PLAYER 0
#define B_TRAINER_PLAYER 0
#define B_TRAINER_OPPONENT_B 2
#define SC_PASSIVE_PREPARATION 5
#define min(a,b) ((a)<(b)?(a):(b))
enum BattlerId {PLAYER_LEFT,ENEMY_LEFT,PLAYER_RIGHT,ENEMY_RIGHT};
struct {struct ScTrainerProgress sc;} save,*gSaveBlock3Ptr=&save;
struct {u32 inBattle;} gMain;
struct {struct {u16 originalItem;} itemLost[4][6];} bs,*gBattleStruct=&bs;
struct {u16 opponentA,opponentB;} params;
#define TRAINER_BATTLE_PARAM params
u32 gBattleTypeFlags, inventory[ITEMS_COUNT], targetUseful=1, npcPassives;
enum Item npcItems[4];
u32 CountTotalItemQuantityInBag(enum Item item){return inventory[item];}
bool32 RemoveBagItem(enum Item item,u16 count){if(inventory[item]<count)return 0;inventory[item]-=count;return 1;}
u32 GetBattlerSide(enum BattlerId battler){return battler&1;}
u32 GetBattlerTrainer(enum BattlerId battler){return battler&1;}
u32 ScGetNpcTrainerBudget(u16 id){return id==TRAINER_LEADER_MISTY?3:0;}
u32 ScGetNpcTrainerPassives(u16 id){return npcPassives;}
const enum Item *GetTrainerItemsFromId(u16 id){return npcItems;}
const u8 *GetItemEffect(enum Item item){
 static const u8 hp[7]={[4]=ITEM4_HEAL_HP},status[7]={[3]=ITEM3_STATUS_ALL},pp[7]={[4]=ITEM4_HEAL_PP|ITEM4_HEAL_PP_ONE},boost[7]={0},mixed[7]={[3]=ITEM3_STATUS_ALL,[4]=ITEM4_HEAL_HP},revive[7]={[4]=ITEM4_REVIVE|ITEM4_HEAL_HP};
 switch(item){case ITEM_POTION:case ITEM_SUPER_POTION:return hp;case ITEM_FULL_HEAL:return status;case ITEM_ETHER:return pp;case ITEM_X_ATTACK:return boost;case ITEM_FULL_RESTORE:return mixed;case ITEM_REVIVE:return revive;default:return 0;}}
u32 GetItemConsumability(enum Item item){return item!=ITEM_POKE_FLUTE;}
u32 GetItemImportance(enum Item item){return 0;}
enum EffectItem GetItemBattleUsage(enum Item item){switch(item){case ITEM_POTION:case ITEM_SUPER_POTION:return EFFECT_ITEM_RESTORE_HP;case ITEM_FULL_HEAL:return EFFECT_ITEM_CURE_STATUS;case ITEM_ETHER:return EFFECT_ITEM_RESTORE_PP;case ITEM_X_ATTACK:return EFFECT_ITEM_INCREASE_STAT;case ITEM_FULL_RESTORE:return EFFECT_ITEM_HEAL_AND_CURE_STATUS;case ITEM_REVIVE:return EFFECT_ITEM_REVIVE;default:return 0;}}
static bool32 TargetHasEffect(enum BattlerId actor,enum Item item){return targetUseful;}
'''
CASES=r'''
int main(void){
 ScInitTrainerProgress(&save.sc);
 u16 items[3]={ITEM_POTION,ITEM_POTION,ITEM_FULL_HEAL};u8 quantities[3]={1,1,1};
 assert(!ScSuppliesSetSelection(items,quantities));
 inventory[ITEM_POTION]=2;inventory[ITEM_FULL_HEAL]=1;
 assert(ScSuppliesSetSelection(items,quantities));assert(inventory[ITEM_POTION]==2);assert(ScValidateTrainerProgress(&save.sc));
 quantities[1]=2;assert(!ScSuppliesSetSelection(items,quantities));assert(save.sc.supplyQuantities[1]==1);quantities[1]=1;
 assert(ScSuppliesCategory(ITEM_FULL_RESTORE)==SC_SUPPLY_INVALID);assert(ScSuppliesCategory(ITEM_REVIVE)==SC_SUPPLY_INVALID);
 assert(ScSuppliesCategory(ITEM_ETHER)==SC_SUPPLY_PP);assert(ScSuppliesCategory(ITEM_X_ATTACK)==SC_SUPPLY_BOOST);
 gBattleTypeFlags=BATTLE_TYPE_TRAINER;params.opponentA=TRAINER_LEADER_MISTY;npcItems[0]=ITEM_SUPER_POTION;
 assert(!ScSuppliesCanUse(PLAYER_LEFT,ITEM_POTION)); /* fail closed before lock */
 assert(ScSuppliesBeginBattle());assert(!ScSuppliesSetSelection(items,quantities));
 assert(ScSuppliesReserve(PLAYER_LEFT,ITEM_POTION));assert(inventory[ITEM_POTION]==2);
 assert(ScSuppliesReserve(PLAYER_RIGHT,ITEM_POTION));assert(inventory[ITEM_POTION]==2);
 ScSuppliesCancel(PLAYER_LEFT);assert(inventory[ITEM_POTION]==2);
 assert(!ScSuppliesCommit(PLAYER_LEFT,ITEM_POTION));
 assert(ScSuppliesReserve(PLAYER_LEFT,ITEM_POTION));
 targetUseful=0;assert(!ScSuppliesCommit(PLAYER_LEFT,ITEM_POTION));assert(inventory[ITEM_POTION]==2);
 targetUseful=1;assert(ScSuppliesReserve(PLAYER_LEFT,ITEM_POTION));assert(ScSuppliesCommit(PLAYER_LEFT,ITEM_POTION));assert(inventory[ITEM_POTION]==1);
 assert(!ScSuppliesCommit(PLAYER_LEFT,ITEM_POTION));
 assert(ScSuppliesCommit(PLAYER_RIGHT,ITEM_POTION));assert(inventory[ITEM_POTION]==0);
 assert(!ScSuppliesReserve(PLAYER_LEFT,ITEM_POTION));
 assert(ScSuppliesReserve(ENEMY_LEFT,ITEM_SUPER_POTION));assert(ScSuppliesCommit(ENEMY_LEFT,ITEM_SUPER_POTION));assert(inventory[ITEM_SUPER_POTION]==0);
 save.sc.activePassives=1u<<SC_PASSIVE_PREPARATION;npcPassives=1u<<SC_PASSIVE_PREPARATION;
 assert(ScSuppliesHealAmount(PLAYER_LEFT,20)==22);assert(ScSuppliesHealAmount(ENEMY_LEFT,20)==22);
 bs.itemLost[0][0].originalItem=ITEM_AIR_BALLOON;ScSuppliesRecordHeldConsumption(0,0,ITEM_AIR_BALLOON);assert(ScSuppliesWasHeldConsumed(0,0,ITEM_AIR_BALLOON));assert(!ScSuppliesWasHeldConsumed(0,0,ITEM_POTION));
 ScSuppliesEndBattle();assert(inventory[ITEM_POTION]==0);assert(!ScSuppliesWasHeldConsumed(0,0,ITEM_AIR_BALLOON));
 params.opponentA=TRAINER_YOUNGSTER_BEN;assert(!ScSuppliesBeginBattle());assert(!ScSuppliesCanUse(ENEMY_LEFT,ITEM_SUPER_POTION));
 ScSuppliesEndBattle();gBattleTypeFlags=0;assert(ScSuppliesBeginBattle());assert(ScSuppliesCanUse(PLAYER_LEFT,ITEM_POKE_BALL));assert(ScSuppliesReserve(PLAYER_LEFT,ITEM_POKE_DOLL));
 return 0;
}
'''
class AdapterTests(unittest.TestCase):
    def test_inventory_lock_revalidation_commit_cancel_npc_and_passive(self):
        source=(ROOT/'engine/src/sc_supplies.c').read_text()
        start=source.index('static bool32 TargetHasEffect(');opening=source.index('{',start);end=opening+1;depth=1
        while depth:depth+=(source[end]=='{')-(source[end]=='}');end+=1
        source=source[:start]+source[end:]
        source='\n'.join(l for l in source.splitlines() if not l.startswith('#include'))
        with tempfile.TemporaryDirectory(prefix='sc-supply-adapter-') as d:
            p=Path(d);(p/'test.c').write_text(FIXTURE+source+CASES)
            cmd=['cc','-std=c11','-Werror','-I'+str(ROOT/'engine/include'),str(p/'test.c'),str(ROOT/'engine/src/sc_supplies_core.c'),str(ROOT/'engine/src/sc_progression_core.c'),'-o',str(p/'test')]
            result=subprocess.run(cmd,capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)
            result=subprocess.run([str(p/'test')],capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)
if __name__=='__main__':unittest.main()
