# Safe Kanto field access

Implemented on `agent/field-access` from `2bebedee944411770bce2cb93060487b9f39731c`. All new gameplay entry hooks apply only under `P_SC_KANTO_RULES`; ordinary upstream test fixtures retain the original field behavior. This implements the restoration requirement in ADR-0011 and the five badge permissions in ADR-0027 under the lead's delegated field-access decision.

A compatible original-151, original-form, non-egg party Pokémon can perform an acquired HM's field action without learning the move. Fainted compatible Pokémon remain eligible, matching inherited field behavior. Both the badge and HM must be present:

| Field action | Badge | Bag item |
| --- | --- | --- |
| Flash | Boulder | HM05 |
| Cut | Cascade | HM01 |
| Fly | Thunder | HM02 |
| Strength | Rainbow | HM04 |
| Surf | Soul | HM03 |

Compatibility comes from the authoritative `CanLearnTeachableMove` lookup. Battle moves and their four slots are unchanged. The existing item system marks these HMs important, preventing ordinary deposit, giving, sale or disposal, so acquired permits remain in the bag. HMs remain reusable when taught normally.

## Entry and recovery behavior

- `engine/src/sc_field.c` and `engine/include/sc_field.h` own shared permit, compatibility, party-replacement, owned-Pokémon release and menu-page policy.
- `field_player_avatar.c:PartyHasMonWithSurf` and `scrcmd.c:ScrCmd_checkfieldmove` use the shared party lookup. Even legacy script calls that omit the unlocked-check argument require the badge and acquired HM. Existing scripts receive the valid party index and species needed by their animation/nickname paths.
- `party_menu.c` replaces learned-HM dependence with a FIELD submenu. Each page contains at most five field actions plus MORE and BACK; the parent menu contains one FIELD entry. The original eight-entry action buffer and existing window dimensions are respected. Learned Dig, Teleport, Soft-Boiled and Sweet Scent remain available in this submenu; excluded-region field actions are not added. Opening, paging and returning to the parent are covered by native host fixtures using the actual menu functions.
- `party_menu.c:DoesSelectedMonKnowHM` now evaluates the party after capture replacement, including the actual caught Pokémon selected by `GetCatchingBattler`. It permits a compatible replacement and blocks loss of the last currently available required field capability, including while standing on isolated land. This protection does not depend on whether the outgoing Pokémon still knows an HM.
- `pokemon_storage_system.c:InitCanReleaseMonVars` rejects release of the last compatible owned Pokémon for any currently acquired field permit. The search covers party and boxes, excludes the released location and handles a Pokémon currently held by the storage cursor. Ordinary PC deposits remain reversible at that same terminal. Existing minimum-owned-Pokémon release restrictions remain intact. A future remote-storage feature would also need party-after-deposit protection.
- `region_map.c:GetMapsecType`, `FilterFlyDestination` and `SetFlyDestination` restrict campaign Fly selection to visited mainland destinations. Kanto's region-map subtype distinguishes the mainland from Sevii; `REGION_KANTO` by itself would not. Unvisited destinations retain their existing restrictions.

The field effects and map scripts are preserved: Cut removes its local tree object, Strength activation uses its existing flag, Victory Road stores completed barriers and boulder positions, and Seafoam stores stopped-current flags and map layouts. This change does not bypass those puzzles or grant Waterfall/Dive permissions.

## Verification

TDD checkpoints:

1. `84fbb496`: missing shared field policy reproduced.
2. `3e34345d`: permit, compatibility, replacement, release and page policy passes.
3. `fe67767d`: actual Surf entry still rejects a compatible Pokémon that forgot Surf; compiling host fixture fails at that runtime assertion before entry/UI edits.
4. Integration changes pass the same fixture plus menu and Fly regressions.

Commands run:

```sh
python3 -m unittest discover -s tests/field -v
python3 -m unittest discover -s tests/safety -v
tests/field/check_native_objects.sh /path/to/generated/full-game/engine
git diff --check
```

Results: four field tests and six existing safety tests pass. The field tests compile the actual shared policy, Surf/script/catch callbacks, parent/submenu functions and mainland Fly selector with deterministic host fixtures. They cover all five badge/HM combinations, missing gates, excluded/egg Pokémon, absent learned moves, compatible and incompatible catches, alternative boxed recovery, menu overflow/paging/back behavior, unvisited destinations and excluded regions.

A separate host `--coverage`/gcov run of `sc_field.c` with the policy fixture executed 67/67 reported executable lines (100%). This is focused policy line coverage, not branch coverage or whole-engine coverage.

All six allocated C translation units compile to real ARM7TDMI objects with `-Werror -Wall`: `sc_field.c`, `party_menu.c`, `field_player_avatar.c`, `scrcmd.c`, `pokemon_storage_system.c`, `region_map.c`. The script reads actual generated assets/map headers and the existing preprocessor from the lead's full build, writes temporary objects under `/tmp`, and removes them afterward. It does not mutate the shared build or replace graphics/string macros with stubs.

## Still required after integration

A full linked ROM build and emulator checks remain necessary. In particular, visually exercise FIELD → MORE → BACK and field-effect return paths; verify all five moves after forgetting/save-load; catch replacements while surfing and on Seafoam/Route 20 land; release from party/box/storage cursor at Cinnabar; verify Cut reentry, Victory Road completion persistence and Seafoam current persistence. Test Fly from an eligible outdoor map to visited mainland destinations and cancellation from the destination screen. No Android or end-to-end campaign playtest was run by this worker.
