# Ordinary mainland trainer correction pass

This source pass audits **325 ordinary trainer IDs / 799 party slots** referenced by objects in **253 connected mainland maps**. It changes **108 slots in 63 trainers**. All trainer IDs, headers, levels, party sizes/order, held items and individual-trait lines remain unchanged. No authored gym leader, rival, Giovanni, Jessie/James, League, Champion, gym-rematch or unused ordinary tier party changes.

Baseline: `80ac8b3c`. Source checkpoint: `ef1c66d5`. The imported foundation is RHH expansion1.17.0, pinned at `7644501f3065b38aea2cfc64b5b58ca011bce4f5` in ADR0028. Current project data—not the original cartridge's Generation3 learnsets—is authoritative. This is source consistency work, not actual gameplay, Android or human balance validation.

## Scope and correction policy

The audit walks map warps/connections from Pallet, then follows each map object's script through local map scripts and the shared `data/scripts/trainers_frlg.inc`. This includes ordinary gym subordinates, Rocket grunts, Dojo trainers, ships and dungeons. It excludes disconnected Sevii/facility maps and unused higher-tier party records. Physical connectivity is a conservative content inventory; it does not prove current story-state access, collision traversal or complete journey reachability.

Under the owner's delegated numerical/design authority and the coordinator's approved policy, preserve the inherited class, party size and level curve. Replace an unsupported premature evolution with its nearest lawful original-family predecessor at the same level. Count actual mainland wild/gift acquisition before deciding that a species is premature. Replace stale moves with supported current moves, retaining attack/support purpose where practical. Do not increase IVs, add held items, add NPC bags, or automatically raise ordinary levels to the boss cap.

Level-up moves, real evolution signatures (level0), retained moves from original-form predecessors and compatible moves in the engine's actual TM/HM catalog count as supported. This intentionally allows Nidoqueen's Body Slam and Nidoking's Thrash. Compatibility comes from the same imported learnable data used to generate teachable tables; this does not assert every TM is available to the player before every trainer. No replacement in this pass depends on a new TM source: the chosen replacements are available through the curated species/family level schedule.

Default move sets are evaluated with the engine's skip-level0, deduplicate and shift-oldest rule from `GiveBoxMonInitialMoveset`, using the host-compiled active learnsets. An omitted explicit set can therefore be legal but nonfunctional: some level ranges retained four consecutive support moves and lost every direct attack. Fourteen such slots now explicitly remember an appropriate earlier attack. Early Metapod/Kakuna remain deliberately weak Harden-only stages; single-battle Metronome and Ditto's Transform can contribute indirectly and are not rejected as empty offense.

## Evolution findings and lawful exceptions

The initial threshold-only audit flagged59 slots. Actual encounter sources account for25 of them; only34 unsupported slots are changed. These are source-supported specimens, not a promise that the player can catch them before meeting the NPC.

| Retained species / minimum direct level | Actual source | Consequence and stage caveat |
| --- | --- | --- |
| Raticate17 | Route11 land | Yasu17 and Hideout17/19 Raticate remain; Route6 Jeff16 becomes Rattata. |
| Haunter20 | Pokémon Tower3F land | Tammy23 remains consistent with the same dungeon's catches. |
| Seaking20 | Safari Center fishing | Ronald28 and other28–31 Seaking remain; obtaining the fishing tool/Safari access is separate. |
| Seadra25 | Route19 fishing | Ocean trainers'28/30 Seadra remain; requires the appropriate fishing access. |
| Slowbro15 | Route12 surfing | Cameron33 remains; the player's Surf permission is separate and precedes Sabrina in the fixed gym order. |
| Muk33 | Power Plant land | Billy33 remains; earlier Muk22/25/29 slots become Grimer. A player can visit some of those routes before Surf permits Power Plant access. |
| Poliwhirl20 | Route6/Route22/Route25 and Viridian fishing | Stan22 remains a specimen supported by ordinary mainland fishing. Acquiring the necessary rod can occur later than first reaching Route8; this is not a symmetric early-access claim. |

The34 changes retain the existing levels: unsupported Magneton→Magnemite, Weezing→Koffing, Electrode→Voltorb, Muk→Grimer, Graveler→Geodude, Rapidash→Ponyta, Dodrio→Doduo and Tentacruel→Tentacool. Machamp29 becomes Machoke; Machamp26 recursively becomes Machop. The exact affected slots are below. These changes usually reduce bulk, Speed and/or attack power; no compensating invisible stat increase is introduced.

## Move identity and practical effects

| Correction family | Purpose and ordinary counterplay | Remaining tuning risk |
| --- | --- | --- |
| Rock/Ground Hikers | Magnitude/Mud Sport become current Bulldoze/Protect; premature Screech becomes Rock Tomb. Rock/Ground identity and Water/Grass weaknesses remain. | Predictable60-power Ground damage and Speed reduction differ from random Magnitude; Protect may lengthen battles. |
| Electric Scientists/Engineers | Removed Sonic Boom becomes level-legal Charge Beam or Electro Ball. Attackless screen/terrain sets retain Electro Ball alongside public support. | Ground immunity and lower-Speed targets reduce Electro Ball pressure; Charge Beam can accumulate boosts. These are changed damage profiles, not balance-equivalent renames. |
| Poison Bikers/Grunts | Premature Screech becomes Poison Gas; early Self Destruct becomes Clear Smog. Other legal Sludge/Smog/status tools remain. | Clear Smog can undo stat stages and Poison Gas increases status pressure; neither equals the old self-KO threat. |
| Late Cooltrainers | Removed attacks use current family tools: Persian Assurance, Charizard Air Slash, Exeggutor Psyshock/Hypnosis/Leech Seed, Cloyster Icicle Spear, Kingler Razor Shell/Bubble Beam, Tentacruel Acid Armor. | Physical/special fit, weather, multihit abilities and accuracy still require battle testing. Some inherited legal weak moves remain deliberately outside this bounded repair. |
| Attackless ordinary support sets | Clefairy gets Disarming Voice; Pidgey Wing Attack; Goldeen/Seaking Water Pulse; Vulpix Incinerate; Voltorb/Electrode Electro Ball, retaining support identity. | This increases real threat compared with accidentally attackless parties, particularly Ronald's four fish and screen users. |

All eight ordinary double formats and their party sizes remain. Eli/Anne's Clefairy uses Disarming Voice/Follow Me/Life Dew/Protect rather than uncontrolled Metronome. Kiri/Jan retain Charmander/Squirtle: Flamethrower/Dragon Breath and Water Pulse/Icy Wind, each with Helping Hand/Protect, replace competing sun/rain defaults. Other doubles retain their existing functional combinations. The source guard rejects friendly-fire spread attacks and Metronome in these ordinary doubles; it is an authoring constraint for this content pass, not a claim that such moves are universally forbidden.

## Stage and rematch risks

- All317 singles and8 doubles retain their numerical levels. Most ordinary opponents are substantially below the new major-boss caps. Victory Road's inherited42–48 versus the player's68 cap is a notable late-curve risk; this pass does not certify its challenge or earning pace.
- Optional Saffron Dojo parties at31–37 appear reachable through Tea/Cut before the player reaches that ceiling (two-badge cap28). Map/story execution and a useful warning or major-challenge treatment need coordinator follow-up; neither headers nor scripts are changed here.
- The22 existing ordinary held items are Black Belts on Fighting trainers. No ordinary trainer has bag supplies. Neither distribution changes.
- The repaired Vs. Seeker worker resolves the same authoritative trainer IDs. Because party levels, IDs and locations are preserved, its maximum-level eligibility is unchanged. Corrected species/moves automatically apply to repeats; no bonus EXP or payout is added. Four formerly invalid explicit-move trainers were excluded by that worker's initial selection, but their first battles are repaired here regardless.
- Rick and Ethan remain entirely unchanged, preserving their already validated no-fee practice payouts and teams.

## Verification

The RED checkpoint `e23c7447` recorded four failing content contracts: unsupported evolutions, stale moves, attackless defaults, and Metronome risk in doubles. That first invocation also discovered six imported League tests; the import was corrected so the targeted command now reports exactly **five** ordinary-team tests. GREEN at `ef1c66d5` passes all five. The complete content suite passes **34 tests**.

```sh
python3 -m unittest discover -s tests/content -p test_ordinary_trainers.py
python3 -m unittest discover -s tests/content
```

The entire party file also passed the real ARM preprocessor and upstream `trainerproc`, writing its generated header only to an automatically removed temporary directory. This establishes accepted party syntax, not a linked GBA build. No full engine build or emulator was run for these edits; the coordinator owns combined authentic trainer-table native/runtime checks.

A baseline-to-result audit confirms that every nonallocated trainer is byte-identical, every header is unchanged, and every allocated party retains count/order, levels, held items and non-move trait lines. All799 ordinary slots are checked for original species and source-supported evolution/access; every explicit move/ability is checked; all default or explicit sets are examined for contribution, and all eight doubles for party count and selected unsafe moves. This is bounded data coverage, not an engine coverage percentage or human balance claim.

## Exact slot changes

Every row uses the authoritative trainer ID and1-based party slot. Shared scripts and rematches keep those IDs.

| Trainer / slot | Map | Species / level | Change |
| --- | --- | --- | --- |
| `TRAINER_BEAUTY_SHEILA` / 1 | Route13_Frlg | Clefairy 29 | replace status-only generated set with attack/support set: Disarming Voice, Protect, Moonlight, Misty Terrain |
| `TRAINER_BIKER_ALEX` / 2 | Route15_Frlg | Grimer 28 | Screech → Poison Gas |
| `TRAINER_BIKER_ALEX` / 3 | Route15_Frlg | Weezing→Koffing 28 | Weezing → Koffing: no mainland specimen/evolution at level28 |
| `TRAINER_BIKER_ERNEST` / 3 | Route15_Frlg | Weezing→Koffing 25 | Weezing → Koffing: no mainland specimen/evolution at level25 |
| `TRAINER_BIKER_GERALD` / 2 | Route14_Frlg | Muk→Grimer 29 | Muk → Grimer: no mainland specimen/evolution at level29; Screech → Poison Gas |
| `TRAINER_BIKER_HIDEO` / 1 | Route16_Frlg | Weezing→Koffing 33 | Weezing → Koffing: no mainland specimen/evolution at level33 |
| `TRAINER_BIKER_ISAAC` / 1 | Route14_Frlg | Grimer 28 | Screech → Poison Gas |
| `TRAINER_BIKER_ISAAC` / 2 | Route14_Frlg | Grimer 28 | Screech → Poison Gas |
| `TRAINER_BIKER_JAXON` / 1 | Route17_Frlg | Weezing→Koffing 29 | Weezing → Koffing: no mainland specimen/evolution at level29 |
| `TRAINER_BIKER_JAXON` / 2 | Route17_Frlg | Muk→Grimer 29 | Muk → Grimer: no mainland specimen/evolution at level29; Screech → Poison Gas |
| `TRAINER_BIKER_LAO` / 1 | Route16_Frlg | Grimer 29 | Screech → Poison Gas |
| `TRAINER_BIKER_LUKAS` / 3 | Route14_Frlg | Grimer 26 | Screech → Poison Gas |
| `TRAINER_BIKER_MALIK` / 2 | Route14_Frlg | Grimer 29 | Screech → Poison Gas |
| `TRAINER_BIKER_NIKOLAS` / 1 | Route17_Frlg | Voltorb 29 | Sonic Boom → Electro Ball |
| `TRAINER_BIKER_NIKOLAS` / 2 | Route17_Frlg | Voltorb 29 | Sonic Boom → Electro Ball |
| `TRAINER_BIKER_RUBEN` / 1 | Route16_Frlg | Weezing→Koffing 28 | Weezing → Koffing: no mainland specimen/evolution at level28 |
| `TRAINER_BIKER_RUBEN` / 3 | Route16_Frlg | Weezing→Koffing 28 | Weezing → Koffing: no mainland specimen/evolution at level28 |
| `TRAINER_BIKER_VIRGIL` / 1 | Route17_Frlg | Weezing→Koffing 28 | Weezing → Koffing: no mainland specimen/evolution at level28 |
| `TRAINER_BIKER_VIRGIL` / 3 | Route17_Frlg | Weezing→Koffing 28 | Weezing → Koffing: no mainland specimen/evolution at level28 |
| `TRAINER_BIKER_WILLIAM` / 2 | Route17_Frlg | Weezing→Koffing 25 | Weezing → Koffing: no mainland specimen/evolution at level25 |
| `TRAINER_BIKER_WILLIAM` / 5 | Route17_Frlg | Weezing→Koffing 25 | Weezing → Koffing: no mainland specimen/evolution at level25; Self Destruct → Clear Smog |
| `TRAINER_BIRD_KEEPER_CHESTER` / 1 | Route15_Frlg | Dodrio→Doduo 28 | Dodrio → Doduo: no mainland specimen/evolution at level28 |
| `TRAINER_BIRD_KEEPER_SEBASTIAN` / 1 | Route13_Frlg | Pidgey 29 | replace status-only generated set with attack/support set: Wing Attack, Tailwind, Roost, Feather Dance |
| `TRAINER_CAMPER_JEFF` / 2 | Route6_Frlg | Raticate→Rattata 16 | Raticate → Rattata: no mainland specimen/evolution at level16 |
| `TRAINER_COOLTRAINER_ALEXA` / 1 | VictoryRoad_3F_Frlg | Clefairy 42 | Double Slap → Disarming Voice |
| `TRAINER_COOLTRAINER_ALEXA` / 2 | VictoryRoad_3F_Frlg | Jigglypuff 42 | Rollout → Icy Wind |
| `TRAINER_COOLTRAINER_ALEXA` / 3 | VictoryRoad_3F_Frlg | Persian 42 | Faint Attack → Assurance |
| `TRAINER_COOLTRAINER_ALEXA` / 5 | VictoryRoad_3F_Frlg | Chansey 42 | Egg Bomb → Echoed Voice; Soft Boiled → Life Dew; Minimize → Light Screen |
| `TRAINER_COOLTRAINER_COLBY` / 1 | VictoryRoad_3F_Frlg | Kingler 41 | Guillotine → Razor Shell; Bubble → Bubble Beam |
| `TRAINER_COOLTRAINER_COLBY` / 2 | VictoryRoad_3F_Frlg | Poliwhirl 42 | Double Slap → Waterfall |
| `TRAINER_COOLTRAINER_COLBY` / 3 | VictoryRoad_3F_Frlg | Tentacruel 42 | Barrier → Acid Armor |
| `TRAINER_COOLTRAINER_GEORGE` / 1 | VictoryRoad_3F_Frlg | Exeggutor 42 | Egg Bomb → Psyshock; Stun Spore → Leech Seed; Sleep Powder → Hypnosis |
| `TRAINER_COOLTRAINER_GEORGE` / 3 | VictoryRoad_3F_Frlg | Cloyster 42 | Spike Cannon → Icicle Spear |
| `TRAINER_COOLTRAINER_GEORGE` / 4 | VictoryRoad_3F_Frlg | Electrode 42 | Sonic Boom → Electro Ball |
| `TRAINER_COOLTRAINER_NAOMI` / 1 | VictoryRoad_1F_Frlg | Persian 42 | Faint Attack → Assurance |
| `TRAINER_COOLTRAINER_NAOMI` / 3 | VictoryRoad_1F_Frlg | Rapidash 42 | Fury Attack → Flame Wheel |
| `TRAINER_COOLTRAINER_ROLANDO` / 1 | VictoryRoad_1F_Frlg | Raticate 42 | Pursuit → Assurance |
| `TRAINER_COOLTRAINER_ROLANDO` / 5 | VictoryRoad_1F_Frlg | Charizard 42 | Wing Attack → Air Slash |
| `TRAINER_COOLTRAINER_SAMUEL` / 3 | ViridianCity_Gym_Frlg | Rhyhorn 38 | Take Down → Drill Run; Fury Attack → Stomp |
| `TRAINER_COOLTRAINER_WARREN` / 1 | ViridianCity_Gym_Frlg | Marowak 37 | Leer → Tail Whip |
| `TRAINER_COOLTRAINER_WARREN` / 2 | ViridianCity_Gym_Frlg | Marowak 37 | Leer → Tail Whip |
| `TRAINER_COOLTRAINER_WARREN` / 3 | ViridianCity_Gym_Frlg | Rhyhorn 38 | Take Down → Drill Run; Fury Attack → Stomp |
| `TRAINER_COOLTRAINER_YUJI` / 2 | ViridianCity_Gym_Frlg | Graveler 38 | Magnitude → Bulldoze; Mud Sport → Protect |
| `TRAINER_COOLTRAINER_YUJI` / 4 | ViridianCity_Gym_Frlg | Graveler 38 | Magnitude → Bulldoze |
| `TRAINER_COOLTRAINER_YUJI` / 5 | ViridianCity_Gym_Frlg | Marowak 38 | Leer → Tail Whip |
| `TRAINER_COOL_COUPLE_RAY_TYRA` / 1 | VictoryRoad_3F_Frlg | Nidoqueen 45 | Superpower → Earth Power |
| `TRAINER_COOL_COUPLE_RAY_TYRA` / 2 | VictoryRoad_3F_Frlg | Nidoking 45 | Megahorn → Poison Jab |
| `TRAINER_CUE_BALL_ISAIAH` / 2 | Route17_Frlg | Machamp→Machoke 29 | Machamp → Machoke: no mainland specimen/evolution at level29 |
| `TRAINER_CUE_BALL_JAMAL` / 3 | Route17_Frlg | Machamp→Machop 26 | Machamp → Machop: no mainland specimen/evolution at level26 |
| `TRAINER_ENGINEER_BERNIE` / 3 | Route11_Frlg | Magneton→Magnemite 18 | Magneton → Magnemite: no mainland specimen/evolution at level18 |
| `TRAINER_FISHERMAN_RONALD` / 1 | Route21_North_Frlg | Seaking 28 | replace status-only generated set with attack/support set: Water Pulse, Horn Attack, Aqua Ring, Rain Dance |
| `TRAINER_FISHERMAN_RONALD` / 2 | Route21_North_Frlg | Goldeen 28 | replace status-only generated set with attack/support set: Water Pulse, Horn Attack, Aqua Ring, Rain Dance |
| `TRAINER_FISHERMAN_RONALD` / 3 | Route21_North_Frlg | Seaking 28 | replace status-only generated set with attack/support set: Water Pulse, Horn Attack, Aqua Ring, Rain Dance |
| `TRAINER_FISHERMAN_RONALD` / 4 | Route21_North_Frlg | Seaking 28 | replace status-only generated set with attack/support set: Water Pulse, Horn Attack, Aqua Ring, Rain Dance |
| `TRAINER_GAMER_RICH` / 2 | Route8_Frlg | Vulpix 24 | replace status-only generated set with attack/support set: Incinerate, Protect, Confuse Ray, Will O Wisp |
| `TRAINER_HIKER_ALAN` / 1 | Route9_Frlg | Geodude 21 | Magnitude → Bulldoze; Mud Sport → Protect |
| `TRAINER_HIKER_ALAN` / 2 | Route9_Frlg | Onix 21 | Screech → Rock Tomb |
| `TRAINER_HIKER_ALLEN` / 1 | RockTunnel_B1F_Frlg | Geodude 25 | Magnitude → Bulldoze; Mud Sport → Protect |
| `TRAINER_HIKER_CLARK` / 1 | Route10_Frlg | Geodude 21 | Magnitude → Bulldoze; Mud Sport → Protect |
| `TRAINER_HIKER_CLARK` / 2 | Route10_Frlg | Onix 21 | Screech → Rock Tomb |
| `TRAINER_HIKER_DUDLEY` / 1 | RockTunnel_B1F_Frlg | Geodude 21 | Magnitude → Bulldoze; Mud Sport → Protect |
| `TRAINER_HIKER_DUDLEY` / 2 | RockTunnel_B1F_Frlg | Geodude 21 | Magnitude → Bulldoze; Mud Sport → Protect |
| `TRAINER_HIKER_DUDLEY` / 3 | RockTunnel_B1F_Frlg | Graveler→Geodude 21 | Graveler → Geodude: no mainland specimen/evolution at level21; Magnitude → Bulldoze; Mud Sport → Protect |
| `TRAINER_HIKER_LUCAS` / 1 | RockTunnel_1F_Frlg | Geodude 21 | Magnitude → Bulldoze; Mud Sport → Protect |
| `TRAINER_HIKER_LUCAS` / 2 | RockTunnel_1F_Frlg | Graveler→Geodude 21 | Graveler → Geodude: no mainland specimen/evolution at level21; Magnitude → Bulldoze; Mud Sport → Protect |
| `TRAINER_HIKER_TRENT` / 2 | Route10_Frlg | Graveler→Geodude 19 | Graveler → Geodude: no mainland specimen/evolution at level19 |
| `TRAINER_JUGGLER_GREGORY` / 1 | VictoryRoad_2F_Frlg | Mr Mime 48 | Double Slap → Dazzling Gleam |
| `TRAINER_PICNICKER_ALMA` / 1 | Route13_Frlg | Goldeen 28 | replace status-only generated set with attack/support set: Water Pulse, Horn Attack, Aqua Ring, Rain Dance |
| `TRAINER_SCIENTIST_BEAU` / 1 | SilphCo_5F_Frlg | Magneton→Magnemite 26 | Magneton → Magnemite: no mainland specimen/evolution at level26; Sonic Boom → Electro Ball |
| `TRAINER_SCIENTIST_BEAU` / 3 | SilphCo_5F_Frlg | Weezing→Koffing 26 | Weezing → Koffing: no mainland specimen/evolution at level26 |
| `TRAINER_SCIENTIST_BEAU` / 4 | SilphCo_5F_Frlg | Magnemite 26 | Sonic Boom → Electro Ball |
| `TRAINER_SCIENTIST_BRAYDON` / 3 | PokemonMansion_3F_Frlg | Voltorb 33 | replace status-only generated set with attack/support set: Electro Ball, Light Screen, Electric Terrain, Reflect |
| `TRAINER_SCIENTIST_CONNOR` / 1 | SilphCo_2F_Frlg | Grimer 26 | Screech → Poison Gas |
| `TRAINER_SCIENTIST_CONNOR` / 2 | SilphCo_2F_Frlg | Weezing→Koffing 26 | Weezing → Koffing: no mainland specimen/evolution at level26 |
| `TRAINER_SCIENTIST_CONNOR` / 3 | SilphCo_2F_Frlg | Koffing 26 | Self Destruct → Clear Smog |
| `TRAINER_SCIENTIST_CONNOR` / 4 | SilphCo_2F_Frlg | Weezing→Koffing 26 | Weezing → Koffing: no mainland specimen/evolution at level26 |
| `TRAINER_SCIENTIST_ED` / 1 | SilphCo_9F_Frlg | Voltorb 28 | Sonic Boom → Electro Ball |
| `TRAINER_SCIENTIST_ED` / 3 | SilphCo_9F_Frlg | Magneton→Magnemite 28 | Magneton → Magnemite: no mainland specimen/evolution at level28; Sonic Boom → Electro Ball |
| `TRAINER_SCIENTIST_IVAN` / 2 | PokemonMansion_B1F_Frlg | Electrode 34 | replace status-only generated set with attack/support set: Electro Ball, Light Screen, Electric Terrain, Reflect |
| `TRAINER_SCIENTIST_JERRY` / 3 | SilphCo_2F_Frlg | Magneton→Magnemite 28 | Magneton → Magnemite: no mainland specimen/evolution at level28 |
| `TRAINER_SCIENTIST_JOSE` / 1 | SilphCo_3F_Frlg | Electrode 29 | Sonic Boom → Electro Ball |
| `TRAINER_SCIENTIST_JOSE` / 2 | SilphCo_3F_Frlg | Weezing→Koffing 29 | Weezing → Koffing: no mainland specimen/evolution at level29 |
| `TRAINER_SCIENTIST_JOSHUA` / 2 | SilphCo_7F_Frlg | Muk→Grimer 29 | Muk → Grimer: no mainland specimen/evolution at level29 |
| `TRAINER_SCIENTIST_RODNEY` / 1 | SilphCo_4F_Frlg | Electrode 33 | replace status-only generated set with attack/support set: Electro Ball, Light Screen, Electric Terrain, Reflect |
| `TRAINER_SCIENTIST_TAYLOR` / 1 | SilphCo_6F_Frlg | Voltorb 25 | Sonic Boom → Electro Ball |
| `TRAINER_SCIENTIST_TAYLOR` / 3 | SilphCo_6F_Frlg | Magneton→Magnemite 25 | Magneton → Magnemite: no mainland specimen/evolution at level25; Sonic Boom → Electro Ball |
| `TRAINER_SCIENTIST_TAYLOR` / 4 | SilphCo_6F_Frlg | Magnemite 25 | Sonic Boom → Electro Ball |
| `TRAINER_SCIENTIST_TAYLOR` / 5 | SilphCo_6F_Frlg | Koffing 25 | Self Destruct → Clear Smog |
| `TRAINER_SCIENTIST_TED` / 2 | PokemonMansion_1F_Frlg | Weezing→Koffing 29 | Weezing → Koffing: no mainland specimen/evolution at level29 |
| `TRAINER_SUPER_NERD_AIDAN` / 1 | Route8_Frlg | Voltorb 20 | Sonic Boom → Charge Beam |
| `TRAINER_SUPER_NERD_AIDAN` / 3 | Route8_Frlg | Voltorb 20 | Sonic Boom → Charge Beam |
| `TRAINER_SUPER_NERD_AIDAN` / 4 | Route8_Frlg | Magnemite 20 | Sonic Boom → Electro Ball |
| `TRAINER_SUPER_NERD_GLENN` / 2 | Route8_Frlg | Muk→Grimer 22 | Muk → Grimer: no mainland specimen/evolution at level22 |
| `TRAINER_SUPER_NERD_LESLIE` / 1 | Route8_Frlg | Koffing 26 | Self Destruct → Clear Smog |
| `TRAINER_SWIMMER_MALE_AXLE` / 5 | Route19_Frlg | Tentacruel→Tentacool 27 | Tentacruel → Tentacool: no mainland specimen/evolution at level27 |
| `TRAINER_SWIMMER_MALE_DAVID` / 1 | Route19_Frlg | Goldeen 29 | replace status-only generated set with attack/support set: Water Pulse, Horn Attack, Aqua Ring, Rain Dance |
| `TRAINER_SWIMMER_MALE_DAVID` / 3 | Route19_Frlg | Seaking 29 | replace status-only generated set with attack/support set: Water Pulse, Horn Attack, Aqua Ring, Rain Dance |
| `TRAINER_SWIMMER_MALE_REECE` / 1 | Route19_Frlg | Goldeen 29 | replace status-only generated set with attack/support set: Water Pulse, Horn Attack, Aqua Ring, Rain Dance |
| `TRAINER_TEAM_ROCKET_GRUNT_10` / 1 | RocketHideout_B1F_Frlg | Grimer 20 | Minimize → Poison Gas |
| `TRAINER_TEAM_ROCKET_GRUNT_13` / 1 | RocketHideout_B2F_Frlg | Zubat 17 | Bite → Poison Fang; Leech Life → Absorb |
| `TRAINER_TEAM_ROCKET_GRUNT_13` / 4 | RocketHideout_B2F_Frlg | Zubat 17 | Bite → Poison Fang; Leech Life → Absorb |
| `TRAINER_TEAM_ROCKET_GRUNT_13` / 5 | RocketHideout_B2F_Frlg | Raticate 17 | Hyper Fang → Take Down |
| `TRAINER_TEAM_ROCKET_GRUNT_32` / 1 | SilphCo_8F_Frlg | Raticate 26 | Hyper Fang → Take Down |
| `TRAINER_TEAM_ROCKET_GRUNT_36` / 1 | SilphCo_8F_Frlg | Weezing→Koffing 28 | Weezing → Koffing: no mainland specimen/evolution at level28 |
| `TRAINER_TWINS_ELI_ANNE` / 1 | Route8_Frlg | Clefairy 22 | replace unpredictable Metronome in doubles with reliable Fairy attack: Disarming Voice, Follow Me, Life Dew, Protect |
| `TRAINER_TWINS_KIRI_JAN` / 1 | Route14_Frlg | Charmander 29 | retain contrasting starter pair without mutually cancelling weather: Flamethrower, Dragon Breath, Helping Hand, Protect |
| `TRAINER_TWINS_KIRI_JAN` / 2 | Route14_Frlg | Squirtle 29 | retain contrasting starter pair without mutually cancelling weather: Water Pulse, Icy Wind, Helping Hand, Protect |
| `TRAINER_YOUNG_COUPLE_LEA_JED` / 1 | Route16_Frlg | Rapidash→Ponyta 29 | Rapidash → Ponyta: no mainland specimen/evolution at level29 |
