# ADR-0007: Provide full boss team scouting before the first attempt

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers in design interview round 2
- Implementation: Not started — design policy only
- Supersedes: None
- Superseded by: None

## Context

[ADR-0005](0005-team-rotation-and-boss-mastery.md) makes boss mastery central. The interview asked what information should be available in-game before the first attempt.

## Decision

Make the boss's full team, moves, abilities and held items available in-game before the first attempt. Players need not lose to obtain these details. This establishes available information, not a requirement to open a scouting screen.

## Alternatives considered

Theme and strategic hints only, or species/levels with other details hidden. The user selected full team information.

## Consequences

Boss challenge must hold up when these details are known. Displayed information must match actual encounter data and relevant variants. Once an engine exists, derive it from authoritative trainer data where practical.

## Evidence and validation

User answer on 2026-09-05: "Their full team, moves, abilities and held items." Future checks must verify first-attempt access and accuracy against actual teams.

No game implementation or runtime validation has occurred.

## Open questions

- Access location, presentation and any acquisition cost.
- Levels, natures, IVs/EVs and exact stats beyond the explicitly selected fields.
- Opponent bag-consumable loadout visibility.
- Boss definitions, variants and ordinary-trainer scouting.
- Additional tactical explanations or calculation tools.

## Follow-up

Resolve presentation and remaining information fields before implementing scouting. Coordinate with [ADR-0008](0008-consumable-loadouts-and-loss-costs.md).
