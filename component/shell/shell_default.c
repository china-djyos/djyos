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
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <version.h>
#include <time.h>
#include <systime.h>
#include "../include/shell.h"

// ============================================================================
// ���ܣ���ʾos�İ汾
// ������
// ���أ�
// ��ע��
// ============================================================================

bool_t djyos_ver(char *param)
{
    printf("\r\n%s\r\n", djyos_kernel_version);
    if(param)
        return true;
    return true;

}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// ============================================================================
// ���ܣ���ʾ������������
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t date(char *param)
{
    s64 nowtime;
    struct tm dtm;
    char command[20];
    char buf[12];
    int res;

    nowtime = Time_Time(NULL);
    Time_LocalTime_r(&nowtime,&dtm);
    Time_AscTime(&dtm,command);

    printf("\r\n��ǰʱ�䣺%10.10s %s",command, g_cTmWdays[dtm.tm_wday]);
    printf("\r\n���������ڣ�");

    fgets(buf,11,stdin);
    if(strlen(buf) != 0)
    {
        memcpy(command,buf,10);
        res = Time_SetDateTimeStr(command);
        switch (res)
        {
        case EN_CLOCK_YEAR_ERROR:
            printf("��ݴ���");
            break;
        case EN_CLOCK_MON_ERROR:
            printf("�·ݴ���");
            break;
        case EN_CLOCK_DAY_ERROR:
            printf("���ڴ���");
            break;
        case EN_CLOCK_FMT_ERROR:
            printf("��ʽ����");
            break;
        default:
            break;
        }
    }

    printf("\r\n");
    return true;
}

// ============================================================================
// ���ܣ���ʾ��ǰʱ���������������ʱ��
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t __time(char *param)
{
    s64 nowtime;
    struct tm dtm;
    char command[20];
    int res;

    nowtime = Time_Time(NULL);
    Time_LocalTime_r(&nowtime,&dtm);
    Time_AscTime(&dtm,command);

    printf("\r\n��ǰʱ�䣺%s",command+11);
    printf("\r\n������ʱ�䣺");

    fgets(command+11,9,stdin);
    if(strlen(command+11) != 0)
    {
        res = Time_SetDateTimeStr(command);
        switch (res)
        {
        case EN_CLOCK_HOUR_ERROR:
            printf("Сʱ����");
            break;
        case EN_CLOCK_MIN_ERROR:
            printf("���Ӵ���");
            break;
        case EN_CLOCK_SEC_ERROR:
            printf("���Ӵ���");
            break;
        case EN_CLOCK_FMT_ERROR:
            printf("��ʽ����");
            break;
        default:
            break;
        }
    }

    printf("\r\n");
    return true;
}
#pragma GCC diagnostic pop

// ============================================================================
// ���ܣ���ʾ�ڴ棬��ʾĳ��ַ��ʼ��һ���ڴ棬ÿ����ʾ16����Ԫ��ֻ��ʾ�������޸�
// ������param -- ����������������Ҫ�����������ÿո����
//               ����1:��ʼ��ַ��10���ƻ�16����(0x��ͷ)
//               ����2:��ʾ�ĵ�Ԫ��
//               ����3:ÿ��Ԫ�ֽ������Ϸ�ֵ��1��2��4��������ֵ�����ش���
//               ����4:����������
// ���أ�true=������ʾ��false=����
// ��ע��
// ============================================================================

bool_t d(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len;
    char *word_addr,*word_un,*word_ub,*word_trail,*next_param;

    //��ȡ3������
    word_addr = shell_inputs(param,&next_param);
    word_un = shell_inputs(next_param,&next_param);
    word_ub = shell_inputs(next_param,&next_param);
    word_trail = shell_inputs(next_param,&next_param);
    if((word_addr == NULL)||(word_un == NULL)
            ||(word_ub == NULL)||(word_trail != NULL))
    {
        printf("\r\n��ʽ������ȷ��ʽ�ǣ�\r\n>d ��ַ ��Ԫ�� ÿ��Ԫ�ֽ���\r\n");
        return false;
    }
    addr = strtoul(word_addr, (char **)NULL, 0);
    unit_num = strtol(word_un, (char **)NULL, 0);
    unit_bytes = strtol(word_ub, (char **)NULL, 0);
#if (CN_BYTE_BITS == 8)  //�ֽ�λ��=8����������
    switch(unit_bytes)
    {
        case 1:
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%02x ", *(uint8_t*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        case 2:
            addr &= ~(ptu32_t)1;//ֻ�ܴ�ż����ʼ
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%04x ", *(u16*)addr);
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        case 4:
            addr &= ~(ptu32_t)3;//ֻ�ܴ�4�ֽڶ��뿪ʼ
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=4;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        case 8:
            addr &= ~(ptu32_t)7;//ֻ�ܴ�8�ֽڶ��뿪ʼ
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x", *(uint32_t*)addr);
                addr +=4;
                printf("%08x ", *(uint32_t*)addr);
                addr +=4;
                if(addr %32 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        default :
            printf("\r\n��������\r\n");
            return false;
            break;
    }
#elif (CN_BYTE_BITS == 16)  //�ֽ�λ��=16
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%04x ", *(u16*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        case 4:
        {
            addr &= ~(ptu32_t)3;
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=2;
                printf("%08x ", *(uint32_t*)addr);
                putchar(' ');
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        default :
        {
            printf("\r\n��������\r\n");
            return false;
        } break;
    }
#elif (CN_BYTE_BITS == 32)  //�ֽ�λ��=32
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=2;
                printf("%08x ", *(uint32_t*)addr);
                putchar(' ');
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        default :
        {
            printf("\r\n��������\r\n");
            return false;
        } break;
    }
#endif
    printf("\r\n");
    return true;
}

// ============================================================================
// ���ܣ�д��ĳ��ַ��ʼ��һ���ڴ�
// ������param -- ����������������Ҫ�����������ÿո����
//               ����1:��ʼ��ַ��10���ƻ�16����(0x��ͷ)
//               ����2:��ʾ�ĵ�Ԫ��
//               ����3:ÿ��Ԫ�ֽ������Ϸ�ֵ��1��2��4��������ֵ�����ش���
//               ����4:����������
// ���أ�true=������ʾ��false=����
// ��ע��
// ============================================================================

bool_t f(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len,data;
    char *word,*next_param;

    word = shell_inputs(param,&next_param);
    addr = strtoul(word, (char **)NULL, 0);
    word = shell_inputs(next_param,&next_param);
    unit_num = strtol(word, (char **)NULL, 0);
    word = shell_inputs(next_param,&next_param);
    unit_bytes = strtol(word, (char **)NULL, 0);
    word = shell_inputs(next_param,&next_param);
    data = strtoul(word, (char **)NULL, 0);

#if (CN_BYTE_BITS == 8)  //�ֽ�λ��=8����������
    switch(unit_bytes)
    {
        case 1:
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(uint8_t*)addr = (u8)data;
                addr ++;
            }
            break;
        case 2:
            addr &= ~(ptu32_t)1;
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u16*)addr = (u16)data;
                addr +=2;
            }
            break;
        case 4:
            addr &= ~(ptu32_t)3;
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr +=4;
            }
            break;
        default :
            printf("\r\n��������\r\n");
            return false;
            break;
    }
#elif (CN_BYTE_BITS == 16)  //�ֽ�λ��=16
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u16*)addr = (u16)data;
                addr ++;
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr +=2;
            }
        } break;
        default :
        {
            printf("\r\n��������\r\n");
            return false;
        } break;
    }
#elif (CN_BYTE_BITS == 32)  //�ֽ�λ��=32
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr ++;
            }
        } break;
        default :
        {
            printf("\r\n��������\r\n");
            return false;
        } break;
    }
#endif
    return true;
}

ADD_TO_ROUTINE_SHELL(f,f,"д���ݵ��ڴ�,�����ʽ��f ��ʼ��ַ  ��Ԫ�� ÿ��Ԫ�ֽ��� �������");
ADD_TO_ROUTINE_SHELL(ver,djyos_ver,"��ʾos�İ汾");
ADD_TO_ROUTINE_SHELL(date,date,"��ʾ������������");
ADD_TO_ROUTINE_SHELL(d,d,"��ȡ�ڴ��������,�����ʽ:d ��ַ ��Ԫ�� ÿ��Ԫ�ֽ���");
ADD_TO_ROUTINE_SHELL(time,time,"��ʾ��ǰʱ���������������ʱ��");
