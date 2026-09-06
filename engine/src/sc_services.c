#include "global.h"
#include "main.h"
#include "data.h"
#include "pokemon.h"
#include "sc_services.h"
#include "sc_progression.h"
#include "sc_build_catalog.h"
#include "sc_supplies.h"
#include "bg.h"
#include "gpu_regs.h"
#include "palette.h"
#include "sprite.h"
#include "menu.h"
#include "menu_helpers.h"
#include "text.h"
#include "string_util.h"
#include "window.h"
#include "overworld.h"
#include "script.h"
#include "task.h"
#include "item.h"
#include "move.h"
#include "event_data.h"
#include "constants/characters.h"

// One independent native screen for both callers. Center-only mutations also
// validate the actual map at commitment in sc_services_model.c.
enum ServicePage
{
    PAGE_HOME, PAGE_PARTY, PAGE_MON, PAGE_TRAINING, PAGE_FOCUS,
    PAGE_CAPABILITIES, PAGE_TRAINER, PAGE_EFFECT, PAGE_ABILITIES,
    PAGE_ABILITY_DETAIL, PAGE_IVS, PAGE_IV_CONFIRM, PAGE_MOVES,
    PAGE_MOVE_DETAIL, PAGE_REPLACE_MOVE, PAGE_SUPPLIES, PAGE_SUPPLY_PICK,
    PAGE_HELP, PAGE_NOTICE,
};

static EWRAM_DATA struct
{
    void (*returnCallback)(void);
    const u8 *notice;
    u16 choices[ITEMS_COUNT];
    u16 supplyUnits[SC_SUPPLY_SLOTS];
    u16 count, cursor, selection, returnCursor;
    u8 partyIndex, page, returnPage, effectKind, scroll, supplySlot;
    bool8 active, center;
} sUi = {0};
static EWRAM_DATA u16 sBgTilemap[1024] = {0};
static EWRAM_DATA u8 sLines[32][128] = {0};
static const u8 sColors[] = {TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};
static const struct BgTemplate sBg[] = {{.bg = 0, .charBaseIndex = 0, .mapBaseIndex = 31, .priority = 0}};
static const struct WindowTemplate sWindows[] =
{
    {.bg = 0, .tilemapLeft = 1, .tilemapTop = 1, .width = 28, .height = 18, .paletteNum = 15, .baseBlock = 1},
    DUMMY_WIN_TEMPLATE,
};
static const u8 *const sStatNames[] =
{
    COMPOUND_STRING("HP"), COMPOUND_STRING("Attack"), COMPOUND_STRING("Defense"),
    COMPOUND_STRING("Speed"), COMPOUND_STRING("Sp. Atk"), COMPOUND_STRING("Sp. Def"),
    COMPOUND_STRING("Balanced"),
};
static const u32 sEvFields[] = {MON_DATA_HP_EV, MON_DATA_ATK_EV, MON_DATA_DEF_EV, MON_DATA_SPEED_EV, MON_DATA_SPATK_EV, MON_DATA_SPDEF_EV};
static const u32 sIvFields[] = {MON_DATA_HP_IV, MON_DATA_ATK_IV, MON_DATA_DEF_IV, MON_DATA_SPEED_IV, MON_DATA_SPATK_IV, MON_DATA_SPDEF_IV};

static void CB2_InitServices(void);
static void Draw(void);

static struct Pokemon *CurrentMon(void)
{
    return &gParties[B_TRAINER_PLAYER][sUi.partyIndex];
}

static void Print(u32 x, u32 y, const u8 *text)
{
    AddTextPrinterParameterized4(0, FONT_SMALL, x, y, 0, 0, sColors, TEXT_SKIP_DRAW, text);
}

static void Number(u32 x, u32 y, u32 value)
{
    u8 buffer[16];
    ConvertIntToDecimalStringN(buffer, value, STR_CONV_MODE_LEFT_ALIGN, 10);
    Print(x, y, buffer);
}

static void Footer(const u8 *text)
{
    Print(3, 130, text);
}

static void Go(u32 page)
{
    sUi.page = page;
    sUi.cursor = sUi.scroll = 0;
}

static void Notice(const u8 *text)
{
    sUi.returnPage = sUi.page;
    sUi.returnCursor = sUi.cursor;
    sUi.notice = text;
    Go(PAGE_NOTICE);
}

static void Result(u32 result)
{
    static const u8 *const messages[] =
    {
        COMPOUND_STRING("Done. Your choice is now active."),
        COMPOUND_STRING("This change requires the development\nlounge upstairs in a mainland\nPOKéMON CENTER, outside battle."),
        COMPOUND_STRING("Choose a non-egg Kanto POKéMON\nfrom your current party."),
        COMPOUND_STRING("That is not a legal choice."),
        COMPOUND_STRING("This effect has not been earned.\nOwnership is required before\nassignment."),
        COMPOUND_STRING("That combination exceeds your\navailable slots. Remove an active\neffect before assigning this one."),
        COMPOUND_STRING("That choice is already active."),
        COMPOUND_STRING("You need more Practice Points.\nWin battles to earn this shared\ntrainer resource."),
        COMPOUND_STRING("This IV is already at its maximum\nof 31. No points were spent."),
    };
    Notice(messages[result < ARRAY_COUNT(messages) ? result : SC_SERVICE_INVALID]);
}

static u32 Wrap(const u8 *source)
{
    u32 line = 0, length = 0, space = 0;
    memset(sLines, EOS, sizeof(sLines));
    while (*source != EOS && line < ARRAY_COUNT(sLines))
    {
        u8 ch = *source++;
        if (ch == CHAR_NEWLINE) { line++; length = space = 0; continue; }
        sLines[line][length++] = ch;
        sLines[line][length] = EOS;
        if (ch == CHAR_SPACE) space = length;
        if (GetStringWidth(FONT_SMALL, sLines[line], 0) > 214 || length >= 120)
        {
            u32 split = space ? space : length - 1;
            if (line + 1 >= ARRAY_COUNT(sLines)) break;
            memcpy(sLines[line + 1], &sLines[line][split], length - split);
            sLines[line + 1][length - split] = EOS;
            sLines[line][space ? split - 1 : split] = EOS;
            length -= split; space = 0; line++;
        }
    }
    return line < ARRAY_COUNT(sLines) ? line + 1 : ARRAY_COUNT(sLines);
}

static void Description(const u8 *text, u32 firstY, u32 visible)
{
    u32 lines = Wrap(text), maximum = lines > visible ? lines - visible : 0;
    if (sUi.scroll > maximum) sUi.scroll = maximum;
    for (u32 i = 0; i < visible && i + sUi.scroll < lines; i++)
        Print(3, firstY + 12 * i, sLines[i + sUi.scroll]);
}

static void MonTitle(void)
{
    Print(3, 0, GetSpeciesName(GetMonData(CurrentMon(), MON_DATA_SPECIES)));
    Print(170, 0, COMPOUND_STRING("Lv."));
    Number(185, 0, GetMonData(CurrentMon(), MON_DATA_LEVEL));
}

static u32 MaskCost(u32 mask, bool32 trainer)
{
    const struct ScPassiveInfo *catalog = trainer ? gScTrainerPassiveInfo : gScCapabilityInfo;
    u32 cost = 0, count = trainer ? SC_TRAINER_PASSIVE_COUNT : SC_CAPABILITY_COUNT;
    for (u32 i = 0; i < count; i++) if (mask & (1u << i)) cost += catalog[i].cost;
    return cost;
}

static u32 ActiveMask(bool32 trainer)
{
    return trainer ? gSaveBlock3Ptr->sc.activePassives : GetMonData(CurrentMon(), MON_DATA_SC_ACTIVE_CAPABILITIES);
}

static u32 OwnedMask(bool32 trainer)
{
    return trainer ? gSaveBlock3Ptr->sc.unlockedPassives : GetMonData(CurrentMon(), MON_DATA_SC_EARNED_CAPABILITIES);
}

static void LoadParty(void)
{
    sUi.count = 0;
    for (u32 i = 0; i < PARTY_SIZE; i++)
        if (ScServicesValidMon(i)) sUi.choices[sUi.count++] = i;
}

static void LoadAbilities(void)
{
    u32 species = GetMonData(CurrentMon(), MON_DATA_SPECIES);
    sUi.count = 0;
    for (u32 i = 0; i < ARRAY_COUNT(gSpeciesInfo[species].abilities); i++)
    {
        u32 ability = gSpeciesInfo[species].abilities[i];
        bool32 duplicate = ability == ABILITY_NONE;
        for (u32 j = 0; j < i; j++) if (gSpeciesInfo[species].abilities[j] == ability) duplicate = TRUE;
        if (!duplicate) sUi.choices[sUi.count++] = i;
    }
}

static void LoadSupplyUnits(void)
{
    u32 count = 0;
    memset(sUi.supplyUnits, 0, sizeof(sUi.supplyUnits));
    for (u32 i = 0; i < SC_SUPPLY_SLOTS; i++)
        for (u32 n = 0; n < gSaveBlock3Ptr->sc.supplyQuantities[i] && count < SC_SUPPLY_SLOTS; n++)
            sUi.supplyUnits[count++] = gSaveBlock3Ptr->sc.supplyItems[i];
}

static void SetSupply(u32 slot, u32 item)
{
    u16 units[SC_SUPPLY_SLOTS], items[SC_SUPPLY_SLOTS] = {0};
    u8 quantities[SC_SUPPLY_SLOTS] = {0};
    memcpy(units, sUi.supplyUnits, sizeof(units));
    if (slot < SC_SUPPLY_SLOTS) units[slot] = item;
    else memset(units, 0, sizeof(units));
    for (u32 i = 0; i < SC_SUPPLY_SLOTS; i++)
        if (units[i] != ITEM_NONE)
            for (u32 j = 0; j < SC_SUPPLY_SLOTS; j++)
                if (items[j] == ITEM_NONE || items[j] == units[i])
                {
                    items[j] = units[i]; quantities[j]++; break;
                }
    if (ScServicesAtCenter() && ScSuppliesSetSelection(items, quantities))
    {
        LoadSupplyUnits();
        Go(PAGE_SUPPLIES);
    }
    else
        Notice(COMPOUND_STRING("Choose up to 3 owned units:\nHP heals: 2; status, PP or boost: 1\neach. Revives and mixed items\nare not allowed. Nothing changed."));
}

static u32 RowCount(void)
{
    switch (sUi.page)
    {
    case PAGE_HOME: return 5;
    case PAGE_PARTY: return sUi.count;
    case PAGE_MON: return sUi.center ? 5 : 4;
    case PAGE_FOCUS: return 7;
    case PAGE_CAPABILITIES: return SC_CAPABILITY_COUNT;
    case PAGE_TRAINER: return SC_TRAINER_PASSIVE_COUNT;
    case PAGE_ABILITIES: case PAGE_MOVES: case PAGE_SUPPLY_PICK: return sUi.count;
    case PAGE_IVS: return SC_STAT_COUNT;
    case PAGE_IV_CONFIRM: return 2;
    case PAGE_REPLACE_MOVE: return MAX_MON_MOVES;
    case PAGE_SUPPLIES: return SC_SUPPLY_SLOTS;
    default: return 0;
    }
}

static void DrawListRow(u32 index, u32 y)
{
    const u8 *name = COMPOUND_STRING("Empty");
    bool32 trainer = sUi.page == PAGE_TRAINER;
    switch (sUi.page)
    {
    case PAGE_HOME:
    {
        static const u8 *const names[] = {COMPOUND_STRING("Develop a POKéMON"), COMPOUND_STRING("Trainer build"), COMPOUND_STRING("Battle supplies"), COMPOUND_STRING("How development works"), COMPOUND_STRING("Leave")};
        name = names[index]; break;
    }
    case PAGE_PARTY: name = GetSpeciesName(GetMonData(&gParties[B_TRAINER_PLAYER][sUi.choices[index]], MON_DATA_SPECIES)); break;
    case PAGE_MON:
    {
        static const u8 *const center[] = {COMPOUND_STRING("Training and focus"), COMPOUND_STRING("Earned capabilities"), COMPOUND_STRING("Choose innate ability"), COMPOUND_STRING("Relearn a move"), COMPOUND_STRING("Improve an IV")};
        static const u8 *const field[] = {COMPOUND_STRING("Training and focus"), COMPOUND_STRING("Inspect capabilities"), COMPOUND_STRING("Inspect trainer build"), COMPOUND_STRING("Inspect innate abilities")};
        name = sUi.center ? center[index] : field[index]; break;
    }
    case PAGE_FOCUS:
        name = sStatNames[index];
        if (GetMonData(CurrentMon(), MON_DATA_SC_FOCUS) == index) Print(170, y, COMPOUND_STRING("Current"));
        break;
    case PAGE_CAPABILITIES: case PAGE_TRAINER:
    {
        const struct ScPassiveInfo *catalog = trainer ? gScTrainerPassiveInfo : gScCapabilityInfo;
        name = catalog[index].name;
        Number(137, y, catalog[index].cost);
        Print(160, y, ActiveMask(trainer) & (1u << index) ? COMPOUND_STRING("Active") : OwnedMask(trainer) & (1u << index) ? COMPOUND_STRING("Owned") : COMPOUND_STRING("Locked"));
        break;
    }
    case PAGE_ABILITIES:
    {
        u32 slot = sUi.choices[index], species = GetMonData(CurrentMon(), MON_DATA_SPECIES);
        name = gAbilitiesInfo[gSpeciesInfo[species].abilities[slot]].name;
        if (slot == GetMonData(CurrentMon(), MON_DATA_ABILITY_NUM)) Print(170, y, COMPOUND_STRING("Current"));
        break;
    }
    case PAGE_IVS:
        name = sStatNames[index]; Number(160, y, GetMonData(CurrentMon(), sIvFields[index])); Print(180, y, COMPOUND_STRING("/31")); break;
    case PAGE_IV_CONFIRM: name = index ? COMPOUND_STRING("No, keep my points") : COMPOUND_STRING("Yes, improve this IV"); break;
    case PAGE_MOVES: name = GetMoveName(sUi.choices[index]); break;
    case PAGE_REPLACE_MOVE:
    {
        u32 move = GetMonData(CurrentMon(), MON_DATA_MOVE1 + index);
        name = move == MOVE_NONE ? COMPOUND_STRING("Empty move slot") : GetMoveName(move); break;
    }
    case PAGE_SUPPLIES:
        name = sUi.supplyUnits[index] == ITEM_NONE ? COMPOUND_STRING("Empty unit") : GetItemName(sUi.supplyUnits[index]);
        if (sUi.supplyUnits[index]) Print(188, y, COMPOUND_STRING("x1"));
        break;
    case PAGE_SUPPLY_PICK:
        name = index ? GetItemName(sUi.choices[index]) : COMPOUND_STRING("Empty unit");
        if (index) Number(182, y, CountTotalItemQuantityInBag(sUi.choices[index]));
        break;
    default: break;
    }
    Print(10, y, name);
    if (index == sUi.cursor) Print(0, y, COMPOUND_STRING(">"));
}

static void Draw(void)
{
    FillWindowPixelBuffer(0, PIXEL_FILL(TEXT_COLOR_WHITE));
    u32 rows = RowCount(), firstY = 34, spacing = 14;
    switch (sUi.page)
    {
    case PAGE_HOME:
        Print(3, 0, COMPOUND_STRING("CENTER development lounge"));
        Print(3, 15, COMPOUND_STRING("Practice Points:")); Number(100, 15, gSaveBlock3Ptr->sc.practiceExp);
        Footer(COMPOUND_STRING("A choose  B leave")); break;
    case PAGE_PARTY:
        Print(3, 0, COMPOUND_STRING("Choose your POKéMON"));
        if (!rows) Print(3, 35, COMPOUND_STRING("No eligible POKéMON in your party."));
        Footer(COMPOUND_STRING("A choose  B back")); break;
    case PAGE_MON:
        MonTitle(); Print(3, 15, sUi.center ? COMPOUND_STRING("Free services; IV improvement uses points.") : COMPOUND_STRING("Field training and public build inspection"));
        Footer(COMPOUND_STRING("A choose  B back")); break;
    case PAGE_TRAINING:
        MonTitle(); Print(3, 14, COMPOUND_STRING("Training ceiling per stat:")); Number(141, 14, ScTrainingCeiling(ScGetBadgeCount()));
        for (u32 i = 0; i < SC_STAT_COUNT; i++)
        {
            Print(8, 30 + i * 13, sStatNames[i]); Number(155, 30 + i * 13, GetMonData(CurrentMon(), sEvFields[i]));
        }
        Print(3, 111, COMPOUND_STRING("Focus:"));
        u32 focus = GetMonData(CurrentMon(), MON_DATA_SC_FOCUS);
        Print(42, 111, sStatNames[focus <= SC_FOCUS_BALANCED ? focus : SC_FOCUS_BALANCED]);
        Footer(COMPOUND_STRING("A change future focus  B back")); break;
    case PAGE_FOCUS:
        Print(3, 0, COMPOUND_STRING("Direct future training"));
        Print(3, 14, COMPOUND_STRING("Existing points never move."));
        Footer(COMPOUND_STRING("A choose  SELECT details  B back")); break;
    case PAGE_CAPABILITIES: case PAGE_TRAINER:
    {
        bool32 trainer = sUi.page == PAGE_TRAINER;
        Print(3, 0, trainer ? COMPOUND_STRING("Trainer-owned passive build") : COMPOUND_STRING("This POKéMON's earned capabilities"));
        Print(3, 14, COMPOUND_STRING("Slots used:")); Number(67, 14, MaskCost(ActiveMask(trainer), trainer));
        Print(85, 14, COMPOUND_STRING("/")); Number(96, 14, trainer ? ScTrainerCapacity(ScGetBadgeCount()) : 3);
        Footer(sUi.center ? COMPOUND_STRING("A effect and assignment  B back") : COMPOUND_STRING("A inspect  B back; assign at a CENTER")); break;
    }
    case PAGE_EFFECT:
    {
        bool32 trainer = sUi.effectKind;
        const struct ScPassiveInfo *info = &(trainer ? gScTrainerPassiveInfo : gScCapabilityInfo)[sUi.selection];
        Print(3, 0, info->name); Print(3, 14, COMPOUND_STRING("Cost:")); Number(36, 14, info->cost);
        Print(62, 14, ActiveMask(trainer) & (1u << sUi.selection) ? COMPOUND_STRING("Active") : OwnedMask(trainer) & (1u << sUi.selection) ? COMPOUND_STRING("Owned; inactive") : COMPOUND_STRING("Not earned"));
        Description(info->description, 31, 6);
        Print(3, 113, COMPOUND_STRING("Multipliers stack; each step rounds down."));
        Footer(sUi.center ? COMPOUND_STRING("A assign/remove  UP/DOWN scroll  B back") : COMPOUND_STRING("UP/DOWN scroll  B back")); break;
    }
    case PAGE_ABILITIES:
        MonTitle(); Print(3, 14, COMPOUND_STRING("Official ability choices; nature stays fixed."));
        Footer(COMPOUND_STRING("A effect  B back")); break;
    case PAGE_ABILITY_DETAIL:
    {
        u32 species = GetMonData(CurrentMon(), MON_DATA_SPECIES);
        u32 ability = gSpeciesInfo[species].abilities[sUi.selection];
        Print(3, 0, gAbilitiesInfo[ability].name);
        Print(3, 14, gNaturesInfo[GetNature(CurrentMon())].name);
        Print(85, 14, COMPOUND_STRING("nature (fixed)"));
        Description(gAbilitiesInfo[ability].description, 34, 7);
        Footer(sUi.center ? COMPOUND_STRING("A choose ability  UP/DOWN scroll  B back") : COMPOUND_STRING("UP/DOWN scroll  B back")); break;
    }
    case PAGE_IVS:
        Print(3, 0, COMPOUND_STRING("Improve individual potential (IV)"));
        Print(3, 14, COMPOUND_STRING("Practice Points:")); Number(100, 14, gSaveBlock3Ptr->sc.practiceExp);
        Print(3, 27, COMPOUND_STRING("Cost per +1:")); Number(79, 27, ScServicesIvCost(GetMonData(CurrentMon(), MON_DATA_LEVEL)));
        firstY = 44; spacing = 13;
        Footer(COMPOUND_STRING("A review +1  B back")); break;
    case PAGE_IV_CONFIRM:
        Print(3, 0, COMPOUND_STRING("Spend Practice Points?")); Print(3, 18, sStatNames[sUi.selection]);
        Print(77, 18, COMPOUND_STRING("IV +1")); Print(3, 38, COMPOUND_STRING("Cost:")); Number(39, 38, ScServicesIvCost(GetMonData(CurrentMon(), MON_DATA_LEVEL)));
        Print(3, 57, COMPOUND_STRING("Improves this stat's IV, up to 31.\nNature and training stay fixed."));
        firstY = 93; Footer(COMPOUND_STRING("A confirm  B cancel")); break;
    case PAGE_MOVES:
        MonTitle(); Print(3, 14, COMPOUND_STRING("Free current-level move relearning"));
        if (!rows) Print(3, 35, COMPOUND_STRING("No forgotten level-up moves available."));
        Footer(COMPOUND_STRING("A move details  B back")); break;
    case PAGE_MOVE_DETAIL:
        Print(3, 0, GetMoveName(sUi.selection)); Description(GetMoveDescription(sUi.selection), 25, 8);
        Footer(COMPOUND_STRING("A choose move slot  UP/DOWN scroll  B back")); break;
    case PAGE_REPLACE_MOVE:
        Print(3, 0, COMPOUND_STRING("Replace which move?")); Print(3, 14, GetMoveName(sUi.selection));
        Footer(COMPOUND_STRING("A replace and learn  B cancel")); break;
    case PAGE_SUPPLIES:
        Print(3, 0, COMPOUND_STRING("Trainer battle: selected units"));
        Print(3, 15, COMPOUND_STRING("Total 3; HP 2; status, PP, boost 1 each."));
        Print(3, 98, COMPOUND_STRING("Owned stock is checked at battle entry.\nUsed supplies stay spent after a loss."));
        Footer(COMPOUND_STRING("A choose unit  START clear  B back")); break;
    case PAGE_SUPPLY_PICK:
        Print(3, 0, COMPOUND_STRING("Choose an owned legal supply")); Print(3, 14, COMPOUND_STRING("Each choice fills one unit."));
        Footer(COMPOUND_STRING("A choose  B back")); break;
    case PAGE_HELP:
        Print(3, 0, COMPOUND_STRING("Development and ownership"));
        Description(COMPOUND_STRING("Badges raise every POKéMON's training ceiling, not its points. Surviving field participants earn permanent training after a win. Focus directs future points only. A full focused stat discards gains, including fractions. Balanced focus fills the least-trained stat and discards only when all are full. Capabilities belong to the individual and require repeatable boss victories. Trainer passives belong to you. Assign both freely here, within separate budgets. Practice Points are your shared battle-win resource for IV improvements, including recruits. Nature stays fixed."), 20, 8);
        Footer(COMPOUND_STRING("UP/DOWN scroll  B back")); break;
    case PAGE_NOTICE:
        Print(3, 0, COMPOUND_STRING("Development")); Description(sUi.notice, 24, 8); Footer(COMPOUND_STRING("A or B back")); break;
    }
    u32 start = sUi.cursor >= 6 ? sUi.cursor - 5 : 0;
    for (u32 i = start; i < rows && i < start + 6; i++) DrawListRow(i, firstY + spacing * (i - start));
    PutWindowTilemap(0);
    CopyWindowToVram(0, COPYWIN_FULL);
}

static void ExitServices(void)
{
    void (*callback)(void) = sUi.returnCallback;
    sUi.active = FALSE;
    SetVBlankCallback(NULL);
    FreeAllWindowBuffers();
    UnsetBgTilemapBuffer(0);
    SetMainCallback2(callback);
}

static void Back(void)
{
    switch (sUi.page)
    {
    case PAGE_HOME: ExitServices(); return;
    case PAGE_MON: if (!sUi.center) { ExitServices(); return; } LoadParty(); Go(PAGE_PARTY); break;
    case PAGE_NOTICE: sUi.page = sUi.returnPage; sUi.cursor = sUi.returnCursor; sUi.scroll = 0; break;
    case PAGE_PARTY: case PAGE_SUPPLIES: Go(PAGE_HOME); break;
    case PAGE_TRAINER: Go(sUi.center ? PAGE_HOME : PAGE_MON); break;
    case PAGE_HELP: Go(sUi.returnPage); break;
    case PAGE_FOCUS: Go(PAGE_TRAINING); break;
    case PAGE_EFFECT: Go(sUi.effectKind ? PAGE_TRAINER : PAGE_CAPABILITIES); sUi.cursor = sUi.selection; break;
    case PAGE_ABILITY_DETAIL: Go(PAGE_ABILITIES); LoadAbilities(); break;
    case PAGE_IV_CONFIRM: Go(PAGE_IVS); sUi.cursor = sUi.selection; break;
    case PAGE_MOVE_DETAIL: Go(PAGE_MOVES); sUi.count = ScServicesRelearnMoves(sUi.partyIndex, sUi.choices, SC_SERVICE_MAX_MOVES); break;
    case PAGE_REPLACE_MOVE: Go(PAGE_MOVE_DETAIL); break;
    case PAGE_SUPPLY_PICK: Go(PAGE_SUPPLIES); break;
    default: Go(PAGE_MON); break;
    }
}

static void Select(void)
{
    switch (sUi.page)
    {
    case PAGE_HOME:
        if (sUi.cursor == 0)
        {
            LoadParty();
            Go(PAGE_PARTY);
        }
        else if (sUi.cursor == 1) { ScRefreshTrainerUnlocks(); Go(PAGE_TRAINER); }
        else if (sUi.cursor == 2) { LoadSupplyUnits(); Go(PAGE_SUPPLIES); }
        else if (sUi.cursor == 3) { sUi.returnPage = PAGE_HOME; Go(PAGE_HELP); }
        else ExitServices();
        break;
    case PAGE_PARTY: if (sUi.count) { sUi.partyIndex = sUi.choices[sUi.cursor]; Go(PAGE_MON); } break;
    case PAGE_MON:
        if (sUi.cursor == 0) Go(PAGE_TRAINING);
        else if (sUi.cursor == 1) Go(PAGE_CAPABILITIES);
        else if (!sUi.center && sUi.cursor == 2) { ScRefreshTrainerUnlocks(); Go(PAGE_TRAINER); }
        else if (sUi.cursor == 2 || (!sUi.center && sUi.cursor == 3)) { LoadAbilities(); Go(PAGE_ABILITIES); }
        else if (sUi.cursor == 3) { sUi.count = ScServicesRelearnMoves(sUi.partyIndex, sUi.choices, SC_SERVICE_MAX_MOVES); Go(PAGE_MOVES); }
        else Go(PAGE_IVS);
        break;
    case PAGE_TRAINING: Go(PAGE_FOCUS); break;
    case PAGE_FOCUS:
    {
        u32 result = ScServicesSetFocus(sUi.partyIndex, sUi.cursor);
        if (result == SC_SERVICE_OK)
            Notice(COMPOUND_STRING("Focus updated for future gains.\nExisting training never moves.\nA full focused stat discards gains.\nBalanced fills the least-trained stat."));
        else Result(result);
        break;
    }
    case PAGE_CAPABILITIES: case PAGE_TRAINER:
        sUi.effectKind = sUi.page == PAGE_TRAINER; sUi.selection = sUi.cursor; Go(PAGE_EFFECT); break;
    case PAGE_EFFECT:
        if (sUi.center)
        {
            u32 mask = ActiveMask(sUi.effectKind) ^ (1u << sUi.selection);
            Result(sUi.effectKind ? ScServicesSetTrainerPassives(mask) : ScServicesSetCapabilities(sUi.partyIndex, mask));
        }
        break;
    case PAGE_ABILITIES:
        if (sUi.count) { sUi.selection = sUi.choices[sUi.cursor]; Go(PAGE_ABILITY_DETAIL); } break;
    case PAGE_ABILITY_DETAIL: if (sUi.center) Result(ScServicesSetAbility(sUi.partyIndex, sUi.selection)); break;
    case PAGE_IVS: sUi.selection = sUi.cursor; Go(PAGE_IV_CONFIRM); sUi.cursor = 1; break;
    case PAGE_IV_CONFIRM:
        if (sUi.cursor) Go(PAGE_IVS);
        else { u32 result = ScServicesImproveIv(sUi.partyIndex, sUi.selection); Go(PAGE_IVS); sUi.cursor = sUi.selection; Result(result); }
        break;
    case PAGE_MOVES: if (sUi.count) { sUi.selection = sUi.choices[sUi.cursor]; Go(PAGE_MOVE_DETAIL); } break;
    case PAGE_MOVE_DETAIL: Go(PAGE_REPLACE_MOVE); break;
    case PAGE_REPLACE_MOVE:
    {
        u32 result = ScServicesTeachMove(sUi.partyIndex, sUi.selection, sUi.cursor);
        Go(PAGE_MOVES); sUi.count = ScServicesRelearnMoves(sUi.partyIndex, sUi.choices, SC_SERVICE_MAX_MOVES); Result(result); break;
    }
    case PAGE_SUPPLIES:
        sUi.supplySlot = sUi.cursor; sUi.count = 1; sUi.choices[0] = ITEM_NONE;
        for (u32 i = 1; i < ITEMS_COUNT; i++)
            if (ScSuppliesCategory(i) != SC_SUPPLY_INVALID && CountTotalItemQuantityInBag(i)) sUi.choices[sUi.count++] = i;
        Go(PAGE_SUPPLY_PICK); break;
    case PAGE_SUPPLY_PICK: SetSupply(sUi.supplySlot, sUi.choices[sUi.cursor]); break;
    case PAGE_NOTICE: Back(); break;
    default: break;
    }
}

static void CB2_Services(void)
{
    u32 keys = gMain.newKeys;
    if (keys & B_BUTTON) Back();
    else if (keys & A_BUTTON) Select();
    else if ((keys & START_BUTTON) && sUi.page == PAGE_SUPPLIES) SetSupply(SC_SUPPLY_SLOTS, ITEM_NONE);
    else if ((keys & SELECT_BUTTON) && sUi.page == PAGE_FOCUS) { sUi.returnPage = PAGE_FOCUS; Go(PAGE_HELP); }
    else if (keys & (DPAD_UP | DPAD_DOWN))
    {
        u32 rows = RowCount();
        if (rows) sUi.cursor = (sUi.cursor + (keys & DPAD_UP ? rows - 1 : 1)) % rows;
        else if (keys & DPAD_UP) { if (sUi.scroll) sUi.scroll--; }
        else if (sUi.scroll < 31) sUi.scroll++;
    }
    if (!sUi.active) return;
    if (keys) Draw();
    RunTasks();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlank_Services(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_InitServices(void)
{
    SetVBlankCallback(NULL);
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    FreeAllWindowBuffers();
    ResetVramOamAndBgCntRegs();
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, sBg, ARRAY_COUNT(sBg));
    ResetAllBgsCoordinates();
    memset(sBgTilemap, 0, sizeof(sBgTilemap));
    SetBgTilemapBuffer(0, sBgTilemap);
    if (!InitWindows(sWindows)) { ExitServices(); return; }
    ResetPaletteFade();
    LoadMessageBoxAndBorderGfx();
    SetStandardWindowBorderStyle(0, FALSE);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    ShowBg(0);
    Draw();
    SetVBlankCallback(VBlank_Services);
    SetMainCallback2(CB2_Services);
}

bool32 ScShowFieldTraining(u32 partyIndex, void (*returnCallback)(void))
{
    if (sUi.active || !returnCallback || gMain.inBattle || !ScProgressionEnabled() || !ScServicesValidMon(partyIndex))
        return FALSE;
    memset(&sUi, 0, sizeof(sUi));
    sUi.active = TRUE; sUi.partyIndex = partyIndex; sUi.returnCallback = returnCallback;
    Go(PAGE_MON);
    SetMainCallback2(CB2_InitServices);
    return TRUE;
}

static void Task_RejectCenter(u8 taskId)
{
    DestroyTask(taskId);
    ScriptContext_Enable();
}

void ScShowCenterServices(void)
{
    gSpecialVar_Result = FALSE;
    if (sUi.active || !ScServicesAtCenter()) { CreateTask(Task_RejectCenter, 8); return; }
    memset(&sUi, 0, sizeof(sUi));
    sUi.active = sUi.center = TRUE;
    sUi.returnCallback = CB2_ReturnToFieldContinueScriptPlayMapMusic;
    ScRefreshTrainerUnlocks();
    Go(PAGE_HOME);
    SetMainCallback2(CB2_InitServices);
}
