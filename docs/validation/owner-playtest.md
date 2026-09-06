# Owner playtest build validation

The owner requested coding-first work, automated battle tests and a PC-cheat build for their own playtesting. Agent-controlled campaign playthroughs stopped. This report describes the test build, not final game acceptance.

## Implemented

The existing integrated campaign, bosses, original-151 acquisition routes, progression, services, scouting, AI, supplies and rematches remain in source. The test build adds the [PC tools](../testing-cheats.md), with ordinary builds disabled by default and isolated object directories. It also fixes automatic double-rematch refusal loops, trainer sight-list bounds, the inaccessible supply-rejection explanation, and the missing supply-commit call in the actual battle action dispatcher.

## Executed checks

The following checks were executed before packaging, with production code through `372e7e59`. Build identity and reproducibility are reported by the generated `BUILD-MANIFEST.json`; they are not inferred from these tests:

- All 103 host tests pass across 15 suites (briefing, cheats, content, economy, field, presentation, progression, release, rematches, roster, safety, services, simulation, supplies and test tools). Logs and counts are retained locally in `build/owner-test/host-results.json`.
- The integrated native regression executes 119 groups: 109 passes and 10 deliberate framework checks, with no unexpected failures or skips. `bash tests/effects/run_native.sh` verifies the counts and expected outcomes, including teardown recovery. Evidence: `build/battle-effects/regression-native.log`.
- The owner native wrapper passes ten groups with tools enabled and three with tools disabled. It covers editors, real inventory/candy hooks and actual item-action dispatch. The subsequent full Potion-turn check passes both prepared and unrestricted quota parameter cases: HP 1 → 21 and stock 3 → 2, with ordinary quota spent and unrestricted quota retained. Evidence: `build/owner-test/native-tools-{1,0}.log` and `build/sc-potion-quota-modes.log`.
- Six native editor groups pass: encrypted nature/gender/development preservation, rejected writes, full-storage atomicity, every original species placement, level/species/PP editing and CRC-protected switches. Source `1fdf122b`; `build/sc-owner-native-final.log`.
- Seven source-backed native battle interaction groups pass, covering nine parameter cases. Six associated host checks cover all 73 major/League teams and selected counter availability. See [simulation assumptions and findings](../balance/simulation.md).
- Four native campaign rematch groups plus one actual authored-bag audit pass after the refusal/bounds fixes. Eight host rematch checks pass, including full-capacity trainer sight sorting under UBSan.
- Six host tests execute the actual cheat hook bodies: stock validation/key consumption, cap reversal, automatic encounter/sight avoidance, player-only supply quota bypass and held-item restoration between battles.
- Build-mode checks prove normal/owner and synthetic/campaign native object outputs are separate. The source policy rejects tracked ROMs and embedded legacy transfer ROM dependencies.
- The battle-item menu regression passes: failed uses remain explainable and dispatch no item action. A stale field-menu test fixture was repaired to include the already implemented Training entry; four field checks pass.

The native supply-dispatch regression first failed: an accepted Potion was not removed, and a full-health target followed the same effect script. The commit function existed but had no gameplay caller. Connecting it before item effects fixes both cases. A complete simulated Potion turn then exposed a duplicate debit in the native recorded-player adapter; changing that adapter to reserve supplies fixes the test path. Dispatcher, full-turn and broader regression checks pass. No fight-difficulty tuning was made from these isolated item tests. See the [supply validation record](battle-supplies.md).

## Evidence boundaries

The continuous ordinary journey previously reached Viridian Forest before Brock. It has not been continued for this build. No completed campaign, all-151 ordinary save, complete rematch journey, human difficulty/preparation assessment or final release acceptance is claimed.

The new PC model and scripts are compiled and automatically tested; a complete owner-operated UI journey remains unperformed. Older Android evidence covers the same engine's boot, imported save, ordinary save/reload and basic play, with exact historical versions in [Android validation](android-runtime.md). Do not confuse that with complete Android validation of this artifact. Physical phones and touch comfort/audio/performance remain owner checks.

No public patch is packaged yet: the exact supported distribution base and patch round-trip remain release work. The private local `.gba` provides direct owner testing without pretending that requirement is complete.
