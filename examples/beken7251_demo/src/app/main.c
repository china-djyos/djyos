
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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
/*
 * app_main.c
 *
 *  Created on: 2014-5-28
 *      Author: Administrator
 */

#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "cpu_peri_wifi.h"
#include <sys/dhcp.h>
#include <sys/socket.h>

static struct sta_scan_res *scan_result = NULL;

static void scan_ap_callback(void *ctxt, uint8_t param)
{
    uint32_t ap_num = DjyWifi_GetScanResult(&scan_result);
}

ptu32_t djy_main(void)
{
    int i = 0;
    DjyWifi_ApOpen("djyos-yunap","djyos12345");
    DjyWifi_StartScan(scan_ap_callback);
    i = 40;
    do
    {
        i--;
        printf("ap mode!\r\n");
        Djy_EventDelay(2000*1000);
    }while(i>0);
    DjyWifi_ApClose();
    DjyWifi_StaAdvancedConnect("djyos-ap", "djyos12345");
    if(DhcpAddClientTask(CFG_NETCARD_NAME))
   {
      printf("%s:Add %s success\r\n",__FUNCTION__,CFG_NETCARD_NAME);
   }
   else
   {
       printf("%s:Add %s failed\r\n",__FUNCTION__,CFG_NETCARD_NAME);
   }

   do
   {
       printf("wait connect!\r\n");
       Djy_EventDelay(2000*1000);
   }while(DHCP_ConnetStatus(EN_DHCP_GET_STATUS_CMD,0)!=EN_DHCP_CONNET_STATUS);
   DjyWifi_StaConnectDone();
   while(1)
   {
      printf("sta mode!\r\n");
      Djy_EventDelay(2000*1000);
   }
    return 0;
}


