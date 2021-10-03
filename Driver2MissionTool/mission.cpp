#include "mission.hpp"

std::map<std::string, int> settingsFlags =
{
	// City
	{"CHICAGO", 0},
	{"HAVANA", 1},
	{"VEGAS", 2},
	{"RIO", 3},

	// Time
	{"DAY", 0},
	{"DUSK", 1},
	{"NIGHT", 2},
	{"DAWN", 3},

	// Weather
	{"CLEAR", 0},
	{"WET", 1},
	{"RAINY", 2},

	// Type
	{"ON_CAR", 0},
	{"START_DATA_REQUIRED", 1},
	{"ON_FOOT", 2},
	{"COUNTDOWN", 4},
	{"FELONY_BAR_NOT_ACTIVE", 0x10},
	{"FELONY_BAR_NO_FILL", 0x20},

	// TimerFlags
	{"TIMER_FLAG_ACTIVE", 1},
	{"TIMER_FLAG_COUNTER", 2},
	{"TIMER_FLAG_PAUSED", 4},
	{"TIMER_FLAG_COMPLETE_ON_OUT", 8},
	{"TIMER_FLAG_BOMB_COUNTDOWN", 0x10},
	{"TIMER_FLAG_BOMB_TRIGGERED", 0x20},

	// TargetFlags
	{"TARGET_FLAG_ACTIVE_P1", 1},
	{"TARGET_FLAG_ACTIVE_P2", 0x800},
	{"TARGET_FLAG_ACTIVE_ALLP", (TARGET_FLAG_ACTIVE_P1 | TARGET_FLAG_ACTIVE_P2)},

	{"TARGET_FLAG_COMPLETED_P1", 0x2},
	{"TARGET_FLAG_COMPLETED_P2", 0x100},
	{"TARGET_FLAG_COMPLETED_ALLP", (TARGET_FLAG_COMPLETED_P1 | TARGET_FLAG_COMPLETED_P2)},

	{"TARGET_FLAG_VISIBLE_P1", 0x200},
	{"TARGET_FLAG_VISIBLE_P2", 0x400},
	{"TARGET_FLAG_VISIBLE_ALLP", (TARGET_FLAG_VISIBLE_P1 | TARGET_FLAG_VISIBLE_P2)},

	{"TARGET_FLAG_POINT_CTF_BASE_P1", 0x10000},
	{"TARGET_FLAG_POINT_CTF_BASE_P2", 0x20000},
	{"TARGET_FLAG_POINT_CTF_FLAG", 0x30000},
	
	{"TARGET_FLAG_POINT_SECRET_POINT1", 0x1000000},
	{"TARGET_FLAG_POINT_SECRET_POINT2", 0x2000000},
	{"TARGET_FLAG_POINT_SECRET_STARTFINISH", 0x3000000},

	{"TARGET_FLAG_POINT_ON_BOAT", 0x100000},
	{"TARGET_FLAG_POINT_STOP_COPS_TRIGGER", 0x200000},

	{"TARGET_FLAG_POINT_PLAYER_MUSTHAVE_CAR", 0x800000},

	{"TARGET_FLAG_CAR_SAVED", 0x10},
	{"TARGET_FLAG_CAR_PLAYERCONTROLLED", 0x20},
	{"TARGET_FLAG_CAR_SWAPPED", 0x40},
	{"TARGET_FLAG_CAR_PINGED_IN", 0x40000000},
	
	{"TARGET_FLAG_EVENT_TRIGGERED", 0x1000},	

	// DisplayFlags
	{"DISP_TARGET_BLIP", 0x10},
	{"DISP_TARGET_ARROW", 0x40},
	{"DISP_TARGET_ARROW_TWO", 0x70},

	// CarTargetFlags
	{"CARTARGET_FLAG_RANDOMCHASE", 0x1},
	{"CARTARGET_FLAG_THROWS_BOMBS", 0x4},
	{"CARTARGET_FLAG_ISCOP", 0x8},
	
	{"CARTARGET_FLAG_PROXIMITY_TARGET", 0x10},
	{"CARTARGET_FLAG_STEAL_TARGET", 0x30},
	{"CARTARGET_FLAG_ESCAPE_TARGET", 0x40},
	{"CARTARGET_FLAG_CnR_TARGET", 0x50},
	
	{"CARTARGET_FLAG_TO_BE_STOLEN", 0x10000},
	{"CARTARGET_FLAG_FLIPPED", 0x40000},
	{"CARTARGET_FLAG_DAMAGED", 0x80000},
	{"CARTARGET_FLAG_PED_ESCAPES", 0x100000},
	{"CARTARGET_FLAG_DETONATOR", 0x200000},
	
	{"CARTARGET_FLAG_BACK_TO_PLAYERCAR", 0x400000},
	{"CARTARGET_FLAG_SET_PLAYERCAR", 0x800000},
};

std::map<int, std::string> carTargetsProp =
{
	{MAKE_VAR(0,INTEGER), "type"},
	{MAKE_VAR(1,MAP_VALUES), "targetFlags"},
	{MAKE_VAR(2,MAP_VALUES), "displayFlags"},
	{MAKE_VAR(3,INTEGER), "posX"},
	{MAKE_VAR(4,INTEGER), "posZ"},
	{MAKE_VAR(5,INTEGER), "rotation"},
	{MAKE_VAR(6,INTEGER), "slot"},
	{MAKE_VAR(7,INTEGER), "carModel"},
	{MAKE_VAR(8,INTEGER), "carColour"},
	{MAKE_VAR(9,SKIP), "carType"},
	{MAKE_VAR(10,MAP_VALUES), "flags"},
	{MAKE_VAR(11,INTEGER), "cutscene"},
	{MAKE_VAR(12,INTEGER), "maxDistance"},
};

std::map<int, std::string> carChasingProp =
{
	{MAKE_VAR(0,INTEGER), "maxDamage"},
	{MAKE_VAR(1,STRING_OFFSET), "tooFarMessage"},
	{MAKE_VAR(2,STRING_OFFSET), "gettingFarMessage"},
};

std::map<int, std::string> carTailingProp =
{
	{MAKE_VAR(0,STRING_OFFSET), "closeMessage"},
	{MAKE_VAR(1,STRING_OFFSET), "farMessage"},
};

std::map<int, std::string> pointTargetsProp =
{
	{MAKE_VAR(0,SKIP), "type"},
	{MAKE_VAR(1,MAP_VALUES), "targetFlags"},
	{MAKE_VAR(2,MAP_VALUES), "displayFlags"},
	{MAKE_VAR(3,INTEGER), "posX"},
	{MAKE_VAR(4,INTEGER), "posZ"},
	{MAKE_VAR(5,INTEGER), "posY"},
	{MAKE_VAR(6,INTEGER), "height"},
	{MAKE_VAR(7,INTEGER), "radius"},
	{MAKE_VAR(8,STRING_OFFSET), "looseTailMessage"},
	{MAKE_VAR(9,MAP_VALUES), "actionFlag"},
	{MAKE_VAR(10,INTEGER), "boatOffsetX"},
	{MAKE_VAR(11,INTEGER), "boatOffsetZ"},
};

std::map <std::string, std::pair<int, int>> functionsProp =
{
	{"PlayCutscene", {1, CMD_PlayCutscene}},
	{"CompleteAllActiveTargets", {0, CMD_CompleteAllActiveTargets}},
	{"SetVariable", {2, CMD_SetVariable}},
	// {"Jump", {1, CMD_Jump}},
	{"MultiCarEvent", {1, CMD_MultiCarEvent}},
	{"SetPlayerFelony", {1, CMD_SetPlayerFelony}},
	{"ShowPlayerMessage", {2, CMD_ShowPlayerMessage}},
	{"TriggerEvent", {1, CMD_TriggerEvent}},
	{"SetDoorsLocked", {1, CMD_SetDoorsLocked}},
	{"ShowOutOfTimeMessage", {0, CMD_ShowOutOfTimeMessage}},
	{"StopThread", {0, CMD_StopThread}},
	{"StartThreadForPlayer", {1, CMD_StartThreadForPlayer}},
	{"StartThread2", {1, CMD_StartThread2}},
	{"SetCameraEvent", {1, CMD_SetCameraEvent}},
	{"AwardPlayerCheat", {1, CMD_AwardPlayerCheat}},
	{"SetRaining", {1, CMD_SetRaining}},
	{"SetMissionComplete", {0, CMD_SetMissionComplete}},
	{"SetTimerFlagCounter", {0, CMD_SetTimerFlagCounter}},
	{"SetBombTimerFlag", {0, CMD_SetBombTimerFlag}},
	{"UnSetTimerFlagCounter", {0, CMD_UnSetTimerFlagCounter}},
	{"IfProcessTarget", {1, CMD_BranchIf}},
};