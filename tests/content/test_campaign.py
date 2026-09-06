"""Data-level campaign regressions; these checks do not replace emulator playthroughs."""
import json
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ENGINE = ROOT / 'engine'
CANONICAL = 'Bulbasaur Ivysaur Venusaur Charmander Charmeleon Charizard Squirtle Wartortle Blastoise Caterpie Metapod Butterfree Weedle Kakuna Beedrill Pidgey Pidgeotto Pidgeot Rattata Raticate Spearow Fearow Ekans Arbok Pikachu Raichu Sandshrew Sandslash NidoranF Nidorina Nidoqueen NidoranM Nidorino Nidoking Clefairy Clefable Vulpix Ninetales Jigglypuff Wigglytuff Zubat Golbat Oddish Gloom Vileplume Paras Parasect Venonat Venomoth Diglett Dugtrio Meowth Persian Psyduck Golduck Mankey Primeape Growlithe Arcanine Poliwag Poliwhirl Poliwrath Abra Kadabra Alakazam Machop Machoke Machamp Bellsprout Weepinbell Victreebel Tentacool Tentacruel Geodude Graveler Golem Ponyta Rapidash Slowpoke Slowbro Magnemite Magneton Farfetchd Doduo Dodrio Seel Dewgong Grimer Muk Shellder Cloyster Gastly Haunter Gengar Onix Drowzee Hypno Krabby Kingler Voltorb Electrode Exeggcute Exeggutor Cubone Marowak Hitmonlee Hitmonchan Lickitung Koffing Weezing Rhyhorn Rhydon Chansey Tangela Kangaskhan Horsea Seadra Goldeen Seaking Staryu Starmie MrMime Scyther Jynx Electabuzz Magmar Pinsir Tauros Magikarp Gyarados Lapras Ditto Eevee Vaporeon Jolteon Flareon Porygon Omanyte Omastar Kabuto Kabutops Aerodactyl Snorlax Articuno Zapdos Moltres Dratini Dragonair Dragonite Mewtwo Mew'.split()
def normalized(name):
    return re.sub('[^a-z0-9]', '', name.lower())
ALLOWED = {normalized(x) for x in CANONICAL}

def parties():
    text = (ENGINE/'src/data/trainers_frlg.party').read_text()
    return dict(re.findall(r'^=== (\w+) ===\n(.*?)(?=^=== |\Z)', text, re.M|re.S))

class CampaignDataTests(unittest.TestCase):
    def test_all_frlg_trainer_species_are_original_151(self):
        invalid = []
        for trainer, data in parties().items():
            for name in re.findall(r'^([^\n:]+)\nLevel:', data, re.M):
                species = name.split(' @ ')[0]
                if normalized(species) not in ALLOWED:
                    invalid.append((trainer, species))
        self.assertEqual(invalid, [])

    def test_every_yellow_excluded_base_species_has_nontrade_wild_access(self):
        groups = json.loads((ENGINE/'src/data/wild_encounters.json').read_text())['wild_encounter_groups']
        species = set()
        for encounter in groups[0]['encounters']:
            if not encounter.get('base_label', '').endswith('_FireRed'):
                continue
            if any(x in encounter['map'] for x in ['ISLAND', 'MT_EMBER']) and 'SEAFOAM' not in encounter['map'] and 'CINNABAR' not in encounter['map']:
                continue
            for field in ['land_mons', 'water_mons', 'fishing_mons']:
                species.update(mon['species'] for mon in encounter.get(field, {}).get('mons', []))
        required = {'SPECIES_'+s for s in ['WEEDLE','EKANS','MEOWTH','KOFFING','JYNX','ELECTABUZZ','MAGMAR','EEVEE','PIKACHU']}
        self.assertEqual(required - species, set())

    def test_no_kanto_npc_trade_execution(self):
        offenders=[]
        for path in (ENGINE/'data/maps').glob('*_Frlg/scripts.inc'):
            if any(x in path.parent.name for x in ['Island','MtEmber']) and not any(x in path.parent.name for x in ['CinnabarIsland','SeafoamIslands']):
                continue
            if re.search(r'^\s*call EventScript_DoInGameTrade\s*$',path.read_text(),re.M):
                offenders.append(path.parent.name)
        self.assertEqual(offenders, [])

    def test_selected_major_double_formats(self):
        data=parties()
        for name in ['TRAINER_LEADER_MISTY','TRAINER_LEADER_ERIKA','TRAINER_LEADER_SABRINA','TRAINER_ELITE_FOUR_LORELEI','TRAINER_ELITE_FOUR_AGATHA']:
            with self.subTest(trainer=name):
                self.assertRegex(data[name], r'(?m)^Double Battle: Yes$')
                self.assertGreaterEqual(len(re.findall(r'^Level:',data[name],re.M)),2)

if __name__=='__main__': unittest.main()
