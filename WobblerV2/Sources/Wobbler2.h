
#define Wobbler2_GATECOUNTDOWN 100
#include <stdint.h>
#include "../../EurorackShared/EURORACKSHARED.H"

typedef struct Wobbler2_Params
{
	unsigned char T;
} Wobbler2_Params;
	
typedef struct Wobbler2_Settings
{
	unsigned char SlowSpeedMult;
} Wobbler2_Settings;

#define Wobbler2_ATTACK 2
#define Wobbler2_IDLE 3
#define Wobbler2_RELEASE 4
#define WOBBLER_FIXBITS 24

typedef struct Wobbler2_RandomGen
{
	long RandomMemory;
} Wobbler2_RandomGen;


typedef struct Wobbler2_LFO_SNH_t
{
	uint32_t countdown;
	
	uint32_t lastval1;
	uint32_t lastval2;
	uint32_t store1;
	uint32_t store2;

	
	uint8_t lastseg;
	
	uint16_t segbuffer[32];
	struct Wobbler2_RandomGen random;	

} Wobbler2_LFO_SNH_t;
typedef struct Wobbler2_Pendulum_t
{
	int32_t A;
	int32_t B;
	float m1;
	float m2;
	
	float l1;
	float l2;
	float g;
	float mu;


	float _2sub1;
	float _1sub2;
	float c1sub2;
	float c1sub2SQUARED;
	float s1sub2;
	//float T1a1, T1a2, T1a, T1;
	//float T2b1, T2b, T2;
	//float T3b, T3, T4;
	//float T5a1, T5a2, T5a, T5;
	//float T6b, T6;
	
	float st1;
	float st2;
	float dTheta1SQUARED;
	float dTheta2SQUARED;
	float l1_x_dTheta1SQUARED;
	float l2_x_dTheta2SQUARED;

	float Theta1;
	float Theta2;
	float d2Theta1;
	float d2Theta2;
	float dTheta1;
	float dTheta2;

	float Damping;

	int32_t As;
	int32_t Bs;

} Wobbler2_Pendulum_t;

typedef struct ivec_t
{
	int32_t X; //7.24
	int32_t Y; //7.24
} ivec_t;

typedef struct Mass_t
{
	ivec_t Pos;
	ivec_t PrevPos;
	ivec_t Speed;
	ivec_t Force;
	int fixed;
	int32_t mass; //7.24
} Mass_t;

typedef struct Spring_t
{
	int32_t K; // 7.24
	
	unsigned char A;
	unsigned char B;

	int32_t RestLength; // 7.24
} Spring_t;

typedef struct SpringMassSystem_t
{
	int SpringCount;
	int MassCount;
	Spring_t Springs[4];
	Mass_t Masses[5];
} SpringMassSystem_t;

typedef struct Wobbler2_PendulumInt_t
{
	int32_t A;
	int32_t B;
	int32_t m1;
	int32_t m2;
	
	int32_t l1;
	int32_t l2;
	int32_t g;
	int32_t mu;


	uint32_t _2sub1;
	uint32_t _1sub2;
	int32_t c1sub2;
	int32_t c1sub2SQUARED;
	int32_t s1sub2;
	int32_t T1a1, T1a2, T1a, T1;
	int32_t T2b1, T2b, T2;
	int32_t T3b, T3, T4;
	int32_t T5a1, T5a2, T5a, T5;
	int32_t T6b, T6;

	int32_t st1;
	int32_t st2;
	int32_t dTheta1SQUARED;
	int32_t dTheta2SQUARED;
	int32_t l1_x_dTheta1SQUARED;
	int32_t l2_x_dTheta2SQUARED;

	uint32_t Theta1;
	uint32_t Theta2;
	int32_t d2Theta1;
	int32_t d2Theta2;
	int32_t  dTheta1;
	int32_t dTheta2;
} Wobbler2_PendulumInt_t;

typedef struct SteppedResult_t
{
	uint8_t index;
	uint8_t fractional;
} SteppedResult_t;


//float GetInterpolatedResultFloat(float *table, SteppedResult_t *inp);
int32_t GetInterpolatedResultInt(int32_t *table, SteppedResult_t *inp);



typedef struct Wobbler2_LFO_t
{
	uint16_t Speed;
	uint16_t Shape;
	uint16_t Mod;
	uint16_t Phasing;
	uint16_t LastPhasing;

	int16_t Amount1;
	int16_t Amount2;

	uint32_t Phase1;
	uint32_t OldPhase1;
	uint32_t Phase2;
	int32_t OldPhase2;
	uint32_t PhasedShift;

	int32_t OutputPhased;
	int32_t Output;

	unsigned char Gate[2];
	 int Led[2][9];
	unsigned char ModeLed[5];
	unsigned char TriggerLed;

	uint32_t PhasedCountdown;
	int32_t EnvelopeVal;
	uint8_t EnvelopeState;

	Wobbler2_LFO_SNH_t SNH;
#ifdef INTPENDULUM
	Wobbler2_PendulumInt_t Pendulum;
#else
	Wobbler2_Pendulum_t Pendulum;

#endif
	int synctime[3];
	uint32_t SyncedPeriodTime;
	uint32_t SyncDP;
	int timesincesync;
	int extsync;
	int syncindex;

	SpringMassSystem_t PendulumSystem;

	SteppedResult_t ShapeStepped;
	Shapes_t BasicShapesA;
	Shapes_t BasicShapesB;

	int32_t OutputsNormal[6];
	int32_t OutputsPhased[6];

	ShapeCompensationVals_t CompensationVals;

} Wobbler2_LFO_t;



#ifdef __cplusplus
extern "C"
{
#endif

	extern void Wobbler2_GetSteppedResult(uint16_t param, uint8_t steps, SteppedResult_t *out);

	extern int Wobbler2_Get(Wobbler2_LFO_t *LFO,  Wobbler2_Params *Params);
	extern void Wobbler2_Init( Wobbler2_LFO_t *LFO);
	extern void Wobbler2_InitPendulum( Wobbler2_Pendulum_t *Pendulum,Wobbler2_LFO_t *LFO);
	extern void Wobbler2_DoublePendulum( Wobbler2_Pendulum_t *P, float const DT);
	extern void Wobbler2_Trigger(Wobbler2_LFO_t *LFO, unsigned char N, Wobbler2_Params *Params);
	extern void Wobbler2_LoadSettings( Wobbler2_Settings *settings, Wobbler2_Params *params);
	extern void Wobbler2_ValidateParams(Wobbler2_Params *params);
	extern void Wobbler2_StartTwang(Wobbler2_LFO_t *LFO);
	extern void Wobbler2_UpdatePendulumSettings(Wobbler2_Pendulum_t *P, Wobbler2_LFO_t *W);
	extern void Wobbler2_DoLeds(Wobbler2_LFO_t *LFO);
	extern unsigned long Wobbler2_LFORange3(int32_t V, int32_t SR);


	extern int32_t LERP(int32_t *V, int total, int fade);
#ifdef __cplusplus
}
#endif
