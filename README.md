# Pokémon Supercharged Yellow

A Kanto ROM-hack project inspired by Pokémon Recharged Yellow: familiar Pokémon and places, modern physical/special move categories, greater strategic depth, and balance that preserves distinct identities.

## Current state

Project governance is established. No engine has been selected or imported, no game build has been verified, and no playable prototype is available. Recharged Yellow source is unavailable and is not required for this project.

## Confirmed direction

- Original 151 species in original forms only in the main campaign; no regional variants or Mega Evolutions.
- Frequent team rebuilding and repeated attempts to master major bosses.
- Substantial levelling investment for newly caught Pokémon: roughly 20–30 minutes or more.
- Full boss team, move, ability, held-item and bag-consumable scouting before the first attempt.
- VGC-style open-team-list information for boss AI, with hidden stats and pending actions protected.
- Preselected battle consumables with one campaign-wide allowance and separate category limits, their numbers chosen through testing; boss defeats cost money and spent consumables.
- Mixed-category battle supplies are excluded. Both player and boss see each other's complete selected supplies and quantities.
- Player supplies allow HP healing, status cures, PP restoration and temporary stat boosters, but no revives. Only opposing major bosses use bag consumables, with the same maximum as the player.
- A substantial mix of singles and doubles, with major bosses in both; Set rules throughout trainer battles.
- Carefully selected moves from any generation that fit Pokémon identity and balance.
- Justified reassignment of existing abilities and custom typing changes from a modern official baseline; Fairy is included to strengthen familiar Pokémon identities and create more interesting battle roles ([ADR-0013](docs/adr/0013-fairy-type-evaluation.md)). Exact type-chart policy remains open.
- One initial difficulty setting.
- Level caps.
- Physical/special split.
- Restrained player customisation and useful quality-of-life improvements, especially adjustable battle speed.
- Challenging encounters and well-designed Pokémon without making every species equally strong or viable.
- Distinct, playable types, with longer usefulness for Bug Pokémon and attention to late-game Flying options such as Pidgeot ([ADR-0014](docs/adr/0014-type-identity-and-campaign-viability.md)).

See [ADR-0001](docs/adr/0001-project-direction.md) for the initial scope, [ADR-0004](docs/adr/0004-original-151-roster.md) for the roster boundary, and [ADR-0005](docs/adr/0005-team-rotation-and-boss-mastery.md) for team rotation and boss mastery. [ADRs 0006–0008](docs/adr/README.md) define levelling, scouting and consumable/loss policies. [ADR-0009](docs/adr/0009-vgc-style-ai-information.md) defines the boss AI information standard. [ADRs 0010–0011](docs/adr/README.md) define mixed battle formats, Set rules and cross-generation move curation. [ADR-0012](docs/adr/0012-ability-and-typing-rebalance.md) defines ability/typing changes; ADR-0004 now excludes regional and Mega forms. Detailed implementation remains open; numerical category limits and the overall quota are delegated to balance testing.

## Working in this repository

- [Agent instructions](AGENTS.md): how coding and design agents work.
- [Decision index and workflow](docs/adr/README.md): how consequential choices are recorded.
- [ADR template](docs/adr/template.md): starting point for a new decision.
- [Foundation proposal](docs/adr/0003-source-foundation.md): the next engineering investigation; not an accepted engine choice.

The recommended next implementation task is to audit source foundations, verify a reproducible baseline build, and record the selection with evidence. The physical/special split, caps and speed controls are the first technical capabilities to prove. The exact playable milestone remains to be decided.

## Build and testing

There are no game build or test commands yet. Add verified setup instructions after the foundation audit. Do not infer playable status from the presence of source files or a successful documentation change.

## Contributions and distribution

Preserve upstream attribution and evaluate reuse terms before importing code or assets. No project-wide license has been selected. Keep ROM images, saves and credentials out of git. Patch format, base-ROM checksum and supported release targets will be documented when packaging is implemented.
