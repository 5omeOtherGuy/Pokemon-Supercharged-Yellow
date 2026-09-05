# ADR-0011: Curate moves from any generation for the original 151

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answer in design interview round 6
- Implementation: Not started — move-selection policy only
- Supersedes: None
- Superseded by: None

## Context

[ADR-0004](0004-original-151-roster.md) restricts main-campaign species to the original 151. The interview separately asked which generations may supply moves for their learnsets.

## Decision

Moves from any generation may be considered for the original 151 when they fit Pokémon identity and campaign balance. Curate inclusion and distribution deliberately.

This is permission to select appropriate moves, not a requirement to import every move, every modern learnset or unrelated mechanics. It does not establish a particular generation's version of a move, approve custom moves, or decide whether a species may receive a move it has never learned officially.

## Alternatives considered

Restrict the move pool to Generation 3 or earlier, or to Generation 1 only with balance adjustments. The user selected cross-generation curation.

## Consequences

Evaluate each move together with its user, role, acquisition method, level and availability relative to bosses. Check impact in both singles and doubles under [ADR-0010](0010-mixed-battles-and-set-rules.md).

Source-foundation evaluation must account for implementing selected later-generation moves, including effects, descriptions, animations, AI handling and tests. Documentation must identify the actual implemented behavior rather than assume the latest official version.

This leaves species eligibility unchanged and does not approve new abilities, typings, alternate forms or generation-specific transformation systems.

## Evidence and validation

User answer on 2026-09-05: "Use carefully selected moves from any generation when they fit identity and balance."

No moves have been selected or implemented. Future validation should cover mechanics, learnset reachability, availability timing, AI understanding and strategic impact in both formats.

## Open questions

- Specific moves and versions of their effects.
- Permission to grant species moves outside their official learnsets.
- Custom moves and changes to existing move values.
- Move acquisition, relearning and TM/tutor policies.
- Ability, typing and alternate-form policies.

## Follow-up

Set remaining modernisation boundaries, then audit move support in candidate foundations and curate learnsets against the campaign progression.
