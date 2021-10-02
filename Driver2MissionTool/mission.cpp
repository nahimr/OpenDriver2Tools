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
};