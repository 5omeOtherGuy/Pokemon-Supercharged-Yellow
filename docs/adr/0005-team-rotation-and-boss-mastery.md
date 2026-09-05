# ADR-0005: Build the campaign around team rotation and boss mastery

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers in design interview round 1
- Implementation: Not started — design targets only
- Supersedes: None
- Superseded by: None

## Context

The original brief called for significantly greater challenge and depth. The interview clarified team attachment and the expected experience of a returning player who knows type matchups but has little competitive experience.

The difficulty question assumed the player was at the level cap with a reasonably prepared team.

## Decision

- Frequently adapting team composition for different major opponents is an intended part of the campaign.
- Repeated defeats are expected for the described returning player; mastering major bosses is a central part of the game.
- Keeping one reasonably balanced favourite six unchanged is not the primary balance target.

These are experience targets. They do not impose forced team replacement, a required number of losses or a scripted solution. An effective team or skilled player may still win without replacing members or retrying.

## Alternatives considered

For team composition:
- Rare changes, relying mainly on moves and tactics.
- A stable core with occasional specialist rotation.

For difficulty:
- Most bosses defeated first try, with a few retries for the hardest.
- Several bosses needing a few attempts.

The user selected frequent team rebuilding and repeated boss defeats as the intended experience.

## Consequences

Encounter variety and the available Pokémon pool need to support meaningful composition changes. Preparation effort, scouting, retry logistics and learning feedback become consequential design choices.

Balance evaluation must look beyond whether one standard team can clear every boss. It should consider different available teams, the reasons attempts fail, and whether adaptation changes outcomes. Difficulty for the intended returning player is distinct from difficulty for an expert.

The user's answers do not authorise hidden AI advantages, luck-dependent wins, mandatory external calculators, excessive grinding or a single required counter. Policies in these areas remain to be decided.

## Evidence and validation

User answers, 2026-09-05:
- "Frequently: build different teams for different major opponents."
- "Repeated defeats are expected; mastering each major boss is a central part of the game."

No encounters have been implemented or playtested. Future playtests should distinguish knowledge gaps, team composition, execution, resource access and randomness as causes of losses. Numerical retry targets remain unchosen.

## Open questions

- Time and resources needed to prepare a replacement Pokémon.
- IV/EV/nature and move-relearning systems.
- Boss information available before the first attempt and after defeat.
- Retry location, healing, consumed-item restoration and loss penalties.
- AI information rules and predictability.
- How the opening teaches required concepts and how ordinary trainers differ from bosses.

## Follow-up

Use the next interview round to clarify preparation, scouting and retries. Resolve battle rules and AI before specifying boss teams. This refines the challenge requirement in ADR-0001 without changing its other constraints.
