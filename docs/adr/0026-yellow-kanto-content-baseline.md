# ADR-0026: Use FireRed/LeafGreen Kanto for the Yellow content baseline

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user clarification of Yellow identity, encounters, Pikachu starter, current content scope and Mew under the truck
- Implementation: Not started — content baseline only; no maps, scripts or encounters verified
- Supersedes: None
- Superseded by: None

## Context

The design review asked which aspects of Yellow must survive the rebalance. The user wants a familiar Yellow adventure built around FireRed/LeafGreen's Kanto map, adjusted where needed to fit Yellow, with the already accepted gameplay changes. Recharged Yellow is the concrete reference for the intended result.

## Decision

- Use FireRed/LeafGreen's Kanto map and layout as the baseline, with adjustments where necessary to fit Yellow. Preserve Yellow's main encounters and plot anchors, with the familiar Yellow/FireRed/LeafGreen look and feel.
- Use Recharged Yellow as the reference hack for that rendition of Kanto. Its features are not adopted wholesale; the project's accepted decisions define our mechanics and scope.
- Pikachu is the starter, as in Yellow.
- Mew is obtainable under the truck. This is an intentional project encounter; access, trigger and timing remain to be designed.
- Outside the main encounters and plot anchors, trainer content may change substantially: add trainers, replace trainers and change which routes they occupy.
- Wild Pokémon encounters may change, but prefer staying as close to Yellow's original distribution as practical within the accepted design goals. Specific encounter tables are not fixed by this decision.
- Other mainline versions of Kanto, including the user's suggested Generation II inspiration, may inform improvements. They do not replace the principal Yellow/FireRed/LeafGreen identity or automatically add their content.
- Prioritise polishing Kanto before adding extra content. No Sevii Islands postgame is planned within the current scope.
- Apply the already accepted mechanics, progression and balance changes to this content baseline. This does not select a FireRed/LeafGreen source dependency or reverse the GBA/NDS flexibility in [ADR-0025](0025-android-emulation-platform.md).

The main-campaign original-151/original-form boundary in [ADR-0004](0004-original-151-roster.md) remains in effect; its subsequent clarification requires all 151 to be obtainable in one playthrough without trading. Future additions beyond the current Kanto focus require a separate scope decision; this record does not ban Kanto-based post-League activity or remove accepted capability-rematch requirements.

## Alternatives considered

- A substantially redesigned Kanto map or replacement plot: inconsistent with the requested same-map, same-game and same-layout baseline.
- Preserve every original trainer and wild encounter unchanged: more restrictive than the user's explicit permission to rebalance encounter content.
- Use another generation's Kanto as the principal campaign: not selected; those versions are inspiration.
- Include Sevii Islands merely because a foundation contains them: outside the current Kanto-polish scope.

## Consequences

The foundation audit must include the cost of delivering FireRed/LeafGreen's Kanto with Yellow adaptations and a Pikachu start, not just battle features. Inventory maps, campaign progression, main encounters and Yellow-specific scripts/presentation against documented references before modifying content. Identify the Recharged Yellow version used for comparison; its source remains unnecessary under [ADR-0001](0001-project-direction.md).

Main encounters retain their campaign role and plot anchors while the accepted boss-rebalance policies apply. Fit the selected training, scouting, capability and service systems into the existing campaign; identify any necessary material departure before changing the baseline.

A candidate's bundled extras are not project requirements. Assess removal or isolation of out-of-scope content without breaking Kanto progression, saving or rematch access. No actual content or balance validation has occurred.

## Evidence and validation

User clarification on 2026-09-05 initially requested "same map, same game, same layout", Pikachu as starter and Kanto polish before extras, while permitting the encounter changes and inspiration above. The follow-up explicitly resolved the map reference: "Record FireRed/LeafGreen Kanto map with adjustments so it fits yellow if necessary. Recharged Yellow Rom Hack is basically what we are aiming for here."

These statements establish product intent, not verified game differences or Recharged Yellow mechanics. The author's [Ko-fi listing](https://ko-fi.com/s/7fec26b127), inspected on 2026-09-05, points to the [project page](https://jaizu.moe/games/recharged-yellow); that page could not be retrieved during this clarification. This is an unpinned reference, not source or gameplay validation.

Future checks should compare map/layout and plot-anchor fidelity, the Pikachu start, required system access and allowed encounter changes. Validate the rendition on Android under ADR-0025 and playtest whether it retains the intended feel.

Subsequent user decision, 2026-09-05: "MEW IS UNDER TRUCK!" This selects Mew's location for this project, not a claim about an official game's content. Verify that the truck encounter remains reachable within the complete solo-collection route in ADR-0004.

## Open questions

- Detailed reference inventory, necessary Yellow adaptations and exact asset/source revisions; Recharged Yellow comparison version.
- Specific trainer and wild-encounter changes, acquisition methods/timing for other starters and legendaries, and Mew's truck-access/encounter trigger. All original 151 must be obtainable under ADR-0004.
- First playable milestone and detailed Kanto post-League content. Extra-region and future postgame roster scope remain unselected.

## Follow-up

Use this baseline and ADR-0025 during [the foundation comparison](0003-source-foundation.md). The platform/content portion of review finding F5 is resolved sufficiently for that investigation; inherited badge effects remain a separate question in ADR-0021.
