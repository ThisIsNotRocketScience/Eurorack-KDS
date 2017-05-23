#include "../MachineInterface.h"
#include "../../BigFishLib/BigFish.h"

#include "../../BigFishLib/BigFish.cpp"
#include "../../BigFishLib/ADSREnvelope.cpp"
#include "../../BigFishLib/MinBlepGen.cpp" 
#include "../../BigFishLib/BleppyOscs.cpp" 
#include "../../BigFishLib/OrganOsc.cpp" 


#define _USE_MATH_DEFINES

#include <Windows.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <tuple>


#define MAXPARAMVALUE 0xfffe

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
		case FILTER_TYPE: 
			return true;
		}
		return false;
	}
	void SetParam(int p, int v)
	{
		Inertia[p].SetTarget(v );
		
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

CMachineParameter const paraPITCH_FINE = { pt_word, "Tune", "Tune", 0,	0xfffe, 0xffff, MPF_STATE, 0xfffe/2 };
CMachineParameter const paraPITCH_COARSE = { pt_word, "Transpose", "Transpose", 0,	0xfffe, 0xffff, MPF_STATE, 0xfffe / 2 };
CMachineParameter const paraPITCH_CHORD = { pt_word, "Chord", "Chord", 0,	0xfffe, 0xffff, MPF_STATE, 0 };
CMachineParameter const paraOSC_SHAPE = { pt_word, "Shape", "Shape", 0,	0xfffe, 0xffff, MPF_STATE, 0 };
CMachineParameter const paraOSC_SIZE = { pt_word, "Size", "Size", 0,	0xfffe, 0xffff, MPF_STATE, 0 };
CMachineParameter const paraOSC_SPREAD = { pt_word, "Spread", "Spread", 0,	0xfffe, 0xffff, MPF_STATE, 0 };
CMachineParameter const paraAMP_ATTACK = { pt_word, "Attack", "Attack", 0,	0xfffe, 0xffff, MPF_STATE, 10<<9 };
CMachineParameter const paraAMP_DECAY = { pt_word, "Decay", "Decay", 0,	0xfffe, 0xffff, MPF_STATE, 10<<9 };
CMachineParameter const paraAMP_SUSTAIN = { pt_word, "Sustain", "Sustain", 0,	0xfffe, 0xffff, MPF_STATE, 64<<9 };
CMachineParameter const paraAMP_RELEASE = { pt_word, "Release", "Releae", 0,	0xfffe, 0xffff, MPF_STATE, 64<<9 };
CMachineParameter const paraFILTER_TYPE= { pt_word, "Filter Type", "Filter Type", 0,	4, 0xffff, MPF_STATE, 0 };
CMachineParameter const paraFILTER_CUTOFF = { pt_word, "Cutoff", "Cutoff", 0,	0xfffe, 0xffff, MPF_STATE, 80 <<9};
CMachineParameter const paraFILTER_RESONANCE = { pt_word, "Resonance", "Resonance", 0,	0xfffe, 0xffff, MPF_STATE, 20<<9 };
CMachineParameter const paraFILTER_PEAKS = { pt_word, "Peaks", "Peaks", 0,	0xfffe, 0xffff, MPF_STATE, 0 };
CMachineParameter const paraFILTER_DRIFT = { pt_word, "Drift", "Drift", 0,	0xfffe, 0xffff, MPF_STATE, 0 };
CMachineParameter const paraFILTER_KEYTRACK = { pt_word, "Keytrack", "Keytrack", 0,	0xfffe, 0xffff, MPF_STATE, 63<<9};
CMachineParameter const paraFILTER_ENVELOPE = { pt_word, "Filter Envelope", "Filter Envelope", 0,	0xfffe, 0xffff, MPF_STATE, 90<<9 };
CMachineParameter const paraFILTER_DRIVE = { pt_word, "Drive", "Drive", 0,	0xfffe, 0xffff, MPF_STATE, 90<<9 };
CMachineParameter const paraFILTER_ATTACK = { pt_word, "Filter Attack", "Filter Attack", 0,	0xfffe, 0xffff, MPF_STATE, 0 };
CMachineParameter const paraFILTER_DECAY = { pt_word, "Filter Decay", "Filter Decay", 0,	0xfffe, 0xffff, MPF_STATE, 40<<9 };

CMachineParameter const paraFILTER_ACCENT = { pt_word, "Accent Amount", "Accent Amount", 0,	0xfffe, 0xffff, MPF_STATE, 80 <<9};

CMachineParameter const paraAMP_VELOCITY = { pt_word, "Velocity", "Velocity", 0,	0xfffe, 0xffff, MPF_STATE, 0 };

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
	word params[__PARAMCOUNT];
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

	virtual void MidiNote(int const channel, int const value, int const velocity);

public:
	BigFish *Fish;
	
	int NoteTimes[128];
	int NoteVelocities[128];
	int lastplayednote;
	
	int activenote;
	int lastnoteid;
	void AddNoteToStack(int note, int vel)
	{
		char txt[200];
		sprintf(txt, "adding %d (%d)\n", note, vel);
		OutputDebugStringA(txt);

		NoteVelocities[note] = vel;
		NoteTimes[note] = lastnoteid++;
		activenote = note;
		Fish->SetParam(AMP_VELOCITY, vel * 512);
		Fish->SetGate(FISHGATE_GATE, 0);
		Fish->CheckGates();
		Fish->SetGate(FISHGATE_GATE, 1);
		Fish->SetNote(activenote);

	
	}
	void RemoveNoteFromStack(int note)
	{
		char txt[200];
		sprintf(txt, "removing %d\n", note);
		OutputDebugStringA(txt);
		NoteTimes[note] = -1;
		int highest = 0;
		for (int i = 1; i < 128; i++)
		{
			if (NoteTimes[i] > NoteTimes[highest])  highest = i;
		}
		if (NoteTimes[highest] > -1)
		{
			activenote = highest;

			
			Fish->SetParam(AMP_VELOCITY, NoteVelocities[highest] * 512);
			Fish->SetGate(FISHGATE_GATE, 0);
			Fish->CheckGates();
			Fish->SetGate(FISHGATE_GATE, 1);
			Fish->SetNote(activenote);

		}
		else
		{
			activenote = -1;
			Fish->SetGate(FISHGATE_GATE, 0);
		}
	}


	float lastmingain;
	bool IdleMode;
	int IdleCount;
	gvals gval;

};


DLL_EXPORTS

mi::mi()
{
	lastnoteid = 1;
	lastplayednote = 0;
	for (int i = 0; i < 128; i++)
	{
		NoteTimes[i] = -1;
		NoteVelocities[i] = 127;
	}
	GlobalVals = &gval;
	Fish = NULL;
	AttrVals = NULL;
	activenote = -1;
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
	
	if (value<32768)
	{
		float V = (32768 - value) / 327.68f;
		sprintf_s(txt, 16, "%.1f%%", -V);
	}
	else
	{
		float V = (value - 32768) / ((32767)*0.01f);
		sprintf_s(txt, 16, "%.1f%%", V);
	}
}

void DescribeKnob(int const value, char *txt) 
{
	sprintf_s(txt, 16, "%.1f%%", value*100.0f/(float)0xfffe);
}

char const *mi::DescribeValue(int const param, int const value)
{
	static char txt[16];
	switch (param -2 )
	{


	case PITCH_COARSE:
		sprintf_s(txt, 16, "%d semitones", (int)((value - 32768)*48/ 32768.0f));
		break;

	case PITCH_FINE:
		sprintf_s(txt, 16, "%.1f semitone", (value - 32768)*2.0f / 32768.0f);
		break;
	
	case FILTER_DRIVE:
	{
		if (value == 32768)
		{
			sprintf_s(txt, 16, "OFF");
		}
		else
		{

			if (value<32768)
			{
				float V = (32768 - value) / 327.68;
				sprintf_s(txt, 16, "%.1f%% Fuzz", V);
			}
			else
			{
				float V = (value- 32768) / (327.67);
				sprintf_s(txt, 16, "%.1f%% Clean", V);
			}
		}
	}
		break;
	case FILTER_ENVELOPE:
		if (value == 32768)
		{
			sprintf_s(txt, 16, "OFF");
		}
		else
		{
			DescribeMidKnob(value, txt);
		}
			break;

	case OSC_SHAPE:
	{

		SteppedResult_t sr;
		GetSteppedResult(Fish->Fish.Parameters[OSC_SHAPE], 6, &sr);

		int FuncIdx = sr.index;
		int32_t crossfade = sr.fractional;
		
		//GenFunc* Funcs[8] = { HyperSaw, HyperPulse, SuperFish, Vosim, Organ, Choir, Grain, Copy };

		char STRING[] = 
			"--- HyperSaw --"
			"-- HyperPulse -"
			"--- Standard --"
			"---- Vosim ----"
			"---- Organ ----"
			"---- Choir ----"
			"---- Grain ----"
			"---------------"
			
			;
		for (int i = 0; i < 15; i++)
		{
			txt[i] = STRING[i + FuncIdx * 15 + (crossfade * 15) / 256];
		}
		txt[15] = 0;
	}
	break;


	case PITCH_CHORD:
	default:
		sprintf_s(txt, 16, "%d", value);
		break;

	case FILTER_KEYTRACK:
	{
		SteppedResult_t sr;
		GetSteppedResult(Fish->Fish.Parameters[FILTER_KEYTRACK], __FILTERKEYTRACK_COUNT - 1, &sr);
	
		int KeyTrackIdx = sr.index;
		int32_t crossfade = sr.fractional;
		
		char STRING[] =
			"---- -2.0 -----"
			"---- -1.5 -----"
			"---- -1.0 -----"
			"---- -0.5 -----"
			"----- OFF ----"
			"----- 0.5 -----"
			"----- 1.0 -----"
			"----- 1.5 -----"
			"----- 2.0 -----"
			"---------------";

		for (int i = 0; i < 15; i++)
		{
			txt[i] = STRING[i + KeyTrackIdx * 15 + (crossfade * 15) / 256];
		}
		txt[15] = 0;

	}
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

void mi::MidiNote(int const channel, int const value, int const velocity) 
{
	if (pCB->HostMIDIFiltering())
	{
		if (velocity == 0)
		{
			RemoveNoteFromStack(value);
			
		}
		else if (velocity > 0)
		{
			AddNoteToStack(value, velocity);
		}
	}

}

void mi::Tick()
{
	for (int i = 0; i < __PARAMCOUNT; i++)
	{
		if (gval.params[i] != 0xffff)
		{
			switch (i)
			{
			case FILTER_TYPE:
			{
				int F = gval.params[i];
				
				Fish->Fish.Parameters[FILTER_TYPE] = F;


			}
				break;
			
			default:
				Fish->SetParam(i, gval.params[i]);
				break;
			}
		}
	}

	if (gval.note == NOTE_OFF)
	{
		RemoveNoteFromStack(lastplayednote);
	}
	else if (gval.note != NOTE_NO)	
	{
		lastplayednote = (gval.note >> 4) * 12 + (gval.note & 0xf);
		AddNoteToStack(lastplayednote, Fish->Fish.Parameters[AMP_VELOCITY] >> 9);		
	}
	
}

void mi::Stop()
{
	for (int i = 0; i < 128; i++)
	{
		NoteTimes[i] = -1;
	}
	
	Fish->SetGate(FISHGATE_GATE, 0);
}


bool mi::Work(float *psamples, int numsamples, int const mode)
{
	Fish->Process(psamples, numsamples);
	for (int i = 0; i < numsamples; i++) if (psamples[i] != 0) return true;
	return false;
}
