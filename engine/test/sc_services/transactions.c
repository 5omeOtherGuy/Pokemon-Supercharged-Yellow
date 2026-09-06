#include "global.h"
#include "main.h"
#include "pokemon.h"
#include "sc_services.h"
#include "sc_progression.h"
#include "constants/maps.h"
#include "test/test.h"

TEST("SC services: encrypted IV improvement preserves nature, damage and other traits")
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    ScProgressionEnableForTests(TRUE);
    gMain.inBattle = FALSE;
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_VIRIDIAN_CITY_POKEMON_CENTER_2F);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_VIRIDIAN_CITY_POKEMON_CENTER_2F);
    ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
    gSaveBlock3Ptr->sc.practiceExp = 20;
    CreateMonWithIVs(mon, SPECIES_PIKACHU, 5, 0, OTID_STRUCT_PRESET(0), 15);
    u32 nature = GetNature(mon), hp = GetMonData(mon, MON_DATA_MAX_HP) - 3;
    SetMonData(mon, MON_DATA_HP, &hp);
    EXPECT_EQ(ScServicesImproveIv(0, 0), SC_SERVICE_OK);
    EXPECT_EQ(GetMonData(mon, MON_DATA_HP_IV), 16);
    EXPECT_EQ(GetMonData(mon, MON_DATA_ATK_IV), 15);
    EXPECT_EQ(GetMonData(mon, MON_DATA_MAX_HP) - GetMonData(mon, MON_DATA_HP), 3);
    EXPECT_EQ(GetNature(mon), nature);
    EXPECT_EQ(GetMonData(mon, MON_DATA_SANITY_IS_BAD_EGG), FALSE);
    EXPECT_EQ(gSaveBlock3Ptr->sc.practiceExp, 0);
    EXPECT_EQ(ScValidateTrainerProgress(&gSaveBlock3Ptr->sc), TRUE);
    ScProgressionEnableForTests(FALSE);
}

TEST("SC services: field context cannot mutate builds and focus preserves permanent points")
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    ScProgressionEnableForTests(TRUE);
    gMain.inBattle = FALSE;
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_PALLET_TOWN);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_PALLET_TOWN);
    CreateMonWithIVs(mon, SPECIES_PIKACHU, 5, 0, OTID_STRUCT_PRESET(0), 15);
    u32 points = 17, earned = 3;
    SetMonData(mon, MON_DATA_SPEED_EV, &points);
    SetMonData(mon, MON_DATA_SC_EARNED_CAPABILITIES, &earned);
    EXPECT_EQ(ScServicesSetCapabilities(0, 3), SC_SERVICE_BAD_CONTEXT);
    EXPECT_EQ(GetMonData(mon, MON_DATA_SC_ACTIVE_CAPABILITIES), 0);
    EXPECT_EQ(ScServicesSetFocus(0, 3), SC_SERVICE_OK);
    EXPECT_EQ(GetMonData(mon, MON_DATA_SC_FOCUS), 3);
    EXPECT_EQ(GetMonData(mon, MON_DATA_SPEED_EV), 17);
    gMain.inBattle = TRUE;
    EXPECT_EQ(ScServicesSetFocus(0, 2), SC_SERVICE_BAD_CONTEXT);
    EXPECT_EQ(GetMonData(mon, MON_DATA_SC_FOCUS), 3);
    gMain.inBattle = FALSE;
    ScProgressionEnableForTests(FALSE);
}
