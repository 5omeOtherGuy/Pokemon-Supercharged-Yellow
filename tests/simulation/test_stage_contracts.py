"""Availability and fixture-drift checks; actual damage belongs to the GBA tests."""
import json
from pathlib import Path
import re
import sys
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tests/content"))
sys.path.insert(0, str(ROOT / "tests/roster"))
from test_campaign import ENGINE
from test_campaign_bosses import allocated, stage_for, CAPS
from test_champion_finale import league_parties, members
from roster_data import evaluate
sys.path.insert(0, str(ENGINE / "tools/learnset_helpers"))
from make_learnables import from_batch


class StageContracts(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.roster, cls.moves = evaluate()
        cls.teams = allocated() | league_parties()

    def test_all_73_authored_teams_have_nonempty_stage_bounded_parties(self):
        self.assertEqual(len(allocated()), 59)
        self.assertEqual(len(league_parties()), 14)
        for trainer, body in self.teams.items():
            stage = stage_for(trainer)
            cap = CAPS[stage] if stage is not None else (75 if "REMATCH" in trainer or trainer.endswith("_2") else 68)
            with self.subTest(trainer=trainer):
                team = members(body)
                self.assertTrue(team)
                self.assertTrue(all(1 <= mon["level"] <= cap for mon in team))

    def test_native_party_slot_dependencies_match_authored_data(self):
        expected = {
            "BROCK": ("Geodude", "Onix"),
            "MISTY": ("Psyduck", "Starmie", "Seel"),
            "KOGA": ("Venomoth", "Muk", "Golbat", "Weezing"),
            "SABRINA": ("Mr. Mime", "Hypno", "Kadabra", "Alakazam"),
            "BLAINE": ("Ninetales", "Rapidash", "Magmar", "Arcanine"),
        }
        for gym, species in expected.items():
            with self.subTest(gym=gym):
                self.assertEqual(tuple(mon["species"] for mon in members(self.teams["TRAINER_LEADER_" + gym])), species)

    def test_native_player_levelup_tools_exist_by_fixture_levels(self):
        cases = {
            ("MANKEY", 15): ("LOW_KICK",),
            ("NIDORAN_M", 15): ("DOUBLE_KICK",),
            ("IVYSAUR", 22): ("RAZOR_LEAF",),
            ("PIKACHU", 22): ("HELPING_HAND", "GROWL"),
            ("PIKACHU", 50): ("LIGHT_SCREEN", "QUICK_ATTACK"),
            ("NIDOKING", 44): ("EARTH_POWER",),
            ("SNORLAX", 50): ("CRUNCH",),
            ("GOLDUCK", 56): ("RAIN_DANCE", "TAIL_WHIP", "WATER_PULSE"),
        }
        for (species, level), moves in cases.items():
            learned = {move for learned_level, move in self.roster[species]["learnset"] if 0 < learned_level <= level}
            for move in moves:
                with self.subTest(species=species, move=move, level=level):
                    self.assertIn(self.moves["MOVE_" + move], learned)

    def test_brock_counters_are_land_encounters_before_first_gym(self):
        encounters = json.loads((ENGINE / "src/data/wild_encounters.json").read_text())["wild_encounter_groups"][0]["encounters"]
        route22 = [entry for entry in encounters if entry["map"] == "MAP_ROUTE22" and entry.get("base_label", "").endswith("_FireRed")]
        self.assertEqual(len(route22), 1)
        mons = route22[0]["land_mons"]["mons"]
        for species in ("SPECIES_MANKEY", "SPECIES_NIDORAN_M"):
            with self.subTest(species=species):
                self.assertTrue(any(mon["species"] == species and mon["max_level"] <= 15 for mon in mons))

    def test_fixture_immunities_and_weather_are_real_innate_abilities(self):
        for species, ability in (("GEODUDE", "STURDY"), ("WEEZING", "LEVITATE"),
                                 ("NINETALES", "DROUGHT"), ("GOLDUCK", "SWIFT_SWIM")):
            abilities = re.search(r"\.abilities\s*=\s*\{([^}]+)", self.roster[species]["block"])[1]
            with self.subTest(species=species):
                self.assertIn("ABILITY_" + ability, abilities)

    def test_fixture_tm_tools_have_compatibility_and_mainland_sources(self):
        learnables = from_batch(ENGINE / "tools/learnset_helpers/porymoves_files")
        self.assertIn("MOVE_THUNDERBOLT", learnables["NIDOKING"])
        self.assertIn("MOVE_PROTECT", learnables["SNORLAX"])
        self.assertIn("ITEM_TM24", (ENGINE / "data/maps/CeladonCity_GameCorner_PrizeRoom_Frlg/scripts.inc").read_text())
        self.assertIn("PowerPlant_EventScript_ItemTM17", (ENGINE / "data/maps/PowerPlant_Frlg/map.json").read_text())
        scripts = (ENGINE / "data/scripts/item_ball_scripts_frlg.inc").read_text()
        self.assertRegex(scripts, r"PowerPlant_EventScript_ItemTM17::\s+finditem ITEM_TM17")


if __name__ == "__main__":
    unittest.main()
