#include "../MachineInterface.h"
#include "../../BigFishLib/BigFish.h"

#include "../../BigFishLib/BigFish.cpp"
#include "../../BigFishLib/ADSREnvelope.cpp"
#include "../../BigFishLib/MinBlepGen.cpp" 
#include "../../BigFishLib/BleppyOscs.cpp" 


#define _USE_MATH_DEFINES

#include <Windows.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define MAXPARAMVALUE 127

class InertiaBlock
{
public:
	InertiaBlock()
	{
		Left = Current = Delta = 0;
	}
	int Left;
	float Current;
	float Delta;
	float Get()
	{
		if (Left > 0)
		{
			Left--;
			Current += Delta;
		}
		return Current;
	}
	void SetTarget(float Target)
	{
		
		Delta = (Target - Current) / 50;
		Left = 50;

	}
};

class BigFish
{
public :
	BigFish_t Fish;
	BigFish(int samplerate)
	{

		BigFish_Init(&Fish, samplerate);


	}
	

	
	void Process(float *psamples, int numsamples)
	{
		int32_t blockOSC[MAXFISHBUFFER];
		int32_t blockMAIN[MAXFISHBUFFER];
		while (numsamples)
		{
			int L = __min(numsamples, MAXFISHBUFFER);
			UpdateParams();
			BigFish_GenerateBlock(&Fish, blockOSC, blockMAIN, L);
			for (int i = 0; i < L; i++)
			{
				*psamples++ = blockMAIN[i] *1.0f / (float)(1<<2);
			}
			numsamples -= L;
		}
	}
	void UpdateParams()
	{
		for (int i = 0; i < __PARAMCOUNT; i++)
		{
			if (!IgnoreParam(i)) Fish.Parameters[i] = Inertia[i].Get();
		}
	}
	bool IgnoreParam(int i)
	{
		switch (i)
		{
		case FILTER_KEYTRACK:
		case FILTER_TYPE: 
			return true;
		}
		return false;
	}
	void SetParam(int p, int v)
	{
		Inertia[p].SetTarget((v * 65535) / MAXPARAMVALUE);
		
	}
	
	InertiaBlock Inertia[__PARAMCOUNT];

	void SetGate(int g, int v)
	{
		Fish.Gates[g] = v;
	}

	void CheckGates()
	{
		BigFish_CheckGates(&Fish);
	}

	void SetNote(int N)
	{
		int32_t pitchtarget = ((N-24) * (1 << 14)) / 12;
		Fish.PitchInput = pitchtarget;
	}
};


inline double DBToAmplitude(double const a)
{
	return pow(10, (a * 0.05));
}

inline double ExpIntp(double const x, double const y, double const a)
{
	assert((x > 0 && y > 0) || (x < 0 && y < 0));
	double const lx = log(x);
	double const ly = log(y);
	return exp(lx + a * (ly - lx));
}

CMachineParameter const paraPITCH_FINE = { pt_byte, "Tune", "Tune", 0,	127, 255, MPF_STATE, 64 };
CMachineParameter const paraPITCH_COARSE = { pt_byte, "Transpose", "Transpose", 0,	127, 255, MPF_STATE, 64 };
CMachineParameter const paraPITCH_CHORD = { pt_byte, "Chord", "Chord", 0,	127, 255, MPF_STATE, 0 };
CMachineParameter const paraOSC_SHAPE = { pt_byte, "Shape", "Shape", 0,	127, 255, MPF_STATE, 0 };
CMachineParameter const paraOSC_SIZE = { pt_byte, "Size", "Size", 0,	127, 255, MPF_STATE, 0 };
CMachineParameter const paraOSC_SPREAD = { pt_byte, "Spread", "Spread", 0,	127, 255, MPF_STATE, 0 };
CMachineParameter const paraAMP_ATTACK = { pt_byte, "Attack", "Attack", 0,	127, 255, MPF_STATE, 10 };
CMachineParameter const paraAMP_DECAY = { pt_byte, "Decay", "Decay", 0,	127, 255, MPF_STATE, 10 };
CMachineParameter const paraAMP_SUSTAIN = { pt_byte, "Sustain", "Sustain", 0,	127, 255, MPF_STATE, 64 };
CMachineParameter const paraAMP_RELEASE = { pt_byte, "Release", "Releae", 0,	127, 255, MPF_STATE, 64 };
CMachineParameter const paraFILTER_TYPE= { pt_byte, "Filter Type", "Filter Type", 0,	4, 255, MPF_STATE, 0 };
CMachineParameter const paraFILTER_CUTOFF = { pt_byte, "Cutoff", "Cutoff", 0,	127, 255, MPF_STATE, 80 };
CMachineParameter const paraFILTER_RESONANCE = { pt_byte, "Resonance", "Resonance", 0,	127, 255, MPF_STATE, 20 };
CMachineParameter const paraFILTER_PEAKS = { pt_byte, "Peaks", "Peaks", 0,	127, 255, MPF_STATE, 0 };
CMachineParameter const paraFILTER_DRIFT = { pt_byte, "Drift", "Drift", 0,	127, 255, MPF_STATE, 0 };
CMachineParameter const paraFILTER_KEYTRACK = { pt_byte, "Keytrack", "Keytrack", 0,	__FILTERKEYTRACK_COUNT-1, 255, MPF_STATE, FILTERKEYTRACK_OFF };
CMachineParameter const paraFILTER_ENVELOPE = { pt_byte, "Filter Envelope", "Filter Envelope", 0,	127, 255, MPF_STATE, 90 };
CMachineParameter const paraFILTER_DRIVE = { pt_byte, "Drive", "Drive", 0,	127, 255, MPF_STATE, 90 };
CMachineParameter const paraFILTER_ATTACK = { pt_byte, "Filter Attack", "Filter Attack", 0,	127, 255, MPF_STATE, 0 };
CMachineParameter const paraFILTER_DECAY = { pt_byte, "Filter Decay", "Filter Decay", 0,	127, 255, MPF_STATE, 40 };

CMachineParameter const paraFILTER_ACCENT = { pt_byte, "Accent Amount", "Accent Amount", 0,	127, 255, MPF_STATE, 80 };

CMachineParameter const paraAMP_VELOCITY = { pt_byte, "Velocity", "Velocity", 0,	127, 255, MPF_STATE, 0 };

CMachineParameter const paraACCENT= { pt_switch, "Accent", "Accent", SWITCH_OFF,	SWITCH_ON, SWITCH_NO, 0, SWITCH_OFF };
CMachineParameter const paraNOTE = { pt_note, "Note", "Note", NOTE_MIN,	NOTE_MAX, NOTE_NO, MPF_TICK_ON_EDIT,  0 };

CMachineParameter const *pParameters[] =
{ 
	&paraNOTE,
	&paraACCENT,
	
	&paraPITCH_FINE,
	&paraPITCH_COARSE,
	&paraPITCH_CHORD,
	&paraOSC_SHAPE,
	&paraOSC_SIZE,
	&paraOSC_SPREAD,
	&paraAMP_ATTACK,
	&paraAMP_DECAY,
	&paraAMP_SUSTAIN,
	&paraAMP_RELEASE,
	&paraFILTER_TYPE,
	&paraFILTER_CUTOFF,
	&paraFILTER_RESONANCE,
	&paraFILTER_PEAKS,
	&paraFILTER_DRIFT,
	&paraFILTER_KEYTRACK,
	&paraFILTER_ENVELOPE,
	&paraFILTER_DRIVE,
	&paraFILTER_ATTACK,
	&paraFILTER_DECAY,
	&paraFILTER_ACCENT,
	&paraAMP_VELOCITY
};


#pragma pack(1)		

class gvals
{
public:
	byte note;
	byte accent;
	byte params[__PARAMCOUNT];
};

#pragma pack()

CMachineInfo const MacInfo =
{
	MT_GENERATOR,		// type
	MI_VERSION,	
	0,			// flags		
	0,										// min tracks
	0,										// max tracks
	__PARAMCOUNT + 2,										// numGlobalParameters
	0,										// numTrackParameters
	pParameters,
	0,
	NULL,
#ifdef _DEBUG
	"Zephod BigFish (Debug build)",		// name
#else
	"Zephod BigFish",					// name
#endif
	"BigFish",									// short name
	"Stijn Kuipers",						// author
	NULL
};


class mi : public CMachineInterface
{
public:
	mi();
	virtual ~mi();

	virtual void Init(CMachineDataInput * const pi);
	virtual void Tick();
	virtual bool Work(float *psamples, int numsamples, int const mode);
	virtual char const *DescribeValue(int const param, int const value);
	virtual void Stop();

public:
	BigFish *Fish;

	float lastmingain;
	bool IdleMode;
	int IdleCount;
	gvals gval;

};


DLL_EXPORTS

mi::mi()
{
	GlobalVals = &gval;
	Fish = NULL;
	AttrVals = NULL;
}

mi::~mi()
{
	if (Fish) delete Fish;
};


void mi::Init(CMachineDataInput * const pi)
{
	if (Fish) delete Fish;
	Fish = new BigFish(pMasterInfo->SamplesPerSec);

	for (int i = 0; i < __PARAMCOUNT; i++)
	{
		//Fish->SetParam(i Inertia[i].SetTarget(gval.params[i])
	}

};


void DescribeMidKnob(int const value, char *txt)
{
	if (value<63)
	{
		float V = (63 - value) / 0.63f;
		sprintf_s(txt, 16, "%.1f%%", -V);
	}
	else
	{
		float V = (value - 63) / ((127 - 63)*0.01f);
		sprintf_s(txt, 16, "%.1f%%", V);
	}
}

void DescribeKnob(int const value, char *txt) 
{

	sprintf_s(txt, 16, "%.1f%%", value*100.0f/127.0f);
}
char const *mi::DescribeValue(int const param, int const value)
{
	static char txt[16];
	switch (param -2 )
	{


	case PITCH_COARSE:
		sprintf_s(txt, 16, "%d semitones", (int)((value - 63.0f)*48/ 127.0f));
		break;

	case PITCH_FINE:
		sprintf_s(txt, 16, "%.1f semitone", (value - 63.0f)*2.0f / 127.0f);
		break;
	
	case FILTER_DRIVE:
	{
		if (value == 63)
		{
			sprintf_s(txt, 16, "OFF");
		}
		else
		{

			if (value<63 )
			{
				float V = (63 - value) / 0.63f;
				sprintf_s(txt, 16, "%.1f%% Fuzz", V);
			}
			else
			{
				float V = (value-63) / ((127-63)*0.01f);
				sprintf_s(txt, 16, "%.1f%% Clean", V);
			}
		}
	}
		break;
	case FILTER_ENVELOPE:
		if (value == 63)
		{
			sprintf_s(txt, 16, "OFF");
		}
		else
		{
			DescribeMidKnob(value, txt);
		}
			break;

	case PITCH_CHORD:
	case OSC_SHAPE:
	default:
		sprintf_s(txt, 16, "%d", value);
		break;

	case FILTER_KEYTRACK:

		switch (value)
		{
		case FILTERKEYTRACK_NEG2: sprintf_s(txt, 16, "-2.0"); break;
		case FILTERKEYTRACK_NEG1HALF: sprintf_s(txt, 16, "-1.5"); break;
		case FILTERKEYTRACK_NEG1: sprintf_s(txt, 16, "-1.0"); break;
		case FILTERKEYTRACK_NEGHALF: sprintf_s(txt, 16, "-0.5"); break;
		case FILTERKEYTRACK_OFF: sprintf_s(txt, 16, "OFF"); break;
		case FILTERKEYTRACK_POSHALF: sprintf_s(txt, 16, "+0.5"); break;
		case FILTERKEYTRACK_POS1: sprintf_s(txt, 16, "+1.0"); break;
		case FILTERKEYTRACK_POS1HALF: sprintf_s(txt, 16, "+1.5"); break;
		case FILTERKEYTRACK_POS2: sprintf_s(txt, 16, "+2.0"); break;
		};
		break;

		break;

	case OSC_SIZE:
	case OSC_SPREAD:
	case AMP_ATTACK:
	case AMP_DECAY:
	case AMP_SUSTAIN:
	case AMP_RELEASE:
	case FILTER_CUTOFF:
	case FILTER_RESONANCE:
	case FILTER_PEAKS:
	case FILTER_DRIFT:
	
	case FILTER_ATTACK:
	case FILTER_DECAY:
	case FILTER_ACCENT:

		DescribeKnob(value, txt);
		break;

	case FILTER_TYPE:
		switch (value)
		{
		case FILTERTYPE_LP: sprintf_s(txt, 16, "Lowpass"); break;
		case FILTERTYPE_HP: sprintf_s(txt, 16, "Highpass"); break;
		case FILTERTYPE_VOCAL: sprintf_s(txt, 16, "Vocal"); break;
		case FILTERTYPE_BP: sprintf_s(txt, 16, "Bandpass"); break;
		case FILTERTYPE_BR: sprintf_s(txt, 16, "Bandreject"); break;
		};
		break;
	}

	return txt;
}

void mi::Tick()
{
	for (int i = 0; i < __PARAMCOUNT; i++)
	{
		if (gval.params[i] != 255)
		{
			switch (i)
			{
			case FILTER_TYPE:
			{
				int F = gval.params[i];
				int filtermode = (((int32_t)F) * 65536) / (__FILTERTYPE_COUNT-1 );
				Fish->Fish.Parameters[FILTER_TYPE] = filtermode;
				int rev = (((int32_t)Fish->Fish.Parameters[FILTER_TYPE]) * (__FILTERTYPE_COUNT-1 )) / 65536;
				printf("%d", rev);

			}
				break;
			case FILTER_KEYTRACK:
				Fish->Fish.Parameters[FILTER_KEYTRACK] =  (gval.params[i] * 65536) / (__FILTERKEYTRACK_COUNT-1);
				break;
			default:
				Fish->SetParam(i, gval.params[i]);
				break;
			}
		}
	}

	if (gval.note == NOTE_OFF)
	{
		Fish->SetGate(FISHGATE_GATE, 0);
	}
	else if (gval.note != NOTE_NO)	
	{
		Fish->SetGate(FISHGATE_GATE, 0);
		Fish->CheckGates();
		Fish->SetGate(FISHGATE_GATE, 1);
		Fish->SetNote((gval.note >> 4) * 12 + (gval.note & 0xf));
	}
	
}

void mi::Stop()
{
	Fish->SetGate(FISHGATE_GATE, 0);
}


bool mi::Work(float *psamples, int numsamples, int const mode)
{
	Fish->Process(psamples, numsamples);

	return true;
}
