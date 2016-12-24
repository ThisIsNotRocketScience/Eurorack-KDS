#include "Cpu.h"
#include "Reader.h"
#include "MessageDecoder.h"
#pragma optimize level=0
#pragma OPTIMIZE OFF

byte leds[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte gates[6] = {0, 0, 0, 0, 0, 0};
byte pwm = 0;

//ptb7
#define  DATA_ON     GPIOB_PSOR=(1<<7)
#define  DATA_OFF    GPIOB_PCOR=(1<<7)

//ptb6
#define  CLOCK_ON     GPIOB_PSOR=(1<<6)
#define  CLOCK_OFF    GPIOB_PCOR=(1<<6)

//ptb5
#define  LATCH_ON     GPIOB_PSOR=(1<<5)
#define  LATCH_OFF    GPIOB_PCOR=(1<<5)


void ShiftOut()
{
	pwm += 16;

	LATCH_OFF;
	for (int i = 0; i < 6; i++)
	{
		if (gates[i] > 0)
			DATA_OFF;
		else
			DATA_ON;
		CLOCK_OFF;
		CLOCK_ON;
	}

	for (int i = 0; i < 16; i++)
	{
		if (leds[i] > pwm)
		{
			DATA_ON;
		}
		else
		{
			DATA_OFF;
		}
		CLOCK_OFF;
		CLOCK_ON;
	}

	LATCH_ON;
}


void GUIInit()
{
	PORTB_PCR5 =  PORT_PCR_MUX(0x1);
	PORTB_PCR6 =  PORT_PCR_MUX(0x1);
	PORTB_PCR7 =  PORT_PCR_MUX(0x1);
	PORTB_PCR10 =  PORT_PCR_MUX(0x1);

	GPIOB_PDDR = (1<<5) + (1<<6) + (1<<7);
}

uint8_t GetButton()
{
	return  (GPIOB_PDIR & (1<<10))>0?1:0;
}

void SuccesLedOn()
{
	gates[0] = 1;
}

void SuccesLedOff()
{
	gates[0] = 0;
}


void ErrorLedOn()
{
	gates[1] = 255;
}

void ErrorLedOff()
{
	gates[1] = 0;
}

extern AudioReaderStruct Reader;
byte prog = 0;
void GUIProgress(byte progress)
{
	prog  = progress;
} // 255 = 99.999%

int T =0 ;
void GUIUpdate()
{
	T++;
	for(int i =0 ;i<16;i++) leds[i] = 0;
	for(int i =0 ;i<6;i++) gates[i] =0 ;
	gates[Reader.Sync + 3] = 1;
	for (int i = 0;i<prog/16;i++)
	{
		leds[i] =( i * 64 + Reader.TotalBytes * 5 + T);
	}
	for(int i =prog/16;i<16;i++)
	{
		leds[i] = 0;
	}
	if (GetErrorLed()==1) gates[1] = 1;else gates[1] = 0;
	if (GetSuccesLed()==1) gates[0] = 1;else gates[0] =0;

		//WAIT1_Waitms(1000);
	ShiftOut();

}

uint8_t Boot_CheckButtons()
{
	//return 1;
	SIM_SCGC5 |= (SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTA_MASK);


	 //PORTC_PCR5 = PORT_PCR_MUX(0x1);
	// GPIOC_PDDR = (1<<5);

	GPIOB_PDDR &= (uint32_t)~(uint32_t)(GPIO_PDDR_PDD(0x08));
	PORTB_PCR3 = (uint32_t)((PORTB_PCR3 & (uint32_t)~(uint32_t)(PORT_PCR_ISF_MASK |PORT_PCR_MUX(0x06))) | (uint32_t)(PORT_PCR_MUX(0x01)));

	uint32_t b = 0;

	for(int i=0;i<1000;i++)
	{
		b+= GetButton();
	}

	if (b<500) return 1;
	return 0;
}
