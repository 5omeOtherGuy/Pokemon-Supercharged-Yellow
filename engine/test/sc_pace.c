#include "global.h"
#include "main.h"
#include "sc_pace.h"
#include "sc_progression.h"
#include "test/test.h"

TEST("SC pace: battle pauses and bars accelerate without changing field timing")
{
    ScProgressionEnableForTests(TRUE);
    gMain.inBattle = TRUE;
    ScInitTrainerProgress(&gSaveBlock3Ptr->sc);
    EXPECT_EQ(ScBattleDelay(60, 0), 60);
    gSaveBlock3Ptr->sc.battleSpeed = 2;
    EXPECT_EQ(ScBattleDelay(60, 0), 30);
    EXPECT_EQ(ScBattleBarStep(3), 6);
    gSaveBlock3Ptr->sc.battleSpeed = 4;
    EXPECT_EQ(ScBattleDelay(60, 0), 15);
    EXPECT_EQ(ScBattleDelay(16, 12), 12);
    EXPECT_EQ(ScBattleDelay(0, 12), 0);
    EXPECT_EQ(ScBattleDelay(4, 12), 4); // Never lengthen a short upstream delay.
    EXPECT_EQ(ScBattleBarStep(20000), UINT16_MAX);
    gMain.inBattle = FALSE;
    EXPECT_EQ(ScBattleDelay(60, 0), 60);
    EXPECT_EQ(ScBattleBarStep(3), 3);
    gMain.inBattle = TRUE;
    gSaveBlock3Ptr->sc.battleSpeed = 255;
    EXPECT_EQ(ScGetBattlePace(), 1);
    EXPECT_EQ(ScBattleDelay(60, 0), 60);
    gMain.inBattle = FALSE;
}
