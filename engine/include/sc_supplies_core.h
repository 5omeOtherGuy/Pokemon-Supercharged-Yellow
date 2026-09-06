#ifndef GUARD_SC_SUPPLIES_CORE_H
#define GUARD_SC_SUPPLIES_CORE_H

#include "sc_progression_core.h"

enum ScSupplyCategory
{
    SC_SUPPLY_INVALID, SC_SUPPLY_HP, SC_SUPPLY_STATUS, SC_SUPPLY_PP, SC_SUPPLY_BOOST,
    SC_SUPPLY_CATEGORY_COUNT
};
struct ScSupplyPlan
{
    uint16_t items[SC_SUPPLY_SLOTS];
    uint8_t quantities[SC_SUPPLY_SLOTS];
};
struct ScSupplyBattle
{
    uint16_t units[2][SC_SUPPLY_SLOTS];
    uint8_t reservedUnit[4]; // unit index + 1; zero means no reservation
    uint8_t reservedSide[4];
};
unsigned ScSupplyPlanValid(const struct ScSupplyPlan *plan, const unsigned categories[SC_SUPPLY_SLOTS]);
void ScSupplyLoad(struct ScSupplyBattle *battle, unsigned side, const struct ScSupplyPlan *plan);
unsigned ScSupplyCanUse(const struct ScSupplyBattle *battle, unsigned side, unsigned actor, uint16_t item);
unsigned ScSupplyReserve(struct ScSupplyBattle *battle, unsigned side, unsigned actor, uint16_t item);
void ScSupplyCancel(struct ScSupplyBattle *battle, unsigned actor);
unsigned ScSupplyCommit(struct ScSupplyBattle *battle, unsigned side, unsigned actor, uint16_t item);

#endif
