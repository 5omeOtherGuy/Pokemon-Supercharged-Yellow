#include "global.h"
#include "bg.h"
#include "clear_save_data_menu.h"
#include "decompress.h"
#include "event_data.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "intro.h"
#include "m4a.h"
#include "main.h"
#include "main_menu.h"
#include "malloc.h"
#include "menu.h"
#include "palette.h"
#include "reset_rtc_screen.h"
#include "scanline_effect.h"
#include "sc_title_controls.h"
#include "sound.h"
#include "task.h"
#include "text.h"
#include "trainer_pokemon_sprites.h"
#include "window.h"
#include "util.h"
#include "constants/rgb.h"
#include "constants/songs.h"

#if IS_FRLG

// Reuse the included, version-neutral Pokémon logo and original Pikachu art.
// No campaign data, save data or random Pokémon are created by this screen.
#define TITLE_SPECIES SPECIES_PIKACHU
#define TITLE_IDLE_FRAMES 2700
#define TITLE_WINDOW 0
#define TITLE_INK 1
#define TITLE_GOLD 2
#define TITLE_PAPER 3

static EWRAM_DATA u16 sLogoTilemap[32 * 32] = {};
static EWRAM_DATA u16 sPikachuSpriteId = 0;

static const struct BgTemplate sTitleBgs[] =
{
    {.bg = 0, .charBaseIndex = 0, .mapBaseIndex = 31, .paletteMode = 1, .priority = 0},
    {.bg = 1, .charBaseIndex = 2, .mapBaseIndex = 30, .paletteMode = 0, .priority = 1},
};

static const struct WindowTemplate sTitleWindows[] =
{
    {.bg = 1, .tilemapLeft = 0, .tilemapTop = 0, .width = 30, .height = 20, .paletteNum = 15, .baseBlock = 1},
    DUMMY_WIN_TEMPLATE,
};

static const u16 sTitlePalette[] =
{
    RGB(31, 27, 9), RGB(3, 6, 12), RGB(23, 17, 4), RGB(31, 30, 24),
};
static const u8 sTitleColors[] = {TEXT_COLOR_TRANSPARENT, TITLE_INK, TITLE_GOLD};
static const u8 sFooterColors[] = {TEXT_COLOR_TRANSPARENT, TITLE_PAPER, TITLE_INK};
static const u8 sText_Supercharged[] = _("SUPERCHARGED");
static const u8 sText_Yellow[] = _("YELLOW");
static const u8 sText_PressStart[] = _("PRESS START");
static const u8 sText_Copyright[] = _("(C)1995-2004 GAME FREAK inc.");

static void CB2_TitleScreenRun(void);
static void VBlankCB_TitleScreen(void);
static void Task_TitleScreen(u8 taskId);

static void PrintCentered(u8 font, const u8 *text, u16 center, u8 y, s8 spacing, const u8 *colors)
{
    s32 width = GetStringWidth(font, text, spacing);
    AddTextPrinterParameterized4(TITLE_WINDOW, font, center - width / 2, y, spacing, 0, colors, TEXT_SKIP_DRAW, text);
}

static void DrawTitleText(void)
{
    FillWindowPixelBuffer(TITLE_WINDOW, PIXEL_FILL(0));
    // A small inset electric motif anchors the wordmark; the footer is stable
    // while the start prompt gently changes color without disappearing.
    FillWindowPixelRect(TITLE_WINDOW, PIXEL_FILL(TITLE_GOLD), 22, 117, 104, 1);
    FillWindowPixelRect(TITLE_WINDOW, PIXEL_FILL(TITLE_INK), 0, 128, 240, 32);
    PrintCentered(FONT_SMALL, sText_Supercharged, 75, 76, 0, sTitleColors);
    PrintCentered(FONT_NORMAL, sText_Yellow, 75, 94, 2, sTitleColors);
    PrintCentered(FONT_NORMAL, sText_PressStart, 120, 129, 0, sFooterColors);
    PrintCentered(FONT_SMALL, sText_Copyright, 120, 147, 0, sFooterColors);
    PutWindowTilemap(TITLE_WINDOW);
    CopyWindowToVram(TITLE_WINDOW, COPYWIN_FULL);
}

static void LoadTitleGraphics(void)
{
    u32 x, y;
    // Source sheet is 256x64, with visible logo pixels in x=3..168. Use its
    // first 22 columns unchanged and center those pixels at the native width.
    // Tile 255 is transparent; the unused sheet cells never enter the scene.
    for (x = 0; x < ARRAY_COUNT(sLogoTilemap); x++)
        sLogoTilemap[x] = 255;
    for (y = 0; y < 8; y++)
        for (x = 0; x < 22; x++)
            sLogoTilemap[y * 32 + x + 4] = y * 32 + x;
    SetBgTilemapBuffer(0, sLogoTilemap);
    DecompressAndCopyTileDataToVram(0, gTitleScreenPokemonLogoGfx, 0, 0, 0);
    CopyBgTilemapBufferToVram(0);
    LoadPalette(gTitleScreenBgPalettes, BG_PLTT_ID(0), 14 * PLTT_SIZE_4BPP);
    LoadPalette(sTitlePalette, BG_PLTT_ID(0), sizeof(u16));
    LoadPalette(sTitlePalette, BG_PLTT_ID(15), sizeof(sTitlePalette));
    InitWindows(sTitleWindows);
    DeactivateAllTextPrinters();
    DrawTitleText();
    ResetAllPicSprites();
    sPikachuSpriteId = CreateMonPicSprite(TITLE_SPECIES, FALSE, 0, TRUE, 175, 95, 0, TAG_NONE);
    if (sPikachuSpriteId < MAX_SPRITES)
    {
        gSprites[sPikachuSpriteId].oam.priority = 0;
        gSprites[sPikachuSpriteId].animPaused = TRUE;
    }
}

void CB2_InitTitleScreenFrlg(void)
{
    switch (gMain.state)
    {
    default:
        gMain.state = 0;
        // fallthrough
    case 0:
        SetVBlankCallback(NULL);
        StartTimer1();
        InitHeap(gHeap, HEAP_SIZE);
        ResetTasks();
        ResetSpriteData();
        FreeAllSpritePalettes();
        ResetPaletteFade();
        ScanlineEffect_Stop();
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        DmaFill16(3, 0, (void *)VRAM, VRAM_SIZE);
        DmaFill32(3, 0, (void *)OAM, OAM_SIZE);
        DmaFill16(3, 0, (void *)PLTT, PLTT_SIZE);
        ResetBgsAndClearDma3BusyFlags(FALSE);
        InitBgsFromTemplates(0, sTitleBgs, ARRAY_COUNT(sTitleBgs));
        ResetBgPositions();
        SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        gMain.state++;
        break;
    case 1:
        LoadTitleGraphics();
        gMain.state++;
        break;
    case 2:
        if (!FreeTempTileDataBuffersIfPossible())
        {
            BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
            ShowBg(0);
            ShowBg(1);
            CreateTask(Task_TitleScreen, 4);
            SetVBlankCallback(VBlankCB_TitleScreen);
            SetMainCallback2(CB2_TitleScreenRun);
            m4aSongNumStart(MUS_TITLE);
        }
        break;
    }
}

static void CB2_TitleScreenRun(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_TitleScreen(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void ReleaseTitleScreen(void)
{
    SetVBlankCallback(NULL);
    if (sPikachuSpriteId < MAX_SPRITES)
        FreeAndDestroyMonPicSprite(sPikachuSpriteId);
    sPikachuSpriteId = 0xFFFF;
    FreeAllWindowBuffers();
    UnsetBgTilemapBuffer(0);
    UnsetBgTilemapBuffer(1);
    ScanlineEffect_Stop();
}

#define tState data[0]
#define tTimer data[1]
#define tAction data[2]
#define tExitTimer data[3]

static void Task_TitleScreen(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (tState == 0)
    {
        // A/B/Start can skip the short reveal, as on the inherited title.
        if (JOY_NEW(A_BUTTON | B_BUTTON | START_BUTTON))
        {
            ResetPaletteFade();
            BlendPalettes(PALETTES_ALL, 0, RGB_BLACK);
        }
        if (!gPaletteFade.active)
            tState = 1;
        return;
    }
    if (tState == 1)
    {
        enum ScTitleAction action;
        tTimer++;
        if (sPikachuSpriteId < MAX_SPRITES)
            gSprites[sPikachuSpriteId].y2 = (tTimer % 120 >= 90) ? -1 : 0;
        // Only paper-color footer pixels breathe, retaining readable contrast.
        BlendPalette(BG_PLTT_ID(15) + TITLE_PAPER, 1, (tTimer % 120 >= 80) ? 4 : 0, sTitlePalette[0]);
        action = ScTitleChooseAction(gMain.heldKeys, gMain.newKeys, CanResetRTC(), tTimer >= TITLE_IDLE_FRAMES);
        if (action == SC_TITLE_WAIT)
            return;
        tAction = action;
        tState = 2;
        if (action == SC_TITLE_ENTER)
            PlayCry_Normal(TITLE_SPECIES, 0);
        else
        {
            FadeOutBGM(4);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            tState = 3;
        }
    }
    else if (tState == 2)
    {
        if (++tExitTimer >= 90)
        {
            FadeOutBGM(4);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_WHITE);
            tState = 3;
        }
    }
    else if (!gPaletteFade.active && IsNotWaitingForBGMStop())
    {
        enum ScTitleAction action = tAction;
        ReleaseTitleScreen();
        DestroyTask(taskId);
        if (action == SC_TITLE_ENTER)
            SetMainCallback2(CB2_InitMainMenu);
        else if (action == SC_TITLE_CLEAR_SAVE)
            SetMainCallback2(CB2_InitClearSaveDataScreen);
        else if (action == SC_TITLE_RESET_RTC)
        {
            m4aMPlayAllStop();
            SetMainCallback2(CB2_InitResetRtcScreen);
        }
        else
            SetMainCallback2(CB2_InitCopyrightScreenAfterTitleScreen);
    }
}

#undef tState
#undef tTimer
#undef tAction
#undef tExitTimer

#endif // IS_FRLG
