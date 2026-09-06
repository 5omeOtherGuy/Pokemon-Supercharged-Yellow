# ADR-0034: Keep preparation supplies replaceable and equipment staged

- Status: Accepted
- Date: 2026-09-06
- Decision authority: Economy and numerical choices delegated by the owner's full-project mandate
- Implementation: Shop/pickup source checks and map assembly pass; integrated runtime and economic/human balance checks pending
- Supersedes: None
- Superseded by: None

## Decision

Sell repeatable held recovery and status berries from the early mainland Marts, expanding to Sitrus, Lum and Leppa at Cerulean. Replace the unusable Berry Powder service with an ordinary cash shop. Offer stage-appropriate medicine and consumed held-item replacements at later Marts and the Indigo entry shop. Keep purchases and respec outside the League interior.

Replace disabled vitamin shops and all twenty-two mainland vitamin pickups with usable equipment, berries or medicine, preserving pickup flags, positions and full-bag retry behavior. Celadon offers normal type enhancers, Eviolite, Focus Sash, Air Balloon and Light Clay. Add Light Ball only after four badges and remove wild Pikachu's rare-held Light Ball assignment so early capture farming cannot bypass that timing. Preserve wild Pikachu itself and its base species identity.

Retain a meaningful held-slot choice: Light Ball supports Pikachu's later offensive role, while its frailty remains; Eviolite supports unevolved recruits; type enhancers trade defensive utility for sustained damage. These item effects are unchanged. Initial prices range from80 for basic berries through500 for Focus Sash,3000 for Eviolite and5000 for Light Ball. The exact authoritative price and access table, twenty-two pickup replacements and matchup hypotheses are in [the economy record](../balance/campaign-economy.md).

Provide two no-fee, explicit talk-to-rematch practice sources: Rick in Viridian Forest and Ethan on Route24. Preserve their first encounter, defeated flag, original team, ordinary EXP and ordinary prize. Their locations keep an earning option accessible on each side of Mt. Moon's one-way crossing. Subsequent battles require consent; no automatic repeat aggression or artificial grants. These low-stage encounters are recovery routes, not the sole planned late-game training activity.

Losses still consume used supplies and incur bounded money loss. Free Centers, optional zero-supply attempts and free repeatable battles prevent cash alone from blocking another attempt. Affordability is not established merely because an item can eventually be purchased. Measure failed attempts, travel, healing and complete specialist preparation, alongside the unchanged substantial levelling target in ADR-0006.

## Rationale and alternatives

Selling disabled vitamins charges money for an effect the campaign rejects. Restoring those effects would bypass permanent training policy. A finite supply of consumed equipment would eventually remove strategies after repeated losses. Automatic refunds would erase the accepted loss cost. Repeatable purchases and earned recovery preserve both preparation and consequences.

Unrestricted early Light Ball would double the starter's attacking stats before opening gyms and reduce the usefulness of ordinary alternatives. A four-badge access point preserves an offensive role for the unevolved Yellow starter later. Human and matchup testing may justify numerical or price refinements within these goals.

## Verification and unresolved seams

Source tests cover exact stage-gated stock, prices, ordinary first/repeat/decline script paths, all disabled mainland pickups, full-bag gift retry and the early wild-held bypass. Actual event preprocessing and ARM assembly cover29 changed maps. The integrated regression suite is broader mechanical evidence, not an economic playthrough.

The inherited Vs. Seeker is disabled and still contains Hoenn-specific rematch logic. Its delivered item cannot be cited as working training access until that separate repair is tested. Required follow-up includes real shop transactions, first and repeat practice battles, money loss and replenishment, zero-money recovery, Android controls and measured human preparation at each battle pace.
