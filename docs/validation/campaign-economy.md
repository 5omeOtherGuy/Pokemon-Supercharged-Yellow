# Campaign economy runtime validation

Six isolated desktop mGBA fixtures passed on production source **9d479bc5**. This validates the targeted scripts and transactions; it does not establish campaign difficulty, earning burden, organic access, human playability, or Android behavior. No production source changes resulted from this runtime pass.

The frozen input ROM SHA256 is `73759e33b3fe975f887e9b6286002ad0b00ef6a0bf8abd8da821fade2025afe3`; the paired ELF is `763258ce59c9a7002bbd59638cc347c22896eaf47d94003a455fea2f5512cbf7`. Parent supplied immutable artifacts in `full-game/build/sc-economy-runtime/`. This revision includes the Pikachu rare-held Light Ball removal. Vs. Seeker charging/rematches remain disabled in this revision; only its gift transaction is tested here.

## Observed results

Evidence is in [tests/economy/evidence/9d479bc5](../../tests/economy/evidence/9d479bc5). Every listed image was visually inspected at its native 240×160 resolution. Filenames on intermediate exploratory screenshots describe the intended checkpoint, which sometimes arrived later; use the specific confirmed checkpoints below.

| Scenario | Actual result | Confirmed evidence |
| --- | --- | --- |
| Celadon5F, no badges | Equipment list ends at Light Clay; no Light Ball. Focus Sash costs ₽500 and buying one changes cash ₽13000→12500, bag0→1. | `celadon/before-badge-stock-end.png`; state at frame18926 |
| Celadon5F, four injected badges | Reopening stock adds Light Ball at ₽5000. Buying one changes cash ₽12500→7500; bag contains one Sash and one Light Ball. | `celadon/four-badge-stock-end.png`, `light-ball-quantity.png`, `light-ball-paid.png`; frame20756 |
| Cerulean berry house | Ordinary cash shop; Sitrus200, Lum300, Leppa200, Eviolite3000, Magnet1000 rendered. Two Sitrus cost ₽400: cash13000→12600, berries0→2. | `berries/late-stock-confirmed.png`, `two-sitrus.png`, `sitrus-paid.png`; frame22092 |
| Indigo entry Center | Sitrus/Lum/Leppa, Hyper Potion800, Ether400, Sash500 and Balloon300 rendered. One Balloon costs ₽300: cash13000→12700, bag0→1. Cancel returns control to field. | `indigo/replacement-stock.png`, `balloon-paid.png`, `shop-free-exit.png`; frame20031 |
| Forest Rick | Actual first fight and replay retain Weedle6+Caterpie6. Cash0→72→144. First-win callback displays future practice instructions and ends; no immediate replay prompt. Later talk opens YES/NO. B declines, leaving cash and the entire active100-byte Pokémon unchanged. Walking through his current sight line and idling360frames triggers no approach. | `rick/first-fight.png`, `first-progress.png`, `first-payout.png`, `first-postbattle.png`, `replay-payout.png`, `postwin-sight-line.png`, `consent-before-decline.png`, `declined-free-exit.png`; decline frames36080→36201 |
| Route24 Ethan | Actual original and replay Mankey18. Cash0→360→720. First-win callback returns normally. Walking his current sight line does not autoapproach. Later talk requires consent; B leaves cash and active Pokémon unchanged. YES begins the ordinary repeat fight. | `ethan/first-return.png`, `first-sight-no-approach.png`, `decline-prompt.png`, `declined.png`, `replay-start.png`, `replay-mankey.png`, `replay-payout.png`; decline frames23832→23953 |
| Vermilion Vs. Seeker gift | With all30 key slots occupied, actual dialogue says BAG is full; claim flag remains false and all slot contents unchanged. Clearing one diagnostic slot permits actual delivery of exactly one Vs. Seeker. The flag becomes true after the receipt finishes. Subsequent conversation gives explanation and no second item. | `gift/full-bag-failure.png`, `space-freed-retry.png`, `gift-claimed.png`, `repeat-conversation.png`, `repeat-no-second-gift.png`, `free-exit.png`; flag false18867/18988, true21515/23681 |

The Rick decline check followed its accepted replay; Ethan's decline preceded its accepted replay. Both flows are recorded, including the ordinary first-victory path. Payouts are engine awards from actual won battles, without money injections after battle starts. The trainer diagnostic deliberately uses a strong Pokémon and therefore cannot support a balance judgment.

## Diagnostic setup and provenance

`make_runtime_fixture.py` checks both supplied hashes, checks their canonical special-table pairing, compiles genuine event macros against generated map constants, and patches only a disposable ROM's Mom interaction block. Each scene's `fixture.json` preserves source/ELF/fixture hashes, exact patch assembly/bytes, original bytes, target script, and warp destination. No ROM, ELF, save, or executable is included in tracked evidence.

Each session starts with a fresh private save and replays recorded normal title/new-game/house inputs. At frame17041 it is at the diagnostic destination. Reusing those introduction inputs from the earlier service trace reuses no earlier ROM or game state.

- Shop and gift fixtures add10000 to the ordinary initial3000 and warp directly to the relevant NPC. They bypass travel/story progress and have no Pokémon.
- Rick/Ethan fixtures remove all initial money, give one MewtwoLv15, Modest, all six IVs15, and set the Pokémon-acquired system flag before warping. The engine supplies current-level moves (Confusion, Swift, Life Dew, Ancient Power). Battles then use ordinary inputs only. No enemy, win, HP, cash, move or party writes occur during either battle scenario.
- Celadon has one explicit RAM injection at frame18926: the word containing badge flags changes `0x40000000→0x4000000f`, setting the first four badges and preserving other bits. Reopening the same NPC checks the actual gate. This is injected ownership, not four completed gyms.
- Gift setup writes all30 real key-pocket slots at frame17041 with distinct item IDs706..736 excluding714 (Vs. Seeker), each quantity1. This includes key items from the wider engine database and is intentionally artificial inventory, not claimed campaign acquisitions. At frame18988 one slot is cleared. The encrypted quantities, exact addresses and both before/after reads are in the trace. All subsequent delivery and flag changes are normal script execution.
- Save addresses move and encryption keys change when field/battle/shop contexts reload. Checks use the current pointers and corresponding key; the ARM ABI probe uses the engine's `-mabi=apcs-gnu` and its flags table. Verified offsets: money0x490, flags0x1270, encryption key0xAC. ELF `GetMoney`/`FlagGet` disassembly independently agrees.

The separate diagnostic driver is built by `tests/services/build_fixture_driver.py` from the ordinary headless driver. It adds explicit aligned work-RAM writes and labels every affected session `diagnostic-state-injected`. It uses libmGBA0.10.2; exact installed package versions are in `manifest.json`. No desktop, personal save, login, Android emulator or external ROM source is used. All six owned processes exited with `quit`.

## Verification and reproduction

```sh
python3 -m unittest discover -s tests/economy
python3 tests/economy/verify_runtime_evidence.py
python3 tests/services/build_fixture_driver.py
python3 tests/economy/runtime_abi.py --generated-engine ../full-game/engine --output /tmp/economy-abi.json
```

Seven source-contract tests pass. The Light Ball acquisition guard also passes against the integrated source; the isolated economy branch intentionally retains its earlier RED species dependency. The runtime verifier checks evidence SHA256s, command/response alignment, frame counts, source provenance, screenshot dimensions, the exact allowed RAM writes, decrypted purchases/payouts, actual party equality across decline, and gift failure/success/repeat state. **1426 successful commands** across six primary sessions are retained. Visual navigation, displayed text, and lack of sight-line approach remain explicitly inspected trace/screenshot evidence rather than inferred from the numeric verifier.

To reproduce a fixture, pass its recorded hashes/source revision and the frozen ROM/ELF to `make_runtime_fixture.py` with `--scenario celadon|berries|gift|rick|ethan|indigo`, an absent `--output`, and `--generated-engine`. Then replay a complete scenario with:

```sh
python3 tests/economy/run_runtime_fixture.py --rom /path/to/diagnostic.gba --output /path/to/fresh-run --commands tests/economy/evidence/9d479bc5/gift/commands.txt
```

The complete gift trace was replayed once more from a fresh save: every response and all nine PNGs matched byte for byte; see `gift-replay-comparison.json`.

Use the matching scene's trace. Screenshot paths are rewritten to the fresh output directory. Omitting `--commands` runs the normal introduction and accepts manual driver commands. Replays containing RAM writes are tied to the exact frozen artifact and introduction inputs; do not apply their addresses to a later build.

## Tooling corrections and limits

An exploratory Celadon session ended on a malformed read operand (`0x02003?`). Its failure log is retained under `exploratory-invalid-read`; it is excluded from the six passing scenarios. A fresh Celadon session then completed both purchases and the badge check. Before RAM injection, the ABI probe was corrected to match the actual ARM calling/layout ABI; its first compile attempts produced no runtime writes. A few early read-only probes used obsolete/transcribed save addresses, and some planned screenshot checkpoints arrived while text was still printing. These remain in the raw logs but are excluded from asserted transactions. Rick's final decline comparison uses the actual `gParties` address, not the earlier unused save-party copy.

Not established: shops on every mainland map, all pickup rewards, organic progression to each store, the no-money novice's effort to win either practice battle, late-campaign purchasing burden, battle balance, and Android behavior. The separate [economy design record](../balance/campaign-economy.md) identifies tuning and access assumptions. Vs. Seeker's existing explanation promises functionality that remains a known integration dependency in this frozen revision; successful gift delivery does not validate that feature.
