# ADR-0013: Evaluate Fairy for the original-151 campaign

- Status: Proposed
- Date: 2026-09-05
- Decision authority: User requested discussion; recommendation below is the agent's, not an accepted requirement
- Implementation: Not started — no engine selected or playable build verified
- Supersedes: None
- Superseded by: None

## Context

[ADR-0012](0012-ability-and-typing-rebalance.md) selects modern official typings as a starting point while permitting justified changes. The user then explicitly asked to discuss Fairy's viability. Inclusion remains open. Original forms only and the strict original-151 campaign boundary remain accepted in [ADR-0004](0004-original-151-roster.md).

## Proposal

Evaluate inclusion of Fairy with standard modern matchups and its existing original-form Kanto assignments first: Clefairy/Clefable (Fairy), Jigglypuff/Wigglytuff (Normal/Fairy), and Mr. Mime (Psychic/Fairy).

Do not add further Fairy retypings or change the type chart just to distribute types evenly. This is a proposed initial evaluation boundary, not a reversal of the user's permission to justify custom typings later.

Fairy is a plausible design choice, not a demonstrated balance improvement. Once the source foundation is chosen, verify end-to-end support rather than assuming a database type entry is sufficient.

## Alternatives considered

1. Include Fairy with standard interactions: agent's recommended starting candidate.
2. Exclude Fairy: reduces changes to childhood type expectations but requires explicit replacement typings and a review of curated moves and abilities.
3. Include Fairy with a custom chart: permits targeted adjustments but increases explanation and testing burden; reserve for a demonstrated problem.

## Consequences and hypotheses

- Giving Poison attacks additional relevant targets could improve the roles of Kanto's Poison species. Actual benefit depends on stage-appropriate moves, opponents and acquisition timing.
- The existing Fairy families offer distinct role-design opportunities without introducing unfamiliar species.
- Kanto's limited Dragon roster and absence of original-form official Dark species weaken the case for Fairy as a general Dragon/Dark corrective. Dark moves can still exist without Dark species.
- Extra pressure on Fighting and Bug attacks needs scrutiny. Evaluate combined typings; the Fairy component alone does not determine every matchup.
- Clefable could become an overly universal choice through the combination of typing, ability, recovery and coverage. Wigglytuff should have its own contribution rather than becoming a weaker copy.
- Fairy does not directly counter Psychic through type effectiveness; Psychic balance still needs separate attention.
- Poison and Steel access must be assessed through moves and viable teams, not just species counts. Many nominal counters may fail against coverage or at a particular cap.

These are design inferences, not playtest findings.

## Evidence and validation

Preliminary, unpinned external references consulted on 2026-09-05:
- [Fairy roster and standard interactions](https://pokemondb.net/type/fairy).
- [Dragon roster](https://pokemondb.net/type/dragon) and [Dark roster](https://pokemondb.net/type/dark).
- Official [Magnemite](https://www.pokemon.com/us/pokedex/magnemite) and [Magneton](https://www.pokemon.com/us/pokedex/magneton) entries identify their Electric/Steel typing.

Use pinned engine data for implementation validation. No runtime or battle-balance results exist.

Proposed evaluation:
- Test early access, midgame progression and League encounters at their intended caps.
- Compare plausible teams with and without a Fairy against both singles and doubles bosses; include ordinary returning-player choices.
- Test whether Poison choices gain a useful purpose, Fighting/Bug choices retain meaningful contributions, and Dragon encounters retain multiple solutions.
- Check Fairy abilities, recovery, coverage and doubles support together rather than testing typing in isolation.
- Verify damage interactions, immunity messages, move categories, displayed types and AI evaluation.
- Test whether players can learn the added matchups naturally, for example through an optional Mt. Moon encounter or explanation. This location is a proposal, not an accepted story change.

## Open questions

- Is the user's main concern nostalgia, small-roster balance, particular matchups or implementation effort?
- Include Fairy, exclude it, or compare candidates before deciding?
- Exact type chart and source-data revision.
- Move access, encounter/evolution timing and ability assignments for affected species.
- Any justified custom type changes after the official starting roster has been evaluated.

## Follow-up

Discuss the tradeoffs with the user before accepting or rejecting this proposal. Resolve any changed baseline explicitly in ADR-0012 and the project summaries. Choose one campaign ruleset; this proposal does not add a player-facing toggle.
