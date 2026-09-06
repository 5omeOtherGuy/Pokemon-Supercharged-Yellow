# Implementation and release evidence

Updated 2026-09-06. **The game is not complete and no Supercharged Yellow release is playable yet.** This file is the current execution record; the earlier design review is historical.

## Authority and scope

The owner’s 2026-09-06 mandate authorizes full implementation, autonomous outstanding design and numerical choices, subagents, system/tool installation, research, commits, pushes, merges and release. It supersedes the review brief’s documentation-only boundary and previous instructions to stop before engine selection. The accepted game identity and mechanics remain binding; the design interview is not being restarted.

Completion includes the Yellow adventure across FRLG Kanto, all agreed systems and progression, the League, progression-appropriate gym rematches, repeat access to individual rewards, all 151 obtainable on one save without trading, and Mew under the truck. Kanto post-League collection/rematches are in scope; Sevii is not. A prototype, simulator or successful compilation is an intermediate result.

## Verified starting point

- Inspected and fetched repository: `b0022d34030efda2d79e581b16d860ca932b0277`, clean `main` matching `origin/main`.
- Read root instructions, local collaboration workflow, review brief/report and all 26 ADRs, including superseded records and decision history. Independent requirement/history and foundation audits are running.
- The starting revision contains governance/design only: no engine, build, tests, campaign implementation or release artifact.
- Supersession remains 0005 → 0017; 0018 → 0022 → 0023; 0020 → 0021. ADR-0024 is accepted. A stale reference to its proposal status in ADR-0016 was corrected.

## Completion ledger

| Required outcome | Current evidence | Remaining acceptance |
| --- | --- | --- |
| Foundation and reproducible build | Source audit in progress; expansion FireRed build started at pinned upstream `7644501f3065b38aea2cfc64b5b58ca011bce4f5` | Select after representative runtime checks; record provenance, toolchain, reproducibility |
| Yellow/Kanto campaign | No project implementation | Play from new game through League; verify anchors, all gates, repeat access and recovery |
| Battle mechanics, split, Fairy, weather/terrain, Set and speed | Candidate source inspected; no project runtime evidence | Functional and interaction regression tests, UI/AI correctness and measured speed behavior |
| Levels, traits, training, capabilities and trainer builds | Accepted policies; no project implementation | All reward/outcome paths, ceilings, persistence, services and effect combinations |
| Supplies, loss economy, scouting and AI information | Candidate AI has hidden-stat access requiring changes | Entry/locking/action/outcome tests; authoritative disclosure; hidden-data invariance |
| Encounters and balance | No encounter tuning or measured balance | Stage-legal accessible teams in both formats, strongest legal builds, poor traits, specialist burden and retesting |
| Complete collection and required rematches | No implemented acquisition route | All 151 on one save; branches, solo evolutions, missed-encounter recovery and all eight personal gym rewards |
| Save integrity | No project save format or compatibility promise | Party/box/evolution/reload, reward transaction and failure/recovery tests |
| Actual gameplay | No project gameplay performed | Complete organic journey plus clearly labeled diagnostic fixtures and regressions |
| Human experience | No playtester results | Returning-player difficulty, learning, preparation timing, identity and enjoyment evidence; iterate and retest |
| Android | No tested Android target; local KVM accessible, no identified attached Android phone | Record actual Android OS/device or VM and emulator versions; controls/readability/battles/speed/audio/in-game save tests |
| Distribution | No patch | Reproducible release build, exact base checksum, verified patch round-trip, player instructions, attribution and accurate validation report |

## Work in progress

The integration checkout remains on `main`; implementation uses `agent/full-game` in a separate worktree. Native leaf workers audit foundations and requirements and build a real mGBA control harness. Local task allocation and recovery state live in the common Git directory’s `info/agent-tasks.md` and `info/full-game/`; those local records are not release evidence.

No human playtest, Android test, simulated balance result or unperformed test is claimed. Evidence will be added with exact source/artifact revisions and commands as each check runs. External validation requirements do not excuse leaving independent implementation unfinished.
