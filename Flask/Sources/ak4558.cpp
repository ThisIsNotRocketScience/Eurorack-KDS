#include "ak4558.h"

#include "Cpu.h"
extern "C"
{
#include "CODEC_PDN.h"
#include "WAIT1.h"
	#include "GI2C1.h"
#include "PDD_Includes.h"
}

void ak4558_init()
{
	CODEC_PDN_SetVal(CODEC_PDN_DeviceData);
	WAIT1_Waitms(100);

	GI2C1_SelectSlave(0b0010000);
	unsigned char ak4558_configdata[] = {
			0, 0b00011111,
			2, 0b00000000,
			3, 0b00111110,
			6, 0b00011001,
			7, 0b00001100,
			//4, 0b00010100,//187.5khz
			4, 0b00010000,//46.875khz
			//5, 0b01110010,//187.5khz
			5, 0b01000010,//46.875khz
			8, 255,
			9, 255,
			1, 0b00001111
	};
	for (int i = 0; i < sizeof(ak4558_configdata); i += 2) {
		word sent = 0;
		byte res = GI2C1_WriteBlock(&ak4558_configdata[i], 2, GI2C1_SEND_STOP);
		WAIT1_Waitms(100);

	}
}

static int32_t audio_in_data[AUDIO_BUFFER_SIZE * 4] = {0};
static int32_t audio_out_data[AUDIO_BUFFER_SIZE * 4] = {0};

int32_t *audio_in_buffer = &audio_in_data[2*AUDIO_BUFFER_SIZE];
int32_t *audio_out_buffer = &audio_out_data[2*AUDIO_BUFFER_SIZE];
volatile int audio_buffers_fresh = 1;

static int32_t *audio_in_pos = audio_in_data;
static int32_t *audio_out_pos = audio_out_data;

extern "C"
{
PE_ISR(I2S0_TX)
{
    I2S0_TDR0 = *(audio_out_pos++);
    I2S0_TDR0 = *(audio_out_pos++);
    I2S_PDD_ClearTxInterruptFlags(I2S0_BASE_PTR, I2S_PDD_ALL_INT_FLAG);

    *(audio_in_pos++) = I2S0_RDR0;
	*(audio_in_pos++) = I2S0_RDR0;
    I2S_PDD_ClearRxInterruptFlags(I2S0_BASE_PTR, I2S_PDD_ALL_INT_FLAG);

    if (audio_in_pos == &audio_in_data[2*AUDIO_BUFFER_SIZE]) {
    	audio_in_buffer = audio_in_data;
    	audio_out_buffer = audio_out_data;
    	audio_buffers_fresh = 1;
    	audio_buffers_fresh = 1;
    }
    else if (audio_in_pos == &audio_in_data[4*AUDIO_BUFFER_SIZE]) {
    	audio_in_pos = audio_in_data;
    	audio_out_pos = audio_out_data;
    	audio_in_buffer = &audio_in_data[2*AUDIO_BUFFER_SIZE];
    	audio_out_buffer = &audio_out_data[2*AUDIO_BUFFER_SIZE];
    	audio_buffers_fresh = 1;
    	audio_buffers_fresh = 1;
    }
}

PE_ISR(I2S0_RX)
{
    I2S_PDD_ClearRxInterruptFlags(I2S0_BASE_PTR, I2S_PDD_ALL_INT_FLAG);
}
}
