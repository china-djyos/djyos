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
// =============================================================================

// �ļ���     ��MMA8451Q.c
// ģ������: ���ٶȴ��������������õ���IIC�Ľӿ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 18/09.2014
// =============================================================================

#include "stdint.h"
#include "iicbus.h"
#include "cpu_peri.h"
#include <mma8451q_config.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern ptu32_t MMA8541Q_ModuleInit(void);
//    MMA8541Q_ModuleInit();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"3 axis accelerometer mma8451q"//IIC�ӿڵ�3����ٶȴ�����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","iicbus","cpu onchip iic"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_3_AXIS_ACCELEROMETER_MMA8451Q == false )
//#warning  " 3_axis_accelerometer_mma8451q  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_3_AXIS_ACCELEROMETER_MMA8451Q    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
#define CFG_MMA_ADDRESS                0x50      //"�豸��ַ",IIC�����豸��ַ
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_MMA_BUS_NAME              "IIC0"    //"��������",mma8451ʹ�õ�IIC��������
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

//�Ĵ������壨���֣�
#define F_STATUS        0x00
#define OUT_X_MSB       0x01
#define OUT_X_LSB       0x02
#define OUT_Y_MSB       0x03
#define OUT_Y_LSB       0x04
#define OUT_Z_MSB       0x05
#define OUT_Z_LSB       0x06
#define WHO_AM_I        0x0D

//����IICBUS�ܹ��µ�IIC�豸�ṹ
static struct IIC_Device *ps_MMA_Dev = NULL;
static u32 s_MMA_Timeout = CN_TIMEOUT_FOREVER;

//#define CFG_MMA_ADDRESS         0x50            //�豸��ַ
#define MMA_CLK_FRE         (100*1000)      //�����ٶȣ���λHz

// =============================================================================
// ���ܣ������ٶ����ļĴ������ڲ����ã�
// ������RegAddr,�Ĵ�����ַ��8λ��
//       pbyBuf,��ȡ����洢�ռ�
//       wBytesNum,��ȡ���ݵĳ���
// ���أ�true����ȡ�ɹ�;false����ȡʧ��
// =============================================================================
bool_t __MMA8541Q_ReadReg(u8 RegAddr, u8 *pbyBuf, u16 wBytesNum)
{
    if(wBytesNum == IIC_Read(ps_MMA_Dev,RegAddr,pbyBuf,wBytesNum,s_MMA_Timeout))
        return true;
    else
        return false;
}

// =============================================================================
// ���ܣ������ٶ�����ID
// ��������
// ���أ���ȡ���
// =============================================================================
u8 MMA8541Q_ID_Read(void)
{
    u8 pbyBuf,RegAddr,wBytesNum;

    pbyBuf = 0;
    RegAddr = WHO_AM_I;
    wBytesNum = 1;

    if(true == __MMA8541Q_ReadReg(RegAddr,&pbyBuf,wBytesNum))
        return pbyBuf;
    else
        return 0;
}

// =============================================================================
// ���ܣ������ٶ�����״̬�Ĵ���
// ��������
// ���أ���ȡ���
// =============================================================================
u8 MMA8541Q_StatusRead(void)
{
    u8 pbyBuf,RegAddr,wBytesNum;

    pbyBuf = 0;
    RegAddr = F_STATUS;
    wBytesNum = 1;

    if(true == __MMA8541Q_ReadReg(RegAddr,&pbyBuf,wBytesNum))
        return pbyBuf;
    else
        return 0;
}

// =============================================================================
// ���ܣ������ٶ�����X�᷽��ֵ
// ��������
// ���أ���ȡ���
// =============================================================================
u16 MMA8541Q_X_Read(void)
{
    u8 pbyBuf[2],RegAddr,wBytesNum;

    pbyBuf[0] = 0;
    pbyBuf[1] = 0;
    RegAddr = OUT_X_MSB;
    wBytesNum = 2;

    if(true == __MMA8541Q_ReadReg(RegAddr,pbyBuf,wBytesNum))
        return (u16)(pbyBuf[1]<<8 + pbyBuf[0]);
    else
        return 0;
}

// =============================================================================
// ���ܣ������ٶ�����Y�᷽��ֵ
// ��������
// ���أ���ȡ���
// =============================================================================
u16 MMA8541Q_Y_Read(void)
{
    u8 pbyBuf[2],RegAddr,wBytesNum;

    pbyBuf[0] = 0;
    pbyBuf[1] = 0;
    RegAddr = OUT_Y_MSB;
    wBytesNum = 2;

    if(true == __MMA8541Q_ReadReg(RegAddr,pbyBuf,wBytesNum))
        return (u16)(pbyBuf[1]<<8 + pbyBuf[0]);
    else
        return 0;
}
// =============================================================================
// ���ܣ������ٶ�����Z�᷽��ֵ
// ��������
// ���أ���ȡ���
// =============================================================================
u16 MMA8541Q_Z_Read(void)
{
    u8 pbyBuf[2],RegAddr,wBytesNum;

    pbyBuf[0] = 0;
    pbyBuf[1] = 0;
    RegAddr = OUT_Z_MSB;
    wBytesNum = 2;

    if(true == __MMA8541Q_ReadReg(RegAddr,pbyBuf,wBytesNum))
        return (u16)(pbyBuf[1]<<8 + pbyBuf[0]);
    else
        return 0;
}
// =============================================================================
// ���ܣ�MMAоƬ��ʼ��������WPд�����������ų�ʼ���ͼ����豸����Ӧ��IIC����
// ��������
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================

ptu32_t MMA8541Q_ModuleInit(void)
{
    bool_t result = false;
    //GPIO��ʼ����SDA��SCL�Ѿ���IIC�г�ʼ���ˣ��˴�ֻ���ʼ��WP����
//  __MMA_GpioInit();

    //���MMA8451��IIC0����
    if(ps_MMA_Dev = IIC_DevAdd(CFG_MMA_BUS_NAME,"IIC_Dev_MMA8451Q",CFG_MMA_ADDRESS,0,8))
    {
        IIC_BusCtrl(ps_MMA_Dev,CN_IIC_SET_CLK,MMA_CLK_FRE,0);
        result = true;
    }

    return result;
}
