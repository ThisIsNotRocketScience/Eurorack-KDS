#include <stdint.h>
#include "Boot.h"
#include "IFsh1.h"

uint8_t ValidAppAddress(dword addr)
{
	return ((addr>=MIN_APP_FLASH_ADDRESS) && (addr<=MAX_APP_FLASH_ADDRESS));
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

	/* erase application flash pages */
	for(addr=MIN_APP_FLASH_ADDRESS;addr<=MAX_APP_FLASH_ADDRESS;addr+=FLASH_PAGE_SIZE)
	{
		if(IFsh1_EraseSector(addr) != ERR_OK)
		{ /* Error Erasing Flash */
			return ERR_FAILED;
		}
	}
	return ERR_OK;
}

uint8_t __attribute__ ((weak)) Boot_CheckButtons()
{
	return 1;
}

void Boot_Check(void)
{
	uint32_t startup;

	startup = ((uint32_t*)APP_FLASH_VECTOR_START)[1];
	if (startup!=-1 && !Boot_CheckButtons())
	{
		((void(*)(void))startup)();
	}
}
