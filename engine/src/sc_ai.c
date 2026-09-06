#ifdef SC_AI_HOST_TEST
#include <string.h>
#else
#include "global.h"
#include "battle.h"
#endif
#include "sc_ai.h"

unsigned int ScAiEstimateStat(unsigned int base, unsigned int level, unsigned int naturePercent, unsigned int hp)
{
    /* Explicit prior: median IV 16, zero training. Nature is public. */
    unsigned int value = (2 * base + 16) * level / 100;
    return hp ? value + level + 10 : (value + 5) * naturePercent / 100;
}

unsigned int ScAiEstimateHp(unsigned int estimatedMax, unsigned int visiblePixels, unsigned int fullPixels)
{
    if (!fullPixels || !visiblePixels) return 0;
    return (estimatedMax * visiblePixels + fullPixels - 1) / fullPixels;
}

static int ScAiPairScore(const struct ScAiObservation *o, const struct ScAiOption *a, const struct ScAiOption *b)
{
    if (b && a->kind != SC_AI_MOVE && a->kind == b->kind
        && a->resource == b->resource && a->index == b->index)
        return -30000; /* One shared party slot or bag unit cannot be used twice. */
    int score = a->utility + (b ? b->utility : 0);
    if (b && (a->effectMask & b->effectMask)) score -= 80;
    for (unsigned int i = 0; i < SC_AI_BATTLERS; ++i)
    {
        if (!(o->aliveMask & (1u << i)) || !o->hp[i]) continue;
        unsigned int damage = a->damage[i] + (b ? b->damage[i] : 0);
        if ((a->protects && i == o->actors[0]) || (b && b->protects && i == o->actors[1])) damage = 0;
        unsigned int fraction = (damage > o->hp[i] ? o->hp[i] : damage) * 100 / o->hp[i];
        if (o->ownMask & (1u << i))
            score -= fraction * 2 + (damage >= o->hp[i] ? 120 : 0);
        else
            score += fraction + (damage >= o->hp[i] ? 80 : 0);
    }
    return score;
}

void ScAiChoose(const struct ScAiObservation *o, struct ScAiChoice choices[SC_AI_ACTORS])
{
    memset(choices, 0, sizeof(*choices) * SC_AI_ACTORS);
    if (!o->actorCount || !o->count[0]) return;
    unsigned int countA = o->count[0] > SC_AI_OPTIONS ? SC_AI_OPTIONS : o->count[0];
    unsigned int countB = o->actorCount > 1 ? o->count[1] : 0;
    if (countB > SC_AI_OPTIONS) countB = SC_AI_OPTIONS;
    int best = -32000;
    for (unsigned int i = 0; i < countA; ++i)
    {
        for (unsigned int j = 0; j < (countB ? countB : 1); ++j)
        {
            const struct ScAiOption *a = &o->options[0][i];
            const struct ScAiOption *b = countB ? &o->options[1][j] : 0;
            int score = ScAiPairScore(o, a, b);
            if (score <= best) continue; /* Stable tie order; never battle RNG. */
            best = score;
            choices[0] = (struct ScAiChoice){a->kind, a->index, a->target, i};
            if (b) choices[1] = (struct ScAiChoice){b->kind, b->index, b->target, j};
        }
    }
}
