# Agent instructions — Pokémon Supercharged Yellow

These instructions apply throughout this repository. Read any more specific AGENTS.md before changing files in its directory. Current user instructions take precedence over repository guidance.

## Start here

1. Read README.md and docs/adr/README.md.
2. Read accepted ADRs relevant to the task, and any proposals the task touches. Proposed ADRs are not approved requirements.
3. Inspect the relevant source, upstream version and working-tree status before making claims or changes.
4. Establish what is actually implemented from source and [project status](docs/project-status.md). The pinned GBA foundation and campaign systems are being integrated; compilation and focused native tests do not establish a finished game.

## Product intent

Build a familiar Kanto adventure with substantially greater strategic depth and challenge. Preserve Pokémon identities and the atmosphere remembered from childhood. The StarCraft II balance-team comparison describes a quality ambition, not an affiliation or evidence of balance.

The [project charter](docs/adr/0001-project-direction.md) and the accepted records in the [ADR reading map](docs/adr/README.md#current-decision-map) own the product requirements. Read the relevant records in full before changing a system; this file is operational guidance, not a duplicate specification.

Use the [progression ownership map](docs/adr/README.md#progression-ownership) to distinguish badge-wide ceilings, personal training points, Pokémon capabilities and trainer builds. Follow supersession links: ADR-0005 → ADR-0017; ADR-0018 → ADR-0022 → ADR-0023; ADR-0020 → ADR-0021. Historical decisions are not current constraints.

Do not silently adopt assistant suggestions as requirements. ADR-0028 supersedes ADR-0003's foundation proposal. The owner accepted ADR-0024's passive-disclosure policy and subsequently delegated remaining implementation choices through the full-project mandate. The [design review](docs/design-review.md) distinguishes its original audit from later resolutions; use current ADRs and implementation evidence.

## Ownership and changes

- Complete authorised work through implementation, proportionate verification and a clear handoff. Do not stop at a plan when asked to make changes.
- Make routine, reversible engineering decisions within the task scope. Ask only when missing information materially blocks work or a decision would change the user's product goals.
- Prefer the smallest correct change. Avoid speculative frameworks, extra modes, duplicated documentation and unnecessary helpers.
- Preserve other contributors' changes. Never reset, overwrite, force-push or rewrite shared history without explicit authorisation.
- Do not invent success: distinguish proposed, implemented, compiled, automatically tested and manually played.
- Report what changed, why, validation results and concrete remaining limitations.
- Keep written project material in English unless requested otherwise.

## Decision records

Follow docs/adr/README.md. Record consequential architecture, ruleset and balance-policy decisions with context, alternatives, tradeoffs and evidence. Small fixes and individual numerical tuning normally belong in commits or balance notes.

Update an ADR's status and the index together. Accepted means decided, not implemented or verified. Preserve decision history when superseding a decision. Do not require a new user approval for an engineering decision already covered by the task.

## Sources and mechanics

- Prefer inspected source from a pinned upstream revision, then authoritative project documentation. Record URL, revision/version, relevant path and date for external evidence.
- A README claim is a candidate capability, not proof of correctness. Verify relevant implementation and behavior before relying on it.
- Keep baseline data, upstream changes and our overrides distinguishable. Once an engine exists, derive documentation and analysis inputs from the authoritative game data where practical.
- Never mix Recharged Yellow, Recharged Pink, Renegade Platinum, Champions or main-series mechanics without explicitly verifying applicability.
- Evaluate upstream reuse requirements and attribution before importing code or assets. Preserve applicable notices. Do not assume public availability means unrestricted reuse.
- Do not commit ROM images, player saves, credentials or personal playtester data. Specify the exact base checksum when patch packaging is implemented.

## Balance and encounter work

For substantive changes, document:
- The Pokémon's identity and intended contribution.
- The concrete problem and the smallest justified adjustment.
- Its availability, evolution timing, move/item access and relevant level cap.
- The resulting strengths, weaknesses, alternatives and accessible counterplay.
- Matchup evidence and the remaining playtest questions.

Balance against the campaign stage and available teams, not base-stat totals alone. Evaluate whether a change trivialises earlier encounters or removes an intended weakness. Treat examples and untested calculations as hypotheses.

For major encounters, examine several plausible player teams, including ordinary returning-player choices. Validate access to the resources those strategies require. Separate mathematical viability from discoverability, preparation burden and enjoyment. Record assumptions about AI knowledge and test them once an AI policy is chosen.

## Verification

- Use relevant existing checks first. Add tests for meaningful mechanics, progression or regression risks; do not add tests that merely repeat the implementation.
- Documentation-only work needs consistency, status and link checks, not game tests.
- Use the verified commands in README.md and the relevant validation record. Pin the toolchain and upstream revision for reproducibility. Check the native runner's executed count; an empty filter may exit successfully.
- For the split, check damage, categories, UI, AI and affected item/ability/status interactions.
- For caps, cover every applicable experience/level path and milestone unlock.
- For speed controls, check battle duration, text readability, animation completion, audio and saved settings. Do not call shortened delays an exact 2×/4× speed without measurement.
- For releases, verify patch application, booting, saves and required emulator/hardware targets. Explicitly identify targets not tested.
- Automated battle results depend on the simulated player and are not proof of enjoyable or professionally balanced gameplay. Human playtests are necessary.

## Parallel work

Use subagents when the user authorises them or applicable instructions call for delegation. Give each a bounded task and explicit file ownership. Avoid overlapping writes and duplicate research. Share relevant decisions and dependencies; the coordinating agent integrates and reviews the result. Subagents must not independently change product scope.

## Repository workflow

Use short-lived branches and reviewable commits for subsequent changes when practical; this empty repository is being bootstrapped on main at the user's request. Do not create a PR or branch merely to avoid finishing an authorised task.

Add reusable skills only after a real recurring workflow needs procedural detail beyond these instructions. Keep each skill focused, avoid copying the charter or ADRs, and track any repository skill in git.
