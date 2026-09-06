# ADR-0032: Offer named battle-pacing presets

- Status: Accepted
- Date: 2026-09-06
- Decision authority: Usability and engineering choices delegated by the owner's full-project mandate
- Implementation: Native helper/build, actual three-preset sequence and QUICK save/restart checks pass; broader timing, Android/audio and human checks pending
- Supersedes: None
- Superseded by: None

## Decision

Replace the ineffective campaign Battle Style option with **Battle Pace: NORM / FAST / QUICK**. All trainer encounters retain Set rules. New games default to NORM; save the selected pace in the versioned trainer record. The existing text-speed and animation options remain separately available.

NORM preserves the foundation's battle delays. FAST divides selected battle-script pauses and message waits by two and multiplies HP/EXP bar steps by two; QUICK uses four. Round fractional delays upward. Keep a twelve-frame minimum for message waits that were originally at least twelve frames, and never lengthen a shorter original wait. Preserve field timing, move animations, audio playback rate, battle calculations and input decisions. This is selective pacing, so **the presets do not claim exact whole-battle 2×/4× acceleration**.

Treat a missing or invalid saved pace as NORM. Store legal values 1, 2 or 4 inside the existing CRC-protected trainer record; no save-schema expansion is needed.

## Rationale and alternatives

A whole-emulator clock multiplier changes music, animation and input cadence and depends on the Android frontend. Selective delays give the game a portable convenience control without changing numerical battle rules. Naming the presets avoids a false promise about elapsed battle duration when animation, text and decision time remain variable. Leaving a Shift option visible would imply a rule the campaign does not permit.

The fastest settings may materially reduce training time. Measure the preparation target from ADR-0006 at each tested setting, explicitly recording text speed and animation setting; do not infer enjoyable or substantial preparation from helper arithmetic.

## Verification

The native test checks field isolation, all three factors, upward rounding, minimum waits, zero/short waits, invalid settings and saturated bar arithmetic. A production build compiles the Options branch and real battle hooks. Neither establishes animation completion, readability, audio quality or an observed duration ratio. Required follow-up: exercise Options, save/restart, compare the same battle at each preset using actual emulated frames, inspect messages and animations, and validate Android controls/audio and human preparation time.

## Implementation evidence — 2026-09-06

[Actual pacing validation](../validation/battle-pace.md) records951/891/831 emulated frames for one controlled Thunder Shock sequence at NORM/FAST/QUICK, identical resulting party data, and normal QUICK save/restart persistence. The boundary is move acceptance to battle-mode exit; field redraw and decision time are excluded. No audio or human pacing conclusion follows.
