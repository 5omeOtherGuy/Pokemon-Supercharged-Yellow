"""Validate repeat parties against reachable map objects and actual source teams."""
import json
import re
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tests/content'))
from test_campaign import parties, ALLOWED, normalized
from test_champion_finale import members
import test_champion_finale as finale

class RematchRoutesTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        finale.FinaleTests.setUpClass.__func__(cls)
        cls.keys={normalized(k):k for k in cls.roster}

    def entries(self):
        path=ROOT/'engine/src/data/rematches_frlg.h'
        self.assertTrue(path.exists(), 'Kanto rematch table must replace Hoenn aliases')
        return re.findall(r'SC_REMATCH\((TRAINER_\w+), (MAP_\w+)\)', path.read_text())

    def test_every_repeat_is_a_real_mainland_outdoor_ordinary_trainer(self):
        entries=self.entries(); self.assertGreaterEqual(len(entries),60); self.assertLessEqual(len(entries),78)
        self.assertEqual(len({t for t,m in entries}),len(entries))
        maps={json.loads(p.read_text())['id']:(p.parent,json.loads(p.read_text())) for p in (ROOT/'engine/data/maps').glob('*_Frlg/map.json')}
        script=(ROOT/'engine/data/scripts/trainers_frlg.inc').read_text(); teams=parties()
        for trainer,mapid in entries:
            with self.subTest(trainer=trainer,map=mapid):
                folder,mapdata=maps[mapid]
                self.assertRegex(folder.name,r'^Route\d+(?:_North|_South)?_Frlg$')
                self.assertEqual(mapdata['map_type'],'MAP_TYPE_ROUTE')
                labels=[]
                for obj in mapdata['object_events']:
                    label=obj.get('script','NULL')
                    if re.search(r'^'+re.escape(label)+r'::\n\s*trainerbattle_(single|double) '+trainer+r',',script,re.M): labels.append(label)
                self.assertTrue(labels, 'trainer must be an actual persistent object')
                body=teams[trainer]; self.assertNotRegex(body,r'(?m)^Items:')
                self.assertNotRegex(trainer,r'LEADER|RIVAL|CHAMPION|TEAM_ROCKET|GIOVANNI')
                self.assertTrue(1<=len(members(body))<=6)
                for mon in members(body):
                    self.assertIn(normalized(mon['species']), ALLOWED)
                    self.assertIn(mon['item'], ('', 'Black Belt'))
                    key=self.keys[normalized(mon['species'])]; data=self.roster[key]
                    legal={v for level,v in data['learnset'] if 0<level<=mon['level']}
                    legal.update(self.moves[m] for m in self.learnables.get(key,set()) if m[5:] in self.tm_moves and m in self.moves)
                    self.assertTrue(legal, 'automatic level-up moves require a real source learnset')
                    for move in mon['moves']:
                        self.assertIn(self.move_ids[normalized(move)], legal, (trainer,key,move))
                # Reachable repeats preserve script branch, full trainer identity and format.
                for label in labels:
                    block=script.split(label+'::\n',1)[1].split('\n\n',1)[0]
                    self.assertIn('ShouldTryRematchBattle',block)
                    rematch=re.search(r'goto_if_eq VAR_RESULT, TRUE, (\w+)',block)[1]
                    target=script.split(rematch+'::\n',1)[1].split('\n\n',1)[0]
                    expected='trainerbattle_rematch_double' if 'Double Battle: Yes' in body else 'trainerbattle_rematch'
                    self.assertRegex(target,r'\b'+expected+' '+trainer+r',')

    def test_training_access_spans_mainland_and_preserves_original_levels(self):
        entries=self.entries(); maps={m for t,m in entries}; teams=parties()
        for m in ('MAP_ROUTE3','MAP_ROUTE6','MAP_ROUTE8','MAP_ROUTE9','MAP_ROUTE11','MAP_ROUTE12','MAP_ROUTE15','MAP_ROUTE17','MAP_ROUTE19','MAP_ROUTE20','MAP_ROUTE21_NORTH','MAP_ROUTE24','MAP_ROUTE25'): self.assertIn(m,maps)
        self.assertEqual(sum(m=='MAP_ROUTE11' for t,m in entries),10)
        self.assertIn(('TRAINER_YOUNGSTER_BEN','MAP_ROUTE3'),entries)
        self.assertGreaterEqual(sum('Double Battle: Yes' in teams[t] for t,m in entries),6)
        self.assertLessEqual(max(mon['level'] for t,m in entries for mon in members(teams[t])),40)

if __name__=='__main__': unittest.main()
