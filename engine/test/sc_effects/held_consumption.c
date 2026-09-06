#include "global.h"
#include "battle.h"
#include "battle_util.h"
#include "sc_supplies.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("SC supplies: consumed held items stay spent while actual recovery and Knock Off restore")
{
    enum Item original, current, expected;
    bool32 consumed;
    PARAMETRIZE { original = ITEM_FOCUS_SASH; current = ITEM_NONE; consumed = TRUE; expected = ITEM_NONE; }
    PARAMETRIZE { original = ITEM_AIR_BALLOON; current = ITEM_NONE; consumed = TRUE; expected = ITEM_NONE; }
    PARAMETRIZE { original = ITEM_SITRUS_BERRY; current = ITEM_NONE; consumed = TRUE; expected = ITEM_NONE; }
    PARAMETRIZE { original = ITEM_FOCUS_SASH; current = ITEM_FOCUS_SASH; consumed = TRUE; expected = ITEM_FOCUS_SASH; }
    PARAMETRIZE { original = ITEM_LEFTOVERS; current = ITEM_NONE; consumed = FALSE; expected = ITEM_LEFTOVERS; }
    PARAMETRIZE { original = ITEM_FOCUS_SASH; current = ITEM_LIFE_ORB; consumed = TRUE; expected = ITEM_NONE; }
    GIVEN {
        PLAYER(SPECIES_PIKACHU) { Item(original); }
        OPPONENT(SPECIES_MAGIKARP);
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } THEN {
        ScSuppliesEnableForTests(TRUE);
        ScSuppliesBeginBattle();
        gBattleStruct->itemLost[B_TRAINER_PLAYER][0].originalItem = original;
        SetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_HELD_ITEM, &current);
        if (consumed)
            ScSuppliesRecordHeldConsumption(B_TRAINER_PLAYER, 0, original);
        TryRestoreHeldItems();
        EXPECT_EQ(GetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_HELD_ITEM), expected);
        ScSuppliesEnableForTests(FALSE);
    }
}
