#ifndef GUARD_SC_BATTLE_H
#define GUARD_SC_BATTLE_H

#include "sc_effects.h"

void ScGetPublicSwitchInEffects(u32 battler, u32 partyIndex, struct ScPublicEffects *out);
void ScGetPublicBattlerEffects(u32 battler, struct ScPublicEffects *out);
void ScGetPublicSwitchInEffects(u32 battler, u32 partyIndex, struct ScPublicEffects *out);

#endif
