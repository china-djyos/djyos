//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//date  :����1:56:41/2017��4��13��
//usage :
//-----------------------------------------------------------------------------

#include <sys/socket.h>



//typedef enum
//{
//	EN_NETDEVEVENT_DOWN = 0,
//	EN_NETDEVEVENT_UP,
//	EN_NETDEVEVENT_REMOVE,
//	EN_NETDEVEVENT_ADD,
//	EN_NETDEVEVENT_RESET,
//}enNetDevEvent;
//typedef bool (*fnNetDevEventHook)(enNetDevEvent event);
#include <sys/socket.h>
#include "cpu_peri.h"
bool_t devEvent(enNetDevEvent event)
{

	switch(event)
	{
	case EN_NETDEVEVENT_DOWN:
		printf("dev down");
		break;
	case EN_NETDEVEVENT_UP:
		printf("dev up");
		break;
	default:
		break;
	}

	return true;
}
//check dev
bool_t pppStart(void)
{
	//fisrt we should check the usb dev up
	printf("wait the usb dev up \n\r");
	while(0 != CUSTOM_DeviceReady())
	{
		printf(".");
		Djy_EventDelay(1000*mS);
	}
	printf("done\n\r");

    const char *iodevname = "umodem";
    const char *netdevname = "pppnet";

    //add the ppp device to the stack
    extern bool_t AtDial(char *devname,char *apn);
    extern bool_t ModemReset(void);
    extern bool_t PppDevAdd(char *netdev,char *iodev,const char *user,const char *passwd,const char *apn,\
    		         bool_t (*fnModemReset)(void),bool_t (*fnModemAtregnet)(char *devname,char *apn),\
    				 fnNetDevEventHook eventhook);
    PppDevAdd(netdevname,iodevname,"cyg","zhangqf","cmnet",ModemReset,AtDial,devEvent);
    AtDefaultDevSet(iodevname);

    return true;
}


