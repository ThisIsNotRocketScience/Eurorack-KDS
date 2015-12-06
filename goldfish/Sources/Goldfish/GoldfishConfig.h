#ifndef GOLDFISHCONFIG
#define GOLDFISHCONFIG


#define GOLDFISHSYNTH



#define MAXSTEPPERSEQUENCE 16 // -> 16 x 16 ticks or 16 x 24 ticks
#define MAXSTEPPERPATTERN 96 // 4 x 24 tick triplet patterns!
#define MAXPRESET 4 // loaded in ram
#define MAXPATTERN 4
#define MAXSEQUENCE 2


#ifdef GOLDFISHSYNTH
#define FILEPREFIX "gf_"
#define FISHTYPE Goldfish
#define PARAMCOUNTDEF __GoldFishParamCount
#define TICKTYPE GoldFishTick
#define PATTERNTYPE GoldFishPattern

#define HASNOTE
#define HASACCENTSLIDE

enum
{
	ALL_NOTES_OFF,
	RESET,
	NOTE_ON,
	NOTE_OFF,
	WAVEFORM = 30,
	GLIDE,
	CUTOFF,
	RESONANCE,
	FENV,
	FDEC,
	PUSH,
	PUSHTYPE,
	FXSEND,
	FXPARAM,
	FXTYPE,
	ACCENT,
	FILTERTYPE,
	CHORD
};


typedef enum
{
	Param_WAVEFORM,
	Param_GLIDE,
	Param_CUTOFF,
	Param_RESONANCE,
	Param_FENV,
	Param_FDEC,
	Param_PUSH,
	Param_PUSHTYPE,
	Param_FXSEND,
	Param_FXPARAM,
	Param_FXTYPE,
	Param_ACCENT,
	Param_FILTERTYPE,
	Param_CHORD,
	__GoldFishParamCount
} ParamID;

#endif

#define MAXFISH 1



//#define OVERSAMPLING 1

#define INTERNAL_RES signed short
#define WORD unsigned short
#define CONTROLRES 64
#define CONTROLBITS 6 

#endif
