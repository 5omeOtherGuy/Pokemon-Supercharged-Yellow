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
The initial rematch task did not edit their original first-battle records.
The subsequent ordinary-trainer correction `ef1c66d5` (integrated into frozen
source `d2dd72da`) resolves unsupported species/moves in that wider scope; see
[the ordinary-trainer report](../balance/ordinary-trainers.md). It updates twelve
parties used by this table while preserving every ID, party size, level and
format, so the dynamic resolver needs no matching table edit.

Some low-level evolved opponents are legitimate caught forms rather than
premature player evolutions: examples retained after that source audit include
Yasu's Route11 Raticate17, Billy's Power Plant Muk33, and ocean Seaking/Seadra
parties. Unsupported cases such as Bernie's Magneton18, Hideo's Weezing33,
Virgil's Weezing28, Isaiah's Machamp29 and Lea/Jed's Rapidash29 were corrected
in the ordinary-trainer task. This is source legality evidence, not certification
of ordinary-trainer difficulty or player preparation time.

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
Its focused source selection includes all four mandatory runner files, including
`test/test_test_runner.c`, explicitly named by the linker script. The first clean
campaign-mode build exposed that missing fourth object after compilation; this
was a build dependency failure, not an executed gameplay failure.

The native file defines four campaign groups for real step charging, table resolution,
high-ID badge/readiness and actual battle-exit cleanup, plus packed expiry and
malformed counters. The coordinator then ran the counted wrapper against root `c21aa863` plus its
campaign-data Makefile/data-selection hooks on 2026-09-06: **four rematch groups
and one authored-bag group passed**, with no failures or skips. The retained logs
are `build/sc-rematches-verified.log`, `build/sc-rematches-verified.bags.log` and
`build/sc-rematches-counted-final.log` in the integration worktree. This run used
all 652 real trainer records, including the six Champion bag assertions, and
supersedes the earlier fixture-only catalog result. The build-mode hooks were
uncommitted at execution and are owned/documented by the coordinator.

## Recorded diagnostic runtime checks

On 2026-09-06, before the owner changed the testing approach, the ordinary mGBA
headless driver executed two explicitly disclosed diagnostic ROM setups against
frozen production source `d2dd72dae3be97c094e7f7d5be80456b505aae49`:

- ROM SHA-256 `f98b9b8d02ddd0d9a35f6baf2a81750a42c4c89cf525e8411c9dfe927f556acd`.
- ELF SHA-256 `8cfa881f978aa3b1ff35494d91928272adc9a2a4e4b29ba3771711583554edfe`.

Only the disposable ROM's isolated Mom interaction was changed. The first setup
granted Mewtwo21 and Pikachu21 with explicit Modest natures/IV15, Boulder Badge,
and a warp before the real Vermilion gift. The second granted Mewtwo21, Pikachu5,
one badge, the device and one Revive, then warped beside Route8's twins. Neither
setup injected trainer victories, charge or readiness. All subsequent commands
were ordinary inputs, reads, screenshots and a normal save/reset; there were no
runtime RAM writes, cheat commands or savestates. These are test scenarios, not
organic campaign progress or stage-available team recommendations.

| Case | Observed result on that frozen source |
| --- | --- |
| Real gift and first battle | Device received through its actual dialogue; walked to Route11 and defeated Eddie's original Ekans21. |
| Charge and controls | Bag activation rejected undercharge; actual 99-step state displayed one remaining step through registered Select; the 100th step enabled signaling and consumed the charge. |
| Save and repeat | Ready slot3 survived normal save/reset/Continue. Eddie's rematch dialogue entered actual trainer ID9 and the same Ekans21; a second win paid another ordinary ₽336. Readiness then cleared and talking again produced postbattle text. |
| Expiry and transition | At response step99, readiness remained; step100 cleared it and preserved full recharge. A later signal followed by walking into Vermilion cleared readiness and the response timer, retaining 16 recharge steps. |
| High-ID doubles | First fight used trainer376 and the correct Clefairy22/Jigglypuff22 doubles party. Pikachu fainted; the ready rematch correctly refused with one usable Pokémon, but repeatedly re-approached after refusal and blocked movement/Bag access. This was a gameplay defect, not a successful recovery test. |

The [evidence folder](../../tests/rematches/evidence/d2dd72da/) retains manifests,
checksums, 803 gift-sequence commands and 481 doubles-RED commands, responses and
screenshots. `python3 tests/rematches/verify_runtime_evidence.py` verifies those
records, state assertions and checksums, including the historical failed
recovery behavior. It does not claim that the later fix was played. The checked-in
fixture helpers document the historical setup; no further agent-controlled play
or fixture generation is planned under the owner's changed approach.

The sight refusal loop was reproduced with the actual source helper in
`29d42df2`, then fixed in `15c27521`. `GetRematchFromScriptPointer` is used only by
the automatic sight path; it now requires two usable party members for a ready
doubles team. Readiness and manual refusal remain intact, so the player can
recover and retry. Native follow-ups `a9a8d44a` / `d133909b` extend the existing
high-ID group with one, two and fainted-partner cases. The first native object
attempt used an obsolete creation API; the corrected source compiles with
`SC_TEST_CAMPAIGN=1`. Execution of these new native assertions is coordinated by
the lead and was pending at this worker's handoff.

A second source-executed regression (`96eff976`) reproduced an index16 read in
the actual sight-list function. Fix `0f019032` changes both sorting and visitation
to stop before the populated count. Empty, sparse, non-trainer and full16 lists
pass with undefined-behavior/bounds instrumentation. All **eight rematch host
groups pass**; complete ARM objects for `battle_setup.c`, `trainer_see.c` and the
campaign-enabled native rematch test compile with `-Werror`.

## Remaining owner gameplay and pacing validation

The owner will play the test build. The updated doubles sight fix has **not** been
played after integration. Confirm that one-usable refusal allows movement and
Bag/Center recovery, both twins' interactions work, and recovery permits the
pending full-ID double rematch without consuming readiness early. The prior
attempt to reach Anne remained trapped in Eli's repeated sight dialogue, so it
does not validate Anne's path. Retry after loss and the full bag/registered flow
on the final build also remain to be checked.

Record actual EXP, money, Practice Points, survivor training and consumed supplies
on wins and losses. Measure recruit catch-up and additional focused training at
several badge stages and battle speeds, including weak recruits supported by an
established core. Fixed route parties deliberately remain easier as that core
grows; late-stage gym rematches offer a stronger alternative. Neither the number
of available repeats nor these tests establishes ADR-0006's 20–30-minute-or-more
preparation target. Full ordinary journey, human enjoyment and Android rematch
interaction are not established by this worker's evidence.
