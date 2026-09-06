# Campaign access and rules audit

Audited against `8464edbcb580b9d8ad78c21204b6e8c70f549485` in `agent/campaign-safety`. This report separates implemented guards from shared-engine integration requirements. It is not a campaign playtest or release certification.

## Implemented in this change

`P_SC_KANTO_RULES` is true for FIRERED production, and for FIRERED tests explicitly compiled with `SC_TEST_CAMPAIGN=1`. Ordinary upstream `TESTING=1` fixtures retain their original mechanics and broad species database. Only the explicit campaign test configuration validates these production guards.

| Path | Behavior |
| --- | --- |
| `engine/include/config/pokemon.h`, `battle.h` | Gen III species EXP yield, ordinary yield × defeated level / 7, participant splitting, trainer ×1.5; no catch EXP, unevolved multiplier, or affection bonus. Modern physical/special split, Fairy typing, weather and terrain remain enabled. |
| `engine/include/config/item.h` | Reusable TMs. The existing held EXP Share behavior and disabled automatic global EXP Share remain unchanged. |
| `engine/include/config/pokemon.h` | HMs can be forgotten. Safe field recovery still requires the integration described below. |
| `engine/include/config/debug.h` | No overworld debug menu, battle debug menu or sprite visualizer in campaign builds, even without RELEASE=1. |
| `engine/src/battle_gimmick.c` | Common gimmick eligibility rejects Mega/Z/Dynamax/Tera activation in campaign builds. |
| `engine/src/daycare.c` | Reverse egg-species lookup cannot select species IDs after Mew, so Pikachu-family eggs do not become Pichu. The broad database remains available for upstream tests. |
| `engine/src/party_menu.c` | Mint, EV-reset and EV-reducing callbacks reject before mutation or item removal; the medicine callback rejects vitamin/feather/mochi EV effects. Healing and Rare Candy retain their normal callbacks. |
| Twelve mainland `*PokemonCenter_2F_Frlg` maps and `data/scripts/cable_club_frlg.inc` | Upstairs is a lounge: only the downstairs warp remains; attendants give lounge dialogue; deliveryman and automatic link scripts are removed. Sevii maps are excluded. |
| `engine/data/scripts/questionnaire.inc` | Campaign interaction cannot unlock Mystery Gift/Event. |

Ordinary EXP pacing changes materially from the inherited modern formula. Equal-level and low-level opponents now award Gen III unscaled EXP; held Lucky Egg and classic held EXP Share still work. Root-owned battle training uses its separate Gen III baseline. No route-duration, grind-time or stage-balance claim has been measured here.

## Shared-engine integration checklist

These paths are owned by the integrating lead and are **not patched by this commit**. Apply with the same `P_SC_KANTO_RULES` boundary; do not globally remove later species or alter ordinary upstream test fixtures.

1. `engine/src/pokemon.c`, `GetSpeciesFormChanges`: return NULL before retrieving a form table when campaign rules apply. Existing form-target callers accept NULL. This closes item/weather/move form changes in addition to the gimmick activation guard.
2. `GetSpeciesPreEvolution`: inside the species scan, `if (P_SC_KANTO_RULES && i > SPECIES_MEW) continue;`. The summary relearner can otherwise discover excluded babies if predecessor lookup is enabled later.
3. `GetBoxMonData`, `MON_DATA_HIDDEN_NATURE`: return personality nature directly under campaign rules, rather than XOR with `hiddenNatureModifier`. In `SetBoxMonData` for that attribute, keep the modifier zero under campaign rules. This preserves acquired personality nature and its actual stat effect, including previously modified data.
4. `PokemonUseItemEffects`, immediately after obtaining `itemEffect`, return TRUE (no effect) under campaign rules for the following predicate. This common function handles direct/script/AI uses as well as the guarded party UI:

```c
(itemEffect[4] & (ITEM4_EV_HP | ITEM4_EV_ATK))
    || (itemEffect[5] & (ITEM5_EV_DEF | ITEM5_EV_SPEED
                        | ITEM5_EV_SPDEF | ITEM5_EV_SPATK))
    || item == ITEM_FRESH_START_MOCHI
```

5. `engine/src/battle_main.c`, `ClearSetBScriptingStruct`: force `gBattleScripting.battleStyle = OPTIONS_BATTLE_STYLE_SET` for the campaign, even if saved options contain SHIFT. New-game defaults alone do not enforce this.
6. `engine/src/battle_util.c`, `GetAttackerObedienceForAction`: return OBEYS immediately under campaign rules, before any obedience RNG/check. Generation-selection settings alone do not disable outsider disobedience.
7. `engine/src/event_data.c`, `IsMysteryGiftEnabled` and `IsMysteryEventEnabled`: return `!P_SC_KANTO_RULES && FlagGet(existingFlag)` if defense against imported/edited enable flags is required. Normal-game unlock and delivery paths are already closed here.
8. Defensively check evolution/form/acquisition results in the root species boundary. Do not replace an invalid producer with an arbitrary valid species: preserve a safe failure result and validate each caller. Static campaign tests already enforce the authored original-151 acquisition/trainer data.

### Field recovery seam

`P_CAN_FORGET_HIDDEN_MOVE` alone is insufficient to guarantee traversal. `engine/src/field_player_avatar.c:PartyHasMonWithSurf` still checks learned Surf; `engine/src/scrcmd.c:ScrCmd_checkfieldmove` checks the learned field move and badge permission. The owned `engine/data/scripts/field_move_scripts.inc` scripts depend on the resulting party index for field animation and nicknames. Party-menu field commands are assembled from learned moves in `engine/src/party_menu.c`.

A player can forget the last Surf, then replace the only compatible party member while isolated from a Center. Retained reusable HMs do not guarantee restoration in that state. The lead must supply the agreed always-reachable recovery/field-service hook and preserve legitimate badge + HM acquisition gates. Exercise Cut, Strength, Surf and Flash interactions plus Fly menu access; do not silently grant early field access.

## Validation

- RED commit `bfdd283f` exposed inherited defaults, excluded baby lookup, gimmick activation and Center link entry failures before production edits.
- `python3 -m unittest discover -s tests/safety -v`: six tests pass. The tests compile actual extracted engine functions with host fixtures: four build-policy modes; breeding predecessor chain; gimmick eligibility; EV item classification preserving healing/candy; relearner excluding known and future-level moves. Map checks inspect actual warp/object data for twelve mainland Centers. Upstream incorrectly labels Sevii maps REGION_KANTO too, so the test explicitly excludes their names.
- `arm-none-eabi-gcc -fsyntax-only` with ARM7TDMI, `-Werror -Wall`, production FIRERED definitions: `party_menu.c`, `daycare.c`, `battle_gimmick.c` pass. The command uses the lead worktree's generated map headers read-only and stubs string/graphics preprocessing macros; this validates C syntax/types, not assembled assets or linking. Exact command is in `tests/safety/check_arm_syntax.sh`.
- `git diff --check`: pass.
- Source audit: `DEXNAV_ENABLED=FALSE`, `DN_FLAG_DEXNAV_GET=0`, `B_VAR_DIFFICULTY=0`; ordinary relearner `P_ENABLE_ALL_LEVEL_UP_MOVES=FALSE` and `P_PRE_EVO_MOVES=FALSE`. No reachable Kanto difficulty-setting script was found. Broad unused Hoenn facilities remain in source.

Still required after integration: real `make -C engine BUILD=firered -j2`, production-rule native tests with `SC_TEST_CAMPAIGN=1`, emulator rejection/no-consumption tests for Mints/EV items, original-form closure across item/battle/save paths, Center upstairs entry/exit and all HM recovery cases. This worker did not run a full ROM build or claim these runtime outcomes.
