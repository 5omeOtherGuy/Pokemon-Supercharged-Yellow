# Android runtime evidence

On 2026-09-06, frozen source `9d479bc5` booted in an actual Android 11/API30 AOSP x86_64 VM. RetroArch `1.22.2_GIT` with mGBA `0.11-219-e31759b` loaded the 32 MB game, restored an imported ordinary desktop save, ran native game menus and wrote an ordinary in-game save. A fresh app process restored that save and its QUICK battle-pace setting. A subsequent VM cold boot preserved the same SRAM hash and restored the Center and QUICK setting through Continue. **Android battles, the complete journey, physical-phone coverage and human experience remain unfinished.**

## Target and provenance

Google Android Emulator 37.1.11.0 runs system-image revision 11 at 960×540 landscape, with KVM, two virtual CPU cores, 1536 MB guest RAM and software GLES. The installed RetroArch package reports versionCode 1763607132. The [retained manifest](../../tests/android/evidence/9d479bc5/manifest.json) pins the ROM, save and screenshot hashes. This is x86_64 emulation of Android, not a physical ARM phone.

The exact game SHA256 is `73759e33b3fe975f887e9b6286002ad0b00ef6a0bf8abd8da821fade2025afe3`. The desktop save originated in the [ordinary Yellow opening](yellow-opening.md) on earlier source `195bd43e`; a separate copy was imported without RAM writes, cheats or state injection. Its 131088 bytes include the desktop mGBA RTC tail. That historical save retains the documented premature Tempo ownership defect, with the passive unassigned. This checks cross-build and cross-platform compatibility; it is not a new Android playthrough.

## Observed sequence

| Check | Actual result |
| --- | --- |
| Launch and normal Start | Intro and inherited title render; normal Start reaches the game menu. The inherited developer Quickstart HUD was exposed and subsequently disabled in source `5d0fc594`. This older artifact retains it. |
| Imported save | Continue shows player A, two Pokédex entries, zero badges and the prior play time. Viridian Center location restores. |
| Party and custom interface | Pikachu 6 with 21/21 HP and Pidgey 4 with 18/18 HP restore. Party → Training opens the field inspection screen and its six training values, ceiling 24 and Balanced focus. Back returns safely. |
| Pace controls | D-pad/A/B navigation selects NORM, FAST and QUICK. Only menu selection and persistence were tested here; no Android speed measurement is claimed. |
| Ordinary save | SAVE → Yes → overwrite Yes reaches saving and returns to the Center. RetroArch writes 131072 bytes with SHA256 `cad385783f996956c8eeb61598cb080591e0c4932152dd273f9024c308e444b4`. Its normal core serialization omits the imported RTC tail; the operator did not truncate it. |
| Fresh process | Close Content and Quit end the app. Launching the same ROM again shows the newer save time; Continue restores the Center, and Options visibly retains QUICK. |
| VM cold boot | SRAM hash is unchanged; Continue restores the Center and Options retains QUICK. |
| Touch controls | A/B/Start and D-pad taps operate game menus. A held down input moves the character through the Center. No controller or physical-touch comfort assessment was performed. |
| Audio output | An eight-second excerpt of the VM output contains 690526 nonzero 16-bit samples, peak 316, stereo 44.1 kHz. This establishes non-silent output during the recorded menus/Center session. Human listening quality, latency, synchronization and Android battle audio remain unassessed. |

Selected evidence: [restored party](../../tests/android/evidence/9d479bc5/015-save-compat-party.png), [field training](../../tests/android/evidence/9d479bc5/017-field-training.png), [save overwrite](../../tests/android/evidence/9d479bc5/026-save-overwrite-ready.png), [fresh Continue](../../tests/android/evidence/9d479bc5/036-android-save-continue.png), [restored QUICK](../../tests/android/evidence/9d479bc5/037-restarted-options.png), [cold-boot QUICK](../../tests/android/evidence/9d479bc5/045-cold-boot-quick.png). Screenshots were inspected directly. The operator sequence is summarized above; it is not a complete replayable ADB trace.

## Test setup corrections

The default Neo RetroPad exposes unnecessary buttons and overlaps game text at this VM's aspect ratio. The installed GBA overlay was selected for subsequent checks. An initial task-local config copy made through root ADB had the wrong ownership and SELinux file label, so the app fell back to its default overlay and could not save configuration. Restoring the app file's owner and normal `media_rw_data_file` label fixed that setup error; SELinux remained enforcing. The final screenshot visibly uses the GBA overlay. Root ADB served test-file provisioning and inspection; normal game controls did not require root.

The VM uses an isolated ADB server/serial and task-local SDK/AVD. Host microphone and cameras are disabled; the WAV contains VM output only. A later VM exit followed the emulator's graceful shutdown path, and its temporary KVM access entry disappeared. No operator shutdown had been requested at that moment and the cause is unresolved. Preserve the save and logs, retain the cold-boot recovery evidence, and do not describe this as a game crash or completed stability validation.

## Remaining acceptance

Verify the newer artifact's removed developer shortcut and Yellow presentation; exercise actual singles/doubles, scouting, services, supplies, all pace settings and custom effects. Assess audio, sustained performance and readable controls without a concurrent build. The full campaign, all 151 collection and required rematches still need their ordinary journeys. Human testing is required for preparation burden, challenge, reading comfort and enjoyment.

## Owner build boot check — source 131c263d

Under the later coding-first direction, the exact owner ROM SHA256 `07bdf5ef7e07d6486311395f2279ee21ce98b2d8788c175a4cd0549f95987971` booted on the same pinned VM/frontend/core. The new Yellow/Pikachu title is visible without the old Quickstart HUD, and Start reaches New Game/Option. The previous save was backed up and its checksum was unchanged after this check. Only title/menu input was performed. See [owner validation](owner-playtest.md) for screenshots, the clean rebuild and evidence boundaries. New-ROM campaign, PC-tool UI acceptance, sustained performance and physical-phone coverage remain owner checks.
