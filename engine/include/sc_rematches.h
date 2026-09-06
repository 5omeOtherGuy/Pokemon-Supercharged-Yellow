#ifndef GUARD_SC_REMATCHES_H
#define GUARD_SC_REMATCHES_H

#include "global.h"

// Kanto readiness stores one boolean per table entry, never a truncated trainer ID.
bool32 ScRematchIsMainlandMap(u16 mapGroup, u16 mapNum);
u16 ScRematchResolve(u16 trainerId);
bool32 ScRematchIsReady(u16 trainerId);
bool32 ScRematchSetReady(u16 trainerId);
void ScRematchClearReady(u16 trainerId);

#endif
