#ifdef SC_AI_HOST_TEST
#include <string.h>
#else
#include "global.h"
#include "battle.h"
#endif
#include "sc_ai.h"

unsigned int ScAiEstimateStat(unsigned int base, unsigned int level, unsigned int naturePercent, unsigned int hp)
{
    /* Explicit prior: median IV 16, zero training. Nature is public. */
    unsigned int value = (2 * base + 16) * level / 100;
    return hp ? value + level + 10 : (value + 5) * naturePercent / 100;
}

unsigned int ScAiEstimateHp(unsigned int estimatedMax, unsigned int visiblePixels, unsigned int fullPixels)
{
    if (!fullPixels || !visiblePixels) return 0;
    return (estimatedMax * visiblePixels + fullPixels - 1) / fullPixels;
}

static int ScAiPairScore(const struct ScAiObservation *o, const struct ScAiOption *a, const struct ScAiOption *b)
{
    if (b && a->kind != SC_AI_MOVE && a->kind == b->kind
        && a->resource == b->resource && a->index == b->index)
        return -30000; /* One shared party slot or bag unit cannot be used twice. */
    int score = a->utility + (b ? b->utility : 0);
    if (b && (a->effectMask & b->effectMask)) score -= 80;
    for (unsigned int i = 0; i < SC_AI_BATTLERS; ++i)
    {
        if (!(o->aliveMask & (1u << i)) || !o->hp[i]) continue;
        unsigned int damage = a->damage[i] + (b ? b->damage[i] : 0);
        if ((a->protects && i == o->actors[0]) || (b && b->protects && i == o->actors[1])) damage = 0;
        unsigned int fraction = (damage > o->hp[i] ? o->hp[i] : damage) * 100 / o->hp[i];
        if (o->ownMask & (1u << i))
            score -= fraction * 2 + (damage >= o->hp[i] ? 120 : 0);
        else
            score += fraction + (damage >= o->hp[i] ? 80 : 0);
    }
    return score;
}

void ScAiChoose(const struct ScAiObservation *o, struct ScAiChoice choices[SC_AI_ACTORS])
{
    memset(choices, 0, sizeof(*choices) * SC_AI_ACTORS);
    if (!o->actorCount || !o->count[0]) return;
    unsigned int countA = o->count[0] > SC_AI_OPTIONS ? SC_AI_OPTIONS : o->count[0];
    unsigned int countB = o->actorCount > 1 ? o->count[1] : 0;
    if (countB > SC_AI_OPTIONS) countB = SC_AI_OPTIONS;
    int best = -32000;
    for (unsigned int i = 0; i < countA; ++i)
    {
        for (unsigned int j = 0; j < (countB ? countB : 1); ++j)
        {
            const struct ScAiOption *a = &o->options[0][i];
            const struct ScAiOption *b = countB ? &o->options[1][j] : 0;
            int score = ScAiPairScore(o, a, b);
            if (score <= best) continue; /* Stable tie order; never battle RNG. */
            best = score;
            choices[0] = (struct ScAiChoice){a->kind, a->index, a->target, i};
            if (b) choices[1] = (struct ScAiChoice){b->kind, b->index, b->target, j};
        }
    }
}

#ifndef SC_AI_HOST_TEST
#include "battle_ai_main.h"
#include "battle_ai_util.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_util.h"
#include "data.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "sc_battle.h"
#include "constants/abilities.h"
#include "constants/hold_effects.h"
#include "constants/item_effects.h"
#include "constants/items.h"
#include "constants/moves.h"

/* Only this adapter reads game data. It never invokes legacy damage simulation. */
struct ScAiMonView
{
    u16 stats[NUM_STATS];
    u16 hp, maxHp;
    enum Species species;
    enum Move moves[MAX_MON_MOVES];
    enum Ability ability;
    enum HoldEffect hold;
    enum Type types[3];
    u32 status;
    u8 level, side, available, grounded, own, healBlocked, protectUses;
    s8 stages[NUM_BATTLE_STATS];
    struct ScPublicEffects effects;
};
static EWRAM_DATA struct ScAiMonView sScMons[SC_AI_BATTLERS] = {0};
static EWRAM_DATA struct ScAiObservation sScObservation = {0};
static EWRAM_DATA struct ScAiChoice sScChoices[SC_AI_BATTLERS] = {0};
static EWRAM_DATA bool8 sScTestEnabled = FALSE;

bool32 ScAiEnabled(void)
{
    if (TESTING && !sScTestEnabled) return FALSE;
    return (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK | BATTLE_TYPE_FRONTIER
                                | BATTLE_TYPE_PALACE | BATTLE_TYPE_INGAME_PARTNER));
}

void ScAiEnableForTests(bool32 enabled)
{
    if (TESTING) sScTestEnabled = enabled;
}

static u32 ScStage(u32 stat, s32 stage)
{
    stage = max(0, min(12, stage));
    if (stage >= 6) return max(1, stat * (2 + stage - 6) / 2);
    return max(1, stat * 2 / (2 + 6 - stage));
}

static bool32 ScHasType(const struct ScAiMonView *mon, enum Type type)
{
    return mon->types[0] == type || mon->types[1] == type || mon->types[2] == type;
}

static u32 ScPublicStatus(u32 status)
{
    /* Sleep countdown and toxic counter are not part of the visible icon. */
    return (status & (STATUS1_POISON | STATUS1_BURN | STATUS1_FREEZE | STATUS1_PARALYSIS | STATUS1_TOXIC_POISON))
        | ((status & STATUS1_SLEEP) ? STATUS1_SLEEP : 0);
}

static void ScReadActive(u32 battler, struct ScAiMonView *out)
{
    const struct BattlePokemon *mon = &gBattleMons[battler];
    memset(out, 0, sizeof(*out));
    out->side = GetBattlerSide(battler);
    out->own = out->side == B_SIDE_OPPONENT;
    out->species = mon->species;
    out->level = mon->level;
    out->available = IsBattlerAlive(battler);
    if (!out->available) return;
    out->ability = mon->volatiles.gastroAcid ? ABILITY_NONE : mon->ability;
    out->hold = (mon->volatiles.embargoTimer || (gFieldStatuses & STATUS_FIELD_MAGIC_ROOM))
        ? HOLD_EFFECT_NONE : GetItemHoldEffect(mon->item);
    out->status = ScPublicStatus(mon->status1);
    for (u32 i = 0; i < 3; ++i) out->types[i] = mon->types[i];
    for (u32 i = 0; i < MAX_MON_MOVES; ++i) out->moves[i] = mon->moves[i];
    for (u32 i = 0; i < NUM_BATTLE_STATS; ++i) out->stages[i] = mon->statStages[i];
    out->grounded = !ScHasType(out, TYPE_FLYING) && out->ability != ABILITY_LEVITATE && out->hold != HOLD_EFFECT_AIR_BALLOON;
    if (gFieldStatuses & STATUS_FIELD_GRAVITY) out->grounded = TRUE;
    ScGetPublicBattlerEffects(battler, &out->effects);
    if (out->own)
    {
        out->stats[STAT_ATK] = mon->attack;
        out->stats[STAT_DEF] = mon->defense;
        out->stats[STAT_SPEED] = mon->speed;
        out->stats[STAT_SPATK] = mon->spAttack;
        out->stats[STAT_SPDEF] = mon->spDefense;
        out->maxHp = mon->maxHP;
        out->hp = mon->hp;
        out->healBlocked = !!mon->volatiles.healBlockTimer;
        out->protectUses = mon->volatiles.consecutiveMoveUses;
    }
    else
    {
        u32 nature = GetNature(GetBattlerMon(battler)); /* Explicit public nature field. */
        for (u32 i = STAT_ATK; i < NUM_STATS; ++i)
            out->stats[i] = ModifyStatByNature(nature, ScAiEstimateStat(GetSpeciesBaseStat(out->species, i), out->level, 100, FALSE), i);
        out->maxHp = ScAiEstimateStat(GetSpeciesBaseHP(out->species), out->level, 100, TRUE);
        if (out->species == SPECIES_SHEDINJA) out->maxHp = 1;
        /* The only exact opponent HP access projects the existing public 48-pixel bar. */
        out->hp = ScAiEstimateHp(out->maxHp, GetScaledHPFraction(mon->hp, mon->maxHP, 48), 48);
    }
    out->stats[STAT_HP] = out->maxHp;
}

static u32 ScSpeed(const struct ScAiMonView *mon)
{
    u32 speed = ScStage(mon->stats[STAT_SPEED], mon->stages[STAT_SPEED]);
    speed = speed * mon->effects.speedPercent / 100;
    if (mon->status & STATUS1_PARALYSIS) speed /= 2;
    if (mon->hold == HOLD_EFFECT_CHOICE_SCARF) speed = speed * 3 / 2;
    if (gSideStatuses[mon->side] & SIDE_STATUS_TAILWIND) speed *= 2;
    if ((gBattleWeather & B_WEATHER_RAIN) && mon->ability == ABILITY_SWIFT_SWIM) speed *= 2;
    if ((gBattleWeather & B_WEATHER_SUN) && mon->ability == ABILITY_CHLOROPHYLL) speed *= 2;
    if ((gBattleWeather & B_WEATHER_SANDSTORM) && mon->ability == ABILITY_SAND_RUSH) speed *= 2;
    return max(1, speed);
}

static bool32 ScBypassesAbility(const struct ScAiMonView *atk)
{
    return atk->ability == ABILITY_MOLD_BREAKER || atk->ability == ABILITY_TERAVOLT || atk->ability == ABILITY_TURBOBLAZE;
}

static u32 ScTypeMultiplier(const struct ScAiMonView *atk, const struct ScAiMonView *def, enum Type type)
{
    u32 multiplier = 4096;
    for (u32 i = 0; i < 3; ++i)
    {
        if (def->types[i] == TYPE_NONE || (i && def->types[i] == def->types[0]) || (i == 2 && def->types[i] == def->types[1])) continue;
        multiplier = multiplier * GetTypeModifier(type, def->types[i]) / 4096;
    }
    if (!ScBypassesAbility(atk))
    {
        enum Ability a = def->ability;
        if ((type == TYPE_GROUND && a == ABILITY_LEVITATE && !def->grounded)
         || (type == TYPE_WATER && (a == ABILITY_WATER_ABSORB || a == ABILITY_STORM_DRAIN || a == ABILITY_DRY_SKIN))
         || (type == TYPE_ELECTRIC && (a == ABILITY_VOLT_ABSORB || a == ABILITY_LIGHTNING_ROD || a == ABILITY_MOTOR_DRIVE))
         || (type == TYPE_FIRE && a == ABILITY_FLASH_FIRE)
         || (type == TYPE_GRASS && a == ABILITY_SAP_SIPPER)
         || (a == ABILITY_WONDER_GUARD && multiplier <= 4096)) return 0;
    }
    if (type == TYPE_GROUND && def->hold == HOLD_EFFECT_AIR_BALLOON) return 0;
    return multiplier;
}

static u32 ScDamage(const struct ScAiMonView *atk, const struct ScAiMonView *def, enum Move move, bool32 spread)
{
    if (!atk->available || !def->available || move == MOVE_NONE || GetMoveCategory(move) == DAMAGE_CATEGORY_STATUS) return 0;
    enum Type type = GetMoveType(move);
    enum BattleMoveEffects effect = GetMoveEffect(move);
    u32 power = GetMovePower(move);
    bool32 physical = GetMoveCategory(move) == DAMAGE_CATEGORY_PHYSICAL;
    if (effect == EFFECT_WEATHER_BALL && gBattleWeather)
    {
        power *= 2;
        if (gBattleWeather & B_WEATHER_RAIN) type = TYPE_WATER;
        else if (gBattleWeather & B_WEATHER_SUN) type = TYPE_FIRE;
        else if (gBattleWeather & B_WEATHER_SANDSTORM) type = TYPE_ROCK;
        else if (gBattleWeather & B_WEATHER_ICY_ANY) type = TYPE_ICE;
    }
    u32 multiplier = ScTypeMultiplier(atk, def, type);
    if (!multiplier) return 0;
    if (move == MOVE_SEISMIC_TOSS || move == MOVE_NIGHT_SHADE) return atk->level;
    if (move == MOVE_DRAGON_RAGE) return 40;
    if (move == MOVE_SONIC_BOOM) return 20;
    if (effect == EFFECT_FIXED_PERCENT_DAMAGE) return def->hp * GetMoveDamagePercentage(move) / 100;
    if (effect == EFFECT_FACADE && atk->status) power *= 2;
    if (effect == EFFECT_STORED_POWER)
        for (u32 i = STAT_ATK; i < NUM_BATTLE_STATS; ++i)
            if (atk->stages[i] > 6) power += 20 * (atk->stages[i] - 6);
    if (!power) power = 60; /* Explicit fallback estimate for unmodeled variable-power moves. */
    if (move == MOVE_HIDDEN_POWER) { power = 60; type = TYPE_NORMAL; } /* Never inspect IV-derived type. */
    u32 attack = ScStage(atk->stats[physical ? STAT_ATK : STAT_SPATK], atk->stages[physical ? STAT_ATK : STAT_SPATK]);
    u32 defense = ScStage(def->stats[physical ? STAT_DEF : STAT_SPDEF], def->stages[physical ? STAT_DEF : STAT_SPDEF]);
    if (physical && (atk->ability == ABILITY_HUGE_POWER || atk->ability == ABILITY_PURE_POWER)) attack *= 2;
    if (physical && atk->ability == ABILITY_GUTS && atk->status) attack = attack * 3 / 2;
    if (physical && atk->hold == HOLD_EFFECT_CHOICE_BAND) attack = attack * 3 / 2;
    if (!physical && atk->hold == HOLD_EFFECT_CHOICE_SPECS) attack = attack * 3 / 2;
    if (!physical && def->hold == HOLD_EFFECT_ASSAULT_VEST) defense = defense * 3 / 2;
    if ((gBattleWeather & B_WEATHER_SANDSTORM) && !physical && ScHasType(def, TYPE_ROCK)) defense = defense * 3 / 2;
    if ((gBattleWeather & B_WEATHER_SNOW) && physical && ScHasType(def, TYPE_ICE)) defense = defense * 3 / 2;
    u32 damage = ((2 * atk->level / 5 + 2) * power * attack / max(1, defense)) / 50 + 2;
    if (ScHasType(atk, type)) damage = damage * (atk->ability == ABILITY_ADAPTABILITY ? 2 : 3) / (atk->ability == ABILITY_ADAPTABILITY ? 1 : 2);
    damage = damage * multiplier / 4096;
    if (physical && (atk->status & STATUS1_BURN) && atk->ability != ABILITY_GUTS) damage /= 2;
    if (atk->ability == ABILITY_TECHNICIAN && power <= 60) damage = damage * 3 / 2;
    if (atk->hp * 3 <= atk->maxHp && ((type == TYPE_FIRE && atk->ability == ABILITY_BLAZE)
      || (type == TYPE_WATER && atk->ability == ABILITY_TORRENT) || (type == TYPE_GRASS && atk->ability == ABILITY_OVERGROW))) damage = damage * 3 / 2;
    if (!ScBypassesAbility(atk) && def->ability == ABILITY_THICK_FAT && (type == TYPE_ICE || type == TYPE_FIRE)) damage /= 2;
    if (!ScBypassesAbility(atk) && (def->ability == ABILITY_MULTISCALE || def->ability == ABILITY_SHADOW_SHIELD) && def->hp == def->maxHp) damage /= 2;
    if (atk->hold == HOLD_EFFECT_LIFE_ORB) damage = damage * 13 / 10;
    if (atk->hold == HOLD_EFFECT_EXPERT_BELT && multiplier > 4096) damage = damage * 6 / 5;
    if ((gBattleWeather & B_WEATHER_RAIN) && type == TYPE_WATER) damage = damage * 3 / 2;
    if ((gBattleWeather & B_WEATHER_RAIN) && type == TYPE_FIRE) damage /= 2;
    if ((gBattleWeather & B_WEATHER_SUN) && type == TYPE_FIRE) damage = damage * 3 / 2;
    if ((gBattleWeather & B_WEATHER_SUN) && type == TYPE_WATER) damage /= 2;
    if (atk->grounded && ((gFieldTimers.terrain == B_TERRAIN_ELECTRIC && type == TYPE_ELECTRIC)
      || (gFieldTimers.terrain == B_TERRAIN_GRASSY && type == TYPE_GRASS)
      || (gFieldTimers.terrain == B_TERRAIN_PSYCHIC && type == TYPE_PSYCHIC))) damage = damage * 13 / 10;
    if (def->grounded && gFieldTimers.terrain == B_TERRAIN_MISTY && type == TYPE_DRAGON) damage /= 2;
    if (def->grounded && gFieldTimers.terrain == B_TERRAIN_PSYCHIC && GetMovePriority(move) > 0 && atk->side != def->side) return 0;
    if (gSideStatuses[def->side] & (physical ? SIDE_STATUS_REFLECT : SIDE_STATUS_LIGHTSCREEN)) damage = IsDoubleBattle() ? damage * 2 / 3 : damage / 2;
    damage = damage * (physical ? atk->effects.offensivePhysicalPercent : atk->effects.offensiveSpecialPercent) / 100;
    damage = damage * (physical ? def->effects.defensivePhysicalPercent : def->effects.defensiveSpecialPercent) / 100;
    if (spread && IsDoubleBattle()) damage = damage * 3 / 4;
    u32 accuracy = GetMoveAccuracy(move);
    if (!accuracy || atk->ability == ABILITY_NO_GUARD || def->ability == ABILITY_NO_GUARD) accuracy = 100;
    else accuracy = min(100, accuracy * ScStage(100, atk->stages[STAT_ACC]) / ScStage(100, def->stages[STAT_EVASION]));
    damage = damage * accuracy / 100;
    /* Expected median damage; no future critical hit, accuracy or damage roll is read. */
    return min(65535, max(1, damage * 93 / 100));
}

static u32 ScThreat(const struct ScAiMonView *def)
{
    u32 total = 0;
    for (u32 foe = 0; foe < gBattlersCount; ++foe)
    {
        const struct ScAiMonView *atk = &sScMons[foe];
        if (!atk->available || atk->side == def->side) continue;
        u32 best = 0;
        for (u32 m = 0; m < MAX_MON_MOVES; ++m) best = max(best, ScDamage(atk, def, atk->moves[m], FALSE));
        total += best;
    }
    return total;
}

static s32 ScHealingUtility(const struct ScAiMonView *mon, u32 heal, u32 threat)
{
    if (mon->healBlocked || !mon->hp || mon->hp >= mon->maxHp) return -100;
    heal = min(heal, mon->maxHp - mon->hp);
    if (!heal || mon->hp + heal <= threat) return -80;
    s32 score = heal * 110 / mon->maxHp;
    if (mon->hp <= threat) score += 55;
    if (heal <= threat && mon->hp > threat) score -= 35;
    return score;
}

static bool32 ScStatusAllowed(const struct ScAiMonView *atk, const struct ScAiMonView *def, enum Move move)
{
    if (def->status || (gSideStatuses[def->side] & SIDE_STATUS_SAFEGUARD)) return FALSE;
    if (def->grounded && gFieldTimers.terrain == B_TERRAIN_MISTY) return FALSE;
    enum MoveEffect status = GetMoveNonVolatileStatus(move);
    enum Ability ability = ScBypassesAbility(atk) ? ABILITY_NONE : def->ability;
    switch (status)
    {
    case MOVE_EFFECT_SLEEP:
        return !(def->effects.statusImmunity & STATUS1_SLEEP) && ability != ABILITY_INSOMNIA && ability != ABILITY_VITAL_SPIRIT
            && !(def->grounded && gFieldTimers.terrain == B_TERRAIN_ELECTRIC);
    case MOVE_EFFECT_POISON:
    case MOVE_EFFECT_TOXIC:
        return !(def->effects.statusImmunity & STATUS1_PSN_ANY) && ability != ABILITY_IMMUNITY
            && ((!ScHasType(def, TYPE_POISON) && !ScHasType(def, TYPE_STEEL)) || atk->ability == ABILITY_CORROSION);
    case MOVE_EFFECT_BURN:
        return !(def->effects.statusImmunity & STATUS1_BURN) && ability != ABILITY_WATER_VEIL && !ScHasType(def, TYPE_FIRE);
    case MOVE_EFFECT_PARALYSIS:
        return !(def->effects.statusImmunity & STATUS1_PARALYSIS) && ability != ABILITY_LIMBER && !ScHasType(def, TYPE_ELECTRIC)
            && ScTypeMultiplier(atk, def, GetMoveType(move));
    default:
        return FALSE;
    }
}

static s32 ScStatusUtility(u32 battler, u32 target, enum Move move, struct ScAiOption *option)
{
    const struct ScAiMonView *atk = &sScMons[battler], *def = &sScMons[target];
    enum BattleMoveEffects effect = GetMoveEffect(move);
    u32 threat = ScThreat(atk);
    u32 ownSide = atk->side;
    switch (effect)
    {
    case EFFECT_RESTORE_HP:
    case EFFECT_REST:
    case EFFECT_LIFE_DEW:
        if (effect == EFFECT_REST && (atk->status & STATUS1_SLEEP)) return -100;
        return ScHealingUtility(atk, effect == EFFECT_REST ? atk->maxHp : atk->maxHp / 2, threat);
    case EFFECT_HEAL_PULSE:
        return target == battler || def->side != ownSide ? -100 : ScHealingUtility(def, def->maxHp / 2, ScThreat(def));
    case EFFECT_PROTECT:
        option->protects = TRUE;
        if (atk->protectUses) return -75;
        /* Protection alone must not create an endless stall loop. */
        return (atk->effects.endTurnHealFraction || atk->hold == HOLD_EFFECT_LEFTOVERS || atk->status & STATUS1_PSN_ANY)
            ? min(50, threat * 65 / max(1, atk->hp)) : -15;
    case EFFECT_NON_VOLATILE_STATUS:
    case EFFECT_YAWN:
        if (def->side == ownSide || !ScStatusAllowed(atk, def, move)) return -100;
        option->effectMask = 1u << target;
        if (GetMoveNonVolatileStatus(move) == MOVE_EFFECT_SLEEP) return 65;
        if (GetMoveNonVolatileStatus(move) == MOVE_EFFECT_BURN)
            return def->stats[STAT_ATK] > def->stats[STAT_SPATK] ? 60 : 30;
        if (GetMoveNonVolatileStatus(move) == MOVE_EFFECT_PARALYSIS) return ScSpeed(def) > ScSpeed(atk) ? 55 : 35;
        return def->hp * 2 > def->maxHp ? 45 : 15;
    case EFFECT_STAT_CHANGE:
    case EFFECT_STAT_CHANGE_HALF_HP:
    {
        if (threat >= atk->hp || (effect == EFFECT_STAT_CHANGE_HALF_HP && threat * 2 >= atk->hp)) return -60;
        s32 benefit = 0;
        const struct MoveInfo *info = &gMovesInfo[move];
        for (u32 i = 0; i < info->numAdditionalEffects; ++i)
        {
            const struct AdditionalEffect *extra = &info->additionalEffects[i];
            if (extra->moveEffect == STAT_CHANGE_EFFECT_MINUS)
            {
                if (target == battler) benefit -= (extra->defense + extra->spDef) * 12;
                else if (def->side != ownSide && def->ability != ABILITY_CLEAR_BODY && def->ability != ABILITY_WHITE_SMOKE)
                {
                    if (extra->attack && def->stages[STAT_ATK] > 2 && def->stats[STAT_ATK] >= def->stats[STAT_SPATK]) benefit += 30;
                    if (extra->spAtk && def->stages[STAT_SPATK] > 2 && def->stats[STAT_SPATK] >= def->stats[STAT_ATK]) benefit += 30;
                    if (extra->speed && def->stages[STAT_SPEED] > 2 && ScSpeed(def) > ScSpeed(atk)) benefit += 25;
                    if (extra->defense && def->stages[STAT_DEF] > 2) benefit += 20;
                    if (extra->spDef && def->stages[STAT_SPDEF] > 2) benefit += 20;
                }
                continue;
            }
            if (target != battler && !extra->self) continue;
            if (extra->attack && atk->stages[STAT_ATK] < 10 && atk->stats[STAT_ATK] >= atk->stats[STAT_SPATK]) benefit += 35;
            if (extra->spAtk && atk->stages[STAT_SPATK] < 10 && atk->stats[STAT_SPATK] >= atk->stats[STAT_ATK]) benefit += 35;
            if (extra->speed && atk->stages[STAT_SPEED] < 10) benefit += 20;
            if (extra->defense && atk->stages[STAT_DEF] < 10) benefit += 15;
            if (extra->spDef && atk->stages[STAT_SPDEF] < 10) benefit += 15;
        }
        return benefit ? benefit - threat * 40 / max(1, atk->hp) : -30;
    }
    case EFFECT_LIGHT_SCREEN:
    case EFFECT_REFLECT:
        option->effectMask = effect == EFFECT_REFLECT ? 1u << 4 : 1u << 5;
        if (gSideStatuses[ownSide] & (effect == EFFECT_REFLECT ? SIDE_STATUS_REFLECT : SIDE_STATUS_LIGHTSCREEN)) return -100;
        return threat < atk->hp ? (IsDoubleBattle() ? 60 : 35) : -10;
    case EFFECT_TAILWIND:
        option->effectMask = 1u << 6;
        if (gSideStatuses[ownSide] & SIDE_STATUS_TAILWIND) return -100;
        for (u32 foe = 0; foe < gBattlersCount; ++foe)
            if (sScMons[foe].available && sScMons[foe].side != ownSide && ScSpeed(&sScMons[foe]) >= ScSpeed(atk)) return IsDoubleBattle() ? 70 : 40;
        return -20;
    case EFFECT_TRICK_ROOM:
        option->effectMask = 1u << 7;
        {
            s32 benefit = 0;
            for (u32 foe = 0; foe < gBattlersCount; ++foe)
                if (sScMons[foe].available && sScMons[foe].side != ownSide) benefit += ScSpeed(atk) < ScSpeed(&sScMons[foe]) ? 35 : -35;
            return (gFieldStatuses & STATUS_FIELD_TRICK_ROOM) ? -benefit : benefit;
        }
    case EFFECT_WEATHER:
    case EFFECT_WEATHER_AND_SWITCH:
    {
        option->effectMask = 1u << 8;
        u32 weather = GetMoveWeatherType(move);
        if (gBattleWeather & (1u << weather)) return -100;
        s32 benefit = 0;
        for (u32 b = 0; b < gBattlersCount; ++b)
        {
            if (!sScMons[b].available) continue;
            s32 value = 0;
            for (u32 m = 0; m < MAX_MON_MOVES; ++m)
            {
                enum Type type = GetMoveType(sScMons[b].moves[m]);
                if (weather == BATTLE_WEATHER_RAIN) value += type == TYPE_WATER ? 20 : type == TYPE_FIRE ? -15 : 0;
                if (weather == BATTLE_WEATHER_SUN) value += type == TYPE_FIRE ? 20 : type == TYPE_WATER ? -15 : 0;
            }
            if (weather == BATTLE_WEATHER_RAIN && sScMons[b].ability == ABILITY_SWIFT_SWIM) value += 40;
            if (weather == BATTLE_WEATHER_SUN && sScMons[b].ability == ABILITY_CHLOROPHYLL) value += 40;
            benefit += sScMons[b].side == ownSide ? value : -value;
        }
        return benefit;
    }
    case EFFECT_TERRAIN:
        option->effectMask = 1u << 9;
        if (gFieldTimers.terrain == GetMoveTerrainType(move)) return -100;
        return threat < atk->hp && ((GetMoveTerrainType(move) == B_TERRAIN_ELECTRIC && ScHasType(atk, TYPE_ELECTRIC))
           || (GetMoveTerrainType(move) == B_TERRAIN_GRASSY && ScHasType(atk, TYPE_GRASS))
           || (GetMoveTerrainType(move) == B_TERRAIN_PSYCHIC && ScHasType(atk, TYPE_PSYCHIC))) ? 40 : 5;
    case EFFECT_HELPING_HAND:
        return def->side == ownSide && target != battler ? 15 : -100;
    case EFFECT_FOLLOW_ME:
        option->effectMask = 1u << 10;
        return IsDoubleBattle() && atk->hp * 3 > atk->maxHp * 2 ? 20 : -40;
    case EFFECT_LEECH_SEED:
        return def->side != ownSide && !ScHasType(def, TYPE_GRASS) && def->hp * 2 > def->maxHp ? 40 : -60;
    case EFFECT_SUBSTITUTE:
        return !gBattleMons[battler].volatiles.substitute && atk->hp > atk->maxHp / 2 && threat < atk->maxHp / 4 ? 35 : -60;
    case EFFECT_HAZE:
    {
        s32 benefit = 0;
        for (u32 b = 0; b < gBattlersCount; ++b)
            if (sScMons[b].available)
                for (u32 s = STAT_ATK; s < NUM_BATTLE_STATS; ++s)
                    benefit += (sScMons[b].stages[s] - 6) * (sScMons[b].side == ownSide ? -12 : 12);
        option->effectMask = 1u << 11;
        return benefit;
    }
    case EFFECT_HEAL_BELL:
        return atk->status ? 55 : -30;
    case EFFECT_SPIKES:
    case EFFECT_STEALTH_ROCK:
    case EFFECT_TOXIC_SPIKES:
        /* Hazard layering and remaining public roster are not modeled yet. */
        return -25;
    default:
        return -35; /* A legal but unmodeled status action remains a last resort. */
    }
}

static bool32 ScBenchAvailable(u32 battler, u32 index)
{
    enum BattleTrainer trainer = GetBattlerTrainer(battler);
    struct Pokemon *mon = &gParties[trainer][index];
    if (!GetMonData(mon, MON_DATA_SPECIES) || GetMonData(mon, MON_DATA_IS_EGG) || !GetMonData(mon, MON_DATA_HP)) return FALSE;
    for (u32 b = 0; b < gBattlersCount; ++b)
    {
        if (GetBattlerSide(b) != B_SIDE_OPPONENT || GetBattlerTrainer(b) != trainer) continue;
        if (gBattlerPartyIndexes[b] == index && IsBattlerAlive(b)) return FALSE;
        /* Only this AI side's own replacement reservations are permitted. */
        if (b != battler && gBattleStruct->monToSwitchIntoId[b] == index) return FALSE;
    }
    return TRUE;
}

static void ScReadBench(u32 battler, u32 index, struct ScAiMonView *out)
{
    struct Pokemon *mon = &gParties[GetBattlerTrainer(battler)][index];
    memset(out, 0, sizeof(*out));
    out->own = TRUE;
    out->side = B_SIDE_OPPONENT;
    out->available = TRUE;
    out->species = GetMonData(mon, MON_DATA_SPECIES);
    out->level = GetMonData(mon, MON_DATA_LEVEL);
    out->hp = GetMonData(mon, MON_DATA_HP);
    out->maxHp = GetMonData(mon, MON_DATA_MAX_HP);
    out->stats[STAT_HP] = out->maxHp;
    out->stats[STAT_ATK] = GetMonData(mon, MON_DATA_ATK);
    out->stats[STAT_DEF] = GetMonData(mon, MON_DATA_DEF);
    out->stats[STAT_SPATK] = GetMonData(mon, MON_DATA_SPATK);
    out->stats[STAT_SPDEF] = GetMonData(mon, MON_DATA_SPDEF);
    out->stats[STAT_SPEED] = GetMonData(mon, MON_DATA_SPEED);
    out->ability = GetMonAbility(mon);
    out->hold = GetItemHoldEffect(GetMonData(mon, MON_DATA_HELD_ITEM));
    out->status = ScPublicStatus(GetMonData(mon, MON_DATA_STATUS));
    out->types[0] = GetSpeciesType(out->species, 0);
    out->types[1] = GetSpeciesType(out->species, 1);
    out->types[2] = TYPE_NONE;
    out->grounded = !ScHasType(out, TYPE_FLYING) && out->ability != ABILITY_LEVITATE && out->hold != HOLD_EFFECT_AIR_BALLOON;
    for (u32 i = 0; i < NUM_BATTLE_STATS; ++i) out->stages[i] = 6;
    for (u32 i = 0; i < MAX_MON_MOVES; ++i)
        if (GetMonData(mon, MON_DATA_PP1 + i)) out->moves[i] = GetMonData(mon, MON_DATA_MOVE1 + i);
    /* Candidate effect API is separate from the outgoing battler's capabilities. */
    ScGetPublicSwitchInEffects(battler, index, &out->effects);
}

static s32 ScMatchup(const struct ScAiMonView *mon)
{
    s32 offense = 0;
    for (u32 foe = 0; foe < gBattlersCount; ++foe)
    {
        const struct ScAiMonView *def = &sScMons[foe];
        if (!def->available || def->side == mon->side) continue;
        u32 damage = 0;
        for (u32 m = 0; m < MAX_MON_MOVES; ++m) damage = max(damage, ScDamage(mon, def, mon->moves[m], FALSE));
        offense = max(offense, (s32)(min(damage, def->hp) * 100 / max(1, def->hp)));
    }
    u32 incoming = ScThreat(mon);
    return offense - min(180, incoming * 100 / max(1, mon->hp));
}

static bool32 ScCanSwitch(u32 battler)
{
    const struct ScAiMonView *mon = &sScMons[battler];
    if (mon->hold == HOLD_EFFECT_SHED_SHELL || ScHasType(mon, TYPE_GHOST)) return TRUE;
    if (!CanBattlerEscape(battler) || (gFieldStatuses & STATUS_FIELD_FAIRY_LOCK)) return FALSE;
    for (u32 foe = 0; foe < gBattlersCount; ++foe)
    {
        if (!sScMons[foe].available || sScMons[foe].side == mon->side) continue;
        enum Ability ability = sScMons[foe].ability;
        if ((ability == ABILITY_SHADOW_TAG && mon->ability != ABILITY_SHADOW_TAG)
          || (ability == ABILITY_ARENA_TRAP && mon->grounded)
          || (ability == ABILITY_MAGNET_PULL && ScHasType(mon, TYPE_STEEL))) return FALSE;
    }
    return TRUE;
}

static void ScAddOption(struct ScAiObservation *out, u32 actor, const struct ScAiOption *option)
{
    if (out->count[actor] < SC_AI_OPTIONS) out->options[actor][out->count[actor]++] = *option;
}

static bool32 ScIsSpread(enum MoveTarget target)
{
    return target == TARGET_BOTH || target == TARGET_FOES_AND_ALLY || target == TARGET_ALL_BATTLERS;
}

static void ScMoveOptions(struct ScAiObservation *out, u32 actor)
{
    u32 battler = out->actors[actor];
    const struct ScAiMonView *atk = &sScMons[battler];
    /* Own legal-action state, never a player's pending choice or actual PP. */
    u32 limitations = CheckMoveLimitations(battler, 0, ~(MOVE_LIMITATION_UNUSABLE));
    for (u32 m = 0; m < MAX_MON_MOVES; ++m)
    {
        enum Move move = atk->moves[m];
        if (move == MOVE_NONE || (limitations & (1u << m))) continue;
        enum MoveTarget targetType = GetMoveTarget(move);
        bool32 spread = ScIsSpread(targetType);
        bool32 status = GetMoveCategory(move) == DAMAGE_CATEGORY_STATUS;
        for (u32 target = 0; target < gBattlersCount; ++target)
        {
            if (!sScMons[target].available) continue;
            bool32 ally = sScMons[target].side == atk->side;
            if (targetType == TARGET_USER || targetType == TARGET_FIELD || targetType == TARGET_OPPONENTS_FIELD || targetType == TARGET_USER_AND_ALLY)
            {
                if (target != battler) continue;
            }
            else if (targetType == TARGET_ALLY)
            {
                if (!ally || target == battler) continue;
            }
            else if (targetType == TARGET_USER_OR_ALLY)
            {
                if (!ally) continue;
            }
            else if (ally) continue;
            struct ScAiOption option = {.kind = SC_AI_MOVE, .index = m, .target = target};
            if (status)
            {
                option.utility = ScStatusUtility(battler, target, move, &option);
                u32 accuracy = GetMoveAccuracy(move);
                if (option.utility > 0 && accuracy) option.utility = option.utility * accuracy / 100;
            }
            else
            {
                for (u32 hit = 0; hit < gBattlersCount; ++hit)
                {
                    if (!sScMons[hit].available || hit == battler) continue;
                    if (!spread && hit != target) continue;
                    if (spread && targetType == TARGET_BOTH && sScMons[hit].side == atk->side) continue;
                    if (sScMons[hit].side == atk->side && sScMons[hit].ability == ABILITY_TELEPATHY) continue;
                    option.damage[hit] = ScDamage(atk, &sScMons[hit], move, spread);
                }
                /* Known field redirection can make selected attacks ineffective. */
                if (!spread)
                    for (u32 other = 0; other < gBattlersCount; ++other)
                        if (other != battler && other != target && sScMons[other].available
                         && ((GetMoveType(move) == TYPE_ELECTRIC && sScMons[other].ability == ABILITY_LIGHTNING_ROD)
                          || (GetMoveType(move) == TYPE_WATER && sScMons[other].ability == ABILITY_STORM_DRAIN)))
                            memset(option.damage, 0, sizeof(option.damage));
                bool32 faster = ScSpeed(atk) >= ScSpeed(&sScMons[target]);
                if (gFieldStatuses & STATUS_FIELD_TRICK_ROOM) faster = !faster;
                if (GetMovePriority(move) > 0) faster = TRUE;
                if (GetMovePriority(move) < 0) faster = FALSE;
                if (option.damage[target] >= sScMons[target].hp && faster) option.utility += 30;
                if (!faster && ScThreat(atk) >= atk->hp) option.utility -= 30;
                if (GetMoveEffect(move) == EFFECT_RECOIL) option.utility -= 10;
                if (move == MOVE_EXPLOSION || move == MOVE_SELF_DESTRUCT) option.damage[battler] = atk->hp;
                if (GetMoveEffect(move) == EFFECT_HIT_ESCAPE) option.utility += ScMatchup(atk) < 0 ? 15 : -5;
            }
            ScAddOption(out, actor, &option);
            if (spread) break;
        }
    }
    if (!out->count[actor])
    {
        struct ScAiOption struggle = {.kind = SC_AI_MOVE, .index = 0, .target = GetOppositeBattler(battler)};
        ScAddOption(out, actor, &struggle); /* Engine's legal Struggle handling owns execution. */
    }
}

static void ScSwitchOptions(struct ScAiObservation *out, u32 actor)
{
    u32 battler = out->actors[actor];
    if (!ScCanSwitch(battler)) return;
    const struct ScAiMonView *current = &sScMons[battler];
    s32 currentValue = ScMatchup(current);
    for (u32 i = 0; i < PARTY_SIZE; ++i)
    {
        if (!ScBenchAvailable(battler, i)) continue;
        struct ScAiMonView candidate;
        ScReadBench(battler, i, &candidate);
        s32 improvement = ScMatchup(&candidate) - currentValue;
        struct ScAiOption option = {.kind = SC_AI_SWITCH, .index = i, .resource = GetBattlerTrainer(battler)};
        option.utility = improvement - 40; /* Give up a turn; avoid marginal cycling. */
        if (current->ability == ABILITY_NATURAL_CURE && current->status) option.utility += 25;
        if (current->ability == ABILITY_REGENERATOR && current->hp * 2 < current->maxHp) option.utility += 25;
        if (gBattleMons[battler].volatiles.perishSong && gBattleMons[battler].volatiles.perishSongTimer <= 1) option.utility += 200;
        ScAddOption(out, actor, &option);
    }
}

static u32 ScItemHeal(u32 battler, enum Item item)
{
    const u8 *effect = GetItemEffect(item);
    if (!effect) return 0;
    u32 value = effect[GetItemEffectParamOffset(battler, item, 4, ITEM4_HEAL_HP)];
    u32 maxHp = sScMons[battler].maxHp;
    if (value == ITEM6_HEAL_HP_FULL) return maxHp;
    if (value == ITEM6_HEAL_HP_HALF) return maxHp / 2;
    if (value == ITEM6_HEAL_HP_QUARTER) return maxHp / 4;
    if (value == ITEM6_HEAL_HP_LVL_UP) return 0;
    return value;
}

static void ScItemOptions(struct ScAiObservation *out, u32 actor)
{
    u32 battler = out->actors[actor];
    const struct ScAiMonView *mon = &sScMons[battler];
    if (gBattleMons[battler].volatiles.embargoTimer || gBattleMons[battler].volatiles.semiInvulnerable == STATE_SKY_DROP_TARGET) return;
    u32 threat = ScThreat(mon);
    for (u32 i = 0; i < MAX_TRAINER_ITEMS; ++i)
    {
        enum Item item = gBattleHistory->trainerItems[i];
        if (item == ITEM_NONE) continue;
        struct ScAiOption option = {.kind = SC_AI_ITEM, .index = i, .target = battler, .resource = 0, .utility = -100};
        switch (GetItemBattleUsage(item))
        {
        case EFFECT_ITEM_RESTORE_HP:
            option.utility = ScHealingUtility(mon, ScItemHeal(battler, item), threat) - 5;
            break;
        case EFFECT_ITEM_CURE_STATUS:
        {
            const u8 *e = GetItemEffect(item);
            if (e && ((e[3] & ITEM3_SLEEP && mon->status & STATUS1_SLEEP)
             || (e[3] & ITEM3_POISON && mon->status & STATUS1_PSN_ANY)
             || (e[3] & ITEM3_BURN && mon->status & STATUS1_BURN)
             || (e[3] & ITEM3_FREEZE && mon->status & STATUS1_ICY_ANY)
             || (e[3] & ITEM3_PARALYSIS && mon->status & STATUS1_PARALYSIS))) option.utility = threat < mon->hp ? 70 : -50;
            break;
        }
        case EFFECT_ITEM_INCREASE_STAT:
            option.utility = threat * 3 < mon->hp ? 25 : -50;
            break;
        case EFFECT_ITEM_RESTORE_PP:
            for (u32 m = 0; m < MAX_MON_MOVES; ++m)
                if (mon->moves[m] && !gBattleMons[battler].pp[m] && GetMoveCategory(mon->moves[m]) != DAMAGE_CATEGORY_STATUS)
                    option.utility = threat < mon->hp ? 80 : 10;
            break;
        default:
            continue; /* Mixed-category supplies and revives are not legal SC loadout options. */
        }
        ScAddOption(out, actor, &option);
    }
}

void ScAiObserve(struct ScAiObservation *out)
{
    memset(out, 0, sizeof(*out));
    memset(sScMons, 0, sizeof(sScMons));
    for (u32 b = 0; b < gBattlersCount; ++b)
    {
        ScReadActive(b, &sScMons[b]);
        if (!sScMons[b].available) continue;
        out->aliveMask |= 1u << b;
        out->hp[b] = sScMons[b].hp;
        if (sScMons[b].own)
        {
            out->ownMask |= 1u << b;
            if (out->actorCount < SC_AI_ACTORS) out->actors[out->actorCount++] = b;
        }
    }
    for (u32 actor = 0; actor < out->actorCount; ++actor)
    {
        ScMoveOptions(out, actor);
        ScSwitchOptions(out, actor);
        ScItemOptions(out, actor);
    }
}

void ScAiPrepareTurn(void)
{
    ScAiObserve(&sScObservation);
    struct ScAiChoice choices[SC_AI_ACTORS];
    ScAiChoose(&sScObservation, choices);
    memset(sScChoices, 0, sizeof(sScChoices));
    gAiBattleData->aiUsingGimmick = 0;
    gAiBattleData->actionFlee = FALSE;
    gAiBattleData->choiceWatch = FALSE;
    for (u32 actor = 0; actor < sScObservation.actorCount; ++actor)
    {
        u32 battler = sScObservation.actors[actor];
        sScChoices[battler] = choices[actor];
        ScAiComputeBattler(battler);
    }
}

void ScAiComputeBattler(enum BattlerId battler)
{
    gAiBattleData->chosenMoveIndex[battler] = sScChoices[battler].kind == SC_AI_MOVE ? sScChoices[battler].index : 0;
    gAiBattleData->chosenTarget[battler] = sScChoices[battler].target;
    gAiLogicData->battlerMovesScored |= 1u << battler;
}

u32 ScAiMoveIndex(enum BattlerId battler)
{
    ScAiComputeBattler(battler);
    return gAiBattleData->chosenMoveIndex[battler];
}

u32 ScAiSwitchIn(enum BattlerId battler)
{
    if (GetBattlerSide(battler) != B_SIDE_OPPONENT) return PARTY_SIZE;
    /* Forced replacements and pivot moves use a fresh public observation. */
    for (u32 b = 0; b < gBattlersCount; ++b) ScReadActive(b, &sScMons[b]);
    s32 best = -32000;
    u32 chosen = PARTY_SIZE;
    for (u32 i = 0; i < PARTY_SIZE; ++i)
    {
        if (!ScBenchAvailable(battler, i)) continue;
        struct ScAiMonView candidate;
        ScReadBench(battler, i, &candidate);
        s32 score = ScMatchup(&candidate);
        if (score > best) { best = score; chosen = i; }
    }
    return chosen;
}

bool32 ScAiUseItem(enum BattlerId battler)
{
    const struct ScAiChoice *choice = &sScChoices[battler];
    if (choice->kind != SC_AI_ITEM || choice->index >= MAX_TRAINER_ITEMS) return FALSE;
    enum Item item = gBattleHistory->trainerItems[choice->index];
    if (item == ITEM_NONE) return FALSE;
    gBattleStruct->itemPartyIndex[battler] = gBattlerPartyIndexes[battler];
    gBattleStruct->chosenItem[battler] = item;
    if (GetItemBattleUsage(item) == EFFECT_ITEM_RESTORE_PP)
    {
        gBattleStruct->itemMoveIndex[battler] = 0;
        for (u32 m = 0; m < MAX_MON_MOVES; ++m)
            if (gBattleMons[battler].moves[m] && !gBattleMons[battler].pp[m]
                && GetMoveCategory(gBattleMons[battler].moves[m]) != DAMAGE_CATEGORY_STATUS)
            { gBattleStruct->itemMoveIndex[battler] = m; break; }
    }
    gBattleHistory->trainerItems[choice->index] = ITEM_NONE;
    BtlController_EmitTwoReturnValues(battler, B_COMM_TO_ENGINE, B_ACTION_USE_ITEM, 0);
    return TRUE;
}

void ScAiEmitAction(enum BattlerId battler)
{
    const struct ScAiChoice *choice = &sScChoices[battler];
    if (choice->kind == SC_AI_SWITCH && choice->index < PARTY_SIZE)
    {
        gBattleStruct->AI_monToSwitchIntoId[battler] = choice->index;
        gBattleStruct->monToSwitchIntoId[battler] = choice->index;
        gAiLogicData->monToSwitchInId[battler] = choice->index;
        BtlController_EmitTwoReturnValues(battler, B_COMM_TO_ENGINE, B_ACTION_SWITCH, 0);
        return;
    }
    if (ScAiUseItem(battler)) return;
    ScAiComputeBattler(battler);
    BtlController_EmitTwoReturnValues(battler, B_COMM_TO_ENGINE, B_ACTION_USE_MOVE, choice->target << 8);
}
#endif
