# Pokémon Supercharged Yellow

A Kanto ROM-hack project inspired by Pokémon Recharged Yellow: familiar Pokémon and places, modern physical/special move categories, greater strategic depth, and balance that preserves distinct identities.

## Current state

Project governance is established. No engine has been selected or imported, no game build has been verified, and no playable prototype is available. Recharged Yellow source is unavailable and is not required for this project.

## Confirmed direction

- Original 151 species in original forms only in the main campaign; no regional variants or Mega Evolutions.
- Keep a trusted core and rotate specialists for different bosses; repeated attempts and boss mastery remain central ([ADR-0017](docs/adr/0017-trusted-core-and-boss-mastery.md)).
- Substantial levelling investment for newly caught Pokémon: roughly 20–30 minutes or more; total preparation time and overlap remain open.
- Random IVs that can be improved, with a safeguard against unusable random outcomes; acquired natures and their stat effects remain fixed.
- Training focus can change on the fly and directs future gains; earned stat points never move. Each stat has its own training maximum, and a Pokémon can theoretically reach all of them. The trainer's badges unlock the next training ceiling for every Pokémon, including new recruits; individual gym participation is unnecessary. Stage limits allow modest survival or Speed adjustments while constraining overtraining. Normal late-game development targets relevant stats; maxing everything should be difficult and exceptional ([ADR-0023](docs/adr/0023-badge-wide-training-ceilings.md)).
- Ordinary stat training should usually require additional focused battles after levelling; the extra duration remains open.
- Clear, predictable battle-earned development, especially from boss experience, rewarding continuity without excessive penalties for rotation. Points follow a player-selected focus automatically ([ADR-0023](docs/adr/0023-badge-wide-training-ceilings.md)). Retain badge-wide training capacity alongside individually earned Pokémon passives; each Pokémon has three slots, with capability costs of one, two or three and possible benefits plus drawbacks ([ADR-0019](docs/adr/0019-gym-resistances-and-capabilities.md)). Capabilities may also be earned from repeatable Elite Four and other notable encounters, with the same participation, no-fainting and victory requirements. Preserve intended existing badge benefits and select a limited trainer build at a Pokémon Center, supporting training/EXP and modest combat benefits. Trainer expertise must be distinct from Pokémon skills. Non-repeatable story encounters may award trainer passives, never Pokémon capabilities; trainer build capacity, exact effects and the inherited badge reference remain open ([ADR-0021](docs/adr/0021-trainer-builds-and-reward-ownership.md)).
- Individual capability rewards require a player victory, field participation and no fainting during that battle. Stronger rematch teams suited to campaign progress let recruits earn the same capabilities as veterans ([ADR-0016](docs/adr/0016-individual-traits-and-battle-training.md)).
- Each granting encounter awards one fixed capability to every qualifying Pokémon. Major bosses and selected experienced trainers use capabilities in loadouts consistent with their experience; full teams with all slots filled are not the default.
- Capability reassignment is free only at a Pokémon Center or designated service. The normal held-item slot remains separate from the three capability slots.
- Collecting all eight gym capabilities requires each Pokémon to earn all eight distinct leader rewards. Full training capacity instead follows the trainer's badges; recruits need no gym rematches for ceiling access.
- Full boss team, move, ability, held-item and bag-consumable scouting before the first attempt.
- VGC-style open-team-list information for boss AI, with hidden stats and pending actions protected.
- Preselected battle consumables with one campaign-wide allowance and separate category limits, their numbers chosen through testing; boss defeats cost money and spent consumables.
- Mixed-category battle supplies are excluded. Both player and boss see each other's complete selected supplies and quantities.
- Player supplies allow HP healing, status cures, PP restoration and temporary stat boosters, but no revives. Only opposing major bosses use bag consumables, with the same maximum as the player.
- A substantial mix of singles and doubles, with major bosses in both; Set rules throughout trainer battles.
- Carefully selected moves from any generation that fit Pokémon identity and balance.
- Justified reassignment of existing abilities and custom typing changes from a modern official baseline; Fairy is included to strengthen familiar Pokémon identities and create more interesting battle roles ([ADR-0013](docs/adr/0013-fairy-type-evaluation.md)). Keep the standard modern type-effectiveness chart.
- No type-representation quota. Prefer stat redistribution and modest strength changes; permit substantial changes only in justified special or necessary cases ([ADR-0015](docs/adr/0015-standard-chart-and-stat-rebalance.md)).
- One initial difficulty setting.
- Level caps.
- Physical/special split.
- Restrained player customisation including the requested training-focus and learned-passive choices, plus useful quality-of-life improvements, especially adjustable battle speed.
- Challenging encounters and well-designed Pokémon without making every species equally strong or viable.
- Distinct, playable types, with longer usefulness for Bug Pokémon and attention to late-game Flying options such as Pidgeot ([ADR-0014](docs/adr/0014-type-identity-and-campaign-viability.md)).

See [ADR-0001](docs/adr/0001-project-direction.md) for the initial scope, [ADR-0004](docs/adr/0004-original-151-roster.md) for the roster boundary, and [ADR-0017](docs/adr/0017-trusted-core-and-boss-mastery.md) for a trusted core, rotating specialists and boss mastery (replacing ADR-0005). [ADRs 0006–0008](docs/adr/README.md) define levelling, scouting and consumable/loss policies. [ADR-0009](docs/adr/0009-vgc-style-ai-information.md) defines the boss AI information standard. [ADRs 0010–0011](docs/adr/README.md) define mixed battle formats, Set rules and cross-generation move curation. [ADR-0012](docs/adr/0012-ability-and-typing-rebalance.md) defines ability/typing changes; ADR-0004 now excludes regional and Mega forms. Detailed implementation remains open; numerical category limits and the overall quota are delegated to balance testing.

## Working in this repository

- [Agent instructions](AGENTS.md): how coding and design agents work.
- [Decision index and workflow](docs/adr/README.md): how consequential choices are recorded.
- [ADR template](docs/adr/template.md): starting point for a new decision.
- [Foundation proposal](docs/adr/0003-source-foundation.md): the next engineering investigation; not an accepted engine choice.

The recommended next implementation task is to audit source foundations, verify a reproducible baseline build, and record the selection with evidence. The physical/special split, caps and speed controls are the first technical capabilities to prove. The exact playable milestone remains to be decided.

## Build and testing

There are no game build or test commands yet. Add verified setup instructions after the foundation audit. Do not infer playable status from the presence of source files or a successful documentation change.

## Contributions and distribution

Preserve upstream attribution and evaluate reuse terms before importing code or assets. No project-wide license has been selected. Keep ROM images, saves and credentials out of git. Patch format, base-ROM checksum and supported release targets will be documented when packaging is implemented.
