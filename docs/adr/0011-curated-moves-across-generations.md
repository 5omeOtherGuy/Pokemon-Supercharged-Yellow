# ADR-0011: Curate moves from any generation for the original 151

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answer in design interview round 6 and subsequent weather/terrain and HM/TM clarifications
- Implementation: Not started — move-selection policy only
- Supersedes: None
- Superseded by: None

## Context

[ADR-0004](0004-original-151-roster.md) restricts main-campaign species to the original 151. The interview separately asked which generations may supply moves for their learnsets.

## Decision

Moves from any generation may be considered for the original 151 when they fit Pokémon identity and campaign balance. Curate inclusion and distribution deliberately.

Include weather and terrain moves to broaden strategic options, implementing the required weather/terrain mechanics where the chosen foundation does not already provide them. Curate the specific moves, effect versions and learnset distribution under the same identity and campaign-balance policy.

Cross-generation curation does not require importing every move, every modern learnset or unrelated mechanics. It does not establish a particular generation's version of a move, approve custom moves, or decide whether a species may receive a move it has never learned officially.

HM moves must be forgettable; HM status must not make a learned move permanent. TMs must be reusable: teaching a compatible Pokémon must not consume the TM, so an acquired TM can be used again. Acquisition and species compatibility remain separate policies.

## Alternatives considered

Restrict the move pool to Generation 3 or earlier, or to Generation 1 only with balance adjustments. The user selected cross-generation curation.

## Consequences

Evaluate each move together with its user, role, acquisition method, level and availability relative to bosses. Check impact in both singles and doubles under [ADR-0010](0010-mixed-battles-and-set-rules.md).

Source-foundation evaluation must account for implementing selected later-generation moves, including effects, descriptions, animations, AI handling and tests. Documentation must identify the actual implemented behavior rather than assume the latest official version.

For weather and terrain, verify activation, duration, replacement and coexistence rules, affected targets, move/ability/item interactions, field-state presentation and AI evaluation in both battle formats. Audit inherited support before adding or changing it; a foundation's feature list alone does not establish that these interactions work.

This leaves species eligibility unchanged and does not approve new abilities, typings, alternate forms or generation-specific transformation systems.

Forgetting an HM must not strand the player or make required campaign progress impossible. Verify a reachable way to restore a required field move or another valid traversal path, including after save/load and party changes. Reusable TMs shift availability control to acquisition timing and learnset eligibility. These requirements do not remove badge checks for field use or select replacement field tools.

## Evidence and validation

User answer on 2026-09-05: "Use carefully selected moves from any generation when they fit identity and balance."

Weather and terrain move categories are explicitly required; individual moves and learnsets remain unselected. No moves have been implemented. Future validation should cover mechanics, learnset reachability, availability timing, AI understanding and strategic impact in both formats.

## Open questions

- Specific moves and versions of their effects, including weather/terrain rules and campaign availability.
- Permission to grant species moves outside their official learnsets.
- Custom moves and changes to existing move values.
- Move acquisition and relearning, HM-forgetting interface, TM distribution/compatibility and tutor policies. HM forgettability and TM reuse are decided.

## Follow-up

Resolve remaining move-specific boundaries as needed, audit move support in candidate foundations and curate learnsets against the campaign progression.

## Related decisions and editorial note — 2026-09-05

[ADR-0012](0012-ability-and-typing-rebalance.md) owns the later ability/typing permissions; [ADR-0004](0004-original-151-roster.md) owns the original-form boundary. These supplement move curation without approving a specific move assignment. The audit replaced the duplicated round-7 clarification with these references.

## Clarification — 2026-09-05, weather and terrain

The user explicitly requested integrating weather and terrain moves if absent, to make the move pool more interesting. This makes their inclusion a requirement within the existing cross-generation policy. It does not select every such move, a mechanics generation, particular species assignments or automatic weather/terrain on routes and boss arenas.

## Clarification — 2026-09-05, HM/TM usability

The user explicitly stated: "HMs must be forgettable, TMs must be reusable." This resolves those usability policies without selecting a move-deletion service, relearning cost, TM locations or universal TM compatibility. Verify forgetting/replacement, safe restoration of required field access, repeat TM teaching and item persistence when an engine exists. No HM/TM behavior has been implemented or tested.
