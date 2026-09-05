# ADR-0020: Retain badge benefits and give trainers progression consistent with their experience

- Status: Superseded
- Date: 2026-09-05
- Decision authority: Explicit user answers on trainer badge benefits and selective opponent capability use
- Implementation: Not started — scope and narrative constraints only
- Supersedes: None
- Superseded by: [ADR-0021](0021-trainer-builds-and-reward-ownership.md)

Historical record. Its decisions and open questions describe the earlier policy. Follow ADR-0020 → ADR-0021; [current policy](0021-trainer-builds-and-reward-ownership.md) governs new work. Editorial navigation note added 2026-09-05.

## Context

[ADR-0019](0019-gym-resistances-and-capabilities.md) defines individually earned capabilities with three capacity slots. The user has now selected trainer-wide badge benefits and explained how opposing trainers should use capabilities without undermining the player's sense of accomplishment.

## Decision

### Trainer-wide badge benefits

- Preserve the badge benefits that already exist in the intended reference game, and add modest passive combat benefits.
- Treat these as trainer-owned benefits applying to the Pokémon the trainer uses, separate from individually earned Pokémon capabilities.
- A new recruit can benefit from its trainer's badge passives without personally earning the corresponding individual gym reward.
- Do not interpret the answer as approval for additional new exploration or training perks beyond the intended existing badge effects.

The reference game/version and exact inherited effects remain unresolved. There is no selected engine or implemented badge system in this repository. Inventory the intended benefits before implementation; do not silently equate them with whichever behaviors a future source happens to include. No specific inherited stat bonus, multiplier or bug behavior is accepted by this record.

### Opponent capability use

- Major bosses and selected experienced trainers may use Pokémon capabilities within the same three-slot capacity rule.
- Not every Pokémon on such a trainer's team needs capabilities, and slots need not be filled.
- Fully filled capability capacity across an entire team should not be the general campaign norm. Do not assume the player or every opponent must reach maximum loadouts for meaningful play.
- Scale capability use with player progress while keeping it consistent with each trainer's experience, identity and place in the story.
- After completing all gyms and Elite Four battles, the player's accomplishment should be reflected relative to most gym leaders. Do not give every lesser trainer equivalent development merely to mirror the player.
- An occasional trainer near a gym may have an ace with that gym's earned capability, as a plausible expression of local experience. This is a permitted example, not a rule for every nearby trainer.

These are encounter-design constraints, not an automatic scaling algorithm. Exact progression triggers, rematch tiers and loadouts remain open. They do not impose a lower formal slot cap on weaker trainers, require specific player slots to stay empty, or remove the intended challenge from major encounters.

## Relationship to individual rewards

[ADR-0019](0019-gym-resistances-and-capabilities.md) now fixes one capability reward per granting encounter, the same for every qualifying Pokémon. The player's qualifying Pokémon must enter the field, never faint during the encounter and be on the winning team.

A trainer-wide badge benefit and a Pokémon-owned reward have different ownership and eligibility. Opponent use of individual capabilities is selected here; which opponents also receive trainer-wide badge benefits remains a separate question.

## Alternatives considered

- Trainer-wide benefits confined to new exploration/training perks: not selected; preserve existing benefits and add modest combat effects.
- Give capabilities to every opposing trainer or automatically fill every slot: not selected.
- Player-exclusive capabilities: not selected.
- Scale opponents to mirror the player's exact accomplishments or loadout regardless of story: inconsistent with the user's narrative constraint.

## Consequences

Design capability loadouts with a brief explanation of relevant trainer experience or role. Partial loadouts and capable aces can communicate progression without making every battle an endgame build.

Assess the combined effects of badge passives, Pokémon capabilities, held items, innate abilities and trained stats. Decide how trainer-wide effects stack and what is visible in scouting before implementing them.

The accomplishment goal does not imply every late-game gym rematch must be easy. Believable team design, tactics and specialisation can maintain challenge. Avoid depending on universal slot saturation to create it.

## Evidence and validation

User answers on 2026-09-05 retain existing badge benefits plus additional modest combat passives; select major bosses and some experienced trainers for capabilities; explicitly reject assuming every Pokémon or slot is filled; require progression to fit the story; and give a nearby trainer's ace with the local gym capability as an example.

No game implementation, source audit of badge effects or balance results exist. Future review should check inherited badge behavior against the selected reference, trainer and individual effect ownership, varied slot use, understandable progression, and challenging encounters across several player cores. Test combined effects in singles and doubles.

## Open questions

- Reference game/version and exact existing badge effects to preserve.
- Specific new combat benefits, amounts, stacking and activation rules.
- Which opponents have trainer-wide badge passives and which benefits they possess.
- Progression triggers, rematch tiers, individual trainer loadouts and local examples.
- Scouting and AI information mapping for trainer-wide effects and assigned capabilities.
- How post-League accomplishment is reflected within the eventual postgame scope.

## Follow-up

Resolve the badge reference and activation policy, then document individual benefits and representative trainer loadouts. Keep exact numbers provisional until tested.

## Supersession — 2026-09-05

[ADR-0021](0021-trainer-builds-and-reward-ownership.md) is the current policy. This record preserves the earlier decision and rationale; its superseded restrictions are not current requirements.
