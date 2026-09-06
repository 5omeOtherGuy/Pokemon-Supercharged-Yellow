#include "global.h"
#include "battle.h"
#include "sc_ai.h"
#include "sc_battle.h"
#include "test/test.h"
#include "test_runner.h"

TEST("SC isolation: an expected failure leaves both opt-ins enabled")
{
    ScAiEnableForTests(TRUE);
    ScEffectsEnableForTests(TRUE);
    Test_ExpectedResult(TEST_RESULT_FAIL);
    EXPECT(FALSE);
}

TEST("SC isolation: teardown resets opt-ins after a failed test")
{
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    EXPECT(!ScAiEnabled());
    EXPECT(!ScEffectsEnabled());
}
