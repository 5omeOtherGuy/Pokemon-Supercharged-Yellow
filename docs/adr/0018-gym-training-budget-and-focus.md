# ADR-0018: Unlock gym training budgets and allocate development through a focus

- Status: Superseded
- Date: 2026-09-05
- Decision authority: Explicit user answers on gym training potential, allocation and distinct-leader rewards
- Implementation: Not started — mechanism boundaries chosen; numbers and progression details open
- Supersedes: None
- Superseded by: [ADR-0022](0022-permanent-training-and-flexible-focus.md)

## Context

[ADR-0016](0016-individual-traits-and-battle-training.md) establishes battle-earned development, acquired traits and gym reward eligibility. [ADR-0017](0017-trusted-core-and-boss-mastery.md) targets a trusted core with rotating specialists. The interview asked whether gym rewards extend ordinary training potential, how points are allocated and how many distinct gym leaders contribute.

## Decision

- Gym victories unlock a limited extra training budget beyond ordinary training. ADR-0019 confirms retaining this capacity alongside earned Pokémon passives.
- The player selects a training focus and earned points are allocated automatically according to it.
- All eight gym leaders contribute separate rewards to each eligible individual Pokémon. Full gym-related development requires earning all eight.
- Apply the participation, survival and player-victory conditions from ADR-0016. The initial encounter or a progression-appropriate rematch can earn that leader's reward.
- Each leader's contribution to a Pokémon's extra budget is counted once. Repeating one leader cannot substitute for a missing leader or increase that same contribution indefinitely.
- Late recruits can earn the same set of gym contributions through rematches; joining later does not inherently reduce this potential.

The reward choice selects additional capacity rather than faster ordinary training. ADR-0019 resolves the subsequent review by retaining it alongside modest Pokémon-earned passive effects. Focus definitions, numerical stat effects and per-stat limits remain open; earned focus-changing rules are selected below.

IV improvement remains separate from this earned-training budget, and acquired natures remain fixed. Neither the species base-stat policy nor the standard type chart changes.

## Earned focus changes and ordinary training

- Changing an established focus is available only later in the game.
- It must be earned through a limited rare item or a challenge. The exact route, scarcity and repeatability remain to be selected; money alone is insufficient.
- Once a focus change is earned and applied, redistribution of existing focus points happens instantly. Do not add retraining time to that redistribution.
- This is not an unrestricted menu reset. The initial focus-selection rules remain separate and unresolved.
- Ordinary stat training should usually require additional focused battles after reaching the level cap. The amount of extra training remains open.

## Allocation model

The training-capacity model combines the ordinary training limit with gym contributions; ADR-0019 confirms keeping this model alongside passive rewards. The selected focus directs allocation of earned points. Capacity, earned points and their allocation must remain distinguishable in implementation and explanations.

The exact conversion of points to stats is open. A focus is a player-facing preference, not permission for hidden random allocation or unrestricted manual distribution. Established focuses can be changed later through the earned access rule above, with instant redistribution; exact item/challenge access remains open.

## Alternatives considered

- Boss victories only accelerate progress toward the ordinary maximum: not selected.
- Direct manual allocation of each earned point: not selected.
- Automatic species-specific allocation without a player-selected focus: not selected.
- Fewer distinct gym leaders suffice, or repeat any leader to fill a shared track: not selected.

## Consequences

Gym capacity gives veterans greater training potential than otherwise comparable same-level Pokémon with only ordinary training. ADR-0019 also adds earned Pokémon passives; both benefits remain recoverable through the appropriate gym rewards.

Full development entails eight distinct eligible gym victories per Pokémon. This is a completion requirement for the full bonus, not a decision that every Pokémon must have all bonuses to contribute or face the League. Encounter assumptions about training completeness remain open.

Provide access to each leader's catch-up reward despite later story events, through an implementation still to be chosen. Track earned and missing gym contributions per Pokémon; show reward conditions and the effects of the selected focus clearly.

Measure the combined burden of levelling, IV improvement, training and eight gym rewards. The 20–30-minute levelling target is unchanged and does not establish a separate grind of that length for every system.

## Evidence and validation

User answers on 2026-09-05:
- "Yes: gym victories unlock a limited extra training budget."
- "Choose a training focus; points are allocated automatically."
- "All eight: each leader contributes a separate reward."

No game data, implementation or playtests exist. Future checks should cover:
- Eligible original wins and rematches granting the same leader contribution.
- Fainted participants and unused reserves receiving no gym reward.
- Duplicate wins never replacing missing distinct-leader contributions.
- Eight earned contributions reaching the defined full gym budget.
- Focus allocation respecting total and per-stat limits with predictable results.
- Otherwise comparable veterans and recruits reaching the same gym-earned potential.
- Access to all catch-up rewards and total preparation time at relevant campaign stages.

## Open questions

- Ordinary and additional gym budget sizes, whether all leader contributions have equal size, and point-to-stat conversion.
- Immediate points versus unlocked capacity requiring additional training; the gym budget is retained alongside passive effects.
- Focus choices and allocation proportions; the later-game limited-item or challenge route for instant redistribution.
- Ordinary training eligibility, rates and additional post-levelling duration.
- Rematch access timing, progression tiers and exact teams.
- Trainer training budgets and encounter assumptions about player training completeness.
- Evolution, storage and other identity-preserving transitions for recorded gym rewards.

## Follow-up

Resolve budget filling, passive details in ADR-0019, the focus-change access route and training duration. Then draft a numerical model and prototype plan using the settled boundaries; do not treat untested numbers as balanced.

## Clarification — 2026-09-05, earned focus changes and reopened gym rewards

The user selected instant redistribution after earning a focus change later in the game, through a limited rare item or a challenge rather than money alone. Ordinary training should usually require focused battles after levelling. The same reply suggested partial type resistance or a unique capability as the gym reward; ADR-0019 initially recorded that unresolved alternative; the resolution is recorded below.

## Clarification — 2026-09-05, retain capacity alongside passives

The user selected both extra gym training capacity and modest individual passive effects. [ADR-0019](0019-gym-resistances-and-capabilities.md) now accepts that supplemental system with three capacity slots per Pokémon and capabilities costing one, two or three slots. This resolves the budget review without replacing this record. Immediate versus later filling of unlocked capacity remains open.

## Supersession — 2026-09-05

[ADR-0022](0022-permanent-training-and-flexible-focus.md) is the current policy. This record preserves the earlier decision and rationale; its superseded restrictions are not current requirements.
