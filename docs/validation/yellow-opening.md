# Yellow opening: automated emulator gameplay

The frozen production build at `195bd43e` passed the opening through the first
rival battle, Oak's Parcel/Pokédex, a first catch, Viridian Center development
services, PC deposit/withdrawal, and two ordinary save/new-process Continue
checks. This is actual libmgba gameplay driven by buttons, not a simulated game
or a human enjoyment/balance assessment. It is not Android validation.

One progression defect is visible: **Tempo is already Owned before the S.S.
Anne rival victory**. No trainer passive was assigned in this journey. The
coordinator identified the cause as using `FLAG_HIDE_SS_ANNE_RIVAL`, initialized
by the new-game script, as evidence of victory. A later source fix is separate
from this frozen artifact's evidence. No fix was applied to this run or save.

## Artifact and provenance

- Source: `195bd43e`.
- ROM SHA256: `5013582bc8a35db93d8432a1a1144f1a65423e6992627c8d4384950904d2e765`.
- Matching ELF SHA256: `6a0e6c545f5ea2f60a8aede736a148398c5e6705d99508bf6ceb890e0dae2efe`.
- Frozen input: `full-game/build/sc-services-runtime/sc-services.gba` under the
  sibling worktrees directory; the mutable engine output was never used.
- Emulator: Ubuntu libmgba `0.10.2+dfsg-1.1build3`; the existing
  [headless driver](../../tools/emulator/headless.c), compiled with
  `cc -std=c11 -Wall -Wextra -Werror -O2 -o build/emulator/headless tools/emulator/headless.c -lmgba`.
- The explicit `build/emulator/yellow-opening/organic.sav` path did not exist at
  first launch. The first process reported `fresh-input-only`. Subsequent
  processes correctly reported `existing-save-unverified`; provenance comes
  from the recorded prior process and unchanged save hash, not that label.
- No cheats, injected saves, savestates, reset, memory writes, gameplay debug
  fixtures, or personal saves were used. The nineteen `read` commands are
  read-only navigation probes. Matching ELF/map symbols and source coordinate
  layouts were inspected. One stale SaveBlock pointer read returned zeros and
  was not used to establish a result. Screens establish the visible checkpoints.

The [manifest](../../tests/emulator/yellow-opening/manifest.json) records source,
artifact/save hashes, per-process counts, original terminal-transcript hashes,
and hashes of committed commands, responses and screenshots. All 1,251 JSON
responses report success. All 1,248 commands are retained, including navigation
detours and redundant button presses. The three final `quit` commands completed
normally. Recorded frame totals exactly equal the sum of requested run frames.

| Process | Frames | Commands | Screenshots captured |
| --- | ---: | ---: | ---: |
| fresh | 87,820 | 844 | 117 |
| continue | 31,922 | 388 | 48 |
| center-continue | 3,138 | 16 | 4 |

Thirty-six selected screenshots are committed. All 169 originals, the raw
`script --quiet --flush --return` transcripts, driver and SRAM remain in the
ignored local `build/emulator/yellow-opening/` directory for continuation. ROM,
ELF and SRAM are not committed. Screenshot filenames are operator labels at the
time of capture; a label such as `save-complete-check` is not itself a claim that
the operation had completed.

## Observed checkpoints

| Check | Evidence in `tests/emulator/yellow-opening/screens/` |
| --- | --- |
| Oak catches wild Pikachu on leaving Pallet | `022-pikachu-capture.png` |
| Rival takes Eevee; Pikachu leaves its ball | `025-eevee.png`, `027-pikachu-out.png` |
| Actual level-5 Pikachu/Eevee battle | `029-rival-eevee.png`, `032-move-choice.png` |
| Thunder Shock wins; Pikachu ends at 1/19 HP and reaches level 6 | `038-first-battle-result.png`, `039-level-six.png` |
| Script restores Pikachu to 21/21 HP | `049-party-healed.png` |
| Viridian nurse heals normally; upstairs lounge opens | `075-lounge-welcome.png`, `077-services-home.png` |
| Public training display: level 6, ceiling 24, Speed training 1, balanced focus | `079-training-screen.png` |
| Parcel received and returned; Oak supplies Poké Balls | `082-oaks-parcel.png`, `094-parcel-delivered.png`, `097-poke-balls.png` |
| Level-4 Pidgey caught with first ordinary Poké Ball; Pokédex registration | `104-first-throw.png`, `106-pokedex-entry.png` |
| Both party members present at full HP | `108-two-party.png` |
| Explicit save completion; fresh process Continue restores location and party | `117-save-complete.png`, `118-continue-menu.png`, `119-continued-route.png`, `123-party-restored.png` |
| Pidgey deposited to Box 1, inspected there, then withdrawn | `149-deposit-pidgey.png`, `150-box-choice.png`, `151-stored.png`, `154-box-one-pidgey.png`, `155-pidgey-withdrawn.png` |
| Premature Tempo ownership; 0/2 trainer slots used | `161-trainer-slots.png` |
| Second explicit save completion; third process restores Center and both party members | `165-center-saved.png`, `167-center-restored.png`, `169-withdrawn-party-persisted.png` |

The first battle is one observed successful outcome, not a win-rate estimate.
The Pikachu/Eevee battle used Thunder Shock; paralysis occurred and Pikachu
survived with 1 HP. One Route 1 wild Pidgey battle was won before the parcel
return; later encounters were escaped. The caught Pidgey was captured at full HP.
No RNG resets were used. The first Center service screen showed 120 Practice
Points; no points, training assignment, capability, ability, or trainer build
was changed through those menus.

The inherited FireRed/Charizard title remains visible in `001-title.png` despite
the Yellow opening scripts. The visited first-floor nurse dialogue did not
advertise the development lounge upstairs; discovering it required exploring
the former cable-club floor. These are presentation/discovery observations.

## Save persistence

At the first checkpoint, the game displayed **A saved the game** at frame 87,640.
After another 180 released frames the driver quit. The 131,088-byte SRAM had
SHA256 `7221d61ce78f293ed67a509fe4c4506812142d3b92caa76743f16569de2fed45`.
A newly started process offered Continue, restored Route 1, and showed both
party members with matching levels and HP. The file hash remained unchanged
through that reload inspection.

After PC withdrawal and the trainer-menu inspection, the second process saved
at Viridian Center upstairs. It displayed **A saved the game** at frame 31,742,
ran another 180 released frames, and quit. SRAM SHA256 became
`f2729534c657395903623ec04184b9d4836a3785c417061faec69edf11f67156`.
A third process restored that location and both party members. Its graceful
quit left the same save hash unchanged. No save was inferred complete solely
from a fixed wait or the existence of a `.sav` file: screenshots also preserve
the preceding SAVING messages.

## Replay and continuation

The three `.commands` files are the exact accepted input stream, with matching
`.responses.jsonl` output. To replay, compile the driver, use the same verified
ROM and an absent explicit SRAM path, and run each command file in order through
a separate driver process sharing that SRAM. Use a fresh working directory
containing `build/emulator/yellow-opening/`, because screenshot commands refuse
to overwrite existing files. For example, from that empty working directory:

```sh
/path/to/headless /path/to/sc-services.gba ./organic.sav < /path/to/fresh.commands
/path/to/headless /path/to/sc-services.gba ./organic.sav < /path/to/continue.commands
/path/to/headless /path/to/sc-services.gba ./organic.sav < /path/to/center-continue.commands
```

This command extraction was checked against executed responses; a separate
whole-trace replay has not been run. RTC/process timing can affect encounters,
so inspect checkpoints when replaying rather than assuming identical gameplay
from a different environment. Route 2, Viridian Forest, Pewter and Brock have
not yet been exercised by this checkpoint. Continuing this SRAM on a newer
artifact must record both artifact hashes and the save handoff hash; it cannot
be presented as a single final-ROM fresh journey. Existing premature Tempo
ownership is an internal-build legacy and must not be silently erased.
