
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
} EdgeCutter2_Params;

typedef struct EdgeCutter2_Settings
{
	unsigned char SlowSpeedMult;
	unsigned char GatedMode;
} EdgeCutter2_Settings;

#define ENVSTATE_IDLE 0
#define ENVSTATE_ATTACK 1
#define ENVSTATE_DECAY 2
#define ENVSTATE_SUSTAIN 3
#define ENVSTATE_RELEASE 4

#define ENVMODE_TRIGGER 0
#define ENVMODE_GATE 1
#define ENVMODE_LOOP 2

#define GATE_ATTACKEND 3
#define GATE_DECAYEND 2
#define GATE_RELEASESTART 1
#define GATE_RELEASEEND 0

#define GATE_COUNTDOWN 30;


typedef struct EdgeCutter2_Envelope
{
	unsigned short A;
	unsigned short D;
	unsigned short S;
	unsigned short R;
	unsigned short Curvature;

	unsigned char TriggerState;
	unsigned char Velocity;
	
	int32_t LinearOutput;
	int32_t CurvedOutput;

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

	EdgeCutter2_Settings Settings;

} EdgeCutter2_Envelope;

#ifdef __cplusplus
extern "C"
{
#endif
	extern int EdgeCutter2_GetEnv(struct EdgeCutter2_Envelope *Env, struct EdgeCutter2_Params *Params);
	extern void EdgeCutter2_Init(struct EdgeCutter2_Envelope *Env);
	extern void EdgeCutter2_Trigger(struct EdgeCutter2_Envelope *Env, unsigned char N, struct EdgeCutter2_Params *Params);
	extern void EdgeCutter2_LoadSettings(struct EdgeCutter2_Settings *settings, struct EdgeCutter2_Params *params);
	extern void EdgeCutter2_ValidateParams(struct EdgeCutter2_Params *params);
#ifdef __cplusplus
}
#endif
