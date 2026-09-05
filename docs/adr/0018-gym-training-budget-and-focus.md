# ADR-0018: Unlock gym training budgets and allocate development through a focus

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers on gym training potential, allocation and distinct-leader rewards
- Implementation: Not started — mechanism boundaries chosen; numbers and progression details open
- Supersedes: None
- Superseded by: None

## Context

[ADR-0016](0016-individual-traits-and-battle-training.md) establishes battle-earned development, acquired traits and gym reward eligibility. [ADR-0017](0017-trusted-core-and-boss-mastery.md) targets a trusted core with rotating specialists. The interview asked whether gym rewards extend ordinary training potential, how points are allocated and how many distinct gym leaders contribute.

## Decision

- Gym victories unlock a limited extra training budget beyond the maximum available through ordinary training.
- The player selects a training focus and earned points are allocated automatically according to it.
- All eight gym leaders contribute separate rewards to each eligible individual Pokémon. Full gym-related development requires earning all eight.
- Apply the participation, survival and player-victory conditions from ADR-0016. The initial encounter or a progression-appropriate rematch can earn that leader's reward.
- Each leader's contribution to a Pokémon's extra budget is counted once. Repeating one leader cannot substitute for a missing leader or increase that same contribution indefinitely.
- Late recruits can earn the same set of gym contributions through rematches; joining later does not inherently reduce this potential.

This selects additional training capacity, not merely faster ordinary training. It does not yet decide whether a gym reward immediately fills the unlocked capacity, partially fills it or requires further battle training. It also does not choose focus definitions, numerical stat effects, per-stat limits or reallocation rules.

IV improvement remains separate from this earned-training budget, and acquired natures remain fixed. Neither the species base-stat policy nor the standard type chart changes.

## Allocation model

At a policy level, a Pokémon's available training capacity consists of its ordinary training limit plus the limited contributions unlocked from individual gyms. The selected focus directs allocation of earned points. Capacity, earned points and their allocation must remain distinguishable in implementation and explanations.

The exact conversion of points to stats is open. A focus is a player-facing preference, not permission for hidden random allocation or unrestricted manual distribution. Whether a focus can be changed and what happens to existing allocations must be decided separately.

## Alternatives considered

- Boss victories only accelerate progress toward the ordinary maximum: not selected.
- Direct manual allocation of each earned point: not selected.
- Automatic species-specific allocation without a player-selected focus: not selected.
- Fewer distinct gym leaders suffice, or repeat any leader to fill a shared track: not selected.

## Consequences

Veterans can develop further than otherwise comparable same-level Pokémon that have completed only ordinary training. A recruit can match the gym-earned component after earning the corresponding rewards and completing any required training.

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
- Immediate points versus unlocked capacity requiring additional training.
- Focus choices, allocation proportions, focus-changing and redistribution.
- Ordinary training eligibility, rates and overlap with levelling.
- Rematch access timing, progression tiers and exact teams.
- Trainer training budgets and encounter assumptions about player training completeness.
- Evolution, storage and other identity-preserving transitions for recorded gym rewards.

## Follow-up

Resolve reward filling, focus changes and training overlap. Then draft a small numerical model and prototype plan using these accepted boundaries, without treating untested numbers as balanced.
