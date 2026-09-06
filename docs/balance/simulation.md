# Bounded campaign battle simulations

These tests execute tactical interactions in the real GBA battle runner. They
are not complete boss battles, win-rate estimates, a campaign playthrough, or
human balance acceptance. Agent-controlled campaign gameplay stopped at the
owner's request; this work uses code and automated tests only.

## Source and execution

The source audit starts at `73d5d7ce`. It reuses the existing campaign/League
party parsers and `tests/roster/roster_data.py`, whose host C probe selects the
actual configured species and level-up data. Six host groups pass:

```sh
python3 -m unittest discover -s tests/simulation -v
```

The union contains **73 teams, 310 party members and 20 doubles teams**. Of
those teams, 17 carry a weather-setting move, 14 a terrain-setting move, 56
Protect, and 41 at least one of Quick Attack, Extreme Speed, Aqua Jet, Ice Shard,
Mach Punch, Bullet Punch, Fake Out or Sucker Punch. Seventeen members hold an
item. These are source inventory counts, not measures of tactical strength.
Existing [boss](campaign-bosses.md) and [League](champion-finale.md) audits own
the broader authored move/evolution legality checks.

Run the focused native suite with:

```sh
bash tests/simulation/run_native.sh
```

It uses `BUILD=firered SC_TEST_CAMPAIGN=1`, the three runner sources, runner
self-test definitions and `test/sc_simulation/stage_interactions.c`, with filter
`SC simulation:*`. The wrapper requires exactly **7 passed/total groups**;
two parameterized groups produce nine tactical cases. An empty filter result
cannot pass the wrapper. The separate campaign build cache links the real
authored trainer database, avoiding the default synthetic trainer table.

`AuthoredMon` invokes the production trainer generator for the selected party
entry. NPC species, level, nature, IVs, training, innate ability, held item and
four moves therefore come from compiled source instead of a copied roster.
The host tests guard the selected party-slot identities. Player fixtures use
the runner's default Hardy, zero-IV/untrained setup, the stated level and
selected moves. **Explicit initiative speeds isolate these interactions**;
they are not the species' naturally calculated Speed stats. Boss trainer
identity, assigned capability/passive builds, bench choices and prepared bags
are not reproduced by these fixtures. One case opts into the actual SC AI;
the other six script both sides. The root runner resets test opt-ins after
every group.

## Cases and findings

| Case | What the assertion establishes | What it does not establish |
| --- | --- | --- |
| Brock: Geodude versus Pikachu | SC AI selects Bulldoze; Electric damage is ineffective while Ground coverage damages Pikachu. | A complete two-Pokémon boss policy or Pikachu solo viability. |
| Brock: Mankey versus Onix | Two Low Kicks defeat the authored Onix while Mankey moves first and Onix uses Rock Tomb. | An OHKO, natural speed advantage, or success at Mankey's level-8 acquisition point. |
| Brock: Nidoran♂ versus Geodude | Double Kick produces two damage events but leaves the authored Sturdy Geodude alive. | A one-turn Sturdy bypass or a safe sweep through Ground attacks. |
| Misty: Ivysaur/Pikachu versus Psyduck/Starmie | Helping Hand increases Razor Leaf's spread damage; the partner is unharmed. | A win through enemy concentrated attacks, Protect or switching. |
| Koga: Nidoking versus Weezing | Levitate blocks Earth Power; Thunderbolt provides damaging alternative coverage. | A free Ground sweep or guaranteed victory against Koga's complete team. |
| Sabrina: Pikachu/Snorlax versus Mr. Mime/Hypno | Psychic Terrain blocks hostile priority; ordinary Crunch still damages Hypno. | A full solution to screens, concentrated attacks or the complete doubles team. |
| Blaine: Golduck versus Ninetales | Rain replaces Drought's sun and substantially increases Water Pulse damage. | Automatic weather dominance, a switch-in calculation, or a full boss win. |

The initial native run at root `cf9c70d2` compiled and executed all seven groups:
**five passed; two failed**. The failed one-turn knockout assumptions left
Onix at **12 HP** after Low Kick and Geodude at **21 HP** after Double Kick.
Fighting is 2× against Rock/Ground, not 4×: Ground adds no Fighting weakness.
The corrected tests retain this finding, exercising a two-turn Mankey line
and Nidoran's multi-hit chip rather than modifying stats to force a knockout.
The root's initial log is `build/sc-simulation-native.log`.

The corrected integrated rerun at source `f5c94ce0`
passes all **7 groups / 9 parameter cases**, with no failed or skipped groups.
Its log is `build/sc-simulation-refined-native.log`. No production battle stats
were changed to satisfy these checks.

## Availability and remaining work

The host checks confirm Mankey and Nidoran♂ in the actual pre-gym Route 22
land encounters, and Low Kick/Double Kick in their level-up data by level 15.
The other tested level-up tools are available by their stated fixture levels.
Nidoking's Thunderbolt and Snorlax's Protect have source TM compatibility;
TM24 is offered at Celadon's prize room and TM17 is placed in the Power Plant.
This checks source placement, not a simulated travel/purchase route or its cost.

No new full damage matrix or replacement battle engine was created. Held-item
consumption, active SC effect stacking and information-boundary invariance
remain covered by their existing focused native suites; these seven cases do
not add held-item coverage. Follow-up simulation should be driven by specific
owner observations: full-party choices, realistic initiative, low/high
training, supplies, active builds and repeated boss attempts remain outside
this inexpensive first set.
