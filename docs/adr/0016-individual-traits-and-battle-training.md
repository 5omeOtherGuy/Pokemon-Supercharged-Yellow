# ADR-0016: Preserve individual traits and reward battle experience

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers on IVs, battle-earned development and fixed natures
- Implementation: Not started — traits and gym rules accepted; ADR-0023 selects focus and training boundaries; ADR-0019 accepts extra capacity plus learned Pokémon passives
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

ADR-0023 selects automatic focus allocation and trainer-wide badge-stage training ceilings. ADR-0019 retains individually earned capabilities alongside this global capacity progression. Ordinary training credit requires field participation, no fainting and a team victory under ADR-0023. Numerical rewards, focus definitions and eligible opponent encounters remain open. Random starting IVs and deterministic training progression are distinct choices.

### Individual gym capabilities and rematches

- Award individual gym capabilities after the player wins the encounter.
- A Pokémon qualifies only if it entered the field and did not faint at any point during that battle.
- Switching out does not remove eligibility. A qualifying Pokémon need not be active on the final turn or score a knockout.
- Unused reserves and participants that fainted do not qualify. Healing after the battle does not alter its completed reward eligibility.
- Use stronger gym-rematch teams suited to current campaign progress so recruits can earn the same gym rewards as veterans.
- These rules govern personal capability rewards. They do not restrict access to badge-wide training ceilings; ordinary experience, IV improvement and training credit remain separate.

Surviving support Pokémon can qualify. A participant that helps win but faints forfeits its personal gym capability reward; other eligible team members can still receive theirs.

## Relationship to earlier decisions

[ADR-0017](0017-trusted-core-and-boss-mastery.md) supersedes ADR-0005: encourage a trusted core with rotating specialists. Repeated attempts and boss mastery remain intended. No fixed core size or permanent-party requirement is chosen.

[ADR-0006](0006-substantial-levelling-investment.md) remains a 20–30-minute-or-more levelling target. This does not authorise stacking several equally long IV and battle-training chores on top. Total preparation time and whether the activities overlap remain open.

Use the existing hidden-stat information boundary in [ADR-0009](0009-vgc-style-ai-information.md). Do not automatically reveal new training allocations or exact bonuses to boss AI. Public training-history fields, if any, need an explicit mapping.

## Training direction and supplemental gym passives

[ADR-0023](0023-badge-wide-training-ceilings.md) now records the selected direction:
- Retain the extra gym budget alongside modest Pokémon-earned passives in [ADR-0019](0019-gym-resistances-and-capabilities.md); use three capacity slots, with capabilities costing one, two or three slots and optionally combining benefits and drawbacks.
- The player chooses a focus and earned points are allocated automatically.
- Full training capacity follows the trainer's badge progression for every Pokémon. Collecting all eight individual gym capabilities still requires personally qualifying against all eight leaders.
- Recruits share current training ceilings immediately; qualifying rematches let them earn missed personal capabilities.

The gym reward direction now combines extra capacity and learned Pokémon passives. Point-to-stat conversion, numerical caps and initial focus choices remain open. Focus can change on the fly to direct future gains; points never relocate. Independent per-stat limits permit eventual completion of every stat, with relevant-stat completion the normal late-game goal. Show the trainer's badge-stage ceiling, individual accumulated points, personal capability eligibility and focus effects clearly.

## Alternatives and tradeoffs

- Bosses only accelerate a shared ordinary training pool: considered but not selected; ADR-0023 grants additional capacity.
- Badge progression unlocks additional capacity trainer-wide: selected in ADR-0023; individual rematches recover capabilities only.
- Campaign-only, permanently exclusive stat rewards: preserves a lifelong veteran advantage but risks penalising late catches and missed opportunities.
- Cosmetic records alone: can supplement development but do not meet the requested same-level gameplay benefit.
- Final-knockout-only and party-wide credit were considered. The user selected field participation plus survival in a won encounter, including surviving support Pokémon but excluding unused reserves and fainted participants.

Automatic focus allocation and extra gym capacity remain selected in ADR-0023; ADR-0019 supplements them with individual passive effects. Numerical formulas and remaining progression details are open. Gym eligibility and progression-appropriate rematches remain as stated above.

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
- Eligible opponent encounters and award amounts for ordinary training, plus other non-capability rewards from non-gym bosses. ADR-0023 selects participation/no-fainting/victory for ordinary stat-training credit; ADR-0019 applies these conditions to capability rewards.
- Numerical IV safeguards, costs, access timing and targeted IV adjustment.
- Numerical growth formula, focus definitions, permanent allocation, gain rates and badge-stage limits within ADR-0023.
- Whether bosses and ordinary trainers use comparable training bonuses.
- Additional focused training duration after levelling, rematch access and late-game catch-up.
- Treatment of gifts, eggs and evolution for acquired traits and earned progression.

## Follow-up

Follow ADR-0023 for permanent allocation and flexible focus, ADR-0017 for team continuity, ADR-0019 for fixed individual capability rewards, and ADR-0021 for trainer-wide badge scope and selective opponent capability use. Resolve numerical limits, concrete effects, access and pacing before implementing the progression mechanism. Accepted policies do not establish tested game balance.

## Proposal history — 2026-09-05, gym rematches

While this record was being drafted, the user suggested rematching gym leaders to earn the rewards veterans received. The candidate now uses that concrete route instead of an unspecified catch-up activity. Per-Pokémon reward tracking, duplicate prevention and meaningful rematch difficulty are agent recommendations. Accepted requirements remain distinct from these proposed implementation rules.

## Clarification — 2026-09-05, accepted gym rules

The user selected only Pokémon that entered battle and did not faint for the reward after victory, and stronger rematch teams suited to current campaign progress. These replace the earlier open eligibility and difficulty questions. The trusted-core choice is in ADR-0017. At that stage, per-Pokémon milestones, duplicate-award rules, allocations and numerical limits remained proposed or unresolved; later choices are recorded below.

## Clarification — 2026-09-05, budget and allocation selected

The user selected limited extra gym training capacity, automatic allocation through a chosen focus and all eight leaders contributing separate rewards. [ADR-0018](0018-gym-training-budget-and-focus.md) records these choices, resolving the earlier proposal's core direction. Exact budget sizes, filling of unlocked capacity, focus changes and remaining progression rules stay open.

## Clarification — 2026-09-05, focus access and gym reward proposal

The user selected later-game earned focus changes with instant redistribution, and additional ordinary training after levelling. Those choices are recorded in ADR-0018. At that stage, partial resistance or a unique capability was proposed in ADR-0019. The later resolution retains capacity and adds modest individual passive capabilities, with the user's amended three-slot capacity model. Elite Four and other notable encounters may also grant capabilities; trainer-wide badge scope is subsequently accepted in ADR-0020, with exact effects open.

## Clarification — 2026-09-05, capability reassignment and credit

ADR-0019 fixes free reassignment of earned capabilities only at Pokémon Centers or designated services and retains the separate normal held-item slot. The gym eligibility rule now also governs capability rewards from Elite Four and other notable encounters. This does not decide ordinary experience, IV-improvement or training-point credit after those battles, and does not relax the earned-access requirement for changing a training focus.

## Clarification — 2026-09-05, current training and ownership policy

[ADR-0022](0022-permanent-training-and-flexible-focus.md) supersedes ADR-0018: focus may change on the fly, existing points never relocate, and every stat can eventually reach its own maximum. The earlier earned-focus restrictions above are historical. [ADR-0021](0021-trainer-builds-and-reward-ownership.md) supersedes ADR-0020 and limits Pokémon capabilities to repeatable encounters; non-repeatable story passive rewards belong to the trainer.

## Clarification — 2026-09-05, global training capacity

[ADR-0023](0023-badge-wide-training-ceilings.md) replaces the individual gym-capacity interpretation. Badges raise ceilings for all Pokémon; participation and survival govern capabilities only. Earlier history referring to individual capacity contributions no longer describes the current training rule.

## Clarification — 2026-09-05, ordinary training eligibility selected

The user now applies field participation, never fainting and team victory to ordinary stat-training points as well as capability eligibility. Switched-out survivors qualify; unused reserves and fainted participants do not. Losing or fainting never removes previously earned points. Earlier notes leaving ordinary training credit open are historical. Ordinary EXP and IV improvement remain separate, and trainer-wide training ceilings still follow badges regardless of individual battle outcomes. See [ADR-0023](0023-badge-wide-training-ceilings.md).
