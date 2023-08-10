/**************************************************************************//**
 * @file     system_CM3DS_MPS2.c
 * @brief    CMSIS Device System Source File for
 *           CM3DS_MPS2 M3 Device
 * @version  V3.02
 * @date     15. November 2013
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2011-2013 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup pangu01_system
  * @{
  */


#include "pt32z192xx.h"

void Wait_ClockReady(void)
{
  volatile int delay;

    for (delay=0;delay<200;delay++);
}

/** @addtogroup pangu01_System_Private_Defines
  * @{
  */
//<<< Use Configuration Wizard in Context Menu >>>

/*--------------------- Clock Configuration ----------------------------------
//
//    <h> High Speed Internal RC Oscillator Configuration (24M)
//      <q0>    IOSC_24M Enable
//                <i> Default = ENABLE
//    </h>
//
//    <h> High Speed External Crystal Oscillator Configuration (4~25M)
//      <q1>    EOSC_20M Enable
//                <i> Default = DISABLE
//    </h>
//
//    <h> Low Speed Internal RC Oscillator Configuration (32.768K)
//      <q2>    IOSC_32K Enable
//                <i> Default = ENABLE
//    </h>
//
//    <h> Frequency Double Clock Configuration
//      <e3>    FD_CLK Enable
//                <i> Default  = DISABLE
//        <o4>  Frequency Double Clock Source
//                <0=> Internal RC_24M
//                <1=> External Crystal
//                <2=> Internal RC_32K
//                <i> Default Frequency Double clock source = Internal RC
//                <i> Frequency Double Clock = Source clock * 2
//      </e>
//    </h>
//    <h> SystemCoreClock Configuration (HCLK)
//      <o5>    SystemCoreClock Source
//                <0=> Internal RC_24M
//                <1=> External Crystal
//                <2=> Frequency Double Clock
//                <3=> Internal RC_32K
//                <i> Default SystemCoreClock source = Internal RC_24M
//      <o6>    SystemCoreClock Source Divider
//                <0=> 1
//                <1=> 2
//                <2=> 4
//                <3=> 8
//                <4=> 16
//                <5=> 32
//                <i> Default SystemCoreClock source divider = 1
//      <o7>    PCLK Divider
//                <0=> 1
//                <1=> 2
//                <2=> 4
//                <3=> 8
//                <4=> 16
//                <5=> 32
//                <i> Default PCLK divider = 1
//    </h>
//
//    <h> FLASH Configuration
//      <o8>   Wait state
//                <0=> 0 WS
//                <1=> 1 WS
//                <2=> 2 WS
//                <i> 0 WS:  1 KHz <= HCLK <= 20 MHz
//                <i> 1 WS: 20 MHz <  HCLK <= 40 MHz
//                <i> 2 WS: 40 MHz <  HCLK <= 60 MHz
//    </h>
*/

#define IOSC_24M_ENABLE               (1)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define EOSC_20M_ENABLE               (1)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define IOSC_32K_ENABLE               (0)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define FD_CLK_ENABLE                 (1)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define FD_CLK_SRC                    (1)     /*!< 0: IOCS_24M, 1: EOSC_20M    2:IOSC_32K                               */
#define HCLK_SRC                      (2)     /*!< 0: IOCS_24M, 1: EOSC_20M    2:FD_CLK     3:IOSC_32K                  */
#define HCLK_DIV                      (0)     /*!< 0: DIV1,     1: DIV2,   2: DIV4,   3: DIV8,   4: DIV16,  5: DIV32    */
#define PCLK_DIV                      (3)     /*!< 0: DIV1,     1: DIV2,   2: DIV4,   3: DIV8,   4: DIV16,  5: DIV32    */
#define WAIT_STATE                    (1)     /*!< 0: 0 WS,     1: 1 WS,   2: 2 WS,                                     */



/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __XTAL            (48000000UL)    /* Oscillator frequency             */

#define __SYSTEM_CLOCK    (__XTAL / 2)

#define  IOSC_24M_VALUE   24000000UL
#define  EOSC_20M_VALUE   20000000UL
#define  I0SC_32K_VALUE   32768UL

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __SYSTEM_CLOCK;/*!< System Clock Frequency (Core Clock)*/


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
void SystemCoreClockUpdate (void)
{
  uint8_t SystemCoreClockDiv;
  uint8_t SystemCoreClockSrc;
  uint8_t DoubleClockSrc;

    SystemCoreClockDiv = sys_clk_con_hclk_div_sel;

    SystemCoreClockSrc = sys_clk_con_sys_clksel;

    DoubleClockSrc = sys_clk_con_double_clk_sel;

  if (SystemCoreClockSrc == 0)
  {
     SystemCoreClock = IOSC_24M_VALUE >> SystemCoreClockDiv;
  }

  if (SystemCoreClockSrc == 1)
  {
     SystemCoreClock = EOSC_20M_VALUE >> SystemCoreClockDiv;
  }

  if (SystemCoreClockSrc == 2)
  {
         if(DoubleClockSrc == 0)

     SystemCoreClock = (IOSC_24M_VALUE*2) >> SystemCoreClockDiv;

         if(DoubleClockSrc == 1)

     SystemCoreClock = (EOSC_20M_VALUE*2) >> SystemCoreClockDiv;

         if(DoubleClockSrc == 2)

     SystemCoreClock = (I0SC_32K_VALUE*2) >> SystemCoreClockDiv;
  }

  if (SystemCoreClockSrc == 3)
  {
     SystemCoreClock = I0SC_32K_VALUE >> SystemCoreClockDiv;
  }

}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */

void SystemInit (void)
{

//#if (IOSC_24M_ENABLE == 1)
//
//    osc_con1_osc_en = 0x1;            //enable osc 24M
//
//#else
//
//     osc_con1_osc_en = 0;           //disable osc 24M
//
//#endif
//
//#if (EOSC_20M_ENABLE == 1)
//
//   CMSDK_PF->ALTFUNCSET |= (1<<0);    // PF0, used as OSC20M_IN
//   CMSDK_PF->ALTFUNCSET |= (1<<2);    // PF1, used as OSC20K_OUT
// // osc_con2_osc_crystal_c_ctrl = 0x2;
////    osc_con2_osc_drive = 0;
//
//   osc_con1_ex_20m_clk_en = 0;
//   osc_con1_osc_en = 0x1;         //enable crystal 20M
//   while(!osc_con1_rdy_crystal_clock);
//
//#else
//      osc_con1_osc_en = 0x0;          //disable crystal 20M

//#endif


//#if (IOSC_32K_ENABLE == 1)
//
//    iosc_32k_con_iosc_32k_en = 1;   //enable iosc_32k
//
//#else

//      iosc_32k_con_iosc_32k_en = 0x0;
//
//#endif
//
//#if (FD_CLK_ENABLE == 1)
//
//  fd_con_fd_en = 0x0;
//
//  #if (FD_CLK_SRC == 0 )
//  sys_clk_con_double_clk_sel = 0x0;  //freq_double: iosc_24m
//  #endif

//  #if (FD_CLK_SRC == 1 )
//  sys_clk_con_double_clk_sel = 0x1;  //freq_double: eosc_20m
//  #endif
//
//  #if (FD_CLK_SRC == 2 )
//  sys_clk_con_double_clk_sel = 0x2;  //freq_double: iosc_32k
//  #endif
//
//  fd_con_fd_en = 0x1;
//
//#else

//   fd_con_fd_en = 0x0;
//
//#endif

//   pRfFLSCTRL_0 -> FL_CMD |= (FLC_CMD_KEY_CODE_MAIN) | (WAIT_STATE<<5) ;  //wait state


//   sys_clk_con_sys_clksel   =  HCLK_SRC ;    //HCLK_SROUCE

//   sys_clk_con_hclk_div_sel =  HCLK_DIV;    /* set HCLK prescaler */
//  mco_con_mco_div_sel =  PCLK_DIV;
////   sys_clk_con_pclk_div_sel =  PCLK_DIV;    /* set PCLK prescaler */
//
//   Wait_ClockReady();
//
//   if(sys_clk_con_sys_clksel != HCLK_SRC)
//   {
//        while(1);    //clock switch fail
//   }
//

}

//<<< end of configuration section >>>

/**
  * @}
  */


/**
  * @}
  */
