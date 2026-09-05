# ADR-0006: Preserve substantial levelling investment

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers in design interview round 2
- Implementation: Not started — design policy only
- Supersedes: None
- Superseded by: None

## Context

At the time of the interview, [ADR-0005](0005-team-rotation-and-boss-mastery.md) called for frequent rebuilding. [ADR-0017](0017-trusted-core-and-boss-mastery.md) now replaces that target with a trusted core and rotating specialists. The interview asked how long a newly caught Pokémon should normally take to reach the current cap, explicitly considering levelling only.

## Decision

Target substantial training investment: roughly 20–30 minutes or more under typical relevant conditions. This is a pacing target, not an enforced minimum timer for every catch or campaign stage. Moves, natures and stat training are separate decisions.

## Alternatives considered

Almost immediate catch-up, or 5–10 minutes of targeted training. The user selected substantial training.

## Consequences

Levelling is intentional preparation. Do not introduce universal instant catch-up as a default QoL feature. Training multiple replacements could multiply this cost; shared experience and group-training policies remain unresolved.

## Evidence and validation

User answer on 2026-09-05: "Substantial training: roughly 20–30 minutes or more." Future timing checks must record starting level, cap, campaign stage, party size, training activity and battle-speed setting. Measure both one replacement and multiple replacements.

No game implementation or runtime validation has occurred.

## Open questions

- Training activities and access.
- Individual versus shared experience and group-training time.
- Reference speed setting for the pacing target.
- Stage-dependent exceptions and naturally near-cap catches.

## Follow-up

Design training and experience access alongside encounters, and measure actual pacing before accepting numerical experience tuning.

## Clarification — 2026-09-05, total preparation burden

[ADR-0016](0016-individual-traits-and-battle-training.md) adds random improvable IVs, fixed natures and the goal of clear, non-tedious development from battle use and boss experience. The 20–30-minute-or-more target remains specifically about levelling. Whether levelling, IV improvement and earned development overlap is unresolved; do not treat the levelling target as approval for several additional training grinds. Measure the complete cost of introducing a replacement when designing the combined system.

When evaluating gym-rematch catch-up in ADR-0016, include rematch duration and repeated attempts in total preparation cost. Eligibility requires participation without fainting in a player victory; rematch teams must suit campaign progress. [ADR-0018](0018-gym-training-budget-and-focus.md) now requires all eight distinct leader rewards for full gym-related development; measure that completion path separately from the training needed for a recruit to contribute usefully.

## Clarification — 2026-09-05, additional ordinary training

The user selected ordinary stat training that usually requires additional focused battles after levelling. This resolves the general overlap preference; the extra duration, activity and total preparation burden are still unchosen. The original levelling target remains separate. Instant redistribution after an earned later-game focus change does not require repeating that training (ADR-0018).
