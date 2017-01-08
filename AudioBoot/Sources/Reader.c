#include <stdint.h>
#include <stdio.h>

#include "Reader.h"

void __attribute__ ((weak)) ByteReceived(AudioReaderStruct *S, int bytes, unsigned char Dat)
{
	// by default - do nothing.
}



void AudioReader_Init( AudioReaderStruct *S)
{
	S->LastVal = 0;
	S->TimeSinceLastUpCrossing = 0;
	S->CurrentBit = 7;
	S->Bytes = 0;
	S->CurrentVal = 0;
	S->Sync = AUDIOREADER_SYNCLOST;

}

void AudioReader_NewPacket( AudioReaderStruct *S)
{
	S->CurrentBit = 7;
	S->Bytes = 0;
	S->Sync = AUDIOREADER_SYNCLOST;
}

void AudioReader_Update(AudioReaderStruct *S, int32_t NewVal)
{

	S->TimeSinceLastUpCrossing++;
	if (S->TimeSinceLastUpCrossing > AUDIOREADER_SYNCLOSTTIME)
	{
		S->Sync = AUDIOREADER_SYNCLOST;
	}
	if (S->LastVal <= 0 && NewVal > 0)
	{

		if (S->TimeSinceLastUpCrossing < AUDIOREADER_DEBOUNCE)
		{
			// skip this..
		}
		else
		{
			int Bit = 0;


			if (S->TimeSinceLastUpCrossing >= AUDIOREADER_LONGPULSEDETECTIONTHRESHOLD)
			{
				Bit = 1;
				//				printf("1: %d\n", TimeSinceLastUpCrossing);
			}
			else
			{
				Bit = 0;
				//					printf("0: %d\n", TimeSinceLastUpCrossing);

			}

			switch (S->Sync)
			{
			case AUDIOREADER_SYNCLOST:
			{
				S->CurrentVal = (S->CurrentVal << 1) & 0xff;
				if (Bit) S->CurrentVal++;

				if (S->CurrentVal == 0x02)
				{
					S->Sync = AUDIOREADER_SYNCING;
					S->LastSyncByte = 0x02;
					S->CurrentBit = 0;

				}
				else
				{
				}
			}
			break;
			case AUDIOREADER_SYNCING:
				S->CurrentVal = (S->CurrentVal << 1) & 0xff;
				if (Bit) S->CurrentVal++;
				S->CurrentBit = (S->CurrentBit + 1) % 8;
				if (S->CurrentBit == 0)
				{
					if (S->LastSyncByte == 0x02 && S->CurrentVal == 0x02)
					{
						// still waiting
					}
					else
					{

						if (S->LastSyncByte == 0x02 && S->CurrentVal == 0x03)
						{

						}
						else
						{
							if (S->CurrentVal > 0 && S->LastSyncByte == S->CurrentVal + 1)
							{
								// still counting!
								if (S->CurrentVal == 1)
								{
									S->Sync = AUDIOREADER_SYNCED;
									S->Bytes = 0;
								}
							}
							else
							{
								S->Sync = AUDIOREADER_SYNCLOST;
							}
						}
					}
					S->LastSyncByte = S->CurrentVal;
				}
				else
				{
				}

				break;
			case AUDIOREADER_SYNCED:
			{
				S->CurrentVal = (S->CurrentVal << 1) & 0xff;
				if (Bit) S->CurrentVal++;
				S->CurrentBit = (S->CurrentBit + 1) % 8;
				if (S->CurrentBit == 0)
				{
					ByteReceived(S, S->Bytes, S->CurrentVal);
					S->Bytes++;
					S->CurrentBit = 0;
					if(S->Bytes >= AUDIOREADER_MAXBYTESPERMESSAGE)
					{
						AudioReader_NewPacket(S);
					}
				}
				break;
			}
			}
		}


		S->TimeSinceLastUpCrossing = 0;
	}
	S->LastVal = NewVal;
}

