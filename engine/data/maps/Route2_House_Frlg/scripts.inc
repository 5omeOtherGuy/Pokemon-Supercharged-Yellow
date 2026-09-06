Route2_House_Frlg_MapScripts::
	.byte 0

Route2_House_EventScript_Scientist::
	msgbox Route2_House_Text_FaintedMonsCanUseFieldMoves, MSGBOX_NPC
	end

Route2_House_EventScript_Reyley::
	lock
	faceplayer
	goto_if_set FLAG_DID_MIMIEN_TRADE, Route2_House_EventScript_Reyley_ScEnd
	msgbox ScY_Text_Adoption, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route2_House_EventScript_Reyley_ScEnd
	givemon SPECIES_MR_MIME, 15
	goto_if_eq VAR_RESULT, 2, Common_EventScript_NoMoreRoomForPokemon
	setflag FLAG_DID_MIMIEN_TRADE
	msgbox ScY_Text_Adopted
Route2_House_EventScript_Reyley_ScEnd::
	release
	end

Route2_House_EventScript_DeclineTrade::
	msgbox Trade_Text_AwwOhWell
	release
	end

Route2_House_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox Trade_Text_WhatThatsNoMon
	release
	end

Route2_House_EventScript_AlreadyTraded::
	msgbox Trade_Text_IsntMyOldMonGreat
	release
	end

Route2_House_Text_FaintedMonsCanUseFieldMoves::
	.string "A fainted POKéMON just has no\n"
	.string "energy left to battle.\p"
	.string "It can still use moves like CUT\n"
	.string "outside of battle.$"

