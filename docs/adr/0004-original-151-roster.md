# ADR-0004: Restrict the main campaign roster to the original 151

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user answer in design interview round 1
- Implementation: Not started — design decision only
- Supersedes: None
- Superseded by: None

## Context

[ADR-0001](0001-project-direction.md) left roster boundaries open. The interview asked which Pokémon should be eligible for the main campaign.

## Decision

Restrict main-campaign species eligibility to the original 151 Pokémon. Later evolutions, pre-evolutions and unrelated species outside that set are excluded from the main campaign, including opposing trainer rosters.

This establishes an eligibility boundary, not a promise that every eligible species can be caught before the League. It does not choose encounter locations, starter access, legendary availability or a mechanics generation.

## Alternatives considered

- Original 151 plus later evolutions and pre-evolutions.
- Mostly Kanto with a small curated selection of unrelated Pokémon.

The user selected the strict original-151 option.

## Consequences

Balance must work within this species pool. We cannot solve a campaign design problem by adding an out-of-roster evolution or counter. Pokémon whose later evolutions are excluded need to be assessed in their campaign roles without assuming equal power is required.

Any source foundation with a larger database will need encounter, evolution, gift and trainer access audited; engine database support alone does not make a species campaign-eligible.

## Evidence and validation

User answer, 2026-09-05: "Strictly the original 151", in response to the main-campaign roster question. No game implementation or runtime checks have occurred.

Future validation should audit all campaign acquisition and opponent-team paths against the agreed species pool, including transformations if later enabled.

## Open questions

- Forms and temporary transformations of those species, including regional forms and Mega Evolution, require separate clarification; they are not implicitly enabled by sharing a species identity.
- Postgame roster scope.
- Encounter distribution, starter access and legendary timing.

## Follow-up

Resolve forms and campaign availability before encounter design. Use this constraint during the source-foundation audit. The roster question in ADR-0001 is resolved for main-campaign species eligibility.
