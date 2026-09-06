#include "global.h"
#include "battle.h"
#include "battle_util.h"
#include "battle_scripts.h"
#include "item.h"
#include "event_data.h"
#include "sc_supplies.h"
#include "sc_debug.h"
#include "test/battle.h"

static void PreparePlayerPotion(bool32 fullHp)
{
    ScSuppliesEnableForTests(TRUE);
    ScSuppliesEndBattle();
    ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
    RemoveBagItem(ITEM_POTION, CountTotalItemQuantityInBag(ITEM_POTION));
    AddBagItem(ITEM_POTION, 1);
    const u16 items[3] = {ITEM_POTION, 0, 0};
    const u8 quantities[3] = {1, 0, 0};
    // Prepare through the field service boundary, then lock at battle entry.
    gMain.inBattle = FALSE;
    EXPECT(ScSuppliesSetSelection(items, quantities));
    gMain.inBattle = TRUE;
    EXPECT(ScSuppliesBeginBattle());
    u32 hp = fullHp ? GetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_MAX_HP) : 1;
    SetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_HP, &hp);
    gBattleMons[B_BATTLER_0].hp = hp;
    gBattleStruct->itemPartyIndex[B_BATTLER_0] = 0;
    gBattlerByTurnOrder[0] = B_BATTLER_0;
    gCurrentTurnActionNumber = 0;
    gBattleResources->bufferB[B_BATTLER_0][1] = ITEM_POTION & 255;
    gBattleResources->bufferB[B_BATTLER_0][2] = ITEM_POTION >> 8;
    EXPECT(ScSuppliesReserve(B_BATTLER_0, ITEM_POTION));
}

SINGLE_BATTLE_TEST("SC item action: dispatcher consumes a reserved player supply exactly once")
{
    GIVEN { PLAYER(SPECIES_PIKACHU); OPPONENT(SPECIES_MAGIKARP); }
    WHEN { TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); } }
    THEN {
        PreparePlayerPotion(FALSE);
        HandleAction_UseItem();
        EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_POTION), 0);
        EXPECT(!ScSuppliesCanUse(B_BATTLER_0, ITEM_POTION));
        const u8 *acceptedScript = gBattlescriptCurrInstr;
        HandleAction_UseItem();
        EXPECT_NE(gBattlescriptCurrInstr, acceptedScript);
        EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_POTION), 0);
        ScSuppliesEnableForTests(FALSE);
    }
}

SINGLE_BATTLE_TEST("SC item action: stale full HP cancels reservation without spending stock or quota")
{
    GIVEN { PLAYER(SPECIES_PIKACHU); OPPONENT(SPECIES_MAGIKARP); }
    WHEN { TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); } }
    THEN {
        PreparePlayerPotion(TRUE);
        HandleAction_UseItem();
        const u8 *rejectedScript = gBattlescriptCurrInstr;
        EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_POTION), 1);
        EXPECT(ScSuppliesCanUse(B_BATTLER_0, ITEM_POTION));
        u32 hp = 1;
        SetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_HP, &hp);
        gBattleMons[B_BATTLER_0].hp = hp;
        EXPECT(ScSuppliesReserve(B_BATTLER_0, ITEM_POTION));
        HandleAction_UseItem();
        EXPECT_NE(gBattlescriptCurrInstr, rejectedScript);
        EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_POTION), 0);
        ScSuppliesEnableForTests(FALSE);
    }
}

#if SC_TEST_TOOLS
SINGLE_BATTLE_TEST("SC owner action: simulated Potion turns spend once with prepared and free quotas")
{
    bool32 freeQuota;
    PARAMETRIZE { freeQuota = FALSE; }
    PARAMETRIZE { freeQuota = TRUE; }
    GIVEN {
        gMain.inBattle = FALSE;
        ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
        ScSuppliesEndBattle();
        EXPECT(ScDebugSetOption(SC_DEBUG_FREE_SUPPLIES, freeQuota));
        GIVE_PLAYER_ITEM(ITEM_POTION, 3);
        const u16 items[3] = {ITEM_POTION, 0, 0};
        const u8 quantities[3] = {1, 0, 0};
        EXPECT(ScSuppliesSetSelection(items, quantities));
        PLAYER(SPECIES_PIKACHU) { HP(1); MaxHP(100); Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_MAGIKARP) { Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { USE_ITEM(player, ITEM_POTION, partyIndex: 0); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        HP_BAR(player);
        MESSAGE("Pikachu had its HP restored.");
    } THEN {
        EXPECT_EQ(player->hp, 21);
        EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_POTION), 2);
        EXPECT_EQ(ScSuppliesCanUse(B_BATTLER_0, ITEM_POTION), freeQuota);
        ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
    }
}

TEST("SC owner hook: actual Rare Candy cap and encrypted bag honor reversible switches")
{
    gMain.inBattle = FALSE;
    ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
    for (u32 i = 0; i < 8; i++) FlagClear(FLAG_BADGE01_GET + i);
    FlagClear(FLAG_IS_CHAMPION);
    struct Pokemon mon;
    CreateMonWithIVs(&mon, SPECIES_PIKACHU, 15, 0, OTID_STRUCT_PRESET(0), 15);
    EXPECT(PokemonUseItemEffects(&mon, ITEM_RARE_CANDY, 0, 0, FALSE));
    EXPECT(ScDebugSetOption(SC_DEBUG_IGNORE_LEVEL_CAP, TRUE));
    EXPECT(!PokemonUseItemEffects(&mon, ITEM_RARE_CANDY, 0, 0, FALSE));
    EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), 16);
    RemoveBagItem(ITEM_RARE_CANDY, CountTotalItemQuantityInBag(ITEM_RARE_CANDY));
    EXPECT(AddBagItem(ITEM_RARE_CANDY, 1));
    EXPECT(ScDebugSetOption(SC_DEBUG_INFINITE_ITEMS, TRUE));
    EXPECT(RemoveBagItem(ITEM_RARE_CANDY, 1));
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_RARE_CANDY), 1);
    EXPECT(!RemoveBagItem(ITEM_RARE_CANDY, 2));
    EXPECT(ScDebugSetOption(SC_DEBUG_INFINITE_ITEMS, FALSE));
    EXPECT(RemoveBagItem(ITEM_RARE_CANDY, 1));
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_RARE_CANDY), 0);
    ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
}
#endif
