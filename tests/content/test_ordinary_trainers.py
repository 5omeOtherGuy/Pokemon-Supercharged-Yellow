"""Reachable mainland ordinary-team source audit; not gameplay balance evidence."""
import json
import re
import unittest
from collections import defaultdict
from test_campaign import ENGINE, normalized, parties
from test_campaign_bosses import stage_for
from test_champion_finale import FinaleTests, members


def mainland_maps():
    """Physical warp/connection component; story locks deliberately remain possible."""
    maps = {data['id']: (path.parent.name, data)
            for path in (ENGINE / 'data/maps').glob('*_Frlg/map.json')
            for data in [json.loads(path.read_text())]}
    seen = set()
    pending = ['MAP_PALLET_TOWN']
    while pending:
        name = pending.pop()
        if name in seen or name not in maps:
            continue
        seen.add(name)
        data = maps[name][1]
        pending.extend(x['dest_map'] for x in data['warp_events'])
        pending.extend(x['map'] for x in data.get('connections') or [])
    return {name: maps[name] for name in seen}


def ordinary_references():
    code = {}
    paths = list((ENGINE / 'data/maps').glob('*_Frlg/scripts.inc'))
    paths += [ENGINE / 'data/scripts/trainers_frlg.inc']
    for path in paths:
        code.update(dict(re.findall(r'^(\w+)::\n(.*?)(?=^\w+::|\Z)', path.read_text(), re.M | re.S)))
    refs = defaultdict(set)
    for folder, data in mainland_maps().values():
        for obj in data['object_events']:
            pending = [obj.get('script', '')]
            seen = set()
            while pending:
                label = pending.pop()
                if label in seen or label not in code:
                    continue
                seen.add(label)
                body = code[label]
                for trainer in re.findall(r'trainerbattle_\w+\s+(TRAINER_\w+)', body):
                    if stage_for(trainer) is None and not trainer.startswith(('TRAINER_CHAMPION_', 'TRAINER_ELITE_FOUR_')):
                        refs[trainer].add(folder)
                pending.extend(x for x in re.findall(r'\b\w+\b', body) if x in code)
    return dict(refs)


def default_moves(learnset, level):
    """Same skip-0, unique, oldest-shift rule as GiveBoxMonInitialMoveset."""
    result = []
    for learned, move in learnset:
        if learned > level:
            break
        if learned and move not in result:
            result.append(move)
            result = result[-4:]
    return result


class OrdinaryTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        FinaleTests.setUpClass.__func__(cls)
        cls.keys = {normalized(k): k for k in cls.roster}
        cls.refs = ordinary_references()
        cls.teams = {key: parties()[key] for key in cls.refs}
        cls.minimum = {}
        cls.acquisitions = {}
        cls.parents = {}
        cls.evolution_parents = {}
        for parent, row in cls.roster.items():
            for target in re.findall(r'EVO_\w+,\s*[^,]+,\s*SPECIES_(\w+)', row['block']):
                if target in cls.roster:
                    cls.evolution_parents[target] = parent
            for level, target in re.findall(r'EVO_LEVEL,\s*(\d+),\s*SPECIES_(\w+)', row['block']):
                cls.minimum[target] = min(cls.minimum.get(target, 100), int(level))
                cls.parents[target] = parent

        reachable = mainland_maps()
        groups = json.loads((ENGINE / 'src/data/wild_encounters.json').read_text())['wild_encounter_groups']
        for group in groups:
            for encounter in group.get('encounters', []):
                if encounter.get('map') not in reachable or not encounter.get('base_label', '').endswith('_FireRed'):
                    continue
                for method in ('land_mons', 'water_mons', 'fishing_mons'):
                    for mon in encounter.get(method, {}).get('mons', []):
                        key = mon['species'][8:]
                        level = mon['min_level']
                        if level < cls.acquisitions.get(key, (101, ''))[0]:
                            cls.acquisitions[key] = (level, encounter['map'])
        for folder, _ in reachable.values():
            source = (ENGINE / 'data/maps' / folder / 'scripts.inc').read_text()
            for key, level in re.findall(r'(?:givemon|setwildbattle) SPECIES_(\w+), (\d+)', source):
                level = int(level)
                if level < cls.acquisitions.get(key, (101, ''))[0]:
                    cls.acquisitions[key] = (level, folder)
        for key, (level, _) in cls.acquisitions.items():
            if key in cls.minimum:
                cls.minimum[key] = min(cls.minimum[key], level)
        info = dict(re.findall(r'\[MOVE_(\w+)\]\s*=\s*\{(.*?)^    \},', (ENGINE / 'src/data/moves_info.h').read_text(), re.M | re.S))
        cls.status_moves = {cls.moves['MOVE_' + key] for key, body in info.items()
                            if re.search(r'\.category\s*=\s*DAMAGE_CATEGORY_STATUS', body)}

    @classmethod
    def legal_moves(cls, key, level):
        legal = {move for learned, move in cls.roster[key]['learnset'] if 0 < learned <= level or learned == 0 and key in cls.evolution_parents}
        legal.update(cls.moves[m] for m in cls.learnables.get(key, set()) if m[5:] in cls.tm_moves and m in cls.moves)
        parent = cls.evolution_parents.get(key)
        if parent:
            legal.update(cls.legal_moves(parent, level))
        return legal

    def test_scope_includes_shared_route_scripts_and_excludes_unused_tiers(self):
        self.assertIn('TRAINER_YOUNGSTER_YASU', self.teams)
        self.assertIn('TRAINER_ENGINEER_BERNIE', self.teams)
        self.assertNotIn('TRAINER_YOUNGSTER_YASU_2', self.teams)
        self.assertNotIn('TRAINER_LEADER_BROCK', self.teams)
        self.assertTrue(all(not any(x.startswith(('OneIsland', 'TwoIsland', 'ThreeIsland', 'FourIsland', 'FiveIsland', 'SixIsland', 'SevenIsland')) for x in maps) for maps in self.refs.values()))

    def test_original_roster_and_level_evolution_thresholds(self):
        errors = []
        for trainer, body in self.teams.items():
            for mon in members(body):
                key = self.keys.get(normalized(mon['species']))
                if key is None or mon['level'] < self.minimum.get(key, 1):
                    errors.append((trainer, mon['species'], mon['level'], self.minimum.get(key), sorted(self.refs[trainer])))
        self.assertEqual(errors, [])

    def test_explicit_moves_and_abilities_are_supported_by_species(self):
        errors = []
        for trainer, body in self.teams.items():
            for mon in members(body):
                key = self.keys[normalized(mon['species'])]
                data = self.roster[key]
                legal = self.legal_moves(key, mon['level'])
                for move in mon['moves']:
                    if self.move_ids.get(normalized(move)) not in legal:
                        errors.append((trainer, key, move))
                ability = re.search(r'^Ability: (.+)$', mon['block'], re.M)
                if ability:
                    available = {normalized(a) for a in re.findall(r'ABILITY_(\w+)', re.search(r'\.abilities\s*=\s*\{([^}]+)', data['block'])[1])}
                    if normalized(ability[1]) not in available:
                        errors.append((trainer, key, ability[1]))
        self.assertEqual(errors, [])

    def test_default_or_explicit_sets_can_contribute(self):
        errors = []
        exceptions = {'METAPOD', 'KAKUNA'}  # Deliberately weak early Bug Catcher stages.
        indirect = {self.moves['MOVE_METRONOME'], self.moves['MOVE_TRANSFORM']}
        for trainer, body in self.teams.items():
            for mon in members(body):
                key = self.keys[normalized(mon['species'])]
                moves = [self.move_ids[normalized(x)] for x in mon['moves']] if mon['moves'] else default_moves(self.roster[key]['learnset'], mon['level'])
                if key not in exceptions and not any(move not in self.status_moves or move in indirect for move in moves):
                    errors.append((trainer, key, mon['level']))
        self.assertEqual(errors, [])

    def test_doubles_have_partner_safe_moves_and_two_members(self):
        banned = {'Surf', 'Earthquake', 'Discharge', 'Bulldoze', 'Self Destruct', 'Explosion', 'Metronome'}
        errors = []
        for trainer, body in self.teams.items():
            if 'Double Battle: Yes' not in body:
                continue
            team = members(body)
            self.assertGreaterEqual(len(team), 2)
            for mon in team:
                key = self.keys[normalized(mon['species'])]
                moves = {self.move_ids[normalized(x)] for x in mon['moves']} if mon['moves'] else set(default_moves(self.roster[key]['learnset'], mon['level']))
                if moves & {self.move_ids[normalized(x)] for x in banned}:
                    errors.append((trainer, key))
        self.assertEqual(errors, [])


if __name__ == '__main__':
    unittest.main()
