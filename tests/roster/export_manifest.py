"""Regenerate the roster's source-derived balance manifest (requires Git history)."""

import json
import re
import subprocess

from roster_data import ROOT, evaluate

BASE = "a44f7a98a2e20432053827b712435a4da8851614"
SPECIES_PATH = "engine/src/data/pokemon/species_info/gen_1_families.h"
LEARNSET_PATH = "engine/src/data/pokemon/level_up_learnsets/gen_9.h"
OUTPUT = ROOT / "docs/balance/roster-manifest.json"


def historical(path):
    return subprocess.check_output(["git", "show", f"{BASE}:{path}"], cwd=ROOT, text=True)


def describe(row, move_names):
    block = row["block"]
    evolution = re.search(r"\.evolutions\s*=\s*(.*)", block, re.S)
    return {
        "stats_hp_atk_def_speed_spatk_spdef": row["stats"],
        "types_expression": re.search(r"\.types = (.*)", block)[1].rstrip(","),
        "abilities": re.findall(r"ABILITY_\w+", re.search(r"\.abilities = (.*)", block)[1]),
        "evolution_expression": " ".join(evolution[1].split()).rstrip(",") if evolution else None,
        "level_up_moves": [[level, move_names[move]] for level, move in row["learnset"] if move != 65535],
    }


def manifest():
    current, moves = evaluate()
    baseline, _ = evaluate(historical(SPECIES_PATH), historical(LEARNSET_PATH))
    canonical = {number: name for name, number in reversed(list(moves.items()))}
    roles = json.loads((ROOT / "docs/balance/roles.json").read_text())
    assert current.keys() == roles.keys()
    return {
        "baseline_revision": BASE,
        "baseline_configuration": "GEN9 data evaluated under the active modern Pokemon configuration",
        "sources": [SPECIES_PATH, LEARNSET_PATH],
        "validation_limit": "Compiled data evaluation only; encounter access, battle effects and human balance are separate checks.",
        "species": {name: {"role": roles[name], "baseline": describe(baseline[name], canonical),
                           "curated": describe(row, canonical)} for name, row in current.items()},
    }


if __name__ == "__main__":
    OUTPUT.write_text(json.dumps(manifest(), indent=2) + "\n")
    print(OUTPUT.relative_to(ROOT))
