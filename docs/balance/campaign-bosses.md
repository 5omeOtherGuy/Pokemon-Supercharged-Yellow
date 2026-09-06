# Campaign boss and rematch curation

This is provisional tuning under the full-project mandate and ADRs 0010–0017,
0024, 0027, 0029–0031. It changes 59 team bodies: eight first gyms, 24 gym
rematches, 21 rival variants, two story Giovanni battles and four Jessie/James
encounters. League and ordinary trainers are outside this change. Every trainer
header, battle format and supply bag is preserved from `5b501680`.

## Problems corrected

Inherited automatic moves supplied poor modern-category sets and doubles plans.
Some explicit sets used moves before their active source learnsets allowed them
(Starmie's early Recover, Seel's early Helping Hand, and others). Equal-level
rematches assigned the signature gym capability to the last teammate rather than
the gym's signature Pokémon. Brock's first rematch used Golem before its level-37
evolution. Rival supporting teams failed to branch even after Eevee's evolution
branch had been fixed. Several story battles exceeded the earliest reachable cap.

All updated members have explicit species-legal innate abilities, fixed natures,
IVs, restrained permanent training and selected moves. No new innate ability,
stat edit, type-chart change, battle format, supply bag or held equipment is
introduced. Lab Eevee retains level 5, IV12, Tackle/Tail Whip and zero training;
its Run Away ability adds no battle bonus. Other major members have four distinct
moves. Level-evolved species respect their source evolution thresholds.

## Story-stage contract

S.S. Anne can be reached before Misty using Bill's ticket, so its rival is now
level 19–22 and fits one badge. Cut plus the Cascade Badge permits access to
Celadon before Surge; Fuji rescue previously allowed Silph at that same early
badge count. Root-owned acceptance checks therefore require three badges for
Hideout/Tower major battles and five for Silph major battles, after showing the
full scouting information. These are an integration dependency, not map/script
changes made by this worker.

| Stage | Trainer IDs | Team level range | Training per stat |
| --- | --- | --- | --- |
| Laboratory / early Route 22 | 226–228 / 229–231 | 5 / 9–10 | 0 / 4 |
| Cerulean / S.S. Anne | 232–234 / 326–328 | 18–20 / 19–22 | 8 / 12 |
| Mt. Moon Jessie/James | 624 | 16 | 8 |
| Hideout Jessie/James / Giovanni; three badges | 625 / 248 | 32–33 / 32–35 | 20 / 24 |
| Tower rival / Jessie/James; three badges | 329–331 / 626 | 32–34 / 34–35 | 20 / 24 |
| Silph rival / Jessie/James / Giovanni; five badges | 332–334 / 627 / 249 | 45–48 / 47–48 / 45–49 | 32 / 32 / 36 |
| Late Route 22; eight badges | 335–337 | 63–65 | 56 |

The first gyms use 8,12,16,24,28,32,40,48 training per stat in badge order. Their
explicit IVs run 12,14,16,18,20,22,24,26. General rematch tiers use 32/48/64
training and IV26/28/30; post-Champion-only Blaine/Giovanni tiers use 64/72/80
training. None requires a player to fill all six training stats or perfect IVs.

## Gym plans and ordinary counterplay hypotheses

| Gym | First plan | Rematch development | Accessible answers and retained risks |
| --- | --- | --- | --- |
| Brock | Geodude's Rock Tomb/Bulldoze gives simple speed pressure; Sturdy prevents a single-hit bypass. Onix retains Bind/Rock Tomb and ordinary physical pressure, without a surprise Dragon coverage attack. | Graveler/Kabuto/Rhyhorn at tier one; later Golem, fossils and Aerodactyl widen physical/special coverage. Onix's Body Press/Curse makes its Defense meaningful. Onix remains the highest-level ace. | Route22 Mankey has Low Kick by8; Nidoran male has Double Kick by12; Caterpie evolves to Butterfree with Confusion. Pikachu can remain in the core, but pure Electric damage cannot solve this familiar Yellow hurdle. Frail Butterfree must respect Rock attacks. |
| Misty, doubles | Swift Swim Psyduck sets rain and Icy Wind. Starmie uses Water Pulse/Swift plus Light Screen/Protect; its later Recover is absent. Seel uses Charm/Encore/Icy Wind instead of unavailable early support. | Golduck, Dewgong and Lapras support Starmie; later Hydro Pump increases pressure without ally-hitting Surf. Recovery arrives at legal levels. | Bulbasaur's Cerulean gift provides Razor Leaf/Seed Bomb through Ivysaur; Pikachu offers Electric damage and Helping Hand. Pidgeotto Tailwind and Psyduck Icy Wind give alternate speed support. Grass must survive Ice coverage and concentrated attacks. |
| Surge | Voltorb sets screens or uses Taunt; Pikachu introduces Electric Terrain/Nuzzle. Raichu has Thunderbolt and Iron Tail with Lightning Rod. | Magneton and Electabuzz replace redundant Raichu copies; later Jolteon adds a fast special role. Raichu gains Nasty Plot and legal Surf coverage in singles. | Diglett from Diglett's Cave learns Bulldoze16; Sandslash and the Nido families offer sturdier Ground alternatives. A frail unevolved Diglett is not promised a sweep through Iron Tail. Rematch Surf is disclosed and preserves a reason for Grass or special support. |
| Erika, doubles | Sunny Day activates Chlorophyll; Weepinbell provides physical Razor Leaf/Poison Jab, Vileplume has Giga Drain/Moonblast/Grassy Terrain, and Exeggcute contributes Reflect/Leech Seed. | Victreebel/Exeggutor mature; Tangela gains legal Rage Powder; the final tier adds Venusaur. Sun and terrain remain explicit, answerable setup. | Gift Charmander has Flamethrower30 and can become Charizard36; Pidgeotto and Butterfree retain Flying options. Psyduck's Rain Dance21 can contest weather. Sun empowers Fire attacks but also speeds the enemy Grass team. |
| Koga | Venomoth's Bug Buzz/Psybeam, physical Muk, Haze/Roost Golbat and special Weezing have distinct roles. Burns and Clear Smog punish unchecked physical setup without evasion or explosion loops. | Venomoth gains Quiver Dance; Muk gains Acid Armor; final Arbok adds Intimidate/Glare. Weezing remains the ace. | Abra/Kadabra offers Psychic35; Ground attacks work against grounded Poison targets but not Levitate Weezing or Golbat. Physical and special attacks need different targets. Common Normal or Flying teammates remain useful support. |
| Sabrina, doubles | Mr. Mime uses Fairy/Psychic coverage and Reflect; Hypno supplies Psychic Terrain/Helping Hand; Kadabra has recovery and Alakazam a special setup role. No unsupported Dazzling Gleam on Alakazam or self-conflicting priority plan. | Slowbro replaces the duplicate Alakazam support slot; final Exeggutor broadens Grass/Psychic coverage. | Snorlax has Crunch24 after the Poké Flute route; Butterfree has Bug Buzz32. Ghost damage is useful but Gengar also risks super-effective Psychic. Grounded Psychic Terrain protection must be considered before selecting priority attacks. |
| Blaine | Drought Ninetales, physical Rapidash, utility Magmar and Intimidate Arcanine make a sun team. Arcanine's Flare Blitz arrives at its level56 threshold, matching its physical capability. | Ninetales can Nasty Plot; final Charizard adds a Flying/special attacker. | Surf-capable Water partners, Psyduck/Golduck Rain Dance and Ground/Rock coverage are ordinary options. Sun weakens Water damage, while Rapidash High Horsepower punishes careless Fire/Electric answers. Rain is a tactical choice, not mandatory inventory. |
| Giovanni | Sand Force Dugtrio, special Nidoqueen/Nidoking, fast Persian and physical Rhydon avoid five interchangeable Ground attackers. No Arena Trap lock or OHKO move. | Higher tiers retain the mixed roles and add Scary Face on Rhydon rather than an inaccessible setup move. | Water/Grass remain strong against Rhydon; Lapras's Silph gift has Ice Beam45. Persian's Fake Out/Bite and Nido coverage make a single Psychic or Water answer insufficient by assumption. Matchups still need measured trials. |

First-gym sizes remain unchanged. Rematches generally expand to four members,
then five for the final tier where appropriate. Their earliest reachable stages
come from executing the actual `ScY_Rematch_*` script selection branches:

| Gym | Tier1 / tier2 / tier3 earliest eligibility | Ace levels |
| --- | --- | --- |
| Brock | 3 badges / 5 badges / Champion | 36 / 50 / 75 |
| Misty | 4 / 6 / Champion | 44 / 56 / 75 |
| Surge | 5 / 7 / Champion | 50 / 62 / 75 |
| Erika | 6 / 8 / Champion | 56 / 68 / 75 |
| Koga | 7 / 8 / Champion | 62 / 68 / 75 |
| Sabrina | 8 / 8 / Champion | 66 / 68 / 75 |
| Blaine | Champion / Champion / Champion | 70 / 72 / 75 |
| Giovanni | Champion / Champion / Champion | 70 / 72 / 75 |

A lower tier remains repeatable while later gates are unmet. Unlocking room for
training does not fill it. The source tests verify every tier's actual dispatch
against its authored cap and preserve the signature ace under highest-level,
last-tie selection. Gym/League catalog policy itself is unchanged.

## Rival and Rocket identity

The pinned Yellow reference is pret/pokeyellow
`e89ead154b9968aa50eed9328ff2b38b6c194382`,
`data/trainers/parties.asm:Rival2Data`. Laboratory and early Route22 outcomes
continue to select the existing saved branch. Eevee remains Eevee through Tower.
Tower partners now branch as Shellder+Vulpix for Jolteon, Shellder+Magnemite for
Flareon, and Vulpix+Magnemite for Vaporeon. Silph and late Route22 use the matching
Cloyster/Ninetales/Magneton combinations. Supporting order can change for the
actual battle roles; the species branch identity is retained. The late battle's
Alakazam and Exeggutor are deliberate mature preparations for the already-curated
Champion teams.

The rival develops from straightforward physical pressure into mixed coverage,
then a branch-specific ace. Flareon retains physical Fire/Superpower/Flame Charge;
Jolteon uses special Electric/Ghost coverage; Vaporeon uses Water/Ice and Haze.
Early coverage stays modest, and later moves must exist in the active learnset
or current TM/HM compatibility data. No encounter uses a future evolution's
level moves through a predecessor lookup.

Hideout Giovanni keeps Onix/Rhyhorn and an Early Bird Kangaskhan physical ace.
Silph adds Scrappy Kangaskhan and special Nidoqueen; the latter is the highest
level. Jessie/James keep Ekans/Koffing/Meowth, then Arbok/Weezing/Meowth. Meowth
retains Fake Out and develops Helping Hand/Assurance; its evolved partners use
Glare, burns and legal direct damage. Levitate is retained instead of an
accidental Neutralizing Gas conflict with their own Intimidate. No duo has
self-destructive friendly fire or an evasion loop. All use the existing doubles
entry checks, shared supply quota and scouting contract.

## Partial narrative progression catalog

`engine/src/sc_build_catalog.c` now supplies explicit story builds. These are
partial encounter records, not the player's full collection. The ordinary lab
and early Route22 rival have no extra trainer effects or capabilities. Tempo
appears in the Tower rival after the ship milestone, not in the tutorial.
Preparation is not assigned to opponents with empty bags merely to fill capacity.

| Encounter | Trainer budget | Active trainer effects | Ace capabilities | Separate supporting member |
| --- | --- | --- | --- | --- |
| Cerulean rival | 3 | Formation | Brace | None |
| Ship rival | 3 | Formation | Flow | None |
| Tower rival | 5 | Formation, Tempo | Quick Start | Flow |
| Silph rival | 7 | Formation, Composure, Tempo | Staying Power | Flow |
| Late Route22 rival | 8 | Formation, Composure, Offense | Staying Power, Brace | Flow |
| Hideout Giovanni | 5 | Formation, Offense | Brace | None |
| Silph Giovanni | 7 | Formation, Composure, Offense | Staying Power | Brace |
| Jessie/James1 | 2 | Formation | None | None |
| Jessie/James2 | 5 | Formation, Tempo | Brace | Flow |
| Jessie/James3 | 5 | Composure, Tempo | Quick Start | Brace |
| Jessie/James4 | 7 | Formation, Composure, Tempo | Quick Start, Brace | Flow |

Budgets bound weighted costs and may have unused capacity. Capability costs stay
within three per Pokémon; support costs at most one. No blanket Focus/Pressure
assignment penalizes Meowth, Flareon or mixed attackers. The actual highest-level
member is the ace, with last-slot tie breaking; support always goes to a distinct
member. Gym and League tables, descriptions and policy were preserved.

## Evidence and acceptance limits

- `faa1ead6` RED exposed missing traits, stage problems, wrong signature aces
  and lost rival branch companions. `40235f0d` implements the 59 team bodies.
- `125340b6` RED compiled the real catalog and failed because narrative budgets
  were zero. `a82f15dd` implements the partial narrative records.
- Seven boss-specific source tests verify all 59 parties, level evolution
  thresholds, actual rematch dispatch, Yellow branch companions, doubles
  exclusions, legal abilities/moves/training, signature aces and cited ordinary
  counterplay moves. Source legality uses compiled active species/level-up data
  plus current TM/HM compatibility, not a copied modern-franchise movepool.
- The real catalog C test visits all 652 trainer IDs and invalid slots, checks
  weighted budgets, every narrative variant and ace relocation/tie behavior.
  Host coverage is 94.74% of 114 executable catalog lines (not game coverage).
- Actual trainerproc accepted the complete file. Complete `data.c` and
  `sc_build_catalog.c` ARM objects compiled with FIRERED/TESTING=0, actual root
  generated assets read-only and `-Werror`; only an inherited unused graphics
  constant warning was suppressed. Temporary generated targets were removed.

```sh
python3 -m unittest discover -s tests/content -v
python3 -m unittest discover -s tests/roster -v
python3 -m unittest discover -s tests/briefing -v
```

The commands above passed 29 content tests, 10 roster tests and 2 briefing tests.

Source checks do not prove AI uses support/setup well, that damage ranges are
appropriate, or that preparation is enjoyable. Native tactical battles and
ordinary play must examine at least a familiar Pikachu/starter/Pidgey core,
a Ground/Fighting-oriented core, and one or two specialist rotations. Record
actual catch-up time, wins/losses, HP/PP/supply use, weather/terrain decisions,
status targeting and repeated rematch behavior. Fixed natures, incomplete
training and imperfect IVs must be represented. The shared AI's public prior
assumes zero player training, so prediction errors remain a deliberate fairness
tradeoff. Root must integrate the agreed story acceptance gates and validate
scouting against the fought data. Ordinary trainer pacing, complete human play
and Android validation remain outside this worker's evidence.
