#include "C:\Projects\Code\Kinetis2\Eurorack-KDS\EdgeCutter\Sources\EdgeCutter.h"
#include "C:\Projects\Code\Kinetis2\Eurorack-KDS\Wobbler\Sources\Wobbler.h"
#include "C:\Projects\Code\Kinetis2\Eurorack-KDS\Tuesday\Sources\Tuesday.h"
#include "C:\Projects\Code\Kinetis2\Eurorack-KDS\Tuesday\Sources\Algo.h"


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
		Tuesday_Clock(&Tuesday, state);
	}
	__declspec(dllexport) void __stdcall Init()
	{
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
		Tuesday_Clock(&Tuesday, val);
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


}

#include <Windows.h>
#include <stdio.h>
void RunTimingTest()
{
	Init();
	Tuesday.CurrentPattern.TPB = 4;
	Tuesday_UpdatePattern(0, 0, 4, 4, 200, 0, 0, 255);
	int LastI = 0;
	int gates[TUESDAY_GATES];
	for (int i = 0; i < TUESDAY_GATES; i++) gates[i] = -1;
	int LastTick = -2;
	for (int i = 0; i < 1000; i++)
	{

		Tuesday_Clock(&Tuesday, 1);
		Tuesday_Clock(&Tuesday, 0);

		int GateDiff = 0;
		for (int j = 0; j < TUESDAY_GATES; j++)
		{
			if (gates[j] != Tuesday.Gates[j])
			{
				GateDiff++;
				gates[j] = Tuesday.Gates[j];
			}
		}
		if (i == 590)
		{
			printf("reset!\n");
			Tuesday_Reset(&Tuesday);
		}
		if (GateDiff > 0 || Tuesday.Tick != LastTick)
		{
			printf("%d) ", i);
			for (int j = 0; j < TUESDAY_GATES; j++)
			{
				printf("%d ", Tuesday.Gates[j] > 0 ? 1 : 0);
				Tuesday.Gates[j] = 0;
			}
			if (Tuesday.Tick != LastTick)
			{
				printf("Tick: %d - delta %d ", Tuesday.Tick, i - LastI);
				LastI = i;
				LastTick = Tuesday.Tick;
			}
			printf("\n");
		}
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
BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
)
{

	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		int32_t V[4] = { 0,65536,0,65536 };
		CalTest();
		Wobbler_LoadSettings(&LFOSettings, &LFOParams);
		Wobbler_Init(&LFORunning);
		Wobbler_Init(&LFOStatic);
		LFOStatic.Speed = 0x80;
		Init();

		RunTest("Tests", ALGO_TESTS);
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


		RunTimingTest();

		if (0)
		{
			for (int i = 0; i < 256; i++)
			{
				LFOStatic.Phasing = 80 + ((i / 80) % 2);

				Wobbler_Get(&LFOStatic, &LFOParams);

				printf("%x %x %x\n", LFOStatic.OldPhase2, LFOStatic.Phase2, LFOStatic.Gate[1]);

			}
		}
	}
	return TRUE;
}