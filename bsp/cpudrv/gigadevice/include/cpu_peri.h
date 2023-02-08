#ifndef __CPU_PERI_H__
#define __CPU_PERI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "cpu_peri_dma.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_iic.h"
#include "cpu_peri_spi.h"
#include "cpu_peri_int_line.h"
#include "cpu_peri_uart.h"
//#include "cpu_peri_rcc.h"
//#include "cpu_peri_rtc.h"
//#include "cpu_peri_wdt.h"
//#include "cpu_peri_timer.h"
#include <system_gd32e10x.h>
#include <gd32e10x.h>
#include "get_cpuid.h"

extern struct DMA_StReg volatile * const pg_dma1_reg;
extern struct DMA_StReg volatile * const pg_dma2_reg;

//dma1的各通道寄存器
extern struct DMA_StChannelReg volatile * const pg_dma1_channel1_reg;
extern struct DMA_StChannelReg volatile * const pg_dma1_channel2_reg;
extern struct DMA_StChannelReg volatile * const pg_dma1_channel3_reg;
extern struct DMA_StChannelReg volatile * const pg_dma1_channel4_reg;
extern struct DMA_StChannelReg volatile * const pg_dma1_channel5_reg;
extern struct DMA_StChannelReg volatile * const pg_dma1_channel6_reg;
extern struct DMA_StChannelReg volatile * const pg_dma1_channel7_reg;
//dma2的各通道寄存器
extern struct DMA_StChannelReg volatile * const pg_dma2_channel1_reg;
extern struct DMA_StChannelReg volatile * const pg_dma2_channel2_reg;
extern struct DMA_StChannelReg volatile * const pg_dma2_channel3_reg;
extern struct DMA_StChannelReg volatile * const pg_dma2_channel4_reg;
extern struct DMA_StChannelReg volatile * const pg_dma2_channel5_reg;


//rcc(复位和时钟控制寄存器)
extern struct st_rcc_reg volatile * const pg_rcc_reg;

//片内flash控制寄存器
extern struct st_inflash_fpec_reg volatile * const pg_inflash_fpec_reg;
extern struct st_inflash_ob_reg volatile * const pg_inflash_ob_reg;

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_H__

