/*
** ###################################################################
**     Processors:          MKL02Z32CAF4
**                          MKL02Z32VFG4
**                          MKL02Z16VFG4
**                          MKL02Z8VFG4
**                          MKL02Z32VFK4
**                          MKL02Z16VFK4
**                          MKL02Z32VFM4
**                          MKL02Z16VFM4
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manuals:   KL02P20M48SF0RM Rev2.1, July 2013
**                          KL02P32M48SF0RM Rev3.1, July 2013
**
**     Version:             rev. 1.1, 2013-04-05
**     Build:               b150309
**
**     Abstract:
**         This header file implements peripheral memory map for MKL02Z4
**         processor.
**
**     Copyright (c) 1997 - 2013 Freescale Semiconductor, Inc.
**     All Rights Reserved.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2012-10-04)
**         Initial version.
**     - rev. 1.1 (2013-04-05)
**         Changed start of doxygen comment.
**
** ###################################################################
*/

/*!
 * @file MKL02Z4.h
 * @version 1.1
 * @date 2013-04-05
 * @brief Peripheral memory map for MKL02Z4
 *
 * This header file implements peripheral memory map for MKL02Z4 processor.
 */


/* ----------------------------------------------------------------------------
   -- MCU activation
   ---------------------------------------------------------------------------- */

/* Prevention from multiple including the same memory map */
#if !defined(MCU_MKL02Z4)  /* Check if memory map has not been already included */
#define MCU_MKL02Z4

/* Check if another memory map has not been also included */
#if (defined(MCU_ACTIVE))
  #error MKL02Z4 memory map: There is already included another memory map. Only one memory map can be included.
#endif /* (defined(MCU_ACTIVE)) */
#define MCU_ACTIVE

#include <stdint.h>

/** Memory map major version (memory maps with equal major version number are
 * compatible) */
#define MCU_MEM_MAP_VERSION 0x0100u
/** Memory map minor version */
#define MCU_MEM_MAP_VERSION_MINOR 0x0001u


/* ----------------------------------------------------------------------------
   -- Interrupt vector numbers
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Interrupt_vector_numbers Interrupt vector numbers
 * @{
 */

/** Interrupt Number Definitions */
typedef enum {
  INT_Initial_Stack_Pointer    = 0,                /**< Initial Stack Pointer */
  INT_Initial_Program_Counter  = 1,                /**< Initial Program Counter */
  INT_NMI                      = 2,                /**< Non-maskable Interrupt (NMI) */
  INT_Hard_Fault               = 3,                /**< Hard Fault */
  INT_Reserved4                = 4,                /**< Reserved interrupt */
  INT_Reserved5                = 5,                /**< Reserved interrupt */
  INT_Reserved6                = 6,                /**< Reserved interrupt */
  INT_Reserved7                = 7,                /**< Reserved interrupt */
  INT_Reserved8                = 8,                /**< Reserved interrupt */
  INT_Reserved9                = 9,                /**< Reserved interrupt */
  INT_Reserved10               = 10,               /**< Reserved interrupt */
  INT_SVCall                   = 11,               /**< Supervisor call (SVCall) */
  INT_Reserved12               = 12,               /**< Reserved interrupt */
  INT_Reserved13               = 13,               /**< Reserved interrupt */
  INT_PendableSrvReq           = 14,               /**< Pendable request for system service (PendableSrvReq) */
  INT_SysTick                  = 15,               /**< System tick timer (SysTick) */
  INT_Reserved16               = 16,               /**< Reserved interrupt */
  INT_Reserved17               = 17,               /**< Reserved interrupt */
  INT_Reserved18               = 18,               /**< Reserved interrupt */
  INT_Reserved19               = 19,               /**< Reserved interrupt */
  INT_Reserved20               = 20,               /**< Reserved interrupt */
  INT_FTFA                     = 21,               /**< FTFA command complete and read collision */
  INT_LVD_LVW                  = 22,               /**< Low-voltage detect, low-voltage warning */
  INT_Reserved23               = 23,               /**< Reserved interrupt */
  INT_I2C0                     = 24,               /**< I2C0 interrupt */
  INT_I2C1                     = 25,               /**< I2C1 interrupt */
  INT_SPI0                     = 26,               /**< SPI0 single interrupt vector for all sources */
  INT_Reserved27               = 27,               /**< Reserved interrupt */
  INT_UART0                    = 28,               /**< UART0 status and error */
  INT_Reserved29               = 29,               /**< Reserved interrupt */
  INT_Reserved30               = 30,               /**< Reserved interrupt */
  INT_ADC0                     = 31,               /**< ADC0 interrupt */
  INT_CMP0                     = 32,               /**< CMP0 interrupt */
  INT_TPM0                     = 33,               /**< TPM0 single interrupt vector for all sources */
  INT_TPM1                     = 34,               /**< TPM1 single interrupt vector for all sources */
  INT_Reserved35               = 35,               /**< Reserved interrupt */
  INT_Reserved36               = 36,               /**< Reserved interrupt */
  INT_Reserved37               = 37,               /**< Reserved interrupt */
  INT_Reserved38               = 38,               /**< Reserved interrupt */
  INT_Reserved39               = 39,               /**< Reserved interrupt */
  INT_Reserved40               = 40,               /**< Reserved interrupt */
  INT_Reserved41               = 41,               /**< Reserved interrupt */
  INT_Reserved42               = 42,               /**< Reserved interrupt */
  INT_MCG                      = 43,               /**< MCG interrupt */
  INT_LPTMR0                   = 44,               /**< LPTMR0 interrupt */
  INT_Reserved45               = 45,               /**< Reserved interrupt */
  INT_PORTA                    = 46,               /**< PORTA pin detect */
  INT_PORTB                    = 47                /**< PORTB pin detect */
} IRQInterruptIndex;

/*!
 * @}
 */ /* end of group Interrupt_vector_numbers */


/* ----------------------------------------------------------------------------
   -- Peripheral type defines
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Peripheral_defines Peripheral type defines
 * @{
 */


/*
** Start of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #pragma push
  #pragma anon_unions
#elif defined(__CWCC__)
  #pragma push
  #pragma cpp_extensions on
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=extended
#else
  #error Not supported compiler type
#endif

/* ----------------------------------------------------------------------------
   -- ADC
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Peripheral ADC
 * @{
 */

/** ADC - Peripheral register structure */
typedef struct ADC_MemMap {
  uint32_t SC1[2];                                 /**< ADC Status and Control Registers 1, array offset: 0x0, array step: 0x4 */
  uint32_t CFG1;                                   /**< ADC Configuration Register 1, offset: 0x8 */
  uint32_t CFG2;                                   /**< ADC Configuration Register 2, offset: 0xC */
  uint32_t R[2];                                   /**< ADC Data Result Register, array offset: 0x10, array step: 0x4 */
  uint32_t CV1;                                    /**< Compare Value Registers, offset: 0x18 */
  uint32_t CV2;                                    /**< Compare Value Registers, offset: 0x1C */
  uint32_t SC2;                                    /**< Status and Control Register 2, offset: 0x20 */
  uint32_t SC3;                                    /**< Status and Control Register 3, offset: 0x24 */
  uint32_t OFS;                                    /**< ADC Offset Correction Register, offset: 0x28 */
  uint32_t PG;                                     /**< ADC Plus-Side Gain Register, offset: 0x2C */
  uint8_t RESERVED_0[4];
  uint32_t CLPD;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x34 */
  uint32_t CLPS;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x38 */
  uint32_t CLP4;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x3C */
  uint32_t CLP3;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x40 */
  uint32_t CLP2;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x44 */
  uint32_t CLP1;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x48 */
  uint32_t CLP0;                                   /**< ADC Plus-Side General Calibration Value Register, offset: 0x4C */
} volatile *ADC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- ADC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Register_Accessor_Macros ADC - Register accessor macros
 * @{
 */


/* ADC - Register accessors */
#define ADC_SC1_REG(base,index)                  ((base)->SC1[index])
#define ADC_CFG1_REG(base)                       ((base)->CFG1)
#define ADC_CFG2_REG(base)                       ((base)->CFG2)
#define ADC_R_REG(base,index)                    ((base)->R[index])
#define ADC_CV1_REG(base)                        ((base)->CV1)
#define ADC_CV2_REG(base)                        ((base)->CV2)
#define ADC_SC2_REG(base)                        ((base)->SC2)
#define ADC_SC3_REG(base)                        ((base)->SC3)
#define ADC_OFS_REG(base)                        ((base)->OFS)
#define ADC_PG_REG(base)                         ((base)->PG)
#define ADC_CLPD_REG(base)                       ((base)->CLPD)
#define ADC_CLPS_REG(base)                       ((base)->CLPS)
#define ADC_CLP4_REG(base)                       ((base)->CLP4)
#define ADC_CLP3_REG(base)                       ((base)->CLP3)
#define ADC_CLP2_REG(base)                       ((base)->CLP2)
#define ADC_CLP1_REG(base)                       ((base)->CLP1)
#define ADC_CLP0_REG(base)                       ((base)->CLP0)

/*!
 * @}
 */ /* end of group ADC_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- ADC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Register_Masks ADC Register Masks
 * @{
 */

/* SC1 Bit Fields */
#define ADC_SC1_ADCH_MASK                        0x1Fu
#define ADC_SC1_ADCH_SHIFT                       0
#define ADC_SC1_ADCH(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC1_ADCH_SHIFT))&ADC_SC1_ADCH_MASK)
#define ADC_SC1_AIEN_MASK                        0x40u
#define ADC_SC1_AIEN_SHIFT                       6
#define ADC_SC1_COCO_MASK                        0x80u
#define ADC_SC1_COCO_SHIFT                       7
/* CFG1 Bit Fields */
#define ADC_CFG1_ADICLK_MASK                     0x3u
#define ADC_CFG1_ADICLK_SHIFT                    0
#define ADC_CFG1_ADICLK(x)                       (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_ADICLK_SHIFT))&ADC_CFG1_ADICLK_MASK)
#define ADC_CFG1_MODE_MASK                       0xCu
#define ADC_CFG1_MODE_SHIFT                      2
#define ADC_CFG1_MODE(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_MODE_SHIFT))&ADC_CFG1_MODE_MASK)
#define ADC_CFG1_ADLSMP_MASK                     0x10u
#define ADC_CFG1_ADLSMP_SHIFT                    4
#define ADC_CFG1_ADIV_MASK                       0x60u
#define ADC_CFG1_ADIV_SHIFT                      5
#define ADC_CFG1_ADIV(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_ADIV_SHIFT))&ADC_CFG1_ADIV_MASK)
#define ADC_CFG1_ADLPC_MASK                      0x80u
#define ADC_CFG1_ADLPC_SHIFT                     7
/* CFG2 Bit Fields */
#define ADC_CFG2_ADLSTS_MASK                     0x3u
#define ADC_CFG2_ADLSTS_SHIFT                    0
#define ADC_CFG2_ADLSTS(x)                       (((uint32_t)(((uint32_t)(x))<<ADC_CFG2_ADLSTS_SHIFT))&ADC_CFG2_ADLSTS_MASK)
#define ADC_CFG2_ADHSC_MASK                      0x4u
#define ADC_CFG2_ADHSC_SHIFT                     2
#define ADC_CFG2_ADACKEN_MASK                    0x8u
#define ADC_CFG2_ADACKEN_SHIFT                   3
#define ADC_CFG2_MUXSEL_MASK                     0x10u
#define ADC_CFG2_MUXSEL_SHIFT                    4
/* R Bit Fields */
#define ADC_R_D_MASK                             0xFFFFu
#define ADC_R_D_SHIFT                            0
#define ADC_R_D(x)                               (((uint32_t)(((uint32_t)(x))<<ADC_R_D_SHIFT))&ADC_R_D_MASK)
/* CV1 Bit Fields */
#define ADC_CV1_CV_MASK                          0xFFFFu
#define ADC_CV1_CV_SHIFT                         0
#define ADC_CV1_CV(x)                            (((uint32_t)(((uint32_t)(x))<<ADC_CV1_CV_SHIFT))&ADC_CV1_CV_MASK)
/* CV2 Bit Fields */
#define ADC_CV2_CV_MASK                          0xFFFFu
#define ADC_CV2_CV_SHIFT                         0
#define ADC_CV2_CV(x)                            (((uint32_t)(((uint32_t)(x))<<ADC_CV2_CV_SHIFT))&ADC_CV2_CV_MASK)
/* SC2 Bit Fields */
#define ADC_SC2_REFSEL_MASK                      0x3u
#define ADC_SC2_REFSEL_SHIFT                     0
#define ADC_SC2_REFSEL(x)                        (((uint32_t)(((uint32_t)(x))<<ADC_SC2_REFSEL_SHIFT))&ADC_SC2_REFSEL_MASK)
#define ADC_SC2_ACREN_MASK                       0x8u
#define ADC_SC2_ACREN_SHIFT                      3
#define ADC_SC2_ACFGT_MASK                       0x10u
#define ADC_SC2_ACFGT_SHIFT                      4
#define ADC_SC2_ACFE_MASK                        0x20u
#define ADC_SC2_ACFE_SHIFT                       5
#define ADC_SC2_ADTRG_MASK                       0x40u
#define ADC_SC2_ADTRG_SHIFT                      6
#define ADC_SC2_ADACT_MASK                       0x80u
#define ADC_SC2_ADACT_SHIFT                      7
/* SC3 Bit Fields */
#define ADC_SC3_AVGS_MASK                        0x3u
#define ADC_SC3_AVGS_SHIFT                       0
#define ADC_SC3_AVGS(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC3_AVGS_SHIFT))&ADC_SC3_AVGS_MASK)
#define ADC_SC3_AVGE_MASK                        0x4u
#define ADC_SC3_AVGE_SHIFT                       2
#define ADC_SC3_ADCO_MASK                        0x8u
#define ADC_SC3_ADCO_SHIFT                       3
#define ADC_SC3_CALF_MASK                        0x40u
#define ADC_SC3_CALF_SHIFT                       6
#define ADC_SC3_CAL_MASK                         0x80u
#define ADC_SC3_CAL_SHIFT                        7
/* OFS Bit Fields */
#define ADC_OFS_OFS_MASK                         0xFFFFu
#define ADC_OFS_OFS_SHIFT                        0
#define ADC_OFS_OFS(x)                           (((uint32_t)(((uint32_t)(x))<<ADC_OFS_OFS_SHIFT))&ADC_OFS_OFS_MASK)
/* PG Bit Fields */
#define ADC_PG_PG_MASK                           0xFFFFu
#define ADC_PG_PG_SHIFT                          0
#define ADC_PG_PG(x)                             (((uint32_t)(((uint32_t)(x))<<ADC_PG_PG_SHIFT))&ADC_PG_PG_MASK)
/* CLPD Bit Fields */
#define ADC_CLPD_CLPD_MASK                       0x3Fu
#define ADC_CLPD_CLPD_SHIFT                      0
#define ADC_CLPD_CLPD(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLPD_CLPD_SHIFT))&ADC_CLPD_CLPD_MASK)
/* CLPS Bit Fields */
#define ADC_CLPS_CLPS_MASK                       0x3Fu
#define ADC_CLPS_CLPS_SHIFT                      0
#define ADC_CLPS_CLPS(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLPS_CLPS_SHIFT))&ADC_CLPS_CLPS_MASK)
/* CLP4 Bit Fields */
#define ADC_CLP4_CLP4_MASK                       0x3FFu
#define ADC_CLP4_CLP4_SHIFT                      0
#define ADC_CLP4_CLP4(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLP4_CLP4_SHIFT))&ADC_CLP4_CLP4_MASK)
/* CLP3 Bit Fields */
#define ADC_CLP3_CLP3_MASK                       0x1FFu
#define ADC_CLP3_CLP3_SHIFT                      0
#define ADC_CLP3_CLP3(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLP3_CLP3_SHIFT))&ADC_CLP3_CLP3_MASK)
/* CLP2 Bit Fields */
#define ADC_CLP2_CLP2_MASK                       0xFFu
#define ADC_CLP2_CLP2_SHIFT                      0
#define ADC_CLP2_CLP2(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLP2_CLP2_SHIFT))&ADC_CLP2_CLP2_MASK)
/* CLP1 Bit Fields */
#define ADC_CLP1_CLP1_MASK                       0x7Fu
#define ADC_CLP1_CLP1_SHIFT                      0
#define ADC_CLP1_CLP1(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLP1_CLP1_SHIFT))&ADC_CLP1_CLP1_MASK)
/* CLP0 Bit Fields */
#define ADC_CLP0_CLP0_MASK                       0x3Fu
#define ADC_CLP0_CLP0_SHIFT                      0
#define ADC_CLP0_CLP0(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_CLP0_CLP0_SHIFT))&ADC_CLP0_CLP0_MASK)

/*!
 * @}
 */ /* end of group ADC_Register_Masks */


/* ADC - Peripheral instance base addresses */
/** Peripheral ADC0 base pointer */
#define ADC0_BASE_PTR                            ((ADC_MemMapPtr)0x4003B000u)
/** Array initializer of ADC peripheral base pointers */
#define ADC_BASE_PTRS                            { ADC0_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- ADC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Register_Accessor_Macros ADC - Register accessor macros
 * @{
 */


/* ADC - Register instance definitions */
/* ADC0 */
#define ADC0_SC1A                                ADC_SC1_REG(ADC0_BASE_PTR,0)
#define ADC0_SC1B                                ADC_SC1_REG(ADC0_BASE_PTR,1)
#define ADC0_CFG1                                ADC_CFG1_REG(ADC0_BASE_PTR)
#define ADC0_CFG2                                ADC_CFG2_REG(ADC0_BASE_PTR)
#define ADC0_RA                                  ADC_R_REG(ADC0_BASE_PTR,0)
#define ADC0_RB                                  ADC_R_REG(ADC0_BASE_PTR,1)
#define ADC0_CV1                                 ADC_CV1_REG(ADC0_BASE_PTR)
#define ADC0_CV2                                 ADC_CV2_REG(ADC0_BASE_PTR)
#define ADC0_SC2                                 ADC_SC2_REG(ADC0_BASE_PTR)
#define ADC0_SC3                                 ADC_SC3_REG(ADC0_BASE_PTR)
#define ADC0_OFS                                 ADC_OFS_REG(ADC0_BASE_PTR)
#define ADC0_PG                                  ADC_PG_REG(ADC0_BASE_PTR)
#define ADC0_CLPD                                ADC_CLPD_REG(ADC0_BASE_PTR)
#define ADC0_CLPS                                ADC_CLPS_REG(ADC0_BASE_PTR)
#define ADC0_CLP4                                ADC_CLP4_REG(ADC0_BASE_PTR)
#define ADC0_CLP3                                ADC_CLP3_REG(ADC0_BASE_PTR)
#define ADC0_CLP2                                ADC_CLP2_REG(ADC0_BASE_PTR)
#define ADC0_CLP1                                ADC_CLP1_REG(ADC0_BASE_PTR)
#define ADC0_CLP0                                ADC_CLP0_REG(ADC0_BASE_PTR)

/* ADC - Register array accessors */
#define ADC0_SC1(index)                          ADC_SC1_REG(ADC0_BASE_PTR,index)
#define ADC0_R(index)                            ADC_R_REG(ADC0_BASE_PTR,index)

/*!
 * @}
 */ /* end of group ADC_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group ADC_Peripheral */


/* ----------------------------------------------------------------------------
   -- BP
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup BP_Peripheral BP
 * @{
 */

/** BP - Peripheral register structure */
typedef struct BP_MemMap {
  uint32_t CTRL;                                   /**< FlashPatch Control Register, offset: 0x0 */
  uint8_t RESERVED_0[4];
  uint32_t COMP[2];                                /**< FlashPatch Comparator Register 0..FlashPatch Comparator Register 1, array offset: 0x8, array step: 0x4 */
  uint8_t RESERVED_1[4032];
  uint32_t PID4;                                   /**< Peripheral Identification Register 4., offset: 0xFD0 */
  uint32_t PID5;                                   /**< Peripheral Identification Register 5., offset: 0xFD4 */
  uint32_t PID6;                                   /**< Peripheral Identification Register 6., offset: 0xFD8 */
  uint32_t PID7;                                   /**< Peripheral Identification Register 7., offset: 0xFDC */
  uint32_t PID0;                                   /**< Peripheral Identification Register 0., offset: 0xFE0 */
  uint32_t PID1;                                   /**< Peripheral Identification Register 1., offset: 0xFE4 */
  uint32_t PID2;                                   /**< Peripheral Identification Register 2., offset: 0xFE8 */
  uint32_t PID3;                                   /**< Peripheral Identification Register 3., offset: 0xFEC */
  uint32_t CID0;                                   /**< Component Identification Register 0., offset: 0xFF0 */
  uint32_t CID1;                                   /**< Component Identification Register 1., offset: 0xFF4 */
  uint32_t CID2;                                   /**< Component Identification Register 2., offset: 0xFF8 */
  uint32_t CID3;                                   /**< Component Identification Register 3., offset: 0xFFC */
} volatile *BP_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- BP - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup BP_Register_Accessor_Macros BP - Register accessor macros
 * @{
 */


/* BP - Register accessors */
#define BP_CTRL_REG(base)                        ((base)->CTRL)
#define BP_COMP_REG(base,index)                  ((base)->COMP[index])
#define BP_PID4_REG(base)                        ((base)->PID4)
#define BP_PID5_REG(base)                        ((base)->PID5)
#define BP_PID6_REG(base)                        ((base)->PID6)
#define BP_PID7_REG(base)                        ((base)->PID7)
#define BP_PID0_REG(base)                        ((base)->PID0)
#define BP_PID1_REG(base)                        ((base)->PID1)
#define BP_PID2_REG(base)                        ((base)->PID2)
#define BP_PID3_REG(base)                        ((base)->PID3)
#define BP_CID0_REG(base)                        ((base)->CID0)
#define BP_CID1_REG(base)                        ((base)->CID1)
#define BP_CID2_REG(base)                        ((base)->CID2)
#define BP_CID3_REG(base)                        ((base)->CID3)

/*!
 * @}
 */ /* end of group BP_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- BP Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup BP_Register_Masks BP Register Masks
 * @{
 */


/*!
 * @}
 */ /* end of group BP_Register_Masks */


/* BP - Peripheral instance base addresses */
/** Peripheral BP base pointer */
#define BP_BASE_PTR                              ((BP_MemMapPtr)0xE0002000u)
/** Array initializer of BP peripheral base pointers */
#define BP_BASE_PTRS                             { BP_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- BP - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup BP_Register_Accessor_Macros BP - Register accessor macros
 * @{
 */


/* BP - Register instance definitions */
/* BP */
#define BP_CTRL                                  BP_CTRL_REG(BP_BASE_PTR)
#define BP_COMP0                                 BP_COMP_REG(BP_BASE_PTR,0)
#define BP_COMP1                                 BP_COMP_REG(BP_BASE_PTR,1)
#define BP_PID4                                  BP_PID4_REG(BP_BASE_PTR)
#define BP_PID5                                  BP_PID5_REG(BP_BASE_PTR)
#define BP_PID6                                  BP_PID6_REG(BP_BASE_PTR)
#define BP_PID7                                  BP_PID7_REG(BP_BASE_PTR)
#define BP_PID0                                  BP_PID0_REG(BP_BASE_PTR)
#define BP_PID1                                  BP_PID1_REG(BP_BASE_PTR)
#define BP_PID2                                  BP_PID2_REG(BP_BASE_PTR)
#define BP_PID3                                  BP_PID3_REG(BP_BASE_PTR)
#define BP_CID0                                  BP_CID0_REG(BP_BASE_PTR)
#define BP_CID1                                  BP_CID1_REG(BP_BASE_PTR)
#define BP_CID2                                  BP_CID2_REG(BP_BASE_PTR)
#define BP_CID3                                  BP_CID3_REG(BP_BASE_PTR)

/* BP - Register array accessors */
#define BP_COMP(index)                           BP_COMP_REG(BP_BASE_PTR,index)

/*!
 * @}
 */ /* end of group BP_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group BP_Peripheral */


/* ----------------------------------------------------------------------------
   -- CMP
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CMP_Peripheral CMP
 * @{
 */

/** CMP - Peripheral register structure */
typedef struct CMP_MemMap {
  uint8_t CR0;                                     /**< CMP Control Register 0, offset: 0x0 */
  uint8_t CR1;                                     /**< CMP Control Register 1, offset: 0x1 */
  uint8_t FPR;                                     /**< CMP Filter Period Register, offset: 0x2 */
  uint8_t SCR;                                     /**< CMP Status and Control Register, offset: 0x3 */
  uint8_t DACCR;                                   /**< DAC Control Register, offset: 0x4 */
  uint8_t MUXCR;                                   /**< MUX Control Register, offset: 0x5 */
} volatile *CMP_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- CMP - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CMP_Register_Accessor_Macros CMP - Register accessor macros
 * @{
 */


/* CMP - Register accessors */
#define CMP_CR0_REG(base)                        ((base)->CR0)
#define CMP_CR1_REG(base)                        ((base)->CR1)
#define CMP_FPR_REG(base)                        ((base)->FPR)
#define CMP_SCR_REG(base)                        ((base)->SCR)
#define CMP_DACCR_REG(base)                      ((base)->DACCR)
#define CMP_MUXCR_REG(base)                      ((base)->MUXCR)

/*!
 * @}
 */ /* end of group CMP_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- CMP Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CMP_Register_Masks CMP Register Masks
 * @{
 */

/* CR0 Bit Fields */
#define CMP_CR0_HYSTCTR_MASK                     0x3u
#define CMP_CR0_HYSTCTR_SHIFT                    0
#define CMP_CR0_HYSTCTR(x)                       (((uint8_t)(((uint8_t)(x))<<CMP_CR0_HYSTCTR_SHIFT))&CMP_CR0_HYSTCTR_MASK)
#define CMP_CR0_FILTER_CNT_MASK                  0x70u
#define CMP_CR0_FILTER_CNT_SHIFT                 4
#define CMP_CR0_FILTER_CNT(x)                    (((uint8_t)(((uint8_t)(x))<<CMP_CR0_FILTER_CNT_SHIFT))&CMP_CR0_FILTER_CNT_MASK)
/* CR1 Bit Fields */
#define CMP_CR1_EN_MASK                          0x1u
#define CMP_CR1_EN_SHIFT                         0
#define CMP_CR1_OPE_MASK                         0x2u
#define CMP_CR1_OPE_SHIFT                        1
#define CMP_CR1_COS_MASK                         0x4u
#define CMP_CR1_COS_SHIFT                        2
#define CMP_CR1_INV_MASK                         0x8u
#define CMP_CR1_INV_SHIFT                        3
#define CMP_CR1_PMODE_MASK                       0x10u
#define CMP_CR1_PMODE_SHIFT                      4
#define CMP_CR1_TRIGM_MASK                       0x20u
#define CMP_CR1_TRIGM_SHIFT                      5
#define CMP_CR1_WE_MASK                          0x40u
#define CMP_CR1_WE_SHIFT                         6
#define CMP_CR1_SE_MASK                          0x80u
#define CMP_CR1_SE_SHIFT                         7
/* FPR Bit Fields */
#define CMP_FPR_FILT_PER_MASK                    0xFFu
#define CMP_FPR_FILT_PER_SHIFT                   0
#define CMP_FPR_FILT_PER(x)                      (((uint8_t)(((uint8_t)(x))<<CMP_FPR_FILT_PER_SHIFT))&CMP_FPR_FILT_PER_MASK)
/* SCR Bit Fields */
#define CMP_SCR_COUT_MASK                        0x1u
#define CMP_SCR_COUT_SHIFT                       0
#define CMP_SCR_CFF_MASK                         0x2u
#define CMP_SCR_CFF_SHIFT                        1
#define CMP_SCR_CFR_MASK                         0x4u
#define CMP_SCR_CFR_SHIFT                        2
#define CMP_SCR_IEF_MASK                         0x8u
#define CMP_SCR_IEF_SHIFT                        3
#define CMP_SCR_IER_MASK                         0x10u
#define CMP_SCR_IER_SHIFT                        4
/* DACCR Bit Fields */
#define CMP_DACCR_VOSEL_MASK                     0x3Fu
#define CMP_DACCR_VOSEL_SHIFT                    0
#define CMP_DACCR_VOSEL(x)                       (((uint8_t)(((uint8_t)(x))<<CMP_DACCR_VOSEL_SHIFT))&CMP_DACCR_VOSEL_MASK)
#define CMP_DACCR_VRSEL_MASK                     0x40u
#define CMP_DACCR_VRSEL_SHIFT                    6
#define CMP_DACCR_DACEN_MASK                     0x80u
#define CMP_DACCR_DACEN_SHIFT                    7
/* MUXCR Bit Fields */
#define CMP_MUXCR_MSEL_MASK                      0x7u
#define CMP_MUXCR_MSEL_SHIFT                     0
#define CMP_MUXCR_MSEL(x)                        (((uint8_t)(((uint8_t)(x))<<CMP_MUXCR_MSEL_SHIFT))&CMP_MUXCR_MSEL_MASK)
#define CMP_MUXCR_PSEL_MASK                      0x38u
#define CMP_MUXCR_PSEL_SHIFT                     3
#define CMP_MUXCR_PSEL(x)                        (((uint8_t)(((uint8_t)(x))<<CMP_MUXCR_PSEL_SHIFT))&CMP_MUXCR_PSEL_MASK)

/*!
 * @}
 */ /* end of group CMP_Register_Masks */


/* CMP - Peripheral instance base addresses */
/** Peripheral CMP0 base pointer */
#define CMP0_BASE_PTR                            ((CMP_MemMapPtr)0x40073000u)
/** Array initializer of CMP peripheral base pointers */
#define CMP_BASE_PTRS                            { CMP0_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- CMP - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CMP_Register_Accessor_Macros CMP - Register accessor macros
 * @{
 */


/* CMP - Register instance definitions */
/* CMP0 */
#define CMP0_CR0                                 CMP_CR0_REG(CMP0_BASE_PTR)
#define CMP0_CR1                                 CMP_CR1_REG(CMP0_BASE_PTR)
#define CMP0_FPR                                 CMP_FPR_REG(CMP0_BASE_PTR)
#define CMP0_SCR                                 CMP_SCR_REG(CMP0_BASE_PTR)
#define CMP0_DACCR                               CMP_DACCR_REG(CMP0_BASE_PTR)
#define CMP0_MUXCR                               CMP_MUXCR_REG(CMP0_BASE_PTR)

/*!
 * @}
 */ /* end of group CMP_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group CMP_Peripheral */


/* ----------------------------------------------------------------------------
   -- CoreDebug
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CoreDebug_Peripheral CoreDebug
 * @{
 */

/** CoreDebug - Peripheral register structure */
typedef struct CoreDebug_MemMap {
  union {                                          /* offset: 0x0 */
    uint32_t base_DHCSR_Read;                        /**< Debug Halting Control and Status Register, offset: 0x0 */
    uint32_t base_DHCSR_Write;                       /**< Debug Halting Control and Status Register, offset: 0x0 */
  };
  uint32_t base_DCRSR;                             /**< Debug Core Register Selector Register, offset: 0x4 */
  uint32_t base_DCRDR;                             /**< Debug Core Register Data Register, offset: 0x8 */
  uint32_t base_DEMCR;                             /**< Debug Exception and Monitor Control Register, offset: 0xC */
} volatile *CoreDebug_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- CoreDebug - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CoreDebug_Register_Accessor_Macros CoreDebug - Register accessor macros
 * @{
 */


/* CoreDebug - Register accessors */
#define CoreDebug_base_DHCSR_Read_REG(base)      ((base)->base_DHCSR_Read)
#define CoreDebug_base_DHCSR_Write_REG(base)     ((base)->base_DHCSR_Write)
#define CoreDebug_base_DCRSR_REG(base)           ((base)->base_DCRSR)
#define CoreDebug_base_DCRDR_REG(base)           ((base)->base_DCRDR)
#define CoreDebug_base_DEMCR_REG(base)           ((base)->base_DEMCR)

/*!
 * @}
 */ /* end of group CoreDebug_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- CoreDebug Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CoreDebug_Register_Masks CoreDebug Register Masks
 * @{
 */


/*!
 * @}
 */ /* end of group CoreDebug_Register_Masks */


/* CoreDebug - Peripheral instance base addresses */
/** Peripheral CoreDebug base pointer */
#define CoreDebug_BASE_PTR                       ((CoreDebug_MemMapPtr)0xE000EDF0u)
/** Array initializer of CoreDebug peripheral base pointers */
#define CoreDebug_BASE_PTRS                      { CoreDebug_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- CoreDebug - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CoreDebug_Register_Accessor_Macros CoreDebug - Register accessor macros
 * @{
 */


/* CoreDebug - Register instance definitions */
/* CoreDebug */
#define DHCSR_Read                               CoreDebug_base_DHCSR_Read_REG(CoreDebug_BASE_PTR)
#define DHCSR_Write                              CoreDebug_base_DHCSR_Write_REG(CoreDebug_BASE_PTR)
#define DCRSR                                    CoreDebug_base_DCRSR_REG(CoreDebug_BASE_PTR)
#define DCRDR                                    CoreDebug_base_DCRDR_REG(CoreDebug_BASE_PTR)
#define DEMCR                                    CoreDebug_base_DEMCR_REG(CoreDebug_BASE_PTR)

/*!
 * @}
 */ /* end of group CoreDebug_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group CoreDebug_Peripheral */


/* ----------------------------------------------------------------------------
   -- DWT
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DWT_Peripheral DWT
 * @{
 */

/** DWT - Peripheral register structure */
typedef struct DWT_MemMap {
  uint32_t CTRL;                                   /**< Control Register, offset: 0x0 */
  uint8_t RESERVED_0[24];
  uint32_t PCSR;                                   /**< Program Counter Sample Register, offset: 0x1C */
  struct {                                         /* offset: 0x20, array step: 0x10 */
    uint32_t COMP;                                   /**< Comparator Register 0..Comparator Register 1, array offset: 0x20, array step: 0x10 */
    uint32_t MASK;                                   /**< Mask Register 0..Mask Register 1, array offset: 0x24, array step: 0x10 */
    uint32_t FUNCTION;                               /**< Function Register 0..Function Register 1, array offset: 0x28, array step: 0x10 */
    uint8_t RESERVED_0[4];
  } COMPARATOR[2];
} volatile *DWT_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- DWT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DWT_Register_Accessor_Macros DWT - Register accessor macros
 * @{
 */


/* DWT - Register accessors */
#define DWT_CTRL_REG(base)                       ((base)->CTRL)
#define DWT_PCSR_REG(base)                       ((base)->PCSR)
#define DWT_COMP_REG(base,index)                 ((base)->COMPARATOR[index].COMP)
#define DWT_MASK_REG(base,index)                 ((base)->COMPARATOR[index].MASK)
#define DWT_FUNCTION_REG(base,index)             ((base)->COMPARATOR[index].FUNCTION)

/*!
 * @}
 */ /* end of group DWT_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- DWT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DWT_Register_Masks DWT Register Masks
 * @{
 */


/*!
 * @}
 */ /* end of group DWT_Register_Masks */


/* DWT - Peripheral instance base addresses */
/** Peripheral DWT base pointer */
#define DWT_BASE_PTR                             ((DWT_MemMapPtr)0xE0001000u)
/** Array initializer of DWT peripheral base pointers */
#define DWT_BASE_PTRS                            { DWT_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- DWT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DWT_Register_Accessor_Macros DWT - Register accessor macros
 * @{
 */


/* DWT - Register instance definitions */
/* DWT */
#define DWT_CTRL                                 DWT_CTRL_REG(DWT_BASE_PTR)
#define DWT_PCSR                                 DWT_PCSR_REG(DWT_BASE_PTR)
#define DWT_COMP0                                DWT_COMP_REG(DWT_BASE_PTR,0)
#define DWT_MASK0                                DWT_MASK_REG(DWT_BASE_PTR,0)
#define DWT_FUNCTION0                            DWT_FUNCTION_REG(DWT_BASE_PTR,0)
#define DWT_COMP1                                DWT_COMP_REG(DWT_BASE_PTR,1)
#define DWT_MASK1                                DWT_MASK_REG(DWT_BASE_PTR,1)
#define DWT_FUNCTION1                            DWT_FUNCTION_REG(DWT_BASE_PTR,1)

/* DWT - Register array accessors */
#define DWT_COMP(index)                          DWT_COMP_REG(DWT_BASE_PTR,index)
#define DWT_MASK(index)                          DWT_MASK_REG(DWT_BASE_PTR,index)
#define DWT_FUNCTION(index)                      DWT_FUNCTION_REG(DWT_BASE_PTR,index)

/*!
 * @}
 */ /* end of group DWT_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group DWT_Peripheral */


/* ----------------------------------------------------------------------------
   -- FGPIO
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FGPIO_Peripheral FGPIO
 * @{
 */

/** FGPIO - Peripheral register structure */
typedef struct FGPIO_MemMap {
  uint32_t PDOR;                                   /**< Port Data Output Register, offset: 0x0 */
  uint32_t PSOR;                                   /**< Port Set Output Register, offset: 0x4 */
  uint32_t PCOR;                                   /**< Port Clear Output Register, offset: 0x8 */
  uint32_t PTOR;                                   /**< Port Toggle Output Register, offset: 0xC */
  uint32_t PDIR;                                   /**< Port Data Input Register, offset: 0x10 */
  uint32_t PDDR;                                   /**< Port Data Direction Register, offset: 0x14 */
} volatile *FGPIO_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- FGPIO - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FGPIO_Register_Accessor_Macros FGPIO - Register accessor macros
 * @{
 */


/* FGPIO - Register accessors */
#define FGPIO_PDOR_REG(base)                     ((base)->PDOR)
#define FGPIO_PSOR_REG(base)                     ((base)->PSOR)
#define FGPIO_PCOR_REG(base)                     ((base)->PCOR)
#define FGPIO_PTOR_REG(base)                     ((base)->PTOR)
#define FGPIO_PDIR_REG(base)                     ((base)->PDIR)
#define FGPIO_PDDR_REG(base)                     ((base)->PDDR)

/*!
 * @}
 */ /* end of group FGPIO_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- FGPIO Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FGPIO_Register_Masks FGPIO Register Masks
 * @{
 */

/* PDOR Bit Fields */
#define FGPIO_PDOR_PDO_MASK                      0xFFFFFFFFu
#define FGPIO_PDOR_PDO_SHIFT                     0
#define FGPIO_PDOR_PDO(x)                        (((uint32_t)(((uint32_t)(x))<<FGPIO_PDOR_PDO_SHIFT))&FGPIO_PDOR_PDO_MASK)
/* PSOR Bit Fields */
#define FGPIO_PSOR_PTSO_MASK                     0xFFFFFFFFu
#define FGPIO_PSOR_PTSO_SHIFT                    0
#define FGPIO_PSOR_PTSO(x)                       (((uint32_t)(((uint32_t)(x))<<FGPIO_PSOR_PTSO_SHIFT))&FGPIO_PSOR_PTSO_MASK)
/* PCOR Bit Fields */
#define FGPIO_PCOR_PTCO_MASK                     0xFFFFFFFFu
#define FGPIO_PCOR_PTCO_SHIFT                    0
#define FGPIO_PCOR_PTCO(x)                       (((uint32_t)(((uint32_t)(x))<<FGPIO_PCOR_PTCO_SHIFT))&FGPIO_PCOR_PTCO_MASK)
/* PTOR Bit Fields */
#define FGPIO_PTOR_PTTO_MASK                     0xFFFFFFFFu
#define FGPIO_PTOR_PTTO_SHIFT                    0
#define FGPIO_PTOR_PTTO(x)                       (((uint32_t)(((uint32_t)(x))<<FGPIO_PTOR_PTTO_SHIFT))&FGPIO_PTOR_PTTO_MASK)
/* PDIR Bit Fields */
#define FGPIO_PDIR_PDI_MASK                      0xFFFFFFFFu
#define FGPIO_PDIR_PDI_SHIFT                     0
#define FGPIO_PDIR_PDI(x)                        (((uint32_t)(((uint32_t)(x))<<FGPIO_PDIR_PDI_SHIFT))&FGPIO_PDIR_PDI_MASK)
/* PDDR Bit Fields */
#define FGPIO_PDDR_PDD_MASK                      0xFFFFFFFFu
#define FGPIO_PDDR_PDD_SHIFT                     0
#define FGPIO_PDDR_PDD(x)                        (((uint32_t)(((uint32_t)(x))<<FGPIO_PDDR_PDD_SHIFT))&FGPIO_PDDR_PDD_MASK)

/*!
 * @}
 */ /* end of group FGPIO_Register_Masks */


/* FGPIO - Peripheral instance base addresses */
/** Peripheral FGPIOA base pointer */
#define FGPIOA_BASE_PTR                          ((FGPIO_MemMapPtr)0xF80FF000u)
/** Peripheral FGPIOB base pointer */
#define FGPIOB_BASE_PTR                          ((FGPIO_MemMapPtr)0xF80FF040u)
/** Array initializer of FGPIO peripheral base pointers */
#define FGPIO_BASE_PTRS                          { FGPIOA_BASE_PTR, FGPIOB_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- FGPIO - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FGPIO_Register_Accessor_Macros FGPIO - Register accessor macros
 * @{
 */


/* FGPIO - Register instance definitions */
/* FGPIOA */
#define FGPIOA_PDOR                              FGPIO_PDOR_REG(FGPIOA_BASE_PTR)
#define FGPIOA_PSOR                              FGPIO_PSOR_REG(FGPIOA_BASE_PTR)
#define FGPIOA_PCOR                              FGPIO_PCOR_REG(FGPIOA_BASE_PTR)
#define FGPIOA_PTOR                              FGPIO_PTOR_REG(FGPIOA_BASE_PTR)
#define FGPIOA_PDIR                              FGPIO_PDIR_REG(FGPIOA_BASE_PTR)
#define FGPIOA_PDDR                              FGPIO_PDDR_REG(FGPIOA_BASE_PTR)
/* FGPIOB */
#define FGPIOB_PDOR                              FGPIO_PDOR_REG(FGPIOB_BASE_PTR)
#define FGPIOB_PSOR                              FGPIO_PSOR_REG(FGPIOB_BASE_PTR)
#define FGPIOB_PCOR                              FGPIO_PCOR_REG(FGPIOB_BASE_PTR)
#define FGPIOB_PTOR                              FGPIO_PTOR_REG(FGPIOB_BASE_PTR)
#define FGPIOB_PDIR                              FGPIO_PDIR_REG(FGPIOB_BASE_PTR)
#define FGPIOB_PDDR                              FGPIO_PDDR_REG(FGPIOB_BASE_PTR)

/*!
 * @}
 */ /* end of group FGPIO_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group FGPIO_Peripheral */


/* ----------------------------------------------------------------------------
   -- FTFA
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTFA_Peripheral FTFA
 * @{
 */

/** FTFA - Peripheral register structure */
typedef struct FTFA_MemMap {
  uint8_t FSTAT;                                   /**< Flash Status Register, offset: 0x0 */
  uint8_t FCNFG;                                   /**< Flash Configuration Register, offset: 0x1 */
  uint8_t FSEC;                                    /**< Flash Security Register, offset: 0x2 */
  uint8_t FOPT;                                    /**< Flash Option Register, offset: 0x3 */
  uint8_t FCCOB3;                                  /**< Flash Common Command Object Registers, offset: 0x4 */
  uint8_t FCCOB2;                                  /**< Flash Common Command Object Registers, offset: 0x5 */
  uint8_t FCCOB1;                                  /**< Flash Common Command Object Registers, offset: 0x6 */
  uint8_t FCCOB0;                                  /**< Flash Common Command Object Registers, offset: 0x7 */
  uint8_t FCCOB7;                                  /**< Flash Common Command Object Registers, offset: 0x8 */
  uint8_t FCCOB6;                                  /**< Flash Common Command Object Registers, offset: 0x9 */
  uint8_t FCCOB5;                                  /**< Flash Common Command Object Registers, offset: 0xA */
  uint8_t FCCOB4;                                  /**< Flash Common Command Object Registers, offset: 0xB */
  uint8_t FCCOBB;                                  /**< Flash Common Command Object Registers, offset: 0xC */
  uint8_t FCCOBA;                                  /**< Flash Common Command Object Registers, offset: 0xD */
  uint8_t FCCOB9;                                  /**< Flash Common Command Object Registers, offset: 0xE */
  uint8_t FCCOB8;                                  /**< Flash Common Command Object Registers, offset: 0xF */
  uint8_t FPROT3;                                  /**< Program Flash Protection Registers, offset: 0x10 */
  uint8_t FPROT2;                                  /**< Program Flash Protection Registers, offset: 0x11 */
  uint8_t FPROT1;                                  /**< Program Flash Protection Registers, offset: 0x12 */
  uint8_t FPROT0;                                  /**< Program Flash Protection Registers, offset: 0x13 */
} volatile *FTFA_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- FTFA - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTFA_Register_Accessor_Macros FTFA - Register accessor macros
 * @{
 */


/* FTFA - Register accessors */
#define FTFA_FSTAT_REG(base)                     ((base)->FSTAT)
#define FTFA_FCNFG_REG(base)                     ((base)->FCNFG)
#define FTFA_FSEC_REG(base)                      ((base)->FSEC)
#define FTFA_FOPT_REG(base)                      ((base)->FOPT)
#define FTFA_FCCOB3_REG(base)                    ((base)->FCCOB3)
#define FTFA_FCCOB2_REG(base)                    ((base)->FCCOB2)
#define FTFA_FCCOB1_REG(base)                    ((base)->FCCOB1)
#define FTFA_FCCOB0_REG(base)                    ((base)->FCCOB0)
#define FTFA_FCCOB7_REG(base)                    ((base)->FCCOB7)
#define FTFA_FCCOB6_REG(base)                    ((base)->FCCOB6)
#define FTFA_FCCOB5_REG(base)                    ((base)->FCCOB5)
#define FTFA_FCCOB4_REG(base)                    ((base)->FCCOB4)
#define FTFA_FCCOBB_REG(base)                    ((base)->FCCOBB)
#define FTFA_FCCOBA_REG(base)                    ((base)->FCCOBA)
#define FTFA_FCCOB9_REG(base)                    ((base)->FCCOB9)
#define FTFA_FCCOB8_REG(base)                    ((base)->FCCOB8)
#define FTFA_FPROT3_REG(base)                    ((base)->FPROT3)
#define FTFA_FPROT2_REG(base)                    ((base)->FPROT2)
#define FTFA_FPROT1_REG(base)                    ((base)->FPROT1)
#define FTFA_FPROT0_REG(base)                    ((base)->FPROT0)

/*!
 * @}
 */ /* end of group FTFA_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- FTFA Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTFA_Register_Masks FTFA Register Masks
 * @{
 */

/* FSTAT Bit Fields */
#define FTFA_FSTAT_MGSTAT0_MASK                  0x1u
#define FTFA_FSTAT_MGSTAT0_SHIFT                 0
#define FTFA_FSTAT_FPVIOL_MASK                   0x10u
#define FTFA_FSTAT_FPVIOL_SHIFT                  4
#define FTFA_FSTAT_ACCERR_MASK                   0x20u
#define FTFA_FSTAT_ACCERR_SHIFT                  5
#define FTFA_FSTAT_RDCOLERR_MASK                 0x40u
#define FTFA_FSTAT_RDCOLERR_SHIFT                6
#define FTFA_FSTAT_CCIF_MASK                     0x80u
#define FTFA_FSTAT_CCIF_SHIFT                    7
/* FCNFG Bit Fields */
#define FTFA_FCNFG_ERSSUSP_MASK                  0x10u
#define FTFA_FCNFG_ERSSUSP_SHIFT                 4
#define FTFA_FCNFG_ERSAREQ_MASK                  0x20u
#define FTFA_FCNFG_ERSAREQ_SHIFT                 5
#define FTFA_FCNFG_RDCOLLIE_MASK                 0x40u
#define FTFA_FCNFG_RDCOLLIE_SHIFT                6
#define FTFA_FCNFG_CCIE_MASK                     0x80u
#define FTFA_FCNFG_CCIE_SHIFT                    7
/* FSEC Bit Fields */
#define FTFA_FSEC_SEC_MASK                       0x3u
#define FTFA_FSEC_SEC_SHIFT                      0
#define FTFA_FSEC_SEC(x)                         (((uint8_t)(((uint8_t)(x))<<FTFA_FSEC_SEC_SHIFT))&FTFA_FSEC_SEC_MASK)
#define FTFA_FSEC_FSLACC_MASK                    0xCu
#define FTFA_FSEC_FSLACC_SHIFT                   2
#define FTFA_FSEC_FSLACC(x)                      (((uint8_t)(((uint8_t)(x))<<FTFA_FSEC_FSLACC_SHIFT))&FTFA_FSEC_FSLACC_MASK)
#define FTFA_FSEC_MEEN_MASK                      0x30u
#define FTFA_FSEC_MEEN_SHIFT                     4
#define FTFA_FSEC_MEEN(x)                        (((uint8_t)(((uint8_t)(x))<<FTFA_FSEC_MEEN_SHIFT))&FTFA_FSEC_MEEN_MASK)
#define FTFA_FSEC_KEYEN_MASK                     0xC0u
#define FTFA_FSEC_KEYEN_SHIFT                    6
#define FTFA_FSEC_KEYEN(x)                       (((uint8_t)(((uint8_t)(x))<<FTFA_FSEC_KEYEN_SHIFT))&FTFA_FSEC_KEYEN_MASK)
/* FOPT Bit Fields */
#define FTFA_FOPT_OPT_MASK                       0xFFu
#define FTFA_FOPT_OPT_SHIFT                      0
#define FTFA_FOPT_OPT(x)                         (((uint8_t)(((uint8_t)(x))<<FTFA_FOPT_OPT_SHIFT))&FTFA_FOPT_OPT_MASK)
/* FCCOB3 Bit Fields */
#define FTFA_FCCOB3_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB3_CCOBn_SHIFT                  0
#define FTFA_FCCOB3_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB3_CCOBn_SHIFT))&FTFA_FCCOB3_CCOBn_MASK)
/* FCCOB2 Bit Fields */
#define FTFA_FCCOB2_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB2_CCOBn_SHIFT                  0
#define FTFA_FCCOB2_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB2_CCOBn_SHIFT))&FTFA_FCCOB2_CCOBn_MASK)
/* FCCOB1 Bit Fields */
#define FTFA_FCCOB1_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB1_CCOBn_SHIFT                  0
#define FTFA_FCCOB1_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB1_CCOBn_SHIFT))&FTFA_FCCOB1_CCOBn_MASK)
/* FCCOB0 Bit Fields */
#define FTFA_FCCOB0_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB0_CCOBn_SHIFT                  0
#define FTFA_FCCOB0_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB0_CCOBn_SHIFT))&FTFA_FCCOB0_CCOBn_MASK)
/* FCCOB7 Bit Fields */
#define FTFA_FCCOB7_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB7_CCOBn_SHIFT                  0
#define FTFA_FCCOB7_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB7_CCOBn_SHIFT))&FTFA_FCCOB7_CCOBn_MASK)
/* FCCOB6 Bit Fields */
#define FTFA_FCCOB6_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB6_CCOBn_SHIFT                  0
#define FTFA_FCCOB6_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB6_CCOBn_SHIFT))&FTFA_FCCOB6_CCOBn_MASK)
/* FCCOB5 Bit Fields */
#define FTFA_FCCOB5_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB5_CCOBn_SHIFT                  0
#define FTFA_FCCOB5_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB5_CCOBn_SHIFT))&FTFA_FCCOB5_CCOBn_MASK)
/* FCCOB4 Bit Fields */
#define FTFA_FCCOB4_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB4_CCOBn_SHIFT                  0
#define FTFA_FCCOB4_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB4_CCOBn_SHIFT))&FTFA_FCCOB4_CCOBn_MASK)
/* FCCOBB Bit Fields */
#define FTFA_FCCOBB_CCOBn_MASK                   0xFFu
#define FTFA_FCCOBB_CCOBn_SHIFT                  0
#define FTFA_FCCOBB_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOBB_CCOBn_SHIFT))&FTFA_FCCOBB_CCOBn_MASK)
/* FCCOBA Bit Fields */
#define FTFA_FCCOBA_CCOBn_MASK                   0xFFu
#define FTFA_FCCOBA_CCOBn_SHIFT                  0
#define FTFA_FCCOBA_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOBA_CCOBn_SHIFT))&FTFA_FCCOBA_CCOBn_MASK)
/* FCCOB9 Bit Fields */
#define FTFA_FCCOB9_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB9_CCOBn_SHIFT                  0
#define FTFA_FCCOB9_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB9_CCOBn_SHIFT))&FTFA_FCCOB9_CCOBn_MASK)
/* FCCOB8 Bit Fields */
#define FTFA_FCCOB8_CCOBn_MASK                   0xFFu
#define FTFA_FCCOB8_CCOBn_SHIFT                  0
#define FTFA_FCCOB8_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB8_CCOBn_SHIFT))&FTFA_FCCOB8_CCOBn_MASK)
/* FPROT3 Bit Fields */
#define FTFA_FPROT3_PROT_MASK                    0xFFu
#define FTFA_FPROT3_PROT_SHIFT                   0
#define FTFA_FPROT3_PROT(x)                      (((uint8_t)(((uint8_t)(x))<<FTFA_FPROT3_PROT_SHIFT))&FTFA_FPROT3_PROT_MASK)
/* FPROT2 Bit Fields */
#define FTFA_FPROT2_PROT_MASK                    0xFFu
#define FTFA_FPROT2_PROT_SHIFT                   0
#define FTFA_FPROT2_PROT(x)                      (((uint8_t)(((uint8_t)(x))<<FTFA_FPROT2_PROT_SHIFT))&FTFA_FPROT2_PROT_MASK)
/* FPROT1 Bit Fields */
#define FTFA_FPROT1_PROT_MASK                    0xFFu
#define FTFA_FPROT1_PROT_SHIFT                   0
#define FTFA_FPROT1_PROT(x)                      (((uint8_t)(((uint8_t)(x))<<FTFA_FPROT1_PROT_SHIFT))&FTFA_FPROT1_PROT_MASK)
/* FPROT0 Bit Fields */
#define FTFA_FPROT0_PROT_MASK                    0xFFu
#define FTFA_FPROT0_PROT_SHIFT                   0
#define FTFA_FPROT0_PROT(x)                      (((uint8_t)(((uint8_t)(x))<<FTFA_FPROT0_PROT_SHIFT))&FTFA_FPROT0_PROT_MASK)

/*!
 * @}
 */ /* end of group FTFA_Register_Masks */


/* FTFA - Peripheral instance base addresses */
/** Peripheral FTFA base pointer */
#define FTFA_BASE_PTR                            ((FTFA_MemMapPtr)0x40020000u)
/** Array initializer of FTFA peripheral base pointers */
#define FTFA_BASE_PTRS                           { FTFA_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- FTFA - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTFA_Register_Accessor_Macros FTFA - Register accessor macros
 * @{
 */


/* FTFA - Register instance definitions */
/* FTFA */
#define FTFA_FSTAT                               FTFA_FSTAT_REG(FTFA_BASE_PTR)
#define FTFA_FCNFG                               FTFA_FCNFG_REG(FTFA_BASE_PTR)
#define FTFA_FSEC                                FTFA_FSEC_REG(FTFA_BASE_PTR)
#define FTFA_FOPT                                FTFA_FOPT_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB3                              FTFA_FCCOB3_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB2                              FTFA_FCCOB2_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB1                              FTFA_FCCOB1_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB0                              FTFA_FCCOB0_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB7                              FTFA_FCCOB7_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB6                              FTFA_FCCOB6_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB5                              FTFA_FCCOB5_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB4                              FTFA_FCCOB4_REG(FTFA_BASE_PTR)
#define FTFA_FCCOBB                              FTFA_FCCOBB_REG(FTFA_BASE_PTR)
#define FTFA_FCCOBA                              FTFA_FCCOBA_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB9                              FTFA_FCCOB9_REG(FTFA_BASE_PTR)
#define FTFA_FCCOB8                              FTFA_FCCOB8_REG(FTFA_BASE_PTR)
#define FTFA_FPROT3                              FTFA_FPROT3_REG(FTFA_BASE_PTR)
#define FTFA_FPROT2                              FTFA_FPROT2_REG(FTFA_BASE_PTR)
#define FTFA_FPROT1                              FTFA_FPROT1_REG(FTFA_BASE_PTR)
#define FTFA_FPROT0                              FTFA_FPROT0_REG(FTFA_BASE_PTR)

/*!
 * @}
 */ /* end of group FTFA_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group FTFA_Peripheral */


/* ----------------------------------------------------------------------------
   -- GPIO
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GPIO_Peripheral GPIO
 * @{
 */

/** GPIO - Peripheral register structure */
typedef struct GPIO_MemMap {
  uint32_t PDOR;                                   /**< Port Data Output Register, offset: 0x0 */
  uint32_t PSOR;                                   /**< Port Set Output Register, offset: 0x4 */
  uint32_t PCOR;                                   /**< Port Clear Output Register, offset: 0x8 */
  uint32_t PTOR;                                   /**< Port Toggle Output Register, offset: 0xC */
  uint32_t PDIR;                                   /**< Port Data Input Register, offset: 0x10 */
  uint32_t PDDR;                                   /**< Port Data Direction Register, offset: 0x14 */
} volatile *GPIO_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- GPIO - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GPIO_Register_Accessor_Macros GPIO - Register accessor macros
 * @{
 */


/* GPIO - Register accessors */
#define GPIO_PDOR_REG(base)                      ((base)->PDOR)
#define GPIO_PSOR_REG(base)                      ((base)->PSOR)
#define GPIO_PCOR_REG(base)                      ((base)->PCOR)
#define GPIO_PTOR_REG(base)                      ((base)->PTOR)
#define GPIO_PDIR_REG(base)                      ((base)->PDIR)
#define GPIO_PDDR_REG(base)                      ((base)->PDDR)

/*!
 * @}
 */ /* end of group GPIO_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- GPIO Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GPIO_Register_Masks GPIO Register Masks
 * @{
 */

/* PDOR Bit Fields */
#define GPIO_PDOR_PDO_MASK                       0xFFFFFFFFu
#define GPIO_PDOR_PDO_SHIFT                      0
#define GPIO_PDOR_PDO(x)                         (((uint32_t)(((uint32_t)(x))<<GPIO_PDOR_PDO_SHIFT))&GPIO_PDOR_PDO_MASK)
/* PSOR Bit Fields */
#define GPIO_PSOR_PTSO_MASK                      0xFFFFFFFFu
#define GPIO_PSOR_PTSO_SHIFT                     0
#define GPIO_PSOR_PTSO(x)                        (((uint32_t)(((uint32_t)(x))<<GPIO_PSOR_PTSO_SHIFT))&GPIO_PSOR_PTSO_MASK)
/* PCOR Bit Fields */
#define GPIO_PCOR_PTCO_MASK                      0xFFFFFFFFu
#define GPIO_PCOR_PTCO_SHIFT                     0
#define GPIO_PCOR_PTCO(x)                        (((uint32_t)(((uint32_t)(x))<<GPIO_PCOR_PTCO_SHIFT))&GPIO_PCOR_PTCO_MASK)
/* PTOR Bit Fields */
#define GPIO_PTOR_PTTO_MASK                      0xFFFFFFFFu
#define GPIO_PTOR_PTTO_SHIFT                     0
#define GPIO_PTOR_PTTO(x)                        (((uint32_t)(((uint32_t)(x))<<GPIO_PTOR_PTTO_SHIFT))&GPIO_PTOR_PTTO_MASK)
/* PDIR Bit Fields */
#define GPIO_PDIR_PDI_MASK                       0xFFFFFFFFu
#define GPIO_PDIR_PDI_SHIFT                      0
#define GPIO_PDIR_PDI(x)                         (((uint32_t)(((uint32_t)(x))<<GPIO_PDIR_PDI_SHIFT))&GPIO_PDIR_PDI_MASK)
/* PDDR Bit Fields */
#define GPIO_PDDR_PDD_MASK                       0xFFFFFFFFu
#define GPIO_PDDR_PDD_SHIFT                      0
#define GPIO_PDDR_PDD(x)                         (((uint32_t)(((uint32_t)(x))<<GPIO_PDDR_PDD_SHIFT))&GPIO_PDDR_PDD_MASK)

/*!
 * @}
 */ /* end of group GPIO_Register_Masks */


/* GPIO - Peripheral instance base addresses */
/** Peripheral GPIOA base pointer */
#define GPIOA_BASE_PTR                           ((GPIO_MemMapPtr)0x400FF000u)
/** Peripheral GPIOB base pointer */
#define GPIOB_BASE_PTR                           ((GPIO_MemMapPtr)0x400FF040u)
/** Array initializer of GPIO peripheral base pointers */
#define GPIO_BASE_PTRS                           { GPIOA_BASE_PTR, GPIOB_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- GPIO - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GPIO_Register_Accessor_Macros GPIO - Register accessor macros
 * @{
 */


/* GPIO - Register instance definitions */
/* GPIOA */
#define GPIOA_PDOR                               GPIO_PDOR_REG(GPIOA_BASE_PTR)
#define GPIOA_PSOR                               GPIO_PSOR_REG(GPIOA_BASE_PTR)
#define GPIOA_PCOR                               GPIO_PCOR_REG(GPIOA_BASE_PTR)
#define GPIOA_PTOR                               GPIO_PTOR_REG(GPIOA_BASE_PTR)
#define GPIOA_PDIR                               GPIO_PDIR_REG(GPIOA_BASE_PTR)
#define GPIOA_PDDR                               GPIO_PDDR_REG(GPIOA_BASE_PTR)
/* GPIOB */
#define GPIOB_PDOR                               GPIO_PDOR_REG(GPIOB_BASE_PTR)
#define GPIOB_PSOR                               GPIO_PSOR_REG(GPIOB_BASE_PTR)
#define GPIOB_PCOR                               GPIO_PCOR_REG(GPIOB_BASE_PTR)
#define GPIOB_PTOR                               GPIO_PTOR_REG(GPIOB_BASE_PTR)
#define GPIOB_PDIR                               GPIO_PDIR_REG(GPIOB_BASE_PTR)
#define GPIOB_PDDR                               GPIO_PDDR_REG(GPIOB_BASE_PTR)

/*!
 * @}
 */ /* end of group GPIO_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group GPIO_Peripheral */


/* ----------------------------------------------------------------------------
   -- I2C
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup I2C_Peripheral I2C
 * @{
 */

/** I2C - Peripheral register structure */
typedef struct I2C_MemMap {
  uint8_t A1;                                      /**< I2C Address Register 1, offset: 0x0 */
  uint8_t F;                                       /**< I2C Frequency Divider register, offset: 0x1 */
  uint8_t C1;                                      /**< I2C Control Register 1, offset: 0x2 */
  uint8_t S;                                       /**< I2C Status register, offset: 0x3 */
  uint8_t D;                                       /**< I2C Data I/O register, offset: 0x4 */
  uint8_t C2;                                      /**< I2C Control Register 2, offset: 0x5 */
  uint8_t FLT;                                     /**< I2C Programmable Input Glitch Filter register, offset: 0x6 */
  uint8_t RA;                                      /**< I2C Range Address register, offset: 0x7 */
} volatile *I2C_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- I2C - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup I2C_Register_Accessor_Macros I2C - Register accessor macros
 * @{
 */


/* I2C - Register accessors */
#define I2C_A1_REG(base)                         ((base)->A1)
#define I2C_F_REG(base)                          ((base)->F)
#define I2C_C1_REG(base)                         ((base)->C1)
#define I2C_S_REG(base)                          ((base)->S)
#define I2C_D_REG(base)                          ((base)->D)
#define I2C_C2_REG(base)                         ((base)->C2)
#define I2C_FLT_REG(base)                        ((base)->FLT)
#define I2C_RA_REG(base)                         ((base)->RA)

/*!
 * @}
 */ /* end of group I2C_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- I2C Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup I2C_Register_Masks I2C Register Masks
 * @{
 */

/* A1 Bit Fields */
#define I2C_A1_AD_MASK                           0xFEu
#define I2C_A1_AD_SHIFT                          1
#define I2C_A1_AD(x)                             (((uint8_t)(((uint8_t)(x))<<I2C_A1_AD_SHIFT))&I2C_A1_AD_MASK)
/* F Bit Fields */
#define I2C_F_ICR_MASK                           0x3Fu
#define I2C_F_ICR_SHIFT                          0
#define I2C_F_ICR(x)                             (((uint8_t)(((uint8_t)(x))<<I2C_F_ICR_SHIFT))&I2C_F_ICR_MASK)
#define I2C_F_MULT_MASK                          0xC0u
#define I2C_F_MULT_SHIFT                         6
#define I2C_F_MULT(x)                            (((uint8_t)(((uint8_t)(x))<<I2C_F_MULT_SHIFT))&I2C_F_MULT_MASK)
/* C1 Bit Fields */
#define I2C_C1_WUEN_MASK                         0x2u
#define I2C_C1_WUEN_SHIFT                        1
#define I2C_C1_RSTA_MASK                         0x4u
#define I2C_C1_RSTA_SHIFT                        2
#define I2C_C1_TXAK_MASK                         0x8u
#define I2C_C1_TXAK_SHIFT                        3
#define I2C_C1_TX_MASK                           0x10u
#define I2C_C1_TX_SHIFT                          4
#define I2C_C1_MST_MASK                          0x20u
#define I2C_C1_MST_SHIFT                         5
#define I2C_C1_IICIE_MASK                        0x40u
#define I2C_C1_IICIE_SHIFT                       6
#define I2C_C1_IICEN_MASK                        0x80u
#define I2C_C1_IICEN_SHIFT                       7
/* S Bit Fields */
#define I2C_S_RXAK_MASK                          0x1u
#define I2C_S_RXAK_SHIFT                         0
#define I2C_S_IICIF_MASK                         0x2u
#define I2C_S_IICIF_SHIFT                        1
#define I2C_S_SRW_MASK                           0x4u
#define I2C_S_SRW_SHIFT                          2
#define I2C_S_RAM_MASK                           0x8u
#define I2C_S_RAM_SHIFT                          3
#define I2C_S_ARBL_MASK                          0x10u
#define I2C_S_ARBL_SHIFT                         4
#define I2C_S_BUSY_MASK                          0x20u
#define I2C_S_BUSY_SHIFT                         5
#define I2C_S_IAAS_MASK                          0x40u
#define I2C_S_IAAS_SHIFT                         6
#define I2C_S_TCF_MASK                           0x80u
#define I2C_S_TCF_SHIFT                          7
/* D Bit Fields */
#define I2C_D_DATA_MASK                          0xFFu
#define I2C_D_DATA_SHIFT                         0
#define I2C_D_DATA(x)                            (((uint8_t)(((uint8_t)(x))<<I2C_D_DATA_SHIFT))&I2C_D_DATA_MASK)
/* C2 Bit Fields */
#define I2C_C2_AD_MASK                           0x7u
#define I2C_C2_AD_SHIFT                          0
#define I2C_C2_AD(x)                             (((uint8_t)(((uint8_t)(x))<<I2C_C2_AD_SHIFT))&I2C_C2_AD_MASK)
#define I2C_C2_RMEN_MASK                         0x8u
#define I2C_C2_RMEN_SHIFT                        3
#define I2C_C2_SBRC_MASK                         0x10u
#define I2C_C2_SBRC_SHIFT                        4
#define I2C_C2_HDRS_MASK                         0x20u
#define I2C_C2_HDRS_SHIFT                        5
#define I2C_C2_ADEXT_MASK                        0x40u
#define I2C_C2_ADEXT_SHIFT                       6
#define I2C_C2_GCAEN_MASK                        0x80u
#define I2C_C2_GCAEN_SHIFT                       7
/* FLT Bit Fields */
#define I2C_FLT_FLT_MASK                         0xFu
#define I2C_FLT_FLT_SHIFT                        0
#define I2C_FLT_FLT(x)                           (((uint8_t)(((uint8_t)(x))<<I2C_FLT_FLT_SHIFT))&I2C_FLT_FLT_MASK)
#define I2C_FLT_STARTF_MASK                      0x10u
#define I2C_FLT_STARTF_SHIFT                     4
#define I2C_FLT_SSIE_MASK                        0x20u
#define I2C_FLT_SSIE_SHIFT                       5
#define I2C_FLT_STOPF_MASK                       0x40u
#define I2C_FLT_STOPF_SHIFT                      6
#define I2C_FLT_SHEN_MASK                        0x80u
#define I2C_FLT_SHEN_SHIFT                       7
/* RA Bit Fields */
#define I2C_RA_RAD_MASK                          0xFEu
#define I2C_RA_RAD_SHIFT                         1
#define I2C_RA_RAD(x)                            (((uint8_t)(((uint8_t)(x))<<I2C_RA_RAD_SHIFT))&I2C_RA_RAD_MASK)

/*!
 * @}
 */ /* end of group I2C_Register_Masks */


/* I2C - Peripheral instance base addresses */
/** Peripheral I2C0 base pointer */
#define I2C0_BASE_PTR                            ((I2C_MemMapPtr)0x40066000u)
/** Peripheral I2C1 base pointer */
#define I2C1_BASE_PTR                            ((I2C_MemMapPtr)0x40067000u)
/** Array initializer of I2C peripheral base pointers */
#define I2C_BASE_PTRS                            { I2C0_BASE_PTR, I2C1_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- I2C - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup I2C_Register_Accessor_Macros I2C - Register accessor macros
 * @{
 */


/* I2C - Register instance definitions */
/* I2C0 */
#define I2C0_A1                                  I2C_A1_REG(I2C0_BASE_PTR)
#define I2C0_F                                   I2C_F_REG(I2C0_BASE_PTR)
#define I2C0_C1                                  I2C_C1_REG(I2C0_BASE_PTR)
#define I2C0_S                                   I2C_S_REG(I2C0_BASE_PTR)
#define I2C0_D                                   I2C_D_REG(I2C0_BASE_PTR)
#define I2C0_C2                                  I2C_C2_REG(I2C0_BASE_PTR)
#define I2C0_FLT                                 I2C_FLT_REG(I2C0_BASE_PTR)
#define I2C0_RA                                  I2C_RA_REG(I2C0_BASE_PTR)
/* I2C1 */
#define I2C1_A1                                  I2C_A1_REG(I2C1_BASE_PTR)
#define I2C1_F                                   I2C_F_REG(I2C1_BASE_PTR)
#define I2C1_C1                                  I2C_C1_REG(I2C1_BASE_PTR)
#define I2C1_S                                   I2C_S_REG(I2C1_BASE_PTR)
#define I2C1_D                                   I2C_D_REG(I2C1_BASE_PTR)
#define I2C1_C2                                  I2C_C2_REG(I2C1_BASE_PTR)
#define I2C1_FLT                                 I2C_FLT_REG(I2C1_BASE_PTR)
#define I2C1_RA                                  I2C_RA_REG(I2C1_BASE_PTR)

/*!
 * @}
 */ /* end of group I2C_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group I2C_Peripheral */


/* ----------------------------------------------------------------------------
   -- LPTMR
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LPTMR_Peripheral LPTMR
 * @{
 */

/** LPTMR - Peripheral register structure */
typedef struct LPTMR_MemMap {
  uint32_t CSR;                                    /**< Low Power Timer Control Status Register, offset: 0x0 */
  uint32_t PSR;                                    /**< Low Power Timer Prescale Register, offset: 0x4 */
  uint32_t CMR;                                    /**< Low Power Timer Compare Register, offset: 0x8 */
  uint32_t CNR;                                    /**< Low Power Timer Counter Register, offset: 0xC */
} volatile *LPTMR_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- LPTMR - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LPTMR_Register_Accessor_Macros LPTMR - Register accessor macros
 * @{
 */


/* LPTMR - Register accessors */
#define LPTMR_CSR_REG(base)                      ((base)->CSR)
#define LPTMR_PSR_REG(base)                      ((base)->PSR)
#define LPTMR_CMR_REG(base)                      ((base)->CMR)
#define LPTMR_CNR_REG(base)                      ((base)->CNR)

/*!
 * @}
 */ /* end of group LPTMR_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- LPTMR Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LPTMR_Register_Masks LPTMR Register Masks
 * @{
 */

/* CSR Bit Fields */
#define LPTMR_CSR_TEN_MASK                       0x1u
#define LPTMR_CSR_TEN_SHIFT                      0
#define LPTMR_CSR_TMS_MASK                       0x2u
#define LPTMR_CSR_TMS_SHIFT                      1
#define LPTMR_CSR_TFC_MASK                       0x4u
#define LPTMR_CSR_TFC_SHIFT                      2
#define LPTMR_CSR_TPP_MASK                       0x8u
#define LPTMR_CSR_TPP_SHIFT                      3
#define LPTMR_CSR_TPS_MASK                       0x30u
#define LPTMR_CSR_TPS_SHIFT                      4
#define LPTMR_CSR_TPS(x)                         (((uint32_t)(((uint32_t)(x))<<LPTMR_CSR_TPS_SHIFT))&LPTMR_CSR_TPS_MASK)
#define LPTMR_CSR_TIE_MASK                       0x40u
#define LPTMR_CSR_TIE_SHIFT                      6
#define LPTMR_CSR_TCF_MASK                       0x80u
#define LPTMR_CSR_TCF_SHIFT                      7
/* PSR Bit Fields */
#define LPTMR_PSR_PCS_MASK                       0x3u
#define LPTMR_PSR_PCS_SHIFT                      0
#define LPTMR_PSR_PCS(x)                         (((uint32_t)(((uint32_t)(x))<<LPTMR_PSR_PCS_SHIFT))&LPTMR_PSR_PCS_MASK)
#define LPTMR_PSR_PBYP_MASK                      0x4u
#define LPTMR_PSR_PBYP_SHIFT                     2
#define LPTMR_PSR_PRESCALE_MASK                  0x78u
#define LPTMR_PSR_PRESCALE_SHIFT                 3
#define LPTMR_PSR_PRESCALE(x)                    (((uint32_t)(((uint32_t)(x))<<LPTMR_PSR_PRESCALE_SHIFT))&LPTMR_PSR_PRESCALE_MASK)
/* CMR Bit Fields */
#define LPTMR_CMR_COMPARE_MASK                   0xFFFFu
#define LPTMR_CMR_COMPARE_SHIFT                  0
#define LPTMR_CMR_COMPARE(x)                     (((uint32_t)(((uint32_t)(x))<<LPTMR_CMR_COMPARE_SHIFT))&LPTMR_CMR_COMPARE_MASK)
/* CNR Bit Fields */
#define LPTMR_CNR_COUNTER_MASK                   0xFFFFu
#define LPTMR_CNR_COUNTER_SHIFT                  0
#define LPTMR_CNR_COUNTER(x)                     (((uint32_t)(((uint32_t)(x))<<LPTMR_CNR_COUNTER_SHIFT))&LPTMR_CNR_COUNTER_MASK)

/*!
 * @}
 */ /* end of group LPTMR_Register_Masks */


/* LPTMR - Peripheral instance base addresses */
/** Peripheral LPTMR0 base pointer */
#define LPTMR0_BASE_PTR                          ((LPTMR_MemMapPtr)0x40040000u)
/** Array initializer of LPTMR peripheral base pointers */
#define LPTMR_BASE_PTRS                          { LPTMR0_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- LPTMR - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LPTMR_Register_Accessor_Macros LPTMR - Register accessor macros
 * @{
 */


/* LPTMR - Register instance definitions */
/* LPTMR0 */
#define LPTMR0_CSR                               LPTMR_CSR_REG(LPTMR0_BASE_PTR)
#define LPTMR0_PSR                               LPTMR_PSR_REG(LPTMR0_BASE_PTR)
#define LPTMR0_CMR                               LPTMR_CMR_REG(LPTMR0_BASE_PTR)
#define LPTMR0_CNR                               LPTMR_CNR_REG(LPTMR0_BASE_PTR)

/*!
 * @}
 */ /* end of group LPTMR_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group LPTMR_Peripheral */


/* ----------------------------------------------------------------------------
   -- MCG
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCG_Peripheral MCG
 * @{
 */

/** MCG - Peripheral register structure */
typedef struct MCG_MemMap {
  uint8_t C1;                                      /**< MCG Control 1 Register, offset: 0x0 */
  uint8_t C2;                                      /**< MCG Control 2 Register, offset: 0x1 */
  uint8_t C3;                                      /**< MCG Control 3 Register, offset: 0x2 */
  uint8_t C4;                                      /**< MCG Control 4 Register, offset: 0x3 */
  uint8_t RESERVED_0[1];
  uint8_t C6;                                      /**< MCG Control 6 Register, offset: 0x5 */
  uint8_t S;                                       /**< MCG Status Register, offset: 0x6 */
  uint8_t RESERVED_1[1];
  uint8_t SC;                                      /**< MCG Status and Control Register, offset: 0x8 */
  uint8_t RESERVED_2[1];
  uint8_t ATCVH;                                   /**< MCG Auto Trim Compare Value High Register, offset: 0xA */
  uint8_t ATCVL;                                   /**< MCG Auto Trim Compare Value Low Register, offset: 0xB */
} volatile *MCG_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- MCG - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCG_Register_Accessor_Macros MCG - Register accessor macros
 * @{
 */


/* MCG - Register accessors */
#define MCG_C1_REG(base)                         ((base)->C1)
#define MCG_C2_REG(base)                         ((base)->C2)
#define MCG_C3_REG(base)                         ((base)->C3)
#define MCG_C4_REG(base)                         ((base)->C4)
#define MCG_C6_REG(base)                         ((base)->C6)
#define MCG_S_REG(base)                          ((base)->S)
#define MCG_SC_REG(base)                         ((base)->SC)
#define MCG_ATCVH_REG(base)                      ((base)->ATCVH)
#define MCG_ATCVL_REG(base)                      ((base)->ATCVL)

/*!
 * @}
 */ /* end of group MCG_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- MCG Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCG_Register_Masks MCG Register Masks
 * @{
 */

/* C1 Bit Fields */
#define MCG_C1_IREFSTEN_MASK                     0x1u
#define MCG_C1_IREFSTEN_SHIFT                    0
#define MCG_C1_IRCLKEN_MASK                      0x2u
#define MCG_C1_IRCLKEN_SHIFT                     1
#define MCG_C1_IREFS_MASK                        0x4u
#define MCG_C1_IREFS_SHIFT                       2
#define MCG_C1_FRDIV_MASK                        0x38u
#define MCG_C1_FRDIV_SHIFT                       3
#define MCG_C1_FRDIV(x)                          (((uint8_t)(((uint8_t)(x))<<MCG_C1_FRDIV_SHIFT))&MCG_C1_FRDIV_MASK)
#define MCG_C1_CLKS_MASK                         0xC0u
#define MCG_C1_CLKS_SHIFT                        6
#define MCG_C1_CLKS(x)                           (((uint8_t)(((uint8_t)(x))<<MCG_C1_CLKS_SHIFT))&MCG_C1_CLKS_MASK)
/* C2 Bit Fields */
#define MCG_C2_IRCS_MASK                         0x1u
#define MCG_C2_IRCS_SHIFT                        0
#define MCG_C2_LP_MASK                           0x2u
#define MCG_C2_LP_SHIFT                          1
#define MCG_C2_EREFS0_MASK                       0x4u
#define MCG_C2_EREFS0_SHIFT                      2
#define MCG_C2_HGO0_MASK                         0x8u
#define MCG_C2_HGO0_SHIFT                        3
#define MCG_C2_RANGE0_MASK                       0x30u
#define MCG_C2_RANGE0_SHIFT                      4
#define MCG_C2_RANGE0(x)                         (((uint8_t)(((uint8_t)(x))<<MCG_C2_RANGE0_SHIFT))&MCG_C2_RANGE0_MASK)
#define MCG_C2_FCFTRIM_MASK                      0x40u
#define MCG_C2_FCFTRIM_SHIFT                     6
#define MCG_C2_LOCRE0_MASK                       0x80u
#define MCG_C2_LOCRE0_SHIFT                      7
/* C3 Bit Fields */
#define MCG_C3_SCTRIM_MASK                       0xFFu
#define MCG_C3_SCTRIM_SHIFT                      0
#define MCG_C3_SCTRIM(x)                         (((uint8_t)(((uint8_t)(x))<<MCG_C3_SCTRIM_SHIFT))&MCG_C3_SCTRIM_MASK)
/* C4 Bit Fields */
#define MCG_C4_SCFTRIM_MASK                      0x1u
#define MCG_C4_SCFTRIM_SHIFT                     0
#define MCG_C4_FCTRIM_MASK                       0x1Eu
#define MCG_C4_FCTRIM_SHIFT                      1
#define MCG_C4_FCTRIM(x)                         (((uint8_t)(((uint8_t)(x))<<MCG_C4_FCTRIM_SHIFT))&MCG_C4_FCTRIM_MASK)
#define MCG_C4_DRST_DRS_MASK                     0x60u
#define MCG_C4_DRST_DRS_SHIFT                    5
#define MCG_C4_DRST_DRS(x)                       (((uint8_t)(((uint8_t)(x))<<MCG_C4_DRST_DRS_SHIFT))&MCG_C4_DRST_DRS_MASK)
#define MCG_C4_DMX32_MASK                        0x80u
#define MCG_C4_DMX32_SHIFT                       7
/* C6 Bit Fields */
#define MCG_C6_CME_MASK                          0x20u
#define MCG_C6_CME_SHIFT                         5
/* S Bit Fields */
#define MCG_S_IRCST_MASK                         0x1u
#define MCG_S_IRCST_SHIFT                        0
#define MCG_S_OSCINIT0_MASK                      0x2u
#define MCG_S_OSCINIT0_SHIFT                     1
#define MCG_S_CLKST_MASK                         0xCu
#define MCG_S_CLKST_SHIFT                        2
#define MCG_S_CLKST(x)                           (((uint8_t)(((uint8_t)(x))<<MCG_S_CLKST_SHIFT))&MCG_S_CLKST_MASK)
#define MCG_S_IREFST_MASK                        0x10u
#define MCG_S_IREFST_SHIFT                       4
/* SC Bit Fields */
#define MCG_SC_LOCS0_MASK                        0x1u
#define MCG_SC_LOCS0_SHIFT                       0
#define MCG_SC_FCRDIV_MASK                       0xEu
#define MCG_SC_FCRDIV_SHIFT                      1
#define MCG_SC_FCRDIV(x)                         (((uint8_t)(((uint8_t)(x))<<MCG_SC_FCRDIV_SHIFT))&MCG_SC_FCRDIV_MASK)
#define MCG_SC_FLTPRSRV_MASK                     0x10u
#define MCG_SC_FLTPRSRV_SHIFT                    4
#define MCG_SC_ATMF_MASK                         0x20u
#define MCG_SC_ATMF_SHIFT                        5
#define MCG_SC_ATMS_MASK                         0x40u
#define MCG_SC_ATMS_SHIFT                        6
#define MCG_SC_ATME_MASK                         0x80u
#define MCG_SC_ATME_SHIFT                        7
/* ATCVH Bit Fields */
#define MCG_ATCVH_ATCVH_MASK                     0xFFu
#define MCG_ATCVH_ATCVH_SHIFT                    0
#define MCG_ATCVH_ATCVH(x)                       (((uint8_t)(((uint8_t)(x))<<MCG_ATCVH_ATCVH_SHIFT))&MCG_ATCVH_ATCVH_MASK)
/* ATCVL Bit Fields */
#define MCG_ATCVL_ATCVL_MASK                     0xFFu
#define MCG_ATCVL_ATCVL_SHIFT                    0
#define MCG_ATCVL_ATCVL(x)                       (((uint8_t)(((uint8_t)(x))<<MCG_ATCVL_ATCVL_SHIFT))&MCG_ATCVL_ATCVL_MASK)

/*!
 * @}
 */ /* end of group MCG_Register_Masks */


/* MCG - Peripheral instance base addresses */
/** Peripheral MCG base pointer */
#define MCG_BASE_PTR                             ((MCG_MemMapPtr)0x40064000u)
/** Array initializer of MCG peripheral base pointers */
#define MCG_BASE_PTRS                            { MCG_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- MCG - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCG_Register_Accessor_Macros MCG - Register accessor macros
 * @{
 */


/* MCG - Register instance definitions */
/* MCG */
#define MCG_C1                                   MCG_C1_REG(MCG_BASE_PTR)
#define MCG_C2                                   MCG_C2_REG(MCG_BASE_PTR)
#define MCG_C3                                   MCG_C3_REG(MCG_BASE_PTR)
#define MCG_C4                                   MCG_C4_REG(MCG_BASE_PTR)
#define MCG_C6                                   MCG_C6_REG(MCG_BASE_PTR)
#define MCG_S                                    MCG_S_REG(MCG_BASE_PTR)
#define MCG_SC                                   MCG_SC_REG(MCG_BASE_PTR)
#define MCG_ATCVH                                MCG_ATCVH_REG(MCG_BASE_PTR)
#define MCG_ATCVL                                MCG_ATCVL_REG(MCG_BASE_PTR)

/*!
 * @}
 */ /* end of group MCG_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group MCG_Peripheral */


/* ----------------------------------------------------------------------------
   -- MCM
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCM_Peripheral MCM
 * @{
 */

/** MCM - Peripheral register structure */
typedef struct MCM_MemMap {
  uint8_t RESERVED_0[8];
  uint16_t PLASC;                                  /**< Crossbar Switch (AXBS) Slave Configuration, offset: 0x8 */
  uint16_t PLAMC;                                  /**< Crossbar Switch (AXBS) Master Configuration, offset: 0xA */
  uint32_t PLACR;                                  /**< Platform Control Register, offset: 0xC */
  uint8_t RESERVED_1[48];
  uint32_t CPO;                                    /**< Compute Operation Control Register, offset: 0x40 */
} volatile *MCM_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- MCM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCM_Register_Accessor_Macros MCM - Register accessor macros
 * @{
 */


/* MCM - Register accessors */
#define MCM_PLASC_REG(base)                      ((base)->PLASC)
#define MCM_PLAMC_REG(base)                      ((base)->PLAMC)
#define MCM_PLACR_REG(base)                      ((base)->PLACR)
#define MCM_CPO_REG(base)                        ((base)->CPO)

/*!
 * @}
 */ /* end of group MCM_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- MCM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCM_Register_Masks MCM Register Masks
 * @{
 */

/* PLASC Bit Fields */
#define MCM_PLASC_ASC_MASK                       0xFFu
#define MCM_PLASC_ASC_SHIFT                      0
#define MCM_PLASC_ASC(x)                         (((uint16_t)(((uint16_t)(x))<<MCM_PLASC_ASC_SHIFT))&MCM_PLASC_ASC_MASK)
/* PLAMC Bit Fields */
#define MCM_PLAMC_AMC_MASK                       0xFFu
#define MCM_PLAMC_AMC_SHIFT                      0
#define MCM_PLAMC_AMC(x)                         (((uint16_t)(((uint16_t)(x))<<MCM_PLAMC_AMC_SHIFT))&MCM_PLAMC_AMC_MASK)
/* PLACR Bit Fields */
#define MCM_PLACR_ARB_MASK                       0x200u
#define MCM_PLACR_ARB_SHIFT                      9
#define MCM_PLACR_CFCC_MASK                      0x400u
#define MCM_PLACR_CFCC_SHIFT                     10
#define MCM_PLACR_DFCDA_MASK                     0x800u
#define MCM_PLACR_DFCDA_SHIFT                    11
#define MCM_PLACR_DFCIC_MASK                     0x1000u
#define MCM_PLACR_DFCIC_SHIFT                    12
#define MCM_PLACR_DFCC_MASK                      0x2000u
#define MCM_PLACR_DFCC_SHIFT                     13
#define MCM_PLACR_EFDS_MASK                      0x4000u
#define MCM_PLACR_EFDS_SHIFT                     14
#define MCM_PLACR_DFCS_MASK                      0x8000u
#define MCM_PLACR_DFCS_SHIFT                     15
#define MCM_PLACR_ESFC_MASK                      0x10000u
#define MCM_PLACR_ESFC_SHIFT                     16
/* CPO Bit Fields */
#define MCM_CPO_CPOREQ_MASK                      0x1u
#define MCM_CPO_CPOREQ_SHIFT                     0
#define MCM_CPO_CPOACK_MASK                      0x2u
#define MCM_CPO_CPOACK_SHIFT                     1
#define MCM_CPO_CPOWOI_MASK                      0x4u
#define MCM_CPO_CPOWOI_SHIFT                     2

/*!
 * @}
 */ /* end of group MCM_Register_Masks */


/* MCM - Peripheral instance base addresses */
/** Peripheral MCM base pointer */
#define MCM_BASE_PTR                             ((MCM_MemMapPtr)0xF0003000u)
/** Array initializer of MCM peripheral base pointers */
#define MCM_BASE_PTRS                            { MCM_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- MCM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCM_Register_Accessor_Macros MCM - Register accessor macros
 * @{
 */


/* MCM - Register instance definitions */
/* MCM */
#define MCM_PLASC                                MCM_PLASC_REG(MCM_BASE_PTR)
#define MCM_PLAMC                                MCM_PLAMC_REG(MCM_BASE_PTR)
#define MCM_PLACR                                MCM_PLACR_REG(MCM_BASE_PTR)
#define MCM_CPO                                  MCM_CPO_REG(MCM_BASE_PTR)

/*!
 * @}
 */ /* end of group MCM_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group MCM_Peripheral */


/* ----------------------------------------------------------------------------
   -- MTB
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTB_Peripheral MTB
 * @{
 */

/** MTB - Peripheral register structure */
typedef struct MTB_MemMap {
  uint32_t POSITION;                               /**< MTB Position Register, offset: 0x0 */
  uint32_t MASTER;                                 /**< MTB Master Register, offset: 0x4 */
  uint32_t FLOW;                                   /**< MTB Flow Register, offset: 0x8 */
  uint32_t BASE;                                   /**< MTB Base Register, offset: 0xC */
  uint8_t RESERVED_0[3824];
  uint32_t MODECTRL;                               /**< Integration Mode Control Register, offset: 0xF00 */
  uint8_t RESERVED_1[156];
  uint32_t TAGSET;                                 /**< Claim TAG Set Register, offset: 0xFA0 */
  uint32_t TAGCLEAR;                               /**< Claim TAG Clear Register, offset: 0xFA4 */
  uint8_t RESERVED_2[8];
  uint32_t LOCKACCESS;                             /**< Lock Access Register, offset: 0xFB0 */
  uint32_t LOCKSTAT;                               /**< Lock Status Register, offset: 0xFB4 */
  uint32_t AUTHSTAT;                               /**< Authentication Status Register, offset: 0xFB8 */
  uint32_t DEVICEARCH;                             /**< Device Architecture Register, offset: 0xFBC */
  uint8_t RESERVED_3[8];
  uint32_t DEVICECFG;                              /**< Device Configuration Register, offset: 0xFC8 */
  uint32_t DEVICETYPID;                            /**< Device Type Identifier Register, offset: 0xFCC */
  uint32_t PERIPHID[8];                            /**< Peripheral ID Register, array offset: 0xFD0, array step: 0x4 */
  uint32_t COMPID[4];                              /**< Component ID Register, array offset: 0xFF0, array step: 0x4 */
} volatile *MTB_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- MTB - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTB_Register_Accessor_Macros MTB - Register accessor macros
 * @{
 */


/* MTB - Register accessors */
#define MTB_POSITION_REG(base)                   ((base)->POSITION)
#define MTB_MASTER_REG(base)                     ((base)->MASTER)
#define MTB_FLOW_REG(base)                       ((base)->FLOW)
#define MTB_BASE_REG(base)                       ((base)->BASE)
#define MTB_MODECTRL_REG(base)                   ((base)->MODECTRL)
#define MTB_TAGSET_REG(base)                     ((base)->TAGSET)
#define MTB_TAGCLEAR_REG(base)                   ((base)->TAGCLEAR)
#define MTB_LOCKACCESS_REG(base)                 ((base)->LOCKACCESS)
#define MTB_LOCKSTAT_REG(base)                   ((base)->LOCKSTAT)
#define MTB_AUTHSTAT_REG(base)                   ((base)->AUTHSTAT)
#define MTB_DEVICEARCH_REG(base)                 ((base)->DEVICEARCH)
#define MTB_DEVICECFG_REG(base)                  ((base)->DEVICECFG)
#define MTB_DEVICETYPID_REG(base)                ((base)->DEVICETYPID)
#define MTB_PERIPHID_REG(base,index)             ((base)->PERIPHID[index])
#define MTB_COMPID_REG(base,index)               ((base)->COMPID[index])

/*!
 * @}
 */ /* end of group MTB_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- MTB Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTB_Register_Masks MTB Register Masks
 * @{
 */

/* POSITION Bit Fields */
#define MTB_POSITION_WRAP_MASK                   0x4u
#define MTB_POSITION_WRAP_SHIFT                  2
#define MTB_POSITION_POINTER_MASK                0xFFFFFFF8u
#define MTB_POSITION_POINTER_SHIFT               3
#define MTB_POSITION_POINTER(x)                  (((uint32_t)(((uint32_t)(x))<<MTB_POSITION_POINTER_SHIFT))&MTB_POSITION_POINTER_MASK)
/* MASTER Bit Fields */
#define MTB_MASTER_MASK_MASK                     0x1Fu
#define MTB_MASTER_MASK_SHIFT                    0
#define MTB_MASTER_MASK(x)                       (((uint32_t)(((uint32_t)(x))<<MTB_MASTER_MASK_SHIFT))&MTB_MASTER_MASK_MASK)
#define MTB_MASTER_TSTARTEN_MASK                 0x20u
#define MTB_MASTER_TSTARTEN_SHIFT                5
#define MTB_MASTER_TSTOPEN_MASK                  0x40u
#define MTB_MASTER_TSTOPEN_SHIFT                 6
#define MTB_MASTER_SFRWPRIV_MASK                 0x80u
#define MTB_MASTER_SFRWPRIV_SHIFT                7
#define MTB_MASTER_RAMPRIV_MASK                  0x100u
#define MTB_MASTER_RAMPRIV_SHIFT                 8
#define MTB_MASTER_HALTREQ_MASK                  0x200u
#define MTB_MASTER_HALTREQ_SHIFT                 9
#define MTB_MASTER_EN_MASK                       0x80000000u
#define MTB_MASTER_EN_SHIFT                      31
/* FLOW Bit Fields */
#define MTB_FLOW_AUTOSTOP_MASK                   0x1u
#define MTB_FLOW_AUTOSTOP_SHIFT                  0
#define MTB_FLOW_AUTOHALT_MASK                   0x2u
#define MTB_FLOW_AUTOHALT_SHIFT                  1
#define MTB_FLOW_WATERMARK_MASK                  0xFFFFFFF8u
#define MTB_FLOW_WATERMARK_SHIFT                 3
#define MTB_FLOW_WATERMARK(x)                    (((uint32_t)(((uint32_t)(x))<<MTB_FLOW_WATERMARK_SHIFT))&MTB_FLOW_WATERMARK_MASK)
/* BASE Bit Fields */
#define MTB_BASE_BASEADDR_MASK                   0xFFFFFFFFu
#define MTB_BASE_BASEADDR_SHIFT                  0
#define MTB_BASE_BASEADDR(x)                     (((uint32_t)(((uint32_t)(x))<<MTB_BASE_BASEADDR_SHIFT))&MTB_BASE_BASEADDR_MASK)
/* MODECTRL Bit Fields */
#define MTB_MODECTRL_MODECTRL_MASK               0xFFFFFFFFu
#define MTB_MODECTRL_MODECTRL_SHIFT              0
#define MTB_MODECTRL_MODECTRL(x)                 (((uint32_t)(((uint32_t)(x))<<MTB_MODECTRL_MODECTRL_SHIFT))&MTB_MODECTRL_MODECTRL_MASK)
/* TAGSET Bit Fields */
#define MTB_TAGSET_TAGSET_MASK                   0xFFFFFFFFu
#define MTB_TAGSET_TAGSET_SHIFT                  0
#define MTB_TAGSET_TAGSET(x)                     (((uint32_t)(((uint32_t)(x))<<MTB_TAGSET_TAGSET_SHIFT))&MTB_TAGSET_TAGSET_MASK)
/* TAGCLEAR Bit Fields */
#define MTB_TAGCLEAR_TAGCLEAR_MASK               0xFFFFFFFFu
#define MTB_TAGCLEAR_TAGCLEAR_SHIFT              0
#define MTB_TAGCLEAR_TAGCLEAR(x)                 (((uint32_t)(((uint32_t)(x))<<MTB_TAGCLEAR_TAGCLEAR_SHIFT))&MTB_TAGCLEAR_TAGCLEAR_MASK)
/* LOCKACCESS Bit Fields */
#define MTB_LOCKACCESS_LOCKACCESS_MASK           0xFFFFFFFFu
#define MTB_LOCKACCESS_LOCKACCESS_SHIFT          0
#define MTB_LOCKACCESS_LOCKACCESS(x)             (((uint32_t)(((uint32_t)(x))<<MTB_LOCKACCESS_LOCKACCESS_SHIFT))&MTB_LOCKACCESS_LOCKACCESS_MASK)
/* LOCKSTAT Bit Fields */
#define MTB_LOCKSTAT_LOCKSTAT_MASK               0xFFFFFFFFu
#define MTB_LOCKSTAT_LOCKSTAT_SHIFT              0
#define MTB_LOCKSTAT_LOCKSTAT(x)                 (((uint32_t)(((uint32_t)(x))<<MTB_LOCKSTAT_LOCKSTAT_SHIFT))&MTB_LOCKSTAT_LOCKSTAT_MASK)
/* AUTHSTAT Bit Fields */
#define MTB_AUTHSTAT_BIT0_MASK                   0x1u
#define MTB_AUTHSTAT_BIT0_SHIFT                  0
#define MTB_AUTHSTAT_BIT1_MASK                   0x2u
#define MTB_AUTHSTAT_BIT1_SHIFT                  1
#define MTB_AUTHSTAT_BIT2_MASK                   0x4u
#define MTB_AUTHSTAT_BIT2_SHIFT                  2
#define MTB_AUTHSTAT_BIT3_MASK                   0x8u
#define MTB_AUTHSTAT_BIT3_SHIFT                  3
/* DEVICEARCH Bit Fields */
#define MTB_DEVICEARCH_DEVICEARCH_MASK           0xFFFFFFFFu
#define MTB_DEVICEARCH_DEVICEARCH_SHIFT          0
#define MTB_DEVICEARCH_DEVICEARCH(x)             (((uint32_t)(((uint32_t)(x))<<MTB_DEVICEARCH_DEVICEARCH_SHIFT))&MTB_DEVICEARCH_DEVICEARCH_MASK)
/* DEVICECFG Bit Fields */
#define MTB_DEVICECFG_DEVICECFG_MASK             0xFFFFFFFFu
#define MTB_DEVICECFG_DEVICECFG_SHIFT            0
#define MTB_DEVICECFG_DEVICECFG(x)               (((uint32_t)(((uint32_t)(x))<<MTB_DEVICECFG_DEVICECFG_SHIFT))&MTB_DEVICECFG_DEVICECFG_MASK)
/* DEVICETYPID Bit Fields */
#define MTB_DEVICETYPID_DEVICETYPID_MASK         0xFFFFFFFFu
#define MTB_DEVICETYPID_DEVICETYPID_SHIFT        0
#define MTB_DEVICETYPID_DEVICETYPID(x)           (((uint32_t)(((uint32_t)(x))<<MTB_DEVICETYPID_DEVICETYPID_SHIFT))&MTB_DEVICETYPID_DEVICETYPID_MASK)
/* PERIPHID Bit Fields */
#define MTB_PERIPHID_PERIPHID_MASK               0xFFFFFFFFu
#define MTB_PERIPHID_PERIPHID_SHIFT              0
#define MTB_PERIPHID_PERIPHID(x)                 (((uint32_t)(((uint32_t)(x))<<MTB_PERIPHID_PERIPHID_SHIFT))&MTB_PERIPHID_PERIPHID_MASK)
/* COMPID Bit Fields */
#define MTB_COMPID_COMPID_MASK                   0xFFFFFFFFu
#define MTB_COMPID_COMPID_SHIFT                  0
#define MTB_COMPID_COMPID(x)                     (((uint32_t)(((uint32_t)(x))<<MTB_COMPID_COMPID_SHIFT))&MTB_COMPID_COMPID_MASK)

/*!
 * @}
 */ /* end of group MTB_Register_Masks */


/* MTB - Peripheral instance base addresses */
/** Peripheral MTB base pointer */
#define MTB_BASE_PTR                             ((MTB_MemMapPtr)0xF0000000u)
/** Array initializer of MTB peripheral base pointers */
#define MTB_BASE_PTRS                            { MTB_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- MTB - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTB_Register_Accessor_Macros MTB - Register accessor macros
 * @{
 */


/* MTB - Register instance definitions */
/* MTB */
#define MTB_POSITION                             MTB_POSITION_REG(MTB_BASE_PTR)
#define MTB_MASTER                               MTB_MASTER_REG(MTB_BASE_PTR)
#define MTB_FLOW                                 MTB_FLOW_REG(MTB_BASE_PTR)
#define MTB_BASE                                 MTB_BASE_REG(MTB_BASE_PTR)
#define MTB_MODECTRL                             MTB_MODECTRL_REG(MTB_BASE_PTR)
#define MTB_TAGSET                               MTB_TAGSET_REG(MTB_BASE_PTR)
#define MTB_TAGCLEAR                             MTB_TAGCLEAR_REG(MTB_BASE_PTR)
#define MTB_LOCKACCESS                           MTB_LOCKACCESS_REG(MTB_BASE_PTR)
#define MTB_LOCKSTAT                             MTB_LOCKSTAT_REG(MTB_BASE_PTR)
#define MTB_AUTHSTAT                             MTB_AUTHSTAT_REG(MTB_BASE_PTR)
#define MTB_DEVICEARCH                           MTB_DEVICEARCH_REG(MTB_BASE_PTR)
#define MTB_DEVICECFG                            MTB_DEVICECFG_REG(MTB_BASE_PTR)
#define MTB_DEVICETYPID                          MTB_DEVICETYPID_REG(MTB_BASE_PTR)
#define MTB_PERIPHID4                            MTB_PERIPHID_REG(MTB_BASE_PTR,0)
#define MTB_PERIPHID5                            MTB_PERIPHID_REG(MTB_BASE_PTR,1)
#define MTB_PERIPHID6                            MTB_PERIPHID_REG(MTB_BASE_PTR,2)
#define MTB_PERIPHID7                            MTB_PERIPHID_REG(MTB_BASE_PTR,3)
#define MTB_PERIPHID0                            MTB_PERIPHID_REG(MTB_BASE_PTR,4)
#define MTB_PERIPHID1                            MTB_PERIPHID_REG(MTB_BASE_PTR,5)
#define MTB_PERIPHID2                            MTB_PERIPHID_REG(MTB_BASE_PTR,6)
#define MTB_PERIPHID3                            MTB_PERIPHID_REG(MTB_BASE_PTR,7)
#define MTB_COMPID0                              MTB_COMPID_REG(MTB_BASE_PTR,0)
#define MTB_COMPID1                              MTB_COMPID_REG(MTB_BASE_PTR,1)
#define MTB_COMPID2                              MTB_COMPID_REG(MTB_BASE_PTR,2)
#define MTB_COMPID3                              MTB_COMPID_REG(MTB_BASE_PTR,3)

/* MTB - Register array accessors */
#define MTB_PERIPHID(index)                      MTB_PERIPHID_REG(MTB_BASE_PTR,index)
#define MTB_COMPID(index)                        MTB_COMPID_REG(MTB_BASE_PTR,index)

/*!
 * @}
 */ /* end of group MTB_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group MTB_Peripheral */


/* ----------------------------------------------------------------------------
   -- MTBDWT
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTBDWT_Peripheral MTBDWT
 * @{
 */

/** MTBDWT - Peripheral register structure */
typedef struct MTBDWT_MemMap {
  uint32_t CTRL;                                   /**< MTB DWT Control Register, offset: 0x0 */
  uint8_t RESERVED_0[28];
  struct {                                         /* offset: 0x20, array step: 0x10 */
    uint32_t COMP;                                   /**< MTB_DWT Comparator Register, array offset: 0x20, array step: 0x10 */
    uint32_t MASK;                                   /**< MTB_DWT Comparator Mask Register, array offset: 0x24, array step: 0x10 */
    uint32_t FCT;                                    /**< MTB_DWT Comparator Function Register 0..MTB_DWT Comparator Function Register 1, array offset: 0x28, array step: 0x10 */
    uint8_t RESERVED_0[4];
  } COMPARATOR[2];
  uint8_t RESERVED_1[448];
  uint32_t TBCTRL;                                 /**< MTB_DWT Trace Buffer Control Register, offset: 0x200 */
  uint8_t RESERVED_2[3524];
  uint32_t DEVICECFG;                              /**< Device Configuration Register, offset: 0xFC8 */
  uint32_t DEVICETYPID;                            /**< Device Type Identifier Register, offset: 0xFCC */
  uint32_t PERIPHID[8];                            /**< Peripheral ID Register, array offset: 0xFD0, array step: 0x4 */
  uint32_t COMPID[4];                              /**< Component ID Register, array offset: 0xFF0, array step: 0x4 */
} volatile *MTBDWT_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- MTBDWT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTBDWT_Register_Accessor_Macros MTBDWT - Register accessor macros
 * @{
 */


/* MTBDWT - Register accessors */
#define MTBDWT_CTRL_REG(base)                    ((base)->CTRL)
#define MTBDWT_COMP_REG(base,index)              ((base)->COMPARATOR[index].COMP)
#define MTBDWT_MASK_REG(base,index)              ((base)->COMPARATOR[index].MASK)
#define MTBDWT_FCT_REG(base,index)               ((base)->COMPARATOR[index].FCT)
#define MTBDWT_TBCTRL_REG(base)                  ((base)->TBCTRL)
#define MTBDWT_DEVICECFG_REG(base)               ((base)->DEVICECFG)
#define MTBDWT_DEVICETYPID_REG(base)             ((base)->DEVICETYPID)
#define MTBDWT_PERIPHID_REG(base,index)          ((base)->PERIPHID[index])
#define MTBDWT_COMPID_REG(base,index)            ((base)->COMPID[index])

/*!
 * @}
 */ /* end of group MTBDWT_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- MTBDWT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTBDWT_Register_Masks MTBDWT Register Masks
 * @{
 */

/* CTRL Bit Fields */
#define MTBDWT_CTRL_DWTCFGCTRL_MASK              0xFFFFFFFu
#define MTBDWT_CTRL_DWTCFGCTRL_SHIFT             0
#define MTBDWT_CTRL_DWTCFGCTRL(x)                (((uint32_t)(((uint32_t)(x))<<MTBDWT_CTRL_DWTCFGCTRL_SHIFT))&MTBDWT_CTRL_DWTCFGCTRL_MASK)
#define MTBDWT_CTRL_NUMCMP_MASK                  0xF0000000u
#define MTBDWT_CTRL_NUMCMP_SHIFT                 28
#define MTBDWT_CTRL_NUMCMP(x)                    (((uint32_t)(((uint32_t)(x))<<MTBDWT_CTRL_NUMCMP_SHIFT))&MTBDWT_CTRL_NUMCMP_MASK)
/* COMP Bit Fields */
#define MTBDWT_COMP_COMP_MASK                    0xFFFFFFFFu
#define MTBDWT_COMP_COMP_SHIFT                   0
#define MTBDWT_COMP_COMP(x)                      (((uint32_t)(((uint32_t)(x))<<MTBDWT_COMP_COMP_SHIFT))&MTBDWT_COMP_COMP_MASK)
/* MASK Bit Fields */
#define MTBDWT_MASK_MASK_MASK                    0x1Fu
#define MTBDWT_MASK_MASK_SHIFT                   0
#define MTBDWT_MASK_MASK(x)                      (((uint32_t)(((uint32_t)(x))<<MTBDWT_MASK_MASK_SHIFT))&MTBDWT_MASK_MASK_MASK)
/* FCT Bit Fields */
#define MTBDWT_FCT_FUNCTION_MASK                 0xFu
#define MTBDWT_FCT_FUNCTION_SHIFT                0
#define MTBDWT_FCT_FUNCTION(x)                   (((uint32_t)(((uint32_t)(x))<<MTBDWT_FCT_FUNCTION_SHIFT))&MTBDWT_FCT_FUNCTION_MASK)
#define MTBDWT_FCT_DATAVMATCH_MASK               0x100u
#define MTBDWT_FCT_DATAVMATCH_SHIFT              8
#define MTBDWT_FCT_DATAVSIZE_MASK                0xC00u
#define MTBDWT_FCT_DATAVSIZE_SHIFT               10
#define MTBDWT_FCT_DATAVSIZE(x)                  (((uint32_t)(((uint32_t)(x))<<MTBDWT_FCT_DATAVSIZE_SHIFT))&MTBDWT_FCT_DATAVSIZE_MASK)
#define MTBDWT_FCT_DATAVADDR0_MASK               0xF000u
#define MTBDWT_FCT_DATAVADDR0_SHIFT              12
#define MTBDWT_FCT_DATAVADDR0(x)                 (((uint32_t)(((uint32_t)(x))<<MTBDWT_FCT_DATAVADDR0_SHIFT))&MTBDWT_FCT_DATAVADDR0_MASK)
#define MTBDWT_FCT_MATCHED_MASK                  0x1000000u
#define MTBDWT_FCT_MATCHED_SHIFT                 24
/* TBCTRL Bit Fields */
#define MTBDWT_TBCTRL_ACOMP0_MASK                0x1u
#define MTBDWT_TBCTRL_ACOMP0_SHIFT               0
#define MTBDWT_TBCTRL_ACOMP1_MASK                0x2u
#define MTBDWT_TBCTRL_ACOMP1_SHIFT               1
#define MTBDWT_TBCTRL_NUMCOMP_MASK               0xF0000000u
#define MTBDWT_TBCTRL_NUMCOMP_SHIFT              28
#define MTBDWT_TBCTRL_NUMCOMP(x)                 (((uint32_t)(((uint32_t)(x))<<MTBDWT_TBCTRL_NUMCOMP_SHIFT))&MTBDWT_TBCTRL_NUMCOMP_MASK)
/* DEVICECFG Bit Fields */
#define MTBDWT_DEVICECFG_DEVICECFG_MASK          0xFFFFFFFFu
#define MTBDWT_DEVICECFG_DEVICECFG_SHIFT         0
#define MTBDWT_DEVICECFG_DEVICECFG(x)            (((uint32_t)(((uint32_t)(x))<<MTBDWT_DEVICECFG_DEVICECFG_SHIFT))&MTBDWT_DEVICECFG_DEVICECFG_MASK)
/* DEVICETYPID Bit Fields */
#define MTBDWT_DEVICETYPID_DEVICETYPID_MASK      0xFFFFFFFFu
#define MTBDWT_DEVICETYPID_DEVICETYPID_SHIFT     0
#define MTBDWT_DEVICETYPID_DEVICETYPID(x)        (((uint32_t)(((uint32_t)(x))<<MTBDWT_DEVICETYPID_DEVICETYPID_SHIFT))&MTBDWT_DEVICETYPID_DEVICETYPID_MASK)
/* PERIPHID Bit Fields */
#define MTBDWT_PERIPHID_PERIPHID_MASK            0xFFFFFFFFu
#define MTBDWT_PERIPHID_PERIPHID_SHIFT           0
#define MTBDWT_PERIPHID_PERIPHID(x)              (((uint32_t)(((uint32_t)(x))<<MTBDWT_PERIPHID_PERIPHID_SHIFT))&MTBDWT_PERIPHID_PERIPHID_MASK)
/* COMPID Bit Fields */
#define MTBDWT_COMPID_COMPID_MASK                0xFFFFFFFFu
#define MTBDWT_COMPID_COMPID_SHIFT               0
#define MTBDWT_COMPID_COMPID(x)                  (((uint32_t)(((uint32_t)(x))<<MTBDWT_COMPID_COMPID_SHIFT))&MTBDWT_COMPID_COMPID_MASK)

/*!
 * @}
 */ /* end of group MTBDWT_Register_Masks */


/* MTBDWT - Peripheral instance base addresses */
/** Peripheral MTBDWT base pointer */
#define MTBDWT_BASE_PTR                          ((MTBDWT_MemMapPtr)0xF0001000u)
/** Array initializer of MTBDWT peripheral base pointers */
#define MTBDWT_BASE_PTRS                         { MTBDWT_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- MTBDWT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTBDWT_Register_Accessor_Macros MTBDWT - Register accessor macros
 * @{
 */


/* MTBDWT - Register instance definitions */
/* MTBDWT */
#define MTBDWT_CTRL                              MTBDWT_CTRL_REG(MTBDWT_BASE_PTR)
#define MTBDWT_COMP0                             MTBDWT_COMP_REG(MTBDWT_BASE_PTR,0)
#define MTBDWT_MASK0                             MTBDWT_MASK_REG(MTBDWT_BASE_PTR,0)
#define MTBDWT_FCT0                              MTBDWT_FCT_REG(MTBDWT_BASE_PTR,0)
#define MTBDWT_COMP1                             MTBDWT_COMP_REG(MTBDWT_BASE_PTR,1)
#define MTBDWT_MASK1                             MTBDWT_MASK_REG(MTBDWT_BASE_PTR,1)
#define MTBDWT_FCT1                              MTBDWT_FCT_REG(MTBDWT_BASE_PTR,1)
#define MTBDWT_TBCTRL                            MTBDWT_TBCTRL_REG(MTBDWT_BASE_PTR)
#define MTBDWT_DEVICECFG                         MTBDWT_DEVICECFG_REG(MTBDWT_BASE_PTR)
#define MTBDWT_DEVICETYPID                       MTBDWT_DEVICETYPID_REG(MTBDWT_BASE_PTR)
#define MTBDWT_PERIPHID4                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,0)
#define MTBDWT_PERIPHID5                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,1)
#define MTBDWT_PERIPHID6                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,2)
#define MTBDWT_PERIPHID7                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,3)
#define MTBDWT_PERIPHID0                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,4)
#define MTBDWT_PERIPHID1                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,5)
#define MTBDWT_PERIPHID2                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,6)
#define MTBDWT_PERIPHID3                         MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,7)
#define MTBDWT_COMPID0                           MTBDWT_COMPID_REG(MTBDWT_BASE_PTR,0)
#define MTBDWT_COMPID1                           MTBDWT_COMPID_REG(MTBDWT_BASE_PTR,1)
#define MTBDWT_COMPID2                           MTBDWT_COMPID_REG(MTBDWT_BASE_PTR,2)
#define MTBDWT_COMPID3                           MTBDWT_COMPID_REG(MTBDWT_BASE_PTR,3)

/* MTBDWT - Register array accessors */
#define MTBDWT_COMP(index)                       MTBDWT_COMP_REG(MTBDWT_BASE_PTR,index)
#define MTBDWT_MASK(index)                       MTBDWT_MASK_REG(MTBDWT_BASE_PTR,index)
#define MTBDWT_FCT(index)                        MTBDWT_FCT_REG(MTBDWT_BASE_PTR,index)
#define MTBDWT_PERIPHID(index)                   MTBDWT_PERIPHID_REG(MTBDWT_BASE_PTR,index)
#define MTBDWT_COMPID(index)                     MTBDWT_COMPID_REG(MTBDWT_BASE_PTR,index)

/*!
 * @}
 */ /* end of group MTBDWT_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group MTBDWT_Peripheral */


/* ----------------------------------------------------------------------------
   -- NV
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NV_Peripheral NV
 * @{
 */

/** NV - Peripheral register structure */
typedef struct NV_MemMap {
  uint8_t BACKKEY3;                                /**< Backdoor Comparison Key 3., offset: 0x0 */
  uint8_t BACKKEY2;                                /**< Backdoor Comparison Key 2., offset: 0x1 */
  uint8_t BACKKEY1;                                /**< Backdoor Comparison Key 1., offset: 0x2 */
  uint8_t BACKKEY0;                                /**< Backdoor Comparison Key 0., offset: 0x3 */
  uint8_t BACKKEY7;                                /**< Backdoor Comparison Key 7., offset: 0x4 */
  uint8_t BACKKEY6;                                /**< Backdoor Comparison Key 6., offset: 0x5 */
  uint8_t BACKKEY5;                                /**< Backdoor Comparison Key 5., offset: 0x6 */
  uint8_t BACKKEY4;                                /**< Backdoor Comparison Key 4., offset: 0x7 */
  uint8_t FPROT3;                                  /**< Non-volatile P-Flash Protection 1 - Low Register, offset: 0x8 */
  uint8_t FPROT2;                                  /**< Non-volatile P-Flash Protection 1 - High Register, offset: 0x9 */
  uint8_t FPROT1;                                  /**< Non-volatile P-Flash Protection 0 - Low Register, offset: 0xA */
  uint8_t FPROT0;                                  /**< Non-volatile P-Flash Protection 0 - High Register, offset: 0xB */
  uint8_t FSEC;                                    /**< Non-volatile Flash Security Register, offset: 0xC */
  uint8_t FOPT;                                    /**< Non-volatile Flash Option Register, offset: 0xD */
} volatile *NV_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- NV - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NV_Register_Accessor_Macros NV - Register accessor macros
 * @{
 */


/* NV - Register accessors */
#define NV_BACKKEY3_REG(base)                    ((base)->BACKKEY3)
#define NV_BACKKEY2_REG(base)                    ((base)->BACKKEY2)
#define NV_BACKKEY1_REG(base)                    ((base)->BACKKEY1)
#define NV_BACKKEY0_REG(base)                    ((base)->BACKKEY0)
#define NV_BACKKEY7_REG(base)                    ((base)->BACKKEY7)
#define NV_BACKKEY6_REG(base)                    ((base)->BACKKEY6)
#define NV_BACKKEY5_REG(base)                    ((base)->BACKKEY5)
#define NV_BACKKEY4_REG(base)                    ((base)->BACKKEY4)
#define NV_FPROT3_REG(base)                      ((base)->FPROT3)
#define NV_FPROT2_REG(base)                      ((base)->FPROT2)
#define NV_FPROT1_REG(base)                      ((base)->FPROT1)
#define NV_FPROT0_REG(base)                      ((base)->FPROT0)
#define NV_FSEC_REG(base)                        ((base)->FSEC)
#define NV_FOPT_REG(base)                        ((base)->FOPT)

/*!
 * @}
 */ /* end of group NV_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- NV Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NV_Register_Masks NV Register Masks
 * @{
 */

/* BACKKEY3 Bit Fields */
#define NV_BACKKEY3_KEY_MASK                     0xFFu
#define NV_BACKKEY3_KEY_SHIFT                    0
#define NV_BACKKEY3_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY3_KEY_SHIFT))&NV_BACKKEY3_KEY_MASK)
/* BACKKEY2 Bit Fields */
#define NV_BACKKEY2_KEY_MASK                     0xFFu
#define NV_BACKKEY2_KEY_SHIFT                    0
#define NV_BACKKEY2_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY2_KEY_SHIFT))&NV_BACKKEY2_KEY_MASK)
/* BACKKEY1 Bit Fields */
#define NV_BACKKEY1_KEY_MASK                     0xFFu
#define NV_BACKKEY1_KEY_SHIFT                    0
#define NV_BACKKEY1_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY1_KEY_SHIFT))&NV_BACKKEY1_KEY_MASK)
/* BACKKEY0 Bit Fields */
#define NV_BACKKEY0_KEY_MASK                     0xFFu
#define NV_BACKKEY0_KEY_SHIFT                    0
#define NV_BACKKEY0_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY0_KEY_SHIFT))&NV_BACKKEY0_KEY_MASK)
/* BACKKEY7 Bit Fields */
#define NV_BACKKEY7_KEY_MASK                     0xFFu
#define NV_BACKKEY7_KEY_SHIFT                    0
#define NV_BACKKEY7_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY7_KEY_SHIFT))&NV_BACKKEY7_KEY_MASK)
/* BACKKEY6 Bit Fields */
#define NV_BACKKEY6_KEY_MASK                     0xFFu
#define NV_BACKKEY6_KEY_SHIFT                    0
#define NV_BACKKEY6_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY6_KEY_SHIFT))&NV_BACKKEY6_KEY_MASK)
/* BACKKEY5 Bit Fields */
#define NV_BACKKEY5_KEY_MASK                     0xFFu
#define NV_BACKKEY5_KEY_SHIFT                    0
#define NV_BACKKEY5_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY5_KEY_SHIFT))&NV_BACKKEY5_KEY_MASK)
/* BACKKEY4 Bit Fields */
#define NV_BACKKEY4_KEY_MASK                     0xFFu
#define NV_BACKKEY4_KEY_SHIFT                    0
#define NV_BACKKEY4_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY4_KEY_SHIFT))&NV_BACKKEY4_KEY_MASK)
/* FPROT3 Bit Fields */
#define NV_FPROT3_PROT_MASK                      0xFFu
#define NV_FPROT3_PROT_SHIFT                     0
#define NV_FPROT3_PROT(x)                        (((uint8_t)(((uint8_t)(x))<<NV_FPROT3_PROT_SHIFT))&NV_FPROT3_PROT_MASK)
/* FPROT2 Bit Fields */
#define NV_FPROT2_PROT_MASK                      0xFFu
#define NV_FPROT2_PROT_SHIFT                     0
#define NV_FPROT2_PROT(x)                        (((uint8_t)(((uint8_t)(x))<<NV_FPROT2_PROT_SHIFT))&NV_FPROT2_PROT_MASK)
/* FPROT1 Bit Fields */
#define NV_FPROT1_PROT_MASK                      0xFFu
#define NV_FPROT1_PROT_SHIFT                     0
#define NV_FPROT1_PROT(x)                        (((uint8_t)(((uint8_t)(x))<<NV_FPROT1_PROT_SHIFT))&NV_FPROT1_PROT_MASK)
/* FPROT0 Bit Fields */
#define NV_FPROT0_PROT_MASK                      0xFFu
#define NV_FPROT0_PROT_SHIFT                     0
#define NV_FPROT0_PROT(x)                        (((uint8_t)(((uint8_t)(x))<<NV_FPROT0_PROT_SHIFT))&NV_FPROT0_PROT_MASK)
/* FSEC Bit Fields */
#define NV_FSEC_SEC_MASK                         0x3u
#define NV_FSEC_SEC_SHIFT                        0
#define NV_FSEC_SEC(x)                           (((uint8_t)(((uint8_t)(x))<<NV_FSEC_SEC_SHIFT))&NV_FSEC_SEC_MASK)
#define NV_FSEC_FSLACC_MASK                      0xCu
#define NV_FSEC_FSLACC_SHIFT                     2
#define NV_FSEC_FSLACC(x)                        (((uint8_t)(((uint8_t)(x))<<NV_FSEC_FSLACC_SHIFT))&NV_FSEC_FSLACC_MASK)
#define NV_FSEC_MEEN_MASK                        0x30u
#define NV_FSEC_MEEN_SHIFT                       4
#define NV_FSEC_MEEN(x)                          (((uint8_t)(((uint8_t)(x))<<NV_FSEC_MEEN_SHIFT))&NV_FSEC_MEEN_MASK)
#define NV_FSEC_KEYEN_MASK                       0xC0u
#define NV_FSEC_KEYEN_SHIFT                      6
#define NV_FSEC_KEYEN(x)                         (((uint8_t)(((uint8_t)(x))<<NV_FSEC_KEYEN_SHIFT))&NV_FSEC_KEYEN_MASK)
/* FOPT Bit Fields */
#define NV_FOPT_LPBOOT0_MASK                     0x1u
#define NV_FOPT_LPBOOT0_SHIFT                    0
#define NV_FOPT_NMI_DIS_MASK                     0x4u
#define NV_FOPT_NMI_DIS_SHIFT                    2
#define NV_FOPT_RESET_PIN_CFG_MASK               0x8u
#define NV_FOPT_RESET_PIN_CFG_SHIFT              3
#define NV_FOPT_LPBOOT1_MASK                     0x10u
#define NV_FOPT_LPBOOT1_SHIFT                    4
#define NV_FOPT_FAST_INIT_MASK                   0x20u
#define NV_FOPT_FAST_INIT_SHIFT                  5

/*!
 * @}
 */ /* end of group NV_Register_Masks */


/* NV - Peripheral instance base addresses */
/** Peripheral FTFA_FlashConfig base pointer */
#define FTFA_FlashConfig_BASE_PTR                ((NV_MemMapPtr)0x400u)
/** Array initializer of NV peripheral base pointers */
#define NV_BASE_PTRS                             { FTFA_FlashConfig_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- NV - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NV_Register_Accessor_Macros NV - Register accessor macros
 * @{
 */


/* NV - Register instance definitions */
/* FTFA_FlashConfig */
#define NV_BACKKEY3                              NV_BACKKEY3_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY2                              NV_BACKKEY2_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY1                              NV_BACKKEY1_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY0                              NV_BACKKEY0_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY7                              NV_BACKKEY7_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY6                              NV_BACKKEY6_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY5                              NV_BACKKEY5_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_BACKKEY4                              NV_BACKKEY4_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_FPROT3                                NV_FPROT3_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_FPROT2                                NV_FPROT2_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_FPROT1                                NV_FPROT1_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_FPROT0                                NV_FPROT0_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_FSEC                                  NV_FSEC_REG(FTFA_FlashConfig_BASE_PTR)
#define NV_FOPT                                  NV_FOPT_REG(FTFA_FlashConfig_BASE_PTR)

/*!
 * @}
 */ /* end of group NV_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group NV_Peripheral */


/* ----------------------------------------------------------------------------
   -- NVIC
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NVIC_Peripheral NVIC
 * @{
 */

/** NVIC - Peripheral register structure */
typedef struct NVIC_MemMap {
  uint32_t ISER;                                   /**< Interrupt Set Enable Register, offset: 0x0 */
  uint8_t RESERVED_0[124];
  uint32_t ICER;                                   /**< Interrupt Clear Enable Register, offset: 0x80 */
  uint8_t RESERVED_1[124];
  uint32_t ISPR;                                   /**< Interrupt Set Pending Register, offset: 0x100 */
  uint8_t RESERVED_2[124];
  uint32_t ICPR;                                   /**< Interrupt Clear Pending Register, offset: 0x180 */
  uint8_t RESERVED_3[380];
  uint32_t IP[8];                                  /**< Interrupt Priority Register 0..Interrupt Priority Register 7, array offset: 0x300, array step: 0x4 */
} volatile *NVIC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- NVIC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NVIC_Register_Accessor_Macros NVIC - Register accessor macros
 * @{
 */


/* NVIC - Register accessors */
#define NVIC_ISER_REG(base)                      ((base)->ISER)
#define NVIC_ICER_REG(base)                      ((base)->ICER)
#define NVIC_ISPR_REG(base)                      ((base)->ISPR)
#define NVIC_ICPR_REG(base)                      ((base)->ICPR)
#define NVIC_IP_REG(base,index)                  ((base)->IP[index])

/*!
 * @}
 */ /* end of group NVIC_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- NVIC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NVIC_Register_Masks NVIC Register Masks
 * @{
 */

/* ISER Bit Fields */
#define NVIC_ISER_SETENA0_MASK                   0x1u
#define NVIC_ISER_SETENA0_SHIFT                  0
#define NVIC_ISER_SETENA1_MASK                   0x2u
#define NVIC_ISER_SETENA1_SHIFT                  1
#define NVIC_ISER_SETENA2_MASK                   0x4u
#define NVIC_ISER_SETENA2_SHIFT                  2
#define NVIC_ISER_SETENA3_MASK                   0x8u
#define NVIC_ISER_SETENA3_SHIFT                  3
#define NVIC_ISER_SETENA4_MASK                   0x10u
#define NVIC_ISER_SETENA4_SHIFT                  4
#define NVIC_ISER_SETENA5_MASK                   0x20u
#define NVIC_ISER_SETENA5_SHIFT                  5
#define NVIC_ISER_SETENA6_MASK                   0x40u
#define NVIC_ISER_SETENA6_SHIFT                  6
#define NVIC_ISER_SETENA7_MASK                   0x80u
#define NVIC_ISER_SETENA7_SHIFT                  7
#define NVIC_ISER_SETENA8_MASK                   0x100u
#define NVIC_ISER_SETENA8_SHIFT                  8
#define NVIC_ISER_SETENA9_MASK                   0x200u
#define NVIC_ISER_SETENA9_SHIFT                  9
#define NVIC_ISER_SETENA10_MASK                  0x400u
#define NVIC_ISER_SETENA10_SHIFT                 10
#define NVIC_ISER_SETENA11_MASK                  0x800u
#define NVIC_ISER_SETENA11_SHIFT                 11
#define NVIC_ISER_SETENA12_MASK                  0x1000u
#define NVIC_ISER_SETENA12_SHIFT                 12
#define NVIC_ISER_SETENA13_MASK                  0x2000u
#define NVIC_ISER_SETENA13_SHIFT                 13
#define NVIC_ISER_SETENA14_MASK                  0x4000u
#define NVIC_ISER_SETENA14_SHIFT                 14
#define NVIC_ISER_SETENA15_MASK                  0x8000u
#define NVIC_ISER_SETENA15_SHIFT                 15
#define NVIC_ISER_SETENA16_MASK                  0x10000u
#define NVIC_ISER_SETENA16_SHIFT                 16
#define NVIC_ISER_SETENA17_MASK                  0x20000u
#define NVIC_ISER_SETENA17_SHIFT                 17
#define NVIC_ISER_SETENA18_MASK                  0x40000u
#define NVIC_ISER_SETENA18_SHIFT                 18
#define NVIC_ISER_SETENA19_MASK                  0x80000u
#define NVIC_ISER_SETENA19_SHIFT                 19
#define NVIC_ISER_SETENA20_MASK                  0x100000u
#define NVIC_ISER_SETENA20_SHIFT                 20
#define NVIC_ISER_SETENA21_MASK                  0x200000u
#define NVIC_ISER_SETENA21_SHIFT                 21
#define NVIC_ISER_SETENA22_MASK                  0x400000u
#define NVIC_ISER_SETENA22_SHIFT                 22
#define NVIC_ISER_SETENA23_MASK                  0x800000u
#define NVIC_ISER_SETENA23_SHIFT                 23
#define NVIC_ISER_SETENA24_MASK                  0x1000000u
#define NVIC_ISER_SETENA24_SHIFT                 24
#define NVIC_ISER_SETENA25_MASK                  0x2000000u
#define NVIC_ISER_SETENA25_SHIFT                 25
#define NVIC_ISER_SETENA26_MASK                  0x4000000u
#define NVIC_ISER_SETENA26_SHIFT                 26
#define NVIC_ISER_SETENA27_MASK                  0x8000000u
#define NVIC_ISER_SETENA27_SHIFT                 27
#define NVIC_ISER_SETENA28_MASK                  0x10000000u
#define NVIC_ISER_SETENA28_SHIFT                 28
#define NVIC_ISER_SETENA29_MASK                  0x20000000u
#define NVIC_ISER_SETENA29_SHIFT                 29
#define NVIC_ISER_SETENA30_MASK                  0x40000000u
#define NVIC_ISER_SETENA30_SHIFT                 30
#define NVIC_ISER_SETENA31_MASK                  0x80000000u
#define NVIC_ISER_SETENA31_SHIFT                 31
/* ICER Bit Fields */
#define NVIC_ICER_CLRENA0_MASK                   0x1u
#define NVIC_ICER_CLRENA0_SHIFT                  0
#define NVIC_ICER_CLRENA1_MASK                   0x2u
#define NVIC_ICER_CLRENA1_SHIFT                  1
#define NVIC_ICER_CLRENA2_MASK                   0x4u
#define NVIC_ICER_CLRENA2_SHIFT                  2
#define NVIC_ICER_CLRENA3_MASK                   0x8u
#define NVIC_ICER_CLRENA3_SHIFT                  3
#define NVIC_ICER_CLRENA4_MASK                   0x10u
#define NVIC_ICER_CLRENA4_SHIFT                  4
#define NVIC_ICER_CLRENA5_MASK                   0x20u
#define NVIC_ICER_CLRENA5_SHIFT                  5
#define NVIC_ICER_CLRENA6_MASK                   0x40u
#define NVIC_ICER_CLRENA6_SHIFT                  6
#define NVIC_ICER_CLRENA7_MASK                   0x80u
#define NVIC_ICER_CLRENA7_SHIFT                  7
#define NVIC_ICER_CLRENA8_MASK                   0x100u
#define NVIC_ICER_CLRENA8_SHIFT                  8
#define NVIC_ICER_CLRENA9_MASK                   0x200u
#define NVIC_ICER_CLRENA9_SHIFT                  9
#define NVIC_ICER_CLRENA10_MASK                  0x400u
#define NVIC_ICER_CLRENA10_SHIFT                 10
#define NVIC_ICER_CLRENA11_MASK                  0x800u
#define NVIC_ICER_CLRENA11_SHIFT                 11
#define NVIC_ICER_CLRENA12_MASK                  0x1000u
#define NVIC_ICER_CLRENA12_SHIFT                 12
#define NVIC_ICER_CLRENA13_MASK                  0x2000u
#define NVIC_ICER_CLRENA13_SHIFT                 13
#define NVIC_ICER_CLRENA14_MASK                  0x4000u
#define NVIC_ICER_CLRENA14_SHIFT                 14
#define NVIC_ICER_CLRENA15_MASK                  0x8000u
#define NVIC_ICER_CLRENA15_SHIFT                 15
#define NVIC_ICER_CLRENA16_MASK                  0x10000u
#define NVIC_ICER_CLRENA16_SHIFT                 16
#define NVIC_ICER_CLRENA17_MASK                  0x20000u
#define NVIC_ICER_CLRENA17_SHIFT                 17
#define NVIC_ICER_CLRENA18_MASK                  0x40000u
#define NVIC_ICER_CLRENA18_SHIFT                 18
#define NVIC_ICER_CLRENA19_MASK                  0x80000u
#define NVIC_ICER_CLRENA19_SHIFT                 19
#define NVIC_ICER_CLRENA20_MASK                  0x100000u
#define NVIC_ICER_CLRENA20_SHIFT                 20
#define NVIC_ICER_CLRENA21_MASK                  0x200000u
#define NVIC_ICER_CLRENA21_SHIFT                 21
#define NVIC_ICER_CLRENA22_MASK                  0x400000u
#define NVIC_ICER_CLRENA22_SHIFT                 22
#define NVIC_ICER_CLRENA23_MASK                  0x800000u
#define NVIC_ICER_CLRENA23_SHIFT                 23
#define NVIC_ICER_CLRENA24_MASK                  0x1000000u
#define NVIC_ICER_CLRENA24_SHIFT                 24
#define NVIC_ICER_CLRENA25_MASK                  0x2000000u
#define NVIC_ICER_CLRENA25_SHIFT                 25
#define NVIC_ICER_CLRENA26_MASK                  0x4000000u
#define NVIC_ICER_CLRENA26_SHIFT                 26
#define NVIC_ICER_CLRENA27_MASK                  0x8000000u
#define NVIC_ICER_CLRENA27_SHIFT                 27
#define NVIC_ICER_CLRENA28_MASK                  0x10000000u
#define NVIC_ICER_CLRENA28_SHIFT                 28
#define NVIC_ICER_CLRENA29_MASK                  0x20000000u
#define NVIC_ICER_CLRENA29_SHIFT                 29
#define NVIC_ICER_CLRENA30_MASK                  0x40000000u
#define NVIC_ICER_CLRENA30_SHIFT                 30
#define NVIC_ICER_CLRENA31_MASK                  0x80000000u
#define NVIC_ICER_CLRENA31_SHIFT                 31
/* ISPR Bit Fields */
#define NVIC_ISPR_SETPEND0_MASK                  0x1u
#define NVIC_ISPR_SETPEND0_SHIFT                 0
#define NVIC_ISPR_SETPEND1_MASK                  0x2u
#define NVIC_ISPR_SETPEND1_SHIFT                 1
#define NVIC_ISPR_SETPEND2_MASK                  0x4u
#define NVIC_ISPR_SETPEND2_SHIFT                 2
#define NVIC_ISPR_SETPEND3_MASK                  0x8u
#define NVIC_ISPR_SETPEND3_SHIFT                 3
#define NVIC_ISPR_SETPEND4_MASK                  0x10u
#define NVIC_ISPR_SETPEND4_SHIFT                 4
#define NVIC_ISPR_SETPEND5_MASK                  0x20u
#define NVIC_ISPR_SETPEND5_SHIFT                 5
#define NVIC_ISPR_SETPEND6_MASK                  0x40u
#define NVIC_ISPR_SETPEND6_SHIFT                 6
#define NVIC_ISPR_SETPEND7_MASK                  0x80u
#define NVIC_ISPR_SETPEND7_SHIFT                 7
#define NVIC_ISPR_SETPEND8_MASK                  0x100u
#define NVIC_ISPR_SETPEND8_SHIFT                 8
#define NVIC_ISPR_SETPEND9_MASK                  0x200u
#define NVIC_ISPR_SETPEND9_SHIFT                 9
#define NVIC_ISPR_SETPEND10_MASK                 0x400u
#define NVIC_ISPR_SETPEND10_SHIFT                10
#define NVIC_ISPR_SETPEND11_MASK                 0x800u
#define NVIC_ISPR_SETPEND11_SHIFT                11
#define NVIC_ISPR_SETPEND12_MASK                 0x1000u
#define NVIC_ISPR_SETPEND12_SHIFT                12
#define NVIC_ISPR_SETPEND13_MASK                 0x2000u
#define NVIC_ISPR_SETPEND13_SHIFT                13
#define NVIC_ISPR_SETPEND14_MASK                 0x4000u
#define NVIC_ISPR_SETPEND14_SHIFT                14
#define NVIC_ISPR_SETPEND15_MASK                 0x8000u
#define NVIC_ISPR_SETPEND15_SHIFT                15
#define NVIC_ISPR_SETPEND16_MASK                 0x10000u
#define NVIC_ISPR_SETPEND16_SHIFT                16
#define NVIC_ISPR_SETPEND17_MASK                 0x20000u
#define NVIC_ISPR_SETPEND17_SHIFT                17
#define NVIC_ISPR_SETPEND18_MASK                 0x40000u
#define NVIC_ISPR_SETPEND18_SHIFT                18
#define NVIC_ISPR_SETPEND19_MASK                 0x80000u
#define NVIC_ISPR_SETPEND19_SHIFT                19
#define NVIC_ISPR_SETPEND20_MASK                 0x100000u
#define NVIC_ISPR_SETPEND20_SHIFT                20
#define NVIC_ISPR_SETPEND21_MASK                 0x200000u
#define NVIC_ISPR_SETPEND21_SHIFT                21
#define NVIC_ISPR_SETPEND22_MASK                 0x400000u
#define NVIC_ISPR_SETPEND22_SHIFT                22
#define NVIC_ISPR_SETPEND23_MASK                 0x800000u
#define NVIC_ISPR_SETPEND23_SHIFT                23
#define NVIC_ISPR_SETPEND24_MASK                 0x1000000u
#define NVIC_ISPR_SETPEND24_SHIFT                24
#define NVIC_ISPR_SETPEND25_MASK                 0x2000000u
#define NVIC_ISPR_SETPEND25_SHIFT                25
#define NVIC_ISPR_SETPEND26_MASK                 0x4000000u
#define NVIC_ISPR_SETPEND26_SHIFT                26
#define NVIC_ISPR_SETPEND27_MASK                 0x8000000u
#define NVIC_ISPR_SETPEND27_SHIFT                27
#define NVIC_ISPR_SETPEND28_MASK                 0x10000000u
#define NVIC_ISPR_SETPEND28_SHIFT                28
#define NVIC_ISPR_SETPEND29_MASK                 0x20000000u
#define NVIC_ISPR_SETPEND29_SHIFT                29
#define NVIC_ISPR_SETPEND30_MASK                 0x40000000u
#define NVIC_ISPR_SETPEND30_SHIFT                30
#define NVIC_ISPR_SETPEND31_MASK                 0x80000000u
#define NVIC_ISPR_SETPEND31_SHIFT                31
/* ICPR Bit Fields */
#define NVIC_ICPR_CLRPEND0_MASK                  0x1u
#define NVIC_ICPR_CLRPEND0_SHIFT                 0
#define NVIC_ICPR_CLRPEND1_MASK                  0x2u
#define NVIC_ICPR_CLRPEND1_SHIFT                 1
#define NVIC_ICPR_CLRPEND2_MASK                  0x4u
#define NVIC_ICPR_CLRPEND2_SHIFT                 2
#define NVIC_ICPR_CLRPEND3_MASK                  0x8u
#define NVIC_ICPR_CLRPEND3_SHIFT                 3
#define NVIC_ICPR_CLRPEND4_MASK                  0x10u
#define NVIC_ICPR_CLRPEND4_SHIFT                 4
#define NVIC_ICPR_CLRPEND5_MASK                  0x20u
#define NVIC_ICPR_CLRPEND5_SHIFT                 5
#define NVIC_ICPR_CLRPEND6_MASK                  0x40u
#define NVIC_ICPR_CLRPEND6_SHIFT                 6
#define NVIC_ICPR_CLRPEND7_MASK                  0x80u
#define NVIC_ICPR_CLRPEND7_SHIFT                 7
#define NVIC_ICPR_CLRPEND8_MASK                  0x100u
#define NVIC_ICPR_CLRPEND8_SHIFT                 8
#define NVIC_ICPR_CLRPEND9_MASK                  0x200u
#define NVIC_ICPR_CLRPEND9_SHIFT                 9
#define NVIC_ICPR_CLRPEND10_MASK                 0x400u
#define NVIC_ICPR_CLRPEND10_SHIFT                10
#define NVIC_ICPR_CLRPEND11_MASK                 0x800u
#define NVIC_ICPR_CLRPEND11_SHIFT                11
#define NVIC_ICPR_CLRPEND12_MASK                 0x1000u
#define NVIC_ICPR_CLRPEND12_SHIFT                12
#define NVIC_ICPR_CLRPEND13_MASK                 0x2000u
#define NVIC_ICPR_CLRPEND13_SHIFT                13
#define NVIC_ICPR_CLRPEND14_MASK                 0x4000u
#define NVIC_ICPR_CLRPEND14_SHIFT                14
#define NVIC_ICPR_CLRPEND15_MASK                 0x8000u
#define NVIC_ICPR_CLRPEND15_SHIFT                15
#define NVIC_ICPR_CLRPEND16_MASK                 0x10000u
#define NVIC_ICPR_CLRPEND16_SHIFT                16
#define NVIC_ICPR_CLRPEND17_MASK                 0x20000u
#define NVIC_ICPR_CLRPEND17_SHIFT                17
#define NVIC_ICPR_CLRPEND18_MASK                 0x40000u
#define NVIC_ICPR_CLRPEND18_SHIFT                18
#define NVIC_ICPR_CLRPEND19_MASK                 0x80000u
#define NVIC_ICPR_CLRPEND19_SHIFT                19
#define NVIC_ICPR_CLRPEND20_MASK                 0x100000u
#define NVIC_ICPR_CLRPEND20_SHIFT                20
#define NVIC_ICPR_CLRPEND21_MASK                 0x200000u
#define NVIC_ICPR_CLRPEND21_SHIFT                21
#define NVIC_ICPR_CLRPEND22_MASK                 0x400000u
#define NVIC_ICPR_CLRPEND22_SHIFT                22
#define NVIC_ICPR_CLRPEND23_MASK                 0x800000u
#define NVIC_ICPR_CLRPEND23_SHIFT                23
#define NVIC_ICPR_CLRPEND24_MASK                 0x1000000u
#define NVIC_ICPR_CLRPEND24_SHIFT                24
#define NVIC_ICPR_CLRPEND25_MASK                 0x2000000u
#define NVIC_ICPR_CLRPEND25_SHIFT                25
#define NVIC_ICPR_CLRPEND26_MASK                 0x4000000u
#define NVIC_ICPR_CLRPEND26_SHIFT                26
#define NVIC_ICPR_CLRPEND27_MASK                 0x8000000u
#define NVIC_ICPR_CLRPEND27_SHIFT                27
#define NVIC_ICPR_CLRPEND28_MASK                 0x10000000u
#define NVIC_ICPR_CLRPEND28_SHIFT                28
#define NVIC_ICPR_CLRPEND29_MASK                 0x20000000u
#define NVIC_ICPR_CLRPEND29_SHIFT                29
#define NVIC_ICPR_CLRPEND30_MASK                 0x40000000u
#define NVIC_ICPR_CLRPEND30_SHIFT                30
#define NVIC_ICPR_CLRPEND31_MASK                 0x80000000u
#define NVIC_ICPR_CLRPEND31_SHIFT                31
/* IP Bit Fields */
#define NVIC_IP_PRI_0_MASK                       0xFFu
#define NVIC_IP_PRI_0_SHIFT                      0
#define NVIC_IP_PRI_0(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_0_SHIFT))&NVIC_IP_PRI_0_MASK)
#define NVIC_IP_PRI_28_MASK                      0xFFu
#define NVIC_IP_PRI_28_SHIFT                     0
#define NVIC_IP_PRI_28(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_28_SHIFT))&NVIC_IP_PRI_28_MASK)
#define NVIC_IP_PRI_24_MASK                      0xFFu
#define NVIC_IP_PRI_24_SHIFT                     0
#define NVIC_IP_PRI_24(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_24_SHIFT))&NVIC_IP_PRI_24_MASK)
#define NVIC_IP_PRI_20_MASK                      0xFFu
#define NVIC_IP_PRI_20_SHIFT                     0
#define NVIC_IP_PRI_20(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_20_SHIFT))&NVIC_IP_PRI_20_MASK)
#define NVIC_IP_PRI_4_MASK                       0xFFu
#define NVIC_IP_PRI_4_SHIFT                      0
#define NVIC_IP_PRI_4(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_4_SHIFT))&NVIC_IP_PRI_4_MASK)
#define NVIC_IP_PRI_16_MASK                      0xFFu
#define NVIC_IP_PRI_16_SHIFT                     0
#define NVIC_IP_PRI_16(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_16_SHIFT))&NVIC_IP_PRI_16_MASK)
#define NVIC_IP_PRI_12_MASK                      0xFFu
#define NVIC_IP_PRI_12_SHIFT                     0
#define NVIC_IP_PRI_12(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_12_SHIFT))&NVIC_IP_PRI_12_MASK)
#define NVIC_IP_PRI_8_MASK                       0xFFu
#define NVIC_IP_PRI_8_SHIFT                      0
#define NVIC_IP_PRI_8(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_8_SHIFT))&NVIC_IP_PRI_8_MASK)
#define NVIC_IP_PRI_13_MASK                      0xFF00u
#define NVIC_IP_PRI_13_SHIFT                     8
#define NVIC_IP_PRI_13(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_13_SHIFT))&NVIC_IP_PRI_13_MASK)
#define NVIC_IP_PRI_21_MASK                      0xFF00u
#define NVIC_IP_PRI_21_SHIFT                     8
#define NVIC_IP_PRI_21(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_21_SHIFT))&NVIC_IP_PRI_21_MASK)
#define NVIC_IP_PRI_29_MASK                      0xFF00u
#define NVIC_IP_PRI_29_SHIFT                     8
#define NVIC_IP_PRI_29(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_29_SHIFT))&NVIC_IP_PRI_29_MASK)
#define NVIC_IP_PRI_1_MASK                       0xFF00u
#define NVIC_IP_PRI_1_SHIFT                      8
#define NVIC_IP_PRI_1(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_1_SHIFT))&NVIC_IP_PRI_1_MASK)
#define NVIC_IP_PRI_9_MASK                       0xFF00u
#define NVIC_IP_PRI_9_SHIFT                      8
#define NVIC_IP_PRI_9(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_9_SHIFT))&NVIC_IP_PRI_9_MASK)
#define NVIC_IP_PRI_17_MASK                      0xFF00u
#define NVIC_IP_PRI_17_SHIFT                     8
#define NVIC_IP_PRI_17(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_17_SHIFT))&NVIC_IP_PRI_17_MASK)
#define NVIC_IP_PRI_25_MASK                      0xFF00u
#define NVIC_IP_PRI_25_SHIFT                     8
#define NVIC_IP_PRI_25(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_25_SHIFT))&NVIC_IP_PRI_25_MASK)
#define NVIC_IP_PRI_5_MASK                       0xFF00u
#define NVIC_IP_PRI_5_SHIFT                      8
#define NVIC_IP_PRI_5(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_5_SHIFT))&NVIC_IP_PRI_5_MASK)
#define NVIC_IP_PRI_2_MASK                       0xFF0000u
#define NVIC_IP_PRI_2_SHIFT                      16
#define NVIC_IP_PRI_2(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_2_SHIFT))&NVIC_IP_PRI_2_MASK)
#define NVIC_IP_PRI_26_MASK                      0xFF0000u
#define NVIC_IP_PRI_26_SHIFT                     16
#define NVIC_IP_PRI_26(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_26_SHIFT))&NVIC_IP_PRI_26_MASK)
#define NVIC_IP_PRI_18_MASK                      0xFF0000u
#define NVIC_IP_PRI_18_SHIFT                     16
#define NVIC_IP_PRI_18(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_18_SHIFT))&NVIC_IP_PRI_18_MASK)
#define NVIC_IP_PRI_14_MASK                      0xFF0000u
#define NVIC_IP_PRI_14_SHIFT                     16
#define NVIC_IP_PRI_14(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_14_SHIFT))&NVIC_IP_PRI_14_MASK)
#define NVIC_IP_PRI_6_MASK                       0xFF0000u
#define NVIC_IP_PRI_6_SHIFT                      16
#define NVIC_IP_PRI_6(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_6_SHIFT))&NVIC_IP_PRI_6_MASK)
#define NVIC_IP_PRI_30_MASK                      0xFF0000u
#define NVIC_IP_PRI_30_SHIFT                     16
#define NVIC_IP_PRI_30(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_30_SHIFT))&NVIC_IP_PRI_30_MASK)
#define NVIC_IP_PRI_22_MASK                      0xFF0000u
#define NVIC_IP_PRI_22_SHIFT                     16
#define NVIC_IP_PRI_22(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_22_SHIFT))&NVIC_IP_PRI_22_MASK)
#define NVIC_IP_PRI_10_MASK                      0xFF0000u
#define NVIC_IP_PRI_10_SHIFT                     16
#define NVIC_IP_PRI_10(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_10_SHIFT))&NVIC_IP_PRI_10_MASK)
#define NVIC_IP_PRI_31_MASK                      0xFF000000u
#define NVIC_IP_PRI_31_SHIFT                     24
#define NVIC_IP_PRI_31(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_31_SHIFT))&NVIC_IP_PRI_31_MASK)
#define NVIC_IP_PRI_27_MASK                      0xFF000000u
#define NVIC_IP_PRI_27_SHIFT                     24
#define NVIC_IP_PRI_27(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_27_SHIFT))&NVIC_IP_PRI_27_MASK)
#define NVIC_IP_PRI_23_MASK                      0xFF000000u
#define NVIC_IP_PRI_23_SHIFT                     24
#define NVIC_IP_PRI_23(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_23_SHIFT))&NVIC_IP_PRI_23_MASK)
#define NVIC_IP_PRI_3_MASK                       0xFF000000u
#define NVIC_IP_PRI_3_SHIFT                      24
#define NVIC_IP_PRI_3(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_3_SHIFT))&NVIC_IP_PRI_3_MASK)
#define NVIC_IP_PRI_19_MASK                      0xFF000000u
#define NVIC_IP_PRI_19_SHIFT                     24
#define NVIC_IP_PRI_19(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_19_SHIFT))&NVIC_IP_PRI_19_MASK)
#define NVIC_IP_PRI_15_MASK                      0xFF000000u
#define NVIC_IP_PRI_15_SHIFT                     24
#define NVIC_IP_PRI_15(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_15_SHIFT))&NVIC_IP_PRI_15_MASK)
#define NVIC_IP_PRI_11_MASK                      0xFF000000u
#define NVIC_IP_PRI_11_SHIFT                     24
#define NVIC_IP_PRI_11(x)                        (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_11_SHIFT))&NVIC_IP_PRI_11_MASK)
#define NVIC_IP_PRI_7_MASK                       0xFF000000u
#define NVIC_IP_PRI_7_SHIFT                      24
#define NVIC_IP_PRI_7(x)                         (((uint32_t)(((uint32_t)(x))<<NVIC_IP_PRI_7_SHIFT))&NVIC_IP_PRI_7_MASK)

/*!
 * @}
 */ /* end of group NVIC_Register_Masks */


/* NVIC - Peripheral instance base addresses */
/** Peripheral NVIC base pointer */
#define NVIC_BASE_PTR                            ((NVIC_MemMapPtr)0xE000E100u)
/** Array initializer of NVIC peripheral base pointers */
#define NVIC_BASE_PTRS                           { NVIC_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- NVIC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NVIC_Register_Accessor_Macros NVIC - Register accessor macros
 * @{
 */


/* NVIC - Register instance definitions */
/* NVIC */
#define NVIC_ISER                                NVIC_ISER_REG(NVIC_BASE_PTR)
#define NVIC_ICER                                NVIC_ICER_REG(NVIC_BASE_PTR)
#define NVIC_ISPR                                NVIC_ISPR_REG(NVIC_BASE_PTR)
#define NVIC_ICPR                                NVIC_ICPR_REG(NVIC_BASE_PTR)
#define NVIC_IPR0                                NVIC_IP_REG(NVIC_BASE_PTR,0)
#define NVIC_IPR1                                NVIC_IP_REG(NVIC_BASE_PTR,1)
#define NVIC_IPR2                                NVIC_IP_REG(NVIC_BASE_PTR,2)
#define NVIC_IPR3                                NVIC_IP_REG(NVIC_BASE_PTR,3)
#define NVIC_IPR4                                NVIC_IP_REG(NVIC_BASE_PTR,4)
#define NVIC_IPR5                                NVIC_IP_REG(NVIC_BASE_PTR,5)
#define NVIC_IPR6                                NVIC_IP_REG(NVIC_BASE_PTR,6)
#define NVIC_IPR7                                NVIC_IP_REG(NVIC_BASE_PTR,7)

/* NVIC - Register array accessors */
#define NVIC_IP(index)                           NVIC_IP_REG(NVIC_BASE_PTR,index)

/*!
 * @}
 */ /* end of group NVIC_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group NVIC_Peripheral */


/* ----------------------------------------------------------------------------
   -- OSC
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup OSC_Peripheral OSC
 * @{
 */

/** OSC - Peripheral register structure */
typedef struct OSC_MemMap {
  uint8_t CR;                                      /**< OSC Control Register, offset: 0x0 */
} volatile *OSC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- OSC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup OSC_Register_Accessor_Macros OSC - Register accessor macros
 * @{
 */


/* OSC - Register accessors */
#define OSC_CR_REG(base)                         ((base)->CR)

/*!
 * @}
 */ /* end of group OSC_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- OSC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup OSC_Register_Masks OSC Register Masks
 * @{
 */

/* CR Bit Fields */
#define OSC_CR_SC16P_MASK                        0x1u
#define OSC_CR_SC16P_SHIFT                       0
#define OSC_CR_SC8P_MASK                         0x2u
#define OSC_CR_SC8P_SHIFT                        1
#define OSC_CR_SC4P_MASK                         0x4u
#define OSC_CR_SC4P_SHIFT                        2
#define OSC_CR_SC2P_MASK                         0x8u
#define OSC_CR_SC2P_SHIFT                        3
#define OSC_CR_EREFSTEN_MASK                     0x20u
#define OSC_CR_EREFSTEN_SHIFT                    5
#define OSC_CR_ERCLKEN_MASK                      0x80u
#define OSC_CR_ERCLKEN_SHIFT                     7

/*!
 * @}
 */ /* end of group OSC_Register_Masks */


/* OSC - Peripheral instance base addresses */
/** Peripheral OSC0 base pointer */
#define OSC0_BASE_PTR                            ((OSC_MemMapPtr)0x40065000u)
/** Array initializer of OSC peripheral base pointers */
#define OSC_BASE_PTRS                            { OSC0_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- OSC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup OSC_Register_Accessor_Macros OSC - Register accessor macros
 * @{
 */


/* OSC - Register instance definitions */
/* OSC0 */
#define OSC0_CR                                  OSC_CR_REG(OSC0_BASE_PTR)

/*!
 * @}
 */ /* end of group OSC_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group OSC_Peripheral */


/* ----------------------------------------------------------------------------
   -- PMC
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMC_Peripheral PMC
 * @{
 */

/** PMC - Peripheral register structure */
typedef struct PMC_MemMap {
  uint8_t LVDSC1;                                  /**< Low Voltage Detect Status And Control 1 register, offset: 0x0 */
  uint8_t LVDSC2;                                  /**< Low Voltage Detect Status And Control 2 register, offset: 0x1 */
  uint8_t REGSC;                                   /**< Regulator Status And Control register, offset: 0x2 */
} volatile *PMC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- PMC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMC_Register_Accessor_Macros PMC - Register accessor macros
 * @{
 */


/* PMC - Register accessors */
#define PMC_LVDSC1_REG(base)                     ((base)->LVDSC1)
#define PMC_LVDSC2_REG(base)                     ((base)->LVDSC2)
#define PMC_REGSC_REG(base)                      ((base)->REGSC)

/*!
 * @}
 */ /* end of group PMC_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- PMC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMC_Register_Masks PMC Register Masks
 * @{
 */

/* LVDSC1 Bit Fields */
#define PMC_LVDSC1_LVDV_MASK                     0x3u
#define PMC_LVDSC1_LVDV_SHIFT                    0
#define PMC_LVDSC1_LVDV(x)                       (((uint8_t)(((uint8_t)(x))<<PMC_LVDSC1_LVDV_SHIFT))&PMC_LVDSC1_LVDV_MASK)
#define PMC_LVDSC1_LVDRE_MASK                    0x10u
#define PMC_LVDSC1_LVDRE_SHIFT                   4
#define PMC_LVDSC1_LVDIE_MASK                    0x20u
#define PMC_LVDSC1_LVDIE_SHIFT                   5
#define PMC_LVDSC1_LVDACK_MASK                   0x40u
#define PMC_LVDSC1_LVDACK_SHIFT                  6
#define PMC_LVDSC1_LVDF_MASK                     0x80u
#define PMC_LVDSC1_LVDF_SHIFT                    7
/* LVDSC2 Bit Fields */
#define PMC_LVDSC2_LVWV_MASK                     0x3u
#define PMC_LVDSC2_LVWV_SHIFT                    0
#define PMC_LVDSC2_LVWV(x)                       (((uint8_t)(((uint8_t)(x))<<PMC_LVDSC2_LVWV_SHIFT))&PMC_LVDSC2_LVWV_MASK)
#define PMC_LVDSC2_LVWIE_MASK                    0x20u
#define PMC_LVDSC2_LVWIE_SHIFT                   5
#define PMC_LVDSC2_LVWACK_MASK                   0x40u
#define PMC_LVDSC2_LVWACK_SHIFT                  6
#define PMC_LVDSC2_LVWF_MASK                     0x80u
#define PMC_LVDSC2_LVWF_SHIFT                    7
/* REGSC Bit Fields */
#define PMC_REGSC_BGBE_MASK                      0x1u
#define PMC_REGSC_BGBE_SHIFT                     0
#define PMC_REGSC_REGONS_MASK                    0x4u
#define PMC_REGSC_REGONS_SHIFT                   2
#define PMC_REGSC_ACKISO_MASK                    0x8u
#define PMC_REGSC_ACKISO_SHIFT                   3
#define PMC_REGSC_BGEN_MASK                      0x10u
#define PMC_REGSC_BGEN_SHIFT                     4

/*!
 * @}
 */ /* end of group PMC_Register_Masks */


/* PMC - Peripheral instance base addresses */
/** Peripheral PMC base pointer */
#define PMC_BASE_PTR                             ((PMC_MemMapPtr)0x4007D000u)
/** Array initializer of PMC peripheral base pointers */
#define PMC_BASE_PTRS                            { PMC_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- PMC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMC_Register_Accessor_Macros PMC - Register accessor macros
 * @{
 */


/* PMC - Register instance definitions */
/* PMC */
#define PMC_LVDSC1                               PMC_LVDSC1_REG(PMC_BASE_PTR)
#define PMC_LVDSC2                               PMC_LVDSC2_REG(PMC_BASE_PTR)
#define PMC_REGSC                                PMC_REGSC_REG(PMC_BASE_PTR)

/*!
 * @}
 */ /* end of group PMC_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group PMC_Peripheral */


/* ----------------------------------------------------------------------------
   -- PORT
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PORT_Peripheral PORT
 * @{
 */

/** PORT - Peripheral register structure */
typedef struct PORT_MemMap {
  uint32_t PCR[32];                                /**< Pin Control Register n, array offset: 0x0, array step: 0x4 */
  uint32_t GPCLR;                                  /**< Global Pin Control Low Register, offset: 0x80 */
  uint32_t GPCHR;                                  /**< Global Pin Control High Register, offset: 0x84 */
  uint8_t RESERVED_0[24];
  uint32_t ISFR;                                   /**< Interrupt Status Flag Register, offset: 0xA0 */
} volatile *PORT_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- PORT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PORT_Register_Accessor_Macros PORT - Register accessor macros
 * @{
 */


/* PORT - Register accessors */
#define PORT_PCR_REG(base,index)                 ((base)->PCR[index])
#define PORT_GPCLR_REG(base)                     ((base)->GPCLR)
#define PORT_GPCHR_REG(base)                     ((base)->GPCHR)
#define PORT_ISFR_REG(base)                      ((base)->ISFR)

/*!
 * @}
 */ /* end of group PORT_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- PORT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PORT_Register_Masks PORT Register Masks
 * @{
 */

/* PCR Bit Fields */
#define PORT_PCR_PS_MASK                         0x1u
#define PORT_PCR_PS_SHIFT                        0
#define PORT_PCR_PE_MASK                         0x2u
#define PORT_PCR_PE_SHIFT                        1
#define PORT_PCR_SRE_MASK                        0x4u
#define PORT_PCR_SRE_SHIFT                       2
#define PORT_PCR_PFE_MASK                        0x10u
#define PORT_PCR_PFE_SHIFT                       4
#define PORT_PCR_DSE_MASK                        0x40u
#define PORT_PCR_DSE_SHIFT                       6
#define PORT_PCR_MUX_MASK                        0x700u
#define PORT_PCR_MUX_SHIFT                       8
#define PORT_PCR_MUX(x)                          (((uint32_t)(((uint32_t)(x))<<PORT_PCR_MUX_SHIFT))&PORT_PCR_MUX_MASK)
#define PORT_PCR_IRQC_MASK                       0xF0000u
#define PORT_PCR_IRQC_SHIFT                      16
#define PORT_PCR_IRQC(x)                         (((uint32_t)(((uint32_t)(x))<<PORT_PCR_IRQC_SHIFT))&PORT_PCR_IRQC_MASK)
#define PORT_PCR_ISF_MASK                        0x1000000u
#define PORT_PCR_ISF_SHIFT                       24
/* GPCLR Bit Fields */
#define PORT_GPCLR_GPWD_MASK                     0xFFFFu
#define PORT_GPCLR_GPWD_SHIFT                    0
#define PORT_GPCLR_GPWD(x)                       (((uint32_t)(((uint32_t)(x))<<PORT_GPCLR_GPWD_SHIFT))&PORT_GPCLR_GPWD_MASK)
#define PORT_GPCLR_GPWE_MASK                     0xFFFF0000u
#define PORT_GPCLR_GPWE_SHIFT                    16
#define PORT_GPCLR_GPWE(x)                       (((uint32_t)(((uint32_t)(x))<<PORT_GPCLR_GPWE_SHIFT))&PORT_GPCLR_GPWE_MASK)
/* GPCHR Bit Fields */
#define PORT_GPCHR_GPWD_MASK                     0xFFFFu
#define PORT_GPCHR_GPWD_SHIFT                    0
#define PORT_GPCHR_GPWD(x)                       (((uint32_t)(((uint32_t)(x))<<PORT_GPCHR_GPWD_SHIFT))&PORT_GPCHR_GPWD_MASK)
#define PORT_GPCHR_GPWE_MASK                     0xFFFF0000u
#define PORT_GPCHR_GPWE_SHIFT                    16
#define PORT_GPCHR_GPWE(x)                       (((uint32_t)(((uint32_t)(x))<<PORT_GPCHR_GPWE_SHIFT))&PORT_GPCHR_GPWE_MASK)
/* ISFR Bit Fields */
#define PORT_ISFR_ISF_MASK                       0xFFFFFFFFu
#define PORT_ISFR_ISF_SHIFT                      0
#define PORT_ISFR_ISF(x)                         (((uint32_t)(((uint32_t)(x))<<PORT_ISFR_ISF_SHIFT))&PORT_ISFR_ISF_MASK)

/*!
 * @}
 */ /* end of group PORT_Register_Masks */


/* PORT - Peripheral instance base addresses */
/** Peripheral PORTA base pointer */
#define PORTA_BASE_PTR                           ((PORT_MemMapPtr)0x40049000u)
/** Peripheral PORTB base pointer */
#define PORTB_BASE_PTR                           ((PORT_MemMapPtr)0x4004A000u)
/** Array initializer of PORT peripheral base pointers */
#define PORT_BASE_PTRS                           { PORTA_BASE_PTR, PORTB_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- PORT - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PORT_Register_Accessor_Macros PORT - Register accessor macros
 * @{
 */


/* PORT - Register instance definitions */
/* PORTA */
#define PORTA_PCR0                               PORT_PCR_REG(PORTA_BASE_PTR,0)
#define PORTA_PCR1                               PORT_PCR_REG(PORTA_BASE_PTR,1)
#define PORTA_PCR2                               PORT_PCR_REG(PORTA_BASE_PTR,2)
#define PORTA_PCR3                               PORT_PCR_REG(PORTA_BASE_PTR,3)
#define PORTA_PCR4                               PORT_PCR_REG(PORTA_BASE_PTR,4)
#define PORTA_PCR5                               PORT_PCR_REG(PORTA_BASE_PTR,5)
#define PORTA_PCR6                               PORT_PCR_REG(PORTA_BASE_PTR,6)
#define PORTA_PCR7                               PORT_PCR_REG(PORTA_BASE_PTR,7)
#define PORTA_PCR8                               PORT_PCR_REG(PORTA_BASE_PTR,8)
#define PORTA_PCR9                               PORT_PCR_REG(PORTA_BASE_PTR,9)
#define PORTA_PCR10                              PORT_PCR_REG(PORTA_BASE_PTR,10)
#define PORTA_PCR11                              PORT_PCR_REG(PORTA_BASE_PTR,11)
#define PORTA_PCR12                              PORT_PCR_REG(PORTA_BASE_PTR,12)
#define PORTA_PCR13                              PORT_PCR_REG(PORTA_BASE_PTR,13)
#define PORTA_PCR14                              PORT_PCR_REG(PORTA_BASE_PTR,14)
#define PORTA_PCR15                              PORT_PCR_REG(PORTA_BASE_PTR,15)
#define PORTA_PCR16                              PORT_PCR_REG(PORTA_BASE_PTR,16)
#define PORTA_PCR17                              PORT_PCR_REG(PORTA_BASE_PTR,17)
#define PORTA_PCR18                              PORT_PCR_REG(PORTA_BASE_PTR,18)
#define PORTA_PCR19                              PORT_PCR_REG(PORTA_BASE_PTR,19)
#define PORTA_PCR20                              PORT_PCR_REG(PORTA_BASE_PTR,20)
#define PORTA_PCR21                              PORT_PCR_REG(PORTA_BASE_PTR,21)
#define PORTA_PCR22                              PORT_PCR_REG(PORTA_BASE_PTR,22)
#define PORTA_PCR23                              PORT_PCR_REG(PORTA_BASE_PTR,23)
#define PORTA_PCR24                              PORT_PCR_REG(PORTA_BASE_PTR,24)
#define PORTA_PCR25                              PORT_PCR_REG(PORTA_BASE_PTR,25)
#define PORTA_PCR26                              PORT_PCR_REG(PORTA_BASE_PTR,26)
#define PORTA_PCR27                              PORT_PCR_REG(PORTA_BASE_PTR,27)
#define PORTA_PCR28                              PORT_PCR_REG(PORTA_BASE_PTR,28)
#define PORTA_PCR29                              PORT_PCR_REG(PORTA_BASE_PTR,29)
#define PORTA_PCR30                              PORT_PCR_REG(PORTA_BASE_PTR,30)
#define PORTA_PCR31                              PORT_PCR_REG(PORTA_BASE_PTR,31)
#define PORTA_GPCLR                              PORT_GPCLR_REG(PORTA_BASE_PTR)
#define PORTA_GPCHR                              PORT_GPCHR_REG(PORTA_BASE_PTR)
#define PORTA_ISFR                               PORT_ISFR_REG(PORTA_BASE_PTR)
/* PORTB */
#define PORTB_PCR0                               PORT_PCR_REG(PORTB_BASE_PTR,0)
#define PORTB_PCR1                               PORT_PCR_REG(PORTB_BASE_PTR,1)
#define PORTB_PCR2                               PORT_PCR_REG(PORTB_BASE_PTR,2)
#define PORTB_PCR3                               PORT_PCR_REG(PORTB_BASE_PTR,3)
#define PORTB_PCR4                               PORT_PCR_REG(PORTB_BASE_PTR,4)
#define PORTB_PCR5                               PORT_PCR_REG(PORTB_BASE_PTR,5)
#define PORTB_PCR6                               PORT_PCR_REG(PORTB_BASE_PTR,6)
#define PORTB_PCR7                               PORT_PCR_REG(PORTB_BASE_PTR,7)
#define PORTB_PCR8                               PORT_PCR_REG(PORTB_BASE_PTR,8)
#define PORTB_PCR9                               PORT_PCR_REG(PORTB_BASE_PTR,9)
#define PORTB_PCR10                              PORT_PCR_REG(PORTB_BASE_PTR,10)
#define PORTB_PCR11                              PORT_PCR_REG(PORTB_BASE_PTR,11)
#define PORTB_PCR12                              PORT_PCR_REG(PORTB_BASE_PTR,12)
#define PORTB_PCR13                              PORT_PCR_REG(PORTB_BASE_PTR,13)
#define PORTB_PCR14                              PORT_PCR_REG(PORTB_BASE_PTR,14)
#define PORTB_PCR15                              PORT_PCR_REG(PORTB_BASE_PTR,15)
#define PORTB_PCR16                              PORT_PCR_REG(PORTB_BASE_PTR,16)
#define PORTB_PCR17                              PORT_PCR_REG(PORTB_BASE_PTR,17)
#define PORTB_PCR18                              PORT_PCR_REG(PORTB_BASE_PTR,18)
#define PORTB_PCR19                              PORT_PCR_REG(PORTB_BASE_PTR,19)
#define PORTB_PCR20                              PORT_PCR_REG(PORTB_BASE_PTR,20)
#define PORTB_PCR21                              PORT_PCR_REG(PORTB_BASE_PTR,21)
#define PORTB_PCR22                              PORT_PCR_REG(PORTB_BASE_PTR,22)
#define PORTB_PCR23                              PORT_PCR_REG(PORTB_BASE_PTR,23)
#define PORTB_PCR24                              PORT_PCR_REG(PORTB_BASE_PTR,24)
#define PORTB_PCR25                              PORT_PCR_REG(PORTB_BASE_PTR,25)
#define PORTB_PCR26                              PORT_PCR_REG(PORTB_BASE_PTR,26)
#define PORTB_PCR27                              PORT_PCR_REG(PORTB_BASE_PTR,27)
#define PORTB_PCR28                              PORT_PCR_REG(PORTB_BASE_PTR,28)
#define PORTB_PCR29                              PORT_PCR_REG(PORTB_BASE_PTR,29)
#define PORTB_PCR30                              PORT_PCR_REG(PORTB_BASE_PTR,30)
#define PORTB_PCR31                              PORT_PCR_REG(PORTB_BASE_PTR,31)
#define PORTB_GPCLR                              PORT_GPCLR_REG(PORTB_BASE_PTR)
#define PORTB_GPCHR                              PORT_GPCHR_REG(PORTB_BASE_PTR)
#define PORTB_ISFR                               PORT_ISFR_REG(PORTB_BASE_PTR)

/* PORT - Register array accessors */
#define PORTA_PCR(index)                         PORT_PCR_REG(PORTA_BASE_PTR,index)
#define PORTB_PCR(index)                         PORT_PCR_REG(PORTB_BASE_PTR,index)

/*!
 * @}
 */ /* end of group PORT_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group PORT_Peripheral */


/* ----------------------------------------------------------------------------
   -- RCM
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RCM_Peripheral RCM
 * @{
 */

/** RCM - Peripheral register structure */
typedef struct RCM_MemMap {
  uint8_t SRS0;                                    /**< System Reset Status Register 0, offset: 0x0 */
  uint8_t SRS1;                                    /**< System Reset Status Register 1, offset: 0x1 */
  uint8_t RESERVED_0[2];
  uint8_t RPFC;                                    /**< Reset Pin Filter Control register, offset: 0x4 */
  uint8_t RPFW;                                    /**< Reset Pin Filter Width register, offset: 0x5 */
} volatile *RCM_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- RCM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RCM_Register_Accessor_Macros RCM - Register accessor macros
 * @{
 */


/* RCM - Register accessors */
#define RCM_SRS0_REG(base)                       ((base)->SRS0)
#define RCM_SRS1_REG(base)                       ((base)->SRS1)
#define RCM_RPFC_REG(base)                       ((base)->RPFC)
#define RCM_RPFW_REG(base)                       ((base)->RPFW)

/*!
 * @}
 */ /* end of group RCM_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- RCM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RCM_Register_Masks RCM Register Masks
 * @{
 */

/* SRS0 Bit Fields */
#define RCM_SRS0_WAKEUP_MASK                     0x1u
#define RCM_SRS0_WAKEUP_SHIFT                    0
#define RCM_SRS0_LVD_MASK                        0x2u
#define RCM_SRS0_LVD_SHIFT                       1
#define RCM_SRS0_LOC_MASK                        0x4u
#define RCM_SRS0_LOC_SHIFT                       2
#define RCM_SRS0_WDOG_MASK                       0x20u
#define RCM_SRS0_WDOG_SHIFT                      5
#define RCM_SRS0_PIN_MASK                        0x40u
#define RCM_SRS0_PIN_SHIFT                       6
#define RCM_SRS0_POR_MASK                        0x80u
#define RCM_SRS0_POR_SHIFT                       7
/* SRS1 Bit Fields */
#define RCM_SRS1_LOCKUP_MASK                     0x2u
#define RCM_SRS1_LOCKUP_SHIFT                    1
#define RCM_SRS1_SW_MASK                         0x4u
#define RCM_SRS1_SW_SHIFT                        2
#define RCM_SRS1_MDM_AP_MASK                     0x8u
#define RCM_SRS1_MDM_AP_SHIFT                    3
#define RCM_SRS1_SACKERR_MASK                    0x20u
#define RCM_SRS1_SACKERR_SHIFT                   5
/* RPFC Bit Fields */
#define RCM_RPFC_RSTFLTSRW_MASK                  0x3u
#define RCM_RPFC_RSTFLTSRW_SHIFT                 0
#define RCM_RPFC_RSTFLTSRW(x)                    (((uint8_t)(((uint8_t)(x))<<RCM_RPFC_RSTFLTSRW_SHIFT))&RCM_RPFC_RSTFLTSRW_MASK)
#define RCM_RPFC_RSTFLTSS_MASK                   0x4u
#define RCM_RPFC_RSTFLTSS_SHIFT                  2
/* RPFW Bit Fields */
#define RCM_RPFW_RSTFLTSEL_MASK                  0x1Fu
#define RCM_RPFW_RSTFLTSEL_SHIFT                 0
#define RCM_RPFW_RSTFLTSEL(x)                    (((uint8_t)(((uint8_t)(x))<<RCM_RPFW_RSTFLTSEL_SHIFT))&RCM_RPFW_RSTFLTSEL_MASK)

/*!
 * @}
 */ /* end of group RCM_Register_Masks */


/* RCM - Peripheral instance base addresses */
/** Peripheral RCM base pointer */
#define RCM_BASE_PTR                             ((RCM_MemMapPtr)0x4007F000u)
/** Array initializer of RCM peripheral base pointers */
#define RCM_BASE_PTRS                            { RCM_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- RCM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RCM_Register_Accessor_Macros RCM - Register accessor macros
 * @{
 */


/* RCM - Register instance definitions */
/* RCM */
#define RCM_SRS0                                 RCM_SRS0_REG(RCM_BASE_PTR)
#define RCM_SRS1                                 RCM_SRS1_REG(RCM_BASE_PTR)
#define RCM_RPFC                                 RCM_RPFC_REG(RCM_BASE_PTR)
#define RCM_RPFW                                 RCM_RPFW_REG(RCM_BASE_PTR)

/*!
 * @}
 */ /* end of group RCM_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group RCM_Peripheral */


/* ----------------------------------------------------------------------------
   -- ROM
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ROM_Peripheral ROM
 * @{
 */

/** ROM - Peripheral register structure */
typedef struct ROM_MemMap {
  uint32_t ENTRY[3];                               /**< Entry, array offset: 0x0, array step: 0x4 */
  uint32_t TABLEMARK;                              /**< End of Table Marker Register, offset: 0xC */
  uint8_t RESERVED_0[4028];
  uint32_t SYSACCESS;                              /**< System Access Register, offset: 0xFCC */
  uint32_t PERIPHID4;                              /**< Peripheral ID Register, offset: 0xFD0 */
  uint32_t PERIPHID5;                              /**< Peripheral ID Register, offset: 0xFD4 */
  uint32_t PERIPHID6;                              /**< Peripheral ID Register, offset: 0xFD8 */
  uint32_t PERIPHID7;                              /**< Peripheral ID Register, offset: 0xFDC */
  uint32_t PERIPHID0;                              /**< Peripheral ID Register, offset: 0xFE0 */
  uint32_t PERIPHID1;                              /**< Peripheral ID Register, offset: 0xFE4 */
  uint32_t PERIPHID2;                              /**< Peripheral ID Register, offset: 0xFE8 */
  uint32_t PERIPHID3;                              /**< Peripheral ID Register, offset: 0xFEC */
  uint32_t COMPID[4];                              /**< Component ID Register, array offset: 0xFF0, array step: 0x4 */
} volatile *ROM_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- ROM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ROM_Register_Accessor_Macros ROM - Register accessor macros
 * @{
 */


/* ROM - Register accessors */
#define ROM_ENTRY_REG(base,index)                ((base)->ENTRY[index])
#define ROM_TABLEMARK_REG(base)                  ((base)->TABLEMARK)
#define ROM_SYSACCESS_REG(base)                  ((base)->SYSACCESS)
#define ROM_PERIPHID4_REG(base)                  ((base)->PERIPHID4)
#define ROM_PERIPHID5_REG(base)                  ((base)->PERIPHID5)
#define ROM_PERIPHID6_REG(base)                  ((base)->PERIPHID6)
#define ROM_PERIPHID7_REG(base)                  ((base)->PERIPHID7)
#define ROM_PERIPHID0_REG(base)                  ((base)->PERIPHID0)
#define ROM_PERIPHID1_REG(base)                  ((base)->PERIPHID1)
#define ROM_PERIPHID2_REG(base)                  ((base)->PERIPHID2)
#define ROM_PERIPHID3_REG(base)                  ((base)->PERIPHID3)
#define ROM_COMPID_REG(base,index)               ((base)->COMPID[index])

/*!
 * @}
 */ /* end of group ROM_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- ROM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ROM_Register_Masks ROM Register Masks
 * @{
 */

/* ENTRY Bit Fields */
#define ROM_ENTRY_ENTRY_MASK                     0xFFFFFFFFu
#define ROM_ENTRY_ENTRY_SHIFT                    0
#define ROM_ENTRY_ENTRY(x)                       (((uint32_t)(((uint32_t)(x))<<ROM_ENTRY_ENTRY_SHIFT))&ROM_ENTRY_ENTRY_MASK)
/* TABLEMARK Bit Fields */
#define ROM_TABLEMARK_MARK_MASK                  0xFFFFFFFFu
#define ROM_TABLEMARK_MARK_SHIFT                 0
#define ROM_TABLEMARK_MARK(x)                    (((uint32_t)(((uint32_t)(x))<<ROM_TABLEMARK_MARK_SHIFT))&ROM_TABLEMARK_MARK_MASK)
/* SYSACCESS Bit Fields */
#define ROM_SYSACCESS_SYSACCESS_MASK             0xFFFFFFFFu
#define ROM_SYSACCESS_SYSACCESS_SHIFT            0
#define ROM_SYSACCESS_SYSACCESS(x)               (((uint32_t)(((uint32_t)(x))<<ROM_SYSACCESS_SYSACCESS_SHIFT))&ROM_SYSACCESS_SYSACCESS_MASK)
/* PERIPHID4 Bit Fields */
#define ROM_PERIPHID4_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID4_PERIPHID_SHIFT             0
#define ROM_PERIPHID4_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID4_PERIPHID_SHIFT))&ROM_PERIPHID4_PERIPHID_MASK)
/* PERIPHID5 Bit Fields */
#define ROM_PERIPHID5_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID5_PERIPHID_SHIFT             0
#define ROM_PERIPHID5_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID5_PERIPHID_SHIFT))&ROM_PERIPHID5_PERIPHID_MASK)
/* PERIPHID6 Bit Fields */
#define ROM_PERIPHID6_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID6_PERIPHID_SHIFT             0
#define ROM_PERIPHID6_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID6_PERIPHID_SHIFT))&ROM_PERIPHID6_PERIPHID_MASK)
/* PERIPHID7 Bit Fields */
#define ROM_PERIPHID7_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID7_PERIPHID_SHIFT             0
#define ROM_PERIPHID7_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID7_PERIPHID_SHIFT))&ROM_PERIPHID7_PERIPHID_MASK)
/* PERIPHID0 Bit Fields */
#define ROM_PERIPHID0_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID0_PERIPHID_SHIFT             0
#define ROM_PERIPHID0_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID0_PERIPHID_SHIFT))&ROM_PERIPHID0_PERIPHID_MASK)
/* PERIPHID1 Bit Fields */
#define ROM_PERIPHID1_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID1_PERIPHID_SHIFT             0
#define ROM_PERIPHID1_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID1_PERIPHID_SHIFT))&ROM_PERIPHID1_PERIPHID_MASK)
/* PERIPHID2 Bit Fields */
#define ROM_PERIPHID2_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID2_PERIPHID_SHIFT             0
#define ROM_PERIPHID2_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID2_PERIPHID_SHIFT))&ROM_PERIPHID2_PERIPHID_MASK)
/* PERIPHID3 Bit Fields */
#define ROM_PERIPHID3_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID3_PERIPHID_SHIFT             0
#define ROM_PERIPHID3_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID3_PERIPHID_SHIFT))&ROM_PERIPHID3_PERIPHID_MASK)
/* COMPID Bit Fields */
#define ROM_COMPID_COMPID_MASK                   0xFFFFFFFFu
#define ROM_COMPID_COMPID_SHIFT                  0
#define ROM_COMPID_COMPID(x)                     (((uint32_t)(((uint32_t)(x))<<ROM_COMPID_COMPID_SHIFT))&ROM_COMPID_COMPID_MASK)

/*!
 * @}
 */ /* end of group ROM_Register_Masks */


/* ROM - Peripheral instance base addresses */
/** Peripheral ROM base pointer */
#define ROM_BASE_PTR                             ((ROM_MemMapPtr)0xF0002000u)
/** Array initializer of ROM peripheral base pointers */
#define ROM_BASE_PTRS                            { ROM_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- ROM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ROM_Register_Accessor_Macros ROM - Register accessor macros
 * @{
 */


/* ROM - Register instance definitions */
/* ROM */
#define ROM_ENTRY0                               ROM_ENTRY_REG(ROM_BASE_PTR,0)
#define ROM_ENTRY1                               ROM_ENTRY_REG(ROM_BASE_PTR,1)
#define ROM_ENTRY2                               ROM_ENTRY_REG(ROM_BASE_PTR,2)
#define ROM_TABLEMARK                            ROM_TABLEMARK_REG(ROM_BASE_PTR)
#define ROM_SYSACCESS                            ROM_SYSACCESS_REG(ROM_BASE_PTR)
#define ROM_PERIPHID4                            ROM_PERIPHID4_REG(ROM_BASE_PTR)
#define ROM_PERIPHID5                            ROM_PERIPHID5_REG(ROM_BASE_PTR)
#define ROM_PERIPHID6                            ROM_PERIPHID6_REG(ROM_BASE_PTR)
#define ROM_PERIPHID7                            ROM_PERIPHID7_REG(ROM_BASE_PTR)
#define ROM_PERIPHID0                            ROM_PERIPHID0_REG(ROM_BASE_PTR)
#define ROM_PERIPHID1                            ROM_PERIPHID1_REG(ROM_BASE_PTR)
#define ROM_PERIPHID2                            ROM_PERIPHID2_REG(ROM_BASE_PTR)
#define ROM_PERIPHID3                            ROM_PERIPHID3_REG(ROM_BASE_PTR)
#define ROM_COMPID0                              ROM_COMPID_REG(ROM_BASE_PTR,0)
#define ROM_COMPID1                              ROM_COMPID_REG(ROM_BASE_PTR,1)
#define ROM_COMPID2                              ROM_COMPID_REG(ROM_BASE_PTR,2)
#define ROM_COMPID3                              ROM_COMPID_REG(ROM_BASE_PTR,3)

/* ROM - Register array accessors */
#define ROM_ENTRY(index)                         ROM_ENTRY_REG(ROM_BASE_PTR,index)
#define ROM_COMPID(index)                        ROM_COMPID_REG(ROM_BASE_PTR,index)

/*!
 * @}
 */ /* end of group ROM_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group ROM_Peripheral */


/* ----------------------------------------------------------------------------
   -- SCB
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SCB_Peripheral SCB
 * @{
 */

/** SCB - Peripheral register structure */
typedef struct SCB_MemMap {
  uint8_t RESERVED_0[8];
  uint32_t ACTLR;                                  /**< Auxiliary Control Register,, offset: 0x8 */
  uint8_t RESERVED_1[3316];
  uint32_t CPUID;                                  /**< CPUID Base Register, offset: 0xD00 */
  uint32_t ICSR;                                   /**< Interrupt Control and State Register, offset: 0xD04 */
  uint32_t VTOR;                                   /**< Vector Table Offset Register, offset: 0xD08 */
  uint32_t AIRCR;                                  /**< Application Interrupt and Reset Control Register, offset: 0xD0C */
  uint32_t SCR;                                    /**< System Control Register, offset: 0xD10 */
  uint32_t CCR;                                    /**< Configuration and Control Register, offset: 0xD14 */
  uint8_t RESERVED_2[4];
  uint32_t SHPR2;                                  /**< System Handler Priority Register 2, offset: 0xD1C */
  uint32_t SHPR3;                                  /**< System Handler Priority Register 3, offset: 0xD20 */
  uint32_t SHCSR;                                  /**< System Handler Control and State Register, offset: 0xD24 */
  uint8_t RESERVED_3[8];
  uint32_t DFSR;                                   /**< Debug Fault Status Register, offset: 0xD30 */
} volatile *SCB_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SCB - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SCB_Register_Accessor_Macros SCB - Register accessor macros
 * @{
 */


/* SCB - Register accessors */
#define SCB_ACTLR_REG(base)                      ((base)->ACTLR)
#define SCB_CPUID_REG(base)                      ((base)->CPUID)
#define SCB_ICSR_REG(base)                       ((base)->ICSR)
#define SCB_VTOR_REG(base)                       ((base)->VTOR)
#define SCB_AIRCR_REG(base)                      ((base)->AIRCR)
#define SCB_SCR_REG(base)                        ((base)->SCR)
#define SCB_CCR_REG(base)                        ((base)->CCR)
#define SCB_SHPR2_REG(base)                      ((base)->SHPR2)
#define SCB_SHPR3_REG(base)                      ((base)->SHPR3)
#define SCB_SHCSR_REG(base)                      ((base)->SHCSR)
#define SCB_DFSR_REG(base)                       ((base)->DFSR)

/*!
 * @}
 */ /* end of group SCB_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- SCB Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SCB_Register_Masks SCB Register Masks
 * @{
 */

/* CPUID Bit Fields */
#define SCB_CPUID_REVISION_MASK                  0xFu
#define SCB_CPUID_REVISION_SHIFT                 0
#define SCB_CPUID_REVISION(x)                    (((uint32_t)(((uint32_t)(x))<<SCB_CPUID_REVISION_SHIFT))&SCB_CPUID_REVISION_MASK)
#define SCB_CPUID_PARTNO_MASK                    0xFFF0u
#define SCB_CPUID_PARTNO_SHIFT                   4
#define SCB_CPUID_PARTNO(x)                      (((uint32_t)(((uint32_t)(x))<<SCB_CPUID_PARTNO_SHIFT))&SCB_CPUID_PARTNO_MASK)
#define SCB_CPUID_VARIANT_MASK                   0xF00000u
#define SCB_CPUID_VARIANT_SHIFT                  20
#define SCB_CPUID_VARIANT(x)                     (((uint32_t)(((uint32_t)(x))<<SCB_CPUID_VARIANT_SHIFT))&SCB_CPUID_VARIANT_MASK)
#define SCB_CPUID_IMPLEMENTER_MASK               0xFF000000u
#define SCB_CPUID_IMPLEMENTER_SHIFT              24
#define SCB_CPUID_IMPLEMENTER(x)                 (((uint32_t)(((uint32_t)(x))<<SCB_CPUID_IMPLEMENTER_SHIFT))&SCB_CPUID_IMPLEMENTER_MASK)
/* ICSR Bit Fields */
#define SCB_ICSR_VECTPENDING_MASK                0x3F000u
#define SCB_ICSR_VECTPENDING_SHIFT               12
#define SCB_ICSR_VECTPENDING(x)                  (((uint32_t)(((uint32_t)(x))<<SCB_ICSR_VECTPENDING_SHIFT))&SCB_ICSR_VECTPENDING_MASK)
#define SCB_ICSR_PENDSTCLR_MASK                  0x2000000u
#define SCB_ICSR_PENDSTCLR_SHIFT                 25
#define SCB_ICSR_PENDSTSET_MASK                  0x4000000u
#define SCB_ICSR_PENDSTSET_SHIFT                 26
#define SCB_ICSR_PENDSVCLR_MASK                  0x8000000u
#define SCB_ICSR_PENDSVCLR_SHIFT                 27
#define SCB_ICSR_PENDSVSET_MASK                  0x10000000u
#define SCB_ICSR_PENDSVSET_SHIFT                 28
#define SCB_ICSR_NMIPENDSET_MASK                 0x80000000u
#define SCB_ICSR_NMIPENDSET_SHIFT                31
/* VTOR Bit Fields */
#define SCB_VTOR_TBLOFF_MASK                     0xFFFFFF80u
#define SCB_VTOR_TBLOFF_SHIFT                    7
#define SCB_VTOR_TBLOFF(x)                       (((uint32_t)(((uint32_t)(x))<<SCB_VTOR_TBLOFF_SHIFT))&SCB_VTOR_TBLOFF_MASK)
/* AIRCR Bit Fields */
#define SCB_AIRCR_VECTCLRACTIVE_MASK             0x2u
#define SCB_AIRCR_VECTCLRACTIVE_SHIFT            1
#define SCB_AIRCR_SYSRESETREQ_MASK               0x4u
#define SCB_AIRCR_SYSRESETREQ_SHIFT              2
#define SCB_AIRCR_ENDIANNESS_MASK                0x8000u
#define SCB_AIRCR_ENDIANNESS_SHIFT               15
#define SCB_AIRCR_VECTKEY_MASK                   0xFFFF0000u
#define SCB_AIRCR_VECTKEY_SHIFT                  16
#define SCB_AIRCR_VECTKEY(x)                     (((uint32_t)(((uint32_t)(x))<<SCB_AIRCR_VECTKEY_SHIFT))&SCB_AIRCR_VECTKEY_MASK)
/* SCR Bit Fields */
#define SCB_SCR_SLEEPONEXIT_MASK                 0x2u
#define SCB_SCR_SLEEPONEXIT_SHIFT                1
#define SCB_SCR_SLEEPDEEP_MASK                   0x4u
#define SCB_SCR_SLEEPDEEP_SHIFT                  2
#define SCB_SCR_SEVONPEND_MASK                   0x10u
#define SCB_SCR_SEVONPEND_SHIFT                  4
/* CCR Bit Fields */
#define SCB_CCR_UNALIGN_TRP_MASK                 0x8u
#define SCB_CCR_UNALIGN_TRP_SHIFT                3
#define SCB_CCR_STKALIGN_MASK                    0x200u
#define SCB_CCR_STKALIGN_SHIFT                   9
/* SHPR2 Bit Fields */
#define SCB_SHPR2_PRI_11_MASK                    0xFF000000u
#define SCB_SHPR2_PRI_11_SHIFT                   24
#define SCB_SHPR2_PRI_11(x)                      (((uint32_t)(((uint32_t)(x))<<SCB_SHPR2_PRI_11_SHIFT))&SCB_SHPR2_PRI_11_MASK)
/* SHPR3 Bit Fields */
#define SCB_SHPR3_PRI_14_MASK                    0xFF0000u
#define SCB_SHPR3_PRI_14_SHIFT                   16
#define SCB_SHPR3_PRI_14(x)                      (((uint32_t)(((uint32_t)(x))<<SCB_SHPR3_PRI_14_SHIFT))&SCB_SHPR3_PRI_14_MASK)
#define SCB_SHPR3_PRI_15_MASK                    0xFF000000u
#define SCB_SHPR3_PRI_15_SHIFT                   24
#define SCB_SHPR3_PRI_15(x)                      (((uint32_t)(((uint32_t)(x))<<SCB_SHPR3_PRI_15_SHIFT))&SCB_SHPR3_PRI_15_MASK)
/* SHCSR Bit Fields */
#define SCB_SHCSR_SVCALLPENDED_MASK              0x8000u
#define SCB_SHCSR_SVCALLPENDED_SHIFT             15
/* DFSR Bit Fields */
#define SCB_DFSR_HALTED_MASK                     0x1u
#define SCB_DFSR_HALTED_SHIFT                    0
#define SCB_DFSR_BKPT_MASK                       0x2u
#define SCB_DFSR_BKPT_SHIFT                      1
#define SCB_DFSR_DWTTRAP_MASK                    0x4u
#define SCB_DFSR_DWTTRAP_SHIFT                   2
#define SCB_DFSR_VCATCH_MASK                     0x8u
#define SCB_DFSR_VCATCH_SHIFT                    3
#define SCB_DFSR_EXTERNAL_MASK                   0x10u
#define SCB_DFSR_EXTERNAL_SHIFT                  4

/*!
 * @}
 */ /* end of group SCB_Register_Masks */


/* SCB - Peripheral instance base addresses */
/** Peripheral SystemControl base pointer */
#define SystemControl_BASE_PTR                   ((SCB_MemMapPtr)0xE000E000u)
/** Array initializer of SCB peripheral base pointers */
#define SCB_BASE_PTRS                            { SystemControl_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- SCB - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SCB_Register_Accessor_Macros SCB - Register accessor macros
 * @{
 */


/* SCB - Register instance definitions */
/* SystemControl */
#define SCB_ACTLR                                SCB_ACTLR_REG(SystemControl_BASE_PTR)
#define SCB_CPUID                                SCB_CPUID_REG(SystemControl_BASE_PTR)
#define SCB_ICSR                                 SCB_ICSR_REG(SystemControl_BASE_PTR)
#define SCB_VTOR                                 SCB_VTOR_REG(SystemControl_BASE_PTR)
#define SCB_AIRCR                                SCB_AIRCR_REG(SystemControl_BASE_PTR)
#define SCB_SCR                                  SCB_SCR_REG(SystemControl_BASE_PTR)
#define SCB_CCR                                  SCB_CCR_REG(SystemControl_BASE_PTR)
#define SCB_SHPR2                                SCB_SHPR2_REG(SystemControl_BASE_PTR)
#define SCB_SHPR3                                SCB_SHPR3_REG(SystemControl_BASE_PTR)
#define SCB_SHCSR                                SCB_SHCSR_REG(SystemControl_BASE_PTR)
#define SCB_DFSR                                 SCB_DFSR_REG(SystemControl_BASE_PTR)

/*!
 * @}
 */ /* end of group SCB_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group SCB_Peripheral */


/* ----------------------------------------------------------------------------
   -- SIM
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SIM_Peripheral SIM
 * @{
 */

/** SIM - Peripheral register structure */
typedef struct SIM_MemMap {
  uint8_t RESERVED_0[4100];
  uint32_t SOPT2;                                  /**< System Options Register 2, offset: 0x1004 */
  uint8_t RESERVED_1[4];
  uint32_t SOPT4;                                  /**< System Options Register 4, offset: 0x100C */
  uint32_t SOPT5;                                  /**< System Options Register 5, offset: 0x1010 */
  uint8_t RESERVED_2[4];
  uint32_t SOPT7;                                  /**< System Options Register 7, offset: 0x1018 */
  uint8_t RESERVED_3[8];
  uint32_t SDID;                                   /**< System Device Identification Register, offset: 0x1024 */
  uint8_t RESERVED_4[12];
  uint32_t SCGC4;                                  /**< System Clock Gating Control Register 4, offset: 0x1034 */
  uint32_t SCGC5;                                  /**< System Clock Gating Control Register 5, offset: 0x1038 */
  uint32_t SCGC6;                                  /**< System Clock Gating Control Register 6, offset: 0x103C */
  uint8_t RESERVED_5[4];
  uint32_t CLKDIV1;                                /**< System Clock Divider Register 1, offset: 0x1044 */
  uint8_t RESERVED_6[4];
  uint32_t FCFG1;                                  /**< Flash Configuration Register 1, offset: 0x104C */
  uint32_t FCFG2;                                  /**< Flash Configuration Register 2, offset: 0x1050 */
  uint8_t RESERVED_7[4];
  uint32_t UIDMH;                                  /**< Unique Identification Register Mid-High, offset: 0x1058 */
  uint32_t UIDML;                                  /**< Unique Identification Register Mid Low, offset: 0x105C */
  uint32_t UIDL;                                   /**< Unique Identification Register Low, offset: 0x1060 */
  uint8_t RESERVED_8[156];
  uint32_t COPC;                                   /**< COP Control Register, offset: 0x1100 */
  uint32_t SRVCOP;                                 /**< Service COP Register, offset: 0x1104 */
} volatile *SIM_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SIM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SIM_Register_Accessor_Macros SIM - Register accessor macros
 * @{
 */


/* SIM - Register accessors */
#define SIM_SOPT2_REG(base)                      ((base)->SOPT2)
#define SIM_SOPT4_REG(base)                      ((base)->SOPT4)
#define SIM_SOPT5_REG(base)                      ((base)->SOPT5)
#define SIM_SOPT7_REG(base)                      ((base)->SOPT7)
#define SIM_SDID_REG(base)                       ((base)->SDID)
#define SIM_SCGC4_REG(base)                      ((base)->SCGC4)
#define SIM_SCGC5_REG(base)                      ((base)->SCGC5)
#define SIM_SCGC6_REG(base)                      ((base)->SCGC6)
#define SIM_CLKDIV1_REG(base)                    ((base)->CLKDIV1)
#define SIM_FCFG1_REG(base)                      ((base)->FCFG1)
#define SIM_FCFG2_REG(base)                      ((base)->FCFG2)
#define SIM_UIDMH_REG(base)                      ((base)->UIDMH)
#define SIM_UIDML_REG(base)                      ((base)->UIDML)
#define SIM_UIDL_REG(base)                       ((base)->UIDL)
#define SIM_COPC_REG(base)                       ((base)->COPC)
#define SIM_SRVCOP_REG(base)                     ((base)->SRVCOP)

/*!
 * @}
 */ /* end of group SIM_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- SIM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SIM_Register_Masks SIM Register Masks
 * @{
 */

/* SOPT2 Bit Fields */
#define SIM_SOPT2_TPMSRC_MASK                    0x3000000u
#define SIM_SOPT2_TPMSRC_SHIFT                   24
#define SIM_SOPT2_TPMSRC(x)                      (((uint32_t)(((uint32_t)(x))<<SIM_SOPT2_TPMSRC_SHIFT))&SIM_SOPT2_TPMSRC_MASK)
#define SIM_SOPT2_UART0SRC_MASK                  0xC000000u
#define SIM_SOPT2_UART0SRC_SHIFT                 26
#define SIM_SOPT2_UART0SRC(x)                    (((uint32_t)(((uint32_t)(x))<<SIM_SOPT2_UART0SRC_SHIFT))&SIM_SOPT2_UART0SRC_MASK)
/* SOPT4 Bit Fields */
#define SIM_SOPT4_TPM1CH0SRC_MASK                0x40000u
#define SIM_SOPT4_TPM1CH0SRC_SHIFT               18
#define SIM_SOPT4_TPM0CLKSEL_MASK                0x1000000u
#define SIM_SOPT4_TPM0CLKSEL_SHIFT               24
#define SIM_SOPT4_TPM1CLKSEL_MASK                0x2000000u
#define SIM_SOPT4_TPM1CLKSEL_SHIFT               25
/* SOPT5 Bit Fields */
#define SIM_SOPT5_UART0TXSRC_MASK                0x1u
#define SIM_SOPT5_UART0TXSRC_SHIFT               0
#define SIM_SOPT5_UART0RXSRC_MASK                0x4u
#define SIM_SOPT5_UART0RXSRC_SHIFT               2
#define SIM_SOPT5_UART0ODE_MASK                  0x10000u
#define SIM_SOPT5_UART0ODE_SHIFT                 16
/* SOPT7 Bit Fields */
#define SIM_SOPT7_ADC0TRGSEL_MASK                0xFu
#define SIM_SOPT7_ADC0TRGSEL_SHIFT               0
#define SIM_SOPT7_ADC0TRGSEL(x)                  (((uint32_t)(((uint32_t)(x))<<SIM_SOPT7_ADC0TRGSEL_SHIFT))&SIM_SOPT7_ADC0TRGSEL_MASK)
#define SIM_SOPT7_ADC0PRETRGSEL_MASK             0x10u
#define SIM_SOPT7_ADC0PRETRGSEL_SHIFT            4
#define SIM_SOPT7_ADC0ALTTRGEN_MASK              0x80u
#define SIM_SOPT7_ADC0ALTTRGEN_SHIFT             7
/* SDID Bit Fields */
#define SIM_SDID_PINID_MASK                      0xFu
#define SIM_SDID_PINID_SHIFT                     0
#define SIM_SDID_PINID(x)                        (((uint32_t)(((uint32_t)(x))<<SIM_SDID_PINID_SHIFT))&SIM_SDID_PINID_MASK)
#define SIM_SDID_DIEID_MASK                      0xF80u
#define SIM_SDID_DIEID_SHIFT                     7
#define SIM_SDID_DIEID(x)                        (((uint32_t)(((uint32_t)(x))<<SIM_SDID_DIEID_SHIFT))&SIM_SDID_DIEID_MASK)
#define SIM_SDID_REVID_MASK                      0xF000u
#define SIM_SDID_REVID_SHIFT                     12
#define SIM_SDID_REVID(x)                        (((uint32_t)(((uint32_t)(x))<<SIM_SDID_REVID_SHIFT))&SIM_SDID_REVID_MASK)
#define SIM_SDID_SRAMSIZE_MASK                   0xF0000u
#define SIM_SDID_SRAMSIZE_SHIFT                  16
#define SIM_SDID_SRAMSIZE(x)                     (((uint32_t)(((uint32_t)(x))<<SIM_SDID_SRAMSIZE_SHIFT))&SIM_SDID_SRAMSIZE_MASK)
#define SIM_SDID_SERIESID_MASK                   0xF00000u
#define SIM_SDID_SERIESID_SHIFT                  20
#define SIM_SDID_SERIESID(x)                     (((uint32_t)(((uint32_t)(x))<<SIM_SDID_SERIESID_SHIFT))&SIM_SDID_SERIESID_MASK)
#define SIM_SDID_SUBFAMID_MASK                   0xF000000u
#define SIM_SDID_SUBFAMID_SHIFT                  24
#define SIM_SDID_SUBFAMID(x)                     (((uint32_t)(((uint32_t)(x))<<SIM_SDID_SUBFAMID_SHIFT))&SIM_SDID_SUBFAMID_MASK)
#define SIM_SDID_FAMID_MASK                      0xF0000000u
#define SIM_SDID_FAMID_SHIFT                     28
#define SIM_SDID_FAMID(x)                        (((uint32_t)(((uint32_t)(x))<<SIM_SDID_FAMID_SHIFT))&SIM_SDID_FAMID_MASK)
/* SCGC4 Bit Fields */
#define SIM_SCGC4_I2C0_MASK                      0x40u
#define SIM_SCGC4_I2C0_SHIFT                     6
#define SIM_SCGC4_I2C1_MASK                      0x80u
#define SIM_SCGC4_I2C1_SHIFT                     7
#define SIM_SCGC4_UART0_MASK                     0x400u
#define SIM_SCGC4_UART0_SHIFT                    10
#define SIM_SCGC4_CMP_MASK                       0x80000u
#define SIM_SCGC4_CMP_SHIFT                      19
#define SIM_SCGC4_SPI0_MASK                      0x400000u
#define SIM_SCGC4_SPI0_SHIFT                     22
/* SCGC5 Bit Fields */
#define SIM_SCGC5_LPTMR_MASK                     0x1u
#define SIM_SCGC5_LPTMR_SHIFT                    0
#define SIM_SCGC5_PORTA_MASK                     0x200u
#define SIM_SCGC5_PORTA_SHIFT                    9
#define SIM_SCGC5_PORTB_MASK                     0x400u
#define SIM_SCGC5_PORTB_SHIFT                    10
/* SCGC6 Bit Fields */
#define SIM_SCGC6_FTF_MASK                       0x1u
#define SIM_SCGC6_FTF_SHIFT                      0
#define SIM_SCGC6_TPM0_MASK                      0x1000000u
#define SIM_SCGC6_TPM0_SHIFT                     24
#define SIM_SCGC6_TPM1_MASK                      0x2000000u
#define SIM_SCGC6_TPM1_SHIFT                     25
#define SIM_SCGC6_ADC0_MASK                      0x8000000u
#define SIM_SCGC6_ADC0_SHIFT                     27
/* CLKDIV1 Bit Fields */
#define SIM_CLKDIV1_OUTDIV4_MASK                 0x70000u
#define SIM_CLKDIV1_OUTDIV4_SHIFT                16
#define SIM_CLKDIV1_OUTDIV4(x)                   (((uint32_t)(((uint32_t)(x))<<SIM_CLKDIV1_OUTDIV4_SHIFT))&SIM_CLKDIV1_OUTDIV4_MASK)
#define SIM_CLKDIV1_OUTDIV1_MASK                 0xF0000000u
#define SIM_CLKDIV1_OUTDIV1_SHIFT                28
#define SIM_CLKDIV1_OUTDIV1(x)                   (((uint32_t)(((uint32_t)(x))<<SIM_CLKDIV1_OUTDIV1_SHIFT))&SIM_CLKDIV1_OUTDIV1_MASK)
/* FCFG1 Bit Fields */
#define SIM_FCFG1_FLASHDIS_MASK                  0x1u
#define SIM_FCFG1_FLASHDIS_SHIFT                 0
#define SIM_FCFG1_FLASHDOZE_MASK                 0x2u
#define SIM_FCFG1_FLASHDOZE_SHIFT                1
#define SIM_FCFG1_PFSIZE_MASK                    0xF000000u
#define SIM_FCFG1_PFSIZE_SHIFT                   24
#define SIM_FCFG1_PFSIZE(x)                      (((uint32_t)(((uint32_t)(x))<<SIM_FCFG1_PFSIZE_SHIFT))&SIM_FCFG1_PFSIZE_MASK)
/* FCFG2 Bit Fields */
#define SIM_FCFG2_MAXADDR0_MASK                  0x7F000000u
#define SIM_FCFG2_MAXADDR0_SHIFT                 24
#define SIM_FCFG2_MAXADDR0(x)                    (((uint32_t)(((uint32_t)(x))<<SIM_FCFG2_MAXADDR0_SHIFT))&SIM_FCFG2_MAXADDR0_MASK)
/* UIDMH Bit Fields */
#define SIM_UIDMH_UID_MASK                       0xFFFFu
#define SIM_UIDMH_UID_SHIFT                      0
#define SIM_UIDMH_UID(x)                         (((uint32_t)(((uint32_t)(x))<<SIM_UIDMH_UID_SHIFT))&SIM_UIDMH_UID_MASK)
/* UIDML Bit Fields */
#define SIM_UIDML_UID_MASK                       0xFFFFFFFFu
#define SIM_UIDML_UID_SHIFT                      0
#define SIM_UIDML_UID(x)                         (((uint32_t)(((uint32_t)(x))<<SIM_UIDML_UID_SHIFT))&SIM_UIDML_UID_MASK)
/* UIDL Bit Fields */
#define SIM_UIDL_UID_MASK                        0xFFFFFFFFu
#define SIM_UIDL_UID_SHIFT                       0
#define SIM_UIDL_UID(x)                          (((uint32_t)(((uint32_t)(x))<<SIM_UIDL_UID_SHIFT))&SIM_UIDL_UID_MASK)
/* COPC Bit Fields */
#define SIM_COPC_COPW_MASK                       0x1u
#define SIM_COPC_COPW_SHIFT                      0
#define SIM_COPC_COPCLKS_MASK                    0x2u
#define SIM_COPC_COPCLKS_SHIFT                   1
#define SIM_COPC_COPT_MASK                       0xCu
#define SIM_COPC_COPT_SHIFT                      2
#define SIM_COPC_COPT(x)                         (((uint32_t)(((uint32_t)(x))<<SIM_COPC_COPT_SHIFT))&SIM_COPC_COPT_MASK)
/* SRVCOP Bit Fields */
#define SIM_SRVCOP_SRVCOP_MASK                   0xFFu
#define SIM_SRVCOP_SRVCOP_SHIFT                  0
#define SIM_SRVCOP_SRVCOP(x)                     (((uint32_t)(((uint32_t)(x))<<SIM_SRVCOP_SRVCOP_SHIFT))&SIM_SRVCOP_SRVCOP_MASK)

/*!
 * @}
 */ /* end of group SIM_Register_Masks */


/* SIM - Peripheral instance base addresses */
/** Peripheral SIM base pointer */
#define SIM_BASE_PTR                             ((SIM_MemMapPtr)0x40047000u)
/** Array initializer of SIM peripheral base pointers */
#define SIM_BASE_PTRS                            { SIM_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- SIM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SIM_Register_Accessor_Macros SIM - Register accessor macros
 * @{
 */


/* SIM - Register instance definitions */
/* SIM */
#define SIM_SOPT2                                SIM_SOPT2_REG(SIM_BASE_PTR)
#define SIM_SOPT4                                SIM_SOPT4_REG(SIM_BASE_PTR)
#define SIM_SOPT5                                SIM_SOPT5_REG(SIM_BASE_PTR)
#define SIM_SOPT7                                SIM_SOPT7_REG(SIM_BASE_PTR)
#define SIM_SDID                                 SIM_SDID_REG(SIM_BASE_PTR)
#define SIM_SCGC4                                SIM_SCGC4_REG(SIM_BASE_PTR)
#define SIM_SCGC5                                SIM_SCGC5_REG(SIM_BASE_PTR)
#define SIM_SCGC6                                SIM_SCGC6_REG(SIM_BASE_PTR)
#define SIM_CLKDIV1                              SIM_CLKDIV1_REG(SIM_BASE_PTR)
#define SIM_FCFG1                                SIM_FCFG1_REG(SIM_BASE_PTR)
#define SIM_FCFG2                                SIM_FCFG2_REG(SIM_BASE_PTR)
#define SIM_UIDMH                                SIM_UIDMH_REG(SIM_BASE_PTR)
#define SIM_UIDML                                SIM_UIDML_REG(SIM_BASE_PTR)
#define SIM_UIDL                                 SIM_UIDL_REG(SIM_BASE_PTR)
#define SIM_COPC                                 SIM_COPC_REG(SIM_BASE_PTR)
#define SIM_SRVCOP                               SIM_SRVCOP_REG(SIM_BASE_PTR)

/*!
 * @}
 */ /* end of group SIM_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group SIM_Peripheral */


/* ----------------------------------------------------------------------------
   -- SMC
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SMC_Peripheral SMC
 * @{
 */

/** SMC - Peripheral register structure */
typedef struct SMC_MemMap {
  uint8_t PMPROT;                                  /**< Power Mode Protection register, offset: 0x0 */
  uint8_t PMCTRL;                                  /**< Power Mode Control register, offset: 0x1 */
  uint8_t STOPCTRL;                                /**< Stop Control Register, offset: 0x2 */
  uint8_t PMSTAT;                                  /**< Power Mode Status register, offset: 0x3 */
} volatile *SMC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SMC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SMC_Register_Accessor_Macros SMC - Register accessor macros
 * @{
 */


/* SMC - Register accessors */
#define SMC_PMPROT_REG(base)                     ((base)->PMPROT)
#define SMC_PMCTRL_REG(base)                     ((base)->PMCTRL)
#define SMC_STOPCTRL_REG(base)                   ((base)->STOPCTRL)
#define SMC_PMSTAT_REG(base)                     ((base)->PMSTAT)

/*!
 * @}
 */ /* end of group SMC_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- SMC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SMC_Register_Masks SMC Register Masks
 * @{
 */

/* PMPROT Bit Fields */
#define SMC_PMPROT_AVLLS_MASK                    0x2u
#define SMC_PMPROT_AVLLS_SHIFT                   1
#define SMC_PMPROT_AVLP_MASK                     0x20u
#define SMC_PMPROT_AVLP_SHIFT                    5
/* PMCTRL Bit Fields */
#define SMC_PMCTRL_STOPM_MASK                    0x7u
#define SMC_PMCTRL_STOPM_SHIFT                   0
#define SMC_PMCTRL_STOPM(x)                      (((uint8_t)(((uint8_t)(x))<<SMC_PMCTRL_STOPM_SHIFT))&SMC_PMCTRL_STOPM_MASK)
#define SMC_PMCTRL_STOPA_MASK                    0x8u
#define SMC_PMCTRL_STOPA_SHIFT                   3
#define SMC_PMCTRL_RUNM_MASK                     0x60u
#define SMC_PMCTRL_RUNM_SHIFT                    5
#define SMC_PMCTRL_RUNM(x)                       (((uint8_t)(((uint8_t)(x))<<SMC_PMCTRL_RUNM_SHIFT))&SMC_PMCTRL_RUNM_MASK)
/* STOPCTRL Bit Fields */
#define SMC_STOPCTRL_VLLSM_MASK                  0x7u
#define SMC_STOPCTRL_VLLSM_SHIFT                 0
#define SMC_STOPCTRL_VLLSM(x)                    (((uint8_t)(((uint8_t)(x))<<SMC_STOPCTRL_VLLSM_SHIFT))&SMC_STOPCTRL_VLLSM_MASK)
#define SMC_STOPCTRL_PORPO_MASK                  0x20u
#define SMC_STOPCTRL_PORPO_SHIFT                 5
#define SMC_STOPCTRL_PSTOPO_MASK                 0xC0u
#define SMC_STOPCTRL_PSTOPO_SHIFT                6
#define SMC_STOPCTRL_PSTOPO(x)                   (((uint8_t)(((uint8_t)(x))<<SMC_STOPCTRL_PSTOPO_SHIFT))&SMC_STOPCTRL_PSTOPO_MASK)
/* PMSTAT Bit Fields */
#define SMC_PMSTAT_PMSTAT_MASK                   0x7Fu
#define SMC_PMSTAT_PMSTAT_SHIFT                  0
#define SMC_PMSTAT_PMSTAT(x)                     (((uint8_t)(((uint8_t)(x))<<SMC_PMSTAT_PMSTAT_SHIFT))&SMC_PMSTAT_PMSTAT_MASK)

/*!
 * @}
 */ /* end of group SMC_Register_Masks */


/* SMC - Peripheral instance base addresses */
/** Peripheral SMC base pointer */
#define SMC_BASE_PTR                             ((SMC_MemMapPtr)0x4007E000u)
/** Array initializer of SMC peripheral base pointers */
#define SMC_BASE_PTRS                            { SMC_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- SMC - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SMC_Register_Accessor_Macros SMC - Register accessor macros
 * @{
 */


/* SMC - Register instance definitions */
/* SMC */
#define SMC_PMPROT                               SMC_PMPROT_REG(SMC_BASE_PTR)
#define SMC_PMCTRL                               SMC_PMCTRL_REG(SMC_BASE_PTR)
#define SMC_STOPCTRL                             SMC_STOPCTRL_REG(SMC_BASE_PTR)
#define SMC_PMSTAT                               SMC_PMSTAT_REG(SMC_BASE_PTR)

/*!
 * @}
 */ /* end of group SMC_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group SMC_Peripheral */


/* ----------------------------------------------------------------------------
   -- SPI
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Peripheral SPI
 * @{
 */

/** SPI - Peripheral register structure */
typedef struct SPI_MemMap {
  uint8_t C1;                                      /**< SPI control register 1, offset: 0x0 */
  uint8_t C2;                                      /**< SPI control register 2, offset: 0x1 */
  uint8_t BR;                                      /**< SPI baud rate register, offset: 0x2 */
  uint8_t S;                                       /**< SPI status register, offset: 0x3 */
  uint8_t RESERVED_0[1];
  uint8_t D;                                       /**< SPI data register, offset: 0x5 */
  uint8_t RESERVED_1[1];
  uint8_t M;                                       /**< SPI match register, offset: 0x7 */
} volatile *SPI_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SPI - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Register_Accessor_Macros SPI - Register accessor macros
 * @{
 */


/* SPI - Register accessors */
#define SPI_C1_REG(base)                         ((base)->C1)
#define SPI_C2_REG(base)                         ((base)->C2)
#define SPI_BR_REG(base)                         ((base)->BR)
#define SPI_S_REG(base)                          ((base)->S)
#define SPI_D_REG(base)                          ((base)->D)
#define SPI_M_REG(base)                          ((base)->M)

/*!
 * @}
 */ /* end of group SPI_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- SPI Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Register_Masks SPI Register Masks
 * @{
 */

/* C1 Bit Fields */
#define SPI_C1_LSBFE_MASK                        0x1u
#define SPI_C1_LSBFE_SHIFT                       0
#define SPI_C1_SSOE_MASK                         0x2u
#define SPI_C1_SSOE_SHIFT                        1
#define SPI_C1_CPHA_MASK                         0x4u
#define SPI_C1_CPHA_SHIFT                        2
#define SPI_C1_CPOL_MASK                         0x8u
#define SPI_C1_CPOL_SHIFT                        3
#define SPI_C1_MSTR_MASK                         0x10u
#define SPI_C1_MSTR_SHIFT                        4
#define SPI_C1_SPTIE_MASK                        0x20u
#define SPI_C1_SPTIE_SHIFT                       5
#define SPI_C1_SPE_MASK                          0x40u
#define SPI_C1_SPE_SHIFT                         6
#define SPI_C1_SPIE_MASK                         0x80u
#define SPI_C1_SPIE_SHIFT                        7
/* C2 Bit Fields */
#define SPI_C2_SPC0_MASK                         0x1u
#define SPI_C2_SPC0_SHIFT                        0
#define SPI_C2_SPISWAI_MASK                      0x2u
#define SPI_C2_SPISWAI_SHIFT                     1
#define SPI_C2_BIDIROE_MASK                      0x8u
#define SPI_C2_BIDIROE_SHIFT                     3
#define SPI_C2_MODFEN_MASK                       0x10u
#define SPI_C2_MODFEN_SHIFT                      4
#define SPI_C2_SPMIE_MASK                        0x80u
#define SPI_C2_SPMIE_SHIFT                       7
/* BR Bit Fields */
#define SPI_BR_SPR_MASK                          0xFu
#define SPI_BR_SPR_SHIFT                         0
#define SPI_BR_SPR(x)                            (((uint8_t)(((uint8_t)(x))<<SPI_BR_SPR_SHIFT))&SPI_BR_SPR_MASK)
#define SPI_BR_SPPR_MASK                         0x70u
#define SPI_BR_SPPR_SHIFT                        4
#define SPI_BR_SPPR(x)                           (((uint8_t)(((uint8_t)(x))<<SPI_BR_SPPR_SHIFT))&SPI_BR_SPPR_MASK)
/* S Bit Fields */
#define SPI_S_MODF_MASK                          0x10u
#define SPI_S_MODF_SHIFT                         4
#define SPI_S_SPTEF_MASK                         0x20u
#define SPI_S_SPTEF_SHIFT                        5
#define SPI_S_SPMF_MASK                          0x40u
#define SPI_S_SPMF_SHIFT                         6
#define SPI_S_SPRF_MASK                          0x80u
#define SPI_S_SPRF_SHIFT                         7
/* D Bit Fields */
#define SPI_D_Bits_MASK                          0xFFu
#define SPI_D_Bits_SHIFT                         0
#define SPI_D_Bits(x)                            (((uint8_t)(((uint8_t)(x))<<SPI_D_Bits_SHIFT))&SPI_D_Bits_MASK)
/* M Bit Fields */
#define SPI_M_Bits_MASK                          0xFFu
#define SPI_M_Bits_SHIFT                         0
#define SPI_M_Bits(x)                            (((uint8_t)(((uint8_t)(x))<<SPI_M_Bits_SHIFT))&SPI_M_Bits_MASK)

/*!
 * @}
 */ /* end of group SPI_Register_Masks */


/* SPI - Peripheral instance base addresses */
/** Peripheral SPI0 base pointer */
#define SPI0_BASE_PTR                            ((SPI_MemMapPtr)0x40076000u)
/** Array initializer of SPI peripheral base pointers */
#define SPI_BASE_PTRS                            { SPI0_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- SPI - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Register_Accessor_Macros SPI - Register accessor macros
 * @{
 */


/* SPI - Register instance definitions */
/* SPI0 */
#define SPI0_C1                                  SPI_C1_REG(SPI0_BASE_PTR)
#define SPI0_C2                                  SPI_C2_REG(SPI0_BASE_PTR)
#define SPI0_BR                                  SPI_BR_REG(SPI0_BASE_PTR)
#define SPI0_S                                   SPI_S_REG(SPI0_BASE_PTR)
#define SPI0_D                                   SPI_D_REG(SPI0_BASE_PTR)
#define SPI0_M                                   SPI_M_REG(SPI0_BASE_PTR)

/*!
 * @}
 */ /* end of group SPI_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group SPI_Peripheral */


/* ----------------------------------------------------------------------------
   -- SysTick
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SysTick_Peripheral SysTick
 * @{
 */

/** SysTick - Peripheral register structure */
typedef struct SysTick_MemMap {
  uint32_t CSR;                                    /**< SysTick Control and Status Register, offset: 0x0 */
  uint32_t RVR;                                    /**< SysTick Reload Value Register, offset: 0x4 */
  uint32_t CVR;                                    /**< SysTick Current Value Register, offset: 0x8 */
  uint32_t CALIB;                                  /**< SysTick Calibration Value Register, offset: 0xC */
} volatile *SysTick_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SysTick - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SysTick_Register_Accessor_Macros SysTick - Register accessor macros
 * @{
 */


/* SysTick - Register accessors */
#define SysTick_CSR_REG(base)                    ((base)->CSR)
#define SysTick_RVR_REG(base)                    ((base)->RVR)
#define SysTick_CVR_REG(base)                    ((base)->CVR)
#define SysTick_CALIB_REG(base)                  ((base)->CALIB)

/*!
 * @}
 */ /* end of group SysTick_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- SysTick Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SysTick_Register_Masks SysTick Register Masks
 * @{
 */

/* CSR Bit Fields */
#define SysTick_CSR_ENABLE_MASK                  0x1u
#define SysTick_CSR_ENABLE_SHIFT                 0
#define SysTick_CSR_TICKINT_MASK                 0x2u
#define SysTick_CSR_TICKINT_SHIFT                1
#define SysTick_CSR_CLKSOURCE_MASK               0x4u
#define SysTick_CSR_CLKSOURCE_SHIFT              2
#define SysTick_CSR_COUNTFLAG_MASK               0x10000u
#define SysTick_CSR_COUNTFLAG_SHIFT              16
/* RVR Bit Fields */
#define SysTick_RVR_RELOAD_MASK                  0xFFFFFFu
#define SysTick_RVR_RELOAD_SHIFT                 0
#define SysTick_RVR_RELOAD(x)                    (((uint32_t)(((uint32_t)(x))<<SysTick_RVR_RELOAD_SHIFT))&SysTick_RVR_RELOAD_MASK)
/* CVR Bit Fields */
#define SysTick_CVR_CURRENT_MASK                 0xFFFFFFu
#define SysTick_CVR_CURRENT_SHIFT                0
#define SysTick_CVR_CURRENT(x)                   (((uint32_t)(((uint32_t)(x))<<SysTick_CVR_CURRENT_SHIFT))&SysTick_CVR_CURRENT_MASK)
/* CALIB Bit Fields */
#define SysTick_CALIB_TENMS_MASK                 0xFFFFFFu
#define SysTick_CALIB_TENMS_SHIFT                0
#define SysTick_CALIB_TENMS(x)                   (((uint32_t)(((uint32_t)(x))<<SysTick_CALIB_TENMS_SHIFT))&SysTick_CALIB_TENMS_MASK)
#define SysTick_CALIB_SKEW_MASK                  0x40000000u
#define SysTick_CALIB_SKEW_SHIFT                 30
#define SysTick_CALIB_NOREF_MASK                 0x80000000u
#define SysTick_CALIB_NOREF_SHIFT                31

/*!
 * @}
 */ /* end of group SysTick_Register_Masks */


/* SysTick - Peripheral instance base addresses */
/** Peripheral SysTick base pointer */
#define SysTick_BASE_PTR                         ((SysTick_MemMapPtr)0xE000E010u)
/** Array initializer of SysTick peripheral base pointers */
#define SysTick_BASE_PTRS                        { SysTick_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- SysTick - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SysTick_Register_Accessor_Macros SysTick - Register accessor macros
 * @{
 */


/* SysTick - Register instance definitions */
/* SysTick */
#define SYST_CSR                                 SysTick_CSR_REG(SysTick_BASE_PTR)
#define SYST_RVR                                 SysTick_RVR_REG(SysTick_BASE_PTR)
#define SYST_CVR                                 SysTick_CVR_REG(SysTick_BASE_PTR)
#define SYST_CALIB                               SysTick_CALIB_REG(SysTick_BASE_PTR)

/*!
 * @}
 */ /* end of group SysTick_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group SysTick_Peripheral */


/* ----------------------------------------------------------------------------
   -- TPM
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup TPM_Peripheral TPM
 * @{
 */

/** TPM - Peripheral register structure */
typedef struct TPM_MemMap {
  uint32_t SC;                                     /**< Status and Control, offset: 0x0 */
  uint32_t CNT;                                    /**< Counter, offset: 0x4 */
  uint32_t MOD;                                    /**< Modulo, offset: 0x8 */
  struct {                                         /* offset: 0xC, array step: 0x8 */
    uint32_t CnSC;                                   /**< Channel (n) Status and Control, array offset: 0xC, array step: 0x8 */
    uint32_t CnV;                                    /**< Channel (n) Value, array offset: 0x10, array step: 0x8 */
  } CONTROLS[2];
  uint8_t RESERVED_0[52];
  uint32_t STATUS;                                 /**< Capture and Compare Status, offset: 0x50 */
  uint8_t RESERVED_1[48];
  uint32_t CONF;                                   /**< Configuration, offset: 0x84 */
} volatile *TPM_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- TPM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup TPM_Register_Accessor_Macros TPM - Register accessor macros
 * @{
 */


/* TPM - Register accessors */
#define TPM_SC_REG(base)                         ((base)->SC)
#define TPM_CNT_REG(base)                        ((base)->CNT)
#define TPM_MOD_REG(base)                        ((base)->MOD)
#define TPM_CnSC_REG(base,index)                 ((base)->CONTROLS[index].CnSC)
#define TPM_CnV_REG(base,index)                  ((base)->CONTROLS[index].CnV)
#define TPM_STATUS_REG(base)                     ((base)->STATUS)
#define TPM_CONF_REG(base)                       ((base)->CONF)

/*!
 * @}
 */ /* end of group TPM_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- TPM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup TPM_Register_Masks TPM Register Masks
 * @{
 */

/* SC Bit Fields */
#define TPM_SC_PS_MASK                           0x7u
#define TPM_SC_PS_SHIFT                          0
#define TPM_SC_PS(x)                             (((uint32_t)(((uint32_t)(x))<<TPM_SC_PS_SHIFT))&TPM_SC_PS_MASK)
#define TPM_SC_CMOD_MASK                         0x18u
#define TPM_SC_CMOD_SHIFT                        3
#define TPM_SC_CMOD(x)                           (((uint32_t)(((uint32_t)(x))<<TPM_SC_CMOD_SHIFT))&TPM_SC_CMOD_MASK)
#define TPM_SC_CPWMS_MASK                        0x20u
#define TPM_SC_CPWMS_SHIFT                       5
#define TPM_SC_TOIE_MASK                         0x40u
#define TPM_SC_TOIE_SHIFT                        6
#define TPM_SC_TOF_MASK                          0x80u
#define TPM_SC_TOF_SHIFT                         7
#define TPM_SC_DMA_MASK                          0x100u
#define TPM_SC_DMA_SHIFT                         8
/* CNT Bit Fields */
#define TPM_CNT_COUNT_MASK                       0xFFFFu
#define TPM_CNT_COUNT_SHIFT                      0
#define TPM_CNT_COUNT(x)                         (((uint32_t)(((uint32_t)(x))<<TPM_CNT_COUNT_SHIFT))&TPM_CNT_COUNT_MASK)
/* MOD Bit Fields */
#define TPM_MOD_MOD_MASK                         0xFFFFu
#define TPM_MOD_MOD_SHIFT                        0
#define TPM_MOD_MOD(x)                           (((uint32_t)(((uint32_t)(x))<<TPM_MOD_MOD_SHIFT))&TPM_MOD_MOD_MASK)
/* CnSC Bit Fields */
#define TPM_CnSC_DMA_MASK                        0x1u
#define TPM_CnSC_DMA_SHIFT                       0
#define TPM_CnSC_ELSA_MASK                       0x4u
#define TPM_CnSC_ELSA_SHIFT                      2
#define TPM_CnSC_ELSB_MASK                       0x8u
#define TPM_CnSC_ELSB_SHIFT                      3
#define TPM_CnSC_MSA_MASK                        0x10u
#define TPM_CnSC_MSA_SHIFT                       4
#define TPM_CnSC_MSB_MASK                        0x20u
#define TPM_CnSC_MSB_SHIFT                       5
#define TPM_CnSC_CHIE_MASK                       0x40u
#define TPM_CnSC_CHIE_SHIFT                      6
#define TPM_CnSC_CHF_MASK                        0x80u
#define TPM_CnSC_CHF_SHIFT                       7
/* CnV Bit Fields */
#define TPM_CnV_VAL_MASK                         0xFFFFu
#define TPM_CnV_VAL_SHIFT                        0
#define TPM_CnV_VAL(x)                           (((uint32_t)(((uint32_t)(x))<<TPM_CnV_VAL_SHIFT))&TPM_CnV_VAL_MASK)
/* STATUS Bit Fields */
#define TPM_STATUS_CH0F_MASK                     0x1u
#define TPM_STATUS_CH0F_SHIFT                    0
#define TPM_STATUS_CH1F_MASK                     0x2u
#define TPM_STATUS_CH1F_SHIFT                    1
#define TPM_STATUS_TOF_MASK                      0x100u
#define TPM_STATUS_TOF_SHIFT                     8
/* CONF Bit Fields */
#define TPM_CONF_DOZEEN_MASK                     0x20u
#define TPM_CONF_DOZEEN_SHIFT                    5
#define TPM_CONF_DBGMODE_MASK                    0xC0u
#define TPM_CONF_DBGMODE_SHIFT                   6
#define TPM_CONF_DBGMODE(x)                      (((uint32_t)(((uint32_t)(x))<<TPM_CONF_DBGMODE_SHIFT))&TPM_CONF_DBGMODE_MASK)
#define TPM_CONF_GTBEEN_MASK                     0x200u
#define TPM_CONF_GTBEEN_SHIFT                    9
#define TPM_CONF_CSOT_MASK                       0x10000u
#define TPM_CONF_CSOT_SHIFT                      16
#define TPM_CONF_CSOO_MASK                       0x20000u
#define TPM_CONF_CSOO_SHIFT                      17
#define TPM_CONF_CROT_MASK                       0x40000u
#define TPM_CONF_CROT_SHIFT                      18
#define TPM_CONF_TRGSEL_MASK                     0xF000000u
#define TPM_CONF_TRGSEL_SHIFT                    24
#define TPM_CONF_TRGSEL(x)                       (((uint32_t)(((uint32_t)(x))<<TPM_CONF_TRGSEL_SHIFT))&TPM_CONF_TRGSEL_MASK)

/*!
 * @}
 */ /* end of group TPM_Register_Masks */


/* TPM - Peripheral instance base addresses */
/** Peripheral TPM0 base pointer */
#define TPM0_BASE_PTR                            ((TPM_MemMapPtr)0x40038000u)
/** Peripheral TPM1 base pointer */
#define TPM1_BASE_PTR                            ((TPM_MemMapPtr)0x40039000u)
/** Array initializer of TPM peripheral base pointers */
#define TPM_BASE_PTRS                            { TPM0_BASE_PTR, TPM1_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- TPM - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup TPM_Register_Accessor_Macros TPM - Register accessor macros
 * @{
 */


/* TPM - Register instance definitions */
/* TPM0 */
#define TPM0_SC                                  TPM_SC_REG(TPM0_BASE_PTR)
#define TPM0_CNT                                 TPM_CNT_REG(TPM0_BASE_PTR)
#define TPM0_MOD                                 TPM_MOD_REG(TPM0_BASE_PTR)
#define TPM0_C0SC                                TPM_CnSC_REG(TPM0_BASE_PTR,0)
#define TPM0_C0V                                 TPM_CnV_REG(TPM0_BASE_PTR,0)
#define TPM0_C1SC                                TPM_CnSC_REG(TPM0_BASE_PTR,1)
#define TPM0_C1V                                 TPM_CnV_REG(TPM0_BASE_PTR,1)
#define TPM0_STATUS                              TPM_STATUS_REG(TPM0_BASE_PTR)
#define TPM0_CONF                                TPM_CONF_REG(TPM0_BASE_PTR)
/* TPM1 */
#define TPM1_SC                                  TPM_SC_REG(TPM1_BASE_PTR)
#define TPM1_CNT                                 TPM_CNT_REG(TPM1_BASE_PTR)
#define TPM1_MOD                                 TPM_MOD_REG(TPM1_BASE_PTR)
#define TPM1_C0SC                                TPM_CnSC_REG(TPM1_BASE_PTR,0)
#define TPM1_C0V                                 TPM_CnV_REG(TPM1_BASE_PTR,0)
#define TPM1_C1SC                                TPM_CnSC_REG(TPM1_BASE_PTR,1)
#define TPM1_C1V                                 TPM_CnV_REG(TPM1_BASE_PTR,1)
#define TPM1_STATUS                              TPM_STATUS_REG(TPM1_BASE_PTR)
#define TPM1_CONF                                TPM_CONF_REG(TPM1_BASE_PTR)

/* TPM - Register array accessors */
#define TPM0_CnSC(index)                         TPM_CnSC_REG(TPM0_BASE_PTR,index)
#define TPM1_CnSC(index)                         TPM_CnSC_REG(TPM1_BASE_PTR,index)
#define TPM0_CnV(index)                          TPM_CnV_REG(TPM0_BASE_PTR,index)
#define TPM1_CnV(index)                          TPM_CnV_REG(TPM1_BASE_PTR,index)

/*!
 * @}
 */ /* end of group TPM_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group TPM_Peripheral */


/* ----------------------------------------------------------------------------
   -- UART0
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UART0_Peripheral UART0
 * @{
 */

/** UART0 - Peripheral register structure */
typedef struct UART0_MemMap {
  uint8_t BDH;                                     /**< UART Baud Rate Register High, offset: 0x0 */
  uint8_t BDL;                                     /**< UART Baud Rate Register Low, offset: 0x1 */
  uint8_t C1;                                      /**< UART Control Register 1, offset: 0x2 */
  uint8_t C2;                                      /**< UART Control Register 2, offset: 0x3 */
  uint8_t S1;                                      /**< UART Status Register 1, offset: 0x4 */
  uint8_t S2;                                      /**< UART Status Register 2, offset: 0x5 */
  uint8_t C3;                                      /**< UART Control Register 3, offset: 0x6 */
  uint8_t D;                                       /**< UART Data Register, offset: 0x7 */
  uint8_t MA1;                                     /**< UART Match Address Registers 1, offset: 0x8 */
  uint8_t MA2;                                     /**< UART Match Address Registers 2, offset: 0x9 */
  uint8_t C4;                                      /**< UART Control Register 4, offset: 0xA */
  uint8_t C5;                                      /**< UART Control Register 5, offset: 0xB */
} volatile *UART0_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- UART0 - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UART0_Register_Accessor_Macros UART0 - Register accessor macros
 * @{
 */


/* UART0 - Register accessors */
#define UART0_BDH_REG(base)                      ((base)->BDH)
#define UART0_BDL_REG(base)                      ((base)->BDL)
#define UART0_C1_REG(base)                       ((base)->C1)
#define UART0_C2_REG(base)                       ((base)->C2)
#define UART0_S1_REG(base)                       ((base)->S1)
#define UART0_S2_REG(base)                       ((base)->S2)
#define UART0_C3_REG(base)                       ((base)->C3)
#define UART0_D_REG(base)                        ((base)->D)
#define UART0_MA1_REG(base)                      ((base)->MA1)
#define UART0_MA2_REG(base)                      ((base)->MA2)
#define UART0_C4_REG(base)                       ((base)->C4)
#define UART0_C5_REG(base)                       ((base)->C5)

/*!
 * @}
 */ /* end of group UART0_Register_Accessor_Macros */


/* ----------------------------------------------------------------------------
   -- UART0 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UART0_Register_Masks UART0 Register Masks
 * @{
 */

/* BDH Bit Fields */
#define UART0_BDH_SBR_MASK                       0x1Fu
#define UART0_BDH_SBR_SHIFT                      0
#define UART0_BDH_SBR(x)                         (((uint8_t)(((uint8_t)(x))<<UART0_BDH_SBR_SHIFT))&UART0_BDH_SBR_MASK)
#define UART0_BDH_SBNS_MASK                      0x20u
#define UART0_BDH_SBNS_SHIFT                     5
#define UART0_BDH_RXEDGIE_MASK                   0x40u
#define UART0_BDH_RXEDGIE_SHIFT                  6
#define UART0_BDH_LBKDIE_MASK                    0x80u
#define UART0_BDH_LBKDIE_SHIFT                   7
/* BDL Bit Fields */
#define UART0_BDL_SBR_MASK                       0xFFu
#define UART0_BDL_SBR_SHIFT                      0
#define UART0_BDL_SBR(x)                         (((uint8_t)(((uint8_t)(x))<<UART0_BDL_SBR_SHIFT))&UART0_BDL_SBR_MASK)
/* C1 Bit Fields */
#define UART0_C1_PT_MASK                         0x1u
#define UART0_C1_PT_SHIFT                        0
#define UART0_C1_PE_MASK                         0x2u
#define UART0_C1_PE_SHIFT                        1
#define UART0_C1_ILT_MASK                        0x4u
#define UART0_C1_ILT_SHIFT                       2
#define UART0_C1_WAKE_MASK                       0x8u
#define UART0_C1_WAKE_SHIFT                      3
#define UART0_C1_M_MASK                          0x10u
#define UART0_C1_M_SHIFT                         4
#define UART0_C1_RSRC_MASK                       0x20u
#define UART0_C1_RSRC_SHIFT                      5
#define UART0_C1_DOZEEN_MASK                     0x40u
#define UART0_C1_DOZEEN_SHIFT                    6
#define UART0_C1_LOOPS_MASK                      0x80u
#define UART0_C1_LOOPS_SHIFT                     7
/* C2 Bit Fields */
#define UART0_C2_SBK_MASK                        0x1u
#define UART0_C2_SBK_SHIFT                       0
#define UART0_C2_RWU_MASK                        0x2u
#define UART0_C2_RWU_SHIFT                       1
#define UART0_C2_RE_MASK                         0x4u
#define UART0_C2_RE_SHIFT                        2
#define UART0_C2_TE_MASK                         0x8u
#define UART0_C2_TE_SHIFT                        3
#define UART0_C2_ILIE_MASK                       0x10u
#define UART0_C2_ILIE_SHIFT                      4
#define UART0_C2_RIE_MASK                        0x20u
#define UART0_C2_RIE_SHIFT                       5
#define UART0_C2_TCIE_MASK                       0x40u
#define UART0_C2_TCIE_SHIFT                      6
#define UART0_C2_TIE_MASK                        0x80u
#define UART0_C2_TIE_SHIFT                       7
/* S1 Bit Fields */
#define UART0_S1_PF_MASK                         0x1u
#define UART0_S1_PF_SHIFT                        0
#define UART0_S1_FE_MASK                         0x2u
#define UART0_S1_FE_SHIFT                        1
#define UART0_S1_NF_MASK                         0x4u
#define UART0_S1_NF_SHIFT                        2
#define UART0_S1_OR_MASK                         0x8u
#define UART0_S1_OR_SHIFT                        3
#define UART0_S1_IDLE_MASK                       0x10u
#define UART0_S1_IDLE_SHIFT                      4
#define UART0_S1_RDRF_MASK                       0x20u
#define UART0_S1_RDRF_SHIFT                      5
#define UART0_S1_TC_MASK                         0x40u
#define UART0_S1_TC_SHIFT                        6
#define UART0_S1_TDRE_MASK                       0x80u
#define UART0_S1_TDRE_SHIFT                      7
/* S2 Bit Fields */
#define UART0_S2_RAF_MASK                        0x1u
#define UART0_S2_RAF_SHIFT                       0
#define UART0_S2_LBKDE_MASK                      0x2u
#define UART0_S2_LBKDE_SHIFT                     1
#define UART0_S2_BRK13_MASK                      0x4u
#define UART0_S2_BRK13_SHIFT                     2
#define UART0_S2_RWUID_MASK                      0x8u
#define UART0_S2_RWUID_SHIFT                     3
#define UART0_S2_RXINV_MASK                      0x10u
#define UART0_S2_RXINV_SHIFT                     4
#define UART0_S2_MSBF_MASK                       0x20u
#define UART0_S2_MSBF_SHIFT                      5
#define UART0_S2_RXEDGIF_MASK                    0x40u
#define UART0_S2_RXEDGIF_SHIFT                   6
#define UART0_S2_LBKDIF_MASK                     0x80u
#define UART0_S2_LBKDIF_SHIFT                    7
/* C3 Bit Fields */
#define UART0_C3_PEIE_MASK                       0x1u
#define UART0_C3_PEIE_SHIFT                      0
#define UART0_C3_FEIE_MASK                       0x2u
#define UART0_C3_FEIE_SHIFT                      1
#define UART0_C3_NEIE_MASK                       0x4u
#define UART0_C3_NEIE_SHIFT                      2
#define UART0_C3_ORIE_MASK                       0x8u
#define UART0_C3_ORIE_SHIFT                      3
#define UART0_C3_TXINV_MASK                      0x10u
#define UART0_C3_TXINV_SHIFT                     4
#define UART0_C3_TXDIR_MASK                      0x20u
#define UART0_C3_TXDIR_SHIFT                     5
#define UART0_C3_R9T8_MASK                       0x40u
#define UART0_C3_R9T8_SHIFT                      6
#define UART0_C3_R8T9_MASK                       0x80u
#define UART0_C3_R8T9_SHIFT                      7
/* D Bit Fields */
#define UART0_D_R0T0_MASK                        0x1u
#define UART0_D_R0T0_SHIFT                       0
#define UART0_D_R1T1_MASK                        0x2u
#define UART0_D_R1T1_SHIFT                       1
#define UART0_D_R2T2_MASK                        0x4u
#define UART0_D_R2T2_SHIFT                       2
#define UART0_D_R3T3_MASK                        0x8u
#define UART0_D_R3T3_SHIFT                       3
#define UART0_D_R4T4_MASK                        0x10u
#define UART0_D_R4T4_SHIFT                       4
#define UART0_D_R5T5_MASK                        0x20u
#define UART0_D_R5T5_SHIFT                       5
#define UART0_D_R6T6_MASK                        0x40u
#define UART0_D_R6T6_SHIFT                       6
#define UART0_D_R7T7_MASK                        0x80u
#define UART0_D_R7T7_SHIFT                       7
/* MA1 Bit Fields */
#define UART0_MA1_MA_MASK                        0xFFu
#define UART0_MA1_MA_SHIFT                       0
#define UART0_MA1_MA(x)                          (((uint8_t)(((uint8_t)(x))<<UART0_MA1_MA_SHIFT))&UART0_MA1_MA_MASK)
/* MA2 Bit Fields */
#define UART0_MA2_MA_MASK                        0xFFu
#define UART0_MA2_MA_SHIFT                       0
#define UART0_MA2_MA(x)                          (((uint8_t)(((uint8_t)(x))<<UART0_MA2_MA_SHIFT))&UART0_MA2_MA_MASK)
/* C4 Bit Fields */
#define UART0_C4_OSR_MASK                        0x1Fu
#define UART0_C4_OSR_SHIFT                       0
#define UART0_C4_OSR(x)                          (((uint8_t)(((uint8_t)(x))<<UART0_C4_OSR_SHIFT))&UART0_C4_OSR_MASK)
#define UART0_C4_M10_MASK                        0x20u
#define UART0_C4_M10_SHIFT                       5
#define UART0_C4_MAEN2_MASK                      0x40u
#define UART0_C4_MAEN2_SHIFT                     6
#define UART0_C4_MAEN1_MASK                      0x80u
#define UART0_C4_MAEN1_SHIFT                     7
/* C5 Bit Fields */
#define UART0_C5_RESYNCDIS_MASK                  0x1u
#define UART0_C5_RESYNCDIS_SHIFT                 0
#define UART0_C5_BOTHEDGE_MASK                   0x2u
#define UART0_C5_BOTHEDGE_SHIFT                  1

/*!
 * @}
 */ /* end of group UART0_Register_Masks */


/* UART0 - Peripheral instance base addresses */
/** Peripheral UART0 base pointer */
#define UART0_BASE_PTR                           ((UART0_MemMapPtr)0x4006A000u)
/** Array initializer of UART0 peripheral base pointers */
#define UART0_BASE_PTRS                          { UART0_BASE_PTR }

/* ----------------------------------------------------------------------------
   -- UART0 - Register accessor macros
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UART0_Register_Accessor_Macros UART0 - Register accessor macros
 * @{
 */


/* UART0 - Register instance definitions */
/* UART0 */
#define UART0_BDH                                UART0_BDH_REG(UART0_BASE_PTR)
#define UART0_BDL                                UART0_BDL_REG(UART0_BASE_PTR)
#define UART0_C1                                 UART0_C1_REG(UART0_BASE_PTR)
#define UART0_C2                                 UART0_C2_REG(UART0_BASE_PTR)
#define UART0_S1                                 UART0_S1_REG(UART0_BASE_PTR)
#define UART0_S2                                 UART0_S2_REG(UART0_BASE_PTR)
#define UART0_C3                                 UART0_C3_REG(UART0_BASE_PTR)
#define UART0_D                                  UART0_D_REG(UART0_BASE_PTR)
#define UART0_MA1                                UART0_MA1_REG(UART0_BASE_PTR)
#define UART0_MA2                                UART0_MA2_REG(UART0_BASE_PTR)
#define UART0_C4                                 UART0_C4_REG(UART0_BASE_PTR)
#define UART0_C5                                 UART0_C5_REG(UART0_BASE_PTR)

/*!
 * @}
 */ /* end of group UART0_Register_Accessor_Macros */


/*!
 * @}
 */ /* end of group UART0_Peripheral */


/*
** End of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #pragma pop
#elif defined(__CWCC__)
  #pragma pop
#elif defined(__GNUC__)
  /* leave anonymous unions enabled */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=default
#else
  #error Not supported compiler type
#endif

/*!
 * @}
 */ /* end of group Peripheral_defines */


/* ----------------------------------------------------------------------------
   -- Backward Compatibility
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Backward_Compatibility_Symbols Backward Compatibility
 * @{
 */

#define INT_LPTimer                              INT_LPTMR0
#define FPTA_BASE_PTR                            FGPIOA_BASE_PTR
#define FPTA                                     FGPIOA
#define FPTB_BASE_PTR                            FGPIOB_BASE_PTR
#define FPTB                                     FGPIOB
#define PTA_BASE_PTR                             GPIOA_BASE_PTR
#define PTA                                      GPIOA
#define PTB_BASE_PTR                             GPIOB_BASE_PTR
#define PTB                                      GPIOB
#define I2C_FLT_STOPIE_MASK                      This_symbol_has_been_deprecated
#define I2C_FLT_STOPIE_SHIFT                     This_symbol_has_been_deprecated
#define MCG_S_LOLS_MASK                          MCG_S_LOLS0_MASK
#define MCG_S_LOLS_SHIFT                         MCG_S_LOLS0_SHIFT
#define NVIC_ISER_SETENA_MASK                    0xFFFFFFFFu
#define NVIC_ISER_SETENA_SHIFT                   0
#define NVIC_ISER_SETENA(x)                      (((uint32_t)(((uint32_t)(x))<<NVIC_ISER_SETENA_SHIFT))&NVIC_ISER_SETENA_MASK)
#define NVIC_ICER_CLRENA_MASK                    0xFFFFFFFFu
#define NVIC_ICER_CLRENA_SHIFT                   0
#define NVIC_ICER_CLRENA(x)                      (((uint32_t)(((uint32_t)(x))<<NVIC_ICER_CLRENA_SHIFT))&NVIC_ICER_CLRENA_MASK)
#define NVIC_ISPR_SETPEND_MASK                   0xFFFFFFFFu
#define NVIC_ISPR_SETPEND_SHIFT                  0
#define NVIC_ISPR_SETPEND(x)                     (((uint32_t)(((uint32_t)(x))<<NVIC_ISPR_SETPEND_SHIFT))&NVIC_ISPR_SETPEND_MASK)
#define NVIC_ICPR_CLRPEND_MASK                   0xFFFFFFFFu
#define NVIC_ICPR_CLRPEND_SHIFT                  0
#define NVIC_ICPR_CLRPEND(x)                     (((uint32_t)(((uint32_t)(x))<<NVIC_ICPR_CLRPEND_SHIFT))&NVIC_ICPR_CLRPEND_MASK)

/*!
 * @}
 */ /* end of group Backward_Compatibility_Symbols */


#else /* #if !defined(MCU_MKL02Z4) */
  /* There is already included the same memory map. Check if it is compatible (has the same major version) */
  #if (MCU_MEM_MAP_VERSION != 0x0100u)
    #if (!defined(MCU_MEM_MAP_SUPPRESS_VERSION_WARNING))
      #warning There are included two not compatible versions of memory maps. Please check possible differences.
    #endif /* (!defined(MCU_MEM_MAP_SUPPRESS_VERSION_WARNING)) */
  #endif /* (MCU_MEM_MAP_VERSION != 0x0100u) */
#endif  /* #if !defined(MCU_MKL02Z4) */

/* MKL02Z4.h, eof. */
