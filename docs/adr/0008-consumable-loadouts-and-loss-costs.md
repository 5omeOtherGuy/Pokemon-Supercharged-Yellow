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

## Decision history — 2026-09-05

- Round 2 selected money loss and spent supplies remaining spent; the user's added preselection quota excluded held items and used three only as an example. The meaning of “option 3” is explicitly recorded in [the original ADR at 030b85d](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/blob/030b85d/docs/adr/0008-consumable-loadouts-and-loss-costs.md); this audit has no raw interview transcript.
- Round 3 selected a fixed campaign-wide maximum, the four player categories and equal boss allowance with no ordinary-trainer bag use. Revives were omitted from the explicit player category selection and recorded as excluded; this is a documented interpretation of that selection.
- Round 4 added category-based duplicate limits and full boss-supply scouting. Round 5 delegated separate numerical category caps to testing, excluded mixed-category supplies and confirmed mutual visibility. One HP item was an example, not a universal category cap.

Editorial audit: consolidated the resolved round 3–5 questions here. Future checks must reject mixed-category supplies/player revives, enforce overall and category maxima across differently named items, keep the campaign limit constant, prohibit ordinary-trainer bag use, and verify both visible loadouts match actual supplies without revealing pending actions. No gameplay checks have run.
