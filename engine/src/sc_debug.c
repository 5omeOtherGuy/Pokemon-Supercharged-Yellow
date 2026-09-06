#include "global.h"
#include "main.h"
#include "sc_debug.h"
#include "sc_progression.h"
#include "pokemon.h"
#include "data.h"
#include "item.h"
#include "move.h"
#include "bg.h"
#include "gpu_regs.h"
#include "palette.h"
#include "sprite.h"
#include "menu.h"
#include "menu_helpers.h"
#include "text.h"
#include "window.h"
#include "string_util.h"
#include "task.h"
#include "overworld.h"
#include "field_screen_effect.h"
#include "script.h"
#include "event_data.h"
#include "naming_screen.h"
#include "constants/heal_locations.h"
#include "constants/field_specials.h"

#if SC_TEST_TOOLS
// The test screen owns no saved data. Every mutation goes through the bounded
// model. Number adjustments are previews until A; B cancels without changing it.
enum { HOME, PARTY, EDIT, STATS, MOVES, OPTIONS, WARPS, NUMBER, NOTICE };
enum { ACTION_EDIT, ACTION_GIVE_SPECIES, ACTION_GIVE_LEVEL, ACTION_ITEM,
       ACTION_QUANTITY, ACTION_BADGES, ACTION_ALL_LEVEL };
static EWRAM_DATA struct
{
    const u8 *notice;
    u32 value, minimum, maximum;
    u16 cursor, previousCursor, choice;
    u8 page, previousPage, parentPage, parentCursor, party, field, slot, action;
    u8 nickname[POKEMON_NAME_LENGTH + 1];
    bool8 active;
} sUi = {0};
static EWRAM_DATA u16 sTilemap[1024] = {0};
static const u8 sColors[] = {TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};
static const struct BgTemplate sBg[] = {{.bg = 0, .charBaseIndex = 0, .mapBaseIndex = 31, .priority = 0}};
static const struct WindowTemplate sWindows[] =
{
    {.bg = 0, .tilemapLeft = 1, .tilemapTop = 1, .width = 28, .height = 18, .paletteNum = 15, .baseBlock = 1},
    DUMMY_WIN_TEMPLATE,
};
static const u8 *const sHome[] =
{
    COMPOUND_STRING("Edit party POKéMON"), COMPOUND_STRING("Give a POKéMON"),
    COMPOUND_STRING("Give all original 151"), COMPOUND_STRING("Give an item"),
    COMPOUND_STRING("Refill supplies (99 each)"), COMPOUND_STRING("Travel keys and HMs"),
    COMPOUND_STRING("Heal party + restore PP"), COMPOUND_STRING("Prepare party at badge cap"),
    COMPOUND_STRING("Unlock builds + Practice Points"), COMPOUND_STRING("Set badge count"),
    COMPOUND_STRING("Max money"), COMPOUND_STRING("Recharge VS SEEKER"),
    COMPOUND_STRING("Cheat switches"), COMPOUND_STRING("Warp in Kanto"), COMPOUND_STRING("Leave"),
};
static const u8 *const sEdits[] =
{
    COMPOUND_STRING("Species"), COMPOUND_STRING("Level (1-100)"), COMPOUND_STRING("Nature"),
    COMPOUND_STRING("Individual values (IVs)"), COMPOUND_STRING("Training per stat (0-255)"),
    COMPOUND_STRING("Innate ability slot"), COMPOUND_STRING("Moves"), COMPOUND_STRING("Current move PP"),
    COMPOUND_STRING("Held item"), COMPOUND_STRING("Shiny"), COMPOUND_STRING("Gender"),
    COMPOUND_STRING("Friendship"), COMPOUND_STRING("Nickname"),
};
static const u8 *const sStats[] = {COMPOUND_STRING("HP"), COMPOUND_STRING("Attack"), COMPOUND_STRING("Defense"), COMPOUND_STRING("Speed"), COMPOUND_STRING("Sp. Atk"), COMPOUND_STRING("Sp. Def")};
static const u8 sIvs[] = {MON_DATA_HP_IV, MON_DATA_ATK_IV, MON_DATA_DEF_IV, MON_DATA_SPEED_IV, MON_DATA_SPATK_IV, MON_DATA_SPDEF_IV};
static const u8 sEvs[] = {MON_DATA_HP_EV, MON_DATA_ATK_EV, MON_DATA_DEF_EV, MON_DATA_SPEED_EV, MON_DATA_SPATK_EV, MON_DATA_SPDEF_EV};
static const u8 *const sOptions[] =
{
    COMPOUND_STRING("Infinite owned items"), COMPOUND_STRING("Ignore level cap"),
    COMPOUND_STRING("No walking encounters"), COMPOUND_STRING("Free player battle supplies"),
    COMPOUND_STRING("Avoid trainer sight"),
};
static const u8 *const sWarpNames[] =
{
    COMPOUND_STRING("Pallet Town"), COMPOUND_STRING("Viridian City"), COMPOUND_STRING("Pewter City"),
    COMPOUND_STRING("Cerulean City"), COMPOUND_STRING("Vermilion City"), COMPOUND_STRING("Lavender Town"),
    COMPOUND_STRING("Celadon City"), COMPOUND_STRING("Fuchsia City"), COMPOUND_STRING("Saffron City"),
    COMPOUND_STRING("Cinnabar Island"), COMPOUND_STRING("Indigo Plateau"),
    COMPOUND_STRING("Route 4 CENTER"), COMPOUND_STRING("Route 10 CENTER"),
};
static const u8 sWarps[] =
{
    HEAL_LOCATION_PALLET_TOWN, HEAL_LOCATION_VIRIDIAN_CITY, HEAL_LOCATION_PEWTER_CITY,
    HEAL_LOCATION_CERULEAN_CITY, HEAL_LOCATION_VERMILION_CITY, HEAL_LOCATION_LAVENDER_TOWN,
    HEAL_LOCATION_CELADON_CITY, HEAL_LOCATION_FUCHSIA_CITY, HEAL_LOCATION_SAFFRON_CITY,
    HEAL_LOCATION_CINNABAR_ISLAND, HEAL_LOCATION_INDIGO_PLATEAU, HEAL_LOCATION_ROUTE4, HEAL_LOCATION_ROUTE10,
};

static void Init(void);
static struct Pokemon *Mon(void) { return &gParties[B_TRAINER_PLAYER][sUi.party]; }
static void Print(u32 x, u32 y, const u8 *text) { AddTextPrinterParameterized4(0, FONT_SMALL, x, y, 0, 0, sColors, TEXT_SKIP_DRAW, text); }
static void NumberAt(u32 x, u32 y, u32 value)
{
    u8 buffer[12];
    ConvertIntToDecimalStringN(buffer, value, STR_CONV_MODE_LEFT_ALIGN, 8);
    Print(x, y, buffer);
}
static void Go(u32 page) { sUi.page = page; sUi.cursor = 0; }
static void Notice(const u8 *text)
{
    sUi.previousPage = sUi.page; sUi.previousCursor = sUi.cursor;
    sUi.notice = text; Go(NOTICE);
}
static void Result(bool32 okay)
{
    Notice(okay ? COMPOUND_STRING("Done. Save normally to keep changes.")
        : COMPOUND_STRING("No change: invalid choice, absent\nability, impossible gender, invalid\nPP, or insufficient storage space."));
}
static void ReleaseScreen(void)
{
    SetVBlankCallback(NULL); FreeAllWindowBuffers(); UnsetBgTilemapBuffer(0);
}
static void Leave(void)
{
    sUi.active = FALSE; ReleaseScreen();
    SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}
static void EditNumber(u32 action, u32 minimum, u32 maximum, u32 value)
{
    sUi.parentPage = sUi.page; sUi.parentCursor = sUi.cursor;
    sUi.action = action; sUi.minimum = minimum; sUi.maximum = maximum;
    sUi.value = value < minimum ? minimum : value > maximum ? maximum : value;
    Go(NUMBER);
}
static void BeginField(void)
{
    u32 min = 0, max = 0, value = 0;
    switch (sUi.field)
    {
    case SC_EDIT_SPECIES: min = 1; max = 151; value = GetMonData(Mon(), MON_DATA_SPECIES); break;
    case SC_EDIT_LEVEL: min = 1; max = MAX_LEVEL; value = GetMonData(Mon(), MON_DATA_LEVEL); break;
    case SC_EDIT_NATURE: max = NUM_NATURES - 1; value = GetNature(Mon()); break;
    case SC_EDIT_IV: max = 31; value = GetMonData(Mon(), sIvs[sUi.slot]); break;
    case SC_EDIT_TRAINING: max = 255; value = GetMonData(Mon(), sEvs[sUi.slot]); break;
    case SC_EDIT_ABILITY: max = 2; value = GetMonData(Mon(), MON_DATA_ABILITY_NUM); break;
    case SC_EDIT_MOVE: max = MOVES_COUNT - 1; value = GetMonData(Mon(), MON_DATA_MOVE1 + sUi.slot); break;
    case SC_EDIT_PP:
        max = CalculatePPWithBonus(GetMonData(Mon(), MON_DATA_MOVE1 + sUi.slot), GetMonData(Mon(), MON_DATA_PP_BONUSES), sUi.slot);
        value = GetMonData(Mon(), MON_DATA_PP1 + sUi.slot); break;
    case SC_EDIT_ITEM: max = ITEMS_COUNT - 1; value = GetMonData(Mon(), MON_DATA_HELD_ITEM); break;
    case SC_EDIT_SHINY: max = 1; value = GetMonData(Mon(), MON_DATA_IS_SHINY); break;
    case SC_EDIT_GENDER: max = 2; value = GetMonGender(Mon()) == MON_MALE ? 0 : GetMonGender(Mon()) == MON_FEMALE ? 1 : 2; break;
    case SC_EDIT_FRIENDSHIP: max = 255; value = GetMonData(Mon(), MON_DATA_FRIENDSHIP); break;
    }
    EditNumber(ACTION_EDIT, min, max, value);
}
static u32 Rows(void)
{
    switch (sUi.page)
    {
    case HOME: return ARRAY_COUNT(sHome);
    case PARTY: return PARTY_SIZE;
    case EDIT: return ARRAY_COUNT(sEdits);
    case STATS: return SC_STAT_COUNT;
    case MOVES: return MAX_MON_MOVES;
    case OPTIONS: return ARRAY_COUNT(sOptions);
    case WARPS: return ARRAY_COUNT(sWarps);
    default: return 0;
    }
}
static const u8 *NumberName(void)
{
    if (sUi.action == ACTION_GIVE_SPECIES || (sUi.action == ACTION_EDIT && sUi.field == SC_EDIT_SPECIES)) return GetSpeciesName(sUi.value);
    if (sUi.action == ACTION_ITEM || (sUi.action == ACTION_EDIT && sUi.field == SC_EDIT_ITEM)) return GetItemName(sUi.value);
    if (sUi.action == ACTION_EDIT)
    {
        switch (sUi.field)
        {
        case SC_EDIT_MOVE: return sUi.value ? GetMoveName(sUi.value) : COMPOUND_STRING("Empty move slot");
        case SC_EDIT_NATURE: return gNaturesInfo[sUi.value].name;
        case SC_EDIT_ABILITY:
        {
            u32 ability = gSpeciesInfo[GetMonData(Mon(), MON_DATA_SPECIES)].abilities[sUi.value];
            return ability ? gAbilitiesInfo[ability].name : COMPOUND_STRING("No ability in this slot");
        }
        case SC_EDIT_GENDER: return sUi.value == 0 ? COMPOUND_STRING("Male") : sUi.value == 1 ? COMPOUND_STRING("Female") : COMPOUND_STRING("Genderless");
        case SC_EDIT_SHINY: return sUi.value ? COMPOUND_STRING("Shiny") : COMPOUND_STRING("Ordinary colors");
        default: break;
        }
    }
    return COMPOUND_STRING("");
}
static void Draw(void)
{
    FillWindowPixelBuffer(0, PIXEL_FILL(TEXT_COLOR_WHITE));
    Print(3, 0, COMPOUND_STRING("SUPERCHARGED YELLOW - TEST TOOLS"));
    if (sUi.page == NOTICE)
    {
        Print(3, 30, sUi.notice);
        Print(3, 130, COMPOUND_STRING("A or B back"));
    }
    else if (sUi.page == NUMBER)
    {
        const u8 *label = sUi.action == ACTION_EDIT ? sEdits[sUi.field]
            : sUi.action == ACTION_GIVE_SPECIES ? COMPOUND_STRING("Choose species")
            : sUi.action == ACTION_ITEM ? COMPOUND_STRING("Choose item")
            : sUi.action == ACTION_QUANTITY ? COMPOUND_STRING("Quantity to add")
            : sUi.action == ACTION_BADGES ? COMPOUND_STRING("Set first N badges; story stays as-is")
            : COMPOUND_STRING("Choose level (1-100)");
        Print(3, 22, label); NumberAt(3, 44, sUi.value); Print(55, 44, NumberName());
        if (sUi.action == ACTION_EDIT && sUi.field == SC_EDIT_TRAINING)
            Print(3, 65, COMPOUND_STRING("255 bypasses the normal badge ceiling."));
        if (sUi.action == ACTION_ALL_LEVEL)
            Print(3, 65, COMPOUND_STRING("Needs 151 empty party/box slots.\nExisting POKéMON are never replaced."));
        Print(3, 100, COMPOUND_STRING("UP/DOWN: 1    LEFT/RIGHT: 10\nL/R: 100"));
        Print(3, 130, COMPOUND_STRING("A apply  B cancel"));
    }
    else
    {
        if (sUi.page == EDIT || sUi.page == STATS || sUi.page == MOVES)
        { Print(3, 17, GetSpeciesName(GetMonData(Mon(), MON_DATA_SPECIES))); Print(140, 17, COMPOUND_STRING("Lv.")); NumberAt(158, 17, GetMonData(Mon(), MON_DATA_LEVEL)); }
        else if (sUi.page == HOME) Print(3, 17, COMPOUND_STRING("Owner shortcuts. Save normally after edits."));
        else if (sUi.page == WARPS) Print(3, 17, COMPOUND_STRING("Travel only; story events stay as-is."));
        else if (sUi.page == OPTIONS) Print(3, 17, COMPOUND_STRING("Persistent switches; default OFF."));
        else Print(3, 17, COMPOUND_STRING("Withdraw boxed POKéMON here to edit."));
        u32 first = sUi.cursor >= 6 ? sUi.cursor - 5 : 0;
        for (u32 i = first; i < Rows() && i < first + 6; i++)
        {
            u32 y = 35 + 14 * (i - first);
            const u8 *name = COMPOUND_STRING("Empty");
            switch (sUi.page)
            {
            case HOME: name = sHome[i]; break;
            case PARTY: if (ScDebugValidMon(i)) name = GetSpeciesName(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES)); break;
            case EDIT: name = sEdits[i]; break;
            case STATS: name = sStats[i]; NumberAt(160, y, GetMonData(Mon(), sUi.field == SC_EDIT_IV ? sIvs[i] : sEvs[i])); break;
            case MOVES: { u32 move = GetMonData(Mon(), MON_DATA_MOVE1 + i); name = move ? GetMoveName(move) : COMPOUND_STRING("Empty move slot"); break; }
            case OPTIONS: name = sOptions[i]; Print(197, y, ScDebugOptionEnabled(i) ? COMPOUND_STRING("ON") : COMPOUND_STRING("OFF")); break;
            case WARPS: name = sWarpNames[i]; break;
            }
            Print(10, y, name); if (i == sUi.cursor) Print(0, y, COMPOUND_STRING(">"));
        }
        Print(3, 130, COMPOUND_STRING("UP/DOWN choose  A select  B back"));
    }
    PutWindowTilemap(0); CopyWindowToVram(0, COPYWIN_FULL);
}
static void ReturnFromName(void)
{
    bool32 result = ScDebugNickname(sUi.party, sUi.nickname);
    Go(EDIT); Result(result); SetMainCallback2(Init);
}
static void Select(void)
{
    switch (sUi.page)
    {
    case HOME:
        switch (sUi.cursor)
        {
        case 0: Go(PARTY); break;
        case 1: EditNumber(ACTION_GIVE_SPECIES, 1, 151, SPECIES_PIKACHU); break;
        case 2: EditNumber(ACTION_ALL_LEVEL, 1, MAX_LEVEL, ScLevelCap(ScGetBadgeCount(), FlagGet(FLAG_SYS_GAME_CLEAR))); break;
        case 3: EditNumber(ACTION_ITEM, 1, ITEMS_COUNT - 1, ITEM_RARE_CANDY); break;
        case 4: case 5:
            if (ScDebugRefillPack(sUi.cursor == 5)) Notice(COMPOUND_STRING("Some items did not fit in the bag.\nOther items were added. Free space\nand select this pack again."));
            else Result(TRUE);
            break;
        case 6: ScDebugHeal(); Result(TRUE); break;
        case 7: ScDebugPrepareParty(); Notice(COMPOUND_STRING("Party set to current badge level cap.\nIVs 31; training at badge ceiling.\nParty healed and PP restored.\nMoves are unchanged.")); break;
        case 8: ScDebugUnlockBuilds(); Notice(COMPOUND_STRING("Party capabilities and trainer\npassives unlocked; 9,999,999 points.\nAssign builds in the CENTER lounge.\nBoxed POKéMON are unchanged.")); break;
        case 9: EditNumber(ACTION_BADGES, 0, 8, ScGetBadgeCount()); break;
        case 10: ScDebugMaxMoney(); Result(TRUE); break;
        case 11: ScDebugRecharge(); Result(TRUE); break;
        case 12: Go(OPTIONS); break;
        case 13: Go(WARPS); break;
        default: Leave(); break;
        }
        break;
    case PARTY:
        if (ScDebugValidMon(sUi.cursor)) { sUi.party = sUi.cursor; Go(EDIT); }
        break;
    case EDIT:
        if (sUi.cursor == SC_EDIT_COUNT)
        {
            GetMonData(Mon(), MON_DATA_NICKNAME, sUi.nickname);
            ReleaseScreen();
            DoNamingScreen(NAMING_SCREEN_NICKNAME, sUi.nickname, GetMonData(Mon(), MON_DATA_SPECIES), GetMonGender(Mon()), GetMonData(Mon(), MON_DATA_PERSONALITY), ReturnFromName);
        }
        else
        {
            sUi.field = sUi.cursor; sUi.slot = 0;
            if (sUi.field == SC_EDIT_IV || sUi.field == SC_EDIT_TRAINING) Go(STATS);
            else if (sUi.field == SC_EDIT_MOVE || sUi.field == SC_EDIT_PP) Go(MOVES);
            else BeginField();
        }
        break;
    case STATS: case MOVES: sUi.slot = sUi.cursor; BeginField(); break;
    case OPTIONS: ScDebugSetOption(sUi.cursor, !ScDebugOptionEnabled(sUi.cursor)); break;
    case WARPS: gSpecialVar_Result = sUi.cursor + 1; Leave(); break;
    case NUMBER:
    {
        u32 action = sUi.action, value = sUi.value;
        sUi.page = sUi.parentPage; sUi.cursor = sUi.parentCursor;
        switch (action)
        {
        case ACTION_EDIT: Result(ScDebugEditMon(sUi.party, sUi.field, sUi.slot, value)); break;
        case ACTION_GIVE_SPECIES: sUi.choice = value; EditNumber(ACTION_GIVE_LEVEL, 1, MAX_LEVEL, 5); break;
        case ACTION_ITEM: sUi.choice = value; EditNumber(ACTION_QUANTITY, 1, 999, 99); break;
        case ACTION_GIVE_LEVEL: Result(ScDebugGiveMon(sUi.choice, value)); break;
        case ACTION_QUANTITY:
            if (!ScDebugGiveItem(sUi.choice, value)) Notice(COMPOUND_STRING("The bag cannot hold that quantity.\nNothing was added. Try fewer items\nor free a slot in this pocket."));
            else Result(TRUE);
            break;
        case ACTION_ALL_LEVEL:
            if (!ScDebugGiveAll151(value)) Notice(COMPOUND_STRING("Need 151 free party/box slots.\nNothing was added. Existing\nPOKéMON will never be replaced."));
            else Result(TRUE);
            break;
        case ACTION_BADGES: Result(ScDebugSetBadges(value)); break;
        }
        break;
    }
    case NOTICE: sUi.page = sUi.previousPage; sUi.cursor = sUi.previousCursor; break;
    }
}
static void Back(void)
{
    switch (sUi.page)
    {
    case HOME: Leave(); break;
    case NOTICE: sUi.page = sUi.previousPage; sUi.cursor = sUi.previousCursor; break;
    case NUMBER: sUi.page = sUi.parentPage; sUi.cursor = sUi.parentCursor; break;
    case EDIT: Go(PARTY); break;
    case STATS: case MOVES: Go(EDIT); sUi.cursor = sUi.field; break;
    default: Go(HOME); break;
    }
}
static void Main(void)
{
    u32 keys = gMain.newKeys | (gMain.newAndRepeatedKeys & DPAD_ANY);
    if (keys & B_BUTTON) Back();
    else if (keys & A_BUTTON) Select();
    else if (sUi.page == NUMBER)
    {
        int delta = keys & DPAD_UP ? 1 : keys & DPAD_DOWN ? -1 : keys & DPAD_RIGHT ? 10 : keys & DPAD_LEFT ? -10 : keys & R_BUTTON ? 100 : keys & L_BUTTON ? -100 : 0;
        if (delta) sUi.value = ScDebugAdjust(sUi.value, sUi.minimum, sUi.maximum, delta);
    }
    else if (Rows() && (keys & (DPAD_UP | DPAD_DOWN)))
        sUi.cursor = (sUi.cursor + (keys & DPAD_UP ? Rows() - 1 : 1)) % Rows();
    if (!sUi.active || gMain.callback2 != Main) return;
    if (keys) Draw();
    RunTasks(); BuildOamBuffer(); DoScheduledBgTilemapCopiesToVram(); UpdatePaletteFade();
}
static void VBlank(void) { LoadOam(); ProcessSpriteCopyRequests(); TransferPlttBuffer(); }
static void Init(void)
{
    SetVBlankCallback(NULL); ResetTasks(); ResetSpriteData(); FreeAllSpritePalettes(); FreeAllWindowBuffers();
    ResetVramOamAndBgCntRegs(); ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, sBg, ARRAY_COUNT(sBg)); ResetAllBgsCoordinates();
    memset(sTilemap, 0, sizeof(sTilemap)); SetBgTilemapBuffer(0, sTilemap);
    if (!InitWindows(sWindows)) { Leave(); return; }
    ResetPaletteFade(); LoadMessageBoxAndBorderGfx(); SetStandardWindowBorderStyle(0, FALSE);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0); SetGpuReg(REG_OFFSET_BLDCNT, 0); ShowBg(0);
    Draw(); SetVBlankCallback(VBlank); SetMainCallback2(Main);
}
#endif

void ScShowTestTools(void)
{
    gSpecialVar_Result = 0;
#if SC_TEST_TOOLS
    if (!gMain.inBattle && !sUi.active)
    {
        memset(&sUi, 0, sizeof(sUi)); sUi.active = TRUE;
        Go(HOME); SetMainCallback2(Init);
    }
#endif
}

void ScDebugWarp(void)
{
#if SC_TEST_TOOLS
    if (!gMain.inBattle && gSpecialVar_Result >= 1 && gSpecialVar_Result <= ARRAY_COUNT(sWarps))
    {
        SetWarpDestinationToHealLocation(sWarps[gSpecialVar_Result - 1]);
        DoWarp(); ResetInitialPlayerAvatarState(); ScriptContext_Stop();
    }
#endif
}

void ScDebugPcLocation(void)
{
    gSpecialVar_0x8004 = PC_LOCATION_OTHER;
#if SC_TEST_TOOLS
    if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_PALLET_TOWN_PLAYERS_HOUSE_2F)
     && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_PALLET_TOWN_PLAYERS_HOUSE_2F))
        gSpecialVar_0x8004 = PC_LOCATION_PLAYER_HOUSE_FRLG;
#endif
}
