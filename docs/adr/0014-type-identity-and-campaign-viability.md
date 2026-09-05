# ADR-0014: Give each type distinct identity and meaningful campaign viability

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user clarification during the balance-design interview
- Implementation: Not started — design principle and interests recorded
- Supersedes: None
- Superseded by: None

## Context

The initial direction rejects equal strength and universal species viability. After discussing Fairy's identity value, the user identified familiar Pokémon they like and clarified the intended level of balance: each type should feel distinct and playable, while every Pokémon need not be.

## Decision

- Give each type a recognisable identity and meaningful opportunities to contribute in the campaign.
- Do not require every species to be equally powerful, universally useful or viable throughout the campaign.
- Extend the useful campaign life of Bug Pokémon. Address the user's similar concern about Flying Pokémon such as Pidgeot feeling underwhelming late in the game.
- Preserve meaningful strengths, weaknesses and differences between species while pursuing these goals.

The type-level goal does not specify equal representation, success in every boss battle, or completion with a monotype team. There is no representation quota, including for types without an official original-form member in the original 151 such as Dark. Add typings only when the individual Pokémon's identity and balance justify them. Do not silently relax [ADR-0004](0004-original-151-roster.md).

## Player interests

The user named:
- Onix and the Geodude line.
- Ghost and Psychic Pokémon.
- Eevee and its evolutions.
- Tentacool.
- Bug Pokémon, with an explicit desire for longer viability.
- Flying Pokémon, including Pidgeot, with concern about late-game performance.

These are interests and evaluation priorities, not a finding that all listed Pokémon are weak or a promise to buff each one. Eevee's eligible evolutions remain restricted by the original-151 rule. No individual build, stat adjustment or new evolution is approved here.

## Alternatives considered

- Ensure every species remains comparably strong through the League: inconsistent with the user's stated distinction between type and species viability.
- Allow entire types to lose meaningful relevance after the early game: inconsistent with the desired type-level playability and longer Bug/Flying usefulness.
- Improve species solely through higher numbers: insufficient to establish distinct identity, with the permissible scope of base-stat changes now defined in ADR-0015.

## Consequences

Evaluate roles at their acquisition and evolution stages and at later campaign caps. A type may support several strategies; identity should not reduce all its Pokémon to one job.

When reviewing a weak option, assess move access, abilities, typing, encounter timing and the available opposition together. [ADR-0015](0015-standard-chart-and-stat-rebalance.md) selects the standard modern chart and primarily modest base-stat adjustments, with larger changes reserved for special or necessary cases.

Use the revised goal of a trusted core with rotating specialists and difficult bosses in [ADR-0017](0017-trusted-core-and-boss-mastery.md), which supersedes the earlier frequent-rebuilding target. Longer viability means meaningful reasons to bring selected Pokémon back for later encounters, without promising one permanent team can answer every challenge.

## Evidence and validation

Source: user clarification on 2026-09-05. The user explicitly stated that each type should feel distinct and playable, but every Pokémon need not be. The named interests and Bug/Flying concerns are reported player experience, not verified performance data.

No engine or balance results exist. Future review should identify intended roles, obtainable tools, retained weaknesses and plausible uses across campaign stages and both battle formats. Include ordinary player teams in testing, check whether Bug and Flying options retain useful later roles, and ask playtesters whether those roles are understandable and satisfying. Do not substitute raw win rate or type counts for that assessment.

## Open questions

- Specific species roles, access timing and adjustments.
- How many later encounters establish sufficient continued usefulness, assessed through balance testing.

## Follow-up

Apply the representation, chart and stat-change boundaries in ADR-0015. Use these interests to help choose representative Pokémon and encounters for the first balance prototype. This does not make the list an exclusive priority or select the prototype's scope.

## Decision history — 2026-09-05

The user subsequently selected no representation quota, the standard chart and primarily modest stat adjustments with justified exceptions. [ADR-0015](0015-standard-chart-and-stat-rebalance.md) owns these boundaries. Editorial audit: replaced the duplicated clarification; type-level playability remains a goal without requiring an obtainable species of every type.
