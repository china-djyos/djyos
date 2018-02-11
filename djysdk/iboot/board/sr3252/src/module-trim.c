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
#include "timer.h"
#include "lowpower.h"
#include "core_config.h"
#include <cfg/io_config.h>

extern ptu32_t ModuleInstall_DebugInfo(ptu32_t para);
extern ptu32_t ModuleInstall_IAP(void);

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
    struct DjyDevice *Dev;

    Board_GpioInit();

    MoudleInit_Systime(0);
    //��ʼ��ֱ������������Ӳ����Ϊstdio.c�ж���� PutStrDirect��GetCharDirect
    //����ָ�븳ֵ��Ҳ����ֻΪPutStrDirect��ֵ����֧��printk��
    //��������bsp�ĺ�����һ���Ǵ�������,BSPû�ṩ�Ļ����Ͳ�Ҫ���ã��ᵼ��Ӧ�ó�����벻ͨ����
    Stdio_KnlInOutInit( 0 );
   //shellģ��,����:��
    ModuleInstall_Sh(0);
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
    ModuleInstall_Exp(0);

    //flash�ļ�ϵͳ��ʼ��,����:�ļ�ϵͳģ��,shellģ��
    ModuleInstall_FileSystem(0);

    ModuleInstall_UART(CN_USART0);
    ModuleInstall_UART(CN_USART2);

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
    Driver_CtrlDevice(ToDev(stdin),CN_UART_START,0,0);
    //���ô��ڲ�����Ϊ115200��
    Driver_CtrlDevice(ToDev(stdin),CN_UART_SET_BAUD,115200,0);

	#include "ymodem.h"

	ModuleInstall_Ymodem(0);


	ModuleInstall_IAP();

    //����ʱ��ģ��
    ModuleInstall_TM(0);

    //���Ź���ʼ��---
	extern void ModuleInstall_BrdWdt(void);
	ModuleInstall_BrdWdt();//ʹ��Ӳ�����Ź�
	extern ptu32_t ModuleInstall_Wdt(ptu32_t para);
	ModuleInstall_Wdt(0);

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


