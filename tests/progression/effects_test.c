#include <assert.h>
#include "sc_effects.h"
#include "constants/battle.h"

#define CAP(id) (1u << (id))
int main(void)
{
    struct ScPublicEffects effects;
    ScResolvePublicEffects(0, 0, 48, 0, 0, &effects);
    assert(effects.offensivePhysicalPercent == 100 && effects.offensiveSpecialPercent == 100);
    assert(effects.defensivePhysicalPercent == 100 && effects.defensiveSpecialPercent == 100);
    assert(effects.speedPercent == 100 && effects.statusImmunity == 0);
    assert(effects.endTurnHealFraction == 0 && effects.endTurnHealNumerator == 0);
    ScResolvePublicEffects(CAP(SC_CAP_BRACE) | CAP(SC_CAP_FLOW), 0, 48, 0, 0, &effects);
    assert(effects.defensivePhysicalPercent == 95 && effects.defensiveSpecialPercent == 95);
    ScResolvePublicEffects(CAP(SC_CAP_QUICK_START), CAP(SC_PASSIVE_TEMPO), 48, 1, 0, &effects);
    assert(effects.speedPercent == 114);
    ScResolvePublicEffects(CAP(SC_CAP_QUICK_START), 0, 48, 0, 0, &effects);
    assert(effects.speedPercent == 100);
    ScResolvePublicEffects(CAP(SC_CAP_STAYING_POWER), 0, 24, 0, 0, &effects);
    assert(effects.defensivePhysicalPercent == 92 && effects.defensiveSpecialPercent == 92);
    ScResolvePublicEffects(CAP(SC_CAP_STAYING_POWER), 0, 23, 0, 0, &effects);
    assert(effects.defensivePhysicalPercent == 100);
    ScResolvePublicEffects(CAP(SC_CAP_POISE), 0, 48, 0, STATUS1_POISON, &effects);
    assert(effects.offensivePhysicalPercent == 108 && effects.offensiveSpecialPercent == 108);
    ScResolvePublicEffects(CAP(SC_CAP_POISE), 0, 48, 0, 0, &effects);
    assert(effects.offensivePhysicalPercent == 100);
    ScResolvePublicEffects(CAP(SC_CAP_FOCUS), CAP(SC_PASSIVE_OFFENSE), 48, 0, 0, &effects);
    assert(effects.offensiveSpecialPercent == 116 && effects.offensivePhysicalPercent == 93);
    ScResolvePublicEffects(CAP(SC_CAP_PRESSURE), 0, 48, 0, 0, &effects);
    assert(effects.offensiveSpecialPercent == 90 && effects.offensivePhysicalPercent == 112);
    ScResolvePublicEffects(CAP(SC_CAP_LAST_STAND), 0, 24, 0, 0, &effects);
    assert(effects.offensivePhysicalPercent == 110);
    ScResolvePublicEffects(CAP(SC_CAP_LAST_STAND), 0, 25, 0, 0, &effects);
    assert(effects.offensivePhysicalPercent == 100);
    ScResolvePublicEffects(CAP(SC_CAP_CLEAR_HEAD), 0, 48, 0, 0, &effects);
    assert(effects.statusImmunity == STATUS1_FREEZE);
    ScResolvePublicEffects(CAP(SC_CAP_RECOVERY), 0, 48, 0, 0, &effects);
    assert(effects.endTurnHealNumerator == 1 && effects.endTurnHealFraction == 32);
    ScResolvePublicEffects(0, CAP(SC_PASSIVE_RECOVERY_COACH), 48, 0, 0, &effects);
    assert(effects.endTurnHealNumerator == 1 && effects.endTurnHealFraction == 64);
    ScResolvePublicEffects(CAP(SC_CAP_RECOVERY), CAP(SC_PASSIVE_RECOVERY_COACH), 48, 0, 0, &effects);
    assert(effects.endTurnHealNumerator == 3 && effects.endTurnHealFraction == 64);
    ScResolvePublicEffects(CAP(SC_CAP_EQUILIBRIUM), CAP(SC_PASSIVE_FORMATION) | CAP(SC_PASSIVE_COMPOSURE), 48, 0, 0, &effects);
    assert(effects.defensivePhysicalPercent == 89 && effects.defensiveSpecialPercent == 89);
    assert(effects.offensivePhysicalPercent == 96 && effects.offensiveSpecialPercent == 96);
    ScResolvePublicEffects(CAP(SC_CAP_MOMENTUM), 0, 48, 0, 0, &effects);
    assert(effects.offensivePhysicalPercent == 106 && effects.offensiveSpecialPercent == 106 && effects.speedPercent == 104);
    return 0;
}
