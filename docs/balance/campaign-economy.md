# Kanto preparation economy

Implementation base: `4389a899`; economy source `551718f9`, Vs. Seeker delivery correction `691f1be5`. Decisions are delegated initial tuning under ADRs 0006, 0008, 0027, 0029 and 0031; the coordinator owns ADR-0034. This is source and assembly evidence, not measured preparation burden or human balance.

## Access and intended choices

| Stage/source | Repeatable stock and purpose |
| --- | --- |
| Viridian and Pewter Marts, before Brock | Oran, Cheri, Chesto and Pecha Berries. Low-cost held recovery/status protection complements the existing bag medicine without granting an early damage multiplier. Pewter also sells Ether. |
| Cerulean berry specialist (the former Berry Powder house) | Ordinary cash shop: all six basic recovery/status berries, Sitrus, Lum, Leppa, Eviolite and Magnet. Removes wireless Berry Crush and disabled vitamin exchanges. The wall guide explains held versus bag items and permanent training. |
| Cerulean, Vermilion and later mainland Marts | The same berries and Ether remain stocked. Lavender, Fuchsia, Saffron and Cinnabar also sell replacement Focus Sashes and Air Balloons. |
| Celadon Department Store 2F | Repeatable berries, Ether and Full Heal alongside its medicine shelf. |
| Celadon Department Store 5F | Replaces vitamins with an enhancer for each of the eighteen types, Eviolite, Focus Sash, Air Balloon and Light Clay. Retains the separate existing X-item shop. |
| Celadon 5F, after four badges | Adds Light Ball behind `FLAG_BADGE04_GET`; campaign gym order makes this the fourth badge. No free gift or early shop source. |
| Indigo entry Center shop | All replacement berries, Focus Sash, Air Balloon, Ether and Hyper Potion alongside inherited late medicine. Supplies are bought before entering the League; no interior respec/shop is added. |

Sitrus heals a quarter of maximum HP in this configured engine; Oran restores ten HP and costs less. Magnet boosts Electric moves while leaving a held slot unavailable for defensive support. Early Eviolite is a substantial optional purchase for unevolved specialists and Pikachu, with an offensive/evolution opportunity cost. The standard item effect remains unchanged.

Light Ball doubles Pikachu's attacking stats, so delaying its shop until after Misty, Surge and Erika protects those opening matchups. From the Koga/Sabrina stage onward, the starter can fill an offensive specialist role while remaining frail; Raichu retains better base speed and bulk and a freely chosen held slot. Ground immunity to Electric damage, priority, physical pressure and status remain relevant counterplay. These are matchup hypotheses, not demonstrated balance. The original Pokémon stats, typing, ability slots and moves are unchanged by this task.

Focus Sash needs full HP and is vulnerable to chip and multi-hit attacks. Air Balloon can be broken by a non-Ground hit. Both are consumed when triggered and now have affordable repeatable replacements. Silver Powder and Sharp Beak provide ordinary sustained Bug/Flying offense rather than one-time encounter gimmicks. Light Clay makes screening preparation available without modifying its move or held-item effect.

## Prices

Only `.price` fields change in `engine/src/data/items.h`; mechanics, item categories, effects and descriptions retain their canonical implementation. Each altered generation-price branch receives the same selected price. Existing X-item prices, evolution stones and other unlisted prices stay unchanged.

| Price | Items |
| ---: | --- |
| 80 | Oran Berry, Cheri Berry, Chesto Berry, Pecha Berry, Rawst Berry, Aspear Berry |
| 200 | Potion, Sitrus Berry, Leppa Berry |
| 300 | Full Heal, Lum Berry, Air Balloon |
| 400 | Super Potion, Ether |
| 500 | Focus Sash |
| 800 | Hyper Potion |
| 1000 | Silk Scarf, Charcoal, Mystic Water, Magnet, Miracle Seed, Never Melt Ice, Black Belt, Poison Barb, Soft Sand, Sharp Beak, Twisted Spoon, Silver Powder, Hard Stone, Spell Tag, Dragon Fang, Black Glasses, Metal Coat, Fairy Feather |
| 1500 | Light Clay |
| 3000 | Eviolite |
| 5000 | Light Ball |

The Berry specialist's Leppa is cheaper than an Ether with the same basic ten-PP restoration, rewarding use of the new repeatable berry stock. Ether is familiar medicine and available earlier in Pewter. No shop sells instant levels, vitamins or training redistribution. Existing repeatable Moon/Fire/Thunder/Water/Leaf Stones remain at Celadon 4F.

## Earning and retry route

Bug Catcher Rick in Viridian Forest offers an explicit talk-to-rematch prompt after his first victory. His unchanged level-six Weedle and Caterpie provide **72** ordinary prize money per win (`4 × 6 × Bug Catcher class value 3`). He remains available before Brock with no fee, item, badge, timer or Vs. Seeker requirement. Trainer sight retains the initial `trainerbattle_single` opcode and defeated flag; first victory uses its own postbattle callback, so the repeat prompt does not interrupt that first victory. Declining starts no battle.

Camper Ethan on Route 24 similarly repeats his unchanged level-18 Mankey for **360** ordinary money (`4 × 18 × Camper class value 5`). His map object points to a local script with the same first trainer ID. This provides a source on the Cerulean side of Mt. Moon's one-way exit. Access still requires the north-Cerulean route progression and first victory; neither trainer is free money without battle. Free Centers and nearby wild training remain the recovery route for an underprepared party. Later unlocked gym/League rematches supply additional existing earnings. Neither new practice rematch grants artificial EXP, extra money, badges, capability rewards or escalating opponents.

| Example replenishment | Price | Rick wins from zero | Ethan wins from zero |
| --- | ---: | ---: | ---: |
| One Oran Berry | 80 | 2 | 1 |
| Two Potions plus one Antidote (configured Antidote price 200) | 600 | 9 | 2 |
| Two Sitrus plus one Lum held replacements | 700 | 10 | 2 |
| One Focus Sash | 500 | 7 | 2 |

These are arithmetic counts with no loss, Amulet Coin or Pay Day modifier. They do not measure travel, healing, battle duration or player success. Rick is a last-resort early route, not the proposed late-game income source. Repeated losses can still spend supplies and reduce cash to zero, as required; free healing and no-fee rematches prevent a currency requirement from blocking another attempt. Players can also attempt trainers with fewer or no bag supplies.

**Vs. Seeker dependency:** `I_VS_SEEKER_CHARGING` is zero at this base, and the inherited implementation has additional Kanto/rematch issues. Do not cite its gift as a working repeatable earning route. The coordinator owns that repair. This task only fixes the Vermilion gift's ordering: a full bag no longer sets `FLAG_GOT_VS_SEEKER` before delivery, so the player can retry.

## Disabled pickup replacements

Fifteen visible item balls and seven hidden items become usable preparation resources. Existing object positions, graphics, hide flags and hidden-item flags are retained. Local item-ball scripts use the normal `finditem` flow, preserving full-bag retry behavior. Legacy vitamin definitions elsewhere remain for upstream data; these mainland objects no longer reference them. This lowers the sale value of those former vitamin pickups; the overall campaign income/preparation ratio still needs measurement.

| Location | Former reward | New reward |
| --- | --- | --- |
| PokemonMansion 1F | Carbos | Leppa Berry |
| PokemonMansion 1F | Protein | Focus Sash |
| PokemonMansion 2F | Calcium | Ether |
| PokemonMansion 2F | Zinc | Lum Berry |
| PokemonMansion 2F | HPUp | Sitrus Berry |
| PokemonMansion 3F | Iron | Air Balloon |
| RocketHideout B4F | Calcium | Ether |
| Route12 | Iron | Air Balloon |
| Route14 | ZINC | Lum Berry |
| SafariZone North | Protein | Focus Sash |
| SilphCo 10F | Carbos | Leppa Berry |
| SilphCo 10F | HP_UP | Sitrus Berry |
| SilphCo 11F | Zinc | Lum Berry |
| SilphCo 3F | PROTEIN | Focus Sash |
| SilphCo 4F | IRON | Air Balloon |
| SilphCo 5F | Protein | Focus Sash |
| SilphCo 6F | HPUp | Sitrus Berry |
| SilphCo 6F | CARBOS | Leppa Berry |
| SilphCo 7F | Calcium | Ether |
| SilphCo 7F | ZINC | Lum Berry |
| SilphCo 8F | Iron | Air Balloon |
| SilphCo 9F | CALCIUM | Ether |

## Checks, discovery and remaining validation

The Viridian Gentleman now opens with the upstairs development service; a concise local nurse message repeats the preparation direction after the normal nurse callback. Viridian's Mart NPC points to Rick; Cerulean's Mart NPC points to the Berry specialist and Ethan. Celadon 5F signs/NPCs explain equipment, consumption, Light Ball timing and permanent training. Route 13 no longer claims Carbos raises Speed. No shared nurse script is modified.

```sh
python3 -m unittest discover -s tests/economy
python3 tests/economy/compile_scripts.py --generated-engine /absolute/path/to/prepared/engine
python3 tests/economy/check_light_ball_access.py
```

Seven source-evaluation tests pass: staged shop paths, early/late stock, solo berry vendor, all mainland disabled pickups, first/repeat/decline practice paths, replacement prices and full-bag Vs. Seeker delivery. The original six tests failed before implementation (`6a7c0ac7` → `551718f9`); the delivery case independently failed then passed (`b7de64eb` → `691f1be5`). All 29 changed map scripts assemble using the actual text preprocessor, event macros and ARM assembler. This is not a linked game or native runtime test. The small script evaluator explicitly supports its tested instruction subset; it is not a replacement game engine.

**Light Ball integration dependency:** the acceptance guard currently fails because wild Pikachu has `itemRare = ITEM_LIGHT_BALL`, and the FireRed Viridian Forest table retains a level-five Pikachu slot. A shop-only badge gate would be bypassable. The coordinator will remove the rare-held assignment while preserving wild Pikachu; `check_light_ball_access.py` must pass in the integrated source. The earlier audit assumption that there was no wild Pikachu was corrected before this handoff.

Required actual checks include ordinary first victory, trainer sight after defeat, repeated consent/cancel and prize payouts, shops and full-bag behavior, badge-gated stock, lost/consumed-item replacement, nurse discovery and zero-money recovery on both sides of Mt. Moon. Android navigation and complete campaign economic/balance pacing remain untested. Record total time and failed attempts for a trusted team and rotating recruits; do not infer enjoyable preparation from these price tables.

## Integration follow-up — 2026-09-06

The coordinator removed the base Pikachu rare-held Light Ball assignment at `cc2448f0`; wild Pikachu remains available. The explicit access guard now passes. [ADR-0034](../adr/0034-preparation-economy-and-equipment-access.md) records the accepted economy policy. The earlier dependency above describes the worker handoff state.
