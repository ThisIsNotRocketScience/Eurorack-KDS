#pragma once

#define __INLINE inline
#define __ASM __asm__
#include "core_cm4_simd.h"

#define __SSAT_NOSHIFT(ARG1,ARG2) \
({                          \
  uint32_t __RES, __ARG1 = (ARG1); \
  __ASM ("ssat %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1) ); \
  __RES; \
 })

#define __SSAT_LSL(ARG1,ARG2,SHIFT) \
({                          \
  uint32_t __RES, __ARG1 = (ARG1); \
  __ASM ("ssat %0, %1, %2, LSL %3" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1), "I" (SHIFT) ); \
  __RES; \
 })

__attribute__( ( always_inline ) ) static __INLINE uint32_t __QDADD(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  __ASM volatile ("qdadd %0, %1, %2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

__attribute__( ( always_inline ) ) static __INLINE uint32_t __UDIV64(uint32_t a_lo, uint32_t a_hi, uint32_t b)
{
  uint32_t result;

  //__ASM volatile ("qdadd %0, %1, %2" : "=r" (result) : "r" (op1), "r" (op2) );

  __ASM volatile (
  "adds      %1,%1,%1\n"
  "adc       %2,%2,%2\n"

  ".rept 23\n" //31
      "cmp     %2,%3\n"
      "it cs\n"
      "subcs   %2,%2,%3\n"
      "adcs    %1,%1,%1\n"
      "adc     %2,%2,%2\n"
  ".endr\n"

  "cmp     %2,%3\n"
  //subcs    r1,r1,r2
  "it cs\n"
  "subcs   %2,%2,%3\n"
  "adcs    %0,%1,%1\n"
		  : "=r" (result) : "r" (a_lo), "r" (a_hi), "r" (b) );

  return(result);
}

inline uint32_t umull32_hi(uint32_t a, uint32_t b)
{
	return (uint32_t)(((uint64_t)a * (uint64_t)b) >> 32);
}

inline int32_t smull32_hi(int32_t a, int32_t b)
{
	return (int32_t)(((int64_t)a * (int64_t)b) >> 32);
}

