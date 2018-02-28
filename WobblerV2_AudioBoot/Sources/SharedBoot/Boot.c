#include <stdint.h>
#include "Boot.h"
#include "IFsh1.h"
#include "Cpu.h"


uint8_t ValidAppAddress(dword addr){ return ((addr>=MIN_APP_FLASH_ADDRESS) && (addr<=MAX_APP_FLASH_ADDRESS)); }

void  __attribute__((optimize("-O0"))) Reboot()
				{
	__DI();
#if KIN1_IS_USING_KINETIS_SDK
	SCB_AIRCR = (0x5FA<<SCB_AIRCR_VECTKEY_Pos)|SCB_AIRCR_SYSRESETREQ_Msk;
#else
	SCB_AIRCR = SCB_AIRCR_VECTKEY(0x5FA) | SCB_AIRCR_SYSRESETREQ_MASK;
#endif

	for(;;)
	{
	}
				}

uint8_t Boot_FlashProg(uint32_t flash_addr, uint8_t *data_addr, uint16_t nofDataBytes)
{
	if(ValidAppAddress(flash_addr))
	{
		IFsh1_EraseSector(flash_addr);
		if (IFsh1_SetBlockFlash((IFsh1_TDataAddress)data_addr, flash_addr, nofDataBytes) != ERR_OK)
		{
			return ERR_FAILED;
		}
	}
	return ERR_OK;
}

uint8_t Boot_EraseAll(void)
{
	dword addr;

	for(addr=MIN_APP_FLASH_ADDRESS;addr<=MAX_APP_FLASH_ADDRESS;addr+=FLASH_PAGE_SIZE)
	{
		if(IFsh1_EraseSector(addr) != ERR_OK)
		{
			return ERR_FAILED;
		}
	}
	return ERR_OK;
}

uint8_t GetButton()
{
	return  (GPIOA_PDIR & (1<<12))>0?1:0;
}


uint8_t __attribute__ ((weak)) Boot_CheckButtons()
				{
	volatile uint32_t b = 0;
	for(int i=0;i<3000;i++)
	{
		b+= GetButton();
		WAIT1_Waitms(1);
		if (b>500)
		{
			return 1;
		}
	}

	return 0;

}

void Boot_Check(void)
{
	uint32_t startup;


	SIM_SCGC5   |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK ;
	GPIOA_PDDR &= (uint32_t)~(uint32_t)(GPIO_PDDR_PDD((1<<12)));
	PORTA_PCR12 = PORT_PCR_MUX(0x01) | 3;
	startup = ((uint32_t*)APP_FLASH_VECTOR_START)[1];
	uint8_t checkb = Boot_CheckButtons();
	if (startup!=-1 && startup!=0 && checkb == 1)
	{
		__DI();
		((void(*)(void))startup)();
	}
}
