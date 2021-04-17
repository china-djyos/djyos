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

#ifndef SRC_DJYOS_INCLUDE_CPUINFO_H_
#define SRC_DJYOS_INCLUDE_CPUINFO_H_

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

//we defines code and the storage for the system
#define MEM_NOCACHED     __attribute__((section(".data.nocached")))   //which used the none cached ram,like the dma
#define CODE_VECTOR      __attribute__((section(".code.vector")))     //exception or interrupt vector
//this storage is loaded before the preload,maybe not loaded,just in the rom storage
#define CODE_BEFOREPRELOAD    __attribute__((section(".code.beforepreload")))   //maybe called at the start up
#define DATA_BEFOREPRELOAD    __attribute__((section(".data.beforepreload")))   //maybe used by the start up
#define BSS_BEFOREPRELOAD     __attribute__((section(".bss.beforepreload")))    //maybe used by the start up
//this storage is loaded before the load,and this storage loaded by the preload
#define CODE_BEFORELOAD    __attribute__((section(".code.beforeload")))         //maybe called by the preload or critical
#define DATA_BEFORELOAD    __attribute__((section(".data.beforeload")))         //maybe used by the preload or critical
#define BSS_BEFORELOAD     __attribute__((section(".bss.beforeload")))          //maybe used by the preload or critical
//this storage is loaded by the Iboot_Loader
#define CODE_NORMAL    __attribute__((section(".code.normal")))                 //loaded by Iboot_Loader
#define DATA_NORMAL    __attribute__((section(".data.normal")))                 //loaded by Iboot_Loader
#define BSS_NORMAL     __attribute__((section(".bss.normal")))                  //loaded by Iboot_Loader


typedef enum
{
    EN_CPU_MSGTYPE_SIZE = 0,  //THIS IS MEAN IT IS THE STORAGE:based bytes
    EN_CPU_MSGTYPE_FREQUENCY, //THIS IS MEAN IT IS THE CLK:based HZ
    EN_CPU_MSGTYPE_STRING,    //THIS IS MEAN IT IS THE FIRMWARE STRING COMMENT
    EN_CPU_MSGTYPE_MULTIBYTES,//THIS IS MEAN IT IS THE BYTES DATA
}enCpuMsgType;

#define CN_CPU_SIGNATURE    "SIGNATURE"   //�����̶ֹ�����CPU��ΨһIDǩ����Ϣ������Ӧ������ΨһID

CODE_BEFOREPRELOAD bool_t Core_LogCpuMsg(const char *msgname, const void *msg,u8 msglen, enCpuMsgType type);
bool_t Core_ShowCpuInfo(char *param);
s32  Core_GetCpuInfo(const char *name,void *buf, u8 len);


#endif /* SRC_DJYOS_INCLUDE_CPUINFO_H_ */
