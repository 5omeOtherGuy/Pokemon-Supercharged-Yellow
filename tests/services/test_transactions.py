"""Execute production service transactions against narrow native game adapters."""
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]

class ServicesTests(unittest.TestCase):
    def test_ownership_costs_context_and_permanent_data(self):
        with tempfile.TemporaryDirectory(prefix='sc-services-') as name:
            tmp=Path(name)
            (tmp/'global.h').write_text('''
#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32; typedef int32_t s32; typedef uint32_t bool32;
#define TRUE 1
#define FALSE 0
#define ARRAY_COUNT(a) (sizeof(a)/sizeof((a)[0]))
#define PARTY_SIZE 6
#define B_TRAINER_PLAYER 0
#define MAX_MON_MOVES 4
#define COMPOUND_STRING(s) ((const u8 *)(s))
#include "sc_progression_core.h"
struct SaveBlock1 { struct {u8 mapGroup,mapNum;} location; };
struct SaveBlock3 {struct ScTrainerProgress sc;};
extern struct SaveBlock1 *gSaveBlock1Ptr;
extern struct SaveBlock3 *gSaveBlock3Ptr;
#endif
''')
            (tmp/'main.h').write_text('struct Main {u8 inBattle;}; extern struct Main gMain;\n')
            (tmp/'pokemon.h').write_text('''
#ifndef POKEMON_H
#define POKEMON_H
#define SPECIES_NONE 0
#define SPECIES_BULBASAUR 1
#define SPECIES_PIKACHU 25
#define SPECIES_MEW 151
#define ABILITY_NONE 0
#define MOVE_NONE 0
#define LEVEL_UP_MOVE_END 65535
#define MAX_LEVEL 100
#define MOVES_COUNT_ALL 1000
enum {MON_DATA_SPECIES,MON_DATA_IS_EGG,MON_DATA_HP,MON_DATA_MAX_HP,MON_DATA_LEVEL,MON_DATA_ABILITY_NUM,MON_DATA_NATURE,MON_DATA_HP_IV,MON_DATA_ATK_IV,MON_DATA_DEF_IV,MON_DATA_SPEED_IV,MON_DATA_SPATK_IV,MON_DATA_SPDEF_IV,MON_DATA_MOVE1,MON_DATA_MOVE2,MON_DATA_MOVE3,MON_DATA_MOVE4,MON_DATA_PP_BONUSES,MON_DATA_HP_EV};
struct Pokemon {u32 data[32];struct ScMonProgress progress;};
struct LevelUpMove {u16 move,level;};
struct SpeciesInfo {u16 abilities[3]; const struct LevelUpMove *levelUpLearnset;};
extern struct Pokemon gParties[1][6];
extern struct SpeciesInfo gSpeciesInfo[152];
u32 GetMonData(struct Pokemon*,u32);
void SetMonData(struct Pokemon*,u32,const void*);
void CalculateMonStats(struct Pokemon*);
void SetMonMoveSlot(struct Pokemon*,u16,u8);
void RemoveMonPPBonus(struct Pokemon*,u8);
static inline const struct LevelUpMove *GetSpeciesLevelUpLearnset(u16 species) {return gSpeciesInfo[species].levelUpLearnset;}
#endif
''')
            (tmp/'constants').mkdir()
            names=['VIRIDIAN_CITY','PEWTER_CITY','CERULEAN_CITY','VERMILION_CITY','LAVENDER_TOWN','CELADON_CITY','FUCHSIA_CITY','CINNABAR_ISLAND','SAFFRON_CITY','INDIGO_PLATEAU','ROUTE4','ROUTE10']
            (tmp/'constants/map_groups.h').write_text('\n'.join(f'#define MAP_{n}_POKEMON_CENTER_2F {5 | ((39+i)<<8)}' for i,n in enumerate(names)))
            (tmp/'probe.c').write_text('''
#include "global.h"
#include "main.h"
#include "pokemon.h"
#include "sc_services.h"
#include "sc_build_catalog.h"
#include <assert.h>
struct Main gMain;
struct SaveBlock1 save1,*gSaveBlock1Ptr=&save1;
struct SaveBlock3 save3,*gSaveBlock3Ptr=&save3;
struct Pokemon gParties[1][6];
struct SpeciesInfo gSpeciesInfo[152];
const struct ScPassiveInfo gScTrainerPassiveInfo[8]={{0,0,2},{0,0,2},{0,0,2},{0,0,3},{0,0,3},{0,0,2},{0,0,2},{0,0,3}};
u32 badges;
bool32 ScProgressionEnabled(void) {return TRUE;}
u32 ScGetBadgeCount(void) {return badges;}
void ScReadMonProgress(struct Pokemon *mon,struct ScMonProgress *p) {*p=mon->progress;}
void ScWriteMonProgress(struct Pokemon *mon,const struct ScMonProgress *p) {mon->progress=*p;}
u32 GetMonData(struct Pokemon *mon,u32 field) {return mon->data[field];}
void SetMonData(struct Pokemon *mon,u32 field,const void *p) {mon->data[field]=*(const u32*)p;}
void CalculateMonStats(struct Pokemon *mon) {mon->data[MON_DATA_MAX_HP]=20+mon->data[MON_DATA_HP_IV];}
void SetMonMoveSlot(struct Pokemon *mon,u16 move,u8 slot) {mon->data[MON_DATA_MOVE1+slot]=move;}
void RemoveMonPPBonus(struct Pokemon *mon,u8 slot) {mon->data[MON_DATA_PP_BONUSES]&=~(3u<<(slot*2));}
static const struct LevelUpMove learnset[]={{10,0},{11,1},{12,5},{11,6},{13,20},{65535,0}};
int main(void) {
 struct Pokemon *mon=&gParties[0][0], before; struct ScTrainerProgress trainerBefore;
 ScInitTrainerProgress(&save3.sc); save1.location.mapGroup=39; save1.location.mapNum=5;
 mon->data[MON_DATA_SPECIES]=25; mon->data[MON_DATA_LEVEL]=5; mon->data[MON_DATA_HP_IV]=15;
 mon->data[MON_DATA_HP]=28;mon->data[MON_DATA_MAX_HP]=35;mon->data[MON_DATA_NATURE]=3;
 mon->data[MON_DATA_HP_EV]=17; mon->progress.focus=6; mon->progress.earnedCapabilities=7;
 gSpeciesInfo[25]=(struct SpeciesInfo){{9,0,31},learnset};
 assert(ScServicesAtCenter()); assert(ScServicesValidMon(0)); assert(!ScServicesValidMon(6));
 assert(ScServicesSetCapabilities(0,3)==SC_SERVICE_OK); assert(mon->progress.activeCapabilities==3);
 before=*mon; assert(ScServicesSetCapabilities(0,7)==SC_SERVICE_OVER_BUDGET);assert(memcmp(&before,mon,sizeof(before))==0);
 assert(ScServicesSetCapabilities(0,8)==SC_SERVICE_NOT_EARNED);assert(ScServicesSetCapabilities(0,65536)==SC_SERVICE_INVALID);
 assert(ScServicesSetCapabilities(0,3)==SC_SERVICE_NO_CHANGE);
 save3.sc.unlockedPassives=255; assert(ScServicesSetTrainerPassives(1)==SC_SERVICE_OK);
 trainerBefore=save3.sc; assert(ScServicesSetTrainerPassives(3)==SC_SERVICE_OVER_BUDGET);assert(memcmp(&trainerBefore,&save3.sc,sizeof(trainerBefore))==0);
 assert(ScServicesSetTrainerPassives(256)==SC_SERVICE_INVALID); save3.sc.unlockedPassives=1;assert(ScServicesSetTrainerPassives(2)==SC_SERVICE_NOT_EARNED);
 save1.location.mapGroup=99;before=*mon;
 assert(ScServicesSetCapabilities(0,0)==SC_SERVICE_BAD_CONTEXT);assert(ScServicesSetAbility(0,2)==SC_SERVICE_BAD_CONTEXT);
 assert(ScServicesImproveIv(0,0)==SC_SERVICE_BAD_CONTEXT);assert(ScServicesTeachMove(0,12,0)==SC_SERVICE_BAD_CONTEXT);
 assert(memcmp(&before,mon,sizeof(before))==0);assert(ScServicesSetFocus(0,3)==SC_SERVICE_OK);assert(mon->data[MON_DATA_HP_EV]==17);
 assert(ScServicesSetFocus(0,7)==SC_SERVICE_INVALID);
 gMain.inBattle=1;before=*mon;assert(ScServicesSetFocus(0,2)==SC_SERVICE_BAD_CONTEXT);assert(memcmp(&before,mon,sizeof(before))==0);
 gMain.inBattle=0;save1.location.mapGroup=39; save1.location.mapNum=5;
 assert(ScServicesSetAbility(0,1)==SC_SERVICE_INVALID);assert(ScServicesSetAbility(0,2)==SC_SERVICE_OK);assert(mon->data[MON_DATA_ABILITY_NUM]==2);
 assert(mon->data[MON_DATA_HP]==28 && mon->data[MON_DATA_NATURE]==3);
 assert(ScServicesIvCost(1)==20 && ScServicesIvCost(10)==40 && ScServicesIvCost(100)==400);
 save3.sc.practiceExp=19;before=*mon;assert(ScServicesImproveIv(0,0)==SC_SERVICE_INSUFFICIENT_PRACTICE);assert(memcmp(&before,mon,sizeof(before))==0);
 save3.sc.practiceExp=20;assert(ScServicesImproveIv(0,0)==SC_SERVICE_OK);assert(save3.sc.practiceExp==0 && mon->data[MON_DATA_HP_IV]==16);
 assert(mon->data[MON_DATA_MAX_HP]==36 && mon->data[MON_DATA_HP]==29 && mon->data[MON_DATA_NATURE]==3);
 mon->data[MON_DATA_HP_IV]=31;save3.sc.practiceExp=100;assert(ScServicesImproveIv(0,0)==SC_SERVICE_MAX_IV);assert(save3.sc.practiceExp==100);
 mon->data[MON_DATA_HP_IV]=15;mon->data[MON_DATA_HP]=0;assert(ScServicesImproveIv(0,0)==SC_SERVICE_OK);assert(mon->data[MON_DATA_HP]==0);
 u16 moves[64];assert(ScServicesRelearnMoves(0,moves,64)==3);assert(moves[0]==10 && moves[1]==11 && moves[2]==12);
 assert(ScServicesTeachMove(0,13,0)==SC_SERVICE_INVALID);assert(ScServicesTeachMove(0,12,4)==SC_SERVICE_INVALID);
 mon->data[MON_DATA_PP_BONUSES]=255;assert(ScServicesTeachMove(0,12,1)==SC_SERVICE_OK);assert(mon->data[MON_DATA_MOVE2]==12 && mon->data[MON_DATA_PP_BONUSES]==243);
 assert(ScServicesTeachMove(0,12,0)==SC_SERVICE_NO_CHANGE);assert(ScServicesRelearnMoves(0,moves,64)==2);
 mon->data[MON_DATA_IS_EGG]=1;assert(!ScServicesValidMon(0));assert(ScServicesSetFocus(0,1)==SC_SERVICE_BAD_MON);
 return 0;
}
''')
            sources=[ROOT/'engine/src/sc_progression_core.c']
            model=ROOT/'engine/src/sc_services_model.c'
            if model.exists(): sources.append(model)
            cmd=['cc','-std=c11','-Wall','-Wextra','-Werror','-I',str(tmp),'-I',str(ROOT/'engine/include'),str(tmp/'probe.c'),*map(str,sources),'-o',str(tmp/'probe')]
            compiled=subprocess.run(cmd,capture_output=True,text=True)
            self.assertEqual(compiled.returncode,0,compiled.stderr)
            subprocess.run([str(tmp/'probe')],check=True)

if __name__=='__main__':unittest.main()
