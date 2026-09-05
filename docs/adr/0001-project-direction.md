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

- Forms/transformations, postgame roster and main-campaign species availability (species eligibility resolved in ADR-0004).
- Training/scouting implementation and retry logistics beyond the policies accepted in ADRs 0006–0008.
- Engine/upstream project and exact mechanics baseline.
- Starting Pokémon and which Yellow-specific events or presentation to reproduce.
- Level-cap values, unlock milestones and enforcement behavior.
- IVs, EVs, nature handling and team-preparation systems.
- Set mode, numerical category limits, mixed-effect consumables, battle scope and detailed AI information mapping beyond ADR-0009.
- Speed presets, animation/audio behavior and other specific QoL features.
- Initial playable milestone, postgame scope and supported release targets.

## Follow-up

Use these constraints in the foundation audit and later design decisions. Resolve each open question when it affects concrete work; do not require a complete game specification before starting useful engineering.

## Clarification — 2026-09-05, design interview round 1

The main-campaign species boundary is now the original 151: [ADR-0004](0004-original-151-roster.md). Frequent team rebuilding and repeated boss attempts are accepted experience targets: [ADR-0005](0005-team-rotation-and-boss-mastery.md). These clarify previously open scope and challenge questions; the original requirements above remain in effect. Implementation has not started.

## Clarification — 2026-09-05, design interview round 2

The user selected substantial levelling investment ([ADR-0006](0006-substantial-levelling-investment.md)), full boss scouting before the first attempt ([ADR-0007](0007-full-boss-scouting.md)), and money/item loss costs with preselected limited battle consumables ([ADR-0008](0008-consumable-loadouts-and-loss-costs.md)). Three consumables remains an example. These refine the existing direction; implementation has not started.

## Clarification — 2026-09-05, design interview round 3

[ADR-0008](0008-consumable-loadouts-and-loss-costs.md) now fixes the allowance policy across the campaign, delegates the number to balance testing, lists eligible player categories (excluding revives), and permits only opposing major bosses to use bag consumables with the same maximum as the player. These refine previously open item rules; implementation remains unstarted.

## Clarification — 2026-09-05, design interview round 4

The user chose category-based consumable duplicate limits, full boss bag-loadout visibility, and official VGC tournament information for boss AI. See [ADR-0008](0008-consumable-loadouts-and-loss-costs.md), [ADR-0007](0007-full-boss-scouting.md) and [ADR-0009](0009-vgc-style-ai-information.md). Exact category caps and campaign-specific information extensions remain open; these choices do not select the battle format.
