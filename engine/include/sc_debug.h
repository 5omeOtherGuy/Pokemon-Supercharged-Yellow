#ifndef GUARD_SC_DEBUG_H
#define GUARD_SC_DEBUG_H
#include "sc_debug_core.h"
#include "config/sc_campaign.h"

enum ScDebugOption
{
    SC_DEBUG_INFINITE_ITEMS, SC_DEBUG_IGNORE_LEVEL_CAP, SC_DEBUG_NO_ENCOUNTERS,
    SC_DEBUG_FREE_SUPPLIES, SC_DEBUG_AVOID_TRAINERS, SC_DEBUG_OPTION_COUNT,
};

bool32 ScDebugOptionEnabled(u32 option);
bool32 ScDebugSetOption(u32 option, bool32 enabled);
bool32 ScDebugValidMon(u32 party);
bool32 ScDebugEditMon(u32 party, u32 field, u32 slot, u32 value);
bool32 ScDebugNickname(u32 party, const u8 *name);
bool32 ScDebugGiveMon(u32 species, u32 level);
bool32 ScDebugGiveAll151(u32 level);
u32 ScDebugFreeSlots(void);
bool32 ScDebugGiveItem(u32 item, u32 quantity);
u32 ScDebugRefillPack(bool32 travel);
bool32 ScDebugSetBadges(u32 badges);
void ScDebugPrepareParty(void);
void ScDebugUnlockBuilds(void);
void ScDebugRecharge(void);
void ScDebugMaxMoney(void);
void ScDebugHeal(void);
void ScShowTestTools(void);
void ScDebugWarp(void);
void ScDebugPcLocation(void);
#endif
