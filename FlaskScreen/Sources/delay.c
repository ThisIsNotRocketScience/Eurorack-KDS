#include "delay.h"
#include "ak4558.h"
extern int32_t Param[4];

void SetupXDelayLengths(XDelay *D)
{
	float Cross = ((Param[3]/32768.0f)-1.0) * (200000-65536);
	float leftmul = 1-Cross;
	float rightmul = 1+Cross;
	if (rightmul > 1) rightmul = 1;
	if (leftmul > 1) leftmul = 1;
	D->newleft = (10+ ((Param[2]* MAX_WG_DELAY)/(65536.0))*leftmul );
	D->newright= (10+ ((Param[2]* MAX_WG_DELAY)/(65536.0))*rightmul );
	D->delaylenleft = D->delaylenleft*0.999 + D->newleft * 0.001;
	D->delaylenright = D->delaylenright *0.999 + D->newright * 0.001;
}

void ProcessXDelay(XDelay* D, int32_t *in, int32_t *out)
{
	SetupXDelayLengths(D);
	uint16_t  wet= Param[0];
	uint16_t dry = 0xffff-wet;

	//float Dry = 1.0f - D->Wet;
	for (int i =0 ;i<AUDIO_BUFFER_SIZE;i++)
	{
		int32_t lin = (*in++)>>16;
		int32_t rin = (*in++)>>16;

		int32_t monoin =  (rin + lin)/2;

		int32_t lout = WaveGuide_GetI16(&D->LineL, D->delaylenleft);
		int32_t rout = WaveGuide_GetI16(&D->LineR, D->delaylenright);

		WaveGuide_Push(&D->LineL, (monoin +( ((rout) * D->Feedback)>>16))<<16);
		WaveGuide_Push(&D->LineR, (monoin +( ((lout) * D->Feedback)>>16))<<16);

		*out++ = (monoin * dry) + (lout * wet );
		*out++ = (monoin * dry) + (rout * wet) ;
	}


}
void InitXDelay(XDelay *D)
{
	WaveGuide_Init(&D->LineL);
	WaveGuide_Init(&D->LineR);
	SetupXDelayLengths(D);
	D->delaylenleft = D->newleft;
	D->delaylenright = D->newright;
}

void SetupDelayLengths(Delay *D)
{
	D->newleft = (10+ (Param[2]* MAX_WG_DELAY_LONG)/68936.0f);
	D->newright= (1000+ (Param[2]* (MAX_WG_DELAY_LONG-1000))/65936.0f);

	D->delaylenleft = D->delaylenleft*0.999 + D->newleft * 0.001;
	D->delaylenright = D->delaylenright *0.999 + D->newright * 0.001;

}


void InitDelay(Delay *D)
{
	D->delayposleft =0 ;
	D->delayposcenter =0 ;
	D->delayposright =0 ;
	D->delaylenleft = (44100*200)/1000;
	D->delaylenright = (44100*184)/1000;
	D->wet = 0x8000;
	D->dry = 0x8000;
	D->feedback = 0x4000;
	D->lin = 0;
	D->rin =0;
	D->mono = 1;
	WaveGuideLong_Init(&D->Line);
	SetupDelayLengths(D);
	D->delaylenleft = D->newleft;
	D->delaylenright = D->newright;


}

void ProcessDelay(Delay* D, int32_t *in, int32_t *out)
{
	D->wet = Param[0];
	D->dry = 0xffff-D->wet;
	D->feedback = Param[1]>>1;
	SetupDelayLengths(D);
	D->mono = Param[3]>0x8000?1:0;
	for (int i =0 ;i<AUDIO_BUFFER_SIZE;i++)
	{
		D->lin = (*in++)>>16;
		D->rin = (*in++)>>16;

		if (D->mono) D->rin = D->lin = (D->rin + D->lin)/2;

		int32_t lout = WaveGuideLong_GetI16(&D->Line, D->delaylenleft);
		int32_t rout = WaveGuideLong_GetI16(&D->Line, D->delaylenright);

		WaveGuideLong_Push(&D->Line, ((D->rin+D->lin)/2 +( ((lout+rout) * D->feedback)>>16))<<16);

		*out++ = (D->lin * D->dry) + (lout * D->wet );
		*out++ = (D->rin * D->dry) + (rout * D->wet) ;
	}
}
