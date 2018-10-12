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
// Copyright (c) 2018 ����Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

//�ö������ڼ���ջʹ�õ��ڴ��Ƿ��ǺϷ���ַ
typedef struct
{
    const u8   *start;
    const u32   len;
}tagVmMemItem;
//�ö������ڶ��������ǵ�����ģʽ
typedef enum
{
    EN_BOOT_POWERDOWN = 0,//WHICH MEANS BOOT FROM POWER DOWN
    EN_BOOT_HRST,         //WHICH MEANS BOOT FROM EXTERNAL HARD RESET BUT POWERON
    EN_BOOT_SRST,         //WHICH MEANS BOOT FROM INTERNAL SOFT RESET
    EN_BOOT_REBOOT,       //WHICH MEANS BOOT FROM SOFT REBOOT
    EN_BOOT_RELOAD,       //WHICH MEANS BOOT FROM RELOAD
    EN_BOOT_UNKNOWN,      //WHICH MEANS BOOT FROM UNKNOWN REASON
    EN_BOOT_NOTIMPLEMENT, //WHICH MEANS BOOT BSP NOT IMPLEMENT
    EN_BOOT_LAST,
}enBootMode;

#define CN_BOOT_LEGALKEY   0XAA55AA55  //����reboot��reset��reload�����key�������¼���������Ϊ�쳣��¼
typedef enBootMode  (*fnGetBootMode)(void); //���ڻ�ȡ���ǵ�CPU����ģʽ�ĺ���ԭ��
//��װϵͳ������¼�洢ģ�飺tab�����ǵ�ջ����ʹ�õ��ڴ淶Χ�������Ƕ����NULL����,���ɸ��ģ�GetOsBootModeHard�����������־����ҪBSP�ṩ��GetOsBootModeSoft��
//���������־����ҪBSP�ṩ
bool_t ModuleInstall_OsBoot(const tagVmMemItem *tab[],fnGetBootMode GetOsBootModeHard,fnGetBootMode GetOsBootModeSoft);
//�׳��쳣������Ϣ��������ϵͳ��reboot��reset��reload���ṩ�Ƿ���KEY��ʱ��
//�Ϸ���KEY��0xaa55aa55,�����ľ�����Ϊ�ǷǷ��ģ��ⲿ����Ҫcpu��bsp ��port��֧��
bool_t ThrowOsBootInfo(enBootMode mode);





