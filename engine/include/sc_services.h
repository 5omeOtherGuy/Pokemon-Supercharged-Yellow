#ifndef GUARD_SC_SERVICES_H
#define GUARD_SC_SERVICES_H

// The caller closes its party-menu resources after TRUE; this screen takes
// over on the next main callback. Return callback must rebuild the party menu.
bool32 ScShowFieldTraining(u32 partyIndex, void (*returnCallback)(void));
// Script special with explicit waitstate; returns through the normal field
// reconstruction callback and resumes the script, even for an invalid context.
void ScShowCenterServices(void);

enum ScServiceResult
{
    SC_SERVICE_OK, SC_SERVICE_BAD_CONTEXT, SC_SERVICE_BAD_MON,
    SC_SERVICE_INVALID, SC_SERVICE_NOT_EARNED, SC_SERVICE_OVER_BUDGET,
    SC_SERVICE_NO_CHANGE, SC_SERVICE_INSUFFICIENT_PRACTICE, SC_SERVICE_MAX_IV,
};

#define SC_SERVICE_MAX_MOVES 64
bool32 ScServicesAtCenter(void);
bool32 ScServicesValidMon(u32 partyIndex);
u32 ScServicesSetCapabilities(u32 partyIndex, u32 mask);
u32 ScServicesSetTrainerPassives(u32 mask);
u32 ScServicesSetFocus(u32 partyIndex, u32 focus);
u32 ScServicesSetAbility(u32 partyIndex, u32 abilitySlot);
u32 ScServicesImproveIv(u32 partyIndex, u32 stat);
u32 ScServicesIvCost(u32 level);
u32 ScServicesRelearnMoves(u32 partyIndex, u16 *moves, u32 capacity);
u32 ScServicesTeachMove(u32 partyIndex, u32 move, u32 moveSlot);

#endif
