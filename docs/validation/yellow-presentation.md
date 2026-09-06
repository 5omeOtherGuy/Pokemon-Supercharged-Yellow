# Supercharged Yellow title presentation

The title source replaces the inherited Charizard/FireRed composition with the included Pokémon logo, original Pikachu front sprite, warm yellow field and navy native lettering. This is title-only work: the existing copyright/intro, main menu, new-game opening, saves and campaign remain separate.

## Source and design

Based on integrated source `d2dd72da`. Assets remain unchanged: `graphics/title_screen/pokemon_logo.png` and its canonical palette provide the version-neutral logo; `CreateMonPicSprite(SPECIES_PIKACHU, ...)` selects the engine's original Pikachu picture. No regional/partner form or random Pokémon is generated. The native title text says “SUPERCHARGED” and “YELLOW”. The included title music remains; entry plays Pikachu's cry. Existing upstream notices remain, with an additional native `(C)1995-2004 GAME FREAK inc.` footer on the title.

The 240×160 composition uses a centered Pokémon logo above an asymmetric subtitle/Pikachu pair. A navy footer keeps PRESS START legible; it changes color subtly instead of disappearing. Pikachu moves upward one pixel briefly per two-second cycle. A short black fade replaces the inherited flame/slash flashing. A/B/Start can skip that reveal; a fresh A/Start then enters normally. The title waits 2,700 active frames before fading back to the existing copyright/intro callback.

The four presentation colors are GBA RGB5 yellow `(31,27,9)`, navy `(3,6,12)`, ochre `(23,17,4)` and cream `(31,30,24)`. Native font metrics center the text. No custom raster art, asset rewrites, build-system changes or developer HUD are introduced. The original exported title-art symbols have no consumers outside the replaced title implementation (`rg` across `engine/src`); their unused declarations elsewhere do not create a link dependency.

## Behavior and resources

The pure input decision retains save-clear `Up+B+Select` precedence, then enabled RTC-reset `Left+B+Select`, then fresh A/Start, then inactivity. B or Select alone does not launch another mode. The original callback targets handle Continue/New Game/Options, save-clear confirmation and RTC. All departures release the native window buffer, Pikachu picture allocation, BG tilemap bindings and title task, and clear the title VBlank callback. A subsequent screen owns its own resources.

The logo uses unchanged 8bpp tiles at char base 0 and BG map 31; text uses 4bpp char base 2 and map 30. The 30×20 text window occupies tiles 1–600, ending below both tilemaps. Logo palette entries 0–223 are retained except transparent backdrop entry 0; text uses slot 15. Pikachu has an independent OBJ palette.

## Executed checks

- RED: compiled input regression failed because the new production decision header did not yet exist.
- GREEN: `python3 -m unittest discover -s tests/presentation -v` passed one compiled C test with ten entry, chord, held-key and idle assertions, using the real engine button masks.
- `python3 tests/presentation/compile_objects.py --generated-engine /home/someotherguy/projects/Pokemon-Supercharged-Yellow-worktrees/full-game/engine` passed the real text preprocessor and ARM GCC 13.2.1 object compilation with warnings as errors. Temporary objects were removed automatically. The generated engine was read only.
- Scope/source review found no external C consumer of the removed FireRed/LeafGreen title-art definitions. The inherited main-menu, save-clear and RTC callback interfaces are unchanged.

## Remaining runtime validation

Source compilation is not rendered-screen or interaction evidence. After integration, freeze the production ROM/ELF with checksums, then use a fresh explicitly named emulator save and ordinary controller inputs. Capture the settled title, fade skip, A and Start entry, new-game menu, Options entry/return, title reentry, save-clear chord/cancel and inactivity intro return. Use an existing task-owned organically created save only for Continue/reload checks, recording its exact provenance. RTC eligibility may require a separately labeled fixture; never claim that path played if only the input decision was tested. Check repeated reentry for sprite/window leaks. Android screenshots and audio listening remain separate from the muted desktop harness.
