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

- Original frequency target, now under review alongside ADR-0016: frequently adapt team composition for different major opponents.
- Repeated defeats are expected for the described returning player; mastering major bosses is a central part of the game.
- The original direction did not target keeping one reasonably balanced favourite six unchanged. The later continuity incentive in ADR-0016 requires reviewing the expected amount of rotation; no permanent six or specific retained core has been selected.

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

- Training activities, experience sharing and resource access within ADR-0006's levelling-time target.
- Revised rotation frequency and retained-core expectations alongside ADR-0016; IV improvement, battle-training and move-relearning implementation. Fixed acquired natures are selected.
- Scouting implementation and information beyond ADR-0007's accepted fields.
- Retry location, healing, penalty amounts and item rules beyond ADR-0008.
- AI information rules and predictability.
- How the opening teaches required concepts and how ordinary trainers differ from bosses.

## Follow-up

Continue the interview to resolve detailed training, scouting, item and retry systems. Resolve battle rules and AI before specifying boss teams. This refines the challenge requirement in ADR-0001 without changing its other constraints.

## Clarification — 2026-09-05, design interview round 2

The user selected substantial levelling investment ([ADR-0006](0006-substantial-levelling-investment.md)), full boss scouting before the first attempt ([ADR-0007](0007-full-boss-scouting.md)), and money/item loss costs with preselected limited battle consumables ([ADR-0008](0008-consumable-loadouts-and-loss-costs.md)). Three consumables remains an example. These refine the existing direction; implementation has not started.

## Clarification — 2026-09-05, continuity alongside adaptation

The user wants using an individual Pokémon, especially in gym battles, to yield meaningful development beyond levels. This should reward continuity without excessively punishing frequent replacements, and the user explicitly noted that it touches the earlier rotation decision. [ADR-0016](0016-individual-traits-and-battle-training.md) records these requirements and the unresolved progression design.

The original frequency target is therefore under review, not a mandate for high roster turnover. Preserve meaningful adaptation and demanding bosses while evaluating veteran rewards and recruit catch-up. The user has not yet chosen a replacement frequency, fixed core size or permanent team; do not silently adopt one. This clarification does not revoke the repeated-defeat/mastery target.

The user subsequently proposed gym rematches as a way for recruits to earn veterans' rewards. ADR-0016 records this candidate; it supports evaluating continuity and adaptation together without yet choosing a new rotation frequency.
