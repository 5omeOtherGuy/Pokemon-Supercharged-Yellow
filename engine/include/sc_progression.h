#ifndef GUARD_SC_PROGRESSION_H
#define GUARD_SC_PROGRESSION_H

#include "sc_progression_core.h"

struct Pokemon;
bool32 ScProgressionEnabled(void);
#if TESTING
void ScProgressionEnableForTests(bool32 enabled);
#endif
u32 ScGetBadgeCount(void);
void ScRefreshTrainerUnlocks(void);
void ScReadMonProgress(struct Pokemon *mon, struct ScMonProgress *out);
void ScWriteMonProgress(struct Pokemon *mon, const struct ScMonProgress *progress);
void ScProgressionBeginBattle(void);
void ScProgressionEnterField(u32 battler);
void ScProgressionRecordFaint(u32 battler);
void ScProgressionRecordDefeat(u32 battler);
u32 ScProgressionFinishBattle(u32 outcome);
u32 ScProgressionOrdinaryExp(u32 value);

#endif
