#ifndef KL02BOOT
#define KL02BOOT

#define FLASH_PAGE_SIZE 0x400
#define MIN_APP_FLASH_ADDRESS 0x2000
#define MAX_APP_FLASH_ADDRESS 0x7fff
#define APP_FLASH_VECTOR_START MIN_APP_FLASH_ADDRESS
#define APP_FLASH_VECTOR_SIZE 0xc0


void Boot_Run();
void Boot_Check();

uint8_t Boot_FlashProg(uint32_t flash_addr, uint8_t *data_addr, uint16_t nofDataBytes) ;
uint8_t Boot_EraseAll();
void Reboot();

#endif
