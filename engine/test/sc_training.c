#include "global.h"
#include "sc_progression.h"
#include "constants/opponents_frlg.h"
#include "test/battle.h"

WILD_BATTLE_TEST("SC training: level-capped field survivor grows while unused reserve does not")
{
    GIVEN {
        ScProgressionEnableForTests(TRUE);
        PLAYER(SPECIES_PIKACHU) { Level(100); Moves(MOVE_DRAGON_RAGE); }
        PLAYER(SPECIES_RATTATA) { Level(100); }
        OPPONENT(SPECIES_CHANSEY) { Level(100); HP(1); Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_RAGE); }
    } THEN {
        EXPECT_GT(GetMonEVCount(&gParties[B_TRAINER_PLAYER][0]), 0);
        EXPECT_EQ(GetMonEVCount(&gParties[B_TRAINER_PLAYER][1]), 0);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), 100);
        EXPECT_EQ(ScProgressionFinishBattle(B_OUTCOME_WON), 0);
        ScProgressionEnableForTests(FALSE);
    }
}

// Recorded trainer transport controls the forced replacement prompt. Supplies
// remain upstream in this fixture so revival can isolate reward eligibility;
// campaign trainer bags prohibit it, while wild encounters permit it.
SINGLE_BATTLE_TEST("SC training: fainting remains disqualifying after revival and a later victory")
{
    GIVEN {
        ScProgressionEnableForTests(TRUE);
        ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
        PLAYER(SPECIES_PIKACHU) { Level(100); HP(1); Moves(MOVE_SPLASH, MOVE_DRAGON_RAGE); }
        PLAYER(SPECIES_BLASTOISE) { Level(100); }
        OPPONENT(SPECIES_CHANSEY) { Level(100); HP(40); Moves(MOVE_DRAGON_RAGE, MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_DRAGON_RAGE); SEND_OUT(player, 1); }
        TURN { USE_ITEM(player, ITEM_REVIVE, 0); MOVE(opponent, MOVE_SPLASH); }
        TURN { SWITCH(player, 0); MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_DRAGON_RAGE); MOVE(opponent, MOVE_SPLASH); }
    } THEN {
        EXPECT_GT(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 0);
        EXPECT_EQ(GetMonEVCount(&gParties[B_TRAINER_PLAYER][0]), 0);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SC_TRAINING_REMAINDER), 0);
        EXPECT_GT(GetMonEVCount(&gParties[B_TRAINER_PLAYER][1])
            + GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_SC_TRAINING_REMAINDER), 0);
        EXPECT_GT(gSaveBlock3Ptr->sc.practiceExp, 0);
    }
}

SINGLE_BATTLE_TEST("SC training: a gym rewards switched-out support and the finisher but not unused reserves")
{
    GIVEN {
        ScProgressionEnableForTests(TRUE);
        ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
        gBattleTestRunnerState->data.recordedBattle.opponentA = TRAINER_LEADER_BROCK;
        PLAYER(SPECIES_PIKACHU) { Level(100); Moves(MOVE_SPLASH); }
        PLAYER(SPECIES_BUTTERFREE) { Level(100); Moves(MOVE_DRAGON_RAGE); }
        PLAYER(SPECIES_CHARMANDER) { Level(100); }
        OPPONENT(SPECIES_CHANSEY) { Level(100); HP(1); Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
        TURN { SWITCH(player, 1); MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_DRAGON_RAGE); MOVE(opponent, MOVE_SPLASH); }
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SC_EARNED_CAPABILITIES), 1);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_SC_EARNED_CAPABILITIES), 1);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_SC_EARNED_CAPABILITIES), 0);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SC_ACTIVE_CAPABILITIES), 0);
        EXPECT_GT(GetMonEVCount(&gParties[B_TRAINER_PLAYER][0])
            + GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SC_TRAINING_REMAINDER), 0);
        EXPECT_EQ(ScProgressionFinishBattle(B_OUTCOME_WON), 0);
    }
}

SINGLE_BATTLE_TEST("SC training: defeating one opponent before losing commits no training or reward")
{
    GIVEN {
        ScProgressionEnableForTests(TRUE);
        ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
        gBattleTestRunnerState->data.recordedBattle.opponentA = TRAINER_LEADER_BROCK;
        PLAYER(SPECIES_PIKACHU) { Level(100); HP(1); Speed(100); Moves(MOVE_DRAGON_RAGE); }
        OPPONENT(SPECIES_CHANSEY) { Level(100); HP(1); Speed(50); Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_RATTATA) { Level(100); Speed(150); Moves(MOVE_DRAGON_RAGE); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_RAGE); MOVE(opponent, MOVE_SPLASH); SEND_OUT(opponent, 1); }
        TURN { MOVE(player, MOVE_DRAGON_RAGE); MOVE(opponent, MOVE_DRAGON_RAGE); }
    } THEN {
        EXPECT_EQ(GetMonEVCount(&gParties[B_TRAINER_PLAYER][0]), 0);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SC_TRAINING_REMAINDER), 0);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SC_EARNED_CAPABILITIES), 0);
        EXPECT_EQ(gSaveBlock3Ptr->sc.practiceExp, 0);
        EXPECT_EQ(ScProgressionFinishBattle(B_OUTCOME_WON), 0);
    }
}

WILD_BATTLE_TEST("SC training: capture does not create defeated-opponent training")
{
    GIVEN {
        ScProgressionEnableForTests(TRUE);
        PLAYER(SPECIES_PIKACHU) { Level(100); }
        OPPONENT(SPECIES_CHANSEY) { Level(15); HP(1); }
    } WHEN {
        TURN { USE_ITEM(player, ITEM_MASTER_BALL); }
    } THEN {
        EXPECT_EQ(GetMonEVCount(&gParties[B_TRAINER_PLAYER][0]), 0);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SC_TRAINING_REMAINDER), 0);
        ScProgressionEnableForTests(FALSE);
    }
}
