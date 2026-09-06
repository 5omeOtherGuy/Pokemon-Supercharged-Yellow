#include "global.h"
#include "sc_battle.h"

#if !TESTING
#error "Neutral public-effect fixtures must never be linked into a production build"
#endif

/* Intermediate adapter tests can run before the shared effect provider lands.
 * This file is linked ONLY by the test target. A real provider's strong symbols
 * override these weak neutral fixtures. Production has no fallback definitions.
 * Verify W (fixture) versus T (real provider) with arm-none-eabi-nm. */
__attribute__((weak)) void ScGetPublicBattlerEffects(u32 battler, struct ScPublicEffects *out)
{
    (void)battler;
    memset(out, 0, sizeof(*out));
    out->offensivePhysicalPercent = 100;
    out->offensiveSpecialPercent = 100;
    out->defensivePhysicalPercent = 100;
    out->defensiveSpecialPercent = 100;
    out->speedPercent = 100;
}

__attribute__((weak)) void ScGetPublicSwitchInEffects(u32 battler, u32 partyIndex, struct ScPublicEffects *out)
{
    (void)partyIndex;
    ScGetPublicBattlerEffects(battler, out);
}
