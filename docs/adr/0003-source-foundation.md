# ADR-0003: Select a source foundation through a build audit

- Status: Proposed
- Date: 2026-09-05
- Decision authority: Assistant engineering proposal; final foundation selection not yet made
- Implementation: Not started — preliminary documentation research only
- Supersedes: None
- Superseded by: None

## Context

Recharged Yellow source is unavailable and not required. We need a maintainable source foundation that can deliver familiar Kanto, a physical/special split, level caps, adjustable battle speed and later balance changes.

A feature list is not evidence that the relevant implementation works or is suitable for reuse.

## Proposal

Propose auditing the FireRed source ecosystem first, including pret/pokefirered, Deokishisu/FRLG-Plus and egalleta/FRLG-2Plus. Compare the necessary effort with an Emerald expansion foundation if the FireRed candidates have material limitations.

[ADR-0025](0025-android-emulation-platform.md) subsequently requires Android emulator play and explicitly permits NDS as well as GBA. Keep the named GBA projects as initial research leads, and assess NDS alternatives where they could reduce development effort while meeting the accepted requirements. Neither the user's GBA-ease expectation nor this shortlist selects a ROM platform.

[ADR-0026](0026-yellow-kanto-content-baseline.md) now defines the content target: FireRed/LeafGreen's Kanto map/layout with necessary Yellow adaptations, Yellow's main encounters and plot anchors, and Pikachu as starter. Recharged Yellow is the reference hack, with no dependency on its source. Include the cost of that rendition and the accepted encounter changes in every candidate comparison. No Sevii Islands are currently planned.

Select and pin a foundation only after inspecting the relevant code and reuse requirements, building it reproducibly and testing representative behavior. No candidate is selected by this ADR.

## Alternatives considered

- pret/pokefirered: existing Kanto campaign; modern mechanics and QoL integration remain work to assess.
- FRLG-Plus or FRLG-2Plus: potentially useful existing QoL and split work; inherited features, implementation coverage, maintenance and compatibility need inspection.
- rh-hideout/pokeemerald-expansion: modern mechanics toolkit; the Kanto content path and integration cost need explicit assessment.
- NDS source foundations: permitted by ADR-0025; specific candidates, source maturity, required content/mechanics work and Android screen/control/performance fit need investigation.
- Modifying the Recharged Yellow binary as the main foundation: lacks the editable upstream source and reproducibility desired for ongoing development.

## Consequences

The audit should prevent committing to a base solely from advertised features. It should also identify inherited behavior that conflicts with one difficulty and restrained customisation.

Do not import an entire extra rules/menu system simply because it exists upstream. Conversely, avoid rewriting proven components without a concrete reason.

## Evidence and validation

Preliminary references inspected via public documentation on 2026-09-05; revisions are not pinned and behavior is not verified:

- [pret/pokefirered](https://github.com/pret/pokefirered): FireRed/LeafGreen decompilation.
- [FRLG-Plus features](https://github.com/Deokishisu/FRLG-Plus/blob/master/FEATURES.md): documents controls for transitions, intro animation, text delays and HP-bar animation speed.
- [FRLG-2Plus](https://github.com/egalleta/FRLG-2Plus): README describes adding the physical/special split and category icons to FRLG+.
- [pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion): source-based ROM-hack toolkit, not a completed Kanto campaign.

No source foundation has been compiled, imported or tested in this project.

## Open questions

- Relevant upstream revisions, attribution/reuse requirements and outstanding defects.
- Completeness of split integration in damage, UI, AI and interactions.
- Work needed for milestone caps and battle-speed controls.
- Extensibility for the accepted ruleset and progression systems; see the audit scope below.
- Save behavior, toolchain requirements and test support.
- Verified effort to deliver ADR-0026's Yellow/Kanto baseline, including the Pikachu start and required system access.

## Follow-up

1. Inspect candidate source and history; record exact revisions and relevant paths.
2. Check reuse requirements and preserve required attribution.
3. Compile the most promising candidate using documented toolchain versions.
4. Verify boot, basic battles and save/load; record checks actually performed.
5. Assess split, caps and speed controls plus the accepted-system integration risks below.
6. Accept or replace this proposal with a reasoned foundation choice and reproducible instructions.

## Audit scope update — 2026-09-05

The initial split/caps/speed shortlist predates the later progression decisions. Under the documentation-review task, expand the eventual engineering comparison to include:

- Android emulator play under ADR-0025 and the Yellow/Kanto content baseline in ADR-0026. These resolve [review finding F5](../design-review.md#f5-platform-and-yellow-identity-remain-unconfirmed); the [intended badge reference](../design-review.md#f6-existing-badge-benefits-lacks-a-reference) remains open.
- Both battle formats and Set rules; required move/ability/Fairy/chart support, including weather and terrain moves and their field mechanics under ADR-0011; acquisition, evolution and relearning paths for eligible original forms.
- Permanent per-stat training at the level cap, trainer-wide badge ceilings, individual capability rewards and separate trainer budgets ([ownership map](README.md#progression-ownership)). Inspect battle-end eligibility and reward persistence rather than assuming standard EV machinery fits.
- Preselected bag supplies and loss accounting, data-backed scouting and an AI observation boundary capable of handling custom effects. ADR-0024 remains a proposal for extra disclosure fields.
- Save-data space/versioning for individual points, traits and capability collections, trainer unlocks/builds, and settings; storage/evolution transitions and reward transactions.

Record each capability as inspected, demonstrated, missing or unverified at a pinned revision. This is an investigation checklist, not a claim that any listed foundation supports it. Public-documentation claims above remain preliminary; this audit did not import or build an engine.

Subsequent clarification, 2026-09-05: ADR-0025 settles the platform constraints and broadens candidate eligibility to NDS. Verify Android playability for the eventual candidate; current use of RetroArch/mGBA is not an exclusive emulator choice. This foundation proposal remains Proposed.

Subsequent content clarification, 2026-09-05: ADR-0026 resolves the principal Yellow identity requirements. Detailed map/script comparison is engineering investigation; the owner need not choose a foundation or enumerate every familiar event before that work begins.
