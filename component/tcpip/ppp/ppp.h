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
//date  :����4:43:24/2017��1��4��
//usage :
//------------------------------------------------------

#ifndef HD6072C87_1D75_49F4_99A6_5C6D8005AEF2
#define HD6072C87_1D75_49F4_99A6_5C6D8005AEF2

//standard includes
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <os.h>

//defines for the point to point:rfc1661.rfc1662
/*
 * Significant octet values.
 */
#define CN_PPP_ALLSTATIONS 0xff    /* All-Stations broadcast address */
#define CN_PPP_UI          0x03    /* CTRL information not used*/
#define CN_PPP_FLAG        0x7e    /* Flag Sequence */
#define CN_PPP_ESCAPE      0x7d    /* Asynchronous Control Escape */
#define CN_PPP_TRANS       0x20    /* Asynchronous transparency modifier */
/*
 * Protocol field values.
 */
#define CN_PPP_IP          0x0021  /* Internet Protocol */
#define CN_PPP_AT          0x0029  /* AppleTalk Protocol */
#define CN_PPP_VJC_COMP    0x002d  /* VJ compressed TCP */
#define CN_PPP_VJC_UNCOMP  0x002f  /* VJ uncompressed TCP */
#define CN_PPP_COMP        0x00fd  /* compressed packet */
#define CN_PPP_IPCP        0x8021  /* IP Control Protocol */
#define CN_PPP_ATCP        0x8029  /* AppleTalk Control Protocol */
#define CN_PPP_CCP         0x80fd  /* Compression Control Protocol */
#define CN_PPP_LCP         0xc021  /* Link Control Protocol */
#define CN_PPP_PAP         0xc023  /* Password Authentication Protocol */
#define CN_PPP_LQR         0xc025  /* Link Quality Report protocol */
#define CN_PPP_CHAP        0xc223  /* Cryptographic Handshake Auth. Protocol */
#define CN_PPP_CBCP        0xc029  /* Callback Control Protocol */

/*
 *  CP (LCP, IPCP, etc.) codes.
 */
#define CONFREQ     1 /* Configuration Request */
#define CONFACK     2 /* Configuration Ack */
#define CONFNAK     3 /* Configuration Nak */
#define CONFREJ     4 /* Configuration Reject */
#define TERMREQ     5 /* Termination Request */
#define TERMACK     6 /* Termination Ack */
#define CODEREJ     7 /* Code Reject */
#define PROTOREJ    8 /* protocol reject */
#define ECHOREQ     9 /* echo request */
#define ECHOREP     10/* echo reply */
#define DISREQ      11/* discard request*/
//lcp options supported here
#define LCP_OPT_MRU     1
#define LCP_OPT_ACCM    2
#define LCP_OPT_AUTH    3
#define LCP_OPT_QUAL    4
#define LCP_OPT_MAGIC   5
#define LCP_OPT_PFC     7
#define LCP_OPT_ACFC    8
#define PPP_MTU         1500     /* Default MTU (size of Info field) */
//ncp options
#define NCP_OPT_VJTCP 0x02
#define NCP_OPT_IP    0x03
#define NCP_OPT_DNS   0x81
#define NCP_OPT_NBNS  0x82
#define NCP_OPT_DNSBAK  0x83
#define NCP_OPT_NBNSBAK 0x84

typedef bool_t (*fnIO2PPP)(char *dev, char *apn);
typedef bool_t (*fnIORst)(void);


#endif /* HD6072C87_1D75_49F4_99A6_5C6D8005AEF2 */
