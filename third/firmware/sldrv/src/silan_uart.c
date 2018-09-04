/*
 * silan_uart.c
 *
 * Modify Date: 2016-4-28 18:10
 */

#define MODULE_NAME          "SILAN UART"

#include "ap1508.h"
#include "silan_uart_regs.h"
#include "silan_uart.h"
#include "string.h"
#include "silan_pmu.h"
#include "silan_irq.h"

static int sys_uart = SILAN_UART2;

int silan_uart_putc(u32 uart_addr, char c)
{
	while (__sREG32(uart_addr , UART_FR) & UART_FR_TXFF);
	__sREG32(uart_addr,UART_DR) = c;
	return 0;
}

int serial_putc(const char c)
{
	while(__sREG32(SILAN_UART_BASE(sys_uart) , UART_FR) & UART_FR_TXFF);
	__sREG32(SILAN_UART_BASE(sys_uart),UART_DR)=c;
	return 0;
}

int serial_getc(char *c)
{
	if((__sREG32(SILAN_UART_BASE(sys_uart), UART_FR) & UART_FR_RXFE) == 0) {
		c[0] = __sREG32(SILAN_UART_BASE(sys_uart), UART_DR) & 0xff;
		return 1;
	}
	else {
		return 0;
	}
}

int silan_uart_getc(unsigned int uart_addr)
{
	u32 data;
	while (__sREG32(uart_addr,UART_FR)& UART_FR_RXFE);
	data = __sREG32(uart_addr,UART_DR) & 0xff;
	if (data & 0xffffff00)
	{
		//printf("read uart data register error no: %d\n", (data & 0x0f00));
		return -1;
	}
	else
	{
		//printf("read uart data register %d\n", (data & 0x00ff));
		return (data & 0xff);
	}
}

int silan_uart_getc_noblock(unsigned int uart_addr)
{
	u32 data;
	if((__sREG32(uart_addr, UART_FR) & UART_FR_RXFE) == 0) {
		data = __sREG32(uart_addr, UART_DR) & 0xff;
	}
	else {
		data = 0;
	}
	return data;
}

void silan_uart_config(u32 uart_addr, u32 baudrate, u32 en_fifo, u32 en_irq)
{
	u32 temp, remainder, divider, fraction;

	u32 uart_clk = get_sysclk_val_settled();	// TODO

	__sREG32(uart_addr,UART_CR)=0;

	temp = 16 * baudrate;
	divider = uart_clk / temp;
	remainder = uart_clk % temp;
	temp = (8 * remainder) / baudrate;
	fraction = (temp >> 1) + (temp & 1);
	__sREG32(uart_addr,UART_IBRD) = divider;
	__sREG32(uart_addr,UART_FBRD) = fraction; 
	if (en_fifo)
	{
		__sREG32(uart_addr,UART_LCRH) = UART_LCRH_WLEN_8 | \
		                                UART_LCRH_FEN    | \
		                                UART_LCRH_SPS;
	}
	else
	{
		__sREG32(uart_addr,UART_LCRH) = UART_LCRH_WLEN_8 | \
		                                UART_LCRH_SPS;
	}
	__sREG32(uart_addr,UART_CR)   = UART_CR_RXE      | \
	                                UART_CR_TXE      | \
	                                UART_CR_UARTEN;
	if (en_fifo)
	{
		__sREG32(uart_addr,UART_FLS)   =   UART_FLS_TXIFLSEL_1 ;
	}
	if (en_irq)
		__sREG32(uart_addr,UART_INT) |= UART_INT_TX | UART_INT_TX;
}

void silan_uart_sys_init(u32 uart,u32 baudrate)
{
	sys_uart = uart;
	silan_uart_config(SILAN_UART_BASE(uart), baudrate, 1, 0);
}
