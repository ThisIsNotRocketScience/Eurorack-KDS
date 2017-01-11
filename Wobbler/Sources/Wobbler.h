
#define WOBBLER_GATECOUNTDOWN 100
#include <stdint.h>


struct Wobbler_Params
{
	unsigned char T;
};
	
struct Wobbler_Settings
{
	unsigned char SlowSpeedMult;
};


struct Wobbler_LFO
{
	unsigned char Speed;
	unsigned char Shape;
	unsigned char Mod;
	unsigned char Phasing;

	uint32_t Phase1;
	uint32_t OldPhase1;
	uint32_t Phase2;
	uint32_t OldPhase2;


	int32_t OutputPhased;
	int32_t Output;

	unsigned char Gate[2];
	unsigned char Led[12];

	uint32_t PhasedCountdown;
};

#ifdef __cplusplus
extern "C"
{
#endif
	extern int Wobbler_Get(struct Wobbler_LFO *LFO, struct Wobbler_Params *Params);
	extern void Wobbler_Init(struct Wobbler_LFO *LFO);
	extern void Wobbler_Trigger(struct Wobbler_LFO *LFO, unsigned char N, struct Wobbler_Params *Params);
	extern void Wobbler_LoadSettings(struct Wobbler_Settings *settings, struct Wobbler_Params *params);
	extern void Wobbler_ValidateParams(struct Wobbler_Params *params);

	extern int32_t LERP(int32_t *V, int total, int fade);
#ifdef __cplusplus
}
#endif
