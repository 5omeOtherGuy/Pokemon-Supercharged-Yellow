CeruleanCity_House5_Frlg_MapScripts::
	.byte 0

CeruleanCity_House5_EventScript_BerryPowderMan::
	lock
	faceplayer
	msgbox ScEconomy_BerrySpecialist_Welcome
	pokemart ScEconomy_Cerulean_BerriesAndEquipment
	msgbox gText_PleaseComeAgain
	release
	end

CeruleanCity_House5_EventScript_BerryCrushRankings::
	msgbox ScEconomy_BerrySpecialist_Guide, MSGBOX_SIGN
	end

	.align 2
ScEconomy_Cerulean_BerriesAndEquipment::
	.2byte ITEM_ORAN_BERRY
	.2byte ITEM_CHERI_BERRY
	.2byte ITEM_CHESTO_BERRY
	.2byte ITEM_PECHA_BERRY
	.2byte ITEM_RAWST_BERRY
	.2byte ITEM_ASPEAR_BERRY
	.2byte ITEM_SITRUS_BERRY
	.2byte ITEM_LUM_BERRY
	.2byte ITEM_LEPPA_BERRY
	.2byte ITEM_EVIOLITE
	.2byte ITEM_MAGNET
	.2byte ITEM_NONE

ScEconomy_BerrySpecialist_Welcome::
	.string "BERRIES help a held partner recover.\n"
	.string "Consumed ones need replacing.\p"
	.string "I keep a fresh supply here.\n"
	.string "MAGNET boosts ELECTRIC attacks.\p"
	.string "EVIOLITE strengthens the defenses\n"
	.string "of partners that can still evolve.$"

ScEconomy_BerrySpecialist_Guide::
	.string "A held BERRY acts on its own.\n"
	.string "It uses your held-item slot.\p"
	.string "Bag supplies are separate: prepare\n"
	.string "up to three units upstairs at a\l"
	.string "POKéMON CENTER before battle.\p"
	.string "Permanent training comes from wins,\n"
	.string "not vitamins. Change future focus\l"
	.string "with TRAINING in your party menu.$"

