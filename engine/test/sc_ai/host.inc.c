/* Host regression tests for the engine-independent observation scorer. */
#include "sc_ai.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static struct ScAiObservation arena(void)
{
    struct ScAiObservation o = {0};
    o.actorCount = 1;
    o.actors[0] = 1;
    o.aliveMask = 3;
    o.ownMask = 2;
    for (int i = 0; i < 4; ++i) o.hp[i] = 100;
    return o;
}

int main(void)
{
    struct ScAiObservation o = arena();
    struct ScAiChoice choices[2];
    assert(ScAiEstimateStat(100, 50, 110, 0) == 124);
    assert(ScAiEstimateStat(100, 50, 90, 0) == 101);
    assert(ScAiEstimateStat(100, 50, 100, 1) == 168);
    assert(ScAiEstimateHp(100, 24, 48) == 50);
    assert(ScAiEstimateHp(100, 1, 48) == 3);
    o.count[0] = 2;
    o.options[0][0] = (struct ScAiOption){.kind=SC_AI_MOVE,.index=0,.target=0,.damage={30}};
    o.options[0][1] = (struct ScAiOption){.kind=SC_AI_MOVE,.index=1,.target=0,.damage={100}};
    ScAiChoose(&o, choices);
    assert(choices[0].index == 1); /* Secure KO beats chip damage. */
    o.options[0][1].damage[0] = 0;
    ScAiChoose(&o, choices);
    assert(choices[0].index == 0); /* Immunity is never mistaken for a KO. */

    /* Re-evaluation is pure and does not mutate caller-owned observations. */
    struct ScAiObservation before = o;
    struct ScAiChoice baseline[2];
    ScAiChoose(&o, baseline);
    ScAiChoose(&o, choices);
    assert(memcmp(choices, baseline, sizeof(choices)) == 0);
    assert(memcmp(&o, &before, sizeof(o)) == 0);

    /* Joint doubles planning avoids redundant KOs and damage to its ally. */
    o = arena(); o.actorCount=2; o.actors[1]=3; o.aliveMask=15; o.ownMask=10;
    o.count[0]=o.count[1]=2;
    for (unsigned a=0;a<2;a++) {
        o.options[a][0]=(struct ScAiOption){.kind=SC_AI_MOVE,.index=0,.target=0,.damage={100}};
        o.options[a][1]=(struct ScAiOption){.kind=SC_AI_MOVE,.index=0,.target=2,.damage={[2]=100}};
    }
    ScAiChoose(&o,choices);
    assert(choices[0].target != choices[1].target);
    o.options[0][0].damage[3]=150;
    ScAiChoose(&o,choices);
    assert(choices[0].target == 2);

    /* A partner's Protect permits a worthwhile spread move. */
    o.options[1][0]=(struct ScAiOption){.kind=SC_AI_MOVE,.index=1,.target=3,.protects=1,.utility=90};
    o.count[1]=1;
    o.options[0][0].damage[0]=200;
    ScAiChoose(&o,choices);
    assert(choices[0].target == 0);

    /* Never reserve the same party slot or consumable twice in doubles. */
    o=arena(); o.actorCount=2; o.actors[1]=3;
    o.count[0]=o.count[1]=2;
    for (unsigned a=0;a<2;a++) {
        o.options[a][0]=(struct ScAiOption){.kind=SC_AI_SWITCH,.index=2,.resource=1,.utility=100};
        o.options[a][1]=(struct ScAiOption){.kind=SC_AI_MOVE,.index=0,.target=0,.damage={20}};
    }
    ScAiChoose(&o,choices);
    assert(!(choices[0].kind==SC_AI_SWITCH && choices[1].kind==SC_AI_SWITCH));
    for (unsigned a=0;a<2;a++) o.options[a][0].kind=SC_AI_ITEM;
    ScAiChoose(&o,choices);
    assert(!(choices[0].kind==SC_AI_ITEM && choices[1].kind==SC_AI_ITEM));

    /* Duplicate team screens are inferior to screen + attack. */
    for (unsigned a=0;a<2;a++) o.options[a][0]=(struct ScAiOption){.kind=SC_AI_MOVE,.effectMask=1,.utility=40};
    ScAiChoose(&o,choices);
    assert(choices[0].index != 255 && choices[1].index != 255);
    assert(choices[0].option != choices[1].option);
    o=arena(); o.count[0]=0;
    ScAiChoose(&o,choices);
    assert(choices[0].index==0); /* Defined Struggle/fallback slot. */
    puts("SC AI host tests passed: estimates, tactics, doubles, resource conflicts, purity");
    return 0;
}
