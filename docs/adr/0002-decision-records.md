# ADR-0002: Use repository decision records

- Status: Accepted
- Date: 2026-09-05
- Decision authority: User request to create AGENTS.md and an ADR system; workflow details chosen within that setup task
- Implementation: Documentation system established; no automation required
- Supersedes: None
- Superseded by: None

## Context

The project will involve architecture and balance choices across multiple sessions and potentially multiple agents. Confirmed requirements need to remain distinguishable from suggestions, and future contributors need the reasons behind decisions.

## Decision

Keep repository-wide operating instructions in AGENTS.md and consequential decisions in docs/adr/. Use numbered Markdown records, a template, a manually maintained index and four statuses: Proposed, Accepted, Rejected and Superseded.

Record decision authority and implementation status separately. Preserve superseded decisions with reciprocal links. Reference relevant ADRs in changes. Keep routine implementation choices lightweight and avoid duplicating policies in custom skills.

## Alternatives considered

- Conversation history alone: insufficiently visible to future repository contributors.
- A separate external decision service: unnecessary dependency for this project.
- A custom generator or CI validator immediately: additional code and maintenance without a demonstrated need.
- A dedicated skill repeating the same instructions: duplicates the repository's authoritative guidance.

## Consequences

The workflow works with GitHub and ordinary text editors. Authors must keep the index and records consistent. A validator or focused skill can be added later if repeated work justifies it.

## Evidence and validation

The user explicitly requested repository instructions, an ADR system and skills if necessary. The workflow is documented in [the ADR guide](README.md), [the template](template.md) and [AGENTS.md](../../AGENTS.md).

Documentation validation checks IDs, statuses, relative links and requirement provenance. There is no claim of game build or runtime validation.

## Open questions

None required for this setup.

## Follow-up

Use this workflow during the source-foundation decision. Add tooling only when a concrete recurring need appears.
