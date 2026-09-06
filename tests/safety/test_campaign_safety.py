"""Compile actual policy/config and focused source functions on the host.

SC_TEST_CAMPAIGN=1 exercises production guards in a TESTING build; ordinary
upstream TESTING builds deliberately retain broader engine behavior.
"""
import json
from pathlib import Path
import re
import subprocess
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[2]
ENGINE=ROOT/'engine'

def function(file,name):
    text=(ENGINE/file).read_text();start=re.search(r'^(?:static )?[^\n;]+\b'+name+r'\([^;]*?\)\n\{',text,re.M).start()
    opening=text.index('{',start);depth=1;i=opening+1
    while depth:
        depth+=(text[i]=='{')-(text[i]=='}');i+=1
    return text[start:i]

def execute(source,testing=0,campaign=0,firered=True):
    with tempfile.TemporaryDirectory(prefix='sc-safety-') as d:
        p=Path(d);(p/'test.c').write_text(source)
        cmd=['cc','-std=c11','-Werror','-Wno-attributes','-I'+str(ENGINE/'include'),'-DTRUE=1','-DFALSE=0','-DTESTING='+str(testing),'-DSC_TEST_CAMPAIGN='+str(campaign),'-DDISABLED_ON_RELEASE=1','-DGEN_LATEST=GEN_9']
        if firered:cmd+=['-DFIRERED']
        result=subprocess.run(cmd+[str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)
        result=subprocess.run([str(p/'test')],capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr or 'native policy assertion failed')

HEADERS='''#include <assert.h>
#include <stddef.h>
#include "config/general.h"
#include "config/pokemon.h"
#include "config/battle.h"
#include "config/item.h"
#include "config/overworld.h"
#include "config/debug.h"
'''

class CampaignSafetyTests(unittest.TestCase):
    def test_campaign_defaults_and_upstream_test_isolation(self):
        for testing,campaign,firered,enabled in [(0,0,True,True),(1,1,True,True),(1,0,True,False),(0,0,False,False)]:
            with self.subTest(testing=testing,campaign=campaign,firered=firered):
                body='assert(P_SC_KANTO_RULES == '+str(int(enabled))+');'
                if enabled:
                    body+='assert(P_UPDATED_EXP_YIELDS==GEN_3); assert(B_SCALED_EXP==GEN_3); assert(B_SPLIT_EXP==GEN_3); assert(B_TRAINER_EXP_MULTIPLIER==GEN_3); assert(B_EXP_CATCH==GEN_3); assert(B_UNEVOLVED_EXP_MULTIPLIER==GEN_3); assert(!B_AFFECTION_MECHANICS); assert(I_REUSABLE_TMS); assert(P_CAN_FORGET_HIDDEN_MOVE); assert(OW_UNION_DISABLE_CHECK); assert(!DEBUG_OVERWORLD_MENU); assert(!DEBUG_BATTLE_MENU); assert(!DEBUG_POKEMON_SPRITE_VISUALIZER);'
                else:body+='assert(!I_REUSABLE_TMS); assert(!P_CAN_FORGET_HIDDEN_MOVE);'
                body+='assert(B_PHYSICAL_SPECIAL_SPLIT>=GEN_4); assert(P_UPDATED_TYPES>=GEN_6); assert(B_TERRAIN_TYPE_BOOST>=GEN_8); assert(B_ABILITY_WEATHER>=GEN_6); assert(I_EXP_SHARE_FLAG==0); assert(I_EXP_SHARE_ITEM<GEN_6);'
                execute(HEADERS+'int main(void){'+body+'}',testing,campaign,firered)
    def test_breeding_does_not_walk_back_to_excluded_babies(self):
        source=HEADERS+'''#include "constants/species.h"
typedef unsigned char bool8;
struct Evolution {int method; enum Species targetSpecies;};
#define EVOLUTIONS_END 0
static const struct Evolution pika[]={{1,SPECIES_RAICHU},{0,0}},baby[]={{1,SPECIES_PIKACHU},{0,0}},none[]={{0,0}};
int IsSpeciesEnabled(enum Species s){return 1;}
enum Species SanitizeSpeciesId(enum Species s){return s;}
const struct Evolution *GetSpeciesEvolutions(enum Species s){return s==SPECIES_PIKACHU?pika:s==SPECIES_PICHU?baby:none;}
'''+function('src/daycare.c','GetEggSpecies')+'\nint main(void){assert(GetEggSpecies(SPECIES_RAICHU)==(P_SC_KANTO_RULES?SPECIES_PIKACHU:SPECIES_PICHU));}'
        for testing,campaign in [(0,0),(1,1),(1,0)]:execute(source,testing,campaign)
    def test_gimmick_activation_is_closed_only_in_campaign(self):
        source=HEADERS+'''typedef unsigned int bool32;
enum BattlerId {BATTLER_0}; enum Gimmick {GIMMICK_TEST};
struct GimmickInfo {bool32 (*CanActivate)(enum BattlerId);};
bool32 yes(enum BattlerId b){return 1;}
const struct GimmickInfo gGimmicksInfo[]={{yes}};
'''+function('src/battle_gimmick.c','CanActivateGimmick')+'\nint main(void){assert(CanActivateGimmick(BATTLER_0,GIMMICK_TEST)==!P_SC_KANTO_RULES);}'
        for testing,campaign in [(0,0),(1,1),(1,0)]:execute(source,testing,campaign)
    def test_ev_item_classifier_preserves_recovery_and_candies(self):
        source=HEADERS+'''#include "constants/items.h"
#include "constants/item_effects.h"
typedef unsigned int bool32;
int effectType;
int GetItemEffectType(enum Item item){return effectType;}
'''+function('src/party_menu.c','ScItemChangesTraining')+'''
int main(void){
    int ev[]={ITEM_EFFECT_HP_EV,ITEM_EFFECT_ATK_EV,ITEM_EFFECT_DEF_EV,ITEM_EFFECT_SPEED_EV,ITEM_EFFECT_SPATK_EV,ITEM_EFFECT_SPDEF_EV};
    for(unsigned i=0;i<sizeof(ev)/sizeof(*ev);i++){effectType=ev[i]; assert(ScItemChangesTraining(ITEM_HP_UP));}
    effectType=ITEM_EFFECT_HEAL_HP; assert(!ScItemChangesTraining(ITEM_POTION));
    effectType=ITEM_EFFECT_RAISE_LEVEL; assert(!ScItemChangesTraining(ITEM_RARE_CANDY));
    assert(ScItemChangesTraining(ITEM_FRESH_START_MOCHI));
}'''
        execute(source)
    def test_relearner_excludes_future_and_known_moves(self):
        source=HEADERS+'''#include "config/summary_screen.h"
#include "constants/species.h"
typedef unsigned int u32; typedef unsigned short u16; typedef unsigned int bool32;
#define MON_DATA_SPECIES 0
#define MAX_LEVEL 100
#define LEVEL_UP_MOVE_END 65535
struct BoxPokemon {int level;};
struct LevelUpMove {u16 move; u16 level;};
static const struct LevelUpMove moves[]={{1,1},{2,10},{3,20},{4,21},{LEVEL_UP_MOVE_END,0}};
enum Species GetBoxMonData(struct BoxPokemon *m,int attr){return SPECIES_PIKACHU;}
u32 GetLevelFromBoxMonExp(struct BoxPokemon *m){return m->level;}
const struct LevelUpMove *GetSpeciesLevelUpLearnset(enum Species s){return moves;}
int BoxMonKnowsMove(struct BoxPokemon *m,u16 move){return move==2;}
enum Species GetSpeciesPreEvolution(enum Species s){return SPECIES_NONE;}
'''+function('src/move_relearner.c','GetRelearnerLevelUpMoves')+'''
int main(void){struct BoxPokemon mon={20};u16 result[8]={0};assert(GetRelearnerLevelUpMoves(&mon,result)==2);assert(result[0]==1&&result[1]==3);}
'''
        execute(source)
    def test_kanto_centers_have_no_external_facility_entry(self):
        centers=[]
        for p in (ENGINE/'data/maps').glob('*PokemonCenter_2F_Frlg/map.json'):
            data=json.loads(p.read_text())
            # Upstream labels Sevii maps REGION_KANTO too; campaign excludes them.
            if re.match(r'(One|Two|Three|Four|Five|Six|Seven)Island_',p.parent.name):continue
            if data['region']!='REGION_KANTO':continue
            centers.append(data)
            self.assertTrue(all(w['dest_map'].endswith('POKEMON_CENTER_1F') for w in data['warp_events']),p.parent.name)
            self.assertTrue(all(o['script']=='ScSafety_CenterLounge' for o in data['object_events']),p.parent.name)
            script=p.with_name('scripts.inc').read_text()
            self.assertNotIn('CableClub_',script)
        self.assertGreaterEqual(len(centers),10)

if __name__=='__main__':unittest.main()
