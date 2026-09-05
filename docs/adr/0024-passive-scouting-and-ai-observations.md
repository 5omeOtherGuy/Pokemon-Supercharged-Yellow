# ADR-0024: Propose public active passives and bounded AI observations

- Status: Proposed
- Date: 2026-09-05
- Decision authority: Agent recommendation; user confirmed preparation and public gym-leader information as goals but expressed uncertainty about AI access
- Implementation: Not started — no engine selected or AI integration verified
- Supersedes: None
- Superseded by: None

## Context

The interview asked who should see active trainer passives and assigned Pokémon capabilities before a boss battle. The user wants meaningful preparation, considers gym-leader information public knowledge within the fiction, and tentatively suggested the AI might need all information to avoid glitches. The user explicitly lacks the technical background to choose an AI information policy confidently.

Do not interpret that tentative suggestion as approval to override [ADR-0009](0009-vgc-style-ai-information.md), which protects the player's exact stats, allocations, pending actions and future random outcomes. [ADR-0007](0007-full-boss-scouting.md) already accepts full boss team, moves, abilities, held items and bag-supply scouting. Exact disclosure of the new passive fields remains proposed here.

## Proposal

### Preparation and public loadouts

- Show the player every active boss trainer passive and each boss Pokémon's assigned capabilities before the first attempt, alongside existing scouting information.
- Explain the effects precisely enough for preparation: numerical strength where applicable, activation conditions, limitations and drawbacks.
- Give boss AI the equivalent active trainer-passive and assigned-capability fields for the player's battle team before turn one.
- Treat these as public battle loadouts. A gym briefing, published League profile or pre-battle team declaration could explain disclosure in the fiction; the delivery mechanism is an illustrative proposal, not selected story content.
- Do not reveal unassigned capability collections or inactive trainer-passive choices merely because active effects are disclosed.
- Keep scouting synchronized with the actual encounter variant, rematch and selected loadouts. Exact locking and refresh timing remain to be designed.

### Preserve the existing information boundary

- Retain the accepted team and supply fields from ADR-0009.
- Do not give AI direct access to the player's exact stats, IVs or accumulated training allocations.
- Do not expose the player's committed move, switch, item choice or target while the AI is choosing its own action, or expose future random outcomes.
- Allow reasoning from public effects and observed battle events. Estimates remain estimates, even when public data supports a strong inference.
- The AI may inspect its own team state. Player access to exact boss numerical stats remains an open scouting question under ADR-0007; this proposal does not silently decide it.

### Engineering approach

Separate authoritative battle state from the observations used to choose an AI action. The battle engine needs complete state to resolve effects correctly. The decision logic should receive explicitly permitted fields, observed history and estimates for unknowns.

Teach effect evaluation, damage estimates, switching and action scoring how custom trainer passives and Pokémon capabilities work. Passing more hidden data into an evaluator that does not understand a new effect does not implement that effect.

Use legal-action handling and defined behavior for unknown information; do not treat unrestricted access as a substitute for correct mechanics integration. Source-specific feasibility and every data path require an audit after engine selection. This is a proposed implementation approach, not a claim about the repository's current runtime.

## Alternatives considered

- Player sees boss passives while AI discovers player passives during battle: preserves preparation but adds asymmetric information and more inference work.
- Both discover passives during battle: weakens first-attempt preparation and the public-gym-information goal.
- Give AI all internal data for reliability: conflates battle resolution with decision knowledge and would reverse accepted hidden-information protections without resolving missing custom-effect support.

## Consequences

Public active effects make preparation explainable and let bosses account for the player's build without secretly reading choices. Encounter challenge must hold up under disclosed loadouts.

This reveals more of the player's build, while exact training and tactical decisions remain protected. It creates work in scouting presentation, encounter data synchronization and AI effect evaluation. It does not guarantee strong or bug-free AI.

## Evidence and validation

User statement on 2026-09-05: preparation matters, gym-leader information plausibly belongs in public knowledge, and AI knowledge was suggested tentatively to avoid glitches. The user has not yet accepted the reciprocal active-effect proposal.

Primary source inspected on 2026-09-05: [pokeemerald-expansion battle_ai_util.c](https://github.com/rh-hideout/pokeemerald-expansion/blob/7644501f3065b38aea2cfc64b5b58ca011bce4f5/src/battle_ai_util.c), commit `7644501f3065b38aea2cfc64b5b58ca011bce4f5`. `IsAiBattlerAware` branches on omniscience; `AI_DecideKnownAbilityForTurn` handles known abilities and candidate abilities when knowledge is incomplete. This demonstrates explicit knowledge modeling in an existing hack engine, not that its entire AI meets ADR-0009 or supports our custom passives. It is an inspected example, not an engine selection or imported dependency.

Future checks should verify:
- Scouting matches actual active effects before original and rematch encounters.
- The AI evaluates custom effects using the proposed visible fields.
- Changing hidden opponent data, pending commands or future random state does not change action evaluation when permitted observations and the AI's own decision randomness are held fixed.
- Inactive passives do not leak through evaluation helpers.
- Legal choices and effect handling remain reliable across singles, doubles, switching and suppression/trigger interactions.
- Representative boss encounters remain challenging with disclosed effects.

No implementation, compilation or runtime tests exist.

## Open questions

- Acceptance of reciprocal active-passive and assigned-capability disclosure.
- Exact scouting presentation, loadout commitment and refresh timing.
- Boss numerical-stat disclosure and observable HP precision.
- AI implementation foundation, decision depth and inference strategy.

## Follow-up

Review the proposal as a player-experience choice, then map approved fields into the selected engine and prototype a boss with both passive systems. Preserve existing accepted information limits until explicitly revised.

## Evidence review — 2026-09-05

The documentation audit rechecked `IsAiBattlerAware` (lines 200–206) and `AI_DecideKnownAbilityForTurn` (lines 1760–1805) at the pinned revision above. Their awareness and candidate-ability branches support only the stated example of knowledge modelling. They do not establish end-to-end isolation of hidden data or custom-effect support. Status remains Proposed; no user uncertainty was treated as acceptance.
