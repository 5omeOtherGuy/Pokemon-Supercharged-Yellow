#include "global.h"
#include "main.h"
#include "pokemon.h"
#include "sc_services.h"
#include "sc_progression.h"
#include "sc_build_catalog.h"
#include "constants/map_groups.h"

static const u32 sIvFields[] =
{
    MON_DATA_HP_IV, MON_DATA_ATK_IV, MON_DATA_DEF_IV,
    MON_DATA_SPEED_IV, MON_DATA_SPATK_IV, MON_DATA_SPDEF_IV,
};

bool32 ScServicesAtCenter(void)
{
    if (!ScProgressionEnabled() || gMain.inBattle)
        return FALSE;
    switch (gSaveBlock1Ptr->location.mapGroup | (gSaveBlock1Ptr->location.mapNum << 8))
    {
    case MAP_VIRIDIAN_CITY_POKEMON_CENTER_2F:
    case MAP_PEWTER_CITY_POKEMON_CENTER_2F:
    case MAP_CERULEAN_CITY_POKEMON_CENTER_2F:
    case MAP_VERMILION_CITY_POKEMON_CENTER_2F:
    case MAP_LAVENDER_TOWN_POKEMON_CENTER_2F:
    case MAP_CELADON_CITY_POKEMON_CENTER_2F:
    case MAP_FUCHSIA_CITY_POKEMON_CENTER_2F:
    case MAP_CINNABAR_ISLAND_POKEMON_CENTER_2F:
    case MAP_SAFFRON_CITY_POKEMON_CENTER_2F:
    case MAP_INDIGO_PLATEAU_POKEMON_CENTER_2F:
    case MAP_ROUTE4_POKEMON_CENTER_2F:
    case MAP_ROUTE10_POKEMON_CENTER_2F:
        return TRUE;
    }
    return FALSE;
}

bool32 ScServicesValidMon(u32 partyIndex)
{
    if (partyIndex >= PARTY_SIZE)
        return FALSE;
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][partyIndex];
    u32 species = GetMonData(mon, MON_DATA_SPECIES);
    return species >= SPECIES_BULBASAUR && species <= SPECIES_MEW
        && !GetMonData(mon, MON_DATA_IS_EGG);
}

static void RecalculatePreservingDamage(struct Pokemon *mon)
{
    u32 hp = GetMonData(mon, MON_DATA_HP);
    u32 maxHp = GetMonData(mon, MON_DATA_MAX_HP);
    u32 deficit = maxHp > hp ? maxHp - hp : 0;
    CalculateMonStats(mon);
    maxHp = GetMonData(mon, MON_DATA_MAX_HP);
    hp = hp == 0 || deficit >= maxHp ? 0 : maxHp - deficit;
    SetMonData(mon, MON_DATA_HP, &hp);
}

u32 ScServicesSetCapabilities(u32 partyIndex, u32 mask)
{
    struct ScMonProgress progress;
    if (!ScServicesAtCenter())
        return SC_SERVICE_BAD_CONTEXT;
    if (!ScServicesValidMon(partyIndex))
        return SC_SERVICE_BAD_MON;
    if (mask >> SC_CAPABILITY_COUNT)
        return SC_SERVICE_INVALID;
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][partyIndex];
    ScReadMonProgress(mon, &progress);
    if (mask & ~progress.earnedCapabilities)
        return SC_SERVICE_NOT_EARNED;
    if (mask == progress.activeCapabilities)
        return SC_SERVICE_NO_CHANGE;
    if (!ScAssignCapabilities(&progress, mask))
        return SC_SERVICE_OVER_BUDGET;
    ScWriteMonProgress(mon, &progress);
    RecalculatePreservingDamage(mon);
    return SC_SERVICE_OK;
}

u32 ScServicesSetTrainerPassives(u32 mask)
{
    u32 cost = 0;
    if (!ScServicesAtCenter())
        return SC_SERVICE_BAD_CONTEXT;
    if (mask >> SC_TRAINER_PASSIVE_COUNT)
        return SC_SERVICE_INVALID;
    if (mask & ~gSaveBlock3Ptr->sc.unlockedPassives)
        return SC_SERVICE_NOT_EARNED;
    if (mask == gSaveBlock3Ptr->sc.activePassives)
        return SC_SERVICE_NO_CHANGE;
    for (u32 i = 0; i < SC_TRAINER_PASSIVE_COUNT; i++)
        if (mask & (1u << i))
            cost += gScTrainerPassiveInfo[i].cost;
    if (cost > ScTrainerCapacity(ScGetBadgeCount()))
        return SC_SERVICE_OVER_BUDGET;
    gSaveBlock3Ptr->sc.activePassives = mask;
    ScSealTrainerProgress(&gSaveBlock3Ptr->sc);
    return SC_SERVICE_OK;
}

u32 ScServicesSetFocus(u32 partyIndex, u32 focus)
{
    struct ScMonProgress progress;
    if (!ScProgressionEnabled() || gMain.inBattle)
        return SC_SERVICE_BAD_CONTEXT;
    if (!ScServicesValidMon(partyIndex))
        return SC_SERVICE_BAD_MON;
    if (focus > SC_FOCUS_BALANCED)
        return SC_SERVICE_INVALID;
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][partyIndex];
    ScReadMonProgress(mon, &progress);
    if (progress.focus == focus)
        return SC_SERVICE_NO_CHANGE;
    ScSetTrainingFocus(&progress, focus);
    ScWriteMonProgress(mon, &progress);
    return SC_SERVICE_OK;
}

u32 ScServicesSetAbility(u32 partyIndex, u32 abilitySlot)
{
    if (!ScServicesAtCenter())
        return SC_SERVICE_BAD_CONTEXT;
    if (!ScServicesValidMon(partyIndex))
        return SC_SERVICE_BAD_MON;
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][partyIndex];
    u32 species = GetMonData(mon, MON_DATA_SPECIES);
    if (abilitySlot >= ARRAY_COUNT(gSpeciesInfo[species].abilities)
        || gSpeciesInfo[species].abilities[abilitySlot] == ABILITY_NONE)
        return SC_SERVICE_INVALID;
    // Absent and duplicate official slots are never separate choices.
    for (u32 i = 0; i < abilitySlot; i++)
        if (gSpeciesInfo[species].abilities[i] == gSpeciesInfo[species].abilities[abilitySlot])
            return SC_SERVICE_INVALID;
    if (GetMonData(mon, MON_DATA_ABILITY_NUM) == abilitySlot)
        return SC_SERVICE_NO_CHANGE;
    SetMonData(mon, MON_DATA_ABILITY_NUM, &abilitySlot);
    RecalculatePreservingDamage(mon);
    return SC_SERVICE_OK;
}

u32 ScServicesIvCost(u32 level)
{
    return level < 5 ? 20 : 4 * level;
}

u32 ScServicesImproveIv(u32 partyIndex, u32 stat)
{
    if (!ScServicesAtCenter())
        return SC_SERVICE_BAD_CONTEXT;
    if (!ScServicesValidMon(partyIndex))
        return SC_SERVICE_BAD_MON;
    if (stat >= ARRAY_COUNT(sIvFields))
        return SC_SERVICE_INVALID;
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][partyIndex];
    u32 iv = GetMonData(mon, sIvFields[stat]);
    u32 level = GetMonData(mon, MON_DATA_LEVEL);
    if (level == 0 || level > MAX_LEVEL)
        return SC_SERVICE_INVALID;
    if (iv >= 31)
        return SC_SERVICE_MAX_IV;
    u32 cost = ScServicesIvCost(level);
    if (gSaveBlock3Ptr->sc.practiceExp < cost)
        return SC_SERVICE_INSUFFICIENT_PRACTICE;
    iv++;
    SetMonData(mon, sIvFields[stat], &iv);
    RecalculatePreservingDamage(mon);
    gSaveBlock3Ptr->sc.practiceExp -= cost;
    ScSealTrainerProgress(&gSaveBlock3Ptr->sc);
    return SC_SERVICE_OK;
}

u32 ScServicesRelearnMoves(u32 partyIndex, u16 *moves, u32 capacity)
{
    u32 count = 0;
    if (!ScServicesValidMon(partyIndex) || !moves || !capacity)
        return 0;
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][partyIndex];
    const struct LevelUpMove *learnset = GetSpeciesLevelUpLearnset(GetMonData(mon, MON_DATA_SPECIES));
    u32 level = GetMonData(mon, MON_DATA_LEVEL);
    for (u32 i = 0; learnset[i].move != LEVEL_UP_MOVE_END; i++)
    {
        bool32 duplicate = FALSE;
        u32 move = learnset[i].move;
        if (learnset[i].level > level || move == MOVE_NONE || move >= MOVES_COUNT_ALL)
            continue;
        for (u32 j = 0; j < MAX_MON_MOVES; j++)
            if (GetMonData(mon, MON_DATA_MOVE1 + j) == move)
                duplicate = TRUE;
        for (u32 j = 0; j < count; j++)
            if (moves[j] == move)
                duplicate = TRUE;
        if (!duplicate && count < capacity)
            moves[count++] = move;
    }
    return count;
}

u32 ScServicesTeachMove(u32 partyIndex, u32 move, u32 moveSlot)
{
    u16 moves[SC_SERVICE_MAX_MOVES];
    if (!ScServicesAtCenter())
        return SC_SERVICE_BAD_CONTEXT;
    if (!ScServicesValidMon(partyIndex))
        return SC_SERVICE_BAD_MON;
    if (moveSlot >= MAX_MON_MOVES || move == MOVE_NONE || move >= MOVES_COUNT_ALL)
        return SC_SERVICE_INVALID;
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][partyIndex];
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        if (GetMonData(mon, MON_DATA_MOVE1 + i) == move)
            return SC_SERVICE_NO_CHANGE;
    u32 count = ScServicesRelearnMoves(partyIndex, moves, ARRAY_COUNT(moves));
    for (u32 i = 0; i < count; i++)
        if (moves[i] == move)
        {
            RemoveMonPPBonus(mon, moveSlot);
            SetMonMoveSlot(mon, move, moveSlot);
            return SC_SERVICE_OK;
        }
    return SC_SERVICE_INVALID;
}
