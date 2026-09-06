#include "global.h"
#include "test/battle.h"
#include "battle.h"
#include "battle_ai_main.h"
#include "sc_ai.h"
#include "random.h"

AI_SINGLE_BATTLE_TEST("SC AI: pending commands, exact player stats and battle RNG cannot change observations")
{
    GIVEN {
        ScAiEnableForTests(TRUE);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_SNORLAX) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_ALAKAZAM) { Moves(MOVE_PSYCHIC, MOVE_TACKLE); }
        OPPONENT(SPECIES_SNORLAX) { Moves(MOVE_CRUNCH); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_PSYCHIC); }
    } THEN {
        EXPECT(ScAiEnabled());
        struct ScAiObservation first, second;
        struct BattlePokemon saved = gBattleMons[B_BATTLER_0];
        rng_value_t savedRng = gRngValue, savedRng2 = gRng2Value;
        enum Move savedMove = gChosenMoveByBattler[B_BATTLER_0];
        u8 savedAction = gChosenActionByBattler[B_BATTLER_0];
        u8 savedTarget = gBattleStruct->moveTarget[B_BATTLER_0];
        u8 savedSwitch = gBattleStruct->monToSwitchIntoId[B_BATTLER_0];
        u16 savedItem = gBattleStruct->chosenItem[B_BATTLER_0];
        struct Pokemon savedParty = *GetBattlerMon(B_BATTLER_0);
        struct BattleHistory savedHistory = *gBattleHistory;
        gBattleHistory->trainerItems[0] = ITEM_POTION;
        gBattleHistory->trainerItems[1] = ITEM_FULL_HEAL;
        gBattleMons[B_BATTLER_0].hp = gBattleMons[B_BATTLER_0].maxHP;
        ScAiObserve(&first);
        u32 baselineSwitch = ScAiSwitchIn(B_BATTLER_1);
        ScAiPrepareTurn();
        u32 baselineMove = ScAiMoveIndex(B_BATTLER_1);
        bool32 hasSwitch = FALSE, hasItem = FALSE;
        for (u32 i = 0; i < first.count[0]; ++i)
        {
            hasSwitch |= first.options[0][i].kind == SC_AI_SWITCH;
            hasItem |= first.options[0][i].kind == SC_AI_ITEM;
        }
        EXPECT(hasSwitch);
        EXPECT(hasItem);
        gBattleMons[B_BATTLER_0].attack = 999;
        gBattleMons[B_BATTLER_0].defense = 999;
        gBattleMons[B_BATTLER_0].spAttack = 999;
        gBattleMons[B_BATTLER_0].spDefense = 999;
        gBattleMons[B_BATTLER_0].speed = 999;
        gBattleMons[B_BATTLER_0].hpIV = 31;
        gBattleMons[B_BATTLER_0].attackIV = 31;
        memset(gBattleMons[B_BATTLER_0].pp, 0, sizeof(gBattleMons[B_BATTLER_0].pp));
        gBattleMons[B_BATTLER_0].maxHP = 999;
        gBattleMons[B_BATTLER_0].hp = 999; /* Same full public HP bar. */
        gChosenMoveByBattler[B_BATTLER_0] = MOVE_PROTECT;
        gChosenActionByBattler[B_BATTLER_0] = B_ACTION_SWITCH;
        gBattleStruct->moveTarget[B_BATTLER_0] = B_BATTLER_3;
        gBattleStruct->monToSwitchIntoId[B_BATTLER_0] = 5;
        gBattleStruct->chosenItem[B_BATTLER_0] = ITEM_FULL_RESTORE;
        u32 hidden = 31;
        for (u32 field = MON_DATA_HP_IV; field <= MON_DATA_SPDEF_IV; ++field)
            SetMonData(GetBattlerMon(B_BATTLER_0), field, &hidden);
        hidden = 252;
        for (u32 field = MON_DATA_HP_EV; field <= MON_DATA_SPDEF_EV; ++field)
            SetMonData(GetBattlerMon(B_BATTLER_0), field, &hidden);
        hidden = NATURE_MODEST; /* An injected unsupported mint alignment is private. */
        SetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_HIDDEN_NATURE, &hidden);
        struct Pokemon mutatedParty = *GetBattlerMon(B_BATTLER_0);
        memset(&gRngValue, 0xa5, sizeof(gRngValue));
        memset(&gRng2Value, 0x5a, sizeof(gRng2Value));
        struct BattlePokemon mutated = gBattleMons[B_BATTLER_0];
        rng_value_t mutatedRng = gRngValue, mutatedRng2 = gRng2Value;
        ScAiObserve(&second);
        EXPECT_EQ(memcmp(&first, &second, sizeof(first)), 0);
        EXPECT_EQ(ScAiSwitchIn(B_BATTLER_1), baselineSwitch);
        ScAiPrepareTurn();
        EXPECT_EQ(ScAiMoveIndex(B_BATTLER_1), baselineMove);
        EXPECT_EQ(memcmp(&mutatedRng, &gRngValue, sizeof(mutatedRng)), 0);
        EXPECT_EQ(memcmp(&mutatedRng2, &gRng2Value, sizeof(mutatedRng2)), 0);
        EXPECT_EQ(memcmp(&mutated, &gBattleMons[B_BATTLER_0], sizeof(mutated)), 0);
        EXPECT_EQ(memcmp(&mutatedParty, GetBattlerMon(B_BATTLER_0), sizeof(mutatedParty)), 0);
        struct ScAiChoice firstChoice[2], secondChoice[2];
        ScAiChoose(&first, firstChoice);
        ScAiChoose(&second, secondChoice);
        EXPECT_EQ(memcmp(firstChoice, secondChoice, sizeof(firstChoice)), 0);
        gBattleMons[B_BATTLER_0] = saved;
        gChosenMoveByBattler[B_BATTLER_0] = savedMove;
        gChosenActionByBattler[B_BATTLER_0] = savedAction;
        gBattleStruct->moveTarget[B_BATTLER_0] = savedTarget;
        gBattleStruct->monToSwitchIntoId[B_BATTLER_0] = savedSwitch;
        gRngValue = savedRng;
        gRng2Value = savedRng2;
        gBattleStruct->chosenItem[B_BATTLER_0] = savedItem;
        *GetBattlerMon(B_BATTLER_0) = savedParty;
        *gBattleHistory = savedHistory;
        ScAiEnableForTests(FALSE);
    }
}

AI_SINGLE_BATTLE_TEST("SC AI: different exact HP values within one visible band have identical options")
{
    GIVEN {
        ScAiEnableForTests(TRUE);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE);
        PLAYER(SPECIES_SNORLAX) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_ALAKAZAM) { Moves(MOVE_PSYCHIC, MOVE_THUNDER_WAVE, MOVE_RECOVER); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); }
    } THEN {
        EXPECT(ScAiEnabled());
        struct ScAiObservation first, second;
        struct BattlePokemon saved = gBattleMons[B_BATTLER_0];
        gBattleMons[B_BATTLER_0].maxHP = 480;
        gBattleMons[B_BATTLER_0].hp = 241;
        ScAiObserve(&first);
        gBattleMons[B_BATTLER_0].hp = 249;
        ScAiObserve(&second);
        EXPECT_EQ(memcmp(&first, &second, sizeof(first)), 0);
        gBattleMons[B_BATTLER_0] = saved;
        ScAiEnableForTests(FALSE);
    }
}

AI_DOUBLE_BATTLE_TEST("SC AI: doubles coordinates separate KOs without reading either player target")
{
    GIVEN {
        ScAiEnableForTests(TRUE);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_MAGIKARP) { Level(5); Moves(MOVE_SPLASH); }
        PLAYER(SPECIES_MAGIKARP) { Level(5); Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_ALAKAZAM) { Level(50); Moves(MOVE_PSYCHIC); }
        OPPONENT(SPECIES_ALAKAZAM) { Level(50); Moves(MOVE_PSYCHIC); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SPLASH);
            MOVE(playerRight, MOVE_SPLASH);
            EXPECT_MOVE(opponentLeft, MOVE_PSYCHIC, target: playerLeft);
            EXPECT_MOVE(opponentRight, MOVE_PSYCHIC, target: playerRight);
        }
    } THEN {
        EXPECT(ScAiEnabled());
        ScAiEnableForTests(FALSE);
    }
}

AI_SINGLE_BATTLE_TEST("SC AI: a known immunity makes a useful switch preferable to an ineffective attack")
{
    GIVEN {
        ScAiEnableForTests(TRUE);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_SMART_SWITCHING);
        PLAYER(SPECIES_GENGAR) { Moves(MOVE_SHADOW_BALL); }
        OPPONENT(SPECIES_RATTATA) { Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_SNORLAX) { Moves(MOVE_CRUNCH); }
    } WHEN {
        TURN { MOVE(player, MOVE_SHADOW_BALL); EXPECT_SWITCH(opponent, 1); }
    } THEN {
        EXPECT(ScAiEnabled());
        ScAiEnableForTests(FALSE);
    }
}

AI_SINGLE_BATTLE_TEST("SC AI: public nature changes the estimated matchup")
{
    GIVEN {
        ScAiEnableForTests(TRUE);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE);
        PLAYER(SPECIES_SNORLAX) { Nature(NATURE_HARDY); Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_ALAKAZAM) { Moves(MOVE_PSYCHIC); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); }
    } THEN {
        EXPECT(ScAiEnabled());
        struct ScAiObservation first, second;
        struct Pokemon saved = *GetBattlerMon(B_BATTLER_0);
        ScAiObserve(&first);
        u32 personality = NATURE_CALM;
        SetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_PERSONALITY, &personality);
        ScAiObserve(&second);
        EXPECT_LT(second.options[0][0].damage[0], first.options[0][0].damage[0]);
        *GetBattlerMon(B_BATTLER_0) = saved;
        ScAiEnableForTests(FALSE);
    }
}

AI_SINGLE_BATTLE_TEST("SC AI: selects coverage from public typing")
{
    enum Species species;
    enum Move move;
    PARAMETRIZE { species = SPECIES_GYARADOS; move = MOVE_THUNDERBOLT; }
    PARAMETRIZE { species = SPECIES_DRAGONITE; move = MOVE_ICE_BEAM; }
    PARAMETRIZE { species = SPECIES_MACHAMP; move = MOVE_PSYCHIC; }
    PARAMETRIZE { species = SPECIES_ARCANINE; move = MOVE_SURF; }
    GIVEN {
        ScAiEnableForTests(TRUE);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE);
        PLAYER(species) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_STARMIE) { Moves(MOVE_SURF, MOVE_THUNDERBOLT, MOVE_PSYCHIC, MOVE_ICE_BEAM); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, move); }
    } THEN {
        EXPECT(ScAiEnabled());
        ScAiEnableForTests(FALSE);
    }
}

AI_SINGLE_BATTLE_TEST("SC AI: ignores a known ability immunity")
{
    GIVEN {
        ScAiEnableForTests(TRUE);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE);
        PLAYER(SPECIES_VAPOREON) { Ability(ABILITY_WATER_ABSORB); Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_STARMIE) { Moves(MOVE_SURF, MOVE_THUNDERBOLT); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_THUNDERBOLT); }
    } THEN {
        EXPECT(ScAiEnabled());
        ScAiEnableForTests(FALSE);
    }
}
