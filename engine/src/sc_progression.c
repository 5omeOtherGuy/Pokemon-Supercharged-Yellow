#include "global.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_setup.h"
#include "event_data.h"
#include "pokemon.h"
#include "sc_build_catalog.h"
#include "sc_progression.h"
#include "constants/opponents_frlg.h"

static EWRAM_DATA struct ScBattleParticipation sParticipation = {0};
static EWRAM_DATA u8 sOpponentParticipants[MAX_BATTLE_TRAINERS][PARTY_SIZE] = {0};
static EWRAM_DATA u8 sCreditedOpponents[MAX_BATTLE_TRAINERS] = {0};
static EWRAM_DATA u32 sPendingTraining[PARTY_SIZE] = {0};
static EWRAM_DATA u32 sPendingPractice = 0;
static EWRAM_DATA u16 sTrainerId = 0;
static EWRAM_DATA bool8 sRewarding = FALSE;
#if TESTING
static bool32 sEnabledForTests;
void ScProgressionEnableForTests(bool32 enabled)
{
    sEnabledForTests = enabled;
}
#endif

bool32 ScProgressionEnabled(void)
{
#if TESTING
    return P_SC_KANTO_RULES || sEnabledForTests;
#else
    return P_SC_KANTO_RULES;
#endif
}

u32 ScGetBadgeCount(void)
{
    const u16 flags[] = {FLAG_BADGE01_GET, FLAG_BADGE02_GET, FLAG_BADGE03_GET,
        FLAG_BADGE04_GET, FLAG_BADGE05_GET, FLAG_BADGE06_GET, FLAG_BADGE07_GET, FLAG_BADGE08_GET};
    u32 count = 0;
    for (u32 i = 0; i < ARRAY_COUNT(flags); i++)
        count += FlagGet(flags[i]) != 0;
    return count;
}

void ScReadMonProgress(struct Pokemon *mon, struct ScMonProgress *out)
{
    out->earnedCapabilities = GetMonData(mon, MON_DATA_SC_EARNED_CAPABILITIES);
    out->activeCapabilities = GetMonData(mon, MON_DATA_SC_ACTIVE_CAPABILITIES);
    out->focus = GetMonData(mon, MON_DATA_SC_FOCUS);
    out->trainingRemainder = GetMonData(mon, MON_DATA_SC_TRAINING_REMAINDER);
}

void ScWriteMonProgress(struct Pokemon *mon, const struct ScMonProgress *progress)
{
    SetMonData(mon, MON_DATA_SC_EARNED_CAPABILITIES, &progress->earnedCapabilities);
    SetMonData(mon, MON_DATA_SC_ACTIVE_CAPABILITIES, &progress->activeCapabilities);
    SetMonData(mon, MON_DATA_SC_FOCUS, &progress->focus);
    SetMonData(mon, MON_DATA_SC_TRAINING_REMAINDER, &progress->trainingRemainder);
}

void ScRefreshTrainerUnlocks(void)
{
    if (FlagGet(FLAG_BADGE01_GET))
        gSaveBlock3Ptr->sc.unlockedPassives |= 1u << SC_PASSIVE_COMPOSURE;
    if (HasTrainerBeenFought(TRAINER_RIVAL_SS_ANNE_SQUIRTLE)
        || HasTrainerBeenFought(TRAINER_RIVAL_SS_ANNE_BULBASAUR)
        || HasTrainerBeenFought(TRAINER_RIVAL_SS_ANNE_CHARMANDER))
        gSaveBlock3Ptr->sc.unlockedPassives |= 1u << SC_PASSIVE_TEMPO;
    if (FlagGet(FLAG_RESCUED_MR_FUJI))
        gSaveBlock3Ptr->sc.unlockedPassives |= 1u << SC_PASSIVE_PREPARATION;
    if (FlagGet(FLAG_HIDE_SILPH_ROCKETS))
        gSaveBlock3Ptr->sc.unlockedPassives |= 1u << SC_PASSIVE_FIELD_STUDY;
    if (FlagGet(FLAG_IS_CHAMPION))
        gSaveBlock3Ptr->sc.unlockedPassives |= 1u << SC_PASSIVE_RECOVERY_COACH;
}

void ScProgressionBeginBattle(void)
{
    u32 excluded = BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK | BATTLE_TYPE_FRONTIER
        | BATTLE_TYPE_SAFARI | BATTLE_TYPE_CATCH_TUTORIAL | BATTLE_TYPE_INGAME_PARTNER;
#if TESTING
    // The native battle harness transports scripted inputs as recorded battles.
    if (sEnabledForTests)
        excluded &= ~BATTLE_TYPE_RECORDED_LINK;
#endif
    sRewarding = ScProgressionEnabled() && !(gBattleTypeFlags & excluded);
    ScBeginParticipation(&sParticipation);
    memset(sOpponentParticipants, 0, sizeof(sOpponentParticipants));
    memset(sCreditedOpponents, 0, sizeof(sCreditedOpponents));
    memset(sPendingTraining, 0, sizeof(sPendingTraining));
    sPendingPractice = 0;
    sTrainerId = gBattleTypeFlags & BATTLE_TYPE_TRAINER ? TRAINER_BATTLE_PARAM.opponentA : TRAINER_NONE;
    if (sRewarding)
        ScRefreshTrainerUnlocks();
}

void ScProgressionEnterField(u32 battler)
{
    if (!sRewarding || battler >= gBattlersCount || gBattlerPartyIndexes[battler] >= PARTY_SIZE)
        return;
    if (GetBattlerTrainer(battler) == B_TRAINER_PLAYER)
        ScEnterField(&sParticipation, gBattlerPartyIndexes[battler]);
    // Retain per-opponent participation across both sides' switches. The vanilla
    // current-opponent EXP mask alone loses prior support when an enemy returns.
    for (u32 player = 0; player < gBattlersCount; player++)
    {
        if (GetBattlerTrainer(player) != B_TRAINER_PLAYER || !gBattleMons[player].hp)
            continue;
        for (u32 opponent = 0; opponent < gBattlersCount; opponent++)
        {
            if (GetBattlerSide(opponent) != B_SIDE_OPPONENT || !gBattleMons[opponent].hp)
                continue;
            if (player != battler && opponent != battler)
                continue;
            u32 slot = gBattlerPartyIndexes[opponent];
            if (slot < PARTY_SIZE && gBattlerPartyIndexes[player] < PARTY_SIZE)
                sOpponentParticipants[GetBattlerTrainer(opponent)][slot] |= 1u << gBattlerPartyIndexes[player];
        }
    }
}

void ScProgressionRecordFaint(u32 battler)
{
    if (sRewarding && battler < gBattlersCount && GetBattlerTrainer(battler) == B_TRAINER_PLAYER)
        ScFaintInBattle(&sParticipation, gBattlerPartyIndexes[battler]);
}

static u32 AddSaturated(u32 a, u32 b)
{
    return UINT32_MAX - a < b ? UINT32_MAX : a + b;
}

void ScProgressionRecordDefeat(u32 battler)
{
    if (!sRewarding || battler >= gBattlersCount || GetBattlerSide(battler) != B_SIDE_OPPONENT
        || gBattleMons[battler].hp != 0)
        return;
    u32 trainer = GetBattlerTrainer(battler), slot = gBattlerPartyIndexes[battler];
    if (slot >= PARTY_SIZE || (sCreditedOpponents[trainer] & (1u << slot)))
        return;
    sCreditedOpponents[trainer] |= 1u << slot;
    u32 participants = sOpponentParticipants[trainer][slot], count = 0;
    for (u32 i = 0; i < PARTY_SIZE; i++)
        count += (participants >> i) & 1u;
    if (!count)
        return;
    struct Pokemon *opponent = GetBattlerMon(battler);
    u32 species = GetMonData(opponent, MON_DATA_SPECIES);
    u32 value = gSpeciesInfo[species].expYield * GetMonData(opponent, MON_DATA_LEVEL) / 7;
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        value = value * 3 / 2;
    sPendingPractice = AddSaturated(sPendingPractice, value);
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        if (participants & (1u << i))
        {
            u32 fraction = ScTrainingFraction(value / count, GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL));
            if (gSaveBlock3Ptr->sc.activePassives & (1u << SC_PASSIVE_FIELD_STUDY))
                fraction = fraction / 10 * 11 + fraction % 10 * 11 / 10;
            sPendingTraining[i] = AddSaturated(sPendingTraining[i], fraction);
        }
    }
}

u32 ScProgressionFinishBattle(u32 outcome)
{
    if (!sRewarding || sParticipation.finished)
        return 0;
    u32 eligible = ScFinishParticipation(&sParticipation, outcome == B_OUTCOME_WON);
    if (outcome != B_OUTCOME_WON)
        return 0;
    u32 capability = ScGetEncounterCapability(sTrainerId), awarded = 0;
    u32 ceiling = ScTrainingCeiling(ScGetBadgeCount());
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        if (!(eligible & (1u << i)))
            continue;
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        struct ScMonProgress progress;
        u8 training[SC_STAT_COUNT];
        ScReadMonProgress(mon, &progress);
        for (u32 stat = 0; stat < SC_STAT_COUNT; stat++)
            training[stat] = GetMonData(mon, MON_DATA_HP_EV + stat);
        awarded += ScApplyTraining(&progress, training, ceiling, sPendingTraining[i]);
        awarded += ScEarnCapability(&progress, capability);
        for (u32 stat = 0; stat < SC_STAT_COUNT; stat++)
            SetMonData(mon, MON_DATA_HP_EV + stat, &training[stat]);
        ScWriteMonProgress(mon, &progress);
        CalculateMonStats(mon);
    }
    gSaveBlock3Ptr->sc.practiceExp = AddSaturated(gSaveBlock3Ptr->sc.practiceExp, sPendingPractice);
    if (sTrainerId == TRAINER_LEADER_BROCK)
        gSaveBlock3Ptr->sc.unlockedPassives |= 1u << SC_PASSIVE_COMPOSURE;
    if (sTrainerId >= TRAINER_RIVAL_SS_ANNE_SQUIRTLE && sTrainerId <= TRAINER_RIVAL_SS_ANNE_CHARMANDER)
        gSaveBlock3Ptr->sc.unlockedPassives |= 1u << SC_PASSIVE_TEMPO;
    if (sTrainerId == TRAINER_BOSS_GIOVANNI)
        gSaveBlock3Ptr->sc.unlockedPassives |= 1u << SC_PASSIVE_OFFENSE;
    if (sTrainerId == TRAINER_BOSS_GIOVANNI_2)
        gSaveBlock3Ptr->sc.unlockedPassives |= 1u << SC_PASSIVE_FIELD_STUDY;
    ScSealTrainerProgress(&gSaveBlock3Ptr->sc);
    return awarded;
}

u32 ScProgressionOrdinaryExp(u32 value)
{
    if (ScProgressionEnabled() && (gSaveBlock3Ptr->sc.activePassives & (1u << SC_PASSIVE_STUDY)))
        return value / 5 * 6 + value % 5 * 6 / 5;
    return value;
}
