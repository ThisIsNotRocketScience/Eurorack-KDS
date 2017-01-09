#include "PE_Types.h"
extern volatile uint8_t SR_lock;
extern volatile uint8_t SR_reg;

extern void EnterCritical();
extern void ExitCritical();

void EnterCritical()
{
	do {
		uint8_t SR_reg_local;
		/*lint -save  -e586 -e950 Disable MISRA rule (2.1,1.1) checking. */
		__asm (
				"MRS R0, PRIMASK\n\t"
				"CPSID i\n\t"
				"STRB R0, %[output]"
				: [output] "=m" (SR_reg_local)
				  :: "r0");
		/*lint -restore Enable MISRA rule (2.1,1.1) checking. */
		if (++SR_lock == 1u) {
			SR_reg = SR_reg_local;
		}
	} while(0);
}



void ExitCritical()
{
	do {
		if (--SR_lock == 0u) {
			/*lint -save  -e586 -e950 Disable MISRA rule (2.1,1.1) checking. */
			__asm (
					"ldrb r0, %[input]\n\t"
					"msr PRIMASK,r0;\n\t"
					::[input] "m" (SR_reg)
					 : "r0");
			/*lint -restore Enable MISRA rule (2.1,1.1) checking. */
		}
	} while(0);
}
