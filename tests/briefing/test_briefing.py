"""Native execution of the production briefing data/navigation layer."""

from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]


class BriefingTests(unittest.TestCase):
    def test_authoritative_data_navigation_and_safe_entry(self):
        with tempfile.TemporaryDirectory(prefix="psy-briefing-") as folder:
            temporary = Path(folder)
            (temporary / "global.h").write_text('''
#ifndef GLOBAL_TEST_H
#define GLOBAL_TEST_H
#include <stdint.h>
#include <string.h>
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32; typedef int32_t s32; typedef uint32_t bool32;
#define TRUE 1
#define FALSE 0
#define ARRAY_COUNT(x) (sizeof(x)/sizeof((x)[0]))
#define MAX_MON_MOVES 4
#define PARTY_SIZE 6
#define MAX_LEVEL 100
#define NUM_NATURES 25
#define B_TRAINER_MON_RANDOM_ABILITY 0
#define A_BUTTON 1
#define B_BUTTON 2
#define START_BUTTON 8
#define DPAD_RIGHT 16
#define DPAD_LEFT 32
#define DPAD_UP 64
#define DPAD_DOWN 128
#include "constants/species.h"
#include "constants/moves.h"
#include "constants/abilities.h"
#define ITEMS_COUNT 1000
#define LEVEL_UP_MOVE_END 65535
#endif
''')
            (temporary / "data.h").write_text('''
#ifndef DATA_TEST_H
#define DATA_TEST_H
#define TRAINERS_COUNT 652
#define TRAINER_BATTLE_TYPE_SINGLES 0
#define TRAINER_BATTLE_TYPE_DOUBLES 1
struct TrainerMon { u16 species,moves[4],ability,heldItem; u8 lvl,nature; };
struct Trainer { const struct TrainerMon *party; u16 items[4]; u8 partySize,poolSize,poolRuleIndex,poolPickIndex,poolPruneIndex,battleType; u16 overrideTrainer; u8 startingStatus[4]; };
extern struct Trainer trainers[652];
static inline const struct Trainer *GetTrainerStructFromId(u16 id) { return &trainers[id]; }
static inline const struct TrainerMon *GetTrainerPartyFromId(u16 id) { return trainers[id].party; }
#endif
''')
            (temporary / "pokemon.h").write_text('''
#ifndef POKEMON_TEST_H
#define POKEMON_TEST_H
struct LevelUpMove { u16 move,level; };
struct SpeciesInfo { u16 abilities[3]; const struct LevelUpMove *levelUpLearnset; };
extern struct SpeciesInfo gSpeciesInfo[152];
static inline const struct LevelUpMove *GetSpeciesLevelUpLearnset(u16 species) { return gSpeciesInfo[species].levelUpLearnset; }
#endif
''')
            (temporary / "move.h").write_text('')
            probe = temporary / "probe.c"
            probe.write_text('''
#include "global.h"
#include "data.h"
#include "pokemon.h"
#include "sc_briefing.h"
#include <assert.h>
struct Trainer trainers[652];
struct SpeciesInfo gSpeciesInfo[152];
u32 ScGetNpcCapabilities(u16 id,u32 slot) { (void)id;(void)slot;return 3; }
u32 ScGetNpcTrainerPassives(u16 id) { (void)id;return 6; }
static const struct LevelUpMove learnset[]={{MOVE_POUND,0},{MOVE_TACKLE,1},{MOVE_GROWL,1},{MOVE_TAIL_WHIP,3},{MOVE_QUICK_ATTACK,5},{MOVE_TACKLE,6},{MOVE_SPARK,8},{MOVE_THUNDER,30},{65535,0}};
static struct TrainerMon party[2]={{.species=SPECIES_PIKACHU,.lvl=8,.nature=3},{.species=SPECIES_PIKACHU,.lvl=8,.nature=3,.ability=ABILITY_STATIC,.moves={MOVE_THUNDER,MOVE_NONE,MOVE_TACKLE,MOVE_NONE}}};
int main(void) {
 struct ScBriefingModel model; struct ScBriefingView view={0}; u16 output[4];
 struct TrainerMon original[2]; memcpy(original,party,sizeof(party));
 gSpeciesInfo[SPECIES_PIKACHU]=(struct SpeciesInfo){{ABILITY_STATIC,ABILITY_NONE,ABILITY_LIGHTNING_ROD},learnset};
 trainers[314]=(struct Trainer){.party=party,.partySize=2,.battleType=1};
 assert(ScBriefingLoad(314,&model)); assert(model.partySize==2);
 ScBriefingGetMoves(&model,0,output);
 assert(output[0]==MOVE_GROWL && output[1]==MOVE_TAIL_WHIP && output[2]==MOVE_QUICK_ATTACK && output[3]==MOVE_SPARK);
 ScBriefingGetMoves(&model,1,output); assert(output[0]==MOVE_THUNDER && output[1]==0 && output[2]==MOVE_TACKLE && output[3]==0);
 assert(ScBriefingGetAbility(&model,0)==ABILITY_STATIC);
 assert(!ScBriefingCanAccept(&model,0) && !ScBriefingCanAccept(&model,1)); assert(ScBriefingCanAccept(&model,2));
 assert(ScBriefingNavigate(&model,&view,A_BUTTON)==SC_BRIEF_REDRAW && view.screen==SC_BRIEF_MON);
 assert(ScBriefingNavigate(&model,&view,A_BUTTON)==SC_BRIEF_REDRAW && view.screen==SC_BRIEF_DESCRIPTION);
 assert(ScBriefingNavigate(&model,&view,B_BUTTON)==SC_BRIEF_REDRAW && view.screen==SC_BRIEF_MON);
 assert(ScBriefingNavigate(&model,&view,START_BUTTON)==SC_BRIEF_REDRAW && view.screen==SC_BRIEF_CONFIRM);
 assert(ScBriefingNavigate(&model,&view,A_BUTTON)==SC_BRIEF_DECLINE); // Default is no.
 view=(struct ScBriefingView){0}; ScBriefingNavigate(&model,&view,START_BUTTON); ScBriefingNavigate(&model,&view,DPAD_UP);
 assert(ScBriefingNavigate(&model,&view,A_BUTTON)==SC_BRIEF_ACCEPT);
 view=(struct ScBriefingView){0}; assert(ScBriefingNavigate(&model,&view,B_BUTTON)==SC_BRIEF_DECLINE);
 for(unsigned key=0;key<256;key++) { view=(struct ScBriefingView){0}; ScBriefingNavigate(&model,&view,key); assert(view.mon<2); }
 assert(memcmp(original,party,sizeof(party))==0);
 trainers[314].poolSize=3; assert(!ScBriefingLoad(314,&model) && model.error==SC_BRIEF_VARIABLE_PARTY); trainers[314].poolSize=0;
 trainers[314].overrideTrainer=1; assert(!ScBriefingLoad(314,&model)); trainers[314].overrideTrainer=0;
 party[0].ability=ABILITY_OVERGROW; assert(!ScBriefingLoad(314,&model)); party[0].ability=0;
 party[0].species=SPECIES_PICHU; assert(!ScBriefingLoad(314,&model)); party[0].species=SPECIES_PIKACHU;
 party[0].nature=25; assert(!ScBriefingLoad(314,&model)); party[0].nature=3;
 party[0].moves[0]=MOVE_DEFAULT; assert(!ScBriefingLoad(314,&model)); party[0].moves[0]=0;
 assert(!ScBriefingLoad(65535,&model)); assert(!ScBriefingLoad(0,&model));
 return 0;
}
''')
            command = ["cc", "-std=c11", "-Wall", "-Wextra", "-Werror", "-DSC_BRIEFING_HOST_TEST",
                       "-I", str(temporary), "-I", str(ROOT / "engine/include"), str(probe)]
            source = ROOT / "engine/src/sc_briefing.c"
            if source.exists():
                command.append(str(source))
            command += ["-o", str(temporary / "probe")]
            result = subprocess.run(command, capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            subprocess.run([str(temporary / "probe")], check=True)


if __name__ == "__main__":
    unittest.main()
