#include "global.h"
#include "caps.h"
#include "event_data.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "sc_progression_core.h"
#include "test/test.h"

TEST("SC progression: boxed rewards preserve encryption, nature and evolution data")
{
    struct Pokemon mon, restored;
    struct BoxPokemon stored;
    u32 earned = 0x55, active = 0x05, focus = 3, carry = 7;
    u32 stat = 24, species = SPECIES_RAICHU;
    CreateMonWithIVs(&mon, SPECIES_PIKACHU, 15, 0, OTID_STRUCT_PRESET(0), 20);
    u32 nature = GetNature(&mon);
    SetMonData(&mon, MON_DATA_SC_EARNED_CAPABILITIES, &earned);
    SetMonData(&mon, MON_DATA_SC_ACTIVE_CAPABILITIES, &active);
    SetMonData(&mon, MON_DATA_SC_FOCUS, &focus);
    SetMonData(&mon, MON_DATA_SC_TRAINING_REMAINDER, &carry);
    SetMonData(&mon, MON_DATA_SPEED_EV, &stat);
    stored = mon.box;
    BoxMonToMon(&stored, &restored);
    SetMonData(&restored, MON_DATA_SPECIES, &species);
    CalculateMonStats(&restored);
    EXPECT_EQ(GetMonData(&restored, MON_DATA_SC_EARNED_CAPABILITIES), earned);
    EXPECT_EQ(GetMonData(&restored, MON_DATA_SC_ACTIVE_CAPABILITIES), active);
    EXPECT_EQ(GetMonData(&restored, MON_DATA_SC_FOCUS), focus);
    EXPECT_EQ(GetMonData(&restored, MON_DATA_SC_TRAINING_REMAINDER), carry);
    EXPECT_EQ(GetMonData(&restored, MON_DATA_SPEED_EV), stat);
    EXPECT_EQ(GetMonData(&restored, MON_DATA_SANITY_IS_BAD_EGG), FALSE);
    EXPECT_EQ(GetNature(&restored), nature);
    EXPECT_EQ(sizeof(struct BoxPokemon), 80);
    EXPECT_EQ(sizeof(struct PokemonStorage), 34144);
}

TEST("SC progression: legacy contest writes cannot create capabilities")
{
    struct Pokemon mon;
    u32 value = 255;
    CreateMonWithIVs(&mon, SPECIES_PIKACHU, 15, 0, OTID_STRUCT_PRESET(0), 20);
    SetMonData(&mon, MON_DATA_COOL, &value);
    SetMonData(&mon, MON_DATA_BEAUTY, &value);
    SetMonData(&mon, MON_DATA_CUTE, &value);
    SetMonData(&mon, MON_DATA_SMART, &value);
    SetMonData(&mon, MON_DATA_TOUGH, &value);
    SetMonData(&mon, MON_DATA_SHEEN, &value);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SC_EARNED_CAPABILITIES), 0);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SC_ACTIVE_CAPABILITIES), 0);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SC_FOCUS), SC_FOCUS_BALANCED);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SC_TRAINING_REMAINDER), 0);
}

TEST("SC progression: Kanto badge caps replace inherited Hoenn milestones")
{
    const u16 flags[] = {FLAG_BADGE01_GET, FLAG_BADGE02_GET, FLAG_BADGE03_GET,
        FLAG_BADGE04_GET, FLAG_BADGE05_GET, FLAG_BADGE06_GET, FLAG_BADGE07_GET, FLAG_BADGE08_GET};
    u32 i;
    FlagClear(FLAG_IS_CHAMPION);
    for (i = 0; i < 8; i++) FlagClear(flags[i]);
    EXPECT_EQ(GetCurrentLevelCap(), 15);
    for (i = 0; i < 8; i++)
    {
        FlagSet(flags[i]);
        EXPECT_EQ(GetCurrentLevelCap(), ScLevelCap(i + 1, FALSE));
    }
    FlagSet(FLAG_IS_CHAMPION);
    EXPECT_EQ(GetCurrentLevelCap(), 75);
}
