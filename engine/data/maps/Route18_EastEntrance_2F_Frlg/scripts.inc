Route18_EastEntrance_2F_Frlg_MapScripts::
	.byte 0

Route18_EastEntrance_2F_EventScript_LeftBinoculars::
	msgbox Route18_EastEntrance_2F_Text_PalletTownInWest, MSGBOX_SIGN
	end

Route18_EastEntrance_2F_EventScript_RightBinoculars::
	msgbox Route18_EastEntrance_2F_Text_PeopleSwimming, MSGBOX_SIGN
	end

Route18_EastEntrance_2F_EventScript_Haden::
	lock
	faceplayer
	goto_if_set FLAG_DID_MARC_TRADE, Route18_EastEntrance_2F_EventScript_Haden_ScEnd
	msgbox ScY_Text_Adoption, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route18_EastEntrance_2F_EventScript_Haden_ScEnd
	givemon SPECIES_LICKITUNG, 15
	goto_if_eq VAR_RESULT, 2, Common_EventScript_NoMoreRoomForPokemon
	setflag FLAG_DID_MARC_TRADE
	msgbox ScY_Text_Adopted
Route18_EastEntrance_2F_EventScript_Haden_ScEnd::
	release
	end

Route18_EastEntrance_2F_EventScript_DeclineTrade::
	msgbox Trade_Text_AwwOhWell
	release
	end

Route18_EastEntrance_2F_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox Trade_Text_WhatThatsNoMon
	release
	end

Route18_EastEntrance_2F_EventScript_AlreadyTraded::
	msgbox Trade_Text_IsntMyOldMonGreat
	release
	end

Route18_EastEntrance_2F_Text_PalletTownInWest::
	.string "Let's see what the binoculars have\n"
	.string "to show…\p"
	.string "PALLET TOWN is in the west.$"

Route18_EastEntrance_2F_Text_PeopleSwimming::
	.string "Let's see what the binoculars have\n"
	.string "to show…\p"
	.string "There are people swimming.$"

