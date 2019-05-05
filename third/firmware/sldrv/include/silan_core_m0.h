/*
 *-----------------------------------------------------------------------------
 * The confidential and proprietary information contained in this file may
 * only be used by a person authorised under and to the extent permitted
 * by a subsisting licensing agreement from ARM Limited.
 *
 *            (C) COPYRIGHT 2010-2011 ARM Limited.
 *                ALL RIGHTS RESERVED
 *
 * This entire notice must be reproduced on all copies of this file
 * and copies of this file may only be made by a person if such person is
 * permitted to do so under the terms of a subsisting license agreement
 * from ARM Limited.
 *
 *      SVN Information
 *
 *      Checked In          : $Date: 2011-03-22 22:03:22 +0000 (Tue, 22 Mar 2011) $
 *
 *      Revision            : $Revision: 165549 $
 *
 *      Release Information : BP200-r0p0-00rel0
 *-----------------------------------------------------------------------------
 */
 /**************************************************************************//**
 * @file     CMSDK_CM0.h
 * @brief    CMSIS compatible Cortex-M0 Core Peripheral Access Layer Header File for
 *           ARM CMSDK
 * @version  $State:$
 * @date     $Date: 2011-03-22 22:03:22 +0000 (Tue, 22 Mar 2011) $
 *
 ******************************************************************************/


#ifndef _SILAN_APM0_
#define _SILAN_APM0_                               /**< Symbol preventing repeated inclusion */

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup CMSDK_Definitions CMSDK Definitions
  This file defines all structures and symbols for CMSDK:
    - Registers and bitfields
    - peripheral base address
    - peripheral ID
    - PIO definitions
  @{
*/


/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup CMSDK_CMSIS CMSDK CMSIS Definitions
  Configuration of the Cortex-M0 Processor and Core Peripherals
  @{
*/
#include "silan_irq.h"
/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn           = -14,    /*!< 2 Non Maskable Interrupt                           */
  HardFault_IRQn                = -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                   */
  SVCall_IRQn                   = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                     */
  PendSV_IRQn                   = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                     */
  SysTick_IRQn                  = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                 */

/******  CMSDK Specific Interrupt Numbers *******************************************************/
	

} IRQn_Type;



/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */

/*@}*/ /* end of group CMSDK_CMSIS */


#include "core_cm0.h"                       /* Cortex-M0 processor and core peripherals           */
//#include "system_CMSDK.h"                  /* System Header                                      */

/******************************************************************************/
/*                Device Specific Peripheral Registers structures             */
/******************************************************************************/

#if defined ( __CC_ARM   )||  defined ( __GNUC__ )
#pragma anon_unions
#endif


#ifdef __cplusplus
}
#endif

#endif  /* _SILAN_APM0_ */

