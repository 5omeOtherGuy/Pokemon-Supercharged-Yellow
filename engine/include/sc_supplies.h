#ifndef GUARD_SC_SUPPLIES_H
#define GUARD_SC_SUPPLIES_H

#include "global.h"
#include "sc_supplies_core.h"

unsigned ScSuppliesCategory(enum Item item);
bool32 ScSuppliesSetSelection(const u16 items[SC_SUPPLY_SLOTS], const u8 quantities[SC_SUPPLY_SLOTS]);
u32 ScSuppliesGetPlayerLoadout(u16 items[SC_SUPPLY_SLOTS], u8 quantities[SC_SUPPLY_SLOTS]);
bool32 ScSuppliesIsMajorTrainer(u16 trainerId);
bool32 ScSuppliesApplies(void);
// Call at every encounter start, including wild, to reset held-consumption history.
bool32 ScSuppliesBeginBattle(void);
void ScSuppliesEndBattle(void); // after held-item restoration
bool32 ScSuppliesCanUse(enum BattlerId battler, enum Item item);
bool32 ScSuppliesReserve(enum BattlerId battler, enum Item item);
void ScSuppliesCancel(enum BattlerId battler);
bool32 ScSuppliesCommit(enum BattlerId battler, enum Item item);
// Outside battle, applies the player passive; call before the max-HP clamp.
u32 ScSuppliesHealAmount(enum BattlerId battler, u32 amount);
void ScSuppliesRecordHeldConsumption(u32 trainer, u32 slot, enum Item item);
bool32 ScSuppliesWasHeldConsumed(u32 trainer, u32 slot, enum Item item);

#if TESTING
void ScSuppliesEnableForTests(bool32 enabled);
#endif

#endif
