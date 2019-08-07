#pragma once
#include <stdint.h>
#include "../../EurorackShared/EurorackShared.h"

#ifdef WIN32
#define NOINLINE
#else
#define NOINLINE __attribute__((noinline))
#endif

#define TUESDAY_MAXBEAT_VAL 32
#define TUESDAY_MAXTPB_VAL 10
#define TUESDAY_MAXTICK (TUESDAY_MAXTPB_VAL * TUESDAY_MAXBEAT_VAL)

#define TUESDAY_GATES 6

#define GATE_TICK 4
#define GATE_CLOCK 5
#define GATE_BEAT 3
#define GATE_LOOP 2
#define GATE_ACCENT 1
#define GATE_GATE 0
#define GATE_MINGATETIME 10
#define TUESDAY_LEDS 16
#define TUESDAY_NOTEOFF -100
#define TUESDAY_SUBTICKRES 6
#define TUESDAY_DEFAULTGATELENGTH ((TUESDAY_SUBTICKRES * 3) / 4)


#define VERSIONBYTE 0x15
#define CALIBRATIONVERSIONBYTE 0x13



typedef struct Tuesday_Tick_t
{
	unsigned char vel;
	signed char note;
	unsigned char accent :1;
	unsigned char slide : 2;	
	unsigned char maxsubticklength : 4;
} Tuesday_Tick_t;

typedef struct Tuesday_PatternContainer
{
	Tuesday_Tick_t Ticks[TUESDAY_MAXTICK];
	unsigned char TPB;
	unsigned char Length;
} Tuesday_PatternContainer;

typedef struct Tuesday_RandomGen
{
	long RandomMemory;
} Tuesday_RandomGen;


//4096 = 2.048v
/*2.5 * (2.048 * INP)/4096
		(x * 4096)
		/ 2.5 * 2.048
		 = inp*/

#define DAC_VOLT_UNCALIBRATED(x) ((int32_t)((409600 * ((int32_t)x)) / (int32_t)(512)))


#define DAC_VOLT(x, chan) CalibratedDAC(chan, ((int32_t)((409600 * ((int32_t)x)) / (int32_t)(512 * 64))))
#define DAC_NOTE(x, chan) (DAC_VOLT((x*64)/12, chan))


typedef struct Tuesday_PatternGen
{

	uint8_t T;
	uint8_t ClockConnected;
	
	int32_t Gates[TUESDAY_GATES+1];
	int32_t GatesGap[TUESDAY_GATES+1];
	uint8_t RStateLeds[TUESDAY_LEDS];
	uint8_t StateLedTargets[TUESDAY_LEDS];

	uint16_t NoteOut;
	uint16_t VelocityOut;

	int lastnote;
	int CoolDown;
	uint16_t TickOut;
	
	int32_t CVOut;
	int32_t CVOutTarget;
	int32_t CVOutDelta;
	int32_t CVOutCountDown;

	int Tick;
	int Measure;
	int NewTick;
	int DoReset;
	int countdownTick;
	//int countdownNote;
	int msecpertick;
	int msecsincelasttick;

	int clockup;
	int clockshad;
	int clockssincereset;

	int timesincelastclocktick;
	int clocktick;

	int directtick;
	int extclockssincereset[6];
	uint8_t extclockssinceresetcounter[6];
	int extclocksupsincereset;
	int extclocksdowsincereset;
	int lastclocksubdiv;


	// knobs
	uint8_t seed1;
	uint8_t seed2;
	uint8_t intensity;
	uint8_t tempo;

	uint8_t UIMode;
	uint8_t CalibTarget;
	uint8_t OptionSelect;
	uint8_t OptionIndex;
	struct Tuesday_PatternContainer CurrentPattern;

	uint8_t switchmode;
	uint8_t commitchange;
	uint8_t TicksPerMeasure;
	int ticklengthscaler;
	uint8_t LastResetVal;
} Tuesday_PatternGen;

#define TUESDAY_MAXALGO 4
#define TUESDAY_MAXSCALE 4
#define TUESDAY_MAXBEAT 4
#define TUESDAY_MAXTPB 4

#define ALTERNATESCALES

typedef enum
{
#ifndef ALTERNATESCALES
	SCALE_MAJOR,
	SCALE_MINOR,
	SCALE_DORIAN,
	SCALE_BLUES,
	SCALE_PENTA,
	SCALE_12TONE,
	SCALE_MAJORTRIAD,
	SCALE_MINORTRIAD,

	SCALE_EGYPTIAN,
	SCALE_PHRYGIAN,
	SCALE_LOCRIAN,
	SCALE_OCTATONIC,
	SCALE_MELODICMINOR,
	SCALE_SCALE_MINORPENTA,
	SCALE_15,
	SCALE_16,
#else

	SCALE_MINOR,
	SCALE_MELODICMINOR,
	SCALE_MINORPENTA,
	SCALE_MINORTRIAD,
	SCALE_PHRYGIAN,
	SCALE_LOCRIAN,
	SCALE_OCTATONIC,
	SCALE_12TONE,


	SCALE_MAJOR,
	SCALE_DORIAN,
	SCALE_PENTA,
	SCALE_EGYPTIAN,
	SCALE_BLUES,
	SCALE_MAJORTRIAD,
	SCALE_15,
	SCALE_16,
#endif
	__SCALE_COUNT
} TUESDAY_SCALES;


typedef enum
{
	OCTAVELIMIT_OFF,
	OCTAVELIMIT_1,
	OCTAVELIMIT_2,
	OCTAVELIMIT_3,
} TUESDAY_OCTAVELIMIT;

typedef enum
{

	CLOCKMODE_RESETREGULAR = 0,
	CLOCKMODE_RESETINVERTED = 1,

	CLOCKMODE_RESET_BLOCKS_TICKS = 2,
	CLOCKMODE_RESET_TRIGGER_ONLY = 0,

	CLOCKMODE_DOWNSLOPE= 4,
	CLOCKMODE_UPSLOPE= 0

} TUESDAY_RESETFLAGS;


typedef struct Tuesday_Scale
{
	uint8_t notes[12];
	uint8_t count;
} Tuesday_Scale;

typedef struct Tuesday_Settings
{
	uint32_t RandomSeed;

	uint8_t tpboptions[TUESDAY_MAXTPB];
	uint8_t beatoptions[TUESDAY_MAXBEAT];
	uint8_t scale[TUESDAY_MAXSCALE];
	uint8_t algooptions[TUESDAY_MAXALGO];

	struct Tuesday_Scale scales[__SCALE_COUNT];

	uint8_t ClockSubDivMode;
	uint8_t OctaveLimiter;
	uint8_t ClockMode;
} Tuesday_Settings;

typedef struct Tuesday_Params
{
	// buttons
	uint8_t tpbopt;
	uint8_t beatopt;
	uint8_t scale;
	uint8_t algo;
} Tuesday_Params;

typedef enum{
	UI_STARTUP,
	UI_NORMAL,
	UI_CALIBRATION,
	UI_SELECTOPTION,
	UI_GLOBALSETTINGS,
	__TUESDAY_UIMODE_COUNT
} TUESDAY_UIMODE;

typedef enum{
	CALIBRATION_NOTARGET,
	CALIBRATION_VEL,
	CALIBRATION_NOTE,
	__TUESDAY_CALIBRATION_SETTING_COUNT
} TUESDAY_CALIBRATION_SETTING;

typedef enum{
	OPTION_ALGO,
	OPTION_SCALE,
	OPTION_BEATS,
	OPTION_TPB,
	__TUESDAY_OPTION_SETTING_COUNT
} TUESDAY_OPTION_SETTING;

typedef enum
{
	CLOCKSUBDIV_4, 
	CLOCKSUBDIV_8, 
	CLOCKSUBDIV_16,
	CLOCKSUBDIV_24PPQN
} TUESDAY_CLOCKSUBDIVISION_SETTING;

#define EEPROM_OPTIONBASE 4
#define EEPROM_SETTINGSBASE 256
#define EEPROM_CALIBRATIONBASE 512


#ifdef __cplusplus
extern "C"
{
#endif
	extern int CalibrateAdjust(int input);
	extern void Tuesday_Init(Tuesday_PatternGen *T);
	extern void Tuesday_Clock(Tuesday_PatternGen *P, Tuesday_Settings *S, Tuesday_Params *Par, int ClockVal);
	extern void Tuesday_ExtClock(Tuesday_PatternGen *P,Tuesday_Params *Params, Tuesday_Settings *S, int state);
	extern void Tuesday_Reset(Tuesday_PatternGen *T, Tuesday_Settings *s, int Val);
	extern void Tuesday_Tick(Tuesday_PatternGen *T, Tuesday_Params *P);
	extern void Tuesday_TimerTick(Tuesday_PatternGen *T, Tuesday_Params *P);
	extern void Tuesday_ValidateParams(Tuesday_Params *P);
	extern void Tuesday_MainLoop(Tuesday_PatternGen *T,Tuesday_Settings *settings, Tuesday_Params *params);
	extern void Tuesday_SwitchToOptionMode(Tuesday_PatternGen *T,int mode, int startoption);
	extern void Tuesday_SetupLeds(Tuesday_PatternGen *T, Tuesday_Settings *settings, Tuesday_Params *params);
	extern void Tuesday_ValidateSettings(Tuesday_Settings *S);
	extern void Tuesday_LoadSettings(Tuesday_Settings *S, Tuesday_Params *P);
	extern void Tuesday_LoadDefaults(Tuesday_Settings *S, Tuesday_Params *P);
	extern void Tuesday_Generate(Tuesday_PatternGen *T, Tuesday_Params *P, Tuesday_Settings *S);
	extern void Tuesday_RandomSeed(Tuesday_RandomGen *R, unsigned int seed);
	extern void Tuesday_SetupClockSubdivision(Tuesday_PatternGen *P, Tuesday_Settings *S, Tuesday_Params *Par);

	extern void Tuesday_Goa(Tuesday_PatternContainer *T, Tuesday_RandomGen *R, int Length);
	extern void Tuesday_Flat(Tuesday_PatternContainer *T, Tuesday_RandomGen *R, int Length);
	extern void Tuesday_Psych(Tuesday_PatternContainer *T, Tuesday_RandomGen *R, int Length);
	extern void Tuesday_Zeph(Tuesday_PatternContainer *T, Tuesday_RandomGen *R, int stepsperbeat, int beats, int fullcycles);

	extern void DoClock(int state);
	extern void doTick();
	extern void Tuesday_ToggleSlope(Tuesday_Settings *Settings);
	extern void Tuesday_ToggleReset(Tuesday_Settings *Settings);
	extern void Tuesday_ToggleResetPattern(Tuesday_Settings *Settings);
	

#ifdef __cplusplus
}
#endif
