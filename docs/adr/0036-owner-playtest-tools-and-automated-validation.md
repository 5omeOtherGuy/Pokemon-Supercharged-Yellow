# ADR-0036: Deliver an owner playtest build with PC shortcuts

- Status: Accepted
- Date: 2026-09-06
- Decision authority: Explicit owner instruction to prioritize coding and battle simulations, stop agent-controlled campaign playthroughs, and provide a playable cheat-enabled build for their testing
- Implementation: Implemented and packaged at source 131c263d; host/native checks, clean identical rebuild and Android title/menu boot pass; owner gameplay validation remains
- Supersedes: None (test-build exception; ordinary campaign policies remain)
- Superseded by: None

## Decision

Prioritize completing and repairing code using source checks, automated engine tests and bounded battle simulations. Stop allocating agents to continuous campaign gameplay. The owner will perform the next human playtest. Full human/campaign acceptance is not a prerequisite to delivering this clearly identified test build; it remains necessary before claiming the finished game is validated.

Provide `SC_TEST_TOOLS=1` as an explicit FireRed/Kanto build option. It produces a separately named ROM and uses separate normal/native object directories. Default builds keep the tools disabled. Offer a **Test tools** entry at standard PCs and the starting bedroom PC, without enabling the unrelated inherited developer menu.

The tools create items and original-151 Pokémon, edit meaningful individual battle fields, refill supplies, heal and prepare parties, unlock builds, set badge count, recharge the Vs. Seeker and warp among mainland destinations. They intentionally permit level, nature, training and acquisition edits that normal campaign policy forbids. Individual array/value bounds, encrypted Pokémon checksums and existing occupants must remain protected. Mass acquisition preflights space before writing. Boxed Pokémon are withdrawn for editing.

Five independent switches control infinite owned items, the level-cap bypass, random walking encounters, the player's supply quota and trainer sight. They start off. Store their bits in the existing `ScTrainerProgress.reserved[0]`; preserve the save schema and CRC. Normal builds ignore the bits. Switching a cheat off does not undo prior edits or earnings.

Infinite items preserve owned ordinary bag items while leaving quest-item consumption and explicit tossing available. Restore originally held equipment after a battle, preserving normal within-battle consumption timing. Free supplies bypass only the player's reservation/quota; opponents retain their limits. Warps and badge edits do not fabricate completed story scenes. Document those distinctions in the [test tools guide](../testing-cheats.md).

## Alternatives and consequences

Continuing agent-controlled walking and grinding consumed disproportionate time and usage; the owner explicitly changes that priority. Simulations provide inexpensive, repeatable evidence about specific interactions but cannot establish full campaign traversal or enjoyment. Report their assumptions and results separately from human play.

Blindly enabling the inherited debug menu exposes unrelated region utilities and insufficient individual editing. A focused PC screen reuses native UI and Pokémon APIs while excluding raw contest writes, which would corrupt Supercharged metadata. Arbitrary raw save flags are omitted; deliberate badge and travel shortcuts are easier to understand.

Cheated saves can hide preparation costs, skip intended access restrictions and distort balance. Keep a separate ordinary save when assessing those areas. Do not overwrite existing Pokémon to add a collection, and do not silently treat granted collection or rewards as organic completion evidence.

## Delivery and evidence

Deliver a locally compiled `.gba` for the owner's emulator testing, source/build instructions, hashes and accurate test results. Keep ROMs and player saves out of Git and public releases. Public patch distribution still needs a documented exact supported base and verified application; a private local test ROM does not establish that release requirement.

Run meaningful native editor/save and battle tests, normal-build regressions, build-mode isolation checks and a production test build. Record any unperformed Android, UI, full-journey or human checks explicitly. Further simulations should respond to concrete risks or owner findings rather than recreating the entire battle engine.
