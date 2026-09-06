#include "global.h"
#include "sc_field.h"
#include "event_data.h"
#include "field_move.h"
#include "item.h"
#include "party_menu.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "constants/items.h"
#include "constants/moves.h"

bool32 ScFieldIsPermitMove(enum FieldMove fieldMove)
{
    return fieldMove == FIELD_MOVE_CUT || fieldMove == FIELD_MOVE_FLASH
        || fieldMove == FIELD_MOVE_FLY || fieldMove == FIELD_MOVE_STRENGTH
        || fieldMove == FIELD_MOVE_SURF;
}

bool32 ScFieldHasPermit(enum FieldMove fieldMove)
{
    u32 badge;
    enum Item hm;
    switch (fieldMove)
    {
    case FIELD_MOVE_FLASH: badge = FLAG_BADGE01_GET; hm = ITEM_HM05; break;
    case FIELD_MOVE_CUT: badge = FLAG_BADGE02_GET; hm = ITEM_HM01; break;
    case FIELD_MOVE_FLY: badge = FLAG_BADGE03_GET; hm = ITEM_HM02; break;
    case FIELD_MOVE_STRENGTH: badge = FLAG_BADGE04_GET; hm = ITEM_HM04; break;
    case FIELD_MOVE_SURF: badge = FLAG_BADGE05_GET; hm = ITEM_HM03; break;
    default: return FALSE;
    }
    return FlagGet(badge) && CheckBagHasItem(hm, 1);
}

static u32 ScFieldBoxMask(struct BoxPokemon *mon)
{
    enum Species species = GetBoxMonData(mon, MON_DATA_SPECIES);
    u32 mask = 0;
    if (species < SPECIES_BULBASAUR || species > SPECIES_MEW
        || GetBoxMonData(mon, MON_DATA_IS_EGG))
        return 0;
    for (u32 field = 0; field < FIELD_MOVES_COUNT; field++)
    {
        if (ScFieldHasPermit(field) && CanLearnTeachableMove(species, FieldMove_GetMoveId(field)))
            mask |= 1u << field;
    }
    return mask;
}

bool32 ScFieldCanUse(struct Pokemon *mon, enum FieldMove fieldMove)
{
    return fieldMove < FIELD_MOVES_COUNT && (ScFieldBoxMask(&mon->box) & (1u << fieldMove)) != 0;
}

u32 ScFieldFindUser(enum FieldMove fieldMove)
{
    for (u32 slot = 0; slot < PARTY_SIZE; slot++)
    {
        if (ScFieldCanUse(&gParties[B_TRAINER_PLAYER][slot], fieldMove))
            return slot;
    }
    return PARTY_SIZE;
}

bool32 ScFieldCanReplace(u32 slot, struct Pokemon *replacement)
{
    if (slot >= PARTY_SIZE)
        return FALSE;
    u32 needed = ScFieldBoxMask(&gParties[B_TRAINER_PLAYER][slot].box);
    u32 available = ScFieldBoxMask(&replacement->box);
    for (u32 other = 0; other < PARTY_SIZE && (needed & ~available); other++)
    {
        if (other != slot)
            available |= ScFieldBoxMask(&gParties[B_TRAINER_PLAYER][other].box);
    }
    return (needed & ~available) == 0;
}

bool32 ScFieldCanRelease(struct BoxPokemon *mon, s32 box, s32 position)
{
    u32 needed = ScFieldBoxMask(mon);
    for (u32 slot = 0; slot < PARTY_SIZE && needed; slot++)
    {
        if (box != TOTAL_BOXES_COUNT || position != slot)
            needed &= ~ScFieldBoxMask(&gParties[B_TRAINER_PLAYER][slot].box);
    }
    for (u32 otherBox = 0; otherBox < TOTAL_BOXES_COUNT && needed; otherBox++)
    {
        for (u32 slot = 0; slot < IN_BOX_COUNT && needed; slot++)
        {
            if (box != otherBox || position != slot)
                needed &= ~ScFieldBoxMask(GetBoxedMonPtr(otherBox, slot));
        }
    }
    return needed == 0;
}

u32 ScFieldBuildPage(struct Pokemon *mon, u32 page, u8 *actions)
{
    u8 fields[FIELD_MOVES_COUNT];
    u32 count = 0, length = 0;
    u32 permits = ScFieldBoxMask(&mon->box);
    enum Species species = GetMonData(mon, MON_DATA_SPECIES);
    if (species >= SPECIES_BULBASAUR && species <= SPECIES_MEW && !GetMonData(mon, MON_DATA_IS_EGG))
    {
        for (u32 field = 0; field < FIELD_MOVES_COUNT; field++)
        {
            bool32 ordinary = field == FIELD_MOVE_DIG || field == FIELD_MOVE_TELEPORT
                || field == FIELD_MOVE_SOFT_BOILED || field == FIELD_MOVE_SWEET_SCENT;
            if ((permits & (1u << field)) || (ordinary && MonKnowsMove(mon, FieldMove_GetMoveId(field))))
                fields[count++] = field;
        }
    }
    if (page >= (count + SC_FIELD_PAGE_SIZE - 1) / SC_FIELD_PAGE_SIZE)
        page = 0;
    for (u32 i = page * SC_FIELD_PAGE_SIZE; i < count && length < SC_FIELD_PAGE_SIZE; i++)
        actions[length++] = fields[i];
    if (count > SC_FIELD_PAGE_SIZE)
        actions[length++] = SC_FIELD_ACTION_NEXT;
    actions[length++] = SC_FIELD_ACTION_BACK;
    return length;
}
