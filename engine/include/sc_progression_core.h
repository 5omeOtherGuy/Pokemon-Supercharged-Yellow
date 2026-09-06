#ifndef GUARD_SC_PROGRESSION_CORE_H
#define GUARD_SC_PROGRESSION_CORE_H

#include <stdint.h>

#define SC_STAT_COUNT 6
#define SC_FOCUS_BALANCED 6
#define SC_CAPABILITY_COUNT 12

/* Reuses the six contest-condition bytes without expanding an 80-byte BoxPokemon. */
struct ScMonProgress
{
    uint16_t earnedCapabilities;
    uint16_t activeCapabilities;
    uint8_t focus;
    uint8_t trainingRemainder;
};

struct ScBattleParticipation
{
    uint8_t entered;
    uint8_t fainted;
    uint8_t finished;
};

unsigned ScLevelCap(unsigned badges, unsigned champion);
unsigned ScTrainingCeiling(unsigned badges);
unsigned ScTrainerCapacity(unsigned badges);
void ScBeginParticipation(struct ScBattleParticipation *battle);
void ScEnterField(struct ScBattleParticipation *battle, unsigned slot);
void ScFaintInBattle(struct ScBattleParticipation *battle, unsigned slot);
unsigned ScFinishParticipation(struct ScBattleParticipation *battle, unsigned won);
unsigned ScSetTrainingFocus(struct ScMonProgress *mon, unsigned focus);
unsigned ScApplyTraining(struct ScMonProgress *mon, uint8_t training[SC_STAT_COUNT], unsigned ceiling, uint32_t fraction);
unsigned ScCapabilityCost(unsigned capability);
unsigned ScEarnCapability(struct ScMonProgress *mon, unsigned capability);
unsigned ScAssignCapabilities(struct ScMonProgress *mon, unsigned assignment);
uint32_t ScTrainingFraction(uint32_t expValue, unsigned recipientLevel);

#endif
