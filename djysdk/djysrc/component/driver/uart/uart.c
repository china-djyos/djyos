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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ring.h"
#include "systime.h"
#include "lock.h"
#include "djyos.h"
#include "multiplex.h"
#include <driver/include/uart.h>
#include "int.h"
//����״̬�������ƽṹ
 struct UartCB
{
    struct SemaphoreLCB *BlockingSemp;      //�����ź���
//  struct MutexLCB *WriteMutex;            //app���̻߳���д����
//  struct MutexLCB *ReadMutex;             //app���̻߳��������
    struct MultiplexObjectCB * pMultiplexUart;      //��·����Ŀ�����ͷָ��
    const char *Name;                   //UART���ƣ���UART0
    u32 MplUartStatus;          //��·���õĵ�ǰ״̬����ɶ�����д��
    u32 Baud;                   //���ڵ�ǰ������
    ptu32_t UartPortTag;        //���ڱ�ǩ
    UartControl UartCtrl;
};
//һ��ģʽ״̬�ṹ
struct UartGeneralCB
{
     struct UartCB UcbBase;           //���������ڽṹ�����ǰ��

     struct RingBuf SendRingBuf;              //���η��ͻ�����.
     struct RingBuf RecvRingBuf;              //���ν��ջ�����.
     struct SemaphoreLCB *SendRingBufSemp;    //���ͻ������ź���
     struct SemaphoreLCB *RecvRingBufSemp;    //���ջ������ź���
 //  u32 RecvRingTrigLevel;      //����Ring���մ���ˮƽ,���յ���������ʱ�ͷ��ź���
     u32 SendRingTrigLevel;      //����Ring���ʹ���ˮƽ�������ͻ��λ�����������
                                 //��SendRingBufSemp������ǰ�̣߳�ֱ�����ͻ���
                                 //������������SendRingTrigLevelʱ�ͷ�֮��
 //  u32 MplReadTrigLevel;       //��·���ÿɶ�����ˮƽ
     u32 MplWriteTrigLevel;      //��·���ÿ�д����ˮƽ
     UartStartSend StartSend;
 //    UartDirectSend DirectlySend;
};


//����״̬���ƽṹ
struct UartPollCB
{
    struct UartCB UcbBase;
    u32 RecvLen;                //���յ������ݳ���
};

//----�����豸APPд����----------------------------------------------------------
//����: дUART�豸,��Ӧ�ó�����ã�UART��ע��Ϊ�豸�������豸��������
//      Driver_WriteDevice�����ջ���ñ��������������ù���:
//      Driver_WriteDevice() ---> Dev->Write() ---> UART_AppWrite()
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
ptu32_t UART_AppWrite(struct UartCB *UCB,u8* src_buf,u32 len,
                      u32 offset,bool_t block_option,u32 timeout)
{
    u32 completed = 0,written;
    uint8_t *buf;
    u32 base_time,rel_timeout = timeout;
    struct UartGeneralCB *UGCB;

    UGCB = Container(UCB, struct UartGeneralCB, UcbBase);
    if(UGCB == NULL)
        return 0;

    buf = (uint8_t*)src_buf;
    base_time = (u32)DjyGetSysTime();

//  if(Lock_MutexPend(UCB->WriteMutex,timeout)==false)
//      return completed;

    //��PENDһ���ź�������ֹ�����Ѿ����ͷŹ������͹����У�������ɻ��ͷ����
    //�ź���������block_optionֵ��ʲô�������ͷš�
    while(1)
    {
        written = Ring_Write(&UGCB->SendRingBuf,
                            (uint8_t*)buf+completed,len-completed);
        Lock_SempPend(UGCB->UcbBase.BlockingSemp,0);
        UGCB->StartSend(UGCB->UcbBase.UartPortTag,rel_timeout);

        if(written != len-completed)     //����������û���ͳ�ȫ������
        {
            completed += written;
            if(false == Lock_SempPend(UGCB->SendRingBufSemp,rel_timeout))
                break;
            rel_timeout = (u32)DjyGetSysTime() - base_time;
            if(rel_timeout >= timeout)
                break;
            else
                rel_timeout = timeout - rel_timeout;
        }else            //���ˣ��跢�͵������Ѿ�ȫ��copy�������豸������
        {
            if(block_option == CN_BLOCK_COMPLETE)
            {
                //�������ѡ����complete����Ҫ�ȴ�������ɵ��ź�����
                Lock_SempPend(UGCB->UcbBase.BlockingSemp,rel_timeout);
            }
            completed += written;
            break;
        }
    }
    if(Ring_IsFull(&UGCB->SendRingBuf))
    {
        UGCB->UcbBase.MplUartStatus &= (~CN_MULTIPLEX_SENSINGBIT_WRITE);
        Multiplex_Set(UGCB->UcbBase.pMultiplexUart,UGCB->UcbBase.MplUartStatus);
    }

//  Lock_MutexPost(UCB->WriteMutex);
    return completed;
}

//----�����豸APP������----------------------------------------------------------
//����: ��UART�豸,��Ӧ�ó�����ã�UART��ע��Ϊ�豸�������豸��������Driver_ReadDevice,
//      ʵ�ʾ��ǵ����˸ú������������ù���:
//      Driver_ReadDevice() ---> Dev->Read() ---> UART_AppRead()
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
ptu32_t UART_AppRead(struct UartCB *UCB,u8* dst_buf,u32 len,
                    u32 offset,u32 timeout)
{
    uint32_t completed = 0;
    struct UartGeneralCB *UGCB;
    UGCB = Container(UCB, struct UartGeneralCB, UcbBase);
    if(UGCB == NULL)
        return 0;

//  base_time = (u32)DjyGetSysTime();
//  if(Lock_MutexPend(UCB->ReadMutex,timeout)==false)
//      return completed;

    if(Ring_IsEmpty(&UGCB->RecvRingBuf))    //�������գ��ȴ������ź���
    {
        Lock_SempPend(UGCB->RecvRingBufSemp,timeout);

        completed = Ring_Read(&UGCB->RecvRingBuf,(uint8_t*)dst_buf,len);
    }
    else
        completed = Ring_Read(&UGCB->RecvRingBuf,(uint8_t*)dst_buf,len);

    //���������в��������ݣ������·���ô���״̬��
    if(Ring_IsEmpty(&UGCB->RecvRingBuf))
    {
        UGCB->UcbBase.MplUartStatus &= (~CN_MULTIPLEX_SENSINGBIT_READ);
        Multiplex_Set(UGCB->UcbBase.pMultiplexUart,UGCB->UcbBase.MplUartStatus);
    }

//  Lock_MutexPost(UCB->ReadMutex);
    return completed;
}

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
ptu32_t UART_PortWrite(struct UartCB *UCB,u8* buf,u32 len,u32 res)
{
    uint16_t recv_bytes;
    uint32_t check;
    struct UartGeneralCB *UGCB;
    UGCB = Container(UCB, struct UartGeneralCB, UcbBase);
    if( (NULL == UGCB) || (len == 0) )
        return 0;
    recv_bytes = Ring_Write(&UGCB->RecvRingBuf, (u8*)buf,len);
    if(recv_bytes != len)
    {
        UGCB->UcbBase.MplUartStatus |= CN_UART_BUF_OVER_ERR;
        Multiplex_Set(UGCB->UcbBase.pMultiplexUart,UGCB->UcbBase.MplUartStatus);

        printf("%s Receive Ring Over!\r\n",UGCB->UcbBase.Name);
    }
    check = Ring_Check(&UGCB->RecvRingBuf);
    if(check == recv_bytes)
    {
        Lock_SempPost(UGCB->RecvRingBufSemp);
        UGCB->UcbBase.MplUartStatus |= CN_MULTIPLEX_SENSINGBIT_READ;
        Multiplex_Set(UGCB->UcbBase.pMultiplexUart,UGCB->UcbBase.MplUartStatus);
    }
    return (ptu32_t)recv_bytes;
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
ptu32_t UART_PortRead(struct UartCB *UCB,u8* dst_buf,u32 len,u32 res)
{
    uint32_t check,result=0;
    struct UartGeneralCB *UGCB;
    UGCB = Container(UCB, struct UartGeneralCB, UcbBase);

    if(NULL == UGCB)
        return result;
    result = (ptu32_t)Ring_Read(&UGCB->SendRingBuf,(u8 *)dst_buf,len);
    check = Ring_Check(&UGCB->SendRingBuf);
    if((check <= UGCB->SendRingTrigLevel)
            && (check + result > UGCB->SendRingTrigLevel))
    {
        Lock_SempPost(UGCB->SendRingBufSemp);
    }
    if((check <= UGCB->MplWriteTrigLevel)
            && (check + result > UGCB->MplWriteTrigLevel))
    {
        UGCB->UcbBase.MplUartStatus |= CN_MULTIPLEX_SENSINGBIT_WRITE;
        Multiplex_Set(UGCB->UcbBase.pMultiplexUart,UGCB->UcbBase.MplUartStatus);
    }
    if(result == 0)
    {
        Lock_SempPost(UGCB->UcbBase.BlockingSemp);
    }
    return result;
}

//----���ڴ�������----------------------------------------------------------
//����: ������������������ʱ���øú��������ö�·���ü���Ӧ�Ĵ����ʶλ
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      ErrNo,�����ʶ�ı���λ���ñ���λ�����Ƕ�·����ģ��δ�õ��о�λ����3-30���أ�
//����: 0������1����ȷ
//------------------------------------------------------------------------------
ptu32_t UART_ErrHandle( struct UartCB *UCB,u32 ErrNo)
{
    u32 Status,result = 0;
    if(NULL != UCB)
    {
        Status = UCB->pMultiplexUart->PendingBit | ErrNo;
        Multiplex_Set(UCB->pMultiplexUart,Status);
        result = 1;
    }
    return result;
}

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���,��Ӧ�ó�����ã�UART��ע��Ϊ�豸�������豸��������
//      Driver_CtrlDevice��ʵ�ʾ��ǵ����˸ú������������ù���:
//      Driver_CtrlDevice() ---> Dev->dCtrl() ---> UART_Ctrl()
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
ptu32_t UART_Ctrl(struct UartCB *UCB,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    ptu32_t result = 0;
    struct UartGeneralCB *UGCB;
    UGCB = Container(UCB, struct UartGeneralCB, UcbBase);

    if(UGCB == NULL)
        return 0;
    switch(cmd)
    {
        case CN_UART_START:
        case CN_UART_STOP:
        case CN_UART_COM_SET:
        case CN_UART_RX_PAUSE:          //��ͣ����
        case CN_UART_RX_RESUME:         //�ָ�����
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
//        case CN_UART_SETTO_ALL_DUPLEX:
            UGCB->UcbBase.UartCtrl(UCB->UartPortTag,cmd,data1,data2);
             break;
        case CN_UART_SET_BAUD:          //����Baud
            if(UGCB->UcbBase.Baud !=data1)
            {
                UGCB->UcbBase.Baud = data1;
                UGCB->UcbBase.UartCtrl(UGCB->UcbBase.UartPortTag,CN_UART_SET_BAUD,data1,data2);
            }
            break;
//      case CN_UART_RECV_SOFT_LEVEL:   //���ý����������ˮƽ
//          UCB->RecvRingTrigLevel = data1;
//          break;
        case CN_UART_SEND_SOFT_LEVEL:   //���÷����������ˮƽ
            UGCB->SendRingTrigLevel = data1;
            break;
        case CN_DRV_CTRL_SET_LEVEL:
            if((u32*)data1 != NULL)
            {
                if(*(u32*)data1 != 0)
                {
                    result = Ring_Capacity(&UGCB->RecvRingBuf);
                    result = (result > *(u32*)data1)?(*(u32*)data1):result;
//                    UCB->MplReadTrigLevel = result;
                }
//                else
//                {
//                    *(u32*)data1 = UCB->MplReadTrigLevel;
//                }
            }
            if((u32*)data2 != NULL)
            {
                if(*(u32*)data2 != 0)
                {
                    result = Ring_Capacity(&UGCB->SendRingBuf);
                    result = (result > *(u32*)data2)?(*(u32*)data2):result;
                    UGCB->MplWriteTrigLevel = result;
                }
                else
                {
                    *(u32*)data2 = UGCB->MplWriteTrigLevel;
                }
            }
            break;
        default: break;
    }
    return result;
}
//----����Poll�豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���,��Ӧ�ó�����ã�UART��ע��Ϊ�豸�������豸��������
//      Driver_CtrlDevice��ʵ�ʾ��ǵ����˸ú������������ù���:
//      Driver_CtrlDevice() ---> Dev->dCtrl() ---> UART_Ctrl()
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
ptu32_t UART_Poll_Ctrl(struct UartCB *UCB,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    ptu32_t result = 0;
    struct UartPollCB *UPCB ;

    UPCB = Container(UCB, struct UartPollCB, UcbBase);
    if(UPCB == NULL)
        return 0;
    switch(cmd)
    {
        case CN_UART_SET_BAUD:          //����Baud
            if(UPCB->UcbBase.Baud !=data1)
            {
                UPCB->UcbBase.Baud = data1;
                UPCB->UcbBase.UartCtrl(UPCB->UcbBase.UartPortTag,CN_UART_SET_BAUD,data1,data2);
            }
            break;
        default:
            result =  UPCB->UcbBase.UartCtrl(UCB->UartPortTag,cmd,data1,data2);
        break;
    }
    return result;
}

//----UART��·������Ӻ���------------------------------------------------------
//���ܣ���UART��ӵ���·���ü���tagUartCB���ƿ��г�ԱpMultiplexCB�����øú���ʱ��
//      δָ���κ�tagMultiplexSetsCB������踳ֵΪ0
//������PrivateTag,����ģ���˽�б�ǩ���˴�ΪUCB���ƿ�
//      MultiplexSets,��·���ü�
//      DevAlias,��Multiplex�Ĵ��ڵ��豸����
//      SensingBit,��������λ��־��31��bit����Ϊ1��ʾ����������bit��־����
//              bit31��ʾ�������ͣ�CN_SENSINGBIT_AND������CN_SENSINGBIT_OR
//���أ�true,��ӳɹ���false,���ʧ��
//-----------------------------------------------------------------------------
bool_t UART_MultiplexAdd(struct UartCB *UCB,
                         struct MultiplexSetsCB *MultiplexSets,
                         struct DjyDevice *Dev,
                         u32 SensingBit)
{
    bool_t result=false;
    struct UartGeneralCB *UGCB;
    UGCB = Container(UCB, struct UartGeneralCB, UcbBase);
    if(UGCB == NULL)
        return result;

    result = Multiplex_AddObject(MultiplexSets,
                            &UGCB->UcbBase.pMultiplexUart,
                            UGCB->UcbBase.MplUartStatus,
                            (ptu32_t)Dev,SensingBit);
//    UCB->MplReadTrigLevel   = 1;
    UGCB->MplWriteTrigLevel  = 1;
    return result;
}
//----Poll UART��·������Ӻ���------------------------------------------------------
//���ܣ���UART��ӵ���·���ü���tagUartCB���ƿ��г�ԱpMultiplexCB�����øú���ʱ��
//      δָ���κ�tagMultiplexSetsCB������踳ֵΪ0
//������PrivateTag,����ģ���˽�б�ǩ���˴�ΪUCB���ƿ�
//      MultiplexSets,��·���ü�
//      DevAlias,��Multiplex�Ĵ��ڵ��豸����
//      SensingBit,��������λ��־��31��bit����Ϊ1��ʾ����������bit��־����
//              bit31��ʾ�������ͣ�CN_SENSINGBIT_AND������CN_SENSINGBIT_OR
//���أ�true,��ӳɹ���false,���ʧ��
//-----------------------------------------------------------------------------
bool_t UART_Poll_MultiplexAdd(struct UartCB *UCB,
                         struct MultiplexSetsCB *MultiplexSets,
                         struct DjyDevice *Dev,
                         u32 SensingBit)
{

    bool_t result=false;
    struct UartPollCB *UPCB;
    UPCB = Container(UCB, struct UartPollCB, UcbBase);
    if(UPCB == NULL)
        return false;

    result = Multiplex_AddObject(MultiplexSets,
                            &UPCB->UcbBase.pMultiplexUart,
                            UPCB->UcbBase.MplUartStatus,
                            (ptu32_t)Dev,SensingBit);
    return result;
}

//----�����豸�˿�д����-------------------------------------------------------
//����: �Ӷ˿�дUART,����Pollģʽ�£����ջ�������driver�����ʱ�����ֻ���𵽶�
//      ·����֪ͨ���á�
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      len��������(bytes)
//����: ʵ��д�뻷�λ��������ַ���
//-----------------------------------------------------------------------------
ptu32_t UART_Poll_PortWrite(struct UartCB *UCB,u32 len)
{
    struct UartPollCB *UPCB;
    UPCB = Container(UCB, struct UartPollCB, UcbBase);
    if(NULL == UPCB)
        return 0;

    if(UPCB->RecvLen == 0)
    {
        UPCB->UcbBase.MplUartStatus |= CN_MULTIPLEX_SENSINGBIT_READ;
        Multiplex_Set(UPCB->UcbBase.pMultiplexUart,UPCB->UcbBase.MplUartStatus);
    }
    UPCB->RecvLen += len;
    return len;
}

//----�����豸�˿ڶ�����---------------------------------------------------------
//����: ֱ�ӴӶ˿ڶ�ȡ���ݲ��������λ�����
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      dst_buf,��������
//      len,���볤��,
//����: ʵ�ʶ�������
//------------------------------------------------------------------------------

static ptu32_t UART_Poll_AppRead(struct UartCB *UCB,u8* dst_buf,u32 len,
                    u32 offset,u32 timeout)
{
    uint32_t RcvLen ;
    struct UartPollCB *UPCB ;
    bool_t ErrorFlag = false;
    UPCB = Container(UCB, struct UartPollCB, UcbBase);
    if((UPCB == NULL))
        return 0;

//  if(Lock_MutexPend(UCB->ReadMutex,timeout)==false)
//      return completed;
//   Lock_SempPend(UPCB->UcbBase.RecvRingBufSemp,timeout);
    Int_CutTrunk();
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
    UPCB->UcbBase.UartCtrl(UPCB->UcbBase.UartPortTag,CN_UART_RECV_BUF,(u32)dst_buf,RcvLen);

    UPCB->UcbBase.MplUartStatus &= (~CN_MULTIPLEX_SENSINGBIT_READ);
    Multiplex_Set(UPCB->UcbBase.pMultiplexUart,UPCB->UcbBase.MplUartStatus);
    UPCB->RecvLen = 0;

    Int_ContactTrunk( );
    if(ErrorFlag)
        UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
//  Lock_MutexPost(UCB->ReadMutex);
    return RcvLen;
}

//----�����豸�˿�д����-------------------------------------------------------
//����:  д���ߵ��˿ڡ����������λ�����ֱ��д
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//      buf������Դָ�롣
//      len��������(bytes)
//����: ʵ��д����ַ���
//-----------------------------------------------------------------------------
static ptu32_t UART_Poll_AppWrite(struct UartCB *UCB,u8* src_buf,u32 len,
                    u32 offset,bool_t block_option,u32 timeout)
{
    u32 completed;
    struct UartPollCB *UPCB;
    UPCB = Container(UCB, struct UartPollCB, UcbBase);
    if(UPCB == NULL)
        return 0;

    completed = UPCB->UcbBase.UartCtrl(UPCB->UcbBase.UartPortTag,CN_UART_SEND_BUF,(u32)src_buf,len);

    if(block_option == CN_BLOCK_COMPLETE)
    {
        Lock_SempPend(UPCB->UcbBase.BlockingSemp,timeout);
    }
//  Lock_MutexPost(UCB->WriteMutex);
    return completed;
}

//----�����豸�˿ڶ�����---------------------------------------------------------
//����: �Ӷ˿ڶ�ȡUART�豸,�ɵײ���������,���������������ɷ���ʱ�ŵ��ñ�������
//      ����������������ȡAPP���͵����ݣ�ֻ�������ö�·���ü�״̬�Լ���������
//      ��֪ͨ���á�
//����: UCB,�������Ĵ���tagUartCB�ṹ��ָ��.
//����: ʵ�ʶ�������
//------------------------------------------------------------------------------
ptu32_t UART_Poll_PortRead(struct UartCB *UCB)
{
    uint32_t result=0;
    struct UartPollCB *UPCB;
    UPCB = Container(UCB, struct UartPollCB, UcbBase);
    if(NULL == UPCB)
        return result;
    UPCB->UcbBase.MplUartStatus |= CN_MULTIPLEX_SENSINGBIT_WRITE;
    Multiplex_Set(UPCB->UcbBase.pMultiplexUart,UPCB->UcbBase.MplUartStatus);
    Lock_SempPost(UPCB->UcbBase.BlockingSemp);
    return 0;

}
//----���UART�豸-------------------------------------------------------------
//���ܣ���Ӵ����豸��ϵͳ�豸����
//      a����ʼ�����λ�����
//      b���������λ������ź������豸������
//      c����������ӵ��豸����ģ�飬����ʼ���豸����ָ��
//������UCB�����ڿ��ƿ�
//      Param,������ʼ��UART������������������鿴tagUartParam�ṹ��
//���أ����ڿ��ƿ�ָ�룬NULLʧ��
//-----------------------------------------------------------------------------
struct UartCB * __UART_InstallGeneral(struct UartParam *Param)
{
    struct DjyDevice * uart_dev;
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

    //������������ʹ�õ��ź���
    UGCB->UcbBase.BlockingSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart blocking");
    if(UGCB->UcbBase.BlockingSemp == NULL)
        goto exit_from_blocking_semp;
    //�������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    //���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
    UGCB->SendRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart tx buf");
    if(UGCB->SendRingBufSemp == NULL)
        goto exit_from_send_buf_semp;
    UGCB->RecvRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart rx buf");
    if(UGCB->RecvRingBufSemp == NULL)
        goto exit_from_recv_buf_semp;
    //Ϊ�豸�������������ṩ�豸�Ļ������
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
    UGCB->MplWriteTrigLevel  = Param->TxRingBufLen + 1;
    UGCB->UcbBase.Baud               = Param->Baud;
    UGCB->UcbBase.Name               = Param->Name;
    UGCB->UcbBase.UartPortTag        = Param->UartPortTag;
    UGCB->StartSend                  = Param->StartSend;
    UGCB->UcbBase.UartCtrl           = Param->UartCtrl;
    UGCB->UcbBase.pMultiplexUart     = NULL;                     //��ʼ��ʱΪNULL
    UGCB->UcbBase.MplUartStatus      = CN_MULTIPLEX_SENSINGBIT_WRITE;//��ʼʱ��д���ɶ�
    Ring_Init(&UGCB->SendRingBuf,pTxRingBuf,Param->TxRingBufLen);
    Ring_Init(&UGCB->RecvRingBuf,pRxRingBuf,Param->RxRingBufLen);

    uart_dev = Driver_DeviceCreate(NULL,(char*)Param->Name,uart_mutexR,uart_mutexT,
                               (fntDevWrite) UART_AppWrite,
                               (fntDevRead ) UART_AppRead,
                               (fntDevCtrl ) UART_Ctrl,
                               (fntDevMultiplexAdd)UART_MultiplexAdd,
                               (ptu32_t)UGCB
                               );
    if(uart_dev == NULL)
        goto exit_from_add_device;

    printk("%s Install General Mode Succeeded!\r\n",Param->Name);
    return (struct UartCB *)UGCB;
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
    Lock_SempDelete(UGCB->UcbBase.BlockingSemp);
exit_from_blocking_semp:
    free(pTxRingBuf);
exit_from_tx_ring_buf:
    free(pRxRingBuf);
exit_from_rx_ring_buf:
    free(UGCB);
exit_from_ucb:
    return NULL;
}

struct UartCB * __UART_InstallPoll(struct UartParam *Param)
{
    struct DjyDevice * uart_dev;
    struct UartPollCB *UPCB;
    struct MutexLCB *uart_mutexR,*uart_mutexT;

    UPCB = (struct UartPollCB *)M_Malloc(sizeof(struct UartPollCB),0);
    if(UPCB == NULL)
        goto exit_from_ucb;
    memset(UPCB,0x00,sizeof(struct UartPollCB));

    //������������ʹ�õ��ź���
    UPCB->UcbBase.BlockingSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart blocking");
    if(UPCB->UcbBase.BlockingSemp == NULL)
        goto exit_from_blocking_semp;
    //Ϊ�豸�������������ṩ�豸�Ļ������
    uart_mutexR = Lock_MutexCreate("uart receive mutex");
    if(uart_mutexR == NULL)
        goto exit_from_mutexR;
    uart_mutexT = Lock_MutexCreate("uart send mutex");
    if(uart_mutexT == NULL)
        goto exit_from_mutexT;
    UPCB->UcbBase.Baud               = Param->Baud;
    UPCB->UcbBase.Name               = Param->Name;
    UPCB->UcbBase.UartPortTag        = Param->UartPortTag;
    UPCB->UcbBase.UartCtrl           = Param->UartCtrl;
    UPCB->UcbBase.pMultiplexUart     = NULL;                     //��ʼ��ʱΪNULL
    UPCB->UcbBase.MplUartStatus      = CN_MULTIPLEX_SENSINGBIT_WRITE;//��ʼʱ��д���ɶ�

    uart_dev = Driver_DeviceCreate(NULL,(char*)Param->Name,uart_mutexR,uart_mutexT,
                                   (fntDevWrite) UART_Poll_AppWrite,
                                   (fntDevRead ) UART_Poll_AppRead,
                                   (fntDevCtrl ) UART_Poll_Ctrl,
                                   (fntDevMultiplexAdd)UART_Poll_MultiplexAdd,
                                   (ptu32_t)UPCB
                                   );
    if(uart_dev == NULL)
        goto exit_from_add_device;

    printk("%s Install Poll Mode Succeeded!\r\n",Param->Name);

    return (struct UartCB*)UPCB;

exit_from_add_device:
    Lock_MutexDelete(uart_mutexT);
exit_from_mutexT:
    Lock_MutexDelete(uart_mutexR);
exit_from_mutexR:
    Lock_SempDelete(UPCB->UcbBase.BlockingSemp);
exit_from_blocking_semp:
    free(UPCB);
exit_from_ucb:
    return NULL;
}

//----���UART�豸-------------------------------------------------------------
//���ܣ���Ӵ����豸��ϵͳ�豸����
//      a����ʼ�����λ�����
//      b���������λ������ź������豸������
//      c����������ӵ��豸����ģ�飬����ʼ���豸����ָ��
//������UCB�����ڿ��ƿ�
//      Param,������ʼ��UART������������������鿴tagUartParam�ṹ��
//���أ����ڿ��ƿ�ָ�룬NULLʧ��
//-----------------------------------------------------------------------------
struct UartCB * UART_InstallPort(struct UartParam *Param)
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

