# ADR-0035: Use fixed mainland rematches for repeatable training

- Status: Accepted
- Date: 2026-09-06
- Decision authority: Training, technical and numerical choices delegated by the owner's full-project mandate
- Implementation: Mainland source and four campaign-table native rematch tests pass; ordinary item-use checks remain in progress
- Supersedes: None
- Superseded by: None

## Context

The Vermilion gift exists, but the inherited engine disables charging and uses Hoenn-specific rematch milestones and parties. A delivered item therefore did not establish usable training access. [ADR-0034](0034-preparation-economy-and-equipment-access.md) provides two early recovery battles; later preparation also needs repeatable route opponents without granting instant catch-up or inflating reward rates.

## Decision

Enable the Vs. Seeker in its existing Vermilion gift. Walking 100 steps charges it; supported, previously defeated trainers in view respond deterministically when their actual party fits the player's badge level cap. Readiness lasts 100 further steps or until leaving the map. A scan with no eligible defeated opponent does not consume a charge. Explain badge eligibility when a visible defeated opponent is too strong.

Use a curated table of 67 trainers across 20 mainland route maps, including seven double battles. Repeats use the same trainer identity, party, level, ordinary EXP and prize as their first battle. Do not add automatic badge scaling, special repeat EXP, ordinary NPC bags or fees. First battles retain their existing scripts; rematches consume readiness once through the battle-exit path. A double challenge checks two healthy Pokémon before starting, retaining readiness when that check rejects entry.

The table and its exact source-derived route coverage live in [the validation record](../validation/kanto-rematches.md). Validate current parties when resolving an encounter; a table entry alone cannot bypass roster, map, defeated-state or level-cap checks. Store readiness as a boolean marker and recover the complete trainer ID from the table, avoiding truncation for IDs above 255.

Keep progression-appropriate gym rematches and repeatable League rewards separate. The fixed route parties are ordinary training and earned recovery, not substitutes for those required personal rewards. Continue to measure the complete specialist-preparation burden under [ADR-0006](0006-substantial-levelling-investment.md).

## Alternatives and consequences

Retaining the disabled inherited path leaves an unusable reward. Porting Hoenn's tier progression imports unrelated flags and raises the risk of unexpectedly strong or unreachable opponents. Automatically scaling every repeat to the cap removes predictable lower-risk training options and changes the accepted preparation pacing without evidence. Random willingness adds walking variance without tactical value; deterministic responses make the activity easier to plan.

Fixed teams become less valuable as the campaign advances. Players must choose appropriate route opponents, and late preparation may still prove too slow. Source eligibility and available cash do not establish enjoyable pacing or sufficient late-game challenge. Tune specific encounters only with stage, access and gameplay evidence, preserving ordinary reward rules unless a later consequential decision changes them.

## Evidence and remaining validation

Source checks cover table reachability, trainer scripts, format dispatch and party access. Four focused native tests pass charging, full trainer IDs, map boundaries and readiness expiry. An initial run exposed a test-harness limitation: normal battle tests replace the campaign trainer table with sparse synthetic fixtures. The dedicated `SC_TEST_CAMPAIGN=1` build links the real table in a separate object directory. Its counted runner verifies four rematch passes and one additional real authored-bag audit; neither filter is empty.

Ordinary item acquisition, walking, visible responses, single/double battles, loss/retry, leaving/re-entering a map and saved readiness still need runtime checks. Human preparation timing and the complete campaign journey remain unfinished.
