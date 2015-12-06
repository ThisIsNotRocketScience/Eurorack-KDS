#include "ak4558.h"

#include "Cpu.h"
#include "CODEC_PDN.h"
#include "WAIT1.h"
#include "I2C1.h"
#include "PDD_Includes.h"

#include "dsp_main.h"

void ak4558_init()
{
	CODEC_PDN_SetVal(CODEC_PDN_DeviceData);
	WAIT1_Waitms(100);
	volatile byte res = I2C1_SelectSlave(0b0010000);
	unsigned char ak4558_configdata[] = {
			0, 0b00011111,
			2, 0b00000000,
			3, 0b00111110,
			6, 0b00011101,
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
		byte res = I2C1_SendBlock(&ak4558_configdata[i], 2, &sent);
	}
}

static int32_t ak4558_inl = 0;
static int32_t ak4558_inr = 0;
static int32_t ak4558_outl = 0;
static int32_t ak4558_outr = 0;

PE_ISR(I2S0_TX)
{
    I2S0_TDR0 = ak4558_outl;
    I2S0_TDR0 = ak4558_outr;
	ak4558_inl = I2S0_RDR0;
	ak4558_inr = I2S0_RDR0;

    I2S_PDD_ClearTxInterruptFlags(I2S0_BASE_PTR, I2S_PDD_ALL_INT_FLAG);

    dsp_work(&ak4558_outl, &ak4558_outr, ak4558_inl, ak4558_inr);
}

PE_ISR(I2S0_RX)
{
    I2S_PDD_ClearRxInterruptFlags(I2S0_BASE_PTR, I2S_PDD_ALL_INT_FLAG);
}
