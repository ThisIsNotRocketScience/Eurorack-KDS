
#define EDGECUTTER_MAXMODE 3
#define EDGECUTTER_MAXSPEED 2
#include <stdint.h>

#define FIXEDBITS 15

#define FRACMASK ((1<<FIXEDBITS) - 1)

#define FIXED(x) ((int32_t)( x * (float)(1<<FIXEDBITS)))


struct EdgeCutter_Params
{
	unsigned char mode;
	unsigned char speed;
};

struct EdgeCutter_Settings
{
	unsigned char SlowSpeedMult;
};

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


struct EdgeCutter_Envelope
{
	unsigned char A;
	unsigned char D;
	unsigned char S;
	unsigned char R;
	unsigned char Curvature;

	unsigned char TriggerState;
	
	int32_t LinearOutput;
	int32_t CurvedOutput;

	int State;
	
	unsigned char Gates[5];
	unsigned char StateLeds[14];

	int32_t AttackStart;
	int32_t DecayStart;
	int32_t ReleaseStart;
	
	int32_t AttackProgress;
	int32_t DecayProgress;
	int32_t ReleaseProgress;


	int32_t Current;
	int32_t CurrentTarget;
};

#ifdef __cplusplus
extern "C"
{
#endif
	extern int EdgeCutter_GetEnv(struct EdgeCutter_Envelope *Env, struct EdgeCutter_Params *Params);
	extern void EdgeCutter_Init(struct EdgeCutter_Envelope *Env);
	extern void EdgeCutter_Trigger(struct EdgeCutter_Envelope *Env, unsigned char N, struct EdgeCutter_Params *Params);
	extern void EdgeCutter_LoadSettings(struct EdgeCutter_Settings *settings, struct EdgeCutter_Params *params);
	extern void EdgeCutter_ValidateParams(struct EdgeCutter_Params *params);
#ifdef __cplusplus
}
#endif
