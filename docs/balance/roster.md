# Original-151 roster curation

This is the first implemented roster pass, not a declaration of balanced gameplay. It changes 138 original-species learnset arrays and 31 original-species information entries. Thirteen learnsets are deliberately retained. All other species/form definitions remain available internally for upstream engine tests. The active modern configuration selects `level_up_learnsets/gen_9.h`; the historical Yellow `gen_1.h` is unchanged.

Authority: the owner's full execution mandate, ADRs [0004](../adr/0004-original-151-roster.md), [0011](../adr/0011-curated-moves-across-generations.md)–[0017](../adr/0017-trusted-core-and-boss-mastery.md), [0023](../adr/0023-badge-wide-training-ceilings.md), [0026](../adr/0026-yellow-kanto-content-baseline.md) and [0027](../adr/0027-campaign-and-battle-contract.md). Individual choices below exercise delegated numerical and move-assignment judgment. They do not introduce custom move or innate ability effects.

## Authoritative data and full manifest

The source baseline is project commit `a44f7a98a2e20432053827b712435a4da8851614`, evaluated using the active modern Pokémon configuration. This pins inspected imported data rather than claiming perfect correspondence with every official game's learnsets. Baseline and curated stats, abilities, evolution expressions and every level-up move for **all 151** are in [roster-manifest.json](roster-manifest.json). Every species has an explicit role in [roles.json](roles.json). Game data remains authoritative; these files do not drive battles.

The pass retains modern official typings, including Fairy and Magnemite/Magneton's Steel typing. All 151 type expressions are checked unchanged against the baseline. No type chart or move/ability implementation is changed here. Moves borrowed from other acquisition methods or granted outside a species' previous level-up set are intentional project curation; the manifest identifies the exact before/after distribution without claiming every new assignment was historically official.

## Evolution and move progression

Kadabra, Machoke, Graveler and Haunter evolve at level **37**, replacing both trade and Linking Cord methods. They become available in final form after the 36 cap, before the 44 cap. Pikachu uses an ordinary Thunder Stone for original Raichu. Gloom, Poliwhirl, Slowpoke, Exeggcute, Cubone, Koffing and Eevee retain only their original eligible branches, without regional conditions. Original species with exclusively later-generation evolutions become terminal campaign forms. Internal later-family records are retained; breeding, forms, gifts, encounters and predecessor lookup still require the coordinator's runtime boundary.

Thirteen stone evolutions receive their precursor's staged learnset plus selected evolution signatures and later coverage. They retain future support and STAB access when evolved immediately, and no longer put virtually their whole modern movepool at level 1. The three Eevee branches keep their distinct elemental schedules; early inherited Double-Edge and other level-1 shortcuts are removed. Their elemental basic attack is also present at level 1, so a directly generated evolved Pokémon has a usable attack without relying on the evolution event.

Evolution signatures include Body Slam for Nidoqueen, Thrash for Nidoking, Extreme Speed for Arcanine, Brick Break for Poliwrath, Leaf Blade for Victreebel and Icicle Spear for Cloyster. These are deliberate immediate payoffs and must be tested at the earliest obtainable stone. High-power special coverage, Shell Smash and most later finishers remain level-staged. Squirtle's family learns Shell Smash at 52, avoiding an inherited level-27/35 sweeping shortcut around the middle gyms. Primeape loses Rage Fist rather than advertising an excluded evolution's defining route. Mew and Mewtwo's final selected moves now fit the post-Championship level-75 cap.

The level-up relearner must respect the Pokémon's current level (`P_ENABLE_ALL_LEVEL_UP_MOVES=FALSE`). If pre-evolution reminder support is enabled, its predecessor traversal must reject excluded babies and forms. Ordinary TM acquisition is a separate route; a level-up gate is **not** a claim that no earlier TM could teach that move. This pass does not change generated teachable data, tutor lists or TM compatibility.

## Stage access and strategic roles

Caps by badge count 0–8 are 15, 22, 28, 36, 44, 50, 56, 62 and 68; post-Championship is 75. The table gives level access, with acquisition dependencies from the campaign worker's planned Yellow habitat conversion. Final map/story-state reachability is a separate integration check.

| Stage and player source | Selected tools | Intended decision and retained limitation |
| --- | --- | --- |
| Starter Pikachu; forest Caterpie and Pidgey; Mt. Moon Clefairy, before Misty 22 | Pikachu Helping Hand/Protect 18; Butterfree Struggle Bug 14 and Rage Powder 20; Pidgeotto Tailwind 21; Clefairy Follow Me 18 | Alternative damage support, special-attack reduction, redirection or speed control. Redirection is not protection against spread moves; powder moves have immunities; fragile supporters still need safe turns. |
| Oddish in its familiar early habitat; Psyduck where reachable; Geodude Mt. Moon | Sunny Day, Rain Dance and Sandstorm at 21 respectively; Psyduck Icy Wind 18 | Deliberate weather replacement costs an action. Psyduck's level access alone does not establish pre-Misty capture access. Rain may strengthen the opposing Water team; sand damages unprotected allies. |
| Pikachu and Clefairy before Surge 28 | Electric Terrain 26; Misty Terrain 26 | Grounded terrain choices alter sleep/status plans. A player's own terrain can undermine its powders or priority strategy. |
| Gloom after level 21; Drowzee in Route 11 access, before Erika 36 | Grassy Terrain 30; Psychic Terrain 30; Drowzee Light Screen 24 | Grass recovery and reduced Ground spread damage compete with Psychic pressure/priority denial. Terrain is not a blanket effect on airborne targets. |
| Water/Psychic specialists by the middle game | Slowpoke and Drowzee families Trick Room 36; Tentacool Acid Spray 18/Rapid Spin 22; Machop family Wide Guard 24 | Speed reversal, setup assistance and spread protection broaden team alternatives. Slow users risk taking damage first; Wide Guard does not stop single-target attacks. |
| Seel at Seafoam; Jynx Seafoam B4F level 34; later Lapras and Articuno | Seel/Jynx Snowscape 34; Lapras 36; Articuno 50 | Snow has real eligible users by later Kanto access. An early move level is not a promise of an early Ice encounter. Their common Electric/Grass/Rock or physical vulnerabilities remain. |
| Later Electric/Fire and sea recruits | Voltorb Rain Dance 26/terrain 30; Electabuzz terrain 34; Magmar sun 34; Horsea rain 28; all retained after evolution | Late specialists can join an existing weather or terrain core without requiring a specific TM or excluded evolution. Ability selection and relearning services determine practical preparation effort. |

Butterfree keeps accurate powders and gains early special debuffs, then Bug Buzz, redirection and level-44 Quiver Dance. Beedrill gains physical Poison access at 28 and U-turn at 32, rather than relying on special Venoshock. Parasect combines rain-compatible Dry Skin, Spore, Rage Powder 28 and Leech Life 36; its double Fire/Flying weaknesses and low Speed remain severe. Venomoth's special Bug and powder role remains distinct, with Quiver Dance deliberately at 44 rather than level 1. Scyther and Pinsir retain strong physical identities instead of receiving routine stat increases.

Pidgey's family gains Wing Attack 15, Tailwind 21, Roost 28 and U-turn 34. Pidgeot has Air Slash 40 and Hurricane 52, retaining imperfect Hurricane accuracy and competition with dedicated attackers. Fearow emphasizes Drill Peck and later Drill Run. Dodrio emphasizes physical finishing. Farfetch'd emphasizes cutting moves, item removal and Tailwind. Golbat gains Roost and earlier Leech Life while keeping Haze/Quick Guard; no Crobat is required for its support role.

The untouched learnsets are deliberate: Caterpie/Metapod and Weedle/Kakuna retain short weak stages; Abra retains its level-16 investment; Magikarp keeps the level-20 payoff; Ditto keeps Transform. Hitmonlee/Hitmonchan already have distinct guard, priority and Fighting tools. Gyarados, Snorlax, Articuno and Moltres already combine strong identities with suitable modern utility. Their complete moves remain in the manifest.

## Restrained stat and ability changes

Stat order is HP / Attack / Defense / Speed / Special Attack / Special Defense. No individual stat changes by more than 15, and no total rises by more than 30. This is the observed scope of this pass, not a new universal balance-policy limit. The other 141 species keep their active baseline stats.

| Species | Baseline → curated | Reason for the smallest selected adjustment; retained weakness |
| --- | --- | --- |
| Beedrill | 65/90/40/75/45/80 → 65/90/50/85/45/80 (+20) | A little more physical survival and speed for a pivot; low HP, frail defense and poor special offense remain. No Mega or Adaptability multiplier. |
| Pidgeot | 83/80/75/101/70/70 → 83/90/75/101/80/70 (+20) | Both familiar physical Flying attacks and later wind attacks can accompany support. Speed and bulk are unchanged; mixed investment and move slots still cost something. |
| Parasect | 60/95/80/30/60/80 → 70/95/80/30/60/90 (+20) | Improve one-turn redirection survival without accelerating guaranteed sleep. Speed 30 and double weaknesses remain; Fire under Dry Skin is especially punishing. |
| Farfetch'd | 52/90/55/60/58/62 → 62/90/55/70/58/62 (+20) | Limited survival and speed for its new cutting role. Attack remains 90 and bulk stays low; it does not become a Fighting type. |
| Onix | 35/45/160/70/30/45 → 50/60/160/70/30/45 (+30) | Physical defense needs a little HP and ordinary attack relevance without Steelix. Defense is not increased. Body Press is staged at 36; low special defense and double Water/Grass weaknesses remain. |
| Dewgong | 90/70/80/70/70/95 → 90/70/80/75/80/95 (+15) | Slightly improve special Ice/support contribution without Lapras-level offense or HP. Ice's weaknesses and modest power remain. |
| Lickitung | 90/55/75/30/60/75 → 100/65/75/30/60/75 (+20) | Better use of Body Slam/Knock Off while retaining slow utility and Cloud Nine. No Lickilicky-scale attack or Speed boost. |
| Seadra | 55/65/95/85/95/45 → 65/65/95/85/95/55 (+20) | Reduce extreme special frailty without importing Kingdra's typing and stats. Pure Water, mediocre HP and weak special defense remain. |
| Porygon | 65/60/70/40/85/75 → 75/60/70/40/90/75 (+15) | Small survival/output gain for Trace/Download and Trick Room utility. Low Speed and ordinary bulk remain; no Porygon2 defenses. |
| Wigglytuff | 140/70/45/45/85/50 → 140/70/50/45/85/55 (+10) | Small defenses let the high-HP sound-support identity function. Slow Speed and low defenses still distinguish it from Clefable. |

Two existing official abilities are reassigned, with their engine effects unchanged: Farfetch'd's first regular slot changes Keen Eye → **Sharpness**, supporting its leek-and-cutting identity; Inner Focus and Defiant remain alternatives. Seadra's hidden slot changes Damp → **Swift Swim**, allowing a rain specialist without Kingdra; Poison Point and Sniper remain alternatives. Swift Swim availability depends on the coordinator's legal-ability selection service, not an assumed hidden-ability wild catch. Sharpness combined with Leaf Blade/Sacred Sword and critical-hit items needs specific encounter testing.

## Evidence, counterplay and remaining validation

The native C probe evaluates actual selected learnset initializers and stat expressions for 151/151 species, with the real preprocessor configuration. It verifies sorted levels, valid moves, terminators, stat bounds, original-only solo evolution closure, the four level-37 routes, Pikachu's stone method, stage-specific support/weather/terrain, late Bug/Flying tools, stone-form move continuity, blocked level-1 finisher shortcuts and manifest consistency. The baseline RED run executed seven tests and failed 46 subtests for intended missing data. The GREEN checkpoint passed all seven. The final expanded suite has ten passing tests. This is data coverage, not an 80% claim about the whole engine or battle simulation.

The host probe omits graphics, unrelated SpeciesInfo fields, actual evolution execution, combat and Android. It is not a linked GBA build. An ownership audit against the pinned baseline found no changed non-original species definitions or learnset arrays. No full game build targets or emulator processes were created in this worktree.

Priority matchup hypotheses for actual battle testing are: Misty against Pikachu/Pidgeotto support and ordinary early Grass choices; Erika against Bug/Flying alternatives and sun disruption; Sabrina against Bug pivots, terrain replacement, screens and slower Trick Room teams; later Rock/Water/Fire encounters against Onix, Parasect and Pidgeot. Compare ordinary returning-player teams, reasonably trained cores and maximum stage-legal training. Check that boosted Onix does not make Brock excessive and that early Moon Stone Nidoking/Nidoqueen and Compound Eyes Sleep Powder do not trivialize Misty. Test Beedrill's 85 Speed threshold, Parasect's survival against spread moves, and Farfetch'd's Sharpness/critical combinations before calling any increase safe.

Still required: complete one-save encounter/evolution/resource reachability; repeatable Moon/Fire/Water/Thunder/Leaf Stones; Center reminder/ability access; actual battle effects and AI valuation for weather, terrain and support; cap/IV/training integration; full ARM build; actual gameplay and Android; and independent human assessment of preparation burden, discoverability and enjoyment. These remain release work, not reasons to mark this data pass as tested balance.

From the project root:

```sh
python3 -m unittest discover -s tests/roster -v
python3 tests/roster/export_manifest.py
```

The export command requires the pinned baseline commit in local Git history. It regenerates the machine manifest from game data and the authored role annotations. The tests also detect a stale manifest.
