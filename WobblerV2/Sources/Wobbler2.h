
#define Wobbler2_GATECOUNTDOWN 100
#include <stdint.h>
#include "../../EurorackShared/EURORACKSHARED.H"

struct Wobbler2_Params
{
	unsigned char T;
};
	
struct Wobbler2_Settings
{
	unsigned char SlowSpeedMult;
};

#define Wobbler2_ATTACK 2
#define Wobbler2_IDLE 3
#define Wobbler2_RELEASE 4

struct Wobbler2_RandomGen
{
	long RandomMemory;
};


typedef struct Wobbler2_LFO_SNH_t
{
	uint32_t countdown;
	
	uint32_t lastval;
	
	
	uint8_t lastseg;
	struct Wobbler2_RandomGen random;	
	struct EURORACK_SVF filt;
} Wobbler2_LFO_SNH_t;
typedef struct Wobbler2_Pendulum_t
{
	int32_t A;
	int32_t B;
	float m1;
	float m2;
	float Theta1;
	float Theta2;
	float d2Theta1;
	float d2Theta2;
	float dTheta1;
	float dTheta2;
	float l1;
	float l2;
	float g;
	float mu;
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

struct Wobbler2_PendulumInt
{
	int32_t A;
	int32_t B;
	int32_t m1;
	int32_t m2;
	uint32_t Theta1;
	uint32_t Theta2;
	int32_t d2Theta1;
	int32_t d2Theta2;
	int32_t  dTheta1;
	int32_t dTheta2;
	int32_t l1;
	int32_t l2;
	int32_t g;
	int32_t mu;
};

typedef struct SteppedResult_t
{
	uint8_t index;
	uint8_t fractional;
} SteppedResult_t;


void GetSteppedResult(uint16_t param, uint8_t steps, SteppedResult_t *out);
//float GetInterpolatedResultFloat(float *table, SteppedResult_t *inp);
int32_t GetInterpolatedResultInt(int32_t *table, SteppedResult_t *inp);

typedef struct Wobbler2_LFO_t
{
	uint16_t Speed;
	uint16_t Shape;
	uint16_t Mod;
	uint16_t Phasing;

	uint32_t Phase1;
	uint32_t OldPhase1;
	uint32_t Phase2;
	uint32_t OldPhase2;


	int32_t OutputPhased;
	int32_t Output;

	unsigned char Gate[2];
	unsigned char Led[2][9];
	unsigned char ModeLed[3];
	unsigned char TriggerLed;

	uint32_t PhasedCountdown;
	int32_t EnvelopeVal;
	uint8_t EnvelopeState;

	Wobbler2_LFO_SNH_t SNH[2];
	Wobbler2_Pendulum_t Pendulum;
	
	int synctime[3];
	int timesincesync;
	int extsync;
	int syncindex;

	SpringMassSystem_t PendulumSystem;

} Wobbler2_LFO_t;

#ifdef __cplusplus
extern "C"
{
#endif
	extern int Wobbler2_Get(Wobbler2_LFO_t *LFO, struct Wobbler2_Params *Params);
	extern void Wobbler2_Init( Wobbler2_LFO_t *LFO);
	extern void Wobbler2_InitPendulum( Wobbler2_Pendulum_t *Pendulum);
	extern void Wobbler2_InitIntPendulum(SpringMassSystem_t *Pendulum);
	extern void Wobbler2_DoublePendulumInt(SpringMassSystem_t *P, float DT);
	extern void Wobbler2_DoublePendulum( Wobbler2_Pendulum_t *P, float DT);
	extern void Wobbler2_Trigger(Wobbler2_LFO_t *LFO, unsigned char N, struct Wobbler2_Params *Params);
	extern void Wobbler2_LoadSettings(struct Wobbler2_Settings *settings, struct Wobbler2_Params *params);
	extern void Wobbler2_ValidateParams(struct Wobbler2_Params *params);
	extern void Wobbler2_StartTwang(Wobbler2_LFO_t *LFO);

	extern int32_t LERP(int32_t *V, int total, int fade);
#ifdef __cplusplus
}
#endif
