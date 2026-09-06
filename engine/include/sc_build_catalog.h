#ifndef GUARD_SC_BUILD_CATALOG_H
#define GUARD_SC_BUILD_CATALOG_H

struct ScPassiveInfo
{
    const u8 *name;
    const u8 *description;
    u8 cost;
};

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

#define SC_TRAINER_PASSIVE_COUNT 8
#define SC_GYM_REMATCH_FIRST 628
#define SC_GYM_REMATCH_LAST 651

extern const struct ScPassiveInfo gScCapabilityInfo[12];
extern const struct ScPassiveInfo gScTrainerPassiveInfo[8];
u32 ScGetNpcCapabilities(u16 trainerId, u32 partyIndex);
u32 ScGetNpcTrainerPassives(u16 trainerId);
u32 ScGetNpcTrainerBudget(u16 trainerId);

#endif
