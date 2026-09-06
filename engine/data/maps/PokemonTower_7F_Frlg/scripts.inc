PokemonTower_7F_Frlg_MapScripts::
	.byte 0

PokemonTower_7F_EventScript_MrFuji::
	goto_if_unset FLAG_SC_DUO_3, ScY_RocketDuo3
	lock
	faceplayer
	famechecker FAMECHECKER_MRFUJI, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	setflag FLAG_HIDE_TOWER_FUJI
	clearflag FLAG_HIDE_POKEHOUSE_FUJI
	setflag FLAG_RESCUED_MR_FUJI
	msgbox PokemonTower_7F_Text_MrFujiThankYouFollowMe
	closemessage
	warp MAP_LAVENDER_TOWN_VOLUNTEER_POKEMON_HOUSE, 4, 7
	waitstate
	release
	end

PokemonTower_7F_EventScript_Unused::
	release
	end

PokemonTower_7F_Text_MrFujiThankYouFollowMe::
	.string "MR. FUJI: Heh?\n"
	.string "You came to save me?\p"
	.string "Thank you. But, I came here of my\n"
	.string "own free will.\p"
	.string "I came to calm the spirit of\n"
	.string "CUBONE's mother.\p"
	.string "I think MAROWAK's spirit has\n"
	.string "finally left us.\p"
	.string "I must thank you for your kind\n"
	.string "concern.\p"
	.string "Follow me to my home, POKéMON\n"
	.string "HOUSE, at the foot of this tower.$"


ScY_RocketDuo3::
	lockall
	goto_if_set FLAG_SC_DUO_3, ScY_End
	msgbox ScY_RocketDuo3Intro
	setvar VAR_0x8004, TRAINER_SC_ROCKET_DUO_3
	special ScShowBossBriefing
	waitstate
	goto_if_eq VAR_RESULT, FALSE, ScY_End
	trainerbattle_no_intro TRAINER_SC_ROCKET_DUO_3, ScY_RocketDuo3Defeat
	setflag FLAG_SC_DUO_3
	msgbox ScY_RocketBlastOff
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_SC_POKEMONTOWER_7F_FRLG_2
	removeobject LOCALID_SC_POKEMONTOWER_7F_FRLG_3
	fadescreen FADE_FROM_BLACK
	goto ScY_End
ScY_RocketDuo3Intro::
	.string "JESSIE: Prepare for trouble!\nJAMES: And make it double!\pMEOWTH: That POKéMON belongs to us!$"
ScY_RocketDuo3Defeat::
	.string "Our perfect plan… ruined again!$"
