#ifndef _SCM6XX_CLK_H_
#define _SCM6XX_CLK_H_

#include "SCM6XX_ALL.h"


// ±÷”‘¥
#define LOSC   0
#define XLOSC  1
#define HOSC   2
#define XHOSC  3
#define PLL    4
typedef struct
{
    uint32_t refDiv;  //PLL_CTRL_PLL_REFDIV_1~4
    uint32_t fbDiv;   //PLL_CTRL_PLL_FBDIV_(n), n=0x0~0x7f
    uint32_t outDiv;  //PLL_CTRL_PLL_OUTDIV_1~4
    uint32_t refClk;  //PLL_CTRL_PLL_REF_XHOSC/PLL_CTRL_PLL_REF_HOSC/PLL_CTRL_PLL_REF_PAD50M
    uint32_t icpSelA; //PLL_CTRL_PLL_ICPSELA_0~1
    uint32_t icpSelB; //PLL_CTRL_PLL_ICPSELB_0~1
    uint32_t rSel;   //PLL_CTRL_PLL_RSEL_(n)
} PLL_CONFIG ;




/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void CLK_Enable_All_Clk(void);
void CLK_Disable_All_Clk(void);
void CLK_Enable_Peripheral_Clk(UINT8 module) ;
void CLK_Disable_Peripheral_Clk(UINT8 module) ;
void CLK_Enable_HCLK_Matrix(FunctionalState opt);
void CLK_Enable_HOSC(FunctionalState opt);
void CLK_Enable_LOSC(FunctionalState opt);
void CLK_Enable_PLL(FunctionalState opt);
void CLK_Enable_XHOSC(FunctionalState opt);
void CLK_Enable_XLOSC(FunctionalState opt);
void CLK_Select_HCLK(UINT32 HCLK_Clk);
void CLK_Select_PCLK(UINT32 PCLK_Clk);
void CLK_Configure_ADC_PLL_Division(UINT32 ADC_PLL_Clk);
void CLK_Configure_ADC_HOSC_Division(UINT32 ADC_HOSC_Clk);
void CLK_Select_RTC_Clk(UINT32 RTC_Clk);
void CLK_Select_GPIO_Debounce_Clk(UINT32 GPIO_DB_Clk);
void CLK_Select_Clk_Out(UINT32 Clk_Out_Clk);
void CLK_Enable_Clk_Out(FunctionalState opt);
void CLK_Configure_Pll(PLL_CONFIG Pll_Config);
void CLK_Configure_Main_Clk(UINT8 Main_Clk);


/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void CLK_EnAllClk(void) ;
void CLK_EnPeriClk(uint8_t module) ;
void CLK_DisPeriClk(uint8_t module) ;
void CLK_EnableMatrix(uint8_t opt) ;
void CLK_EnableHosc(uint8_t opt) ;
void CLK_EnableLosc(uint8_t opt) ;
void CLK_EnablePll(uint8_t opt) ;
void CLK_EnableXhosc(uint8_t opt) ;
void CLK_EnableXlosc(uint8_t opt) ;
void CLK_SelectHclk(uint32_t clk) ;
void CLK_SelectPclk(uint32_t clk) ;
void CLK_ConfigAdcPllDiv(uint32_t clk) ;
void CLK_ConfigAdcHoscDiv(uint32_t clk) ;
void CLK_SelectRtcClk(uint32_t clk) ;
void CLK_SelectGpioDbClk(uint32_t clk) ;
void CLK_SelectClkOut(uint32_t clk) ;
void CLK_ConfigPll(PLL_CONFIG pllConfig) ;
void CLK_EnClkOut(uint8_t opt) ;
void CLK_ConfigMainClk(uint8_t clk) ;

#endif 


