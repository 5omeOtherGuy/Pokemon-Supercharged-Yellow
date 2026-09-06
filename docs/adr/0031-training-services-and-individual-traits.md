# ADR-0031: Make individual development inspectable and serviceable

- Status: Accepted
- Date: 2026-09-06
- Decision authority: Numerical and usability choices delegated by the owner's full-project mandate
- Implementation: Center model/UI implementation underway; ordinary runtime and balance validation pending
- Supersedes: None
- Superseded by: None

## Decision

Preserve acquired nature and random individuality while preventing unusably poor new recruits: randomly generated campaign IVs range from 15 through 31. Explicitly authored opponent IVs remain explicit. Apply the random range to ordinary, scripted and gift creation; inheritance cannot lower a legitimate campaign parent's IV below that range. This is a usability safeguard, not a guarantee of optimal traits.

At mainland Pokémon Centers, improve one chosen IV by one, up to 31, for **max(20, 4 × current level) Practice Points**. Display the current value, exact cost and resulting value before confirmation. Practice Points are trainer-owned and equal the sum of Gen-III defeated-opponent base EXP values, including the trainer multiplier, paid only after the whole battle is won. They may support a recruit that did not earn them. Personal training and capability eligibility remain separately restricted to field participants who never fainted (ADR-0029). Losses and captures supply no defeated-opponent Practice Points. This common pool avoids requiring extra successful gym rewards to improve a recruit.

Offer free current-level move relearning and free selection among the species' actual official ability slots, including an existing hidden ability. Omit absent/duplicate abilities. Respect the original-151 evolution boundary and acquired nature. Recalculation after service changes preserves the existing HP deficit and never revives a fainted Pokémon.

Use the Center upstairs lounge for earned capability assignment, unlocked trainer builds, ability changes, relearning, IV improvement and battle-supply selection. All assignments obey their separate ownership and budgets. Only Center/designated-service contexts may respec; the Indigo Plateau entry Center is available, but League interior rooms do not provide respec between battles. Show descriptions, costs, active/earned state, personal points, badge ceiling, future focus and Practice Points. Failed or cancelled changes must leave data and payment unchanged.

Add a field party-menu Training entry for inspection and freely changing future focus. Existing points remain permanent. Explain when a focused stat is full and further growth will be discarded. The field screen cannot change capability assignments, trainer builds, abilities or IVs.

Trainer passive sources are initially Study and Formation; Composure from Brock; Tempo from the S.S. Anne rival; Offense from Rocket Hideout Giovanni; Preparation from Mr. Fuji's rescue; Field Study from Silph Giovanni; and Recovery Coach from Champion status. Repeated story victories do not duplicate unlocks. Badge count controls the independent weighted build budget.

## Alternatives and consequences

Perfect IVs on recruitment would erase the accepted individual-development axis. An unrestricted 0–31 range makes a poor recruit require excessive repair. A species-specific item or repeated gym-only currency would add collection friction unrelated to practising battles. The floor and trainer-wide pool preserve differences while letting a developed core support specialists.

The cost and earning rate are provisional numerical tuning. Measure actual time to recruit, level, train and improve a specialist alongside ADR-0006's substantial levelling and ADR-0023's additional focused preparation requirements. A transaction test cannot establish that burden or enjoyable challenge.

## Verification required

Test all random acquisition paths, nature permanence, ability validity, free relearning limits, Center and League context restrictions, overspending/invalid-mask rejection, cancellation, HP-deficit preservation and save/reload. Inspect the actual GBA interfaces and Android controls. Run whole-battle reward tests independently of the service model. Human playtests remain required to judge discoverability and preparation.
