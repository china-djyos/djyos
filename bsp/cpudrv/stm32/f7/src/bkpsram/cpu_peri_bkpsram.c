//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ:
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ��������:������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================

// �ļ���     ��brdexp.c
// ģ������: �쳣ģ��洢��������AT45�����1M����Ϊ�쳣�洢
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 12/03.2016
// =============================================================================
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <os.h>
#include <stm32f7xx_hal.h>

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t BkpSramInit(void);
//    BkpSramInit( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip Backup area sram"//��ر��ֵ�4KƬ��ram��д
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                      //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г�
//weakdependence:"none"                 //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                        //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_BACKUP_AREA_SRAM == false )
//#warning  " cpu_onchip_Backup_area_sram  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_BACKUP_AREA_SRAM    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure


//BKPSRAM:
//1,in stm32f7xx,addr��0X40024000 size��4KB
//2,will retain its data only if the BACKUP POWER IS ON

#define CN_BKPSRAM_SIZE    0X1000               //4KB
#define CN_BKPSRAM_ADDR    ((u8 *)0X40024000)   //BKPSRAM

//enable the access of bkpsram
static void __BkpsramEnable(void)
{
    HAL_PWR_EnableBkUpAccess();
}

static void __BkpsramDisable(void)
{
    HAL_PWR_DisableBkUpAccess();
}
//first to make the bkpsram to could be used
bool_t BkpSramInit(void)
{
    //step 1:Enable the PWR clock
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_SLEEP_ENABLE();

    //step 2:Enable access to the backup domain
    HAL_PWR_EnableBkUpAccess();

    //step 3:Enable backup SRAM Clock
    __HAL_RCC_BKPSRAM_CLK_ENABLE();
    __HAL_RCC_BKPSRAM_CLK_SLEEP_ENABLE();
    //step 4:Enable the Backup SRAM low power Regulator to retain it's content in VBAT mode
    HAL_PWREx_EnableBkUpReg();

    //step 5:check the vbat is OK
    int timeout =100;
    while((!(__HAL_PWR_GET_FLAG(PWR_FLAG_BRR)))&&(timeout > 0))
    {
        timeout--;
        DJY_DelayUs(1*mS);
    }
    if(timeout <=0)
    {
        printf("bkpsram init timeout:vbat ready wait timeout\n\r");
    }
    //step 6:disable the access here to protect the sram
    __BkpsramDisable();
    return true;
}

//use the  enable and disable access to protect the bkpsram
bool_t BkpSramRead(u16 offset,void *buf,u16 len)
{
    bool_t ret = false;
    if((offset < CN_BKPSRAM_SIZE)&&(len <=CN_BKPSRAM_SIZE)&&((offset + len) <= CN_BKPSRAM_SIZE))
    {
        __BkpsramEnable();
        memcpy(buf,CN_BKPSRAM_ADDR+offset,len);
        __BkpsramDisable();
        ret = true;
    }
    return ret;
}
//write the bkpsram len bytes to addr from buf
bool_t BkpSramWrite(u16 offset,void *buf,u16 len)
{
    bool_t ret = false;
    if((offset < CN_BKPSRAM_SIZE)&&(len <=CN_BKPSRAM_SIZE)&&((offset + len) <= CN_BKPSRAM_SIZE))
    {
        __BkpsramEnable();
        memcpy(CN_BKPSRAM_ADDR+offset,buf,len);
        __BkpsramDisable();
    }
    return ret;
}
//zero bkpsram
bool_t  BkpSramFormat(u16 offset,u16 len)
{
    __BkpsramEnable();
    memset(CN_BKPSRAM_ADDR+offset,0,len);
    __BkpsramDisable();
    return true;
}

u32 BkpSramSize(void)
{
    return CN_BKPSRAM_SIZE;
}



