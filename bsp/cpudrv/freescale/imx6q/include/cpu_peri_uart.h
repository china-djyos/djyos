#ifndef __CPU_UART_H__
#define __CPU_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

//UTRSTAT状态位定义
#define UartTXShiftEmpty    (1<<2)  //1:发送移位寄存器空,0:非空
#define UartTxBufEmpty      (1<<1)  //1:发送缓冲区空,0:非空
#define UartRxBufNotEmpty   (1<<0)  //1:接受缓冲区有数据,0:无数据
//UFCON FIFO控制寄存器
#define cn_tx_fifo_reset    (1<<2)  //写1复位发送fifo,读总是0
#define cn_rx_fifo_reset    (1<<1)  //写1复位发送fifo,读总是0
//UFSTAT FIFO状态寄存器
#define cn_rx_over          (1<<6)  //1:接收fifo溢出,0:正常

//通信状态变量各位定义
#define CN_SER_RXBUF_BUSY         (1<<0)
#define CN_SER_RXBUF_EMPTY     (1<<1)
#define CN_SER_RECEIVE_OVER    (1<<2)

//中断控制位，在2410的中断控制器中，每个串口只分配了一个中断，具体中断原因是通过
//查询 SUBSRCPND 寄存器相应的位来得到的，控制 INTSUBMSK 中相应位可独立地禁止
//接收/发送中断。
#define BIT_SUB_ERR2    (0x1<<8)
#define BIT_SUB_TXD2    (0x1<<7)
#define BIT_SUB_RXD2    (0x1<<6)
#define BIT_SUB_ERR1    (0x1<<5)
#define BIT_SUB_TXD1    (0x1<<4)
#define BIT_SUB_RXD1    (0x1<<3)
#define BIT_SUB_ERR0    (0x1<<2)
#define BIT_SUB_TXD0    (0x1<<1)
#define BIT_SUB_RXD0    (0x1<<0)
#define BIT_ALL_UART    (0x1ff)

#define SUB_RXD_FLG     0
#define SUB_TXD_FLG     1
#define SUB_ERR_FLG     2
#define BIT_SUB_INT(m,n)   (0x1<<(m*3 + n))
//#define BIT_SUB_INT(m,n)   (0x1<<((m-1)*3 + n))

#define UART_TX_FIFO_SIZE  16  //硬件的发送fifo深度
#define UART_RX_FIFO_SIZE  16  //硬件的接收fifo深度


struct UART_REG{
    vu32 ULCON;                   // line control reg
    vu32 UCON;                    // control reg
    vu32 UFCON;                   // FIFO control reg
    vu32 UMCON;                   // modem control reg
    vu32 UTRSTAT;                 // tx/rx status reg
    vu32 UERSTAT;                 // rx error status reg
    vu32 UFSTAT;                  // FIFO status reg
    vu32 UMSTAT;                  // modem status reg
    vu32 UTXH;                    // tx buffer reg
    vu32 URXH;                    // rx buffer reg
    vu32 UBRDIV;                  // baud rate divisor
    vu32 UDIVSLOT;                // baud rate divisor
};

#define UART0_REG_BASE  ((struct UART_REG*)0x50000000)
#define UART1_REG_BASE  ((struct UART_REG*)0x50004000)
#define UART2_REG_BASE  ((struct UART_REG*)0x50008000)
#define UART3_REG_BASE  ((struct UART_REG*)0x5000C000)

#define CN_UART0       0
#define CN_UART1       1
#define CN_UART2       2
#define CN_UART_NUM    3

ptu32_t UART_ModuleInit(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif // __CPU_UART_H__

