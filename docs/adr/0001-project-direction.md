# ADR-0001: Project direction and confirmed constraints

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user project brief and scope clarification on 2026-09-05
- Implementation: Not started — requirements recorded only
- Supersedes: None
- Superseded by: None

## Context

The user wants an ultimate Kanto experience: familiar to someone returning after childhood, with significantly greater challenge and depth. Pokémon should retain their identities and meaningful differences in strength. The comparison to professional StarCraft II balancing is a quality aspiration.

Recharged Yellow inspired the concept, but its source is unavailable. The user explicitly clarified that the project does not need it.

## Decision

- Create a Kanto ROM hack inspired by Recharged Yellow.
- Introduce the physical/special split and rebalance Pokémon and encounters.
- Preserve familiar identity and atmosphere; do not target equal strength or universal viability for all Pokémon.
- Begin with one difficulty setting and level caps.
- Keep player-facing customisability limited, including the later requested training-focus, Pokémon-capability and trainer-build choices in [ADR-0023](0023-badge-wide-training-ceilings.md), [ADR-0019](0019-gym-resistances-and-capabilities.md) and [ADR-0021](0021-trainer-builds-and-reward-ownership.md).
- Include QoL improvements, especially the ability to adjust battle speed.
- Do not make access to Recharged Yellow source a project dependency.

## Alternatives considered

- Depending on Recharged Yellow source: discarded by the user's clarification.
- Multiple initial difficulty modes and extensive rules toggles: inconsistent with the clarified initial scope.
- Equalising every Pokémon's power: explicitly outside the user's balance goal.

## Consequences

Rules, encounter design, available resources and Pokémon changes need to be assessed together. A single initial difficulty narrows validation scope. Battle-speed controls are a required capability, while their precise implementation remains open.

These requirements do not establish a particular engine or approve the mechanics and content suggestions made by the assistant.

## Evidence and validation

Source: project conversation on 2026-09-05. The user requested a physical/special split, identity-preserving rebalance, familiar Kanto and substantially greater challenge/depth. In the follow-up, the user confirmed unavailable Recharged Yellow source, one initial difficulty, level caps, limited customisability and adjustable battle speed.

No implementation or gameplay validation has occurred.

## Current refinements

Use the [ADR reading map](README.md#current-decision-map) for the canonical policies that refine this charter. This record owns the initial product direction, not a second copy of every later rule.

## Open questions

Still unresolved here: source foundation and exact Android device/emulator validation targets; starter access and Yellow-specific events/presentation; level-cap values and enforcement; speed presets and other QoL; the first playable milestone and postgame scope. [ADR-0025](0025-android-emulation-platform.md) requires Android emulator play, allows GBA or NDS foundations and does not require original-console hardware support. System-specific open questions belong in their owning ADRs.

## Follow-up

Resolve open questions when they affect concrete work; a complete game specification is not a prerequisite for useful engineering. Follow the [review's next sequence](../design-review.md#recommended-next-sequence).

## Decision history — 2026-09-05

- Interview rounds 1–7 established roster, preparation, scouting, consumables, AI information, battle formats and move/ability/typing boundaries in ADRs 0004–0012. The original team-turnover target in ADR-0005 is historical; ADR-0017 replaces it with a trusted core and rotating specialists while retaining boss mastery.
- Fairy progressed from discussion to a recorded endorsement based on identity and gameplay interest in ADR-0013. ADRs 0014–0015 then established type-level viability, no representation quota, the standard modern chart and restrained stat changes with justified exceptions. Player interests are not automatic buff targets.
- ADR-0016 introduced individual traits and battle-earned development. ADR-0018 selected individual gym budgets and automatic focus allocation, then added earned focus changes with instant redistribution. ADR-0019 retained capacity and added individually earned capabilities; this explicitly extended the restrained-customisation scope.
- ADR-0021 supersedes ADR-0020 with selectable trainer builds, broader training/EXP scope and repeatable individual reward sources. Later clarifications selected weighted trainer capacity, growth through badges and use by major bosses and selected experienced trainers.
- ADR-0022 supersedes ADR-0018's focus/reset model with permanent points and flexible future focus. ADR-0023 supersedes the individual training-ceiling interpretation with badge-wide access, retaining personally earned points and capabilities; subsequent clarifications add ordinary-training eligibility and EXP-style scaling.
- ADR-0024 remains Proposed. Preparation goals and uncertainty about AI reliability did not accept unrestricted hidden-state access or the proposed active-passive disclosure fields.

Editorial audit, 2026-09-05: consolidated the repeated clarification log into these references without replacing a decision. The [complete pre-audit chronology](https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/blob/66c17faf1292626ae5f0ec70531aaf1b2a871ec1/docs/adr/0001-project-direction.md) remains in Git; each owning record retains its rationale and evidence. See the [review](../design-review.md) for remaining work.

Subsequent platform clarification, 2026-09-05: ADR-0025 resolves the play-platform portion of the review. The user requires Android emulator compatibility, accepts GBA or NDS based on development suitability, and does not require cartridge/physical-console use. Yellow-specific content and the foundation itself remain open.
