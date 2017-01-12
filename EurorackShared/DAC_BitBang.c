#include "Cpu.h"

#define DAC_CS 5 // on port A
#define DAC_DATA 7 // on port A

#define DAC_CLOCK 0 // on port B

#define NOP __asm volatile ("nop");


#define CS_SET() {GPIOA_PDOR |= (1<<DAC_CS);NOP;NOP;NOP;NOP;};
#define CS_CLR() {GPIOA_PDOR &= ~(1<<DAC_CS);NOP;NOP;NOP;NOP;NOP;};
#define CLOCK_SET() {GPIOB_PDOR |= (1<<DAC_CLOCK);NOP;NOP;NOP;NOP;};
#define CLOCK_CLR() {GPIOB_PDOR &= ~(1<<DAC_CLOCK);NOP;NOP;NOP;NOP;};
#define DATA_SET() {GPIOA_PDOR |= (1<<DAC_DATA);NOP;NOP;NOP;NOP;};
#define DATA_CLR() {GPIOA_PDOR &= ~(1<<DAC_DATA);NOP;NOP;NOP;NOP;};

void DACBITBANG_InitHardware()
{
	PORTA_PCR5 = PORT_PCR_MUX(0x01);
	PORTA_PCR7 = PORT_PCR_MUX(0x01);

	PORTB_PCR0 = PORT_PCR_MUX(0x01);

	GPIOA_PDDR |= (1 << DAC_CS) | (1 << DAC_DATA);
	GPIOB_PDDR |= (1 << DAC_CLOCK);
}

void SendByte(uint8_t val)
{
	CLOCK_CLR(); if ((val & 0x80) == 0x80){DATA_SET();} else {DATA_CLR();}CLOCK_SET();
	CLOCK_CLR(); if ((val & 0x40) == 0x40){DATA_SET();} else {DATA_CLR();}CLOCK_SET();
	CLOCK_CLR(); if ((val & 0x20) == 0x20){DATA_SET();} else {DATA_CLR();}CLOCK_SET();
	CLOCK_CLR(); if ((val & 0x10) == 0x10){DATA_SET();} else {DATA_CLR();}CLOCK_SET();
	CLOCK_CLR(); if ((val & 0x08) == 0x08){DATA_SET();} else {DATA_CLR();}CLOCK_SET();
	CLOCK_CLR(); if ((val & 0x04) == 0x04){DATA_SET();} else {DATA_CLR();}CLOCK_SET();
	CLOCK_CLR(); if ((val & 0x02) == 0x02){DATA_SET();} else {DATA_CLR();}CLOCK_SET();
	CLOCK_CLR(); if ((val & 0x01) == 0x01){DATA_SET();} else {DATA_CLR();}CLOCK_SET();
}

void DACBITBANG_Update(uint16_t val1, uint16_t val2)
{

	const int shutdown1 = 0;
	const int gain1 = 0;
	const int shutdown2 = 0;
	const int gain2 = 0;

	unsigned int command;

	
	command = 0x0000;
	command |= shutdown1 ? 0x0000 : 0x1000;
	command |= gain1 ? 0x0000 : 0x2000;
	command |= (val1 & 0x0FFF);
	CS_CLR();
	SendByte(command >> 8);
	SendByte(command & 0xff);
	CS_SET();

	command = 0x8000;
	command |= shutdown2 ? 0x0000 : 0x1000;
	command |= gain2 ? 0x0000 : 0x2000;
	command |= (val2 & 0x0FFF);
	CS_CLR();
	SendByte(command >> 8);
	SendByte(command & 0xff);
	CS_SET();
}
