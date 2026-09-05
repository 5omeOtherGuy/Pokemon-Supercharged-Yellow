# ADR-0008: Preselect limited battle consumables and retain loss costs

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers in design interview rounds 2–5
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
- Limit duplicate supplies by functional category. Determine separate category caps through balance testing; do not assume a universal one-item-per-category limit. Different item names must not bypass a category cap.
- Exclude items that combine multiple consumable categories from preselected battle supplies, including items combining HP healing and status curing. This rule applies to the battle-supply system; held items and out-of-battle item rules remain separately scoped.
- Both player and boss see each other's complete selected bag-consumable loadout, including quantities, before battle. This reveals available supplies, not the item-use action chosen for the current turn.
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
- Numerical per-category caps are delegated to balance testing. Overall counting semantics and whether filling all slots is mandatory remain open.
- Individual item access, implementation classification of item effects and any additional boss-category restrictions. Cross-category battle supplies and player revives are excluded; eligible player categories are decided above.
- How boss loadouts are authored and when each side's selection is locked. Mutual visibility of complete supplies, opponent maximum and ordinary-trainer prohibition are decided above.
- Boss/all-trainer/wild-battle scope and capture items.
- Consumed held items and unused-item handling.
- Money-loss formula, replenishment, retry location and healing.

## Follow-up

Test numerical allowances within the confirmed policy. Clarify remaining selection/locking and item-access details. Resolve battle scope, economy and recovery details before implementation.

## Clarification — 2026-09-05, design interview round 3

The user selected one fixed campaign-wide allowance with the number determined through balance testing; HP healing, status cures, PP restoration and temporary stat boosters as the eligible player categories; and only major bosses using bag consumables, with the same maximum as the player. Revives were not selected in the explicit category selection and are therefore excluded from player battle supplies. This resolves open details without superseding the original preselection and loss-cost policy.

Future validation must cover eligible categories, rejection of player revives, a constant campaign limit, equal player/boss maxima and no ordinary-trainer bag use. No gameplay tests have run.

## Clarification — 2026-09-05, design interview round 4

The user selected category-based duplicate limits, using one HP-healing item as an example, and full pre-battle visibility of boss consumables and quantities. Numerical category caps, mixed-effect classification and boss visibility of player supplies remain unresolved. Validate category restrictions against differently named items serving the same role; verify displayed boss supplies match the actual locked loadout.

## Clarification — 2026-09-05, design interview round 5

The user delegated separate numerical category limits to balance testing, excluded items combining categories from battle supplies, and confirmed mutual visibility of complete consumable loadouts. These resolve earlier open questions without changing the overall fixed campaign allowance or loss-cost policy.

Future checks must reject mixed-category supplies, enforce both overall and category limits, and confirm each side sees the actual locked supplies and quantities. Knowing the available supplies must not reveal the opponent's pending item action. No implementation or gameplay validation has occurred.
