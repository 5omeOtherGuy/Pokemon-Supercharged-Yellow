#include "global.h"
#include "sc_progression.h"
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
