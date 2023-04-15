#ifndef __CPU_PERI_H__
#define __CPU_PERI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "int_hard.h"
#include "cpu_peri_ssmc.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_int_line.h"
#include "cpu_peri_lcd.h"
#include "cpu_peri_nand.h"
#include "cpu_peri_rtc.h"
#include "cpu_peri_sdi.h"
#include "cpu_peri_timer.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_wdt.h"
#include "cpu_peri_iic.h"
#include "cpu_peri_adc.h"
#include "cpu_peri_touch.h"

extern struct INTC_REG   *pg_int_reg;
extern struct GPIO_REG   *pg_gpio_reg;
extern struct UART_REG   *pg_uart0_reg;
extern struct UART_REG   *pg_uart1_reg;
extern struct UART_REG   *pg_uart2_reg;
extern struct UART_REG   *pg_uart3_reg;
extern struct NFC_REG    *pg_nand_reg;
extern struct TIMER_REG  *pg_timer_reg;
extern struct WDT_REG    *pg_wdt_reg;

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_H__

