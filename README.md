# Pokémon Supercharged Yellow

A Kanto ROM-hack project inspired by Pokémon Recharged Yellow: familiar Pokémon and places, modern physical/special move categories, greater strategic depth, and balance that preserves distinct identities.

## Current state

Project governance is established. No engine has been selected or imported, no game build has been verified, and no playable prototype is available. Recharged Yellow source is unavailable and is not required for this project.

## Confirmed direction

- One initial difficulty setting.
- Level caps.
- Physical/special split.
- Restrained player customisation and useful quality-of-life improvements, especially adjustable battle speed.
- Challenging encounters and well-designed Pokémon without making every species equally strong or viable.

See [ADR-0001](docs/adr/0001-project-direction.md) for scope, provenance and unresolved choices.

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
