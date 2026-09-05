# Architecture and design decision records

ADRs preserve why consequential choices were made. This directory covers software architecture and durable game-design policies. The system is intentionally plain Markdown; no ADR service or generator is required.

## Index

| ID | Decision | Status |
| --- | --- | --- |
| [0001](0001-project-direction.md) | Project direction and confirmed constraints | Accepted |
| [0002](0002-decision-records.md) | Use repository decision records | Accepted |
| [0003](0003-source-foundation.md) | Select a source foundation through a build audit | Proposed |
| [0004](0004-original-151-roster.md) | Restrict the main campaign roster to the original 151 | Accepted |
| [0005](0005-team-rotation-and-boss-mastery.md) | Build the campaign around team rotation and boss mastery | Accepted |

Accepted means a decision was made. It does not mean implementation or verification is complete. Each record states its implementation status separately.

## When to write an ADR

Use an ADR for engine selection, ruleset policy, roster boundaries, progression/cap design, training systems, AI information policy, save compatibility, distribution, or another choice whose rationale future contributors will need.

Do not create an ADR for every stat adjustment, bug fix or refactor. Record those in focused change notes or commits unless they alter a broader policy. Keep open questions in the relevant proposal rather than creating a separate tracking system.

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

Use ISO dates (YYYY-MM-DD). Cite upstream repository paths and exact commits/tags when available; label references without pinned revisions as preliminary.

## Verification for documentation changes

Check that filenames and IDs agree, IDs are unique, index statuses match records, relative links resolve, and accepted claims have an identified decision authority. Inspect the diff for accidental scope changes. No build or test command exists until the chosen foundation supplies a verified one.
