# ADR-0012: Reassign existing abilities and permit justified typing changes

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers in design interview round 7
- Implementation: Not started — rebalance boundaries only
- Supersedes: None
- Superseded by: None

## Context

The user permits curated cross-generation moves while retaining the original 151. The interview asked how freely abilities and typings may change.

## Decision

- Existing official abilities may be reassigned to species that have not officially had them when identity and balance justify the change.
- Newly invented innate abilities are outside this selected approach. The later, separately authorised gym-earned passive system is scoped in ADR-0019.
- Start from modern official Pokémon typings and permit carefully justified custom typing changes.
- Treat these permissions as design tools, not a requirement to alter every Pokémon.

The exact baseline data revision, specific assignments and versions of ability behavior remain to be selected. [ADR-0013](0013-fairy-type-evaluation.md) includes Fairy; [ADR-0015](0015-standard-chart-and-stat-rebalance.md) fixes the standard modern chart and no representation quota. Species typing permission does not authorise changing that chart.

## Alternatives considered

For abilities: limiting each species to its own historical official abilities, or additionally inventing custom abilities. The user selected reassignment of existing abilities.

For typings: preserving Generation 1 typings, or using modern official typings without custom changes. The user selected modern official typings with justified custom changes.

## Consequences

Document the species identity, specific balance problem, intended role, retained weaknesses and campaign impact of each departure. Assess both singles and doubles, including partner interactions and combinations with curated moves.

Selecting a strong ability or adding a type must not substitute for evaluating the whole Pokémon at its acquisition and evolution stages. Changes need clear in-game descriptions and accurate source-backed data.

This does not expand the roster or permit regional variants or Mega Evolutions. See [ADR-0004](0004-original-151-roster.md) for the clarified form boundary.

## Evidence and validation

User answers on 2026-09-05:
- "Allow reassignment of existing abilities when identity and balance justify it."
- "Start from modern official typings and allow carefully justified custom changes."

No assignments, type changes or game mechanics have been implemented. Future checks should verify implemented effects, type interactions, displayed data, AI understanding, obtainable ability slots and balance in both formats.

## Open questions

- Pinned official typing/ability baseline and generation-specific ability behavior.
- Specific reassignments and typing changes.
- Ability slots, hidden abilities and player ability-selection methods.
- Whether any existing ability effects may be altered.
- Pinned data revision for the standard modern chart selected in ADR-0015, and other battle mechanics.

## Follow-up

Audit candidate engines for required existing abilities and types. Curate individual changes alongside moves, stats, evolution timing and encounters. Keep original forms recognisable.

## Decision history — 2026-09-05

Fairy was initially left open for explicit discussion despite the modern-typing baseline; ADR-0013 subsequently recorded the user's identity-based endorsement. ADR-0015 later settled the standard chart, representation and stat-change boundaries.

ADR-0019 separately authorised learned Pokémon passives. ADR-0020 initially added trainer-wide badge scope, then [ADR-0021](0021-trainer-builds-and-reward-ownership.md) superseded it with trainer builds and reward ownership. Those systems do not change the existing-official-ability policy for innate assignments.

Editorial audit: consolidated these follow-ups into canonical links; no ability/typing decision changed.
