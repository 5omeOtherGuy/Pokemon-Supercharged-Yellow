# Implementation and release evidence

Updated 2026-09-06. **The game is not complete and no Supercharged Yellow release is ready.** Integrated builds and native mechanics tests are intermediate results. This file is the current execution record; the earlier design review is historical.

## Authority and scope

The owner’s 2026-09-06 mandate authorizes full implementation, autonomous outstanding design and numerical choices, subagents, system/tool installation, research, commits, pushes, merges and release. It supersedes the review brief’s documentation-only boundary and previous instructions to stop before engine selection. The accepted game identity and mechanics remain binding; the design interview is not being restarted.

Completion includes the Yellow adventure across FRLG Kanto, all agreed systems and progression, the League, progression-appropriate gym rematches, repeat access to individual rewards, all 151 obtainable on one save without trading, and Mew under the truck. Kanto post-League collection/rematches are in scope; Sevii is not. A prototype, simulator or successful compilation is an intermediate result.

## Verified starting point

- Inspected and fetched repository: `b0022d34030efda2d79e581b16d860ca932b0277`, clean `main` matching `origin/main`.
- Read root instructions, local collaboration workflow, review brief/report and all 26 ADRs, including superseded records and decision history. Independent requirement/history and foundation audits are complete; implementation workers now own campaign, roster and AI changes.
- The starting revision contains governance/design only: no engine, build, tests, campaign implementation or release artifact.
- Historical supersession remains 0005 → 0017; 0018 → 0022 → 0023; 0020 → 0021. ADR-0024 is accepted. A stale reference to its proposal status in ADR-0016 was corrected.

## Completion ledger

| Required outcome | Current evidence | Remaining acceptance |
| --- | --- | --- |
| Foundation and reproducible build | Selected/imported expansion pin `7644501f3065b38aea2cfc64b5b58ca011bce4f5`; unmodified build/battle/save passed; see ADR-0028 | Complete project build and independent clean reproducibility check |
| Yellow/Kanto campaign | Pikachu/Eevee opening, gifts, Jessie/James, mainland gates and collection source implemented; 16 content tests pass | Play from new game through League; verify anchors, all gates, repeat access and recovery |
| Battle mechanics, split, Fairy, weather/terrain, Set and speed | Modern engine, Set, public damage/status/speed/recovery effects implemented; 119-group integrated regression has109 passes and10 intentional checks; named pace presets implemented and a real sequence measured | Broader interactions/animation checks, Android/audio, complete UI and human timing |
| Levels, traits, training, capabilities and trainer builds | Encrypted metadata, whole-victory reward transaction and service model/UI implemented; 6 limit tests pass, including EXP/candy/Day Care/IV floor/NPC ceilings | All-source rewards, full preparation-time validation and remaining integrated journeys; faint/revive/loss/switched-support native cases pass |
| Supplies, loss economy, scouting and AI information | Canonical briefing/public AI and supply hooks implemented; Center/field diagnostic582commands30screens pass; staged shops and two practice routes source/assembly checked | Full outcome journeys, runtime economy/zero-money recovery, ordinary boss briefing; repair disabled Hoenn-specific Vs.Seeker |
| Encounters and balance | 151-roster curation plus all14 League and59 story/gym/rematch parties source/ARM checked; narrative NPC partial builds implemented | Stage-legal accessible teams in both formats, strongest legal builds, poor traits, specialist burden and retesting |
| Complete collection and required rematches | All-151 source acquisition manifest, solo evolutions, Mew truck, encounter retries and 24 gym rematches implemented | All 151 on one save; branch/recovery traversal, required rematches and all 12 personal rewards |
| Save integrity | Custom 48-byte record, native encrypted party/box layout and sector integrity pass; actual save selector rejects torn counters and falls back | Ordinary evolution/reward/corruption recovery journeys; two opening saves/Continue and PC transfer restored, plus QUICK setting persistence |
| Actual gameplay | Supercharged opening: Pikachu/Eevee, parcel/Pokédex, Pidgey catch, Center/PC, two save/new-process restores;1251 recorded responses | Complete organic journey plus clearly labeled diagnostic fixtures and regressions |
| Human experience | No playtester results | Returning-player difficulty, learning, preparation timing, identity and enjoyment evidence; iterate and retest |
| Android | Android 11/API30 x86_64 VM boots; RetroArch1.22.2/mGBA core loads, game untested | Record actual Android OS/device or VM and emulator versions; controls/readability/battles/speed/audio/in-game save tests |
| Distribution | No patch | Reproducible release build, exact base checksum, verified patch round-trip, player instructions, attribution and accurate validation report |

## Work in progress

The integration checkout remains on `main`; implementation uses `agent/full-game` in a separate worktree. Current workers continue the ordinary journey toward Brock, exercise economy fixtures, and repair Kanto Vs.Seeker rematches. The coordinator integrates evidence, fixes and remaining player experience work. Local task allocation and recovery state live in the common Git directory's `info/agent-tasks.md` and `info/full-game/`; those local records are not release evidence.

Focused records: [battle effects](validation/battle-effects.md), [AI](validation/fair-ai.md), [supplies](validation/battle-supplies.md), [boss briefing](validation/boss-briefing.md), [field access](validation/field-access.md), [campaign safety](validation/campaign-safety.md), [emulator harness and foundation opening](validation/emulator-harness.md), [Yellow opening](validation/yellow-opening.md), [Center services](validation/center-services.md), [battle pacing](validation/battle-pace.md), [boss teams](balance/campaign-bosses.md), [League](balance/champion-finale.md), [economy](balance/campaign-economy.md). A 13-group combined native run passed service transactions, supply classification/restoration, cap and training cases; a subsequent 6-group limits run additionally covered Day Care overflow/fees, Preparation field healing and authored NPC ceilings. These do not establish a complete player journey.

No human playtest, Android game test, simulated balance result or unperformed test is claimed. Evidence will be added with exact source/artifact revisions and commands as each check runs. External validation requirements do not excuse leaving independent implementation unfinished.

## Latest integration checks

Production source `9d479bc5` builds with the current economy and boss curation; its immutable runtime artifact is SHA256`73759e33b3fe975f887e9b6286002ad0b00ef6a0bf8abd8da821fade2025afe3`. The focused native suite executes119 groups,109 passes plus10 expected framework failures, no unexpected failures/skips. This includes progression/save, supplies, training outcomes, caps, service transactions, pace helpers, public AI and battle-effect interactions. These are bounded checks, not whole-game coverage.

Actual opening and pacing runs exposed early Tempo ownership: a new-game visibility flag was mistaken for a ship-rival victory. Source `d323369b` fixes it and its native regression passes. Older pinned runtime evidence retains that defect visibly; continued internal saves must disclose it. The complete game and its human/Android acceptance remain unfinished.
