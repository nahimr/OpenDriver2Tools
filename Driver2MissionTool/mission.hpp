#pragma once

#include <map>
#include <unordered_map>
#include <string>

#define MAKE_VAR(pos, var) ((pos << 4) | var)

enum VALUE_TYPES
{
	INTEGER = 0,
	MAP_VALUES,
	STRING_OFFSET,
	SKIP,
};

extern std::map<std::string, int> settingsFlags;
extern std::map<int, std::string> carTargetsProp;
extern std::map<int, std::string> carChasingProp;
extern std::map<int, std::string> carTailingProp;
extern std::map<int, std::string> pointTargetsProp;

#define MISSION_IDENT		(('D' << 24) | ('2' << 16) | ('M' << 8) | 'S' )

typedef unsigned short u_short;
typedef unsigned int u_int;

typedef struct _COP_DATA
{
	int speed;
	int power;
	int min;
	int max;
} MS_COP_DATA;

struct XYPAIR
{
	int x;
	int y;
};

typedef struct {		/* long word type 3D vector */
	long	vx, vy;
	long	vz, pad;
} VECTOR;

struct MULTICAR_DATA
{
	int x;
	int z;
	unsigned char palette;
	unsigned char model;
	short rot;
};

typedef struct _MISSION
{
	int id;
	int size;
	int city;
	int time;
	int weather;
	XYPAIR playerStartPosition;
	int playerStartRotation;
	int type;
	short timer;
	short timerFlags;
	int strings;
	MS_COP_DATA cops;
	int msgCarWrecked;
	int msgOutOfTime;
	int msgComplete;
	int region;
	int route;
	int msgDrowned;
	int playerCarModel;
	int playerCarColour;
	int maxDamage;
	int residentModels[5];
	int nCutscenes;
	int title;
	int msgPoliceCar;
	int msgDoorsLocked;
} MS_MISSION;

typedef struct _TARGET
{
	int type;			// data 0

	union
	{
		MULTICAR_DATA multiCar[5];

		struct
		{
			int target_flags;	// data 1
			int display_flags;	// data 2

			union
			{
				struct
				{
					int posX;				// data 3
					int posZ;				// data 4
					int radius;				// data 5
					int posY;				// data 6
					int height;				// data 7
					int loseTailMessage;	// data 8
					int actionFlag;			// data 9
					int boatOffsetX;		// data 10
					int boatOffsetZ;		// data 11
				} point;

				struct
				{
					int posX;			// data 3
					int posZ;			// data 4
					int rotation;		// data 5
					int slot;			// data 6
					int model;			// data 7
					int palette;		// data 8
					int type;			// data 9
					int flags;			// data 10
					int cutscene;		// data 11
					int maxDistance;	// data 12
					union
					{
						struct
						{
							int closeMessages;		// data 13
							int farMessages;		// data 14
						} tail;
						struct
						{
							int maxDamage;			// data 13
							int tooFarMessage;		// data 14
							int gettingFarMessage;	// data 15
						} chasing;
					};
				} car;

				struct
				{
					int eventId;			// data 3
					union
					{
						VECTOR* eventPos;	// data 4
						int unused[10];
					};
					int loseMessage;		// data 14
				} event;
			};
		};
	};
} MS_TARGET;

enum MS_HEADER_TYPE
{
	MSTYPE_StartOnCar = 0,
	MSTYPE_StartDataRequired = 1,
	MSTYPE_StartOnFoot = 2,
	MSTYPE_Countdown = 4, // Countdown before start
	MSTYPE_FelonyBarNotActive = 0x10,
	MSTYPE_FelonyBarNoFill = 0x20,
};

enum MS_CITY
{
	CHICAGO = 0,
	HAVANA = 1,
	VEGAS = 2,
	RIO = 3
};

enum TIME
{
	DAY = 0,
	DUSK,
	NIGHT,
	DAWN
};

enum WEATHER
{
	CLEAR = 0,
	RAINY,
	WET
};

enum TargetFlags
{
	// common target properties
	TARGET_FLAG_ACTIVE_P1 = 0x1,
	TARGET_FLAG_ACTIVE_P2 = 0x800,
	TARGET_FLAG_ACTIVE_ALLP = (TARGET_FLAG_ACTIVE_P1 | TARGET_FLAG_ACTIVE_P2),

	TARGET_FLAG_COMPLETED_P1 = 0x2,
	TARGET_FLAG_COMPLETED_P2 = 0x100,
	TARGET_FLAG_COMPLETED_ALLP = (TARGET_FLAG_COMPLETED_P1 | TARGET_FLAG_COMPLETED_P2),

	TARGET_FLAG_VISIBLE_P1 = 0x200,
	TARGET_FLAG_VISIBLE_P2 = 0x400,
	TARGET_FLAG_VISIBLE_ALLP = (TARGET_FLAG_VISIBLE_P1 | TARGET_FLAG_VISIBLE_P2),

	// point target properties
	TARGET_FLAG_POINT_CTF_BASE_P1 = 0x10000,
	TARGET_FLAG_POINT_CTF_BASE_P2 = 0x20000,
	TARGET_FLAG_POINT_CTF_FLAG = 0x30000,

	TARGET_FLAG_POINT_SECRET_POINT1 = 0x1000000,
	TARGET_FLAG_POINT_SECRET_POINT2 = 0x2000000,
	TARGET_FLAG_POINT_SECRET_STARTFINISH = 0x3000000,

	TARGET_FLAG_POINT_ON_BOAT = 0x100000,
	TARGET_FLAG_POINT_STOP_COPS_TRIGGER = 0x200000,
	//0x400000,
	TARGET_FLAG_POINT_PLAYER_MUSTHAVE_CAR = 0x800000,

	// car target properties
	TARGET_FLAG_CAR_SAVED = 0x10,
	TARGET_FLAG_CAR_PLAYERCONTROLLED = 0x20,
	TARGET_FLAG_CAR_SWAPPED = 0x40,
	TARGET_FLAG_CAR_PINGED_IN = 0x40000000,

	// event target properties
	TARGET_FLAG_EVENT_TRIGGERED = 0x1000,
};

enum CarTargetFlags
{
	CARTARGET_FLAG_RANDOMCHASE = 0x1,
	CARTARGET_FLAG_THROWS_BOMBS = 0x4,
	CARTARGET_FLAG_ISCOP = 0x8,			// makes CATARGET_FLAG_ISCOP

	// FIXME: those target names can be specified poorly
	CARTARGET_FLAG_PROXIMITY_TARGET = 0x10,
	CARTARGET_FLAG_STEAL_TARGET = 0x30,
	CARTARGET_FLAG_ESCAPE_TARGET = 0x40,
	CARTARGET_FLAG_CnR_TARGET = 0x50,

	CARTARGET_FLAG_TO_BE_STOLEN = 0x10000,
	CARTARGET_FLAG_FLIPPED = 0x40000,
	CARTARGET_FLAG_DAMAGED = 0x80000,
	CARTARGET_FLAG_PED_ESCAPES = 0x100000,
	CARTARGET_FLAG_DETONATOR = 0x200000,		// has a detonator timer

	CARTARGET_FLAG_BACK_TO_PLAYERCAR = 0x400000,		// to retrieve the car to return back to
	CARTARGET_FLAG_SET_PLAYERCAR = 0x800000,		// sets the car to return back to
};

enum ECarControlType
{
	CONTROL_TYPE_NONE = 0,				// car is not in the world
	CONTROL_TYPE_PLAYER = 1,			// controlled by player pads
	CONTROL_TYPE_CIV_AI = 2,			// Civilian car. May be a passive cop car with CONTROL_FLAG_COP flag.
	CONTROL_TYPE_PURSUER_AI = 3,		// Police pursuer car. Always chases player
	CONTROL_TYPE_LEAD_AI = 4,			// FreeRoamer AI

	CONTROL_TYPE_CAMERACOLLIDER = 5,	// Used as a camera collider
	CONTROL_TYPE_TANNERCOLLIDER = 6,	// Used as collision box for tanner
	CONTROL_TYPE_CUTSCENE = 7,			// Pretty same as player car but controllled by cutscene. Can be a chase car.
};

enum MR_COMMAND_FLAGS
{
	CMD_PlayCutscene = 0x1000051, // 1 arg
	CMD_CompleteAllActiveTargets = 0x1000021, // 0 arg
	CMD_SetVariable = 0x1000010, // 2 args (addr,value)
	CMD_Jump = 0x1000011, // 1 arg (addr)
	CMD_BranchIf = 0x1000001, // 2 arg 
	CMD_MultiCarEvent = 0x1000022, // 1 arg
	CMD_SetPlayerFelony = 0x1000030, // 1 arg
	CMD_ShowPlayerMessage = 0x1000050, // 2 arg (offset, timeS)
	CMD_TriggerEvent = 0x1000070, // 1 arg (event)
	CMD_SetDoorsLocked = 0x1000080, // 1 arg (bool)
	CMD_SetStealMessage = 0x1000054, // 1 arg (offset)
	CMD_ShowOutOfTimeMessage = 0x1000055, // 0 arg
	CMD_StopThread = 0x1001000, // 0 arg
	CMD_StartThreadForPlayer = 0x1001002, // 0 arg
	CMD_StartThread2 = 0x1001003, // 1 arg (addr)
	CMD_SetCameraEvent = 0x1000100, // 0 arg
	CMD_AwardPlayerCheat = 0x1000071, // 1 arg
	CMD_SetRaining = 0x1000090, // 0 arg
	CMD_SetMissionComplete = 0x1001001, // 0 arg
	CMD_SetTimerFlagCounter = 0x1000040, // 0 arg
	CMD_SetBombTimerFlag = 0x1000042, // 0 arg
	CMD_UnSetTimerFlagCounter = 0x1000041, //0 arg
	CMD_DisableChaseMusic = 0x1000045, // 1 arg
	CMD_SetCopsSpeech = 0x1000046, // 1 arg
	CMD_PlayOnlyChaseMusic = 0x1000044, // 1 arg

	// TODO: Miss 3 commands of timers 

};

enum TARGET_TYPE
{
	Target_Point = 1,
	Target_Car = 2,				// any mission target car. Also, a multiplayer player 2 car in Cops'n'Robbers
	Target_Event = 3,
	Target_Player2Start = 4,		// player 2 start
	Target_MultiCar = 5
};

typedef enum DISP_FLAGS {
	DISP_TARGET_BLIP = 0x10,
	DISP_TARGET_ARROW = 0x40,
	DISP_TARGET_ARROW_TWO = 0x70,
} DISP_FLAGS;

enum MR_OPERATOR_FLAGS
{
	OP_AND = 0x3000003, // AND
	OP_OR = 0x3000004, // OR
	OP_NEQ = 0x3000005, // NEQ
	OP_EQ = 0x3000006, // EQ
	OP_GT = 0x3000007, // GT
	OP_LT = 0x3000008, // LT
	OP_ADD = 0x3000009, // ADD
	OP_NONE = 0x0, // Stop Thread
};

enum MR_VAR_FLAGS
{
	VAR_Timer = 0x2000008,
	VAR_gCopDesiredSpeedScale = 0x2000100,
	VAR_gCopMaxPowerScale = 0x2000101,
	VAR_gMinimumCops = 0x2000102,
	VAR_maxCopCars = 0x2000103,
};

enum MR_FUNCTION_FLAGS
{
	FUNC_ProcessTarget = 0x4000020, // ProcessTarget(targetIndex)
};
