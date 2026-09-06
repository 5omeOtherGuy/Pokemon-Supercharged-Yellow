#ifndef GUARD_SC_BATTLE_H
#define GUARD_SC_BATTLE_H

/* Current active effects only: never expose stored collections or training.
 * Percentages use 100 as neutral. Defensive percentages multiply incoming damage.
 * Conditions are evaluated from public status/healthbar/turn observations. */
struct ScPublicEffects
{
    u16 offensivePhysicalPercent;
    u16 offensiveSpecialPercent;
    u16 defensivePhysicalPercent;
    u16 defensiveSpecialPercent;
    u16 speedPercent;
    u32 statusImmunity;
    u8 endTurnHealFraction; /* denominator (e.g. 32); zero disables */
};

void ScGetPublicBattlerEffects(u32 battler, struct ScPublicEffects *out);

#endif
