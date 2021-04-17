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

#include <os.h>
//----����CPU��Ϣע��ģ��-----------------------------------------------------------
//���ܣ�����CPU��Ϣע��ģ��
//������
//���أ�
//------------------------------------------------------------------------------
void testCpuInfo(void)
{
    static u32 cpuinclk = 26*1000*1000;
    static u32 periclk=100*1000*1000;
    static u32 cpuclk=216*1000*1000;
    static u32 tick = 100;
    static u8 cpumagic[8] ={0x1,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
    static u8 ethmac[6]={0x01,0x02,0x03,0x04,0x05,0x1};
    static const char *cpuver="1.2.0";
    static const char *corever="m7-1-2";
    static u32 cachelsize = 128*1024;
    static u32 cacheline  = 32;
    static u32 ramsize  = 367*1024;
    static u32 romsize = 2*1024*1024;


    Core_LogCpuMsg("CLKIN",&cpuinclk,sizeof(cpuinclk),EN_CPU_MSGTYPE_FREQUENCY);
    Core_LogCpuMsg("PERICLK",&periclk,sizeof(periclk),EN_CPU_MSGTYPE_FREQUENCY);
    Core_LogCpuMsg("CPUCLK",&cpuclk,sizeof(cpuclk),EN_CPU_MSGTYPE_FREQUENCY);
    Core_LogCpuMsg("TICK",&tick,sizeof(tick),EN_CPU_MSGTYPE_FREQUENCY);

    Core_LogCpuMsg("CPUSIG",&cpumagic,sizeof(cpumagic),EN_CPU_MSGTYPE_MULTIBYTES);
    Core_LogCpuMsg("CPUVER",cpuver,strlen(cpuver),EN_CPU_MSGTYPE_STRING);
    Core_LogCpuMsg("COREVER",corever,strlen(corever),EN_CPU_MSGTYPE_STRING);

    Core_LogCpuMsg("CACHESIZE",&cachelsize,sizeof(cachelsize),EN_CPU_MSGTYPE_SIZE);
    Core_LogCpuMsg("CACHELINE",&cacheline,sizeof(cacheline),EN_CPU_MSGTYPE_SIZE);
    Core_LogCpuMsg("RAMSIZE",&ramsize,sizeof(ramsize),EN_CPU_MSGTYPE_SIZE);
    Core_LogCpuMsg("ROMSIZE",&romsize,sizeof(romsize),EN_CPU_MSGTYPE_SIZE);

    return ;
}



