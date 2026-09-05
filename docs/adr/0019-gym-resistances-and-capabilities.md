# ADR-0019: Combine gym training capacity with earned Pokémon capability slots

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers selecting both rewards and passive capabilities, followed by the amended three-slot capacity and expanded encounter sources
- Implementation: Not started — Pokémon passive direction accepted; trainer-wide passives remain proposed
- Supersedes: None
- Superseded by: None

## Context

The user initially considered partial type resistance or a unique capability instead of the extra training budget in ADR-0018. The follow-up selected both extra training capacity and modest earned effects, described the effects as passive capabilities comparable to skill-tree passives or held-item benefits, and initially allowed one or two active at a time. Before this record was committed, the user replaced that count with three capacity slots per Pokémon, variable capability costs, optional drawbacks and sources beyond gyms. Trainer-wide badge passives were suggested as a possibility.

## Decision

- Retain the limited extra gym training capacity in [ADR-0018](0018-gym-training-budget-and-focus.md).
- Add modest Pokémon-earned passive capabilities that support further customisation, individual roles and a deeper roleplaying experience.
- Treat skill-tree passives and held-item effects as design references. This does not yet choose a branching tree, a particular item effect, a full held item's power, or use of the held-item slot.
- Each Pokémon has three capacity slots for assigned earned passive capabilities. Each capability costs exactly one, two or three slots; the sum of assigned costs must not exceed three.
- Valid full-capacity combinations are 1+1+1, 2+1 and 3. Partial use is allowed; three slots are a capacity limit, not a requirement to assign three capabilities or unlock a second/third slot later.
- A capability may combine positive and negative effects. A drawback does not create extra capacity or a negative slot cost.
- Capabilities can be earned from gym leaders, Elite Four battles and other notable encounters. Eligibility and repeat-access rules for non-gym sources remain to be defined.
- Apply gym reward eligibility from [ADR-0016](0016-individual-traits-and-battle-training.md): the player wins, the Pokémon entered the field, and it did not faint during that battle.
- Each of the eight leaders contributes a distinct reward. Progression-appropriate rematches let a recruit earn the same rewards; duplicate wins do not replace other leaders' contributions.

All eight leader rewards define full gym-related development, not the complete collection of capabilities from every encounter. The capacity limit means earning additional capabilities expands available choices rather than activating all of them together. Exact reward choices and whether learning involves branches remain open. Expanded capability sources do not automatically grant extra training-budget increments beyond the eight gyms.

This supplements ADR-0018; it does not supersede it. The earlier extra-budget review is resolved in favour of retaining that budget.

## Scope alongside existing abilities and customisation

[ADR-0012](0012-ability-and-typing-rebalance.md) continues to govern innate ability assignments: reassign existing official abilities when justified. This record explicitly authorises designing a separate gym-earned passive system. It does not authorise replacing every innate ability with a new custom one.

The earlier restrained-customisation goal now explicitly includes this requested Pokémon-development choice. It does not justify unrelated rules toggles, multiple difficulties or an unrestricted skill tree.

The standard modern type chart remains selected. Blanket partial resistance is not the chosen primary reward model; no specific damage-reduction effect or chart change is selected here.

Focus changes still follow their earned later-game rule with instant redistribution. That rule does not automatically govern choosing or changing active gym passives.

## Proposed trainer-wide badge passives — not accepted

The user suggested that gym badges may also provide trainer passives applying to all Pokémon they use. This would be a trainer-owned progression track, separate from the individually earned Pokémon rewards.

If adopted, a newly recruited Pokémon could benefit from its trainer's existing badge effects without having personally won those gym battles. That does not grant the recruit its individual gym reward or waive participation/survival eligibility for that reward.

Inclusion, effect domain (battle, training or exploration), acquisition rules, simultaneous effects and opponent equivalents remain unresolved. Do not treat this proposed track as approved because the Pokémon-passive policy is Accepted.

## Alternatives considered

- Replace extra gym training capacity with effects: not selected; retain both.
- Primarily small type-damage reductions: not selected as the principal reward design.
- All earned Pokémon effects active at once: not selected.
- A fixed one-or-two active-effect limit: discussed initially, then replaced by the user's three-slot model before commitment.
- Trainer-wide effects: user-proposed, pending clarification.

## Consequences

Evaluate the complete Pokémon: base stats, IVs, fixed nature, earned training, innate ability, held item and active learned passives. Compare interaction strength, not only the value of an effect by itself. Any proposed trainer-wide bonus adds another layer to assess.

Preserve species identity and weaknesses while creating meaningful choices. Examples such as status protection or defensive triggers remain unapproved design candidates, not assigned rewards.

Price and assess combinations, not only isolated effects. Evaluate whether a supposed drawback is a meaningful cost for the build using it; do not assume a nominal penalty necessarily offsets a strong benefit.

The held-item interaction, passive selection rules and reward choices within each encounter need explicit decisions. Define scouting and AI visibility for battle-relevant passives; existing hidden-stat protections do not automatically hide or disclose the new capability fields.

## Evidence and validation

User answers on 2026-09-05 selected both extra capacity and effects, and passive capabilities with skill-tree or held-item-like character. The next message explicitly amended the active limit: each Pokémon has three slots; capabilities cost one, two or three, may have positive and negative effects, and can come from gym leaders, Elite Four or other notable encounters. Trainer badge passives were phrased as a possibility.

No game implementation or balance results exist. Future validation should cover reward eligibility, original/rematch equivalence, duplicate handling, active limits, effect interactions, relevant UI and AI information. Test both singles and doubles, several species roles, and veteran/recruit progression without assuming all rewards are needed for every encounter. Verify slot sums for all allowed combinations, rejection of over-capacity assignments, and predictable application of both benefits and drawbacks.

## Open questions

- Individual capability costs within the selected one-to-three-slot range, effects and drawbacks.
- Relationship to held items and innate ability interactions.
- Fixed encounter rewards versus choices or branches within each source.
- Non-gym eligibility and repeat access for missed Elite Four or other notable-encounter capabilities.
- How active passives are selected and changed, and any associated cost or restrictions.
- Trainer-wide badge passive inclusion and scope.
- Concrete effects, magnitudes, stacking rules and opponent equivalents.
- Public information fields for boss scouting and AI.

## Follow-up

Resolve held-item interaction, capability-changing rules and non-gym reward eligibility; clarify trainer-wide scope separately. Then draft representative one-, two- and three-slot capabilities and assess their combinations alongside the retained training budget.

## Decision history — 2026-09-05

Initially Proposed to compare partial resistance and capabilities and to review replacing the training budget. Updated to Accepted after the user selected both capacity and individual passive effects, then amended the concurrent limit to three slots with costs of one, two or three. Optional drawbacks and expanded encounter sources are accepted. The earlier one-or-two-effect count was never committed as the final rule. Trainer-wide badge effects remain a proposal; no particular effect or numerical power value is selected.
