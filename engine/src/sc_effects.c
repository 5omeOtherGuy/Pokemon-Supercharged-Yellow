#include "sc_effects.h"
#include "constants/pokemon.h"
#include "constants/battle.h"

#define ACTIVE(mask, id) ((mask) & (1u << (id)))
#define SCALE(field, percent) out->field = out->field * (percent) / 100

void ScResolvePublicEffects(unsigned capabilities, unsigned trainerPassives,
    unsigned hpBar, unsigned firstTurn, uint32_t status, struct ScPublicEffects *out)
{
    *out = (struct ScPublicEffects){
        .offensivePhysicalPercent = 100, .offensiveSpecialPercent = 100,
        .defensivePhysicalPercent = 100, .defensiveSpecialPercent = 100,
        .speedPercent = 100,
    };
    if (ACTIVE(capabilities, SC_CAP_BRACE))
        SCALE(defensivePhysicalPercent, 95);
    if (ACTIVE(capabilities, SC_CAP_FLOW))
        SCALE(defensiveSpecialPercent, 95);
    if (ACTIVE(capabilities, SC_CAP_QUICK_START) && firstTurn)
        SCALE(speedPercent, 110);
    if (ACTIVE(capabilities, SC_CAP_STAYING_POWER) && hpBar >= 24)
    {
        SCALE(defensivePhysicalPercent, 92);
        SCALE(defensiveSpecialPercent, 92);
    }
    if (ACTIVE(capabilities, SC_CAP_POISE) && (status & STATUS1_ANY))
    {
        SCALE(offensivePhysicalPercent, 108);
        SCALE(offensiveSpecialPercent, 108);
    }
    if (ACTIVE(capabilities, SC_CAP_FOCUS))
    {
        SCALE(offensivePhysicalPercent, 90);
        SCALE(offensiveSpecialPercent, 112);
    }
    if (ACTIVE(capabilities, SC_CAP_PRESSURE))
    {
        SCALE(offensivePhysicalPercent, 112);
        SCALE(offensiveSpecialPercent, 90);
    }
    if (ACTIVE(capabilities, SC_CAP_LAST_STAND) && hpBar <= 24)
    {
        SCALE(offensivePhysicalPercent, 110);
        SCALE(offensiveSpecialPercent, 110);
    }
    if (ACTIVE(capabilities, SC_CAP_CLEAR_HEAD))
        out->statusImmunity |= STATUS1_FREEZE;
    if (ACTIVE(capabilities, SC_CAP_RECOVERY))
    {
        out->endTurnHealNumerator = 1;
        out->endTurnHealFraction = 32;
    }
    if (ACTIVE(capabilities, SC_CAP_EQUILIBRIUM))
    {
        SCALE(defensivePhysicalPercent, 92);
        SCALE(defensiveSpecialPercent, 92);
        SCALE(offensivePhysicalPercent, 96);
        SCALE(offensiveSpecialPercent, 96);
    }
    if (ACTIVE(capabilities, SC_CAP_MOMENTUM))
    {
        SCALE(offensivePhysicalPercent, 106);
        SCALE(offensiveSpecialPercent, 106);
        SCALE(speedPercent, 104);
    }
    if (ACTIVE(trainerPassives, SC_PASSIVE_FORMATION))
        SCALE(defensivePhysicalPercent, 97);
    if (ACTIVE(trainerPassives, SC_PASSIVE_COMPOSURE))
        SCALE(defensiveSpecialPercent, 97);
    if (ACTIVE(trainerPassives, SC_PASSIVE_TEMPO))
        SCALE(speedPercent, 104);
    if (ACTIVE(trainerPassives, SC_PASSIVE_OFFENSE))
    {
        SCALE(offensivePhysicalPercent, 104);
        SCALE(offensiveSpecialPercent, 104);
    }
    if (ACTIVE(trainerPassives, SC_PASSIVE_RECOVERY_COACH))
    {
        out->endTurnHealNumerator = out->endTurnHealNumerator * 2 + 1;
        out->endTurnHealFraction = 64;
    }
}
