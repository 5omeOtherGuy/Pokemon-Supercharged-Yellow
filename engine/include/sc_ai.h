#ifndef GUARD_SC_AI_H
#define GUARD_SC_AI_H

/* This value-only decision boundary contains no pointers into game state. */
#define SC_AI_BATTLERS 4
#define SC_AI_ACTORS 2
#define SC_AI_OPTIONS 32

enum ScAiActionKind { SC_AI_MOVE, SC_AI_SWITCH, SC_AI_ITEM };
struct ScAiOption
{
    unsigned short damage[SC_AI_BATTLERS];
    short utility;
    unsigned short effectMask;
    unsigned char kind, index, target, resource, protects, boostsPartner;
};
struct ScAiObservation
{
    struct ScAiOption options[SC_AI_ACTORS][SC_AI_OPTIONS];
    unsigned short hp[SC_AI_BATTLERS];
    unsigned char count[SC_AI_ACTORS], actors[SC_AI_ACTORS];
    unsigned char actorCount, aliveMask, ownMask;
};
struct ScAiChoice
{
    unsigned char kind, index, target, option;
};
unsigned int ScAiEstimateStat(unsigned int base, unsigned int level, unsigned int naturePercent, unsigned int hp);
unsigned int ScAiEstimateHp(unsigned int estimatedMax, unsigned int visiblePixels, unsigned int fullPixels);
void ScAiChoose(const struct ScAiObservation *observation, struct ScAiChoice choices[SC_AI_ACTORS]);

#ifndef SC_AI_HOST_TEST
bool32 ScAiEnabled(void);
void ScAiEnableForTests(bool32 enabled);
void ScAiPrepareTurn(void);
void ScAiComputeBattler(enum BattlerId battler);
u32 ScAiMoveIndex(enum BattlerId battler);
u32 ScAiSwitchIn(enum BattlerId battler);
void ScAiEmitAction(enum BattlerId battler);
bool32 ScAiUseItem(enum BattlerId battler);
/* Exposes the exact sanitized value used by tests; never a battle-state pointer. */
void ScAiObserve(struct ScAiObservation *out);
#endif
#endif
