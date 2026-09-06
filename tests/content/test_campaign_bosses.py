"""Stage, identity and source legality checks; not battle-balance certification."""
from copy import copy
import re
import unittest
from test_campaign import ENGINE, parties, normalized
import test_champion_finale as finale
from script_vm import ScriptVM

GYMS=('BROCK','MISTY','LT_SURGE','ERIKA','KOGA','SABRINA','BLAINE','GIOVANNI')
CAPS=(15,22,28,36,44,50,56,62,68,75)
CEILINGS=(24,32,40,48,56,64,72,80,96,96)
REMATCH_STAGES=((3,5,9),(4,6,9),(5,7,9),(6,8,9),(7,8,9),(8,8,9),(9,9,9),(9,9,9))
# Hideout/Tower and Silph are explicit root-owned acceptance-gate dependencies.
STORY_STAGES={'OAKS_LAB':0,'ROUTE22_EARLY':0,'CERULEAN':1,'SS_ANNE':1,
              'POKEMON_TOWER':3,'SILPH':5,'ROUTE22_LATE':8}
BRANCHES={'CHARMANDER':('Jolteon',{'Ninetales','Cloyster'}),
          'BULBASAUR':('Flareon',{'Cloyster','Magneton'}),
          'SQUIRTLE':('Vaporeon',{'Ninetales','Magneton'})}

def stage_for(trainer):
    if trainer.startswith('TRAINER_LEADER_'):return GYMS.index(trainer[15:])
    if trainer.startswith('TRAINER_SC_') and '_REMATCH_' in trainer:
        name,tier=trainer[11:].rsplit('_REMATCH_',1)
        return REMATCH_STAGES[GYMS.index(name)][int(tier)-1]
    if trainer.startswith('TRAINER_RIVAL_'):
        story=trainer[14:].rsplit('_',1)[0]
        return STORY_STAGES[story]
    if trainer.startswith('TRAINER_BOSS_GIOVANNI'):return 5 if trainer.endswith('_2') else 3
    if trainer.startswith('TRAINER_SC_ROCKET_DUO_'):return (1,3,3,5)[int(trainer[-1])-1]
    return None

def allocated():return {k:v for k,v in parties().items() if stage_for(k) is not None}

class BossTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        finale.FinaleTests.setUpClass.__func__(cls)
        cls.species_keys={normalized(k):k for k in cls.roster}
        cls.vm_template=ScriptVM(ENGINE)

    def test_every_allocated_team_has_stage_bounded_legal_explicit_data(self):
        self.assertEqual(len(allocated()),59)
        for trainer,body in allocated().items():
            stage=stage_for(trainer);team=finale.members(body)
            with self.subTest(trainer=trainer):
                self.assertGreaterEqual(len(team),1);self.assertLessEqual(len(team),6)
                self.assertLessEqual(max(m['level'] for m in team),CAPS[stage])
            for mon in team:
                with self.subTest(trainer=trainer,species=mon['species']):
                    key=self.species_keys[normalized(mon['species'])];data=self.roster[key];block=mon['block']
                    ability=re.search(r'^Ability: (.+)$',block,re.M);self.assertIsNotNone(ability)
                    available={normalized(a) for a in re.findall(r'ABILITY_(\w+)',re.search(r'\.abilities\s*=\s*\{([^}]+)',data['block'])[1])}
                    self.assertIn(normalized(ability[1]),available)
                    self.assertRegex(block,r'(?m)^Nature: \w+$')
                    training=re.search(r'^EVs: (.+)$',block,re.M);self.assertIsNotNone(training)
                    values=list(map(int,re.findall(r'\d+',training[1])))
                    self.assertEqual(len(values),6);self.assertLessEqual(max(values),CEILINGS[stage])
                    expected=2 if 'OAKS_LAB' in trainer else 4
                    self.assertEqual(len(mon['moves']),expected);self.assertEqual(len(set(mon['moves'])),expected)
                    legal={v for l,v in data['learnset'] if 0<l<=mon['level']}
                    legal.update(self.moves[m] for m in self.learnables.get(key,set()) if m[5:] in self.tm_moves and m in self.moves)
                    for move in mon['moves']:
                        self.assertIn(self.move_ids[normalized(move)],legal,(trainer,key,move))

    def test_rematch_levels_fit_actual_earliest_script_dispatch(self):
        minimum={}
        for badge in range(10):
            for who in GYMS:
                for won in range(3):
                    vm=copy(self.vm_template);vm.flags={'FLAG_BADGE%02d_GET'%i for i in range(1,min(badge,8)+1)}
                    vm.vars={};vm.battles=[]
                    if badge==9:vm.flags.add('FLAG_IS_CHAMPION')
                    vm.flags.update('FLAG_SC_'+who+'_REMATCH_'+str(i) for i in range(1,won+1))
                    vm.run('ScY_Rematch_'+who)
                    for trainer in vm.battles:minimum.setdefault(trainer,badge)
        self.assertEqual(len(minimum),24)
        for trainer,stage in minimum.items():
            self.assertEqual(stage,stage_for(trainer),trainer)
            self.assertLessEqual(max(m['level'] for m in finale.members(parties()[trainer])),CAPS[stage])

    def test_rival_companions_follow_yellow_branch_identity(self):
        for suffix,(ace,companions) in BRANCHES.items():
            tower=finale.members(parties()['TRAINER_RIVAL_POKEMON_TOWER_'+suffix])
            precursor={'Ninetales':'Vulpix','Cloyster':'Shellder','Magneton':'Magnemite'}
            self.assertEqual({m['species'] for m in tower},{'Fearow','Sandshrew','Eevee'}|{precursor[c] for c in companions})
            for story in ('SILPH','ROUTE22_LATE'):
                team=finale.members(parties()['TRAINER_RIVAL_'+story+'_'+suffix])
                self.assertEqual(team[-1]['species'],ace)
                self.assertTrue(companions <= {m['species'] for m in team})

    def test_doubles_avoid_friendly_fire_and_inherited_evasion_loops(self):
        banned={'Surf','Earthquake','Bulldoze','Discharge','Self Destruct','Explosion','Double Team','Minimize','Dream Eater','Nightmare'}
        for trainer,body in allocated().items():
            if 'Double Battle: Yes' not in body:continue
            team=finale.members(body);self.assertGreaterEqual(len(team),2)
            moves={m for mon in team for m in mon['moves']}
            self.assertFalse(moves & banned,(trainer,moves & banned))

    def test_gym_capability_aces_keep_signature_identity(self):
        aces=dict(zip(GYMS,('Onix','Starmie','Raichu','Vileplume','Weezing','Alakazam','Arcanine','Rhydon')))
        for trainer,body in allocated().items():
            if trainer.startswith('TRAINER_LEADER_'):who=trainer[15:]
            elif '_REMATCH_' in trainer:who=trainer[11:].rsplit('_REMATCH_',1)[0]
            else:continue
            team=finale.members(body)
            ace=max(enumerate(team),key=lambda pair:(pair[1]['level'],pair[0]))[1]
            self.assertEqual(ace['species'],aces[who],trainer)
            self.assertGreaterEqual(len(team),2)

if __name__=='__main__':unittest.main()
