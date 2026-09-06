# Boss briefing implementation and validation

The native field screen reads the selected trainer's authoritative `Trainer` and `TrainerMon` data before the battle script commits. `VAR_0x8004` supplies the trainer ID; `special ScShowBossBriefing` must be followed by explicit `waitstate`. On exit, `VAR_RESULT` is true only after an explicit acceptance with enough healthy, non-egg party members; the script context resumes in either case. The caller must branch on that result before spending supplies or starting battle.

## Player interaction

Left/right changes the Team, Bag and Trainer overview pages. Team lists every opponent's species and level and states Singles/Set or Doubles/Set. A opens the selected Pokémon's four moves, nature with its stat alignment, legal ability, held item and assigned capabilities. Left/right switches teammates there. A opens canonical move, item or ability descriptions and exact public capability/passive descriptions. Up/down scrolls long descriptions. B returns one level or leaves freely from an overview. START opens the explicit battle confirmation, initially set to No. An attempted doubles acceptance with fewer than two healthy Pokémon stays in preparation and explains the problem.

The four boss bag rows each represent one consumable unit; duplicate item rows represent multiple units, and empty rows are labeled Empty. Trainer and capability pages display the actual selected effects, their weighted slot costs, and the applicable budget. Descriptions state percentages, conditions and multiplicative integer rounding. No opponent build is inferred from the player's current progression.

The temporary 28×17-tile window uses BG0 tile base 1, below the standard frame tiles and field tilemaps. On closing, other BG0 windows' retained graphics buffers are restored. Small-font description lines use the font's 12-pixel height. This preserves the 240×160 native resolution; no touch-sized claim is inferred from pixel dimensions alone.

## Authoritative data and safety

`ScBriefingLoad` rejects invalid IDs, parties outside one to six original species, unsupported battle formats, invalid levels/natures/items/moves, illegal explicit abilities, variable trainer pools/overrides, random default abilities and unsupported initial field effects. A rejected briefing cannot accept. Campaign bosses must use deterministic ordinary parties and their configured move-driven weather/terrain. Adding another party resolver or automatic field setup requires matching disclosure support first.

Explicit trainer moves retain their exact positions, including empty slots. Without explicit moves, the model reads the active species learnset and follows `GiveBoxMonInitialMoveset`'s ordered last-four algorithm, skipping evolution-only level-zero moves and deduplicating the current move window. It does not create a Pokémon or consume random numbers. Unspecified trainer abilities resolve to the deterministic first legal ability only while `B_TRAINER_MON_RANDOM_ABILITY` is zero.

`sc_build_catalog.c` supplies the same NPC capability/passive providers intended for battle and AI. Early gym aces usually receive one effect and their teammates none. Later teams add selected supporting effects; rematches spend more of a fixed narrative budget without automatically filling every Pokémon. The champion assigns two effects to the ace and one to the preceding teammate. Provider probes validate every trainer ID and party index against the three-point Pokémon budget and the trainer's budget. Catalog descriptions are mechanics contracts; their existence is not proof that every battle hook is implemented or balanced.

## Reproducible checks

From the repository root:

```sh
python3 -m unittest discover -s tests/briefing -p 'test_*.py'
python3 tests/briefing/compile_objects.py --generated-engine /absolute/path/to/prepared/engine
```

The first command compiles and executes the production catalog and briefing data/navigation functions with narrow native adapters. The probes cover exact default and explicit moves, ability resolution, unchanged source party data, invalid/variable data rejection, single/double readiness, the overview/detail navigation paths, default decline and explicit acceptance. Catalog probes cover every configured trainer ID across party sizes two through six, invalid indexes, effect bounds and weighted budgets. Both tests pass. Missing production APIs were captured as failing link checks before implementation; the catalog's first purported green commit had a host-adapter signedness error, corrected and explicitly recorded in its next commit.

The second command uses a prepared engine's generated headers and text preprocessor read-only, then compiles both real ARM production translation units with warnings treated as errors into a temporary directory. Both units pass. It does not link a ROM or prove rendered behavior.

## Runtime evidence

Runtime checking is pending against the coordinator's combined ROM and exact ELF. `tests/briefing/make_diagnostic_fixture.py` can make an explicitly labeled disposable ROM copy that replaces only Mom's interaction script with a briefing call. It checks the source checksum, the ELF's special/function addresses and the available script extent, and prints the exact changed bytes and output checksum. It never edits the source ROM or a save. This fixture can validate rendering, navigation and return-to-field behavior through the real engine; it cannot establish legitimate boss reachability, campaign completion, Android usability or human balance.

No Android game test or human playtest has been performed as part of this briefing task. End-to-end campaign entry, selected player supplies, actual passive battle effects and AI use require the integrated systems and separate gameplay evidence.
