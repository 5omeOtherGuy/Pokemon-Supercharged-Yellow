#include "global.h"
#include "battle.h"
#include "sc_ai.h"
#include "sc_battle.h"
#include "test/battle.h"

static EWRAM_DATA struct ScAiObservation sNeutral;
static EWRAM_DATA struct ScAiObservation sBrace;

AI_SINGLE_BATTLE_TEST("SC AI: public physical defense changes damage estimates and chosen category")
{
    GIVEN {
        ScAiEnableForTests(TRUE);
        ScEffectsEnableForTests(TRUE);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE);
        PLAYER(SPECIES_DITTO) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_PIKACHU) { Attack(100); SpAttack(100); Moves(MOVE_WING_ATTACK, MOVE_SWIFT); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); }
    } THEN {
        u32 mask = 0;
        struct ScAiChoice neutral[SC_AI_ACTORS], brace[SC_AI_ACTORS];
        SetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_SC_ACTIVE_CAPABILITIES, &mask);
        gBattleMons[B_BATTLER_0].hp = gBattleMons[B_BATTLER_0].maxHP;
        ScAiObserve(&sNeutral);
        ScAiChoose(&sNeutral, neutral);
        mask = 1u << SC_CAP_BRACE;
        SetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_SC_ACTIVE_CAPABILITIES, &mask);
        ScAiObserve(&sBrace);
        ScAiChoose(&sBrace, brace);
        EXPECT_EQ(neutral[0].kind, SC_AI_MOVE);
        EXPECT_EQ(brace[0].kind, SC_AI_MOVE);
        EXPECT_EQ(neutral[0].index, 0);
        EXPECT_EQ(brace[0].index, 1);
        EXPECT_LT(sBrace.options[0][0].damage[0], sNeutral.options[0][0].damage[0]);
        EXPECT_EQ(sBrace.options[0][1].damage[0], sNeutral.options[0][1].damage[0]);
    }
}
