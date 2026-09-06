import unittest
from test_campaign import ENGINE
from script_vm import ScriptVM

class ContentTransactionTests(unittest.TestCase):
    def test_gifts_once_decline_and_full_storage(self):
        for script,species in [('ScY_Bulbasaur','BULBASAUR'),('ScY_Charmander','CHARMANDER'),('ScY_Squirtle','SQUIRTLE')]:
            for answer,result,count in [(0,0,0),(1,2,0),(1,0,1),(1,1,1)]:
                with self.subTest(script=script,answer=answer,result=result):
                    vm=ScriptVM(ENGINE);vm.flags.add('FLAG_BADGE03_GET');vm.answer=answer;vm.gift_result=result
                    vm.run(script);vm.run(script)
                    self.assertEqual(vm.gifts,['SPECIES_'+species]*count)
                    if result==2:
                        vm.gift_result=0;vm.run(script)
                        self.assertEqual(vm.gifts,['SPECIES_'+species])
    def test_mew_capture_is_only_permanent_removal(self):
        for outcome in ['B_OUTCOME_CAUGHT','B_OUTCOME_WON','B_OUTCOME_LOST','B_OUTCOME_RAN','B_OUTCOME_PLAYER_TELEPORTED']:
            with self.subTest(outcome=outcome):
                vm=ScriptVM(ENGINE);vm.flags.update(['FLAG_BADGE04_GET','FLAG_BADGE05_GET']);vm.outcome=outcome
                vm.run('ScY_Mew')
                self.assertEqual('FLAG_SC_CAUGHT_MEW' in vm.flags,outcome=='B_OUTCOME_CAUGHT')
    def test_declined_rocket_battle_does_not_advance_story(self):
        for n in range(1,5):
            vm=ScriptVM(ENGINE);vm.accept=0;vm.run('ScY_RocketDuo'+str(n))
            self.assertEqual(vm.battles,[])
            self.assertNotIn('FLAG_SC_DUO_'+str(n),vm.flags)

    def test_yellow_rival_evolution_outcome_matrix(self):
        for lab_win,route_win,expected in [(False,False,2),(False,True,2),(True,False,1),(True,True,0)]:
            vm=ScriptVM(ENGINE)
            vm.outcome='B_OUTCOME_WON' if lab_win else 'B_OUTCOME_LOST'
            vm.run('ScY_SetLabRivalBranch')
            vm.outcome='B_OUTCOME_WON' if route_win else 'B_OUTCOME_LOST'
            vm.run('ScY_SetRoute22RivalBranch')
            self.assertEqual(vm.vars['VAR_STARTER_MON'],expected)
    def test_champion_decline_keeps_scene_retriable_without_victory(self):
        vm=ScriptVM(ENGINE);vm.accept=0
        vm.run('PokemonLeague_ChampionsRoom_EventScript_BattleCharmander')
        self.assertEqual(vm.battles,[])
        self.assertNotIn('FLAG_DEFEATED_CHAMP',vm.flags)
        self.assertEqual(vm.vars['VAR_TEMP_1'],1)
    def test_all_unique_legendaries_only_commit_capture(self):
        for script,flag in [('CeruleanCave_B1F_EventScript_Mewtwo','FLAG_FOUGHT_MEWTWO'),('SeafoamIslands_B4F_EventScript_Articuno','FLAG_FOUGHT_ARTICUNO'),('PowerPlant_EventScript_Zapdos','FLAG_FOUGHT_ZAPDOS'),('ScY_Moltres','FLAG_SC_CAUGHT_MOLTRES')]:
            for outcome in ['B_OUTCOME_CAUGHT','B_OUTCOME_WON','B_OUTCOME_LOST','B_OUTCOME_RAN']:
                vm=ScriptVM(ENGINE);vm.outcome=outcome
                vm.run(script)
                self.assertEqual(flag in vm.flags,outcome=='B_OUTCOME_CAUGHT',(script,outcome))

    def test_cinnabar_visit_unlocks_complementary_fossil_without_sevii(self):
        vm=ScriptVM(ENGINE);vm.run('CinnabarIsland_OnTransition')
        self.assertIn('FLAG_WORLD_MAP_CINNABAR_ISLAND',vm.flags)
        self.assertNotIn('FLAG_WORLD_MAP_ONE_ISLAND',vm.flags)
        self.assertEqual(vm.vars['VAR_MAP_SCENE_CINNABAR_ISLAND'],2)

    def test_cerulean_decline_does_not_close_route22_challenge(self):
        vm=ScriptVM(ENGINE);vm.accept=0
        vm.vars.update(VAR_STARTER_MON=0,VAR_TEMP_1=0,VAR_MAP_SCENE_ROUTE22=1)
        vm.run('CeruleanCity_EventScript_Rival')
        self.assertEqual(vm.vars['VAR_MAP_SCENE_ROUTE22'],1)
        self.assertEqual(vm.battles,[])
    def test_silph_giovanni_decline_does_not_displace_him(self):
        vm=ScriptVM(ENGINE);vm.accept=0;vm.vars['VAR_TEMP_1']=0
        vm.flags.add('FLAG_SC_DUO_4')
        vm.run('SilphCo_11F_EventScript_BattleGiovanni')
        self.assertEqual(vm.battles,[])
        self.assertEqual(vm.movements,[])

if __name__=='__main__':unittest.main()
