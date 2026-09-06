#include "global.h"
#include "battle.h"
#include "data.h"
#include "item.h"
#include "main.h"
#include "sc_supplies.h"
#include "test/test.h"

TEST("SC supplies: actual item effects reject revives and mixed categories")
{
    EXPECT_EQ(ScSuppliesCategory(ITEM_POTION), SC_SUPPLY_HP);
    EXPECT_EQ(ScSuppliesCategory(ITEM_MAX_POTION), SC_SUPPLY_HP);
    EXPECT_EQ(ScSuppliesCategory(ITEM_FULL_HEAL), SC_SUPPLY_STATUS);
    EXPECT_EQ(ScSuppliesCategory(ITEM_ETHER), SC_SUPPLY_PP);
    EXPECT_EQ(ScSuppliesCategory(ITEM_X_ATTACK), SC_SUPPLY_BOOST);
    EXPECT_EQ(ScSuppliesCategory(ITEM_FULL_RESTORE), SC_SUPPLY_INVALID);
    EXPECT_EQ(ScSuppliesCategory(ITEM_REVIVE), SC_SUPPLY_INVALID);
    EXPECT_EQ(ScSuppliesCategory(ITEM_MAX_REVIVE), SC_SUPPLY_INVALID);
    EXPECT_EQ(ScSuppliesCategory(ITEM_POKE_BALL), SC_SUPPLY_INVALID);
    EXPECT_EQ(ScSuppliesCategory(ITEM_POKE_DOLL), SC_SUPPLY_INVALID);
    EXPECT_EQ(ScSuppliesCategory(ITEM_RARE_CANDY), SC_SUPPLY_INVALID);
    EXPECT_EQ(ScSuppliesCategory(ITEM_POKE_FLUTE), SC_SUPPLY_INVALID);
}

TEST("SC supplies: unavailable reselection is atomic and preparation consumes nothing")
{
    u16 items[3] = {ITEM_POTION, ITEM_POTION, ITEM_FULL_HEAL};
    u8 quantities[3] = {1, 1, 1};
    u16 shown[3];
    u8 counts[3];
    ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
    gMain.inBattle = FALSE;
    ScSuppliesEndBattle();
    RemoveBagItem(ITEM_POTION, CountTotalItemQuantityInBag(ITEM_POTION));
    RemoveBagItem(ITEM_FULL_HEAL, CountTotalItemQuantityInBag(ITEM_FULL_HEAL));
    EXPECT(!ScSuppliesSetSelection(items, quantities));
    AddBagItem(ITEM_POTION, 2); AddBagItem(ITEM_FULL_HEAL, 1);
    EXPECT(ScSuppliesSetSelection(items, quantities));
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_POTION), 2);
    quantities[1] = 2;
    EXPECT(!ScSuppliesSetSelection(items, quantities));
    EXPECT_EQ(ScSuppliesGetPlayerLoadout(shown, counts), 3);
    RemoveBagItem(ITEM_POTION, 1);
    EXPECT_EQ(ScSuppliesGetPlayerLoadout(shown, counts), 2);
    EXPECT_EQ(ScValidateTrainerProgress(&gSaveBlock3Ptr->sc), 1);
}

#if SC_TEST_CAMPAIGN
TEST("SC supplies: every authored opponent bag is major-only and within unit/category caps")
#else
TEST("SC supplies: fixture opponent bags obey unit/category caps")
#endif
{
#if SC_TEST_CAMPAIGN
    u32 stockedMajorBags = 0;
#endif
    for (u32 trainer = 0; trainer < TRAINERS_COUNT; trainer++)
    {
        const enum Item *items = GetTrainerItemsFromId(trainer);
        struct ScSupplyPlan plan = {0};
        unsigned categories[SC_SUPPLY_SLOTS] = {0};
        u32 count = 0;
        for (u32 i = 0; i < MAX_TRAINER_ITEMS; i++)
        {
            if (items[i] == ITEM_NONE) continue;
            EXPECT(ScSuppliesIsMajorTrainer(trainer));
            EXPECT(count < SC_SUPPLY_SLOTS);
            if (count >= SC_SUPPLY_SLOTS) break;
            plan.items[count] = items[i];
            plan.quantities[count] = 1;
            categories[count++] = ScSuppliesCategory(items[i]);
        }
        EXPECT(ScSupplyPlanValid(&plan, categories));
#if SC_TEST_CAMPAIGN
        stockedMajorBags += count != 0;
#endif
    }
#if SC_TEST_CAMPAIGN
    EXPECT(stockedMajorBags > 0); // A sparse fixture table must not pass vacuously.
    const u16 champions[] = {TRAINER_CHAMPION_FIRST_SQUIRTLE,
        TRAINER_CHAMPION_FIRST_BULBASAUR, TRAINER_CHAMPION_FIRST_CHARMANDER,
        TRAINER_CHAMPION_REMATCH_SQUIRTLE, TRAINER_CHAMPION_REMATCH_BULBASAUR,
        TRAINER_CHAMPION_REMATCH_CHARMANDER};
    for (u32 c = 0; c < ARRAY_COUNT(champions); c++)
    {
        u32 hp = 0, status = 0, total = 0;
        const enum Item *items = GetTrainerItemsFromId(champions[c]);
        EXPECT_EQ(GetTrainerPartySizeFromId(champions[c]), PARTY_SIZE);
        for (u32 slot = 0; slot < MAX_TRAINER_ITEMS; slot++)
        {
            hp += items[slot] == ITEM_MAX_POTION;
            status += items[slot] == ITEM_FULL_HEAL;
            total += items[slot] != ITEM_NONE;
        }
        EXPECT_EQ(hp, 2);
        EXPECT_EQ(status, 1);
        EXPECT_EQ(total, 3);
    }
#endif
}
