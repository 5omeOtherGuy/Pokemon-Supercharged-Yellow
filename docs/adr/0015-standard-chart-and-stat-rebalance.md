# ADR-0015: Keep the standard modern chart and use restrained stat rebalancing

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers on type representation, type effectiveness and base-stat freedom
- Implementation: Not started — balance boundaries only
- Supersedes: None
- Superseded by: None

## Context

[ADR-0012](0012-ability-and-typing-rebalance.md) permits justified species typing and existing-ability changes. [ADR-0013](0013-fairy-type-evaluation.md) includes Fairy, and [ADR-0014](0014-type-identity-and-campaign-viability.md) sets a type identity and viability goal. The interview then asked about mandatory representation, chart changes and base-stat adjustment scope.

## Decision

- Keep the standard modern type-effectiveness chart. Do not introduce custom strengths, weaknesses, resistances or immunities into the chart.
- Impose no type-representation quota. Add or change a Pokémon's typing only when that individual Pokémon's identity and balance justify it. In particular, an otherwise absent type does not itself justify retyping a species.
- Use base-stat redistribution and modest changes to overall strength as the primary approach.
- Permit substantial base-stat changes only in special cases and/or where necessary, with a documented species-specific justification.
- Preserve distinct identity, meaningful weaknesses and differences in strength; do not equalise all species or stat totals.

The representation clarification is also recorded in ADR-0014. Species typings and the effectiveness chart are separate: permission to change the former does not authorise changing the latter. The standard chart does not determine every ability or move interaction, or select all mechanics from a particular generation.

## Alternatives considered

- Require an obtainable member of all modern types: user rejected a representation quota.
- Make limited or broad custom chart changes: user selected the standard modern chart.
- Keep official base stats entirely unchanged: user permits redistribution and changes in strength.
- Use substantial stat changes routinely: user allows them only as exceptions or where necessary.

## Consequences

A rebalance needs to state the original and proposed stat spread, intended role, retained weaknesses and campaign impact. Evaluate stat changes together with move access, abilities, held items, evolution timing and available opponents.

For a substantial change, explain what makes the case exceptional or necessary and why a smaller adjustment would not adequately address the intended problem. This is documentation of an authorised design exception, not a new approval workflow. No particular species, numerical limit or substantial change has been approved by this record.

A small stat change can still alter important matchups; magnitude alone does not establish safety or balance. Assess changes at relevant caps in both singles and doubles.

## Evidence and validation

User answers on 2026-09-05:
- "No representation quota: add typings only when individual Pokémon justify them."
- "Keep the standard modern chart."
- For base stats: "Option 2 as primary, option 3 only in special cases and/or where necessary." Option 2 allowed redistribution and modest changes to overall strength; option 3 allowed substantial changes when identity and balance justify them.

No game data has been changed or balance tested. Once a foundation exists, pin a reference for the standard modern chart and verify its complete matrix, displayed matchups and AI handling. Record any ability/move-specific interaction separately from that matrix. Assess stat changes using meaningful matchup checks and human campaign testing.

## Open questions

- Pinned data revision and generation-specific mechanics beyond the chart.
- Individual stat spreads, typing changes and qualifying exceptional cases.
- Practical numerical guidance for modest versus substantial changes, if a recurring need emerges.
- IVs, EVs, nature handling and player preparation systems.

## Follow-up

Use these boundaries when auditing engine data and curating species changes. Keep baseline data and project overrides distinguishable. Resolve player preparation systems before making detailed assumptions about attainable stats at each cap.
