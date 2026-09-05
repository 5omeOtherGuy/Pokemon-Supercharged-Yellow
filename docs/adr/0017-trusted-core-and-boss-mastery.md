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

[ADR-0016](0016-individual-traits-and-battle-training.md) defines traits, development goals and accepted gym rules. Progression-appropriate rematches let recruits earn the same gym rewards as veterans. [ADR-0022](0022-permanent-training-and-flexible-focus.md) selects automatic focus-based allocation and eight distinct leader rewards. [ADR-0019](0019-gym-resistances-and-capabilities.md) retains extra gym capacity alongside modest learned Pokémon passives, using three capacity slots per Pokémon and one-, two- or three-slot capability costs. Repeatable Elite Four and other notable encounters may also grant capabilities. Numerical balance and remaining progression details stay open; ADR-0021 accepts trainer-wide badge scope and selective opponent capability use consistent with trainer experience.

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

## Clarification — 2026-09-05, permanent development

ADR-0022 allows flexible focus for future gains without reallocating existing training. Relevant-stat completion is the normal late-game target; all-stat completion must remain exceptional. Preserve meaningful rotating specialists without requiring every recruit to match a veteran's complete development.
