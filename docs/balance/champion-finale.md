# Yellow finale and League encounter notes

These are provisional encounter sets under ADRs 0010, 0012, 0015, 0017, 0027 and
0029. Source and compilation checks establish legal data and branch identity;
they do not establish win rates, preparation time, enjoyable difficulty or a
completed journey.

## Problem and source boundary

At `664c7c66`, all three first Champion parties still ended in the FRLG starters
at levels 57–63, despite preceding League opponents reaching 68. Rematches also
retained those starters. Lorelei and Agatha had inherited singles-era moves in
doubles, including Surf friendly fire, physical attacks on special attackers,
and sleep/evasion loops. Lance's level-68 Gyarados still used Dragon Rage and
Twister. Fourteen allocated party bodies were replaced; trainer IDs, headers,
bag lines, scripts and formats were preserved.

Authoritative data: `engine/src/data/trainers_frlg.party`. The Yellow reference
is pret/pokeyellow `e89ead154b9968aa50eed9328ff2b38b6c194382`,
`data/trainers/parties.asm:Rival3Data`, inspected from the pinned local audit
clone. Its three Champion rosters share Sandslash, Alakazam and Exeggutor:

| Earlier outcome | Saved branch | First / rematch ID | Other members and ace |
| --- | --- | --- | --- |
| Win laboratory and early Route 22 | 0 / CHARMANDER | 340 / 622 | Cloyster, Ninetales, **Jolteon** |
| Win laboratory; lose or skip early Route 22 | 1 / BULBASAUR | 339 / 621 | Cloyster, Magneton, **Flareon** |
| Lose laboratory, regardless of Route 22 | 2 / SQUIRTLE | 338 / 620 | Ninetales, Magneton, **Vaporeon** |

The legacy constant names remain implementation identifiers. Real script dispatch
in `PokemonLeague_ChampionsRoom_Frlg/scripts.inc` still uses `VAR_STARTER_MON`.
The Flareon branch swaps Cloyster/Magneton order relative to original Yellow so
that the shared NPC capability catalog gives its special-damage Focus assignment
to Magneton. Other branches likewise retain a special attacker before the ace.
No old starter silently survives in a reachable Champion variant.

## Initial numerical envelope

First-run training is explicitly **64 per stat**, rematches **80 per stat**,
both below the eight-badge ceiling of 96. This is permanent-training data, not
an extra badge multiplier. Existing IVs remain 30 for Elite Four and 31 for
Champion. Natures and species-legal innate abilities are now explicit. Each
first ace reaches its intended progression level; all first opponents remain
at or below 68 and rematches at or below 75. No automatic normalization or
player training completion is assumed.

Elite Four bags remain two Max Potions. Champion bags remain two Max Potions
and one Full Heal. Each ace retains a Sitrus Berry. Lance's unevolved Dragonair
also hold Eviolite, preserving their support roles rather than giving them
unexplained Dragonite-level stats. Eviolite is an implemented item and does not
consume a bag unit. A mainland player Eviolite source was not found at this base;
it must not be advertised as an available player preparation requirement.

## Team plans and retained weaknesses

| Encounter | Levels, first → rematch | First-run contribution | Rematch change |
| --- | --- | --- | --- |
| Lorelei, doubles, 310 / 616 | 64–65 → 71–74 | Dewgong opens snow and Icy Wind alongside Skill Link Cloyster. Slowbro offers a bulky special attack/recovery option; Jynx uses special STAB and Fake Tears. Lapras supplies Freeze-Dry and Helping Hand. Protect and single-target Water attacks replace indiscriminate Surf. | Dewgong adds Encore; Cloyster can Shell Smash; Jynx uses snow-accurate Blizzard. These create setup and weather-management questions without OHKO moves. |
| Bruno, singles, 311 / 617 | 65–66 → 72–74 | Sturdy Onix sets Stealth Rock and uses Defense-based Body Press. Iron Fist Hitmonchan supplies elemental punches/Mach Punch; Hitmonlee offers speed and Sucker Punch. The second Onix uses Rock Head Double-Edge and Body Press. No Guard Machamp combines accurate Cross Chop, Knock Off and Bulk Up. | The second Onix uses Curse with Body Press. Higher training makes physical setup harder to ignore. Fighting weakness to Psychic/Fairy and Onix's Water/Grass weaknesses remain. |
| Agatha, doubles, 312 / 618 | 66–67 → 73–75 | Gengar and Intimidate Arbok open with special damage, Glare and Sucker Punch. Golbat supplies Tailwind and physical attacks. Levitate Haunter uses Will-O-Wisp/Hex and Taunt. The final Gengar has reliable special STAB and a burn option. | Golbat adds Haze; the ace can exploit inflicted status with Hex. There is no Hypnosis/Dream Eater dependency, trapping sleep loop or evasion move. Normal immunity, status prevention and fast Psychic attacks retain roles. |
| Lance, singles, 313 / 619 | 67–68 → 73–75 | Intimidate Gyarados now uses physical Waterfall/Crunch and Dragon Dance. One Eviolite Dragonair provides Thunder Wave, special coverage and Safeguard; the other uses Dragon Dance/Outrage. Aerodactyl has physical STAB, Taunt and Roost. Inner Focus Dragonite can set up with Dragon Claw/Fire Punch. | The physical Dragonair evolves; the ace gains Multiscale and Earthquake. The remaining Dragonair keeps its separate support role. Ice/Fairy and Electric pressure remain effective, but setup cannot be ignored. |
| Jolteon Champion, singles, 340 / 622 | 66–68 → 73–75 | Sandslash offers Earthquake/Knock Off and setup. Alakazam is a special attacker with recovery. Exeggutor and Drought Ninetales form a sun package; Cloyster offers physical Ice coverage. Volt Absorb Jolteon provides fast Electric/Ghost coverage and Thunder Wave. | Sandslash adds Rock Tomb, Ninetales adds Nasty Plot and Cloyster gains Shell Smash. Ground attacks answer Jolteon, while its partners demand broader coverage. Sun helps the Grass/Fire plan but weakens Cloyster's Water attack. |
| Flareon Champion, singles, 339 / 621 | 66–68 → 73–75 | Psychic Terrain Exeggutor and Reflect Alakazam support physical attackers. Cloyster can Shell Smash; Magneton provides Light Screen and Electric/Steel coverage. Flash Fire Flareon uses physical Flare Blitz/Superpower and Flame Charge. It has no priority move undermined by its own Psychic Terrain. | Sandslash adds Rock Tomb and Magneton uses Analytic. The slower physical ace retains recoil, Superpower stat drops and Water/Ground/Rock weaknesses rather than receiving free speed or bulk. |
| Vaporeon Champion, singles, 338 / 620 | 66–68 → 73–75 | Exeggutor uses Leech Seed, Ninetales uses Flash Fire/burn utility, and Magneton supplies screens/status. Water Absorb Vaporeon uses Surf/Ice Beam and Haze to contest physical setup. This branch emphasizes coverage and disruption rather than automatic sun. | Ninetales adds Nasty Plot; Magneton uses Analytic; Vaporeon exchanges Protect for Acid Armor while retaining Haze. Electric/Grass special pressure still answers its physical-defense plan. |

The source catalog is part of the matchup. Lorelei has Composure/Tempo; Bruno
Formation/Offense; Agatha Composure/Tempo/Preparation; Lance Tempo/Offense; the
Champion Formation/Composure/Offense. Their aces receive Clear Head, Recovery,
Equilibrium, Momentum and Momentum+Brace respectively. The catalog's partial
supporting assignments remain on separate teammates. These modest effects stack
with the newly coherent moves; they require integrated measurement, not an
assumption that familiar species imply familiar damage.

## Accessible counterplay hypotheses

The following are candidate cores, not prescribed solutions or proven wins.
All cited moves below are present in compiled current level-up data by level 68;
the regression checks this rather than assuming an optional TM or egg source.
Current-level relearning and free official ability selection are Center services,
so a tactic needing a change must be prepared before the League sequence.

| Candidate party/core | Contribution hypotheses | Cost and weaknesses to measure |
| --- | --- | --- |
| Pikachu/Raichu, Charizard, Blastoise, Venusaur, Snorlax, Pidgeot | Raichu has Thunderbolt/Light Screen for Lorelei; Blastoise Icy Wind/Protect helps doubles; Charizard has Flamethrower/Air Slash and Tailwind; Venusaur has Seed Bomb and Sunny Day; Snorlax has Crunch/High Horsepower; Pidgeot has Tailwind/Roost. This familiar core can change leads and moves while keeping most members. | Pikachu may remain unevolved but is substantially frailer. Ice attacks punish Venusaur/Pidgeot; Bruno's Rock coverage punishes careless Flying switches. Snorlax is not immune to Poison damage or burn, only Ghost attacks. |
| Raichu, Blastoise, Nidoking, Alakazam, Snorlax, Butterfree | Nidoking's Earth Power answers Magneton/Arbok; Alakazam's Psychic pressures Poison/Fighting; Butterfree has Bug Buzz and Tailwind for Exeggutor and speed support. Blastoise/Raichu keep useful roles in Lorelei. | Butterfree is vulnerable to Ice/Rock and needs safe entry. Earth Power cannot hit Levitate Haunter. Coverage and reserve HP through five battles matter more than a type-matchup checklist. |
| Rotate Clefable or Lapras into either core; optionally Machamp | Clefable has Moonblast/Misty Terrain for Dragon pressure and grounded status prevention. Lapras has Ice Beam/Freeze-Dry for Lance and bulky Water targets. Machamp's Knock Off can remove Dragonair's Eviolite. | Clefable fears Agatha's Poison moves and cannot simply lead throughout. Lapras fears Electric attacks. Recruiting and training specialists must remain practical; no perfect-IV or maximum-training requirement is assumed. |

Acquisition evidence in this revision:

- Pikachu: starter plus `wild_encounters.json` Viridian Forest; Thunder Stone:
  `CeladonCity_DepartmentStore_4F_Frlg/scripts.inc` regular stock.
- Bulbasaur: `CeruleanCity_House4_Frlg:ScY_Bulbasaur`; Charmander:
  `Route24_Frlg:ScY_Charmander`; Squirtle: `VermilionCity_Frlg:ScY_Squirtle`
  after the Thunder Badge. Their ordinary evolutions are below the League cap.
- Pidgey and Caterpie: Route 1/Viridian Forest; Nidoran male: Route 22/Route 2;
  Abra: Routes 5–8; Clefairy: Mt. Moon; Machop: Rock Tunnel/Route 10. These are
  actual FireRed encounter tables, not generic franchise locations.
- Moon Stone: the same Celadon store sells it. Kadabra and Machoke evolve at
  level 37 without trading in `gen_1_families.h`.
- Snorlax: Route 12/16 Lv30 scripts require the Poké Flute. Lapras is the Lv25
  Silph 7F gift. These alternatives are available before the Championship.

No counterplay argument requires Mewtwo, postgame access, breeding, trading,
Egg Moves, Eviolite ownership, or an undisclosed boss set. Possibility does not
prove that a returning player will discover or enjoy the preparation route.

## Verification and remaining work

RED `df7e503e` exposed wrong Champion identities, inherited doubles hazards and
missing explicit traits. GREEN `d0bb21a4` implements the fourteen parties.
`tests/content/test_champion_finale.py` executes the existing script-state VM for
all early-result Champion branches, declines, and all eight Elite Four variants.
It records the trainer ID at the briefing boundary; shared door animations are
stubbed. This does not execute the GBA script engine, animation or battle AI.

The legality test evaluates the actual configured species/level-up headers with
host C. Non-level moves must be both in the engine's official-source learnable
union and its current TM/HM list. Every changed member has a legal species,
four distinct moves, an available ability, explicit nature/training and a legal
held-item constant. The fourteen party headers—including settled bags—are
byte-for-byte unchanged from `664c7c66`.

The actual `trainerproc` accepted the whole trainer file. A complete `data.c`
ARM object compiled against the newly generated header, using root-generated
assets read-only, `-DFIRERED -DTESTING=0`, APCS/ARM7TDMI Thumb flags and `-Werror`.
The unrelated inherited unused graphics constant warning was suppressed.
Temporary headers/objects were removed. The complete content suite passed **22
tests**, and the roster suite passed **10 tests**. No full ROM build or tactical
native battle test was run by this worker.

```sh
python3 -m unittest discover -s tests/content -v
python3 -m unittest discover -s tests/roster -v
```

Before acceptance, measure all three finale branches with multiple plausible
cores, partial training and fixed acquired natures. Record failed attempts,
preparation/restocking time, AI setup/weather/support choices, Protect usage,
remaining HP/PP after each League member, and whether the first-run→rematch jump
is appropriate. Specifically test snow/Skill Link pressure, No Guard Cross Chop,
Agatha's status/Hex targeting, Eviolite/Multiscale counterplay and Champion Focus
support assignments. Verify ordinary saves, retries, scouting agreement and
consumed resources through a complete League run. Human and Android evidence
remain required; none is claimed here.
