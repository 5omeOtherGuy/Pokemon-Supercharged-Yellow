# Owner test tools

These shortcuts are for the owner's coding-first test build (ADR-0036). They intentionally bypass campaign restrictions. They do not establish a completed playthrough or balanced game.

Build with `make -C engine BUILD=firered SC_TEST_TOOLS=1 -j2`. The coordinator's build configuration produces `engine/pokefirered-playtest.gba` and isolates its object files from the ordinary build. `SC_TEST_TOOLS` defaults to zero. The inherited Hoenn developer menus remain disabled.

## Open and operate

Use any standard PC, including the PC in the player's upstairs bedroom from the beginning, and choose **Test tools**. The test PC always shows five choices; Hall of Fame remains unavailable until Champion status. Boxed Pokémon can be withdrawn through the same PC, then edited in the party.

Use Up/Down to select a row and A to enter. Number screens show the proposed value and species/move/item/nature/ability name where applicable. Up/Down changes one, Left/Right changes ten, and L/R changes one hundred. A applies the displayed choice; B cancels. Native nickname entry uses the ordinary naming screen. Changes persist only after an ordinary game save. For assessing the normal opening, obtain Pikachu and finish Oak's parcel before mass grants or warps; early cheats do not complete those scene scripts.

## Tools

| Tool | Behavior |
| --- | --- |
| Edit party Pokémon | Species 1–151, level 1–100, all 25 natures, six IVs 0–31, six training values 0–255, actual official ability slot, four moves and current PP, held item, shiny appearance, valid species gender, friendship 0–255 and nickname. |
| Give a Pokémon | Creates the chosen original species at level 1–100 in the first empty party or box slot. Enables the party menu, records it seen/caught and preserves existing Pokémon. |
| Give all original 151 | Adds one of each species at the selected level. Requires 151 empty party/box slots before any write; repeated use adds another set. |
| Give an item | Add 1–999 of a displayed engine item. Bag pocket/stack capacity still applies. Full-bag rejection adds nothing. Some engine items are irrelevant to Kanto. |
| Refill supplies | Tops up to 99 each: Rare Candy, Master Ball, Ultra Ball, Full Restore, Max Revive, Max Elixir, Escape Rope, Max Repel, Lum Berry and Sitrus Berry. Reports partial failure if a pocket fills; retry after freeing space. |
| Travel keys and HMs | Adds Bicycle, all rods, Vs. Seeker, Town Map, S.S. Ticket, Poké Flute, Card/Lift/Secret Keys, Silph Scope, Gold Teeth and HM01–05, without duplicates. Also enables running shoes and the Pokédex menu. Possession does not mark their associated stories complete. |
| Heal party | Heals non-egg party Pokémon and restores PP. |
| Prepare party at badge cap | Sets the current badge/Champion cap, all IVs to 31 and all six training values to that badge ceiling, then heals. It can lower an overlevelled Pokémon or excessive training. Moves stay unchanged; use the editor to teach moves. |
| Unlock builds + Practice Points | Gives the current party all twelve earned capabilities, unlocks all eight trainer passives, and sets Practice Points to 9,999,999. Assign effects in the ordinary Center lounge; assignment budgets remain. Withdraw a boxed Pokémon and repeat to unlock its capabilities. |
| Set badge count | Owns the first N badges in campaign order, clearing later badges. Does not complete gym/story scripts or grant Champion status. Existing training and active builds are not automatically reduced. |
| Max money | Sets money to 999,999. |
| Recharge Vs. Seeker | Sets its charge to 100 steps; does not invent defeated trainers or rematch entries. |
| Warp in Kanto | Travels to Pallet, the nine mainland cities/island, Indigo Plateau or Route 4/10 Center locations using canonical heal-location destinations. Does not clear story gates, set respawn, or mark Fly destinations visited. |

Individual edits recalculate stats without automatically healing. Species edits preserve nickname (unless it is the old default species name), level, moves, held item, IVs and personal development; an absent ability slot resets to slot zero. Nature/gender edits use the encrypted personality migration API and preserve custom capability/training bytes. Invalid species genders, absent abilities, out-of-range values, PP above the move's current maximum, and removal of a Pokémon's final move are rejected.

The test editor deliberately permits stronger-than-campaign training and arbitrary normal engine moves. It does not edit eggs, bad eggs, original-trainer identity, encounter provenance, ribbons, contest fields, or raw save flags/variables. Contest fields store Supercharged personal development and must never be edited with an upstream contest editor.

## Persistent switches

All switches start OFF. Their bits occupy `ScTrainerProgress.reserved[0]`; no save structure grows and the existing trainer CRC covers them. Normal builds ignore these bits. Turning a switch off does not reverse Pokémon, money, badge or item changes already made.

| Switch | Intended integrated behavior |
| --- | --- |
| Infinite owned items | Keep owned ordinary bag consumables when used and restore original held consumables after battles. Tossing and story key-item consumption remain functional. Refill packs provide initial stock. |
| Ignore level cap | Allow normal EXP and Rare Candy to reach level 100. The explicit editor always offers 1–100. |
| No walking encounters | Suppress random walking/surfing encounters; fishing and scripted/static encounters remain available. |
| Free player battle supplies | Waive the player's trainer-battle selection/quota restrictions. NPC supply limits remain. |
| Avoid trainer sight | Suppress automatic sight-triggered battles; direct interaction remains available. |

The coordinating change owns the gameplay hook implementation. The switch query API is `ScDebugOptionEnabled(enum ScDebugOption)`; this module provides persisted guarded state and its menu.

## Validation

The portable request-bounds test was first compiled without its implementation and failed for that missing implementation. It passes after implementation. It exercises valid endpoints, invalid adjacent values, stat/move index overflow, unknown fields and number-picker clamping using the cartridge's shared validator.

`python3 -m unittest discover -s tests/cheats -v` executes that host test. The native tests in `engine/test/sc_cheats/editor.c` exercise actual encrypted Pokémon writes, all-nature metadata preservation, invalid/battle-context rejection, atomic full-storage refusal, complete 151-species placement, level/species recalculation, PP rejection, and CRC-protected switches. They compile in both feature configurations; the coordinator owns execution in the integrated mGBA runner. Use `SC_TEST_TOOLS=1 SC_TEST_CAMPAIGN=1 TESTS='SC cheats:*'` for the enabled cases and the ordinary configuration for the disabled-build case. Always check the executed count.

ARM object compilation passed for the model, PC screen, PC menu and native tests with the feature both enabled and disabled. Full linking and native execution are integration checks. No agent-controlled campaign playthrough or manual UI acceptance is claimed for these tools; the owner handles gameplay acceptance.
