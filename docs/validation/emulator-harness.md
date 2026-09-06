# Real mGBA headless validation

This harness executes a locally built GBA ROM in desktop libmgba. It provides deterministic frame/input control, unscaled PNG capture, memory inspection and ordinary cartridge-save persistence. It does not implement or simulate game mechanics.

## Build and run

Validated compilation environment: Ubuntu libmgba-dev / libmgba 0.10.2 and GCC. The installed mGBA library must include PNG support (the Ubuntu package does). No SDL window, display server, proprietary BIOS or Python dependency is needed by the driver.

From the repository root:

```bash
mkdir -p build/emulator
cc -std=c11 -Wall -Wextra -Werror -O2 \
  -o build/emulator/headless tools/emulator/headless.c -lmgba
build/emulator/headless /absolute/path/to/locally-built.gba \
  build/emulator/isolated-test.sav
```

The ROM is loaded from the first argument. The second argument is mandatory: it is the exact normal cartridge save file to create or resume. Use only disposable test saves in the ignored `build/` directory, with a different save path for each concurrent process. Quit gracefully to finish mGBA's normal save teardown; do not kill the process immediately after an in-game save. ROMs and saves must never be committed.

The process reads commands from stdin and emits one JSON response per command, plus an initial ready response. Emulator warnings go to stderr. It remains paused between commands; wall-clock waiting does not advance the game. Use a subprocess with pipes or a terminal. The process is deliberately single-ROM: restart it with another ROM to change games.

```text
run 180 0
screenshot build/emulator/boot.png
run 1 8
run 60 0
read 0x04000130 2
status
release
reset
quit
```

| Command | Behavior |
| --- | --- |
| `run N MASK` | Set the held buttons, then execute exactly N `runFrame` calls; N is 0–36000. Buttons remain held afterward. |
| `release` | Release every button without advancing a frame. |
| `status` | Report total frames since reset, core frame counter, held mask and save provenance. |
| `screenshot PATH` | Write the current 240×160 frame as PNG. PATH is the entire remainder of the line, permitting spaces; it must not already exist. Parent directories must exist. |
| `read ADDRESS LENGTH` | Inspect 1–4096 bytes using mGBA's emulated bus. Return lowercase hex in increasing address order. Addresses must fit the GBA 28-bit bus. |
| `reset` | Release buttons and reset the emulator; reset the harness frame count and clear its video buffer. Preserve normal cartridge save data. Advance frames before capturing a new image. |
| `quit` | Close the emulator and its save file, acknowledge completion, then exit. EOF also closes normally. |

Numeric operands accept decimal or C-style hexadecimal (a leading `0` means octal). Button masks are combined with bitwise OR: A=1, B=2, Select=4, Start=8, Right=16, Left=32, Up=64, Down=128, R=256, L=512. A tap generally needs a pressed frame followed by released frames; dialogs may ignore a button until their animation finishes. Screenshots are evidence of actual rendered frames, not assertions that a requested interaction succeeded.

Errors return `{"ok":false,"error":"..."}` and leave the process available. Unknown commands, excess operands, out-of-range numbers and overlong lines are rejected. Memory reads should target known source symbols or hardware registers; arbitrary MMIO reads may have hardware semantics and are not proof of gameplay. Match symbol addresses to the exact ROM build.

## Evidence provenance

The initial response and every status response label sessions as `fresh-input-only` when the save path did not exist at launch, or `existing-save-unverified` when it did. This driver has no memory-write, cheat, savestate-load or save-injection command and does not load machine emulator configuration, patches, cheats or a neighboring implicit save. Thus a fresh session progresses through normal game inputs. A preexisting save might have been modified by another tool; its provenance must be documented independently before using it as organic gameplay evidence.

Preserve the command transcript, ROM SHA-256, driver revision, libmgba package version, screenshots, and the provenance/derivation of any reused save. If another tool prepares a debug fixture, explicitly identify its mutations and label subsequent results as fixture-based. A fixture can verify a targeted code path but cannot establish that a player can reach it. The session label does not detect external writes or prove an upstream ROM was built without debug modifications.

## Tests and limits

```bash
EMULATOR_TEST_ROM=/absolute/path/to/locally-built.gba \
  python3 tests/emulator/test_harness.py
```

The test compiles the driver, boots the supplied real ROM, checks frame totals and the actual active-low GBA key register, inspects PNG dimensions, compares reset/restart screenshots, verifies explicit save creation/provenance and checks protocol rejection. Test directories and saves are created under `build/emulator` and removed afterward. These are harness checks; creating a save file alone does not establish a successful in-game save/load. A separate gameplay transcript must reach the game's Save command and show the same player state after a new process starts and Continue is selected.

Desktop libmgba validation is not Android/RetroArch validation, an audio-quality check, a human playtest, proof of enjoyable balance, or complete campaign coverage. Built-in BIOS behavior is the only BIOS configuration exercised. Frame stepping measures emulated frames, not Android performance or a claimed battle-speed multiplier. Reset determinism applies to the tested ROM/input sequence; cartridges using a real-time clock may need a separate controlled-clock policy.

## Recorded verification (2026-09-06)

GCC strict compilation and all three integration tests passed against the locally built foundation ROM (SHA-256 `ae61d62c3387de2c4e7fc5e4df174372e6f2da7b72f7e1bcb072820dd937a821`). The tests replay 780 frames, covering a visible Game Freak splash rather than the earlier black transition. GCC coverage reported 92.74% of the driver's 124 executable lines. This covers the driver, not mGBA or the game's mechanics. The desktop package was `libmgba-dev` version `0.10.2+dfsg-1.1build3`.

Visual inspection confirmed the Game Freak splash, animated Gengar/Nidorino intro, FireRed title screen, New Game menu and opening control tutorial. This unmodified foundation ROM still identifies itself as FireRed; these observations do not establish the requested Yellow adaptations. Successful gameplay save/reload is a separate check.

Coverage can be reproduced with `EMULATOR_TEST_CFLAGS='--coverage -O0'` on the test command, then `gcov -o build/emulator tools/emulator/headless.c`. Move the generated `.gcov` reports into ignored `build/emulator/` afterward.

## API evidence

Inspected on 2026-09-06: installed `/usr/include/mgba/core/core.h`, `config.h`, `log.h`, `/usr/include/mgba-util/vfs.h`, and mGBA tag **0.10.2**. [Core file/save/screenshot implementation](https://github.com/mgba-emu/mgba/blob/0.10.2/src/core/core.c) establishes the explicit save loading and built-in PNG APIs. [GBA core implementation](https://github.com/mgba-emu/mgba/blob/0.10.2/src/gba/core.c) supplies emulation, reset, keys and memory access. The harness links the system library and does not copy upstream emulator source.
