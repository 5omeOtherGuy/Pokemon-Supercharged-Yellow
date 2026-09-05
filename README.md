# Pokémon Supercharged Yellow

A Kanto ROM-hack project inspired by Pokémon Recharged Yellow: familiar Pokémon and places, greater strategic depth and challenge, and balance that preserves distinct identities.

## Current state

The repository contains governance and design documentation only. No engine has been selected or imported, no game build has been verified, and no playable prototype exists. Recharged Yellow source was reported unavailable in the project brief and is not a dependency.

## Design direction

- Android emulator play is required; GBA and NDS foundations are eligible, and original-console hardware support is not required. See [platform requirements](docs/adr/0025-android-emulation-platform.md).
- Use FireRed/LeafGreen's Kanto map and layout, adjusted where necessary to fit Yellow, with Pikachu as starter and Yellow's main encounters and plot anchors. Recharged Yellow is the reference hack. Trainer content can change substantially outside those anchors; wild encounters should remain familiar where practical. Polish Kanto before extras; no Sevii Islands are currently planned. See [content baseline](docs/adr/0026-yellow-kanto-content-baseline.md).
- Original 151, original forms only in the main campaign; all 151 obtainable in one playthrough without trading, with Mew under the truck. No regional variants or Mega Evolutions. See [roster and collection](docs/adr/0004-original-151-roster.md).
- A trusted core with rotating specialists, substantial preparation and repeated attempts to master major bosses. See [experience target](docs/adr/0017-trusted-core-and-boss-mastery.md) and [levelling](docs/adr/0006-substantial-levelling-investment.md).
- Individual traits, permanent battle training, earned Pokémon capabilities and separate trainer builds; no obedience restrictions. See the [progression ownership map](docs/adr/README.md#progression-ownership).
- Full boss scouting, reciprocal disclosure of active trainer passives and assigned Pokémon capabilities, bounded AI information, preselected limited consumables and retained loss costs. See [information and supplies](docs/adr/README.md#current-decision-map).
- Substantial singles and doubles content with Set rules; curated cross-generation moves including weather and terrain, forgettable HMs, reusable TMs, justified ability/typing changes, Fairy and the standard modern chart. See the [rules and balance records](docs/adr/README.md#current-decision-map).
- One initial difficulty, level caps, physical/special split, restrained customisation and adjustable battle speed. See the [project charter](docs/adr/0001-project-direction.md).

Accepted records describe decisions, not implemented or tested features. Specific effects, numerical balance and several product boundaries remain open in the owning records.

## Working in this repository

- [Agent instructions](AGENTS.md): operational constraints and verification.
- [ADR index, reading map and lifecycle](docs/adr/README.md): current policies and preserved history.
- [ADR template](docs/adr/template.md): starting point for consequential decisions.
- [Design review](docs/design-review.md): audit of ADRs 0001–0024, fixes, remaining decisions and recommended next work; prepared against the [review brief](docs/design-review-brief.md).
- [Foundation proposal](docs/adr/0003-source-foundation.md): a source audit, not an accepted engine choice.

Documentation is ready for foundation investigation under the platform and content constraints in ADRs 0025–0026. The first playable milestone remains unaccepted. The review separates remaining owner decisions from engineering investigations and balance testing.

## Build and testing

There are no verified game build or test commands. Add reproducible setup instructions after the foundation audit. Documentation checks do not establish playable status.

## Contributions and distribution

Preserve upstream attribution and evaluate reuse terms before importing code or assets. No project-wide license has been selected. Keep ROM images, saves and credentials out of Git. Patch format, base-ROM checksum and supported release targets remain to be documented when packaging is implemented.
