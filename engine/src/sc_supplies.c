#include "global.h"
#include "sc_supplies.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_setup.h"
#include "battle_util.h"
#include "data.h"
#include "item.h"
#include "main.h"
#include "constants/moves.h"
#include "pokemon.h"
#include "sc_build_catalog.h"
#include "constants/item_effects.h"
#include "constants/opponents_frlg.h"

static EWRAM_DATA struct ScSupplyBattle sSupplyBattle;
static EWRAM_DATA bool32 sSuppliesLocked;
static EWRAM_DATA u16 sConsumedOriginal[MAX_BATTLE_TRAINERS][PARTY_SIZE];

#if TESTING
static EWRAM_DATA bool32 sEnableForTests;
void ScSuppliesEnableForTests(bool32 enabled)
{
    sEnableForTests = enabled;
    ScSuppliesEndBattle();
}
#endif

static bool32 CampaignEnabled(void)
{
#if TESTING
    return P_SC_KANTO_RULES || sEnableForTests;
#else
    return P_SC_KANTO_RULES;
#endif
}

unsigned ScSuppliesCategory(enum Item item)
{
    const u8 *effect = GetItemEffect(item);
    unsigned categories = 0;
    enum EffectItem usage = GetItemBattleUsage(item);
    if (item == ITEM_NONE || !effect || !GetItemConsumability(item) || GetItemImportance(item))
        return SC_SUPPLY_INVALID;
    if ((effect[0] & ITEM0_SACRED_ASH) || (effect[3] & ITEM3_LEVEL_UP)
        || (effect[4] & (ITEM4_REVIVE | ITEM4_EV_HP | ITEM4_EV_ATK | ITEM4_PP_UP | ITEM4_EVO_STONE))
        || (effect[5] & (ITEM5_EV_DEF | ITEM5_EV_SPEED | ITEM5_EV_SPDEF | ITEM5_EV_SPATK | ITEM5_PP_MAX)))
        return SC_SUPPLY_INVALID;
    if (effect[4] & ITEM4_HEAL_HP) categories |= 1u << SC_SUPPLY_HP;
    if ((effect[3] & ITEM3_STATUS_ALL) || (effect[0] & ITEM0_INFATUATION)) categories |= 1u << SC_SUPPLY_STATUS;
    if (effect[4] & ITEM4_HEAL_PP) categories |= 1u << SC_SUPPLY_PP;
    if (usage == EFFECT_ITEM_INCREASE_STAT || usage == EFFECT_ITEM_SET_MIST
        || usage == EFFECT_ITEM_SET_FOCUS_ENERGY || usage == EFFECT_ITEM_INCREASE_ALL_STATS)
        categories |= 1u << SC_SUPPLY_BOOST;
    if (categories == (1u << SC_SUPPLY_HP) && usage == EFFECT_ITEM_RESTORE_HP) return SC_SUPPLY_HP;
    if (categories == (1u << SC_SUPPLY_STATUS) && usage == EFFECT_ITEM_CURE_STATUS) return SC_SUPPLY_STATUS;
    if (categories == (1u << SC_SUPPLY_PP) && usage == EFFECT_ITEM_RESTORE_PP) return SC_SUPPLY_PP;
    if (categories == (1u << SC_SUPPLY_BOOST)) return SC_SUPPLY_BOOST;
    return SC_SUPPLY_INVALID;
}

static bool32 ValidPlan(const struct ScSupplyPlan *plan)
{
    unsigned categories[SC_SUPPLY_SLOTS];
    for (u32 i = 0; i < SC_SUPPLY_SLOTS; i++) categories[i] = ScSuppliesCategory(plan->items[i]);
    return ScSupplyPlanValid(plan, categories);
}

bool32 ScSuppliesSetSelection(const u16 items[SC_SUPPLY_SLOTS], const u8 quantities[SC_SUPPLY_SLOTS])
{
    struct ScSupplyPlan plan;
    if (gMain.inBattle || sSuppliesLocked) return FALSE;
    memcpy(plan.items, items, sizeof(plan.items));
    memcpy(plan.quantities, quantities, sizeof(plan.quantities));
    if (!ValidPlan(&plan)) return FALSE;
    for (u32 i = 0; i < SC_SUPPLY_SLOTS; i++)
    {
        u32 needed = 0;
        for (u32 j = 0; j < SC_SUPPLY_SLOTS; j++)
            if (items[j] == items[i]) needed += quantities[j];
        if (needed && CountTotalItemQuantityInBag(items[i]) < needed) return FALSE;
    }
    for (u32 i = 0; i < SC_SUPPLY_SLOTS; i++)
    {
        gSaveBlock3Ptr->sc.supplyItems[i] = quantities[i] ? items[i] : ITEM_NONE;
        gSaveBlock3Ptr->sc.supplyQuantities[i] = quantities[i];
    }
    ScSealTrainerProgress(&gSaveBlock3Ptr->sc);
    return TRUE;
}

u32 ScSuppliesGetPlayerLoadout(u16 items[SC_SUPPLY_SLOTS], u8 quantities[SC_SUPPLY_SLOTS])
{
    struct ScSupplyPlan plan;
    u32 total = 0;
    memcpy(plan.items, gSaveBlock3Ptr->sc.supplyItems, sizeof(plan.items));
    memcpy(plan.quantities, gSaveBlock3Ptr->sc.supplyQuantities, sizeof(plan.quantities));
    memset(items, 0, sizeof(plan.items));
    memset(quantities, 0, sizeof(plan.quantities));
    if (!ValidPlan(&plan)) return 0;
    for (u32 i = 0; i < SC_SUPPLY_SLOTS; i++)
    {
        u32 available = CountTotalItemQuantityInBag(plan.items[i]);
        for (u32 j = 0; j < i; j++)
            if (items[j] == plan.items[i]) available -= quantities[j];
        quantities[i] = min(plan.quantities[i], available);
        items[i] = quantities[i] ? plan.items[i] : ITEM_NONE;
        total += quantities[i];
    }
    return total;
}

bool32 ScSuppliesIsMajorTrainer(u16 id)
{
    return ScGetNpcTrainerBudget(id) != 0 || id == TRAINER_BOSS_GIOVANNI || id == TRAINER_BOSS_GIOVANNI_2
        || (id >= TRAINER_RIVAL_ROUTE22_EARLY_SQUIRTLE && id <= TRAINER_RIVAL_CERULEAN_CHARMANDER)
        || (id >= TRAINER_RIVAL_SS_ANNE_SQUIRTLE && id <= TRAINER_RIVAL_ROUTE22_LATE_CHARMANDER)
        || (id >= TRAINER_SC_ROCKET_DUO_1 && id <= TRAINER_SC_ROCKET_DUO_4);
}

bool32 ScSuppliesApplies(void)
{
    return CampaignEnabled() && (gBattleTypeFlags & BATTLE_TYPE_TRAINER);
}

bool32 ScSuppliesBeginBattle(void)
{
    struct ScSupplyPlan player = {0}, enemy = {0};
    u32 requested = 0;
    memset(&sSupplyBattle, 0, sizeof(sSupplyBattle));
    memset(sConsumedOriginal, 0, sizeof(sConsumedOriginal));
    sSuppliesLocked = ScSuppliesApplies();
    if (!sSuppliesLocked) return TRUE;
    for (u32 i = 0; i < SC_SUPPLY_SLOTS; i++) requested += gSaveBlock3Ptr->sc.supplyQuantities[i];
    u32 supplied = ScSuppliesGetPlayerLoadout(player.items, player.quantities);
    ScSupplyLoad(&sSupplyBattle, 0, &player);
    u16 trainer = TRAINER_BATTLE_PARAM.opponentA;
    if (trainer < TRAINERS_COUNT && ScSuppliesIsMajorTrainer(trainer))
    {
        const enum Item *items = GetTrainerItemsFromId(trainer);
        u32 count = 0;
        for (u32 i = 0; i < MAX_TRAINER_ITEMS; i++)
        {
            if (items[i] == ITEM_NONE) continue;
            if (count == SC_SUPPLY_SLOTS) { count++; break; }
            enemy.items[count] = items[i]; enemy.quantities[count++] = 1;
        }
        if (count <= SC_SUPPLY_SLOTS && ValidPlan(&enemy)) ScSupplyLoad(&sSupplyBattle, 1, &enemy);
    }
    return requested == supplied;
}

void ScSuppliesEndBattle(void)
{
    sSuppliesLocked = FALSE;
    memset(&sSupplyBattle, 0, sizeof(sSupplyBattle));
    memset(sConsumedOriginal, 0, sizeof(sConsumedOriginal));
}

bool32 ScSuppliesCanUse(enum BattlerId battler, enum Item item)
{
    if (!ScSuppliesApplies()) return TRUE;
    if (!sSuppliesLocked || battler >= MAX_BATTLERS_COUNT || ScSuppliesCategory(item) == SC_SUPPLY_INVALID) return FALSE;
    u32 side = GetBattlerSide(battler);
    if (side == B_SIDE_PLAYER)
    {
        u32 reserved = 0;
        for (u32 actor = 0; actor < MAX_BATTLERS_COUNT; actor++)
        {
            u32 unit = sSupplyBattle.reservedUnit[actor];
            if (actor != battler && unit && sSupplyBattle.reservedSide[actor] == B_SIDE_PLAYER
                && sSupplyBattle.units[B_SIDE_PLAYER][unit - 1] == item) reserved++;
        }
        if (CountTotalItemQuantityInBag(item) <= reserved) return FALSE;
    }
    return ScSupplyCanUse(&sSupplyBattle, side, battler, item);
}

bool32 ScSuppliesReserve(enum BattlerId battler, enum Item item)
{
    if (!ScSuppliesApplies()) return TRUE;
    return ScSuppliesCanUse(battler, item) && ScSupplyReserve(&sSupplyBattle, GetBattlerSide(battler), battler, item);
}

void ScSuppliesCancel(enum BattlerId battler)
{
    ScSupplyCancel(&sSupplyBattle, battler);
}

// Recheck target state at execution: an earlier action may already have healed it.
static bool32 TargetHasEffect(enum BattlerId actor, enum Item item)
{
    u32 slot = gBattleStruct->itemPartyIndex[actor];
    if (slot >= PARTY_SIZE) return FALSE;
    struct Pokemon *mon = &GetBattlerParty(actor)[slot];
    u32 hp = GetMonData(mon, MON_DATA_HP);
    if (!hp) return FALSE;
    u32 target = MAX_BATTLERS_COUNT;
    for (u32 i = 0; i < gBattlersCount; i++)
        if (GetBattlerTrainer(i) == GetBattlerTrainer(actor) && gBattlerPartyIndexes[i] == slot) target = i;
    if (target < MAX_BATTLERS_COUNT && gBattleMons[target].volatiles.embargoTimer) return FALSE;
    const u8 *effect = GetItemEffect(item);
    switch (ScSuppliesCategory(item))
    {
    case SC_SUPPLY_HP: return hp < GetMonData(mon, MON_DATA_MAX_HP);
    case SC_SUPPLY_STATUS:
        return (GetMonData(mon, MON_DATA_STATUS) & GetItemStatus1Mask(item))
            || (target < MAX_BATTLERS_COUNT && (((effect[3] & ITEM3_CONFUSION) && gBattleMons[target].volatiles.confusionTimer)
                || (((effect[0] & ITEM0_INFATUATION) || (effect[3] & ITEM3_STATUS_ALL) == ITEM3_STATUS_ALL) && gBattleMons[target].volatiles.infatuation)));
    case SC_SUPPLY_PP:
        for (u32 i = 0; i < MAX_MON_MOVES; i++)
        {
            if ((effect[4] & ITEM4_HEAL_PP_ONE) && i != gBattleStruct->itemMoveIndex[actor]) continue;
            u32 move = GetMonData(mon, MON_DATA_MOVE1 + i);
            if (move != MOVE_NONE && GetMonData(mon, MON_DATA_PP1 + i) < CalculatePPWithBonus(move, GetMonData(mon, MON_DATA_PP_BONUSES), i)) return TRUE;
        }
        return FALSE;
    case SC_SUPPLY_BOOST:
        if (target >= MAX_BATTLERS_COUNT) return FALSE;
        switch (GetItemBattleUsage(item))
        {
        case EFFECT_ITEM_INCREASE_STAT: return effect[1] < NUM_BATTLE_STATS && gBattleMons[target].statStages[effect[1]] < MAX_STAT_STAGE;
        case EFFECT_ITEM_SET_MIST: return !(gSideStatuses[GetBattlerSide(actor)] & SIDE_STATUS_MIST);
        case EFFECT_ITEM_SET_FOCUS_ENERGY: return !gBattleMons[target].volatiles.focusEnergy && !gBattleMons[target].volatiles.dragonCheer;
        case EFFECT_ITEM_INCREASE_ALL_STATS:
            for (u32 stat = STAT_ATK; stat <= STAT_SPDEF; stat++)
                if (gBattleMons[target].statStages[stat] < MAX_STAT_STAGE) return TRUE;
            return FALSE;
        default: return FALSE;
        }
    default: return FALSE;
    }
}

bool32 ScSuppliesCommit(enum BattlerId battler, enum Item item)
{
    if (!ScSuppliesApplies()) return TRUE;
    if (!ScSuppliesCanUse(battler, item) || !TargetHasEffect(battler, item)) { ScSuppliesCancel(battler); return FALSE; }
    struct ScSupplyBattle next = sSupplyBattle;
    if (!ScSupplyCommit(&next, GetBattlerSide(battler), battler, item)) return FALSE;
    if (GetBattlerSide(battler) == B_SIDE_PLAYER && !RemoveBagItem(item, 1)) { ScSuppliesCancel(battler); return FALSE; }
    sSupplyBattle = next;
    return TRUE;
}

u32 ScSuppliesHealAmount(enum BattlerId battler, u32 amount)
{
    if (!CampaignEnabled() || battler >= MAX_BATTLERS_COUNT) return amount;
    u32 passives = 0;
    u32 trainer = gMain.inBattle ? GetBattlerTrainer(battler) : B_TRAINER_PLAYER;
    if (trainer == B_TRAINER_PLAYER) passives = gSaveBlock3Ptr->sc.activePassives;
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        passives = ScGetNpcTrainerPassives(trainer == B_TRAINER_OPPONENT_B ? TRAINER_BATTLE_PARAM.opponentB : TRAINER_BATTLE_PARAM.opponentA);
    return passives & (1u << SC_PASSIVE_PREPARATION) ? amount * 110 / 100 : amount;
}

void ScSuppliesRecordHeldConsumption(u32 trainer, u32 slot, enum Item item)
{
    if (CampaignEnabled() && trainer < MAX_BATTLE_TRAINERS && slot < PARTY_SIZE && item != ITEM_NONE
        && gBattleStruct->itemLost[trainer][slot].originalItem == item)
        sConsumedOriginal[trainer][slot] = item;
}

bool32 ScSuppliesWasHeldConsumed(u32 trainer, u32 slot, enum Item item)
{
    return CampaignEnabled() && trainer < MAX_BATTLE_TRAINERS && slot < PARTY_SIZE && item != ITEM_NONE
        && sConsumedOriginal[trainer][slot] == item;
}
