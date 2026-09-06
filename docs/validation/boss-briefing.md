# Boss briefing implementation and validation

The native field screen reads the selected trainer's authoritative `Trainer` and `TrainerMon` data before the battle script commits. `VAR_0x8004` supplies the trainer ID; `special ScShowBossBriefing` must be followed by explicit `waitstate`. On exit, `VAR_RESULT` is true only after an explicit acceptance with enough healthy, non-egg party members; the script context resumes in either case. The caller must branch on that result before spending supplies or starting battle.

## Player interaction

D-pad left/right and shoulder L/R change the Team, Bag and Trainer overview pages. Team lists every opponent's species and level and states Singles/Set or Doubles/Set. A opens the selected Pokémon's four moves, nature with its stat alignment, legal ability, held item and assigned capabilities. Left/right switches teammates there. A opens canonical move, item or ability descriptions and exact public capability/passive descriptions. Up/down scrolls long descriptions. B returns one level or leaves freely from an overview. START opens the explicit battle confirmation, initially set to No. An attempted doubles acceptance with fewer than two healthy Pokémon stays in preparation and explains the problem.

The four boss bag rows each represent one consumable unit; duplicate item rows represent multiple units, and empty rows are labeled Empty. Trainer and capability pages display the actual selected effects, their weighted slot costs, and the applicable budget. Descriptions state percentages, conditions and multiplicative integer rounding. No opponent build is inferred from the player's current progression.

The temporary 28×17-tile window uses BG0 tile base 1, below the standard frame tiles and field tilemaps. On closing, other BG0 windows' retained graphics buffers are restored. Small-font description lines use the font's 12-pixel height. This preserves the 240×160 native resolution; no touch-sized claim is inferred from pixel dimensions alone.

## Authoritative data and safety

`ScBriefingLoad` rejects invalid IDs, parties outside one to six original species, unsupported battle formats, invalid levels/natures/items/moves, illegal explicit abilities, variable trainer pools/overrides, random default abilities and unsupported initial field effects. A rejected briefing cannot accept. Campaign bosses must use deterministic ordinary parties and their configured move-driven weather/terrain. Adding another party resolver or automatic field setup requires matching disclosure support first.

Explicit trainer moves retain their exact positions, including empty slots. Without explicit moves, the model reads the active species learnset and follows `GiveBoxMonInitialMoveset`'s ordered last-four algorithm, skipping evolution-only level-zero moves and deduplicating the current move window. It does not create a Pokémon or consume random numbers. Unspecified trainer abilities resolve to the deterministic first legal ability only while `B_TRAINER_MON_RANDOM_ABILITY` is zero.

`sc_build_catalog.c` supplies the same NPC capability/passive providers intended for battle and AI. Early gym aces usually receive one effect and their teammates none. Later teams add selected supporting effects; rematches spend more of a fixed narrative budget without automatically filling every Pokémon. The ace is the highest-level actual team member, choosing the last member in a level tie. A supporting effect goes to the final other teammate. The champion assigns two effects to the ace and one to that supporting teammate. Provider probes validate every trainer ID and party index against the three-point Pokémon budget and the trainer's budget. Catalog descriptions are mechanics contracts; their existence is not proof that every battle hook is implemented or balanced.

## Reproducible checks

From the repository root:

```sh
python3 -m unittest discover -s tests/briefing -p 'test_*.py'
python3 tests/briefing/compile_objects.py --generated-engine /absolute/path/to/prepared/engine
```

The first command compiles and executes the production catalog and briefing data/navigation functions with narrow native adapters. The probes cover exact default and explicit moves, ability resolution, unchanged source party data, invalid/variable data rejection, single/double readiness, the overview/detail navigation paths, default decline and explicit acceptance. Catalog probes cover every configured trainer ID across party sizes two through six, invalid indexes, effect bounds and weighted budgets. Both tests pass. Missing production APIs were captured as failing link checks before implementation; the catalog's first purported green commit had a host-adapter signedness error, corrected and explicitly recorded in its next commit.

The second command uses a prepared engine's generated headers and text preprocessor read-only, then compiles both real ARM production translation units with warnings treated as errors into a temporary directory. Both units pass. It does not link a ROM or prove rendered behavior.

## Runtime evidence

Actual desktop libmgba 0.10.2 runs on 2026-09-06 inspected a frozen production build from source `30e94008`. Source ROM SHA-256: `d4c5c399bf4de8cf672443c9308e000476ce6cee9248aecca40f7d9cbc4d3b6c`. Exact ELF SHA-256: `be2977f69f2b8e0da290688b4f2e13f37a2bad621bf0f795b32cf677be308058`.

`tests/briefing/make_diagnostic_fixture.py` changed only twelve bytes at Mom's interaction script, address `0x0833f7ea`, in each disposable ROM copy. It checks the original checksum, the ELF's special/function addresses and available script extent. Special index 621 calls the real production UI. Both runs used absent fresh save paths and ordinary new-game/controller inputs; no RAM writes, cheats, savestates or injected Pokémon were used. They are **diagnostic script fixtures, not legitimate boss reachability or campaign-progress evidence**.

| Fixture | Patched script bytes | Fixture ROM SHA-256 | Result |
| --- | --- | --- | --- |
| Misty, trainer 315 | `691604803b01256d02276b02` | `be03d3c5837d02cab9241467b5c705c01d3e7dd4bb2108209007b477c3147a23` | 246/246 successful driver responses; 18,173 frames |
| Six-member trainer 338 | `691604805201256d02276b02` | `21eb0783859e0c32d7c3f7ab746edca03f1267e8ce3d3f5bb6736af28f993145` | 156/156 successful driver responses; 16,945 frames |

The original twelve bytes were `6a5a2b580206012bf83308a0`. The [Misty trace](../../tests/briefing/misty-ui.commands) and [six-member trace](../../tests/briefing/six-member-ui.commands) reproduce the recorded inputs. Build the [headless driver](emulator-harness.md), create the corresponding fixture ROM with the tool, create the trace's empty screenshot directory (`build/briefing/misty-ui/` or `build/briefing/six-member-ui/`), and feed the trace to the driver with an absent, isolated cartridge-save path. The traces include read-only addresses specific to this exact ELF.

Visual inspection covered the three- and six-member rosters, the sixth member's details, moves and canonical move/ability/item descriptions, empty held items and capabilities, bag quantities, selected capability/trainer effects with costs, and the safe-default confirmation. All inspected text and rows fit at 240×160. The six-member fixture discloses trainer 338's actual levels 57–63; this check does not establish which champion variant the campaign selects.

Representative unmodified captured frames:

| Team and details | Effects and supplies | Exit behavior |
| --- | --- | --- |
| [Misty roster](../../tests/briefing/evidence/misty-team-open.png) | [Rain Dance](../../tests/briefing/evidence/move-description.png) | [Default No](../../tests/briefing/evidence/confirm-default-no.png) |
| [Pokémon details](../../tests/briefing/evidence/psyduck-details.png) | [Ability](../../tests/briefing/evidence/ability-description.png) | [Doubles refusal](../../tests/briefing/evidence/doubles-not-ready.png) |
| [Six-member roster](../../tests/briefing/evidence/six-member-team.png) | [Bag quantities](../../tests/briefing/evidence/bag.png) | [Returned field](../../tests/briefing/evidence/back-to-field.png) |
| [Sixth member](../../tests/briefing/evidence/sixth-member-details.png) | [Trainer effect](../../tests/briefing/evidence/trainer-effect-description.png) | [Working field menu](../../tests/briefing/evidence/field-menu-after-exit.png) |

Selecting Yes with the fresh empty party displayed the doubles-readiness refusal and kept `VAR_RESULT` zero. B returned to the overview, then exited; reopening worked. Confirming the default No also exited with a zero result. Subsequent ordinary movement and the Start menu worked, establishing script resumption and usable field graphics in this case. The same 1,024 bytes starting at `gParties` (`0x02031c7c`) were read before opening and after browsing/refusal and were identical. This includes the initially empty player party; it does not test a populated party or measure RNG invariance.

This run exposed two follow-ups. The frozen catalog assigned Flow to the last member, Seel, rather than the higher-level Starmie; the screen accurately disclosed that assignment. Commit `002fc659` fixes ace selection from actual levels and adds shoulder-button aliases to the existing D-pad navigation. Regression probes failed before these fixes and pass afterward, and both ARM units compile. The captured frames above precede those fixes; their new behavior still needs a refreshed linked-ROM check. Successful positive acceptance with a healthy party has been verified in native model probes only, not yet through the emulated full script.

No Android game test or human playtest has been performed as part of this briefing task. End-to-end campaign entry, selected player supplies, actual passive battle effects and AI use require the integrated systems and separate gameplay evidence.
