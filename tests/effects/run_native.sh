#!/usr/bin/env bash
set -euo pipefail
project_dir=$(cd "$(dirname "$0")/../.." && pwd)
cd "$project_dir"
mkdir -p build/battle-effects
sources='test/test_runner.c test/test_runner_battle.c test/test_runner_args.c test/sc_effects.c test/sc_training.c test/sc_limits.c test/sc_pace.c test/sc_unlocks.c test/sc_progression.c test/sc_save.c test/sc_services/transactions.c test/sc_supplies/status_effect.c test/sc_supplies/selection.c test/sc_effects/runtime.c test/sc_effects/held_consumption.c test/sc_effects/isolation.c test/sc_ai/boundary.c test/sc_ai/effect_sensitivity.c test/battle/damage_formula.c test/battle/move_effect/future_sight.c test/battle/move_effect/aqua_ring.c test/battle/move_effect/fixed_hp_damage.c test/battle/move_effect_secondary/freeze.c'
# Compilation is bounded to two jobs. Run serially to prove teardown ordering.
make -C engine BUILD=firered TESTS='' TEST_SRCS="$sources" pokefirered-test.elf -j2 > build/battle-effects/regression-build.log 2>&1
make -C engine BUILD=firered TESTS='' TEST_SRCS="$sources" check -j1 > build/battle-effects/regression-native.log 2>&1
python3 tests/effects/check_results.py build/battle-effects/regression-native.log
