"""Execute production hook bodies with instrumented engine boundaries."""
from pathlib import Path
import importlib.util
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
OPTIONS = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned u32;
typedef unsigned bool32; typedef unsigned char bool8;
#define TRUE 1
#define FALSE 0
#define SC_TEST_TOOLS 1
#define SC_DEBUG_INFINITE_ITEMS 0
#define SC_DEBUG_IGNORE_LEVEL_CAP 1
#define SC_DEBUG_NO_ENCOUNTERS 2
#define SC_DEBUG_FREE_SUPPLIES 3
#define SC_DEBUG_AVOID_TRAINERS 4
static unsigned cheats;
static bool32 ScDebugOptionEnabled(u32 option){return (cheats>>option)&1;}
'''


def body(filename, signature):
    source = (ROOT / "engine/src" / filename).read_text()
    start = source.index(signature + "\n{")
    end = source.index("{", start) + 1
    depth = 1
    while depth:
        depth += (source[end] == "{") - (source[end] == "}")
        end += 1
    return source[start:end]


def execute(code, extra_sources=()):
    with tempfile.TemporaryDirectory(prefix="sc-cheat-hooks-") as directory:
        path = Path(directory)
        (path / "test.c").write_text(code)
        compiled = subprocess.run(["cc", "-std=gnu17", "-Werror", "-fmax-errors=3", "-iquote", str(ROOT / "engine/include"), str(path / "test.c"),
                                   *map(str, extra_sources), "-o", str(path / "test")], capture_output=True, text=True)
        if compiled.returncode:
            raise AssertionError(compiled.stderr)
        result = subprocess.run([str(path / "test")], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)


class CheatHooks(unittest.TestCase):
    def test_infinite_owned_items_preserves_keys_and_inventory_validation(self):
        fixture = r'''
#include "constants/items.h"
#define PYRAMID_LOCATION_NONE 0
#define FLAG_STORING_ITEMS_IN_PYRAMID_BAG 0
static unsigned quantity[ITEMS_COUNT],gBagPockets[16];
static enum Item SanitizeBagItemId(enum Item id){return id<ITEMS_COUNT?id:ITEM_NONE;}
static unsigned GetItemPocket(enum Item id){return 1;}
static unsigned GetItemImportance(enum Item id){return id==ITEM_OAKS_PARCEL;}
static unsigned CountTotalItemQuantityInBag(enum Item id){return quantity[id];}
static unsigned CurrentBattlePyramidLocation(void){return 0;}
static unsigned FlagGet(unsigned id){return 0;}
static bool32 RemovePyramidBagItem(enum Item id,u16 count){assert(0);return 0;}
static bool32 BagPocket_RemoveItem(void *p,enum Item id,u16 count){if(quantity[id]<count)return 0;quantity[id]-=count;return 1;}
'''
        cases = r'''
int main(void){
 quantity[ITEM_RARE_CANDY]=2;
 assert(RemoveBagItem(ITEM_RARE_CANDY,1));assert(quantity[ITEM_RARE_CANDY]==1);
 cheats=1u<<SC_DEBUG_INFINITE_ITEMS;
 assert(RemoveBagItem(ITEM_RARE_CANDY,1));assert(quantity[ITEM_RARE_CANDY]==1);
 assert(!RemoveBagItem(ITEM_RARE_CANDY,2));assert(!RemoveBagItem(ITEM_NONE,1));
 assert(!RemoveBagItem(ITEM_POTION,1));
 quantity[ITEM_OAKS_PARCEL]=1;assert(RemoveBagItem(ITEM_OAKS_PARCEL,1));assert(!quantity[ITEM_OAKS_PARCEL]);
}
'''
        execute(OPTIONS + fixture + body("item.c", "bool32 RemoveBagItem(enum Item itemId, u16 count)") + cases)

    def test_cap_bypass_is_reversible_and_does_not_set_badges(self):
        fixture = r'''
#include "sc_progression_core.h"
#define IS_FRLG 1
#define ARRAY_COUNT(a) (sizeof(a)/sizeof((a)[0]))
enum {FLAG_BADGE01_GET,FLAG_BADGE02_GET,FLAG_BADGE03_GET,FLAG_BADGE04_GET,FLAG_BADGE05_GET,FLAG_BADGE06_GET,FLAG_BADGE07_GET,FLAG_BADGE08_GET,FLAG_IS_CHAMPION};
#define MAX_LEVEL 100
#define B_LEVEL_CAP_TYPE 1
#define LEVEL_CAP_FLAG_LIST 1
#define LEVEL_CAP_VARIABLE 2
#define B_LEVEL_CAP_VARIABLE 0
static unsigned flags[9];
static unsigned FlagGet(unsigned id){return flags[id];}
static unsigned VarGet(unsigned id){return 0;}
'''
        cases = r'''
int main(void){assert(GetCurrentLevelCap()==15);cheats=2;assert(GetCurrentLevelCap()==100);
 for(unsigned i=0;i<9;i++)assert(!flags[i]);
 cheats=0;flags[0]=1;assert(GetCurrentLevelCap()==22);
 for(unsigned i=0;i<8;i++)flags[i]=1;assert(GetCurrentLevelCap()==68);
 flags[8]=1;assert(GetCurrentLevelCap()==75);}
'''
        execute(OPTIONS + fixture + body("caps.c", "u32 GetCurrentLevelCap(void)") + cases, [ROOT / "engine/src/sc_progression_core.c"])

    def test_walk_encounter_switch_returns_before_random_checks(self):
        prefix = body("wild_encounter.c", "bool8 StandardWildEncounter(u16 curMetatileBehavior, u16 prevMetatileBehavior)").split("    headerId = GetCurrentMapWildMonHeaderId();", 1)[0]
        execute(OPTIONS + "\nenum TimeOfDay {DAY}; struct Roamer; static bool8 sWildEncountersDisabled;\n" + prefix + r'''
 return TRUE;
}
int main(void){assert(StandardWildEncounter(0,0));cheats=4;assert(!StandardWildEncounter(0,0));cheats=0;assert(StandardWildEncounter(0,0));}
''')

    def test_free_player_supplies_leave_opponent_quota_and_stock_checks_intact(self):
        spec = importlib.util.spec_from_file_location("supply_adapter", ROOT / "tests/supplies/test_adapter.py")
        adapter = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(adapter)
        source = (ROOT / "engine/src/sc_supplies.c").read_text()
        target = body("sc_supplies.c", "static bool32 TargetHasEffect(enum BattlerId actor, enum Item item)")
        source = source.replace(target, "")
        source = "\n".join(line for line in source.splitlines() if not line.startswith("#include"))
        cases = r'''
int main(void){
 ScInitTrainerProgress(&save.sc);gBattleTypeFlags=BATTLE_TYPE_TRAINER;params.opponentA=TRAINER_LEADER_MISTY;
 npcItems[0]=ITEM_SUPER_POTION;inventory[ITEM_POTION]=1;
 assert(ScSuppliesBeginBattle());assert(!ScSuppliesCanUse(PLAYER_LEFT,ITEM_POTION));
 cheats=1u<<SC_DEBUG_FREE_SUPPLIES;
 assert(ScSuppliesCanUse(PLAYER_LEFT,ITEM_POTION));assert(ScSuppliesReserve(PLAYER_LEFT,ITEM_POTION));
 assert(ScSuppliesCommit(PLAYER_LEFT,ITEM_POTION));assert(!inventory[ITEM_POTION]);
 assert(!ScSuppliesCommit(PLAYER_LEFT,ITEM_POTION));assert(!ScSuppliesCanUse(PLAYER_LEFT,ITEM_REVIVE));
 inventory[ITEM_REVIVE]=1;assert(ScSuppliesCanUse(PLAYER_LEFT,ITEM_REVIVE));
 assert(ScSuppliesReserve(ENEMY_LEFT,ITEM_SUPER_POTION));assert(ScSuppliesCommit(ENEMY_LEFT,ITEM_SUPER_POTION));
 assert(!ScSuppliesReserve(ENEMY_LEFT,ITEM_SUPER_POTION));
 cheats=0;inventory[ITEM_POTION]=5;assert(!ScSuppliesCanUse(PLAYER_LEFT,ITEM_POTION));
}
'''
        execute(OPTIONS + adapter.FIXTURE + source + cases,
                [ROOT / "engine/src/sc_supplies_core.c", ROOT / "engine/src/sc_progression_core.c"])


if __name__ == "__main__":
    unittest.main()
