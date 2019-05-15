//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>


//
// @brief  ��������ȼ�����
//
#define CN_EMERG        0 // system is unusable
#define CN_ALERT        1 // action must be taken immediately
#define CN_CRIT         2 // critical conditions
#define CN_ERR          3 // error conditions
#define CN_WARNING      4 // warning conditions
#define CN_NOTICE       5 // normal but significant condition
#define CN_INFO         6 // informational
#define CN_DEBUG        7 // debug-level messages

extern u32 debuglevel;

/*���Լ������Ϣ*/
#define debug_printf(module,fmt, ...) do{\
if (debuglevel >= (CN_DEBUG)) \
{\
    printf("\r\ndbug : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
}\
}while (0)

/*��ʾ��Ϣ����������������ʱ����ӡӲ����Ϣ*/
#define info_printf(module, fmt, ...) do{\
if (debuglevel >= CN_INFO) \
{\
    printf("\r\ninfo : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
} \
}while (0)

/*����������Ҫ���������������ѡ��������밲ȫ��ص���Ϣ*/
#define notice_printf(module,fmt, ...) \
do{ if (debuglevel >= (CN_NOTICE)) printf("\r\n: note : %-6.6s : "fmt"", module, ##__VA_ARGS__); } while (0)

/*�����������Կ��ܳ��������������о���*/

#define warning_printf(module,fmt, ...) do { \
if (debuglevel >= (CN_WARNING)) \
{\
    printf("\r\nwarn : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
    printf("\r\n       FILE: %s", __FILE__); \
    printf("\r\n       LINE: %d\r\n", __LINE__); \
}\
} while (0)


/*��������������������KERN_ERR������Ӳ���Ĵ���*/
#define error_printf(module,fmt, ...) do { \
if (debuglevel >= (CN_ERR)) \
{\
    printf("\r\nerro : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
    printf("\r\n       FILE: %s", __FILE__); \
    printf("\r\n       LINE: %d\r\n", __LINE__); \
}\
} while (0)

/*�ٽ�������ͨ���漰���ص�Ӳ�����������ʧ��*/
#define critical_printf(module,fmt, ...) do { \
if (debuglevel >= (CN_CRIT)) \
{\
    printf("\r\ncrit : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
    printf("\r\n       FILE: %s", __FILE__); \
    printf("\r\n       LINE: %d\r\n", __LINE__); \
}\
} while (0)

/*������Ϣ����ʾ����������ȡ��ʩ*/
#define alert_printf(module,fmt, ...) do { \
if (debuglevel >= (CN_ALERT)) \
{\
    printf("\r\nalet : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
    printf("\r\n       FILE: %s", __FILE__); \
    printf("\r\n       LINE: %d\r\n", __LINE__); \
}\
} while (0)

/*�����¼���Ϣ��ϵͳ����֮ǰ��ʾ����ʾϵͳ������*/
#define emergency_printf(module,fmt, ...) do { \
if (debuglevel >= (CN_EMERG)) \
{\
    printf("\r\nemer : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
    printf("\r\n       FILE: %s", __FILE__); \
    printf("\r\n       LINE: %d\r\n", __LINE__); \
}\
} while (0)





#ifdef __cplusplus
}
#endif

#endif // ifndef __DEBUG_H__
