#include <stdint.h>
#include "sys_clock.h"
//#include "sys_types.h"
#include "cpu_peri_io.h"
#include "reg_ccu.h"

#define delay_ms(x) DJY_DelayUs(x * 1000)
#define delay_us(x) DJY_DelayUs(x)
#define u64_t u64
#define u32_t u32
#define u16_t u16
#define u8_t  u8

#define CCU_Base_Address                (u32_t)0x01C20000
#define CCU_BUS_CLK_GATING_REG0         (u32_t)CCU_Base_Address+0x0060
#define CCU_BUS_CLK_GATING_REG1         (u32_t)CCU_Base_Address+0x0064
#define CCU_BUS_CLK_GATING_REG2         (u32_t)CCU_Base_Address+0x0068
#define CCU_BUS_SOFT_RST_REG0           (u32_t)CCU_Base_Address+0x02C0
#define CCU_BUS_SOFT_RST_REG1           (u32_t)CCU_Base_Address+0x02C4
#define CCU_BUS_SOFT_RST_REG2           (u32_t)CCU_Base_Address+0x02D0

/*配置PLL-CPU*/
static void clock_set_pll_cpu(u32_t clk)
{
    u32_t n, k, m, p;
    u32_t rval = 0;
    u32_t div = 0;

    if(clk > 720000000)
        clk = 720000000;

    if((clk % 24000000) == 0)
    {
        div = clk / 24000000;
        n = div - 1;
        k = 0;
        m = 0;
        p = 0;
    }
    else if((clk % 12000000) == 0)
    {
        m = 1;
        div = clk / 12000000;
        if((div % 3) == 0)
            k = 2;
        else if((div % 4) == 0)
            k = 3;
        else
            k = 1;
        n = (div / (k + 1)) - 1;
        p = 0;
    }
    else
    {
        div = clk / 24000000;
        n = div - 1;
        k = 0;
        m = 0;
        p = 0;
    }

    rval = read32(F1C100S_CCU_BASE + CCU_PLL_CPU_CTRL);
    rval &= ~((0x3 << 16) | (0x1f << 8) | (0x3 << 4) | (0x3 << 0));
    rval |= (1U << 31) | (p << 16) | (n << 8) | (k << 4) | m;
    write32(F1C100S_CCU_BASE + CCU_PLL_CPU_CTRL, rval);
    while(!(read32(F1C100S_CCU_BASE + CCU_PLL_CPU_CTRL) & (1 << 28)));
}
/*
配置系统时钟，为24MHZ的整倍数，单位HZ或MHZ
*/
void Sys_Clock_Init(int cpu_frequency)
{
    u32_t val;
    int cpu_frequency_hz=0;
    if(cpu_frequency<1000)//MHZ->HZ
    {
        cpu_frequency_hz=cpu_frequency*1000000;
    }else//HZ
    {
        cpu_frequency_hz=cpu_frequency;
    }
    //24MHZ倍数
    cpu_frequency_hz=cpu_frequency_hz/24000000*24000000;

    /*切换时钟源到OSC24M*/
    val = read32(F1C100S_CCU_BASE + CCU_CPU_CFG);
    val &= ~(0x3 << 16);
    val |= (0x1 << 16);
    write32(F1C100S_CCU_BASE + CCU_CPU_CFG, val);

  /*设置CPU-PLL*/
    clock_set_pll_cpu(cpu_frequency_hz);

  /*切换时钟源到CPU-PLL*/
    val = read32(F1C100S_CCU_BASE + CCU_CPU_CFG);
    val &= ~(0x3 << 16);
    val |= (0x2 << 16);
    write32(F1C100S_CCU_BASE + CCU_CPU_CFG, val);

    /*配置PLL_PERIPH到600MHZ*/
    write32(F1C100S_CCU_BASE + CCU_PLL_PERIPH_CTRL, 0x80041800);
  while(!(read32(F1C100S_CCU_BASE + CCU_PLL_PERIPH_CTRL) & (1 << 28)));

    /*配置PLL_VIDEO到297MHZ*/
    write32(F1C100S_CCU_BASE + CCU_PLL_VIDEO_CTRL, 0x81006207);
  while(!(read32(F1C100S_CCU_BASE + CCU_PLL_VIDEO_CTRL) & (1 << 28)));

  /*配置AHB=CPU/2,APB*/
    val=0;
    /*13:12 AHB_CLK = AHB_CLK_SRC_SEL/ AHB_CLK_DIV_RATIO*/
    val|=(2<<12);/* AHB_CLK_SRC_SEL.0: LOSC  1: OSC24M  2: CPUCLK  3: PLL_PERIPH/AHB_PRE_DIV*/
    /*5:4 AHB_CLK_DIV_RATIO.*/
    val|=(1<<4); /*0: /1      1: /2      2: /4*/
    /*9:8 APB_CLK_RATIO. APB_CLK = AHB_CLK/ APB_CLK_RATIO*/
    val|=(1<<8); /*0&1: /2      2: /4     3: /8*/
    /*17:16 HCLKC_DIV  .The clock source is the CPUCLK*/
    val|=(1<<16);/*00: /1     01: /2      10: /3      11: /4*/
    write32(F1C100S_CCU_BASE + CCU_AHB_APB_CFG, val);

  /*DRAM Gating Register*/
    val = read32(F1C100S_CCU_BASE + CCU_DRAM_CLK_GATE);
    val |= (0x1 << 26); //BE_DCLK_GATING
    val |= (0x1 << 24); //FE_DCLK_GATING
    write32(F1C100S_CCU_BASE + CCU_DRAM_CLK_GATE, val);
}

/*
返回PLL_PERIPH时钟-HZ
*/
unsigned int get_pll_periph_frequency(void)
{
    int reg=read32(F1C100S_CCU_BASE + CCU_PLL_PERIPH_CTRL);
    int N=((reg>>8)&0x1f)+1;
    int K=((reg>>4)&0x3)+1;
    int PLL=24000000*N*K;
  return PLL;
}
/*
返回系统时钟-HZ
*/
unsigned int get_cpu_frequency(void)
{
    int div[3]={1,2,4};
    int reg=read32(F1C100S_CCU_BASE + CCU_CPU_CFG);//时钟源
    int cpu_clk_src=(reg>>16)&0x3;
    if(cpu_clk_src==0)//00: LOSC
    {
        return 32000;
    }else if(cpu_clk_src==1)//01: OSC24M
    {
        return 24000000;
    }else//1X: PLL_CPU
    {
        reg=read32(F1C100S_CCU_BASE + CCU_PLL_CPU_CTRL);//CPU PLL
        int N=((reg>>8)&0x1f)+1;
        int K=((reg>>4)&0x3)+1;
        int M=((reg>>0)&0x3)+1;
        int P=div[(reg>>16)&0x3];
        int PLL= (24000000*N*K) / (M*P);
        return PLL;
    }
}
/*
返回AHB时钟-HZ
*/
unsigned int get_ahb_frequency(void)
{
    int div[4]={1,2,4,8};
    int APB_CLK_RATIO[4]={2,2,4,8};
    int reg=read32(F1C100S_CCU_BASE + CCU_AHB_APB_CFG);
    int ahb_clk_src=(reg>>12)&0x3;
    int clk_src=0;
    if(ahb_clk_src==0)//00: LOSC
    {
        clk_src=32000;
    }else if(ahb_clk_src==1)//01: OSC24M
    {
        clk_src=24000000;
    }else if(ahb_clk_src==2)//10: CPUCLK
    {
        clk_src=get_cpu_frequency();
    }else//11: PLL_PERIPH/AHB_PRE_DIV
    {
        int AHB_PRE_DIV=APB_CLK_RATIO[(reg>>8)&0x2];
        int PLL_PERIPH=get_pll_periph_frequency();
        clk_src=PLL_PERIPH/AHB_PRE_DIV;
    }
    return clk_src/div[(reg>>4)&0x3];
}
/*
返回APB时钟-HZ
*/
unsigned int get_apb_frequency(void)
{
    int div[4]={2,2,4,8};
    int ahb_clk=get_ahb_frequency();
    int d=(read32(F1C100S_CCU_BASE + CCU_AHB_APB_CFG)>>8)&0x3;
    return ahb_clk/div[d];
}
/*
返回HCLK时钟-HZ
*/
unsigned int get_hclk_frequency(void)
{
    int cpu_clk=get_cpu_frequency();
    int div=(read32(F1C100S_CCU_BASE + CCU_AHB_APB_CFG)>>16)&0x3;
    return cpu_clk/(div+1);
}
/*打开外设时钟*/
void Open_Dev_Clock(int Dev)
{
    //使能时钟
    S_BIT((CCU_BUS_CLK_GATING_REG0+(((Dev>>8)&0x3)*0x4)),(Dev&0x1f));
    //关闭复位
    if(((Dev>>8)&0x3)==2)//Bus Software Reset Register 2 = 0x2d0
        S_BIT((CCU_BUS_SOFT_RST_REG0+(((Dev>>8)&0x3)*0x4)+0x8),(Dev&0x1f));
    else
        S_BIT((CCU_BUS_SOFT_RST_REG0+(((Dev>>8)&0x3)*0x4)),(Dev&0x1f));

    delay_us(100);
}

/*关闭外设时钟*/
void Close_Dev_Clock(int Dev)
{
    //关闭时钟
    C_BIT((CCU_BUS_CLK_GATING_REG0+(((Dev>>8)&0x3)*0x4)),(Dev&0x1f));
    //开启复位
    if(((Dev>>8)&0x3)==2)//Bus Software Reset Register 2 = 0x2d0
        C_BIT((CCU_BUS_SOFT_RST_REG0+(((Dev>>8)&0x3)*0x4)+0x8),(Dev&0x1f));
    else
        C_BIT((CCU_BUS_SOFT_RST_REG0+(((Dev>>8)&0x3)*0x4)),(Dev&0x1f));
    delay_us(100);
}


