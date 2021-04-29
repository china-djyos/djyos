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
//����ģ��: uart��������(�����ӿڲ���)
//����:  ������.
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//1. ����:2013-11-10
//   ����:  ������.
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
//------------------------------------------------------------------------------
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <objhandle.h>
#include <ring.h>
#include <systime.h>
#include <lock.h>
#include <djyos.h>
#include <multiplex.h>
#include <device.h>
#include <device/djy_uart.h>
#include <int.h>
#include "dbug.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"uart device file"//uart�豸ϵͳ
//parent:"device file system"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock","heap","ring buffer and line buffer"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_UART_DEVICE_FILE == false )
//#warning  " uart_device_file  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_UART_DEVICE_FILE    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


//һ��ģʽ״̬�ṹ
struct UartGeneralCB
{
    struct Object *Host;
    struct SemaphoreLCB *BlockingSemp;         // �����ź���
//  struct MultiplexObjectCB * pMultiplexUart;      // ��·����Ŀ�����ͷָ��
    u32 Baud;                                  // ���ڵ�ǰ������
    ptu32_t UartPortTag;                       //���ڱ�ǩ
    UartStartSend StartSend;
    UartControl UartCtrl;
    struct MutexLCB *ReadMutex;        //������,�����豸��ռʽ������
    struct MutexLCB *WriteMutex;       //������,�����豸��ռʽд����
    struct RingBuf SendRingBuf;                //���η��ͻ�����.
    struct RingBuf RecvRingBuf;                //���ν��ջ�����.
    struct SemaphoreLCB *SendRingBufSemp;      //���ͻ������ź���
    struct SemaphoreLCB *RecvRingBufSemp;      //���ջ������ź���
//  u32 RecvRingTrigLevel;      //����Ring���մ���ˮƽ,���յ���������ʱ�ͷ��ź���
    u32 SendRingTrigLevel;      //����Ring���ʹ���ˮƽ�������ͻ��λ�����������
                                //��SendRingBufSemp������ǰ�̣߳�ֱ�����ͻ���
                                //������������SendRingTrigLevelʱ�ͷ�֮��
//  u32 MplReadTrigLevel;       //��·���ÿɶ�����ˮƽ
//  u32 MplWriteTrigLevel;      //��·���ÿ�д����ˮƽ
};

// ����״̬���ƽṹ
struct UartPollCB
{
    struct Object *Host;
    struct SemaphoreLCB *BlockingSemp;              //�����ź���
//  struct MultiplexObjectCB * pMultiplexUart;      //��·����Ŀ�����ͷָ��
    u32 Baud;                                       //���ڵ�ǰ������
    struct MutexLCB *ReadMutex;        //������,�����豸��ռʽ������
    struct MutexLCB *WriteMutex;       //������,�����豸��ռʽд����
    ptu32_t UartPortTag;                            //���ڱ�ǩ
    UartSendPkg SendPkg;
    UartControl UartCtrl;
    u32 RecvLen;                                    //���յ������ݳ���
};

s32 UART_Open(struct objhandle *hdl, u32 Mode,u32 timeout);
s32 UART_AppWrite(struct objhandle *hdl,u8* src_buf,u32 len,u32 offset, u32 timeout);
s32 UART_AppRead(struct objhandle *hdl,u8* dst_buf,u32 len,u32 offset, u32 timeout);
s32 UART_Ctrl(struct objhandle *hdl,u32 cmd,va_list *arg0);
s32 UART_Poll_Open(struct objhandle *hdl, u32 Mode,u32 timeout);
s32 UART_PollAppWrite(struct objhandle *hdl,u8* src_buf,u32 len,u32 offset, u32 timeout);
s32 UART_PollAppRead(struct objhandle *hdl,u8* dst_buf,u32 len,u32 offset, u32 timeout);
s32 UART_PollCtrl(struct objhandle* hdl,u32 cmd,va_list *arg0);
struct UartGeneralCB *UART_InstallGeneral(struct UartParam *Param);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----�����豸open����---------------------------------------------------------
//���ܣ��򿪴����豸ʱ�Ļص�������ʹ�ļ�������ָ�򴮿ڶ����Ӧ�Ĵ��ڿ��ƿ顣
//      �����豸�ļ����ǰ�װ����ʱ�����ģ������������𴴽��ļ���
// ������Fp���������Ĵ����ļ�ָ��
//      Mode����ģʽ��O_RDONLY��O_WRONLY��O_RDWR
//      timeout����ʱʱ�䡣
// ���أ�0=success��-1=error
// ��ע��O_RDONLY��O_WRONLY��O_RDWR �����POSIX��2016�������¶���
//-----------------------------------------------------------------------------
s32 UART_Open(struct objhandle *hdl, u32 Mode, u32 timeout)
{
    struct UartGeneralCB *UGCB;

    if(hdl == NULL)
        return (-1);

    UGCB = (struct UartGeneralCB *)Device_GetDrvTag(Handle2fd(hdl));
    Handle_SetContext(hdl, (ptu32_t)UGCB);  //Fd������ָ�򴮿ڿ��ƿ�struct UartGeneralCB
    if( ! Ring_IsEmpty(&UGCB->RecvRingBuf))
    {
        //�ļ���δ�����κζ�·���ü���ֱ�ӳ�ʼ������
        Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ);
    }

    if( ! Ring_IsFull(&UGCB->SendRingBuf))
    {
        //�ļ���δ�����κζ�·���ü���ֱ�ӳ�ʼ������
        Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_WRITE);
    }
    return 0;
}

//----�����豸APPд����----------------------------------------------------------
//����: дUART�豸,��Ӧ�ó�����ã�UART��ע��Ϊ�豸�������豸��������
//      Driver_WriteDevice�����ջ���ñ��������������ù���:
//      Dev_Write() ---> Dev->Write() ---> UART_AppWrite()
//      �ú���ʵ�ֵĹ���˵������:
//      1.��δ�����ȣ������ֱ�ӷ��ͷ�ʽ��ֱ�ӷ���ǰ���Ƚ����������ݷ������.
//      2.����ʼ���ȣ�������д�봮��uart_UCB�Ļ��η��ͻ�������.
//      3.д�����ݺ������������ݣ��������͵ķ�ʽ��Ϊ�жϻ���ѯ������������.
//      4.����������������ȴ������������ݽ��͵�����ˮƽ���ټ���д������.
//      5.�й�timeout��ÿ�εȴ��ź�����ʱ�䶼��ݼ�.
//      6.����������д�뻺�������ж��Ƿ��������ͣ���Ϊ��������ȴ������ź����ٷ���.
//      7.����ǰ�����ô������ڵĶ�·���ü���д��Ӧλ���
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ����ֵ
//      buf������Դָ��.
//      len��������(bytes).
//      offset,ƫ�������ڴ���ģ���У��˱�����Ч
//      block_option,����ѡ�����Ϊ������ʽ.��block_optionΪtrue��������ȴ�����
//         ���������ݷ�����ɺ󷵻أ������������ͷ�ʽ����block_optionΪfalseʱ������
//         ��ȴ��������ݱ����봮��ģ��Ļ������к����̷��أ����������ͷ�ʽ�ں���
//         ����ʱ�����ڿ������ڷ������ݡ�
//      timeout,��ʱ��(us).
//����: ʵ��д�뻷�λ��������ַ���
//-----------------------------------------------------------------------------
s32 UART_AppWrite(struct objhandle *hdl, u8* src_buf, u32 len, u32 offset, u32 timeout)
{
    struct UartGeneralCB *UGCB;
    struct Object *UartObj;
    u32 Mode;
    u32 completed = 0,written;
    uint8_t *buf;
    u32 base_time,rel_timeout = timeout;

    if(hdl == NULL)
        return 0;

    UGCB = (struct UartGeneralCB *)Handle_GetContext(hdl);
    Mode = Handle_GetMode(hdl);
    UartObj = Handle_GetHostObj(hdl);
    buf = (uint8_t*)src_buf;
    base_time = (u32)DJY_GetSysTime();

    if(Lock_MutexPend(UGCB->WriteMutex,timeout)==false)
        return completed;

    while(1)
    {
        //��PENDһ���ź�������ֹ�����Ѿ����ͷŹ������͹����У�������ɻ��ͷ����
        //�ź���������block_optionֵ��ʲô�������ͷš�
        Lock_SempPend(UGCB->BlockingSemp,0);
        written = Ring_Write(&UGCB->SendRingBuf,
                            (uint8_t*)buf+completed,len-completed);
        UGCB->StartSend(UGCB->UartPortTag);

        if(written != len-completed)     //����������û���ͳ�ȫ������
        {
            completed += written;
            if(false == Lock_SempPend(UGCB->SendRingBufSemp,rel_timeout))
                break;
            rel_timeout = (u32)DJY_GetSysTime() - base_time;
            if(rel_timeout >= timeout)
                break;
            else
                rel_timeout = timeout - rel_timeout;
        }
        else // ���ˣ��跢�͵������Ѿ�ȫ��copy�������豸������
        {
            if(Handle_FlagIsBlockComplete(Mode)) // ��Ҫblock complete
            {
                //�������ѡ����complete����Ҫ�ȴ�������ɵ��ź�����
                Lock_SempPend(UGCB->BlockingSemp,rel_timeout);
            }
            completed += written;
            break;
        }
    }

    Lock_MutexPost(UGCB->WriteMutex);
    //�Զ�·���õĲ����������� Lock_MutexPost(UGCB->WriteMutex) ֮ǰ���ã���Ϊ��·����
    //�����ڲ�Ҳ�л������������������������� __Multiplex_Set �����ڲ����ô��ڴ�ӡ��
    if(Ring_IsFull(&UGCB->SendRingBuf))
    {
        OBJ_ClrMultiplexEvent(UartObj, CN_MULTIPLEX_SENSINGBIT_WRITE);
    }
    return completed;
}

//----�����豸APP������----------------------------------------------------------
//����: ��UART�豸,��Ӧ�ó�����ã�UART��ע��Ϊ�豸�������豸��������Driver_ReadDevice,
//      ʵ�ʾ��ǵ����˸ú������������ù���:
//      Dev_Read() ---> Dev->Read() ---> UART_AppRead()
//      �ú���ʵ�ֵĹ���˵������:
//      1.���������������ݣ����������������㹻����ֱ�ӷ���
//      2.�����ݲ��������Ի���������һ��Ϊ��λ�������ν�������
//      3.�ڵȴ�����ǰ�����Ƚ��ź��������ÿ�εȴ�ʱ������ܳ�ʱʱ��۳�
//      4.����ǰ�����ô������ڵĶ�·���ü��Ķ���Ӧλ���
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ����ֵ
//      dst_buf,Ӧ�ó����ṩ�Ľ��ջ�����
//      len,���볤��,�ֽ�
//      offset,ƫ����,�ڴ���ģ���У��˱�����Ч
//      timeout,����ʱ����,΢��
//����: ʵ�ʶ�������
//------------------------------------------------------------------------------
s32 UART_AppRead(struct objhandle *hdl,u8* dst_buf,u32 len, u32 offset, u32 timeout)
{
    struct UartGeneralCB *UGCB;
    uint32_t completed = 0;
//    s32 Fd;

    if(hdl == NULL)
        return 0;

//    Fd = Handle2fd(hdl);
    UGCB = (struct UartGeneralCB *)Handle_GetContext(hdl);

    if(Lock_MutexPend(UGCB->ReadMutex,timeout)==false)
        return 0;
    if(Ring_IsEmpty(&UGCB->RecvRingBuf))    //�������գ��ȴ������ź���
    {
        Lock_SempPend(UGCB->RecvRingBufSemp,timeout);

        completed = Ring_Read(&UGCB->RecvRingBuf,(uint8_t*)dst_buf,len);
    }
    else
        completed = Ring_Read(&UGCB->RecvRingBuf,(uint8_t*)dst_buf,len);

    Lock_MutexPost(UGCB->ReadMutex);

    //���������в��������ݣ������·���ô���״̬��
    //�Զ�·���õĲ����������� Lock_MutexPost(UGCB->ReadMutex) ֮ǰ���ã���Ϊ��·����
    //�����ڲ�Ҳ�л������������������������� __Multiplex_Set �����ڲ����ô��ڴ�ӡ��
    if(Ring_IsEmpty(&UGCB->RecvRingBuf))
    {
        //���1��
        Handle_ClrMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ);
        //������ռ���ڡ����1���� UART_PortWrite �����ã���if������
        if( ! Ring_IsEmpty(&UGCB->RecvRingBuf))
        {
            Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ);
        }
    }

    return completed;
}

#pragma GCC diagnostic pop

//----�����豸�˿�д����-------------------------------------------------------
//����: �Ӷ˿�дUART,�ɵײ���������,�ú���ʵ�ֵĹ���˵������:
//      1.������д�봮��uart_UCB�Ļ��ν��ջ�������.
//      2.���������ԭ���ǿյģ������ź����Լ����ö�·���ü��ϣ�֪ͨ�ϲ�ȡȡ��
//      3.��������ʱ����λ��·���ü������Ӧλ
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      buf������Դָ�롣
//      len��������(bytes)
//����: ʵ��д�뻷�λ��������ַ���
//-----------------------------------------------------------------------------
// ============================================================================
u32 UART_PortWrite(struct UartGeneralCB *pUGCB, u8 *pBuf, u32 dwLen)
{
    u16 recv_bytes;
    u32 check;

    if((!pUGCB) || (dwLen == 0))
        return (0);

    recv_bytes = Ring_Write(&pUGCB->RecvRingBuf, pBuf, dwLen);
    if(recv_bytes != dwLen)
    {
        OBJ_SetMultiplexEvent(pUGCB->Host, CN_UART_BUF_OVER_ERR);
    }

    check = Ring_Check(&pUGCB->RecvRingBuf);
    if(check == recv_bytes)
    {
        OBJ_SetMultiplexEvent(pUGCB->Host, CN_MULTIPLEX_SENSINGBIT_READ);
        Lock_SempPost(pUGCB->RecvRingBufSemp);
    }

    return recv_bytes;
}

//----�����豸�˿ڶ�����---------------------------------------------------------
//����: �Ӷ˿ڶ�ȡUART�豸,�ɵײ���������,�ú���ʵ�ֵĹ���˵������:
//      1.��uart_UCB�������ж�ȡ���ݵ��ײ������ṩ��dst_buf.
//      2.��鱾�δӻ������ж�������ǰ���Ƿ������ʹ���ˮƽ��������˷��ʹ���ˮƽ��
//        �����ź�����֪ͨӦ�ò㣬���Լ���д���ݵ�������.
//      3.�ﵽд����ˮƽʱ����λ��·���ü�����־�о�λ
//      4.������������ʱ����������������Ϊ��ʱ���ͷ������ź���.
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      dst_buf,��������
//      len,���볤��,
//����: ʵ�ʶ�������
//------------------------------------------------------------------------------
u32 UART_PortRead(struct UartGeneralCB *pUGCB, u8 *pBuf, u32 dwLen)
{
    u32 check, result = 0;

    if(!pUGCB)
        return (0);

    result = (ptu32_t)Ring_Read(&pUGCB->SendRingBuf, pBuf, dwLen);
    check = Ring_Check(&pUGCB->SendRingBuf);
    if((check <= pUGCB->SendRingTrigLevel)
            && (check + result > pUGCB->SendRingTrigLevel))
    {
        Lock_SempPost(pUGCB->SendRingBufSemp);
        OBJ_SetMultiplexEvent(pUGCB->Host, CN_MULTIPLEX_SENSINGBIT_WRITE);
    }
//  if((check <= UGCB->MplWriteTrigLevel)
//          && (check + result > UGCB->MplWriteTrigLevel))
//  {
//      OBJ_SetMultiplexEvent(UartObj, CN_MULTIPLEX_SENSINGBIT_WRITE);
//  }
    if(result == 0)
    {
        Lock_SempPost(pUGCB->BlockingSemp);
    }

    return result;
}

//----���ڴ�������----------------------------------------------------------
//����: ������������������ʱ���øú��������ö�·���ü���Ӧ�Ĵ����ʶλ
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      ErrNo,�����ʶ�ı���λ���ñ���λ�����Ƕ�·����ģ��δ�õ��о�λ����3-30���أ�
//����: 0������1����ȷ
//------------------------------------------------------------------------------
u32 UART_ErrHandle(struct UartGeneralCB *pUGCB, u32 dwErrNo)
{
    if(!pUGCB)
        return (0);

    OBJ_SetMultiplexEvent(pUGCB->Host, dwErrNo);
    return (1);
}

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���,��Ӧ�ó�����ã�UART��ע��Ϊ�豸�������豸��������
//      Dev_Ctrl��ʵ�ʾ��ǵ����˸ú������������ù���:
//      Dev_Ctrl() ---> Dev->dCtrl() ---> UART_Ctrl()
//      �ú���ʵ�ֵĹ��ܸ��������ַ�������˵������:
//      1.����ֹͣ���ڣ��ɵײ�����ʵ��
//      2.���ð�˫�����ͻ����״̬���ɵײ�����ʵ��
//      3.��ͣ�ͻָ��������ݣ��ɵײ�����ʵ��
//      4.���ò����ʺ�Ӳ������ˮƽ���ɵײ�����ʵ��
//      5.���ô��󵯳��¼�����
//      ......
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
//-----------------------------------------------------------------------------
s32 UART_Ctrl(struct objhandle* hdl,u32 cmd, va_list *arg0)
{
    struct UartGeneralCB *UGCB;
    ptu32_t result = 0;

    if(hdl == NULL)
        return 0;
    UGCB = (struct UartGeneralCB *)Handle_GetContext(hdl);
    switch(cmd)
    {
        case CN_DEV_CTRL_START:
        case CN_DEV_CTRL_STOP:
        case CN_DEV_CTRL_SHUTDOWN:
        case CN_DEV_CTRL_SLEEP:
        case CN_DEV_CTRL_RESUME:
        case CN_UART_COM_SET:
        case CN_UART_RX_PAUSE:           //��ͣ����
        case CN_UART_RX_RESUME:          //�ָ�����
        case CN_UART_RECV_HARD_LEVEL:    //���ý���fifo����ˮƽ
        case CN_UART_SEND_HARD_LEVEL:    //���÷���fifo����ˮƽ
        case CN_UART_EN_RTS:             //ʹ��rts����
        case CN_UART_DIS_RTS:            //��ֹrts����
        case CN_UART_EN_CTS:             //ʹ��cts����
        case CN_UART_DIS_CTS:            //��ֹcts����
        case CN_UART_DMA_USED:           //ʹ��dma����
        case CN_UART_DMA_UNUSED:         //��ֹdma����
        case CN_UART_HALF_DUPLEX_SEND:
        case CN_UART_HALF_DUPLEX_RECV:
        case CN_UART_SET_BAUD:          //����Baud
        {
            UGCB->UartCtrl(UGCB->UartPortTag,cmd,arg0);
            break;
        }
        case CN_UART_SEND_SOFT_LEVEL:   //���÷����������ˮƽ
        {
            UGCB->SendRingTrigLevel = va_arg(*arg0,u32);
            break;
        }

        default: break;
    }
    return result;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----�����豸open����---------------------------------------------------------
//���ܣ��򿪴����豸ʱ�Ļص�������ʹ�ļ�������ָ�򴮿ڶ����Ӧ�Ĵ��ڿ��ƿ顣
//      �����豸�ļ����ǰ�װ����ʱ�����ģ������������𴴽��ļ���
//������hdl���������Ĵ����ļ�ָ��
//      Mode����ģʽ
//      timeout����ʱʱ�䡣
//���أ�0=success��-1=error
//-----------------------------------------------------------------------------
s32 UART_Poll_Open(struct objhandle *hdl, u32 Mode,u32 timeout)
{
    struct UartPollCB *UPCB;

    if(hdl == NULL)
        return (-1);
    UPCB = (struct UartPollCB*)Device_GetDrvTag(Handle2fd(hdl));
    Handle_SetContext(hdl,(ptu32_t)UPCB);  //Fd������ָ�򴮿ڿ��ƿ�struct UartGeneralCB
    if(UPCB->RecvLen != 0)
    {
        //�ļ���δ�����κζ�·���ü���ֱ�ӳ�ʼ������
        Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ);
    }
    else
    {
        //�ļ���δ�����κζ�·���ü���ֱ�ӳ�ʼ������
        Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ
                               + CN_MULTIPLEX_SENSINGBIT_WRITE);
    }
    return (0);
}
#pragma GCC diagnostic pop

//----�����豸�˿�д����-------------------------------------------------------
//����:  д���ߵ��˿ڡ����������λ�����ֱ��д
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      buf������Դָ�롣
//      len��������(bytes)
//����: ʵ��д����ַ���
//-----------------------------------------------------------------------------
s32 UART_PollAppWrite(struct objhandle *hdl, u8* src_buf, u32 len, u32 offset, u32 timeout)
{
    struct UartPollCB *UPCB;
    s32 completed;

    if((hdl == NULL))
        return 0;

    offset = offset;
    UPCB = (struct UartPollCB *)Handle_GetContext(hdl);
    completed = UPCB->SendPkg(UPCB->UartPortTag, src_buf, len);

    Lock_SempPend(UPCB->BlockingSemp,timeout);
    return (completed);
}

//----�����豸�˿ڶ�����-------------------------------------------------------
//����: ֱ�ӴӶ˿ڶ�ȡ���ݲ��������λ�����
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      dst_buf,��������
//      len,���볤��,
//����: ʵ�ʶ�������
//-----------------------------------------------------------------------------

s32 UART_PollAppRead(struct objhandle *hdl, u8* dst_buf, u32 len, u32 offset, u32 timeout)
{
    struct UartPollCB *UPCB;
    s32 RcvLen ;
    bool_t ErrorFlag = false;

    if((hdl == NULL))
        return 0;

    offset = offset;
    UPCB = (struct UartPollCB *)Handle_GetContext(hdl);

    if(Lock_MutexPend(UPCB->ReadMutex,timeout)==false)
        return 0;
//   Lock_SempPend(UPCB->RecvRingBufSemp,timeout);
    //Int_CutTrunk();
    Int_SaveAsynSignal();

    if(UPCB->RecvLen <= len)
    {
        RcvLen = UPCB->RecvLen;
    }
    else
    {
        RcvLen = len;
        ErrorFlag = true;
    }
    //��driver������copy���û���ͬʱ����ջ�������pollģʽҪ���û�ÿ�ζ��豸��
    //���껺�����е�ȫ�����ݡ�
    fcntl(Handle2fd(hdl), CN_UART_RECV_BUF, dst_buf, RcvLen);
//  UPCB->UartCtrl(UPCB->UartPortTag,CN_UART_RECV_BUF,(u32)dst_buf,RcvLen);

    Handle_ClrMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ);
    UPCB->RecvLen = 0;
    //Int_ContactTrunk();
    Int_RestoreAsynSignal();

    if(ErrorFlag)
        UART_PollPortRead(UPCB);
    Lock_MutexPost(UPCB->ReadMutex);
    return (RcvLen);
}

//----�����豸�˿�д����-------------------------------------------------------
//����: �Ӷ˿�дUART,����Pollģʽ�£����ջ�������driver�����ʱ�����ֻ���𵽶�
//      ·����֪ͨ���á�
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      len��������(bytes)
//����: ʵ��д�뻷�λ��������ַ���
//-----------------------------------------------------------------------------
u32 UART_PollPortWrite(struct UartPollCB *pUPCB, u32 dwLen)
{
    if(!pUPCB)
        return (0);

    if(pUPCB->RecvLen == 0)
    {
        OBJ_SetMultiplexEvent(pUPCB->Host, CN_MULTIPLEX_SENSINGBIT_READ);
    }

    pUPCB->RecvLen += dwLen;
    return (dwLen);
}

//----�����豸�˿ڶ�����---------------------------------------------------------
//����: �Ӷ˿ڶ�ȡUART�豸,�ɵײ���������,���������������ɷ���ʱ�ŵ��ñ�������
//      ����������������ȡAPP���͵����ݣ�ֻ�������ö�·���ü�״̬�Լ���������
//      ��֪ͨ���á�
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//����: 0
//------------------------------------------------------------------------------
u32 UART_PollPortRead(struct UartPollCB *pUPCB)
{
    if(!pUPCB)
        return (0);

    OBJ_SetMultiplexEvent(pUPCB->Host, CN_MULTIPLEX_SENSINGBIT_WRITE);
    Lock_SempPost(pUPCB->BlockingSemp);

    return (0);

}

//----���ڴ�������----------------------------------------------------------
//����: ������������������ʱ���øú��������ö�·���ü���Ӧ�Ĵ����ʶλ
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      ErrNo,�����ʶ�ı���λ���ñ���λ�����Ƕ�·����ģ��δ�õ��о�λ����3-30���أ�
//����: 0������1����ȷ
//------------------------------------------------------------------------------
u32 UART_PollErrHandle(struct UartPollCB *pUPCB, u32 dwErrNo)
{
    if(!pUPCB)
        return (0);

    OBJ_SetMultiplexEvent(pUPCB->Host, dwErrNo);

    return (1);
}

//----����Poll�豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���,��Ӧ�ó�����ã�UART��ע��Ϊ�豸�������豸��������
//      Driver_CtrlDevice��ʵ�ʾ��ǵ����˸ú������������ù���:
//      Dev_Ctrl() ---> Dev->dCtrl() ---> UART_Ctrl()
//      �ú���ʵ�ֵĹ��ܸ��������ַ�������˵������:
//      1.����ֹͣ���ڣ��ɵײ�����ʵ��
//      2.���ð�˫�����ͻ����״̬���ɵײ�����ʵ��
//      3.��ͣ�ͻָ��������ݣ��ɵײ�����ʵ��
//      4.���ò����ʺ�Ӳ������ˮƽ���ɵײ�����ʵ��
//      5.���ô��󵯳��¼�����
//      ......
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
//-----------------------------------------------------------------------------
s32 UART_PollCtrl(struct objhandle* hdl,u32 cmd, va_list *arg0)
{
    struct UartPollCB *UPCB;
    s32 result = 0;

    if(hdl == NULL)
        return (0);

    UPCB = (struct UartPollCB *)Handle_GetContext(hdl);
    result = UPCB->UartCtrl(UPCB->UartPortTag,cmd,arg0);

    return (result);
}

//----���UART�豸-------------------------------------------------------------
//���ܣ���Ӵ����豸��ϵͳ�豸����
//      a����ʼ�����λ�����
//      b���������λ������ź������豸������
//      c����������ӵ��豸����ģ�飬����ʼ���豸����ָ��
// ������UCB�����ڿ��ƿ�
//      Param,������ʼ��UART������������������鿴tagUartParam�ṹ��
//���أ����ڿ��ƿ�ָ�룬NULLʧ��
//-----------------------------------------------------------------------------
#if 0
struct Object * __UART_InstallGeneral(struct UartParam *Param)
{
    struct Object * uart_dev;
    struct UartGeneralCB *UGCB;
    struct MutexLCB *uart_mutexR,*uart_mutexT;
    u8 *pRxRingBuf,*pTxRingBuf;


    UGCB = (struct UartGeneralCB *)M_Malloc(sizeof(struct UartGeneralCB),0);
    if(UGCB == NULL)
        goto exit_from_ucb;
    memset(UGCB,0x00,sizeof(struct UartGeneralCB));
    pRxRingBuf = (u8*)M_Malloc(Param->RxRingBufLen,0);
    if(pRxRingBuf == NULL)
        goto exit_from_rx_ring_buf;
    pTxRingBuf = (u8*)M_Malloc(Param->TxRingBufLen,0);
    if(pTxRingBuf == NULL)
        goto exit_from_tx_ring_buf;

    // ������������ʹ�õ��ź���
    UGCB->BlockingSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart blocking");
    if(UGCB->BlockingSemp == NULL)
        goto exit_from_blocking_semp;
    // �������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    // ���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
    UGCB->SendRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart tx buf");
    if(UGCB->SendRingBufSemp == NULL)
        goto exit_from_send_buf_semp;
    UGCB->RecvRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart rx buf");
    if(UGCB->RecvRingBufSemp == NULL)
        goto exit_from_recv_buf_semp;
    // Ϊ�豸�������������ṩ�豸�Ļ������
    uart_mutexR = Lock_MutexCreate("uart receive mutex");
    if(uart_mutexR == NULL)
        goto exit_from_mutexR;
    uart_mutexT = Lock_MutexCreate("uart send mutex");
    if(uart_mutexT == NULL)
        goto exit_from_mutexT;

//  UCB->WriteMutex = Lock_MutexCreate("UART_WriteMutex");
//  if( NULL == UCB->WriteMutex)
//      goto exit_from_mutexWite;
//  UCB->ReadMutex  = Lock_MutexCreate("UART_ReadMutex");
//  if(NULL == UCB->ReadMutex)
//      goto exit_from_mutexRead;

    UGCB->SendRingTrigLevel  = (Param->TxRingBufLen)>>2;  //Ĭ�ϻ��崥��ˮƽΪ1/16
//    UCB->RecvRingTrigLevel  = (Param->RxRingBufLen)>>4;
//    UCB->MplReadTrigLevel   = Param->RxRingBufLen + 1;
//  UGCB->MplWriteTrigLevel  = Param->TxRingBufLen + 1;
    UGCB->Baud               = Param->Baud;
    UGCB->UartPortTag        = Param->UartPortTag;
    UGCB->StartSend                  = Param->StartSend;
    UGCB->UartCtrl           = Param->UartCtrl;
//  UGCB->pMultiplexUart     = NULL;                     //��ʼ��ʱΪNULL
//  UGCB->MplUartStatus      = CN_MULTIPLEX_SENSINGBIT_WRITE;//��ʼʱ��д���ɶ�
    Ring_Init(&UGCB->SendRingBuf,pTxRingBuf,Param->TxRingBufLen);
    Ring_Init(&UGCB->RecvRingBuf,pRxRingBuf,Param->RxRingBufLen);

    uart_dev = Dev_Create((char*)Param->Name,uart_mutexR,uart_mutexT,
                                    UART_Open,
                                    NULL,
                                    UART_AppWrite,
                                    UART_AppRead,
                                    UART_Ctrl,
                                    (ptu32_t)UGCB
                                   );
    UGCB->Host = uart_dev;
    if(uart_dev == NULL)
        goto exit_from_add_device;

    printk("%s Install General Mode Succeeded!\r\n",Param->Name);
    return uart_dev;
    //������ִ������ͷŴ�������Դ�������ؿ�ָ��
exit_from_add_device:
//    Lock_MutexDelete(UCB->ReadMutex);
//exit_from_mutexRead:
//    Lock_MutexDelete(UCB->WriteMutex);
//exit_from_mutexWite:
    Lock_MutexDelete(uart_mutexT);
exit_from_mutexT:
    Lock_MutexDelete(uart_mutexR);
exit_from_mutexR:
    Lock_SempDelete(UGCB->RecvRingBufSemp);
exit_from_recv_buf_semp:
    Lock_SempDelete(UGCB->SendRingBufSemp);
exit_from_send_buf_semp:
    Lock_SempDelete(UGCB->BlockingSemp);
exit_from_blocking_semp:
    free(pTxRingBuf);
exit_from_tx_ring_buf:
    free(pRxRingBuf);
exit_from_rx_ring_buf:
    free(UGCB);
exit_from_ucb:
    return NULL;
}
#else
struct UartGeneralCB *UART_InstallGeneral(struct UartParam *Param)
{
    struct Object * uart_dev;
    struct UartGeneralCB *UGCB;
    u8 *pRxRingBuf,*pTxRingBuf;

    UGCB = (struct UartGeneralCB *)M_Malloc(sizeof(struct UartGeneralCB),0);
    if(UGCB == NULL)
        goto exit_from_ucb;

    memset(UGCB,0x00,sizeof(struct UartGeneralCB));
    pRxRingBuf = (u8*)M_Malloc(Param->RxRingBufLen,0);
    if(pRxRingBuf == NULL)
        goto exit_from_rx_ring_buf;

    pTxRingBuf = (u8*)M_Malloc(Param->TxRingBufLen,0);
    if(pTxRingBuf == NULL)
        goto exit_from_tx_ring_buf;

    // ������������ʹ�õ��ź���
    UGCB->BlockingSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart blocking");
    if(UGCB->BlockingSemp == NULL)
        goto exit_from_blocking_semp;

    // �������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    // ���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
    UGCB->SendRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart tx buf");
    if(UGCB->SendRingBufSemp == NULL)
        goto exit_from_send_buf_semp;

    UGCB->RecvRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart rx buf");
    if(UGCB->RecvRingBufSemp == NULL)
        goto exit_from_recv_buf_semp;

    // Ϊ�豸�������������ṩ�豸�Ļ������
//  uart_mutexR = Lock_MutexCreate("uart receive mutex");
//  if(uart_mutexR == NULL)
//      goto exit_from_mutexR;
//
//  uart_mutexT = Lock_MutexCreate("uart send mutex");
//  if(uart_mutexT == NULL)
//      goto exit_from_mutexT;

    UGCB->WriteMutex = Lock_MutexCreate("UART_WriteMutex");
    if( NULL == UGCB->WriteMutex)
        goto exit_from_mutexWite;
    UGCB->ReadMutex  = Lock_MutexCreate("UART_ReadMutex");
    if(NULL == UGCB->ReadMutex)
        goto exit_from_mutexRead;

    UGCB->SendRingTrigLevel  = (Param->TxRingBufLen)>>2;  //Ĭ�ϻ��崥��ˮƽΪ1/16
//    UCB->RecvRingTrigLevel  = (Param->RxRingBufLen)>>4;
//    UCB->MplReadTrigLevel   = Param->RxRingBufLen + 1;
//  UGCB->MplWriteTrigLevel  = Param->TxRingBufLen + 1;
    UGCB->Baud               = Param->Baud;
    UGCB->UartPortTag        = Param->UartPortTag;
    UGCB->StartSend          = Param->StartSend;
    UGCB->UartCtrl           = Param->UartCtrl;
//  UGCB->pMultiplexUart     = NULL;                     //��ʼ��ʱΪNULL
//  UGCB->MplUartStatus      = CN_MULTIPLEX_SENSINGBIT_WRITE;//��ʼʱ��д���ɶ�
    Ring_Init(&UGCB->SendRingBuf,pTxRingBuf,Param->TxRingBufLen);
    Ring_Init(&UGCB->RecvRingBuf,pRxRingBuf,Param->RxRingBufLen);

    uart_dev = Device_Create(Param->Name, //uart_mutexR,uart_mutexT,
                          UART_Open,
                          NULL,
                          UART_AppWrite,
                          UART_AppRead,
                          UART_Ctrl,
                          (ptu32_t)UGCB
                                   );
    UGCB->Host = uart_dev;
    if(uart_dev == NULL)
        goto exit_from_add_device;

    info_printf("device","%s initialized in general mode.", Param->Name);
    return (UGCB);

    //������ִ������ͷŴ�������Դ�������ؿ�ָ��
exit_from_add_device:
    Lock_MutexDelete(UGCB->ReadMutex);
exit_from_mutexRead:
    Lock_MutexDelete(UGCB->WriteMutex);
exit_from_mutexWite:
//    Lock_MutexDelete(uart_mutexT);
//exit_from_mutexT:
//    Lock_MutexDelete(uart_mutexR);
//exit_from_mutexR:
    Lock_SempDelete(UGCB->RecvRingBufSemp);
exit_from_recv_buf_semp:
    Lock_SempDelete(UGCB->SendRingBufSemp);
exit_from_send_buf_semp:
    Lock_SempDelete(UGCB->BlockingSemp);
exit_from_blocking_semp:
    free(pTxRingBuf);
exit_from_tx_ring_buf:
    free(pRxRingBuf);
exit_from_rx_ring_buf:
    free(UGCB);
exit_from_ucb:

    error_printf("device","%s initialized in general mode failed.", Param->Name);
    return NULL;
}
#endif

#if 0
struct Object * __UART_InstallPoll(struct UartParam *Param)
{
    struct Object * uart_dev;
    struct UartPollCB *UPCB;
    struct MutexLCB *uart_mutexR,*uart_mutexT;

    UPCB = (struct UartPollCB *)M_Malloc(sizeof(struct UartPollCB),0);
    if(UPCB == NULL)
        goto exit_from_ucb;
    memset(UPCB,0x00,sizeof(struct UartPollCB));

    //������������ʹ�õ��ź���
    UPCB->BlockingSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart blocking");
    if(UPCB->BlockingSemp == NULL)
        goto exit_from_blocking_semp;
    //Ϊ�豸�������������ṩ�豸�Ļ������
    uart_mutexR = Lock_MutexCreate("uart receive mutex");
    if(uart_mutexR == NULL)
        goto exit_from_mutexR;
    uart_mutexT = Lock_MutexCreate("uart send mutex");
    if(uart_mutexT == NULL)
        goto exit_from_mutexT;
    UPCB->Baud               = Param->Baud;
    UPCB->UartPortTag        = Param->UartPortTag;
    UPCB->UartCtrl           = Param->UartCtrl;
//  UPCB->pMultiplexUart     = NULL;                     //��ʼ��ʱΪNULL
//  UPCB->MplUartStatus      = CN_MULTIPLEX_SENSINGBIT_WRITE;//��ʼʱ��д���ɶ�

    uart_dev = Dev_Create((char*)Param->Name,uart_mutexR,uart_mutexT,
                                    UART_Poll_Open,
                                    NULL,
                                    UART_PollAppWrite,
                                    UART_PollAppRead,
                                    UART_PollCtrl,
                                    (ptu32_t)UPCB
                                   );
    UPCB->Host = uart_dev;
    if(uart_dev == NULL)
        goto exit_from_add_device;

    printk("%s Install Poll Mode Succeeded!\r\n",Param->Name);

    return uart_dev;
    //������ִ������ͷŴ�������Դ�������ؿ�ָ��
exit_from_add_device:
    Lock_MutexDelete(uart_mutexT);
exit_from_mutexT:
    Lock_MutexDelete(uart_mutexR);
exit_from_mutexR:
    Lock_SempDelete(UPCB->BlockingSemp);
exit_from_blocking_semp:
    free(UPCB);
exit_from_ucb:
    return NULL;
}
#else
struct UartPollCB *UART_InstallPoll(struct UartParam *Param)
{
    struct Object * uart_dev;
    struct UartPollCB *UPCB;
    struct MutexLCB *uart_mutexR,*uart_mutexT;

    UPCB = (struct UartPollCB *)M_Malloc(sizeof(struct UartPollCB),0);
    if(UPCB == NULL)
        goto exit_from_ucb;

    memset(UPCB,0x00,sizeof(struct UartPollCB));
    //������������ʹ�õ��ź���
    UPCB->BlockingSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart blocking");
    if(UPCB->BlockingSemp == NULL)
        goto exit_from_blocking_semp;

    //Ϊ�豸�������������ṩ�豸�Ļ������
    uart_mutexR = Lock_MutexCreate("uart receive mutex");
    if(uart_mutexR == NULL)
        goto exit_from_mutexR;

    uart_mutexT = Lock_MutexCreate("uart send mutex");
    if(uart_mutexT == NULL)
        goto exit_from_mutexT;

    UPCB->Baud               = Param->Baud;
    UPCB->UartPortTag        = Param->UartPortTag;
    UPCB->UartCtrl           = Param->UartCtrl;
//  UPCB->pMultiplexUart     = NULL;                     //��ʼ��ʱΪNULL
//  UPCB->MplUartStatus      = CN_MULTIPLEX_SENSINGBIT_WRITE;//��ʼʱ��д���ɶ�

    uart_dev = Device_Create((const char*)Param->Name,// uart_mutexR, uart_mutexT,
                        UART_Poll_Open,
                        NULL,
                        UART_PollAppWrite,
                        UART_PollAppRead,
                        UART_PollCtrl,
                        (ptu32_t)UPCB
                       );
    UPCB->Host = uart_dev;
    if(uart_dev == NULL)
        goto exit_from_add_device;

    info_printf("device","%s initialized in poll mode.", Param->Name);
    return (UPCB);

    //������ִ������ͷŴ�������Դ�������ؿ�ָ��
exit_from_add_device:
    Lock_MutexDelete(uart_mutexT);
exit_from_mutexT:
    Lock_MutexDelete(uart_mutexR);
exit_from_mutexR:
    Lock_SempDelete(UPCB->BlockingSemp);
exit_from_blocking_semp:
    free(UPCB);
exit_from_ucb:
    error_printf("device","%s initialized in poll mode failed.", Param->Name);
    return (NULL);
}
#endif
#if 0
//----���UART�豸-------------------------------------------------------------
//���ܣ���Ӵ����豸��ϵͳ�豸����
//      a����ʼ�����λ�����
//      b���������λ������ź������豸������
//      c����������ӵ��豸����ģ�飬����ʼ���豸����ָ��
//������UCB�����ڿ��ƿ�
//      Param,������ʼ��UART������������������鿴tagUartParam�ṹ��
//���أ����ڿ��ƿ�ָ�룬NULLʧ��
//-----------------------------------------------------------------------------
struct Object * UART_InstallPort(struct UartParam *Param)
{
    if(Param == NULL)
        return NULL;

    if(Param->mode == CN_UART_POLL)
    {
        return __UART_InstallPoll(Param);
    }
    else
    {
        return __UART_InstallGeneral(Param);
    }
}
#endif
