from pathlib import Path
import re
import subprocess
import tempfile
import unittest
ROOT=Path(__file__).resolve().parents[2]
class StatusPreviewTests(unittest.TestCase):
    def test_single_status_cure_does_not_advertise_volatile_healing(self):
        source=(ROOT/'engine/src/item_use.c').read_text()
        start=source.index('static bool32 IteamHealsMonVolatile(');opening=source.index('{',start);end=opening+1;depth=1
        while depth:depth+=(source[end]=='{')-(source[end]=='}');end+=1
        fixture='''#include <assert.h>
#include "constants/items.h"
#include "constants/item_effects.h"
typedef unsigned int bool32;typedef unsigned char u8;
#define FALSE 0
enum BattlerId {ACTOR};
struct {struct {unsigned infatuation,confusionTimer;} volatiles;} gBattleMons[1];
const u8 *GetItemEffect(enum Item item){static const u8 poison[6]={[3]=ITEM3_POISON},full[6]={[3]=ITEM3_STATUS_ALL};return item==ITEM_ANTIDOTE?poison:full;}
'''
        cases='''int main(void){gBattleMons[0].volatiles.infatuation=1;gBattleMons[0].volatiles.confusionTimer=3;assert(!IteamHealsMonVolatile(ACTOR,ITEM_ANTIDOTE));assert(IteamHealsMonVolatile(ACTOR,ITEM_FULL_HEAL));return 0;}'''
        with tempfile.TemporaryDirectory(prefix='sc-status-preview-') as d:
            p=Path(d);(p/'test.c').write_text(fixture+source[start:end]+cases)
            result=subprocess.run(['cc','-std=c11','-Werror','-I'+str(ROOT/'engine/include'),str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)
            result=subprocess.run([str(p/'test')],capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)
if __name__=='__main__':unittest.main()
