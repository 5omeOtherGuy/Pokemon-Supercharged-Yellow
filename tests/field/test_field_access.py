"""Compile the production field policy with deterministic inventory/party fixtures."""
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
ENGINE = ROOT / 'engine'

FIXTURE = r'''
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "config/general.h"
#include "config/pokemon.h"
#include "constants/species.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/field_move.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef unsigned int bool32;
#define PARTY_SIZE 6
#define B_TRAINER_PLAYER 0
#define TOTAL_BOXES_COUNT 2
#define IN_BOX_COUNT 4
#define MON_DATA_SPECIES 0
#define MON_DATA_IS_EGG 1
#define FLAG_BADGE01_GET 0
#define FLAG_BADGE02_GET 1
#define FLAG_BADGE03_GET 2
#define FLAG_BADGE04_GET 3
#define FLAG_BADGE05_GET 4
#define SC_FIELD_PAGE_SIZE 5
#define SC_FIELD_ACTION_OPEN 240
#define SC_FIELD_ACTION_NEXT 241
#define SC_FIELD_ACTION_BACK 242
struct BoxPokemon {enum Species species;u32 egg;};
struct Pokemon {struct BoxPokemon box;u32 known;};
struct Pokemon gParties[1][PARTY_SIZE];
struct BoxPokemon boxes[TOTAL_BOXES_COUNT][IN_BOX_COUNT];
u32 badges, hms;
u32 compatibility[NUM_SPECIES];
u32 GetBoxMonData(struct BoxPokemon *mon,u32 attr,...){return attr==MON_DATA_SPECIES?mon->species:mon->egg;}
u32 GetMonData(struct Pokemon *mon,u32 attr,...){return GetBoxMonData(&mon->box,attr);}
bool32 FlagGet(u32 flag){return badges>>flag&1;}
bool32 CheckBagHasItem(enum Item item,u16 count){return hms>>(item-ITEM_HM01)&1;}
struct BoxPokemon *GetBoxedMonPtr(u8 box,u8 pos){return &boxes[box][pos];}
static const enum Move fieldMoves[FIELD_MOVES_COUNT]={[FIELD_MOVE_CUT]=MOVE_CUT,[FIELD_MOVE_FLASH]=MOVE_FLASH,[FIELD_MOVE_STRENGTH]=MOVE_STRENGTH,[FIELD_MOVE_SURF]=MOVE_SURF,[FIELD_MOVE_FLY]=MOVE_FLY,[FIELD_MOVE_DIG]=MOVE_DIG,[FIELD_MOVE_TELEPORT]=MOVE_TELEPORT,[FIELD_MOVE_SOFT_BOILED]=MOVE_SOFT_BOILED,[FIELD_MOVE_SWEET_SCENT]=MOVE_SWEET_SCENT};
enum Move FieldMove_GetMoveId(enum FieldMove field){return fieldMoves[field];}
bool32 CanLearnTeachableMove(enum Species species,enum Move move){for(u32 i=0;i<FIELD_MOVES_COUNT;i++)if(fieldMoves[i]==move)return compatibility[species]>>i&1;return 0;}
bool32 MonKnowsMove(struct Pokemon *mon,enum Move move){for(u32 i=0;i<FIELD_MOVES_COUNT;i++)if(fieldMoves[i]==move)return mon->known>>i&1;return 0;}
'''

CASES = r'''
static void reset(void){memset(gParties,0,sizeof(gParties));memset(boxes,0,sizeof(boxes));memset(compatibility,0,sizeof(compatibility));badges=31;hms=31;}
int main(void){
    reset();
    struct Pokemon surf={{SPECIES_LAPRAS,0},0}, other={{SPECIES_PIDGEY,0},0};
    compatibility[SPECIES_LAPRAS]=1u<<FIELD_MOVE_SURF;
    assert(ScFieldCanUse(&surf,FIELD_MOVE_SURF)); /* no learned move and no HP requirement */
    badges &= ~(1u<<4); assert(!ScFieldCanUse(&surf,FIELD_MOVE_SURF));badges=31;
    hms &= ~(1u<<2);assert(!ScFieldCanUse(&surf,FIELD_MOVE_SURF));hms=31;
    surf.box.egg=1;assert(!ScFieldCanUse(&surf,FIELD_MOVE_SURF));surf.box.egg=0;
    assert(!ScFieldCanUse(&other,FIELD_MOVE_SURF));
    struct Pokemon excluded={{SPECIES_PICHU,0},0};compatibility[SPECIES_PICHU]=~0u;assert(!ScFieldCanUse(&excluded,FIELD_MOVE_SURF));
    assert(!ScFieldHasPermit(FIELD_MOVE_WATERFALL));
    /* Correct HM and badge pair for every permit. */
    enum FieldMove fs[]={FIELD_MOVE_FLASH,FIELD_MOVE_CUT,FIELD_MOVE_FLY,FIELD_MOVE_STRENGTH,FIELD_MOVE_SURF};
    u32 hmBits[]={4,0,1,3,2};
    for(u32 i=0;i<5;i++){badges=1u<<i;hms=1u<<hmBits[i];assert(ScFieldHasPermit(fs[i]));for(u32 j=0;j<5;j++)if(i!=j)assert(!ScFieldHasPermit(fs[j]));}badges=hms=31;
    gParties[0][0]=surf;gParties[0][1]=other;
    assert(ScFieldFindUser(FIELD_MOVE_SURF)==0);
    assert(!ScFieldCanReplace(0,&other)); /* isolated land is protected too */
    assert(ScFieldCanReplace(0,&surf)); /* caught compatible replacement is allowed */
    gParties[0][2]=surf;assert(ScFieldCanReplace(0,&other));gParties[0][2]=(struct Pokemon){0};
    assert(!ScFieldCanRelease(&surf.box,TOTAL_BOXES_COUNT,0));
    boxes[1][3]=surf.box;assert(ScFieldCanRelease(&surf.box,TOTAL_BOXES_COUNT,0));
    gParties[0][0]=(struct Pokemon){0};assert(!ScFieldCanRelease(&surf.box,1,3));
    boxes[0][0]=surf.box;assert(ScFieldCanRelease(&surf.box,1,3));
    assert(ScFieldFindUser(FIELD_MOVE_SURF)==PARTY_SIZE);
    /* Mew can have five permits plus four distinct learned field actions. */
    struct Pokemon mew={{SPECIES_MEW,0},(1u<<FIELD_MOVE_DIG)|(1u<<FIELD_MOVE_TELEPORT)|(1u<<FIELD_MOVE_SOFT_BOILED)|(1u<<FIELD_MOVE_SWEET_SCENT)};
    compatibility[SPECIES_MEW]=~0u;
    u8 actions[9];memset(actions,0xA5,sizeof(actions));
    u32 count=ScFieldBuildPage(&mew,0,actions);assert(count==7);assert(actions[5]==SC_FIELD_ACTION_NEXT&&actions[6]==SC_FIELD_ACTION_BACK);assert(actions[7]==0xA5);
    u32 seen=0;for(u32 i=0;i<5;i++){assert(actions[i]<FIELD_MOVES_COUNT);seen|=1u<<actions[i];}
    count=ScFieldBuildPage(&mew,1,actions);assert(count==6);for(u32 i=0;i<4;i++){assert(!(seen>>actions[i]&1));seen|=1u<<actions[i];}
    assert(actions[4]==SC_FIELD_ACTION_NEXT&&actions[5]==SC_FIELD_ACTION_BACK);
    assert(ScFieldBuildPage(&other,0,actions)==1&&actions[0]==SC_FIELD_ACTION_BACK);
    return 0;
}
'''

class FieldAccessTests(unittest.TestCase):
    def test_production_policy_inventory_compatibility_recovery_and_pages(self):
        source_path=ENGINE/'src/sc_field.c'
        self.assertTrue(source_path.exists(), 'Missing shared field policy implementation')
        source='\n'.join(line for line in source_path.read_text().splitlines() if not line.startswith('#include'))
        with tempfile.TemporaryDirectory(prefix='sc-field-test-') as d:
            p=Path(d);(p/'test.c').write_text(FIXTURE+'\n'+source+'\n'+CASES)
            for testing,campaign in [(0,0),(1,1)]:
                cmd=['cc','-std=c11','-Werror','-I'+str(ENGINE/'include'),'-DTRUE=1','-DFALSE=0','-DFIRERED','-DTESTING='+str(testing),'-DSC_TEST_CAMPAIGN='+str(campaign),str(p/'test.c'),'-o',str(p/'test')]
                result=subprocess.run(cmd,capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)
                result=subprocess.run([str(p/'test')],capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)

if __name__=='__main__':unittest.main()
