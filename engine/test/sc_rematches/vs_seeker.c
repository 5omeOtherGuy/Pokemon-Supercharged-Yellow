#include "global.h"
#include "battle_setup.h"
#include "battle.h"
#include "event_data.h"
#include "item.h"
#include "pokemon.h"
#include "test/test.h"
#include "vs_seeker.h"
#include "constants/maps.h"

#if SC_TEST_CAMPAIGN
// All four cases run with real campaign rules and trainer data, never sparse fixtures.
TEST("SC rematches: Vermilion item charges without five badges and saturates")
{
    const u16 badgeFlags[] = {FLAG_BADGE01_GET,FLAG_BADGE02_GET,FLAG_BADGE03_GET,FLAG_BADGE04_GET,
        FLAG_BADGE05_GET,FLAG_BADGE06_GET,FLAG_BADGE07_GET,FLAG_BADGE08_GET};
    bool32 badges[8];
    for (u32 i = 0; i < 8; i++) { badges[i] = FlagGet(badgeFlags[i]); FlagClear(badgeFlags[i]); }
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
    for (u32 i = 0; i < 8; i++) if (badges[i]) FlagSet(badgeFlags[i]);
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

#include "sc_rematches.h"
#include "sc_progression.h"

TEST("SC rematches: high trainer IDs preserve identity and consume readiness once")
{
    struct WarpData location = gSaveBlock1Ptr->location;
    u32 battleFlags = gBattleTypeFlags;
    TrainerBattleParameter battleParams = gTrainerBattleParameter;
    bool32 owned = CheckBagHasItem(ITEM_VS_SEEKER, 1);
    bool32 defeated = HasTrainerBeenFought(TRAINER_TWINS_ELI_ANNE);
    bool32 charging = FlagGet(FLAG_SYS_VS_SEEKER_CHARGING);
    bool32 badges[8];
    bool32 champion = FlagGet(FLAG_IS_CHAMPION);
    FlagClear(FLAG_IS_CHAMPION);
    const u16 badgeFlags[] = {FLAG_BADGE01_GET,FLAG_BADGE02_GET,FLAG_BADGE03_GET,FLAG_BADGE04_GET,
        FLAG_BADGE05_GET,FLAG_BADGE06_GET,FLAG_BADGE07_GET,FLAG_BADGE08_GET};
    s32 index = FirstBattleTrainerIdToRematchTableId(gRematchTable, TRAINER_TWINS_ELI_ANNE);
    EXPECT(index >= 0 && index < MAX_REMATCH_ENTRIES);
    if (index < 0 || index >= MAX_REMATCH_ENTRIES) return;
    u8 previousReady = gSaveBlock1Ptr->trainerRematches[index];
    for (u32 i = 0; i < 8; i++) { badges[i] = FlagGet(badgeFlags[i]); FlagClear(badgeFlags[i]); }
    if (!owned) AddBagItem(ITEM_VS_SEEKER, 1);
    SetTrainerFlag(TRAINER_TWINS_ELI_ANNE);
    FlagSet(FLAG_SYS_VS_SEEKER_CHARGING);
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_ROUTE8);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_ROUTE8);
    EXPECT(TRAINER_TWINS_ELI_ANNE > 255);
    EXPECT_EQ(GetRematchTrainerIdVSSeeker(TRAINER_TWINS_ELI_ANNE), TRAINER_NONE);
    FlagSet(FLAG_BADGE01_GET); // Level 22 party is now eligible, without changing it.
    EXPECT(CheckBagHasItem(ITEM_VS_SEEKER, 1));
    EXPECT(HasTrainerBeenFought(TRAINER_TWINS_ELI_ANNE));
    EXPECT_EQ(ScGetBadgeCount(), 1);
    EXPECT_EQ(gRematchTable[index].mapGroup, gSaveBlock1Ptr->location.mapGroup);
    EXPECT_EQ(gRematchTable[index].mapNum, gSaveBlock1Ptr->location.mapNum);
    EXPECT_EQ(GetTrainerPartySizeFromId(TRAINER_TWINS_ELI_ANNE), 2);
    EXPECT_EQ((u32)GetTrainerPartyFromId(TRAINER_TWINS_ELI_ANNE)[0].species, SPECIES_CLEFAIRY);
    EXPECT_EQ((u32)GetTrainerPartyFromId(TRAINER_TWINS_ELI_ANNE)[1].species, SPECIES_JIGGLYPUFF);
    EXPECT_EQ((u32)GetTrainerPartyFromId(TRAINER_TWINS_ELI_ANNE)[0].lvl, 22);
    EXPECT_EQ((u32)GetTrainerPartyFromId(TRAINER_TWINS_ELI_ANNE)[1].lvl, 22);
    EXPECT_EQ(GetRematchTrainerIdVSSeeker(TRAINER_TWINS_ELI_ANNE), TRAINER_TWINS_ELI_ANNE);
    EXPECT(ScRematchSetReady(TRAINER_TWINS_ELI_ANNE));
    EXPECT_EQ(gSaveBlock1Ptr->trainerRematches[index], 1);
    EXPECT(ShouldTryRematchBattleForTrainerId(TRAINER_TWINS_ELI_ANNE));
    {
        struct Pokemon previousParty[PARTY_SIZE];
        u8 previousCount = gPartiesCount[B_TRAINER_PLAYER];
        TrainerBattleParameter parameters = {0};
        u8 script[TRAINERBATTLE_OPCODE_OFFSET + sizeof(parameters)] = {0};
        memcpy(previousParty, gParties[B_TRAINER_PLAYER], sizeof(previousParty));
        memset(gParties[B_TRAINER_PLAYER], 0, sizeof(previousParty));
        CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 5, 0, OTID_STRUCT_PRESET(1234), 15);
        parameters.params.opponentA = TRAINER_TWINS_ELI_ANNE;
        memcpy(script + TRAINERBATTLE_OPCODE_OFFSET, &parameters, sizeof(parameters));
        // Automatic sight must not repeatedly trap a one-mon party in refusal.
        EXPECT(!GetRematchFromScriptPointer(script));
        EXPECT(ShouldTryRematchBattleForTrainerId(TRAINER_TWINS_ELI_ANNE));
        EXPECT_EQ(gSaveBlock1Ptr->trainerRematches[index], 1);
        CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][1], SPECIES_BULBASAUR, 5, 0, OTID_STRUCT_PRESET(1234), 15);
        EXPECT(GetRematchFromScriptPointer(script));
        SetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_HP, &((u16){0}));
        EXPECT(!GetRematchFromScriptPointer(script));
        EXPECT_EQ(gSaveBlock1Ptr->trainerRematches[index], 1);
        memcpy(gParties[B_TRAINER_PLAYER], previousParty, sizeof(previousParty));
        gPartiesCount[B_TRAINER_PLAYER] = previousCount;
    }
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_ROUTE11);
    EXPECT(!ShouldTryRematchBattleForTrainerId(TRAINER_TWINS_ELI_ANNE));
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_ROUTE8);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_TWINS_ELI_ANNE;
    TRAINER_BATTLE_PARAM.opponentB = TRAINER_NONE;
    ClearCurrentTrainerWantRematchVsSeeker();
    EXPECT(!ShouldTryRematchBattleForTrainerId(TRAINER_TWINS_ELI_ANNE));
    EXPECT(!ScRematchSetReady(65535));
    gSaveBlock1Ptr->trainerRematches[index] = previousReady;
    gSaveBlock1Ptr->location = location;
    gBattleTypeFlags = battleFlags;
    gTrainerBattleParameter = battleParams;
    for (u32 i = 0; i < 8; i++) { if (badges[i]) FlagSet(badgeFlags[i]); else FlagClear(badgeFlags[i]); }
    if (!charging) FlagClear(FLAG_SYS_VS_SEEKER_CHARGING);
    if (!defeated) ClearTrainerFlag(TRAINER_TWINS_ELI_ANNE);
    if (champion) FlagSet(FLAG_IS_CHAMPION);
    if (!owned) RemoveBagItem(ITEM_VS_SEEKER, 1);
}

TEST("SC rematches: response expiry clears every slot and retains recharge")
{
    u16 previous = gSaveBlock1Ptr->trainerRematchStepCounter;
    u8 previousReady[MAX_REMATCH_ENTRIES];
    bool32 owned = CheckBagHasItem(ITEM_VS_SEEKER, 1);
    bool32 charging = FlagGet(FLAG_SYS_VS_SEEKER_CHARGING);
    memcpy(previousReady, gSaveBlock1Ptr->trainerRematches, sizeof(previousReady));
    if (!owned) AddBagItem(ITEM_VS_SEEKER, 1);
    memset(gSaveBlock1Ptr->trainerRematches, 1, sizeof(previousReady));
    FlagSet(FLAG_SYS_VS_SEEKER_CHARGING);
    gSaveBlock1Ptr->trainerRematchStepCounter = (98 << 8) | 98;
    EXPECT(!UpdateVsSeekerStepCounter());
    EXPECT_EQ(gSaveBlock1Ptr->trainerRematchStepCounter, (99 << 8) | 99);
    EXPECT(UpdateVsSeekerStepCounter());
    EXPECT_EQ(gSaveBlock1Ptr->trainerRematchStepCounter, 100);
    EXPECT(!FlagGet(FLAG_SYS_VS_SEEKER_CHARGING));
    for (u32 i = 0; i < MAX_REMATCH_ENTRIES; i++) EXPECT_EQ(gSaveBlock1Ptr->trainerRematches[i], 0);
    // Malformed packed values also clear safely instead of wrapping or trapping.
    FlagSet(FLAG_SYS_VS_SEEKER_CHARGING);
    gSaveBlock1Ptr->trainerRematchStepCounter = 65535;
    EXPECT(UpdateVsSeekerStepCounter());
    EXPECT_EQ(gSaveBlock1Ptr->trainerRematchStepCounter, 100);
    memcpy(gSaveBlock1Ptr->trainerRematches, previousReady, sizeof(previousReady));
    gSaveBlock1Ptr->trainerRematchStepCounter = previous;
    if (charging) FlagSet(FLAG_SYS_VS_SEEKER_CHARGING);
    if (!owned) RemoveBagItem(ITEM_VS_SEEKER, 1);
}

#endif // SC_TEST_CAMPAIGN
