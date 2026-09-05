# ADR-0016: Preserve individual traits and reward battle experience

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers on IVs, battle-earned development, fixed natures and gym participation/rematches
- Implementation: Not started — design policy only
- Supersedes: None
- Superseded by: None

## Context

The user selected improvable random IVs with a usability safeguard and fixed natures. For EVs, the user described an experience instead of choosing an existing system: using an individual Pokémon, especially in gym battles, should develop it beyond levelling, without excessively penalising rotation. Rematching gym leaders should let recruits earn veterans' rewards.

## Decision

### Individual traits

- IVs are random and can be improved through resources or training.
- IV randomisation must not make a Pokémon unusable.
- Keep its acquired nature and nature-based stat effects fixed.
- The IV safeguard does not promise universal species viability, an optimal individual for every role, or success in every matchup.

Exact IV distributions, safeguards, improvement access and costs remain open. Evaluate allowed IV outcomes together with fixed nature effects and species roles. No numerical IV floor, universal maximum spread or method of adjusting particular IVs is selected.

### Battle-earned development and rematches

- Reward individual battle use, particularly boss experience, with meaningful development beyond level. A gym-experienced Pokémon should be better developed than an otherwise comparable same-species, same-level Pokémon lacking that experience.
- Reward continuity without excessive rotation penalties. Progression must be understandable, predictable and non-tedious; random starting IVs are separate from deterministic earned development.
- Use stronger gym-rematch teams suited to current campaign progress so recruits can earn the same personal gym rewards as veterans.

Current reward mechanics have separate owners: [ADR-0019](0019-gym-resistances-and-capabilities.md) defines personal capabilities and their participation/survival/victory eligibility; [ADR-0023](0023-badge-wide-training-ceilings.md) defines permanent focus-directed training, ordinary-training eligibility and automatic trainer-wide badge ceilings. Recruits earn their own points and capabilities; they need no personal gym victory for ceiling access. Neither system changes ordinary level EXP or IV-improvement eligibility by implication.

## Relationship to other decisions

[ADR-0017](0017-trusted-core-and-boss-mastery.md) selects a trusted core with rotating specialists. [ADR-0006](0006-substantial-levelling-investment.md) owns levelling and preparation pacing, including additional ordinary training after levelling. Total preparation duration remains open.

Keep IVs, nature, earned points, training ceilings and capability loadouts distinguishable. [ADR-0009](0009-vgc-style-ai-information.md) protects exact opponent stats and hidden allocations; new public passive fields remain Proposed in ADR-0024.

## Alternatives and tradeoffs

- Bosses merely accelerate ordinary training: not the selected complete model; badges unlock capacity under ADR-0023 and individuals earn capabilities under ADR-0019.
- Permanently exclusive campaign-only stat rewards: could preserve veteran advantage but risk penalising late catches and missed opportunities.
- Cosmetic battle records alone: insufficient for the requested same-level gameplay benefit.
- Final-knockout-only or whole-party capability credit: not selected. Surviving support participants qualify; unused reserves and fainted participants do not. A participant can help win and still forfeit its reward by fainting.

Progression-appropriate rematches preserve catch-up potential but can add travel, training and repeated-attempt costs. Eligibility may encourage brief participation or discourage sacrificial support roles; these are playtest risks, not grounds to silently change the selected survival rule.

## Evidence and validation

Recorded user answers on 2026-09-05 selected random improvable IVs with a usability safeguard, fixed acquired natures and transparent development rewarding gym experience while accommodating rotation. The user proposed rematches, then chose surviving field participants after victory, stronger progression-appropriate rematch teams and a trusted core.

No implementation, simulation or timing results exist. Future validation should compare poor allowed IV outcomes with fixed natures at relevant caps; otherwise comparable veterans and recruits; retained cores and rotating specialists in both formats; and complete preparation time. Verify support/bench/fainting outcomes, recoverable rewards and understandable progression using the canonical mechanics above.

## Open questions

- Numerical IV safeguards, targeted improvements, resources, costs and access timing.
- Gym-rematch access and exact progression-appropriate teams.
- Treatment of gifts, eggs, evolution and storage for individual traits and earned progression.
- Comparable opponent training assumptions; separate trainer-build and capability use is governed by [ADR-0021](0021-trainer-builds-and-reward-ownership.md).
- Other non-capability development rewards from non-gym bosses.
- Detailed training formulas/focus definitions in ADR-0023 and preparation duration in ADR-0006.

## Follow-up

Evaluate the trait safeguard against representative species roles and fixed natures. Use ADRs 0019, 0021 and 0023 for concrete reward and progression design; accepted goals are not tested balance.

## Decision history — 2026-09-05

- [c0a0dd5](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/c0a0dd5) recorded the traits/development goals and rematch proposal; per-Pokémon tracking and duplicate prevention were agent recommendations, not independent user choices. [1dd1c2c](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/1dd1c2c) accepted gym eligibility and rematch difficulty.
- ADR-0018 selected individual gym budgets and focus allocation, later earned focus changes/instant redistribution and additional ordinary training. ADR-0019 then added three-slot learned capabilities, expanded reward sources, free service reassignment, held-item coexistence and shared capability eligibility.
- ADR-0021 superseded ADR-0020's trainer scope and restricted individual capability sources to repeatable encounters. ADR-0022 superseded ADR-0018's reset model; ADR-0023 replaced individual training ceilings with badge-wide unlocks.
- ADR-0023 subsequently applied participation, no fainting and victory to ordinary stat training too, then adopted EXP-style reward scaling. Earlier statements that participation governs capabilities “only” concern ceiling access, not the now-selected training-point eligibility.

Editorial audit: consolidated duplicate mechanics and resolved open questions into canonical links. The [complete pre-audit clarification log](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/blob/66c17faf1292626ae5f0ec70531aaf1b2a871ec1/docs/adr/0016-individual-traits-and-battle-training.md) remains available.
