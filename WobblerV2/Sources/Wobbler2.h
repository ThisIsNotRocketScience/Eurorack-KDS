
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


struct Wobbler2_LFO_SNH
{
	uint32_t countdown;
	
	uint32_t lastval;
	
	
	uint8_t lastseg;
	struct Wobbler2_RandomGen random;	
	struct EURORACK_SVF filt;
};
struct Wobbler2_Pendulum
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
};

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

struct Wobbler2_LFO
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

	struct Wobbler2_LFO_SNH SNH[2];
	struct Wobbler2_Pendulum Pendulum;
};

#ifdef __cplusplus
extern "C"
{
#endif
	extern int Wobbler2_Get(struct Wobbler2_LFO *LFO, struct Wobbler2_Params *Params);
	extern void Wobbler2_Init(struct Wobbler2_LFO *LFO);
	extern void Wobbler2_InitPendulum(struct Wobbler2_Pendulum *Pendulum);
	extern void Wobbler2_InitIntPendulum(struct Wobbler2_PendulumInt *Pendulum);
	extern void Wobbler2_DoublePendulumInt(struct Wobbler2_PendulumInt *P, float DT);
	extern void Wobbler2_DoublePendulum(struct Wobbler2_Pendulum *P, float DT);
	extern void Wobbler2_Trigger(struct Wobbler2_LFO *LFO, unsigned char N, struct Wobbler2_Params *Params);
	extern void Wobbler2_LoadSettings(struct Wobbler2_Settings *settings, struct Wobbler2_Params *params);
	extern void Wobbler2_ValidateParams(struct Wobbler2_Params *params);
	extern void Wobbler2_StartTwang(struct Wobbler2_LFO *LFO);

	extern int32_t LERP(int32_t *V, int total, int fade);
#ifdef __cplusplus
}
#endif
