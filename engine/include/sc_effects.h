#ifndef GUARD_SC_EFFECTS_H
#define GUARD_SC_EFFECTS_H

#include <stdint.h>

// These indices are the save/battle contract, not display-order suggestions.
enum ScCapabilityId
{
    SC_CAP_BRACE, SC_CAP_FLOW, SC_CAP_QUICK_START, SC_CAP_STAYING_POWER,
    SC_CAP_POISE, SC_CAP_FOCUS, SC_CAP_PRESSURE, SC_CAP_LAST_STAND,
    SC_CAP_CLEAR_HEAD, SC_CAP_RECOVERY, SC_CAP_EQUILIBRIUM, SC_CAP_MOMENTUM,
};

enum ScTrainerPassiveId
{
    SC_PASSIVE_STUDY, SC_PASSIVE_FORMATION, SC_PASSIVE_COMPOSURE,
    SC_PASSIVE_TEMPO, SC_PASSIVE_OFFENSE, SC_PASSIVE_PREPARATION,
    SC_PASSIVE_FIELD_STUDY, SC_PASSIVE_RECOVERY_COACH,
};

/* Current active effects only. Percentages use 100 as neutral; defensive
 * percentages multiply incoming damage. Healing is numerator / denominator. */
struct ScPublicEffects
{
    uint16_t offensivePhysicalPercent;
    uint16_t offensiveSpecialPercent;
    uint16_t defensivePhysicalPercent;
    uint16_t defensiveSpecialPercent;
    uint16_t speedPercent;
    uint32_t statusImmunity;
    uint8_t endTurnHealFraction;
    uint8_t endTurnHealNumerator;
};

// hpBar is the public 0..48 pixel band, never an exact HP percentage.
void ScResolvePublicEffects(unsigned capabilities, unsigned trainerPassives,
    unsigned hpBar, unsigned firstTurn, uint32_t status, struct ScPublicEffects *out);

#endif
