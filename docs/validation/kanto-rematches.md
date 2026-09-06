# Kanto Vs Seeker implementation and validation

The Vermilion Vs Seeker now enables repeatable ordinary training and earning on
20 mainland route maps. This is implemented source and focused verification,
not evidence of a completed journey or validated preparation time. The task
started from `9d479bc5` on `agent/kanto-rematches`; see ADRs 0006, 0008, 0027,
0029 and 0031 for the preparation, loss and reward contracts. ADR-0035 owns the
coordinator's repeatable-training policy.

## Behavior and source ownership

- The existing woman at `VermilionCity_PokemonCenter_1F_Frlg` gives the item for
  free. No additional badge prerequisite or payment was introduced. Route11's
  ten original level17–21 trainers are nearby and fit the one-badge cap22.
- Walk 100 ordinary steps with the item to charge it. A successful signal makes
  every supported, defeated, visible, currently level-eligible trainer ready;
  there is no random refusal. Readiness expires after 100 further steps or an
  area transition. Each actual trainer battle consumes that trainer's readiness
  on exit, including loss. Walking recharges independently during that window.
- A signal without an eligible nearby rematch retains its charge and explains
  that trainers must first be defeated and stronger parties require more badges.
  Buildings, islands and maps outside the selected mainland routes cannot use
  the item. Initial charge is earned by walking; no instant recharge service was
  added. Forced-movement steps are excluded by the existing field step callback.
- Each of 67 selected trainer entries reuses its original party at every table
  tier. The table contains 144 Pokémon, levels11–37, and seven double teams.
  No party, level, EXP bonus, money multiplier or new supply bag was authored.
  Trainer identity stays a full `u16`; saved readiness is one byte containing
  exactly 0 or 1. The 78-entry table and 100 saved slots retain their sizes.
- `ScRematchResolve` checks ownership, first victory, the current map, original
  species/form, valid party size and every member's actual level against
  `ScLevelCap`. This withholds an over-cap party rather than lowering its level.
  Eligibility is checked before signal consumption and again before dispatch.
- Existing `data/scripts/trainers_frlg.inc` first-battle and rematch branches are
  preserved. The inherited double-rematch macro encodes a single battle bit;
  `BattleSetup_ConfigureTrainerBattle` now obtains the rematch format from the
  authoritative party before the two-live-Pokémon check. Failing that check keeps
  readiness and cannot start a fight. Ordinary first battles keep their path.
- Gym and League rematches remain separate, including their individually earned
  capability rewards. Ordinary Vs Seeker repeats grant no such rewards and use
  no opposing bag items. Their ordinary EXP, prize money, Practice Points and
  personal training pass through the shared battle/outcome systems. Losses retain
  the normal penalties; the seeker itself never pays or grants EXP.

`engine/src/data/rematches_frlg.h` is the selected identity/map list. The actual
source parties remain in `trainers_frlg.party`. `sc_rematches.c/h` owns Kanto
eligibility and readiness, `vs_seeker.c` owns charging/scanning/movement, and the
bounded `battle_setup.c` branches connect existing script dispatch and cleanup.
`item_use.c` restricts locations. Hoenn Match Call generation/registration is
inactive in the FireRed build, even before receiving the item; the unrelated
Hoenn table is retained for other builds. These are FireRed source-selection
rules in both production and its native tests, not a separate test-only opt-in.

## Selected route coverage

Ranges describe the unchanged parties, not promises about when a map is reached.
All selected parties fit by the four-badge cap44; field and story access still
apply. Later gym and League rematches provide stronger training opponents.

| Route | Trainers | Party levels | Selected opponents |
| --- | ---: | --- | --- |
| 3 | 3 | 11–14 | Robin, James, Ben |
| 4 | 1 | 31 | Crissy |
| 6 | 2 | 16–20 | Keigo, Ricky |
| 8 | 4 | 22–24 | Julia, Rich, Andrea, Eli/Anne |
| 9 | 3 | 18–23 | Alicia, Jeremy, Caitlin |
| 10 | 3 | 20–29 | Carol, Herman, Mark |
| 11 | 10 | 17–21 | Eddie, Hugo, Dillon, Dave, Jasper, Darian, Braxton, Yasu, Dirk, Bernie |
| 12 | 4 | 22–29 | Ned, Chip, Luca, Gia/Jes |
| 13 | 3 | 28–30 | Alma, Sebastian, Valerie |
| 14 | 3 | 29–33 | Beck, Donald, Kiri/Jan |
| 15 | 4 | 26–33 | Yazmin, Edwin, Celia, Ron/Mya |
| 16 | 4 | 28–33 | Luke, Koji, Hideo, Lea/Jed |
| 17 | 3 | 28–33 | Virgil, Isaiah, Billy |
| 18 | 2 | 26–34 | Jacob, Ramiro |
| 19 | 3 | 29–30 | Reece, Richard, Lia/Luc |
| 20 | 3 | 31–35 | Melissa, Missy, Dean |
| 21 North | 4 | 27–33 | Ronald, Wade, Spencer, Lil/Ian |
| 21 South | 3 | 31–37 | Claude, Nolan, Jack |
| 24 | 2 | 14–16 | Reli, Timmy |
| 25 | 3 | 15–17 | Franklin, Joey, Wayne |

The free early Rick and Ethan repeat-talk scripts introduced by the economy task
remain separate. They are not silently converted into Vs Seeker-only encounters.
Ron/Mya retain their original Black Belts; the integrated Celadon fifth-floor
shop sells replacements/player counterparts for 1000. Other selected parties
hold no equipment. Current compiled level-up data and actual TM/HM compatibility
validate explicitly authored moves; unspecified moves use the engine's normal
current-level move initialization.

Four initially selected trainers failed that move audit and were replaced in
this list: Clark by Herman, Gerald by Beck, Lao by Luke, and Leslie by Andrea.
Their original first-battle records remain unchanged and need an ordinary-trainer
review: Geodude Magnitude/Mud Sport, Onix/Muk/Grimer Screech, and Koffing Self
Destruct were unavailable at those current source levels.

Some preserved originals have evolved species below the player's evolution
threshold: Yasu's Raticate17, Bernie's Magneton18, Luca's Electrode29, Hideo's
Weezing33, Lea/Jed's Rapidash29, Virgil's Weezing28, Isaiah's Machamp29, Billy's
Muk33, Lia/Luc's Seaking30, Melissa/Missy's Seaking31 and Ronald's Seaking28.
These are inherited opponent exceptions, not new evolutions granted to the
player. This task preserves their identities and levels; it does not certify
ordinary trainer balance or claim these meet player evolution timing.

## Executed checks

Native RED commit `c5777b7b` reproduced both failures in the actual GBA runner:
charging remained 0 instead of 99, and defeated Ben resolved to 0 instead of 1.
The coordinator retained `build/sc-rematches-red.log`. Source RED `1fdcb4c9`
required the missing Kanto map/party coverage before implementation `aeb9f247`.

All 29 existing content tests and six rematch host tests pass:

- Real table entries resolve to persistent mainland outdoor objects and their
  existing first/rematch script branches, preserving singles/doubles and no bags.
- Source teams are original151; compiled current level-up and TM/HM data validate
  explicit moves. All specified route coverage and training-level ranges hold.
- The actual Kanto C adapter exercises all 65536 input IDs, every badge cap and
  Championship state, invalid/missing parties, lost ownership, wrong maps,
  malformed readiness and slot canaries under undefined-behavior/bounds checks.
- The actual scanner functions handle 64 templates with 16 live objects, missing
  objects, sentinel IDs, stale maps, invalid sprites, graphics IDs above255,
  noncontiguous local IDs, NULL scripts and a malformed template count.
- The counted native wrapper rejects empty results, a missing second bag test,
  expected failures and command failures, and requires the campaign build option.
- The actual dispatch function preserves first battles, rejects ineligible or
  underfilled doubles without battle/consumption, then resolves the full trainer
  ID on a valid retry. Its script-stack dependencies are boundary spies.

Complete production `sc_rematches.c`, `vs_seeker.c`, `battle_setup.c`, `item_use.c`
and the native test source compiled as ARM objects with `-Werror` and the pinned
ARM GCC13.2.1 toolchain. Only generated root assets were read; temporary object
files were removed. No duplicate full build was launched by this worker.

```sh
python3 -m unittest discover -s tests/rematches -v
tests/rematches/run_native.sh
```

The default native run reached three passes and one failure: the high-ID trainer
had party size0. Investigation established that `data.c` omits campaign trainers
under `TESTING`, while `test/test_runner_battle.c` supplies sparse test fixtures.
ID1 was a test trainer, not Ben. That run therefore did not validate campaign
party lookup. The coordinator's data-selection hooks let `SC_TEST_CAMPAIGN=1`
use the actual campaign trainer table and retain the ordinary fixture table when
the option is off. All four rematch cases compile only in that explicit campaign configuration.
The counted wrapper selects `BUILD=firered SC_TEST_CAMPAIGN=1` and requires exactly
four passing groups, rejecting an empty or mismatched filter. It then runs the
strengthened actual authored-bag test separately and requires exactly one pass.

The native file defines four campaign groups for real step charging, table resolution,
high-ID badge/readiness and actual battle-exit cleanup, plus packed expiry and
malformed counters. Check the coordinator's GREEN execution separately from
these ARM compilation results.

## Remaining runtime and pacing validation

Run an actual gift-to-rematch sequence with ordinary input: receive the item,
walk99/100 steps, use it through both bag and registered-item controls, approach
and talk to a responding trainer, win, recharge, repeat and save/reload. Repeat
after loss and after leaving the route. Check zero-money access, visible charge
messages and expiry animation. Exercise both objects of a double trainer with
one then two usable Pokémon, and verify no immediate second fight after victory.

Record actual EXP, money, Practice Points, survivor training and consumed supplies
on wins and losses. Measure recruit catch-up and additional focused training at
several badge stages and battle speeds, including weak recruits supported by an
established core. Fixed route parties deliberately remain easier as that core
grows; late-stage gym rematches offer a stronger alternative. Neither the number
of available repeats nor these tests establishes ADR-0006's 20–30-minute-or-more
preparation target. Full ordinary journey, human enjoyment and Android rematch
interaction are not established by this worker's evidence.
