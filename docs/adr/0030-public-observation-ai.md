# ADR-0030: Choose actions from a bounded public observation

- Status: Accepted
- Date: 2026-09-06
- Decision authority: Engineering choice delegated by the owner's full-project mandate
- Implementation: Native adapter and action hooks implemented; boundary and tactical tests pass; campaign balance pending
- Supersedes: None
- Superseded by: None

## Context and decision

ADRs 0009 and 0024 require capable opponents without privileged player information. The pinned expansion AI reads exact player stats and can run damage calculations against authoritative battle state. Auditing isolated reads in that prediction pipeline would leave a large, changing implicit boundary.

Use a separate campaign adapter that reads permitted state into value-only options, followed by a pure deterministic scorer. Bypass the legacy prediction, random iteration order, item selection and replacement paths for campaign trainer battles. Keep the upstream controller for wild Pokémon and excluded multiplayer/facility modes. Test opt-ins exercise this path without changing unrelated upstream fixtures.

The AI knows its own party, exact stats and remaining supplies. For the player's team it may use disclosed species, level, acquired nature, moves, innate ability, held item and active capability/passive assignments, together with observable battle state. Player HP is projected through the same 48-pixel bar used by the interface. Sleep duration, toxic counters, numeric player stats, IVs, permanent training, inactive collections, pending actions/targets and future random draws are excluded. No estimated data is written into the real party or battlers.

Estimate player stats from species, level, public nature and a common public prior. With ADR-0031's random range the IV prior is 23; assume zero training, accepting prediction error rather than discovering hidden development. Ordinary damage uses an accuracy-adjusted median estimate. The estimates are deliberately incomplete and require campaign testing; fairness alone does not establish strong play.

Evaluate both doubles actors jointly, including friendly fire, duplicate targets, Protect/Helping Hand and shared item/party resources. Stable option ordering breaks ties without accessing or advancing battle RNG. Actual engine legality and execution still govern the chosen move, target, switch or item. Revalidate supplies at execution because earlier actions can change target eligibility.

## Alternatives and consequences

Sanitizing copies and calling the upstream simulator would retain hidden dependencies and mutation risk. Using exact player stats would violate the accepted information boundary. A deterministic observation scorer makes invariance tests and review practical, but requires explicit support for relevant tactical interactions and can become predictable. Expand its estimates based on observed campaign failures without widening its information access.

The adapter recognizes common immunities, damage categories, public effects, speed/priority, status, setup, recovery, weather, terrain and switching. Every move/ability interaction is not modeled. Full-battle tests and returning-player trials must detect repeated tactical failures, ineffective team plans and avoidable preparation traps.

## Evidence

See [AI validation](../validation/fair-ai.md) and the native fixtures in `engine/test/sc_ai/`. Tests perturb hidden data, same-band HP, pending decisions and RNG; verify unchanged authoritative state; and test public nature, coverage, immunity, switching and doubles choices. Strong integrated public-effect providers and an actual Brace-driven category choice have also passed the native runner. These are executed tactical cases, not an organic campaign or human balance result.
