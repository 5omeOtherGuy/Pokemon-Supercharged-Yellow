# Battle pacing: actual emulator checks

Source `57d08b70` implements NORM, FAST and QUICK in Options under [ADR-0032](../adr/0032-battle-pacing-presets.md). The real ARM helper test passes, the production build succeeds, and all three presets were selected through the actual GBA menu. Text and choices fit the 240×160 screen. The latest combined native suite executes119 groups:109 pass and10 are intentional framework checks, with no unexpected failures or skips.

## Measured sequence

A disclosed disposable ROM fixture replaces Mom's interaction with creation of an Adamant male level-five Pikachu, all IV15, followed by a level-two wild Magikarp. Actual engine scripts generate the Pokémon and start the battle. The ordinary mGBA driver sends controls and reads state; it never injects RAM. This is actual game execution with diagnostic setup, not organic encounter access or human playtesting.

All runs use MID text, battle animations ON, identical initial party bytes, ordinary Options input, and Thunder Shock. The test presses A for one frame every30 emulated frames to advance messages. Measurement begins with move acceptance and ends when `gMain.inBattle` clears. **The final field redraw, battle introduction and decision time are excluded.** The original generated result/PNG labels say “return to field”; inspection showed a black transition at that boundary, so the manifest and this record state the precise measurement. A subsequent runtime replay visibly returned to the house.

| Preset | Stored value | Measured frames | Ratio to NORM |
| --- | ---: | ---: | ---: |
| NORM | 1 | 951 | 1.000 |
| FAST | 2 | 891 | 0.937 |
| QUICK | 4 | 831 | 0.874 |

Every run ended in victory. The complete100-byte resulting party record is identical across the three runs, including HP, PP, EXP and training metadata. Inspection shows Thunder Shock's animation, resulting battle messages, and all three selected Options states. These measurements are specific to this one-hit sequence and the declared input cadence. They do not imply exact2×/4× whole-battle acceleration, general turn timing, readable text for every player, or the required20–30+minute recruit preparation target.

## Cartridge setting persistence

A separate fresh replay selected QUICK and completed the fixture battle. It used the normal Save menu and overwrite confirmation, then quit gracefully. A new emulator process offered Continue, restored the house state and the exact48-byte trainer record, and rendered QUICK selected in Options. SRAM SHA256 remained `819b5606616e74ed750ed8ab7176f8879ff402282df7369c83f20bd667100f6a` across that restart. No save or state injection was used. The final save-completion text was not captured; the normal Continue restoration establishes persistence.

The fixture build also exposed premature Tempo ownership from the ship rival's initial hide flag. That separate defect was reproduced by the native regression (`11` unlocked mask versus expected `3`) and fixed in `d323369b` using real rival-victory flags. Its active mask was zero during pacing measurements. The frozen fixture and its saved record were retained unchanged; they do not represent that later fix.

Earlier operator trials are retained under ignored `build/pace/`: one selected Growl while waiting for a battle victory, another omitted the A input needed to advance the faint message, and an initial save trial stopped at overwrite confirmation. They are setup/protocol failures, not game defects or successful persistence results. Corrected commands and actual results are committed below. No audio check was performed: the headless driver mutes audio. Android and human readability/preparation validation remain required, along with additional move/format animation checks and title-menu option carry into a new game.

## Reproduction and evidence

The [manifest](../../tests/pace/evidence/57d08b70/manifest.json) pins source/fixture hashes, emulator version, measurement boundary and every retained file. Commands, actual responses, measurements, selected native screenshots and save/restart trace are committed; ROM/ELF/SRAM are not. Local source ROM SHA256 is `d6bcb4354386456fd2a36bd857df0b7e4876954981f6a79f900f03d10c47da7e`; matching ELF SHA256 is `1e783e95250e0e9e6addb82068583a4d277d9f2649f9d9408cc1651377a50dcd`.

```sh
python3 tests/pace/verify_evidence.py
# Build the ordinary driver as documented in emulator-harness.md first.
python3 tests/pace/make_diagnostic_fixture.py --rom /path/to/source.gba \
  --elf /path/to/source.elf --sha256 SOURCE_SHA256 \
  --output build/pace/new-fixture.gba > build/pace/new-fixture.json
python3 tests/pace/run_timing.py --rom build/pace/new-fixture.gba \
  --elf /path/to/source.elf --fixture-manifest build/pace/new-fixture.json \
  --output build/pace/new-measurements
bash tests/effects/run_native.sh
```

Use absent output directories and a matching artifact. The verifier checks retained hashes, successful command/response correspondence, frame arithmetic, identical party results and restored setting; it does not itself replay the game. The runner creates isolated fresh SRAM paths for each preset. A new source build can change navigation or learnsets; inspect its actual screenshots before interpreting measurements.
