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
// �ļ���     ��iicbus.c
// ģ������: djybusģ���һ���ӽ��IIC���ߣ����ļ���Ҫ����IIC���͵����ӣ����ߺ�����
//           �����Լ���IIC���ߵĲ���������Ӧ��IIC�Ĳ���API������
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 17/06.2014
// =============================================================================

#include "stdint.h"
#include "stdio.h"
#include "object.h"
#include "lock.h"
#include "stdlib.h"
#include "driver.h"
#include "string.h"
#include "djybus.h"
#include "iicbus.h"
#include "djyos.h"

static struct Object s_tIICBusType;

//ICB�ĳ�ԱFLAG��λ���
#define CN_IIC_FLAG_R    (1<<0)         //��д��־λ
#define CN_IIC_FLAG_POLL    (1<<1)      //��ѯ�жϱ�־λ

// =============================================================================
// ���ܣ���IIC�������ͽ��ҽӵ�DjyBus�����
// ������para,��ʵ������
// ���أ����ؽ�������Դ���ָ�룬ʧ��ʱ����NULL
// =============================================================================
struct Object *ModuleInstall_IICBus(ptu32_t Para)
{
    struct Object *BusTypeIIC = NULL;
    BusTypeIIC = DjyBus_BusTypeAdd_s(&s_tIICBusType,"BusTypeIIC");
    if(NULL != BusTypeIIC)
        printf("BUS Type IIC Added Succeeded!\r\n");
    return BusTypeIIC;
}

// =============================================================================
// ���ܣ�����IIC���߽�㵽IIC�������ͽ��,����ʼ��IIC_CB���ƿ�ṹ��
// ������NewIICParam,�������߲���������˵����ϸ�����IIC_Param�ṹ��
// ���أ����ؽ�������Դ���ָ�룬ʧ��ʱ����NULL
// =============================================================================
struct IIC_CB *IIC_BusAdd(struct IIC_Param *NewIICParam)
{
    struct IIC_CB *NewIIC;
    if(NULL == NewIICParam)
        goto exit_from_param;

    //�����ظ�����ͬ����IIC����
    if(NULL != OBJ_SearchChild(&s_tIICBusType,(const char*)(NewIICParam->BusName)))
        goto exit_from_readd;

    NewIIC = (struct IIC_CB *)M_Malloc(sizeof(struct IIC_CB),0);
    if(NewIIC == NULL)
        goto exit_from_malloc;

    //�����߽��ҽӵ��������ͽ����ӽ��
    OBJ_AddChild(&s_tIICBusType,&NewIIC->IIC_BusNode,
                sizeof(struct IIC_CB),RSC_IICBUS,(const char*)(NewIICParam->BusName));
    if(&NewIIC->IIC_BusNode == NULL)
        goto exit_from_add_node;

    //���������ź����ͻ������ź���
    NewIIC->IIC_BusSemp= Lock_SempCreate(1,1,CN_BLOCK_FIFO,"iic bus semp");
    if(NewIIC->IIC_BusSemp== NULL)
        goto exit_from_iic_bus_semp;
    NewIIC->IIC_BufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"iic buf semp");
    if(NewIIC->IIC_BufSemp== NULL)
        goto exit_from_iic_buf_semp;

    //IIC_CB���ƿ��ʼ��
    NewIIC->SpecificFlag        = NewIICParam->SpecificFlag;
    NewIIC->pWriteReadPoll      = NewIICParam->pWriteReadPoll;
    NewIIC->pGenerateWriteStart = NewIICParam->pGenerateWriteStart;
    NewIIC->pGenerateReadStart  = NewIICParam->pGenerateReadStart;
    NewIIC->pGenerateEnd        = NewIICParam->pGenerateEnd;
    NewIIC->pBusCtrl            = NewIICParam->pBusCtrl;

    //��������ʼ��
    NewIIC->IIC_Buf.pBuf     = NewIICParam->IICBuf;
    NewIIC->IIC_Buf.MaxLen   = NewIICParam->IICBufLen;
    NewIIC->IIC_Buf.Offset   = 0;
    //��־��ʼ�������� ��д��־����ѯ�жϷ�ʽ
    NewIIC->Flag =0;

    printf("%s Added Succeeded!\r\n",NewIICParam->BusName);

    return NewIIC;

exit_from_iic_buf_semp:
    Lock_SempDelete(NewIIC->IIC_BusSemp);
exit_from_iic_bus_semp:
    OBJ_Del(&NewIIC->IIC_BusNode);
exit_from_add_node:
    free(NewIIC);
exit_from_malloc:
exit_from_readd:
exit_from_param:
    return NULL;
}

// =============================================================================
// ���ܣ�����IIC���߽�㵽IIC�������ͽ��,����ʼ��IIC_CB���ƿ�ṹ��,IIC���ƿ��ɵ���
//       ���ṩ
// ������NewIICParam,�������߲���������˵����ϸ�����IIC_Param�ṹ��
//       NewIIC,����IIC���ƿ�ָ��
// ���أ����ؽ�������Դ���ָ�룬ʧ��ʱ����NULL
// =============================================================================
struct IIC_CB *IIC_BusAdd_s(struct IIC_CB *NewIIC,struct IIC_Param *NewIICParam)
{
    if((NULL == NewIIC) || (NULL == NewIICParam))
        goto exit_from_param;

    //�����ظ�����ͬ����IIC����
    if(NULL != OBJ_SearchChild(&s_tIICBusType,(const char*)(NewIICParam->BusName)))
        goto exit_from_readd;

    //�����߽��ҽӵ��������ͽ����ӽ��
    OBJ_AddChild(&s_tIICBusType,&NewIIC->IIC_BusNode,
                sizeof(struct IIC_CB),RSC_IICBUS,(const char*)(NewIICParam->BusName));
    if(&NewIIC->IIC_BusNode == NULL)
        goto exit_from_add_node;

    //���������ź����ͻ������ź���
    NewIIC->IIC_BusSemp= Lock_SempCreate(1,1,CN_BLOCK_FIFO,"iic bus semp");
    if(NewIIC->IIC_BusSemp== NULL)
        goto exit_from_iic_bus_semp;
    NewIIC->IIC_BufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"iic buf semp");
    if(NewIIC->IIC_BufSemp== NULL)
        goto exit_from_iic_buf_semp;

    //IIC_CB���ƿ��ʼ��
    NewIIC->SpecificFlag        = NewIICParam->SpecificFlag;
    NewIIC->pWriteReadPoll      = NewIICParam->pWriteReadPoll;
    NewIIC->pGenerateWriteStart = NewIICParam->pGenerateWriteStart;
    NewIIC->pGenerateReadStart  = NewIICParam->pGenerateReadStart;
    NewIIC->pGenerateEnd        = NewIICParam->pGenerateEnd;
    NewIIC->pBusCtrl            = NewIICParam->pBusCtrl;

    //��������ʼ��
    NewIIC->IIC_Buf.pBuf     = NewIICParam->IICBuf;
    NewIIC->IIC_Buf.MaxLen = NewIICParam->IICBufLen;
    NewIIC->IIC_Buf.Offset  = 0;

    //��־��ʼ�������� ��д��־����ѯ�жϷ�ʽ
    NewIIC->Flag =0;

    printf("%s Added Succeeded!\r\n",NewIICParam->BusName);
    return NewIIC;

exit_from_iic_buf_semp:
    Lock_SempDelete(NewIIC->IIC_BusSemp);
exit_from_iic_bus_semp:
    OBJ_Del(&NewIIC->IIC_BusNode);
exit_from_add_node:
exit_from_readd:
exit_from_param:
    return NULL;
}

// =============================================================================
// ���ܣ�ɾ����IIC���������ϵ�IIC��㣬����ɾ������������������㣬��ɾ���᲻�ɹ�,��
//       �ͷ��������ʱmalloc�Ŀ��ƿ��ڴ�
// ������DelIIC,ɾ��IIC���ƿ�ָ��
// ���أ�true,ɾ���ɹ�;false,ɾ��ʧ��
// =============================================================================
bool_t IIC_BusDelete(struct IIC_CB *DelIIC)
{
    bool_t result;
    if(NULL == DelIIC)
        return false;
    if(NULL == OBJ_Del(&DelIIC->IIC_BusNode))
    {
        result = false;
    }
    else
    {
        result = true;
        free(DelIIC);
    }
    return result;
}

// =============================================================================
// ���ܣ�ɾ����IIC���������ϵ�IIC��㣬����ɾ������������������㣬��ɾ���᲻�ɹ�
// ������DelIIC,ɾ��IIC���ƿ�ָ��
// ���أ�true,ɾ���ɹ�;false,ɾ��ʧ��
// =============================================================================
bool_t IIC_BusDelete_s(struct IIC_CB *DelIIC)
{
    bool_t result;
    if(NULL == DelIIC)
        return false;
    if(NULL == OBJ_Del(&DelIIC->IIC_BusNode))
    {
        result = false;
    }
    else
    {
        result = true;
    }
    return result;
}

// =============================================================================
// ���ܣ�����IIC�������ͽ����ӽ�����Ƿ�汻�������Ƶ�����
// ������BusName,���ҵ���������
// ���أ����ҵĿ��ƿ���ָ�룬δ�ҵ�ʱ����NULL
// =============================================================================
struct IIC_CB *IIC_BusFind(const char *BusName)
{
    return (struct IIC_CB*)OBJ_SearchChild(&s_tIICBusType,BusName);
}

// =============================================================================
// ���ܣ���IIC���߽���������������������Ϲҽӵ��������������������߽����ӽ��
// ������BusName,�����������ҽӵ���������
//       DevName,��������
//       DevAddr,������ַ����7λ��Ч��δ������д��־λ����0x50
//       BitOfMaddrInDaddr,�洢����ַ��������ַ�ı���λ��
//       BitOfMaddr,�����洢��ַ����λ��
// ���أ����ƿ���ָ�룬���ʧ��ʱ����NULL
// =============================================================================
struct IIC_Device *IIC_DevAdd(const char *BusName ,const char *DevName, u8 DevAddr,
                                u8 BitOfMaddrInDaddr, u8 BitOfMaddr)
{
    struct IIC_CB      *IIC;
    struct IIC_Device *NewDev;

    //��ѯ�Ƿ�����ߴ���
    IIC = IIC_BusFind(BusName);
    if(NULL == IIC)
        return NULL;

    //���⽨��ͬ����IIC����
    if(NULL != OBJ_SearchChild(&IIC->IIC_BusNode,DevName))
        return NULL;

    //Ϊ�µ�������㶯̬�����ڴ�
    NewDev = (struct IIC_Device *)M_Malloc(sizeof(struct IIC_Device),0);
    if(NULL == NewDev)
        return NULL;

    //Ϊ�½���ʼ��
    NewDev->DevAddr              = DevAddr;
    NewDev->BitOfMemAddrInDevAddr = BitOfMaddrInDaddr;
    NewDev->BitOfMemAddr          = BitOfMaddr;
    OBJ_AddChild(&IIC->IIC_BusNode,&NewDev->DevNode,
                sizeof(struct IIC_Device),RSC_IIC_DEVICE,DevName);
    if(NULL == &NewDev->DevNode)
    {
        free(NewDev);
        return NULL;
    }

    printf("IIC Device %s Added Succeeded!\r\n",DevName);
    return NewDev;
}

// =============================================================================
// ���ܣ����������ṩ�������������ҽӵ�IIC���߽����,�������ṩIIC_Device���ƿ�
// ������BusName,�����������ҽӵ���������
//       DevName,������������
//       NewDev,��������ָ��
// ���أ����ƿ���ָ�룬���ʧ��ʱ����NULL
// =============================================================================
struct IIC_Device *IIC_DevAdd_s(struct IIC_Device *NewDev,const char *BusName,
                                   const char *DevName, u8 DevAddr,
                                   u8 BitOfMaddrInDaddr, u8 BitOfMaddr)
{
    struct IIC_CB *IIC;
    //��ѯ�Ƿ�����ߴ���
    IIC = IIC_BusFind(BusName);
    if((NULL == IIC) || (NULL == NewDev))
        return NULL;

    //���⽨��ͬ����IIC����
    if(NULL != OBJ_SearchChild(&IIC->IIC_BusNode,DevName))
        return NULL;

    NewDev->DevAddr              = DevAddr;
    NewDev->BitOfMemAddrInDevAddr = BitOfMaddrInDaddr;
    NewDev->BitOfMemAddr          = BitOfMaddr;
    OBJ_AddChild(&IIC->IIC_BusNode,&NewDev->DevNode,
                sizeof(struct IIC_Device),RSC_IIC_DEVICE,DevName);

    printf("IIC Device %s Added Succeeded!\r\n",DevName);
    return (struct IIC_Device *)&NewDev->DevNode;
}

// =============================================================================
// ���ܣ�ɾ��IIC�����ϵ��������������ӽ����ɾ��
// ������DelDev,ɾ��������ָ��
// ���أ�true,ɾ���ɹ�;false,ɾ��ʧ��
// =============================================================================
bool_t IIC_DevDelete(struct IIC_Device *DelDev)
{
    bool_t result;
    if(NULL == DelDev)
        return false;

    if(NULL == OBJ_Del(&DelDev->DevNode))
    {
        result = false;
    }
    else
    {
        free(DelDev);
        result = true;
    }
    return result;
}

// =============================================================================
// ���ܣ�ɾ��IIC�����ϵ��������������ӽ����ɾ��
// ������DelDev,ɾ��������ָ��
// ���أ�true,ɾ���ɹ�;false,ɾ��ʧ��
// =============================================================================
bool_t IIC_DevDelete_s(struct IIC_Device *DelDev)
{
    bool_t result;
    if(NULL == DelDev)
        return false;

    if(NULL == OBJ_Del(&DelDev->DevNode))
    {
        result = false;
    }
    else
    {
        result = true;
    }
    return result;
}

// =============================================================================
// ���ܣ�����������㣬�ý���Ȼ�ǹҽ�����Ӧ��IIC���߽����
// ������DevName,��������
// ���أ���㸽���ṹ��ָ��
// =============================================================================
struct IIC_Device *IIC_DevFind(const char *DevName)
{
    //ͨ��IIC���ͽ�㣬��������������
    return (struct IIC_Device*)OBJ_SearchScion(&s_tIICBusType,DevName);
}

// =============================================================================
// ���ܣ��ϲ����дAPI�������ú�������Dev��������������߽�㣬�������߻ص��������
//       ��������ʱ�򣬲��Ҹ�������ѡ������������ȴ�������ɻ���ֱ�ӷ��ء�timeout
//       ��������ָ��������ȷ����ʱ������ɣ���ʱ��ǿ�Ʒ��أ���ֹͣʱ���ͷ��ź���
// ������DelDev,�����ṹ��ָ��
//       addr,���͵�ַ����ָ�洢��ַ
//       buf,�������ݻ�����ָ��
//       len,�������ݳ��ȣ��ֽڵ�λ
//       block_option,����ѡ��,true��ʾ�������ͣ�falseΪ����������
//       timeout,��ʱʱ�䣬us
// ���أ����͵��ֽ���
// =============================================================================
s32  IIC_Write(struct IIC_Device *Dev, u32 addr,u8 *buf,u32 len,
                bool_t block_option,u32 timeout)
{
    struct IIC_CB *IIC;
    u8 DevAddr,MemAddrLen;
    u32 written=0;
    s32 result = 0;
    u32 base_time = 0,rel_timeout = timeout;

    if(NULL == Dev)
        return CN_IIC_EXIT_PARAM_ERR;

    base_time = (u32)DjyGetSysTime();
    //���Ҹ�����������������
    IIC = (struct IIC_CB *)OBJ_Parent(&Dev->DevNode);
    if(NULL == IIC)
        return CN_IIC_EXIT_PARAM_ERR;
    //��Ҫ�ȴ����߿���
    if(false == Lock_SempPend(IIC->IIC_BusSemp,timeout))
        return CN_IIC_EXIT_TIMEOUT;

    rel_timeout = timeout - ((u32)DjyGetSysTime() - base_time);
    DevAddr = Dev->DevAddr |
            ((u8)(addr >> (Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr))
            & ~(0xFF<<Dev->BitOfMemAddrInDevAddr));
    //���㷢�͵�ַ�ֽ���
    //change by lst
//    MemAddrLen = (Dev->BitOfMemAddr)/8 +
//            ((Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr)%8 ? 1:0);
    MemAddrLen = (Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr +7)/8;
    IIC->Flag &= ~CN_IIC_FLAG_R;
    //�ж��Ƿ�����ѯ��ʽ
    if((Djy_QuerySch() == false) || (IIC->pGenerateWriteStart == NULL)
            || (IIC->Flag & CN_IIC_FLAG_POLL))
    {
        if(IIC->pWriteReadPoll != NULL)
        {
            result = IIC->pWriteReadPoll(IIC->SpecificFlag,DevAddr,addr,
                                MemAddrLen,buf,len,CN_IIC_WRITE_FLAG);
            if(result)
                result = len;
            else
                result = 0;
        }
        else
        {
            result = CN_IIC_EXIT_FUN_NULL;
        }
        Lock_SempPost(IIC->IIC_BusSemp);
        return result;
    }

    IIC->IIC_Buf.Offset = 0;                            //����ǰ����ջ�����
    IIC->Counter = len;

    //��������ֽ���С�ڵ��ڻ�������С����ֱ����д��������
    //�����ǣ����ȷ��͵������ṩ�Ļ�������ֱ��ʣ���ֽ����ܹ���䵽������
    if(len <= IIC->IIC_Buf.MaxLen)
    {
        memcpy(IIC->IIC_Buf.pBuf,buf,len);                  //д������
        written = len;
    }
    else
    {
        IIC->pBuf = buf;                                        //ָ���û�������
    }

    //��������ʱ��Ļص�����
    if(true == IIC->pGenerateWriteStart(IIC->SpecificFlag,DevAddr,
                                        addr,MemAddrLen,len,IIC->IIC_BusSemp))
    {
        rel_timeout = (u32)DjyGetSysTime();
        if(rel_timeout - base_time < timeout)
            rel_timeout = timeout - (rel_timeout - base_time);
        else
        {
            IIC->pGenerateEnd(IIC->SpecificFlag);
            return CN_IIC_EXIT_TIMEOUT;
        }

        if(true == block_option)                    //����������ȴ�bus�ź���
        {
            if(Lock_SempPend(IIC->IIC_BusSemp,rel_timeout))//�ȴ��жϺ����ͷ��ź���
            {
                //��������ʱ�����ɱ������ͷ������ź���
                Lock_SempPost(IIC->IIC_BusSemp);
                result = len;
            }else                                    //������ʱ����
            {
                IIC->pGenerateEnd(IIC->SpecificFlag);//��ʱ�˳�������ʱ��
                Lock_SempPost(IIC->IIC_BusSemp);         //�ͷ������ź���
                result = len - IIC->Counter;
            }
        }else                                       //����������
        {
            if(written != len)                      //û���������
            {
                //IIC_PortRead��buf��ʣ�������������׻�����ʱ���ͷ��ź���
                if(false == Lock_SempPend(IIC->IIC_BufSemp,rel_timeout))
                {
                    IIC->pGenerateEnd(IIC->SpecificFlag);
                    return CN_IIC_EXIT_TIMEOUT;
                }else
                {
                    result = len - IIC->Counter;
                }
            }else                                   //��д�˼��׻�����
            {
                result = len;
            }
        }
    }else
    {
        IIC->pGenerateEnd(IIC->SpecificFlag);       //��ʱ�˳�������ʱ��
        Lock_SempPost(IIC->IIC_BusSemp);                //�ͷ������ź���
        return CN_IIC_EXIT_UNKNOW_ERR;
    }
    //��������£����������ͻ��ɵײ������ͷ������ź���
    return result;
}

// =============================================================================
// ���ܣ����������ú������ȸ���Dev������������ߣ�Ȼ��������߶��ص�������������ȡ��
//       ����������ʱ�򣬲������ȴ�������ɣ�����ý�����ɵ��ź���ʱ���ͷ����߿���
//       �ź�������ʱ����timeout�����û�ָ����ȷ��ʱ����������߲�������ʱ��ǿ�Ʒ���
// ������Dev,�����ṹ��ָ��
//       addr,���յ�ַ����ָ�洢��ַ
//       buf,�������ݻ�����ָ��
//       len,�������ݳ��ȣ��ֽڵ�λ
//       timeout,��ʱʱ�䣬us
// ���أ����յ��ֽ���
// =============================================================================
s32  IIC_Read(struct IIC_Device *Dev,u32 addr,u8 *buf,u32 len,u32 timeout)
{
    struct IIC_CB *IIC;
    u8 DevAddr,MemAddrLen;
    s32 result = len;
    u32 base_time = 0,rel_timeout = timeout;

    if(NULL == Dev)
        return CN_IIC_EXIT_PARAM_ERR;

    base_time = (u32)DjyGetSysTime();
    //���Ҹ�����������������
    IIC = (struct IIC_CB *)OBJ_Parent(&Dev->DevNode);
    if(NULL == IIC)
        return CN_IIC_EXIT_PARAM_ERR;

    if(false == Lock_SempPend(IIC->IIC_BusSemp,timeout))        //��Ҫ�ȴ����߿���
        return CN_IIC_EXIT_TIMEOUT;

    Lock_SempPend(IIC->IIC_BufSemp,0);                          //�൱�����ֵ�ź���

    rel_timeout = timeout - ((u32)DjyGetSysTime() - base_time);
    DevAddr = Dev->DevAddr |
            ((u8)(addr >> (Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr))
            & ~(0xFF<<Dev->BitOfMemAddrInDevAddr));
    //���㷢�͵�ַ�ֽ���
//    MemAddrLen = (Dev->BitOfMemAddr)/8 +
//            ((Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr)%8 ? 1:0);
    MemAddrLen = (Dev->BitOfMemAddr - Dev->BitOfMemAddrInDevAddr +7)/8;

    IIC->Flag |= CN_IIC_FLAG_R;
    //�ж��Ƿ�����ѯ��ʽ
    if((Djy_QuerySch() == false) || (IIC->pGenerateReadStart == NULL)
            || (IIC->Flag & CN_IIC_FLAG_POLL))
    {
        if(IIC->pWriteReadPoll != NULL)
        {
            result = IIC->pWriteReadPoll(IIC->SpecificFlag,DevAddr,addr,
                                MemAddrLen,buf,len,CN_IIC_READ_FLAG);
            if(result)
                result = len;
            else
                result = 0;
        }
        else
        {
            result = CN_IIC_EXIT_FUN_NULL;
        }
        Lock_SempPost(IIC->IIC_BusSemp);
        return result;
    }
    IIC->Counter = len;
    IIC->pBuf    = buf;

    //����������ʱ��
    if(IIC->pGenerateReadStart(IIC->SpecificFlag,DevAddr,addr,MemAddrLen,
                                len,IIC->IIC_BufSemp))
    {
        rel_timeout = (u32)DjyGetSysTime();
        if(rel_timeout - base_time < timeout)           //δ��ʱ
        {
            rel_timeout = timeout - (rel_timeout - base_time);

            if(!Lock_SempPend(IIC->IIC_BufSemp,rel_timeout))//�����ȴ��������
            {
                result = CN_IIC_EXIT_TIMEOUT;
            }
            //��ʱ��ȷ���յ���������
        }
        else                                            //��ʱ�˳�
        {
            result = CN_IIC_EXIT_TIMEOUT;
        }
    }

    if(result == CN_IIC_EXIT_TIMEOUT)
    {
        IIC->pGenerateEnd(IIC->SpecificFlag);   //��ʱ�˳���������
    }
    Lock_SempPost(IIC->IIC_BusSemp);                //�ͷ������ź���
    return len-IIC->Counter;                    //���ض���������
}

// =============================================================================
// ���ܣ������ͻ��������ݣ��������������ã�����������ȡ�����ݺ�����д��Ĵ�������
// ������IIC,IIC���ƿ�ָ��
//       buf,�����ݻ�����ָ��
//       len,�����ݳ��ȣ��ֽڵ�λ
// ���أ��ֽ���
// =============================================================================
s32 IIC_PortRead( struct IIC_CB *IIC,u8 *buf,u32 len)
{
    u32 Result = 0,RingLen,CpyLen;
    u8 *pbuf;

    //�ݴ�������Ƕ����ݹ��̣��򲻸ý��뵽�˴�
    if(IIC->Flag & CN_IIC_FLAG_R)
        return 0;

    if((len > 0) && (IIC->Counter > 0))
    {
        CpyLen = IIC->Counter >= len ?len:IIC->Counter;
        RingLen = IIC->IIC_Buf.MaxLen;
        //�ж��Ƿ�Ӽ��׻������ж�����
        if(IIC->Counter <= RingLen)                     //�ӻ������ж�����
        {
            pbuf = &IIC->IIC_Buf.pBuf[IIC->IIC_Buf.Offset];
            IIC->IIC_Buf.Offset += CpyLen;
            memcpy(buf,pbuf,CpyLen);
        }
        else                                            //��pbuf�ж�����
        {
            memcpy(buf,IIC->pBuf,CpyLen);
            IIC->pBuf += CpyLen;

            if(IIC->Counter - CpyLen <= RingLen)        //��pbufʣ������д�뻺��
            {
                memcpy(IIC->IIC_Buf.pBuf,IIC->pBuf,IIC->Counter-CpyLen);
                Lock_SempPost(IIC->IIC_BufSemp);
            }
        }
        IIC->Counter -= CpyLen;
        Result = CpyLen;
    }

    return Result;
}

// =============================================================================
// ���ܣ������յ�������д���û��ṩ�Ļ������У�������������ʽ�����ʹ���û��Ļ�����
// ������IIC,IIC���ƿ�ָ��
//       buf,����ָ��
//       len,���ݳ��ȣ��ֽڵ�λ
// ���أ��ֽ���
// =============================================================================
s32 IIC_PortWrite(struct IIC_CB *IIC,u8 *buf,u32 len)
{
    u32 cpylen = 0;

    //�ݴ��������д���ݹ��̣��򲻸ý��뵽�˴�
    if(!(IIC->Flag & CN_IIC_FLAG_R))
        return 0;
    if((len > 0) && (IIC->Counter > 0))
    {
        cpylen = IIC->Counter >= len ?len:IIC->Counter;
        memcpy(IIC->pBuf,buf,cpylen);
        IIC->pBuf += cpylen;
        IIC->Counter -= cpylen;
    }

    return cpylen;
}

// =============================================================================
// ���ܣ�����cmd�����������data1��Data2������IIC��Ӳ����ص���������ûص�����
//       �ɵײ��������
// ������Dev,�����ṹ��ָ��
//       cmd,����
//       data1,data2,�������ݣ����ݾ�������岻ͬ
// ���أ�-1=����������,������cmd����,����Ҫ�����û���pBusCtrl,���Ǹú�������ֵ
// =============================================================================
s32 IIC_BusCtrl(struct IIC_Device *Dev,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    struct IIC_CB *IIC;
    s32 result = 1;
    if(NULL == Dev)
        return -1;

    IIC = (struct IIC_CB *)OBJ_Parent(&Dev->DevNode);
    if(NULL == IIC)
        return -1;

    switch(cmd)
    {
    case CN_IIC_SET_CLK:
    case CN_IIC_DMA_USED:
    case CN_IIC_DMA_UNUSED:
        result = IIC->pBusCtrl(IIC->SpecificFlag,cmd,data1,data2);
        break;
    case CN_IIC_SET_ERROR_POP_EVENT:
        IIC->ErrorPopEvtt = data1;
        result = 1;
        break;
    case CN_IIC_SET_POLL:
        IIC->Flag |=  CN_IIC_FLAG_POLL;
        break;
    case CN_IIC_SET_INT:
        IIC->Flag &= ~CN_IIC_FLAG_POLL;
        break;
    default:
        break;
    }
    return result;
}

// =============================================================================
// ���ܣ�IIC���󵯳�API����������������ʱ�����øú����������������¼������������
//       �Բ�������ʽ���ݸ��������¼�
// ������IIC,IIC���ƿ�ָ��
//       ErrNo,����������ţ���δ�յ�ACK�ź�CN_IIC_NO_ACK_ERR
// ���أ�ִ�н����CN_EXIT_NO_ERRΪ�޴���
// =============================================================================
s32 IIC_ErrPop(struct IIC_CB *IIC, u32 ErrNo)
{
    if(NULL == IIC)
        return CN_IIC_EXIT_PARAM_ERR;

    //�����������¼�������������Բ�����ʽ����
    Djy_EventPop(IIC->ErrorPopEvtt,NULL,0,ErrNo,0,0);

    return 0;
}

