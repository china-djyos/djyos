/*
 * based on aterytek's demo:
 * hello/cmsis/system_at32f435_437.c
 */

/**
  **************************************************************************
  * @file     system_at32f435_437.c
  * @version  v2.0.8
  * @date     2022-04-25
  * @brief    contains all the functions for cmsis cortex-m4 system source file
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup AT32F435_437_system
  * @{
  */

#include "at32f435_437.h"
#include "at32f435_437_clock.h"
/* djyos stuff */
#include <cpu_peri.h>
#include <board-config.h>


/** @addtogroup AT32F435_437_system_private_defines
  * @{
  */
#define VECT_TAB_OFFSET                  0x0 /*!< vector table base offset field. this value must be a multiple of 0x200. */
/**
  * @}
  */

/** @addtogroup AT32F435_437_system_private_variables
  * @{
  */
unsigned int system_core_clock           = HICK_VALUE; /*!< system clock frequency (core clock) */
/**
  * @}
  */

/** @addtogroup AT32F435_437_system_private_functions
  * @{
  */

/**
  * @brief  setup the microcontroller system
  *         initialize the flash interface.
  * @note   this function should be used only after reset.
  * @param  none
  * @retval none
  */
void SystemInit (void)
{
#if defined (__FPU_USED) && (__FPU_USED == 1U)
  SCB->CPACR |= ((3U << 10U * 2U) |         /* set cp10 full access */
                 (3U << 11U * 2U)  );       /* set cp11 full access */
#endif

  /* reset the crm clock configuration to the default reset state(for debug purpose) */
  /* set hicken bit */
  CRM->ctrl_bit.hicken = TRUE;

  /* wait hick stable */
  while(CRM->ctrl_bit.hickstbl != SET);

  /* hick used as system clock */
  CRM->cfg_bit.sclksel = CRM_SCLK_HICK;

  /* wait sclk switch status */
  while(CRM->cfg_bit.sclksts != CRM_SCLK_HICK);

  /* reset hexten, hextbyps, cfden and pllen bits */
  CRM->ctrl &= ~(0x010D0000U);

  /* reset cfg register, include sclk switch, ahbdiv, apb1div, apb2div, adcdiv, clkout bits */
  CRM->cfg = 0;

  /* reset pllms pllns pllfr pllrcs bits */
  CRM->pllcfg = 0x00033002U;

  /* reset clkout[3], usbbufs, hickdiv, clkoutdiv */
  CRM->misc1 = 0;

  /* disable all interrupts enable and clear pending bits  */
  CRM->clkint = 0x009F0000U;

#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE  | VECT_TAB_OFFSET;  /* vector table relocation in internal sram. */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;  /* vector table relocation in internal flash. */
#endif
}

/**
  * @brief  update system_core_clock variable according to clock register values.
  *         the system_core_clock variable contains the core clock (hclk), it can
  *         be used by the user application to setup the systick timer or configure
  *         other parameters.
  * @param  none
  * @retval none
  */
void system_core_clock_update(void)
{
  uint32_t pll_ns = 0, pll_ms = 0, pll_fr = 0, pll_clock_source = 0, pllrcsfreq = 0;
  uint32_t temp = 0, div_value = 0;
  crm_sclk_type sclk_source;

  static const uint8_t sys_ahb_div_table[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
  static const uint8_t pll_fr_table[6] = {1, 2, 4, 8, 16, 32};

  /* get sclk source */
  sclk_source = crm_sysclk_switch_status_get();

  switch(sclk_source)
  {
    case CRM_SCLK_HICK:
      if(((CRM->misc1_bit.hick_to_sclk) != RESET) && ((CRM->misc1_bit.hickdiv) != RESET))
        system_core_clock = HICK_VALUE * 6;
      else
        system_core_clock = HICK_VALUE;
      break;
    case CRM_SCLK_HEXT:
      system_core_clock = HEXT_VALUE;
      break;
    case CRM_SCLK_PLL:
      /* get pll clock source */
      pll_clock_source = CRM->pllcfg_bit.pllrcs;

      /* get multiplication factor */
      pll_ns = CRM->pllcfg_bit.pllns;
      pll_ms = CRM->pllcfg_bit.pllms;
      pll_fr = pll_fr_table[CRM->pllcfg_bit.pllfr];

      if (pll_clock_source == CRM_PLL_SOURCE_HICK)
      {
        /* hick selected as pll clock entry */
        pllrcsfreq = HICK_VALUE;
      }
      else
      {
        /* hext selected as pll clock entry */
        pllrcsfreq = HEXT_VALUE;
      }

      system_core_clock = (uint32_t)(((uint64_t)pllrcsfreq * pll_ns) / (pll_ms * pll_fr));
      break;
    default:
      system_core_clock = HICK_VALUE;
      break;
  }

  /* compute sclk, ahbclk frequency */
  /* get ahb division */
  temp = CRM->cfg_bit.ahbdiv;
  div_value = sys_ahb_div_table[temp];
  /* ahbclk frequency */
  system_core_clock = system_core_clock >> div_value;
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */




/**
  * @brief  system clock config program
  * @note   the system clock is configured as follow:
  *         - system clock        = (hext * pll_ns)/(pll_ms * pll_fr)
  *         - system clock source = pll (hext)
  *         - hext                = 8000000
  *         - sclk                = 288000000
  *         - ahbdiv              = 1
  *         - ahbclk              = 288000000
  *         - apb2div             = 2
  *         - apb2clk             = 144000000
  *         - apb1div             = 2
  *         - apb1clk             = 144000000
  *         - pll_ns              = 144
  *         - pll_ms              = 1
  *         - pll_fr              = 4
  * @param  none
  * @retval none
  */
void system_clock_config(void)
{
  /* enable pwc periph clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

  /* config ldo voltage */
  pwc_ldo_output_voltage_set(PWC_LDO_OUTPUT_1V3);

  /* set the flash clock divider */
  flash_clock_divider_set(FLASH_CLOCK_DIV_3);

  /* reset crm */
  crm_reset();

  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);

  /* wait till hext is ready */
  while(crm_hext_stable_wait() == ERROR)
  {
  }

  /* config pll clock resource */
  crm_pll_config(CRM_PLL_SOURCE_HEXT, 125, 1, CRM_PLL_FR_4);

  /* enable pll */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  /* wait till pll is ready */
  while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
  {
  }

  /* config ahbclk */
  crm_ahb_div_set(CRM_AHB_DIV_1);

  /* config apb2clk, the maximum frequency of APB1/APB2 clock is 144 MHz  */
  crm_apb2_div_set(CRM_APB2_DIV_2);

  /* config apb1clk, the maximum frequency of APB1/APB2 clock is 144 MHz  */
  crm_apb1_div_set(CRM_APB1_DIV_2);

  /* enable auto step mode */
  crm_auto_step_mode_enable(TRUE);

  /* select pll as system clock source */
  crm_sysclk_switch(CRM_SCLK_PLL);

  /* wait till pll is used as system clock source */
  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
  {
  }

  /* disable auto step mode */
  crm_auto_step_mode_enable(FALSE);

  /* update system_core_clock global variable */
  system_core_clock_update();
}


void SysClockInit(void)
{
    // system_core_clock_update();
    system_clock_config();
}

void SRAM_Init(void)
{
    /* if you have external ram. */
}
