/* ###################################################################
 **     Filename    : Events.c
 **     Project     : Keyboard_Keyscanner
 **     Processor   : MK22FN256VLH12
 **     Component   : Events
 **     Version     : Driver 01.00
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2018-03-08, 23:03, # CodeGen: 0
 **     Abstract    :
 **         This is user's event module.
 **         Put your event handler code here.
 **     Contents    :
 **         Cpu_OnNMI - void Cpu_OnNMI(void);
 **
 ** ###################################################################*/
/*!
 ** @file Events.c
 ** @version 01.00
 ** @brief
 **         This is user's event module.
 **         Put your event handler code here.
 */
/*!
 **  @addtogroup Events_module Events module documentation
 **  @{
 */
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Init_Config.h"
#include "PDD_Includes.h"
#include <stdio.h>
#include "DAC_BitBang.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
 ** ===================================================================
 **     Event       :  Cpu_OnNMI (module Events)
 **
 **     Component   :  Cpu [MK22FN256DC12]
 */
/*!
 **     @brief
 **         This event is called when the Non maskable interrupt had
 **         occurred. This event is automatically enabled when the [NMI
 **         interrupt] property is set to 'Enabled'.
 */
/* ===================================================================*/
void Cpu_OnNMI(void)
{
	/* Write your code here ... */
}

int Keyboard[128]={0};
int Velocity[128]={0};
int KeyboardTime[128]={0};


int T;
int StateNew[128];
int StateTime[128];
int State[128];

enum
{
	STATE_IDLE = 0,
	STATE_MK =  1,
	STATE_BRK = 2,
	STATE_MKBRK = 3
};

void MK(int offs)
{
	StateNew[offs] += STATE_MK;
}

void BRK(int offs)
{
	StateNew[offs] += STATE_BRK;

}

int ConvertVelocity(int i)
{
	if (i<50) return 127;
	if (i>2000) return 1;
	return 127 - (((i-50)*127)/(2000-50));

}

extern int NoteVal ;
extern int VelVal ;


void NoteOn(int n, int v){
	Keyboard[n] = 1;
	Velocity[n] = v;
	KeyboardTime[n] = T;


}

void NoteOff(int n)
{
	Keyboard[n] = 0;
}

#define DAC_VOLT_UNCALIBRATED(x) ((int32_t)((409600 * ((int32_t)x)) / (int32_t)(512)))
#define DAC_VOLT(x, chan) CalibratedDAC(chan, ((int32_t)((409600 * ((int32_t)x)) / (int32_t)(512 * 64))))
#define DAC_NOTE(x, chan) (DAC_VOLT((x*64)/12, chan))

int CurrentCV = 0;
int lastIdx;
int LastV = 0;
void UpdateGateNote()
{
	int HighestIDX =-1 ;
	for (int i =0 ;i<37;i++)
	{
		if (Keyboard[i]>0) HighestIDX = i;
	};

	if (HighestIDX > -1)
	{
		NoteVal = ((HighestIDX+12)*4096)/(12*5);
		int V = (Velocity[HighestIDX] * 4095)/127;
		if (V!=LastV || NoteVal != CurrentCV || lastIdx !=HighestIDX )
		{
			LastV = V;
			DACBITBANG_Update(NoteVal, V);
			CurrentCV = NoteVal;
			lastIdx = HighestIDX ;
			//printf("%d: %d\n", lastIdx, CurrentCV);
		}
		GATEPIN_ClrVal(0);
	}
	else
	{
		GATEPIN_SetVal(0);
	}
}

void CheckNotes(uint32_t RV, int idx)
{
	uint32_t val = RV;
	int offs = idx * 6;
	for (int i =0;i<6;i++) StateNew[i+offs] = STATE_IDLE;

	if ((val & 0x01)!=0) BRK(offs+0);  // BRK1
	if ((val & 0x04)!=0) BRK(offs+1);  // BRK2
	if ((val & 0x20)!=0) BRK(offs+2);  // BRK3
	if ((val & 0x80)!=0) BRK(offs+3);  // BRK4
	if ((val & 0x200)!=0) BRK(offs+4); // BRK5
	if ((val & 0x800)!=0) BRK(offs+5); // BRK6

	if ((val & 0x02)!=0) MK(offs+0);   // MK1
	if ((val & 0x08)!=0) MK(offs+1);   // MK2
	if ((val & 0x10)!=0) MK(offs+2);   // MK3
	if ((val & 0x40)!=0) MK(offs+3);   // MK4
	if ((val & 0x100)!=0) MK(offs+4);  // MK5
	if ((val & 0x400)!=0) MK(offs+5);  // MK6

	for(int i = 0;i<6;i++)
	{
		int id = i + offs;
		if (State[id]!=StateNew[id])
		{
			//	printf("%d -> %x %x\n",i, StateNew[id], RV);
			switch(StateNew[id])
			{
			case STATE_IDLE:
				NoteOff(i*8 + idx);
				break;
			case STATE_MKBRK:
				NoteOn(i*8 + idx, ConvertVelocity(T-StateTime[id]));
				break;
			}
			StateTime[id] = T;
			State[id] = StateNew[id];
		}
	}


}
/*
 ** ===================================================================
 **     Event       :  TI1_OnInterrupt (module Events)
 **
 **     Component   :  TI1 [TimerInt_LDD]
 */
/*!
 **     @brief
 **         Called if periodic event occur. Component and OnInterrupt
 **         event must be enabled. See [SetEventMask] and [GetEventMask]
 **         methods. This event is available only if a [Interrupt
 **         service/event] is enabled.
 **     @param
 **         UserDataPtr     - Pointer to the user or
 **                           RTOS specific data. The pointer passed as
 **                           the parameter of Init method.
 */
/* ===================================================================*/
uint32_t Get()
{
	return GPIO_PDD_GetPortDataInput(GPIO1_MODULE_BASE_ADDRESS)& (GPIO1_TPortValue)GPIO1_C_MK_MASK;
}

uint32_t R[8];

void TI1_OnInterrupt(LDD_TUserData *UserDataPtr)
{
	T++;

	switch(T % 8)
	{
	case 0:

		SINK1_SetDir(0,1);
		SINK1_SetVal(0);
		WAIT1_Waitns(10);
		R[0] = Get();
		CheckNotes(R[0], 0);
		SINK1_ClrVal(0);
		SINK1_SetDir(0,0);
		break;
	case 1:

		SINK2_SetDir(0,1);
		SINK2_SetVal(0);
		WAIT1_Waitns(10);
		R[1] = Get();
		CheckNotes(R[1], 1);
		SINK2_ClrVal(0);
		SINK2_SetDir(0,0);
		break;
	case 2:

		SINK3_SetDir(0,1);
		SINK3_SetVal(0);
		WAIT1_Waitns(10);
		R[2] = Get();
		CheckNotes(R[2], 2);
		SINK3_ClrVal(0);
		SINK3_SetDir(0,0);
		break;
	case 3:
		SINK4_SetDir(0,1);
		SINK4_SetVal(0);
		WAIT1_Waitns(10);
		R[3] = Get();
		CheckNotes(R[3], 3);
		SINK4_ClrVal(0);
		SINK4_SetDir(0,0);
		break;
	case 4:
		SINK5_SetDir(0,1);
		SINK5_SetVal(0);
		WAIT1_Waitns(10);
		R[4] = Get();
		CheckNotes(R[4], 4);
		SINK5_ClrVal(0);
		SINK5_SetDir(0,0);
		break;
	case 5:
		SINK6_SetDir(0,1);
		SINK6_SetVal(0);
		WAIT1_Waitns(10);
		R[5] = Get();
		CheckNotes(R[5], 5);
		SINK6_ClrVal(0);
		SINK6_SetDir(0,0);
		break;
	case 6:
		SINK7_SetDir(0,1);
		SINK7_SetVal(0);
		WAIT1_Waitns(10);
		R[6] = Get();
		SINK7_ClrVal(0);
		SINK7_SetInput(0);
		CheckNotes(R[6], 6);
		break;
	case 7:
		SINK8_SetDir(0,1);
		SINK8_SetVal(0);
		WAIT1_Waitns(10);
		R[7] = Get();
		CheckNotes(R[7], 7);
		SINK8_ClrVal(0);
		SINK8_SetInput(0);

		break;
	};




	UpdateGateNote();
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.5 [05.21]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
