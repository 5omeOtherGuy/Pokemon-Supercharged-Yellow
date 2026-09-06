#include "global.h"
#include "main.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "sc_debug.h"
#include "sc_progression.h"
#include "sc_build_catalog.h"
#include "data.h"
#include "item.h"
#include "money.h"
#include "event_data.h"
#include "pokedex.h"
#include "string_util.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/characters.h"

static bool32 CanEdit(void)
{
    return SC_TEST_TOOLS && !gMain.inBattle;
}

bool32 ScDebugOptionEnabled(u32 option)
{
    return SC_TEST_TOOLS && option < SC_DEBUG_OPTION_COUNT
        && (gSaveBlock3Ptr->sc.reserved[0] & (1u << option));
}

bool32 ScDebugSetOption(u32 option, bool32 enabled)
{
    if (!CanEdit() || option >= SC_DEBUG_OPTION_COUNT) return FALSE;
    if (enabled) gSaveBlock3Ptr->sc.reserved[0] |= 1u << option;
    else gSaveBlock3Ptr->sc.reserved[0] &= ~(1u << option);
    ScSealTrainerProgress(&gSaveBlock3Ptr->sc);
    return TRUE;
}

bool32 ScDebugValidMon(u32 party)
{
    if (!CanEdit() || party >= PARTY_SIZE) return FALSE;
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][party];
    u32 species = GetMonData(mon, MON_DATA_SPECIES);
    return species >= SPECIES_BULBASAUR && species <= SPECIES_MEW
        && !GetMonData(mon, MON_DATA_IS_EGG) && !GetMonData(mon, MON_DATA_SANITY_IS_BAD_EGG);
}

bool32 ScDebugEditMon(u32 party, u32 field, u32 slot, u32 value)
{
    static const u8 ivs[] = {MON_DATA_HP_IV, MON_DATA_ATK_IV, MON_DATA_DEF_IV, MON_DATA_SPEED_IV, MON_DATA_SPATK_IV, MON_DATA_SPDEF_IV};
    static const u8 evs[] = {MON_DATA_HP_EV, MON_DATA_ATK_EV, MON_DATA_DEF_EV, MON_DATA_SPEED_EV, MON_DATA_SPATK_EV, MON_DATA_SPDEF_EV};
    if (!ScDebugValidMon(party) || !ScDebugValidEdit(field, slot, value, MOVES_COUNT, ITEMS_COUNT)) return FALSE;
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][party];
    u32 species = GetMonData(mon, MON_DATA_SPECIES);
    u32 level = GetMonData(mon, MON_DATA_LEVEL);
    switch (field)
    {
    case SC_EDIT_SPECIES:
    {
        u32 ability = GetMonData(mon, MON_DATA_ABILITY_NUM);
        bool32 defaultName;
        u8 name[POKEMON_NAME_LENGTH + 1];
        GetMonData(mon, MON_DATA_NICKNAME, name);
        defaultName = StringCompare(name, GetSpeciesName(species)) == 0;
        SetMonData(mon, MON_DATA_SPECIES, &value);
        if (defaultName) SetMonData(mon, MON_DATA_NICKNAME, GetSpeciesName(value));
        if (ability >= ARRAY_COUNT(gSpeciesInfo[value].abilities) || !gSpeciesInfo[value].abilities[ability])
        { ability = 0; SetMonData(mon, MON_DATA_ABILITY_NUM, &ability); }
        value = gExperienceTables[gSpeciesInfo[value].growthRate][level];
        SetMonData(mon, MON_DATA_EXP, &value);
        break;
    }
    case SC_EDIT_LEVEL:
        value = gExperienceTables[gSpeciesInfo[species].growthRate][value];
        SetMonData(mon, MON_DATA_EXP, &value); break;
    case SC_EDIT_NATURE: case SC_EDIT_GENDER:
    {
        u32 gender = GetMonGender(mon), nature = GetNature(mon);
        if (field == SC_EDIT_NATURE) nature = value;
        else
        {
            static const u8 genders[] = {MON_MALE, MON_FEMALE, MON_GENDERLESS};
            gender = genders[value];
            u32 ratio = gSpeciesInfo[species].genderRatio;
            if ((ratio == MON_GENDERLESS && gender != MON_GENDERLESS)
             || (ratio == MON_MALE && gender != MON_MALE)
             || (ratio == MON_FEMALE && gender != MON_FEMALE)
             || (ratio != MON_GENDERLESS && gender == MON_GENDERLESS)) return FALSE;
        }
        // Personality determines encrypted block order: use the re-encrypting API.
        UpdateMonPersonality(&mon->box, GetMonPersonality(species, gender, nature, 0));
        break;
    }
    case SC_EDIT_IV: SetMonData(mon, ivs[slot], &value); break;
    case SC_EDIT_TRAINING: SetMonData(mon, evs[slot], &value); break;
    case SC_EDIT_ABILITY:
        if (!gSpeciesInfo[species].abilities[value]) return FALSE;
        SetMonData(mon, MON_DATA_ABILITY_NUM, &value); break;
    case SC_EDIT_MOVE:
        if (value == MOVE_NONE)
        {
            bool32 other = FALSE;
            for (u32 i = 0; i < MAX_MON_MOVES; i++)
                if (i != slot && GetMonData(mon, MON_DATA_MOVE1 + i)) other = TRUE;
            if (!other) return FALSE;
        }
        SetMonMoveSlot(mon, value, slot);
        { u32 bonuses = GetMonData(mon, MON_DATA_PP_BONUSES) & ~(3u << (2 * slot)); SetMonData(mon, MON_DATA_PP_BONUSES, &bonuses); }
        break;
    case SC_EDIT_PP:
        if (value > CalculatePPWithBonus(GetMonData(mon, MON_DATA_MOVE1 + slot), GetMonData(mon, MON_DATA_PP_BONUSES), slot)) return FALSE;
        SetMonData(mon, MON_DATA_PP1 + slot, &value); break;
    case SC_EDIT_ITEM: SetMonData(mon, MON_DATA_HELD_ITEM, &value); break;
    case SC_EDIT_SHINY: SetMonData(mon, MON_DATA_IS_SHINY, &value); break;
    case SC_EDIT_FRIENDSHIP: SetMonData(mon, MON_DATA_FRIENDSHIP, &value); break;
    default: return FALSE;
    }
    CalculateMonStats(mon);
    return TRUE;
}

bool32 ScDebugNickname(u32 party, const u8 *name)
{
    if (!ScDebugValidMon(party) || !name) return FALSE;
    u32 length;
    for (length = 0; length <= POKEMON_NAME_LENGTH && name[length] != EOS; length++);
    if (length == 0 || length > POKEMON_NAME_LENGTH) return FALSE;
    SetMonData(&gParties[B_TRAINER_PLAYER][party], MON_DATA_NICKNAME, name);
    return TRUE;
}

u32 ScDebugFreeSlots(void)
{
    u32 count = 0;
    for (u32 i = 0; i < PARTY_SIZE; i++)
        if (!GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SANITY_HAS_SPECIES)) count++;
    for (u32 box = 0; box < TOTAL_BOXES_COUNT; box++)
        for (u32 slot = 0; slot < IN_BOX_COUNT; slot++)
            if (!GetBoxMonDataAt(box, slot, MON_DATA_SANITY_HAS_SPECIES)) count++;
    return count;
}

bool32 ScDebugGiveMon(u32 species, u32 level)
{
    if (!CanEdit() || species < SPECIES_BULBASAUR || species > SPECIES_MEW || level < 1 || level > MAX_LEVEL || !ScDebugFreeSlots()) return FALSE;
    struct Pokemon mon;
    CreateMon(&mon, species, level, GetMonPersonality(species, MON_GENDER_RANDOM, NATURE_RANDOM, 0), OTID_STRUCT_PLAYER_ID);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        if (!GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SANITY_HAS_SPECIES))
        {
            gParties[B_TRAINER_PLAYER][i] = mon;
            CalculatePlayerPartyCount();
            goto received;
        }
    for (u32 box = 0; box < TOTAL_BOXES_COUNT; box++)
        for (u32 slot = 0; slot < IN_BOX_COUNT; slot++)
            if (!GetBoxMonDataAt(box, slot, MON_DATA_SANITY_HAS_SPECIES))
            { SetBoxMonAt(box, slot, &mon.box); goto received; }
    return FALSE;
received:
    FlagSet(FLAG_SYS_POKEMON_GET);
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_SEEN);
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_CAUGHT);
    return TRUE;
}

bool32 ScDebugGiveAll151(u32 level)
{
    if (!CanEdit() || level < 1 || level > MAX_LEVEL || ScDebugFreeSlots() < 151) return FALSE;
    for (u32 species = SPECIES_BULBASAUR; species <= SPECIES_MEW; species++)
        ScDebugGiveMon(species, level);
    return TRUE;
}

bool32 ScDebugGiveItem(u32 item, u32 quantity)
{
    if (!CanEdit() || item == ITEM_NONE || item >= ITEMS_COUNT || !quantity || quantity > 999) return FALSE;
    return AddBagItem(item, quantity);
}

u32 ScDebugRefillPack(bool32 travel)
{
    static const u16 supplies[] = {ITEM_RARE_CANDY, ITEM_MASTER_BALL, ITEM_ULTRA_BALL, ITEM_FULL_RESTORE, ITEM_MAX_REVIVE, ITEM_MAX_ELIXIR, ITEM_ESCAPE_ROPE, ITEM_MAX_REPEL, ITEM_LUM_BERRY, ITEM_SITRUS_BERRY};
    static const u16 keys[] = {ITEM_BICYCLE, ITEM_OLD_ROD, ITEM_GOOD_ROD, ITEM_SUPER_ROD, ITEM_VS_SEEKER, ITEM_TOWN_MAP, ITEM_SS_TICKET, ITEM_POKE_FLUTE, ITEM_CARD_KEY, ITEM_LIFT_KEY, ITEM_SECRET_KEY, ITEM_SILPH_SCOPE, ITEM_GOLD_TEETH, ITEM_HM01, ITEM_HM02, ITEM_HM03, ITEM_HM04, ITEM_HM05};
    const u16 *items = travel ? keys : supplies;
    u32 count = travel ? ARRAY_COUNT(keys) : ARRAY_COUNT(supplies), failures = 0;
    if (!CanEdit()) return count;
    for (u32 i = 0; i < count; i++)
    {
        u32 have = CountTotalItemQuantityInBag(items[i]), target = travel ? 1 : 99;
        if (have < target && !ScDebugGiveItem(items[i], target - have)) failures++;
    }
    return failures;
}

bool32 ScDebugSetBadges(u32 badges)
{
    if (!CanEdit() || badges > 8) return FALSE;
    for (u32 i = 0; i < 8; i++)
        if (i < badges) FlagSet(FLAG_BADGE01_GET + i); else FlagClear(FLAG_BADGE01_GET + i);
    return TRUE;
}

void ScDebugHeal(void)
{
    if (!CanEdit()) return;
    for (u32 i = 0; i < PARTY_SIZE; i++) if (ScDebugValidMon(i)) HealPokemon(&gParties[B_TRAINER_PLAYER][i]);
}

void ScDebugPrepareParty(void)
{
    if (!CanEdit()) return;
    for (u32 i = 0; i < PARTY_SIZE; i++)
        if (ScDebugValidMon(i))
        {
            ScDebugEditMon(i, SC_EDIT_LEVEL, 0, ScLevelCap(ScGetBadgeCount(), FlagGet(FLAG_SYS_GAME_CLEAR)));
            for (u32 stat = 0; stat < SC_STAT_COUNT; stat++)
            {
                ScDebugEditMon(i, SC_EDIT_IV, stat, 31);
                ScDebugEditMon(i, SC_EDIT_TRAINING, stat, ScTrainingCeiling(ScGetBadgeCount()));
            }
        }
    ScDebugHeal();
}

void ScDebugUnlockBuilds(void)
{
    if (!CanEdit()) return;
    gSaveBlock3Ptr->sc.unlockedPassives = (1u << SC_TRAINER_PASSIVE_COUNT) - 1;
    gSaveBlock3Ptr->sc.practiceExp = 9999999;
    ScSealTrainerProgress(&gSaveBlock3Ptr->sc);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        if (ScDebugValidMon(i))
        {
            u32 earned = (1u << SC_CAPABILITY_COUNT) - 1;
            SetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SC_EARNED_CAPABILITIES, &earned);
        }
}

void ScDebugRecharge(void)
{
    if (CanEdit()) gSaveBlock1Ptr->trainerRematchStepCounter = 100;
}

void ScDebugMaxMoney(void)
{
    if (CanEdit()) SetMoney(&gSaveBlock1Ptr->money, 999999);
}
