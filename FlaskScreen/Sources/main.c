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
#include "OLED_DC.h"
#include "SM1.h"

/* User includes (#include below this line is not maintained by Processor Expert) */



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

int sent = 0 ;

void ShiftByte(uint8_t dat)
{
	sent = 0;
	SM1_SendBlock(SM1_DeviceData, &dat, 1);
	while (sent == 0){};
}


void command(uint8_t cmd)
{
	OLED_DC_ClrVal(0);
	ShiftByte(cmd);
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


void SetWindow(int x, int y, int w, int h)
{
	command(SSD1331_CMD_SETCOLUMN);
	command(x);
	command(w);

	command(SSD1331_CMD_SETROW);
	command(y);
	command(h);
}


void SSD1331_draw_point(int x, int y, unsigned short hwColor) {

	unsigned char buffer[3];

	if(x >= OLED_WIDTH || y >= OLED_HEIGHT)
	{
		return;
	}



	// set x and y coordinate
	command(SSD1331_CMD_SETCOLUMN);
	command(x);
	command(WIDTH-1);

	buffer[0] = SSD1331_CMD_SETCOLUMN;
	buffer[1] = x;
	buffer[2] = WIDTH-1;
	//	sent = 0;
	//	SM1_SendBlock(SM1_DeviceData, buffer, 3);
	//	while (sent == 0){}

	//	command(SSD1331_CMD_SETROW);
	//	command(y);
	//	command(HEIGHT-1);
	buffer[0] = SSD1331_CMD_SETROW;
	buffer[1] = y;
	buffer[2] = HEIGHT-1;
	sent = 0;
	SM1_SendBlock(SM1_DeviceData, buffer, 3);
	while (sent == 0){}

	OLED_DC_SetVal(OLED_DC_DeviceData);

	//  OLED_CS_ClrVal(OLED_CS_DeviceData);
	buffer[0] = (hwColor & 0xFF00) >> 8;
	buffer[1] = hwColor & 0xFF;
	sent = 0;
	SM1_SendBlock(SM1_DeviceData, buffer, 2);
	while (sent == 0){}




}


void OledInit()
{
	OLED_RESET_SetVal(0);
	WAIT1_Waitms(200);
	OLED_RESET_ClrVal(0);
	WAIT1_Waitms(200);
	OLED_RESET_SetVal(0);
	WAIT1_Waitms(200);



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

	WAIT1_Waitms(10);
	SetWindow(0,0,95,63);
	for (int x = 0;x<96;x++)
	{
		for (int y = 0;y<64;y++)
		{
			uint16_t T = RGB(x,y,0);
			SM1_SendBlock(SM1_DeviceData, &T, 2);
		}
	}


}
#include "ak4558.h"
#define delaylen 8820

typedef struct Delay
{
	int16_t buffer[delaylen] ;

	int delayposleft ;
	int delayposcenter ;
	int delayposright ;
	int delaylenleft ;
	int delaylenright;
	int32_t wet ;
	int32_t dry ;
	int32_t feedback;
	int32_t lin ;
	int32_t rin ;
	int mono ;
} Delay;

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

	for(int i = 0;i<delaylen;i++) D->buffer[i] = 0;

}
#include "MasterChorus.h"
typedef struct EffectsOverlay
{
	union
	{
		Delay Delay;
		StereoChorus_t Chorus;
	};
} EffectsOverlay;

EffectsOverlay TheSet;
int32_t Param[4];

int BlockT = 0;


void ProcessDelay(Delay* D, int32_t *in, int32_t *out)
{
	D->wet = Param[0];
	D->dry = 0xffff-D->wet;
	D->feedback = Param[1]>>1;
	D->mono = Param[2]>0x8000?1:0;
	for (int i =0 ;i<AUDIO_BUFFER_SIZE;i++)
	{
		D->lin = (*in++)>>16;
		D->rin = (*in++)>>16;

		if (D->mono) D->rin = D->lin = (D->rin + D->lin)/2;
		int32_t lout = D->buffer[(D->delayposcenter-D->delaylenleft+delaylen+delaylen)%delaylen];
		int32_t rout = D->buffer[(D->delayposcenter-D->delaylenright+delaylen+delaylen)%delaylen];

		D->buffer[D->delayposcenter] = (D->rin+D->lin)/2 +( ((lout+rout) * D->feedback)>>16);


		D->delayposcenter = (D->delayposcenter+1) % delaylen;

		*out++ = (D->lin * D->dry) + (lout * D->wet );
		*out++ = (D->rin * D->dry) + (rout * D->wet) ;
	}
}


void NextBlock(int32_t *in, int32_t *out)
{
	BlockT++;
	//ProcessDelay(&TheSet.Delay,in, out);
	{
		Chorus_SetMix(&TheSet.Chorus, Param[0]/65535.0);
		Chorus_SetSpeed(&TheSet.Chorus, Param[1]/65535.0);
		Chorus_SetShift(&TheSet.Chorus, Param[2]/65535.0);
	ProcessChorus(&TheSet.Chorus, in, out);
	}
}
int measuring =0 ;
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

int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/

	InitDelay(&TheSet.Delay);
	InitChorus(&TheSet.Chorus);
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
		uint16_t buffer2[64];

		int Ap = ((0xffff-Param[0]) * 95)/65535;
		int Bp = ((0xffff-Param[1]) * 95)/65535;
		int Cp = ((0xffff-Param[2]) * 95)/65535;
		int Dp = ((0xffff-Param[3]) * 95)/65535;
		for(int x =0;x<64;x++){buffer2[x]= 0;};
		for (int x = 0;x<96;x++)
		{
			CheckAudio();

			int c =0 ;
			int r = ((BlockT + x)%96>48)?255:0;
			uint16_t base = RGB(r,r,r);
			uint8_t A1 = x>=Ap?255:0;
			uint8_t B1 = x>=Bp?255:0;
			uint8_t C1 = x>=Cp?255:0;
			uint8_t D1 = x>=Dp?255:0;

			uint16_t T1 = RGB(0,A1/2,A1);
			uint16_t T2 = RGB(0,B1,B1/2);
			uint16_t T3 = RGB(0,C1/2,C1);
			uint16_t T4 = RGB(0,D1,D1/2);
			for (int y = 1;y<15;y++)
			{

				if (y%2 == 1)
				{
					buffer2[y/2 + 0*8] =T1;
					buffer2[y/2 + 1*8] =T2;
					buffer2[y/2 + 2*8] =T3;
					buffer2[y/2 + 3*8] =T4;
				}
				else
				{
					buffer2[y/2 + 0*8 + 32] =T1;
					buffer2[y/2 + 1*8 + 32] =T2;
					buffer2[y/2 + 2*8 + 32] =T3;
					buffer2[y/2 + 3*8 + 32] =T4;

				}
				//	buffer2[y+ 1*16] =T2;
				//	buffer2[y+ 2*16] =T3;
				//		buffer2[y+ 3*16] =T4;
			}
			CheckAudio();
			sent =0;
			SM1_SendBlock(SM1_DeviceData, buffer2, 2*64);
			while(sent ==0 ){CheckAudio();};
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
