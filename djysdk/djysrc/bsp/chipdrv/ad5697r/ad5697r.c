// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��ad5697r.c
// ģ������: �¶ȼ��оƬmax31865������
// ģ��汾: V1.00
// ������Ա:
// ����ʱ��: 1/20.2017
// =============================================================================

#include "string.h"
#include "os.h"
#include "iicbus.h"
#include "cpu_peri.h"
#include "ad5697r.h"

// =============================================================================
#define AD5697R_ADDR           0x0D            //�豸��ַ
#define AD5697R_CLK_FRE        (50*1000)      //�����ٶȣ���λHz

static struct SemaphoreLCB AD5697R_Semp;   //оƬ������ʱ���

//����IICBUS�ܹ��µ�IIC�豸�ṹ
static struct IIC_Device *ps_AD5697R_Dev = NULL;
static u32 s_AD5697R_Timeout = CN_TIMEOUT_FOREVER;
static bool_t sAD5697RInited = false;

//static const Pin AD5697R_LDAC_pin[] = {
//		{PIO_PD25, PIOD, ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT},
//};

// =============================================================================
void __AD5697R_GpioInit(void)
{
	//һ��ὫLDAC����������Ϊ�͵�ƽ����board.c��������
//	PIO_Configure(AD5697R_LDAC_pin,1);
}

// =============================================================================
// ���ܣ�DA2���
// ������DacValue2��DAͨ��2��DAֵ
// ���أ�true,�ɹ�;false,ʧ��
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
// ���ܣ�DA1���
// ������DacValue1��DAͨ��1��DAֵ
// ���أ�true,�ɹ�;false,ʧ��
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
// ���ܣ�DA�������ͨ��ͬʱ����
// ������DacValue1��DAͨ��1��DAֵ
//      DacValue2��DAͨ��2��DAֵ
// ���أ�true,�ɹ�;false,ʧ��
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
// ���ܣ�оƬ��ʼ��
// ��������
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t AD5697R_ModuleInit(char *BusName)
{
    static struct IIC_Device s_AD5697R_Dev;
    //GPIO��ʼ����SDA��SCL�Ѿ���IIC�г�ʼ���ˣ��˴�ֻ���ʼ��WP����
    __AD5697R_GpioInit();

    if(NULL == Lock_SempCreate_s(&AD5697R_Semp,1,1,CN_BLOCK_FIFO,"AD5697R_semp"))
        return false;

    //����FM24CL64��IIC0����
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



