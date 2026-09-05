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

Select and pin a foundation only after inspecting the relevant code and reuse requirements, building it reproducibly and testing representative behavior. No candidate is selected by this ADR.

## Alternatives considered

- pret/pokefirered: existing Kanto campaign; modern mechanics and QoL integration remain work to assess.
- FRLG-Plus or FRLG-2Plus: potentially useful existing QoL and split work; inherited features, implementation coverage, maintenance and compatibility need inspection.
- rh-hideout/pokeemerald-expansion: modern mechanics toolkit; the Kanto content path and integration cost need explicit assessment.
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
- Effort needed to reproduce desired Yellow elements.

## Follow-up

1. Inspect candidate source and history; record exact revisions and relevant paths.
2. Check reuse requirements and preserve required attribution.
3. Compile the most promising candidate using documented toolchain versions.
4. Verify boot, basic battles and save/load; record checks actually performed.
5. Assess split, caps and speed controls plus the accepted-system integration risks below.
6. Accept or replace this proposal with a reasoned foundation choice and reproducible instructions.

## Audit scope update — 2026-09-05

The initial split/caps/speed shortlist predates the later progression decisions. Under the documentation-review task, expand the eventual engineering comparison to include:

- The owner's platform/hardware targets and minimum Yellow-specific content/presentation; these are not established by the candidate list. See [review finding F5](../design-review.md#f5-platform-and-yellow-identity-remain-unconfirmed). The [intended badge reference](../design-review.md#f6-existing-badge-benefits-lacks-a-reference) also remains open.
- Both battle formats and Set rules; required move/ability/Fairy/chart support; acquisition, evolution and relearning paths for eligible original forms.
- Permanent per-stat training at the level cap, trainer-wide badge ceilings, individual capability rewards and separate trainer budgets ([ownership map](README.md#progression-ownership)). Inspect battle-end eligibility and reward persistence rather than assuming standard EV machinery fits.
- Preselected bag supplies and loss accounting, data-backed scouting and an AI observation boundary capable of handling custom effects. ADR-0024 remains a proposal for extra disclosure fields.
- Save-data space/versioning for individual points, traits and capability collections, trainer unlocks/builds, and settings; storage/evolution transitions and reward transactions.

Record each capability as inspected, demonstrated, missing or unverified at a pinned revision. This is an investigation checklist, not a claim that any listed foundation supports it. Public-documentation claims above remain preliminary; this audit did not import or build an engine.
