# Source provenance

The `engine/` baseline is a source snapshot of [RHH (Rom Hacking Hideout) pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion) 1.17.0, commit `7644501f3065b38aea2cfc64b5b58ca011bce4f5`, imported for FireRed/Kanto integration on 2026-09-06. Imported blobs were checked against the pinned upstream. Three legacy multiboot ROM images are deliberately omitted: `data/mb_berry_fix.gba`, `data/mb_colosseum.gba`, `data/mb_ereader.gba`. Their external-device launchers and embedded-image assembly dependencies have been removed. Legacy e-Reader entry is rejected by the existing invalid-action menu; Berry Fix shortcuts and GameCube download execution are absent.

Subsequent Supercharged changes are visible against the source-import commit. The snapshot has no upstream Git ancestry; fetch the pin separately for historical comparison or future updates. Root project documentation is separate from preserved upstream documentation in `engine/`.

RHH builds on pret’s Pokémon Emerald decompilation and incorporates FRLG content. Preserve [all upstream credits](engine/CREDITS.md), [upstream README](engine/README.md), tool licenses and embedded notices. The README invites hack-base use and requests RHH attribution. No blanket root license was found; this project does not relabel all upstream code/assets as MIT/GPL. Bundled test-tool provenance remains in [tools/mgba/README.md](engine/tools/mgba/README.md).

No game ROMs or saves belong in Git or public releases. Release gameplay artifacts will be patches. Tool licenses remain specific to their components.

Reference sources, not additional engine dependencies: [pret/pokefirered](https://github.com/pret/pokefirered/tree/c75f352304d529f6ba92d4f74b9cf8b5c3810788) for FRLG maps and [pret/pokeyellow](https://github.com/pret/pokeyellow/tree/e89ead154b9968aa50eed9328ff2b38b6c194382) for Yellow plot/encounters. Recharged Yellow remains the design reference; no source or binary from it was imported, and its exact comparison version has not yet been verified.

Import is an engineering milestone. An isolated unmodified upstream build compiles and boots; complete campaign behavior and Supercharged’s gameplay are not established by this import. See [project status](docs/project-status.md).
