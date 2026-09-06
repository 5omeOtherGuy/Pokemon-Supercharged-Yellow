"""Evaluate authored Kanto shops, pickups and opt-in earning script paths."""
import json
from pathlib import Path
import re
import unittest
ROOT=Path(__file__).resolve().parents[2]
MAPS=ROOT/'engine/data/maps'
DISABLED={'ITEM_'+s for s in ('HP_UP','PROTEIN','IRON','CALCIUM','ZINC','CARBOS','POMEG_BERRY','KELPSY_BERRY','QUALOT_BERRY','HONDEW_BERRY','GREPA_BERRY','TAMATO_BERRY')}
ISLANDS=('OneIsland','TwoIsland','ThreeIsland','FourIsland','FiveIsland','SixIsland','SevenIsland','TrainerTower','NavelRock','BirthIsland')

def blocks(text):
    return {m.group(1):m.group(2) for m in re.finditer(r'^(\w+)::\n(.*?)(?=^\w+::|\Z)',text,re.M|re.S)}

def script(mapname):return (MAPS/(mapname+'_Frlg')/'scripts.inc').read_text()

def run(text,label,flags=(),defeated=False,yes=False):
    """Small declared script subset; unknown commands fail rather than guess."""
    data=blocks(text);pending=data[label].splitlines();result=0;shops=[];battles=[];prompts=0
    for _ in range(100):
        if not pending:break
        line=pending.pop(0).strip()
        if not line or line.startswith('@'):continue
        op,*rest=line.split(None,1);args=rest[0].split(', ') if rest else []
        if op in ('end','return'):break
        if op in ('lock','lockall','release','releaseall','faceplayer','waitmessage','message','closemessage'):continue
        if op=='msgbox':
            if len(args)>1 and args[1]=='MSGBOX_YESNO':prompts+=1;result=int(yes)
        elif op=='goto':pending=data[args[0]].splitlines()
        elif op=='goto_if_set':
            if args[0] in flags:pending=data[args[1]].splitlines()
        elif op=='goto_if_eq':
            if result=={'YES':1,'NO':0,'TRUE':1,'FALSE':0}.get(args[1],-1):pending=data[args[2]].splitlines()
        elif op=='pokemart':shops.extend(re.findall(r'\.2byte (ITEM_\w+)',data[args[0]]))
        elif op=='trainerbattle_single':
            if not defeated:
                battles.append(args[0]);defeated=True
                if len(args)>3:pending=data[args[3]].splitlines()
        elif op=='trainerbattle_no_intro':battles.append(args[0])
        else:raise AssertionError('unsupported script instruction: '+line)
    else:raise AssertionError('script did not terminate')
    return shops,battles,prompts

class EconomyContract(unittest.TestCase):
    def test_repeatable_equipment_is_staged(self):
        early=run(script('CeladonCity_DepartmentStore_5F'),'CeladonCity_DepartmentStore_5F_EventScript_ClerkVitamins')[0]
        late=run(script('CeladonCity_DepartmentStore_5F'),'CeladonCity_DepartmentStore_5F_EventScript_ClerkVitamins',flags={'FLAG_BADGE04_GET'})[0]
        self.assertFalse(DISABLED.intersection(early+late))
        self.assertNotIn('ITEM_LIGHT_BALL',early)
        self.assertIn('ITEM_LIGHT_BALL',late)
        self.assertTrue({'ITEM_FOCUS_SASH','ITEM_AIR_BALLOON','ITEM_LIGHT_CLAY','ITEM_SILVER_POWDER','ITEM_SHARP_BEAK'}.issubset(early))
    def test_early_berries_and_indigo_replacements(self):
        for town in ('ViridianCity','PewterCity'):
            items=run(script(town+'_Mart'),town+'_Mart_EventScript_Clerk',flags={'FLAG_SYS_POKEDEX_GET'})[0]
            self.assertTrue({'ITEM_ORAN_BERRY','ITEM_CHERI_BERRY','ITEM_CHESTO_BERRY','ITEM_PECHA_BERRY'}.issubset(items),town)
        items=run(script('IndigoPlateau_PokemonCenter_1F'),'IndigoPlateau_PokemonCenter_1F_EventScript_Clerk')[0]
        self.assertTrue({'ITEM_SITRUS_BERRY','ITEM_LUM_BERRY','ITEM_LEPPA_BERRY','ITEM_FOCUS_SASH','ITEM_AIR_BALLOON','ITEM_HYPER_POTION','ITEM_ETHER'}.issubset(items))
    def test_cerulean_solo_vendor_has_no_wireless_currency(self):
        text=script('CeruleanCity_House5')
        items=run(text,'CeruleanCity_House5_EventScript_BerryPowderMan')[0]
        self.assertTrue({'ITEM_SITRUS_BERRY','ITEM_LUM_BERRY','ITEM_LEPPA_BERRY','ITEM_EVIOLITE','ITEM_MAGNET'}.issubset(items))
        self.assertNotIn('DisplayBerryPowderVendorMenu',text)
    def test_disabled_training_items_have_no_mainland_pickup(self):
        for folder in MAPS.glob('*_Frlg'):
            if folder.name.startswith(ISLANDS):continue
            data=json.loads((folder/'map.json').read_text())
            for event in data.get('object_events',[])+data.get('bg_events',[]):
                self.assertNotIn(event.get('item'),DISABLED,folder.name)
                self.assertFalse(re.search(r'Item(?:HPUp|Protein|Iron|Calcium|Zinc|Carbos)$',event.get('script','')),folder.name)
    def test_practice_preserves_first_battle_and_requires_consent(self):
        for mapname,label,trainer in [('ViridianForest','ViridianForest_EventScript_Rick','TRAINER_BUG_CATCHER_RICK'),('Route24','ScEconomy_Route24_Ethan','TRAINER_CAMPER_ETHAN')]:
            text=script(mapname);entry=blocks(text)[label].strip().splitlines()[0]
            self.assertTrue(entry.startswith('trainerbattle_single '+trainer+','))
            self.assertEqual(run(text,label)[1:],([trainer],0))
            self.assertEqual(run(text,label,defeated=True,yes=False)[1:],([],1))
            self.assertEqual(run(text,label,defeated=True,yes=True)[1:],([trainer],1))
            self.assertNotRegex(blocks(text)[label],r'(addmoney|givemoney|cleartrainerflag|settrainerflag)')
    def test_replacement_prices_bound_retry_cost(self):
        text=(ROOT/'engine/src/data/items.h').read_text()
        expected={'ORAN_BERRY':80,'SITRUS_BERRY':200,'LUM_BERRY':300,'LEPPA_BERRY':200,'FOCUS_SASH':500,'AIR_BALLOON':300,'EVIOLITE':3000,'LIGHT_BALL':5000,'POTION':200,'SUPER_POTION':400,'HYPER_POTION':800,'FULL_HEAL':300,'ETHER':400}
        for item,price in expected.items():
            body=re.search(r'\[ITEM_'+item+r'\] =\n    \{(.*?)\n    \},',text,re.S).group(1)
            expressions=re.findall(r'\.price = ([^,]+),',body)
            self.assertTrue(expressions,item)
            self.assertTrue(all(e.strip()==str(price) for e in expressions),(item,expressions))

if __name__=='__main__':unittest.main()
