# ADR-0027: Complete Kanto with explicit battle and field rules

- Status: Accepted
- Date: 2026-09-06
- Decision authority: Agent design decisions under the owner’s explicit full-project execution mandate
- Implementation: Not started — contract selected; numerical values remain subject to testing
- Supersedes: None
- Superseded by: None

## Context

The accepted records define the game’s identity and systems but leave battle-entry scope, field permissions and completion boundaries open. The owner now delegates those choices and requires the entire game through a tested Android patch release. The former review-only approval boundaries do not apply to this task.

## Decision

- Finish the Yellow adventure in FRLG Kanto through the first Championship, then Kanto collection and progression-appropriate rematches. Provide repeat access to all eight gym capability rewards and any other individual reward sources, including after Giovanni leaves his story role. Do not add Sevii or another region. Milestones used for engineering are not product completion.
- Omit automatic badge stat multipliers. Badges separately unlock field permissions, training ceilings and trainer-build capacity. The selected personal training and selectable trainer effects provide additional combat growth without another invisible multiplier.
- Retain Kanto field permissions: Boulder/Flash, Cascade/Cut, Thunder/Fly, Rainbow/Strength and Soul/Surf. Volcano and Earth retain progression significance without importing unrelated Rock Smash/Waterfall requirements. HMs remain reusable and forgettable; field progression must always have a reachable restoration route. No obedience checks or trading are enabled.
- Use the player’s whole available party, up to six, for campaign battles. No bring-four rule or artificial level normalization. Major doubles encounters require two non-fainted Pokémon, announce the requirement before commitment, and leave the player free to return to a Center. Ordinary doubles encounters also check party eligibility before initiating. Singles use Set rules.
- Apply the same fixed preselected supply quota to all trainer battles. Wild battles retain capture balls and escape items outside that quota; trainer-only preparation rules cannot strand a player in a capture encounter. Wild recovery supplies remain ordinary inventory use. Safari retains its separately explained catch/escape rules.
- Count individual consumable units, not item names or stack slots. Initially allow at most three units, with category maxima of two HP heals, one status cure, one PP recovery and one stat booster. Bringing fewer or none is legal. These are initial tuning values under ADR-0008, not proven balanced values. Reject revives and mixed-category items for both sides. Only major bosses use opposing bag supplies, within the same maxima.
- Select trainer-battle supplies outside battle; lock selection on encounter entry and check owned quantities again. Show the exact enemy team, format, levels, nature alignment, moves, abilities, held items, active passives/capabilities and supplies before accepting a major battle. Exact hidden player numerical stats remain unavailable to AI. Do not require a scouting fee or a failed attempt. Inspectable boss data and the fought variant share their authoritative definition.
- Each bag use consumes one battler’s action in doubles. No extra free action or doubled quota. Used supplies stay spent after loss; unused inventory remains. Consumed held items also stay consumed, with repeatable replacement access. Out-of-battle revives and combined healing items do not become legal trainer-battle supplies.
- Reassign earned capabilities and trainer passives for free at Centers; keep the Pokémon and trainer capacity budgets separate. Change training focus from the field party service, directing future gains only. League entry is a designated preparation service; do not insert free reassignment between its individual battles.
- Major loss uses a bounded money deduction and ordinary Center recovery; the numerical economy must support continued attempts even at zero money. Repeatable earning/training and affordable replenishment must remain reachable. No wipe, permanent item theft, mandatory paid reset or failure-count gate.

## Alternatives considered

Inherited automatic badge boosts would stack an additional, poorly visible source of strength on three chosen progression systems. Omitting them gives preparation and balance tests clearer causes. Universal trainer/wild restrictions would complicate catching without adding the intended boss planning; unrestricted trainer inventory would defeat preselection. Bring-four and level normalization would import unrelated VGC rules. A larger postgame region would delay the explicitly prioritized Kanto work.

## Consequences

Battle entry, bag UI, AI supplies, double-battle action handling and scripts need a shared contract. Supply counts and category caps require balance evidence before release. Every reward and collection source needs story-state recovery. Field permissions must be checked against map reachability rather than merely preserving source flags. Documentation and UI must explain the ownership of progression and exact active effects.

## Evidence and validation

Authority: owner’s 2026-09-06 completion mandate; prior accepted constraints in ADRs 0004, 0007–0011, 0016, 0019, 0021, 0023–0026. These choices resolve the corresponding open questions without replacing their accepted policies. No game code or balance test validates these choices yet. Track implementation and actual evidence in [project status](../project-status.md).

## Open questions

Individual encounter teams, tier/cap tables, effect values, loss amounts and supply availability require source integration and testing. Save format, engine revision and build/distribution mechanics receive separate records after their audits. Actual human and Android evidence is required for release.

## Follow-up

Implement the contract in all entry and outcome paths. Test duplicate names/category counting, inventory changes, singles/doubles actions, unique encounter recovery, zero-money retry access, and first-attempt scouting. Play the entire campaign and post-League completion route and revise numerical tuning using measured results.
