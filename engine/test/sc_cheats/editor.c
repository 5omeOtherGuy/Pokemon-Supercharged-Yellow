#include "global.h"
#include "main.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "sc_debug.h"
#include "sc_progression.h"
#include "event_data.h"
#include "data.h"
#include "test/test.h"

#if SC_TEST_TOOLS
TEST("SC cheats: nature and gender edits preserve encrypted personal development")
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    gMain.inBattle = FALSE;
    CreateMonWithIVs(mon, SPECIES_PIKACHU, 5, 0, OTID_STRUCT_PRESET(0), 15);
    u32 earned = 0xFFF, active = 3, focus = 2, training = 77, fraction = 123;
    SetMonData(mon, MON_DATA_SC_EARNED_CAPABILITIES, &earned);
    SetMonData(mon, MON_DATA_SC_ACTIVE_CAPABILITIES, &active);
    SetMonData(mon, MON_DATA_SC_FOCUS, &focus);
    SetMonData(mon, MON_DATA_SC_TRAINING_REMAINDER, &fraction);
    SetMonData(mon, MON_DATA_DEF_EV, &training);
    for (u32 nature = 0; nature < NUM_NATURES; nature++)
    {
        EXPECT(ScDebugEditMon(0, SC_EDIT_NATURE, 0, nature));
        EXPECT_EQ(GetNature(mon), nature);
        EXPECT_EQ(GetMonData(mon, MON_DATA_SC_EARNED_CAPABILITIES), earned);
        EXPECT_EQ(GetMonData(mon, MON_DATA_SC_ACTIVE_CAPABILITIES), active);
        EXPECT_EQ(GetMonData(mon, MON_DATA_SC_FOCUS), focus);
        EXPECT_EQ(GetMonData(mon, MON_DATA_SC_TRAINING_REMAINDER), fraction);
        EXPECT_EQ(GetMonData(mon, MON_DATA_DEF_EV), training);
        EXPECT_EQ(GetMonData(mon, MON_DATA_SANITY_IS_BAD_EGG), FALSE);
    }
    EXPECT(ScDebugEditMon(0, SC_EDIT_GENDER, 0, 1));
    EXPECT_EQ(GetMonGender(mon), MON_FEMALE);
    EXPECT_EQ(GetNature(mon), NUM_NATURES - 1);
    EXPECT(ScDebugEditMon(0, SC_EDIT_SHINY, 0, 1));
    EXPECT_EQ(GetMonData(mon, MON_DATA_IS_SHINY), TRUE);
}

TEST("SC cheats: invalid edits and battle context leave the Pokemon unchanged")
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    gMain.inBattle = FALSE;
    CreateMonWithIVs(mon, SPECIES_PIKACHU, 5, 0, OTID_STRUCT_PRESET(0), 15);
    struct Pokemon before = *mon;
    EXPECT(!ScDebugEditMon(PARTY_SIZE, SC_EDIT_IV, 0, 31));
    EXPECT(!ScDebugEditMon(0, SC_EDIT_IV, 6, 31));
    EXPECT(!ScDebugEditMon(0, SC_EDIT_SPECIES, 0, 152));
    EXPECT(!ScDebugEditMon(0, SC_EDIT_MOVE, 4, MOVE_TACKLE));
    EXPECT(!ScDebugEditMon(0, SC_EDIT_ABILITY, 0, 1)); // Pikachu's absent slot.
    EXPECT(!ScDebugEditMon(0, SC_EDIT_GENDER, 0, 2));
    EXPECT_EQ(memcmp(mon, &before, sizeof(before)), 0);
    gMain.inBattle = TRUE;
    EXPECT(!ScDebugEditMon(0, SC_EDIT_LEVEL, 0, 100));
    EXPECT(!ScDebugGiveMon(SPECIES_MEW, 5));
    gMain.inBattle = FALSE;
    EXPECT_EQ(memcmp(mon, &before, sizeof(before)), 0);
}

TEST("SC cheats: original151 grant is atomic when storage is full")
{
    gMain.inBattle = FALSE;
    struct Pokemon mon;
    CreateMonWithIVs(&mon, SPECIES_MAGIKARP, 5, 0, OTID_STRUCT_PRESET(0), 15);
    for (u32 i = 0; i < PARTY_SIZE; i++) gParties[B_TRAINER_PLAYER][i] = mon;
    for (u32 b = 0; b < TOTAL_BOXES_COUNT; b++)
        for (u32 s = 0; s < IN_BOX_COUNT; s++) SetBoxMonAt(b, s, &mon.box);
    ZeroBoxMonAt(0, 0);
    EXPECT_EQ(ScDebugFreeSlots(), 1);
    EXPECT(!ScDebugGiveAll151(5));
    EXPECT_EQ(ScDebugFreeSlots(), 1);
    EXPECT(ScDebugGiveMon(SPECIES_MEW, 5));
    EXPECT_EQ(GetBoxMonDataAt(0, 0, MON_DATA_SPECIES), SPECIES_MEW);
    EXPECT(!ScDebugGiveMon(SPECIES_PIKACHU, 5));
    EXPECT_EQ(GetBoxMonDataAt(0, 1, MON_DATA_SPECIES), SPECIES_MAGIKARP);
    ResetPokemonStorageSystem();
    ZeroPlayerPartyMons();
}

TEST("SC cheats: all151 grants every original species without replacing an existing mon")
{
    gMain.inBattle = FALSE;
    ResetPokemonStorageSystem();
    ZeroPlayerPartyMons();
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 5, 0, OTID_STRUCT_PRESET(0), 15);
    struct Pokemon before = gParties[B_TRAINER_PLAYER][0];
    EXPECT(ScDebugGiveAll151(50));
    EXPECT_EQ(memcmp(&before, &gParties[B_TRAINER_PLAYER][0], sizeof(before)), 0);
    u32 seen[152] = {0}, count = 0;
    for (u32 i = 1; i < PARTY_SIZE; i++)
    {
        u32 species = GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES);
        EXPECT(species >= 1 && species <= 151);
        seen[species]++; count++;
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL), 50);
    }
    for (u32 box = 0; box < TOTAL_BOXES_COUNT; box++)
        for (u32 slot = 0; slot < IN_BOX_COUNT; slot++)
        {
            u32 species = GetBoxMonDataAt(box, slot, MON_DATA_SPECIES);
            if (!species) continue;
            EXPECT(species <= 151);
            seen[species]++; count++;
            EXPECT_EQ(GetLevelFromBoxMonExp(GetBoxedMonPtr(box, slot)), 50);
        }
    EXPECT_EQ(count, 151);
    for (u32 species = 1; species <= 151; species++) EXPECT_EQ(seen[species], 1);
    ResetPokemonStorageSystem();
    ZeroPlayerPartyMons();
}

TEST("SC cheats: level and species edits derive safe stats and invalid PP is rejected")
{
    gMain.inBattle = FALSE;
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    CreateMonWithIVs(mon, SPECIES_PIKACHU, 5, 0, OTID_STRUCT_PRESET(0), 15);
    EXPECT(ScDebugEditMon(0, SC_EDIT_LEVEL, 0, 100));
    EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 100);
    EXPECT(ScDebugEditMon(0, SC_EDIT_SPECIES, 0, SPECIES_MEW));
    EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 100);
    EXPECT_EQ(GetMonData(mon, MON_DATA_EXP), gExperienceTables[gSpeciesInfo[SPECIES_MEW].growthRate][100]);
    EXPECT(ScDebugEditMon(0, SC_EDIT_LEVEL, 0, 1));
    EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 1);
    EXPECT(GetMonData(mon, MON_DATA_HP) <= GetMonData(mon, MON_DATA_MAX_HP));
    EXPECT(ScDebugEditMon(0, SC_EDIT_MOVE, 3, MOVE_THUNDERBOLT));
    EXPECT(!ScDebugEditMon(0, SC_EDIT_PP, 3, 99));
    EXPECT(ScDebugEditMon(0, SC_EDIT_PP, 3, 0));
    EXPECT_EQ(GetMonData(mon, MON_DATA_PP4), 0);
}

TEST("SC cheats: persistent toggles preserve save schema and reject invalid bits")
{
    gMain.inBattle = FALSE;
    ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
    EXPECT(ScDebugSetOption(SC_DEBUG_INFINITE_ITEMS, TRUE));
    EXPECT(ScDebugOptionEnabled(SC_DEBUG_INFINITE_ITEMS));
    EXPECT(!ScDebugOptionEnabled(SC_DEBUG_IGNORE_LEVEL_CAP));
    EXPECT(!ScDebugSetOption(32, TRUE));
    EXPECT(!ScDebugOptionEnabled(32));
    EXPECT_EQ(ScValidateTrainerProgress(&gSaveBlock3Ptr->sc), TRUE);
    EXPECT(ScDebugSetOption(SC_DEBUG_INFINITE_ITEMS, FALSE));
    EXPECT(!ScDebugOptionEnabled(SC_DEBUG_INFINITE_ITEMS));
}
#else
TEST("SC cheats: ordinary build ignores test save options")
{
    ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
    gSaveBlock3Ptr->sc.reserved[0] = 0xFFFFFFFF;
    for (u32 option = 0; option < SC_DEBUG_OPTION_COUNT; option++)
        EXPECT(!ScDebugOptionEnabled(option));
    EXPECT(!ScDebugSetOption(SC_DEBUG_INFINITE_ITEMS, TRUE));
    EXPECT(!ScDebugGiveMon(SPECIES_MEW, 5));
}
#endif
