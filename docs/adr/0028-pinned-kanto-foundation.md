# ADR-0028: Build on the pinned expansion Kanto port

- Status: Accepted
- Date: 2026-09-06
- Decision authority: Engineering selection under the owner's full-project execution mandate
- Implementation: Source imported; representative unmodified build, battle and save/reload passed; project integration underway
- Supersedes: [ADR-0003](0003-source-foundation.md)
- Superseded by: None

## Context

The required Yellow adventure needs familiar FRLG Kanto maps, modern battles and extensive new progression systems. The foundation audit inspected source rather than relying on feature lists. Expansion 1.17.0 contains a substantive FireRed campaign port, reducing the earlier assumed cost of using an Emerald toolkit.

## Decision

Use RHH pokeemerald-expansion **1.17.0**, commit `7644501f3065b38aea2cfc64b5b58ca011bce4f5`, with its `firered` build and Kanto region. The editable snapshot lives in `engine/`; [UPSTREAM.md](../../UPSTREAM.md) records provenance and omitted legacy transfer ROMs. Preserve upstream credits and component notices. Ship patches, never ROMs or player saves.

The reference host toolchain is Ubuntu's ARM GCC **13.2.1 20231009** (`gcc-arm-none-eabi` package `15:13.2.rel1-2`), binutils **2.42**, newlib **4.4.0**, libpng and libelf development headers. Run `make firered -j2` in `engine/`. For engine tests, explicitly use `make BUILD=firered TESTS='<matching prefix>*' check -j2`. A zero-test invocation is not a pass. Final release tooling must pin/check prerequisites and prove a second clean build and patch round-trip.

Remove bundled Berry Fix, e-Reader and Colosseum transfer-program dependencies and their launch paths. These obsolete external-device functions do not belong to this solo Kanto game. No downloaded commercial base is necessary to compile the engine source; patch installation will separately identify an exact supported base.

## Alternatives considered

| Inspected source | Pin | Reason not selected |
| --- | --- | --- |
| pret/pokefirered | `c75f352304d529f6ba92d4f74b9cf8b5c3810788` | Accurate Kanto baseline; materially more modern battle integration |
| Deokishisu/FRLG-Plus | `f198fb944256778dd022946ba4be6675b287472f` | Useful QoL, narrower modern mechanics/test foundation |
| egalleta/FRLG-2Plus | `86a1bb3a0b0f50c6626ac1c2023a21d88bb3c3cd` | Split and FRLG improvements; more required custom integration than the expansion port |
| pret/pokeheartgold | `0985e8718df4f25e64d6507d89c0c97c0d288981` | Permitted NDS candidate; different campaign/rendering baseline and greater Yellow/FRLG conversion cost |

Recharged Yellow remains a design reference, not a source dependency. A specific reference release comparison is still outstanding.

## Evidence and validation

- Source inspection found 421 Kanto maps versus 425 in pinned pret/FRLG, the four omitted maps being unused prototype Sevii maps. Of matching layout blobs, 640 were byte-identical and none differed; unmatched identifiers were investigated separately. This is map reuse evidence, not full campaign validation.
- Oak, Brock, Champion and Hall of Fame scripts are substantive. Fame Checker portions are commented out. All Yellow adaptations and Supercharged systems remain implementation work.
- An isolated unmodified `make firered -j2` passed. Its ROM SHA-256 is `ae61d62c3387de2c4e7fc5e4df174372e6f2da7b72f7e1bcb072820dd937a821`. Link usage: EWRAM 226700/262144 bytes, IWRAM 28628/32768, ROM 26954084/33554432. Memory headroom must be rechecked after integration.
- Three actual Electric Terrain engine tests passed: power boost, duration and sleep protection for grounded Pokémon. This does not establish complete split/Fairy/weather coverage.
- The ordinary-input [emulator walkthrough and replay](../validation/emulator-harness.md) created a character, selected the upstream Squirtle, won the rival battle, waited for explicit save completion, restarted and restored the lab and level-6 party. No memory writes or injected saves were used.
- Upstream `SaveBlock*` tests passed two size checks and failed the SaveBlock1 expected-size fixture: the fixture expects Emerald's 15568 bytes, while FRLG uses 15756. Both fit four 3968-byte sectors. This is an unresolved test expectation, distinct from the successful in-game save/reload and from future custom save tests.
- The source policy regression now rejects complete ROMs in tracked files and embedded assembly dependencies; both checks pass after legacy launcher removal.
- The imported source after launcher removal also built successfully: ROM SHA-256 `7d46af7eae40879091204e2cd031d165ce690bad687bd6684b79cb5121009fc6`, EWRAM 226656 bytes, IWRAM 28556, ROM 26755368. Its ordinary-input replay produced 450 successful opening responses; visual inspection confirmed explicit save completion. A fresh process ran the 15-response Continue trace and restored level-6 Squirtle at 22/22 HP. This still uses the upstream campaign opening.

## Consequences and follow-up

The modern engine is reusable; its defaults are not our rules. Restrict all reachable species/forms and evolution paths, replace Yellow events and acquisition, implement services/progression/supplies/scouting, enforce Set and milestone caps, and remove multiplayer/external-event access. Audit inherited cheats/debug menus and non-Kanto routes.

The legacy AI reads hidden player statistics and uses simulation helpers with broad battle-state access. An explicit public observation boundary is required. SaveBlock1 has only 116 spare bytes; new individual data must preserve the 80-byte boxed Pokémon format, and new trainer state must use a versioned, tested allocation. These are implementation constraints, not evidence the requested systems exist.

Android 11/API30 x86_64 with RetroArch 1.22.2 and mGBA `0.11-219-e31759b` has been provisioned and the core loads. No game has yet been tested there. Complete gameplay, human experience, Android game validation and release reproducibility remain mandatory.
