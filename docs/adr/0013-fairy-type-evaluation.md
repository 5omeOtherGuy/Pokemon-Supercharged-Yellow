# ADR-0013: Include Fairy to strengthen familiar Pokémon identities

- Status: Accepted
- Date: 2026-09-05
- Decision authority: User's endorsement of Fairy's identity and gameplay value, interpreted in the context of the previously selected modern typing baseline and the proposal to include it
- Implementation: Not started — no engine selected or playable build verified
- Supersedes: None
- Superseded by: None

## Context

[ADR-0012](0012-ability-and-typing-rebalance.md) selects modern official typings as a starting point while permitting justified changes. The user then explicitly asked to discuss Fairy's viability, initially leaving inclusion open. In the follow-up, the user explained that Fairy made familiar Pokémon more interesting and fitted their identities extremely well. Original forms only and the strict original-151 campaign boundary remain accepted in [ADR-0004](0004-original-151-roster.md).

## Decision

Include Fairy in the campaign ruleset. Its primary design rationale is to make familiar Pokémon more interesting through mechanics that fit their identities. Retain the modern official starting assignments: Clefairy/Clefable (Fairy), Jigglypuff/Wigglytuff (Normal/Fairy), and Mr. Mime (Psychic/Fairy), subject to the existing policy for carefully justified changes.

This settles inclusion, not the exact type-effectiveness chart, numerical balance, moves or ability assignments. Increased interest does not require equal power or universal viability. The user described the physical/special split and Fairy as impactful examples of modernization; that is a statement of player experience, not measured balance evidence.

The agent recommends evaluating standard modern matchups and these existing Fairy assignments before adding custom chart changes or additional Fairy retypings. This recommendation remains distinct from the accepted inclusion decision and does not revoke the user's permission for justified changes under ADR-0012.

Once the source foundation is chosen, verify end-to-end support rather than assuming a database type entry is sufficient.

## Alternatives considered

1. Include Fairy: selected direction. Standard interactions remain the agent's recommended starting candidate for the unresolved chart policy.
2. Exclude Fairy: considered during the initial discussion. The user's enthusiasm for Fairy's identity fit and gameplay interest supports inclusion instead.
3. Include Fairy with a custom chart: permits targeted adjustments but increases explanation and testing burden; reserve for a demonstrated problem.

## Consequences and hypotheses

- Giving Poison attacks additional relevant targets could improve the roles of Kanto's Poison species. Actual benefit depends on stage-appropriate moves, opponents and acquisition timing.
- The existing Fairy families offer distinct role-design opportunities without introducing unfamiliar species.
- Kanto's limited Dragon roster and absence of original-form official Dark species weaken the case for Fairy as a general Dragon/Dark corrective. Dark moves can still exist without Dark species.
- Extra pressure on Fighting and Bug attacks needs scrutiny. Evaluate combined typings; the Fairy component alone does not determine every matchup.
- Clefable could become an overly universal choice through the combination of typing, ability, recovery and coverage. Wigglytuff should have its own contribution rather than becoming a weaker copy.
- Fairy does not directly counter Psychic through type effectiveness; Psychic balance still needs separate attention.
- Poison and Steel access must be assessed through moves and viable teams, not just species counts. Many nominal counters may fail against coverage or at a particular cap.

These are design inferences, not playtest findings. The matchup hypotheses assume standard modern interactions; chart policy remains open.

## Evidence and validation

User follow-up on 2026-09-05: the physical/special split and Fairy felt especially impactful; Fairy made Clefable and the Jigglypuff family interesting where they had previously felt boring, and the typing fitted them extremely well. This is recorded as endorsement of inclusion in the existing design context. The user did not specify an exact type chart or approve individual balance changes.

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

- Exact type chart and source-data revision.
- Move access, encounter/evolution timing and ability assignments for affected species.
- Any justified custom type changes after the official starting roster has been evaluated.

## Follow-up

Resolve the exact type-chart policy and curate affected Pokémon around distinct roles, strengths and weaknesses. Evaluate whether the resulting Pokémon feel interesting and recognisable as well as whether encounters are challenging. Use one campaign ruleset; Fairy inclusion does not introduce a player-facing toggle.

## Decision history — 2026-09-05

Initially Proposed to discuss Fairy's viability. Updated to Accepted after the user's follow-up explained its value to familiar Pokémon identities and player interest. Acceptance is limited to inclusion and that rationale. The original standard-chart recommendation and evaluation suggestions remain proposals where not separately decided.
