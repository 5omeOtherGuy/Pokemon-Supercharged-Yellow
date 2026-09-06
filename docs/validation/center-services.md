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

Actual rendered service UI and callback checks are pending the coordinator's frozen ROM/ELF. The separate diagnostic tooling preserves the ordinary emulator driver's no-write interface: `build_fixture_driver.py` generates an isolated test driver that explicitly labels work-RAM injections, and `make_diagnostic_fixture.py` patches only a disposable ROM copy. `fixture_state.py` records exact before/after words, updates Pokémon encryption/checksums and the trainer checksum, and grants test-only development resources. Such setups can exercise transactions and rendering, but cannot establish organic rewards, story reachability, preparation pacing, Android usability or human balance. No ROM or save belongs in source control.
