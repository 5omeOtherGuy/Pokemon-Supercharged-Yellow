# ADR-0022: Keep training gains permanent and let focus direct future growth

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user revision of training focus, permanent allocations, per-stat limits and late-game development expectations
- Implementation: Not started — growth policy chosen; numerical model unresolved
- Supersedes: [ADR-0018](0018-gym-training-budget-and-focus.md)
- Superseded by: None

## Context

ADR-0018 combined gym-earned training capacity with automatic focus allocation and later-game, earned focus changes that instantly redistributed existing points. The user replaces that focus model: focus may change on the fly, existing points never move, each stat has its own training maximum, and one Pokémon can theoretically reach all of those maxima.

## Decision

### Permanent growth and flexible focus

- A training focus determines the stat area receiving future earned training points. Allocate earned points automatically according to that focus.
- Focus can change on the fly. Remove the earlier late-game unlock, rare-item/challenge requirement and instant redistribution rule.
- Changing focus never relocates previously allocated points. Existing training gains remain in their stats; no stat-point reset or redistribution mechanism is selected.
- Every stat has a maximum for additional points earned through training.
- One Pokémon can theoretically reach every stat's training maximum. Do not impose a shared total-point ceiling that makes this impossible.
- Reaching all maxima should be difficult and exceptional. Normal late-game development aims to maximise the stats relevant to a Pokémon's role, rather than every stat.
- Exact per-stat limits, growth rates, focus options and point-to-stat conversion remain open. Do not assume equal caps, a particular main-series EV formula or a specific diminishing-return curve.
- On-the-fly focus changes concern future training; they do not change battle stats instantly. Exact UI timing, including whether focus can be changed during a battle, is an implementation detail still to resolve.

### Retained gym development and pacing

- Retain limited extra gym-earned training capacity alongside the individual capabilities in [ADR-0019](0019-gym-resistances-and-capabilities.md). The user revised focus and allocation, not the earlier decision to keep both rewards.
- All eight gym leaders contribute separately to each eligible Pokémon's full gym-related development.
- A qualifying Pokémon entered battle, never fainted during it, and belonged to the winning team, as in [ADR-0016](0016-individual-traits-and-battle-training.md).
- Each leader's contribution counts once. Repeated wins against one leader cannot replace another's contribution or grant unlimited increments.
- Progression-appropriate rematches let recruits earn the same gym contributions as veterans.
- Gym-earned training capacity raises that Pokémon's training ceiling to the next gym stage, analogous to campaign level-cap progression. This unlocks capacity for further training, not an automatic award of filled stat points. Exact per-stat ceilings at each stage and how missed or out-of-order leader rewards map to stages remain open. Do not replace this with faster training or an incompatible shared ceiling.
- Ordinary stat training should usually require additional focused battles after reaching the level cap. The additional duration remains open.
- Keep the substantial levelling target and trusted-core-with-rotating-specialists direction. Neither implies every team member needs full training in every stat.

IV improvement remains separate and acquired nature effects remain fixed. Species base stats, the standard chart, three-slot capability capacity and service-only capability reassignment are unchanged.

## Alternatives considered

- Earn later-game focus changes and instantly redistribute previous points: explicitly replaced.
- A shared training ceiling that forces permanently exclusive stat specialisations: incompatible with eventually maximising every stat.
- Easy or routine maximum training in every stat: inconsistent with the desired progression.
- Remove gym training capacity when changing focus rules: not requested; retain it pending a compatible numerical model.

## Consequences

Focus expresses what a Pokémon trains next, while its previous development persists. Specialisation is driven by preparation time and campaign progress rather than a permanent exclusive allocation.

A fully trained Pokémon can accumulate more broad strength than a role-focused recruit. Encounter balance must preserve value for recruits and ordinary late-game builds without assuming universal maximum training. The system must remain clear and avoid making difficult completion synonymous with tedious repetition.

Training and assigned passive builds now have distinct costs: training adds permanent development through battles, while assigning earned Pokémon capabilities remains free at the designated services. Trainer builds follow [ADR-0021](0021-trainer-builds-and-reward-ownership.md).

## Evidence and validation

The user explicitly selected freely changing focus for future gains, no relocation of earned stat points, independent per-stat maxima, theoretical completion of every stat, and relevant-stat completion as the normal late-game target on 2026-09-05.

No engine, numerical tuning, implementation or playtest evidence exists. Future validation should check:
- Focus changes preserve all previously earned allocations.
- New gains follow the selected focus and respect each stat's cap.
- Every stat maximum remains reachable on the same Pokémon.
- Duplicate gym rewards cannot create unlimited capacity.
- Veterans and recruits can ultimately reach the same potential.
- Relevant-stat preparation fits campaign pacing and full-stat completion is exceptional.
- Both singles and doubles remain challenging for plausible role-focused teams.

## Open questions

- Per-stat training ceilings for each gym stage, including the initial and post-eighth-gym stages; how missed or out-of-order rewards affect stage access.
- Numerical stat caps, conversion, focus options and gain rates.
- Ordinary training credit: participation, fainting, victory and opponent eligibility.
- Gain behavior when the focused stat is already at its cap.
- Ordinary versus exceptional training time; encounter assumptions about player and opponent development.
- Rematch access and persistence through evolution, storage and other identity-preserving transitions.

## Follow-up

Specify numerical per-stat ceilings by gym stage and resolve missed-reward handling, then draft a numerical progression model. Test typical role-focused development and the exceptional all-stat endpoint separately.

## Clarification — 2026-09-05, gym-stage ceilings

Before this record was committed, the user clarified that gym-earned capacity raises the maximum to the next gym stage, analogous to the Pokémon level cap. This selects staged ceiling unlocks rather than an unspecified extra pool. The existing individual eligibility, eight distinct contributions and rematch catch-up rules remain; the exact interaction between missing earlier contributions and later stages still needs a decision.
