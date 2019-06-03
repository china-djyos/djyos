/*
 * silan_irq.h
 * 
 * Modified Date: 2016-03-22
 */

#ifndef __SILAN_IRQ_H__
#define __SILAN_IRQ_H__

#include "silan_types.h"
#include "silan_config.h"

#if defined(__CC_ARM)||  defined ( __GNUC__ )
#include "silan_core_m0.h"
#endif


typedef void (*hdl_t)(int subid);

typedef enum {
	PIC_IRQID_NONE             = -1,
	PIC_IRQID_NONE1            =  0,
	PIC_IRQID_NONE2            =  1,
	PIC_IRQID_SAR              =  2,
	PIC_IRQID_SPIFLASH         =  3,
	PIC_IRQID_MISC             =  4,
	PIC_IRQID_PDM              =  5, 
	PIC_IRQID_GPIO1            =  6,
	PIC_IRQID_GPIO2            =  7,
	PIC_IRQID_TIMER            =  8,
	PIC_IRQID_SDMAC            =  9,
	PIC_IRQID_SDMMC            = 10,
	PIC_IRQID_IIR              = 11,
	PIC_IRQID_SDIO             = 12,
	PIC_IRQID_USBFS            = 13,
	PIC_IRQID_USBHS            = 14,
	PIC_IRQID_CXC              = 15,
	PIC_IRQID_ADMAC            = 16,
	PIC_IRQID_SPDIF            = 17,
	PIC_IRQID_PCM              = 18,
	PIC_IRQID_I2S              = 19,
	PIC_IRQID_RTC              = 20,
	PIC_IRQID_PMU              = 21,
	PIC_IRQID_DSP_DMAC         = 22,
	PIC_IRQID_DSP_SDMAC,  /* Only DSP */
	PIC_IRQID_DSP_ADMAC,  /* Only DSP */
	PIC_IRQID_DSP_GPIO,   /* Only DSP */
	PIC_IRQID_WDOG             = 30,
	PIC_IRQID_CTIMER,
	PIC_IRQID_LMT,
}pic_irq_t;

#define PIC_SUBID_SOLO                (-1)
#define PIC_SUBID_LMT                 (32)
#define PIC_SUBID_MISC_UART1          (0)
#define PIC_SUBID_MISC_UART2          (1)
#define PIC_SUBID_MISC_UART3          (2)
#define PIC_SUBID_MISC_UART4          (3)
#define PIC_SUBID_MISC_SPI1           (4)
#define PIC_SUBID_MISC_SPI2           (5)
#define PIC_SUBID_MISC_I2C1           (6)
#define PIC_SUBID_MISC_I2C2           (7)
#define PIC_SUBID_MISC_PDB            (9)
#define PIC_SUBID_I2C_SPI1            (0)
#define PIC_SUBID_I2C_SPI2            (1)
#define PIC_SUBID_I2C_I2C1            (2)
#define PIC_SUBID_I2C_I2C2            (3)
#define PIC_SUBID_DMAC_CH0            (0)
#define PIC_SUBID_DMAC_CH1            (1)
#define PIC_SUBID_DMAC_CH2            (2)
#define PIC_SUBID_DMAC_CH3            (3)
#define PIC_SUBID_DMAC_CH4            (4)
#define PIC_SUBID_DMAC_CH5            (5)
#define PIC_SUBID_DMAC_CH6            (6)
#define PIC_SUBID_DMAC_CH7            (7)
#define PIC_SUBID_SPDIF_IN            (0)
#define PIC_SUBID_SPDIF_OUT           (1)
#define PIC_SUBID_I2S_O1              (0)
#define PIC_SUBID_I2S_O2              (1)
#define PIC_SUBID_I2S_O3              (2)
#define PIC_SUBID_I2S_I1              (8)
#define PIC_SUBID_I2S_I2              (9)
#define PIC_SUBID_I2S_I3              (10)
#define PIC_SUBID_TIMER_0             (0)
#define PIC_SUBID_TIMER_1             (1)
#define PIC_SUBID_TIMER_2             (2)
#define PIC_SUBID_TIMER_3             (3)
#define PIC_SUBID_TIMER_4             (4)
#define PIC_SUBID_TIMER_MSO           (5)     /* millisecond */
#define PIC_SUBID_TIMER_SO            (6)     /* second      */

int  silan_pic_request(pic_irq_t id, int subid, hdl_t hdl);
void silan_pic_free(pic_irq_t id, int subid);
void silan_pic_enable_irq(pic_irq_t irq);
void silan_pic_disable_irq(pic_irq_t irq);
//uint32_t  silan_pic_get_subid(pic_irq_t id);
//uint32_t  silan_pic_get_subid_stat(pic_irq_t id);
void silan_pic_init(void);

int  pic_port_init(void);
int pic_port_request(int id, hdl_t hdl);
void pic_port_free(int id);

inline u32 pic_get_enable(void);
inline void pic_set_enable(u32 restored);
inline void pic_clr_enable(u32 restored);

//#define SILAN_PIC_DBGTOOL
#ifdef SILAN_PIC_DBGTOOL
void silan_pic_show(void);
#endif

#endif
