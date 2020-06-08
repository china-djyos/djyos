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

// �ļ���     ��SC1161Y_spi.c
// ģ������:SC1161Y����оƬ�ӿ�����
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 8/10.2017
// =============================================================================

#include "stdint.h"
#include "string.h"
#include "cpu_peri.h"
#include "spibus.h"
#include "board-config.h"
#include "os.h"
#include "systime.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_SC1161Y(void);
//    ModuleInstall_SC1161Y();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"SC1161Y"//spi�ӿڵļ���оƬ
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","spi bus","cpu onchip spi"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_SC1161Y == false )
//#warning  " SC1161Y  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_SC1161Y    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#define CFG_SC1161Y_BUS_NAME              ""      //"��������",������SPI�������ƣ���"SPI0"
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

// =============================================================================
static struct SPI_Device *s_ptSc1161yDev;
static struct SemaphoreLCB sSc1161ySemp;   //оƬ������ʱ���

#define SC1161Y_SPI_SPEED       (5*1000*1000)//����оƬ������5M�ٶ�
#define SC1161Y_TIMEOUT         (3000*mS)   //�ֲ�����д�����3S

#define RD_DATA_HEAD_LEN   5
// =============================================================================
//���ܣ�SPI�ӿڵ�Ƭѡʹ�ܣ�SC1161YƬѡʹ�ܺ�����ʱ50us�ſɶ�д����
//��������
//���أ���
// =============================================================================
void SC1161Y_CsActive(void)
{
    SPI_CsActive(s_ptSc1161yDev,SC1161Y_TIMEOUT);
    DJY_DelayUs(50);
}

// =============================================================================
//���ܣ�SPI�ӿڵ�Ƭѡ���ܣ�SC1161YƬѡ���ܺ�����ʱ10us�ſ�����Ƭѡ
//��������
//���أ���
// =============================================================================
void SC1161Y_CsInActive(void)
{
    SPI_CsInactive(s_ptSc1161yDev);
    DJY_DelayUs(10);
}
// =============================================================================
//���ܣ�SPI��д��������ΪSPI�Ƕ�дͬʱ���еģ���˶�д����һ��
//������byWrBuf,д���ݻ�����ָ�룬��д�����ݿ�дΪNULL
//     dwWrLen,д�����ֽ�������д�����ݿ�дΪ0
//     byRdBuf,�����ݻ�����ָ�룬�޶�ȡ���ݿ�дΪNULL
//     dwRdLen���������ֽ������޶�ȡ���ݿ�д0
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
// =============================================================================
//bool_t SC1161Y_SpiTransfer(u8* byWrBuf,u32 dwWrLen,u8* byRdBuf, u32 dwRdLen)
//{
//  struct SPI_DataFrame data;
//    s32 Ret = false;
//
//  if(Lock_SempPend(&sSc1161ySemp,SC1161Y_TIMEOUT))
//  {
//      SC1161Y_CsActive();
//
//      data.RecvBuf = byRdBuf;
//      data.RecvLen = dwRdLen;
//      data.RecvOff = dwWrLen;
//      data.SendBuf = byWrBuf;
//      data.SendLen = dwWrLen;
//
//      Ret = SPI_Transfer(s_ptSc1161yDev,&data,true,SC1161Y_TIMEOUT);
//      if(Ret != CN_SPI_EXIT_NOERR)
//          Ret = false;
//      else
//          Ret = true;
//
//      SC1161Y_CsInActive();
//      Lock_SempPost(&sSc1161ySemp);
//  }
//  return Ret;
//}

static bool_t __SC1161Y_SpiTransfer(u8* byWrBuf,u32 dwWrLen,u8* byRdBuf, u32 dwRdLen)
{
    bool_t Ret = false;
    struct SPI_DataFrame data;

    data.RecvBuf = byRdBuf;
    data.RecvLen = dwRdLen;
    data.RecvOff = dwWrLen;
    data.SendBuf = byWrBuf;
    data.SendLen = dwWrLen;

    Ret = SPI_Transfer(s_ptSc1161yDev,&data,true,SC1161Y_TIMEOUT);
    if(Ret != CN_SPI_EXIT_NOERR)
        Ret = false;
    else
        Ret = true;
    return Ret;
}

// =============================================================================
//���ܣ�д����оƬ
//������byWrBuf,д���ݻ�����ָ�룬����дΪNULL
//     dwWrLen,д�����ֽ���������дΪ0
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
// =============================================================================
bool_t SC1161Y_Write(u8* byWrBuf,u32 dwLen)
{
    bool_t Ret = false;

    if( (NULL == byWrBuf) || (0 == dwLen) )
        return Ret;
    if(Lock_SempPend(&sSc1161ySemp,SC1161Y_TIMEOUT))
    {
        SC1161Y_CsActive();
        Ret = __SC1161Y_SpiTransfer(byWrBuf,dwLen,NULL,0);
        SC1161Y_CsInActive();
        Lock_SempPost(&sSc1161ySemp);
    }

    return Ret;
}
// =============================================================================
//���ܣ�������оƬ
//������pbRdBuf,�����ݻ�����ָ�룬����дΪNULL
//     timeout����ʱʱ��
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
// =============================================================================
u32 SC1161Y_Read(u8* pbRdBuf,u32 timeout)
{
    bool_t Ret = false;
    u16 DataLen = 0,i;
    u32 Timeout = SC1161Y_TIMEOUT;
    u8 sRdData[RD_DATA_HEAD_LEN];

    if(NULL == pbRdBuf)
        return DataLen;
//  DJY_EventDelay(timeout);
    if(Lock_SempPend(&sSc1161ySemp,SC1161Y_TIMEOUT))
    {
        SC1161Y_CsActive();
        while(1)
        {
            Ret = __SC1161Y_SpiTransfer(NULL,0,sRdData,RD_DATA_HEAD_LEN);
            for(i = 0; i < RD_DATA_HEAD_LEN; i++)
            {
                if(sRdData[i] == 0x55)
                {
                    memcpy(pbRdBuf,&sRdData[i],RD_DATA_HEAD_LEN-i);

                    Ret = __SC1161Y_SpiTransfer(NULL,0,sRdData,i);
                    memcpy(pbRdBuf+RD_DATA_HEAD_LEN-i,sRdData,i);
                    DataLen =  (pbRdBuf[3] << 8) + pbRdBuf[4];
                    Ret = __SC1161Y_SpiTransfer(NULL,0,pbRdBuf+RD_DATA_HEAD_LEN,DataLen + 1);
                    DataLen = DataLen + RD_DATA_HEAD_LEN + 1;
                    break;
                }
            }
            if( (i != RD_DATA_HEAD_LEN) || (Timeout < 100) || (Ret != true))
                break;
            DJY_EventDelay(100);
            Timeout -= 100;
        }

        SC1161Y_CsInActive();
        Lock_SempPost(&sSc1161ySemp);
    }
    return DataLen;
}
// =============================================================================
//���ܣ�ע�����оƬ��SPI�����ϣ�������SPI�Ĺ���ģʽ�������ٶȵȡ�
//�������������ƣ��硰SPI1��
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
// =============================================================================
bool_t ModuleInstall_SC1161Y(void)
{
    s_ptSc1161yDev = SPI_DevAdd(CFG_SC1161Y_BUS_NAME,"SC1161Y",1,8,SPI_MODE_3,
                                SPI_SHIFT_MSB,SC1161Y_SPI_SPEED,false);
    if(NULL != s_ptSc1161yDev)
    {
        SPI_BusCtrl(s_ptSc1161yDev,CN_SPI_SET_POLL,0,0);
    }

    if(NULL == Lock_SempCreate_s(&sSc1161ySemp,1,1,CN_BLOCK_FIFO,"SC1161Y_SEMP"))
        return false;

    return true;
}
