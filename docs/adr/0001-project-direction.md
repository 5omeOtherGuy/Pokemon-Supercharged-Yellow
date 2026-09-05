# ADR-0001: Project direction and confirmed constraints

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user project brief and scope clarification on 2026-09-05
- Implementation: Not started — requirements recorded only
- Supersedes: None
- Superseded by: None

## Context

The user wants an ultimate Kanto experience: familiar to someone returning after childhood, with significantly greater challenge and depth. Pokémon should retain their identities and meaningful differences in strength. The comparison to professional StarCraft II balancing is a quality aspiration.

Recharged Yellow inspired the concept, but its source is unavailable. The user explicitly clarified that the project does not need it.

## Decision

- Create a Kanto ROM hack inspired by Recharged Yellow.
- Introduce the physical/special split and rebalance Pokémon and encounters.
- Preserve familiar identity and atmosphere; do not target equal strength or universal viability for all Pokémon.
- Begin with one difficulty setting and level caps.
- Keep player-facing customisability limited.
- Include QoL improvements, especially the ability to adjust battle speed.
- Do not make access to Recharged Yellow source a project dependency.

## Alternatives considered

- Depending on Recharged Yellow source: discarded by the user's clarification.
- Multiple initial difficulty modes and extensive rules toggles: inconsistent with the clarified initial scope.
- Equalising every Pokémon's power: explicitly outside the user's balance goal.

## Consequences

Rules, encounter design, available resources and Pokémon changes need to be assessed together. A single initial difficulty narrows validation scope. Battle-speed controls are a required capability, while their precise implementation remains open.

These requirements do not establish a particular engine or approve the mechanics and content suggestions made by the assistant.

## Evidence and validation

Source: project conversation on 2026-09-05. The user requested a physical/special split, identity-preserving rebalance, familiar Kanto and substantially greater challenge/depth. In the follow-up, the user confirmed unavailable Recharged Yellow source, one initial difficulty, level caps, limited customisability and adjustable battle speed.

No implementation or gameplay validation has occurred.

## Open questions

- Original 151 only, later evolutions, or another roster boundary.
- Engine/upstream project and exact mechanics baseline.
- Starting Pokémon and which Yellow-specific events or presentation to reproduce.
- Level-cap values, unlock milestones and enforcement behavior.
- IVs, EVs, nature handling and team-preparation systems.
- Set mode, bag-item restrictions and AI information policy.
- Speed presets, animation/audio behavior and other specific QoL features.
- Initial playable milestone, postgame scope and supported release targets.

## Follow-up

Use these constraints in the foundation audit and later design decisions. Resolve each open question when it affects concrete work; do not require a complete game specification before starting useful engineering.
