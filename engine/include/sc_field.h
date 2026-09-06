#ifndef GUARD_SC_FIELD_H
#define GUARD_SC_FIELD_H

#include "global.h"
#include "constants/field_move.h"

#define SC_FIELD_PAGE_SIZE 5
#define SC_FIELD_ACTION_OPEN 240
#define SC_FIELD_ACTION_NEXT 241
#define SC_FIELD_ACTION_BACK 242

bool32 ScFieldIsPermitMove(enum FieldMove fieldMove);
bool32 ScFieldHasPermit(enum FieldMove fieldMove);
bool32 ScFieldCanUse(struct Pokemon *mon, enum FieldMove fieldMove);
u32 ScFieldFindUser(enum FieldMove fieldMove);
bool32 ScFieldCanReplace(u32 slot, struct Pokemon *replacement);
bool32 ScFieldCanRelease(struct BoxPokemon *mon, s32 box, s32 position);
// Output contains at most five fields, NEXT and BACK (seven entries).
u32 ScFieldBuildPage(struct Pokemon *mon, u32 page, u8 *actions);

#endif
