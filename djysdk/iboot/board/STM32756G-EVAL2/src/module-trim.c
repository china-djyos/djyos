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

#include <board-config.h>
#include <charset.h>
#include <core_config.h>
#include <cpu_peri_iic.h>
#include <cpu_peri_lcd.h>
#include <cpu_peri_uart.h>
#include <djyfs/vfile.h>
#include <djyos.h>
#include <driver.h>
#include <exp.h>
#include <font.h>
#include <gdd.h>
#include <gkernel.h>
#include <iicbus.h>
#include <cfg/IO_config.h>
#include <msgqueue.h>
#include <multiplex.h>
#include <shell.h>
#include <spibus.h>
#include <stdint.h>
#include <stdio.h>
#include <timer.h>
#include <uartctrl.h>

#include "../../../../../djysrc/bsp/arch/arm/arm32_stdint.h"

extern ptu32_t ModuleInstall_DebugInfo(ptu32_t para);

#define TOUCH_DEV_NAME  "stmpe811"
#define KBD_DEV_NAME    "keyboard_driver"
#define DISPLAY_NAME   "AM-640480"


static  const char *gdd_input_dev[]={

    TOUCH_DEV_NAME,
    KBD_DEV_NAME,

    NULL, //����Ҫ��NULL��Ϊ�������

};
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

    extern bool_t Board_GpioInit(void);
    Board_GpioInit();
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
    ModuleInstall_DebugInfo(0);

    //�쳣����ģ��,����:shellģ��
    ModuleInstall_Exp(0);

    // SD�豸 -- �����豸����ģ��
//    ModuleInstall_SD("sd", 0);
    // NAND�豸 -- �����豸����ģ��
//    ModuleInstall_NAND("nand", 0, 0);
    // �ļ�ϵͳģ��
    ModuleInstall_FileSystem();
    // ��װyaffs2�ļ�ϵͳ,����:�ļ�ϵͳģ���NAND�豸
//    ModuleInstall_YAFFS2("/dev/nand");
    // ��װfat�ļ�ϵͳ,����:�ļ�ϵͳģ���SD�豸
//  ModuleInstall_FAT("dev/sd");
    //���ù���·��,����:�ļ�ϵͳ,�����·������.
//      Djyfs_SetWorkPath(gc_pCfgWorkPath);
		ModuleInstall_UART(CN_UART1);
//		ModuleInstall_UART(CN_UART2);
//		ModuleInstall_UART(CN_UART3);   //232TXD/232RXD
//		ModuleInstall_UART(CN_UART4);
//		ModuleInstall_UART(CN_UART5);
//		ModuleInstall_UART(CN_UART6);
//		ModuleInstall_UART(CN_UART7);
//		ModuleInstall_UART(CN_UART8);   //DEBUG����
    //��IO�豸,��ͬ�İ��,�ⲿ�ֿ�������Ƚϴ�,��Ӱ��printk������
    //�˺�,printf��scanf��ʹ��stdin/out��������롣
    //����: ��stdin/out/err���ļ�,�������ļ�ϵͳ
    //      �����豸,�������豸����
    //      ͬʱ,���������������Ϣ����ʩ,���紮��,LCD��
		OpenStdin(gc_pCfgStdinName);
		OpenStdout(gc_pCfgStdoutName);
		OpenStderr(gc_pCfgStderrName);
//    //һ���������������stdin��stdout��stderrʹ�ò�ͬ�Ĵ��ڻ�����IOͨ������Ҫ
//    //�ֱ��趨������CN_UART_DMA_UNUSED
//     Driver_CtrlDevice(ToDev(stdin),CN_UART_START,0,0);
     Driver_CtrlDevice(ToDev(stdin),CN_UART_DMA_USED,0,0);
//     Driver_CtrlDevice(ToDev(stdin),CN_UART_DMA_UNUSED,0,0);

//    //���ô��ڲ�����Ϊ115200��
//    Driver_CtrlDevice(ToDev(stdin),CN_UART_SET_BAUD,115200,0);
	#include "ymodem.h"
	//  Dev = Driver_OpenDevice("UART3",D_RDONLY,CN_TIMEOUT_FOREVER);
	ModuleInstall_Ymodem(0);
	Ymodem_PathSet("/iboot");
	ModuleInstall_IAP_FS(NULL);
	ModuleInstall_IAP();
//
//    //��װ�˻���������ģ�飬������̡�����
    ModuleInstall_HmiIn( 0 );

    //djybusģ��
    ModuleInstall_DjyBus(0);
    //IIC����ģ��,����:djybus
    ModuleInstall_IICBus(0);
//    IIC_Init(CN_IIC1);
//    IIC_Init(CN_IIC2);
//    IIC_Init(CN_IIC3);
    IIC_Init(CN_IIC4);
    //SPI����ģ��,����:djybus
//    ModuleInstall_SPIBus(0);
//    ModuleInstall_SPIPort(CN_SPI1);
//    ModuleInstall_SPIPort(CN_SPI2);
//    ModuleInstall_SPIPort(CN_SPI3);
//    ModuleInstall_SPIPort(CN_SPI4);
//    ModuleInstall_SPIPort(CN_SPI5);
//    ModuleInstall_SPIPort(CN_SPI6);
//    ModuleInstall_SPIPort(CN_TWI0);

    //����ʱ��ģ��
      ModuleInstall_TM(0);
//    //ʹ��Ӳ��RTC,ע�͵���ʹ��tick��RTC��ʱ,����:����ʱ��ģ��
//    ModuleInstall_RTC(0);
//    //��ʱ�����
    extern ptu32_t ModuleInstall_Timer(ptu32_t para);
    ModuleInstall_Timer(CN_TIMER_SOURCE_HARD);
//
//   //����Э��ջ�������ο�tcpipĿ¼�µ�boardnetcfg.cfg�ļ���ʵ����������
//   //�����øð�������磬�������ʹ������Ļ�
//    ModuleInstall_NetStaticIP(0);//ʹ�þ�̬IP
//    Module_BoardNetInstall_Dynamic(0); //ʹ�ö�̬����IP
//
//    //��������ģ��
    ModuleInstall_KeyBoard(0);
//    //������������,����:��������ģ��
//    ModuleInstall_KeyBoardHard(0);
//
//    //�ַ���ģ��
//    ModuleInstall_Charset(0);
//    //gb2312�ַ�����,����:�ַ���ģ��
//    ModuleInstall_CharsetGb2312(0);
//    //ascii�ַ���,ע��,gb2312������ascii,��ʼ����gb2312��,���뱾ģ��
//    //����:�ַ���ģ��
//    ModuleInstall_CharsetAscii(0);
//    ��ʼ��utf8�ַ���
//    ModuleInstall_CharsetUtf8(0);
//    //���ʻ��ַ���֧��,���������ַ���ģ���Լ������ַ�����ʼ��
//    ModuleInstall_CharsetNls("C");
//
//
//     ModuleInstall_Font(0);                 //����ģ��
//
//    //8*8�����ascii��������:����ģ��
//    ModuleInstall_FontAscii8x8Font(0);
//    //6*12�����ascii��������:����ģ��
//    ModuleInstall_FontAscii6x12Font(0);
//    //�����鰲װGB2312��������,������8*16��ascii����.����:����ģ��
//    ModuleInstall_FontGb2312_816_1616_Array(0);
//    //���ļ���װGB2312��������,������8*16��ascii����.����:����ģ��,�ļ�ϵͳ
////    ModuleInstall_FontGb2312_816_1616_File("sys:\\gb2312_1616");
//    //8*16 ascii�����ʼ��,������128�ֽ�,����:����ģ��
//    //ע:�����װ��GB2312,�����ٰ�װ
////    ModuleInstall_FonAscii8x16Font(0);
//
//
    //��ʼ��gui kernelģ��
    static struct GkWinRsc desktop;
    struct DisplayRsc *lcd;
//    ModuleInstall_GK(0);           //gkernelģ��
    //lcd������ʼ��,�����ϵͳ�ѵĻ�,�ڶ���������NULL
    //�ѵ�����,����lds�ļ���������,ע�ⲻҪ���.
    //����: gkernelģ��

//    lcd = (struct DisplayRsc*)ModuleInstall_LCD(DISPLAY_NAME,"extram");

    //��������,����:��ʾ������
//    GK_ApiCreateDesktop(lcd,&desktop,0,0,
//                        CN_COLOR_BLUE,CN_WINBUF_PARENT,CN_SYS_PF_DISPLAY,0);


//    //������ģ��,����:gkernelģ�����ʾ������
//    ModuleInstall_Touch(0);
//    //����������,
//    //����:������ģ��,������ʾ������,�Լ���������Ӳ��,����qh_1��IIC����.
//    //     ����������ݴ����ļ���,�������ļ�ϵͳ.
//    ModuleInstall_Touch(DISPLAY_NAME,"IIC1");
//
//    //���Ź�ģ��,��������˼���ʱι��,���Ź����ģ��Ӵ˿�ʼ�ӹ�Ӳ����.
//    extern ptu32_t ModuleInstall_Wdt(ptu32_t para);
//    ModuleInstall_Wdt(0);
//
//    //GDD�����ʼ��
//    ModuleInstall_GDD(&desktop,gdd_input_dev);

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


