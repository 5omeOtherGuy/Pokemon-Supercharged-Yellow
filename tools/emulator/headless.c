#define _POSIX_C_SOURCE 200809L
#include <mgba/core/core.h>
#include <mgba/core/log.h>
#include <mgba-util/vfs.h>

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* No cheats, savestates, memory writes, personal config, or implicit save paths. */
static struct mCore *core;
static color_t *pixels;
static uint64_t frames;
static const char *provenance;

static void log_stderr(struct mLogger *logger, int category, enum mLogLevel level,
                       const char *format, va_list args) {
    (void) logger;
    (void) category;
    if (level & (mLOG_FATAL | mLOG_ERROR | mLOG_WARN)) {
        vfprintf(stderr, format, args);
        fputc('\n', stderr);
    }
}

static void error(const char *message) {
    printf("{\"ok\":false,\"error\":\"%s\"}\n", message);
}

static void status(void) {
    printf("{\"ok\":true,\"frames\":%" PRIu64 ",\"core_frames\":%u,"
           "\"keys\":%u,\"provenance\":\"%s\"}\n", frames,
           core->frameCounter(core), core->getKeys(core), provenance);
}

/* Numeric commands have exactly two nonnegative C-style integer operands. */
static bool pair(const char *text, uint32_t *a, uint32_t *b) {
    uint32_t *out[] = {a, b};
    for (unsigned i = 0; i < 2; ++i) {
        while (isspace((unsigned char) *text)) ++text;
        if (!isdigit((unsigned char) *text)) return false;
        errno = 0;
        char *end;
        unsigned long long value = strtoull(text, &end, 0);
        if (errno || value > UINT32_MAX || end == text ||
            (*end && !isspace((unsigned char) *end))) return false;
        *out[i] = (uint32_t) value;
        text = end;
    }
    while (isspace((unsigned char) *text)) ++text;
    return !*text;
}

static void cleanup(void) {
    if (core) {
        mCoreConfigDeinit(&core->config);
        core->deinit(core); /* closes the normal save VFile and ROM */
    }
    free(pixels);
}

int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IOLBF, 0);
    if (argc != 3) {
        fprintf(stderr, "Usage: %s ROM.gba TEST-SAVE.sav\n", argv[0]);
        return 2;
    }
    struct stat rom_stat, save_stat;
    bool existed = stat(argv[2], &save_stat) == 0;
    if (stat(argv[1], &rom_stat) != 0 || !S_ISREG(rom_stat.st_mode) ||
        (existed && (!S_ISREG(save_stat.st_mode) ||
        (rom_stat.st_dev == save_stat.st_dev && rom_stat.st_ino == save_stat.st_ino)))) {
        error("ROM must exist; save must be a separate regular file");
        return 2;
    }
    provenance = existed ? "existing-save-unverified" : "fresh-input-only";
    struct mLogger logger = {.log = log_stderr, .filter = NULL};
    mLogSetDefaultLogger(&logger);
    core = mCoreFind(argv[1]);
    if (!core || core->platform(core) != mPLATFORM_GBA) {
        if (core) free(core);
        error("unsupported GBA ROM");
        return 2;
    }
    if (!core->init(core)) {
        free(core);
        error("emulator initialization failed");
        return 2;
    }
    mCoreInitConfig(core, "supercharged-headless");
    /* Do not load any machine/user config or BIOS; use mGBA's built-in BIOS. */
    mCoreConfigSetDefaultIntValue(&core->config, "mute", 1);
    mCoreConfigSetDefaultIntValue(&core->config, "skipBios", 1);
    mCoreConfigSetDefaultIntValue(&core->config, "useBios", 0);
    mCoreLoadForeignConfig(core, &core->config);
    if (!mCoreLoadFile(core, argv[1]) || !mCoreLoadSaveFile(core, argv[2], false)) {
        error("could not load ROM or explicit save path");
        cleanup();
        return 2;
    }
    unsigned width, height;
    core->desiredVideoDimensions(core, &width, &height);
    pixels = calloc((size_t) width * height, sizeof(*pixels));
    if (!pixels) {
        error("video allocation failed");
        cleanup();
        return 2;
    }
    core->setVideoBuffer(core, pixels, width);
    core->reset(core);
    status();

    char line[8192];
    while (fgets(line, sizeof(line), stdin)) {
        size_t length = strlen(line);
        if (length == sizeof(line) - 1 && line[length - 1] != '\n') {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            error("command too long");
            continue;
        }
        while (length && (line[length - 1] == '\n' || line[length - 1] == '\r'))
            line[--length] = 0;
        uint32_t a, b;
        if (!strcmp(line, "quit")) {
            /* Acknowledge only after normal save teardown has completed. */
            cleanup();
            puts("{\"ok\":true}");
            return 0;
        } else if (!strcmp(line, "status")) {
            status();
        } else if (!strcmp(line, "release")) {
            core->setKeys(core, 0);
            status();
        } else if (!strcmp(line, "reset")) {
            core->setKeys(core, 0);
            core->reset(core);
            frames = 0;
            memset(pixels, 0, (size_t) width * height * sizeof(*pixels));
            status();
        } else if (!strncmp(line, "run ", 4)) {
            if (!pair(line + 4, &a, &b) || a > 36000 || b > 0x3ff) {
                error("run requires frames 0..36000 and key mask 0..1023");
                continue;
            }
            core->setKeys(core, b);
            for (uint32_t i = 0; i < a; ++i) core->runFrame(core);
            frames += a;
            status();
        } else if (!strncmp(line, "read ", 5)) {
            if (!pair(line + 5, &a, &b) || !b || b > 4096 ||
                a > 0x0fffffff || b - 1 > 0x0fffffff - a) {
                error("read requires GBA address and length 1..4096");
                continue;
            }
            printf("{\"ok\":true,\"address\":%u,\"hex\":\"", a);
            for (uint32_t i = 0; i < b; ++i)
                printf("%02x", (unsigned) core->busRead8(core, a + i) & 255);
            puts("\"}");
        } else if (!strncmp(line, "screenshot ", 11) && line[11]) {
            struct VFile *vf = VFileOpen(line + 11, O_WRONLY | O_CREAT | O_EXCL);
            if (!vf) {
                error("cannot create screenshot; path must not exist");
                continue;
            }
            bool ok = mCoreTakeScreenshotVF(core, vf);
            ok = vf->close(vf) && ok;
            if (ok) status();
            else error("PNG write failed");
        } else {
            error("unknown command or invalid operands");
        }
    }
    cleanup();
    return ferror(stdin) ? 1 : 0;
}
