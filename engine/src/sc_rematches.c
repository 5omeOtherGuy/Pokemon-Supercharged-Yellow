#include "global.h"
#include "battle_setup.h"
#include "data.h"
#include "event_data.h"
#include "item.h"
#include "sc_progression.h"
#include "sc_rematches.h"
#include "constants/items.h"
#include "constants/maps.h"

bool32 ScRematchIsMainlandMap(u16 mapGroup, u16 mapNum)
{
#ifdef FIRERED
    // The selected table contains only mainland outdoor route maps.
    for (u32 i = 0; i < REMATCH_TABLE_ENTRIES; i++)
        if (gRematchTable[i].trainerIds[0] != TRAINER_NONE
         && gRematchTable[i].mapGroup == mapGroup && gRematchTable[i].mapNum == mapNum)
            return TRUE;
#endif
    return FALSE;
}

u16 ScRematchResolve(u16 trainerId)
{
#ifdef FIRERED
    s32 index;
    u32 size, cap;
    const struct TrainerMon *party;
    if (trainerId == TRAINER_NONE || trainerId >= TRAINERS_COUNT
     || !CheckBagHasItem(ITEM_VS_SEEKER, 1) || !HasTrainerBeenFought(trainerId))
        return TRAINER_NONE;
    index = FirstBattleTrainerIdToRematchTableId(gRematchTable, trainerId);
    if (index < 0 || index >= MAX_REMATCH_ENTRIES
     || gRematchTable[index].mapGroup != gSaveBlock1Ptr->location.mapGroup
     || gRematchTable[index].mapNum != gSaveBlock1Ptr->location.mapNum)
        return TRAINER_NONE;
    size = GetTrainerPartySizeFromId(trainerId);
    party = GetTrainerPartyFromId(trainerId);
    cap = ScLevelCap(ScGetBadgeCount(), FlagGet(FLAG_IS_CHAMPION));
    if (size == 0 || size > PARTY_SIZE || party == NULL)
        return TRAINER_NONE;
    for (u32 slot = 0; slot < size; slot++)
        if (party[slot].species < SPECIES_BULBASAUR || party[slot].species > SPECIES_MEW
         || party[slot].lvl == 0 || party[slot].lvl > cap)
            return TRAINER_NONE;
    return trainerId;
#else
    return TRAINER_NONE;
#endif
}

bool32 ScRematchIsReady(u16 trainerId)
{
#ifdef FIRERED
    if (!FlagGet(FLAG_SYS_VS_SEEKER_CHARGING) || ScRematchResolve(trainerId) == TRAINER_NONE)
        return FALSE;
    return gSaveBlock1Ptr->trainerRematches[FirstBattleTrainerIdToRematchTableId(gRematchTable, trainerId)] == 1;
#else
    return FALSE;
#endif
}

bool32 ScRematchSetReady(u16 trainerId)
{
    if (ScRematchResolve(trainerId) == TRAINER_NONE)
        return FALSE;
    gSaveBlock1Ptr->trainerRematches[FirstBattleTrainerIdToRematchTableId(gRematchTable, trainerId)] = 1;
    return TRUE;
}

void ScRematchClearReady(u16 trainerId)
{
    s32 index = FirstBattleTrainerIdToRematchTableId(gRematchTable, trainerId);
    if (trainerId != TRAINER_NONE && index >= 0 && index < MAX_REMATCH_ENTRIES)
        gSaveBlock1Ptr->trainerRematches[index] = 0;
}
