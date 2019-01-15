#include "ap1508.h"
#include "silan_mcu2risc.h"
#include "silan_mcu2risc_regs.h"
#include "silan_types.h"
#include "silan_printf.h"

#define LOCK      __REG32(SCFG_APB_CXC_ADDR_BASE)

u8 Flag_CMDACK_FAIL=0;

void wait_xus(void)
{
	u16 i=0xFF;
	while(i--);
}

u8 SendCmd()
{
    u32 retry = 100000;
    if((LOCK&0x1))
        return 0;
    CXC_RAW_INT_SET |= 0x2;
    while(!(CXC_RAW_INT_STATUS&0x1))
    {
      retry--;
      if(!retry)
                return 0;
    }
    CXC_RAW_INT_CLR |= 0x1;
//  LOCK = 0;
    return 1;
}
u8 SendCmd1()
{
    u32 retry = 100000;
    if((LOCK&0x1))
        return 0;
    CXC_RAW_INT_SET |= 0x8;
    while(!(CXC_RAW_INT_STATUS&0x4))        // zhugang 20180109
    {
      retry--;
      if(!retry)
                return 0;
    }
    CXC_RAW_INT_CLR |= 0x4;
//  LOCK = 0;
    return 1;
}

void mcu2dsp_message(u8 cmd,u8 *para)
{
    int i;
    int retry = 1;
    for(i=0;i<MBOX_SIZE;i++){
        if(i==0)
        {
            __sREG8(CXC_MBOX_BASEADDR, 0x0)    = cmd;
          __sREG8(CXC_MBOX_BASEADDR, 0x40)    = cmd;
        }
        else
            __sREG8(CXC_MBOX_BASEADDR, 0x1*i)    = para[i-1];

    }
    //SendCmd();
    #if 1
    while(1)
    {
        if(SendCmd()==1)
        {

            break;
        }
        else
            retry --;
        if(!retry)
        {

            break;
        }
  }
    #endif
}

void mcu2risc_message(u8 cmd,u8 *para)
{
    int i;
    int retry =1;
    for(i=0;i<MBOX_SIZE;i++){
        if(i==0)
        {
            __sREG8(CXC_MBOX_BASEADDR, 0x0)    = cmd;
          __sREG8(CXC_MBOX_BASEADDR, 0x40)    = cmd;
        }
        else
            __sREG8(CXC_MBOX_BASEADDR, 0x1*i)    = para[i-1];

    }
    //SendCmd1();
    #if 1
    while(1)
    {
        if(SendCmd1()==1)
        {
            break;
        }
        else
            retry --;
        if(!retry)
        {
            break;
        }
  }
    #endif
}

