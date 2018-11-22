#ifndef CPU_PERI_H_
#define CPU_PERI_H_

#ifdef __cplusplus
extern "C" {
#endif
/*这几个头文件应该不放在这里，但是新建工程的时候*/
/*不会自动包含这几个头文件，没办法，只好先放这里*/
#include "djyos.h"
#include "stdio.h"
#include "board.h"
#include "core_ck803.h"
#include "silan_config.h"
#include "silan_syscfg.h"
#include "silan_uart.h"
#include "silan_printf.h"
#include "silan_iomux.h"
#include "silan_iomux_regs.h"
#include "silan_syscfg.h"
#include "silan_pmu.h"
#include "silan_gpio.h"
#include "silan_irq.h"
#include "silan_timer.h"
#include "silan_timer_regs.h"
#include "silan_adc.h"
#include "silan_bootspi.h"
#include "silan_bootspi_regs.h"
#include "silan_rtc.h"
#include "silan_sdram.h"
#include "silan_pwm.h"
#include "silan_version.h"
#include "silan_irq.h"
#include "silan_pic_regs.h"
#include "cpu_peri_printf.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_isr.h"
#include "cpu_peri_spiflash.h"
#ifdef __cplusplus
}
#endif

#endif //CPU_PERI_PRINTF_H_
