#include <assert.h>
#include <limits.h>
#include <string.h>
#include "sc_progression_core.h"

int main(void)
{
    const unsigned levels[] = {15, 22, 28, 36, 44, 50, 56, 62, 68};
    const unsigned ceilings[] = {24, 32, 40, 48, 56, 64, 72, 80, 96};
    struct ScMonProgress mon = {0};
    struct ScBattleParticipation battle;
    unsigned char training[6] = {0};
    unsigned char before[6];
    unsigned i;

    assert(sizeof(mon) == 6); /* Preserve the boxed Pokemon's existing six-byte field. */
    for (i = 0; i < 9; i++)
    {
        assert(ScLevelCap(i, 0) == levels[i]);
        assert(ScTrainingCeiling(i) == ceilings[i]);
        assert(ScTrainerCapacity(i) == i + 2);
    }
    assert(ScLevelCap(8, 1) == 75);
    assert(ScLevelCap(255, 0) == 68);
    assert(ScTrainingCeiling(255) == 96);
    assert(ScTrainerCapacity(255) == 10);

    ScBeginParticipation(&battle);
    ScEnterField(&battle, 0);
    ScEnterField(&battle, 2); /* switched-out support is eligible */
    ScEnterField(&battle, 3);
    ScFaintInBattle(&battle, 3);
    ScEnterField(&battle, 3); /* revival cannot erase faint history */
    ScEnterField(&battle, 9); /* invalid slots do not shift outside the party */
    assert(ScFinishParticipation(&battle, 1) == ((1 << 0) | (1 << 2)));
    assert(ScFinishParticipation(&battle, 1) == 0); /* one transaction */
    ScEnterField(&battle, 1);
    assert(ScFinishParticipation(&battle, 1) == 0);
    ScBeginParticipation(&battle);
    ScEnterField(&battle, 0);
    assert(ScFinishParticipation(&battle, 0) == 0);

    assert(ScSetTrainingFocus(&mon, 2));
    assert(ScApplyTraining(&mon, training, 24, 255) == 0);
    assert(mon.trainingRemainder == 255);
    assert(ScApplyTraining(&mon, training, 24, 1) == 1);
    assert(training[2] == 1 && mon.trainingRemainder == 0);
    assert(ScApplyTraining(&mon, training, 24, 30 * 256) == 23);
    assert(training[2] == 24 && mon.trainingRemainder == 0);
    memcpy(before, training, sizeof(training));
    assert(ScSetTrainingFocus(&mon, 3));
    assert(memcmp(before, training, sizeof(training)) == 0);
    assert(!ScSetTrainingFocus(&mon, 7));
    assert(mon.focus == 3);
    assert(ScApplyTraining(&mon, training, 24, 256) == 1);
    assert(training[3] == 1 && training[2] == 24);

    assert(ScSetTrainingFocus(&mon, SC_FOCUS_BALANCED));
    assert(ScApplyTraining(&mon, training, 24, 1000 * 256) == 119);
    for (i = 0; i < 6; i++) assert(training[i] == 24);
    assert(ScApplyTraining(&mon, training, 24, UINT_MAX) == 0);
    assert(mon.trainingRemainder == 0);
    /* A new badge raises room for every stat without resetting earlier points. */
    assert(ScApplyTraining(&mon, training, 32, 6 * 256) == 6);
    for (i = 0; i < 6; i++) assert(training[i] == 25);
    assert(ScApplyTraining(&mon, training, 0, 256) == 0);
    for (i = 0; i < 6; i++) assert(training[i] == 25);

    assert(!ScAssignCapabilities(&mon, 1)); /* must earn first */
    assert(!ScEarnCapability(&mon, 16));
    for (i = 0; i < SC_CAPABILITY_COUNT; i++) assert(ScEarnCapability(&mon, i));
    assert(!ScEarnCapability(&mon, 0)); /* duplicate source adds no capacity */
    assert(ScAssignCapabilities(&mon, (1 << 0) | (1 << 1) | (1 << 4))); /* 1+1+1 */
    assert(ScAssignCapabilities(&mon, (1 << 2) | (1 << 0))); /* 2+1 */
    assert(ScAssignCapabilities(&mon, 1 << 5)); /* 3 */
    assert(!ScAssignCapabilities(&mon, (1 << 5) | (1 << 0)));
    assert(mon.activeCapabilities == (1 << 5));
    assert(!ScAssignCapabilities(&mon, 1 << 15));
    assert(ScAssignCapabilities(&mon, 0));
    assert(mon.earnedCapabilities == (1 << SC_CAPABILITY_COUNT) - 1);
    assert(ScCapabilityCost(15) == 0);

    assert(ScTrainingFraction(64, 4) == 128);
    assert(ScTrainingFraction(128, 8) == 128);
    assert(ScTrainingFraction(0, 8) == 0);
    assert(ScTrainingFraction(1, 0) == 0);
    return 0;
}
