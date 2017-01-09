/*
 * LEDS.c
 *
 *  Created on: 6 jan. 2017
 *      Author: Stijn
 */
#include "Cpu.h"

#define LED_LATCH 5
#define LED_CLOCK 6
#define LED_DATA 7

#define LATCH_SET() {GPIOB_PDOR |= (1<<LED_LATCH);};
#define LATCH_CLR() {GPIOB_PDOR &= ~(1<<LED_LATCH);};
#define CLOCK_SET() {GPIOB_PDOR |= (1<<LED_CLOCK);};
#define CLOCK_CLR() {GPIOB_PDOR &= ~(1<<LED_CLOCK);};
#define  DATA_SET() {GPIOB_PDOR |= (1<<LED_DATA);};
#define  DATA_CLR() {GPIOB_PDOR &= ~(1<<LED_DATA);};

void LEDS_InitHardware()
{
	PORTB_PCR5 = PORT_PCR_MUX(0x01);
	PORTB_PCR6 = PORT_PCR_MUX(0x01);
	PORTB_PCR7 = PORT_PCR_MUX(0x01);

	GPIOB_PDDR |= (1 << LED_LATCH) | (1 << LED_CLOCK) | (1 << LED_DATA);
}

uint8_t PWM = 0;

void LEDS_Update(uint8_t *leds, int count, uint8_t *gates, int gatecount)
{
	PWM+= 16;

	LATCH_CLR();
	for (int i = 0; i < gatecount; i++)
	{
		if (gates[i] > 0)
		{
			DATA_CLR();
		}
		else
		{
			DATA_SET();
		}
		CLOCK_CLR();
		CLOCK_SET();
	}

	for (int i = 0; i < count; i++)
	{
		if (leds[i]>0 && leds[i] >= PWM)
		{
			DATA_SET();
		}
		else
		{
			DATA_CLR();
		}
		CLOCK_CLR();
		CLOCK_SET();
	}

	LATCH_SET();
}
