#include "..\..\EdgeCutter\Sources\EdgeCutter.h"
#include "..\..\Wobbler\Sources\Wobbler.h"
#include "..\..\WobblerV2\Sources\Wobbler2.h"
#include "..\..\Tuesday\Sources\Tuesday.h"
#include "..\..\Tuesday\Sources\Algo.h"

#include "../../BigFishLib/BigFish.cpp"
#include "../../BigFishLib/BleppyOscs.cpp"
#include "../../BigFishLib/OrganOsc.cpp"
#include "../../BigFishLib/ADSREnvelope.cpp"
#include "../../BigFishLib/FMOsc.cpp" 



struct BigFish_t Fish;
struct Tuesday_PatternGen Tuesday;
struct Tuesday_Settings TuesdaySettings;
struct Tuesday_Params TuesdayParams;


struct  EdgeCutter_Envelope EnvRunning;
struct  EdgeCutter_Envelope EnvStatic;
struct  EdgeCutter_Settings EnvSettings;
struct  EdgeCutter_Params EnvParams;


struct  Wobbler_LFO LFORunning;
struct  Wobbler_LFO LFOStatic;
struct  Wobbler_Settings LFOSettings;
struct  Wobbler_Params LFOParams;


extern "C"
{
	void __cdecl doTick()
	{
		Tuesday_Tick(&Tuesday, &TuesdayParams);
	}
	void __cdecl DoClock(int state)
	{
		Tuesday_Clock(&Tuesday, &TuesdaySettings, &TuesdayParams, state);
	}

	Wobbler2_Pendulum_t Pendulum;
	SpringMassSystem_t PendulumInt;


	__declspec(dllexport) void __stdcall Init()
	{
		Wobbler2_InitPendulum(&Pendulum);
		Wobbler2_InitIntPendulum(&PendulumInt);

		EdgeCutter_LoadSettings(&EnvSettings, &EnvParams);
		EdgeCutter_Init(&EnvRunning);
		EdgeCutter_Init(&EnvStatic);
		Wobbler_LoadSettings(&LFOSettings, &LFOParams);
		Wobbler_Init(&LFORunning);
		Wobbler_Init(&LFOStatic);

		EuroRack_InitCalibration();
		Tuesday_LoadSettings(&TuesdaySettings, &TuesdayParams);

		Tuesday_Init(&Tuesday);
		Tuesday_Generate(&Tuesday, &TuesdayParams, &TuesdaySettings);
	}

	__declspec(dllexport) void __stdcall  ResetStatic()
	{
		EdgeCutter_Init(&EnvStatic);
		Wobbler_Init(&LFOStatic);
	}

	__declspec(dllexport) int __stdcall GetEnv(int staticenv, int attack, int decay, int sustain, int release, int curvature, int speed, int mode)
	{
		EnvParams.mode = mode;
		EnvParams.speed = speed;
		EnvStatic.A = EnvRunning.A = attack;
		EnvStatic.D = EnvRunning.D = decay;
		EnvStatic.S = EnvRunning.S = sustain;
		EnvStatic.R = EnvRunning.R = release;
		EnvStatic.Curvature = EnvRunning.Curvature = curvature;

		if (staticenv == 0) return EdgeCutter_GetEnv(&EnvRunning, &EnvParams);
		return	EdgeCutter_GetEnv(&EnvStatic, &EnvParams);
	}

	__declspec(dllexport) int __stdcall GetEnvCurve(int staticenv)
	{
		if (staticenv == 0) return (int)(EnvRunning.CurvedOutput);
		return (int)(EnvStatic.CurvedOutput);
	}

	__declspec(dllexport) int __stdcall GetLed(int Led)
	{
		return EnvRunning.StateLeds[Led];
	}

	__declspec(dllexport) int __stdcall GetGate(int gate)
	{
		return EnvRunning.Gates[gate];
	}

	__declspec(dllexport) void __stdcall Trigger(int val, int staticenv, int mode)
	{
		EnvParams.mode = mode;
		if (staticenv == 0) EdgeCutter_Trigger(&EnvRunning, val, &EnvParams);
		EdgeCutter_Trigger(&EnvStatic, val, &EnvParams);
	}

	__declspec(dllexport) int __stdcall GetLFOLed(int led)
	{
		return LFORunning.Led[led];
	}

	__declspec(dllexport)  int __stdcall GetLFO(int staticlfo, int speed, int shape, int mod, int phaseing)
	{
		struct Wobbler_LFO *L = &LFORunning;
		if (staticlfo) L = &LFOStatic;

		L->Speed = speed;
		L->Shape = shape;
		L->Mod = mod;
		L->Phasing = phaseing;

		return Wobbler_Get(L, &LFOParams);
	}

	__declspec(dllexport) int __stdcall GetLFOGate(int gate)
	{
		return LFORunning.Gate[gate];
	}

	__declspec(dllexport) int __stdcall GetLFOPhased(int staticlfo)
	{
		if (staticlfo) return (int)(LFOStatic.OutputPhased);
		return (int)(LFORunning.OutputPhased);
	}

	__declspec(dllexport) void __stdcall LFOTrigger(int value, int staticlfo)
	{
		struct Wobbler_LFO *L = &LFORunning;
		if (staticlfo) L = &LFOStatic;

		Wobbler_Trigger(L, value, &LFOParams);
	}

	__declspec(dllexport) int __stdcall Tuesday_GetTickVel(int tick)
	{
		return Tuesday.CurrentPattern.Ticks[tick].vel;
	}

	__declspec(dllexport) int __stdcall Tuesday_GetTickNote(int tick)
	{
		return Tuesday.CurrentPattern.Ticks[tick].note;
	}

	__declspec(dllexport) int __stdcall Tuesday_GetTickGate(int tick)
	{
		return Tuesday.CurrentPattern.Ticks[tick].vel > 0 ? 1 : 0;
	}

	__declspec(dllexport) int __stdcall Tuesday_GetTickSlide(int tick)
	{
		return Tuesday.CurrentPattern.Ticks[tick].slide;
	}

	__declspec(dllexport) int __stdcall Tuesday_GetTickLength(int tick)
	{
		return Tuesday.CurrentPattern.Ticks[tick].maxsubticklength;
	}

	__declspec(dllexport) int __stdcall Tuesday_GetTickAccent(int tick)
	{
		return Tuesday.CurrentPattern.Ticks[tick].accent;
	}

	__declspec(dllexport) int __stdcall Tuesday_GetPatternLength()
	{
		return Tuesday.CurrentPattern.Length;
	}

	__declspec(dllexport) void __stdcall Tuesday_UpdatePattern(int algo, int scale, int ticks, int beats, int tempo, int x, int y, int i)
	{
		TuesdayParams.algo = 0;
		TuesdaySettings.algooptions[0] = algo;

		TuesdayParams.beatopt = 0;
		TuesdaySettings.beatoptions[0] = beats;
		
		TuesdayParams.scale = 0;
		TuesdaySettings.scale[0] = scale;


		TuesdayParams.tpbopt = 0;
		TuesdaySettings.tpboptions[0] = ticks;

		Tuesday.seed2 = y;
		Tuesday.seed1 = x;
		Tuesday.intensity = i;
		Tuesday.tempo = tempo;

		Tuesday_Generate(&Tuesday, &TuesdayParams, &TuesdaySettings);
	}

	__declspec(dllexport) void __stdcall Tuesday_ClockTick(int ClockConnected, int val)
	{
		Tuesday.ClockConnected = ClockConnected;
		Tuesday_Clock(&Tuesday, &TuesdaySettings,&TuesdayParams,val);
	}

	__declspec(dllexport) void __stdcall Tuesday_DoTimer()
	{
		Tuesday_TimerTick(&Tuesday, &TuesdayParams);
	}


	__declspec(dllexport) bool __stdcall  Tuesday_GetAccent()
	{
		return Tuesday.Gates[GATE_ACCENT] > 0;
	}

	__declspec(dllexport) int __stdcall Tuesday_GetVel()
	{
		return Tuesday.VelocityOut;

	}

	__declspec(dllexport) int __stdcall Tuesday_GetCV()
	{
		return Tuesday.CVOut;
	}

	__declspec(dllexport) bool __stdcall  Tuesday_GetGate()
	{
		return Tuesday.Gates[GATE_GATE] > 0;
	}

	__declspec(dllexport) float RunPendulum()
	{
		Wobbler2_DoublePendulum(&Pendulum, 0.05);
		return Pendulum.A;
	}

	__declspec(dllexport) float RunPendulum2()
	{
		return Pendulum.B;
	}

	__declspec(dllexport) void RunPendulumInt()
	{
		Wobbler2_DoublePendulumInt(&PendulumInt, (int)((0.4) * (1<<16)));
		
	}

	__declspec(dllexport) float RunPendulum2Int(int32_t SpringOrMass, int32_t ID, int32_t Coord)
	{
		switch (SpringOrMass)
		{
		case 0:
			switch (Coord)
			{
			case 0: return PendulumInt.Masses[ID].Pos.X;
			case 1: return PendulumInt.Masses[ID].Pos.Y;
			case 2: return PendulumInt.Masses[ID].Speed.X;
			case 3: return PendulumInt.Masses[ID].Speed.Y;
			case 4: return PendulumInt.Masses[ID].Force.X;
			case 5: return PendulumInt.Masses[ID].Force.Y;
			}
			break;
		case 1:
			switch (Coord)
			{
			case 0: return PendulumInt.Springs[ID].A;
			case 1: return PendulumInt.Springs[ID].B;
			case 2: return PendulumInt.Springs[ID].K;
			}
		case 2:
			return PendulumInt.MassCount;
		case 3:
			return PendulumInt.SpringCount;
		}
		return 0;
	}
}

#include <Windows.h>
#include <stdio.h>

void DoSubDivTest(int clockmode, int maxclocks, int tpbopt)
{

	Init();
	TuesdaySettings.ClockSubDivMode = clockmode;
	Tuesday_SetupClockSubdivision(&Tuesday, &TuesdaySettings, &TuesdayParams);

	Tuesday.CurrentPattern.TPB = 4;
	Tuesday.tempo = 128;
	Tuesday.intensity = 100;
	TuesdayParams.tpbopt = tpbopt;
	Tuesday_UpdatePattern(0, 0, 4, 4, 200, 0, 0, 255);
	int LastI = 0;
	int gates[TUESDAY_GATES];
	for (int i = 0; i < TUESDAY_GATES; i++) gates[i] = -1;
	int LastTick = -2;
	int LastNewTick = -3;
	Tuesday_Reset(&Tuesday);
	int lastnonzeroes = -1;
	int cv = 0;
	for (int i = 0; i < maxclocks; i++)
	{
		Tuesday_Clock(&Tuesday,&TuesdaySettings, &TuesdayParams, ((cv++)%2==0)?1:0);

		int nonzeroes = 0;
		for (int j = 0; j < TUESDAY_GATES - 1; j++)
		{
			nonzeroes += Tuesday.Gates[j];
		}
		bool print = true;
		if (nonzeroes == 0) print = false;
		lastnonzeroes = nonzeroes;
		if (print)
		{
			printf("%d) ", i);

			for (int j = 0; j < TUESDAY_GATES - 1; j++)
			{
				nonzeroes += Tuesday.Gates[j];
				char L[TUESDAY_GATES] = { 'G', 'a', 'L', 'b', 'c', 't' };
				printf("%c ", Tuesday.Gates[j] > 0 ? L[j] : ' ');
				Tuesday.Gates[j] = 0;
			}
		}


		if (Tuesday.Tick != LastTick || Tuesday.NewTick != LastNewTick)
		{
			if (!print)
			{
				printf("%d) ", i);
			}
			print = true;
			if (Tuesday.Tick != LastTick || Tuesday.NewTick != LastNewTick)
			{
				printf("NewTick: %d - Tick: %d - delta %d ", Tuesday.NewTick, Tuesday.Tick, i - LastI);
				LastI = i;
				LastTick = Tuesday.Tick;
				LastNewTick = Tuesday.NewTick;
			}
		}
		if (print) printf("\n");
	}
}

void RunTimingTest()
{
	for (int i = 0; i < 4; i++)
	{
		printf("**** 4 PPQN %d tpb\n", TuesdaySettings.tpboptions[i]);
		DoSubDivTest(CLOCKSUBDIV_4, 4 * 4, i);
		printf("\n\n**** 8 PPQN %d tpb\n", TuesdaySettings.tpboptions[i]);
		DoSubDivTest(CLOCKSUBDIV_8, 8 * 4,i);
		printf("\n\n**** 16 PPQN %d tpb\n", TuesdaySettings.tpboptions[i]);
		DoSubDivTest(CLOCKSUBDIV_16, 16 * 4,i);
		printf("\n\n**** 24 PPQN %d tpb\n", TuesdaySettings.tpboptions[i]);
		DoSubDivTest(CLOCKSUBDIV_24PPQN, 24 * 4,i);
	}

}

void checkbuffer(int32_t *buf, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (buf[i] > 35000 || buf[i] < -35000)
		{
			printf("^");
		}
	}
}
void FishCycle()
{
	int bufferosc[1000];
	int buffermain[1000];
	int inputfalse[1000];
	for (int i = 0; i < 127; i++)
	{
		printf(".");
		BigFish_GenerateBlock(&Fish, inputfalse, bufferosc, buffermain, 1000);
		checkbuffer(bufferosc, 1000);
		checkbuffer(buffermain, 1000);

		int32_t pitchtarget = (i * (1 << 14)) / 12;
		Fish.PitchInput = pitchtarget;

		Fish.Gates[GATE_GATE] = 1;
		BigFish_GenerateBlock(&Fish, inputfalse, bufferosc, buffermain, 1000);
		checkbuffer(bufferosc, 1000);
		checkbuffer(buffermain, 1000);
		Fish.Gates[GATE_GATE] = 0;
	}
	printf("!\n");
}

void RunFishTest()
{
	BigFish_Update(&Fish);
	for (int i = 0; i < __PARAMCOUNT; i++)
	{
		Fish.Parameters[i] = 0;
	}
	Fish.Parameters[AMP_ATTACK] = (10 * 65536) / 127;;
	Fish.Parameters[AMP_DECAY] = (10 * 65536) / 127;;

	Fish.Parameters[FILTER_ATTACK] = (10 * 65536) / 127;;
	Fish.Parameters[FILTER_DECAY] = (80 * 65536) / 127;;

	Fish.Parameters[AMP_SUSTAIN] = (64 * 65536) / 127;
	Fish.Parameters[AMP_RELEASE] = (64 * 65536) / 127;;
	Fish.Parameters[PITCH_COARSE] = (64 * 65536) / 127;;
	Fish.Parameters[PITCH_FINE] = (64 * 65536) / 127;;

	//Fish.Parameters[OSC_SHAPE] = ( * 65536) / 127;;

	FishCycle();

	for (int i = 0; i < 2; i++)
	{
		for (int i2 = 0; i2 < __PARAMCOUNT; i2++)
		{
			Fish.Parameters[i2] = rand()%65536;
		}
		FishCycle();
	}
}

void RunTest(const char * name, int i)
{
	int min = 10000;
	int max = -1000;
	double avg = 0;
	int c = 0;
	bool HasOffs = false;
	for (int bang = 0; bang < 255; bang += 16)
	{
		for (int y = 0; y < 255; y += 16)
		{

			for (int x = 0; x < 255; x += 16)
			{
				int patlen = Tuesday.CurrentPattern.Length;
				
				Tuesday_UpdatePattern(i, 0, 1, 2, 0, x, y, bang);
				for (int j = 0; j < patlen; j++)
				{
					struct Tuesday_Tick &T = Tuesday.CurrentPattern.Ticks[j];
					if (T.note != TUESDAY_NOTEOFF)
					{
						c++;
						if (T.note < min) min = T.note; else if (T.note > max) max = T.note;
					}
					else
					{
						HasOffs = true;
					}
					avg = avg + T.note;
				}
			}
		}
	}
	avg /= (double)c;

	printf("%s (%d): \tmin %d \tmax %d\tavg:%2f\tHas Offs: %d\n", name, i, min, max, avg, HasOffs ? 1 : 0);
}

void CalTest()
{
	for (int i = 0; i < 4096; i += 256)
	{
		int R = CalibratedDAC(0, i);
		printf("%d %d\n", i, R);
	}
}

void PrintPlot(int V, int min, int max)
{
	float F = (V - min) / (float)(max - min);
	if (F < 0) F = 0;
	if (F > 1) F = 1;
	printf("|");
	for (int i = 0; i < 42; i++)
	{
		float F2 = i / 41.0f;
		if (F < F2) printf(" "); else printf("*");
	}
	printf("|\n");
}
void PrintPlot2(int V,int V2, int min, int max)
{
	float F = (V - min) / (float)(max - min);
	if (F < 0) F = 0;
	if (F > 1) F = 1;
	float F2 = (V2 - min) / (float)(max - min);
	if (F2 < 0) F2 = 0;
	if (F2 > 1) F2 = 1;
	printf("|");
	for (int i = 0; i < 75; i++)
	{
		float Fl = i / 74.0f;
		bool a = F > Fl;
		bool b = F2 > Fl;
		int R = (a ? 1 : 0) + (b ? 2 : 0);
		switch (R)
		{
		case 0: printf(" "); break;
		case 1: printf("*"); break;
		case 2: printf("o"); break;
		case 3: printf("#"); break;
		}
	}
	printf("|\n");
}
extern void ADSR_BuildTable();
void ExpTest()
{


	ADSR_BuildTable();
	ADSR_Envelope_t Env;
	ADSR_Init(&Env, ENVMODE_GATE, 0, ENVTABLE_LOG);
	Env.A = 140<<1;
	Env.D = 140<<2;
	Env.S = 150;
	Env.R = 160<<1;
	ADSR_Trigger(&Env, 1);
	ADSR_Update(&Env, 1000);
	for (int i = 0; i < 1000; i++)
	{

		int R = ADSR_Get(&Env);
		int R2 = Env.CurvedOutput >> (ENVFIXEDBITS - 16);
		if (i == 500) ADSR_Trigger(&Env, 0);
		//if (i%10 == 0) PrintPlot(R, 0, 0xffff);
		if (i % 10 == 0) PrintPlot2(R,R2, 0, 0xffff);
	}

	for (uint32_t i = 0; i < (uint32_t)(65536.0* 60000); i += uint32_t( 60000 * 600))
	{
//		printf("%08x\n",GetExpTable(i));
	}
//	printf("done!\n");
}

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
)
{

	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		int32_t V[4] = { 0,65536,0,65536 };
		//CalTest();
		Wobbler_LoadSettings(&LFOSettings, &LFOParams);
		Wobbler_Init(&LFORunning);
		Wobbler_Init(&LFOStatic);
		BigFish_Init(&Fish, 44100);
//		ExpTest();
	//	RunFishTest();
		LFOStatic.Speed = 0x80;
		Init();

	/*	RunTest("Tests", ALGO_TESTS);
		RunTest("TriTrance", ALGO_TRITRANCE);
		RunTest("Stomper", ALGO_STOMPER);
		RunTest("Markov", ALGO_MARKOV);
		RunTest("Wobble", ALGO_WOBBLE);
		RunTest("Chip 1", ALGO_CHIPARP1);
		RunTest("Chip 2", ALGO_CHIPARP2);
		RunTest("SNH", ALGO_SNH);
		RunTest("Saiko Lead", ALGO_SAIKO_LEAD);
		RunTest("Saiko Classic", ALGO_SAIKO_CLASSIC);
		RunTest("ScaleWalker", ALGO_SCALEWALKER);
		RunTest("TooEasy", ALGO_TOOEASY);
		RunTest("Random", ALGO_RANDOM);
		*/

		RunTimingTest();

		if (0)
		{
			for (int i = 0; i < 256; i++)
			{
				LFOStatic.Phasing = 80 + ((i / 80) % 2);

				Wobbler_Get(&LFOStatic, &LFOParams);

			//	printf("%x %x %x\n", LFOStatic.OldPhase2, LFOStatic.Phase2, LFOStatic.Gate[1]);

			}
		}
	}
	return TRUE;
}