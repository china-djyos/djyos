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

// �ļ���     ��at24c02a.c
// ģ������: EEPROMоƬ������������IICBUS��������ģ��
// ģ��汾: V1.00
// ������Ա: huzb
// ����ʱ��: 02/08.2019
// =============================================================================

#include "stdint.h"
#include "os.h"
#include "iicbus.h"
#include "cpu_peri.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t AT24_ModuleInit(void);
//    AT24_ModuleInit();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"at24c08"     //iic�ӿ�eeprom
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","iicbus","cpu oncip iic"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_AT24C08 == false )
//#warning  " at24c08  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_AT24C08    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,0xFFFFFFFF,
#define CFG_AT24_TIMEOUT           (-1)   //"��ʱʱ��",-1��ʾ����
//%$#@enum,0xA0,0xA2,0xA4,0xA6,0xA8,0xAC,0xAA,0xAE
#define CFG_AT24_ADDRESS            0xA0                //"�豸��ַ",Ӳ������AT24��IIC�豸��ַ
//%$#@num,,
#define CFG_AT24_CLK_FRE           (100*1000)           //"�����ٶ�",��λHz

//%$#@string,1,10,
#define CFG_AT24C08_BUS_NAME       "I2C0"        //"name",AT24ʹ�õ�����
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

// =============================================================================
#define CN_AT24_CHIP_SIZE       (1024)              //оƬ��С 1K Bytes
#define CN_AT24_PAGE_SIZE       (16)                //оƬҳ��С16 Bytes
#define CN_AT24_PAGE_SUM        (CN_AT24_CHIP_SIZE/CN_AT24_PAGE_SIZE)

#define AT24_ADDRESS            0xA0            //�豸��ַ ???δ�ҵ�,��Ҫȷ��  //TODO
#define AT24_CLK_FRE           (100*1000)      //�����ٶȣ���λHz

static struct SemaphoreLCB AT24_Semp;   //оƬ������ʱ���

//����IICBUS�ܹ��µ�IIC�豸�ṹ
static struct IIC_Device *s_ptAT24_Dev = NULL;
static u32 s_AT24_Timeout = CN_TIMEOUT_FOREVER;

// =============================================================================
// ���ܣ�AT24оƬWPд�������ų�ʼ��������Ϊ��ʱ������д��Ϊ��ʱ��д����
// ��������
// ���أ���
// =============================================================================
void __AT24_GpioInit(void)
{

}

__attribute__((weak))  void AT24_WPEn(u8 En)
{

}

// =============================================================================
// ���ܣ�AT24оƬ��ҳд,��ַ��Χֻ�����ڸ�ҳ��
// ������wAddr:����оƬ��Ƭ�ڵ�ַ
//       pbyBuf:������
//       wBytesNum:�����ֽ���
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
u32 __AT24_PageProgram(u32 wAddr, u8 *pbyBuf, u32 wBytesNum)
{
    u32 DelayMs;

    if(((wAddr%CN_AT24_PAGE_SIZE) + wBytesNum) > CN_AT24_PAGE_SIZE)
        return 0;

    IIC_Write(s_ptAT24_Dev,wAddr,pbyBuf,wBytesNum,true,s_AT24_Timeout);

    //AT24�ڲ�дʱ�䣬���Ϊ5mS
    DelayMs = 1000 / CN_CFG_TICK_HZ;
    if(DelayMs >= 6)
        DelayMs = DelayMs + 1;
    else
        DelayMs = 6;

    DJY_EventDelay(DelayMs*mS);

    return wBytesNum;
}

// =============================================================================
// ���ܣ�AT24оƬ��ҳ��,��ַ��Χֻ�����ڸ�ҳ��
// ������wAddr:����оƬ��Ƭ�ڵ�ַ
//       pbyBuf:������
//       wBytesNum:�����ֽ���
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
u32 __AT24_PageRead(u32 wAddr, u8 *pbyBuf, u32 wBytesNum)
{
    u32 DelayMs;

    if(((wAddr%CN_AT24_PAGE_SIZE) + wBytesNum) > CN_AT24_PAGE_SIZE)
        return 0;

    IIC_Read(s_ptAT24_Dev,wAddr,pbyBuf,wBytesNum,s_AT24_Timeout);

    //AT24�ڲ�дʱ�䣬���Ϊ5mS
    DelayMs = 1000 / CN_CFG_TICK_HZ;
    if(DelayMs >= 6)
        DelayMs = DelayMs + 1;
    else
        DelayMs = 6;

    DJY_EventDelay(DelayMs*mS);

    return wBytesNum;
}

// =============================================================================
// ���ܣ�AT24оƬд������EEPROM��ҳ������ÿ�ζ�д���ܳ���ҳ��С
// ������wAddr:����оƬ��Ƭ�ڵ�ַ
//       pbyBuf:������
//       wBytesNum:�����ֽ���
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
u32 AT24_WriteBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum)
{
    u32 offset = 0,wSize = 0;

    //�ж��Ƿ񳬳�оƬ��ַ��Χ
    if((wAddr + wBytesNum-1) >= CN_AT24_CHIP_SIZE)
        return false;

    if(false == Lock_SempPend(&AT24_Semp,2500*mS))// TODO --- 10*mS
    {
        return false;
    }
    AT24_WPEn(0);
    //��ҳ����
    while(wBytesNum)
    {
        offset = wAddr % CN_AT24_PAGE_SIZE;              //��ʼд���ַƫ�뱾ҳ�ֽ���
        wSize = (wBytesNum > CN_AT24_PAGE_SIZE - offset)?//��Ҫд���ֽڸ�ҳ�Ƿ���д��
                (CN_AT24_PAGE_SIZE - offset): wBytesNum; //��ҳд�ֽ���
        __AT24_PageProgram(wAddr,pbyBuf,wSize);          //д��ҳ
        wAddr += wSize;                                  //д��ַ����
        pbyBuf += wSize;                                 //����������
        wBytesNum -= wSize;                              //д���ȵݼ�
    }
    AT24_WPEn(1);
    Lock_SempPost(&AT24_Semp);

    return true;
}

// =============================================================================
// ���ܣ�AT24оƬ��������EEPROM��ҳ������ÿ�ζ�д���ܳ���ҳ��С
// ������wAddr:����оƬ��Ƭ�ڵ�ַ
//       pbyBuf:������
//       wBytesNum:�����ֽ���
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
u32 AT24_ReadBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum)
{
    u16 offset = 0,rSize = 0;

    //�ж��Ƿ񳬳�оƬ��ַ��Χ
    if((wAddr + wBytesNum-1) >= CN_AT24_CHIP_SIZE)
        return false;

    if(false == Lock_SempPend(&AT24_Semp,2500*mS))//TODO
    {
        return false;
    }

    //��ҳ����
    while(wBytesNum)
    {
        offset = wAddr % CN_AT24_PAGE_SIZE;             //��ҳƫ���ֽ���
        rSize = (wBytesNum > CN_AT24_PAGE_SIZE - offset)?
                (CN_AT24_PAGE_SIZE - offset): wBytesNum;//��ҳ���ֽ���
        __AT24_PageRead(wAddr,pbyBuf,rSize);            //����ҳ
        wAddr += rSize;                                 //����ַ����
        pbyBuf += rSize;                                //����������
        wBytesNum -= rSize;                             //�����ȵݼ�
    }
    Lock_SempPost(&AT24_Semp);
    return true;
}

// =============================================================================
// ���ܣ�д�ֵ�ָ����ַ
// ������wAddr,��ַ
//       wValue,��ֵ��16������ֵ
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
u16 AT24_WriteWord(u32 wAddr,u16 wValue)
{
    u8 pbyBuf[2];
    pbyBuf[0]=wValue;
    pbyBuf[1]=wValue>>8;
    if(true == AT24_WriteBytes(wAddr,pbyBuf,2))
        return true;
    else
        return false;
}

// =============================================================================
// ���ܣ�����
// ������wAddr,��ַ
// ���أ�wValue,�ɹ�;-1,ʧ��
// =============================================================================
u16 AT24_ReadWord(u32 wAddr)
{
    u8 pbyBuf[2];
    u16 wValue;

    if(true == AT24_ReadBytes(wAddr, pbyBuf,2))
    {
        wValue=pbyBuf[0]+(pbyBuf[1]<<8);
        return wValue;
    }
    else
        return -1;
}


// =============================================================================
// ���ܣ�AT24оƬ��ʼ��������WPд�����������ų�ʼ���ͼ����豸����Ӧ��IIC����
// ��������
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t AT24_ModuleInit(void)
{
    bool_t result = false;
    //GPIO��ʼ����SDA��SCL�Ѿ���IIC�г�ʼ���ˣ��˴�ֻ���ʼ��WP����
    __AT24_GpioInit();

    if(NULL == Lock_SempCreate_s(&AT24_Semp,1,1,CN_BLOCK_FIFO,"AT24 Semp"))
        return false;

    //���AT24��IIC0����
    s_ptAT24_Dev = IIC_DevAdd(CFG_AT24C08_BUS_NAME,"at24c08",AT24_ADDRESS, 2, 10);
    if(s_ptAT24_Dev)
    {
        IIC_BusCtrl(s_ptAT24_Dev,CN_IIC_SET_CLK,AT24_CLK_FRE,0);
        IIC_BusCtrl(s_ptAT24_Dev,CN_IIC_SET_POLL,0,0);
        result = true;
    }
    else
    {
        Lock_SempDelete_s(&AT24_Semp);
        result = false;
    }

    return result;
}


