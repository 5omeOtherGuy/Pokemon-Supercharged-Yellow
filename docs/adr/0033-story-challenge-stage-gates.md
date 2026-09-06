# ADR-0033: Align story challenges with badge-stage preparation

- Status: Accepted
- Date: 2026-09-06
- Decision authority: Campaign progression choices delegated by the owner's full-project mandate
- Implementation: Acceptance-model regression tests and production build pass; ordinary story journey pending
- Supersedes: None
- Superseded by: None

## Context

The inherited map order is more permissive than the intended boss stage. Bill's ticket permits the S.S. Anne rival before Misty. Cut permits Celadon and Pokémon Tower after two badges. Rescuing Mr. Fuji permits the Silph sequence without requiring the badge stage assumed by the midgame teams. A boss authored for a later cap would otherwise confront a player whose training and level ceilings cannot reach that stage.

## Decision

Keep the S.S. Anne rival available before Misty and author its team for the one-badge cap of 22. Keep the Mt. Moon Rocket duo at that same stage.

Require at least **three badges** to accept Rocket Hideout Giovanni, the Hideout Rocket duo, the Pokémon Tower rival variants and the Tower Rocket duo. These challenges are authored against the three-badge cap of 36. Require at least **five badges** for the Silph rival variants, Silph Rocket duo and Silph Giovanni, corresponding to cap 50.

Apply the requirement at the final challenge acceptance check, alongside party readiness. Allow complete free scouting even when the requirement is not met. State the required badge count and permit returning to the field without starting the battle. Do not add an exploration barrier or consume an attempt, item or reward on refusal. Existing fixed gym-order and story requirements still apply independently.

## Alternatives and consequences

Reducing every story team to its earliest inherited reachability would compress midgame difficulty into the two-badge stage and undermine the established cap progression. Implicitly expecting the player to grind beyond the cap is impossible. Blocking entire cities would unnecessarily restrict collection and preparation. Explicit challenge gates preserve access to those activities and tell the player how to proceed.

## Verification

Host tests exercise all eleven gated trainer IDs below and at their required stage, healthy-party requirements, and navigation while acceptance is unavailable. The production UI rechecks current badge count immediately before committing. The S.S. Anne and Mt. Moon encounters remain ungated by this addition. Required follow-up: inspect refusal/return in the real UI and play the ordinary story through both gate stages, including loss/retry and alternative exploration routes. Team curation and numerical/human balance evidence remain separate.
