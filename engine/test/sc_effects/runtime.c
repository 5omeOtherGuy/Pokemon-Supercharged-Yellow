#include "global.h"
#include "battle.h"
#include "battle_util.h"
#include "sc_battle.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("SC runtime effects: speed combines only active public effects")
{
    GIVEN {
        PLAYER(SPECIES_PIKACHU) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_MAGIKARP) { Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } THEN {
        u32 active = 1u << SC_CAP_QUICK_START;
        ScEffectsEnableForTests(TRUE);
        SetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_SC_ACTIVE_CAPABILITIES, &active);
        gSaveBlock3Ptr->sc.activePassives = 1u << SC_PASSIVE_TEMPO;
        gBattleStruct->battlerState[B_BATTLER_0].isFirstTurn = 1;
        EXPECT_EQ(ScApplySpeedEffects(B_BATTLER_0, 100), 114);
        gBattleStruct->battlerState[B_BATTLER_0].isFirstTurn = 0;
        EXPECT_EQ(ScApplySpeedEffects(B_BATTLER_0, 100), 104);
        EXPECT_EQ(ScApplySpeedEffects(B_BATTLER_0, 0), 0);
        ScEffectsEnableForTests(FALSE);
    }
}

// These are explicit test fixtures, installed in the generated test party.
static void AssignPlayer(u32 slot, u32 mask)
{
    SetMonData(&PLAYER_PARTY[slot], MON_DATA_SC_ACTIVE_CAPABILITIES, &mask);
}

SINGLE_BATTLE_TEST("SC runtime effects: physical offense modifies ordinary damage", s16 damage)
{
    u32 mask;
    PARAMETRIZE { mask = 0; }
    PARAMETRIZE { mask = 1u << SC_CAP_PRESSURE; }
    GIVEN {
        ScEffectsEnableForTests(TRUE);
        PLAYER(SPECIES_PIKACHU) { Attack(200); }
        OPPONENT(SPECIES_CHANSEY) { Defense(100); MaxHP(1000); HP(1000); }
        AssignPlayer(0, mask);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[1].damage, results[0].damage * 112 / 100);
    }
}

SINGLE_BATTLE_TEST("SC runtime effects: special offense modifies ordinary damage", s16 damage)
{
    u32 mask;
    PARAMETRIZE { mask = 0; }
    PARAMETRIZE { mask = 1u << SC_CAP_FOCUS; }
    GIVEN {
        ScEffectsEnableForTests(TRUE);
        PLAYER(SPECIES_PIKACHU) { SpAttack(200); }
        OPPONENT(SPECIES_CHANSEY) { SpDefense(100); MaxHP(1000); HP(1000); }
        AssignPlayer(0, mask);
    } WHEN {
        TURN { MOVE(player, MOVE_PSYCHIC, WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[1].damage, results[0].damage * 112 / 100);
    }
}

SINGLE_BATTLE_TEST("SC runtime effects: recovery stacks exactly with trainer recovery")
{
    u32 mask, passives, healed;
    PARAMETRIZE { mask = 0; passives = 0; healed = 0; }
    PARAMETRIZE { mask = 1u << SC_CAP_RECOVERY; passives = 0; healed = 2; }
    PARAMETRIZE { mask = 0; passives = 1u << SC_PASSIVE_RECOVERY_COACH; healed = 1; }
    PARAMETRIZE { mask = 1u << SC_CAP_RECOVERY; passives = 1u << SC_PASSIVE_RECOVERY_COACH; healed = 3; }
    GIVEN {
        ScEffectsEnableForTests(TRUE);
        gSaveBlock3Ptr->sc.activePassives = passives;
        PLAYER(SPECIES_PIKACHU) { MaxHP(64); HP(30); }
        OPPONENT(SPECIES_MAGIKARP);
        AssignPlayer(0, mask);
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } THEN {
        EXPECT_EQ(player->hp, 30 + healed);
    }
}

SINGLE_BATTLE_TEST("SC runtime effects: Clear Head blocks secondary freeze without blocking damage")
{
    GIVEN {
        ScEffectsEnableForTests(TRUE);
        PLAYER(SPECIES_PIKACHU);
        OPPONENT(SPECIES_MAGIKARP);
        AssignPlayer(0, 1u << SC_CAP_CLEAR_HEAD);
    } WHEN {
        TURN { MOVE(opponent, MOVE_POWDER_SNOW, WITH_RNG(RNG_SECONDARY_EFFECT, TRUE)); MOVE(player, MOVE_SPLASH); }
    } SCENE {
        HP_BAR(player);
        NOT ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_FRZ, player);
    } THEN {
        EXPECT_EQ(player->status1 & STATUS1_FREEZE, 0);
    }
}

SINGLE_BATTLE_TEST("SC runtime effects: fixed damage keeps its specified amount")
{
    GIVEN {
        ScEffectsEnableForTests(TRUE);
        gSaveBlock3Ptr->sc.activePassives = 1u << SC_PASSIVE_OFFENSE;
        PLAYER(SPECIES_PIKACHU);
        OPPONENT(SPECIES_CHANSEY);
        AssignPlayer(0, (1u << SC_CAP_FOCUS) | (1u << SC_CAP_PRESSURE));
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_RAGE); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        HP_BAR(opponent, damage: 40);
    }
}

SINGLE_BATTLE_TEST("SC runtime effects: recovery respects Heal Block and full HP")
{
    u32 hp, expected;
    enum Move move;
    PARAMETRIZE { hp = 32; move = MOVE_HEAL_BLOCK; expected = 32; }
    PARAMETRIZE { hp = 64; move = MOVE_SPLASH; expected = 64; }
    PARAMETRIZE { hp = 63; move = MOVE_SPLASH; expected = 64; }
    GIVEN {
        ScEffectsEnableForTests(TRUE);
        gSaveBlock3Ptr->sc.activePassives = 1u << SC_PASSIVE_RECOVERY_COACH;
        PLAYER(SPECIES_PIKACHU) { MaxHP(64); HP(hp); }
        OPPONENT(SPECIES_MAGIKARP);
        AssignPlayer(0, 1u << SC_CAP_RECOVERY);
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, move); }
    } THEN {
        EXPECT_EQ(player->hp, expected);
    }
}

SINGLE_BATTLE_TEST("SC runtime effects: delayed hit keeps original capability after a switch", s16 damage)
{
    u32 original, replacement;
    PARAMETRIZE { original = 0; replacement = 0; }
    PARAMETRIZE { original = 1u << SC_CAP_FOCUS; replacement = 1u << SC_CAP_PRESSURE; }
    GIVEN {
        ScEffectsEnableForTests(TRUE);
        gSaveBlock3Ptr->sc.activePassives = 0;
        PLAYER(SPECIES_PIKACHU) { SpAttack(150); }
        PLAYER(SPECIES_RAICHU);
        OPPONENT(SPECIES_CHANSEY) { MaxHP(1000); HP(1000); SpDefense(100); }
        AssignPlayer(0, original);
        AssignPlayer(1, replacement);
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); MOVE(opponent, MOVE_SPLASH); }
        TURN { SWITCH(player, 1); MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        MESSAGE("The opposing Chansey took the Future Sight attack!");
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } THEN {
        struct ScPublicEffects effects;
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], 1);
        EXPECT_EQ(GetMonData(GetBattlerMon(B_BATTLER_0), MON_DATA_SC_ACTIVE_CAPABILITIES), replacement);
        ScGetPublicBattlerEffects(B_BATTLER_0, &effects);
        EXPECT_EQ(effects.offensiveSpecialPercent, i ? 90 : 100);
    } FINALLY {
        EXPECT_EQ(results[1].damage, results[0].damage * 112 / 100);
    }
}

SINGLE_BATTLE_TEST("SC runtime effects: defensive category and public HP bands govern mitigation", s16 damage)
{
    enum Move move;
    u32 mask, percent;
    PARAMETRIZE { move = MOVE_TACKLE; mask = 0; percent = 100; }
    PARAMETRIZE { move = MOVE_TACKLE; mask = 1u << SC_CAP_BRACE; percent = 95; }
    PARAMETRIZE { move = MOVE_TACKLE; mask = 1u << SC_CAP_FLOW; percent = 100; }
    PARAMETRIZE { move = MOVE_PSYCHIC; mask = 0; percent = 100; }
    PARAMETRIZE { move = MOVE_PSYCHIC; mask = 1u << SC_CAP_BRACE; percent = 100; }
    PARAMETRIZE { move = MOVE_PSYCHIC; mask = 1u << SC_CAP_FLOW; percent = 95; }
    GIVEN {
        ScEffectsEnableForTests(TRUE);
        gSaveBlock3Ptr->sc.activePassives = 0;
        PLAYER(SPECIES_CHANSEY) { MaxHP(1000); HP(1000); Defense(100); SpDefense(100); }
        OPPONENT(SPECIES_PIKACHU) { Attack(200); SpAttack(200); }
        AssignPlayer(0, mask);
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, move, WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } THEN {
        EXPECT_EQ(results[i].damage, results[i / 3 * 3].damage * percent / 100);
    }
}

DOUBLE_BATTLE_TEST("SC runtime effects: spread hit uses each target's own defensive assignment")
{
    s16 leftDamage, rightDamage;
    GIVEN {
        ScEffectsEnableForTests(TRUE);
        gSaveBlock3Ptr->sc.activePassives = 0;
        PLAYER(SPECIES_PIKACHU) { MaxHP(1000); HP(1000); Defense(100); }
        PLAYER(SPECIES_PIKACHU) { MaxHP(1000); HP(1000); Defense(100); }
        OPPONENT(SPECIES_SNORLAX) { Attack(200); }
        OPPONENT(SPECIES_MAGIKARP) { MaxHP(1000); HP(1000); }
        AssignPlayer(0, 1u << SC_CAP_BRACE);
        AssignPlayer(1, 1u << SC_CAP_FLOW);
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SPLASH); MOVE(playerRight, MOVE_SPLASH);
            MOVE(opponentLeft, MOVE_EARTHQUAKE); MOVE(opponentRight, MOVE_SPLASH);
        }
    } SCENE {
        HP_BAR(playerLeft, captureDamage: &leftDamage);
        HP_BAR(playerRight, captureDamage: &rightDamage);
    } THEN {
        EXPECT_EQ(leftDamage, rightDamage * 95 / 100);
    }
}

WILD_BATTLE_TEST("SC runtime effects: wild battles retain player recovery and neutral wild effects")
{
    GIVEN {
        ScEffectsEnableForTests(TRUE);
        gSaveBlock3Ptr->sc.activePassives = 1u << SC_PASSIVE_RECOVERY_COACH;
        PLAYER(SPECIES_PIKACHU) { HP(30); MaxHP(64); }
        OPPONENT(SPECIES_MAGIKARP) { HP(30); MaxHP(64); }
        AssignPlayer(0, 1u << SC_CAP_RECOVERY);
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
        MESSAGE("Pikachu recovered a little HP!");
        HP_BAR(player, damage: -3);
    } THEN {
        EXPECT_EQ(player->hp, 33);
        EXPECT_EQ(opponent->hp, 30);
    }
}

SINGLE_BATTLE_TEST("SC runtime effects: Quick Start changes actual order only on the entry turn")
{
    GIVEN {
        ScEffectsEnableForTests(TRUE);
        gSaveBlock3Ptr->sc.activePassives = 0;
        PLAYER(SPECIES_PIKACHU) { Speed(100); }
        OPPONENT(SPECIES_MAGIKARP) { Speed(105); }
        AssignPlayer(0, 1u << SC_CAP_QUICK_START);
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        MESSAGE("Pikachu used Splash!");
        MESSAGE("The opposing Magikarp used Splash!");
        MESSAGE("The opposing Magikarp used Splash!");
        MESSAGE("Pikachu used Splash!");
    }
}
