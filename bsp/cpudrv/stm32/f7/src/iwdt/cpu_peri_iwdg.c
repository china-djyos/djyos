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
#include "stdint.h"
#include "stddef.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "shell.h"
#include <math.h>//����ָ�������ȣ�pow(a,b),a��b�η�
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_BrdWdt(void);
//    ModuleInstall_BrdWdt();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip iwdt"//�����
//parent:"watch dog"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:pre-main               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"watch dog"        //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_IWDT == false )
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_IWDT    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100000000,
#define CFG_BOOT_TIME_LIMIT         30000000        //"�������س���ʱ��",�������������ع��̲���Ҫ���ô���
//%$#@enum,4,8,16,32,64,128,256
#define CFG_IWDG_PRESCALER           128             //iwdg�ķ���ϵ����
//%$#@num,0,4095,
#define CFG_RELOAD_CYCLE             4000            //�Զ���װ��ֵ,���Ź����ʱ��ΪTout=((4�� 2^prer) �� rlr) /32
//%$#@free,
#define CFG_WDT_TIM                 TIM12    //���������õ��Ķ�ʱ��
//%$#@enum,true,false,
#define CFG_DEFEND_ON_BOOT           false          //"������������",�������ع�������������������Ź�����λ
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


// BOARD WDT
// =============================================================================
static IWDG_HandleTypeDef   IwdgHandle;


static u32 sBootDogFeedTime = 0;
static u32 FeedCnt1=0;
static u64 FeedCnt2=0;
static bool_t bSoftFeedDog=false;


static bool_t Sh_IwdgCounter(char *param);
ADD_TO_ROUTINE_SHELL(wdgc,Sh_IwdgCounter,"���Ź�ι������");
static bool_t Sh_IwdgCounter(char *param)
{
    uint32_t data[2];//used to print the s64 type
    u64 Cnt;
    Cnt=FeedCnt2-FeedCnt1;
    memcpy(data,&Cnt,sizeof(data));
    printf("BootLoader Period Counter:%d.\r\n",FeedCnt1);
    printf("Normal Period Counter:0x%08x 0x%08x \r\n",data[1],data[0]);
    return true;
}

// =============================================================================
// ���ܣ����Ͽ��Ź�ι������,max706��ι��ʱ����1.6s,�����ֲᣬι���Ǳ��ش���
// ��������
// ���أ���
// =============================================================================
bool_t BrdWdt_FeedDog(void)
{
    /* Refresh IWDG: reload counter */
    if(HAL_IWDG_Refresh(&IwdgHandle) == HAL_OK)
    {
//      return false;
        if(bSoftFeedDog)
        {
//          printk("Feed Dog.\r\n");   //todo for test by zhb20171025
        }
        FeedCnt2++;
        return true;
    }
    else
    {
//      printk("Feed Dog failed.\r\n");    //todo for test by zhb20171025
        return false;
    }
}


u32 __FeedDog_Isr(ptu32_t intline)
{
    CFG_WDT_TIM->CNT = 0;
    CFG_WDT_TIM->SR = 0;//���жϱ�־
    BrdWdt_FeedDog();
    Int_ClearLine(intline);
    FeedCnt1++;
    return 1;
}

//----����boot�ڼ�ι��----------------------------------------------------------
//���ܣ�����boot�ڼ�ι�����ܣ�һ����˵��������һ����ʱ�����ж�������CFG_WDT_FEED_CYCLE
//      ��80%���ж���Ϊʵʱ�жϣ�����ISR�У�ִ��ι��������CFG_BOOT_TIME_LIMIT ʱ��֮��
//      ֹͣι����
//��������
//���أ�true
//-----------------------------------------------------------------------------
bool_t __BrdBoot_FeedStart( void)
{
    u8 irqline = CN_INT_LINE_TIM8_BRK_TIM12;
    IWDG_Init();
    sBootDogFeedTime = CFG_BOOT_TIME_LIMIT;
    RCC->APB1ENR |=RCC_APB1ENR_TIM12EN;
    CFG_WDT_TIM->CR1 &= ~(TIM_CR1_CEN); //��ֹTIMER
    CFG_WDT_TIM->CR1 |= TIM_CR1_ARPE | TIM_CR1_DIR;//�Զ���װ
    CFG_WDT_TIM->DIER |= TIM_DIER_UIE;//ʹ�ܸ����ж�
    CFG_WDT_TIM->PSC = 4000-1;//��Ƶϵ�� Ϊ�� ����Ƶ(1/108)1uS
    CFG_WDT_TIM->ARR = 27000;//��ʱ��Ԥװ��ֵ
    Int_Register(irqline);
    Int_IsrConnect(irqline,__FeedDog_Isr);
    Int_SettoReal(irqline);
    Int_ClearLine(irqline);
    Int_RestoreRealLine(irqline);
    CFG_WDT_TIM->CR1 |= (TIM_CR1_CEN); //ʹ��TIMER
    return true;
}

//----ֹͣ����boot�ڼ�ι��-------------------------------------------------------
//���ܣ���Ӳ�����Ź�ģ�鰲װ��ϵͳ��������ι������������ʼ���ɶ�ʱ��ISRι����������
//��������
//���أ�true
//-----------------------------------------------------------------------------
bool_t __BrdBoot_FeedEnd(void)
{
    u8 irqline = CN_INT_LINE_TIM8_BRK_TIM12;
    BrdWdt_FeedDog();
    CFG_WDT_TIM->CR1 &=~(TIM_CR1_CEN); //��ֹTIMER
    Int_SaveRealLine(irqline);
    Int_IsrDisConnect(irqline);
    Int_UnRegister(irqline);
    RCC->APB1ENR &=(~RCC_APB1ENR_TIM12EN);
    bSoftFeedDog=true;
    return true;
}


void IWDG_Init(void)
{
    /*##-1- Check if the system has resumed from IWDG reset ####################*/
      if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
      {
          /* Clear reset flags */
             __HAL_RCC_CLEAR_RESET_FLAGS();
      }

   /*##-2- Configure the IWDG peripheral ######################################*/
      /*##-3- Configure the IWDG peripheral ######################################*/
      switch(CFG_IWDG_PRESCALER)
      {
          case 4:
              IwdgHandle.Init.Prescaler = IWDG_PRESCALER_4;break;
          case 16:
              IwdgHandle.Init.Prescaler = IWDG_PRESCALER_16;break;
          case 32:
              IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;break;
          case 64:
              IwdgHandle.Init.Prescaler = IWDG_PRESCALER_64;break;
          case 128:
              IwdgHandle.Init.Prescaler = IWDG_PRESCALER_128;break;
          case 256:
              IwdgHandle.Init.Prescaler = IWDG_PRESCALER_256;break;

          default:
              return ;
      }
      if(CFG_RELOAD_CYCLE >= 4096)
      {
          return ;
      }

      IwdgHandle.Instance = IWDG;
      IwdgHandle.Init.Reload    = CFG_RELOAD_CYCLE;
      IwdgHandle.Init.Window    = IWDG_WINDOW_DISABLE;

    if (HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
    {
//        printf("IWDG Init failed.\r\n");
    }
//    printf("IWDG Init success.\r\n");
}

// =============================================================================
// ���ܣ����Ͽ��Ź�оƬ��ʼ�����˺��������Ź���������ʼ������������ˡ������������ع��̡�
//      �Ĺ��ܣ����������ú󣬽�ֹͣ�Զ�ι����
// ��������
// ���أ���
// =============================================================================
bool_t ModuleInstall_BrdWdt(void)
{
    bool_t result;
    u32 feed;
    IWDG_Init();

    feed = (pow(2,IwdgHandle.Init.Prescaler) * IwdgHandle.Init.Reload) / 8; //���Ź����ʱ�䵥λms
    result = WdtHal_RegisterWdtChip("Wdt_IWDG", feed * 1000 /4, BrdWdt_FeedDog);
#if(CFG_DEFEND_ON_BOOT == true)
    __BrdBoot_FeedEnd();
#endif
    return result;
}



