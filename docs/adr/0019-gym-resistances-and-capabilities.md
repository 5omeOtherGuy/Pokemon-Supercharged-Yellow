# ADR-0019: Combine gym training capacity with earned Pokémon capability slots

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers on combined rewards, three-slot capability capacity, expanded sources, service-only reassignment, separate held items and universal encounter eligibility
- Implementation: Not started — Pokémon capabilities and fixed encounter rewards accepted; trainer builds in ADR-0021; permanent training in ADR-0023
- Supersedes: None
- Superseded by: None

## Context

The user initially considered partial type resistance or a unique capability instead of the extra training budget in ADR-0018. The follow-up selected both extra training capacity and modest earned effects, described the effects as passive capabilities comparable to skill-tree passives or held-item benefits, and initially allowed one or two active at a time. Before this record was committed, the user replaced that count with three capacity slots per Pokémon, variable capability costs, optional drawbacks and sources beyond gyms. Trainer-wide badge passives were suggested as a possibility.

## Decision

- Retain the trainer-wide, badge-unlocked training ceilings in [ADR-0023](0023-badge-wide-training-ceilings.md). No personal gym participation is required for ceiling access.
- Add modest Pokémon-earned passive capabilities that support further customisation, individual roles and a deeper roleplaying experience.
- Treat skill-tree passives and held-item effects as design references. No branching reward choice is selected: each granting encounter has one fixed capability, the same for every qualifying Pokémon. Particular effects and their power remain open.
- Each Pokémon has three capacity slots for assigned earned passive capabilities. Each capability costs exactly one, two or three slots; the sum of assigned costs must not exceed three.
- Valid full-capacity combinations are 1+1+1, 2+1 and 3. Partial use is allowed; three slots are a capacity limit, not a requirement to assign three capabilities or unlock a second/third slot later.
- A capability may combine positive and negative effects. A drawback does not create extra capacity or a negative slot cost.
- Capabilities can be earned from gym leaders, Elite Four battles and other notable encounters only where repeat access lets later recruits earn them. Non-repeatable notable story encounters grant trainer passives instead, never Pokémon capabilities (ADR-0021).
- Apply the same eligibility at all these sources: the player wins the encounter, the Pokémon entered the field, and it never fainted during that battle. Switched-out survivors and surviving support participants qualify; unused reserves and fainted participants do not.
- Keep the normal held-item slot in addition to the three capability slots. An ordinary held item does not consume capability capacity.
- Assigned earned capabilities can be changed freely, without a resource cost, but only at a Pokémon Center or designated service. Do not provide unrestricted reassignment elsewhere or during battle. This changes assignments among already earned capabilities; it does not grant unearned capabilities.
- Each capability-granting encounter awards one fixed capability, identical for every qualifying Pokémon; there is no species-specific variant or choice of reward at that encounter.
- Each of the eight leaders contributes a distinct reward. Progression-appropriate rematches let a recruit earn the same rewards; duplicate wins do not replace other leaders' contributions.

All eight personal leader rewards define the full collection of gym capabilities, not training-ceiling access or the complete collection of capabilities from every encounter. The capacity limit means earning additional capabilities expands available choices rather than activating all of them together. Reward identity is fixed per encounter; player customisation comes from assigning already earned capabilities within capacity. Expanded capability sources do not automatically grant extra training-budget increments beyond the eight gyms.

This originally supplemented ADR-0018. ADR-0023 is now the current training policy after ADR-0022: capacity is unlocked trainer-wide through badges, alongside this individually earned capability system.

## Scope alongside existing abilities and customisation

[ADR-0012](0012-ability-and-typing-rebalance.md) continues to govern innate ability assignments: reassign existing official abilities when justified. This record explicitly authorises designing a separate gym-earned passive system. It does not authorise replacing every innate ability with a new custom one.

The earlier restrained-customisation goal now explicitly includes this requested Pokémon-development choice. It does not justify unrelated rules toggles, multiple difficulties or an unrestricted skill tree.

The standard modern type chart remains selected. Blanket partial resistance is not the chosen primary reward model; no specific damage-reduction effect or chart change is selected here.

Under ADR-0023, focus may change on the fly and only directs future gains; existing stat points never relocate. Capability reassignment follows the free, service-only rule above. Keep these actions distinct.

## Trainer-wide badge passives and opponents

[ADR-0021](0021-trainer-builds-and-reward-ownership.md) accepts intended existing badge benefits and a limited selectable trainer build at a Pokémon Center, with training/EXP and modest combat benefits. Trainer expertise must be distinct from individually learned capabilities. The exact reference, effects and trainer capacity remain open.

Trainer-owned effects apply separately from personally earned capabilities. A recruit gains access to the trainer's current training ceiling through badge ownership, but does not gain an individual capability merely because its trainer holds the badge.

Major bosses and selected experienced trainers may use capabilities within the same capacity limit. Teams and individuals need not fill all slots. Progression must fit trainer experience and story; ADR-0021 records the player's accomplishment goal and local ace example.

## Alternatives considered

- Replace extra gym training capacity with effects: not selected; retain both.
- Primarily small type-damage reductions: not selected as the principal reward design.
- All earned Pokémon effects active at once: not selected.
- A fixed one-or-two active-effect limit: discussed initially, then replaced by the user's three-slot model before commitment.
- Trainer-wide effects: originally proposed, now accepted in scope through ADR-0021.
- Reward selection or species-specific variants: not selected; use one fixed capability per granting encounter.

## Consequences

Evaluate the complete Pokémon: base stats, IVs, fixed nature, earned training, innate ability, held item and active learned passives. Compare interaction strength, not only the value of an effect by itself. The trainer-wide bonuses accepted in ADR-0021 add another layer to assess.

Preserve species identity and weaknesses while creating meaningful choices. Examples such as status protection or defensive triggers remain unapproved design candidates, not assigned rewards.

Price and assess combinations, not only isolated effects. Evaluate whether a supposed drawback is a meaningful cost for the build using it; do not assume a nominal penalty necessarily offsets a strong benefit.

A normal held item coexists with the capability loadout, and reassignment is free only at the selected service locations. Exact effects and interactions still need decisions; each encounter's reward is fixed for all qualifying Pokémon. Define scouting and AI visibility for battle-relevant passives; existing hidden-stat protections do not automatically hide or disclose the new capability fields.

## Evidence and validation

User answers on 2026-09-05 selected both extra capacity and effects, and passive capabilities with skill-tree or held-item-like character. The next message explicitly amended the active limit: each Pokémon has three slots; capabilities cost one, two or three, may have positive and negative effects, and can come from gym leaders, Elite Four or other notable encounters. Trainer badge passives were phrased as a possibility.

No game implementation or balance results exist. Future validation should cover reward eligibility, original/rematch equivalence, duplicate handling, active limits, effect interactions, relevant UI and AI information. Test both singles and doubles, several species roles, and veteran/recruit progression without assuming all rewards are needed for every encounter. Verify slot sums for all allowed combinations, rejection of over-capacity assignments, and predictable application of both benefits and drawbacks.

## Open questions

- Individual capability costs within the selected one-to-three-slot range, effects and drawbacks.
- Detailed effect stacking and interactions with held items and innate abilities; the separate normal held-item slot is selected.
- Exact identity of the single fixed capability granted by each source.
- Concrete repeat-access arrangements for Elite Four and other capability sources; non-repeatable story encounters cannot grant Pokémon capabilities.
- Designated service locations and access timing, including capability reassignment during an Elite Four sequence.
- Exact trainer-passive effects, reference badge behavior and build capacity within ADR-0021; selection at a Pokémon Center is accepted.
- Concrete effects, magnitudes, stacking rules and individual opponent loadouts under ADR-0021.
- Public information fields for boss scouting and AI.

## Follow-up

Follow ADR-0021 for trainer-wide scope and opponent use. Draft representative one-, two- and three-slot fixed capability rewards and assess their combinations alongside held items, badge passives and the retained training budget.

## Decision history — 2026-09-05

Initially Proposed to compare partial resistance and capabilities and to review replacing the training budget. Updated to Accepted after the user selected both capacity and individual passive effects, then amended the concurrent limit to three slots with costs of one, two or three. Optional drawbacks and expanded encounter sources are accepted. The earlier one-or-two-effect count was never committed as the final rule. Trainer-wide badge effects were still a proposal at that stage; ADR-0020 later accepts their scope. No particular effect or numerical power value was selected by that earlier decision.

## Clarification — 2026-09-05, service reassignment and shared eligibility

The user selected free capability reassignment only at a Pokémon Center or designated service, retention of the normal held-item slot in addition to capability capacity, and the same participation/no-fainting/team-victory eligibility for Elite Four and other notable encounters as for gyms. Ordinary battle experience and training-credit rules are not automatically changed.

Future verification should distinguish focus changes from capability reassignment; reject reassignment outside permitted services and in battle; preserve earned capabilities when unassigned; and enforce the same reward eligibility at every capability-granting encounter. No implementation or runtime tests have occurred.

## Clarification — 2026-09-05, fixed rewards and selective opponent use

The user selected one fixed capability per granting encounter, identical for all qualifying Pokémon. ADR-0020 accepts intended existing badge benefits plus modest combat passives, and major bosses/selected experienced trainers using capabilities with incomplete loadouts where appropriate. Progression must fit trainer experience and preserve the player's sense of accomplishment.

## Clarification — 2026-09-05, reward ownership and revised training

ADR-0021 limits individual capability sources to repeatable encounters and assigns non-repeatable story passive rewards to the trainer. ADR-0022 replaces earned focus changes and redistribution with flexible focus directing future permanent gains. Neither changes the three-slot capability system or its free service-only reassignment rule.

## Clarification — 2026-09-05, global ceiling versus personal capability

[ADR-0023](0023-badge-wide-training-ceilings.md) makes stat-training capacity trainer-wide through badge ownership. Personal participation, no fainting and victory remain conditions for capability rewards; the earlier broad description of individual gym development does not require recruits to earn stat ceilings through rematches.
