# Fresh-session ADR review brief

## User request and scope

The user requested the next step be a fresh session reviewing all ADRs, streamlining them and identifying gaps, potential issues and contradictions. Perform that review and make evidence-supported documentation improvements. This brief prepares the task; it is not the audit result or a new game-design decision.

Repository: [Pokémon Supercharged Yellow](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow).

At handoff, the repository contains governance and design documentation only. No engine, verified build or playable prototype exists. Verify the live state before making claims. The last discussion suggested a playable slice through Brock and Misty; that suggestion has not been accepted. Platform/hardware requirements and exact Yellow-specific presentation also remain unconfirmed.

## Start from the repository

1. Read [AGENTS.md](../AGENTS.md), [README.md](../README.md), the [ADR index and lifecycle](adr/README.md), and every ADR in full, including Proposed and Superseded records. Discover the live set; do not assume the current numbering is final.
2. Record the reviewed commit and inspect relevant Git history where wording, authority or supersession is ambiguous. Check any directory-specific instructions before editing.
3. Reconstruct current decisions from the evidence. Treat this brief and the summaries in AGENTS/README as navigation aids to be checked, not independent proof that every statement accurately reflects the user.
4. Distinguish explicit user choices, documented agent interpretations, proposals, illustrative examples, unresolved alternatives and actual implementation.
5. If an earlier answer is referenced only as an option number, or its meaning cannot be established from available evidence, flag the missing evidence. Do not invent the unseen options or resolve an ambiguous product choice silently.

## Audit coverage

Review all records and their interactions. The following are prompts for investigation, not predetermined findings.

- Decision integrity: contradictory active requirements; decisions changed without clear supersession; stale open questions; proposed ideas presented as accepted; unsupported claims of approval; conclusions stronger than the cited answer.
- Document structure: duplicated rules across ADRs, bloated clarification histories, unclear terminology, broken links, inconsistent statuses, overlapping ownership and summaries that lag the current decision.
- Progression ownership: trainer-wide badge ceilings versus individual earned points and capabilities; trainer passive capacity versus Pokémon capability capacity; permanent training and freely changing focus versus service-only passive assignment.
- Preparation and rewards: levelling plus training plus rematches; loss costs and replenishment; survival requirements and support roles; recruits catching up; repeatability of individual reward sources; the treatment of one-time story rewards.
- Balance coherence: permanent all-stat potential, stage ceilings, fixed natures, improvable random IVs, combined innate abilities/held items/capabilities/trainer effects, species identity and viable rotating specialists.
- Battle rules: singles/doubles assumptions, party sizes, item action costs and supplies, scope of wild/trainer/boss rules, timing of rewards and loadout commitments.
- Information and AI: what is disclosed, inferred or hidden; engine state versus decision observations; the status of ADR-0024; custom-effect support and source assumptions.
- Project readiness: platform requirements, source foundation, mechanics baseline, starter and Yellow identity, encounter access, evolution and move-learning access, QoL and speed, prototype scope, save/persistence needs and practical validation.
- Evidence quality: main-series or hack-specific claims with missing baseline revisions; source references that were only partially inspected; engineering deductions mislabeled as direct user decisions.

Separate a real contradiction from a balance tension, an intentional constraint, a reasonable implementation detail or a number deliberately left to testing.

## Streamlining rules

- Apply clear editorial fixes and reconciliations supported by an unambiguous later accepted decision.
- Keep the current decision easy to find in each active ADR. Move historical explanations into clearly marked history where helpful; preserve rationale, authority and traceability.
- Prefer references to a canonical decision over repeating full rules throughout the repository. Keep AGENTS focused on operational constraints and README concise.
- Preserve ADR IDs and existing links where practical. Do not erase superseded records or rewrite their original decisions into present-day policy.
- Follow the established supersession lifecycle when a genuine decision is replaced. Do not mint an ADR for each typo or minor clarification.
- Do not change gameplay choices to make contradictions disappear. If accepted choices genuinely conflict and the evidence does not resolve them, document options and a recommendation for the user.
- Keep Proposed decisions proposed unless the session provides authority to accept them. In particular, the earlier uncertainty about AI omniscience is not approval to remove hidden-information boundaries.
- Keep examples, prototype assumptions and balance hypotheses explicitly provisional.
- This task is documentation review and cleanup. Do not begin importing an engine, writing game code or implementing a prototype as a substitute for completing the review.

## Deliverables

1. Streamlined ADRs, index and related summaries, with evidence-supported fixes committed through the repository workflow.
2. A concise review report in `docs/design-review.md` identifying the reviewed base commit and all ADRs covered. For each substantive finding, include affected records, evidence, practical consequence, priority, proposed resolution and disposition: fixed, needs user decision, needs engineering investigation or needs playtesting.
3. A short list of genuinely pressing user decisions, ordered by what blocks the next concrete milestone. Do not turn every tunable number or technical choice into another interview question.
4. A recommended next work sequence, distinguishing documentation readiness, foundation investigation and playable validation. State what can proceed without further clarification.

Use source-linked findings rather than a duplicate copy of the full game specification. If fresh-session context is missing, finish all independent review and cleanup first, then ask only for the specific missing information.

## Completion checks

- Every discovered ADR is covered, including history and proposals.
- IDs, filenames, statuses, reciprocal supersession links, index entries and relative links agree.
- Current summaries match the canonical decisions and no resolved question remains misleadingly open in an active section.
- The diff preserves user intent and makes all uncertain changes explicit.
- Findings distinguish confirmed defects from risks and hypotheses.
- The report states documentation checks actually performed and does not imply game testing occurred.
- Final handoff links the review and changes, summarizes the most consequential findings and presents only the remaining high-priority questions.
