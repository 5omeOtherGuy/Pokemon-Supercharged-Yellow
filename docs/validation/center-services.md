# Center and field development services

`ScShowCenterServices` opens the native development screen from the shared upstairs lounge script in twelve mainland Centers, including Indigo's entry Center. All service mutations recheck the actual map and reject battle contexts. League battle rooms are not designated services. One downstairs NPC in each corresponding Center explains where to prepare; obsolete trading/Union Room recommendations in the selected Cerulean, Pewter and Cinnabar dialogues are replaced with the service direction.

`ScShowFieldTraining(partyIndex, returnCallback)` accepts a valid non-egg original Pokémon outside battle and takes over on its next main callback. Its caller closes party-menu resources after a true result; its return callback rebuilds the party menu. The field screen permits inspection and future-focus changes. Capability/passive assignment, ability selection, move relearning and IV purchases are unavailable there. The coordinator owns the party-menu entry and callback wrapper.

## Player behavior

The Center home menu separates Pokémon development, the trainer's build, and battle supplies. Pokémon training displays six permanent point totals, the current badge-owned ceiling and current focus. Balanced focus directs future rewards to the least-trained stat; six single-stat focuses are available. Changing focus never moves existing points. The focus confirmation and scrollable help explain that a full focused stat discards rewards, including fractional carry; balanced focus discards only when every stat is full.

Capability lists show this Pokémon's owned/active effects and their costs within three points. The trainer list separately shows owned/active trainer effects and the badge-plus-two budget, capped at ten. Effect details use the shared canonical catalog; A assigns/removes an owned effect for free in the Center. Invalid masks, unavailable ownership and excess costs leave the existing assignment intact. Partial builds are legal. SELECT from any effect detail opens always-accessible rules explaining calculated-damage multipliers and unchanged fixed, level-based, reflected, fractional and OHKO damage.

Ability choices come from the species' actual official slots, including hidden abilities, omitting absent and duplicate entries. Canonical descriptions are inspectable before selection. The acquired nature stays fixed. Free move relearning lists distinct forgotten moves from the current species' level-up learnset at or below its current level, including evolution moves, with no future-level, egg, tutor or TM grant. The selected replacement slot is explicit; PP bonuses for the forgotten move are removed and the learned move receives its normal PP. Legality and duplicate checks run again at commitment.

**Practice Points** are the trainer-wide resource stored in `practiceExp`. The IV service shows the balance and current price, then requires an explicit Yes (default No) to improve one selected IV by one. The price is `max(20, 4 × current level)`; an IV cannot exceed 31. Insufficient funds and invalid targets spend nothing. A successful improvement preserves acquired nature, other IVs, permanent training and current HP deficit; a fainted Pokémon remains fainted. The coordinator owns random-acquisition IV safeguards and battle-win Practice Point earnings.

Supplies are displayed as three individual unit rows. The picker offers owned legal supplies; duplicate units are consolidated to item/quantity pairs before calling the shared atomic `ScSuppliesSetSelection` API. Total capacity is three, with HP heals at most two and status/PP/boost categories at most one each. Revives and mixed-category items are excluded. Bringing fewer units or clearing all rows is legal. The UI explains entry-time inventory checks and retained consumption after losses; the supplies subsystem owns locking, consumption and battle actions.

The screen uses the existing small font, standard frame and a dedicated BG0/window allocation. Long descriptions and help scroll; lists accept held D-pad movement. Exit frees its window resources and returns through the normal field reconstruction/continue-script path or supplied party reconstruction callback.

## Checks and evidence boundary

```sh
python3 -m unittest discover -s tests/services
SC_SERVICES_COVERAGE=1 python3 -m unittest discover -s tests/services
python3 tests/services/compile_objects.py --generated-engine /absolute/path/to/prepared/engine
```

The host check compiles the production service transaction model and progression arithmetic with narrow native game adapters. It covers ownership, weighted budgets, invalid masks and duplicates, field/battle rejection, focus permanence, legal ability slots, IV limits/costs/atomic rejection, preserved HP deficit and fainting, fixed nature, current-level move availability and explicit replacement. It first failed on missing service APIs. A later realistic map-encoding fixture exposed and then verified the correction of swapped group/number bytes. Latest model coverage: 90.97% of 155 lines, all 130 branches executed, 77.69% of branch outcomes taken. These metrics exclude the renderer and engine adapters.

Both production translation units and `engine/test/sc_services/transactions.c` compile through real game text preprocessing and the ARM compiler with warnings treated as errors. The coordinator reports that both native service groups passed in its integrated thirteen-group mGBA run: encrypted IV improvement with nature/damage preservation, and field rejection plus focus permanence. Source/runtime artifact references are to be attached with the integrated build evidence.

## Actual native runtime evidence

The production artifact from source `195bd43e` was exercised through libmGBA and an isolated headless diagnostic driver. ROM SHA-256: `5013582bc8a35db93d8432a1a1144f1a65423e6992627c8d4384950904d2e765`; ELF SHA-256: `6a0e6c545f5ea2f60a8aede736a148398c5e6705d99508bf6ceb890e0dae2efe`. This includes the SELECT effect-rules page and predates the held-D-pad change in `bbc1ff2c`.

[Evidence manifest](../../tests/services/evidence/195bd43e/manifest.json) pins 30 native 240×160 screenshots, all 582 successful commands and responses, the ROM fixture checksum and each explicit state-injection manifest. Run `python3 tests/services/verify_runtime_evidence.py` to verify file hashes, trace completeness and the observed encrypted Pokémon/trainer-state invariants. This verifies the recorded observations; it does not replay the game.

The disposable ROM changes only Mom's interaction: the actual engine creates a level-5 male Pikachu, Adamant nature, all six IVs 15, and warps to Viridian Center 2F. Ordinary D-pad/A inputs then reach its real production lounge NPC. Separate recorded RAM writes grant all twelve capabilities, all eight trainer unlocks, 2,000 Practice Points and a three-HP deficit. Later diagnostic writes clear Growl and its PP from move slot one, enable the Pokémon menu, and provide three Potions, two Antidotes and one Revive. These are **test fixtures, not earned campaign progress**.

The initial fixture guard rejected the valid packed species/tera-type field before writing anything. A focused RED/GREEN test fixed its 11-bit species mask. While enabling the Pokémon menu, two diagnostic flag addresses derived from the wrong regional flag table were set and restored. The second briefly displayed Safari's menu; no Safari movement or battle occurred. The final flag `0x828` comes from the frozen ELF and FRLG flag table. All attempted writes, restorations and their exact before/after bytes remain in the extra-injection manifest. No product source defect was inferred from these fixture mistakes.

| Actual rendered path | Observed result |
| --- | --- |
| Center NPC → home → Pokémon development | Correct title, level, Practice Points and navigable sections; exit and reopen work. |
| Training → focus | Six zero permanent totals and badge-zero ceiling 21; Attack focus selected, with future-only/full-stat-discard explanation. Totals stay zero. |
| Capability list and details | Owned/active/cost labels; Brace and Flow assigned; Quick Start rejected at combined cost four. Encrypted active mask stays three. SELECT rules render with scrolling available. |
| Ability list and detail | Static and hidden Lightning Rod appear once each. Lightning Rod becomes actual ability slot two; Adamant remains fixed. |
| IV confirmation | Default No; explicit Yes spends exactly 20, raises HP IV 15→16 and leaves other IVs/nature/training unchanged. Balance 2,000→1,980; current/max HP 16/19 preserves the injected three-HP deficit. Pokémon and trainer checksums remain valid. |
| Relearning | Initially empty list backs out safely. After the explicitly cleared slot, Growl appears; its detail leads to an explicit replacement choice. Learning into that empty slot restores Growl and its normal 40 PP. |
| Party menu → Training → effect inspection → B | Root's Training entry opens the restricted field screen. A on Brace does not alter any of the 100 Pokémon bytes. B returns to the rebuilt party action menu with Pikachu selected. |
| Trainer build | Badge-zero budget two; Study assigned. Formation as an additional cost-two effect is rejected; active trainer mask stays one. |
| Supply picker | Empty inventory is safe. With injected stock, Potion and Antidote appear with quantities; Revive is omitted. Two Potions are accepted; a third HP heal is rejected. Two Potions plus one Antidote succeeds, START clears the plan, and bag stock is unchanged. |
| Final exit | Normal field is reconstructed and responsive after repeated Center and party transitions. |

No product defect was found in these exercised paths. These observations do not establish organic rewards, story reachability, all twelve Center maps, Android touch usability, audio, save/reload persistence, or human balance. Held-D-pad repeat still requires the newer artifact. Field focus mutation and less common transaction rejection conditions have model/native coverage but were not all exercised through the rendered UI in this session. The screenshots inspect representative long descriptions; they are not a renderer-wide text audit.

The diagnostic tools preserve the ordinary emulator driver's no-write interface: `build_fixture_driver.py` generates a separate test driver that labels every work-RAM injection, and `make_diagnostic_fixture.py` modifies only an absent disposable ROM copy. No ROM or save is committed. Both diagnostic emulator processes have stopped; their generated ROM and executable build target have been removed. Test saves and supplementary scratch logs remain isolated in this task worktree.
