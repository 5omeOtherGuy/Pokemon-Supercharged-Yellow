#include <assert.h>
#include <string.h>
#include "sc_supplies_core.h"

int main(void)
{
    struct ScSupplyPlan plan = {{10, 20, 30}, {2, 1, 0}};
    unsigned categories[3] = {SC_SUPPLY_HP, SC_SUPPLY_STATUS, SC_SUPPLY_PP};
    assert(ScSupplyPlanValid(&plan, categories));
    plan.quantities[2] = 1; assert(!ScSupplyPlanValid(&plan, categories));
    plan.quantities[0] = 1; assert(ScSupplyPlanValid(&plan, categories));
    categories[2] = SC_SUPPLY_STATUS; assert(!ScSupplyPlanValid(&plan, categories));
    categories[2] = SC_SUPPLY_INVALID; assert(!ScSupplyPlanValid(&plan, categories));
    plan.quantities[2] = 0; assert(ScSupplyPlanValid(&plan, categories));
    plan.quantities[0] = 255; assert(!ScSupplyPlanValid(&plan, categories));
    plan = (struct ScSupplyPlan){{10,10,30},{1,1,1}};
    categories[0]=categories[1]=SC_SUPPLY_HP;categories[2]=SC_SUPPLY_BOOST;
    assert(ScSupplyPlanValid(&plan,categories));
    struct ScSupplyBattle battle = {0};
    ScSupplyLoad(&battle,0,&plan);
    assert(ScSupplyReserve(&battle,0,0,10));
    assert(ScSupplyReserve(&battle,0,0,10)); /* idempotent, not two units */
    assert(ScSupplyReserve(&battle,0,2,10));
    assert(!ScSupplyCanUse(&battle,0,1,10));
    assert(!ScSupplyCommit(&battle,0,0,30)); /* wrong action cannot consume */
    assert(ScSupplyCommit(&battle,0,0,10));
    assert(!ScSupplyCommit(&battle,0,0,10)); /* no duplicate commit */
    ScSupplyCancel(&battle,2);assert(ScSupplyCanUse(&battle,0,0,10));
    assert(ScSupplyReserve(&battle,0,0,10));
    assert(!ScSupplyReserve(&battle,0,0,99)); /* failed reselection preserves reservation */
    assert(ScSupplyCommit(&battle,0,0,10));
    assert(!ScSupplyCanUse(&battle,0,2,10));
    assert(ScSupplyReserve(&battle,0,2,30));assert(ScSupplyCommit(&battle,0,2,30));
    assert(!ScSupplyCanUse(&battle,0,0,30));
    assert(!ScSupplyCanUse(&battle,1,1,10)); /* NPC empty independent */
    ScSupplyLoad(&battle,1,&plan);assert(ScSupplyReserve(&battle,1,1,10));
    assert(ScSupplyReserve(&battle,1,3,10));assert(ScSupplyCommit(&battle,1,1,10));
    assert(ScSupplyCommit(&battle,1,3,10));assert(!ScSupplyCanUse(&battle,1,1,10));
    assert(!ScSupplyReserve(&battle,2,0,10));assert(!ScSupplyReserve(&battle,0,4,10));
    return 0;
}
