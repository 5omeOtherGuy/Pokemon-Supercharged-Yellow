# Pokémon Supercharged Yellow

A Kanto ROM-hack project inspired by Pokémon Recharged Yellow: familiar Pokémon and places, greater strategic depth and challenge, and balance that preserves distinct identities.

## Current state

The GBA engine, Yellow campaign source and Supercharged systems are being integrated and tested. **The complete game and release are not ready.** See [project status](docs/project-status.md) for implemented features, executed checks and remaining requirements. Recharged Yellow is a design reference, not a source dependency.

An **owner playtest build with PC cheat tools** is packaged locally, under the owner's coding-first direction. Source `131c263d` builds reproducibly and boots to the game menu on the Android VM target. See [player instructions](docs/player-playtest.md), [PC shortcuts](docs/testing-cheats.md) and [validation](docs/validation/owner-playtest.md). Agent-controlled campaign walkthroughs are paused; coding, automated tests and bounded battle simulations support the owner's playtesting.

## Design direction

- Android emulator play is required; GBA and NDS foundations are eligible, and original-console hardware support is not required. See [platform requirements](docs/adr/0025-android-emulation-platform.md).
- Use FireRed/LeafGreen's Kanto map and layout, adjusted where necessary to fit Yellow, with Pikachu as starter and Yellow's main encounters and plot anchors. Recharged Yellow is the reference hack. Trainer content can change substantially outside those anchors; wild encounters should remain familiar where practical. Polish Kanto before extras; no Sevii Islands are currently planned. See [content baseline](docs/adr/0026-yellow-kanto-content-baseline.md).
- Original 151, original forms only in the main campaign; all 151 obtainable in one playthrough without trading, with Mew under the truck. No regional variants or Mega Evolutions. See [roster and collection](docs/adr/0004-original-151-roster.md).
- A trusted core with rotating specialists, substantial preparation and repeated attempts to master major bosses. See [experience target](docs/adr/0017-trusted-core-and-boss-mastery.md) and [levelling](docs/adr/0006-substantial-levelling-investment.md).
- Individual traits, permanent battle training, earned Pokémon capabilities and separate trainer builds; no obedience restrictions. See the [progression ownership map](docs/adr/README.md#progression-ownership).
- Full boss scouting, reciprocal disclosure of active trainer passives and assigned Pokémon capabilities, bounded AI information, preselected limited consumables and retained loss costs. See [information and supplies](docs/adr/README.md#current-decision-map).
- Substantial singles and doubles content with Set rules; curated cross-generation moves including weather and terrain, forgettable HMs, reusable TMs, justified ability/typing changes, Fairy and the standard modern chart. See the [rules and balance records](docs/adr/README.md#current-decision-map).
- One initial difficulty, level caps, physical/special split, restrained customisation and adjustable battle speed. See the [project charter](docs/adr/0001-project-direction.md).

Accepted records describe decisions, not proof of implementation or balance. Later execution ADRs select the foundation, campaign boundaries, progression numbers, AI architecture and services. Numerical tuning remains subject to gameplay and human testing.

## Working in this repository

- [Agent instructions](AGENTS.md): operational constraints and verification.
- [ADR index, reading map and lifecycle](docs/adr/README.md): current policies and preserved history.
- [ADR template](docs/adr/template.md): starting point for consequential decisions.
- [Design review](docs/design-review.md): audit of ADRs 0001–0024, fixes, remaining decisions and recommended next work; prepared against the [review brief](docs/design-review-brief.md).
- [Foundation decision](docs/adr/0028-pinned-kanto-foundation.md): pinned source, toolchain and audit evidence.

The owner's full-project execution mandate supersedes the review brief's documentation-only scope. Historical proposals and review findings remain preserved; follow their later resolutions.

## Build and testing

The reference Ubuntu toolchain is ARM GCC 13.2.1 20231009, binutils 2.42 and newlib 4.4.0. Install `build-essential`, `binutils-arm-none-eabi`, `gcc-arm-none-eabi`, `libnewlib-arm-none-eabi`, `libpng-dev`, `libelf-dev`, `libmgba-dev`, `git` and `python3`. The imported source is pinned in [UPSTREAM.md](UPSTREAM.md).

```sh
make -C engine firered -j2
bash tests/effects/run_native.sh
bash tests/test_tools/run_native.sh
bash tests/simulation/run_native.sh
python3 -m unittest discover -s tests/progression -v
```

To build and package the private owner version with a second clean-source reproducibility check:

```sh
python3 tools/build_playtest.py --repro-check
```

It outputs a local `.gba`, ZIP, guides and manifest in ignored `dist/owner-playtest/`. The explicit `SC_TEST_TOOLS=1` build uses separate output/object paths; ordinary builds default to zero. Do not publish the local ROM or ZIP. A public release patch remains separate work.

The build produces local `engine/pokefirered.gba`; that inherited filename does not certify a release. Native tests execute real GBA code in mGBA. Check the executed count: the runner can return success for an empty filter. The [validation records](docs/project-status.md) distinguish native, host, diagnostic-fixture and ordinary gameplay evidence. The owner build passed a second clean-source byte-for-byte rebuild. Complete campaign and final release validation remain pending.

## Contributions and distribution

Preserve upstream attribution and component notices; see [UPSTREAM.md](UPSTREAM.md). No project-wide license has been selected. Keep ROM images, saves and credentials out of Git. No release patch is published yet. Packaging must identify the exact supported base checksum and prove patch application, saves and Android playability before release.
