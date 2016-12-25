
#define EDGECUTTER_MAXMODE 3
#define EDGECUTTER_MAXSPEED 2

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
	
	int LinearOutput;
	float CurvedOutput;

	int State;
	
	unsigned char Gates[5];
	unsigned char StateLeds[14];

	float AttackStart;
	float DecayStart;
	float ReleaseStart;
	
	float AttackProgress;
	float DecayProgress;
	float ReleaseProgress;


	float Current;
	float CurrentTarget;
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
