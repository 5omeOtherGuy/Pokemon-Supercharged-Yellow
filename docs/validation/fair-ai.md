# Campaign AI validation

The campaign trainer controller uses `sc_ai.c` rather than the upstream prediction and damage-simulation pipeline. Its adapter constructs value-only action options, then a pure scorer selects a singles action or jointly selects both doubles actions. Ties follow a stable order; no battle RNG is read or advanced by the scorer.

## Information boundary

The adapter may read the AI's own exact active/bench state and its remaining `gBattleHistory->trainerItems` inventory. For the player, it uses public species/form, level, displayed nature, moves, ability, held item, types, status icon, stat stages, visible field effects and the existing 48-pixel HP bar. It estimates ordinary stats using species, level, median IV 16 and zero training, with public nature modifiers. Estimated HP is projected from the visible bar; exact HP within a bar band does not enter evaluation.

Player IVs, EVs, training, numeric stats, unsupported injected mint alignment, selected actions, move targets, switches, item choices and future RNG are outside the observation. The adapter never installs estimated Pokémon into authoritative battle or party data. It does not call the upstream damage simulator. It uses the project's public active-effect API and a separate API for the AI's prospective switch-in effects; neither may expose inactive player collections.

The upstream implementation remains for noncampaign modes and ordinary upstream tests. SC integration tests explicitly enable the campaign path. All regular trainer battles use it in production, except link/recorded-link, frontier/palace and in-game-partner modes. Wild Pokémon retain their upstream behavior.

## Decision paths and tactics

Early guards cover per-turn AI setup, per-battler decisions, move selection, action emission, trainer-item selection and forced replacement selection. Party-knowledge initialization also bypasses the old pipeline. The caller must suppress the legacy random AI iteration order in `HandleTurnActionSelectionState` when `ScAiEnabled()` is true. `SetFinalChosenTarget` continues to apply legal target handling to the cached move/target.

The scorer rewards effective damage and estimated knockouts, penalizes friendly fire, avoids wasting two attacks on one already defeated target and reserves each shared party slot or consumable only once. It evaluates an ally's Protect and Helping Hand together with the partner's chosen attack. The adapter considers speed/priority, STAB and type effectiveness, common immunity/offense/defense abilities and items, public capability effects, status, setup, recovery, screens, Tailwind, Trick Room, weather, terrain, pivot moves and switching under trapping rules. Trainer healing/status/PP/stat supplies use their actual remaining inventory slots and standard controller actions.

Damage is an explicit estimate, not an engine-exact oracle. It assumes an ordinary accuracy-adjusted median damage roll and does not predict the player's committed action. Variable-power moves without a dedicated estimate use a 60-power prior; Hidden Power never reads IV-derived type. Hazard layering, detailed residual sequences and every generation-specific move/ability interaction are not fully modeled. Automated tactical cases establish particular choices, not professional balance or enjoyable difficulty.

## Reproduction

From the worktree root:

```sh
mkdir -p build/fair-ai
cc -std=c11 -Wall -Wextra -Werror -DSC_AI_HOST_TEST -Iengine/include \
  engine/src/sc_ai.c engine/test/sc_ai/host.inc.c -o build/fair-ai/host-tests
build/fair-ai/host-tests
python3 engine/test/sc_ai/test_boundary_source.py
make -C engine BUILD=firered TESTS='SC AI*' check -j2
```

The engine build requires the repository's native build prerequisites. Production requires linked implementations of `ScGetPublicBattlerEffects` and `ScGetPublicSwitchInEffects`; missing implementations fail linkage. The test target alone includes explicit weak neutral fixtures in `test/sc_ai/public_effect_fixture.c` so adapter tests can run before the shared provider is integrated. Strong real provider definitions override them. Record the symbol type with `arm-none-eabi-nm engine/pokefirered-test.elf`: `W` means neutral fixture evidence; `T` means the real provider. Neutral fixtures do not validate capability effects. Check the executed test count because the upstream runner can return success for an empty prefix.

For host scorer coverage, add `--coverage` to the host compiler command and run `gcov -b -c` on the resulting `sc_ai.gcno`. This measures only the pure scorer selected by `SC_AI_HOST_TEST`; it does not measure ARM adapter coverage.

## Evidence

Executed TDD checkpoints include the missing pure API, missing campaign boundary guards and missing joint Helping Hand metadata, followed by their passing host/source tests. The ARM compiler accepts the adapter, guards and SC battle test source with warnings treated as errors. Runtime battle results are pending the current native test build and shared public-effects linkage; this document will be updated with its actual count and failures before handoff.
