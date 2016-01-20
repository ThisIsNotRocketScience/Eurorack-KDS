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

// core_cm4_simd.h has a bug in it: qadd/qsub are signed!
__attribute__( ( always_inline ) ) static __INLINE int32_t __SQADD(int32_t op1, int32_t op2)
{
  uint32_t result;

  __ASM volatile ("qadd %0, %1, %2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

__attribute__( ( always_inline ) ) static __INLINE int32_t __SQSUB(int32_t op1, int32_t op2)
{
  uint32_t result;

  __ASM volatile ("qsub %0, %1, %2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

__attribute__( ( always_inline ) ) static __INLINE int32_t __QDADD(int32_t op1, int32_t op2)
{
  int32_t result;

  __ASM volatile ("qdadd %0, %1, %2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

__attribute__( ( always_inline ) ) static __INLINE uint32_t __UDIV64_24(uint32_t a_lo, uint32_t a_hi, uint32_t b)
{
  uint32_t result;

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
  "it cs\n"
  "subcs   %2,%2,%3\n"
  "adcs    %0,%1,%1\n"
		  : "=r" (result) : "r" (a_lo), "r" (a_hi), "r" (b) );

  return result;
}

inline uint32_t umull32_hi(uint32_t a, uint32_t b)
{
	return (uint32_t)(((uint64_t)a * (uint64_t)b) >> 32);
}

__attribute__( ( always_inline ) ) static __INLINE int32_t smmul(int32_t a, int32_t b)
{
	int32_t res_hi;

	__ASM volatile ("smmul %0, %1, %2" : "=r" (res_hi) : "r" (a), "r" (b) );

	return res_hi;
}

__attribute__( ( always_inline ) ) static __INLINE int32_t smlal32_hi(int32_t acc, int32_t a, int32_t b)
{
	int32_t tmp_lo = 0;

	__ASM volatile ("smlal %0, %1, %2, %3" : "+r" (tmp_lo), "+r" (acc) : "r" (a), "r" (b) );
	return acc;
}

__attribute__( ( always_inline ) ) static __INLINE int32_t smmla(int32_t acc, int32_t a, int32_t b)
{
	int32_t result;

	__ASM volatile ("smmlar %0, %2, %3, %1" : "=r" (result) : "r" (acc), "r" (a), "r" (b) );
	return result;
}
