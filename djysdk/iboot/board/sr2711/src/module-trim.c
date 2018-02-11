//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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

#include "stdint.h"
#include "stdio.h"
#include "driver.h"
#include "cpu_peri.h"
#include "cpu_peri_uart.h"
#include "uartctrl.h"
#include "gkernel.h"
#include "djyos.h"
#include "core_config.h"
#include <cfg/io_config.h>
#include "timer.h"
#include "lowpower.h"
#include "list.h"
#include "..\heap\heap-in.h"
#include <driver/flash/flash.h>

#include "../../../../../djysrc/bsp/arch/arm/arm32_stdint.h"
#include "Board-config.h"

extern ptu32_t ModuleInstall_DebugInfo(ptu32_t para);
extern bool_t BrdWdt_FeedDog(void);
u32 QH_EntrySleepReCall(u32 SleepLevel);
u32 QH_ExitSleepReCall(u32 SleepLevel);


//#define TOUCH_DEV_NAME  "LCM240128Touch"
//#define KBD_DEV_NAME    "KBD_SOFT"

//static  const char *gdd_input_dev[]={
//
//    TOUCH_DEV_NAME,
//    KBD_DEV_NAME,
//
//    NULL, //����Ҫ��NULL��Ϊ�������
//
//};
u32 QH_EntrySleepReCall(u32 SleepLevel);
u32 QH_ExitSleepReCall(u32 SleepLevel);

ptu32_t djy_main(void);

ptu32_t __djy_main(void)
{
    djy_main();
    return 0;
}


//----����ϵͳ���в�������-----------------------------------------------------
//����: ���ò���ϵͳ���в���,������������/�¼�������.ʹ���ı��ļ�����ʱʹ�á�
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Sys_ConfigRunParam(char *CfgfileName)
{
}

//----����ϵͳ�ں��������-----------------------------------------------------
//���ܣ����úͳ�ʼ����ѡ������������û�����Ŀ¼�б���ʵ�ֱ����������ں˳�ʼ��
//      �׶ε��á�
//      ������ʵ���ں˲ü����ܣ�����ֻҪע�͵�
//          ModuleInstall_Multiplex(0);
//      ��һ�У���·��������ͱ��ü����ˡ�
//      �û��ɴ�example��copy���ļ����Ѳ�Ҫ�����ע�͵���ǿ�ҽ��飬��Ҫɾ��,Ҳ
//      ��Ҫ�޸ĵ���˳�򡣿��԰��û�ģ��ĳ�ʼ������Ҳ���뵽��������,�������
//      ϵͳģ���ʼ������.
//      ��Щ�����������ϵ,�ü�ʱ,ע���Ķ�ע��.
//��������
//���أ���
//-----------------------------------------------------------------------------

void Sys_ModuleInit(void)
{
    uint16_t evtt_main;
    extern void Board_GpioInit(void);
    Board_GpioInit();

//    extern bool_t MoudleInit_Systime(ptu32_t para);
//    MoudleInit_Systime(0);
    //��ʼ��ֱ������������Ӳ����Ϊstdio.c�ж���� PutStrDirect��GetCharDirect
    //����ָ�븳ֵ��Ҳ����ֻΪPutStrDirect��ֵ����֧��printk��
    //��������bsp�ĺ�����һ���Ǵ�������,BSPû�ṩ�Ļ����Ͳ�Ҫ���ã��ᵼ��Ӧ�ó�����벻ͨ����
    Stdio_KnlInOutInit( 0 );
   //shellģ��,����:��
    ModuleInstall_Sh(0);
    extern ptu32_t ModuleInstall_Wdt(ptu32_t para);
//	ModuleInstall_BrdWdt();
//	WdtHal_BootStart(60*1000*mS);
    //�ļ�ϵͳģ��,����:shell
//    ModuleInstall_Djyfs(0);
    //�豸����ģ��
    ModuleInstall_Driver(0);
    //��·����ģ��,�ṩ����Linux��epoll��select�Ĺ���
    ModuleInstall_Multiplex(0);
    //��Ϣ����ģ��
    ModuleInstall_MsgQ(0);

    //�ṩ��shell������ں���Ϣ�Ĺ���,����:shellģ��
    ModuleInstall_DebugInfo(0);

    //�쳣����ģ��,����:shellģ��
    extern ptu32_t ModuleInstall_Exp(ptu32_t para);
    ModuleInstall_Exp(0);

    //flash�ļ�ϵͳ��ʼ��,����:�ļ�ϵͳģ��,shellģ��
    extern s32  ModuleInstall_FileSystem(void);
    ModuleInstall_FileSystem();
//    ModuleInstall_FAT();
    //flash�ļ�ϵͳ��ʼ��,����:�ļ�ϵͳģ��,shellģ��
    ModuleInstall_UART(CN_UART1);    //debug
//    ModuleInstall_UART(CN_UART3);   //RF
//    ModuleInstall_UART(CN_UART4);  //��Զ ���� ���� ����ģ��modem
//    ModuleInstall_UART(CN_UART5);  //����ģ��

//    ModuleInstall_UART(CN_UART6);   //
//    ModuleInstall_UART(CN_UART7);  //debug

    //��IO�豸,��ͬ�İ��,�ⲿ�ֿ�������Ƚϴ�,��Ӱ��printk������
    //�˺�,printf��scanf��ʹ��stdin/out��������롣
    //����: ��stdin/out/err���ļ�,�������ļ�ϵͳ
    //      �����豸,�������豸����
    //      ͬʱ,���������������Ϣ����ʩ,���紮��,LCD��
    OpenStdin(gc_pCfgStdinName);
    OpenStdout(gc_pCfgStdoutName);
    OpenStderr(gc_pCfgStderrName);
    //һ���������������stdin��stdout��stderrʹ�ò�ͬ�Ĵ��ڻ�����IOͨ������Ҫ
    //�ֱ��趨������
    Driver_CtrlDevice(ToDev(stdin),CN_UART_SET_BAUD,115200,0);
//    Driver_CtrlDevice(ToDev(stdin),CN_UART_DMA_USED,0,0);
    Driver_CtrlDevice(ToDev(stdin),CN_UART_DMA_UNUSED,0,0);
    Driver_CtrlDevice(ToDev(stdin),CN_UART_START,0,0);

	ModuleInstall_EmbededFlash("embedded flash", FLASH_BUFFERED, 0);
	ModuleInstall_IAP_FS("/iboot", "/dev/embedded flash");
	ModuleInstall_IAP();


	ModuleInstall_MMC(3,"emmc",0,0);
    ModuleInstall_FAT("/dev/emmc",1);

//
//    //��װ�˻���������ģ�飬������̡�����
//    ModuleInstall_HmiIn( 0 );

    ModuleInstall_USB(1);
//
//    ModuleInstall_FAT("/dev/usb msc", 2);
//    ModuleInstall_USB_IAP(1, 1);

    //install the usb device for the wireless module
//    ModuleInstall_USB(2);

    //djybusģ��
   // ModuleInstall_DjyBus(0);
    //IIC����ģ��,����:djybus
    ModuleInstall_IICBus(0);
//    IIC_Init(CN_IIC1);
    IIC_Init(CN_IIC2);
    ModuleInstall_RTCDS3232M(0);
//    IIC_Init(CN_IIC3);
//    IIC_Init(CN_IIC4);
    //SPI����ģ��,����:djybus
   // ModuleInstall_SPIBus(0);
//    ModuleInstall_SPIPort(CN_SPI1);
//    ModuleInstall_SPIPort(CN_SPI2);
//    ModuleInstall_SPIPort(CN_SPI3);
   // extern bool_t SPI_Initialize(u8 port);
   // SPI_Initialize(3);
   // SPI_Initialize(0);
//    ModuleInstall_SPIPort(CN_SPI5);
//    ModuleInstall_SPIPort(CN_SPI6);
//    ModuleInstall_SPIPort(CN_TWI0);
//    Eth_GpioInit();
    //����ʱ��ģ��
      ModuleInstall_TM(0);
//    //ʹ��Ӳ��RTC,ע�͵���ʹ��tick��RTC��ʱ,����:����ʱ��ģ��

//      ModuleInstall_CpuRtc(0);

//    //��ʱ�����
      extern bool_t ModuleInstall_HardTimer(void);
      ModuleInstall_HardTimer();
      //��ʱ�����
      extern ptu32_t ModuleInstall_Timer(ptu32_t para);
      ModuleInstall_Timer(CN_TIMER_SOURCE_HARD);
      extern ptu32_t ModuleInstall_Wdt(ptu32_t para);
//      ModuleInstall_Wdt(0);


      extern void Eth_GpioInit(void);
      extern ptu32_t ModuleInstall_NetStaticIP(ptu32_t para);
      extern ptu32_t ModuleInstall_LowPower (u32 (*EntrySleepReCall)(u32 SleepLevel),
                                      u32 (*ExitSleepReCall)(u32 SleepLevel));


      extern void SR2711_EntrySleepReCall(u32 SleepLevel);
      extern void SR2711_ExitSleepReCall(u32 SleepLevel);
//      ModuleInstall_LowPower(SR2711_EntrySleepReCall,SR2711_ExitSleepReCall);


      ModuleInstall_NetStaticIP(0);

      evtt_main = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
                                __djy_main,NULL,gc_u32CfgMainStackLen,
                                "main function");
      //�¼���������������Ϊ0,�����shell����,�������ɼ�shell�����в���
      Djy_EventPop(evtt_main,NULL,0,NULL,0,0);

  //    ModuleInstall_LowPower(QH_EntrySleepReCall,QH_ExitSleepReCall);
    //�û��Լ�����ĳ�ʼ��ģ��,��������￪ʼ����
#if CN_CFG_DYNAMIC_MEM == 1     //CN_CFG_DYNAMIC_MEM�����libOSʱ��������ر���һ��
    Heap_DynamicModuleInit(0);  //�Դ�malloc����ִ�п������㷨
#endif
    //����,��ʼ��ʱʹ�õ�ջ,�Ѿ���ϵͳ����,�������ʹ˽���,��������ú��ѿ�
    printf("Module init end.\r\n");
    return ;
}


