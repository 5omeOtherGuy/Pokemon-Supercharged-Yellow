"""Compile the production field policy with deterministic inventory/party fixtures."""
from pathlib import Path
import subprocess
import tempfile
import re
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
    assert(ScFieldIsPermitMove(FIELD_MOVE_SURF));assert(!ScFieldIsPermitMove(FIELD_MOVE_DIG));
    assert(!ScFieldCanReplace(PARTY_SIZE,&surf));
    assert(!ScFieldCanUse(&surf,FIELD_MOVES_COUNT));
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

def extract_function(path,name):
    source=(ENGINE/path).read_text()
    start=re.search(r'^(?:static )?[^\n;]+\b'+name+r'\([^;]*?\)\n\{',source,re.M).start()
    opening=source.index('{',start);depth=1;end=opening+1
    while depth:
        depth+=(source[end]=='{')-(source[end]=='}');end+=1
    return source[start:end]

ENTRY_FIXTURE = r'''
typedef unsigned char bool8;
#define PLAYER_AVATAR_FLAG_SURFING 1
#define SCREFF_V1 1
#define B_CATCH_SWAP_CHECK_HMS 1
#define MAX_MON_MOVES 4
u32 surfing;
u32 gSpecialVar_Result,gSpecialVar_0x8004;
struct ScriptContext {u8 bytes[2];u8 pos;};
u8 ScriptReadByte(struct ScriptContext *ctx){return ctx->bytes[ctx->pos++];}
void Script_RequestEffects(u32 flags){}
bool32 TestPlayerAvatarFlags(u32 flags){return surfing;}
bool32 IsFieldMoveUnlocked(enum FieldMove field){return badges==31;}
bool32 IsMoveHM(u32 move){return 0;}
struct Pokemon caught;
u8 GetCatchingBattler(void){return 1;}
struct Pokemon *GetBattlerMon(u32 battler){return &caught;}
'''
MENU_FIXTURE = r'''
#define MENU_FIELD_MOVES 100
#define MENU_SUMMARY 0
#define MENU_SWITCH 1
#define MENU_ITEM 2
#define MENU_MAIL 3
#define MENU_CANCEL1 4
#define MAX_MON_MOVES 4
#define MON_DATA_MOVE1 10
#define MON_DATA_HELD_ITEM 20
#define SE_SELECT 1
#define SELECTWINDOW_ACTIONS 0
struct {u8 actions[8],numActions,scFieldPage,windowId[3];} internal,*sPartyMenuInternal=&internal;
struct {u8 slotId;} gPartyMenu;
struct {int data[16];void (*func)(u8);} gTasks[1];
void AppendToList(u8 *items,u8 *count,u8 action){assert(*count<8);items[(*count)++]=action;}
bool32 FieldMove_IsVisible(u32 field){return 1;}
bool32 InBattlePike(void){return 0;}
bool32 ItemIsMail(u32 item){return 0;}
void PlaySE(u32 sound){}
void PartyMenuRemoveWindow(u8 *window){}
void DisplaySelectionWindow(u32 type){assert(internal.numActions<=7);}
void Task_HandleSelectionMenuInput(u8 task){}
'''
MENU_CASES = r'''
int main(void){
    badges=hms=31;compatibility[SPECIES_MEW]=~0u;
    gParties[0][0]=(struct Pokemon){{SPECIES_MEW,0},(1u<<FIELD_MOVE_DIG)|(1u<<FIELD_MOVE_TELEPORT)|(1u<<FIELD_MOVE_SOFT_BOILED)|(1u<<FIELD_MOVE_SWEET_SCENT)};
    gParties[0][1]=(struct Pokemon){{SPECIES_PIDGEY,0},0};
    SetPartyMonFieldSelectionActions(gParties[0],0);
    assert(internal.numActions==5&&internal.actions[1]==SC_FIELD_ACTION_OPEN);
    assert(ScHandleFieldMenuAction(0,SC_FIELD_ACTION_OPEN));assert(internal.numActions==7);
    assert(internal.actions[5]==SC_FIELD_ACTION_NEXT&&internal.actions[6]==SC_FIELD_ACTION_BACK);
    assert(ScHandleFieldMenuAction(0,SC_FIELD_ACTION_NEXT));assert(internal.numActions==6);
    assert(ScHandleFieldMenuAction(0,SC_FIELD_ACTION_NEXT));assert(internal.numActions==7);
    assert(ScHandleFieldMenuAction(0,SC_FIELD_ACTION_BACK));assert(internal.numActions==5&&internal.actions[4]==MENU_CANCEL1);
    assert(!ScHandleFieldMenuAction(0,MENU_SUMMARY));
    return 0;
}
'''

ENTRY_CASES = r'''
int main(void){
    badges=hms=31;compatibility[SPECIES_LAPRAS]=1u<<FIELD_MOVE_SURF;
    gParties[0][0]=(struct Pokemon){{SPECIES_LAPRAS,0},0};
    assert(PartyHasMonWithSurf());
    surfing=1;assert(!PartyHasMonWithSurf());surfing=0;
    struct ScriptContext ctx={{FIELD_MOVE_SURF,0},0};
    ScrCmd_checkfieldmove(&ctx);assert(gSpecialVar_Result==0&&gSpecialVar_0x8004==SPECIES_LAPRAS);
    hms=0;ctx.pos=0;ScrCmd_checkfieldmove(&ctx);assert(gSpecialVar_Result==PARTY_SIZE);assert(!PartyHasMonWithSurf());hms=31;
    u8 slot=0;caught=(struct Pokemon){{SPECIES_PIDGEY,0},0};assert(DoesSelectedMonKnowHM(&slot));
    caught=(struct Pokemon){{SPECIES_LAPRAS,0},0};assert(!DoesSelectedMonKnowHM(&slot));
    return 0;
}
'''

class FieldAccessTests(unittest.TestCase):
    def test_actual_fly_selector_rejects_sevii_and_unvisited_mainland(self):
        function=extract_function('src/region_map.c','GetMapsecType')
        symbols=sorted(set(re.findall(r'\b(?:MAPSEC_|MAPSECTYPE_|FLAG_)[A-Z0-9_]+',function)))
        declarations='\n'.join('#define '+name+' '+str(i+1) for i,name in enumerate(symbols))
        source='#include <assert.h>\n#define P_SC_KANTO_RULES 1\n#define REGION_MAP_KANTO 0\ntypedef unsigned char u8;typedef unsigned int mapsec_u16_t;\n'+declarations+'''
static int palletVisited;
int FlagGet(unsigned flag){return flag==FLAG_WORLD_MAP_PALLET_TOWN?palletVisited:1;}
int GetRegionMapType(unsigned section){return section==MAPSEC_ONE_ISLAND||section==MAPSEC_LITTLEROOT_TOWN?1:REGION_MAP_KANTO;}
'''+function+'''
int main(void){
assert(GetMapsecType(MAPSEC_ONE_ISLAND)==MAPSECTYPE_NONE);
assert(GetMapsecType(MAPSEC_LITTLEROOT_TOWN)==MAPSECTYPE_NONE);
assert(GetMapsecType(MAPSEC_PALLET_TOWN)==MAPSECTYPE_CITY_CANTFLY);
palletVisited=1;assert(GetMapsecType(MAPSEC_PALLET_TOWN)==MAPSECTYPE_CITY_CANFLY);
assert(GetMapsecType(MAPSEC_CINNABAR_ISLAND)==MAPSECTYPE_CITY_CANFLY);
return 0;}
'''
        with tempfile.TemporaryDirectory(prefix='sc-field-fly-') as d:
            p=Path(d);(p/'test.c').write_text(source)
            result=subprocess.run(['cc','-std=c11','-Werror',str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)
            result=subprocess.run([str(p/'test')],capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)

    def test_actual_field_menu_open_page_back_and_capacity(self):
        policy='\n'.join(line for line in (ENGINE/'src/sc_field.c').read_text().splitlines() if not line.startswith('#include'))
        source=FIXTURE+MENU_FIXTURE+policy
        for name in ['SetPartyMonFieldSelectionActions','ScHandleFieldMenuAction']:
            source+='\n'+extract_function('src/party_menu.c',name)
        source+='\n'+MENU_CASES
        with tempfile.TemporaryDirectory(prefix='sc-field-menu-') as d:
            p=Path(d);(p/'test.c').write_text(source)
            cmd=['cc','-std=c11','-Werror','-I'+str(ENGINE/'include'),'-DTRUE=1','-DFALSE=0','-DFIRERED','-DTESTING=0',str(p/'test.c'),'-o',str(p/'test')]
            result=subprocess.run(cmd,capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)
            result=subprocess.run([str(p/'test')],capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)

    def test_actual_surf_script_and_catch_swap_entries(self):
        policy='\n'.join(line for line in (ENGINE/'src/sc_field.c').read_text().splitlines() if not line.startswith('#include'))
        source=FIXTURE+ENTRY_FIXTURE+policy
        for path,name in [('src/field_player_avatar.c','PartyHasMonWithSurf'),('src/scrcmd.c','ScrCmd_checkfieldmove'),('src/party_menu.c','DoesSelectedMonKnowHM')]:
            source+='\n'+extract_function(path,name)
        source+='\n'+ENTRY_CASES
        with tempfile.TemporaryDirectory(prefix='sc-field-entry-') as d:
            p=Path(d);(p/'test.c').write_text(source)
            cmd=['cc','-std=c11','-Werror','-I'+str(ENGINE/'include'),'-DTRUE=1','-DFALSE=0','-DFIRERED','-DTESTING=0','-DMON_DATA_MOVE1=10',str(p/'test.c'),'-o',str(p/'test')]
            result=subprocess.run(cmd,capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)
            result=subprocess.run([str(p/'test')],capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)

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
