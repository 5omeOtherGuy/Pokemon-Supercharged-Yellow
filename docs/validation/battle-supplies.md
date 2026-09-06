# Trainer supplies implementation and validation

ADR-0027 is the policy. `sc_supplies_core.c` counts individual units and owns
reservations; `sc_supplies.c` classifies actual item effects, validates owned
inventory, seals saved selections, locks encounters, and commits an effective
item action once. The limits are three units total: HP 2, status 1, PP 1,
boost 1. Revives, mixed effects, reusable flutes and nonbattle items are excluded.

Selections can contain duplicate item IDs; their combined quantity must be
owned. Invalid reselection leaves the previous plan intact. Preparation consumes
nothing. Battle entry clamps missing units without substitution, leaving the
saved requested plan available for subsequent restocking. An empty plan is legal.
A reserved unit cannot be reserved by the other battler in doubles. Execution
rechecks inventory, HP/status/PP/stat state and Embargo before removing inventory.
Cancellation and ineffective actions spend nothing. Successful actions spend one
unit; battle end does not refund them.

Wild inventory use bypasses the trainer quota. NPC supply limits use the same
core, and only major trainers may load an opposing bag. Authored ordinary bags
were removed. Full Restores were replaced with Max Potions; Champion bags now
contain two Max Potions and one Full Heal. These changes satisfy the category
contract but do not establish encounter balance. The native catalog regression
checks trainer item arrays against major classification and limits. The initial
native test runner substituted sparse fixture data; only the explicit
`SC_TEST_CAMPAIGN=1` mode validates actual authored campaign arrays. That mode now
also requires nonempty major bags and verifies all six Champion bags contain
two Max Potions and one Full Heal.

Preparation uses the shared player/NPC passive catalogs and floors HP bag healing
to 110 percent before the caller clamps to maximum HP. Outside battle the helper
selects the player passive. Held items are separate from supply units. The module
records exact original held items consumed in each trainer/party slot; restoration
callers must preserve legitimate Recycle/Pickup recovery and distinguish actual
consumption from Knock Off/theft.

## Integration contract

The coding-first owner-build audit found that the required
`HandleAction_UseItem` commit call below was still absent. Source `188afee7`
connects it before friendship/effect dispatch. Two real native dispatcher
regressions first failed and then passed: a prepared Potion spends exactly one
unit, while a target that is already at full HP cancels without spending stock
or quota. A full simulated Potion turn also passes after `0094dc74` makes the
native recorded-player adapter reserve rather than pre-debit campaign items.
Earlier adapter/core tests alone did not establish this gameplay connection.

All callers include `sc_supplies.h`. Root and the effects worker own these hooks:

| Caller | Required operation |
| --- | --- |
| Center UI | Classify candidates with `ScSuppliesCategory`; atomically save item/quantity arrays with `ScSuppliesSetSelection`. |
| Boss preview | `ScSuppliesGetPlayerLoadout` returns exact owned units after revalidation. NPC bags remain the authored trainer array. |
| Battle initialization | `ScSuppliesBeginBattle` after final flags/trainer IDs and before actions; call for wild encounters too, resetting held history. FALSE means some requested units were unavailable. |
| Party/PP item selection | Require `ScSuppliesReserve` after ordinary target validation; skip early `RemoveBagItem` when `ScSuppliesApplies`. Owned BagMenu path already does this. |
| Partner cancellation | `ScSuppliesCancel` on the canceled battler; skip inherited `AddBagItem` refund because selection removed nothing. |
| AI selection | Filter `ScSuppliesCanUse`; reserve before mutating chosen item/history. |
| `HandleAction_UseItem` | Commit after resolving actor/item and before friendship/effects. Failed commit takes the ordinary ineffective item action path. It must not grant a replacement/free action. |
| `BS_ItemRestoreHP` | Call `ScSuppliesHealAmount` after item parameter calculation and before max-HP clamp. |
| Field `PokemonUseItemEffects` | Same healing helper for actual bag HP healing only; exclude held effects and level/stat recalculation. |
| `RemoveItem` | Record actual original-item consumption/destruction, including popped Air Balloon; exclude mere Knock Off/theft. |
| `TryRestoreHeldItems` | Query `ScSuppliesWasHeldConsumed` before restoring an original consumable; keep legitimate recovered held items. |
| Battle teardown | `ScSuppliesEndBattle` only after held restoration. Unexecuted reservations disappear without inventory changes. |

`BS_ItemCureStatus` also needs a `targetBattler < MAX_BATTLERS_COUNT` guard before
clearing nightmare on a sleeping bench target. The bag preview previously passed
NULL into `CannotUseItemsInBattle`; the owned implementation now resolves the
acting Pokémon. Single-status cures also incorrectly matched the all-status
volatile predicate; preview and native effect must require full mask equality.

## Evidence

- RED `126194cc`: portable unit/reservation test could not compile without core.
- GREEN `5d61bdd2`: core runtime assertions passed.
- RED `682ab343`: native actual-item/selection tests lacked the adapter header.
- RED `e5102bae`, compiler correction `7270e015`: Antidote native test exposed
  unrelated confusion/infatuation clearing. Effects worker reported native
  GREEN 2/2 with `c4b8f801` in its owned `battle_util.c`.
- GREEN `6d15557e`, `dd3bb2a6`: four host tests pass with actual core/adapter
  functions. Host fixtures supply inventory, party and item data; the target
  test executes the full production target predicate. Cases include zero
  inventory, atomic invalid reselection, duplicate units, cancellation,
  double reservation/commit, NPC budget, ineffective actions, bench targets,
  full/empty PP, max stats, fainting, Embargo, passive healing and held tracking.
- Six actual ARM objects compile with `-Werror`: core, adapter, item_use,
  item_menu, native selection tests and native status tests. Generated assets
  are read from the root build; temporary objects are removed by the script.

Reproduce local checks:

```sh
python3 -m unittest discover -s tests/supplies -v
tests/supplies/check_native_objects.sh /path/to/generated/full-game/engine
```

`ScSuppliesEnableForTests` exists only under TESTING, allowing isolated native
callbacks without changing unrelated upstream fixtures. Test teardown must reset
it unconditionally. This does not replace a full `SC_TEST_CAMPAIGN=1` native run.
At this handoff, the native selection/catalog tests have compiled but have not
been executed by this worker. Root/effects integration must run them with the
actual callback hooks. Full campaign, loss/reload persistence, doubles cancellation,
Center preparation/briefing agreement, and wild bag behavior still require
integrated runtime replay. No human or Android playtest is claimed.


### Later evidence clarification — native trainer fixtures

The subsequent default native catalog pass used `test/test_runner_battle.c`'s
fixture `gTrainers`, because `data.c` excluded the campaign table under `TESTING`.
It was not an all-authored-bags result. Host party-source checks and deliberately
controlled quota/consumption fixtures retain their stated scope. The counted
`tests/rematches/run_native.sh` wrapper runs the strengthened authored-bag case
separately with real campaign data and requires exactly one ordinary pass.


The subsequent counted campaign-data run on 2026-09-06 passed the strengthened
single authored-bag group with all652 real trainer records and all six Champion
assertions. Evidence: integration `build/sc-rematches-verified.bags.log` and
`build/sc-rematches-counted-final.log`, root `c21aa863` plus the coordinator's
then-uncommitted explicit campaign-data build hooks. This supersedes the earlier
fixture-only catalog result; it does not establish an ordinary battle journey.
