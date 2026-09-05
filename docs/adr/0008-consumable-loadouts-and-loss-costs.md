# ADR-0008: Preselect limited battle consumables and retain loss costs

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers in design interview rounds 2 and 3
- Implementation: Not started — design policy only
- Supersedes: None
- Superseded by: None

## Context

The interview offered a major-boss loss policy with a money penalty and spent consumables remaining spent. The user chose it and added a limited, preselected consumable system.

## Decision

- A major boss defeat incurs a money penalty, and consumed items remain spent.
- Use one fixed non-held consumable allowance throughout the campaign. Determine its numerical value through balance testing; do not scale it with progression.
- Require the player to select those consumables beforehand; unrestricted inventory substitution during the fight is incompatible with preselection.
- Held items are excluded from this numerical allowance.
- Three consumables was an example, not a confirmed numerical limit.
- Eligible player supplies: HP healing, status-condition cures, PP restoration and temporary stat boosters such as X Attack. Revives are excluded from the player's preselected battle supplies.
- Only major bosses among opposing trainers may use bag consumables. Their maximum allowance equals the player's; ordinary trainers do not use bag consumables.

Money-loss amounts, precise counting/selection semantics and individual item availability remain open. The eligible player categories are settled, while any additional category restrictions on bosses remain unresolved. Excluding held items from the quota does not by itself settle consumed held-item persistence.

## Alternatives considered

No resource penalty, or a money penalty with consumables restored, were declined. Unlimited bag access conflicts with the user's added preselection requirement.

## Consequences

Consumable selection becomes part of encounter planning. Repeated defeat can deplete supplies and money. Replenishment and the economy must support continued attempts. The shared numerical maximum does not mean player and boss must bring identical supplies or use every permitted item. Powerful healing, PP recovery and stat boosters must be assessed together when choosing the limit. Category eligibility does not make every item available at every campaign stage.

## Evidence and validation

User chose option 3 (money penalty and consumed items remaining spent) and required a fixed number of preselected consumables, excluding held items, giving three as an example. Future tests should cover selection locking, quantities, consumption, unused supplies, cap enforcement, loss accounting and battle-entry paths.

No game implementation or runtime validation has occurred.

## Open questions

- Exact numerical allowance, delegated to balance testing within the accepted campaign-wide fixed-limit policy.
- Individual units/uses versus slots/types; duplicate items; whether filling all slots is mandatory.
- Individual item access, mixed-effect items, and any additional boss-category restrictions. Player revives are excluded; eligible player categories are decided above.
- Visibility of boss supplies and how their preselected loadout is authored. Opponent maximum and ordinary-trainer prohibition are decided above.
- Boss/all-trainer/wild-battle scope and capture items.
- Consumed held items and unused-item handling.
- Money-loss formula, replenishment, retry location and healing.

## Follow-up

Test numerical allowances within the confirmed policy. Clarify remaining selection, information and item-access details. Resolve battle scope, economy and recovery details before implementation.

## Clarification — 2026-09-05, design interview round 3

The user selected one fixed campaign-wide allowance with the number determined through balance testing; HP healing, status cures, PP restoration and temporary stat boosters as the eligible player categories; and only major bosses using bag consumables, with the same maximum as the player. Revives were not selected in the explicit category selection and are therefore excluded from player battle supplies. This resolves open details without superseding the original preselection and loss-cost policy.

Future validation must cover eligible categories, rejection of player revives, a constant campaign limit, equal player/boss maxima and no ordinary-trainer bag use. No gameplay tests have run.
