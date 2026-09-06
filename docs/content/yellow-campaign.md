# Yellow campaign content

This content adapts the Yellow campaign to the imported FRLG maps. The reference is pret/pokeyellow `e89ead154b9968aa50eed9328ff2b38b6c194382`; engine import is rh-hideout/pokeemerald-expansion `7644501f3065b38aea2cfc64b5b58ca011bce4f5`. Original upstream artwork and scripts retain their upstream provenance. Jessie and James use existing Rocket character artwork with explicitly named dialogue and paired encounters.

## Story and acquisition

| Anchor | Script entry / location | Behavior |
|---|---|---|
| Oak catches Pikachu | `PalletTown_Frlg/ScY_OakCapture` | Native portrait, cry, ball sounds and capture dialogue before escort; this is a field scene, not a scripted catching battle. |
| Rival takes Eevee; Pikachu gift | `PalletTown_ProfessorOaksLab_Frlg/ScY_ReceivePikachu` | Automatic level 5 Pikachu and tutorial battle. Pikachu escapes its ball in the scene; no walking follower system is added. |
| Rival evolution | `ScY_SetLabRivalBranch`, `ScY_SetRoute22RivalBranch` | Lab loss → Vaporeon; lab win and Route22 loss/skip → Flareon; both wins → Jolteon. `VAR_STARTER_MON` values 2/1/0 respectively. Eevee remains unevolved through Tower; later teams preserve the branch. |
| Starter gifts | Cerulean House4 `ScY_Bulbasaur`; Route24 `ScY_Charmander`; Vermilion `ScY_Squirtle` | Level10, once each, full storage leaves claim available; Squirtle requires Thunderbadge. Melanie's caring motif replaces Yellow's numerical Pikachu happiness check. |
| Jessie and James | `ScY_RocketDuo1`–`4` in MtMoon B2F, Rocket Hideout B4F, Tower7F, Silph11F | Double battles; MtMoon exit ladder guarded; Hideout barrier and Giovanni gated; Fuji rescue gated; Silph Giovanni gated. |
| Tower ghost / Fuji / flute | Existing Tower6F, Tower7F and Mr Fuji's house scripts | Original ghost revelation and rescue progression retained. |
| Both fossils | MtMoon B2F `MtMoon_B2F_EventScript_MiguelFossilPicked` | After reaching Cinnabar, Miguel offers the complementary fossil. Old Amber and all three Cinnabar revival flows retained. |
| Both Hitmons | Dojo ball scripts | Each has a separate successful-delivery flag; original Koichi victory required. |
| Eevee evolutions | Route7 wild Eevee; Celadon roof gift retained | Repeatable Eevee allows all three evolutions. |
| Former NPC trades | Kanto adoption NPC entry scripts | Give the original output species directly, once; no external Pokémon exchange. |
| Mew truck | SSAnne Exterior `ScY_Mew` | Interact with truck at x55–57,y3; level50, only capture retires it. Surf access remains possible after the ship leaves. |
| Moltres | VictoryRoad2F `ScY_Moltres` at31,3 | Level60; only capture retires it. |
| Cerulean Cave | Cerulean `ScY_OpenCeruleanCave` | Champion opens access directly; no Sevii completion dependency. |
| Unique retry | Mew/Moltres/Mewtwo/Articuno/Zapdos and Route12/16 Snorlax | Failed or escaped legendary encounters stay available; uncaught Snorlax clears the road this visit and returns on re-entry. |
| Evolution stones | Celadon DepartmentStore4F | Repeatable Fire/Water/Thunder/Leaf/Moon Stone stock. |

The campaign uses Kanto through the League plus Kanto collection and rematches. Cinnabar invitations and Vermilion ferry dispatch no longer lead to Sevii. Existing inaccessible island sources remain in the broad source database. Kanto opponent species and wild encounters are original151 forms. The root engine patch owns trade-evolution level37, species evolution filtering, starter helper, level caps, field capabilities, training and briefing UI.

Wild conversion preserves the source Yellow habitats where matching FRLG tables exist, using native twelve-slot land weights. `wild-conversion.json` describes the initial conversion. Subsequent explicit recovery additions are Vulpix on Route8 level20 and Kangaskhan in SafariZoneWest level28. The accompanying 151 inventory is source-level coverage, not a completed collection save or spatial playthrough.

## Boss interfaces and rematches

Before every major non-tutorial battle, scripts set `VAR_0x8004` to the actual trainer ID, call `ScShowBossBriefing`, explicitly `waitstate`, and end safely on FALSE. Registration must have implicit waitstate disabled. The special owns two-usable-party checks for double parties. The initial lab rival battle remains the tutorial exception.

Trainer IDs624–627 are the Rocket duos. IDs628–651 are three rematches per gym, ordered Brock/Misty/Surge/Erika/Koga/Sabrina/Blaine/Giovanni. `TRAINERS_COUNT_FRLG` is652, below saved trainer-flag capacity768. Flags0x300–0x30D hold gifts/duos/legendaries; flags0x310–0x31F hold the first two rematch completions. Existing numeric placeholder aliases remain but upstream does not reference those slots.

The prior badge gates the next leader challenge. The first six gyms offer their initial rematch after two later badges; the next tier requires four later badges (capped at eight). Blaine and Giovanni rematches begin after becoming Champion. Each leader's first two tiers remain available in sequence even when visited late; the third requires Champion. All tiers can be repeated until the next opens; the final tier repeats indefinitely. No content script awards capability save bits. Existing repeatable League runs and original151 rematch teams remain available, capped at75.

Misty, Erika, Sabrina, Lorelei and Agatha are doubles; other gym/League leaders and Champion are singles. Teams have explicit role moves for gym leaders; rival teams use species level learnsets. These are initial authored teams, not a claim of competitive balance.

## Verification

`python3 -m unittest discover -s tests/content -v` exercises content-level trainer identity/formats, missing-species coverage, all151 family closure, gift transactions, rival outcome branches, declined Rocket/Champion transactions and capture-only legendary retirement. The script interpreter deliberately excludes movement, graphics/audio and native battle execution. The closure test checks data sources and canonical evolution relationships; it does not prove spatial reachability, economy or the root evolution implementation.

First full native build reached event script assembly and stopped on the expected missing `ScShowBossBriefing` registration. With registration imported, native compilation and assembly pass; the fourth build reaches the linker with only the expected undefined `ScShowBossBriefing` implementation dependency. Rocket trainer name overflow and obsolete removed Tower grunt references were corrected during these builds. End-to-end opening, each story trigger, dock return, truck interaction, gym order, rematches, storage overflow, blackout and save/reload must be exercised in the integrated ROM before any complete-playthrough claim.
