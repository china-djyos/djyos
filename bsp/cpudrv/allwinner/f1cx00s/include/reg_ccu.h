#ifndef __REG_CCU_H__
#define __REG_CCU_H__


#ifndef __F1C100S_REG_CCU_H__
#define __F1C100S_REG_CCU_H__

#define F1C100S_CCU_BASE		(0x01c20000)

#define CCU_PLL_CPU_CTRL		(0x000)
#define CCU_PLL_AUDIO_CTRL		(0x008)
#define CCU_PLL_VIDEO_CTRL		(0x010)
#define CCU_PLL_VE_CTRL			(0x018)
#define CCU_PLL_DDR_CTRL		(0x020)
#define CCU_PLL_PERIPH_CTRL		(0x028)
#define CCU_CPU_CFG				(0x050)
#define CCU_AHB_APB_CFG			(0x054)

#define CCU_BUS_CLK_GATE0		(0x060)
#define CCU_BUS_CLK_GATE1		(0x064)
#define CCU_BUS_CLK_GATE2		(0x068)

#define CCU_SDMMC0_CLK			(0x088)
#define CCU_SDMMC1_CLK			(0x08c)
#define CCU_DAUDIO_CLK			(0x0b0)
#define CCU_SPDIF_CLK			(0x0b4)
//#define CCU_I2S_CLK				(0x0b8)
#define CCU_USBPHY_CFG			(0x0cc)
#define CCU_DRAM_CLK_GATE		(0x100)
#define CCU_DEBE_CLK			(0x104)
#define CCU_DEFE_CLK			(0x10c)
#define CCU_LCD_CLK				(0x118)
#define CCU_DEINTERLACE_CLK		(0x11c)
#define CCU_TVE_CLK				(0x120)
#define CCU_TVD_CLK				(0x124)
#define CCU_CSI_CLK				(0x134)
#define CCU_VE_CLK				(0x13c)
#define CCU_ADDA_CLK			(0x140)
#define CCU_AVS_CLK				(0x144)

#define CCU_PLL_STABLE_TIME0	(0x200)
#define CCU_PLL_STABLE_TIME1	(0x204)
#define CCU_PLL_CPU_BIAS		(0x220)
#define CCU_PLL_AUDIO_BIAS		(0x224)
#define CCU_PLL_VIDEO_BIAS		(0x228)
#define CCU_PLL_VE_BIAS			(0x22c)
#define CCU_PLL_DDR0_BIAS		(0x230)
#define CCU_PLL_PERIPH_BIAS		(0x234)
#define CCU_PLL_CPU_TUN			(0x250)
#define CCU_PLL_DDR_TUN			(0x260)
#define CCU_PLL_AUDIO_PAT		(0x284)
#define CCU_PLL_VIDEO_PAT		(0x288)
#define CCU_PLL_DDR0_PAT		(0x290)

#define CCU_BUS_SOFT_RST0		(0x2c0)
#define CCU_BUS_SOFT_RST1		(0x2c4)
#define CCU_BUS_SOFT_RST2		(0x2d0)

#endif /* __F1C100S_REG_CCU_H__ */


#endif

