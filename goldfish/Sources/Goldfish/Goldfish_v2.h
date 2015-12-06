#ifndef GOLDFISH
#define GOLDFISH


#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

typedef unsigned char byte;

#define SWITCH_ON 1
#define SWITCH_OFF 2
#define SWITCH_NO 0
#define WM_WRITE 1
#define WM_READ 2
#define WM_NOIO 3

enum
{
	MPF_STATE
};
enum
{
	pt_switch,pt_byte
};

struct CMachineParameter
{
	byte type;
	const char *name;
	const char *longname;
	int min;
	int max;
	int no;
	int def;
	int flags;
};

class Parameter
{
public:
	Parameter(){
		MinValue = 0;
		MaxValue = 127;
		DefValue = 64;
		CurValue = 64;
	}
	char *Name;

	char *Description;
	unsigned char Type;
	unsigned char  MinValue;
	unsigned char MaxValue;
	unsigned char DefValue;
	unsigned char CurValue;
};


#include "GoldfishEnvelopes.h"
#include "PlatinumClip.hpp"
class TickInterface
{
public:
	virtual void Clear() = 0;

};
class SynthInterface
{
public:
	PlatinumClip *m_Dist;
	virtual void Init() {};

	virtual void	Event(WORD EventType, int Parm1, int Parm2, int Parm3 = 0, int Parm4 = 0) {};
	virtual bool	Render(INTERNAL_RES* pSamples, int nSamples, PlatinumClip* dist) { return false; };
	virtual bool	RenderAdd(INTERNAL_RES* pSamples, int nSamples, PlatinumClip* dist) { return false; };

	Parameter ParameterList[PARAMCOUNTDEF];



};

#ifdef GOLDFISHSYNTH




//#ifndef OVERSAMPLING
//#define OVERSAMPLING 1
//#endif

#define HI16(x) (x>>16)
#define LO16(x) (x&65535)




typedef struct 
{
	int32_t lo;
	int32_t mid;
	int32_t hi;
} chan;

class P303: public SynthInterface
{

public:
	P303();
	 ~P303();
	 virtual void Init();
	 
	virtual void	Event(WORD EventType, int Parm1, int Parm2, int Parm3=0, int Parm4=0);
	virtual bool	Render(INTERNAL_RES* pSamples, int nSamples, PlatinumClip* dist);
	virtual bool	RenderAdd(INTERNAL_RES* pSamples, int nSamples, PlatinumClip* dist );

#define DELAYLEN (44100/5)	
	int16_t delay[1][DELAYLEN];
	byte delaymode;
	int32_t *filtres;
	uint32_t writepos;

	int Transpose;

	void DelayProcess(INTERNAL_RES *pSamples, int nSamples);
	unsigned long FXSend;
	uint16_t FXParam;

	uint32_t ChorusPhase;


	uint32_t CalcSample();
	
	uint32_t CalcSample3();
	uint32_t CalcSample4();
	unsigned long AccentAMT;
	bool accented;
	inline void ParamUpdate();

	bool playing;
//	unsigned long NoteTable[128];

	unsigned char Note;

	unsigned long dP;

	unsigned long dP1;
	unsigned long P1;
	unsigned long dP2;
	unsigned long P2;
	unsigned long dP3;
	unsigned long P3;
	unsigned long dP4;
	unsigned long P4;
	//int activechordnotes;
	int	activechord;
	signed long ddP;
	unsigned long pinertiacount;
	unsigned long Glide;

	chan filt;
	
	unsigned long C;
	signed long dC;
	unsigned short R;
	unsigned short sR;
	unsigned short sC;
	
	unsigned long finertiacount;

//	unsigned long Cmapping[128];
//	unsigned long DecayTime[128];
	
// filter env stuff
	unsigned long FEnvCurVal;
	int FEnvDVal;
	unsigned long FEnvStateTime;
	unsigned char FEnvState;
	unsigned long FEnvDecay;

	unsigned long AEnvCurVal;
	int AEnvDVal;
	unsigned long AEnvStateTime;
	unsigned char AEnvState;
	// ampenv stuff

	const signed short *Tab;
	//	signed long longsaw[2048];
	//signed long longpulse[2048];

	//Parameter *m_pParameterList[__GoldFishParamCount];

	unsigned long samplecount;
	signed short FenvAmt;


	uint32_t  DistPreMult;
	uint32_t DistPostMult;

	inline void FEnvStart()
	{
		FEnvStateTime=FEnvDecay;
		FEnvCurVal=0xffffffff;
		FEnvDVal=FEnvCurVal/FEnvStateTime;
		FEnvDVal=-FEnvDVal;
		FEnvState=EF_DECAY;
	}
	inline void FEnvReStart()
	{
		
		FEnvStateTime=FILTER_REATTACKTIME;
		FEnvDVal=(~FEnvCurVal)/FEnvStateTime;
		FEnvState=EF_ATTACK;
	}

	inline void FEnvReset()
	{
		FEnvState=EA_STOP;
		FEnvCurVal=0;
		FEnvDVal=0;
	}

	
	inline unsigned short FEnvGet()
	{
		FEnvCurVal+=FEnvDVal;
		if (FEnvState) FEnvStateTime--;
		if (!FEnvStateTime)
		{
			FEnvState++;
			switch(FEnvState)
			{
			case EF_DECAY+1:
				{
					if (FEnvCurVal>65535)
					{
						FEnvState=EF_DECAY;
						FEnvStateTime=EXTRA_RELEASE;
						FEnvDVal= FEnvCurVal/FEnvStateTime;
						break;
					}
					else
					{
						FEnvStateTime=2;
						FEnvState=EA_STOP;
						FEnvCurVal=0;
						FEnvDVal=0;
					}
				} break;
			case EF_DECAY:
				{
					FEnvStateTime=FEnvDecay;
					FEnvDVal=FEnvCurVal/FEnvStateTime;
					FEnvDVal=-FEnvDVal;
				}break;
			default:
				{
					FEnvStateTime=2;
					FEnvState=EA_STOP;
					FEnvCurVal=0;
					FEnvDVal=0;
				}
			}
		}
		return ((short*)&FEnvCurVal)[1];
	}


// filter env stuff

// ampenv stuff

	inline void AEnvStart()
	{
		AEnvStateTime=AMP_ATTACKTIME;
		AEnvDVal=0xffffffff/AEnvStateTime;
		AEnvCurVal=0;
		AEnvState=1;
	}

	inline void AEnvReStart()
	{
		
		AEnvStateTime=AMP_REATTACKTIME;
		AEnvDVal=(~AEnvCurVal)/AEnvStateTime;
		AEnvState=1;
	}

	inline void AEnvStop()
	{
		if (AEnvState<3 && AEnvState>0)
		{
			AEnvStateTime=AMP_RELEASETIME;
			AEnvDVal=AEnvCurVal/AEnvStateTime;
			AEnvDVal=-AEnvDVal;
			AEnvState=3;
		}
	}

	inline void AEnvReset()
	{
		AEnvState=0;
		AEnvCurVal=0;
		AEnvDVal=0;
	}

	inline unsigned short AEnvGet()
	{
		AEnvCurVal+=AEnvDVal;
		if (AEnvState & 1) AEnvStateTime--;
		if (!AEnvStateTime)
		{
			AEnvState++;
			switch(AEnvState)
			{
			case 4:
				{
					if (AEnvCurVal>65535)
					{
						AEnvState=3;
						AEnvStateTime=EXTRA_RELEASE;
						AEnvDVal= AEnvCurVal/AEnvStateTime;
						break;
					}
					else
					{
						AEnvStateTime=2;
						AEnvState=0;
						AEnvCurVal=0;
						AEnvDVal=0;
					}
				}
			case 2:
				{
					AEnvDVal=0;
					AEnvStateTime=1;
				}break;
			default:
				{
					AEnvStateTime=2;
					AEnvState=0;
					AEnvCurVal=0;
					AEnvDVal=0;
				}
			}
		}
		return ((short*)&AEnvCurVal)[1];
	}


};

#endif

#ifdef DRUMSTICK

#include "DrumStick.h"

#endif

#ifdef KRZYNTH
#include "krzynth.h"
#endif


typedef struct
{
	unsigned char ParamValues[PARAMCOUNTDEF];
	char Name[NAMELENGTH];
} GoldFishPreset; // 6 + 10 = 16 bytes!


class GoldFishTick: public TickInterface
{
public:

	unsigned char Note;
	unsigned char SlideAccentVel;
	static void Render(GoldFishTick &tickval, int y);
	virtual void Clear(){ Note = 0x80 + (60-24); SlideAccentVel = 0; }
} ;


class KRZynthTick: public TickInterface
{
public:
	unsigned char Note;
	unsigned char SlideAccentVel;
	static void Render(KRZynthTick &tickval, int y);
	virtual void Clear(){ Note = 0; SlideAccentVel = 0; }

};



class GoldFishPattern
{
public:
	TICKTYPE Ticks[MAXSTEPPERPATTERN];
	unsigned char TickCountTimeSig;
	char Name[NAMELENGTH];
}; // 64 + 64 +	1 = 65 bytes!

enum
{
	TimeMode_Internal,
	TimeMode_External
};

class GoldFishSettings 
{
public:
	GoldFishSettings()
	{
		 midichannel = 4;
		 calibw = 240;
		 calibh = 320;
		 calibxmin = 0;
		 calibymin = 0;

		 SubticksPerTick = 6;
		 BPM = 125;
		 TimeMode = TimeMode_Internal;
		 TimeBits = 0;
		 Playmode = 0;
		 HeadphoneVolume = 127;
		 MidiRouting = 0;
		 CurrentPresetSet = 0;
		 CurrentPatternSet = 0;
		 for (int i = 0; i < MAXFISH; i++)
		 {
			 CurrentPattern[i] = 0;
			 CurrentPreset[i] = 0;
		 }
		 Transpose = 0;
	}

	unsigned char TimeBits;
	unsigned char MidiRouting;
	unsigned char HeadphoneVolume;
	unsigned char Playmode;
	unsigned int BPM;
	
	uint32_t CurrentPresetSet;
	uint32_t CurrentPatternSet;

	unsigned char CurrentPattern[MAXFISH];
	unsigned char CurrentPreset[MAXFISH];
	
	uint32_t calibw ;
	uint32_t calibh ;
	uint32_t calibxmin;
	uint32_t calibymin;

	int SubticksPerTick;

	int TimeMode ;

	int Transpose;

	char PatternNames[MAXSET][NAMELENGTH];
	char PresetNames[MAXSET][NAMELENGTH];

	unsigned char midichannel;

} __attribute__((packed)) ;





#endif

