# ADR-0017: Encourage a trusted core with rotating specialists and retain boss mastery

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user choice after discussing veteran rewards and gym rematches
- Implementation: Not started — revised experience target only
- Supersedes: [ADR-0005](0005-team-rotation-and-boss-mastery.md)
- Superseded by: None

## Context

ADR-0005 originally targeted frequent team rebuilding and repeated attempts to master major bosses. The user subsequently requested development rewards for continued use, proposed gym rematches for recruits, and selected a trusted core with rotating specialists.

## Decision

- Encourage keeping a trusted core and rotating specialists for different major opponents.
- Reward attachment and development while preserving meaningful reasons to adapt.
- Retain the expectation of repeated defeats for returning players who know type matchups but have limited competitive experience. Mastering major bosses remains central, assuming a reasonably prepared team at the applicable cap.
- Do not enforce a fixed core size, predetermined replacements, scripted losses or one mandatory solution. Skilled play or an effective team can still win without changes or retries.

This replaces ADR-0005 in full and explicitly carries forward its boss-mastery target. The single difficulty setting and intended challenge remain unchanged.

## Alternatives considered

- Frequently rebuild much of the team with recoverable training rewards: the earlier turnover direction, not the revised target.
- Usually retain the same six and adapt mainly moves and tactics: not selected.
- Keep a trusted core and rotate specialists: selected.

## Consequences

Design varied strategic demands while allowing continuing members to contribute. Frequent wholesale rebuilding is no longer the default balance assumption.

[ADR-0016](0016-individual-traits-and-battle-training.md) owns individual traits, development goals and progression-appropriate rematches. Recruits share badge-wide training ceilings under [ADR-0023](0023-badge-wide-training-ceilings.md), while earning their own points and the personal capabilities in [ADR-0019](0019-gym-resistances-and-capabilities.md). [ADR-0021](0021-trainer-builds-and-reward-ownership.md) governs trainer builds and selective opponent progression.

The levelling target in [ADR-0006](0006-substantial-levelling-investment.md) is unchanged. Measure total preparation and rematch effort so specialist rotation remains practical. Existing scouting, information, consumable and format rules remain applicable; this target does not authorise hidden AI advantages.

## Evidence and validation

User answer on 2026-09-05: "Keep a trusted core and rotate specialists for different bosses." The earlier repeated-defeat/mastery answer has not been withdrawn.

No encounters or training systems have been implemented or playtested. Evaluate several plausible cores with different specialists at relevant stages. Check continuing-member contributions, advantages from replacements and total catch-up effort. Separate roster problems, knowledge gaps, execution, resource access and randomness as causes of defeat.

## Open questions

- Representative core sizes and rotation patterns for testing, without enforcing a fixed composition.
- Exact gym rewards, training limits, rematch access and total preparation burden.
- Detailed encounters, cap values and numerical difficulty targets.

## Follow-up

Use this target for encounter design and the training-system prototype. Preserve ADR-0005 as history and direct current planning to this record.

## Decision history — 2026-09-05

ADR-0022 subsequently made training permanent with flexible future focus; ADR-0023 then removed individual gym catch-up for ceiling access. Neither changed the trusted-core target. Useful specialists need not match a veteran's complete development. Editorial audit: consolidated these follow-ups into current links.
