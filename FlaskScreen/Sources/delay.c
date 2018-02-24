#include "delay.h"
#include "ak4558.h"
extern int32_t Param[4];


void ProcessXDelay(XDelay* D, int32_t *in, int32_t *out)
{
	float Cross = ((Param[3]/32768.0f)-1.0) * (200000-65536);

	float newleft = (10+ (Param[2]* MAX_WG_DELAY)/(200000.0f+Cross));
	float newright= (10+ (Param[2]* MAX_WG_DELAY)/(200000.0f-Cross));
	uint16_t  wet= Param[0];
	uint16_t dry = 0xffff-wet;

	D->delaylenleft = D->delaylenleft*0.999 + newleft * 0.001;
	D->delaylenright = D->delaylenright *0.999 + newright * 0.001;
float Dry = 1.0f - D->Wet;
	for (int i =0 ;i<AUDIO_BUFFER_SIZE;i++)
		{
			int32_t lin = (*in++)>>16;
			int32_t rin = (*in++)>>16;

			int32_t monoin =  (rin + lin)/2;

			int32_t lout = WaveGuide_GetI16(&D->LineL, D->delaylenleft);
			int32_t rout = WaveGuide_GetI16(&D->LineR, D->delaylenright);

			//int32_t lout = D->buffer[(D->delayposcenter-D->delaylenleft+MAX_WG_DELAY_LONG+MAX_WG_DELAY_LONG)%MAX_WG_DELAY_LONG];
			//int32_t rout = D->buffer[(D->delayposcenter-D->delaylenright+MAX_WG_DELAY_LONG+MAX_WG_DELAY_LONG)%MAX_WG_DELAY_LONG];
			WaveGuide_Push(&D->LineL, (monoin +( ((rout) * D->Feedback)>>16))<<16);
			WaveGuide_Push(&D->LineR, (monoin +( ((lout) * D->Feedback)>>16))<<16);


		//	D->delayposcenter = (D->delayposcenter+1) % delaylen;

			*out++ = (monoin * dry) + (lout * wet );
			*out++ = (monoin * dry) + (rout * wet) ;
		}


}
void InitXDelay(XDelay *D)
{
	WaveGuide_Init(&D->LineL);
	WaveGuide_Init(&D->LineR);
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


}


void ProcessDelay(Delay* D, int32_t *in, int32_t *out)
{
	D->wet = Param[0];
	D->dry = 0xffff-D->wet;
	D->feedback = Param[1]>>1;
	float newleft = (10+ (Param[2]* MAX_WG_DELAY_LONG)/68936.0f);
	float newright= (10+ (Param[2]* MAX_WG_DELAY_LONG)/65936.0f);
		D->delaylenleft = D->delaylenleft*0.999 + newleft * 0.001;
	D->delaylenright = D->delaylenright *0.999 + newright * 0.001;
	D->mono = Param[3]>0x8000?1:0;
	for (int i =0 ;i<AUDIO_BUFFER_SIZE;i++)
	{
		D->lin = (*in++)>>16;
		D->rin = (*in++)>>16;

		if (D->mono) D->rin = D->lin = (D->rin + D->lin)/2;

		int32_t lout = WaveGuideLong_GetI16(&D->Line, D->delaylenleft);
		int32_t rout = WaveGuideLong_GetI16(&D->Line, D->delaylenright);

		//int32_t lout = D->buffer[(D->delayposcenter-D->delaylenleft+MAX_WG_DELAY_LONG+MAX_WG_DELAY_LONG)%MAX_WG_DELAY_LONG];
		//int32_t rout = D->buffer[(D->delayposcenter-D->delaylenright+MAX_WG_DELAY_LONG+MAX_WG_DELAY_LONG)%MAX_WG_DELAY_LONG];
		WaveGuideLong_Push(&D->Line, ((D->rin+D->lin)/2 +( ((lout+rout) * D->feedback)>>16))<<16);


	//	D->delayposcenter = (D->delayposcenter+1) % delaylen;

		*out++ = (D->lin * D->dry) + (lout * D->wet );
		*out++ = (D->rin * D->dry) + (rout * D->wet) ;
	}
}
