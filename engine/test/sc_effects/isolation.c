#include "global.h"
#include "battle.h"
#include "sc_ai.h"
#include "sc_battle.h"
#include "test/test.h"
#include "test_runner.h"

// This fixture is run serially. The engine's .tests section orders these
// definitions in reverse source order; the validation script checks that order.
TEST("SC isolation: teardown resets opt-ins after a failed test")
{
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    EXPECT(!ScAiEnabled());
    EXPECT(!ScEffectsEnabled());
}

TEST("SC isolation: an expected failure leaves both opt-ins enabled")
{
    ScAiEnableForTests(TRUE);
    ScEffectsEnableForTests(TRUE);
    EXPECT_FAIL { EXPECT(FALSE); }
}
