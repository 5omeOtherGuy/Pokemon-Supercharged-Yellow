#include "global.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_setup.h"
#include "battle_util.h"
#include "pokemon.h"
#include "sc_battle.h"
#include "sc_build_catalog.h"

#if TESTING
static bool32 sEffectsEnabledForTests;
void ScEffectsEnableForTests(bool32 enabled)
{
    sEffectsEnabledForTests = enabled;
}
#endif

bool32 ScEffectsEnabled(void)
{
#if TESTING
    return P_SC_KANTO_RULES || sEffectsEnabledForTests;
#else
    return P_SC_KANTO_RULES;
#endif
}

static u32 TrainerId(enum BattleTrainer trainer)
{
    return trainer == B_TRAINER_OPPONENT_B
        ? TRAINER_BATTLE_PARAM.opponentB : TRAINER_BATTLE_PARAM.opponentA;
}

static void ResolveAssignment(u32 battler, u32 partyIndex, u32 hpBar,
    u32 firstTurn, u32 status, struct ScPublicEffects *out)
{
    u32 capabilities = 0, trainerPassives = 0;
    if (ScEffectsEnabled() && battler < gBattlersCount && partyIndex < PARTY_SIZE)
    {
        enum BattleTrainer trainer = GetBattlerTrainer(battler);
        if (trainer == B_TRAINER_PLAYER)
        {
            // Assignments are public; earned collections and training are never read.
            capabilities = GetMonData(&gParties[trainer][partyIndex], MON_DATA_SC_ACTIVE_CAPABILITIES);
            trainerPassives = gSaveBlock3Ptr->sc.activePassives;
        }
        else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            u32 trainerId = TrainerId(trainer);
            capabilities = ScGetNpcCapabilities(trainerId, partyIndex);
            trainerPassives = ScGetNpcTrainerPassives(trainerId);
        }
    }
    ScResolvePublicEffects(capabilities, trainerPassives, hpBar, firstTurn, status, out);
}

void ScGetPublicBattlerEffects(u32 battler, struct ScPublicEffects *out)
{
    if (battler >= gBattlersCount || gBattleMons[battler].maxHP == 0)
    {
        ScResolvePublicEffects(0, 0, 0, 0, 0, out);
        return;
    }
    ResolveAssignment(battler, gBattlerPartyIndexes[battler],
        GetScaledHPFraction(gBattleMons[battler].hp, gBattleMons[battler].maxHP, 48),
        IsBattlersFirstTurn(battler), gBattleMons[battler].status1, out);
}

void ScGetPublicSwitchInEffects(u32 battler, u32 partyIndex, struct ScPublicEffects *out)
{
    if (battler >= gBattlersCount || partyIndex >= PARTY_SIZE)
    {
        ScResolvePublicEffects(0, 0, 0, 0, 0, out);
        return;
    }
    struct Pokemon *mon = &gParties[GetBattlerTrainer(battler)][partyIndex];
    if (GetMonData(mon, MON_DATA_MAX_HP) == 0 || GetMonData(mon, MON_DATA_IS_EGG))
    {
        ScResolvePublicEffects(0, 0, 0, 0, 0, out);
        return;
    }
    ResolveAssignment(battler, partyIndex,
        GetScaledHPFraction(GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP), 48),
        TRUE, GetMonData(mon, MON_DATA_STATUS), out);
}

// Applied after ordinary speed modifiers, before order and Trick Room compare it.
u32 ScApplySpeedEffects(u32 battler, u32 speed)
{
    struct ScPublicEffects effects;
    ScGetPublicBattlerEffects(battler, &effects);
    return speed * effects.speedPercent / 100;
}

static EWRAM_DATA struct
{
    bool8 active;
    u8 battler;
    u8 partyIndex;
} sDelayedDamage = {0};

void ScBeginDelayedDamage(u32 battler, u32 partyIndex)
{
    sDelayedDamage.active = TRUE;
    sDelayedDamage.battler = battler;
    sDelayedDamage.partyIndex = partyIndex;
}

void ScEndDelayedDamage(void)
{
    sDelayedDamage.active = FALSE;
}

s32 ScApplyDamageEffects(u32 battlerAtk, u32 battlerDef, u32 category, s32 damage)
{
    struct ScPublicEffects attack, defense;
    if (damage <= 0 || (category != DAMAGE_CATEGORY_PHYSICAL && category != DAMAGE_CATEGORY_SPECIAL))
        return damage;
    if (sDelayedDamage.active && sDelayedDamage.battler == battlerAtk)
    {
        // The engine has temporarily installed the original user's party stats.
        // A delayed hit is never an entry turn, even if the current slot just entered.
        ResolveAssignment(battlerAtk, sDelayedDamage.partyIndex,
            GetScaledHPFraction(gBattleMons[battlerAtk].hp, gBattleMons[battlerAtk].maxHP, 48),
            FALSE, gBattleMons[battlerAtk].status1, &attack);
    }
    else
        ScGetPublicBattlerEffects(battlerAtk, &attack);
    ScGetPublicBattlerEffects(battlerDef, &defense);
    damage = damage * (category == DAMAGE_CATEGORY_PHYSICAL
        ? attack.offensivePhysicalPercent : attack.offensiveSpecialPercent) / 100;
    damage = damage * (category == DAMAGE_CATEGORY_PHYSICAL
        ? defense.defensivePhysicalPercent : defense.defensiveSpecialPercent) / 100;
    return max(1, damage);
}

bool32 ScHasStatusImmunity(u32 battler, u32 status)
{
    struct ScPublicEffects effects;
    ScGetPublicBattlerEffects(battler, &effects);
    return (effects.statusImmunity & status) != 0;
}

u32 ScGetEndTurnHeal(u32 battler)
{
    struct ScPublicEffects effects;
    ScGetPublicBattlerEffects(battler, &effects);
    if (!effects.endTurnHealFraction || !effects.endTurnHealNumerator)
        return 0;
    return max(1, GetNonDynamaxMaxHP(battler) * effects.endTurnHealNumerator / effects.endTurnHealFraction);
}
