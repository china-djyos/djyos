// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：ad5697r.c
// 模块描述: 温度检测芯片max31865的驱动
// 模块版本: V1.00
// 创建人员:
// 创建时间: 1/20.2017
// =============================================================================

#include "string.h"
#include "os.h"
#include "iicbus.h"
#include "cpu_peri.h"
#include "ad5697r.h"

// =============================================================================
#define AD5697R_ADDR           0x0D            //设备地址
#define AD5697R_CLK_FRE        (50*1000)      //总线速度，单位Hz

static struct SemaphoreLCB AD5697R_Semp;   //芯片互斥访问保护

//定义IICBUS架构下的IIC设备结构
static struct IIC_Device *ps_AD5697R_Dev = NULL;
static u32 s_AD5697R_Timeout = CN_TIMEOUT_FOREVER;
static bool_t sAD5697RInited = false;

//static const Pin AD5697R_LDAC_pin[] = {
//		{PIO_PD25, PIOD, ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT},
//};

// =============================================================================
void __AD5697R_GpioInit(void)
{
	//一般会将LDAC的引脚配置为低电平，在board.c里面配置
//	PIO_Configure(AD5697R_LDAC_pin,1);
}

// =============================================================================
// 功能：DA2输出
// 参数：DacValue2，DA通道2的DA值
// 返回：true,成功;false,失败
// =============================================================================
bool_t AD5697R_Output1(u16 DacValue1)
{
	u8 cmd,value[2];

    if(!sAD5697RInited)
    {
        return false;
    }

    if(false == Lock_SempPend(&AD5697R_Semp,25*mS))
    {
        return false;
    }

    cmd = 0x31;
    DacValue1 = (DacValue1 << 4) & 0xFFF0;
    value[0] = (DacValue1 & 0xFF00) >> 8;
    value[1] = (DacValue1 & 0x00FF) >> 0;

    IIC_Write(ps_AD5697R_Dev,cmd,value,2,true,s_AD5697R_Timeout);

    Lock_SempPost(&AD5697R_Semp);
    return true;
}

// =============================================================================
// 功能：DA1输出
// 参数：DacValue1，DA通道1的DA值
// 返回：true,成功;false,失败
// =============================================================================
bool_t AD5697R_Output2(u16 DacValue2)
{
	u8 cmd,value[2];

    if(!sAD5697RInited)
    {
        return false;
    }

    if(false == Lock_SempPend(&AD5697R_Semp,25*mS))
    {
        return false;
    }

    cmd = 0x38;
    DacValue2 = (DacValue2 << 4) & 0xFFF0;
    value[0] = (DacValue2 & 0xFF00) >> 8;
    value[1] = (DacValue2 & 0x00FF) >> 0;

    IIC_Write(ps_AD5697R_Dev,cmd,value,2,true,s_AD5697R_Timeout);

    Lock_SempPost(&AD5697R_Semp);
    return true;
}

// =============================================================================
// 功能：DA输出，两通道同时配置
// 参数：DacValue1，DA通道1的DA值
//      DacValue2，DA通道2的DA值
// 返回：true,成功;false,失败
// =============================================================================
bool_t AD5697R_OutputAll(u16 DacValue1,u16 DacValue2)
{
	u8 cmd,value[2];

    if(!sAD5697RInited)
    {
        return false;
    }

    if(false == Lock_SempPend(&AD5697R_Semp,25*mS))
    {
        return false;
    }

    if(DacValue1 == DacValue2)
    {
        cmd = 0x39;
        DacValue2 = (DacValue2 << 4) & 0xFFF0;
        value[0] = (DacValue2 & 0xFF00) >> 8;
        value[1] = (DacValue2 & 0x00FF) >> 0;

        IIC_Write(ps_AD5697R_Dev,cmd,value,2,true,s_AD5697R_Timeout);
    }
    else
    {
    	AD5697R_Output1(DacValue1);
    	AD5697R_Output2(DacValue2);
    }

    Lock_SempPost(&AD5697R_Semp);
    return true;
}

// =============================================================================
// 功能：芯片初始化
// 参数：无
// 返回：true,成功;false,失败
// =============================================================================
bool_t AD5697R_ModuleInit(char *BusName)
{
    static struct IIC_Device s_AD5697R_Dev;
    //GPIO初始化，SDA、SCL已经在IIC中初始化了，此处只需初始化WP即可
    __AD5697R_GpioInit();

    if(NULL == Lock_SempCreate_s(&AD5697R_Semp,1,1,CN_BLOCK_FIFO,"AD5697R_semp"))
        return false;

    //添加FM24CL64到IIC0总线
    if(NULL != IIC_DevAdd_s(&s_AD5697R_Dev,BusName,"AD5697R",
    					AD5697R_ADDR,0,8))
    {
        ps_AD5697R_Dev = &s_AD5697R_Dev;
        IIC_BusCtrl(ps_AD5697R_Dev,CN_IIC_SET_CLK,AD5697R_CLK_FRE,0);
        IIC_BusCtrl(ps_AD5697R_Dev,CN_IIC_SET_POLL,0,0);

        sAD5697RInited = true;
    }

    return sAD5697RInited;
}




