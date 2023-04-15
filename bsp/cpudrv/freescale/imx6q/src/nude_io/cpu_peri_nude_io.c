#include "board-config.h"
#include "stdint.h"
#include "string.h"
#include "cpu_peri.h"
//#include "nude_io.h"


u8 Nude_ScanByte(void)
{
    u32 fifo_reg;
    for(;;)
    {
        fifo_reg = pg_uart0_reg->UFSTAT;
        if((fifo_reg & 0x3f) != 0)
            return pg_uart0_reg->URXH;
    }
}
bool_t Nude_ReadByte(u8 *byte)
{
    u32 fifo_reg;
    fifo_reg = pg_uart0_reg->UFSTAT;
    if((fifo_reg & 0x3f) != 0)
    {
        *byte = pg_uart0_reg->URXH;
        return true;
    }else
        return false;
}

void Nude_PrintfByte(u8 byte)
{
    u32 fifo_reg;
    for(;;)
    {
        fifo_reg = pg_uart0_reg->UFSTAT;
        if(((fifo_reg>>8) & 0x3f) < 0x3f)
        {
            pg_uart0_reg->UTXH = byte;
            return ;
        }
    }
}

void Nude_PrintfStr(u8 *str)
{
    u32 i=0;
    while(str[i] != 0)
    {
        Nude_PrintfByte(str[i]);
        i++;
    }
}

void Nude_PrintfHex(u32 hex,u32 size)
{
    u8 uni;
    if((size < 1 )|| (size >8))
        return ;
    for(;size > 0; size--)
    {
        uni = (hex >>(size*4-4))&0xf;
        if(uni < 10)
            Nude_PrintfByte(uni+0x30);
        else
            Nude_PrintfByte(uni+0x37);
    }
}

bool_t Nude_TerminalInit(void)
{
    u32 temp;
    //初始化IO端口位uart功能
    //Ports  :  GPH10 GPH9 GPH8 GPH7 GPH6 GPH5 GPH4 GPH3 GPH2 GPH1  GPH0
    //Signal :   xx    xx   xx   xx   xx   xx   xx  RXD0 TXD0 nRTS0 nCTS0
    //Binary :   xx    xx   xx   xx   xx   xx   xx   xx   xx   xx   xx

    return true;
}
