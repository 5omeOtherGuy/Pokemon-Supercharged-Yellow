# ADR-0016: Preserve individual traits and reward battle experience

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers on IVs, battle-earned development and fixed natures
- Implementation: Not started — requirements accepted; progression mechanism remains proposed
- Supersedes: None
- Superseded by: None

## Context

The interview asked about IVs, EVs and nature-based stat effects. The user selected improvable random IVs with a usability safeguard and fixed natures. For EVs, the user described a desired experience rather than selecting an existing system: using a Pokémon, particularly in gym battles, should develop it beyond levelling. This must reward continuity without excessively penalising team rotation. In a subsequent message, the user proposed battling gym leaders again so a newer Pokémon can earn the reward a veteran received.

## Decision

### Individual traits

- IVs are random and can be improved through resources or training.
- IV randomisation must not make a Pokémon unusable.
- Keep the Pokémon's acquired nature and its nature-based stat effects fixed.
- Do not interpret the IV safeguard as a promise that every species becomes viable, every individual is optimal for every role, or every random spread wins every matchup.

Exact IV distributions, safeguards, improvement access and costs remain open. Assess the IV safeguard together with the fixed nature and intended species roles. The user has not selected a numerical IV floor, universal maximum values or a method for adjusting particular IVs.

### Battle-earned development

- Reward using a Pokémon in battle, particularly boss battles.
- Battle history should yield meaningful development beyond level: the user wants a gym-experienced Pokémon to be better developed than an otherwise comparable same-species, same-level Pokémon lacking that experience.
- Incentivise continued use of individual Pokémon while avoiding excessive penalties for frequent switching.
- Progression must be clear and understandable, with predictable rewards; it should not feel tedious or random.

These are accepted experience requirements. They do not yet select traditional EVs, an allocation interface, additional stat currencies, permanent exclusive rewards, reward amounts, contribution rules, or a final growth formula. Random starting IVs and deterministic training progression are distinct choices.

## Relationship to earlier decisions

[ADR-0005](0005-team-rotation-and-boss-mastery.md) originally targeted frequent changes to team composition. The new continuity incentive qualifies that target, and the user explicitly requested review of the earlier direction. Adaptation and retaining experienced members must both be considered. No new frequency, fixed core size or permanent-party requirement has been chosen. Repeated attempts and mastery of difficult bosses remain intended.

[ADR-0006](0006-substantial-levelling-investment.md) remains a 20–30-minute-or-more levelling target. This does not authorise stacking several equally long IV and battle-training chores on top. Total preparation time and whether the activities overlap remain open.

Use the existing hidden-stat information boundary in [ADR-0009](0009-vgc-style-ai-information.md). Do not automatically reveal new training allocations or exact bonuses to boss AI. Public training-history fields, if any, need an explicit mapping.

## Proposed design candidate — gym-rematch catch-up, not yet accepted

Use a bounded pool of training points earned through battle:
- Ordinary eligible battles grant predictable progress; qualifying boss victories grant a meaningful bonus.
- Show the reward conditions, current progress, maximum and stat effects in-game.
- Let the player direct earned development toward the Pokémon's role; the interface and reallocation rules remain open.
- Give comparable Pokémon the same eventual training ceiling. Use the user's proposed gym rematches as the preferred catch-up route: a later recruit can earn the same gym reward a veteran previously received.
- Track gym rewards per individual Pokémon and gym milestone. Award each qualifying reward once to that Pokémon, whether obtained in the original fight or a rematch. Already rewarded veterans can help recruits without earning a duplicate reward.
- Credit active support as well as offensive participation; do not base the proposed system solely on the final knockout. Exact field-participation, fainting and victory requirements remain open.
- Select explicit rematch access and difficulty rules. A high-level team repeating an unchanged early gym may not provide the meaningful battle experience intended; level restrictions or progression-appropriate teams are candidates, not chosen mechanics.

The intended benefit is a real veteran advantage before equivalent training, with no permanently lost potential solely from joining late. This shared-ceiling proposal does not satisfy an interpretation requiring a veteran to remain stronger after both Pokémon complete equivalent training; that tradeoff must be decided explicitly.

A candidate illustration: two otherwise identical level-30 Pokémon differ in earned training, so the one with several gym victories is stronger. A late recruit could eventually close that gap by winning the relevant gym rematches and earning the same rewards. Gym rematches are the user's proposed route; point values, caps, access and difficulty have not been selected.

## Alternatives and tradeoffs

- Bosses accelerate a shared training pool: simple and recoverable, but the numerical distinction can disappear once both Pokémon are fully trained.
- Boss experience unlocks recoverable mastery rewards through equivalent challenges: gives boss-like experience a distinct role, with more progression rules to explain.
- Campaign-only, permanently exclusive stat rewards: preserves a lifelong veteran advantage but risks penalising late catches and missed opportunities.
- Cosmetic records alone: can supplement development but do not meet the requested same-level gameplay benefit.
- Final-knockout-only or survival-only credit can disadvantage support roles; party-wide credit can reward passive bench use. Eligibility is unresolved.

These are considered approaches, not settled mechanics.

## Evidence and validation

Source: user answers on 2026-09-05. The user selected random, improvable IVs, adding that randomisation must not make a Pokémon unusable; fixed acquired natures; and a transparent, non-tedious battle-development system rewarding gym experience while accommodating rotation. The user explicitly flagged the relationship with the earlier rotation ADR. The follow-up asked about rematching a gym leader to earn what the veteran received; this is captured as the preferred design candidate pending its remaining rules.

No code, battle simulation or timing results exist. Once a mechanism is selected:
- Evaluate poor allowed IV outcomes alongside fixed natures and meaningful species roles at relevant caps.
- Compare otherwise identical veterans and recruits to isolate training benefits from level, IVs, nature and loadout.
- Compare retained-core teams, frequent-rotation teams and late recruits in both battle formats.
- Measure total preparation time and repeated-defeat effects on progression.
- Test bench participation, support, fainting, partial boss defeats, rematches and duplicate awards.
- Check that players can predict rewards without an external guide and that caps prevent uncontrolled growth.

## Open questions

- Finalise the proposed gym-rematch route for earning the same veteran rewards, its access timing, difficulty and interaction with ordinary training.
- Revised expected team continuity and rotation frequency.
- Which Pokémon qualify for battle and boss credit; whether credit requires winning the encounter.
- Numerical IV safeguards, costs, access timing and targeted IV adjustment.
- Growth formula, allocations, reallocation, progression caps and boss milestones.
- Whether bosses and ordinary trainers use comparable training bonuses.
- Training overlap with levelling, rematch access and late-game catch-up.
- Treatment of gifts, eggs and evolution for acquired traits and earned progression.

## Follow-up

Resolve gym-rematch access/difficulty, participation and team-continuity choices before selecting the final progression mechanism. If a later answer replaces the earlier rotation target, record that change through the ADR lifecycle. Do not implement the proposed training-points system merely because this requirements record is Accepted.

## Proposal history — 2026-09-05, gym rematches

While this record was being drafted, the user suggested rematching gym leaders to earn the rewards veterans received. The candidate now uses that concrete route instead of an unspecified catch-up activity. Per-Pokémon reward tracking, duplicate prevention and meaningful rematch difficulty are agent recommendations. Accepted requirements remain distinct from these proposed implementation rules.
