"""Release acceptance guard for the delegated four-badge Light Ball boundary.

Run explicitly while the coordinator integrates the species-data dependency.
The economy worker owns shop maps/prices, not wild held-item creation.
"""
from pathlib import Path
import re
ROOT=Path(__file__).resolve().parents[2]
text=(ROOT/'engine/src/data/pokemon/species_info/gen_1_families.h').read_text()
pikachu=re.search(r'\[SPECIES_PIKACHU\] =\n    \{(.*?)\n    \},',text,re.S).group(1)
assert not re.search(r'\.item(?:Common|Rare)\s*=\s*ITEM_LIGHT_BALL',pikachu), 'Viridian Forest wild Pikachu can bypass the four-badge Light Ball shop gate'
print('Pikachu wild-held metadata cannot bypass four-badge Light Ball shop access.')
