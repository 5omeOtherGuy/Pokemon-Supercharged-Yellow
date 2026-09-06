"""Campaign data acceptance checks; these do not establish gameplay balance."""

import json
import re
import unittest

from roster_data import ROOT, evaluate


class RosterTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.roster, cls.moves = evaluate()

    def has_move(self, species, move, cap):
        return any(0 < level <= cap and number == self.moves["MOVE_" + move]
                   for level, number in self.roster[species]["learnset"])

    def test_all_original_species_have_valid_compiled_learnsets(self):
        self.assertEqual(len(self.roster), 151)
        for species, data in self.roster.items():
            with self.subTest(species=species):
                moves = data["learnset"]
                self.assertEqual(moves[-1], (0, 65535))
                self.assertTrue(all(0 <= level <= 100 and 0 < move < 65535 for level, move in moves[:-1]))
                self.assertEqual([level for level, _ in moves[:-1]], sorted(level for level, _ in moves[:-1]))
                self.assertTrue(all(1 <= stat <= 255 for stat in data["stats"]))

    def test_evolution_graph_stays_original_and_solo(self):
        for name, row in self.roster.items():
            evolution = re.search(r"\.evolutions\s*=\s*(.*)", row["block"], re.S)
            if evolution:
                with self.subTest(species=name):
                    value = evolution.group(1)
                    self.assertNotIn("EVO_TRADE", value)
                    self.assertTrue(set(re.findall(r"SPECIES_(\w+)", value)) <= self.roster.keys(), value)
                    self.assertNotIn("IF_REGION", value)
                    self.assertNotIn("IF_NOT_REGION", value)

    def test_former_trade_evolutions_are_level_37(self):
        for source, target in (("KADABRA", "ALAKAZAM"), ("MACHOKE", "MACHAMP"),
                               ("GRAVELER", "GOLEM"), ("HAUNTER", "GENGAR")):
            with self.subTest(species=source):
                self.assertRegex(self.roster[source]["block"], rf"EVO_LEVEL,\s*37,\s*SPECIES_{target}")

    def test_pikachu_uses_ordinary_thunder_stone(self):
        self.assertRegex(self.roster["PIKACHU"]["block"], r"EVO_ITEM,\s*ITEM_THUNDER_STONE,\s*SPECIES_RAICHU\s*\}")

    def test_early_doubles_have_distinct_support_options(self):
        for species, move in (("PIKACHU", "HELPING_HAND"), ("CLEFAIRY", "FOLLOW_ME"),
                              ("BUTTERFREE", "STRUGGLE_BUG"), ("PIDGEOTTO", "TAILWIND"),
                              ("PSYDUCK", "ICY_WIND")):
            with self.subTest(species=species, move=move):
                self.assertTrue(self.has_move(species, move, 22))

    def test_weather_and_all_terrain_have_level_access_by_midgame(self):
        for species, move, cap in (("ODDISH", "SUNNY_DAY", 22), ("PSYDUCK", "RAIN_DANCE", 22),
                                   ("GEODUDE", "SANDSTORM", 22), ("SEEL", "SNOWSCAPE", 44),
                                   ("PIKACHU", "ELECTRIC_TERRAIN", 28), ("GLOOM", "GRASSY_TERRAIN", 36),
                                   ("CLEFAIRY", "MISTY_TERRAIN", 28), ("DROWZEE", "PSYCHIC_TERRAIN", 36)):
            with self.subTest(species=species, move=move):
                self.assertTrue(self.has_move(species, move, cap))

    def test_bug_and_flying_keep_late_roles(self):
        for species, moves in (("BEEDRILL", ("U_TURN", "POISON_JAB")),
                                ("BUTTERFREE", ("RAGE_POWDER", "BUG_BUZZ")),
                                ("PARASECT", ("RAGE_POWDER", "LEECH_LIFE")),
                                ("VENOMOTH", ("QUIVER_DANCE", "BUG_BUZZ")),
                                ("PIDGEOT", ("TAILWIND", "ROOST", "HURRICANE")),
                                ("FARFETCHD", ("KNOCK_OFF", "LEAF_BLADE"))):
            for move in moves:
                with self.subTest(species=species, move=move):
                    self.assertTrue(self.has_move(species, move, 56))

    def test_stone_evolution_retains_future_support_and_stab(self):
        for prior, evolved in (("PIKACHU", "RAICHU"), ("NIDORINA", "NIDOQUEEN"),
                               ("NIDORINO", "NIDOKING"), ("CLEFAIRY", "CLEFABLE"),
                               ("VULPIX", "NINETALES"), ("JIGGLYPUFF", "WIGGLYTUFF"),
                               ("GLOOM", "VILEPLUME"), ("GROWLITHE", "ARCANINE"),
                               ("POLIWHIRL", "POLIWRATH"), ("WEEPINBELL", "VICTREEBEL"),
                               ("SHELLDER", "CLOYSTER"), ("EXEGGCUTE", "EXEGGUTOR"),
                               ("STARYU", "STARMIE")):
            for level, move in self.roster[prior]["learnset"]:
                if level <= 1 or move == 65535:
                    continue
                with self.subTest(prior=prior, evolved=evolved, level=level, move=move):
                    self.assertTrue(any(evolved_move == move and evolved_level <= level
                                        for evolved_level, evolved_move in self.roster[evolved]["learnset"]))

    def test_stone_evolution_does_not_offer_late_attacks_at_level_one(self):
        for species, moves in (("RAICHU", ("THUNDER", "DISCHARGE")),
                                ("NIDOQUEEN", ("EARTH_POWER", "SLUDGE_WAVE")),
                                ("NIDOKING", ("EARTH_POWER", "MEGAHORN")),
                                ("CLEFABLE", ("MOONBLAST",)), ("NINETALES", ("FIRE_BLAST",)),
                                ("ARCANINE", ("FLARE_BLITZ",)), ("CLOYSTER", ("SHELL_SMASH",)),
                                ("EXEGGUTOR", ("LEAF_STORM",)), ("STARMIE", ("HYDRO_PUMP",))):
            for move in moves:
                with self.subTest(species=species, move=move):
                    self.assertFalse(self.has_move(species, move, 1))

    def test_manifest_covers_the_entire_active_roster(self):
        from export_manifest import manifest
        actual = manifest()
        self.assertEqual(json.loads((ROOT / "docs/balance/roster-manifest.json").read_text()), actual)
        for name, row in actual["species"].items():
            with self.subTest(species=name):
                self.assertEqual(row["baseline"]["types_expression"], row["curated"]["types_expression"])
                before = row["baseline"]["stats_hp_atk_def_speed_spatk_spdef"]
                after = row["curated"]["stats_hp_atk_def_speed_spatk_spdef"]
                self.assertLessEqual(sum(after) - sum(before), 30)
                self.assertLessEqual(max(abs(a - b) for a, b in zip(after, before)), 15)


if __name__ == "__main__":
    unittest.main()
