//----------------------------------------------------
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
//author:zhangqf
//date  :����11:25:43/2017��5��3��
//usage :
//-----------------------------------------------------------------------------

#ifndef __IOCTL_H__
#define __IOCTL_H__

//ioctl ����������ţ��˴�����ֻ������һ����
//���µ�POSIX�Ѿ�������ioctl��������Ȼ���ѣ�������Դ��Ŀ��Ȼ���á�
#define I_ATMARK    0      //Is the top message "marked''?
#define I_CANPUT    1      //Is a band writable?
#define I_CKBAND    2      //See if any messages exist in a band.
#define I_FDINSERT  3      //Send implementation-defined information about another STREAM.
#define I_FIND      4      //Look for a STREAMS module.
#define I_FLUSH     5      //Flush a STREAM.
#define I_FLUSHBAND 6      //Flush one band of a STREAM.
#define I_GETBAND   7      //Get the band of the top message on a STREAM.
#define I_GETCLTIME 8      //Get close time delay.
#define I_GETSIG    9      //Retrieve current notification signals.
#define I_GRDOPT    10     //Get the read mode.
#define I_GWROPT    11     //Get the write mode.
#define I_LINK      12     //Connect two STREAMs.
#define I_LIST      13     //Get all the module names on a STREAM.
#define I_LOOK      14     //Get the top module name.
#define I_NREAD     15     //Size the top message.
#define I_PEEK      16     //Peek at the top message on a STREAM.
#define I_PLINK     17     //Persistently connect two STREAMs.
#define I_POP       18     //Pop a STREAMS module.
#define I_PUNLINK   19     //Dismantle a persistent STREAMS link.
#define I_PUSH      20     //Push a STREAMS module.
#define I_RECVFD    21     //Get a file descriptor sent via I_SENDFD.
#define I_SENDFD    22     //Pass a file descriptor through a STREAMS pipe.
#define I_SETCLTIME 23     //Set close time delay.
#define I_SETSIG    24     //Ask for notification signals.
#define I_SRDOPT    25     //Set the read mode.
#define I_STR       26     //Send a STREAMS ioctl().
#define I_SWROPT    27     //Set the write mode.
#define I_UNLINK    28     //Disconnect two STREAMs.
//TODO�������䣬����stropts.h.html����POSIX�й涨��������������ĳ�������������

s32 ioctl(s32 fildes, s32 request, ...);

#endif /* __IOCTL_H__ */
