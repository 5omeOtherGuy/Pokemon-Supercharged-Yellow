#include "global.h"
#include "battle.h"
#include "battle_util.h"
#include "sc_battle.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("SC runtime effects: speed combines only active public effects")
{
    GIVEN {
        PLAYER(SPECIES_PIKACHU) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_MAGIKARP) { Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } THEN {
        u32 active = 1u << SC_CAP_QUICK_START;
        ScEffectsEnableForTests(TRUE);
        SetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_SC_ACTIVE_CAPABILITIES, &active);
        gSaveBlock3Ptr->sc.activePassives = 1u << SC_PASSIVE_TEMPO;
        gBattleStruct->battlerState[B_BATTLER_0].isFirstTurn = 1;
        EXPECT_EQ(ScApplySpeedEffects(B_BATTLER_0, 100), 114);
        gBattleStruct->battlerState[B_BATTLER_0].isFirstTurn = 0;
        EXPECT_EQ(ScApplySpeedEffects(B_BATTLER_0, 100), 104);
        EXPECT_EQ(ScApplySpeedEffects(B_BATTLER_0, 0), 0);
        ScEffectsEnableForTests(FALSE);
    }
}
