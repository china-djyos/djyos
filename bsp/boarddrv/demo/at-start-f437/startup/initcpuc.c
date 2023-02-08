#include <stdint.h>
#include <stddef.h>

#include "cpu_peri.h"
#include "hard-exp.h"
#include "arch_feature.h"
#include "at32f435_437.h"
#include "arm32_feature.h"

#include "project_config.h"

#include "core_cm4.h"

extern   uint32_t   msp_top[ ];

struct ScbReg volatile * const startup_scb_reg
                        = (struct ScbReg *)0xe000ed00;
void Startup_NMI(void)
{
    while(1);
}
void Startup_Hardfault(void)
{
    while(1);
}

void Init_Cpu(void);

const u32 gc_u32StartupExpTable[128] __attribute__ ((section(".StartupExpTbl")))=
{
    (u32) msp_top,
    (u32) Init_Cpu,
    (u32) Startup_NMI,
    (u32) Startup_Hardfault
};

void Init_Cpu(void)
{
    __set_PSP((uint32_t)msp_top);
    __set_PRIMASK(1);
    __set_FAULTMASK(1);
    __set_CONTROL(0);

    #if (_D_FPU_USED == 1)
    startup_scb_reg->CPACR = (3UL << 20)|(3UL << 22);    //ʹ��FPU
    startup_scb_reg->FPCCR = (1UL << 31);                //�ر�lazy stacking
    #endif
    switch(startup_scb_reg->CPUID)
    {
    }

    extern void SysClockInit(void);
    SysClockInit();

    extern void SRAM_Init(void);
    SRAM_Init();

#if (CFG_RUNMODE_BAREAPP == 1)
    Iboot_LoadPreload();
#else
    Iboot_IAP_SelectLoadProgam();
#endif
}

extern void Iboot_LoadPreload(void);
void AppStart(void)
{
    __set_MSP((uint32_t)msp_top);
    __set_PSP((uint32_t)msp_top);
    Iboot_LoadPreload();
}

//-----------------------------------------------------------------
//���ܣ�IAP�����������ģʽ�����GPIO���ų�ʼ�������ڴ�ʱϵͳ��û�м��أ�ֻ��ʹ
//      ��ֱ�ӵ�ַ���������ܵ���gpio��صĿ⺯����
//      �������ʹ��gpio����־�����������Ǳ��룬��ɾ����
//��������
//���أ��ޡ�
//-----------------------------------------------------------------
void IAP_GpioPinInit(void)
{
    /* adapt to your project. */
}

//-----------------------------------------------------------------
//���ܣ���Ӳ�������Ƿ�ǿ�ƽ���Iboot�����˺�������TRUE����ǿ������Iboot��ͨ����ʹ
//      ��һ��gpio��ͨ�����߾�����
//      ��������£������������APP���ǿ�����runiboot�����л���Iboot״̬�ģ�����
//      ��Ӳ����Ŀ���ж���
//     1���������쳣���󣬲�����shell�л�ʱ���ṩһ�����ȴ�ʩ��
//     2�����ڰ�ȫ���ǣ�APP��û�а����л����룬����������Դ�Ĺ�ϵ���õ���shell��
//��������
//���أ��ޡ�
//˵�������������漰����Ӳ�������ڱ��ļ��г�ʼ�����ر���Ҫע����ǣ����������δ
//      ���صĺ������ر��ǿ⺯����
//      �����������ṩ�����û��������ӦӲ�������Լ򵥷���false��
//-----------------------------------------------------------------
bool_t Iboot_IAP_IsForceIboot(void)
{
    u32 flag;
    IAP_GpioPinInit( );

    /* TODO: detect the real hardware. */
    if (flag == 0)
        return true;
    return false;

}




