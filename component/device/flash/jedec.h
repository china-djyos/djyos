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

#ifndef __JEDEC_H__
#define __JEDEC_H__

#ifdef __cplusplus
extern "C" {
#endif


#define JEDEC_PAGE_READ_OLD                            0xD2
#define JEDEC_ARRAY_READ_LOW_POWER                     0x01
#define JEDEC_ARRAY_READ_LOW_FREQ                      0x03
#define JEDEC_ARRAY_READ_HIGH_FREQ                     0x0B
#define JEDEC_ARRAY_READ_HIGHER_FREQ                   0x1B
#define JEDEC_BUF1_READ_LOW_FREQ                       0xD1
#define JEDEC_BUF2_READ_LOW_FREQ                       0xD3
#define JEDEC_BUF1_READ_HIGH_FREQ                      0xD4
#define JEDEC_BUF2_READ_HIGH_FREQ                      0xD6

#define JEDEC_BUF1_WRITE                               0x84
#define JEDEC_BUF2_WRITE                               0x87
#define JEDEC_PAGE_ERASE_WRITE_BY_BUF1                 0x83
#define JEDEC_PAGE_ERASE_WRITE_BY_BUF2                 0x86
#define JEDEC_PAGE_WRITE_BY_BUF1                       0x88
#define JEDEC_PAGE_WRITE_BY_BUF2                       0x89
#define JEDEC_PAGE_ERASE_WRITE_DIRECT_BY_BUF1          0x82
#define JEDEC_PAGE_ERASE_WRITE_DIRECT_BY_BUF2          0x85
#define JEDEC_PAGE_WRITE_DIRECT_BY_BUF1                0x02
#define JEDEC_PAGE_ERASE                               0x81
#define JEDEC_BLOCK_ERASE                              0x50
#define JEDEC_SECTOR_ERASE                             0x7C
#define JEDEC_CHIP_ERASE                               0xC7, 0x94, 0x80, 0x9A
#define JEDEC_SUSPEND                                  0xB0
#define JEDEC_RESUME                                   0xD0

#define JEDEC_STATUS_READ                              0xD7
#define JEDEC_ID_READ                                  0x9F

#define JEDEC_BUF1_READ                                0x54
#define JEDEC_BUF2_READ                                0x56
#define JEDEC_PAGE_READ                                0x52
#define JEDEC_ARRAY_READ                               0x68
#define JEDEC_STATUS_READ_OLD                          0x57

#ifdef __cplusplus
}
#endif

#endif // ifndef __JEDEC_H__

