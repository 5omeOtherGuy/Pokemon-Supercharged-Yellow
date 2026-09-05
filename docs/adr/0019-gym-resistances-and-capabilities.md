# ADR-0019: Combine gym training capacity with earned Pokémon capability slots

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user choices on combined rewards, three-slot capacity, expanded sources, service reassignment, separate held items, eligibility and fixed encounter rewards
- Implementation: Not started — capability policy only
- Supersedes: None
- Superseded by: None

## Context

The user considered partial resistance or a capability instead of extra gym training capacity, then selected both capacity and modest learned passive effects. Skill-tree passives and held items were design references. Before commitment, the user replaced an initial one-or-two-active-effect idea with three capacity slots, variable costs, optional drawbacks and sources beyond gyms.

## Decision

### Capacity and assignments

- Add modest individually earned passive capabilities that support Pokémon roles and deeper roleplaying. Retain the separate trainer-wide badge-unlocked training ceilings in [ADR-0023](0023-badge-wide-training-ceilings.md).
- Each Pokémon has three capability slots. Each assigned capability costs one, two or three; assigned costs must total at most three. Full combinations are 1+1+1, 2+1 or 3. Partial use is allowed; later slot unlocks are not selected.
- Capabilities may have benefits and drawbacks. A drawback does not add capacity or create a negative cost.
- Keep the normal held-item slot in addition to capability capacity.
- Reassign already earned capabilities freely, without resource cost, only at a Pokémon Center or designated service. Unrestricted reassignment elsewhere or during battle is excluded. Unassigning does not remove the earned capability or grant an unearned one.

### Fixed rewards and eligibility

- Each granting encounter awards one fixed capability, identical for all qualifying Pokémon. There is no species-specific variant or branching reward choice at that encounter. Customisation comes from assigning the earned collection within capacity.
- Each of the eight gym leaders contributes a distinct personal reward. Progression-appropriate rematches let recruits earn the same rewards as veterans. Repeating one leader cannot replace another leader's reward.
- Gyms, Elite Four battles and other notable encounters can grant capabilities only where repeat access lets later recruits earn them. [ADR-0021](0021-trainer-builds-and-reward-ownership.md) owns the source restriction: non-repeatable story encounters may grant trainer passives, never Pokémon capabilities.
- At every capability source, the player's team must win, and a qualifying Pokémon must have entered the field and never fainted during that encounter. Switched-out survivors and surviving support participants qualify; they need not score a knockout or be active on the final turn.
- Unused reserves and fainted participants do not qualify. Healing or revival cannot undo fainting for that encounter's eligibility; another eligible teammate can still receive its reward.

All eight personal gym rewards define the full gym-capability collection, not training-ceiling access or every capability in the game. More earned capabilities expand choices, not simultaneous capacity. Expanded sources do not automatically add training-ceiling increments beyond badge progression.

## Scope and related systems

[ADR-0012](0012-ability-and-typing-rebalance.md) still restricts innate assignments to existing official abilities. This separately authorised custom passive system does not permit inventing innate abilities. No particular effect is selected; partial type resistance was not chosen as the primary reward model, and [ADR-0015](0015-standard-chart-and-stat-rebalance.md) retains the standard chart.

The restrained-customisation goal explicitly includes this development choice, without approving unrelated toggles, multiple difficulties or an unrestricted skill tree. Stat-training focus changes direct future permanent gains under ADR-0023; capability assignment follows the service-only rule above.

[ADR-0021](0021-trainer-builds-and-reward-ownership.md) owns trainer-passive budgets, rewards and selective opponent progression. Opponent Pokémon use the same three-slot capability limit, with partial loadouts appropriate to experience and story. Trainer-owned effects and badge ceilings do not grant a recruit personally earned capabilities.

## Alternatives considered

Replacing training capacity with effects, primarily small type-damage reductions, all earned effects active simultaneously, and a fixed one-or-two-effect count were not selected. Reward choice/species-specific variants were declined in favour of a fixed encounter reward. Trainer-wide passives were initially only a possibility; ADR-0021 now governs their accepted scope.

## Consequences

Assess the complete combination of species stats, IVs, fixed nature, permanent training, innate ability, held item, assigned capabilities and trainer effects. Evaluate interaction strength and role identity in both singles and doubles. A nominal drawback or high slot cost does not prove that a strong combination is balanced.

Service-only reassignment can add preparation travel and matters during sequences such as the Elite Four. Repeatable sources must remain accessible after story changes. Neither full collections nor full active capacity are mandatory for every useful Pokémon.

[ADR-0024](0024-passive-scouting-and-ai-observations.md), subsequently accepted by the user, makes assigned capabilities public to both sides of boss battles while protecting unassigned collections and the existing hidden-stat boundary.

## Evidence and validation

Recorded user answers on 2026-09-05 selected both training capacity and passive effects, then amended active capacity to three slots with costs of one, two or three, possible drawbacks and expanded sources. Later answers selected free service-only reassignment, a separate held item, universal participation/no-fainting/victory eligibility and one fixed reward per encounter.

No implementation or balance results exist. Future checks should cover all slot combinations and rejection of excess costs; original/rematch reward equivalence; support, bench, fainting and victory outcomes; duplicate handling; earned-collection persistence when unassigned; service boundaries; and predictable effects/UI/AI in both formats. Compare useful veteran and recruit builds without requiring every reward.

## Open questions

- Specific fixed capability rewards, costs within 1–3, benefits, drawbacks and stacking with other effects.
- Duplicate-award handling and repeat-access arrangements for each source, including Elite Four and story changes.
- Designated services and access timing, including reassignment during League sequences.
- Scouting presentation and commitment/refresh timing under ADR-0024's accepted active-effect disclosure. Trainer-passive effects and budgets belong in ADR-0021.

## Follow-up

Draft representative one-, two- and three-slot fixed rewards and compare combinations with held items, trainer builds and training. Confirm each reward source is repeatable before assigning it.

## Decision history — 2026-09-05

- Proposed in [863a7e7](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/863a7e7); accepted with the amended three-slot model in [6a80607](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/6a80607). The earlier one-or-two-effect count was never the committed final rule.
- [0da1ab4](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/0da1ab4) added service reassignment, held-item coexistence and shared eligibility; [300173d](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/300173d) added fixed rewards and ADR-0020's initial opponent/badge scope.
- ADR-0021 superseded that trainer scope and assigned one-time story passive rewards to the trainer. ADR-0022 replaced earned focus resets; ADR-0023 then made training ceilings badge-wide. These changes retain this record's individually earned capability system.

Editorial audit: consolidated repeated trainer/training rules into their owning records. The [pre-audit record](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/blob/66c17faf1292626ae5f0ec70531aaf1b2a871ec1/docs/adr/0019-gym-resistances-and-capabilities.md) preserves the full clarification sequence.
