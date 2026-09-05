# ADR-0009: Use VGC-style opponent information for boss AI

- Status: Accepted
- Date: 2026-09-05
- Decision authority: User selected official VGC tournament information in round 4 and mutual consumable visibility in round 5; documented adaptation below
- Implementation: Not started — information policy only
- Supersedes: None
- Superseded by: None

## Context

The user answered "Same as in official vcg turnaments" when asked what a boss should know about the player's team before turn one. We interpret this as the current official open-team-list standard for live VGC events. This answer concerns information access, not a choice of doubles, team size, level scaling or generation mechanics.

## Decision

Use an open-team-list information boundary for boss AI.

- Disclose the player's battle-team species/relevant forms, known moves, abilities and held items before the first turn.
- Include the Nature/stat-alignment field in the current-standard mapping, if the game uses that mechanic; its implementation has not been chosen.
- Do not directly disclose the player's exact numerical stats or underlying IV/EV/stat allocations.
- The AI may reason from permitted team information and observable battle events. Estimates and deductions must remain distinguishable from direct hidden-data access.
- As the engineering translation of a human opponent's information boundary, do not let the AI read the player's committed action, switch target or item choice for the current turn before choosing its own action, or inspect future random outcomes.
- As an explicitly accepted campaign extension, both sides see the other's complete selected bag consumables and quantities before battle. This does not reveal which consumable, target or other action the opponent has chosen for the current turn.

This is the recorded 2026-09-05 interpretation, not automatic adoption of future rulebook updates. Additional public-information fields must be mapped explicitly before implementation.

## Alternatives considered

Knowing only the active Pokémon and learning during battle, species-only team preview, and unrestricted access to the player's internal state. The user instead specified the official tournament standard.

## Consequences

AI strength must come from decision quality within permitted information. Source engines may expose more opponent data than this policy permits; audit and constrain those paths. The AI may fully inspect its own team state.

This decision does not settle singles/doubles, bring-four rules, Mega Evolution, Terastallization, training systems, battle timers or item bans. Existing campaign requirements remain in force.

## Evidence and validation

Primary references checked on 2026-09-05:
- [Official VGC Tournament Handbook, sections 2.4–2.4.1](https://www.pokemon.com/static-assets/content-assets/cms2/pdf/play-pokemon/rules/play-pokemon-vgc-tournament-handbook-en.pdf): indexed official text specifies open team lists and excludes numerical stats from opponent disclosure.
- [Official Video Game Team List](https://www.pokemon.com/static-assets/content-assets/cms2/pdf/play-pokemon/rules/play-pokemon-vg-team-list.pdf): indexed official fields include Stat Alignment, Ability, Held Item and moves.

The full PDF bodies were not exposed by the web reader; the statements above rely on indexed official passages. A complete field-by-field check of an accessible official copy remains an implementation prerequisite. The Nature/stat-alignment mapping follows the listed field and the handbook's numerical-stat exception; it must not be confused with revealing a stat spread.

No engine or AI code has been implemented or tested. Future checks should demonstrate that hidden opponent data and the player's pending command cannot affect AI evaluation when permitted observations are identical.

## Open questions

- Selection-lock timing and keeping the visible consumable loadout synchronised with actual available supplies.
- Remaining public fields, including level/form details, and reciprocal additions to boss scouting.
- Observable HP precision, stat inference, memory across attempts and ordinary-trainer AI.
- Choice commitment and information filtering in the selected engine.

## Follow-up

Implement the accepted custom consumable-information extension within an explicit field-level information contract against a verified upstream engine. Preserve uncertainty for inferred opponent statistics.

## Clarification — 2026-09-05, design interview round 5

The user confirmed: both sides see complete consumable loadouts. This is a campaign-specific extension to the VGC reference, recorded jointly with [ADR-0008](0008-consumable-loadouts-and-loss-costs.md), not a claim that official VGC includes bag supplies. Hidden statistics and pending actions remain protected.
