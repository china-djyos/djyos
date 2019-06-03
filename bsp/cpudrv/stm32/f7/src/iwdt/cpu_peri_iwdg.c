// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��brdwdt.c
// ģ������: SR2711-V1.1.0�Ŀ��Ź���������ʼ����ι������
// ģ��汾: V2.00
// ������Ա: zhb
// ����ʱ��: 09/30.2017
// ˵��        ��ʹ��MCU��Ƭ�϶������Ź�
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "wdt_hal.h"
#include "shell.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip iwdt"//�����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"kernel","stm32f7"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100000000,
#define CFG_FEED_CYCLE  (2000*1000)     //"feed dog cycle"
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure


// BOARD WDT
// =============================================================================
static IWDG_HandleTypeDef   IwdgHandle;

#define WDT_TIM    TIM12    //���������õ��Ķ�ʱ��TIM12

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


bool_t BrdBoot_FeedEnd(void);
u32 FeedDog_Isr(ptu32_t intline)
{
    WDT_TIM->CNT = 0;
    WDT_TIM->SR = 0;//���жϱ�־
    BrdWdt_FeedDog();
    Int_ClearLine(intline);
    FeedCnt1++;
    return 1;
}
bool_t BrdBoot_FeedStart(u32 bootfeedtime)
{
    u8 irqline = CN_INT_LINE_TIM8_BRK_TIM12;
    sBootDogFeedTime = bootfeedtime;
    RCC->APB1ENR |=RCC_APB1ENR_TIM12EN;
    WDT_TIM->CR1 &= ~(TIM_CR1_CEN); //��ֹTIMER
    WDT_TIM->CR1 |= TIM_CR1_ARPE | TIM_CR1_DIR;//�Զ���װ
    WDT_TIM->DIER |= TIM_DIER_UIE;//ʹ�ܸ����ж�
    WDT_TIM->PSC = 4000-1;//��Ƶϵ�� Ϊ�� ����Ƶ(1/108)1uS
    WDT_TIM->ARR = 27000;//��ʱ��Ԥװ��ֵ
    Int_Register(irqline);
    Int_IsrConnect(irqline,FeedDog_Isr);
    Int_SettoReal(irqline);
    Int_ClearLine(irqline);
    Int_RestoreRealLine(irqline);
    WDT_TIM->CR1 |= (TIM_CR1_CEN); //ʹ��TIMER
    return true;
}

bool_t BrdBoot_FeedEnd(void)
{
    u8 irqline = CN_INT_LINE_TIM8_BRK_TIM12;
    BrdWdt_FeedDog();
    WDT_TIM->CR1 &=~(TIM_CR1_CEN); //��ֹTIMER
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
      IwdgHandle.Instance = IWDG;
      IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;
      IwdgHandle.Init.Reload    = 32000/16;
      IwdgHandle.Init.Window    = IWDG_WINDOW_DISABLE;

    if (HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
    {
        printf("IWDG Init failed.\r\n");
    }
    printf("IWDG Init success.\r\n");
}

// =============================================================================
// ���ܣ����Ͽ��Ź�оƬ��ʼ��
// ��������
// ���أ���
// =============================================================================
void ModuleInstall_BrdWdt(void)
{
    IWDG_Init();
    WdtHal_RegisterWdtChip("Wdt_IWDG",CFG_FEED_CYCLE/4,BrdWdt_FeedDog,\
            BrdBoot_FeedStart,BrdBoot_FeedEnd);
}



