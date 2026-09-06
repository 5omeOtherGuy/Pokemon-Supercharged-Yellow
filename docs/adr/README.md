# Architecture and design decision records

ADRs preserve why consequential choices were made. This directory covers software architecture and durable game-design policies. The system is intentionally plain Markdown; no ADR service or generator is required.

## Current decision map

Read the owning records for complete rules, authority and open questions. This map is navigation, not independent decision evidence.

| Area | Current records |
| --- | --- |
| Execution boundaries; battle-entry and field rules | [0027](0027-campaign-and-battle-contract.md); [0033](0033-story-challenge-stage-gates.md) |
| Numerical progression, saves, AI architecture and training services | [0029](0029-progression-and-save-layout.md); [0030](0030-public-observation-ai.md); [0031](0031-training-services-and-individual-traits.md) |
| Charter; decision workflow | [0001](0001-project-direction.md); [0002](0002-decision-records.md) |
| Source foundation | [0028](0028-pinned-kanto-foundation.md); historical proposal [0003](0003-source-foundation.md) |
| Preparation economy, equipment and ordinary rematches | [0034](0034-preparation-economy-and-equipment-access.md); [0035](0035-mainland-vs-seeker-training.md) |
| Battle pacing | [0032](0032-battle-pacing-presets.md) |
| Android play target; eligible ROM platforms | [0025](0025-android-emulation-platform.md) |
| Yellow/Kanto content baseline; starter; current expansion scope | [0026](0026-yellow-kanto-content-baseline.md) |
| Main-campaign roster; complete solo collection/no trading; team continuity and boss mastery | [0004](0004-original-151-roster.md); [0017](0017-trusted-core-and-boss-mastery.md) |
| Levelling and total preparation | [0006](0006-substantial-levelling-investment.md) |
| Boss scouting; consumables and loss costs; AI information | [0007](0007-full-boss-scouting.md); [0008](0008-consumable-loadouts-and-loss-costs.md); [0009](0009-vgc-style-ai-information.md) |
| Battle formats/Set; move curation, weather/terrain and HM/TM usability | [0010](0010-mixed-battles-and-set-rules.md); [0011](0011-curated-moves-across-generations.md) |
| Innate abilities/typings; Fairy; type viability; chart/stat boundaries | [0012](0012-ability-and-typing-rebalance.md); [0013](0013-fairy-type-evaluation.md); [0014](0014-type-identity-and-campaign-viability.md); [0015](0015-standard-chart-and-stat-rebalance.md) |
| Individual traits, development goals and rematch difficulty | [0016](0016-individual-traits-and-battle-training.md) |
| Pokémon capabilities; trainer builds/reward ownership/NPC progression; stat training | [0019](0019-gym-resistances-and-capabilities.md); [0021](0021-trainer-builds-and-reward-ownership.md); [0023](0023-badge-wide-training-ceilings.md) |
| Active-passive scouting and AI observations | [0024](0024-passive-scouting-and-ai-observations.md) |

Supersession chains: 0003 → 0028; 0005 → 0017; 0018 → 0022 → 0023; 0020 → 0021. Follow them to current policy; preserved records describe the decisions and questions of their time.

## Progression ownership

| Term | Owner and purpose | How it changes | Canonical record |
| --- | --- | --- | --- |
| IVs and acquired nature | Individual traits, separate from earned training | IVs random but improvable with a usability safeguard; acquired nature and its stat effects fixed | [0016](0016-individual-traits-and-battle-training.md) |
| Training ceiling | Trainer's badge stage bounds each stat for all Pokémon, including recruits | Badge acquisition unlocks capacity automatically; it does not fill points | [0023](0023-badge-wide-training-ceilings.md) |
| Training points and focus | Individual permanent stat development | Surviving field participants in a won battle earn points; freely changing focus directs future gains only | [0023](0023-badge-wide-training-ceilings.md) |
| Pokémon capabilities | Individual earned passive collection and assigned loadout | Repeatable fixed encounter rewards; three slots with costs of 1–3; free reassignment at a Center/designated service | [0019](0019-gym-resistances-and-capabilities.md) |
| Trainer passives and capacity | Trainer expertise expressed through a selected build, separate from Pokémon slots | Shared weighted budget grows through badges; select at a Center; numbers open; free reassignment selected in ADR-0027 | [0021](0021-trainer-builds-and-reward-ownership.md) |
| Inherited badge benefits | Field permissions were the user's recalled benefit, separate from the additional trainer build | No obedience restrictions; field mapping and omission of automatic stat boosts are selected in ADR-0027 | [0021](0021-trainer-builds-and-reward-ownership.md) |

Ordinary level EXP, stat-training reward value and stat-training eligibility are separate. A capability is also distinct from an innate ability and the normal held-item slot. “Capacity” alone is ambiguous: name the training ceiling, Pokémon capability capacity or trainer-passive budget.

## Index

| ID | Decision | Status |
| --- | --- | --- |
| [0001](0001-project-direction.md) | Project direction and confirmed constraints | Accepted |
| [0002](0002-decision-records.md) | Use repository decision records | Accepted |
| [0003](0003-source-foundation.md) | Select a source foundation through a build audit | Superseded |
| [0004](0004-original-151-roster.md) | Restrict the main campaign roster to the original 151 | Accepted |
| [0005](0005-team-rotation-and-boss-mastery.md) | Build the campaign around team rotation and boss mastery | Superseded |
| [0006](0006-substantial-levelling-investment.md) | Preserve substantial levelling investment | Accepted |
| [0007](0007-full-boss-scouting.md) | Provide full boss team scouting before the first attempt | Accepted |
| [0008](0008-consumable-loadouts-and-loss-costs.md) | Preselect limited battle consumables and retain loss costs | Accepted |
| [0009](0009-vgc-style-ai-information.md) | Use VGC-style opponent information for boss AI | Accepted |
| [0010](0010-mixed-battles-and-set-rules.md) | Use substantial singles and doubles content with Set rules | Accepted |
| [0011](0011-curated-moves-across-generations.md) | Curate moves from any generation for the original 151 | Accepted |
| [0012](0012-ability-and-typing-rebalance.md) | Reassign existing abilities and permit justified typing changes | Accepted |
| [0013](0013-fairy-type-evaluation.md) | Include Fairy to strengthen familiar Pokémon identities | Accepted |
| [0014](0014-type-identity-and-campaign-viability.md) | Give each type distinct identity and meaningful campaign viability | Accepted |
| [0015](0015-standard-chart-and-stat-rebalance.md) | Keep the standard modern chart and use restrained stat rebalancing | Accepted |
| [0016](0016-individual-traits-and-battle-training.md) | Preserve individual traits and reward battle experience | Accepted |
| [0017](0017-trusted-core-and-boss-mastery.md) | Encourage a trusted core with rotating specialists and retain boss mastery | Accepted |
| [0018](0018-gym-training-budget-and-focus.md) | Unlock gym training budgets and allocate development through a focus | Superseded |
| [0019](0019-gym-resistances-and-capabilities.md) | Combine gym training capacity with earned Pokémon capability slots | Accepted |
| [0020](0020-badge-passives-and-trainer-progression.md) | Retain badge benefits and give trainers progression consistent with their experience | Superseded |
| [0021](0021-trainer-builds-and-reward-ownership.md) | Select trainer builds and separate trainer rewards from Pokémon capabilities | Accepted |
| [0022](0022-permanent-training-and-flexible-focus.md) | Keep training gains permanent and let focus direct future growth | Superseded |
| [0023](0023-badge-wide-training-ceilings.md) | Unlock training ceilings trainer-wide through gym badges | Accepted |
| [0024](0024-passive-scouting-and-ai-observations.md) | Disclose active passives and bound AI observations | Accepted |
| [0025](0025-android-emulation-platform.md) | Target Android emulation while allowing GBA or NDS foundations | Accepted |
| [0026](0026-yellow-kanto-content-baseline.md) | Use FireRed/LeafGreen Kanto for the Yellow content baseline | Accepted |
| [0027](0027-campaign-and-battle-contract.md) | Complete Kanto with explicit battle and field rules | Accepted |
| [0028](0028-pinned-kanto-foundation.md) | Build on the pinned expansion Kanto port | Accepted |
| [0029](0029-progression-and-save-layout.md) | Bound permanent development and version its save data | Accepted |
| [0030](0030-public-observation-ai.md) | Choose actions from a bounded public observation | Accepted |
| [0031](0031-training-services-and-individual-traits.md) | Make individual development inspectable and serviceable | Accepted |
| [0032](0032-battle-pacing-presets.md) | Offer named battle-pacing presets | Accepted |
| [0033](0033-story-challenge-stage-gates.md) | Align story challenges with badge-stage preparation | Accepted |
| [0034](0034-preparation-economy-and-equipment-access.md) | Keep preparation supplies replaceable and equipment staged | Accepted |
| [0035](0035-mainland-vs-seeker-training.md) | Use fixed mainland rematches for repeatable training | Accepted |

Accepted means a decision was made. It does not mean implementation or verification is complete. Each record states its implementation status separately.

## When to write an ADR

Use an ADR for engine selection, ruleset policy, roster boundaries, progression/cap design, training systems, AI information policy, save compatibility, distribution, or another choice whose rationale future contributors will need.

Do not create an ADR for every stat adjustment, bug fix or refactor. Record those in focused change notes or commits unless they alter a broader policy. Keep open questions in their owning record, including accepted records with unresolved details. A review report may identify cross-record risks without becoming a second specification or issue tracker.

## Create and maintain a record

1. Read relevant accepted ADRs and inspect the current implementation.
2. Copy [template.md](template.md) to `NNNN-short-title.md`. Use the next unused four-digit ID; do not reuse deleted or rejected numbers.
3. Start as Proposed unless documenting a decision already made. Identify the decision authority: an explicit user requirement, or an engineering decision within a delegated task.
4. Explain context, decision/proposal, considered alternatives, consequences, evidence and validation. Mark assumptions and unknowns. Do not turn assistant suggestions into accepted product requirements.
5. Add the record to this index and link it from the implementing commit or PR when applicable.
6. Update status in both the record and index in the same change. Resolve numbering collisions before integrating parallel contributions.

Routine authorised engineering decisions can be accepted by the implementing agent with recorded rationale and evidence. Material product choices not resolved by the brief remain Proposed until resolved with the owner. This workflow does not add an approval gate to work already authorised.

## Lifecycle

- **Proposed:** under consideration; not binding.
- **Accepted:** the chosen direction.
- **Rejected:** considered and declined; retain the rationale.
- **Superseded:** replaced by a later accepted record.

To replace an accepted decision, create a new ADR explaining the change. Mark the old one Superseded and add reciprocal links in both records; update the index. Do not silently rewrite the old rationale. Clarifications, evidence, implementation status and factual corrections can be updated in place with a dated note when substantive.

When streamlining, keep the current decision near the top, consolidate resolved clarifications into dated history with source links, and preserve the rationale. Keep old decisions visibly historical instead of updating them into current policy.

Use ISO dates (YYYY-MM-DD). Cite upstream repository paths and exact commits/tags when available; label references without pinned revisions as preliminary.

## Verification for documentation changes

Check that filenames and IDs agree, IDs are unique, index statuses match records, relative links resolve, and accepted claims have an identified decision authority. Inspect the diff for accidental scope changes. Follow the current build and validation documentation for implementation changes.

## Review history

The [2026-09-05 design review](../design-review.md) covers all 24 records at `66c17fa`, including proposals and superseded decisions. It records editorial corrections, evidence limits and remaining work. No ADR status changed in that audit.

The subsequent platform clarification is recorded in ADR-0025. It resolves the Android play target and permits GBA/NDS candidates without choosing a foundation.

ADR-0026 subsequently establishes FireRed/LeafGreen's Kanto map with necessary Yellow adaptations, Recharged Yellow as the reference hack, Pikachu as starter and Kanto polish before extras, with no Sevii Islands currently planned. Together these resolve the review's platform/content question; source selection is still open.

The user subsequently accepted ADR-0024's reciprocal active-effect disclosure, resolving F7. ADR-0011 records the later weather/terrain and forgettable-HM/reusable-TM requirements. ADR-0021 narrows F6 using the user's HM-access recollection and pinned Yellow evidence; other inherited badge effects remain undecided. Current totals are 21 Accepted, 1 Proposed and 4 Superseded records.

The next clarification requires all original 151 to be obtainable in one playthrough without trading (ADR-0004), removes obedience restrictions (ADR-0021), and places Mew under the truck (ADR-0026). Automatic badge stat boosts remain an owner question.

## Execution mandate — 2026-09-06

The owner authorizes full implementation, autonomous outstanding choices and publication. ADR-0027 resolves the campaign endpoint, battle-entry scope and inherited badge policies. Earlier owner-question and review-only wording is historical; use later records for resolved choices and [project status](../project-status.md) for verified implementation. There are now 22 Accepted, 1 Proposed and 4 Superseded records.

ADR-0028 selects the audited expansion Kanto port and supersedes the earlier foundation proposal. Current totals: 23 Accepted and 5 Superseded records. Runtime foundation evidence is separate from game completion.

ADR-0029 selects provisional numerical progression and a versioned save layout. Current totals: 24 Accepted and 5 Superseded; balance and cartridge integration are still pending.

ADRs 0030–0031 select the public-observation AI architecture and individual-development services. Current totals: 26 Accepted and 5 Superseded; implementation and validation states are tracked separately.

ADRs 0032–0033 select named battle-pacing presets and explicit story-challenge stage gates. Current totals: 28 Accepted and 5 Superseded; actual pacing and ordinary story validation remain pending.

ADR-0034 selects repeatable preparation access and staged equipment. Current totals: 29 Accepted and 5 Superseded; economic runtime and human preparation evidence remain pending.

ADR-0035 selects fixed mainland Vs. Seeker rematches and deterministic readiness. Current totals:30 Accepted and5 Superseded. Six economy runtime fixtures now pass; complete preparation and rematch gameplay validation remain unfinished.
