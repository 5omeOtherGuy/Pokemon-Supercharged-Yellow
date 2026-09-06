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
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_PSYCHIC); }
    } THEN {
        struct ScAiObservation first, second;
        struct BattlePokemon saved = gBattleMons[B_BATTLER_0];
        rng_value_t savedRng = gRngValue;
        enum Move savedMove = gChosenMoveByBattler[B_BATTLER_0];
        u8 savedAction = gChosenActionByBattler[B_BATTLER_0];
        u8 savedTarget = gBattleStruct->moveTarget[B_BATTLER_0];
        u8 savedSwitch = gBattleStruct->monToSwitchIntoId[B_BATTLER_0];
        gBattleMons[B_BATTLER_0].hp = gBattleMons[B_BATTLER_0].maxHP;
        ScAiObserve(&first);
        gBattleMons[B_BATTLER_0].attack = 999;
        gBattleMons[B_BATTLER_0].defense = 999;
        gBattleMons[B_BATTLER_0].spAttack = 999;
        gBattleMons[B_BATTLER_0].spDefense = 999;
        gBattleMons[B_BATTLER_0].speed = 999;
        gBattleMons[B_BATTLER_0].hpIV = 31;
        gBattleMons[B_BATTLER_0].attackIV = 31;
        gBattleMons[B_BATTLER_0].maxHP = 999;
        gBattleMons[B_BATTLER_0].hp = 999; /* Same full public HP bar. */
        gChosenMoveByBattler[B_BATTLER_0] = MOVE_PROTECT;
        gChosenActionByBattler[B_BATTLER_0] = B_ACTION_SWITCH;
        gBattleStruct->moveTarget[B_BATTLER_0] = B_BATTLER_3;
        gBattleStruct->monToSwitchIntoId[B_BATTLER_0] = 5;
        memset(&gRngValue, 0xa5, sizeof(gRngValue));
        struct BattlePokemon mutated = gBattleMons[B_BATTLER_0];
        ScAiObserve(&second);
        EXPECT_EQ(memcmp(&first, &second, sizeof(first)), 0);
        EXPECT_EQ(memcmp(&mutated, &gBattleMons[B_BATTLER_0], sizeof(mutated)), 0);
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
        ScAiEnableForTests(FALSE);
    }
}
