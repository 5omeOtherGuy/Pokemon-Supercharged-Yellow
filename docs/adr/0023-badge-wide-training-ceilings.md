# ADR-0023: Unlock training ceilings trainer-wide through gym badges

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user correction that gym training stages are player-wide badge unlocks, with limited tactical stat preparation
- Implementation: Not started — progression policy only; numbers and playtests pending
- Supersedes: [ADR-0022](0022-permanent-training-and-flexible-focus.md)
- Superseded by: None

## Context

ADR-0022 correctly recorded permanent stat gains and freely changing focus, but carried forward individual gym participation as the condition for raising training ceilings. The user clarified that the trainer's badges unlock training stages for every Pokémon, using badge-gated obedience and level progression as an analogy. The earlier question about each Pokémon earning stages in gym order therefore had the wrong premise.

This record replaces that individual capacity requirement and retains the chosen permanent training model. It also narrows the earlier broad references to individual gym rewards in ADR-0016 and ADR-0019: their participation requirements continue to govern Pokémon capabilities, not training ceilings.

## Decision

### Badge-wide stage unlocks

- The trainer's gym badges determine the current training stage and its per-stat training ceilings.
- Receiving the next badge unlocks the next stage's training capacity for all Pokémon the player uses, including reserves, fainted participants and later recruits.
- A Pokémon does not need to participate in, survive or personally win any gym battle to access the trainer's unlocked training ceiling.
- Badges unlock capacity, not automatically filled stat points. Each Pokémon still earns its own training gains through battles.
- No individual gym contribution counter or sequence of rematches is required to unlock stat-training capacity. Rematches cannot repeatedly increase the trainer's ceiling from the same badge.
- Completing the badge progression unlocks the full campaign training ceiling for every Pokémon. Exact ceilings before the first badge, between badges and after the eighth remain to be designed.
- These progression limits follow badge ownership automatically; they are not an optional passive that must be assigned in the trainer build from [ADR-0021](0021-trainer-builds-and-reward-ownership.md).
- The obedience analogy explains progression gating. It does not introduce random disobedience, import an official obedience formula, or require the stat ceilings to equal level-cap numbers.

### Permanent growth and flexible focus

- A training focus determines the stat area receiving future earned training points. Allocate earned points automatically according to that focus.
- Focus can change on the fly. Remove the earlier late-game unlock, rare-item/challenge requirement and instant redistribution rule.
- Changing focus never relocates previously allocated points. Existing training gains remain in their stats; no stat-point reset or redistribution mechanism is selected.
- Every stat has a maximum for additional points earned through training.
- One Pokémon can theoretically reach every stat's training maximum. Do not impose a shared total-point ceiling that makes this impossible.
- Reaching all maxima should be difficult and exceptional. Normal late-game development aims to maximise the stats relevant to a Pokémon's role, rather than every stat.
- Do not assume equal per-stat caps, a main-series EV formula or a particular diminishing-return curve; numerical details remain open.
- On-the-fly focus changes concern future training; they do not change battle stats instantly. Exact UI timing, including whether focus can be changed during a battle, is an implementation detail still to resolve.

### Ordinary stat-training eligibility

- A Pokémon earns ordinary stat-training points from a battle only if it entered the field, never fainted at any point in that battle, and its team won.
- Switched-out survivors qualify; staying active on the final turn or scoring a knockout is not required. Unused reserves do not qualify.
- Fainting disqualifies that Pokémon from this battle's training award even if it is revived. A team defeat grants no ordinary training points from that battle.
- These outcomes do not remove points earned from earlier battles. Existing allocations remain permanent.
- This selects eligibility, not the award size or which opponent encounters provide training. Resolve the complete battle outcome before finalising its training award.
- Ordinary level EXP and IV-improvement rules are separate and are not changed by this decision. Badge-wide ceiling access also remains independent of participation and survival.

### EXP-style training rewards

- Scale ordinary stat-training rewards like regular battle EXP in the main-series Pokémon games. The user's reference selects an EXP-based approach to opponent reward value, rather than an equal award for every victory or a new minimum-challenge cutoff.
- Use the chosen main-series EXP baseline to determine how opponent level and experience yield affect training progress. Do not treat subjective encounter difficulty as an independently selected reward formula.
- The exact reference generation, recipient-level scaling, conversion from EXP-based reward value to training points, rounding and modifiers remain unresolved. This does not select one EXP as one stat-training point.
- Preserve this project's stricter ordinary-training eligibility: entered battle, never fainted and team victory. Referencing EXP reward scaling does not import EXP Share or other eligibility rules into stat training.
- Training must remain available at the campaign level cap, as already required by post-levelling focused training. Base its reward on the battle's EXP value before level-cap suppression rather than the amount of level EXP actually added. This is an implementation constraint derived from the existing pacing decision.
- Earned training remains bounded by the current badge-stage per-stat ceiling. EXP-style scaling does not grant room beyond that ceiling.

### Limited tactical preparation

- Training should offer bounded tactical adjustments, such as enough Defense to survive a particular attack or enough Speed to move before a particular opponent.
- These are intended design possibilities, not guarantees that every Pokémon can reach every desired threshold.
- Stage ceilings must prevent unlimited grinding from overcoming a gym through excessive trained stats. More training cannot exceed the current badge stage's limits.
- Balance the size of training bonuses so Pokémon identities, strengths, weaknesses, team choices and battle decisions remain important.
- Assess several trained stats together, including the strongest legally attainable spread at the current stage. Rare or time-consuming preparation is not a substitute for a real stat ceiling.
- Ordinary stat training should usually require additional focused battles after reaching the level cap. Exact rates and additional duration remain open.
- Preserve the substantial levelling target and trusted core with rotating specialists. Ordinary late-game builds should target relevant stats rather than universal completion.

### Related personal rewards

[ADR-0019](0019-gym-resistances-and-capabilities.md) owns individually earned capabilities, their participation/survival/victory rule, three-slot assignments and service-only reassignment. Each of the eight leaders still grants a distinct personal capability; collecting all eight requires qualifying for all eight rewards. Full training-ceiling access instead follows the trainer's badges. Repeatable capability sources remain governed by ADR-0021, and individual traits by ADR-0016.

## Alternatives considered

- Individual participation unlocks each Pokémon's stat-training stage: rejected by the user's correction.
- Recruits must rematch earlier gyms to access the current training ceiling: no longer required; rematches remain relevant to capabilities.
- Badge acquisition automatically fills everyone's training points: not selected; it raises capacity.
- Unlimited pre-gym growth, or relying only on slow grinding to constrain strength: inconsistent with bounded preparation.
- Remove stat training entirely to prevent overpreparation: not selected; limited survival and Speed adjustments are desired.

## Consequences

A recruit shares the veteran's available training ceiling immediately, while actual stat gains and personally earned capabilities still distinguish their development. This reduces repeated gym catch-up work without granting an instantly finished Pokémon.

Separate three concepts in the UI and implementation: trainer-owned stage ceilings, individually accumulated training points, and individually earned capabilities. Show current gains and the badge-locked maximum clearly.

## Evidence and validation

On 2026-09-05 the user explicitly stated that training stages unlock player-wide through gym badges and should constrain overtraining while allowing limited preparation to survive an attack or outspeed an opponent. No numerical caps, damage calculations, game implementation or playtests exist.

Future validation should check:
- A badge raises the applicable ceiling for active, stored and subsequently acquired Pokémon.
- A Pokémon missing or fainting in that gym battle still benefits from the global ceiling, without receiving an unearned capability.
- Badge acquisition does not fill training points or relocate existing allocations.
- Changing focus preserves earned points and directs future gains.
- Every training award path respects the current per-stat ceiling.
- Surviving field participants receive training credit after a win, including switched-out support participants; unused reserves, fainted/revived participants and all participants in a lost battle receive none.
- Ineligible battle outcomes preserve points earned from earlier battles; no early per-knockout award bypasses the final eligibility check.
- Duplicate rematches do not raise badge-based ceilings.
- Rewards follow the selected EXP baseline across opponent levels and yields, while retaining the stricter training eligibility.
- A Pokémon at the level cap still earns training when eligible and below its stat-training ceiling.
- Role-focused and maximum stage-legal spreads retain challenging singles and doubles encounters.
- Representative survival and Speed thresholds allow useful but limited adjustments without erasing species weaknesses.

## Open questions

- Numerical per-stat ceilings by badge stage and point-to-stat conversion.
- How badge order maps to stages if the eventual campaign permits gyms out of sequence.
- Exact main-series EXP reference and training-point conversion, including recipient-level scaling, rounding and which EXP modifiers affect training. No separate minimum-challenge cutoff is selected.
- Focus choices, gain-rate tuning, eligible encounter edge cases such as catches or scripted outcomes, and handling a focused stat already at its cap.
- Total preparation duration and opponent training assumptions.
- Whether players receive in-game help assessing specific survival and Speed thresholds.
- Rematch access for capabilities and later postgame progression.

## Follow-up

Draft a provisional badge-stage table and evaluate its actual stat impact at the corresponding level caps. Compare targeted preparation, typical role-focused development and maximum stage-legal training. Do not label untested values balanced.

## Decision history — 2026-09-05

[4991131](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/4991131) corrected ADR-0022's individual-ceiling interpretation, carrying forward permanent gains and flexible focus. [2bd8f84](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/2bd8f84) then selected participation, no fainting and team victory for ordinary training too. Neither changed ordinary EXP or IV-improvement rules.

Asked how useful much weaker opponents should be for training, the user answered: "Same as regular exp gain in the main pokemon games." [83b1bd2](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/83b1bd2) recorded EXP-style scaling without selecting a generation or conversion. Continued training at the level cap is an engineering consequence of the accepted post-levelling pacing, not a separately quoted formula choice.

The original [secondary experience overview](https://bulbapedia.bulbagarden.net/wiki/Experience), consulted on 2026-09-05, was preliminary context for differing EXP formulas. It is not a selected baseline or verified source implementation. Pin and inspect primary source before implementing reward scaling.

Editorial audit: consolidated clarifications and linked personal-capability details to their owner. Eligibility, permanent growth, all-stat potential and badge-wide ceilings are unchanged.
