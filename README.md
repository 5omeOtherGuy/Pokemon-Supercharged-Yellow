# Pokémon Supercharged Yellow

A Kanto ROM-hack project inspired by Pokémon Recharged Yellow: familiar Pokémon and places, modern physical/special move categories, greater strategic depth, and balance that preserves distinct identities.

## Current state

Project governance is established. No engine has been selected or imported, no game build has been verified, and no playable prototype is available. Recharged Yellow source is unavailable and is not required for this project.

## Confirmed direction

- Original 151 species only in the main campaign.
- Frequent team rebuilding and repeated attempts to master major bosses.
- Substantial levelling investment for newly caught Pokémon: roughly 20–30 minutes or more.
- Full boss team, move, ability, held-item and bag-consumable scouting before the first attempt.
- VGC-style open-team-list information for boss AI, with hidden stats and pending actions protected.
- Preselected battle consumables with category-based duplicate restrictions and one campaign-wide allowance, its number chosen through testing; boss defeats cost money and spent consumables.
- Player supplies allow HP healing, status cures, PP restoration and temporary stat boosters, but no revives. Only opposing major bosses use bag consumables, with the same maximum as the player.
- One initial difficulty setting.
- Level caps.
- Physical/special split.
- Restrained player customisation and useful quality-of-life improvements, especially adjustable battle speed.
- Challenging encounters and well-designed Pokémon without making every species equally strong or viable.

See [ADR-0001](docs/adr/0001-project-direction.md) for the initial scope, [ADR-0004](docs/adr/0004-original-151-roster.md) for the roster boundary, and [ADR-0005](docs/adr/0005-team-rotation-and-boss-mastery.md) for team rotation and boss mastery. [ADRs 0006–0008](docs/adr/README.md) define levelling, scouting and consumable/loss policies. [ADR-0009](docs/adr/0009-vgc-style-ai-information.md) defines the boss AI information standard. Detailed implementation, category limits and the overall item quota remain open.

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
