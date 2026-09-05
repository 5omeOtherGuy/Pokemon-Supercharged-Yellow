# ADR-0008: Preselect limited battle consumables and retain loss costs

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers in design interview round 2
- Implementation: Not started — design policy only
- Supersedes: None
- Superseded by: None

## Context

The interview offered a major-boss loss policy with a money penalty and spent consumables remaining spent. The user chose it and added a limited, preselected consumable system.

## Decision

- A major boss defeat incurs a money penalty, and consumed items remain spent.
- Fix the number of non-held consumables allowed in a battle.
- Require the player to select those consumables beforehand; unrestricted inventory substitution during the fight is incompatible with preselection.
- Held items are excluded from this numerical allowance.
- Three consumables was an example, not a confirmed numerical limit.

Money-loss amounts, precise quota semantics and eligible items remain open. Excluding held items from the quota does not by itself settle consumed held-item persistence.

## Alternatives considered

No resource penalty, or a money penalty with consumables restored, were declined. Unlimited bag access conflicts with the user's added preselection requirement.

## Consequences

Consumable selection becomes part of encounter planning. Repeated defeat can deplete supplies and money. Replenishment and the economy must support continued attempts. No specific item categories or opponent advantages are implied.

## Evidence and validation

User chose option 3 (money penalty and consumed items remaining spent) and required a fixed number of preselected consumables, excluding held items, giving three as an example. Future tests should cover selection locking, quantities, consumption, unused supplies, cap enforcement, loss accounting and battle-entry paths.

No game implementation or runtime validation has occurred.

## Open questions

- Exact allowance and whether it changes with progression.
- Individual units/uses versus slots/types; duplicate items; whether filling all slots is mandatory.
- Eligible items, especially revives, X items and PP restoration.
- Opponent quota and visibility of their selected supplies.
- Boss/all-trainer/wild-battle scope and capture items.
- Consumed held items and unused-item handling.
- Money-loss formula, replenishment, retry location and healing.

## Follow-up

Clarify allowance, eligible categories and opponent rules next. Resolve battle scope, economy and recovery details before implementation.
