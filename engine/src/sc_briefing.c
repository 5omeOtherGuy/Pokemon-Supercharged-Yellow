#include "global.h"
#include "data.h"
#include "pokemon.h"
#include "move.h"
#include "sc_briefing.h"
#include "sc_build_catalog.h"

#ifndef SC_BRIEFING_HOST_TEST
#include "main.h"
#include "event_data.h"
#include "item.h"
#include "menu.h"
#include "script.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "constants/characters.h"
#endif

static u32 CountBits(u32 mask)
{
    u32 count = 0;
    while (mask)
    {
        count += mask & 1;
        mask >>= 1;
    }
    return count;
}

u16 ScBriefingGetAbility(const struct ScBriefingModel *model, u32 index)
{
    const struct TrainerMon *mon;
    if (model->error || index >= model->partySize)
        return ABILITY_NONE;
    mon = &GetTrainerPartyFromId(model->trainerId)[index];
    return mon->ability ? mon->ability : gSpeciesInfo[mon->species].abilities[0];
}

void ScBriefingGetMoves(const struct ScBriefingModel *model, u32 index, u16 moves[4])
{
    const struct TrainerMon *mon;
    bool32 explicitMoves = FALSE;
    u32 count = 0;
    memset(moves, 0, sizeof(u16) * MAX_MON_MOVES);
    if (model->error || index >= model->partySize)
        return;
    mon = &GetTrainerPartyFromId(model->trainerId)[index];
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        if (mon->moves[i] != MOVE_NONE)
            explicitMoves = TRUE;
    if (explicitMoves)
    {
        for (u32 i = 0; i < MAX_MON_MOVES; i++)
            moves[i] = mon->moves[i];
        return;
    }
    // Same ordered last-four/current-window deduplication as
    // GiveBoxMonInitialMoveset; no generated Pokemon or RNG consumption.
    const struct LevelUpMove *learnset = GetSpeciesLevelUpLearnset(mon->species);
    for (u32 i = 0; learnset[i].move != LEVEL_UP_MOVE_END; i++)
    {
        bool32 known = FALSE;
        if (learnset[i].level > mon->lvl)
            break;
        if (learnset[i].level == 0)
            continue;
        for (u32 j = 0; j < count; j++)
            if (moves[j] == learnset[i].move)
                known = TRUE;
        if (known)
            continue;
        if (count < MAX_MON_MOVES)
            moves[count++] = learnset[i].move;
        else
        {
            for (u32 j = 1; j < MAX_MON_MOVES; j++)
                moves[j - 1] = moves[j];
            moves[MAX_MON_MOVES - 1] = learnset[i].move;
        }
    }
}

bool32 ScBriefingLoad(u16 trainerId, struct ScBriefingModel *model)
{
    const struct Trainer *trainer;
    memset(model, 0, sizeof(*model));
    model->error = SC_BRIEF_BAD_TRAINER;
    if (trainerId == 0 || trainerId >= TRAINERS_COUNT)
        return FALSE;
    trainer = GetTrainerStructFromId(trainerId);
    model->trainer = trainer;
    model->trainerId = trainerId;
    model->partySize = trainer->partySize;
    if (trainer->overrideTrainer || trainer->poolSize || trainer->poolRuleIndex
        || trainer->poolPickIndex || trainer->poolPruneIndex)
    {
        model->error = SC_BRIEF_VARIABLE_PARTY;
        return FALSE;
    }
    if (!trainer->party || trainer->partySize == 0 || trainer->partySize > PARTY_SIZE
        || trainer->battleType > TRAINER_BATTLE_TYPE_DOUBLES)
        return FALSE;
    for (u32 i = 0; i < sizeof(trainer->startingStatus); i++)
        if (((const u8 *)&trainer->startingStatus)[i])
        {
            model->error = SC_BRIEF_FIELD_EFFECT;
            return FALSE;
        }
    for (u32 i = 0; i < trainer->partySize; i++)
    {
        const struct TrainerMon *mon = &GetTrainerPartyFromId(trainerId)[i];
        bool32 legalAbility = FALSE;
        model->error = SC_BRIEF_BAD_MON;
        if (mon->species < SPECIES_BULBASAUR || mon->species > SPECIES_MEW
            || mon->lvl == 0 || mon->lvl > MAX_LEVEL || mon->nature >= NUM_NATURES
            || mon->heldItem >= ITEMS_COUNT)
            return FALSE;
        for (u32 j = 0; j < MAX_MON_MOVES; j++)
            if (mon->moves[j] >= MOVES_COUNT_ALL)
                return FALSE;
        for (u32 j = 0; j < ARRAY_COUNT(gSpeciesInfo[mon->species].abilities); j++)
            if (mon->ability && mon->ability == gSpeciesInfo[mon->species].abilities[j])
                legalAbility = TRUE;
        if (mon->ability && !legalAbility)
            return FALSE;
        if (!mon->ability && B_TRAINER_MON_RANDOM_ABILITY != 0)
        {
            model->error = SC_BRIEF_VARIABLE_ABILITY;
            return FALSE;
        }
    }
    for (u32 i = 0; i < ARRAY_COUNT(trainer->items); i++)
        if (trainer->items[i] >= ITEMS_COUNT)
            return FALSE;
    model->error = SC_BRIEF_OK;
    return TRUE;
}

bool32 ScBriefingCanAccept(const struct ScBriefingModel *model, u32 healthyCount)
{
    return model->error == SC_BRIEF_OK
        && healthyCount >= (model->trainer->battleType == TRAINER_BATTLE_TYPE_DOUBLES ? 2u : 1u);
}

u32 ScBriefingNavigate(const struct ScBriefingModel *model, struct ScBriefingView *view, u16 keys)
{
    u32 rows = 1;
    if (keys & B_BUTTON)
    {
        if (view->screen <= SC_BRIEF_TRAINER || model->error)
            return SC_BRIEF_DECLINE;
        if (view->screen == SC_BRIEF_DESCRIPTION)
        {
            view->screen = view->parent;
            view->cursor = view->entry;
        }
        else
        {
            view->screen = view->screen == SC_BRIEF_CAPABILITIES ? SC_BRIEF_MON : SC_BRIEF_TEAM;
            view->cursor = view->screen == SC_BRIEF_TEAM ? view->mon : 6;
        }
        return SC_BRIEF_REDRAW;
    }
    if (model->error)
        return keys & A_BUTTON ? SC_BRIEF_DECLINE : SC_BRIEF_IDLE;
    if (keys & START_BUTTON)
    {
        view->screen = SC_BRIEF_CONFIRM;
        view->cursor = 1; // Explicit acceptance, safe default.
        return SC_BRIEF_REDRAW;
    }
    if (view->screen <= SC_BRIEF_TRAINER && keys & (DPAD_LEFT | DPAD_RIGHT))
    {
        view->screen = (view->screen + (keys & DPAD_LEFT ? 2 : 1)) % 3;
        view->cursor = 0;
        return SC_BRIEF_REDRAW;
    }
    if (view->screen == SC_BRIEF_MON && keys & (DPAD_LEFT | DPAD_RIGHT))
    {
        view->mon = (view->mon + (keys & DPAD_LEFT ? model->partySize - 1 : 1)) % model->partySize;
        return SC_BRIEF_REDRAW;
    }
    switch (view->screen)
    {
    case SC_BRIEF_TEAM: rows = model->partySize; break;
    case SC_BRIEF_BAG: rows = ARRAY_COUNT(model->trainer->items); break;
    case SC_BRIEF_TRAINER: rows = CountBits(ScGetNpcTrainerPassives(model->trainerId)); break;
    case SC_BRIEF_MON: rows = 7; break;
    case SC_BRIEF_CAPABILITIES: rows = CountBits(ScGetNpcCapabilities(model->trainerId, view->mon)); break;
    case SC_BRIEF_CONFIRM: rows = 2; break;
    case SC_BRIEF_DESCRIPTION: rows = 32; break; // Renderer clamps to actual wrapped lines.
    }
    if (rows && keys & (DPAD_UP | DPAD_DOWN))
    {
        if (view->screen == SC_BRIEF_DESCRIPTION)
        {
            if ((keys & DPAD_UP) && view->cursor)
                view->cursor--;
            else if ((keys & DPAD_DOWN) && view->cursor < 31)
                view->cursor++;
        }
        else
            view->cursor = (view->cursor + (keys & DPAD_UP ? rows - 1 : 1)) % rows;
        return SC_BRIEF_REDRAW;
    }
    if (!(keys & A_BUTTON))
        return SC_BRIEF_IDLE;
    if (view->screen == SC_BRIEF_CONFIRM)
        return view->cursor == 0 ? SC_BRIEF_ACCEPT : SC_BRIEF_DECLINE;
    if (view->screen == SC_BRIEF_TEAM)
    {
        view->mon = view->cursor;
        view->screen = SC_BRIEF_MON;
        view->cursor = 0;
    }
    else if (view->screen == SC_BRIEF_MON && view->cursor == 6)
    {
        view->screen = SC_BRIEF_CAPABILITIES;
        view->cursor = 0;
    }
    else if (view->screen != SC_BRIEF_DESCRIPTION && rows)
    {
        view->parent = view->screen;
        view->entry = view->cursor;
        view->screen = SC_BRIEF_DESCRIPTION;
        view->cursor = 0;
    }
    return SC_BRIEF_REDRAW;
}

#ifndef SC_BRIEFING_HOST_TEST
static EWRAM_DATA struct ScBriefingModel sBriefing = {0};
static EWRAM_DATA struct ScBriefingView sView = {0};
static EWRAM_DATA u8 sWindow = 0;
static EWRAM_DATA bool8 sNeedsPartner = FALSE;
static EWRAM_DATA u8 sLineBuffer[32][128] = {0};
static const u8 sColors[] = {TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};
static const struct WindowTemplate sWindowTemplate =
{
    .bg = 0, .tilemapLeft = 1, .tilemapTop = 1, .width = 28, .height = 17,
    // BG0 starts at VRAM 0x8000; keep 476 tiles below the frame tiles at
    // 0x200 and field tilemaps at 0xE000. Restore overlapped field windows
    // from their retained CPU buffers when this temporary overlay closes.
    .paletteNum = 15, .baseBlock = 1,
};

static void Print(u32 x, u32 y, const u8 *text)
{
    AddTextPrinterParameterized4(sWindow, FONT_SMALL, x, y, 0, 0, sColors, TEXT_SKIP_DRAW, text);
}

static void Label(u32 y, const u8 *label, const u8 *value)
{
    u8 text[256];
    StringCopy(text, label);
    StringAppend(text, value);
    Print(4, y, text);
}

static void Number(u32 x, u32 y, u32 value)
{
    u8 text[12];
    ConvertIntToDecimalStringN(text, value, STR_CONV_MODE_LEFT_ALIGN, 3);
    Print(x, y, text);
}

static u32 SelectedBit(u32 mask, u32 ordinal)
{
    for (u32 i = 0; i < 32; i++)
        if (mask & (1u << i))
        {
            if (ordinal == 0)
                return i;
            ordinal--;
        }
    return 0;
}

static u32 HealthyPartyCount(void)
{
    u32 count = 0;
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE
            && !GetMonData(mon, MON_DATA_IS_EGG) && GetMonData(mon, MON_DATA_HP))
            count++;
    }
    return count;
}

static void MonHeading(void)
{
    const struct TrainerMon *mon = &GetTrainerPartyFromId(sBriefing.trainerId)[sView.mon];
    const struct NatureInfo *nature = &gNaturesInfo[mon->nature];
    static const u8 *const statNames[] =
    {
        COMPOUND_STRING("HP"), COMPOUND_STRING("Attack"), COMPOUND_STRING("Defense"),
        COMPOUND_STRING("Speed"), COMPOUND_STRING("Sp. Atk"), COMPOUND_STRING("Sp. Def"),
    };
    u8 text[128];
    Print(4, 0, GetSpeciesName(mon->species));
    Print(148, 0, COMPOUND_STRING("Lv."));
    Number(163, 0, mon->lvl);
    Number(192, 0, sView.mon + 1);
    Print(201, 0, COMPOUND_STRING("/"));
    Number(207, 0, sBriefing.partySize);
    StringCopy(text, nature->name);
    if (nature->statUp == nature->statDown)
        StringAppend(text, COMPOUND_STRING(" (neutral)"));
    else
    {
        StringAppend(text, COMPOUND_STRING(" (+"));
        StringAppend(text, statNames[nature->statUp]);
        StringAppend(text, COMPOUND_STRING(" -"));
        StringAppend(text, statNames[nature->statDown]);
        StringAppend(text, COMPOUND_STRING(")"));
    }
    Print(4, 13, text);
}

static void PrintRowsCursor(u32 firstY, u32 spacing)
{
    Print(0, firstY + spacing * sView.cursor, COMPOUND_STRING(">"));
}

// Wrap canonical game text by measured glyph width. Scroll instead of silently
// dropping long descriptions; explicit source newlines remain paragraph breaks.
static u32 WrapDescription(const u8 *source)
{
    u32 line = 0, length = 0, lastSpace = 0;
    memset(sLineBuffer, EOS, sizeof(sLineBuffer));
    while (*source != EOS && line < ARRAY_COUNT(sLineBuffer))
    {
        u8 ch = *source++;
        if (ch == CHAR_NEWLINE)
        {
            line++;
            length = lastSpace = 0;
            continue;
        }
        sLineBuffer[line][length++] = ch;
        sLineBuffer[line][length] = EOS;
        if (ch == CHAR_SPACE)
            lastSpace = length;
        if (GetStringWidth(FONT_SMALL, sLineBuffer[line], 0) > 214 || length >= 120)
        {
            u32 split = lastSpace ? lastSpace : length - 1;
            if (line + 1 >= ARRAY_COUNT(sLineBuffer))
                break;
            memcpy(sLineBuffer[line + 1], &sLineBuffer[line][split], length - split);
            sLineBuffer[line + 1][length - split] = EOS;
            sLineBuffer[line][lastSpace ? split - 1 : split] = EOS;
            length -= split;
            lastSpace = 0;
            line++;
        }
    }
    return line < ARRAY_COUNT(sLineBuffer) ? line + 1 : ARRAY_COUNT(sLineBuffer);
}

static void DrawDescription(void)
{
    const u8 *name = COMPOUND_STRING("No effect");
    const u8 *description = COMPOUND_STRING("No active effect is assigned.");
    u16 moves[4];
    u32 id;
    bool32 passive = FALSE;
    if (sView.parent == SC_BRIEF_MON)
    {
        if (sView.entry < 4)
        {
            ScBriefingGetMoves(&sBriefing, sView.mon, moves);
            id = moves[sView.entry];
            name = id == MOVE_NONE ? COMPOUND_STRING("Empty move slot") : GetMoveName(id);
            description = id == MOVE_NONE ? COMPOUND_STRING("No move is assigned to this slot.") : GetMoveDescription(id);
            Print(4, 17, COMPOUND_STRING("Power"));
            Number(33, 17, GetMovePower(id));
            Print(62, 17, COMPOUND_STRING("Acc."));
            Number(84, 17, GetMoveAccuracy(id));
            Print(115, 17, COMPOUND_STRING("PP"));
            Number(131, 17, GetMovePP(id));
            Print(157, 17, GetMoveCategory(id) == DAMAGE_CATEGORY_PHYSICAL ? COMPOUND_STRING("Physical")
                : GetMoveCategory(id) == DAMAGE_CATEGORY_SPECIAL ? COMPOUND_STRING("Special") : COMPOUND_STRING("Status"));
        }
        else if (sView.entry == 4)
        {
            id = ScBriefingGetAbility(&sBriefing, sView.mon);
            name = gAbilitiesInfo[id].name;
            description = gAbilitiesInfo[id].description;
        }
        else
        {
            id = GetTrainerPartyFromId(sBriefing.trainerId)[sView.mon].heldItem;
            name = id == ITEM_NONE ? COMPOUND_STRING("No held item") : GetItemName(id);
            description = id == ITEM_NONE ? COMPOUND_STRING("This POKéMON is not holding an item.") : GetItemDescription(id);
        }
    }
    else if (sView.parent == SC_BRIEF_BAG)
    {
        id = sBriefing.trainer->items[sView.entry];
        name = id == ITEM_NONE ? COMPOUND_STRING("Empty bag slot") : GetItemName(id);
        description = id == ITEM_NONE ? COMPOUND_STRING("No usable item occupies this bag slot.") : GetItemDescription(id);
    }
    else
    {
        const struct ScPassiveInfo *info;
        if (sView.parent == SC_BRIEF_CAPABILITIES)
        {
            id = SelectedBit(ScGetNpcCapabilities(sBriefing.trainerId, sView.mon), sView.entry);
            info = &gScCapabilityInfo[id];
        }
        else
        {
            id = SelectedBit(ScGetNpcTrainerPassives(sBriefing.trainerId), sView.entry);
            info = &gScTrainerPassiveInfo[id];
        }
        name = info->name;
        description = info->description;
        Print(4, 17, COMPOUND_STRING("Slot cost:"));
        Number(55, 17, info->cost);
        passive = TRUE;
    }
    Print(4, 0, name);
    u32 lines = WrapDescription(description);
    u32 visible = 6;
    if (sView.cursor > (lines > visible ? lines - visible : 0))
        sView.cursor = lines > visible ? lines - visible : 0;
    for (u32 i = 0; i < visible && i + sView.cursor < lines; i++)
        Print(4, 34 + i * 12, sLineBuffer[i + sView.cursor]);
    if (passive)
        Print(4, 107, COMPOUND_STRING("Multipliers stack; each step rounds down."));
    Print(4, 121, COMPOUND_STRING("UP/DOWN scroll   B back   START ready"));
}

static void DrawBriefing(void)
{
    FillWindowPixelBuffer(sWindow, PIXEL_FILL(TEXT_COLOR_WHITE));
    if (sBriefing.error)
    {
        Print(4, 4, COMPOUND_STRING("This briefing cannot start a battle."));
        Print(4, 24, sBriefing.error == SC_BRIEF_VARIABLE_PARTY
            ? COMPOUND_STRING("This trainer uses a variable party.\nAn exact roster is required.")
            : sBriefing.error == SC_BRIEF_VARIABLE_ABILITY
            ? COMPOUND_STRING("An ability has not been fixed.\nAn exact loadout is required.")
            : sBriefing.error == SC_BRIEF_FIELD_EFFECT
            ? COMPOUND_STRING("This arena has unsupported effects.")
            : COMPOUND_STRING("The trainer's roster is unavailable."));
        Print(4, 110, COMPOUND_STRING("A or B: return safely"));
    }
    else if (sView.screen == SC_BRIEF_DESCRIPTION)
        DrawDescription();
    else if (sView.screen == SC_BRIEF_MON)
    {
        u16 moves[4];
        MonHeading();
        ScBriefingGetMoves(&sBriefing, sView.mon, moves);
        for (u32 i = 0; i < 4; i++)
            Print(9, 30 + 12 * i, moves[i] == MOVE_NONE ? COMPOUND_STRING("Empty") : GetMoveName(moves[i]));
        Label(78, COMPOUND_STRING(" Ability: "), gAbilitiesInfo[ScBriefingGetAbility(&sBriefing, sView.mon)].name);
        u16 heldItem = GetTrainerPartyFromId(sBriefing.trainerId)[sView.mon].heldItem;
        Label(90, COMPOUND_STRING(" Item: "), heldItem == ITEM_NONE ? COMPOUND_STRING("None") : GetItemName(heldItem));
        Print(9, 102, COMPOUND_STRING("Capabilities"));
        PrintRowsCursor(30, 12);
        Print(4, 121, COMPOUND_STRING("A inspect  L/R teammate  B back"));
    }
    else if (sView.screen == SC_BRIEF_CAPABILITIES || sView.screen == SC_BRIEF_TRAINER)
    {
        bool32 individual = sView.screen == SC_BRIEF_CAPABILITIES;
        u32 mask = individual ? ScGetNpcCapabilities(sBriefing.trainerId, sView.mon) : ScGetNpcTrainerPassives(sBriefing.trainerId);
        const struct ScPassiveInfo *catalog = individual ? gScCapabilityInfo : gScTrainerPassiveInfo;
        u32 count = CountBits(mask), cost = 0;
        Print(4, 0, individual ? COMPOUND_STRING("Assigned capabilities") : COMPOUND_STRING("Active trainer build"));
        for (u32 i = 0; i < count; i++)
        {
            u32 id = SelectedBit(mask, i);
            Print(9, 31 + 15 * i, catalog[id].name);
            Number(198, 31 + 15 * i, catalog[id].cost);
            cost += catalog[id].cost;
        }
        Print(4, 14, COMPOUND_STRING("Slots used:"));
        Number(63, 14, cost);
        Print(75, 14, COMPOUND_STRING("/"));
        Number(83, 14, individual ? 3 : ScGetNpcTrainerBudget(sBriefing.trainerId));
        if (count)
            PrintRowsCursor(31, 15);
        else
            Print(9, 31, COMPOUND_STRING("None assigned"));
        Print(4, 121, individual ? COMPOUND_STRING("A effect  B back  START ready") : COMPOUND_STRING("A effect  L/R pages  B leave"));
    }
    else if (sView.screen == SC_BRIEF_CONFIRM)
    {
        Print(4, 4, COMPOUND_STRING("Begin this battle?"));
        Print(4, 24, sBriefing.trainer->battleType == TRAINER_BATTLE_TYPE_DOUBLES
            ? COMPOUND_STRING("Doubles - two healthy POKéMON needed.") : COMPOUND_STRING("Singles - use your available party."));
        if (sNeedsPartner)
            Print(4, 43, COMPOUND_STRING("Your party is not ready.\nReturn to a POKéMON CENTER first."));
        else
            Print(4, 43, COMPOUND_STRING("Set rules. Used supplies stay spent.\nCheck your selected supplies first."));
        Print(12, 78, COMPOUND_STRING("Yes, begin battle"));
        Print(12, 93, COMPOUND_STRING("No, return to preparation"));
        PrintRowsCursor(78, 15);
        Print(4, 121, COMPOUND_STRING("UP/DOWN choose  A confirm  B back"));
    }
    else if (sView.screen == SC_BRIEF_BAG)
    {
        Print(4, 0, COMPOUND_STRING("Boss bag supplies"));
        Print(4, 14, COMPOUND_STRING("Each occupied row is one usable unit."));
        for (u32 i = 0; i < ARRAY_COUNT(sBriefing.trainer->items); i++)
        {
            Print(9, 32 + i * 17, sBriefing.trainer->items[i] == ITEM_NONE ? COMPOUND_STRING("Empty") : GetItemName(sBriefing.trainer->items[i]));
            if (sBriefing.trainer->items[i] != ITEM_NONE)
                Print(190, 32 + i * 17, COMPOUND_STRING("x1"));
        }
        PrintRowsCursor(32, 17);
        Print(4, 121, COMPOUND_STRING("A item  L/R pages  B leave"));
    }
    else
    {
        Print(4, 0, GetTrainerNameFromId(sBriefing.trainerId));
        Print(4, 14, sBriefing.trainer->battleType == TRAINER_BATTLE_TYPE_DOUBLES ? COMPOUND_STRING("Doubles / Set") : COMPOUND_STRING("Singles / Set"));
        Number(164, 14, sBriefing.partySize);
        Print(177, 14, COMPOUND_STRING("POKéMON"));
        for (u32 i = 0; i < sBriefing.partySize; i++)
        {
            const struct TrainerMon *mon = &GetTrainerPartyFromId(sBriefing.trainerId)[i];
            Print(9, 31 + i * 13, GetSpeciesName(mon->species));
            Print(176, 31 + i * 13, COMPOUND_STRING("Lv."));
            Number(192, 31 + i * 13, mon->lvl);
        }
        PrintRowsCursor(31, 13);
        Print(4, 121, COMPOUND_STRING("A team  L/R pages  START ready  B leave"));
    }
    PutWindowTilemap(sWindow);
    CopyWindowToVram(sWindow, COPYWIN_FULL);
}

static void FinishBriefing(u8 taskId, bool32 accepted)
{
    gSpecialVar_Result = accepted;
    if (sWindow != WINDOW_NONE)
    {
        ClearStdWindowAndFrameToTransparent(sWindow, TRUE);
        RemoveWindow(sWindow);
        sWindow = WINDOW_NONE;
        for (u32 i = 0; i < WINDOWS_MAX; i++)
            if (gWindows[i].window.bg == 0 && gWindows[i].tileData)
                CopyWindowToVram(i, COPYWIN_GFX);
    }
    DestroyTask(taskId);
    ScriptContext_Enable();
}

static void Task_Briefing(u8 taskId)
{
    u32 action;
    if (gTasks[taskId].data[0] == 0)
    {
        gTasks[taskId].data[0] = 1;
        sWindow = AddWindow(&sWindowTemplate);
        if (sWindow == WINDOW_NONE)
        {
            FinishBriefing(taskId, FALSE);
            return;
        }
        SetStandardWindowBorderStyle(sWindow, FALSE);
        DrawBriefing();
        return;
    }
    action = ScBriefingNavigate(&sBriefing, &sView, gMain.newKeys);
    if (action == SC_BRIEF_ACCEPT)
    {
        // Recheck at commitment; never start doubles with one healthy Pokemon.
        if (!ScBriefingCanAccept(&sBriefing, HealthyPartyCount()))
        {
            sNeedsPartner = TRUE;
            sView.cursor = 1;
            DrawBriefing();
            return;
        }
        FinishBriefing(taskId, TRUE);
    }
    else if (action == SC_BRIEF_DECLINE)
        FinishBriefing(taskId, FALSE);
    else if (action == SC_BRIEF_REDRAW)
        DrawBriefing();
}

void ScShowBossBriefing(void)
{
    gSpecialVar_Result = FALSE;
    memset(&sView, 0, sizeof(sView));
    sWindow = WINDOW_NONE;
    sNeedsPartner = FALSE;
    ScBriefingLoad(gSpecialVar_0x8004, &sBriefing);
    // The caller's explicit waitstate executes before this task can resume it.
    CreateTask(Task_Briefing, 8);
}
#endif
