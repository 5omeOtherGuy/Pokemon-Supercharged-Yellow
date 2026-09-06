"""Structural regression checks for the observation boundary, not gameplay tests."""
from pathlib import Path
import re
import unittest

ENGINE = Path(__file__).resolve().parents[2]


class BoundarySourceTests(unittest.TestCase):
    def test_adapter_has_no_pending_commands_or_battle_rng(self):
        source = (ENGINE / 'src/sc_ai.c').read_text()
        self.assertIn('void ScAiObserve(', source)
        for symbol in ['gChosenMoveByBattler', 'gChosenActionByBattler',
                       'gRngValue', 'gRng2Value', 'Random(', 'RandomPercentage(',
                       'AI_CalcDamage(', 'PokemonToBattleMon(', 'SetMonData(',
                       'gBattleResources->bufferA', 'gBattleResources->bufferB']:
            with self.subTest(symbol=symbol):
                self.assertNotIn(symbol, source)
        self.assertNotRegex(source, r'gBattleMons\[[^\]]+\]\.[A-Za-z_]+\s*=')

    def test_every_legacy_decision_entry_is_guarded(self):
        entries = {
            'battle_ai_main.c': ['ComputeAiBattlerDecisions', 'BattleAI_ChooseMoveIndex',
                                 'SetAiLogicDataForTurn', 'AI_TrySwitchOrUseItem'],
            'battle_ai_switch.c': ['GetMostSuitableMonToSwitchInto'],
            'battle_ai_items.c': ['ShouldUseItem'],
        }
        for filename, functions in entries.items():
            source = (ENGINE / 'src' / filename).read_text()
            for function in functions:
                with self.subTest(function=function):
                    body = re.search(r'\b' + function + r'\([^;]*?\)\s*\{(.{0,500})', source, re.S)
                    self.assertIsNotNone(body)
                    self.assertIn('ScAiEnabled()', body[1])


if __name__ == '__main__':
    unittest.main()
