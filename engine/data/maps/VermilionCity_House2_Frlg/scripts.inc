VermilionCity_House2_Frlg_MapScripts::
	.byte 0

VermilionCity_House2_EventScript_Elyssa::
	lock
	faceplayer
	goto_if_set FLAG_DID_CH_DING_TRADE, VermilionCity_House2_EventScript_Elyssa_ScEnd
	msgbox ScY_Text_Adoption, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, VermilionCity_House2_EventScript_Elyssa_ScEnd
	givemon SPECIES_FARFETCHD, 15
	goto_if_eq VAR_RESULT, 2, Common_EventScript_NoMoreRoomForPokemon
	setflag FLAG_DID_CH_DING_TRADE
	msgbox ScY_Text_Adopted
VermilionCity_House2_EventScript_Elyssa_ScEnd::
	release
	end

VermilionCity_House2_EventScript_DeclineTrade::
	msgbox VermilionCity_House2_Text_ThatsTooBad
	release
	end

VermilionCity_House2_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox VermilionCity_House2_Text_ThisIsNoMon
	release
	end

VermilionCity_House2_EventScript_AlreadyTraded::
	msgbox VermilionCity_House2_Text_HowIsMyOldMon
	release
	end

VermilionCity_House2_Text_DoYouHaveMonWantToTradeForMyMon::
	.string "Hi!\n"
	.string "Do you have a {STR_VAR_1}?\p"
	.string "Want to trade it for my\n"
	.string "{STR_VAR_2}?$"

VermilionCity_House2_Text_ThatsTooBad::
	.string "That's too bad.$"

VermilionCity_House2_Text_ThisIsNoMon::
	.string "…This is no {STR_VAR_1}.\p"
	.string "If you get one, please trade it\n"
	.string "to me!$"

VermilionCity_House2_Text_ThankYou::
	.string "Thank you!$"

VermilionCity_House2_Text_HowIsMyOldMon::
	.string "How is my old {STR_VAR_2}?\p"
	.string "My {STR_VAR_1} is doing great!$"
