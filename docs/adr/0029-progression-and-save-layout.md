# ADR-0029: Bound permanent development and version its save data

- Status: Accepted
- Date: 2026-09-06
- Decision authority: Numerical and engineering choices delegated by the owner's execution mandate
- Implementation: Portable core and native boxed-data/save-sector tests pass; battle reward integration underway; balance unvalidated
- Supersedes: None
- Superseded by: None

## Decision

Use these initial tuning values. Acceptance selects a concrete implementation; it does not certify preparation time or balance.

| Badges | Level cap | Per-stat training ceiling | Trainer build budget |
| --- | --- | --- | --- |
| 0 | 15 | 24 | 2 |
| 1 | 22 | 32 | 3 |
| 2 | 28 | 40 | 4 |
| 3 | 36 | 48 | 5 |
| 4 | 44 | 56 | 6 |
| 5 | 50 | 64 | 7 |
| 6 | 56 | 72 | 8 |
| 7 | 62 | 80 | 9 |
| 8 | 68 | 96 | 10 |

Champion status raises the level cap to 75, with the completed badge training ceiling and budget unchanged. Campaign gym gates enforce Brock, Misty, Surge, Erika, Koga, Sabrina, Blaine, Giovanni. Capacity follows owned badges and never increments again from a rematch. Enforce the level cap for EXP and candies; receiving a badge grants room rather than points.

Retain the engine's per-stat EV-to-stat contribution (`floor(training / 4)` in the level-scaled stat formula) but replace normal EV awards with this project's permanent training. There is **no shared total**: six stats can each reach their stage ceiling. Disable vitamins, EV-reducing items and other inherited allocation shortcuts in the campaign. Keep NPC training within a stated encounter-stage ceiling too.

New Pokémon default to balanced focus, distributing future points to the least-trained stat. Six individual stat focuses are also available. Focus changes are free in the field while outside battle and never relocate existing points. If the focused stat is full, further rewards are discarded, including fractional carry; the UI must explain this. Balanced focus discards only when every stat is full.

For each defeated opponent, use Gen-III-style base value `floor(species EXP yield × level / 7)`, multiplied by 1.5 for trainers, then split among that opponent's field participants. Convert each recipient's share to training at **32 × recipient level EXP value per point**, retaining 1/256-point fractions. This conversion supplies diminishing progress as a Pokémon grows. It is independent of level-cap suppression and held-item/ordinary-EXP boosts. Record the share when the opponent faints; commit the accumulated award only after the entire battle is won, and only for participants who never fainted. Switching out retains eligibility; revival cannot restore it. A captured wild Pokémon supplies no defeated-opponent value. Escapes, draws, scripted aborts and team losses give no training or capabilities.

The arithmetic is a starting rate. Measure real levelling and focused preparation for a returning player's core and recruits, including fast text/battle settings and accessible rematches. ADR-0006's substantial levelling requirement and ADR-0023's usual additional focused training remain acceptance tests. Revise rates if those measurements miss the intended experience.

There are twelve individually earned capability records: eight fixed distinct gym rewards and four fixed repeatable Elite Four rewards. All assignments still share three individual capacity points; costs in source are initially `1,1,2,2,1,3,3,2,1,2,3,2`. Duplicate awards add nothing and never grant capacity. Detailed effect and NPC-loadout catalogs must match implementation and scouting before release. Trainer passives use their separate weighted budget above.

## Save allocation and compatibility

Preserve the **80-byte BoxPokemon** and 420 boxed storage slots. Reuse its six contest-condition bytes as two 16-bit capability masks, one focus byte and one fractional-training byte. The existing six EV bytes store permanent points. Expose dedicated `MON_DATA_SC_*` accessors through normal encryption/checksum code; reject legacy contest writes in the Kanto build. Box transfers and evolution must preserve these fields and the acquired nature.

Put a **48-byte trainer record at the start of SaveBlock3**, wholly inside sector zero's 116-byte tail. It holds magic `0x53435931`, schema version 1, record size, unlocked/active trainer-passive masks, practice EXP, three supply-item IDs and quantities, battle speed, a CRC-16/CCITT checksum and reserved space. New games explicitly initialize the record. Do not silently import upstream saves or reinterpret their contest data as capabilities.

Source inspection found that upstream's regular sector checksum does **not** cover the SaveBlock3 tail. Therefore validate the trainer record's independent CRC before accepting sector zero, so a damaged new slot can fall back to a valid older slot. Reject incompatible versions with an explicit message; do not erase the cartridge file. Bounds-check physical sector IDs before indexing sector-location arrays. Seal trainer state as part of the ordinary save transaction; no alternate save-state format is a substitute.

Party/box data remain protected by their existing encryption and sector checksums. Keep reward eligibility transient and finalize once at battle outcome; automatic reset/reload must not produce duplicate rewards. Normal user save/reload semantics still apply, including loss of unsaved progress after closing the emulator.

## Alternatives and consequences

Expanding every boxed Pokémon would exceed the existing storage allocation and force a larger save migration. A separate sparse identity table could collide or lose rewards on transfers. Repurposing unused Kanto contest data preserves the proven storage layout, at the cost of explicitly disabling those old setters and requiring a new-save boundary.

The unrestricted standard 510-EV total conflicts with all-stat potential; conventional per-KO awards conflict with final-outcome eligibility. Both must be replaced in reachable gameplay. The modest per-stat limits bound grinding strength, but only battle and human tests can assess their combined effect with IVs, nature, innate abilities, held items and both passive systems.

## Evidence and follow-up

The host test compiles the same portable core as the GBA. It exercises every badge table row, survivor/fainted/revived/bench/loss cases, one-time finalization, all-stat filling, focus permanence, assignment budgets, duplicate awards, fraction handling, schema rejection and every single-byte corruption position in the trainer record. Latest measured core coverage: 98.26% of 115 executable lines; this is **not game coverage**.

The GBA boxed-data test first failed ARM compilation because the new accessors were absent. The implemented accessor, evolution/nature/storage-size, badge-cap and sector-integrity cases now pass in the actual mGBA native test runner. The real APCS ABI probe confirms sizes 6/12/80/100/15756/34144/52 for personal metadata, substructure 2, boxed Pokémon, party Pokémon, SaveBlock1, storage and SaveBlock3 respectively. These checks do not yet establish an ordinary cartridge save/load journey. Required verification includes actual party/box/evolution saves, corrupted/newer-slot fallback, incompatible-save handling, reward outcomes, cap enforcement and complete gameplay. None is established by the host CRC test alone.
