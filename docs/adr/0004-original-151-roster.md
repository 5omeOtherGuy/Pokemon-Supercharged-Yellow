# ADR-0004: Restrict the main campaign roster to the original 151

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers in design interview rounds 1 and 7 and subsequent complete-collection/no-trading clarification
- Implementation: Not started — design decision only
- Supersedes: None
- Superseded by: None

## Context

[ADR-0001](0001-project-direction.md) left roster boundaries open. The interview asked which Pokémon should be eligible for the main campaign.

## Decision

Restrict main-campaign species eligibility to the original 151 Pokémon. Later evolutions, pre-evolutions and unrelated species outside that set are excluded from the main campaign, including opposing trainer rosters.

Use original forms only in the main campaign. Regional variants and Mega Evolutions are excluded, including opposing trainer teams. Ability and typing changes permitted by [ADR-0012](0012-ability-and-typing-rebalance.md) apply to these original forms; they do not require adding alternate forms.

All original 151 must be obtainable in one playthrough on one save, including the other starters, alternate evolution branches and legendary/mythical Pokémon. There is no trading, including player-to-player and NPC trades; acquisition must be self-contained. Replace any trade-dependent evolution or acquisition requirement with a suitable solo route.

Complete collection is required, but not necessarily before the League or by catching every species in the wild. Acquisition methods and timing remain for design. [ADR-0026](0026-yellow-kanto-content-baseline.md) selects Pikachu as starter and Mew under the truck. This does not expand the roster beyond the original 151 or select a mechanics generation.

## Alternatives considered

- Original 151 plus later evolutions and pre-evolutions.
- Mostly Kanto with a small curated selection of unrelated Pokémon.

The user selected the strict original-151 option.

## Consequences

Balance must work within this species pool. We cannot solve a campaign design problem by adding an out-of-roster evolution or counter. Pokémon whose later evolutions are excluded need to be assessed in their campaign roles without assuming equal power is required.

Any source foundation with a larger database will need encounter, evolution, gift and trainer access audited; engine database support alone does not make a species campaign-eligible.

Build a species-by-species acquisition plan. Check that starter, fossil and branching-evolution choices leave enough obtainable Pokémon or alternate sources to collect every species in the same run. Remove dependencies on another game, another save, external events or trading. Former NPC-trade rewards need another acquisition path; their replacement scenes and evolution methods are not selected here.

## Evidence and validation

User answer, 2026-09-05: "Strictly the original 151", in response to the main-campaign roster question. No game implementation or runtime checks have occurred.

Future validation should audit all campaign acquisition and opponent-team paths against the agreed species pool, including any scripts, items or mechanics that could expose excluded regional variants or Mega Evolutions.

Also verify that all 151 acquisition paths can coexist in one save through the planned campaign/post-League content. Cover alternate branches, required items, story-state access and recovery from missed or failed unique encounters. No acquisition plan or playable completion route has been verified.

## Open questions

- Other battle-system mechanics, if proposed, need separate decisions; regional variants and Mega Evolution are excluded from the main campaign.
- Any future expansion beyond the original-151 roster; none is selected by the complete-collection requirement.
- Specific encounter distribution, solo evolution methods, other-starter acquisition and legendary timing; their attainability is required, and ADR-0026 selects the Pikachu start and Mew location.
- Repeat/recovery behavior for missed or failed unique encounters, and which species become obtainable after the League.

## Follow-up

Resolve campaign availability before encounter design; the original-form boundary is now settled. Use this constraint during the source-foundation audit. The roster question in ADR-0001 is resolved for main-campaign species eligibility.

## Clarification — 2026-09-05, design interview round 7

The user selected: original forms only, with no regional variants or Mega Evolutions in the main campaign. This resolves the previously open form choice. The species restriction and open postgame scope remain unchanged; ordinary evolution among eligible original species is not removed by this clarification. Verify all player-access and trainer paths against the form boundary when an engine exists.

## Clarification — 2026-09-05, Yellow content baseline

[ADR-0026](0026-yellow-kanto-content-baseline.md) selects Pikachu as starter and permits wild encounter changes while preferring fidelity to Yellow where practical. It prioritises Kanto polish with no current Sevii Islands plan. At that clarification, other starter acquisition and future postgame roster remained open.

## Clarification — 2026-09-05, complete solo collection

The user stated: "There are no obedience restructions and no trading. All pokemon must be attainable within one playthrough. MEW IS UNDER TRUCK!" In the established original-151 scope, this requires all 151 on one save without trading; it does not add later-generation species. ADR-0021 records removal of obedience restrictions and ADR-0026 owns Mew's location. The prior eligibility-only policy is extended to require complete collection; exact acquisition methods and timing remain open.
