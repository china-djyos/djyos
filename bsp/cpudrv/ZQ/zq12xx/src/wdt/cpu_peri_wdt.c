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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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

// �ļ���     ��cpu_peri_spi.c
// ģ������: SPIģ��ײ�Ӳ������ģ��
// ģ��汾: V1spi
// ������Ա:
// ����ʱ��:
// =============================================================================
#ifdef CFG_CPU_ZQ12XX_M0
#include "stdint.h"
#include "stddef.h"
#include "systime.h"
#include "wdt_hal.h"
#include "cpu_peri.h"

typedef  bool_t (*fnWdtChip_Feed)(void);

//����ι�����Ź�����
#define CN_WDT_WDTCYCLE (0xffffffff)

//ϵͳ�������
#define SYS_CTR2        *((vu32*)(0x4204000c))
#define CTR_REG2        *((vu32*)(0x42010000 + 0x08))

typedef struct _WatDog_{

    vu32 WDOGLOAD;
    vu32 WDOGVALUE;
    vu32 WDOGCONTROL;
    vu32 WDOGINTCLR;
    vu32 WDOGRIS;
    vu32 WDOGMIS;

}tagWatDogReg;

#define  BASE_WDT_ADDR (0x41870000)

static tagWatDogReg* WatReg[1] = {
    (tagWatDogReg*)BASE_WDT_ADDR,
};

//watDog lock reg
typedef struct _WatDog_Lock_{
    vu32 WDOGLOCK;
}tagWatDogLock;

#define WDT_LOCK_ADDR   (BASE_WDT_ADDR + 0xC00)

static tagWatDogLock* WatLockReg[1] = {

    (tagWatDogLock*)WDT_LOCK_ADDR,
};


// =============================================================================
//����: �������Ź�
//��������
//���أ���
// =============================================================================

void WDT_Start(void)
{
    tagWatDogReg  *Reg     = NULL;
    tagWatDogLock *LockReg = NULL;
    u32 tmp;
    printk("start watDog\r\n");

    Reg     = (tagWatDogReg  *)WatReg[0];
    LockReg = (tagWatDogLock *)WatLockReg[0];

    //first to open the lock
    LockReg->WDOGLOCK = 0x1ACCE551;

    //enable counter and interrupt
    Reg->WDOGCONTROL |= (1<<0);

    //enable reset
    Reg->WDOGCONTROL |= (1<< 1);

    //read diable again
    LockReg->WDOGLOCK = 0x38;
    tmp = LockReg->WDOGLOCK;
    printk("WDT_Start: %d\r\n",tmp);
    //wait for lock again
    //while(!(tmp & 0x00000001 == 0x00000001));

    printk("start dog:%x\r\n",Reg->WDOGVALUE);

}

// =============================================================================
//����: �����λ��
//��������
//���أ���
//ע��: ͨ�����ø�λ����ʵ�֣������ĸ�λ����������������ʵ��
// =============================================================================
void WDT_Reboot(void)
{
    while(1);//��ʱ�򣬿��Ե��ø�λ��������������while(1)����
}

// =============================================================================
//����: ���Ź�ι������
//��������
//���أ���
// =============================================================================

bool_t WDT_WdtFeed(void)
{
    tagWatDogReg  *Reg     = NULL;
    tagWatDogLock *LockReg = NULL;
    u32 tmp = 0;
    u32 setVal;

    if(setVal == 0){

        setVal  = 0xffffffff;
    }

    //first open the lock and then to write
    LockReg->WDOGLOCK = 0x1ACCE551;
    printk("feed dog %x\r\n",LockReg->WDOGLOCK);

    Reg     = WatReg[0];
    LockReg = WatLockReg[0];

    printk("WDT FEED!\r\n");

    //reset the reload value
    Reg->WDOGLOAD   = setVal;

    //write anything to clear int flag
    Reg->WDOGINTCLR = 0x38;
    //read diable again
    tmp = LockReg->WDOGLOCK;
    //wait for lock again
    //while(!(tmp & 0x00000001 == 0x00000001));

    printk("feed dog %x\r\n",setVal);
    printk("---->%d\r\n",tmp);

    return true;
}


// =============================================================================
//����: ���Ź�Ӳ����ʼ��
//��������
//���أ���
//ע��: �����û��Ŀ��Ź���ʼ����Ȼ���djyos�ṩ����������
// =============================================================================

static void WDT_HardInit(void)
{
    u8 WatDogCnt;
    tagWatDogReg  *Reg     = NULL;
    tagWatDogLock *LockReg = NULL;
    u32 tmp;

    //enable watDog clock
    CTR_REG2 |= (1<<26);
    //enable watDog to reset system
    SYS_CTR2 |= (1<<28);

    //disable all the WatDog

    Reg      =   WatReg[0];
    LockReg  =   WatLockReg[0];

    //first open the lock and then to write
    LockReg->WDOGLOCK = 0x1ACCE551;

    Reg->WDOGLOAD = 0x00000f00;
    printk("WatDog%d WDT_HardInit set wdtLoad %x\r\n",WatDogCnt,Reg->WDOGLOAD);
    printk("WatDog%d WDT_HardInit current watDog count value %x\r\n",WatDogCnt,Reg->WDOGVALUE);

    //diable watdog
    Reg->WDOGCONTROL &= ~((1<<0) | (1<<1));

    //read diable again
    LockReg->WDOGLOCK = 0x38;
    tmp = LockReg->WDOGLOCK;
    //wait for lock again
    while(!(tmp & 0x00000001 == 0x00000001));
    printk("WatDog%d WDT_HardInit: %d\r\n",WatDogCnt,tmp);

}

//**************************************************//
//todo����������������ʵ�ֱ������ع��̣�ʹ�õ�ʱ���貹ȫ
u32 __FeedDog_Isr(ptu32_t intline)
{
    return false;
}
bool_t __BrdBoot_FeedStart(u32 bootfeedtime)
{
    return false;
}

bool_t __BrdBoot_FeedEnd(void)
{
    return false;
}
//**************************************************//

// =============================================================================
// ��������: ���Ź�ע��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================

bool_t WatDog_Install(void)
{
    bool_t result;

    WDT_HardInit();

//  result = WdtHal_RegisterWdtChip(pgWatDogName[0],CN_WDT_WDTCYCLE,g_WatDogFeedHandle[0],NULL,NULL);
    result = WdtHal_RegisterWdtChip("zqM0WatDog1", CN_WDT_WDTCYCLE, WDT_WdtFeed);
#if(CFG_DEFEND_ON_BOOT == true)
    __BrdBoot_FeedEnd();
#endif

   return result;

}

#endif














































































































































