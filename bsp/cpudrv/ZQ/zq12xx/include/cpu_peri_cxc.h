

//软件中断以及核间通信模块

#ifndef CPU_PERI_CXC_H
#define CPU_PERI_CXC_H

#ifdef __cplusplus
extern "C"
#endif

#include "cpu_peri_isr.h"
//M0 接收数据来源

enum EN_M0RevSource
{
    EN_M0_REV_FROM_RISC = 0x00, //数据发送方为RISC
    EN_M0_REV_FROM_DSP,         //数据发送方为DSP
    EN_RISC_REV_FROM_M0,        //Risc接收M0  数据
    EN_RISC_REV_FROM_DSP,       //Risc接收DSP 数据
};

//Soft Int Start

//申请软件中断线

extern u8 SoftIntIrqRequst(void);

//释放软件中断线

extern bool_t SoftIntIrqFree(u8 irqID);

//启动软件中断

extern void SoftIntStart(u8 irqID);

//注册软件中断Usr服务函数

extern bool_t RegisterSoftInt(u8 irqID,djybsp_isr_hdl_t cxc_hdl,u32 para);


/*****************************************************
 * M0 SoftInt Usage Example:
 * M0：
 * 	  softIrq = SoftIntIrqRequst();
 * 	  RegisterSoftInt(softIrq,UsrIsr);
 * 	  where need to softInt call :
 *	  SoftIntStart(softIrq);
 *
 *****************************************************/

//Soft Int End


//以下为核间通信

//M0核接收数据初始化
//fromObj = EN_M0_REV_FROM_RISC M0 从 Risc接收 初始化
//fromObj = EN_M0_REV_FROM_DSP  M0 从 Dsp 接收 初始化

extern void M0ToRevMessageInit(u8 fromObj);

//M0 从 Risc 核接收 数据 - 还没 接收到则调用线程 挂起

extern u8 M0ToRevRiscData(u32 *buf,u8 len);

//Risc 向 M0发送 数据

extern void RiscSendToM0(u32 *buf,u8 len);

//M0 接收 Dsp 数据

extern u8 M0ToRevDspData(u32 *buf,u8 len);

/*****************************************************
 * M0 Rev Usage Example:
 * Risc Send To M0：
 * Risc:
 *	    RiscSendToM0(buf,len);
 *
 * M0 :
 *	    M0ToRevMessageInit(EN_M0_REV_FROM_RISC);
 *	    M0ToRevRiscData(buf,len);
 *
 *****************************************************/


//Risc 接收 初始化
//fromObj = EN_RISC_REV_FROM_M0  接收 M0  数据初始化
//fromObj = EN_RISC_REV_FROM_DSP 接收Dsp  数据初始化

extern void RiscToRevMessageInit(u8 fromObj);

//M0 向Risc 发送数据

extern u8 M0SendToRisc(u32 *buf,u8 len);

//Risc 接收M0 数据

extern u8 RiscToRevM0Data(u32 *buf,u8 len);

//Risc 接收Dsp 数据

extern u8 RiscToRevDspData(u32 *buf,u8 len);

/*****************************************************
 * Risc Rev Usage Example:
 * M0 Send To Risc：
 * M0 :
 * 		M0SendToRisc(buf,len);
 * Risc:
 *      RiscToRevMessageInit(EN_RISC_REV_FROM_M0);
 *		RiscToRevM0Data(buf,len);
 *
 *****************************************************/

#ifdef __cplusplus
}
#endif

#endif




















































