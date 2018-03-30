/* ###################################################################
 **     Filename    : main.c
 **     Project     : FlaskScreen
 **     Processor   : MK22FN256VDC12
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2017-11-19, 03:02, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
#include "OLED_RESET.h"
#include "WAIT1.h"
#include "GI2C1.h"
#include "CODEC_PDN.h"
#include "I2C1.h"
#include "SDA1.h"
#include "BitIoLdd1.h"
#include "SCL1.h"
#include "BitIoLdd2.h"
#include "AD1.h"
#include "AdcLdd1.h"
#include "LED_UP.h"
#include "LED_DOWN.h"
#include "LED_MIDDLE.h"
#include "BUTTON_UP.h"
#include "BUTTON_MIDDLE.h"
#include "BUTTON_DOWN.h"
#include "OLED_DC.h"
#include "SM1.h"

/* User includes (#include below this line is not maintained by Processor Expert) */



//Display defines
extern unsigned char GetChar(int  c);


typedef struct denoise_state_t
{
	int counter;
	int down;
	unsigned char pressed:4;
	unsigned char released:4;
	int longpressed;
	int lastcounter;
} denoise_state_t;

denoise_state_t state_up;
denoise_state_t state_middle;
denoise_state_t state_down;

#include "MasterChorus.h"
#include "delay.h"

void denoise(int sw_down,  denoise_state_t *state);
int islongpress( denoise_state_t *state);
int pressed( denoise_state_t *state);

#define LONGPRESSCYCLES 2000

int islongpress(  denoise_state_t *state)
{
	if (state->longpressed >= LONGPRESSCYCLES)
	{
		state->longpressed  = 0;
		return 1;
	}
	return 0;
}

int pressed( denoise_state_t *state)
{
	if (state->pressed == 1)
	{
		state->pressed = 0;
		return 1;
	}
	return 0;
}

int released( denoise_state_t *state)
{
	if (state->released== 1)
	{
		state->released = 0;
		return 1;
	}
	return 0;
}

void denoise(int sw_down,  denoise_state_t *state)
{
	if (sw_down)
	{
		state->counter++;
	}
	else
	{
		state->counter--;
	}

	//state->pressed = 0;
	//state->released = 0;

	if (state->counter < 2)
	{
		if (state->lastcounter == 2)
		{
			state->pressed = 1;
		}
		state->counter = 1;
		state->down = 1;
	}
	else
	{
		if (state->counter > 30)
		{
			if (state->lastcounter == 30)
			{
				state->released = 1;
			}
			state->counter = 31;
			state->down = 0;
		}
	}

	if (state->down > 0)
	{
		state->longpressed++;
	}
	else
	{
		state->longpressed = 0;
	}

	state->lastcounter = state->counter;

	if (state->longpressed >= LONGPRESSCYCLES)
	{
		state->longpressed = LONGPRESSCYCLES;
	}
}


int32_t Param[4];

int BlockT = 0;
#include "Verb1.h"
enum
{
	MODE_DELAY,
	MODE_XDELAY,
	MODE_CHORUS,
	MODE_FLANGER,
	MODE_VERB1,
	MODE_FREQCOUNTER,
	__MODE_COUNT,
	MODE_PLATINUMCLIP
};

const char Names[__MODE_COUNT][10]=
{
		"  Delay   ",
		" X-Delay  ",
		"  Chorus  ",
		" Flanger  ",
		"  Verb 1  ",
		" FreqCount"
};
enum
{
	DISP_DEFAULT,
	DISP_FREQCOUNT
};
int displaymode[__MODE_COUNT] = {DISP_DEFAULT,DISP_DEFAULT,DISP_DEFAULT,DISP_DEFAULT,DISP_DEFAULT,DISP_FREQCOUNT};

const char ParamLabel[__MODE_COUNT][4][8] =
{
		{"Wet/Dry ","Feedback","Length  ","Stereo  "},
		{"Wet/Dry ","Feedback","Length  ","X-over  "},
		{"Wet/Dry ","Speed   ","Phasing ","        "},
		{"Wet/Dry ","Speed   ","Phasing ","Feedback"},
		{"Wet/Dry ","Length  ","Feedback","Damping "},
		{"Wet/Dry ","Length  ","Feedback","Damping "}

};

int CurrentMode  =1;
#define FREQCNT 40
typedef struct FreqCount_t
{
	uint32_t currfreq;
	uint32_t spacing[FREQCNT];
	uint32_t reffreq;
	int32_t lastval;
	uint32_t timesincelast;
	float fcurfreq;
	float freffreq;
	int idx;
} FreqCount_t;

void InitFreqCount(FreqCount_t *f)
{
	f->lastval = 0;
	f->reffreq = 1;
	f->currfreq = 1;
	f->timesincelast = 0;
	for (int i = 0;i<FREQCNT;i++) f->spacing[i] =10;
	f->idx = 0;
}

#include "ak4558.h"

void ProcessFreqCount(FreqCount_t *f,int32_t *in, int32_t *out)
{

	int nSamples = AUDIO_BUFFER_SIZE;


	while (nSamples--)
	{
		int32_t left = *in++;
		int32_t right = *in++;

		if (left>=0 && f->lastval <0)
		{
			f->spacing[f->idx] = f->timesincelast;
			f->idx = (f->idx + 1) % FREQCNT;
			f->timesincelast = 0;
			f->currfreq  =0 ;
			for(int i =0;i<FREQCNT;i++)
			{
				f->currfreq += f->spacing[i];
			}
			;

			f->fcurfreq = (48000.0f*(float)FREQCNT )/ (float)f->currfreq;

		}
		f->lastval = left;
		f->timesincelast++;
		*out++ = left;
		*out++ = right;
	}
}

typedef struct EffectsOverlay
{
	union
	{
		Delay Delay;
		XDelay XDelay;
		StereoChorus_t Chorus;
		Flanger_t Flanger;
		Verb1 Verb1;
		FreqCount_t Freq;
	};
} EffectsOverlay;

EffectsOverlay TheSet;
void DoEnterPress()
{
	switch(CurrentMode)
	{
	case MODE_FREQCOUNTER:
		TheSet.Freq.freffreq = TheSet.Freq.fcurfreq;
		break;
	case MODE_VERB1:
		break;

	case MODE_CHORUS:

		break;
	case MODE_DELAY:
		break;
	case MODE_XDELAY:
		break;
	case MODE_FLANGER:
		break;
	case MODE_PLATINUMCLIP:
		break;
	}
}

void SwitchMode(int newmode)
{
	if (newmode != CurrentMode)
	{
		CurrentMode = -1;
		switch(newmode)
		{
		case MODE_FREQCOUNTER:
			InitFreqCount(&TheSet.Freq);
			break;
		case MODE_VERB1:
			InitVerb1(&TheSet.Verb1);
			break;

		case MODE_CHORUS:

			InitChorus(&TheSet.Chorus);
			break;
		case MODE_DELAY:
			InitDelay(&TheSet.Delay);
			break;
		case MODE_XDELAY:
			InitXDelay(&TheSet.XDelay);
			break;
		case MODE_FLANGER:
			InitFlanger(&TheSet.Flanger);
			break;
		case MODE_PLATINUMCLIP:
			break;
		}
		CurrentMode = newmode;

	}
}


void NextBlock(int32_t *in, int32_t *out)
{
	BlockT++;
	denoise(BUTTON_UP_GetVal(0), &state_up);
	denoise(BUTTON_DOWN_GetVal(0), &state_down);
	denoise(BUTTON_MIDDLE_GetVal(0), &state_middle);

	if (pressed(&state_up)== 1)
	{
		SwitchMode((CurrentMode + 1) % __MODE_COUNT);
	}
	if (pressed(&state_down)== 1)
	{
		SwitchMode((CurrentMode -1+__MODE_COUNT) % __MODE_COUNT);
	}
	if (pressed(&state_middle))
	{
		DoEnterPress();
	}


	switch(CurrentMode)
	{
	//ProcessDelay(&TheSet.Delay,in, out);

	case MODE_VERB1:
		ProcessVerb1(&TheSet.Verb1, in, out);
		break;
	case MODE_FLANGER:
		Flanger_SetMix(&TheSet.Flanger, Param[0]/65535.0);
		Flanger_SetSpeed(&TheSet.Flanger, Param[1]/65535.0);
		Flanger_SetShift(&TheSet.Flanger, Param[2]/65535.0);
		Flanger_SetFeedback(&TheSet.Flanger, Param[3]/65535.0);
		ProcessFlanger(&TheSet.Flanger, in, out);
		break;

	case MODE_CHORUS:
		Chorus_SetMix(&TheSet.Chorus, Param[0]/65535.0);
		Chorus_SetSpeed(&TheSet.Chorus, Param[1]/65535.0);
		Chorus_SetShift(&TheSet.Chorus, Param[2]/65535.0);
		ProcessChorus(&TheSet.Chorus, in, out);
		break;
	case MODE_DELAY:
		ProcessDelay(&TheSet.Delay,in, out);
		break;
	case MODE_XDELAY:
		TheSet.XDelay.Wet = Param[0]/65535.0;
		TheSet.XDelay.Feedback = Param[1];
		ProcessXDelay(&TheSet.XDelay,in, out);
		break;
	case MODE_FREQCOUNTER:
		ProcessFreqCount(&TheSet.Freq,in,out);
		break;
	}
}
int measuring =0 ;
#include "ak4558.h"

void CheckAudio()
{
	if (measuring == 0)
	{
		measuring = 1;
		AD1_Measure(0);

	}

	if (audio_buffers_fresh)
	{
		audio_buffers_fresh = 0;

		int32_t* inbuf = audio_in_buffer;
		int32_t* outbuf = audio_out_buffer;
		NextBlock(inbuf, outbuf);


	}
}
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
#include "OLED.h"
uint16_t buffer2[64];


void DoParam(int base, int32_t T1, int idx, int coffs, int absy)
{
	int b2 = 0;
	for (int y = 0;y<16;y++)
	{
		int32_t t2 = T1;
		b2 = y/2;
		if (y%2 == 0) b2+=32;
		if (T1>0)
		{
			if ((y ==0) || (y==15)) t2 = 0;

			int charc = ( - coffs/7);
			int charx = (-coffs+(14*8))%7 ;
			int chary = (12-y);
			if (chary>=0 && chary < 10 && charc<8 && charc>=0)
			{
				int c = ParamLabel[CurrentMode][idx][charc]-32;
				int G =GetChar(c*10 + chary);
				int mask = (1<<charx );
				if ((G & mask)==0)
				{
					t2 = 0;
				}

			}
			buffer2[base + b2] = t2;
		}
		else
		{
			//if ((y ==0) || (y==15)) t2 = RGB(30,30,30);

			int charc = ( - absy/7);
			int charx = (-absy+(14*8))%7 ;
			int chary = (12-y);
			if (chary>=0 && chary < 10 && charc<8 && charc>=0)
			{
				int c = ParamLabel[CurrentMode][idx][charc]-32;
				int G =GetChar(c*10 + chary);
				int mask = (1<<charx );
				if ((G & mask)==0)
				{
					t2 = RGB(8,8,8);
				}

			}

			buffer2[base + b2] = t2;

		}


	}
}

char* itoa(int i, char b[]){
	char const digit[] = "0123456789";
	for (int i =0;i<10;i++)
	{
		b[i] = ' ';
	}
	char* p = b;
	if(i<0){
		*p++ = '-';
		i *= -1;
	}
	int shifter = i;
	do{ //Move to where representation ends
		++p;
		shifter = shifter/10;
	}while(shifter);
	//*p = '\0';
	do{ //Move back, inserting digits as u go
		*--p = digit[i%10];
		i = i/10;
	}while(i);
	return b;
}
void PrintLine(char*ccc)
{
	for (int x =0;x<16;x++)
			{
				if (x>=2 && x < 12)
				{
					int chary = x-2;
					for(int b = 0;b<64;b++)
					{
						int b2;
						if (b%2 == 1) b2 = b/2;else b2 = b/2 + 32;
						int charc = b/7;
						int charx = b%7;
						if (charc<10)
						{
							int c = ccc[charc]-32;
							int G = GetChar(c*10 + chary);
							int mask = (1<<charx );
							if ((G & mask)==0)
							{
								buffer2[b2] = RGB(255,255,255);
							}
							else
							{
								buffer2[b2] = RGB(0,0,0);

							}
						}
					}
				}
				else
				{
					for(int x =0;x<64;x++){buffer2[x]= 0;};

				}
				CheckAudio();
				sent =0;
				SM1_SendBlock(SM1_DeviceData, buffer2, 2*64);
				while(sent ==0 ){CheckAudio();};
			}

};
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/

	SwitchMode(MODE_FREQCOUNTER);
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/
	ak4558_init();
	/* Write your code here */
	/* For example: for(;;) { } */
	OledInit();
	int t = 0;
	for(;;) {
		t++;

		CheckAudio();

		SetWindow(0,0,95,63);
		CheckAudio();

		OLED_DC_SetVal(0);

		int Ap = ((0xffff-Param[0]) * (79))/65535;
		int Bp = ((0xffff-Param[1]) * 79)/65535;
		int Cp = ((0xffff-Param[2]) * 79)/65535;
		int Dp = ((0xffff-Param[3]) * 79)/65535;
		PrintLine(Names[CurrentMode]);

		for(int x =0;x<64;x++){buffer2[x]= 0;};
		switch(displaymode[CurrentMode])
		{
		case DISP_DEFAULT:
		{
			for (int x = 16 ;x<96;x++)
			{
				CheckAudio();

				int c =0 ;
				int r = ((BlockT + x)%96>48)?255:0;
				uint16_t base = RGB(r,r,r);
				int x2 = x-16;
				uint8_t A1 = x2>=Ap?255:0;
				uint8_t B1 = x2>=Bp?255:0;
				uint8_t C1 = x2>=Cp?255:0;
				uint8_t D1 = x2>=Dp?255:0;

				uint16_t T1 = RGB(0,A1/2,A1);
				uint16_t T2 = RGB(0,B1,B1/2);
				uint16_t T3 = RGB(0,C1/2,C1);
				uint16_t T4 = RGB(0,D1,D1/2);

				DoParam(0,T1,0,Ap-x2,-x2);
				DoParam(8, T2,1,Bp-x2,-x2);
				DoParam(16,T3,2,Cp-x2,-x2);
				DoParam(24,T4,3,Dp-x2,-x2);
				CheckAudio();
				sent =0;
				SM1_SendBlock(SM1_DeviceData, buffer2, 2*64);
				while(sent ==0 ){CheckAudio();};
			}

		}
		break;
		case DISP_FREQCOUNT:
		{
			char freqdisp1[20];
			char freqdisp2[20];
			char freqdisp3[20];
			char freqdisp4[20];
			float ratio =1;
			if (TheSet.Freq.freffreq > 0)
			{
				ratio = TheSet.Freq.fcurfreq/ TheSet.Freq.freffreq;
			}
			itoa((int)(TheSet.Freq.fcurfreq), freqdisp1);
			itoa((int)(TheSet.Freq.freffreq), freqdisp2);
			itoa((int)(ratio*1000.0), freqdisp3);
			itoa((int)((1.0f/ratio)*1000.0), freqdisp4);
			PrintLine(freqdisp1);
			PrintLine(freqdisp2);
			PrintLine(freqdisp3);
			PrintLine(freqdisp4);



			for (int x = 16+16*4 ;x<96;x++)
			{
				CheckAudio();

				SM1_SendBlock(SM1_DeviceData, buffer2, 2*64);
				while(sent ==0 ){CheckAudio();};
			}
			break;
		}
		}

	}
	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.5 [05.21]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
