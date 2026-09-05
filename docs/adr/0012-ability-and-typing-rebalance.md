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

The exact baseline data revision, specific assignments and versions of ability behavior remain to be selected. Pokémon typing changes are distinct from changing the type-effectiveness chart.

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

## Clarification — 2026-09-05, Fairy discussion

Before this record was committed, the user explicitly requested discussion of Fairy's viability. Fairy inclusion was therefore left unresolved, despite the modern official typing starting point. ADR-0013 initially recorded an agent recommendation for discussion. Other boundaries in this record remained accepted. The subsequent resolution is recorded below.

## Clarification — 2026-09-05, Fairy inclusion

The user's follow-up endorses Fairy for its fit with familiar Pokémon and the interest it adds to their battle identities; [ADR-0013](0013-fairy-type-evaluation.md) now accepts inclusion. The existing official Fairy assignments are part of the modern-typing starting point. At that stage, exact chart policy, individual adjustments and any additional justified retypings remained separate choices; the later chart decision follows.

## Clarification — 2026-09-05, fixed chart and species-specific changes

[ADR-0015](0015-standard-chart-and-stat-rebalance.md) selects the standard modern effectiveness chart and restrained base-stat rebalancing with justified exceptions. Typing changes remain permitted on individual identity and balance grounds, without a type-representation quota. Permission to change species typings does not permit custom chart changes.

## Clarification — 2026-09-05, separate learned passives

[ADR-0019](0019-gym-resistances-and-capabilities.md) explicitly permits designing Pokémon-earned passive capabilities in addition to gym training capacity. This extends the project's development systems without replacing the existing-official-ability policy for innate assignments. Exact effects and interactions remain open; ADR-0020 subsequently accepts trainer-wide badge scope, with exact effects still open.
