#ifndef GUARD_SC_DEBUG_CORE_H
#define GUARD_SC_DEBUG_CORE_H

/* Portable request validation; IDs are private to the test tools, never saved. */
enum ScDebugEdit
{
    SC_EDIT_SPECIES, SC_EDIT_LEVEL, SC_EDIT_NATURE, SC_EDIT_IV,
    SC_EDIT_TRAINING, SC_EDIT_ABILITY, SC_EDIT_MOVE, SC_EDIT_PP,
    SC_EDIT_ITEM, SC_EDIT_SHINY, SC_EDIT_GENDER, SC_EDIT_FRIENDSHIP,
    SC_EDIT_COUNT,
};

static inline unsigned ScDebugValidEdit(unsigned field, unsigned slot, unsigned value, unsigned moves, unsigned items)
{
    switch (field)
    {
    case SC_EDIT_SPECIES: return value >= 1 && value <= 151;
    case SC_EDIT_LEVEL: return value >= 1 && value <= 100;
    case SC_EDIT_NATURE: return value < 25;
    case SC_EDIT_IV: return slot < 6 && value <= 31;
    case SC_EDIT_TRAINING: return slot < 6 && value <= 255;
    case SC_EDIT_ABILITY: return value < 3;
    case SC_EDIT_MOVE: return slot < 4 && value < moves;
    case SC_EDIT_PP: return slot < 4 && value <= 99;
    case SC_EDIT_ITEM: return value < items;
    case SC_EDIT_SHINY: return value <= 1;
    case SC_EDIT_GENDER: return value <= 2;
    case SC_EDIT_FRIENDSHIP: return value <= 255;
    default: return 0;
    }
}

static inline unsigned ScDebugAdjust(unsigned value, unsigned minimum, unsigned maximum, int delta)
{
    if (delta < 0) return (unsigned)(-delta) > value - minimum ? minimum : value + delta;
    return (unsigned)delta > maximum - value ? maximum : value + delta;
}
#endif
