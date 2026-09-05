# ADR-0006: Preserve substantial levelling investment

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers in design interview round 2
- Implementation: Not started — design policy only
- Supersedes: None
- Superseded by: None

## Context

[ADR-0005](0005-team-rotation-and-boss-mastery.md) calls for frequent team rebuilding. The interview asked how long a newly caught Pokémon should normally take to reach the current cap, explicitly considering levelling only.

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
