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

- Postgame roster, main-campaign species availability and other battle mechanics; original forms only, no regional variants or Mega Evolution, as resolved in ADR-0004.
- Training/scouting implementation and retry logistics beyond the policies accepted in ADRs 0006–0008.
- Engine/upstream project and exact mechanics baseline.
- Starting Pokémon and which Yellow-specific events or presentation to reproduce.
- Level-cap values, unlock milestones and enforcement behavior.
- IVs, EVs, nature handling and team-preparation systems.
- Numerical category limits (delegated to testing), remaining consumable scope/action rules and detailed AI information mapping beyond ADR-0009.
- Specific singles/doubles boss assignments and team sizes within ADR-0010; individual move choices and behavior within ADR-0011.
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

## Clarification — 2026-09-05, design interview rounds 5–6

Round 5 resolved category-limit tuning, exclusion of mixed-category battle supplies and mutual consumable-loadout visibility in ADRs 0008–0009. Round 6 accepts a substantial mix of singles and doubles with major bosses in both, Set rules throughout trainer battles ([ADR-0010](0010-mixed-battles-and-set-rules.md)), and carefully selected moves from any generation ([ADR-0011](0011-curated-moves-across-generations.md)). These do not select a mechanics generation, exact boss formats, abilities, typings or alternate forms. Implementation remains unstarted.

## Clarification — 2026-09-05, design interview round 7

[ADR-0012](0012-ability-and-typing-rebalance.md) permits reassignment of existing official abilities and carefully justified typing changes starting from modern official typings. [ADR-0004](0004-original-151-roster.md) now requires original forms only, excluding regional variants and Mega Evolutions from the main campaign. These are design boundaries; specific changes and implementation remain pending.

## Clarification — 2026-09-05, Fairy discussion

The user explicitly requested discussion of Fairy's viability after round 7. At that point, the modern-typing starting point did not settle Fairy inclusion; ADR-0013 was recorded as Proposed. Existing ability-reassignment and original-form decisions remained accepted. The subsequent resolution is recorded below.

## Clarification — 2026-09-05, Fairy identity rationale

The user explained that Fairy made previously uninteresting familiar Pokémon feel interesting and that the typing fits them extremely well. In the context of the selected modern typing baseline and the recommendation to include Fairy, this is recorded as endorsement of inclusion in [ADR-0013](0013-fairy-type-evaluation.md). The design purpose is recognisable identity expressed through interesting battle roles. At that point, exact type-chart policy and individual balance changes remained open; no implementation or balance validation was implied. The later chart decision is recorded below.

## Clarification — 2026-09-05, type identity and campaign viability

The user clarified that each type should feel distinct and playable, without requiring every Pokémon to be viable. [ADR-0014](0014-type-identity-and-campaign-viability.md) records this principle, longer Bug usefulness, concerns about late-game Flying options such as Pidgeot, and the user's named Pokémon interests. The interest list is not evidence of weakness or an automatic buff list. At that point, exact type representation, type chart and base-stat-change boundaries remained open. The resolution is recorded below.

## Clarification — 2026-09-05, chart, representation and stat boundaries

The user selected no type-representation quota and the standard modern effectiveness chart. Base-stat redistribution and modest changes are the primary approach, with substantial changes permitted only in special cases and/or where necessary. See [ADR-0015](0015-standard-chart-and-stat-rebalance.md) and the representation clarification in ADR-0014. Specific adjustments and implementation remain pending.
