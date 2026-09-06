#include "global.h"
#include "main.h"
#include "sc_pace.h"
#include "sc_progression.h"

u32 ScGetBattlePace(void)
{
    if (!ScProgressionEnabled())
        return 1;
    u32 pace = gSaveBlock3Ptr->sc.battleSpeed;
    return pace == 2 || pace == 4 ? pace : 1;
}

u32 ScBattleDelay(u32 frames, u32 minimum)
{
    if (!gMain.inBattle || !frames)
        return frames;
    u32 pace = ScGetBattlePace();
    u32 scaled = frames / pace + (frames % pace != 0);
    return min(frames, max(scaled, minimum));
}

u32 ScBattleBarStep(u32 step)
{
    if (!gMain.inBattle)
        return step;
    u32 pace = ScGetBattlePace();
    return step > UINT16_MAX / pace ? UINT16_MAX : step * pace;
}
