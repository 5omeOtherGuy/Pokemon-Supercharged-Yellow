#include "global.h"
#include "battle_setup.h"
#include "event_data.h"
#include "sc_build_catalog.h"
#include "sc_progression.h"
#include "constants/opponents_frlg.h"
#include "test/test.h"

TEST("SC unlocks: hiding the ship rival does not grant victory expertise")
{
    InitEventData();
    ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
    // This visibility flag is set by the actual new-game map-reset script.
    FlagSet(FLAG_HIDE_SS_ANNE_RIVAL);
    ScRefreshTrainerUnlocks();
    EXPECT_EQ(gSaveBlock3Ptr->sc.unlockedPassives,
        (1u << SC_PASSIVE_STUDY) | (1u << SC_PASSIVE_FORMATION));
    for (u32 trainer = TRAINER_RIVAL_SS_ANNE_SQUIRTLE; trainer <= TRAINER_RIVAL_SS_ANNE_CHARMANDER; trainer++)
    {
        ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
        SetTrainerFlag(trainer);
        ScRefreshTrainerUnlocks();
        EXPECT_EQ(gSaveBlock3Ptr->sc.unlockedPassives,
            (1u << SC_PASSIVE_STUDY) | (1u << SC_PASSIVE_FORMATION) | (1u << SC_PASSIVE_TEMPO));
        ClearTrainerFlag(trainer);
    }
    InitEventData();
}
