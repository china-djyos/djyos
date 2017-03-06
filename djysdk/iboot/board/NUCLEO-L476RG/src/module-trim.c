
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
#include "board-config.h"
#include "driver.h"
#include "cpu_peri.h"
#include "cpu_peri_uart.h"
#include "uartctrl.h"
#include "gkernel.h"
#include "djyos.h"
#include "core_config.h"
#include "io_config.h"
#include "timer.h"
#include "lowpower.h"
#include "list.h"
#include "..\heap\heap-in.h"
#include "ymodem.h"

extern ptu32_t ModuleInstall_DebugInfo(ptu32_t para);


#define TOUCH_DEV_NAME  "stmpe811"
#define KBD_DEV_NAME    "keyboard driver"


static  const char *gdd_input_dev[]={

    TOUCH_DEV_NAME,
    KBD_DEV_NAME,

    NULL, //����Ҫ��NULL��Ϊ�������

};

u32 NU_EntrySleepReCall(u32 SleepLevel);
u32 NU_ExitSleepReCall(u32 SleepLevel);
ptu32_t djy_main(void);

u32 NU_EntrySleepReCall(u32 SleepLevel)
{
	return 0;
}
u32 NU_ExitSleepReCall(u32 SleepLevel)
{
	return 0;
}
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
//---------------------------------------------------------------------------
void Sys_ModuleInit(void)
{
    uint16_t evtt_main;

    //��ʼ��ֱ������������Ӳ����Ϊstdio.c�ж���� PutStrDirect��GetCharDirect
    //����ָ�븳ֵ��Ҳ����ֻΪPutStrDirect��ֵ����֧��printk��
    //��������bsp�ĺ�����һ���Ǵ�������,BSPû�ṩ�Ļ����Ͳ�Ҫ���ã��ᵼ��Ӧ�ó�����벻ͨ����
    Stdio_KnlInOutInit( 0 );
   //shellģ��,����:��
    ModuleInstall_Sh(0);

    //�豸����ģ��
    ModuleInstall_Driver(0);
    //��·����ģ��,�ṩ����Linux��epoll��select�Ĺ���
    ModuleInstall_Multiplex(0);
    //��Ϣ����ģ��
    ModuleInstall_MsgQ(0);

    //�ṩ��shell������ں���Ϣ�Ĺ���,����:shellģ��
//    ModuleInstall_DebugInfo(0);

    //�쳣����ģ��,����:shellģ��
    ModuleInstall_Exp(0);
//    ModuleInstall_SD("sd", 0);
//   ModuleInstall_NAND("nand", 1, 0);
//   	ModuleInstall_FileSystem();
//   	ModuleInstall_YAFFS2();
//   	ModuleInstall_FAT();


//    ModuleInstall_UART(CN_UART1);
//    ModuleInstall_UART(CN_UART2);
//    ModuleInstall_UART(CN_UART3);
//    ModuleInstall_UART(CN_UART4);
//    ModuleInstall_UART(CN_UART5);

    //��IO�豸,��ͬ�İ��,�ⲿ�ֿ�������Ƚϴ�,��Ӱ��printk������
    //�˺�,printf��scanf��ʹ��stdin/out��������롣
    //����: ��stdin/out/err���ļ�,�������ļ�ϵͳ
    //      �����豸,�������豸����
    //      ͬʱ,���������������Ϣ����ʩ,���紮��,LCD��
//    OpenStdin(gc_pCfgStdinName);
//    OpenStdout(gc_pCfgStdoutName);
//    OpenStderr(gc_pCfgStderrName);
    //һ���������������stdin��stdout��stderrʹ�ò�ͬ�Ĵ��ڻ�����IOͨ������Ҫ
    //�ֱ��趨������
//    Driver_CtrlDevice(ToDev(stdin),CN_UART_START,0,0);
//    Driver_CtrlDevice(ToDev(stdin),CN_UART_DMA_USED,0,0);
    //���ô��ڲ�����Ϊ115200��
//    Driver_CtrlDevice(ToDev(stdin),CN_UART_SET_BAUD,115200,0);

    //��װ�˻���������ģ�飬������̡�����
//    ModuleInstall_HmiIn( 0 );

//    ModuleInstall_Ymodem(0);
//	Ymodem_PathSet("/iboot");
//	ModuleInstall_IAP_FS(NULL);
//    ModuleInstall_IAP();

    //djybusģ��
//    ModuleInstall_DjyBus(0);
    //IIC����ģ��,����:djybus
//    ModuleInstall_IICBus(0);
    //SPI����ģ��,����:djybus
//    ModuleInstall_SPIBus(0);

    //����ʱ��ģ��
//    ModuleInstall_TM(0);
    //ʹ��Ӳ��RTC,ע�͵���ʹ��tick��RTC��ʱ,����:����ʱ��ģ��
//    ModuleInstall_RTC(0);

    //��ʱ�����
//    ModuleInstall_Timer(CN_TIMER_SOURCE_TICK);
//
   //����Э��ջ���
//    extern ptu32_t ModuleInstall_DjyIp(ptu32_t para);
    //Э��ջ�����ʼ��
//    ModuleInstall_DjyIp(0);

    //��������ģ��
//    ModuleInstall_KeyBoard(0);
    //������������,����:��������ģ��
//    ModuleInstall_KeyBoardHard(0);

    //�ַ���ģ��
//    ModuleInstall_Charset(0);
    //gb2312�ַ�����,����:�ַ���ģ��
//    ModuleInstall_CharsetGb2312(0);
    //ascii�ַ���,ע��,gb2312������ascii,��ʼ����gb2312��,���뱾ģ��
    //����:�ַ���ģ��
//    ModuleInstall_CharsetAscii(0);
    //��ʼ��utf8�ַ���
//    ModuleInstall_CharsetUtf8(0);
    //���ʻ��ַ���֧��,���������ַ���ģ���Լ������ַ�����ʼ��
//    ModuleInstall_CharsetNls("C");


//    ModuleInstall_Font(0);                 //����ģ��

    //8*8�����ascii��������:����ģ��
//    ModuleInstall_FontAscii8x8Font(0);
    //6*12�����ascii��������:����ģ��
//    ModuleInstall_FontAscii6x12Font(0);
    //�����鰲װGB2312��������,������8*16��ascii����.����:����ģ��
//    ModuleInstall_FontGb2312_816_1616_Array(0);
    //���ļ���װGB2312��������,������8*16��ascii����.����:����ģ��,�ļ�ϵͳ
//    ModuleInstall_FontGb2312_816_1616_File("sys:\\gb2312_1616");
    //8*16 ascii�����ʼ��,������128�ֽ�,����:����ģ��
    //ע:�����װ��GB2312,�����ٰ�װ
//    ModuleInstall_FonAscii8x16Font(0);


    //��ʼ��gui kernelģ��
//    static struct tagGkWinRsc desktop;
//    struct tagDisplayRsc *ili9325;

//    ModuleInstall_GK(0);           //gkernelģ��
    //lcd������ʼ��,�����ϵͳ�ѵĻ�,�ڶ���������NULL
    //�ѵ�����,����lds�ļ���������,ע�ⲻҪ���.
    //����: gkernelģ��
//    ili9325 = (struct tagDisplayRsc*)ModuleInstall_ili9325("ili9325","extram");

    //��������,����:��ʾ������
//    GK_ApiCreateDesktop(ili9325,&desktop,0,0,
//                        CN_COLOR_RED+CN_COLOR_GREEN,CN_WINBUF_BUF,0,0);

    //������ģ��,����:gkernelģ�����ʾ������
//    ModuleInstall_Touch(0);
    //����������,
    //����:������ģ��,������ʾ������,�Լ���������Ӳ��,����qh_1��IIC����.
    //     ����������ݴ����ļ���,�������ļ�ϵͳ.
//    ModuleInstall_Touch_Stmpe811((ptu32_t)"ili9325");

    //���Ź�ģ��,��������˼���ʱι��,���Ź����ģ��Ӵ˿�ʼ�ӹ�Ӳ����.
    extern ptu32_t ModuleInstall_Wdt(ptu32_t para);
//    ModuleInstall_Wdt(0);

    //GDD�����ʼ��
//    ModuleInstall_GDD(&desktop,pGddInputDev,sizeof(pGddInputDev)/sizeof(void*));

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

    return ;
}
