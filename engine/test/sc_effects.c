#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "sc_battle.h"
#include "sc_build_catalog.h"
#include "sc_progression_core.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("SC effects: public providers use active assignments and visible health bands")
{
    GIVEN {
        PLAYER(SPECIES_PIKACHU) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_GEODUDE) { Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } THEN {
        struct ScPublicEffects effects, sameBand;
        u32 active = (1u << SC_CAP_QUICK_START) | (1u << SC_CAP_LAST_STAND);
        u32 oldActive = GetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_SC_ACTIVE_CAPABILITIES);
        u32 oldPassives = gSaveBlock3Ptr->sc.activePassives;
        u32 oldUnlocked = gSaveBlock3Ptr->sc.unlockedPassives;
        u16 oldHp = gBattleMons[B_BATTLER_0].hp, oldMaxHp = gBattleMons[B_BATTLER_0].maxHP;
        u32 oldFirstTurn = gBattleStruct->battlerState[B_BATTLER_0].isFirstTurn;
        ScEffectsEnableForTests(TRUE);
        SetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_SC_ACTIVE_CAPABILITIES, &active);
        gSaveBlock3Ptr->sc.activePassives = 1u << SC_PASSIVE_TEMPO;
        gBattleMons[B_BATTLER_0].hp = 240;
        gBattleMons[B_BATTLER_0].maxHP = 480;
        gBattleStruct->battlerState[B_BATTLER_0].isFirstTurn = 1;
        ScGetPublicBattlerEffects(B_BATTLER_0, &effects);
        EXPECT_EQ(effects.speedPercent, 114);
        EXPECT_EQ(effects.offensivePhysicalPercent, 110);
        gBattleMons[B_BATTLER_0].hp = 249;
        gSaveBlock3Ptr->sc.unlockedPassives ^= 255;
        ScGetPublicBattlerEffects(B_BATTLER_0, &sameBand);
        EXPECT_EQ(memcmp(&effects, &sameBand, sizeof(effects)), 0);
        gBattleStruct->battlerState[B_BATTLER_0].isFirstTurn = 0;
        ScGetPublicBattlerEffects(B_BATTLER_0, &effects);
        EXPECT_EQ(effects.speedPercent, 104);
        ScGetPublicBattlerEffects(MAX_BATTLERS_COUNT, &effects);
        EXPECT_EQ(effects.speedPercent, 100);
        EXPECT_EQ(effects.offensivePhysicalPercent, 100);
        SetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_SC_ACTIVE_CAPABILITIES, &oldActive);
        gSaveBlock3Ptr->sc.activePassives = oldPassives;
        gSaveBlock3Ptr->sc.unlockedPassives = oldUnlocked;
        gBattleMons[B_BATTLER_0].hp = oldHp;
        gBattleMons[B_BATTLER_0].maxHP = oldMaxHp;
        gBattleStruct->battlerState[B_BATTLER_0].isFirstTurn = oldFirstTurn;
        ScEffectsEnableForTests(FALSE);
    }
}
