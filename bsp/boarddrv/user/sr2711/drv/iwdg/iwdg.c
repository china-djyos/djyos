// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��brdwdt.c
// ģ������: SR2711-V1.1.0�Ŀ��Ź���������ʼ����ι������
// ģ��汾: V2.00
// ������Ա: zhb
// ����ʱ��: 09/30.2017
// ˵��        ��ʹ��MCU��Ƭ�϶������Ź�
// =============================================================================
#if 0
#include "stdint.h"
#include "stddef.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "wdt_hal.h"
#include "shell.h"

static bool_t Sh_IwdgCounter(char *param);
ptu32_t Iwdg_Shell_Module_Install(void);

// BOARD WDT
// =============================================================================
static IWDG_HandleTypeDef   IwdgHandle;
#define WDT_FEED_CYCLE  (2000*1000) //2s

#define WDT_TIM    TIM12    //���������õ��Ķ�ʱ��TIM12

static u32 sBootDogFeedTime = 0;
static u32 FeedCnt1=0;
static u64 FeedCnt2=0;
static bool_t bSoftFeedDog=false;


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

//**************************************************************************
struct shell_debug const shell_cmd_iwdg_table[]=
{
    {"wdgc",(bool_t(*)(char*))Sh_IwdgCounter,    "���Ź�ι������",     NULL},
};
//**************************************************************************
#define CN_IWDG_SHELL_NUM  sizeof(shell_cmd_iwdg_table)/sizeof(struct shell_debug)
static struct ShellCmdRsc tg_iwdg_shell_cmd_rsc[CN_IWDG_SHELL_NUM];

/*******************************************************************************
����:IWDGģ��shell�������
����:��.
����ֵ:1��
*********************************************************************************/
ptu32_t Iwdg_Shell_Module_Install(void)
{
    Sh_InstallCmd(shell_cmd_iwdg_table,tg_iwdg_shell_cmd_rsc,CN_IWDG_SHELL_NUM);
    return 1;
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
    WdtHal_RegisterWdtChip("Wdt_IWDG",WDT_FEED_CYCLE/4,BrdWdt_FeedDog,\
            BrdBoot_FeedStart,BrdBoot_FeedEnd);
    Iwdg_Shell_Module_Install();
}
#endif


