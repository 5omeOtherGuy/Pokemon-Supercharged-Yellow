# ADR-0016: Preserve individual traits and reward battle experience

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers on IVs, battle-earned development and fixed natures
- Implementation: Not started — traits and gym rules accepted; ADR-0018 selects focus and training boundaries; gym reward form reopened in proposed ADR-0019
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

ADR-0018 selects automatic focus allocation and all eight leaders contributing separately; its earlier gym-budget choice is now under review in proposed ADR-0019. Numerical rewards, focus definitions and ordinary battle-training credit remain open. Random starting IVs and deterministic training progression are distinct choices.

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

## Training direction and reopened gym rewards

[ADR-0018](0018-gym-training-budget-and-focus.md) now records the selected direction:
- The previously selected extra gym budget is under review in [proposed ADR-0019](0019-gym-resistances-and-capabilities.md), following the user's resistance/capability suggestion.
- The player chooses a focus and earned points are allocated automatically.
- Full gym development requires all eight distinct leader rewards per Pokémon. Each contribution counts once and can be earned through the original battle or a qualifying rematch.
- Joining late does not prevent earning the same set of gym contributions.

Gym reward form is now reopened. Point-to-stat conversion, numerical caps and initial focus choices remain open. Established focus changes unlock later through a limited rare item or challenge, with instant redistribution once earned. Show eligibility, earned and missing gym contributions, and focus effects clearly.

## Alternatives and tradeoffs

- Bosses only accelerate a shared ordinary training pool: considered but not selected; ADR-0018 grants additional capacity.
- Boss experience unlocks additional recoverable training capacity: selected through eight separate gym rewards and rematches in ADR-0018.
- Campaign-only, permanently exclusive stat rewards: preserves a lifelong veteran advantage but risks penalising late catches and missed opportunities.
- Cosmetic records alone: can supplement development but do not meet the requested same-level gameplay benefit.
- Final-knockout-only and party-wide credit were considered. The user selected field participation plus survival in a won encounter, including surviving support Pokémon but excluding unused reserves and fainted participants.

Automatic focus allocation remains selected in ADR-0018; its extra gym-budget choice is under review in ADR-0019. Numerical formulas and remaining progression details are open. Gym eligibility and progression-appropriate rematches remain as stated above.

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
- Numerical growth formula, focus definitions, redistribution, reward filling and limits within ADR-0018; eight distinct gym contributions are selected.
- Whether bosses and ordinary trainers use comparable training bonuses.
- Additional focused training duration after levelling, rematch access and late-game catch-up.
- Treatment of gifts, eggs and evolution for acquired traits and earned progression.

## Follow-up

Follow ADR-0018 for allocation and earned focus changes, ADR-0017 for team continuity, and proposed ADR-0019 for the reopened gym-reward discussion. Resolve reward form, numerical limits, access and pacing before implementing the progression mechanism. Accepted policies do not establish tested game balance.

## Proposal history — 2026-09-05, gym rematches

While this record was being drafted, the user suggested rematching gym leaders to earn the rewards veterans received. The candidate now uses that concrete route instead of an unspecified catch-up activity. Per-Pokémon reward tracking, duplicate prevention and meaningful rematch difficulty are agent recommendations. Accepted requirements remain distinct from these proposed implementation rules.

## Clarification — 2026-09-05, accepted gym rules

The user selected only Pokémon that entered battle and did not faint for the reward after victory, and stronger rematch teams suited to current campaign progress. These replace the earlier open eligibility and difficulty questions. The trusted-core choice is in ADR-0017. At that stage, per-Pokémon milestones, duplicate-award rules, allocations and numerical limits remained proposed or unresolved; later choices are recorded below.

## Clarification — 2026-09-05, budget and allocation selected

The user selected limited extra gym training capacity, automatic allocation through a chosen focus and all eight leaders contributing separate rewards. [ADR-0018](0018-gym-training-budget-and-focus.md) records these choices, resolving the earlier proposal's core direction. Exact budget sizes, filling of unlocked capacity, focus changes and remaining progression rules stay open.

## Clarification — 2026-09-05, focus access and gym reward proposal

The user selected later-game earned focus changes with instant redistribution, and additional ordinary training after levelling. Those choices are recorded in ADR-0018. Partial type resistance or a unique capability is proposed as an alternative gym reward in ADR-0019; it is not yet an approved effect or replacement model.
