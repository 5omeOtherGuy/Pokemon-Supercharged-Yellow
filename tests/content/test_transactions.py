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

if __name__=='__main__':unittest.main()
