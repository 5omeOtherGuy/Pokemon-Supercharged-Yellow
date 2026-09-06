#include "global.h"
#include "battle_setup.h"
#include "event_data.h"
#include "item.h"
#include "test/test.h"
#include "vs_seeker.h"
#include "constants/maps.h"

TEST("SC rematches: Vermilion item charges without five badges and saturates")
{
    u16 previous = gSaveBlock1Ptr->trainerRematchStepCounter;
    bool32 owned = CheckBagHasItem(ITEM_VS_SEEKER, 1);
    bool32 charging = FlagGet(FLAG_SYS_VS_SEEKER_CHARGING);
    if (!owned) AddBagItem(ITEM_VS_SEEKER, 1);
    FlagClear(FLAG_SYS_VS_SEEKER_CHARGING);
    gSaveBlock1Ptr->trainerRematchStepCounter = 0;
    for (u32 step = 0; step < 99; step++) EXPECT(!UpdateVsSeekerStepCounter());
    EXPECT_EQ(gSaveBlock1Ptr->trainerRematchStepCounter, 99);
    EXPECT(!UpdateVsSeekerStepCounter());
    EXPECT_EQ(gSaveBlock1Ptr->trainerRematchStepCounter, 100);
    for (u32 step = 0; step < 20; step++) EXPECT(!UpdateVsSeekerStepCounter());
    EXPECT_EQ(gSaveBlock1Ptr->trainerRematchStepCounter, 100);
    gSaveBlock1Ptr->trainerRematchStepCounter = previous;
    if (charging) FlagSet(FLAG_SYS_VS_SEEKER_CHARGING);
    if (!owned) RemoveBagItem(ITEM_VS_SEEKER, 1);
}

TEST("SC rematches: first route parties resolve full trainer IDs without Hoenn milestones")
{
    struct WarpData location = gSaveBlock1Ptr->location;
    bool32 owned = CheckBagHasItem(ITEM_VS_SEEKER, 1);
    bool32 defeated = HasTrainerBeenFought(TRAINER_YOUNGSTER_BEN);
    if (!owned) AddBagItem(ITEM_VS_SEEKER, 1);
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_ROUTE3);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_ROUTE3);
    SetTrainerFlag(TRAINER_YOUNGSTER_BEN);
    EXPECT_EQ(GetRematchTrainerIdVSSeeker(TRAINER_YOUNGSTER_BEN), TRAINER_YOUNGSTER_BEN);
    EXPECT_EQ(GetRematchTrainerIdVSSeeker(TRAINER_NONE), TRAINER_NONE);
    EXPECT_EQ(GetRematchTrainerIdVSSeeker(65535), TRAINER_NONE);
    EXPECT_EQ(GetRematchTrainerIdVSSeeker(TRAINER_LEADER_BROCK), TRAINER_NONE);
    ClearTrainerFlag(TRAINER_YOUNGSTER_BEN);
    EXPECT_EQ(GetRematchTrainerIdVSSeeker(TRAINER_YOUNGSTER_BEN), TRAINER_NONE);
    gSaveBlock1Ptr->location = location;
    if (defeated) SetTrainerFlag(TRAINER_YOUNGSTER_BEN);
    if (!owned) RemoveBagItem(ITEM_VS_SEEKER, 1);
}
