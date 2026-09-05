# ADR design review — 2026-09-05

The current decisions are coherent after following their supersession chains. The audit found stale current-facing references, duplicated policy and incomplete source evidence; it did not find an unresolved pair of accepted decisions that requires silently choosing one over the other. Preparation burden and combined progression strength remain substantial, untested design risks.

Reviewed base: [`66c17faf1292626ae5f0ec70531aaf1b2a871ec1`](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/66c17faf1292626ae5f0ec70531aaf1b2a871ec1). Local `main` was first fast-forwarded from `fbc89ae` to that fetched `origin/main`. Scope: all 24 ADRs, index/template, root instructions/README, [review brief](design-review-brief.md), relevant Git history and the external evidence described below. The tracked tree contains documentation/governance only; no engine, verified build or playable prototype exists.

No ADR status or product decision changed. There remain **18 Accepted, 2 Proposed and 4 Superseded** records. “Accepted” is decision status, not implementation or validation status.

## Coverage

Every numbered record was read in full, including historical decisions, unresolved questions and proposals. Unchanged records were reviewed too.

| ADR | Status retained | Audit outcome |
| --- | --- | --- |
| [0001](adr/0001-project-direction.md) | Accepted | Charter shortened; later policy and history linked to owners (F1–F2). |
| [0002](adr/0002-decision-records.md) | Accepted | Workflow retained; no new ADR/tooling needed for editorial corrections. |
| [0003](adr/0003-source-foundation.md) | Proposed | Foundation remains unselected; audit scope updated for later systems (F5, F12). |
| [0004](adr/0004-original-151-roster.md) | Accepted | Eligibility is not universal acquisition; original-form restriction preserved (F11). |
| [0005](adr/0005-team-rotation-and-boss-mastery.md) | Superseded | Preserved historical turnover target; current owner is 0017. |
| [0006](adr/0006-substantial-levelling-investment.md) | Accepted | Removed stale redistribution/capacity guidance; total preparation still open (F1, F10). |
| [0007](adr/0007-full-boss-scouting.md) | Accepted | Existing fields retained; passive/stat extensions remain open (F7–F8). |
| [0008](adr/0008-consumable-loadouts-and-loss-costs.md) | Accepted | Clarifications consolidated; quota examples and unresolved scope retained (F3, F8, F10). |
| [0009](adr/0009-vgc-style-ai-information.md) | Accepted | Official evidence verified; interpretation and protected fields preserved (F3–F4, F7). |
| [0010](adr/0010-mixed-battles-and-set-rules.md) | Accepted | Mixed formats and trainer Set policy retained; party/action details open (F8). |
| [0011](adr/0011-curated-moves-across-generations.md) | Accepted | Move-specific scope retained; unrelated follow-ups linked (F11–F12). |
| [0012](adr/0012-ability-and-typing-rebalance.md) | Accepted | Innate ability policy separated from custom earned effects (F2, F11). |
| [0013](adr/0013-fairy-type-evaluation.md) | Accepted | Contextual endorsement retained; chart history consolidated; hypotheses remain untested (F3, F11). |
| [0014](adr/0014-type-identity-and-campaign-viability.md) | Accepted | Interests and type-level goals preserved without species buff promises (F11). |
| [0015](adr/0015-standard-chart-and-stat-rebalance.md) | Accepted | Option mapping traced; stale mechanism-selection wording corrected (F1, F3). |
| [0016](adr/0016-individual-traits-and-battle-training.md) | Accepted | Traits/goals/rematch difficulty retained; duplicate progression rules linked (F1–F2, F9–F11). |
| [0017](adr/0017-trusted-core-and-boss-mastery.md) | Accepted | Trusted core and mastery retained; progression restatement shortened (F2, F10). |
| [0018](adr/0018-gym-training-budget-and-focus.md) | Superseded | Original reset/budget rules preserved; navigation follows 0022 to 0023 (F1–F2). |
| [0019](adr/0019-gym-resistances-and-capabilities.md) | Accepted | Canonical capability rules consolidated, including service access and fixed rewards (F2, F7, F9–F11). |
| [0020](adr/0020-badge-passives-and-trainer-progression.md) | Superseded | Historical narrower trainer scope preserved; current owner is 0021. |
| [0021](adr/0021-trainer-builds-and-reward-ownership.md) | Accepted | Trainer ownership/budgets/NPC rules retained; clarification history shortened (F2, F6–F11). |
| [0022](adr/0022-permanent-training-and-flexible-focus.md) | Superseded | Permanent-growth history and replaced individual-ceiling interpretation preserved. |
| [0023](adr/0023-badge-wide-training-ceilings.md) | Accepted | Current ceiling/point/focus/eligibility rules retained; duplicate capabilities linked (F1–F2, F9–F12). |
| [0024](adr/0024-passive-scouting-and-ai-observations.md) | Proposed | Pinned source example rechecked; disclosure remains unaccepted (F7, F12). |

## Findings and dispositions

P1 matters before foundation selection or a meaningful playable system; P2 matters before implementing or tuning the affected detail. A risk is not evidence that the design has failed.

### F1. Superseded rules remained in current guidance

**P1 — Fixed; documentation defect.** ADR-0006 still described instant redistribution and individual eight-gym development through ADR-0018. ADR-0001 linked restrained customisation to that superseded record; ADR-0016 repeated historical restrictions beside current rules and left permanent allocation misleadingly open. ADR-0015 still awaited a training mechanism already selected. README left training overlap broadly unresolved despite the accepted additional focused battles.

Evidence: [bb514f7](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/bb514f7) replaced the reset model; [4991131](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/4991131) made ceilings badge-wide; [2bd8f84](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/commit/2bd8f84) added ordinary-training eligibility. Current references now follow [0023](adr/0023-badge-wide-training-ceilings.md). This prevents implementing personal ceiling catch-up or stat resets by mistake. Original superseded decisions remain historical.

### F2. Repeated rules obscured ownership and history

**P2 — Fixed; maintainability defect.** ADRs 0001, 0016, 0017 and 0019, plus AGENTS/README, repeatedly restated systems later refined elsewhere. “Capacity” could mean a training ceiling, three Pokémon slots or a weighted trainer budget. Long clarification logs mixed old open questions with current choices.

Resolution: added the [current reading and ownership maps](adr/README.md#current-decision-map), shortened AGENTS/README, consolidated active-record histories with commit links, and added prominent navigation to superseded records. No IDs, record paths or supersession relationships changed. Original rationale and dated changes remain in their owning records and pinned pre-audit history. The guide now allows unresolved details in their accepted owning record; they need not become duplicate proposals.

### F3. Authority is documented, but not independently authenticated

**P2 — Fixed attribution; evidence limitation retained.** Git contains recorded user quotations/paraphrases, not the raw interview. ADR-0008's option 3 is decoded in its [original record](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/blob/030b85d/docs/adr/0008-consumable-loadouts-and-loss-costs.md); ADR-0015's options 2/3 are decoded in [cb3ebf9](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/blob/cb3ebf9/docs/adr/0015-standard-chart-and-stat-rebalance.md). No option-number meaning needed invention.

[0013](adr/0013-fairy-type-evaluation.md) explicitly records a contextual endorsement of Fairy; [0009](adr/0009-vgc-style-ai-information.md) labels the VGC adaptation and pending-command/RNG exclusions as engineering translations. ADR-0008 records revive exclusion from the selected category list; ADR-0023 labels training at the level cap as a consequence of accepted post-levelling training. These were not upgraded into verbatim approvals. Keep recorded decisions unless contrary evidence arises; do not require a blanket reapproval interview. If a recorded interpretation is disputed, recover that specific exchange before replacing its decision.

### F4. VGC evidence was incomplete and mislocated

**P1 — Fixed; source-evidence defect.** ADR-0009 relied on indexed passages and cited sections 2.4–2.4.1. The accessible [official handbook](https://mcdn.pokemon.com/pokemon-prod/raw/upload/v1/live/static-assets/content-assets/cms2/pdf/play-pokemon/rules/play-pokemon-vgc-tournament-handbook-en.pdf), revision 2026-05-21, places team-list disclosure in **2.5–2.5.1, pages 7–8**. Full relevant text and the page-8 rendering were checked; the retrieved PDF hash is recorded in the ADR.

This supports the existing field/exact-stat distinction. The nature/stat-alignment translation still needs an engine field mapping; fixed natures are already selected in ADR-0016. No VGC battle-format or other game-specific mechanics were imported. Player disclosure of boss nature/stats remains open in ADR-0007, so reciprocal disclosure beyond accepted supplies must not be assumed.

### F5. Platform and Yellow identity remain unconfirmed

**P1 — Needs user decision before foundation selection.** [0001](adr/0001-project-direction.md), [0003](adr/0003-source-foundation.md) and the [brief](design-review-brief.md) do not select hardware/emulator targets, starter policy or the minimum Yellow-specific presentation/events. A FireRed/Emerald shortlist is an engineering proposal, not platform authority. Kanto familiarity alone cannot price the required content work.

Recommended resolution: identify required play targets and essential Yellow elements, distinguishing requirements from nice-to-have content. Candidate source inspection and reuse/toolchain research can proceed now; final foundation selection should use those answers. The suggested Brock/Misty slice remains unaccepted, and postgame roster/content remain open.

### F6. “Existing badge benefits” lacks a reference

**P1 — Needs user decision before inherited badge behavior is implemented.** [0020](adr/0020-badge-passives-and-trainer-progression.md) and its replacement [0021](adr/0021-trainer-builds-and-reward-ownership.md) retain intended existing benefits without identifying the game/version or effects. Importing a candidate engine's badge system could silently select bonuses, progression rules or bugs and then stack extra trainer effects on top.

Recommended resolution: name the intended reference or describe the benefits to retain; engineering should then inventory that version's actual behavior and propose explicit treatment. No specific multiplier or bug is approved. Keep inherited benefits, automatic training ceilings and the additional trainer-passive budget separate.

### F7. Active passive disclosure remains a product proposal

**P1 — Needs user decision before a passive-aware boss prototype.** [0024](adr/0024-passive-scouting-and-ai-observations.md) proposes both sides seeing active trainer passives and assigned Pokémon capabilities, with effect details, while hiding inactive collections. [0007](adr/0007-full-boss-scouting.md) accepts preparation and existing scouting fields; neither it nor uncertainty about AI glitches accepts the extra fields or unlimited knowledge.

Recommendation: accept reciprocal active-loadout disclosure for predictable preparation. Player-only disclosure and discovery during combat remain alternatives, with asymmetry or reduced preparation. Preserve [0009](adr/0009-vgc-style-ai-information.md)'s hidden-stat/pending-action/RNG limits whichever option is chosen. Correct battle resolution and AI observation filtering require engineering work independently of this product choice.

### F8. Battle-entry and action rules are not a complete contract

**P2 — Needs user decision on remaining gameplay scope before the combat slice; engineering should first make concrete options.** [0008](adr/0008-consumable-loadouts-and-loss-costs.md) leaves player quota scope across ordinary trainers/wild battles, capture items, counting units/uses, unused supplies and held-item persistence open. [0010](adr/0010-mixed-battles-and-set-rules.md) leaves party sizes/selection, undersized doubles parties, item action costs and replacements open. [0007–0009](adr/README.md#current-decision-map) also leave some fields and commitment timing open.

Without explicit rules, different battle paths may evade quotas or consume different numbers of actions. Recommend an encounter-type matrix and a sequence from scouting through selection, lock, commands, outcome and recovery. Present meaningful gameplay alternatives for approval; routine synchronization and legality handling are engineering choices. Do not import VGC bring-four rules, double the supply quota in doubles, or treat numerical caps delegated to testing as unresolved owner approval.

### F9. Permanent progression requires outcome-aware persistence

**P1 — Needs engineering investigation.** [0016](adr/0016-individual-traits-and-battle-training.md), [0019](adr/0019-gym-resistances-and-capabilities.md), [0021](adr/0021-trainer-builds-and-reward-ownership.md) and [0023](adr/0023-badge-wide-training-ceilings.md) separate trainer unlocks, individual earned state and assigned builds. Training eligibility depends on the complete battle; awarding irrevocable training on each knockout would conflict with later fainting or loss.

Recommend one explicit reward/persistence design covering deferred eligibility, badge ceiling access without automatic point filling, capability duplicates, capped-focus gains, focus timing, save/load and evolution/storage/gift transitions. Inspect engine save capacity before designing formats. For the EXP model, pin a primary baseline and specify conversion, rounding, recipients, modifiers and reward value before level-cap suppression. Catches, scripted outcomes and other eligibility edges remain open; choosing an engine must not silently decide them.

### F10. Preparation and survival requirements may discourage rotation

**P1 — Needs playtesting; balance tension, not contradiction.** [0006](adr/0006-substantial-levelling-investment.md) accepts substantial levelling plus additional ordinary training; [0016](adr/0016-individual-traits-and-battle-training.md) wants non-tedious development; [0017](adr/0017-trusted-core-and-boss-mastery.md) wants useful specialists. Personal capability collections require eligible encounters, service access can add travel, and [0008](adr/0008-consumable-loadouts-and-loss-costs.md) retains money/spent-item losses. Failure/fainting can cost supplies and deny new training or capabilities even to a useful support participant.

Recommend timing one recruit and several specialists from acquisition to useful boss readiness, including failures, rematches, money recovery and service travel at a declared speed. Check continuing access to replenishment and rematches. Observe brief participation farming, support/sacrifice incentives and reward comprehension. Badge-wide ceilings reduce catch-up work but do not establish acceptable total time. Preserve the selected survival/loss rules pending evidence; measure useful builds separately from complete collections.

### F11. Combined power and resource access need stage-specific evidence

**P1 — Needs playtesting, with an engineering access audit first.** [0012–0016](adr/README.md#current-decision-map), [0019](adr/0019-gym-resistances-and-capabilities.md), [0021](adr/0021-trainer-builds-and-reward-ownership.md) and [0023](adr/0023-badge-wide-training-ceilings.md) allow many interacting layers. All-stat potential is intentional; claiming that slow acquisition alone contains it would ignore the stage-ceiling requirement. Fixed nature and improvable IVs also require a meaningful usability safeguard without promising every species/individual is optimal.

Recommend comparing poor allowed traits, typical role-focused builds and the strongest legal stage builds in both formats. Include capability/held-item/trainer combinations and actual consumables, not isolated stat totals. Verify species, evolution, move-learning/relearning, ability and item access before calling a counter available. Original-151 eligibility is not a promise all are catchable; type playability and named favourites are not quotas or automatic buffs. Fairy and Bug/Flying hypotheses remain untested. Preserve weaknesses and partial NPC loadouts rather than assuming universal maximum development.

### F12. The original foundation checklist underrepresented later systems

**P1 — Needs engineering investigation; documentation scope fixed.** [0003](adr/0003-source-foundation.md)'s original shortlist focused on split, caps and speed, predating the accepted progression stack. The updated proposal adds both formats, effects, acquisition paths, information filtering, rewards and save state. Candidate feature descriptions remain unpinned historical leads; this review did not build or verify any candidate.

[0024](adr/0024-passive-scouting-and-ai-observations.md)'s pinned `battle_ai_util.c` example was rechecked at `7644501f3065b38aea2cfc64b5b58ca011bce4f5`, specifically the awareness and known/candidate-ability functions. It demonstrates knowledge modelling, not full hidden-data isolation or support for our custom effects. The type/chart references in 0013 and EXP overview in 0023 also remain preliminary; pin primary data and selected behavior before implementation. The standard chart does not select a complete mechanics generation.

## Pressing owner decisions

Resolve these in dependency order; no answer is needed to finish this documentation review.

1. **Before selecting the foundation:** required platform/hardware/emulators and essential Yellow presentation/starter/event requirements (F5).
2. **Before implementing badge effects:** intended reference game/version or exact inherited benefits (F6).
3. **Before implementing passive-aware scouting/AI:** accept or revise ADR-0024's reciprocal active-effect disclosure (F7).

The first playable boundary should be chosen after the foundation comparison makes cost concrete. Brock/Misty is a candidate, not a commitment. Prepare F8's remaining battle-scope choices with that slice; do not turn postgame, numerical budgets or every implementation detail into another interview now.

## Recommended next sequence

1. **Documentation readiness:** this audit is complete; use the current reading/ownership map and preserved history. Proposed records stay Proposed. No further cleanup or blanket reapproval is needed to investigate foundations.
2. **Foundation investigation:** inspect and pin candidate sources, reuse terms, toolchains and actual mechanisms using ADR-0003's expanded checklist. This read-only work can proceed without further clarification. Combine findings with F5's requirements before selecting/importing a foundation in a separately authorised engineering task.
3. **Baseline verification:** after selection, establish reproducible build, boot, save/load and target checks. Record exactly what was compiled, run and observed. Resolve the mechanics baseline and prototype scope; a compile alone is not playability.
4. **Playable validation:** implement a bounded slice exercising relevant singles/doubles encounters, caps, speed, training/rewards, supplies, scouting and the approved information contract. Preserve proposed assumptions visibly. Check legal actions, hidden-data invariance, reward transactions and save persistence, then run human preparation/difficulty playtests for F10–F11. Tune caps/costs/rates using that evidence.

No engine import, game code, simulation or prototype work was performed by this audit.

## Verification performed

- Read all 24 ADRs, including all four superseded and both proposed records; inspected the historical commits cited above for option mappings and decision transitions.
- Checked unique four-digit IDs against filenames/titles, all index rows/statuses, required metadata/sections, reciprocal supersession links and acyclic chains. Statuses match the reviewed base.
- Checked relative Markdown paths and heading fragments throughout tracked documentation, including the new report; verified local commit/path targets for repository history links. External references were selectively reviewed as described above, not all HTTP-tested.
- Reviewed the complete documentation diff for retained intent, current-policy references, unresolved questions and explicitly provisional examples. Superseded decision bodies were preserved; only navigation around them changed.
- Ran `git diff --check`. No game/build/runtime/balance tests exist or were claimed. No generated build targets or source dependencies were introduced. The local instruction bootstrap and task ledger are excluded from the commit.
