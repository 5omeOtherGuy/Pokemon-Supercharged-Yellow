"""Compile and exercise the production public NPC catalog with deterministic parties."""

from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]


class CatalogTests(unittest.TestCase):
    def test_partial_npc_builds_respect_every_budget_and_invalid_index(self):
        with tempfile.TemporaryDirectory(prefix="psy-catalog-") as folder:
            temporary = Path(folder)
            (temporary / "global.h").write_text('''
#include <stdint.h>
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32;
#define COMPOUND_STRING(x) ((const u8 *)(x))
#define ARRAY_COUNT(x) (sizeof(x)/sizeof((x)[0]))
#define TRUE 1
#define FALSE 0
''')
            (temporary / "data.h").write_text('''
#define TRAINERS_COUNT 652
struct TrainerMon { unsigned char lvl; };
extern struct TrainerMon party[6];
extern unsigned char partySize;
static inline const struct TrainerMon *GetTrainerPartyFromId(unsigned short id) { (void)id; return party; }
static inline unsigned char GetTrainerPartySizeFromId(unsigned short id) { (void)id; return partySize; }
''')
            probe = temporary / "probe.c"
            probe.write_text('''
#include "global.h"
#include <assert.h>
#include "data.h"
#include "constants/opponents_frlg.h"
struct ScPassiveInfo { const u8 *name; const u8 *description; u8 cost; };
extern const struct ScPassiveInfo gScCapabilityInfo[12], gScTrainerPassiveInfo[8];
extern u32 ScGetNpcCapabilities(u16,u32), ScGetNpcTrainerPassives(u16), ScGetNpcTrainerBudget(u16);
unsigned char partySize;
struct TrainerMon party[6];
static unsigned cost(unsigned mask, const struct ScPassiveInfo *catalog, unsigned count) {
 unsigned result=0; assert((mask >> count)==0);
 for(unsigned i=0;i<count;i++) if(mask & (1u<<i)) result+=catalog[i].cost;
 return result;
}
int main(void) {
 for(unsigned i=0;i<12;i++) { assert(gScCapabilityInfo[i].name[0]); assert(gScCapabilityInfo[i].description[0]); assert(gScCapabilityInfo[i].cost>=1 && gScCapabilityInfo[i].cost<=3); }
 for(unsigned n=2;n<=6;n++) { partySize=n;
  for(unsigned id=0;id<652;id++) {
   assert(cost(ScGetNpcTrainerPassives(id),gScTrainerPassiveInfo,8)<=ScGetNpcTrainerBudget(id));
   assert(ScGetNpcTrainerBudget(id)<=10);
   for(unsigned slot=0;slot<n;slot++) assert(cost(ScGetNpcCapabilities(id,slot),gScCapabilityInfo,12)<=3);
   assert(ScGetNpcCapabilities(id,n)==0); assert(ScGetNpcCapabilities(id,255)==0);
  }
  assert(ScGetNpcCapabilities(TRAINER_LEADER_BROCK,n-1)==1u);
  assert(ScGetNpcCapabilities(TRAINER_LEADER_BROCK,0)==0);
  assert(ScGetNpcCapabilities(TRAINER_LEADER_MISTY,n-1)==2u);
 }
 partySize=3; party[0].lvl=21; party[1].lvl=22; party[2].lvl=21;
 assert(ScGetNpcCapabilities(TRAINER_LEADER_MISTY,0)==0);
 assert(ScGetNpcCapabilities(TRAINER_LEADER_MISTY,1)==2u);
 assert(ScGetNpcCapabilities(TRAINER_LEADER_MISTY,2)==0);
 assert(ScGetNpcCapabilities(TRAINER_LEADER_KOGA,1)==(1u<<4));
 assert(ScGetNpcCapabilities(TRAINER_LEADER_KOGA,2)==1u);
 party[2].lvl=22; assert(ScGetNpcCapabilities(TRAINER_LEADER_MISTY,2)==2u);
 assert(ScGetNpcCapabilities(TRAINER_LEADER_MISTY,1)==0);

 /* Story builds are partial, stage-bounded and follow the real highest-level ace. */
 for(unsigned id=TRAINER_RIVAL_OAKS_LAB_SQUIRTLE;id<=TRAINER_RIVAL_ROUTE22_EARLY_CHARMANDER;id++) {
  assert(ScGetNpcTrainerBudget(id)==0); assert(ScGetNpcTrainerPassives(id)==0);
  for(unsigned slot=0;slot<3;slot++) assert(ScGetNpcCapabilities(id,slot)==0);
 }
 const unsigned story[][3]={
  {TRAINER_RIVAL_CERULEAN_SQUIRTLE,TRAINER_RIVAL_CERULEAN_CHARMANDER,3},
  {TRAINER_RIVAL_SS_ANNE_SQUIRTLE,TRAINER_RIVAL_SS_ANNE_CHARMANDER,3},
  {TRAINER_RIVAL_POKEMON_TOWER_SQUIRTLE,TRAINER_RIVAL_POKEMON_TOWER_CHARMANDER,5},
  {TRAINER_RIVAL_SILPH_SQUIRTLE,TRAINER_RIVAL_SILPH_CHARMANDER,7},
  {TRAINER_RIVAL_ROUTE22_LATE_SQUIRTLE,TRAINER_RIVAL_ROUTE22_LATE_CHARMANDER,8},
  {TRAINER_BOSS_GIOVANNI,TRAINER_BOSS_GIOVANNI,5},
  {TRAINER_BOSS_GIOVANNI_2,TRAINER_BOSS_GIOVANNI_2,7},
  {TRAINER_SC_ROCKET_DUO_1,TRAINER_SC_ROCKET_DUO_1,2},
  {TRAINER_SC_ROCKET_DUO_2,TRAINER_SC_ROCKET_DUO_2,5},
  {TRAINER_SC_ROCKET_DUO_3,TRAINER_SC_ROCKET_DUO_3,5},
  {TRAINER_SC_ROCKET_DUO_4,TRAINER_SC_ROCKET_DUO_4,7},
 };
 for(unsigned row=0;row<sizeof(story)/sizeof(story[0]);row++)
  for(unsigned id=story[row][0];id<=story[row][1];id++) {
   assert(ScGetNpcTrainerBudget(id)==story[row][2]);
   assert(ScGetNpcTrainerPassives(id)!=0);
   assert(cost(ScGetNpcTrainerPassives(id),gScTrainerPassiveInfo,8)<=story[row][2]);
   party[0].lvl=20;party[1].lvl=30;party[2].lvl=21;
   unsigned ace=ScGetNpcCapabilities(id,1),support=ScGetNpcCapabilities(id,2);
   assert((ace&((1u<<5)|(1u<<6)))==0); /* no blanket category penalty */
   assert(ScGetNpcCapabilities(id,0)==0);
   assert(cost(ace,gScCapabilityInfo,12)<=3);assert(cost(support,gScCapabilityInfo,12)<=1);
   party[2].lvl=30;
   assert(ScGetNpcCapabilities(id,2)==ace);assert(ScGetNpcCapabilities(id,1)==support);
  }
 assert(ScGetNpcCapabilities(TRAINER_RIVAL_SILPH_CHARMANDER,2)!=0);
 assert(ScGetNpcCapabilities(65535,0)==0);
 assert(ScGetNpcTrainerPassives(65535)==0);
 assert(ScGetNpcTrainerBudget(65535)==0);
 assert(ScGetNpcCapabilities(1,0)==0);
 return 0;
}
''')
            source = ROOT / "engine/src/sc_build_catalog.c"
            command = ["cc", "-std=c11", "-Wall", "-Wextra", "-Werror", "-I", str(temporary),
                       "-I", str(ROOT / "engine/include"), str(probe)]
            if source.exists():
                command.append(str(source))
            command += ["-o", str(temporary / "probe")]
            result = subprocess.run(command, capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            subprocess.run([str(temporary / "probe")], check=True)


if __name__ == "__main__":
    unittest.main()
