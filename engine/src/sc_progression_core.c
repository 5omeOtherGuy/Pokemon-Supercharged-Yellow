#include "sc_progression_core.h"

static unsigned BadgeStage(unsigned badges)
{
    return badges > 8 ? 8 : badges;
}

unsigned ScLevelCap(unsigned badges, unsigned champion)
{
    static const uint8_t caps[] = {15, 22, 28, 36, 44, 50, 56, 62, 68};
    return champion ? 75 : caps[BadgeStage(badges)];
}

unsigned ScTrainingCeiling(unsigned badges)
{
    static const uint8_t caps[] = {24, 32, 40, 48, 56, 64, 72, 80, 96};
    return caps[BadgeStage(badges)];
}

unsigned ScTrainerCapacity(unsigned badges)
{
    return BadgeStage(badges) + 2;
}

void ScBeginParticipation(struct ScBattleParticipation *battle)
{
    battle->entered = 0;
    battle->fainted = 0;
    battle->finished = 0;
}

void ScEnterField(struct ScBattleParticipation *battle, unsigned slot)
{
    if (!battle->finished && slot < SC_STAT_COUNT)
        battle->entered |= 1u << slot;
}

void ScFaintInBattle(struct ScBattleParticipation *battle, unsigned slot)
{
    if (!battle->finished && slot < SC_STAT_COUNT)
        battle->fainted |= 1u << slot;
}

unsigned ScFinishParticipation(struct ScBattleParticipation *battle, unsigned won)
{
    if (battle->finished)
        return 0;
    battle->finished = 1;
    return won ? battle->entered & ~battle->fainted : 0;
}

unsigned ScSetTrainingFocus(struct ScMonProgress *mon, unsigned focus)
{
    if (focus > SC_FOCUS_BALANCED)
        return 0;
    mon->focus = focus;
    return 1;
}

unsigned ScApplyTraining(struct ScMonProgress *mon, uint8_t training[SC_STAT_COUNT], unsigned ceiling, uint32_t fraction)
{
    unsigned points, remainder, awarded = 0;
    if (mon->focus > SC_FOCUS_BALANCED || ceiling > 255)
        return 0;
    /* Add quotient/remainder separately: a large reward cannot overflow its carry. */
    remainder = (fraction & 255) + mon->trainingRemainder;
    points = (fraction >> 8) + (remainder >> 8);
    mon->trainingRemainder = remainder & 255;
    while (points != 0)
    {
        unsigned stat = mon->focus, i;
        if (stat == SC_FOCUS_BALANCED)
        {
            stat = 0;
            for (i = 1; i < SC_STAT_COUNT; i++)
                if (training[i] < training[stat])
                    stat = i;
        }
        if (training[stat] >= ceiling)
        {
            mon->trainingRemainder = 0;
            break; /* No bank beyond a stage/focus ceiling, and no redistribution. */
        }
        training[stat]++;
        awarded++;
        points--;
    }
    return awarded;
}

unsigned ScCapabilityCost(unsigned capability)
{
    static const uint8_t costs[SC_CAPABILITY_COUNT] = {1, 1, 2, 2, 1, 3, 3, 2, 1, 2, 3, 2};
    return capability < SC_CAPABILITY_COUNT ? costs[capability] : 0;
}

unsigned ScEarnCapability(struct ScMonProgress *mon, unsigned capability)
{
    unsigned bit;
    if (capability >= SC_CAPABILITY_COUNT)
        return 0;
    bit = 1u << capability;
    if (mon->earnedCapabilities & bit)
        return 0;
    mon->earnedCapabilities |= bit;
    return 1;
}

unsigned ScAssignCapabilities(struct ScMonProgress *mon, unsigned assignment)
{
    unsigned i, cost = 0;
    if ((assignment >> SC_CAPABILITY_COUNT) || (assignment & ~mon->earnedCapabilities))
        return 0;
    for (i = 0; i < SC_CAPABILITY_COUNT; i++)
        if (assignment & (1u << i))
            cost += ScCapabilityCost(i);
    if (cost > 3)
        return 0;
    mon->activeCapabilities = assignment;
    return 1;
}

uint32_t ScTrainingFraction(uint32_t expValue, unsigned recipientLevel)
{
    /* Gen-III defeat EXP value, converted at 32 * recipient level EXP per point.
     * Keep 1/256-point carry; ordinary EXP suppression at the cap is irrelevant.
     * Split before multiplying to avoid overflow for malformed oversized values. */
    uint32_t denominator;
    if (recipientLevel == 0 || recipientLevel > 100)
        return 0;
    denominator = 32 * recipientLevel;
    if (expValue / denominator > UINT32_MAX / 256)
        return UINT32_MAX;
    return expValue / denominator * 256 + (expValue % denominator) * 256 / denominator;
}
