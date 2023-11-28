//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#ifndef __CPU_PERI_UART_H__
#define __CPU_PERI_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

struct tagUartReg
{
    vu32 UART_RBR_THR_DLL;  //00
    vu32 UART_DLH_IER;      //04
    vu32 UART_IIR_FCR;      //08
    vu32 UART_LCR;          //0c
    vu32 UART_MCR;          //10
    vu32 UART_LSR;          //14
    vu32 UART_MSR;          //18
    vu32 UART_SCH;          //1c
    u32  pads[23];
    vu32 UART_USR;          //7c
    vu32 UART_TFL;          //80
    vu32 UART_RFL;          //84
    u32  pads1[6];
    vu32 UART_HALT;         //a4
};

#define CN_UART0    0
#define CN_UART1    1
#define CN_UART2    2
#define CN_UART_NUM 3

#define UART0_ADDR 0x01c25000
#define UART1_ADDR 0x01C25400
#define UART2_ADDR 0x01c25800

//------------------------------------------------------------------------------
// Register read and write operations
//------------------------------------------------------------------------------
#define READ_REG8 (reg)         (*(volatile unsigned char  *)(reg))
#define READ_REG16(reg)         (*(volatile unsigned short *)(reg))
#define READ_REG32(reg)         (*(volatile unsigned int   *)(reg))

#define WRITE_REG8 (Value, reg) (*(volatile unsigned char  *)(reg) = (Value))
#define WRITE_REG16(Value, reg) (*(volatile unsigned short *)(reg) = (Value))
#define WRITE_REG32(Value, reg) (*(volatile unsigned int   *)(reg) = (Value))

//------------------------------------------------------------------------------
// Basic bit operations
//------------------------------------------------------------------------------
#define SET_REG_BIT(val, pos)               ((val) |= 1U<<(pos))
#define CLEAR_REG_BIT(val, pos)             ((val) &= ~(1U<<(pos)))
#define TEST_REG_BIT(val, pos)              ((val) & (1U<<(pos)))
#define BITS_REG_MOD(len)                   ((1U<<(len)) - 1)
#define BITS_REG_MASK(len, pos)             (~(BITS_REG_MOD(len)<<(pos)))
#define CLEAR_REG_BITS(val, pos, len)       (val) &= BITS_REG_MASK(len, pos)
#define SET_REG_BITS(val, pos, len, _val)   (val) = ((val) & BITS_REG_MASK(len, pos)) | (((_val) & BITS_REG_MOD(len))<<(pos))
#define TEST_REG_BITS(val, pos, len, _val)  ((val) & (~BITS_REG_MASK(len, pos))) == ((_val)<<(pos))
#define GET_REG_BITS_VAL(val, pos, len)     (((val)>>(pos)) & BITS_REG_MOD(len))

// Divisor Latch Low Register
#define UART_REG_DLL_DLL_BIT            0

// Divisor Latch Hiht Register
#define UART_REG_DLH_DLH_BIT            0

// Interrupt Enable Register
#define UART_REG_IER_PTIME              7
#define UART_REG_IER_EMSI_BIT           3
#define UART_REG_IER_ELSI_BIT           2
#define UART_REG_IER_ETBEI_BIT          1
#define UART_REG_IER_ERBFI_BIT          0

// Interrupt Identity Register
// interrupt definition
#define UART_INT_MS                     0x00                // modem status
#define UART_INT_NIP                    0x01                // no interrupt pending
#define UART_INT_THRE                   0x02                // THR empty
#define UART_INT_RDA                    0x04                // received data available
#define UART_INT_RLS                    0x06                // receiver line status
#define UART_INT_BD                     0x07                // busy detect
#define UART_INI_CT                     0x0c                // character timeout

// FIFO Control Register
#define UART_REG_FCR_RT_BIT             6
#define UART_REG_FCR_TFT_BIT            4
#define UART_REG_FCR_DMAM_BIT           3
#define UART_REG_FCR_XFIFOR_BIT         2
#define UART_REG_FCR_RFIFOR_BIT         1
#define UART_REG_FCR_FIFOE_BIT          0

// Line Control Register
#define UART_REG_LCR_DLAB_BIT           7
#define UART_REG_LCR_BC_BIT             6
#define UART_REG_LCR_EPS_BIT            4
#define UART_REG_LCR_PEN_BIT            3
#define UART_REG_LCR_STOP_BIT           2
#define UART_REG_LCR_DLS_BIT            0

// Modem Control Register
#define UART_REG_MCR_SIRE_BIT           6
#define UART_REG_MCR_AFCE_BIT           5
#define UART_REG_MCR_LOOP_BIT           4
#define UART_REG_MCR_RTS_BIT            1
#define UART_REG_MCR_DTR_BIT            0

// Line Status Register
#define UART_REG_LSR_FIFOERR_BIT        7
#define UART_REG_LSR_TEMT_BIT           6
#define UART_REG_LSR_THRE_BIT           5
#define UART_REG_LSR_BI_BIT             4
#define UART_REG_LSR_FE_BIT             3
#define UART_REG_LSR_PE_BIT             2
#define UART_REG_LSR_OE_BIT             1
#define UART_REG_LSR_DR_BIT             0

// Status Register
#define UART_REG_USR_RFF_BIT            4
#define UART_REG_USR_RFNE_BIT           3
#define UART_REG_USR_TFE_BIT            2
#define UART_REG_USR_TFNF_BIT           1
#define UART_REG_USR_BUSY_BIT           0

bool_t UART_LowPowerConfig(u8 port,u8 flag);
ptu32_t ModuleInstall_UART(ptu32_t para);

void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut);

#ifdef __cplusplus
}
#endif

#endif // __CPU_PERI_UART_H__

