UndergroundPath_NorthEntrance_Frlg_MapScripts::
	.byte 0

UndergroundPath_NorthEntrance_EventScript_Saige::
	lock
	faceplayer
	goto_if_set FLAG_DID_MS_NIDO_TRADE, UndergroundPath_NorthEntrance_EventScript_Saige_ScEnd
	msgbox ScY_Text_Adoption, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, UndergroundPath_NorthEntrance_EventScript_Saige_ScEnd
	givemon SPECIES_NIDORAN_F, 15
	goto_if_eq VAR_RESULT, 2, Common_EventScript_NoMoreRoomForPokemon
	setflag FLAG_DID_MS_NIDO_TRADE
	msgbox ScY_Text_Adopted
UndergroundPath_NorthEntrance_EventScript_Saige_ScEnd::
	release
	end

UndergroundPath_NorthEntrance_EventScript_DeclineTrade::
	msgbox Trade_Text_ThatsTooBad
	release
	end

UndergroundPath_NorthEntrance_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox Trade_Text_ThisIsNoMon
	release
	end

UndergroundPath_NorthEntrance_EventScript_AlreadyTraded::
	msgbox Trade_Text_HowIsMyOldMon
	release
	end
