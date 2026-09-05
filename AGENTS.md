# Agent instructions — Pokémon Supercharged Yellow

These instructions apply throughout this repository. Read any more specific AGENTS.md before changing files in its directory. Current user instructions take precedence over repository guidance.

## Start here

1. Read README.md and docs/adr/README.md.
2. Read accepted ADRs relevant to the task, and any proposals the task touches. Proposed ADRs are not approved requirements.
3. Inspect the relevant source, upstream version and working-tree status before making claims or changes.
4. Establish what is actually implemented. This repository currently contains project governance only; no engine, build command or playable prototype has been verified.

## Product intent

Build a familiar Kanto adventure with substantially greater strategic depth and challenge. Preserve Pokémon identities and the atmosphere remembered from childhood. The StarCraft II balance-team comparison describes a quality ambition, not an affiliation or evidence of balance.

The authoritative initial requirements are in ADR-0001, with refinements in the accepted records listed in docs/adr/README.md. ADR-0005 is superseded by ADR-0017, ADR-0018 by ADR-0022, ADR-0022 by ADR-0023, and ADR-0020 by ADR-0021. ADRs 0019, 0021 and 0023 define individual capabilities, trainer builds, selective opponent progression and permanent training. In particular:
- Main-campaign species are restricted to the original 151 in their original forms. No regional variants or Mega Evolutions, including opponent teams. Postgame scope remains open.
- Encourage a trusted core with rotating specialists (ADR-0017, superseding ADR-0005). Repeated attempts to master major bosses remain intended. Do not enforce a fixed core size, forced losses or mandatory team changes.
- Levelling a newly caught Pokémon to the cap should normally require substantial training (roughly 20–30 minutes or more); ordinary stat training should usually require additional focused battles afterward. Total preparation duration and reference speed remain open.
- Use random, improvable IVs with a safeguard against randomisation making a Pokémon unusable, and keep its acquired nature and nature-based stat effects fixed (ADR-0016). Exact IV bounds and improvement methods remain open.
- Reward battle use, especially boss experience, through clear, predictable development beyond level. Reward continuity without excessive rotation penalties or tedious training. ADR-0016 defines traits and individual capability eligibility. ADR-0023 selects permanent allocation through a freely changeable focus and trainer-wide training ceilings unlocked by badges; recruits share these ceilings immediately. Individual gym-earned capabilities remain separate (ADR-0019). Each Pokémon has three capability slots; assigned capabilities cost one, two or three slots and may combine benefits with drawbacks. Capabilities can come from repeatable gyms, Elite Four and other notable encounters. Non-repeatable story encounters may grant trainer passives, never Pokémon capabilities (ADR-0021). All capability rewards require field participation, no fainting during that encounter and a player victory. Reassignment among earned capabilities is free only at a Pokémon Center or designated service. Keep the normal held-item slot in addition to capability capacity. Each granting encounter has one fixed capability reward for all qualifying Pokémon. Exact effects and repeat-access rules remain open.
- Preserve the intended existing badge benefits and support limited trainer builds chosen at a Pokémon Center, including training/EXP and modest combat passives (ADR-0021). Trainer effects must express trainer expertise, distinct from individual Pokémon skills; build capacity and exact effects remain open; the reference game/version and exact inherited effects must be resolved, not inferred from an unselected engine.
- Major bosses and selected experienced trainers may use capabilities. Not every Pokémon or slot should be filled by default. Scale use with progression only where it fits the trainer's experience and story; a local ace may carry the nearby gym's capability. Reflect the player's League accomplishments without making maximum whole-team loadouts the norm (ADR-0021).
- Ordinary stat-training points require field participation, no fainting at any point and a team victory. Switched-out survivors qualify; unused reserves, fainted participants and losing teams do not. These outcomes never remove previously earned points. Reward amounts follow main-series battle EXP scaling; the exact formula generation, conversion and modifiers remain open. Stat training must still work at the level cap, using reward value before level-EXP suppression. Ordinary EXP eligibility remains separate (ADR-0023).
- Focus can change on the fly and directs future gains only; earned stat points never relocate. Each stat has a training maximum, and one Pokémon can theoretically reach them all. Maxing relevant stats is the normal late-game target; maxing everything should be difficult and exceptional. The trainer's badges raise per-stat ceilings for every Pokémon regardless of individual participation or fainting. Training can enable limited survival or Speed adjustments; stage ceilings must constrain overtraining. Numerical stage caps and training rates remain open (ADR-0023).
- Individual gym capability rewards require a player victory, field participation and no fainting during that battle. Switched-out survivors qualify; unused reserves and fainted participants do not. Use stronger gym-rematch teams suited to campaign progress so recruits can earn the same personal capabilities as veterans (ADR-0016).
- Collecting all eight gym capabilities requires each Pokémon to qualify for all eight leader rewards. This is not required for full training capacity: the trainer's badge progression unlocks that for everyone. Rematches recover missed capabilities and do not repeatedly raise ceilings (ADR-0023).
- Full boss teams, moves, abilities, held items and selected bag consumables/quantities must be available in-game before the first attempt.
- Boss AI follows the VGC open-team-list information boundary in ADR-0009; do not import VGC battle-format rules. Exact opponent stats, hidden allocations, pending player actions and future RNG are not direct AI inputs.
- Boss defeats cost money and spent consumables. Non-held battle consumables have one fixed campaign-wide, preselected allowance; choose its number through balance testing, not the earlier example of three.
- Player battle supplies may include HP healing, status cures, PP restoration and temporary stat boosters; revives are excluded. Only major bosses among opposing trainers use bag consumables, with the same maximum as the player.
- Consumable duplicates are restricted by functional category; determine separate numerical caps through balance testing. Exclude items combining categories from preselected battle supplies. Both sides see each other's complete selected consumables and quantities; pending item-use actions stay private.
- Use a substantial campaign mix of singles and doubles, including major bosses in both. Use Set rules throughout trainer battles; no free Shift-style switch after an opposing knockout.
- Curate moves from any generation when they fit the original 151's identities and campaign balance. This does not import every move or settle move versions.
- Existing official innate abilities may be reassigned when identity and balance justify it. The separately authorised gym-earned passive system is governed by ADR-0019; it is not blanket permission to invent new innate abilities. Start from modern official typings and allow carefully justified custom changes; include Fairy to strengthen familiar Pokémon identities and make their battle roles more interesting, as recorded in ADR-0013. Keep the standard modern type-effectiveness chart (ADR-0015). Record specific changes and test both formats.
- Physical/special split is required.
- Launch with one difficulty setting and level caps.
- Keep player-facing customisation restrained while supporting the requested training-focus and learned-passive choices; adjustable battle speed and useful QoL are required.
- Pokémon need meaningful strengths, weaknesses and roles; equal power or universal viability is not the goal. Each type should feel distinct and playable. Extend Bug usefulness and address late-game Flying concerns such as Pidgeot (ADR-0014); player favourites are evaluation interests, not automatic buff targets. There is no type-representation quota: justify retypings per species. Use stat redistribution and modest strength changes primarily; substantial changes require a documented exceptional or necessary case (ADR-0015).
- Recharged Yellow is inspiration, not a source-code dependency.

Do not silently adopt earlier assistant suggestions as requirements. Engine choice, postgame roster, exact mechanics generation, cap values, IV safeguards and improvement access, battle-training numbers and remaining progression details, detailed battle-format assignments, remaining consumable selection/scope details, training/scouting implementation, starters, Yellow-specific events, speed presets and prototype boundaries remain unresolved.

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
