"""Execute the battle context branch and use dispatcher with instrumented callbacks."""
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]


def function(source, signature):
    start = source.index(signature + "\n{")
    end = source.index("{", start) + 1
    depth = 1
    while depth:
        depth += (source[end] == "{") - (source[end] == "}")
        end += 1
    return source[start:end]


class MenuRejection(unittest.TestCase):
    def test_rejected_supply_explains_without_dispatching_an_action(self):
        source = (ROOT / "engine/src/item_menu.c").read_text()
        context = function(source, "static void OpenContextMenu(u8 taskId)")
        branch = context.split("case ITEMMENULOCATION_RAIDEND:", 1)[1].split("        break;", 1)[0]
        # The engine's text preprocessor encodes its page-break escape before C.
        use = function(source, "static void ItemMenu_UseInBattle(u8 taskId)").replace(r"\p", r"\n")
        fixture = r'''
#include <assert.h>
#include <string.h>
typedef unsigned char u8;
enum ItemType { ITEM_USE_BAG_MENU, ITEM_USE_BATTLER, ITEM_USE_PARTY_MENU, ITEM_USE_PARTY_MENU_MOVES };
#define ARRAY_COUNT(a) (sizeof(a)/sizeof((a)[0]))
#define COMPOUND_STRING(s) (s)
#define FONT_NORMAL 0
#define SC_SUPPLY_INVALID 0
static const int sContextMenuItems_BattleUse[]={1,0},sContextMenuItems_Cancel[]={0};
static struct {const int *contextMenuItemsPtr; int contextMenuNumItems;} menu,*gBagMenu=&menu;
static int applies=1,allowed,usable=1,category=1,actions,messages,closed,doubleBattle;
static enum ItemType type=ITEM_USE_PARTY_MENU;
static const char *lastMessage;
static u8 gBattlerInMenuId,gSpecialVar_ItemId;
static int GetItemBattleUsage(int item){return usable;}
static int ScSuppliesApplies(void){return applies;}
static int ScSuppliesCanUse(int battler,int item){return allowed;}
static int ScSuppliesCategory(int item){return category;}
static enum ItemType GetItemType(int item){return type;}
static int IsDoubleBattle(void){return doubleBattle;}
static void HandleErrorMessage(u8 task){}
static void RemoveContextWindow(void){closed++;}
static void DisplayItemMessage(u8 task,int font,const char *s,void (*cb)(u8)){messages++;lastMessage=s;assert(cb==HandleErrorMessage);}
static void ItemUseInBattle_BagMenu(u8 task){actions++;}
static void ItemUseInBattle_PartyMenu(u8 task){actions++;}
static void ItemUseInBattle_PartyMenuChooseMove(u8 task){actions++;}
'''
        cases = r'''
int main(void){
  OpenBattleContext();
  assert(menu.contextMenuItemsPtr==sContextMenuItems_BattleUse);
  ItemMenu_UseInBattle(0);
  assert(messages==1 && actions==0 && closed==1);
  assert(strstr(lastMessage,"prepared") && strstr(lastMessage,"CENTER"));
  category=SC_SUPPLY_INVALID;
  ItemMenu_UseInBattle(0);
  assert(messages==2 && actions==0 && strstr(lastMessage,"not allowed"));
  allowed=1;
  for(type=ITEM_USE_BAG_MENU;type<=ITEM_USE_PARTY_MENU_MOVES;type++) ItemMenu_UseInBattle(0);
  assert(actions==4 && messages==2);
  doubleBattle=1;type=ITEM_USE_BATTLER;ItemMenu_UseInBattle(0);assert(actions==5);
  applies=0;allowed=0;ItemMenu_UseInBattle(0);assert(actions==6 && messages==2);
  usable=0;OpenBattleContext();assert(menu.contextMenuItemsPtr==sContextMenuItems_Cancel);
  ItemMenu_UseInBattle(0);assert(actions==6 && messages==2);
}
'''
        with tempfile.TemporaryDirectory(prefix="sc-menu-rejection-") as directory:
            path = Path(directory)
            (path / "test.c").write_text(fixture + "\nstatic void OpenBattleContext(void){" + branch + "}\n" + use + cases)
            compiled = subprocess.run(["cc", "-std=c11", "-Werror", str(path / "test.c"), "-o", str(path / "test")], capture_output=True, text=True)
            self.assertEqual(compiled.returncode, 0, compiled.stderr)
            executed = subprocess.run([str(path / "test")], capture_output=True, text=True)
            self.assertEqual(executed.returncode, 0, executed.stderr)


if __name__ == "__main__":
    unittest.main()
