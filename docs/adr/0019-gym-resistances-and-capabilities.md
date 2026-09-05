# ADR-0019: Explore gym-earned resistances or capabilities

- Status: Proposed
- Date: 2026-09-05
- Decision authority: User reopened gym reward form and suggested partial type resistance or a unique capability; no replacement selected yet
- Implementation: Not started — design exploration only
- Supersedes: None
- Superseded by: None

## Context

[ADR-0018](0018-gym-training-budget-and-focus.md) selected a limited extra gym training budget. When asked how that capacity should fill, the user suggested a different reward: a small partial resistance to the gym's type or a unique ability/capability.

The extra-budget choice is now under review. Do not implement a gym reward formula until this discussion resolves whether these effects replace or supplement that budget. Automatic focus allocation and the eight distinct leader rewards remain selected; their detailed relationship to new reward effects is open.

## Proposal and alternatives

1. Small type-damage reductions associated with each gym.
   - Simple to explain and applicable to many Pokémon.
   - Eight always-active rewards may create broad veteran protection and increase recruit catch-up pressure.
   - If implemented as a separate damage modifier, distinguish it from the standard chart and specify ordering, rounding, stacking and displayed information. This technical possibility is not approval of a chart exception.
2. Distinct situational defensive or utility capabilities.
   - Could add tactical options and communicate learned expertise.
   - Adds interactions beside moves, held items and innate abilities; requires clear scope and descriptions.
   - An illustrative candidate is preventing a specified status condition once per battle. This is not an accepted effect or assigned gym reward.
3. A mixture by gym.
   - Allows varied lessons but increases the explanation and balance burden.

The agent's preferred candidate is a small repertoire of gym-earned capabilities with a limited active selection, initially considering one active reward per Pokémon. This is a proposal, not an accepted slot, equip system or numerical limit. Earning all eight would then complete the repertoire rather than apply eight concurrent effects. The user must decide whether this interpretation matches the intended veteran reward.

## Existing boundaries

- Gym rewards require a player victory, field participation and no fainting during that battle.
- All eight leaders contribute distinct rewards; rematches use stronger teams suited to current campaign progress and let recruits recover the same rewards as veterans.
- A trusted core with rotating specialists remains the team-continuity target.
- The standard modern type chart remains selected. A type-damage modifier or chart departure requires an explicit decision; neither is approved here.
- ADR-0012 currently permits reassignment of existing official abilities, not newly invented innate abilities. A new gym-capability system needs an explicit scope decision and reconciliation with that rule. Do not silently grant extra innate abilities.
- Battle-relevant capabilities need explicit scouting and AI information mapping. Existing hidden-stat protections remain applicable.
- Focus-changing restrictions do not automatically determine how gym capabilities could be selected or changed.

## Tradeoffs to resolve

- Replacement for the extra gym training budget versus a supplement.
- All learned rewards active together versus one or a limited selection.
- Simple resistance, situational capabilities or a mixture.
- How broadly usable each reward is across species and singles/doubles.
- Magnitude, interaction with existing effects and whether full reward collection becomes effectively compulsory.

The user proposed rewards beyond point allocation but has not selected between these alternatives. The earlier extra-capacity decision is retained as history, not silently replaced by an agent preference.

## Evidence and validation

Source: user response on 2026-09-05 suggesting a small partial type resistance or unique ability/capability instead of simply allocating points. No effects, source implementation or gameplay results exist.

Before acceptance, define the chosen effect model, concurrent reward limit and relationship to the prior budget. Later validation should compare otherwise equivalent veterans and recruits, check both formats, and assess whether rewards enrich identity and decisions while retaining weaknesses. Verify eligibility, duplicate reward handling, accessible rematches, UI and AI understanding once implemented.

## Follow-up

Resolve the three product choices above before drafting individual gym rewards or tuning percentages. If the selected model replaces the accepted extra-budget decision, record the supersession explicitly while carrying forward unaffected focus and training policies.
