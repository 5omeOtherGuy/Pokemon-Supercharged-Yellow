# ADR-0006: Preserve substantial levelling investment

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers in design interview round 2; subsequent ordinary-training pacing clarification
- Implementation: Not started — design policy only
- Supersedes: None
- Superseded by: None

## Context

The interview asked how long a newly caught Pokémon should normally take to reach the current cap, considering levelling only. The original frequent-rebuilding target in ADR-0005 has since been replaced by a trusted core with rotating specialists in [ADR-0017](0017-trusted-core-and-boss-mastery.md).

## Decision

Target substantial levelling investment: roughly 20–30 minutes or more under typical relevant conditions. This is a pacing target, not an enforced minimum timer for every catch or campaign stage. Do not introduce universal instant catch-up as a default QoL feature.

Ordinary stat training should usually require additional focused battles after levelling, as subsequently selected and retained in [ADR-0023](0023-badge-wide-training-ceilings.md). Its extra duration is unchosen. The levelling target does not approve equally long chores for every preparation system.

## Alternatives considered

Almost immediate catch-up, or 5–10 minutes of targeted training. The user selected substantial levelling investment.

## Consequences

Training several replacements can multiply preparation cost. Measure the complete path to a useful recruit: levelling, additional stat training, IV improvement, move access, capability rematches, service travel and supplies. Measure useful readiness separately from completing every possible reward.

ADR-0023 gives recruits the trainer's current training ceilings immediately; personal rematches recover capabilities under [ADR-0019](0019-gym-resistances-and-capabilities.md), not stat capacity. Focus changes direct future gains without relocating existing points. EXP-related trainer passives in [ADR-0021](0021-trainer-builds-and-reward-ownership.md) must be evaluated against the unchanged levelling target.

## Evidence and validation

User answer on 2026-09-05: "Substantial training: roughly 20–30 minutes or more." The additional post-levelling training preference was recorded in [commit 863a7e7](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/863a7e7).

Future timing checks must record starting level, cap, campaign stage, party size, activity, battle-speed setting and training modifiers. Compare one and several replacements, including failed rematches and replenishment. No implementation or timing validation has occurred.

## Open questions

- Training activities, access, ordinary EXP sharing and group-training time.
- Reference speed for the levelling target and stage-dependent exceptions, including near-cap catches.
- Additional stat-training duration and total useful-recruit preparation time.
- Overlap with IV improvement, capability acquisition and other preparation activities.

## Follow-up

Design training and resource access alongside encounters. Measure actual pacing before accepting numerical tuning.

## Decision history — 2026-09-05

The levelling target originated in interview round 2. Later discussion added the whole-preparation concern and selected additional ordinary training after levelling. ADR-0018's individual gym-budget catch-up and earned instant redistribution were later superseded through ADR-0022 by ADR-0023.

Editorial audit: removed those superseded rules from current pacing guidance. The [pre-audit clarifications](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/blob/66c17faf1292626ae5f0ec70531aaf1b2a871ec1/docs/adr/0006-substantial-levelling-investment.md) remain in Git; the levelling and additional-training choices are unchanged.
