#include "OLED.h"
#include "SM1.h"
#include "OLED_DC.h"
#include "OLED_RESET.h"
#include "WAIT1.h"

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
