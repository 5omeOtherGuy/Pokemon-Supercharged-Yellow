#ifndef GUARD_SC_BUILD_CATALOG_H
#define GUARD_SC_BUILD_CATALOG_H

struct ScPassiveInfo
{
    const u8 *name;
    const u8 *description;
    u8 cost;
};

#include "sc_effects.h"

#define SC_TRAINER_PASSIVE_COUNT 8
#define SC_GYM_REMATCH_FIRST 628
#define SC_GYM_REMATCH_LAST 651

extern const struct ScPassiveInfo gScCapabilityInfo[12];
extern const struct ScPassiveInfo gScTrainerPassiveInfo[8];
u32 ScGetEncounterCapability(u16 trainerId);
u32 ScGetNpcCapabilities(u16 trainerId, u32 partyIndex);
u32 ScGetNpcTrainerPassives(u16 trainerId);
u32 ScGetNpcTrainerBudget(u16 trainerId);

#endif
