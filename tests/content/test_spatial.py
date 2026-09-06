"""Static collision checks for authored actors and fixed opening movement paths."""
import json
import re
import struct
import unittest
from test_campaign import ENGINE

class SpatialContentTests(unittest.TestCase):
    def layout(self,map_name):
        data=json.loads((ENGINE/'data/maps'/map_name/'map.json').read_text())
        layouts=json.loads((ENGINE/'data/layouts/layouts.json').read_text())['layouts']
        layout=next(x for x in layouts if x['id']==data['layout'])
        raw=(ENGINE/layout['blockdata_filepath']).read_bytes()
        tiles=struct.unpack('<'+'H'*(len(raw)//2),raw)
        return data,layout,tiles
    def test_new_actors_stand_on_open_floor(self):
        for path in (ENGINE/'data/maps').glob('*_Frlg/map.json'):
            data,layout,tiles=self.layout(path.parent.name)
            for actor in data.get('object_events',[]):
                if not actor.get('script','').startswith('ScY_'):continue
                tile=tiles[actor['y']*layout['width']+actor['x']]
                self.assertEqual((tile>>10)&3,0,(path.parent.name,actor['script'],actor['x'],actor['y']))
    def test_lab_rival_fixed_paths_do_not_cross_walls(self):
        data,layout,tiles=self.layout('PalletTown_ProfessorOaksLab_Frlg')
        text=(ENGINE/'data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc').read_text()
        x,y=5,4
        for label in ['ScY_RivalTakeEeveeMovement','ScY_RivalLeaveLabMovement']:
            body=re.search(label+r'::\n(.*?)(?=^\w+::)',text,re.M|re.S)[1]
            for direction in re.findall(r'^\s*walk_(up|down|left|right)\s*$',body,re.M):
                dx,dy={'up':(0,-1),'down':(0,1),'left':(-1,0),'right':(1,0)}[direction]
                x+=dx;y+=dy
                self.assertEqual((tiles[y*layout['width']+x]>>10)&3,0,(label,x,y))
        self.assertEqual((x,y),(6,12))

if __name__=='__main__':unittest.main()
