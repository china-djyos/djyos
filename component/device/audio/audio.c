#if 0
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
//component name:"audio device file"//audio�豸�ļ�ϵͳ
//parent:"device file system"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_AUDIO_DEVICE_FILE == false )
//#warning  " AUDIO_device_file  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_AUDIO_DEVICE_FILE    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


//�������ƿ����ݽṹ
struct AudioCB
{
    struct Object *Host;
    u32 Baud;                           // ����������
    ptu32_t AudioCardTag;               //������ǩ������������������
    UartStartSend StartSend;
    UartControl UartCtrl;
    struct MutexLCB *ReadMutex;        //������,�����豸��ռʽ������
    struct MutexLCB *WriteMutex;       //������,�����豸��ռʽд����
    struct RingBuf SendRingBuf;                //���η��ͻ�����.
    struct RingBuf RecvRingBuf;                //���ν��ջ�����.
    struct SemaphoreLCB *SendRingBufSemp;      //���ͻ������ź���
    struct SemaphoreLCB *RecvRingBufSemp;      //���ջ������ź���
    u32 SendRingTrigLevel;      //����Ring���ʹ���ˮƽ�������ͻ��λ�����������
                                //��SendRingBufSemp������ǰ�̣߳�ֱ�����ͻ���
                                //������������SendRingTrigLevelʱ�ͷ�֮��
};

s32 AUDIO_Open(struct objhandle *hdl, u32 Mode,u32 timeout);
s32 AUDIO_AppWrite(struct objhandle *hdl,u8* src_buf,u32 len,u32 offset, u32 timeout);
s32 AUDIO_Ctrl(struct objhandle *hdl,u32 cmd,va_list *arg0);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----�����豸open����---------------------------------------------------------
//���ܣ��������豸ʱ�Ļص�������ʹ�ļ�������ָ�����������Ӧ���������ƿ顣
//      �����豸�ļ����ǰ�װ����ʱ�����ģ������������𴴽��ļ���
// ������Fp���������������ļ�ָ��
//      Mode����ģʽ��O_RDONLY��O_WRONLY��O_RDWR
//      timeout����ʱʱ�䡣
// ���أ�0=success��-1=error
// ��ע��O_RDONLY��O_WRONLY��O_RDWR �����POSIX��2016�������¶���
//-----------------------------------------------------------------------------
s32 AUDIO_Open(struct objhandle *hdl, u32 Mode, u32 timeout)
{
    struct AudioCB *ACB;

    if(hdl == NULL)
        return (-1);

    ACB = (struct AudioCB *)Device_GetDrvTag(Handle2fd(hdl));
    Handle_SetContext(hdl, (ptu32_t)ACB);  //Fd������ָ���������ƿ�struct AudioCB
    if( ! Ring_IsEmpty(&ACB->RecvRingBuf))
    {
        //�ļ���δ�����κζ�·���ü���ֱ�ӳ�ʼ������
        Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ);
    }

    if( ! Ring_IsFull(&ACB->SendRingBuf))
    {
        //�ļ���δ�����κζ�·���ü���ֱ�ӳ�ʼ������
        Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_WRITE);
    }
    return 0;
}

//----�����豸APPд����----------------------------------------------------------
//����: дUART�豸,��Ӧ�ó�����ã�UART��ע��Ϊ�豸�������豸��������
//      Driver_WriteDevice�����ջ���ñ��������������ù���:
//      Dev_Write() ---> Dev->Write() ---> AUDIO_AppWrite()
//      �ú���ʵ�ֵĹ���˵������:
//      1.��δ�����ȣ������ֱ�ӷ��ͷ�ʽ��ֱ�ӷ���ǰ���Ƚ����������ݷ������.
//      2.����ʼ���ȣ�������д������AUDIO_UCB�Ļ��η��ͻ�������.
//      3.д�����ݺ������������ݣ��������͵ķ�ʽ��Ϊ�жϻ���ѯ������������.
//      4.����������������ȴ������������ݽ��͵�����ˮƽ���ټ���д������.
//      5.�й�timeout��ÿ�εȴ��ź�����ʱ�䶼��ݼ�.
//      6.����������д�뻺�������ж��Ƿ��������ͣ���Ϊ��������ȴ������ź����ٷ���.
//      7.����ǰ�������������ڵĶ�·���ü���д��Ӧλ���
//����: UCB,������������tagUartCB�ṹ��ָ����ֵ
//      buf������Դָ��.
//      len��������(bytes).
//      offset,ƫ������������ģ���У��˱�����Ч
//      block_option,����ѡ�����Ϊ������ʽ.��block_optionΪtrue��������ȴ�����
//         ���������ݷ�����ɺ󷵻أ������������ͷ�ʽ����block_optionΪfalseʱ������
//         ��ȴ��������ݱ���������ģ��Ļ������к����̷��أ����������ͷ�ʽ�ں���
//         ����ʱ�������������ڷ������ݡ�
//      timeout,��ʱ��(us).
//����: ʵ��д�뻷�λ��������ַ���
//-----------------------------------------------------------------------------
s32 AUDIO_AppWrite(struct objhandle *hdl, u8* src_buf, u32 len, u32 offset, u32 timeout)
{
    struct AudioCB *ACB;
    struct Object *AudioObj;
    u32 Mode;
    u32 completed = 0,written;
    uint8_t *buf;
    u32 base_time,rel_timeout = timeout;

    if(hdl == NULL)
        return 0;

    ACB = (struct AudioCB *)Handle_GetContext(hdl);
    Mode = Handle_GetMode(hdl);
    AudioObj = Handle_GetHostObj(hdl);
    buf = (uint8_t*)src_buf;
    base_time = (u32)DJY_GetSysTime();

    if(Lock_MutexPend(ACB->WriteMutex,timeout)==false)
        return completed;

    while(1)
    {
        //��PENDһ���ź�������ֹ�����Ѿ����ͷŹ������͹����У�������ɻ��ͷ����
        //�ź���������block_optionֵ��ʲô�������ͷš�
        Lock_SempPend(ACB->BlockingSemp,0);
        written = Ring_Write(&ACB->SendRingBuf,
                            (uint8_t*)buf+completed,len-completed);
        ACB->StartSend(ACB->AudioCardTag);

        if(written != len-completed)     //����������û���ͳ�ȫ������
        {
            completed += written;
            if(false == Lock_SempPend(ACB->SendRingBufSemp,rel_timeout))
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
                Lock_SempPend(ACB->BlockingSemp,rel_timeout);
            }
            completed += written;
            break;
        }
    }

    Lock_MutexPost(ACB->WriteMutex);
    //�Զ�·���õĲ����������� Lock_MutexPost(ACB->WriteMutex) ֮ǰ���ã���Ϊ��·����
    //�����ڲ�Ҳ�л������������������������� __Multiplex_Set �����ڲ�����������ӡ��
    if(Ring_IsFull(&ACB->SendRingBuf))
    {
        OBJ_ClrMultiplexEvent(AudioObj, CN_MULTIPLEX_SENSINGBIT_WRITE);
    }
    return completed;
}

#pragma GCC diagnostic pop
//----������������----------------------------------------------------------
//����: ������������������ʱ���øú��������ö�·���ü���Ӧ�Ĵ����ʶλ
//����: UCB,������������tagUartCB�ṹ��ָ��.
//      ErrNo,�����ʶ�ı���λ���ñ���λ�����Ƕ�·����ģ��δ�õ��о�λ����3-30���أ�
//����: 0������1����ȷ
//------------------------------------------------------------------------------
u32 AUDIO_ErrHandle(struct AudioCB *pUGCB, u32 dwErrNo)
{
    if(!pUGCB)
        return (0);

    OBJ_SetMultiplexEvent(pUGCB->Host, dwErrNo);
    return (1);
}

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���,��Ӧ�ó�����ã�UART��ע��Ϊ�豸�������豸��������
//      Dev_Ctrl��ʵ�ʾ��ǵ����˸ú������������ù���:
//      Dev_Ctrl() ---> Dev->dCtrl() ---> AUDIO_Ctrl()
//      �ú���ʵ�ֵĹ��ܸ��������ַ�������˵������:
//      1.����ֹͣ�������ɵײ�����ʵ��
//      2.���ð�˫�����ͻ����״̬���ɵײ�����ʵ��
//      3.��ͣ�ͻָ��������ݣ��ɵײ�����ʵ��
//      4.���ò����ʺ�Ӳ������ˮƽ���ɵײ�����ʵ��
//      5.���ô��󵯳��¼�����
//      ......
//����: UCB,������������tagUartCB�ṹ��ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
//-----------------------------------------------------------------------------
s32 AUDIO_Ctrl(struct objhandle* hdl,u32 cmd, va_list *arg0)
{
    struct AudioCB *ACB;
    ptu32_t result = 0;

    if(hdl == NULL)
        return 0;
    ACB = (struct AudioCB *)Handle_GetContext(hdl);
    switch(cmd)
    {
        case CN_DEV_CTRL_START:
        case CN_DEV_CTRL_STOP:
        case CN_DEV_CTRL_SHUTDOWN:
        case CN_DEV_CTRL_SLEEP:
        case CN_DEV_CTRL_RESUME:
        case CN_AUDIO_COM_SET:
        case CN_AUDIO_RX_PAUSE:           //��ͣ����
        case CN_AUDIO_RX_RESUME:          //�ָ�����
        case CN_AUDIO_RECV_HARD_LEVEL:    //���ý���fifo����ˮƽ
        case CN_AUDIO_SEND_HARD_LEVEL:    //���÷���fifo����ˮƽ
        case CN_AUDIO_EN_RTS:             //ʹ��rts����
        case CN_AUDIO_DIS_RTS:            //��ֹrts����
        case CN_AUDIO_EN_CTS:             //ʹ��cts����
        case CN_AUDIO_DIS_CTS:            //��ֹcts����
        case CN_AUDIO_DMA_USED:           //ʹ��dma����
        case CN_AUDIO_DMA_UNUSED:         //��ֹdma����
        case CN_AUDIO_HALF_DUPLEX_SEND:
        case CN_AUDIO_HALF_DUPLEX_RECV:
        case CN_AUDIO_SET_BAUD:          //����Baud
        {
            ACB->UartCtrl(ACB->AudioCardTag,cmd,arg0);
            break;
        }
        case CN_AUDIO_SEND_SOFT_LEVEL:   //���÷����������ˮƽ
        {
            ACB->SendRingTrigLevel = va_arg(*arg0,u32);
            break;
        }

        default: break;
    }
    return result;
}

//----���UART�豸-------------------------------------------------------------
//���ܣ���������豸��ϵͳ�豸����
//      a����ʼ�����λ�����
//      b���������λ������ź������豸������
//      c����������ӵ��豸����ģ�飬����ʼ���豸����ָ��
// ������UCB���������ƿ�
//      Param,������ʼ��UART������������������鿴tagUartParam�ṹ��
//���أ��������ƿ�ָ�룬NULLʧ��
//-----------------------------------------------------------------------------
struct AudioCB *AUDIO_Install(struct AudioParam *Param)
{
    struct Object * AUDIO_dev;
    struct AudioCB *ACB;
    u8 *pRxRingBuf,*pTxRingBuf;

    ACB = (struct AudioCB *)M_Malloc(sizeof(struct AudioCB),0);
    if(ACB == NULL)
        goto exit_from_ucb;

    memset(ACB,0x00,sizeof(struct AudioCB));
    pRxRingBuf = (u8*)M_Malloc(Param->RxRingBufLen,0);
    if(pRxRingBuf == NULL)
        goto exit_from_rx_ring_buf;

    pTxRingBuf = (u8*)M_Malloc(Param->TxRingBufLen,0);
    if(pTxRingBuf == NULL)
        goto exit_from_tx_ring_buf;

    // ������������ʹ�õ��ź���
    ACB->BlockingSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart blocking");
    if(ACB->BlockingSemp == NULL)
        goto exit_from_blocking_semp;

    // �������������ź�����ʹ��������������Ϊ0ʱ����д���̣߳���ȡ�߳�ʹ��������
    // ���ݽ���trigger_level����ʱ�ͷ��ź�����ʹд���߳̽������
    ACB->SendRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart tx buf");
    if(ACB->SendRingBufSemp == NULL)
        goto exit_from_send_buf_semp;

    ACB->RecvRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart rx buf");
    if(ACB->RecvRingBufSemp == NULL)
        goto exit_from_recv_buf_semp;

    // Ϊ�豸�������������ṩ�豸�Ļ������
//  AUDIO_mutexR = Lock_MutexCreate("uart receive mutex");
//  if(AUDIO_mutexR == NULL)
//      goto exit_from_mutexR;
//
//  AUDIO_mutexT = Lock_MutexCreate("uart send mutex");
//  if(AUDIO_mutexT == NULL)
//      goto exit_from_mutexT;

    ACB->WriteMutex = Lock_MutexCreate("AUDIO_WriteMutex");
    if( NULL == ACB->WriteMutex)
        goto exit_from_mutexWite;
    ACB->ReadMutex  = Lock_MutexCreate("AUDIO_ReadMutex");
    if(NULL == ACB->ReadMutex)
        goto exit_from_mutexRead;

    ACB->SendRingTrigLevel  = (Param->TxRingBufLen)>>2;  //Ĭ�ϻ��崥��ˮƽΪ1/16
//    UCB->RecvRingTrigLevel  = (Param->RxRingBufLen)>>4;
//    UCB->MplReadTrigLevel   = Param->RxRingBufLen + 1;
//  ACB->MplWriteTrigLevel  = Param->TxRingBufLen + 1;
    ACB->Baud               = Param->Baud;
    ACB->AudioCardTag        = Param->AudioCardTag;
    ACB->StartSend          = Param->StartSend;
    ACB->UartCtrl           = Param->UartCtrl;
//  ACB->pMultiplexUart     = NULL;                     //��ʼ��ʱΪNULL
//  ACB->MplUartStatus      = CN_MULTIPLEX_SENSINGBIT_WRITE;//��ʼʱ��д���ɶ�
    Ring_Init(&ACB->SendRingBuf,pTxRingBuf,Param->TxRingBufLen);
    Ring_Init(&ACB->RecvRingBuf,pRxRingBuf,Param->RxRingBufLen);

    AUDIO_dev = Device_Create(Param->Name, //AUDIO_mutexR,AUDIO_mutexT,
                          AUDIO_Open,
                          NULL,
                          AUDIO_AppWrite,
                          NULL,
                          AUDIO_Ctrl,
                          (ptu32_t)ACB
                                   );
    ACB->Host = AUDIO_dev;
    if(AUDIO_dev == NULL)
        goto exit_from_add_device;

    info_printf("device","%s initialized in general mode.\r\n", Param->Name);
    return (ACB);

    //������ִ������ͷŴ�������Դ�������ؿ�ָ��
exit_from_add_device:
    Lock_MutexDelete(ACB->ReadMutex);
exit_from_mutexRead:
    Lock_MutexDelete(ACB->WriteMutex);
exit_from_mutexWite:
//    Lock_MutexDelete(AUDIO_mutexT);
//exit_from_mutexT:
//    Lock_MutexDelete(AUDIO_mutexR);
//exit_from_mutexR:
    Lock_SempDelete(ACB->RecvRingBufSemp);
exit_from_recv_buf_semp:
    Lock_SempDelete(ACB->SendRingBufSemp);
exit_from_send_buf_semp:
    Lock_SempDelete(ACB->BlockingSemp);
exit_from_blocking_semp:
    free(pTxRingBuf);
exit_from_tx_ring_buf:
    free(pRxRingBuf);
exit_from_rx_ring_buf:
    free(ACB);
exit_from_ucb:

    error_printf("device","%s initialized in general mode failed.\r\n", Param->Name);
    return NULL;
}
#endif
