#include "global.h"
#include "data.h"
#include "sc_build_catalog.h"
#include "constants/opponents_frlg.h"

const struct ScPassiveInfo gScCapabilityInfo[12] =
{
    {COMPOUND_STRING("Brace"), COMPOUND_STRING("Incoming physical move damage\nis 95% of normal."), 1},
    {COMPOUND_STRING("Flow"), COMPOUND_STRING("Incoming special move damage\nis 95% of normal."), 1},
    {COMPOUND_STRING("Quick Start"), COMPOUND_STRING("Speed is 110% on the first\nturn after entering battle.\nTriggers again after switching\nout and returning."), 2},
    {COMPOUND_STRING("Staying Power"), COMPOUND_STRING("Incoming physical and special\nmove damage is 92% while your\nown HP bar is at least half."), 2},
    {COMPOUND_STRING("Poise"), COMPOUND_STRING("Physical and special move\ndamage dealt is 108% while\nyou have a nonvolatile status:\npoison, burn, sleep, etc."), 1},
    {COMPOUND_STRING("Focus"), COMPOUND_STRING("Special move damage dealt is\n112%. Physical move damage\ndealt is reduced to 90%."), 3},
    {COMPOUND_STRING("Pressure"), COMPOUND_STRING("Physical move damage dealt is\n112%. Special move damage\ndealt is reduced to 90%."), 3},
    {COMPOUND_STRING("Last Stand"), COMPOUND_STRING("Physical and special move\ndamage dealt is 110% while\nyour own HP bar is half\nor lower."), 2},
    {COMPOUND_STRING("Clear Head"), COMPOUND_STRING("Prevents freezing.\nDoes not prevent other status\nconditions."), 1},
    {COMPOUND_STRING("Recovery"), COMPOUND_STRING("At each turn's end, restores\n1/32 of your maximum HP."), 2},
    {COMPOUND_STRING("Equilibrium"), COMPOUND_STRING("Incoming physical and special\nmove damage is 92%. Your own\nphysical and special move\ndamage dealt is 96%."), 3},
    {COMPOUND_STRING("Momentum"), COMPOUND_STRING("Physical and special move\ndamage dealt is 106%.\nSpeed is 104%."), 2},
};

const struct ScPassiveInfo gScTrainerPassiveInfo[8] =
{
    {COMPOUND_STRING("Study"), COMPOUND_STRING("Ordinary battle experience\ngained is 120% of normal.\nDoes not raise the level cap."), 2},
    {COMPOUND_STRING("Formation"), COMPOUND_STRING("Your team's incoming physical\nmove damage is 97% of normal."), 2},
    {COMPOUND_STRING("Composure"), COMPOUND_STRING("Your team's incoming special\nmove damage is 97% of normal."), 2},
    {COMPOUND_STRING("Tempo"), COMPOUND_STRING("Your team's Speed is 104%\nof normal."), 3},
    {COMPOUND_STRING("Offense"), COMPOUND_STRING("Your team's physical and\nspecial move damage dealt\nis 104% of normal."), 3},
    {COMPOUND_STRING("Preparation"), COMPOUND_STRING("HP restored by your team's\nbag healing items is 110%\nof normal."), 2},
    {COMPOUND_STRING("Field Study"), COMPOUND_STRING("Eligible battle training\ngains are 110% of normal.\nSurvival, victory and training\nceilings still apply."), 2},
    {COMPOUND_STRING("Recovery Coach"), COMPOUND_STRING("At each turn's end, your\nactive team members recover\n1/64 of their maximum HP."), 3},
};

static const u16 sGymIds[] =
{
    TRAINER_LEADER_BROCK, TRAINER_LEADER_MISTY, TRAINER_LEADER_LT_SURGE,
    TRAINER_LEADER_ERIKA, TRAINER_LEADER_KOGA, TRAINER_LEADER_SABRINA,
    TRAINER_LEADER_BLAINE, TRAINER_LEADER_GIOVANNI,
};

static const u8 sGymBudgets[] = {2, 3, 4, 5, 6, 6, 7, 8};
static const u32 sGymPassives[] =
{
    1u << SC_PASSIVE_FORMATION,
    1u << SC_PASSIVE_COMPOSURE,
    1u << SC_PASSIVE_TEMPO,
    (1u << SC_PASSIVE_COMPOSURE) | (1u << SC_PASSIVE_PREPARATION),
    (1u << SC_PASSIVE_FORMATION) | (1u << SC_PASSIVE_TEMPO),
    (1u << SC_PASSIVE_COMPOSURE) | (1u << SC_PASSIVE_OFFENSE),
    (1u << SC_PASSIVE_OFFENSE) | (1u << SC_PASSIVE_PREPARATION),
    (1u << SC_PASSIVE_FORMATION) | (1u << SC_PASSIVE_OFFENSE),
};

static u32 GymIndex(u16 id)
{
    if (id >= SC_GYM_REMATCH_FIRST && id <= SC_GYM_REMATCH_LAST)
        return (id - SC_GYM_REMATCH_FIRST) / 3;
    for (u32 i = 0; i < ARRAY_COUNT(sGymIds); i++)
        if (id == sGymIds[i])
            return i;
    return ARRAY_COUNT(sGymIds);
}

static u32 EliteIndex(u16 id)
{
    if (id >= TRAINER_ELITE_FOUR_LORELEI && id <= TRAINER_ELITE_FOUR_LANCE)
        return id - TRAINER_ELITE_FOUR_LORELEI;
    if (id >= TRAINER_ELITE_FOUR_LORELEI_2 && id <= TRAINER_ELITE_FOUR_LANCE_2)
        return id - TRAINER_ELITE_FOUR_LORELEI_2;
    return 4;
}

u32 ScGetEncounterCapability(u16 trainerId)
{
    u32 gym = GymIndex(trainerId), elite = EliteIndex(trainerId);
    if (gym < 8)
        return gym;
    return elite < 4 ? 8 + elite : ARRAY_COUNT(gScCapabilityInfo);
}

static u32 IsChampion(u16 id)
{
    return (id >= TRAINER_CHAMPION_FIRST_SQUIRTLE && id <= TRAINER_CHAMPION_FIRST_CHARMANDER)
        || (id >= TRAINER_CHAMPION_REMATCH_SQUIRTLE && id <= TRAINER_CHAMPION_REMATCH_CHARMANDER);
}

// Story opponents use selected partial builds, never an assumed full player kit.
struct ScStoryNpcBuild
{
    u16 first, last;
    u8 budget;
    u32 passives;
    u16 aceCapabilities, supportCapabilities;
};

static const struct ScStoryNpcBuild sStoryBuilds[] =
{
    {TRAINER_RIVAL_CERULEAN_SQUIRTLE, TRAINER_RIVAL_CERULEAN_CHARMANDER, 3,
        1u << SC_PASSIVE_FORMATION, 1u << SC_CAP_BRACE, 0},
    {TRAINER_RIVAL_SS_ANNE_SQUIRTLE, TRAINER_RIVAL_SS_ANNE_CHARMANDER, 3,
        1u << SC_PASSIVE_FORMATION, 1u << SC_CAP_FLOW, 0},
    {TRAINER_RIVAL_POKEMON_TOWER_SQUIRTLE, TRAINER_RIVAL_POKEMON_TOWER_CHARMANDER, 5,
        (1u << SC_PASSIVE_FORMATION) | (1u << SC_PASSIVE_TEMPO),
        1u << SC_CAP_QUICK_START, 1u << SC_CAP_FLOW},
    {TRAINER_RIVAL_SILPH_SQUIRTLE, TRAINER_RIVAL_SILPH_CHARMANDER, 7,
        (1u << SC_PASSIVE_FORMATION) | (1u << SC_PASSIVE_COMPOSURE) | (1u << SC_PASSIVE_TEMPO),
        1u << SC_CAP_STAYING_POWER, 1u << SC_CAP_FLOW},
    {TRAINER_RIVAL_ROUTE22_LATE_SQUIRTLE, TRAINER_RIVAL_ROUTE22_LATE_CHARMANDER, 8,
        (1u << SC_PASSIVE_FORMATION) | (1u << SC_PASSIVE_COMPOSURE) | (1u << SC_PASSIVE_OFFENSE),
        (1u << SC_CAP_STAYING_POWER) | (1u << SC_CAP_BRACE), 1u << SC_CAP_FLOW},
    {TRAINER_BOSS_GIOVANNI, TRAINER_BOSS_GIOVANNI, 5,
        (1u << SC_PASSIVE_FORMATION) | (1u << SC_PASSIVE_OFFENSE), 1u << SC_CAP_BRACE, 0},
    {TRAINER_BOSS_GIOVANNI_2, TRAINER_BOSS_GIOVANNI_2, 7,
        (1u << SC_PASSIVE_FORMATION) | (1u << SC_PASSIVE_COMPOSURE) | (1u << SC_PASSIVE_OFFENSE),
        1u << SC_CAP_STAYING_POWER, 1u << SC_CAP_BRACE},
    {TRAINER_SC_ROCKET_DUO_1, TRAINER_SC_ROCKET_DUO_1, 2,
        1u << SC_PASSIVE_FORMATION, 0, 0},
    {TRAINER_SC_ROCKET_DUO_2, TRAINER_SC_ROCKET_DUO_2, 5,
        (1u << SC_PASSIVE_FORMATION) | (1u << SC_PASSIVE_TEMPO),
        1u << SC_CAP_BRACE, 1u << SC_CAP_FLOW},
    {TRAINER_SC_ROCKET_DUO_3, TRAINER_SC_ROCKET_DUO_3, 5,
        (1u << SC_PASSIVE_COMPOSURE) | (1u << SC_PASSIVE_TEMPO),
        1u << SC_CAP_QUICK_START, 1u << SC_CAP_BRACE},
    {TRAINER_SC_ROCKET_DUO_4, TRAINER_SC_ROCKET_DUO_4, 7,
        (1u << SC_PASSIVE_FORMATION) | (1u << SC_PASSIVE_COMPOSURE) | (1u << SC_PASSIVE_TEMPO),
        (1u << SC_CAP_QUICK_START) | (1u << SC_CAP_BRACE), 1u << SC_CAP_FLOW},
};

static const struct ScStoryNpcBuild *StoryBuild(u16 trainerId)
{
    for (u32 i = 0; i < ARRAY_COUNT(sStoryBuilds); i++)
        if (trainerId >= sStoryBuilds[i].first && trainerId <= sStoryBuilds[i].last)
            return &sStoryBuilds[i];
    return 0;
}

u32 ScGetNpcTrainerBudget(u16 trainerId)
{
    u32 gym;
    u32 elite;
    if (trainerId >= TRAINERS_COUNT)
        return 0;
    const struct ScStoryNpcBuild *story = StoryBuild(trainerId);
    if (story)
        return story->budget;
    gym = GymIndex(trainerId);
    if (trainerId >= SC_GYM_REMATCH_FIRST && trainerId <= SC_GYM_REMATCH_LAST)
        return 6 + 2 * ((trainerId - SC_GYM_REMATCH_FIRST) % 3);
    if (gym < ARRAY_COUNT(sGymIds))
        return sGymBudgets[gym];
    elite = EliteIndex(trainerId);
    if (elite < 4)
        return elite == 0 ? 8 : elite == 3 ? 10 : 9;
    return IsChampion(trainerId) ? 10 : 0;
}

static u32 TrainerCost(u32 mask)
{
    u32 cost = 0;
    for (u32 i = 0; i < ARRAY_COUNT(gScTrainerPassiveInfo); i++)
        if (mask & (1u << i))
            cost += gScTrainerPassiveInfo[i].cost;
    return cost;
}

u32 ScGetNpcTrainerPassives(u16 trainerId)
{
    u32 gym;
    u32 elite;
    if (trainerId >= TRAINERS_COUNT)
        return 0;
    const struct ScStoryNpcBuild *story = StoryBuild(trainerId);
    if (story)
        return story->passives;
    gym = GymIndex(trainerId);
    if (gym < ARRAY_COUNT(sGymIds))
    {
        u32 mask = sGymPassives[gym];
        if (trainerId >= SC_GYM_REMATCH_FIRST)
        {
            u32 candidate = mask | (1u << SC_PASSIVE_TEMPO);
            if (TrainerCost(candidate) <= ScGetNpcTrainerBudget(trainerId))
                mask = candidate;
        }
        return mask;
    }
    elite = EliteIndex(trainerId);
    switch (elite)
    {
    case 0: return (1u << SC_PASSIVE_COMPOSURE) | (1u << SC_PASSIVE_TEMPO);
    case 1: return (1u << SC_PASSIVE_FORMATION) | (1u << SC_PASSIVE_OFFENSE);
    case 2: return (1u << SC_PASSIVE_COMPOSURE) | (1u << SC_PASSIVE_TEMPO) | (1u << SC_PASSIVE_PREPARATION);
    case 3: return (1u << SC_PASSIVE_TEMPO) | (1u << SC_PASSIVE_OFFENSE);
    }
    return IsChampion(trainerId)
        ? (1u << SC_PASSIVE_FORMATION) | (1u << SC_PASSIVE_COMPOSURE) | (1u << SC_PASSIVE_OFFENSE) : 0;
}

u32 ScGetNpcCapabilities(u16 trainerId, u32 partyIndex)
{
    u32 gym;
    u32 elite;
    u32 size, ace = 0, support;
    const struct TrainerMon *party;
    if (trainerId >= TRAINERS_COUNT)
        return 0;
    size = GetTrainerPartySizeFromId(trainerId);
    if (partyIndex >= size || size == 0 || size > 6)
        return 0;
    party = GetTrainerPartyFromId(trainerId);
    if (!party)
        return 0;
    // Opening order is tactical: the signature/highest-level mon need not be
    // last. Resolve the ace from actual levels; use the last mon in a tie.
    for (u32 i = 1; i < size; i++)
        if (party[i].lvl >= party[ace].lvl)
            ace = i;
    // Keep the separate partial supporting loadout on a different teammate.
    support = ace == size - 1 ? size - 2 : size - 1;
    const struct ScStoryNpcBuild *story = StoryBuild(trainerId);
    if (story)
    {
        if (partyIndex == ace)
            return story->aceCapabilities;
        if (partyIndex == support)
            return story->supportCapabilities;
        return 0;
    }
    gym = GymIndex(trainerId);
    if (gym < ARRAY_COUNT(sGymIds))
    {
        if (partyIndex == ace)
        {
            u32 mask = 1u << gym;
            if (trainerId >= SC_GYM_REMATCH_FIRST && (trainerId - SC_GYM_REMATCH_FIRST) % 3 == 2
                && gScCapabilityInfo[gym].cost <= 2)
                mask |= 1u << (gym == SC_CAP_BRACE ? SC_CAP_FLOW : SC_CAP_BRACE);
            return mask;
        }
        if (partyIndex == support && (gym >= 4 || trainerId >= SC_GYM_REMATCH_FIRST))
            return 1u << (gym % 2 ? SC_CAP_FLOW : SC_CAP_BRACE);
        return 0;
    }
    elite = EliteIndex(trainerId);
    if (elite < 4)
    {
        if (partyIndex == ace)
            return 1u << (8 + elite);
        if (partyIndex == support)
            return 1u << (elite % 2 ? SC_CAP_BRACE : SC_CAP_FLOW);
        return 0;
    }
    if (IsChampion(trainerId))
    {
        if (partyIndex == ace)
            return (1u << SC_CAP_MOMENTUM) | (1u << SC_CAP_BRACE);
        if (partyIndex == support)
            return 1u << SC_CAP_FOCUS;
    }
    return 0;
}
