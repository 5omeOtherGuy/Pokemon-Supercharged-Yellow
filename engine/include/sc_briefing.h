#ifndef GUARD_SC_BRIEFING_H
#define GUARD_SC_BRIEFING_H

struct Trainer;
enum ScBriefingError
{
    SC_BRIEF_OK, SC_BRIEF_BAD_TRAINER, SC_BRIEF_VARIABLE_PARTY,
    SC_BRIEF_BAD_MON, SC_BRIEF_VARIABLE_ABILITY, SC_BRIEF_FIELD_EFFECT,
};

struct ScBriefingModel
{
    const struct Trainer *trainer;
    u16 trainerId;
    u8 partySize;
    u8 error;
};

enum ScBriefingScreen
{
    SC_BRIEF_TEAM, SC_BRIEF_BAG, SC_BRIEF_TRAINER, SC_BRIEF_MON,
    SC_BRIEF_CAPABILITIES, SC_BRIEF_DESCRIPTION, SC_BRIEF_CONFIRM,
};

struct ScBriefingView
{
    u8 screen;
    u8 cursor;
    u8 mon;
    u8 parent;
    u8 entry;
};

enum ScBriefingAction
{
    SC_BRIEF_IDLE, SC_BRIEF_REDRAW, SC_BRIEF_ACCEPT, SC_BRIEF_DECLINE,
};

bool32 ScBriefingLoad(u16 trainerId, struct ScBriefingModel *model);
void ScBriefingGetMoves(const struct ScBriefingModel *model, u32 index, u16 moves[4]);
u16 ScBriefingGetAbility(const struct ScBriefingModel *model, u32 index);
u32 ScBriefingNavigate(const struct ScBriefingModel *model, struct ScBriefingView *view, u16 keys);
bool32 ScBriefingCanAccept(const struct ScBriefingModel *model, u32 healthyCount, u32 badgeCount);
void ScShowBossBriefing(void);

#endif
