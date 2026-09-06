#include "global.h"
#include "battle.h"
#include "battle_util.h"
#include "test/test.h"

TEST("SC supplies: Antidote does not clear confusion or infatuation")
{
    gBattleMons[0].volatiles.confusionTimer = 3;
    gBattleMons[0].volatiles.infatuation = 2;
    EXPECT(!ItemHealMonVolatile(0, ITEM_ANTIDOTE));
    EXPECT_EQ((u32)gBattleMons[0].volatiles.confusionTimer, 3);
    EXPECT_EQ((u32)gBattleMons[0].volatiles.infatuation, 2);
}

TEST("SC supplies: Full Heal clears both confusion and infatuation")
{
    gBattleMons[0].volatiles.confusionTimer = 3;
    gBattleMons[0].volatiles.infatuation = 2;
    EXPECT(ItemHealMonVolatile(0, ITEM_FULL_HEAL));
    EXPECT_EQ((u32)gBattleMons[0].volatiles.confusionTimer, 0);
    EXPECT_EQ((u32)gBattleMons[0].volatiles.infatuation, 0);
}
