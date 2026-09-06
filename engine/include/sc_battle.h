#ifndef GUARD_SC_BATTLE_H
#define GUARD_SC_BATTLE_H

#include "sc_effects.h"

u32 ScApplySpeedEffects(u32 battler, u32 speed);
s32 ScApplyDamageEffects(u32 battlerAtk, u32 battlerDef, u32 category, s32 damage);
bool32 ScHasStatusImmunity(u32 battler, u32 status);
u32 ScGetEndTurnHeal(u32 battler);
// Scope only the original attacker's assignment during delayed damage calculation.
void ScBeginDelayedDamage(u32 battler, u32 partyIndex);
void ScEndDelayedDamage(void);
bool32 ScEffectsEnabled(void);
#if TESTING
void ScEffectsEnableForTests(bool32 enabled);
#endif

void ScGetPublicBattlerEffects(u32 battler, struct ScPublicEffects *out);
void ScGetPublicSwitchInEffects(u32 battler, u32 partyIndex, struct ScPublicEffects *out);

#endif
