"""Execute production helper bodies with controlled species fixtures."""
import unittest
from test_campaign_safety import HEADERS, execute, function

class CampaignHookTests(unittest.TestCase):
    def test_starter_identity_is_independent_of_rival_branch(self):
        source=HEADERS+'''#include "constants/species.h"
typedef unsigned short u16;
#define STARTER_MON_COUNT 3
const u16 sStarterMon[]={SPECIES_TREECKO,SPECIES_TORCHIC,SPECIES_MUDKIP};
'''+function('src/starter_choose.c','GetStarterPokemon')+'''
int main(void){for(unsigned i=0;i<65536;i++)assert(GetStarterPokemon(i)==SPECIES_PIKACHU);}
'''
        execute(source)
    def test_excluded_form_tables_are_unreachable(self):
        source=HEADERS+'''#include "constants/species.h"
struct FormChange {int method;};
struct SpeciesInfo {const struct FormChange *formChangeTable;};
const struct FormChange forms[]={{1},{0}};
const struct SpeciesInfo gSpeciesInfo[NUM_SPECIES]={[SPECIES_CHARIZARD]={forms}};
enum Species SanitizeSpeciesId(enum Species s){return s;}
'''+function('src/pokemon.c','GetSpeciesFormChanges')+'''
int main(void){assert(GetSpeciesFormChanges(SPECIES_CHARIZARD)==NULL);}
'''
        execute(source)

if __name__=='__main__':unittest.main()
