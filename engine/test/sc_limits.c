#include "global.h"
#include "caps.h"
#include "event_data.h"
#include "pokemon.h"
#include "sc_progression.h"
#include "test/battle.h"

static void NoBadges(void)
{
    const u16 flags[] = {FLAG_BADGE01_GET, FLAG_BADGE02_GET, FLAG_BADGE03_GET,
        FLAG_BADGE04_GET, FLAG_BADGE05_GET, FLAG_BADGE06_GET, FLAG_BADGE07_GET, FLAG_BADGE08_GET};
    for (u32 i = 0; i < ARRAY_COUNT(flags); i++)
        FlagClear(flags[i]);
    FlagClear(FLAG_IS_CHAMPION);
}

TEST("SC limits: direct candy use cannot cross the badge cap or lower an over-cap recruit")
{
    struct Pokemon mon;
    ScProgressionEnableForTests(TRUE);
    NoBadges();
    CreateRandomMonWithIVs(&mon, SPECIES_PIKACHU, 15, 20);
    u32 exp = GetMonData(&mon, MON_DATA_EXP);
    EXPECT(PokemonUseItemEffects(&mon, ITEM_RARE_CANDY, 0, 0, FALSE));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_EXP), exp);
    EXPECT(PokemonUseItemEffects(&mon, ITEM_EXP_CANDY_XL, 0, 0, FALSE));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_EXP), exp);
    CreateRandomMonWithIVs(&mon, SPECIES_PIKACHU, 20, 20);
    exp = GetMonData(&mon, MON_DATA_EXP);
    EXPECT(PokemonUseItemEffects(&mon, ITEM_EXP_CANDY_XL, 0, 0, FALSE));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_EXP), exp);
    FlagSet(FLAG_BADGE01_GET);
    EXPECT(!PokemonUseItemEffects(&mon, ITEM_RARE_CANDY, 0, 0, FALSE));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), 21);
}

TEST("SC limits: ordinary random IVs retain individuality within the usability floor")
{
    struct Pokemon mon;
    u32 seen = 0;
    ScProgressionEnableForTests(TRUE);
    for (u32 n = 0; n < 32; n++)
    {
        CreateRandomMon(&mon, SPECIES_PIKACHU, 5);
        for (u32 stat = 0; stat < NUM_STATS; stat++)
        {
            u32 iv = GetMonData(&mon, MON_DATA_HP_IV + stat);
            EXPECT_GE(iv, 15);
            EXPECT_LE(iv, 31);
            seen |= 1u << iv;
        }
    }
    EXPECT((seen & (seen - 1)) != 0);
    CreateRandomMonWithIVs(&mon, SPECIES_PIKACHU, 5, 7);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HP_IV), 7); // Explicit fixtures/authored opponents stay explicit.
}

WILD_BATTLE_TEST("SC limits: a large battle EXP award stops exactly at the current cap")
{
    GIVEN {
        ScProgressionEnableForTests(TRUE);
        NoBadges();
        PLAYER(SPECIES_PIKACHU) { Level(14); Moves(MOVE_DRAGON_RAGE); }
        OPPONENT(SPECIES_CHANSEY) { Level(100); HP(1); Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_RAGE); }
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), 15);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_EXP),
            gExperienceTables[gSpeciesInfo[SPECIES_PIKACHU].growthRate][15]);
        EXPECT_GT(GetMonEVCount(&gParties[B_TRAINER_PLAYER][0]), 0);
    }
}
