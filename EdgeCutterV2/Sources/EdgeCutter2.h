#include "../../EurorackShared/EURORACKSHARED.H"

#define EDGECUTTER2_MAXMODE 3
#define EDGECUTTER2_MAXSPEED 2
#include <stdint.h>

#define FIXEDBITS 15

#define FRACMASK ((1<<FIXEDBITS) - 1)

#define FIXED(x) ((int32_t)( x * (float)(1<<FIXEDBITS)))


typedef struct EdgeCutter2_Params
{
	unsigned char mode;
	unsigned char speed;
	unsigned char GatedMode;
} EdgeCutter2_Params;

typedef struct EdgeCutter2_Calibration
{
	int32_t CalibNormal;
	int32_t CalibCurved;
} EdgeCutter2_Calibration;

typedef struct EdgeCutter2_Settings
{
	unsigned char SlowSpeedMult;
} EdgeCutter2_Settings;

#define ENVSTATE_IDLE 0
#define ENVSTATE_ATTACK 1
#define ENVSTATE_DECAY 2
#define ENVSTATE_SUSTAIN 3
#define ENVSTATE_RELEASE 4

#define ENVMODE_TRIGGER 0
#define ENVMODE_GATE 1
#define ENVMODE_LOOP 2
#define PEAKCOUNTSCALER 0x30
#define GATE_ATTACKEND 3
#define GATE_DECAYEND 2
#define GATE_RELEASESTART 1
#define GATE_RELEASEEND 0

#define GATE_COUNTDOWN 30;

#define EDGECUTTER_VELOCITYSAMPLES 8

typedef struct EdgeCutter2_Envelope
{
	unsigned short A;
	unsigned short D;
	unsigned short S;
	unsigned short R;
	unsigned short Curvature;

	unsigned char TriggerState;
	unsigned short Velocity;
	
	int32_t LinearOutput;
	int32_t CurvedOutput;
	int32_t runninglowpass;
	int State;
	
	unsigned char Gates[8];
	unsigned char StateLeds[16];
	int32_t LastLed;

	int32_t AttackStart;
	int32_t CurvedAttackStart;
	int32_t DecayStart;
	int32_t CurvedDecayStart;
	int32_t ReleaseStart;
	int32_t CurvedReleaseStart;

	int32_t AttackProgress;
	int32_t DecayProgress;
	int32_t ReleaseProgress;


	int32_t Current;
	int32_t CurrentCurved;
	int32_t CurrentTarget;
	int32_t ReleaseTime;
	int32_t AttackTime;
	int32_t DecayTime;

	int32_t PreviouslySampledVelocity;
	int32_t SampledVelocity;
	int32_t VelocityCurrent;
	int32_t VelocitySampleCountdown;
	int32_t VelocitySample[EDGECUTTER_VELOCITYSAMPLES];
	EdgeCutter2_Settings Settings;

} EdgeCutter2_Envelope;

#ifdef __cplusplus
extern "C"
{
#endif
	extern int EdgeCutter2_GetEnv(EdgeCutter2_Envelope *Env, EdgeCutter2_Params *Params, EdgeCutter2_Calibration *calibration);
	extern void EdgeCutter2_Init(EdgeCutter2_Envelope *Env);
	extern void EdgeCutter2_Trigger(EdgeCutter2_Envelope *Env, unsigned char N, EdgeCutter2_Params *Params);
	extern void EdgeCutter2_Retrigger(EdgeCutter2_Envelope *Env);
	extern void EdgeCutter2_LoadSettings(EdgeCutter2_Settings *settings, EdgeCutter2_Params *params);
	extern void EdgeCutter2_ValidateParams(EdgeCutter2_Params *params);
	extern uint32_t DoCurve(int32_t from, int32_t to, uint32_t prog, uint16_t Curve, SteppedResult_t *curB, int32_t Linear);

#ifdef __cplusplus
}
#endif
