
#include "MessageDecoder.h"
#include "Boot.h"

int32_t avg = 9000;
int32_t min = 9000;
int32_t max = 9000;

AudioReaderStruct Reader;

#define POLY 0x82f63b78

uint32_t CalcCrc(uint32_t crc, uint8_t *buf, uint32_t length)
{
	int k;
	crc = ~crc;
	while(length--)
	{
		crc^= *buf++;
		for(k = 0;k<8;k++)
		{
			crc = crc &1 ? (crc>>1) ^ POLY : crc >> 1;
		}
	}
}

void DecoderUpdate(int32_t input)
{
	avg = (avg * 7 + input) >> 3;
	min = (min * 7 + avg ) >> 3;
	max = (max * 7 + avg ) >> 3;
	if (input < min) min = input;
	if  (input > max) max = input;
	int32_t calcavg = (max + min) >> 1;
	int32_t filtered = input - calcavg;
	AudioReader_Update(&Reader, filtered);
}

void DecoderInit()
{
	AudioReader_Init(&Reader);
}

char buf[22];
char wribuf[1024];
char busy = 0;

void ByteReceived(AudioReaderStruct *S, int bytes, unsigned char Dat)
{
	if (busy >0) return;
	if (bytes>21) AudioReader_NewPacket(S);
	buf[bytes] = Dat;
	busy =1;
	switch(bytes)
	{
		case 20:
		{
			if (buf[0] == 'B' &&buf[1] == 'L' &&buf[2] == 'O')
			{
				uint32_t idx = buf[3];
				if (idx > 63)idx = 63;
				for(int i = 0;i<16;i++)
				{
					wribuf[idx*16 + i] = buf[4+i];
				}
				AudioReader_NewPacket(&Reader);
			}
		}break;
		case 4:
		{
			if (buf[0] == 'K' && buf[1] == 'I' && buf[2] == 'L' && buf[3] == 'L')
			{
				Boot_EraseAll();
				AudioReader_NewPacket(&Reader);
			}
		}break;
		case 12:
		{
			if (buf[0] == 'F' && buf[1] == 'L' && buf[2] == 'A' && buf[3] == 'S')
			{
				uint32_t off = (buf[4] << 24) + (buf[5] << 16) + (buf[6] << 8) +(buf[7] );
				uint32_t crc = (buf[8] << 24) + (buf[9] << 16) + (buf[10] << 8) +(buf[11] );
				if (crc == CalcCrc(0, wribuf, 1024))
				{
					Boot_FlashProg(MIN_APP_FLASH_ADDRESS + off,wribuf, 1024);
				}
				else
				{
					//Error!
				}
				AudioReader_NewPacket(&Reader);
			}
		}break;
	}
	busy = 0;
}
