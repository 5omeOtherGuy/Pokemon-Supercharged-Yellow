# ADR-0025: Target Android emulation while allowing GBA or NDS foundations

- Status: Accepted
- Date: 2026-09-05
- Decision authority: Explicit user clarification of play platform and hardware requirements following the ADR audit
- Implementation: Not started — platform requirement only; no foundation or emulator compatibility verified
- Supersedes: None
- Superseded by: None

## Context

The design review left the play platform unresolved. The user plays exclusively on Android, currently through RetroArch with the mGBA core, and is willing to use another suitable GBA or Nintendo DS emulator. The user is not committed to a GBA-era base and permits NDS if it makes development easier. Playing on a physical GBA through a cartridge is not a requirement.

## Decision

- Android emulator play is a required target for the finished game.
- GBA and NDS foundations are both eligible. Choose through the source/build audit in [ADR-0003](0003-source-foundation.md), considering development effort and the accepted game requirements.
- RetroArch with mGBA is the user's current setup, not an exclusive emulator requirement or a selected engine. Another suitable Android emulator is acceptable.
- Original-console hardware compatibility and cartridge distribution are not required. This does not prohibit incidental compatibility.
- The user's expectation that GBA will be easiest is a hypothesis to investigate, not a foundation selection or verified comparison.

This resolves platform constraints without selecting an engine, base game, mechanics generation or exact supported Android device/emulator versions. A working supported Android configuration is required; the answer does not promise compatibility with every emulator. A native Android application is not required or selected by this clarification.

## Alternatives considered

- Require a GBA foundation and physical GBA compatibility: more restrictive than the user's requirements.
- Exclude NDS because the current setup uses mGBA: inconsistent with the user's explicit flexibility.
- Allow desktop-only play: insufficient because Android compatibility is mandatory.
- Commit to a specific foundation now: premature without the source/build evidence required by ADR-0003.

## Consequences

The foundation comparison must consider Android playability alongside implementation effort. Desktop builds or emulator tests can support engineering but cannot establish Android support on their own. For an NDS candidate, assess screen layout and touch interaction as well as performance and controls. No emulator or candidate has been tested for this project.

Original-hardware and cartridge testing need not block the project. Exact Android test devices, versions and emulator configurations should be selected when a candidate is ready to validate.

## Evidence and validation

User clarification on 2026-09-05: "NDS if that makes our development easier is accaptable"; "Any GBA/NDS emulator will do"; and "I am solely playing on android so android compatibility is a must." The user also stated no commitment to transferring the game to a cartridge for a physical GBA. The recorded GBA-ease expectation remains tentative.

Future validation must document a tested Android device/OS and emulator/core version, then check boot, controls, readable presentation, relevant battles, required speed settings, audio and in-game save/load. Assess NDS screen/touch interaction if that platform is chosen. No Android or game-runtime validation has occurred.

## Open questions

- Foundation/base game selection and evidence for comparative development effort.
- Exact Android validation targets and acceptable performance, to be established with a candidate build.
- Essential Yellow presentation, starter and event requirements remain separate in ADR-0001.

## Follow-up

Apply these platform constraints during the source comparison. Continue the design discussion with essential Yellow elements; do not reopen platform choice merely because an existing candidate uses GBA.
