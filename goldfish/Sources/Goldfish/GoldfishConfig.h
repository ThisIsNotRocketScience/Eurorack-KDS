#ifndef GOLDFISHCONFIG
#define GOLDFISHCONFIG

//#define SWAPRGB


//#define FISHSTICKCS4334
//#define GOLDFISHR3
//#define GOLDFISHR4
#define GOLDFISHR3STICK

#define GOLDFISHSYNTH
//#define DRUMSTICK
//#define KRZYNTH


#define MAXSTEPPERSEQUENCE 16 // -> 16 x 16 ticks or 16 x 24 ticks
#define MAXSTEPPERPATTERN 96 // 4 x 24 tick triplet patterns!
#define MAXPRESET 4 // loaded in ram
#define MAXPATTERN 4
#define MAXSEQUENCE 2

// available for load on disk:
#define MAXSET 16
#define NAMELENGTH 10

#ifdef GOLDFISHSYNTH
#define FILEPREFIX "gf_"
#define FISHTYPE P303
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
#ifdef DRUMSTICK
#define FILEPREFIX "ds_"
#define FISHTYPE P909
#define PARAMCOUNTDEF __DrumStickParamCount
#define TICKTYPE DrumStickTick
#define PATTERNTYPE DrumStickPattern

enum
{
	ALL_NOTES_OFF,
	RESET,
	NOTE_ON,
	NOTE_OFF,
	KICKVOL = 30,
	KICKBASENOTE,
	KICKDECAY,
	KICKPITCHDECAY,
	KICKFUZZ,
	KICKPITCHUP,
	SNAREDECAY,
	OPENHIHATDECAY,
	CLOSEDHIHATDECAY,
	PUSH,
	PUSHTYPE
};

enum
{
	Param_KickVolume,
	Param_KickBaseNote,
	Param_KickDecay,
	Param_KickPitchDecay,
	Param_KickFuzz,
	Param_KickPitchUp,
	Param_SnareDecay,
	Param_OpenHihatDecay,
	Param_ClosedHihatDecay,


	Param_PUSH,
	Param_PUSHTYPE,
	__DrumStickParamCount

};

enum
{
	Kick,
	Snare,
	Clap,
	OpenHihat,
	ClosedHihat,
	Crash, 
	DRUMTYPES
};


#endif

#ifdef KRZYNTH
#define FILEPREFIX "kr_"
#define FISHTYPE KRZynth
#define PARAMCOUNTDEF __KRZynthParamCount
#define TICKTYPE KRZynthTick
#define PATTERNTYPE KRZynthPattern

#define HASNOTE

typedef enum
{
	Param_AMPATTACK,
	Param_AMPDECAY,
	Param_AMPSUSTAIN,
	Param_AMPRELEASE,

	Param_FLTATTACK,
	Param_FLTDECAY,
	Param_FLTSUSTAIN,
	Param_FLTRELEASE,

	Param_DETUNE,
	Param_DETUNEAMP,
	Param_SHAPE,
	Param_CUTOFF,
	Param_ENVMOD,
	Param_RESONANCE,
	Param_PUSH,
	Param_PUSHTYPE,
	//	Param_ACCENT,
	//	Param_GLIDE,

	__KRZynthParamCount

} ParamID;

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
	ACCENT
};
#endif

#define MAXFISH 1


#if defined(GOLDFISHR3) || defined(GOLDFISHR3STICK)
	#define FISHEYE 11
#endif

#ifdef FISHSTICKCS4334
#define FISHEYE 15
#endif

#ifdef GOLDFISHR3
#define FISHDISPLAY
#define WM8731
#endif

#ifdef GOLDFISHR4
#define FISHEYE 6
#define FISHDISPLAY
#endif

#if defined(GOLDFISHR3) || defined(GOLDFISHR3STICK)
	#define WM8731
#endif

//#define OVERSAMPLING 1

#define USEMIDISERIAL
#define INTERNAL_RES signed short
#define WORD unsigned short
#define CONTROLRES 64
#define CONTROLBITS 6 


#ifdef FISHDISPLAY
#define USETOUCH
#define HWWIDTH 240
#define HWHEIGHT 320
#endif

#define SETTINGSFILE FILEPREFIX "set.bin"
//#define SUBTICKRES 6

#endif
