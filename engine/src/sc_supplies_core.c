#include "sc_supplies_core.h"

unsigned ScSupplyPlanValid(const struct ScSupplyPlan *plan, const unsigned categories[SC_SUPPLY_SLOTS])
{
    unsigned counts[SC_SUPPLY_CATEGORY_COUNT] = {0};
    unsigned total = 0;
    for (unsigned i = 0; i < SC_SUPPLY_SLOTS; i++)
    {
        unsigned quantity = plan->quantities[i], category = categories[i];
        if (!quantity) continue;
        if (!plan->items[i] || category == SC_SUPPLY_INVALID || category >= SC_SUPPLY_CATEGORY_COUNT)
            return 0;
        total += quantity;
        counts[category] += quantity;
        if (total > SC_SUPPLY_SLOTS || counts[category] > (category == SC_SUPPLY_HP ? 2u : 1u))
            return 0;
    }
    return 1;
}

void ScSupplyLoad(struct ScSupplyBattle *battle, unsigned side, const struct ScSupplyPlan *plan)
{
    if (side >= 2) return;
    unsigned unit = 0;
    for (unsigned i = 0; i < SC_SUPPLY_SLOTS; i++)
        for (unsigned count = 0; count < plan->quantities[i] && unit < SC_SUPPLY_SLOTS; count++)
            battle->units[side][unit++] = plan->items[i];
    while (unit < SC_SUPPLY_SLOTS) battle->units[side][unit++] = 0;
    for (unsigned actor = 0; actor < 4; actor++)
        if (battle->reservedSide[actor] == side) battle->reservedUnit[actor] = 0;
}

static unsigned FindUnit(const struct ScSupplyBattle *battle, unsigned side, unsigned actor, uint16_t item)
{
    if (side >= 2 || actor >= 4 || !item) return 0;
    for (unsigned unit = 0; unit < SC_SUPPLY_SLOTS; unit++)
    {
        if (battle->units[side][unit] != item) continue;
        unsigned other;
        for (other = 0; other < 4; other++)
            if (other != actor && battle->reservedSide[other] == side && battle->reservedUnit[other] == unit + 1)
                break;
        if (other == 4) return unit + 1;
    }
    return 0;
}

unsigned ScSupplyCanUse(const struct ScSupplyBattle *battle, unsigned side, unsigned actor, uint16_t item)
{
    return FindUnit(battle, side, actor, item) != 0;
}

unsigned ScSupplyReserve(struct ScSupplyBattle *battle, unsigned side, unsigned actor, uint16_t item)
{
    unsigned unit = FindUnit(battle, side, actor, item);
    if (!unit) return 0;
    battle->reservedUnit[actor] = unit;
    battle->reservedSide[actor] = side;
    return 1;
}

void ScSupplyCancel(struct ScSupplyBattle *battle, unsigned actor)
{
    if (actor < 4) battle->reservedUnit[actor] = 0;
}

unsigned ScSupplyCommit(struct ScSupplyBattle *battle, unsigned side, unsigned actor, uint16_t item)
{
    if (side >= 2 || actor >= 4 || battle->reservedSide[actor] != side) return 0;
    unsigned unit = battle->reservedUnit[actor];
    if (!unit || !item || battle->units[side][unit - 1] != item) return 0;
    battle->units[side][unit - 1] = 0;
    battle->reservedUnit[actor] = 0;
    return 1;
}
