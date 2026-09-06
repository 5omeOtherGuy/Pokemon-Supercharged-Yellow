#include "global.h"
#include "battle.h"
#include "data.h"
#include "trainer_util.h"
#include "sc_ai.h"
#include "test/battle.h"
#include "constants/opponents_frlg.h"

// The default test table replaces authored trainers with synthetic IDs 1/2.
// These cases require the separately cached campaign-data build.
#if SC_TEST_CAMPAIGN
static void AuthoredMon(u32 line, u32 trainerId, u32 slot)
{
    const struct Trainer *trainer = GetTrainerStructFromId(trainerId);
    const struct TrainerMon *entry = &trainer->party[slot];
    struct TrainerGenerator generator = {0};
    u16 moves[MAX_MON_MOVES];
    MakeTrainerGenerator(&generator, trainer);
    GenerateMonFromTrainerMon(gBattleTestRunnerState->data.currentMon, entry, &generator);
    Nature_(line, entry->nature);
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        moves[i] = entry->moves[i];
    Moves_(line, moves); // Preserve the runner's explicit-moves legality checks.
}
#define AUTHORED(trainer, slot) AuthoredMon(__LINE__, trainer, slot)

AI_SINGLE_BATTLE_TEST("SC simulation: Brock Geodude punishes Electric-only Pikachu with Ground coverage")
{
    GIVEN {
        ScAiEnableForTests(TRUE);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE);
        ASSUME(GetTrainerPartyFromId(TRAINER_LEADER_BROCK)[0].species == SPECIES_GEODUDE);
        PLAYER(SPECIES_PIKACHU) { Level(15); Speed(100); Moves(MOVE_THUNDER_SHOCK); }
        OPPONENT(SPECIES_GEODUDE) { AUTHORED(TRAINER_LEADER_BROCK, 0); Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER_SHOCK); EXPECT_MOVE(opponent, MOVE_BULLDOZE); }
    } SCENE {
        MESSAGE("It doesn't affect the opposing Geodude…");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLDOZE, opponent);
        HP_BAR(player);
    } THEN {
        EXPECT(ScAiEnabled());
        EXPECT_EQ(opponent->hp, opponent->maxHP);
        EXPECT_LT(player->hp, player->maxHP);
    }
}

SINGLE_BATTLE_TEST("SC simulation: cap-level Mankey Low Kick defeats authored first Onix")
{
    GIVEN {
        ASSUME(GetTrainerPartyFromId(TRAINER_LEADER_BROCK)[1].species == SPECIES_ONIX);
        PLAYER(SPECIES_MANKEY) { Level(15); Speed(100); Moves(MOVE_LOW_KICK); }
        OPPONENT(SPECIES_ONIX) { AUTHORED(TRAINER_LEADER_BROCK, 1); Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_LOW_KICK); MOVE(opponent, MOVE_HARDEN); }
    } THEN {
        EXPECT_EQ(opponent->hp, 0);
    }
}

SINGLE_BATTLE_TEST("SC simulation: Nidoran Double Kick answers authored Geodude Sturdy")
{
    GIVEN {
        PLAYER(SPECIES_NIDORAN_M) { Level(15); Speed(100); Moves(MOVE_DOUBLE_KICK); }
        OPPONENT(SPECIES_GEODUDE) { AUTHORED(TRAINER_LEADER_BROCK, 0); Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_KICK); MOVE(opponent, MOVE_DEFENSE_CURL); }
    } THEN {
        EXPECT_EQ(opponent->hp, 0);
    }
}

DOUBLE_BATTLE_TEST("SC simulation: Misty spread counter gains from Helping Hand without hurting its partner", s16 damage)
{
    bool32 help;
    PARAMETRIZE { help = FALSE; }
    PARAMETRIZE { help = TRUE; }
    GIVEN {
        PLAYER(SPECIES_IVYSAUR) { Level(22); Speed(80); Moves(MOVE_RAZOR_LEAF); }
        PLAYER(SPECIES_PIKACHU) { Level(22); Speed(100); Moves(MOVE_HELPING_HAND, MOVE_GROWL); }
        OPPONENT(SPECIES_PSYDUCK) { AUTHORED(TRAINER_LEADER_MISTY, 0); Speed(40); }
        OPPONENT(SPECIES_STARMIE) { AUTHORED(TRAINER_LEADER_MISTY, 1); Speed(60); }
    } WHEN {
        TURN {
            if (help) MOVE(playerRight, MOVE_HELPING_HAND, target: playerLeft);
            else MOVE(playerRight, MOVE_GROWL);
            MOVE(playerLeft, MOVE_RAZOR_LEAF);
            MOVE(opponentRight, MOVE_LIGHT_SCREEN);
            MOVE(opponentLeft, MOVE_RAIN_DANCE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_RAZOR_LEAF, playerLeft);
        HP_BAR(opponentLeft, captureDamage: &results[i].damage);
        HP_BAR(opponentRight);
    } THEN {
        EXPECT_EQ(playerRight->hp, playerRight->maxHP);
    } FINALLY {
        EXPECT_GT(results[1].damage, results[0].damage);
    }
}

SINGLE_BATTLE_TEST("SC simulation: Koga Levitate rejects Ground while Nidoking coverage still works")
{
    GIVEN {
        PLAYER(SPECIES_NIDOKING) { Level(44); Speed(100); Moves(MOVE_EARTH_POWER, MOVE_THUNDERBOLT); }
        OPPONENT(SPECIES_WEEZING) { AUTHORED(TRAINER_LEADER_KOGA, 3); Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_EARTH_POWER); MOVE(opponent, MOVE_WILL_O_WISP); }
        TURN { MOVE(player, MOVE_THUNDERBOLT); MOVE(opponent, MOVE_FLAMETHROWER); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_LEVITATE);
        MESSAGE("It doesn't affect the opposing Weezing…");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THUNDERBOLT, player);
        HP_BAR(opponent);
    } THEN {
        EXPECT_GT(opponent->hp, 0);
        EXPECT_LT(opponent->hp, opponent->maxHP);
    }
}

DOUBLE_BATTLE_TEST("SC simulation: Sabrina terrain stops priority but permits ordinary Crunch counterplay")
{
    GIVEN {
        PLAYER(SPECIES_PIKACHU) { Level(50); Speed(100); Moves(MOVE_LIGHT_SCREEN, MOVE_QUICK_ATTACK); }
        PLAYER(SPECIES_SNORLAX) { Level(50); Speed(40); Moves(MOVE_PROTECT, MOVE_CRUNCH); }
        OPPONENT(SPECIES_MR_MIME) { AUTHORED(TRAINER_LEADER_SABRINA, 0); Speed(60); }
        OPPONENT(SPECIES_HYPNO) { AUTHORED(TRAINER_LEADER_SABRINA, 1); Speed(50); }
    } WHEN {
        TURN {
            MOVE(playerRight, MOVE_PROTECT);
            MOVE(playerLeft, MOVE_LIGHT_SCREEN);
            MOVE(opponentLeft, MOVE_REFLECT);
            MOVE(opponentRight, MOVE_PSYCHIC_TERRAIN);
        }
        TURN {
            MOVE(opponentLeft, MOVE_PROTECT);
            MOVE(opponentRight, MOVE_HELPING_HAND, target: opponentLeft);
            MOVE(playerLeft, MOVE_QUICK_ATTACK, target: opponentRight);
            MOVE(playerRight, MOVE_CRUNCH, target: opponentRight);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PSYCHIC_TERRAIN, opponentRight);
        MESSAGE("The opposing Hypno is protected by the Psychic Terrain!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CRUNCH, playerRight);
        HP_BAR(opponentRight);
    } THEN {
        EXPECT_LT(opponentRight->hp, opponentRight->maxHP);
    }
}

SINGLE_BATTLE_TEST("SC simulation: replacing Blaine Drought with rain strengthens Water damage", s16 damage)
{
    bool32 rain;
    PARAMETRIZE { rain = FALSE; }
    PARAMETRIZE { rain = TRUE; }
    GIVEN {
        PLAYER(SPECIES_GOLDUCK) { Level(56); Ability(ABILITY_SWIFT_SWIM); Speed(100); Moves(MOVE_RAIN_DANCE, MOVE_TAIL_WHIP, MOVE_WATER_PULSE); }
        OPPONENT(SPECIES_NINETALES) { AUTHORED(TRAINER_LEADER_BLAINE, 0); Speed(50); }
    } WHEN {
        TURN { MOVE(player, rain ? MOVE_RAIN_DANCE : MOVE_TAIL_WHIP); MOVE(opponent, MOVE_WILL_O_WISP); }
        TURN { MOVE(player, MOVE_WATER_PULSE); MOVE(opponent, MOVE_WILL_O_WISP); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_DROUGHT);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_PULSE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_GT(results[1].damage, results[0].damage * 2);
    }
}
#endif
