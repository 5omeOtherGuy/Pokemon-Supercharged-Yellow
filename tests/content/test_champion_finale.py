"""Source-backed League regression; no simulated battle or human balance claim."""
import re
import sys
import unittest
from test_campaign import ENGINE, ROOT, parties, normalized
from script_vm import ScriptVM

sys.path.insert(0, str(ROOT/'tests/roster'))
from roster_data import evaluate
sys.path.insert(0, str(ENGINE/'tools/learnset_helpers'))
from make_learnables import from_batch

BRANCHES = {0: ('CHARMANDER', 'Jolteon', {'Cloyster','Ninetales'}),
            1: ('BULBASAUR', 'Flareon', {'Magneton','Cloyster'}),
            2: ('SQUIRTLE', 'Vaporeon', {'Ninetales','Magneton'})}
ELITE = ('LORELEI','BRUNO','AGATHA','LANCE')

def league_parties():
    return {key:value for key,value in parties().items()
            if key.startswith(('TRAINER_CHAMPION_', 'TRAINER_ELITE_FOUR_'))}

def members(body):
    rows=[]
    for header, block in re.findall(r'^([^\n:]+)\n(Level:.*?)(?=\n\n|\Z)', body,re.M|re.S):
        species, _, item=header.partition(' @ ')
        rows.append({'species':species,'item':item,'level':int(re.search(r'Level: (\d+)',block)[1]),
                     'moves':re.findall(r'^- (.+)$',block,re.M),'block':block})
    return rows

class FinaleTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.roster,cls.moves=evaluate()
        cls.move_ids={normalized(key[5:]):value for key,value in cls.moves.items()}
        cls.learnables=from_batch(ENGINE/'tools/learnset_helpers/porymoves_files')
        cls.tm_moves=set(re.findall(r'F\((\w+)\)',(ENGINE/'include/constants/tms_hms.h').read_text()))
        cls.item_names={normalized(n) for n in re.findall(r'ITEM_(\w+)',(ENGINE/'include/constants/items.h').read_text())}

    def test_early_results_reach_exact_yellow_champion_and_rematch_rosters(self):
        data=parties()
        for lab_win,route_win,branch in [(0,0,2),(0,1,2),(1,0,1),(1,1,0)]:
            for rematch in (False,True):
                vm=ScriptVM(ENGINE)
                for label,won in [('ScY_SetLabRivalBranch',lab_win),('ScY_SetRoute22RivalBranch',route_win)]:
                    vm.outcome='B_OUTCOME_WON' if won else 'B_OUTCOME_LOST';vm.run(label)
                vm.run('PokemonLeague_ChampionsRoom_EventScript_'+('Rematch' if rematch else 'Battle'))
                suffix,ace,partners=BRANCHES[branch]
                expected='TRAINER_CHAMPION_'+('REMATCH_' if rematch else 'FIRST_')+suffix
                self.assertEqual(vm.battles,[expected]);self.assertEqual(vm.vars['VAR_0x8004'],expected)
                team=members(data[expected]);self.assertEqual(team[-1]['species'],ace)
                self.assertEqual({m['species'] for m in team},{'Sandslash','Alakazam','Exeggutor',ace}|partners)

    def test_every_league_mon_has_stage_legal_explicit_traits_and_moves(self):
        for trainer,body in league_parties().items():
            cap=75 if ('REMATCH' in trainer or trainer.endswith('_2')) else 68
            team=members(body)
            with self.subTest(trainer=trainer):
                self.assertEqual(len(team),6 if 'CHAMPION' in trainer else 5)
                self.assertLessEqual(max(m['level'] for m in team),cap)
                self.assertGreaterEqual(min(m['level'] for m in team),cap-5)
                self.assertEqual(body.count('Items: Max Potion / Max Potion'),1)
            for mon in team:
                with self.subTest(trainer=trainer,species=mon['species']):
                    name=mon['species'].upper();self.assertIn(name,self.roster)
                    block=mon['block'];data=self.roster[name]
                    ability=re.search(r'^Ability: (.+)$',block,re.M)
                    self.assertIsNotNone(ability)
                    available={normalized(a) for a in re.findall(r'ABILITY_(\w+)',re.search(r'\.abilities\s*=\s*\{([^}]+)',data['block'])[1])}
                    self.assertIn(normalized(ability[1]),available)
                    self.assertRegex(block,r'(?m)^Nature: \w+$')
                    training=re.search(r'^EVs: (.+)$',block,re.M);self.assertIsNotNone(training)
                    self.assertEqual(len(re.findall(r'\d+',training[1])),6)
                    self.assertTrue(all(int(v)<=96 for v in re.findall(r'\d+',training[1])))
                    self.assertEqual(len(mon['moves']),4);self.assertEqual(len(set(mon['moves'])),4)
                    self.assertTrue(not mon['item'] or normalized(mon['item']) in self.item_names)
                    legal={v for level,v in data['learnset'] if 0<level<=mon['level']}
                    legal.update(self.moves[m] for m in self.learnables.get(name,set()) if m[5:] in self.tm_moves and m in self.moves)
                    for move in mon['moves']:
                        self.assertIn(normalized(move),self.move_ids)
                        self.assertIn(self.move_ids[normalized(move)],legal,(trainer,name,move))

    def test_doubles_keep_partner_safe_damage_and_practical_support(self):
        forbidden={'Surf','Earthquake','Discharge','Explosion','Self Destruct','Double Team','Minimize','Dream Eater','Nightmare','Sheer Cold'}
        for who in ('LORELEI','AGATHA'):
            for suffix in ('','_2'):
                body=parties()['TRAINER_ELITE_FOUR_'+who+suffix]
                self.assertIn('Double Battle: Yes',body)
                moves={move for mon in members(body) for move in mon['moves']}
                self.assertFalse(moves&forbidden,moves&forbidden)
                self.assertIn('Protect',moves)
                self.assertTrue(moves&{'Icy Wind','Tailwind','Thunder Wave','Glare'})

    def test_counterplay_moves_exist_by_league_cap_on_ordinary_families(self):
        # These are the level-up tools cited in the balance notes, not assumed TMs.
        tools={'RAICHU':('THUNDERBOLT','LIGHT_SCREEN'), 'BLASTOISE':('ICY_WIND','PROTECT'),
               'CHARIZARD':('FLAMETHROWER','AIR_SLASH','TAILWIND'),
               'VENUSAUR':('SEED_BOMB','SUNNY_DAY'), 'SNORLAX':('CRUNCH','HIGH_HORSEPOWER'),
               'PIDGEOT':('TAILWIND','ROOST'), 'NIDOKING':('EARTH_POWER',),
               'CLEFABLE':('MOONBLAST','MISTY_TERRAIN'),
               'BUTTERFREE':('BUG_BUZZ','TAILWIND'), 'ALAKAZAM':('PSYCHIC',),
               'MACHAMP':('KNOCK_OFF',), 'LAPRAS':('ICE_BEAM','FREEZE_DRY')}
        for species,moves in tools.items():
            learned={move for level,move in self.roster[species]['learnset'] if 0<level<=68}
            for move in moves:
                self.assertIn(self.moves['MOVE_'+move],learned,(species,move))

    def test_elite_four_first_and_rematch_dispatch_use_same_disclosed_id(self):
        for who in ELITE:
            title=who.title()
            for rematch in (False,True):
                vm=ScriptVM(ENGINE)
                if rematch: vm.flags.add('FLAG_IS_CHAMPION')
                for code in vm.code.values():
                    if 'special ScShowBossBriefing' in code:
                        code.insert(code.index('special ScShowBossBriefing'),
                                    'copyvar VAR_SC_TEST_SCOUTED_ID, VAR_0x8004')
                for door in ('OpenDoor','OpenDoorLance'):
                    vm.code['PokemonLeague_EventScript_'+door]=['return']  # animation boundary
                vm.run('PokemonLeague_'+title+'sRoom_EventScript_'+title)
                expected='TRAINER_ELITE_FOUR_'+who+('_2' if rematch else '')
                self.assertEqual(vm.battles,[expected])
                self.assertEqual(vm.vars['VAR_SC_TEST_SCOUTED_ID'],expected)

    def test_finales_and_rematches_remain_declineable_without_battle(self):
        for branch in BRANCHES:
            for rematch in (False,True):
                vm=ScriptVM(ENGINE);vm.vars['VAR_STARTER_MON']=branch;vm.accept=0
                vm.run('PokemonLeague_ChampionsRoom_EventScript_'+('Rematch' if rematch else 'Battle'))
                self.assertEqual(vm.battles,[]);self.assertNotIn('FLAG_DEFEATED_CHAMP',vm.flags)

if __name__=='__main__':unittest.main()
