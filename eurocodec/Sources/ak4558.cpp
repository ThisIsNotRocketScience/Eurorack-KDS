extern "C" {
#include "ak4558.h"

#include "Cpu.h"
#include "CODEC_PDN.h"
#include "WAIT1.h"
#include "I2C1.h"
#include "TESTLED.h"
#include "PDD_Includes.h"

#include "dsp_main.h"

void ak4558_init()
{
	CODEC_PDN_SetVal(CODEC_PDN_DeviceData);
	WAIT1_Waitms(100);
	volatile byte res = I2C1_SelectSlave(0b0010000);
	unsigned char ak4558_configdata[] = {
			0, 0b00011111,
			1, 0b00001110,
			2, 0b00000000,
			6, 0b00011001,
			7, 0b00001100,
			3, 0b00111110,
			//4, 0b00010100, // 192khz
			//5, 0b01110010, // 192khz
			4, 0b00010010, //96khz
			5, 0b01011010, //96khz
			8, 255,
			9, 255,
			1, 0b00001111
	};
	for (int i = 0; i < sizeof(ak4558_configdata); i += 2) {
		word sent = 0;
		byte res = I2C1_SendBlock(&ak4558_configdata[i], 2, &sent);
		WAIT1_Waitms(1);
	}
	WAIT1_Waitms(1);
}

static int32_t ak4558_inl = 0;
static int32_t ak4558_inr = 0;
static int32_t ak4558_outl = 0;
static int32_t ak4558_outr = 0;

PE_ISR(I2S0_TX)
{
	TESTLED_PutVal(TESTLED_DeviceData, 1);
	if (I2S_PDD_GetTxInterruptFlags(I2S0_BASE_PTR) & I2S_TCSR_FRF_MASK) {
		I2S0_TDR0 = ak4558_outl;
		I2S0_TDR0 = ak4558_outr;
	    dsp_work(&ak4558_outl, &ak4558_outr, ak4558_inl, ak4558_inr);
	}
	else {
		I2S_PDD_ClearTxInterruptFlags(I2S0_BASE_PTR, I2S_PDD_ALL_INT_FLAG);
	}

	TESTLED_PutVal(TESTLED_DeviceData, 0);
}

PE_ISR(I2S0_RX)
{
	if (I2S_PDD_GetRxInterruptFlags(I2S0_BASE_PTR) & I2S_RCSR_FRF_MASK) {
		ak4558_inl = I2S0_RDR0;
		ak4558_inr = I2S0_RDR0;
	}
	else {
		I2S_PDD_ClearRxInterruptFlags(I2S0_BASE_PTR, I2S_PDD_ALL_INT_FLAG);
	}
}

}; // extern "C"
