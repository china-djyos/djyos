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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
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

