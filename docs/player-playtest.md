# Supercharged Yellow: owner playtest

This build contains the implemented Kanto campaign and PC test tools. The supplied package is for owner testing. The complete player journey and final balance remain unvalidated. [Validation](validation/owner-playtest.md) lists executed checks and remaining gaps.

## Play on Android

1. Copy `Pokemon-Supercharged-Yellow-playtest.gba` from the supplied local folder or ZIP to your phone.
2. In RetroArch, load the **mGBA** core, then choose **Load Content** and select the `.gba`. This owner ROM has booted to New Game/Option under RetroArch/mGBA on the project's Android 11 emulator target. Physical-phone testing remains yours.
3. Use the on-screen GBA controls. Save through the game's Start menu. Let the save finish before closing content. Keep the ROM filename stable so the emulator finds its matching save.

No external cheat codes are needed. The ROM is compiled locally from this source; the local ZIP is for your testing and must not be uploaded as a public ROM release. Public patch packaging remains separate.

## Fast testing setup

Play the Pikachu opening and return Oak's parcel first when testing normal story progression. Then open a PC and choose **Test tools**. The bedroom PC also exposes the tools immediately if you want earlier shortcuts.

- **Refill supplies** gives 99 Rare Candies, Master Balls and other useful supplies. **Give an item** lets you choose any item and quantity.
- **Infinite owned items** keeps ordinary consumables; pair it with **Ignore level cap** for rapid Rare Candy levelling. The level editor can also directly set 1–100.
- **Give all original 151** adds a complete set into empty party/box slots. Use Pokémon Storage to choose your team, then **Edit party Pokémon** for its moves, nature, IVs, training, ability slot and other fields.
- **Prepare party at badge cap** creates a strong party at the current legal stage: cap level, perfect IVs, maximum stage training and restored health/PP. Teach the moves you want through the editor.
- **Unlock builds + Practice Points** provides all earned capabilities for the current party and all trainer passives. Assign them in a Center's upstairs lounge.
- **Travel keys and HMs**, **Warp in Kanto**, **Set badge count**, **Avoid trainer sight** and **No walking encounters** shorten travel and progression testing.
- **Free player battle supplies** removes the player's trainer-battle preparation/quota restriction. Leave it off when checking normal consumable limits.

Up/Down selects rows. In number pickers, Up/Down changes by 1, Left/Right by 10 and L/R by 100; A applies and B cancels. Save normally to retain edits and switches. See the [complete PC guide](testing-cheats.md) for exact limits.

Warps and badges do not mark story scenes complete. Return to the relevant story event if an NPC still requires it. To test ordinary access and preparation, keep those switches off and use a separate save from an unrestricted speedrun. Turning cheats off does not undo granted Pokémon, items, badges or stats.

## Useful feedback

For a defect, include the ROM hash/build revision from `BUILD-MANIFEST.json`, location and event, badge count, party, enabled cheats, what you selected and what happened. A copy of the in-game save just before the issue is especially useful; keep saves private.

Prioritize campaign gates and rewards, gyms and rematches, singles/doubles supply use, learned moves and training, save/reload, and obtaining the remaining Pokémon. Note which fights felt trivial or impractical and the team you used. Cheat-granted resources help test behavior; preparation-time and normal-difficulty observations need an ordinary setup.

## Rebuild

Install the dependencies and pinned compiler described in the repository README, then run from a clean committed checkout:

```sh
python3 tools/build_playtest.py --repro-check
```

The script compiles with `SC_TEST_TOOLS=1`, checks the GBA header, independently rebuilds a clean source archive, requires identical ROM hashes, and packages the ROM, guides and manifest under `dist/owner-playtest/`. Logs are under `build/playtest-package/`. To create a later build alongside an existing one, pass a different `--output` directory.
