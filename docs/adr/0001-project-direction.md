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
- Keep player-facing customisability limited, including the later explicitly requested training-focus and learned-passive choices in ADRs 0018–0019.
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
- IV safeguards and improvement access; training numbers, earned focus-change access and rematch access within ADRs 0016 and 0018; ADR-0019 selects extra capacity plus individual passives, with trainer-wide badge scope and selective opponent progression now accepted in ADR-0020; exact badge effects remain open. Random improvable IVs, fixed acquired natures and gym eligibility/rematch rules are selected. ADR-0017 selects a trusted core with rotating specialists.
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

## Clarification — 2026-09-05, individual traits and battle experience

[ADR-0016](0016-individual-traits-and-battle-training.md) accepts random improvable IVs with a usability safeguard, fixed acquired natures, and clear, predictable development rewarding battle use and especially boss experience. Continuity should be rewarded without excessively penalising rotation. At that stage, the original frequent-composition-change target was under review; the resolution is recorded below. The progression mechanism was then proposed, and the levelling target did not settle total preparation time. Later mechanism choices are recorded below.

## Clarification — 2026-09-05, gym eligibility and trusted core

The user selected gym rewards only for Pokémon that entered battle and did not faint, after a player victory, and stronger rematch teams suited to campaign progress. ADR-0016 records these rules while leaving the reward formula open. [ADR-0017](0017-trusted-core-and-boss-mastery.md) supersedes ADR-0005 with a trusted core and rotating specialists, retaining the boss-mastery target.

## Clarification — 2026-09-05, gym budgets and training focus

[ADR-0018](0018-gym-training-budget-and-focus.md) selects limited extra gym-earned training capacity beyond ordinary training, automatic point allocation through a player-selected focus, and separate rewards from all eight leaders for full gym development. Each Pokémon can recover missing rewards through rematches. Numerical limits, immediate versus later filling of unlocked capacity, focus changes and total preparation pacing remain open.

## Clarification — 2026-09-05, earned redistribution and gym reward review

Focus changes unlock later through a limited rare item or a challenge, not money alone, and redistribute points instantly once earned (ADR-0018). Ordinary stat training should usually require additional focused battles after levelling. The user reopened gym reward form by suggesting partial type resistance or a distinct capability; ADR-0019 initially recorded that proposal; its subsequent resolution is recorded below.

## Clarification — 2026-09-05, capacity and individual passives

The user selected both extra gym training capacity and modest learned Pokémon passives with skill-tree or held-item-like character. [ADR-0019](0019-gym-resistances-and-capabilities.md) accepts the user's amended three-slot capacity with capability costs of one, two or three. Capabilities may have benefits and drawbacks and may be earned from gyms, Elite Four and other notable encounters; precise effects remain open; non-gym eligibility is resolved in the later clarification below. This explicitly adds role customisation to the earlier restrained scope. Trainer-wide badge passives were suggested as a possibility at that stage; their later scope decision is recorded below.

## Clarification — 2026-09-05, capability services and eligibility

[ADR-0019](0019-gym-resistances-and-capabilities.md) now fixes free capability reassignment at Pokémon Centers or designated services only, keeps the normal held-item slot separate, and applies field participation, no fainting and encounter victory to all capability rewards, including Elite Four and other notable encounters. Earned later-game focus changes remain a distinct restriction; this does not grant free stat redistribution.

## Clarification — 2026-09-05, fixed capability rewards and badge scope

ADR-0019 now fixes one capability per granting encounter, identical for every qualifying Pokémon. [ADR-0020](0020-badge-passives-and-trainer-progression.md) accepts intended existing badge benefits plus modest combat passives and selective capability use by major bosses and experienced trainers. Slot use must fit experience and story; full loadouts are not the norm, and the player's League accomplishment should matter. The badge reference, exact effects and loadouts remain open.
