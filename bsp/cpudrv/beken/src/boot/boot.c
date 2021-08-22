#include <Iboot_Info.h>
#include <sys_config.h>
#include <wdt_pub.h>
#include <power_save_pub.h>
//#include <arch.h>
#include <drv_model_pub.h>
#include <co_math.h>

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip boot"//CPU boot����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:required              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BOOT == false )
//#warning  " boot  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BOOT    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,false,true,
#define CFG_POWER_ON_RESET_TO_BOOT    false //�����ϵ縴λ���Ƿ�ǿ������iboot
//%$#@end configue  ****�������ý���
#endif
//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure

extern void Iboot_LoadPreload(void);
//void WrResetToReg(void);

static void bk_reboot(void)
{
    u32 wdt_val = 1;

#if CFG_USE_STA_PS
    GLOBAL_INT_DECLARATION();

    GLOBAL_INT_DISABLE();
    if(power_save_if_ps_rf_dtim_enabled()
        && power_save_if_rf_sleep())
    {
        power_save_wkup_event_set(NEED_DISABLE_BIT | NEED_REBOOT_BIT);
    }
    else
    {
#endif

//    printf("wdt reboot\r\n");
    sddev_control(WDT_DEV_NAME, WCMD_SET_PERIOD, &wdt_val);
//    WrResetToReg();
    sddev_control(WDT_DEV_NAME, WCMD_POWER_UP, NULL);
//    DJY_DelayUs(100000);
#if CFG_USE_STA_PS
    }
    GLOBAL_INT_RESTORE();
#endif
}

void CPU_Reboot(void)
{
    Iboot_SetRebootFlag();
#if (CFG_RUNMODE_BAREAPP == 0)
    Iboot_SetPreviouResetFlag();
#endif
//  bk_reboot();
    void (*fn_start)();
    fn_start = 0x0;
    fn_start();
}


void CPU_Reset(void)
{
    Iboot_SetSoftResetFlag();
#if (CFG_RUNMODE_BAREAPP == 0)
    Iboot_SetPreviouResetFlag();
#endif
//    void (*fn_start)();
//    fn_start = 0x0;
//    fn_start();
//  _start();
    bk_reboot();
}

void CPU_RestartSystem(void)
{
    Iboot_SetRestartAppFlag();
#if (CFG_RUNMODE_BAREAPP == 0)
    Iboot_SetPreviouResetFlag();
#endif
    AppStart();
//  void (*fn_start)();
//  fn_start = 0x0;
//  fn_start();
}
