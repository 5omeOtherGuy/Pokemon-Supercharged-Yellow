CeladonCity_DepartmentStore_5F_Frlg_MapScripts::
	.byte 0

CeladonCity_DepartmentStore_5F_EventScript_Gentleman::
	msgbox CeladonCity_DepartmentStore_5F_Text_ExplainStatEnhancers, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_5F_EventScript_Sailor::
	msgbox CeladonCity_DepartmentStore_5F_Text_HereForStatEnhancers, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_5F_EventScript_FloorSign::
	msgbox CeladonCity_DepartmentStore_5F_Text_Drugstore, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_5F_EventScript_ClerkXItems::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart CeladonCity_DepartmentStore_5F_XItems
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
CeladonCity_DepartmentStore_5F_XItems::
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFEND
	.2byte ITEM_X_SPEED
	.2byte ITEM_X_SPECIAL
	.2byte ITEM_X_ACCURACY
	.2byte ITEM_GUARD_SPEC
	.2byte ITEM_DIRE_HIT
	.2byte ITEM_NONE
	release
	end

CeladonCity_DepartmentStore_5F_EventScript_ClerkVitamins::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	goto_if_set FLAG_BADGE04_GET, ScEconomy_Celadon_ExperiencedStock
	pokemart ScEconomy_Celadon_Equipment
	goto ScEconomy_Celadon_Finish

ScEconomy_Celadon_ExperiencedStock::
	pokemart ScEconomy_Celadon_EquipmentPlus
ScEconomy_Celadon_Finish::
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
ScEconomy_Celadon_Equipment::
	.2byte ITEM_SILK_SCARF
	.2byte ITEM_CHARCOAL
	.2byte ITEM_MYSTIC_WATER
	.2byte ITEM_MAGNET
	.2byte ITEM_MIRACLE_SEED
	.2byte ITEM_NEVER_MELT_ICE
	.2byte ITEM_BLACK_BELT
	.2byte ITEM_POISON_BARB
	.2byte ITEM_SOFT_SAND
	.2byte ITEM_SHARP_BEAK
	.2byte ITEM_TWISTED_SPOON
	.2byte ITEM_SILVER_POWDER
	.2byte ITEM_HARD_STONE
	.2byte ITEM_SPELL_TAG
	.2byte ITEM_DRAGON_FANG
	.2byte ITEM_BLACK_GLASSES
	.2byte ITEM_METAL_COAT
	.2byte ITEM_FAIRY_FEATHER
	.2byte ITEM_EVIOLITE
	.2byte ITEM_FOCUS_SASH
	.2byte ITEM_AIR_BALLOON
	.2byte ITEM_LIGHT_CLAY
	.2byte ITEM_NONE

	.align 2
ScEconomy_Celadon_EquipmentPlus::
	.2byte ITEM_SILK_SCARF
	.2byte ITEM_CHARCOAL
	.2byte ITEM_MYSTIC_WATER
	.2byte ITEM_MAGNET
	.2byte ITEM_MIRACLE_SEED
	.2byte ITEM_NEVER_MELT_ICE
	.2byte ITEM_BLACK_BELT
	.2byte ITEM_POISON_BARB
	.2byte ITEM_SOFT_SAND
	.2byte ITEM_SHARP_BEAK
	.2byte ITEM_TWISTED_SPOON
	.2byte ITEM_SILVER_POWDER
	.2byte ITEM_HARD_STONE
	.2byte ITEM_SPELL_TAG
	.2byte ITEM_DRAGON_FANG
	.2byte ITEM_BLACK_GLASSES
	.2byte ITEM_METAL_COAT
	.2byte ITEM_FAIRY_FEATHER
	.2byte ITEM_EVIOLITE
	.2byte ITEM_FOCUS_SASH
	.2byte ITEM_AIR_BALLOON
	.2byte ITEM_LIGHT_CLAY
	.2byte ITEM_LIGHT_BALL
	.2byte ITEM_NONE

CeladonCity_DepartmentStore_5F_Text_ExplainStatEnhancers::
	.string "Held equipment supports a battle\n"
	.string "plan. Each partner holds one item.\p"
	.string "A FOCUS SASH needs full HP.\n"
	.string "AIR BALLOON avoids GROUND moves\l"
	.string "until another attack bursts it.\p"
	.string "Both are consumed when triggered.\n"
	.string "You can always buy replacements.$"

CeladonCity_DepartmentStore_5F_Text_HereForStatEnhancers::
	.string "After four badges, this clerk also\n"
	.string "stocks LIGHT BALL for PIKACHU.\p"
	.string "It doubles PIKACHU's ATTACK and\n"
	.string "SP. ATK, but gives no extra bulk.\p"
	.string "RAICHU gains speed and durability.\n"
	.string "Either partner has a place on a team.\p"
	.string "Training points come from victories.\n"
	.string "The CENTER lounge changes builds\l"
	.string "and improves individual potential.$"

CeladonCity_DepartmentStore_5F_Text_Drugstore::
	.string "5F: BATTLE EQUIPMENT & X ITEMS$"

