
#include "MessageDecoder.h"
#include "Boot.h"
#include "Cpu.h"

int32_t avg = 9000;
int32_t min = 9000;
int32_t max = 9000;


#define CHUNKS (1024/AUDIOREADER_MAXCHUNK)
byte blockshad[CHUNKS] = {0};
AudioReaderStruct Reader;

#define POLY 0x82f63b78

void __attribute__ ((weak)) GUIErrorState()
		{

		}

void __attribute__ ((weak)) GUISuccesState()
		{

		}

void __attribute__ ((weak)) GUIProgress(byte progress) // 255 = 99.999%
		{
		}

void __attribute__ ((weak)) GUIComplete()
		{
	Reboot();
		}

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

	return ~crc;
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

char rcvbuf[64 + 4 + 4];
char wribuf[1024];
char busy = 0;

uint32_t ReadInt(uint8_t *buf, int offs)
{
	uint32_t res = 0;
	buf += offs;
	uint32_t a =buf[0]<<24;
	uint32_t b =(buf[1] << 16) & 0xff0000;
	uint32_t c =(buf[2] << 8) & 0xff00;
	uint32_t d =buf[3];
	res = a+b+c+d;
	return res;
}
void ByteReceived(AudioReaderStruct *S, int bytes, unsigned char Dat)
{
	if (busy >0) return;

	if (bytes>= AUDIOREADER_MAXBYTESPERMESSAGE) AudioReader_NewPacket(S);
	rcvbuf[bytes] = Dat;
	busy = 1;


	switch(bytes + 1)
	{
	case AUDIOREADER_MAXBYTESPERMESSAGE:
	{
		if (rcvbuf[0] == 'B' && rcvbuf[1] == 'L' && rcvbuf[2] == 'O')
		{
			uint32_t idx = rcvbuf[3];



			if (idx < CHUNKS)
			{

				uint32_t crccheck = ReadInt(rcvbuf,4);
				uint32_t crcblock = CalcCrc(0, rcvbuf+8, AUDIOREADER_MAXCHUNK);
				if (crccheck == crcblock)
				{
					for(int i = 0;i < AUDIOREADER_MAXCHUNK;i++)
					{
						wribuf[idx*AUDIOREADER_MAXCHUNK + i] = rcvbuf[8+i];
					}
					blockshad[idx] = 1;
				}
				else
				{
					blockshad[idx] = 2;
				}
			}
			AudioReader_NewPacket(&Reader);
		}
	}break;

	case 4:
	{
		if (rcvbuf[0] == 'K' && rcvbuf[1] == 'I' && rcvbuf[2] == 'L' && rcvbuf[3] == 'L')
		{
			Boot_EraseAll();
			AudioReader_NewPacket(&Reader);
		}

		if (rcvbuf[0] == 'B' && rcvbuf[1] == 'O' && rcvbuf[2] == 'O' && rcvbuf[3] == 'T')
		{
			Reboot();
		}
	}break;

	case 12:
	{
		if (rcvbuf[0] == 'F' && rcvbuf[1] == 'L' && rcvbuf[2] == 'A' && rcvbuf[3] == 'S')
		{
			uint32_t off = ReadInt(rcvbuf, 4);
			uint32_t crccheck = ReadInt(rcvbuf, 8);

			uint32_t crcblock = CalcCrc(0, wribuf, 1024);
			if (crccheck  == crcblock)
			{
				if (Boot_FlashProg(MIN_APP_FLASH_ADDRESS + off, wribuf, 1024) == ERR_FAILED)
				{
					GUIErrorState();
				}
				else
				{
					GUISuccesState();
				}
			}
			else
			{
				GUIErrorState();
			}
			for(int i =0 ;i<CHUNKS;i++) blockshad[i] = 0;
			for(int i =0 ;i<1024;i++) wribuf[i] = 0;
			AudioReader_NewPacket(&Reader);
		}
	}break;
	}
	busy = 0;
}
