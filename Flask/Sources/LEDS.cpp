/*
 * LEDS.c
 *
 *  Created on: 6 jan. 2017
 *      Author: Stijn
 */
#include "Cpu.h"

#define LED_LATCH 0
#define LED_CLOCK 1
#define LED_DATA 2

void LED_LATCH_SET() {GPIOD_PDOR |= (1<<LED_LATCH);};
void  LED_LATCH_CLR() {GPIOD_PDOR &= ~(1<<LED_LATCH);};
void  LED_CLOCK_SET() {GPIOD_PDOR |= (1<<LED_CLOCK);};
void LED_CLOCK_CLR() {GPIOD_PDOR &= ~(1<<LED_CLOCK);};
void  LED_DATA_SET() {GPIOD_PDOR |= (1<<LED_DATA);};
void LED_DATA_CLR() {GPIOD_PDOR &= ~(1<<LED_DATA);};

void LEDS_InitHardware()
{
	PORTD_PCR0 = PORT_PCR_MUX(0x01);
	PORTD_PCR1 = PORT_PCR_MUX(0x01);
	PORTD_PCR2 = PORT_PCR_MUX(0x01);

	GPIOD_PDDR |= (1 << LED_LATCH) | (1 << LED_CLOCK) | (1 << LED_DATA);
}

uint8_t PWM = 0;

void LEDS_Update(uint8_t *leds, int count, uint8_t *gates, int gatecount)
{
	PWM+= 16;

	LED_LATCH_CLR();
	for (int i = 0; i < gatecount; i++)
	{
		if (gates[i] > 0)
		{
			LED_DATA_CLR();
		}
		else
		{
			LED_DATA_SET();
		}
		LED_CLOCK_CLR();
		LED_CLOCK_SET();
	}

	for (int i = 0; i < count; i++)
	{
		if (leds[i]>0 && leds[i] >= PWM)
		{
			LED_DATA_SET();
		}
		else
		{
			LED_DATA_CLR();
		}
		LED_CLOCK_CLR();
		LED_CLOCK_SET();
	}

	LED_LATCH_SET();
}
