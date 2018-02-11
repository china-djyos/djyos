#include "os.h"
#include "string.h"
#include "IAP.h"
#include <cfg/iboot_config.h>
#include "stddef.h"
#include "cpu_peri.h"
#include "uartctrl.h"
#include <osboot.h>
#include <IAP_Inner.h>

extern bool_t HAL_ClrUpdateFlag(void);
extern bool_t HAL_CheckUpdateFlag(void);
extern tagIapVar pg_IapVar;
extern void RunApp(void);

// ============================================================================
// ���ܣ�������߳�
// ������
// ���أ�
// ��ע����ʾ����Ƿ��ڹ���
// ============================================================================
static void Led_Run(void)
{
    bool_t ret=true;
    printf("Led Thread Run...\r\n");
    while(1)
    {
        if(ret)
        {
            CPU_Led_Ctrl(1,1);
            ret=false;
        }
        else
        {
            CPU_Led_Ctrl(1,0);
            ret=true;
        }

        Djy_EventDelay(1000*mS);
    }
}

// ============================================================================
// ���ܣ�ͨ��ģʽһ����Ӧ�ó���
// ������pPath -- �ļ�����·��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static void __UpdateApplication(void)
{
    bool_t doUpdate;
    s32 res;
    u8 doPrint = 1;

    while(1)
    {
        doUpdate = HAL_CheckUpdateFlag();
        if(doUpdate)
        {
            if(doPrint)
            {
                printf("\r\nAPP need update(flag).");
                doPrint = 0;
            }

            res = IAP_Update(2, IAP_GetMethod(), IAP_GetPath());
            if(res != -1)
            {
                HAL_ClrUpdateFlag();
                printf("\r\nAPP update succeed. Now going to running APP in 5 seconds.\r\n");
                Djy_EventDelay(5000*mS);
                RunApp();
            }
            else
            {
                printf("\r\nAPP update failed. will retry.");
            }
        }
        else if(pg_IapVar.IbootStatus==EN_CRC_ERR)
        {
            if(doPrint)
            {
                printf("\r\nAPP need update(status).");
                doPrint = 0;
            }
            res = IAP_Update(2, IAP_GetMethod(), IAP_GetPath());
            if(res != -1)
            {
                printf("\r\nAPP update succeed. Now going to running APP in 5 seconds.\r\n");
                Djy_EventDelay(5000*mS);
                RunApp();
            }
            else
            {
                printf("\r\nAPP update failed. will retry.");
            }
        }

        Djy_EventDelay(5000*mS);
    }
}

// ============================================================================
// ���ܣ�Ӧ�ó������
// ������
// ���أ�
// ��ע��
// ============================================================================
void djy_main(void)
{
    u16 LedEvtt;
    extern void Sh_GetStatus(char *param);
    extern void Sh_GetRunMode(char *param);

    Sh_GetRunMode(NULL);
    Sh_GetStatus(NULL);

    LedEvtt = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0, (ptu32_t(*)(void))Led_Run,\
                                NULL, 0x400, "Led Monitor Task");
    if(LedEvtt==CN_EVTT_ID_INVALID)
    {
        printf("Led Monitor Task register failed.\r\n");
    }
    else
    {
        Djy_EventPop(LedEvtt, NULL, 0, 0, 0, 0);
    }

    #if 0
    {
        u16 HardTestEvtt
        HardTestEvtt = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0, (ptu32_t(*)(void))HardDrv_Test,\
                        NULL, 0x4000, "HardDrv Test Task");
        if(HardTestEvtt == CN_EVTT_ID_INVALID)
        {
            printf("HardDrv Test Task register failed.\r\n");
        }
        else
        {
            Djy_EventPop(HardTestEvtt, NULL, 0, 0, 0, 0);
        }
    }
    #endif

    __UpdateApplication();
}

