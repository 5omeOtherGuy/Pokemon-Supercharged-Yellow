# ADR-0010: Use substantial singles and doubles content with Set rules

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answers in design interview round 6
- Implementation: Not started — campaign battle policy only
- Supersedes: None
- Superseded by: None

## Context

The user previously selected VGC-style information for boss AI, without choosing a battle format. The next interview asked how singles and doubles should appear in the campaign and whether defeating an opposing Pokémon should grant a free switch.

## Decision

- Include a substantial mix of singles and doubles in the main campaign.
- Include major bosses in both formats; doubles are not merely occasional novelty encounters.
- Use Set rules throughout trainer battles. Do not grant the player a free voluntary switch when an opponent's Pokémon is knocked out and replaced.

This does not fix a numerical singles/doubles ratio, assign formats to specific bosses, or adopt VGC bring-four, level scaling, timers or best-of-three rules. Normal turn-consuming switches and battle-mechanic effects remain separate from the removed Shift-style prompt.

## Alternatives considered

All singles, predominantly singles with occasional doubles, or primarily doubles. For switching, the offered alternative was the classic Shift-style free switch. The user selected a substantial mix and Set rules.

## Consequences

Both formats need deliberate encounter design and balance validation. Availability and learnsets must support useful roles across the two formats. Boss format assignments should be assessed alongside campaign progression and preparation.

AI targeting, partner interactions and the action cost of bag consumables in doubles need explicit implementation and testing. The fixed consumable policies still apply; this decision does not double the allowance.

## Evidence and validation

User answers on 2026-09-05:
- "A substantial mix, with both singles and doubles major bosses."
- "No: use Set rules throughout trainer battles."

No encounters have been implemented or played. Future checks should confirm substantial representation across the campaign, bosses in both formats and no free Shift-style knockout prompt in trainer battles.

## Open questions

- Format distribution, specific bosses, team sizes and party selection.
- Early-game introduction to doubles and handling an undersized usable party.
- How formats are communicated before battle.
- Doubles item-use action costs, targets and replacement order.
- Multi-trainer, partner and wild-battle formats.

## Follow-up

Map formats onto the campaign before writing final trainer teams. Prove both formats and Set behavior in the selected engine. Apply [ADR-0009](0009-vgc-style-ai-information.md) to information access independently of format.

## Execution clarification — 2026-09-06

Under the owner’s full-project mandate, [ADR-0027](0027-campaign-and-battle-contract.md) resolves relevant battle-entry, disclosure, supply, service and inherited badge choices previously left open here. It preserves this record’s accepted constraints. Numerical implementation and release validation remain separate from acceptance.
