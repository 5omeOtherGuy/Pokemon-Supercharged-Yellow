# Battle effect integration validation

This change applies the published capability and trainer-passive percentages to
ordinary physical/special damage after the engine's normal damage modifiers.
The attacking and defending assignments are evaluated separately, including each
target of a spread move. Fixed, level-based, reflected, fractional-HP and OHKO
move damage retain their existing specified amounts; those paths return before
these ordinary-damage multipliers. Immunities still produce zero damage.

Speed applies the same public percentage after ordinary speed modifiers. Clear
Head prevents freeze through the engine's common status-eligibility function,
including secondary effects; it does not prevent the underlying attack's damage
or bypass existing status rules. It does not grant frostbite immunity.

Recovery is a separate once-per-battler end-of-turn event after Ingrain and before
Leech Seed/poison. It uses the normal healing animation, message and HP update
script, respects Heal Block, skips absent/fainted/full-HP battlers, and heals at
least one HP when a positive fraction rounds down. Recovery heals 1/32 maximum
HP, Recovery Coach 1/64, and both together 3/64 in one event. Other recovery
sources, such as Leftovers, still use their own events. The same player effects
apply in wild battles; wild opponents do not inherit an NPC trainer build.

Delayed damage uses the original attacker's party assignment when that attacker
has left the field. Its current stored HP band and status determine conditional
effects; a delayed hit is not treated as the original attacker's first turn.
A scoped calculation override leaves the actual active party index and public
active-slot observations unchanged, and is cleared before the engine restores
its temporary battle-Pokémon copy.

## Evidence

Test fixtures explicitly assign capabilities to
synthetic parties; these are native engine tests, not organic campaign evidence.
The initial speed API test failed compilation because the API was missing, then
compiled after implementation (commits a952b606 and dddf7ac6).

The upstream battle test runner has a private 1024-byte stack. Observation
snapshots in the AI interaction test therefore use static EWRAM storage.


The full native RED run had nine intended failures: offense, defense/spread,
delayed damage, freeze and healing were missing; fixed damage and speed passed.
The GREEN checkpoint bdbddb09 ran 15 groups: 14 passed and one deliberately
failed inside EXPECT_FAIL to exercise unconditional teardown. Serial logs verify
that the failure is immediately followed by the passing isolation check.

Run the focused native regression from the repository root:

```sh
tests/effects/run_native.sh
```

The script builds the selected test ELF with two jobs, then runs serially with
mGBA. It includes the three required test-runner sources explicitly; the empty
filter runs the selected upstream runner self-tests as well. Their intentional
failure/crash cases are checked as expected results, not gameplay failures. The
result validator rejects empty runs, unexpected outcomes, skipped placeholders,
and an incorrect teardown-fixture ordering. Logs remain under the ignored
`build/battle-effects/` directory. The first broad run had 83 passing groups and
10 expected framework/teardown cases; the final expanded run is recorded below.

Strong providers were checked with:

```sh
arm-none-eabi-nm engine/pokefirered-test.elf | rg ' Sc(GetPublicBattlerEffects|GetPublicSwitchInEffects|ApplyDamageEffects|ApplySpeedEffects)'
```

All four symbols were strong `T` definitions. No neutral weak fixture is selected
by this test target. `ScEffectsEnabled` is active under the campaign production
configuration, including wild battles. Tests explicitly opt into effects and AI;
the runner unconditionally clears effects, AI, progression, supplies, and
delayed-source state when reporting any result. Ordinary upstream tests therefore retain their
baseline rules even after a failed SC fixture.

## Integration seams and limits

The coordinator owns the speed caller in `GetBattlerTotalSpeedStat` (6645f6c6),
progression providers and broader reward hooks (24aac763, 2e0eabac). This worker
adds `ScProgressionRecordFaint` at the start of `SetValuesOnFaint` so every faint
reaches the eligibility record, and resets progression opt-in during teardown.
The coordinator validates faint/revive reward eligibility separately.

The five added text/end-turn support paths are `include/battle_scripts.h`,
`include/constants/battle_end_turn.h`, `include/constants/battle_string_ids.h`,
`src/battle_message.c` and `data/battle_scripts_1.s` under `engine/`. No custom
battle interpreter, fake game simulation or authoritative player-stat mutation
was introduced by effect evaluation. AI uses the same public effects provider;
its new positive test switches from a physical move to an otherwise equivalent
special move after the player's visible Brace assignment changes. Existing
hidden-stat, pending-command, target and battle-RNG invariance tests remain green.

These deterministic native tests establish engine behavior for their fixtures.
They do not establish campaign balance, organic availability, complete human
playthrough, Android behavior, or emulator/hardware parity. No statement or branch
coverage percentage was collected from the ARM runtime.


## Supply and inference follow-up

`ItemHealMonVolatile` now requires the entire all-status mask before clearing
both confusion and infatuation. The native Antidote regression failed before
this fix and passed afterward; Full Heal still clears both.

AI supply options check the locked remaining quota, and action emission reserves
a valid unit before changing the chosen item or trainer history. A stale cached
item choice is rejected without consuming its history slot. The AI's item-heal
estimate includes the public Preparation multiplier. Reserving a player's unit
does not alter the opponent's observation or decision inputs: the new native
reservation-invariance case and the previous hidden-data cases pass.

The generic player-stat prior is now IV 23, the midpoint of the public campaign
range 15..31, with zero training and the public nature modifier. This changed the
portable estimate assertions from RED to GREEN without inspecting actual player
IVs. These are approximate estimates. Protect scoring recognizes recovery
availability but does not yet quantitatively model its stacked amount.

`TryRestoreHeldItems` consults the supplies module's consumed-original record.
It preserves a legitimately recovered original item, prevents restoring a spent
Focus Sash/Air Balloon/berry, restores ordinary Knock Off removals, and retains
the existing return policy for a different item taken from an NPC. The six-case
native fixture explicitly records consumption and simulates these restoration
states. Actual consumption recording in command/removal paths is coordinated
separately: the caller must record after item-remapping/early-return checks in
`Cmd_removeitem`, and at Incinerate, Bug Bite and Natural Gift bypasses. Transfers
through StealTargetItem/Bestow/Symbiosis and Knock Off are not consumption.
Those caller hooks are outside this worker's source ownership; the fixture alone
is not proof that every real removal path records correctly.

Final validation at the code handoff through 54525d81:

- The focused native target reports 102 groups: 92 passing groups and 10 expected
  framework/teardown failures, no unexpected failures or skipped placeholders.
  The framework also deliberately checks crash recovery; only its two named
  expected-crash fixtures are accepted by the log validator.
- All 11 SC AI native groups pass, including public effect sensitivity, exact-stat
  and HP-band invariance, pending commands/targets/RNG, and hidden reservations.
- The portable AI host suite, two boundary source checks and three available
  supply host suites pass. The additional bag audit passed against the native
  runner's sparse fixture trainer table; that result did not validate authored
  campaign bags. Actual campaign-data validation requires `SC_TEST_CAMPAIGN=1`
  and nonempty-bag assertions (see the Kanto rematch validation record).
- Native source build uses ARM GCC 13.2.1 and the imported mGBA test runner.
  No Android or human-play evidence was gathered by this task.

Later correction validation: the explicit campaign-data run on 2026-09-06
passed one strengthened authored-bag group, including all six Champion bag
assertions. The integration log is `build/sc-rematches-verified.bags.log`; see
`kanto-rematches.md` for the source/build-mode context. This provides the actual
campaign-data evidence that the earlier fixture-table pass did not supply.

The coordinator explicitly retains this worker's generated cache and logs for
integration reuse. No running process owns them at handoff; they are not release
artifacts and are not committed.
