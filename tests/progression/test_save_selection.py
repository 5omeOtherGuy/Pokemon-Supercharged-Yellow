"""Execute the actual save-slot selector against controlled flash-sector reads.

This tests selection logic on the host; it does not replace cartridge save/load.
"""
from pathlib import Path
import re
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
ENGINE = ROOT / 'engine'

def source_function(text, name):
    match = re.search(r'^(?:static )?[^\n;]+\b' + name + r'\([^;]*?\)\n\{', text, re.M)
    opening = text.index('{', match.start())
    depth, end = 1, opening + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[match.start():end]

class SaveSelectionTests(unittest.TestCase):
    def test_newer_damage_and_torn_writes_recover_older_complete_slot(self):
        header = (ENGINE / 'include/save.h').read_text().split('extern u16 gLastWrittenSector;')[0]
        header = header.replace('#include "main.h"', '') + '\n#endif\n'
        save = (ENGINE / 'src/save.c').read_text()
        code = '''#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "sc_progression_core.h"
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32;
typedef int32_t s32; typedef uint8_t bool8; typedef uint32_t bool32;
#define TRUE 1
#define FALSE 0
#define IS_FRLG 1
''' + header + '''
static struct SaveSector flash[28], buffer;
static struct SaveSector *gReadWriteSector = &buffer;
static u32 gSaveCounter;
static u16 gLastWrittenSector;
static void ReadFlashSector(u32 id, struct SaveSector *dst){assert(id<28); *dst=flash[id];}
''' + '\n'.join(source_function(save, n) for n in ['CalculateChecksum', 'ScIsValidSaveSector', 'GetSaveValidStatus']) + '''
static void fillSlot(unsigned slot, unsigned counter){
    struct ScTrainerProgress progress; ScInitTrainerProgress(&progress);
    for(unsigned i=0;i<14;i++){
        struct SaveSector *s=&flash[slot*14+i]; memset(s,0,sizeof(*s));
        s->id=i; s->counter=counter; s->signature=SECTOR_SIGNATURE;
        if(i==0)memcpy(s->saveBlock3Chunk,&progress,sizeof(progress));
    }
}
int main(void){
    struct SaveSectorLocation locations[14]={0};
    assert(GetSaveValidStatus(locations)==SAVE_STATUS_EMPTY);
    fillSlot(0,2); fillSlot(1,3);
    assert(GetSaveValidStatus(locations)==SAVE_STATUS_OK && gSaveCounter==3);
    flash[14].saveBlock3Chunk[16]^=1;
    assert(GetSaveValidStatus(locations)==SAVE_STATUS_ERROR && gSaveCounter==2);
    flash[0].saveBlock3Chunk[16]^=1;
    assert(GetSaveValidStatus(locations)==SAVE_STATUS_CORRUPT);
    fillSlot(0,2); fillSlot(1,3); flash[14].saveBlock3Chunk[4]=99;
    assert(GetSaveValidStatus(locations)==SAVE_STATUS_ERROR && gSaveCounter==2);
    flash[0].saveBlock3Chunk[4]=99;
    assert(GetSaveValidStatus(locations)==SAVE_STATUS_INCOMPATIBLE);
    fillSlot(0,2); fillSlot(1,3); flash[15].id=65535;
    assert(GetSaveValidStatus(locations)==SAVE_STATUS_ERROR && gSaveCounter==2);
    fillSlot(0,2); fillSlot(1,3); flash[15].counter=1; // valid data, torn transaction
    assert(GetSaveValidStatus(locations)==SAVE_STATUS_ERROR && gSaveCounter==2);
    fillSlot(0,0); fillSlot(1,UINT32_MAX);
    assert(GetSaveValidStatus(locations)==SAVE_STATUS_OK && gSaveCounter==0);
}
'''
        with tempfile.TemporaryDirectory(prefix='sc-save-selection-') as d:
            path = Path(d)
            (path/'selection.c').write_text(code)
            subprocess.run(['cc','-std=c11','-Wall','-Wextra','-Werror','-Wno-sign-compare',
                '-iquote',str(ENGINE/'include'),str(path/'selection.c'),
                str(ENGINE/'src/sc_progression_core.c'),'-o',str(path/'selection')],check=True)
            subprocess.run([str(path/'selection')],check=True)

if __name__ == '__main__':
    unittest.main()
