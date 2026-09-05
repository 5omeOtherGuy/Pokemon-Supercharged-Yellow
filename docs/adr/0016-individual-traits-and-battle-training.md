# ADR-0016: Preserve individual traits and reward battle experience

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers on IVs, battle-earned development and fixed natures
- Implementation: Not started — traits, development goals and gym rules accepted; reward formula remains proposed
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

These are accepted experience requirements. They do not yet select traditional EVs, an allocation interface, additional stat currencies, permanent exclusive rewards, reward amounts or a final growth formula. Gym eligibility is selected below; ordinary battle-training credit remains open. Random starting IVs and deterministic training progression are distinct choices.

### Gym rewards and rematches

- Award gym rewards after the player wins the encounter.
- A Pokémon qualifies only if it entered the field and did not faint at any point during that battle.
- Switching out does not remove eligibility. A qualifying Pokémon need not be active on the final turn or score a knockout.
- Unused reserves and participants that fainted do not qualify. Healing after the battle does not alter its completed reward eligibility.
- Use stronger gym-rematch teams suited to current campaign progress so recruits can earn the same gym rewards as veterans.
- These rules govern the gym reward, not yet ordinary experience, IV improvement or all other battle credit.

Surviving support Pokémon can qualify. A participant that helps win but faints forfeits its personal gym reward; other eligible team members can still receive theirs.

## Relationship to earlier decisions

[ADR-0017](0017-trusted-core-and-boss-mastery.md) supersedes ADR-0005: encourage a trusted core with rotating specialists. Repeated attempts and boss mastery remain intended. No fixed core size or permanent-party requirement is chosen.

[ADR-0006](0006-substantial-levelling-investment.md) remains a 20–30-minute-or-more levelling target. This does not authorise stacking several equally long IV and battle-training chores on top. Total preparation time and whether the activities overlap remain open.

Use the existing hidden-stat information boundary in [ADR-0009](0009-vgc-style-ai-information.md). Do not automatically reveal new training allocations or exact bonuses to boss AI. Public training-history fields, if any, need an explicit mapping.

## Proposed reward mechanism — not yet accepted

The gym-rematch route and eligibility above are accepted. A bounded training-point pool remains a candidate:
- Ordinary eligible battles grant predictable progress; qualifying boss victories grant a meaningful bonus.
- Show the reward conditions, current progress, maximum and stat effects in-game.
- Let the player direct earned development toward the Pokémon's role; the interface and reallocation rules remain open.
- Give comparable Pokémon the same eventual training ceiling. Use the selected gym-rematch catch-up route: a later recruit can earn the same gym reward a veteran previously received.
- Track gym rewards per individual Pokémon and gym milestone. Award each qualifying reward once to that Pokémon, whether obtained in the original fight or a rematch. Already rewarded veterans can help recruits without earning a duplicate reward.
- Apply the selected gym rule: field participation, no fainting and a player victory. Surviving support Pokémon qualify without needing a knockout.
- Use the selected stronger rematch teams suited to campaign progress. Access timing, progression tiers and exact teams remain open; replaying unchanged early teams with temporary player downlevelling was not selected.

The intended benefit is a real veteran advantage before equivalent training, with no permanently lost potential solely from joining late. This shared-ceiling proposal does not satisfy an interpretation requiring a veteran to remain stronger after both Pokémon complete equivalent training; that tradeoff must be decided explicitly.

A candidate illustration: two otherwise identical level-30 Pokémon differ in earned training, so the one with several gym victories is stronger. A late recruit could eventually close that gap by winning the relevant gym rematches and earning the same rewards. Gym rematches and their general difficulty direction are selected; point values, caps, access and exact teams remain open.

## Alternatives and tradeoffs

- Bosses accelerate a shared training pool: simple and recoverable, but the numerical distinction can disappear once both Pokémon are fully trained.
- Boss experience unlocks recoverable mastery rewards through equivalent challenges: gives boss-like experience a distinct role, with more progression rules to explain.
- Campaign-only, permanently exclusive stat rewards: preserves a lifelong veteran advantage but risks penalising late catches and missed opportunities.
- Cosmetic records alone: can supplement development but do not meet the requested same-level gameplay benefit.
- Final-knockout-only and party-wide credit were considered. The user selected field participation plus survival in a won encounter, including surviving support Pokémon but excluding unused reserves and fainted participants.

Reward-formula alternatives remain under consideration. Gym eligibility and progression-appropriate rematches are settled as stated above.

## Evidence and validation

Source: user answers on 2026-09-05. The user selected random, improvable IVs, adding that randomisation must not make a Pokémon unusable; fixed acquired natures; and a transparent, non-tedious battle-development system rewarding gym experience while accommodating rotation. The user explicitly flagged the relationship with the earlier rotation ADR. The follow-up proposed gym rematches. The next answers selected participants that did not faint, stronger rematch teams suited to campaign progress, and a trusted core with rotating specialists.

No code, battle simulation or timing results exist. Once a mechanism is selected:
- Evaluate poor allowed IV outcomes alongside fixed natures and meaningful species roles at relevant caps.
- Compare otherwise identical veterans and recruits to isolate training benefits from level, IVs, nature and loadout.
- Compare retained-core teams, frequent-rotation teams and late recruits in both battle formats.
- Measure total preparation time and repeated-defeat effects on progression.
- Test bench participation, support, fainting, partial boss defeats, rematches and duplicate awards.
- Check that players can predict rewards without an external guide and that caps prevent uncontrolled growth.

## Open questions

- Gym-rematch access timing, exact progression-appropriate teams and interaction with ordinary training.
- Ordinary battle-training credit and non-gym boss reward policies; gym eligibility is settled.
- Numerical IV safeguards, costs, access timing and targeted IV adjustment.
- Growth formula, allocations, reallocation, progression caps and boss milestones.
- Whether bosses and ordinary trainers use comparable training bonuses.
- Training overlap with levelling, rematch access and late-game catch-up.
- Treatment of gifts, eggs and evolution for acquired traits and earned progression.

## Follow-up

Resolve reward form, limits, allocation and rematch access before implementing the progression mechanism. Follow ADR-0017 for team continuity. Do not implement the proposed training-points system merely because this requirements record is Accepted.

## Proposal history — 2026-09-05, gym rematches

While this record was being drafted, the user suggested rematching gym leaders to earn the rewards veterans received. The candidate now uses that concrete route instead of an unspecified catch-up activity. Per-Pokémon reward tracking, duplicate prevention and meaningful rematch difficulty are agent recommendations. Accepted requirements remain distinct from these proposed implementation rules.

## Clarification — 2026-09-05, accepted gym rules

The user selected only Pokémon that entered battle and did not faint for the reward after victory, and stronger rematch teams suited to current campaign progress. These replace the earlier open eligibility and difficulty questions. The trusted-core choice is in ADR-0017. Per-Pokémon milestone tracking, duplicate-award rules, allocations and numerical limits remain proposed or unresolved; the gym rules do not settle the complete reward economy.
