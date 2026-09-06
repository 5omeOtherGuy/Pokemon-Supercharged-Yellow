"""Evaluate the active roster data with the host C compiler, without building a ROM.

The preprocessor selects the game's real configuration branches. The probe then
compiles the actual learnset header and stat expressions, not a copied fixture.
Graphics and unrelated SpeciesInfo fields are deliberately outside this probe.
"""

from pathlib import Path
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
ENGINE = ROOT / "engine"
SPECIES = ENGINE / "src/data/pokemon/species_info/gen_1_families.h"
PREAMBLE = """
#define TRUE 1
#define FALSE 0
#include "config/general.h"
#include "config/pokemon.h"
#include "config/species_enabled.h"
"""
STATS = ("HP", "Attack", "Defense", "Speed", "SpAttack", "SpDefense")


def species_blocks(text):
    """Species initializers have a unique top-level closing indentation."""
    return dict(re.findall(r"\[SPECIES_(\w+)\]\s*=\s*\{(.*?)^    \},", text, re.S | re.M))


def evaluate():
    definitions = (ENGINE / "include/constants/species.h").read_text()
    names = [name for name, number in re.findall(r"SPECIES_(\w+) = (\d+),", definitions)
             if 1 <= int(number) <= 151]
    assert len(names) == 151
    processed = subprocess.check_output(
        ["cc", "-E", "-P", "-x", "c", "-I", str(ENGINE / "include"), "-"],
        input=PREAMBLE + f'\n#include "{SPECIES}"\n', text=True)
    blocks = species_blocks(processed)
    selector = re.search(r"#if P_LVL_UP_LEARNSETS.*?\n#endif", (ENGINE / "src/pokemon.c").read_text(), re.S).group()
    move_names = list(dict.fromkeys(re.findall(r"\bMOVE_[A-Z0-9_]+\b", (ENGINE / "include/constants/moves.h").read_text())))
    # Aliases may share numeric IDs; source move names are retained separately.
    code = PREAMBLE + '''
#include <stdio.h>
#include "constants/moves.h"
#define LEVEL_UP_MOVE_END 65535
struct LevelUpMove { unsigned short move; unsigned short level; };
''' + selector + '\nint main(void) {\n'
    for move in move_names:
        code += f'printf("M {move} %u\\n", (unsigned){move});\n'
    for name in names:
        block = blocks[name]
        learnset = re.search(r"\.levelUpLearnset = (\w+)", block).group(1)
        stats = [re.search(rf"\.base{stat}\s*=\s*(.*?),", block).group(1) for stat in STATS]
        code += f'printf("S {name} %u %u %u %u %u %u\\n", ' + ', '.join(f'(unsigned)({s})' for s in stats) + ');\n'
        code += f'for (unsigned i=0; i<sizeof({learnset})/sizeof({learnset}[0]); i++) printf("L {name} %u %u\\n", {learnset}[i].level, {learnset}[i].move);\n'
    code += 'return 0; }\n'
    with tempfile.TemporaryDirectory(prefix="psy-roster-") as temporary:
        source = Path(temporary) / "probe.c"
        executable = Path(temporary) / "probe"
        source.write_text(code)
        subprocess.run(["cc", "-std=c11", "-Werror", "-I", str(ENGINE / "include"),
                        "-I", str(ENGINE / "src"), str(source), "-o", str(executable)], check=True)
        output = subprocess.check_output([str(executable)], text=True)
    roster = {name: {"block": blocks[name], "learnset": []} for name in names}
    moves = {}
    for line in output.splitlines():
        kind, name, *values = line.split()
        if kind == "M":
            moves[name] = int(values[0])
        elif kind == "S":
            roster[name]["stats"] = list(map(int, values))
        else:
            roster[name]["learnset"].append(tuple(map(int, values)))
    return roster, moves


if __name__ == "__main__":
    import json
    roster, moves = evaluate()
    canonical = {number: name for name, number in reversed(list(moves.items()))}
    for row in roster.values():
        row["learnset"] = [(level, canonical.get(move, "END")) for level, move in row["learnset"]]
        del row["block"]
    print(json.dumps(roster, indent=2))
