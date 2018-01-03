/* ###################################################################
 **     Filename    : main.c
 **     Project     : BigFish
 **     Processor   : MK22FN256VLH12
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2017-05-24, 01:44, # CodeGen: 0
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

#include "ADMAIN.h"
#include "OLED_DC.h"
#include "OLED_CLK.h"
#include "OLED_DAT.h"
#include "OLED_CS.h"
#include "OLED_RESET.h"

/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "GATE_TRIGGER.h"
#include "GATE_BUTTON.h"
#include "FILTER_BUTTON.h"
#include "ACCENT_TRIGGER.h"
#include "Init_Config.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

#include "LEDS.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define __max(a,b) (((a)>(b))?(a):(b))

#include "ak4558.h"

#define LONGPRESSCYCLES 2000

struct denoise_state_t
{
	int counter;
	int down;
	unsigned char pressed:4;
	unsigned char released:4;
	int longpressed;
	int lastcounter;
};


int islongpress( struct denoise_state_t *state)
{
	if (state->longpressed >= LONGPRESSCYCLES)
	{
		state->longpressed  = 0;
		return 1;
	}
	return 0;
}

int pressed(struct denoise_state_t *state)
{
	if (state->pressed == 1)
	{
		state->pressed = 0;
		return 1;
	}
	return 0;
}

int released(struct denoise_state_t *state)
{
	if (state->released== 1)
	{
		state->released = 0;
		return 1;
	}
	return 0;
}

void denoise(int sw_down, struct denoise_state_t *state)
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



#define BIGFISH_LEDS 16
const int SHAPELEDS[7+1] ={

		(7-3)+8,
		(7-1)+8,

		(7-0)+8,
		(7-2)+8,
		(7-4)+8,

		(7-6)+8,
		(7-5)+8,


		BIGFISH_LEDS+1};
#define TRIGGERBUTTONLED (0+8)
#define FILTERBUTTONLED 4
const int FILTERLEDS[5] = {7-1, 7-2, 7-4, 7-6, 7-5};
#define ENVELOPELED 0
uint8_t LEDS[BIGFISH_LEDS + 2] ={0};
uint8_t LEDTARGET[BIGFISH_LEDS + 2]={0} ;
void UpdateLeds()
{
	for (int i = 0; i < BIGFISH_LEDS; i++)
	{
		if (LEDTARGET[i] > LEDS[i])
		{
			int newL = LEDS[i] +2;
			if(newL>255) newL = 255;

			LEDS[i]++;
		}
		else
		{
			if (LEDTARGET[i] < LEDS[i])
			{
				int newL = LEDS[i] -2;
				if(newL<0) newL = 0;
				LEDS[i] = newL;
			}
			if (LEDTARGET[i] < LEDS[i])
			{
				int newL = LEDS[i] -2;
				if(newL<0) newL = 0;
				LEDS[i]--;
			}
		}
	}
}



extern "C"
{
extern void AudioInit();

static uint8_t bufdat[1];

int sent =0;

void ADCMUXING();
int Measuring = 0;
int MainMeasuring = 0;

extern void DoProcessing(int32_t *In, int32_t *Out1,int32_t *Out2, uint32_t len);



void NextBlock(int32_t *input, int32_t *output)
{

	int32_t OutMain[32];
	int32_t OutOsc[32] ;
	int32_t InFilt[32] ;
	for(int i =0 ;i<32;i++)
	{
		InFilt[i] = input[i*2];
	}
	DoProcessing(InFilt, OutMain, OutOsc,32);
	for(int i =0 ;i<32;i++)
	{
		output[i * 2 + 1] = OutMain[i] << 13;
		output[i * 2 ] = OutOsc[i] << 13;
	}


}

void DoProcessing(int32_t *In, int32_t *Out1,int32_t *Out2, uint32_t len)
{
	for(int i =0 ;i<len;i++)
	{
		*Out1++ = 0;
		*Out2++ = 0;
	}
	//	BigFish_GenerateBlock(&Fish, In, Out1,Out2,len);
}



int Tick10Khz =0 ;
struct denoise_state_t filterbutton_state = {0};

void Timer10khz()
{

	int filterbut = FILTER_BUTTON_GetVal( FILTER_BUTTON_DeviceData);
	denoise(filterbut, &filterbutton_state);

	if (pressed(&filterbutton_state))
	{
		LEDTARGET[FILTERBUTTONLED] = LEDS[FILTERBUTTONLED] = 255;
	}
	else
	{
		LEDTARGET[FILTERBUTTONLED] = 0;
	}

	for (int i =0 ;i<7;i++)
	{
		LEDTARGET[SHAPELEDS[i]] = 0;
	}


	UpdateLeds();
	LEDS_Update(LEDS, 16, 0, 0);
	Tick10Khz++;
}



//Display defines
#define VCCSTATE SSD1331_SWITCHCAPVCC
#define OLED_WIDTH     96
#define OLED_HEIGHT    64

#define RGB(R,G,B)  (((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))
enum Color {
	BLACK     = RGB(  0,  0,  0), // black
	GRAY      = RGB(192,192,192), // gray
	WHITE     = RGB(255,255,255), // white
	RED       = RGB(255,  0,  0), // red
	PINK      = RGB(255,192,203), // pink
	YELLOW    = RGB(255,255,  0), // yellow
	GOLDEN    = RGB(255,215,  0), // golden
	BROWN     = RGB(128, 42, 42), // brown
	BLUE      = RGB(  0,  0,255), // blue
	CYAN      = RGB(  0,255,255), // cyan
	GREEN     = RGB(  0,255,  0), // green
	PURPLE    = RGB(160, 32,240), // purple
};
#define DRAW_LINE                       0x21
#define DRAW_RECTANGLE                  0x22
#define COPY_WINDOW                     0x23
#define DIM_WINDOW                      0x24
#define CLEAR_WINDOW                    0x25
#define FILL_WINDOW                     0x26
#define DISABLE_FILL                    0x00
#define ENABLE_FILL                     0x01
#define CONTINUOUS_SCROLLING_SETUP      0x27
#define DEACTIVE_SCROLLING              0x2E
#define ACTIVE_SCROLLING                0x2F

#define SET_COLUMN_ADDRESS              0x15
#define SET_ROW_ADDRESS                 0x75
#define SET_CONTRAST_A                  0x81
#define SET_CONTRAST_B                  0x82
#define SET_CONTRAST_C                  0x83
#define MASTER_CURRENT_CONTROL          0x87
#define SET_PRECHARGE_SPEED_A           0x8A
#define SET_PRECHARGE_SPEED_B           0x8B
#define SET_PRECHARGE_SPEED_C           0x8C
#define SET_REMAP                       0xA0
#define SET_DISPLAY_START_LINE          0xA1
#define SET_DISPLAY_OFFSET              0xA2
#define NORMAL_DISPLAY                  0xA4
#define ENTIRE_DISPLAY_ON               0xA5
#define ENTIRE_DISPLAY_OFF              0xA6
#define INVERSE_DISPLAY                 0xA7
#define SET_MULTIPLEX_RATIO             0xA8
#define DIM_MODE_SETTING                0xAB
#define SET_MASTER_CONFIGURE            0xAD
#define DIM_MODE_DISPLAY_ON             0xAC
#define DISPLAY_OFF                     0xAE
#define NORMAL_BRIGHTNESS_DISPLAY_ON    0xAF
#define POWER_SAVE_MODE                 0xB0
#define PHASE_PERIOD_ADJUSTMENT         0xB1
#define DISPLAY_CLOCK_DIV               0xB3
#define SET_GRAy_SCALE_TABLE            0xB8
#define ENABLE_LINEAR_GRAY_SCALE_TABLE  0xB9
#define SET_PRECHARGE_VOLTAGE           0xBB

#define SET_V_VOLTAGE                   0xBE

void SSD1331_draw_point(int x, int y, unsigned short hwColor) ;

void SSD1331_clear()
{
	int i, j;
	for(i = 0; i < OLED_WIDTH; i++)
	{
		for(j = 0; j < OLED_WIDTH; j++)
		{
			SSD1331_draw_point(i, j, 0);
		}
	}
}

void Wait()
{
	OLED_CLK_SetVal(0);
	WAIT1_WaitCycles(10);
	//WAIT1_WaitCycles(0);
	OLED_CLK_ClrVal(0);
	//WAIT1_WaitCycles(1);
	//WAIT1_WaitCycles(0);

}
void ShiftByte(uint8_t dat)
{

	if ((dat&0x80)==0x80) OLED_DAT_SetVal(0);else OLED_DAT_ClrVal(0);
	Wait();
	if ((dat&0x40)==0x40) OLED_DAT_SetVal(0);else OLED_DAT_ClrVal(0);
	Wait();
	if ((dat&0x20)==0x20) OLED_DAT_SetVal(0);else OLED_DAT_ClrVal(0);
	Wait();
	if ((dat&0x10)==0x10) OLED_DAT_SetVal(0);else OLED_DAT_ClrVal(0);
	Wait();
	if ((dat&0x8)==0x8) OLED_DAT_SetVal(0);else OLED_DAT_ClrVal(0);
	Wait();
	if ((dat&0x4)==0x4) OLED_DAT_SetVal(0);else OLED_DAT_ClrVal(0);
	Wait();
	if ((dat&0x2)==0x2) OLED_DAT_SetVal(0);else OLED_DAT_ClrVal(0);
	Wait();
	if ((dat&0x1)==0x1) OLED_DAT_SetVal(0);else OLED_DAT_ClrVal(0);
	Wait();

}

void spidata(uint8_t dat) {
	//  OLED_DC_SetVal(0);
	// sent = 0;
	bufdat[0] = dat;
	//SM1_SendBlock(SM1_DeviceData, bufdat, 1);
	//while (sent == 0){}
	ShiftByte(bufdat[0]);

}

void command(uint8_t cmd) {
	OLED_DC_ClrVal(0);


	sent = 0;
	bufdat[0] = cmd;
	//SM1_SendBlock(SM1_DeviceData, bufdat, 1) ;
	//while (sent == 0){}
	ShiftByte(bufdat[0]);

}
#define WIDTH 96
#define HEIGHT 64
// Timing Delays
#define SSD1331_DELAYS_HWFILL		(3)
#define SSD1331_DELAYS_HWLINE       (1)

// SSD1331 Commands
#define SSD1331_CMD_DRAWLINE 		0x21
#define SSD1331_CMD_DRAWRECT 		0x22
#define SSD1331_CMD_FILL 			0x26
#define SSD1331_CMD_SETCOLUMN 		0x15
#define SSD1331_CMD_SETROW    		0x75
#define SSD1331_CMD_CONTRASTA 		0x81
#define SSD1331_CMD_CONTRASTB 		0x82
#define SSD1331_CMD_CONTRASTC		0x83
#define SSD1331_CMD_MASTERCURRENT 	0x87
#define SSD1331_CMD_SETREMAP 		0xA0
#define SSD1331_CMD_STARTLINE 		0xA1
#define SSD1331_CMD_DISPLAYOFFSET 	0xA2
#define SSD1331_CMD_NORMALDISPLAY 	0xA4
#define SSD1331_CMD_DISPLAYALLON  	0xA5
#define SSD1331_CMD_DISPLAYALLOFF 	0xA6
#define SSD1331_CMD_INVERTDISPLAY 	0xA7
#define SSD1331_CMD_SETMULTIPLEX  	0xA8
#define SSD1331_CMD_SETMASTER 		0xAD
#define SSD1331_CMD_DISPLAYOFF 		0xAE
#define SSD1331_CMD_DISPLAYON     	0xAF
#define SSD1331_CMD_POWERMODE 		0xB0
#define SSD1331_CMD_PRECHARGE 		0xB1
#define SSD1331_CMD_CLOCKDIV 		0xB3
#define SSD1331_CMD_PRECHARGEA 		0x8A
#define SSD1331_CMD_PRECHARGEB 		0x8B
#define SSD1331_CMD_PRECHARGEC 		0x8C
#define SSD1331_CMD_PRECHARGELEVEL 	0xBB
#define SSD1331_CMD_VCOMH 			0xBE



void SSD1331_draw_point(int x, int y, unsigned short hwColor) {

	unsigned char buffer[2];

	if(x >= OLED_WIDTH || y >= OLED_HEIGHT)
	{
		return;
	}

	OLED_CS_ClrVal(0);


	  // set x and y coordinate
	  command(SSD1331_CMD_SETCOLUMN);
	  command(x);
	  command(WIDTH-1);

	  command(SSD1331_CMD_SETROW);
	  command(y);
	  command(HEIGHT-1);

	OLED_DC_SetVal(OLED_DC_DeviceData);
	WAIT1_WaitCycles(1000);
	//  OLED_CS_ClrVal(OLED_CS_DeviceData);
	buffer[0] = (hwColor & 0xFF00) >> 8;
	buffer[1] = hwColor & 0xFF;


	spidata(buffer[0]);
	spidata(buffer[1]);

	OLED_CS_SetVal(0);



}


void OledInit()
{
	OLED_RESET_SetVal(0);
	WAIT1_Waitms(200);
	OLED_RESET_ClrVal(0);
	WAIT1_Waitms(200);
	OLED_RESET_SetVal(0);
	WAIT1_Waitms(200);


	OLED_CS_ClrVal(0);

	  command(SSD1331_CMD_DISPLAYOFF);  	// 0xAE
	    command(SSD1331_CMD_SETREMAP); 	// 0xA0
	#if defined SSD1331_COLORORDER_RGB
	    command(0x72);				// RGB Color
	#else
	    command(0x76);				// BGR Color
	#endif
	    command(SSD1331_CMD_STARTLINE); 	// 0xA1
	    command(0x0);
	    command(SSD1331_CMD_DISPLAYOFFSET); 	// 0xA2
	    command(0x0);
	    command(SSD1331_CMD_NORMALDISPLAY);  	// 0xA4
	    command(SSD1331_CMD_SETMULTIPLEX); 	// 0xA8
	    command(0x3F);  			// 0x3F 1/64 duty
	    command(SSD1331_CMD_SETMASTER);  	// 0xAD
	    command(0x8E);
	    command(SSD1331_CMD_POWERMODE);  	// 0xB0
	    command(0x0B);
	    command(SSD1331_CMD_PRECHARGE);  	// 0xB1
	    command(0x31);
	    command(SSD1331_CMD_CLOCKDIV);  	// 0xB3
	    command(0xF0);  // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
	    command(SSD1331_CMD_PRECHARGEA);  	// 0x8A
	    command(0x64);
	    command(SSD1331_CMD_PRECHARGEB);  	// 0x8B
	    command(0x78);
	    command(SSD1331_CMD_PRECHARGEA);  	// 0x8C
	    command(0x64);
	    command(SSD1331_CMD_PRECHARGELEVEL);  	// 0xBB
	    command(0x3A);
	    command(SSD1331_CMD_VCOMH);  		// 0xBE
	    command(0x3E);
	    command(SSD1331_CMD_MASTERCURRENT);  	// 0x87
	    command(0x06);
	    command(SSD1331_CMD_CONTRASTA);  	// 0x81
	    command(0x91);
	    command(SSD1331_CMD_CONTRASTB);  	// 0x82
	    command(0x50);
	    command(SSD1331_CMD_CONTRASTC);  	// 0x83
	    command(0x7D);
	    command(SSD1331_CMD_DISPLAYON);	//--turn on oled panel
		OLED_CS_SetVal(0);

	for (int x = 0;x<96;x++)
	{
		for (int y = 0;y<64;y++)
		{
			SSD1331_draw_point(x,y,RGB(x,y,0));
		}
	}


}

int cppmain(void)
{
	LEDS_InitHardware();
	//ak4558_init();

	Measuring = 1;

	int t =0 ;

	OledInit();

	for(;;)
	{

		t++;
		for (int x = 0;x<96;x++)
		{
			for (int y = 0;y<64;y++)
			{
				SSD1331_draw_point(x,y,RGB(x,255-x*4 ,t+ (y/4)*12));
			}
		}

		WAIT1_Waitms(10);

		if (audio_buffers_fresh)
		{
			audio_buffers_fresh = 0;

			int32_t* inbuf = audio_in_buffer;
			int32_t* outbuf = audio_out_buffer;
			NextBlock(inbuf, outbuf);


		}
		switch (Measuring)
		{
		case 1:
		{
			//ADCMUXING();
			Measuring = 2;
			ADMAIN_Measure(false);
		} break;
		case 3:
		{
			uint16_t values[2];
			ADMAIN_GetValue16(values);
			float pitchvoltage = ((values[0] * 3.3f)/65535.0f) * 1.98f  - 1.0f;
			Measuring = 0;
		}break;
		}


	}
}

}
