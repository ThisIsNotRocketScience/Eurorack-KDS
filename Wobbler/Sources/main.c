/* ###################################################################
 **     Filename    : main.c
 **     Project     : SonicScrewdriver
 **     Processor   : MKL02Z32VFM4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2015-12-04, 13:49, # CodeGen: 0
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
#include "AD1.h"
#include "AdcLdd1.h"
#include "PTA.h"
#include "DACSEL.h"
#include "LATCH.h"
#include "DATA.h"
#include "CLOCK.h"
#include "RETRIGGERINT.h"
#include "CI2C1.h"
#include "PTB.h"
#include "KSDK1.h"
#include "WAIT1.h"
#include "SM1.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

#include <math.h>
#include <stdio.h>
#include "main.h"

volatile int measured = 0;

int adcchannels[ADC_Count];

#include "Wobbler.h"
#include "DAC.h"

struct Wobbler_LFO LFO;
struct Wobbler_Settings Settings;
struct Wobbler_Params Params;

struct denoise_state_t
{
	int counter;
	int down;
	int pressed;
	int released;
	int lastcounter;
};

int denoise(int sw_down, struct denoise_state_t *state)
{
	if (sw_down)
		state->counter++;
	else
		state->counter--;
	state->pressed = 0;
	state->released = 0;

	if (state->counter < 2)
	{
		if (state->lastcounter == 2)
		{
			state->pressed = 1;
		}
		state->counter = 1;
		state->down = 1;
	}
	else if (state->counter > 30)
	{
		if (state->lastcounter == 30)
		{
			state->released = 1;
		}
		state->counter = 31;
		state->down = 0;
	}
	state->lastcounter = state->counter;
	return state->pressed;
}

uint32_t t = 0;

//4096 = 2.048v
/*2.5 * (2.048 * INP)/4096
		(x * 4096)
		/ 2.5 * 2.048
		 = inp*/

#define VOLT(x) ((int)((4096.0 * (x)) / (2.5 * 2.048)))
#define NOTE(x) VOLT((x) / 12.0)


unsigned char pwm = 3;
int counter = 0;

void ShiftOut()
{
	counter++;
	pwm += 9;

	LATCH_ClrVal(LATCH_DeviceData);

	for (int i = 0; i < 2; i++)
	{
		if (LFO.Gate[i] > 0)
			DATA_ClrVal(DATA_DeviceData);
		else
			DATA_SetVal(DATA_DeviceData);
		CLOCK_ClrVal(CLOCK_DeviceData);
		CLOCK_SetVal(CLOCK_DeviceData);
	}

	for (int i = 0; i < 12; i++)
	{
		if (LFO.Led[11-i] > pwm)
		{
			DATA_SetVal(DATA_DeviceData);
		}
		else
		{
			DATA_ClrVal(DATA_DeviceData);
		}
		CLOCK_ClrVal(CLOCK_DeviceData);
		CLOCK_SetVal(CLOCK_DeviceData);
	}

	LATCH_SetVal(LATCH_DeviceData);
}



int KnobOpt(int val)
{
	int r = 0;
	if (val > (65536*1)/5) r++;
	if (val > (65536*2)/5) r++;
	if (val > (65536*3)/5) r++;
	if (val > (65536*4)/5) r++;
	return 1 + 4 - r;
}


int tickssincecommit = 0;

// half-millisecond timer -> update each dacchannel in turn
int LinearOut = 0;
int CurvedOut = 0;

void doTimer()
{
	tickssincecommit++;
	t++;
	switch(t%2)
	{
	case 0:
	{
		LinearOut = Wobbler_Get(&LFO, &Params);
		LinearOut = (int)(LFO.Output);
		DAC_Write(0, LinearOut);
	}
	break;
	case 1:
	{
		CurvedOut = (int)(LFO.OutputPhased);
		DAC_Write(1, CurvedOut);
	}

	break;
	}
	ShiftOut();
}



#define VERSIONBYTE 0x10
uint8_t hi(int address)
{
	return (uint8_t)((address) >> 8);
}

uint8_t lo(int address)
{
	return (uint8_t)((address & 0xFF));
}

uint8_t dev(int address)
{
#define address_24LC16B 0b1010

	return (uint8_t)((address_24LC16B << 3) | ((hi(address)) & 0x07));
}

volatile int i2csending = 0;
volatile int i2creceiving = 0;

byte combuffer[2];

void EE24_WriteByte(unsigned short address, byte value)
{
	//EE241_WriteByte(address, value);
	combuffer[0] = lo(address);
	combuffer[1] = value;
	i2csending = 0;

	byte devaddr = dev(address);
	int i = 0;

	CI2C1_SelectSlaveDevice(CI2C1_DeviceData, LDD_I2C_ADDRTYPE_7BITS, devaddr);
	CI2C1_MasterSendBlock(CI2C1_DeviceData, combuffer, 2, LDD_I2C_SEND_STOP);
	i2csending = 1;
	while (i2csending == 1)
	{
		CI2C1_MasterSendBlock(CI2C1_DeviceData, combuffer, 1, LDD_I2C_SEND_STOP);
		WAIT1_Waitms(10);
		ShiftOut();
	};

	i2csending = 0;
}

void EE24_WriteBlock(unsigned short address, byte *data, int len)
{
	//EE241_WriteBlock(address, data, len);

	for (int i = 0; i < len; i++)
	{

		EE24_WriteByte(address++, data[i]);
		//	WAIT1_Waitms(5);
	}
}

byte EE24_ReadByte(unsigned short address)
{
	//	byte out;
	//	EE241_ReadByte(address, &out);
	//return out;

	byte com[1] = {lo(address)};
	byte devaddr = dev(address);
	CI2C1_SelectSlaveDevice(CI2C1_DeviceData, LDD_I2C_ADDRTYPE_7BITS, devaddr);
	i2csending = 1;
	CI2C1_MasterSendBlock(CI2C1_DeviceData, com, 1, LDD_I2C_SEND_STOP);
	while (i2csending == 1)
	{
	}

	ShiftOut();

	byte out;
	i2creceiving = 1;
	CI2C1_MasterReceiveBlock(CI2C1_DeviceData, &out, 1, LDD_I2C_SEND_STOP);
	while (i2creceiving == 1)
	{
	};

	ShiftOut();

	return out;
}

void EE24_ReadBlock(unsigned short address, byte *out, int len)
{
	//EE241_ReadBlock(address, out, len);
	for (int i = 0; i < len; i++)
	{
		out[i] = EE24_ReadByte(address++);
	}

	/*
	byte com[1] = {lo(address)};
	byte devaddr = dev(address);
	CI2C1_Init(CI2C1_DeviceData);
	CI2C1_SelectSlaveDevice(CI2C1_DeviceData, LDD_I2C_ADDRTYPE_7BITS, devaddr);
	i2csending = 1;
	CI2C1_MasterSendBlock(CI2C1_DeviceData, com, 1, LDD_I2C_NO_SEND_STOP);
	while (i2csending == 1) {}


	i2creceiving = 1;
	CI2C1_MasterReceiveBlock(CI2C1_DeviceData,out, len, LDD_I2C_SEND_STOP);
	while (i2creceiving == 1) {};

	 */
}

void SaveEeprom()
{
	EE24_WriteByte(0, VERSIONBYTE);
	int paramsize = sizeof(Params);
	EE24_WriteBlock(1, (byte *)&Params, paramsize);
}

void LoadEeprom()
{
	byte Ver;
	Ver = EE24_ReadByte(0);
	if (Ver == VERSIONBYTE)
	{
		int paramsize = sizeof(Params);
		EE24_ReadBlock(1, (byte *)&Params, paramsize);

		Wobbler_ValidateParams(&Params);
	}
	else
	{
		SaveEeprom();
	}
}

void EnvelopeTrigger(int sw)
{
	Wobbler_Trigger(&LFO, sw, &Params);
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */

	static struct denoise_state_t speedsw_state = {0};
	static struct denoise_state_t modesw_state = {0};
	static struct denoise_state_t triggersw_state = {0};
	Wobbler_Init(&LFO);

#ifdef USE_SEMIHOST
	initialise_monitor_handles();
#endif

	Wobbler_LoadSettings(&Settings, &Params);
	LoadEeprom();

	TI1_Enable();
	AD1_Calibrate(TRUE);
	AD1_Start();
	AD1_Measure(FALSE);

	ShiftOut();
	int switchmode = 1;


	ShiftOut();

	byte commitchange = 0;
	for (;;)
	{


		if (measured == 1)
		{
			LFO.Mod = 255-(adcchannels[ADC_MODULATION] >> 8);
			LFO.Shape = 255-(adcchannels[ADC_SHAPE] >> 8);
			LFO.Phasing = 255-(adcchannels[ADC_PHASING] >> 8);
			LFO.Speed = ((~adcchannels[ADC_SPEED]) >> 8);
			measured = 0;

		}
		//AD1_Measure(FALSE);


		if (commitchange == 1 && tickssincecommit >= 10)
		{
		//	SaveEeprom();
			commitchange = 0;
			tickssincecommit = 0;
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
