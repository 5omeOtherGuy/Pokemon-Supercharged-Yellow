# ADR-0009: Use VGC-style opponent information for boss AI

- Status: Accepted
- Date: 2026-09-05
- Decision authority: User selected official VGC tournament information in round 4 and mutual consumable visibility in round 5; documented adaptation below
- Implementation: Not started — information policy only
- Supersedes: None
- Superseded by: None

## Context

The user answered "Same as in official vcg turnaments" when asked what a boss should know about the player's team before turn one. The recorded adaptation uses the official open-team-list standard for live VGC events, checked against the dated handbook below. This answer concerns information access, not a choice of doubles, team size, level scaling or generation mechanics.

## Decision

Use an open-team-list information boundary for boss AI.

- Disclose the player's battle-team species/relevant forms, known moves, abilities and held items before the first turn.
- Include the Nature/stat-alignment field in the recorded mapping. [ADR-0016](0016-individual-traits-and-battle-training.md) subsequently selected fixed acquired natures and stat effects; the field representation still needs an explicit engine mapping.
- Do not directly disclose the player's exact numerical stats or underlying IV/EV/stat allocations.
- The AI may reason from permitted team information and observable battle events. Estimates and deductions must remain distinguishable from direct hidden-data access.
- As the engineering translation of a human opponent's information boundary, do not let the AI read the player's committed action, switch target or item choice for the current turn before choosing its own action, or inspect future random outcomes.
- As an explicitly accepted campaign extension, both sides see the other's complete selected bag consumables and quantities before battle. This does not reveal which consumable, target or other action the opponent has chosen for the current turn.
- The subsequently accepted [ADR-0024](0024-passive-scouting-and-ai-observations.md) adds reciprocal active trainer-passive and assigned-capability disclosure as a project extension, retaining the hidden-information limits above.

This is the recorded 2026-09-05 interpretation, not automatic adoption of future rulebook updates. Additional public-information fields must be mapped explicitly before implementation.

## Alternatives considered

Knowing only the active Pokémon and learning during battle, species-only team preview, and unrestricted access to the player's internal state. The user instead specified the official tournament standard.

## Consequences

AI strength must come from decision quality within permitted information. Source engines may expose more opponent data than this policy permits; audit and constrain those paths. The AI may fully inspect its own team state.

This decision does not settle singles/doubles, bring-four rules, Mega Evolution, Terastallization, training systems, battle timers or item bans. Existing campaign requirements remain in force.

## Evidence and validation

The user requested official VGC information; the exclusions of pending commands and future RNG are documented engineering translations of a human opponent's boundary. Mutual bag-loadout visibility was separately selected in round 5.

The [official VGC Tournament Handbook](https://mcdn.pokemon.com/pokemon-prod/raw/upload/v1/live/static-assets/content-assets/cms2/pdf/play-pokemon/rules/play-pokemon-vgc-tournament-handbook-en.pdf), revision **2026-05-21**, was retrieved and its team-list section checked in full on 2026-09-05 (sections **2.5–2.5.1**, pages 7–8; page 8 also visually inspected). It specifies open lists containing species/battle-relevant forms, ability, held item, moves and stat alignment; numerical stats are withheld from the opponent. The Scarlet/Violet-specific Tera field does not select that mechanic for this project.

Retrieved PDF SHA-256: `152ff4de657ee0f001bd8fd5f1290272782798836701ed72c4b311c974404e24`. The URL can change; use the revision and hash to identify the inspected copy. The handbook supports disclosure of stat alignment; mapping that term to this project's fixed nature effects remains the recorded adaptation, not an imported Champions ruleset.

No AI code has been implemented or tested. Future verification must hold permitted observations and the AI's own decision randomness constant while varying hidden opponent data, pending commands and future battle RNG; evaluation must remain unchanged.

## Open questions

- Exact nature/stat-alignment representation, remaining public fields such as level/form details, and reciprocal additions to boss scouting. Player access to boss natures and exact stats remains open under ADR-0007.
- Observable HP precision, stat inference, memory across attempts and ordinary-trainer AI.
- Loadout locking, choice commitment and information filtering in the selected engine.
- Engine mapping of the active-passive fields accepted in ADR-0024.

## Follow-up

Map each accepted field into an explicit information contract against a verified engine. Preserve uncertainty for estimates and audit all evaluation helpers for hidden-data access.

## Decision history — 2026-09-05

Round 4 requested the VGC standard; [eccdd70](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/eccdd70) recorded the adaptation. Round 5 explicitly added mutual consumable visibility as a campaign extension, not an official VGC rule.

The later tentative suggestion that AI might need all information for reliability did not reverse hidden-stat or pending-action protections. ADR-0024 initially proposed active-effect disclosure and an implementation approach; the user explicitly accepted reciprocal disclosure on 2026-09-05, preserving those protections.

Editorial audit: replaced incomplete indexed-PDF evidence with the accessible official handbook and corrected the earlier 2.4–2.4.1 section citation to 2.5–2.5.1. The original [team-list form](https://www.pokemon.com/static-assets/content-assets/cms2/pdf/play-pokemon/rules/play-pokemon-vg-team-list.pdf) remains a historical reference; its full body was not verified and is no longer needed as proof of the listed fields. No accepted information boundary changed.
