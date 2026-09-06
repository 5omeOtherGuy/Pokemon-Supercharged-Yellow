# Yellow opening operator route

Candidate source: 195bd43e. This is a route plan, not completed gameplay evidence.
Use only the frozen ROM/ELF supplied by the coordinator, the read-only headless
mGBA driver, ordinary keys and a previously absent explicit save path.

1. Observe boot/title, start New Game, complete character naming and tutorial.
2. Leave the bedroom and house; walk north in Pallet. Observe Oak's wild Pikachu
   scene, laboratory arrival, rival taking Eevee and receipt of level-5 Pikachu.
3. Play the forced first rival battle. Record the actual result and subsequent
   script return/healing; do not reset to conceal a loss.
4. Walk Route 1, visit Viridian's Center and Mart, receive Oak's Parcel and return
   to Oak for the Pokedex. Inspect service discovery and ordinary party access.
5. Use obtained or bought Poke Balls for a first catch; inspect its party entry,
   deposit/retrieve it through the ordinary Center PC.
6. Save through the normal menu. Wait for explicit save-complete text and extra
   released frames, quit gracefully, hash the save, start another process with
   that exact save, and inspect Continue and the restored state.
7. Progress Route 2/Forest/Pewter and Brock if feasible; capture any blocking
   behavior and report source-specific issues to the coordinator.

For each process retain a `script --flush` terminal transcript containing echoed
commands and JSON responses. PNG screenshots come directly from the emulator's
240x160 framebuffer. `read` commands, if needed to diagnose a fault, are read-only
and should use matching ELF symbols; they are not substitutes for observed UI.
No cheats, save injection, savestates, memory writes or gameplay debug commands.
