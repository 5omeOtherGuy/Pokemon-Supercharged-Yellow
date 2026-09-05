# ADR-0021: Select trainer builds and separate trainer rewards from Pokémon capabilities

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user choices on selectable trainer passives, trainer identity and non-repeatable story rewards
- Implementation: Not started — design policy only
- Supersedes: [ADR-0020](0020-badge-passives-and-trainer-progression.md)
- Superseded by: None

## Context

ADR-0020 accepted intended existing badge benefits, modest additional combat passives and selective opponent capability use. The user now chooses a limited subset of additional trainer passives at a Pokémon Center, explicitly includes training benefits such as increased battle EXP, and restricts non-repeatable notable story encounters to trainer-owned passive rewards. This replaces ADR-0020's narrower scope while carrying forward its opponent progression rules.

## Decision

### Trainer builds and ownership

- Preserve the intended existing badge benefits. The reference game/version and exact inherited effects still require an audit; no engine is selected and no particular inherited multiplier or bug behavior is accepted.
- Additional trainer passives form a selectable build at a Pokémon Center, constrained by one shared trainer capacity budget.
- Each selected passive consumes capacity; stronger passives cost more. The sum of active passive costs must not exceed the trainer's available budget. The active count can vary with the chosen combination.
- Permitted design directions include increased EXP from battles and modest combat benefits, potentially stat boosts. These are categories, not approved numerical effects or a completed perk list.
- Trainer passives must express something the trainer knows or does for the team. Coaching, preparation and training expertise are illustrative design rationales, not approved individual perks.
- Pokémon capabilities express skills learned by that individual. Ownership, descriptions and mechanics must make the distinction understandable within the game's fiction.
- Trainer-owned effects can benefit recruits without giving them personally earned capabilities or filled training points. Badge-wide training-ceiling access is automatic under ADR-0023.
- Trainer passive capacity grows through gym badge acquisition. The starting budget, numerical increases and final campaign budget remain open; equal increments or an exact increase for every badge are not yet specified.
- The shared-budget model and badge-based growth are selected; numerical values, individual passive costs and any reassignment cost remain open. Capacity cost governs simultaneous assignment and does not itself mean spending money or consumable resources.
- Additional passive choices may still come from notable story rewards. Unlocking a passive and increasing capacity are different rewards; this decision selects gym badges as the capacity-growth route.
- Keep trainer capacity separate from each Pokémon's three capability slots. No trainer budget of three or trainer cost range of one to three is selected. Existing badge benefits are not automatically placed inside the additional-passive selection budget.

### Reward sources

- Non-repeatable notable story encounters may grant trainer/player passives, never Pokémon capabilities.
- This restriction concerns the passive reward systems; it does not prohibit ordinary EXP, money, items or story progression rewards.
- Pokémon capability sources must support repeat access so later recruits can earn their rewards. Gyms, Elite Four battles and other notable encounters remain eligible sources where this condition is met.
- Do not solve non-repeatable encounters by giving permanent, individually missable capabilities. Trainer rewards belong to the player and can benefit later recruits.
- Repeatable capability encounters retain one fixed capability for every qualifying Pokémon: entered battle, never fainted during it, and the player's team won. Exact access arrangements for Elite Four and other repeatable sources remain open.

### Opponent capability use

- Major bosses and selected experienced trainers may use Pokémon capabilities within the same three-slot capacity rule.
- Not every Pokémon on such a trainer's team needs capabilities, and slots need not be filled.
- Fully filled capability capacity across an entire team should not be the general campaign norm. Do not assume the player or every opponent must reach maximum loadouts for meaningful play.
- Scale capability use with player progress while keeping it consistent with each trainer's experience, identity and place in the story.
- After completing all gyms and Elite Four battles, the player's accomplishment should be reflected relative to most gym leaders. Do not give every lesser trainer equivalent development merely to mirror the player.
- An occasional trainer near a gym may have an ace with that gym's earned capability, as a plausible expression of local experience. This is a permitted example, not a rule for every nearby trainer.

These are encounter-design constraints, not an automatic scaling algorithm. Exact progression triggers, rematch tiers and loadouts remain open. They do not impose a lower formal slot cap on weaker trainers, require specific player slots to stay empty, or remove the intended challenge from major encounters.

### Opponent trainer-passive builds

- Major bosses and selected experienced trainers use trainer-wide passive builds consistent with their backgrounds.
- Design these builds around the trainer's experience, role and established accomplishments. Selection does not require every ordinary trainer to have a build.
- Use the shared capacity accounting for these builds; specific NPC budgets, costs and assigned effects remain to be designed.
- Do not automatically mirror the player's current budget or loadout. A trainer's background must justify its development; numerical NPC progression and rematch adjustments remain open.
- Trainer-owned passive builds and the Pokémon capabilities described above remain separate systems, even when the same opponent uses both.

## Relationship to other systems

[ADR-0019](0019-gym-resistances-and-capabilities.md) governs Pokémon capability costs, free service-only reassignment and the separate held-item slot. [ADR-0023](0023-badge-wide-training-ceilings.md) governs permanent stat training and flexible focus. Selecting a trainer build does not redistribute Pokémon training points or grant individual capabilities. Badge-stage ceilings are automatic progression unlocks, separate from the selected additional-passive build.

Major bosses and selected experienced trainers now use both systems where justified by their backgrounds. Their trainer passives consume trainer capacity, while each Pokémon's assigned capabilities consume that Pokémon's own capacity.

## Alternatives considered

- Combat-only scope for additional trainer benefits: replaced by explicit permission for training/EXP benefits as well.
- Automatically activate every additional trainer passive: not selected; the player chooses within a shared capacity budget.
- A fixed number of equally priced active trainer passives: not selected; stronger passives consume more capacity.
- Keep trainer capacity fixed throughout the campaign: not selected; badges expand it.
- Increase capacity through badges plus unrelated notable accomplishments: not selected as the growth route; story encounters may still unlock passive choices.
- Player-exclusive or major-boss-only trainer builds: not selected; selected experienced trainers also use them.
- Give Pokémon capabilities from non-repeatable story encounters and devise catch-up substitutes: not selected; those encounters award trainer passives instead.
- Treat trainer and individual passives as interchangeable effects with different labels: inconsistent with the requested distinction.
- Give every opponent complete capability loadouts: remains rejected.

## Consequences

Trainer builds support preparation for training or particular opponents while individual Pokémon retain personal development. Repeatable sources protect future recruits from permanently missing capabilities.

Assess trainer passive costs both individually and in combination. A higher isolated cost does not establish that a combination is balanced; compare effects across the whole team and different activities.

Assess the combined effects of trainer passives, individual capabilities, held items, innate abilities and stat training. EXP effects must be considered against the substantial levelling target; no revised time target is selected.

## Evidence and validation

The user's answers on 2026-09-05 choose the limited-subset-at-a-Pokémon-Center option, describe EXP and stat-boost examples, require a clear trainer/Pokémon distinction, and prohibit individual capabilities from non-repeatable story encounters. Opponent experience and partial-loadout rules carry forward from ADR-0020.

No implementation or balance results exist. Future checks must cover ownership, recruit access, selection limits, repeatable reward access, singles/doubles interactions and lore-consistent opponent loadouts.

## Open questions

- Intended reference badge effects and their interaction with selected additional passives.
- Starting trainer capacity, numerical badge-stage increases, final budget, individual passive costs and effects, magnitudes, stacking and reassignment costs.
- Specific eligible NPCs, their capacity budgets and passive loadouts, with background justification.
- Public scouting and AI information fields for both passive systems.
- Repeat access details, rematch tiers and progression triggers.
- How post-League accomplishment is reflected within the eventual postgame scope.

## Follow-up

Specify numerical badge-based capacity growth and information policy, then draft representative player and NPC trainer builds plus Pokémon effects with explicit ownership and background rationales. Verify the inherited badge baseline before implementing it.

## Clarification — 2026-09-05, automatic badge ceilings

[ADR-0023](0023-badge-wide-training-ceilings.md) clarifies that badge-based training capacity applies player-wide, including to recruits, without assigning a trainer passive. Individual Pokémon still earn their own points and capabilities.

## Clarification — 2026-09-05, weighted trainer capacity

The user selected a shared capacity budget with stronger trainer passives costing more. This resolves the capacity model without selecting a budget size, cost range or progression schedule. Trainer capacity and Pokémon capability capacity remain separate.

Future validation should reject assignments above the trainer budget, calculate active cost correctly when changing builds at the Pokémon Center, and check combinations alongside individual capabilities and held items. Badge-wide training ceilings remain automatic and do not require assigning a passive. No game implementation or balance tests exist.

## Clarification — 2026-09-05, badge growth and opposing trainer builds

The user selected gym badges as the source of trainer passive capacity growth, and major bosses plus selected experienced trainers for trainer-wide builds consistent with their backgrounds. Earlier notes leaving the growth route or opponent category open are historical; numerical budgets and concrete loadouts remain unresolved.

Future validation should check badge-based budget changes, prevent duplicate badge/rematch growth, keep story passive unlocks separate from capacity increases, enforce each trainer's assigned budget, and assess opponent builds against their backgrounds and both battle formats. No implementation or playtest results exist.
