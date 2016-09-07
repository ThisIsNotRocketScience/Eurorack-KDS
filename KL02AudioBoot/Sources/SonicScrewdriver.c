#include "Cpu.h"

uint8_t GetButton()
{
	uint32_t PortData;                   /* Port data masked according to the bit used */
	PortData = ( (uint32_t)((((FGPIO_MemMapPtr)0xF80FF040u))->PDIR)) & 0x08;


	return (PortData != 0U) ? 1:0;
}

void GUISuccesState()
{
	GPIOA_PDDR |= GPIO_PDDR_PDD(0x1000);
	  /* Set initialization value */
	  /* GPIOA_PDOR: PDO|=0x1000 */
	  GPIOA_PDOR |= GPIO_PDOR_PDO(0x1000);
	  /* Initialization of Port Control register */
	  /* PORTA_PCR12: ISF=0,MUX=1 */
	  PORTA_PCR12 = (uint32_t)((PORTA_PCR12 & (uint32_t)~(uint32_t)(
	                 PORT_PCR_ISF_MASK |
	                 PORT_PCR_MUX(0x06)
	                )) | (uint32_t)(
	                 PORT_PCR_MUX(0x01)
	                ));


	for (int j =  0;j<5;j++)
	{
		for(int i =0 ;i<100000;i++)
		{
			((((FGPIO_MemMapPtr)0xF80FF000u))->PSOR) = (uint32_t)(0x1000U);



		}
		for(int i =0 ;i<100000;i++)
		{
			((((FGPIO_MemMapPtr)0xF80FF000u))->PCOR) = (uint32_t)(0x1000U);

		}
	}

}
void GUIErrorState()
{
	GPIOB_PDDR |= GPIO_PDDR_PDD(0x40);
	GPIOB_PDOR |= GPIO_PDOR_PDO(0x40);
	PORTB_PCR6 = (uint32_t)((PORTB_PCR6 & (uint32_t)~(uint32_t)(PORT_PCR_ISF_MASK |PORT_PCR_MUX(0x06))) | (uint32_t)(PORT_PCR_MUX(0x01)));

	for (int j =  0;j<5;j++)
	{
		for(int i =0 ;i<500000;i++)
		{
			((((FGPIO_MemMapPtr)0xF80FF040u))->PSOR) = (uint32_t)(0x40U);


		}
		for(int i =0 ;i<500000;i++)
		{
			((((FGPIO_MemMapPtr)0xF80FF040u))->PCOR) = (uint32_t)(0x40U);

		}
	}
}

uint8_t Boot_CheckButtons()
{
	SIM_SCGC5 |= (SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTA_MASK);

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
