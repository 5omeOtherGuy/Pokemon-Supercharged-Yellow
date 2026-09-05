# Agent instructions — Pokémon Supercharged Yellow

These instructions apply throughout this repository. Read any more specific AGENTS.md before changing files in its directory. Current user instructions take precedence over repository guidance.

## Start here

1. Read README.md and docs/adr/README.md.
2. Read accepted ADRs relevant to the task, and any proposals the task touches. Proposed ADRs are not approved requirements.
3. Inspect the relevant source, upstream version and working-tree status before making claims or changes.
4. Establish what is actually implemented. This repository currently contains project governance only; no engine, build command or playable prototype has been verified.

## Product intent

Build a familiar Kanto adventure with substantially greater strategic depth and challenge. Preserve Pokémon identities and the atmosphere remembered from childhood. The StarCraft II balance-team comparison describes a quality ambition, not an affiliation or evidence of balance.

The authoritative initial requirements are in ADR-0001, with refinements in accepted ADRs 0004–0011. In particular:
- Main-campaign species are restricted to the original 151; forms and postgame scope remain open.
- Frequent team rebuilding and repeated attempts to master major bosses are intended for the target returning player. Do not impose forced losses or mandatory team changes.
- Levelling a newly caught Pokémon to the cap should normally require substantial training (roughly 20–30 minutes or more); preparation systems and reference speed remain open.
- Full boss teams, moves, abilities, held items and selected bag consumables/quantities must be available in-game before the first attempt.
- Boss AI follows the VGC open-team-list information boundary in ADR-0009; do not import VGC battle-format rules. Exact opponent stats, hidden allocations, pending player actions and future RNG are not direct AI inputs.
- Boss defeats cost money and spent consumables. Non-held battle consumables have one fixed campaign-wide, preselected allowance; choose its number through balance testing, not the earlier example of three.
- Player battle supplies may include HP healing, status cures, PP restoration and temporary stat boosters; revives are excluded. Only major bosses among opposing trainers use bag consumables, with the same maximum as the player.
- Consumable duplicates are restricted by functional category; determine separate numerical caps through balance testing. Exclude items combining categories from preselected battle supplies. Both sides see each other's complete selected consumables and quantities; pending item-use actions stay private.
- Use a substantial campaign mix of singles and doubles, including major bosses in both. Use Set rules throughout trainer battles; no free Shift-style switch after an opposing knockout.
- Curate moves from any generation when they fit the original 151's identities and campaign balance. This does not import every move or settle move versions, abilities, typings or forms.
- Physical/special split is required.
- Launch with one difficulty setting and level caps.
- Keep player-facing customisation restrained; adjustable battle speed and useful QoL are required.
- Pokémon need meaningful strengths, weaknesses and roles; equal power or universal viability is not the goal.
- Recharged Yellow is inspiration, not a source-code dependency.

Do not silently adopt earlier assistant suggestions as requirements. Engine choice, forms/postgame roster, exact mechanics generation, cap values, IV/EV/nature policy, detailed battle-format assignments, remaining consumable selection/scope details, training/scouting implementation, starters, Yellow-specific events, speed presets and prototype boundaries remain unresolved.

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
- When engine work begins, document and verify the actual build/test commands before adding them here. Pin the toolchain and upstream revision for reproducibility.
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
