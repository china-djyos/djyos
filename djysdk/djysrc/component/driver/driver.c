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

//����ģ��:�豸����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�ṩ�豸������
//����˵��:
//�޶���ʷ:
//3. ����:2014-06-14
//   ����:  ������.
//   �°汾�ţ�2.0.0
//   �޸�˵��: ���豸����ģ������ȫ���޸ģ������ṩ���������׽ӿڡ�
//2. ����:2009-03-03
//   ����:  ������.
//   �°汾�ţ�1.0.1
//   �޸�˵��: ������dev_close_left������dev_close_right�����Ĵ��󣬸�bug��
//             ����sniper�ύ
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"

#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "errno.h"
#include "string.h"
#include "atomic.h"
#include "object.h"
#include "pool.h"
#include "systime.h"
#include "multiplex.h"
#include "lock.h"
#include "driver.h"
#include "djyos.h"
#include "core_config.h"

static struct  Object *s_ptDeviceRscTree;

static struct DjyDevice *s_ptMenOfDevice;   //�豸���ƿ��ڴ��
static struct MemCellPool *s_ptDevicePool;  //�豸���ƿ��ڴ��ͷָ��
static struct MutexLCB s_tDevMutex;         //�����豸��Դ���Ĳ������ʰ�ȫ

struct VCommon g_tDevCom =
{
    .Context = NULL,
    .Name = "dev",
};
//-----------------------------------------------------------------------------
//����:
//����:
//����: ��NULL -- �ɹ�; NULL -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
struct Object *__DevObjInit(void)
{
    u16 Size, Type;

    Size = sizeof(g_tDevCom) + strlen(g_tDevCom.Name);
    Type = RSC_RSCNODE;

    return (OBJ_AddChild(__RootObj(), &(g_tDevCom.Obj), Size, Type, g_tDevCom.Name));
}

//----��ʼ���豸����---------------------------------------------------------
//���ܣ�1.����Դ��������������Ϊ"dev"�ĸ����.
//      2.��ʼ���豸���ƿ��ڴ��
//��������
//���أ�1
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Driver(ptu32_t para)
{
    s_ptMenOfDevice = M_Malloc(gc_u32CfgDeviceLimit * sizeof(struct DjyDevice),0x1);
    if(s_ptMenOfDevice == NULL)
        return 0;

    s_ptDeviceRscTree = __DevObjInit();

    //��ʼ���豸���ƿ��ڴ��
    s_ptDevicePool = Mb_CreatePool((void*)s_ptMenOfDevice,
                                    gc_u32CfgDeviceLimit,
                                    sizeof(struct DjyDevice ),
                                    0,0,
                                    "�豸���ƿ��");
    Lock_MutexCreate_s(&s_tDevMutex,"device tree");
    return 1;
}

////----��ȡ�豸ָ��-------------------------------------------------------------
////����: ͨ���豸handle��������豸���豸ָ�롣
////����: handle,�豸���
////����: �豸ָ��
////-----------------------------------------------------------------------------
//struct DjyDevice *__dev_Hdl2Dev(struct DjyDevice * Handle)
//{
//    return (struct DjyDevice *)Handle;
//};
//
////----��ȡ�豸���-------------------------------------------------------------
////����: ͨ���豸ָ������豸���豸handle��
////����: Dev,�豸ָ��
////����: �豸handle
////-----------------------------------------------------------------------------
//struct DjyDevice * __dev_Dev2Hdl(struct DjyDevice * Dev)
//{
//    return (struct DjyDevice *)Dev;
//};

////----��ȡ�豸ָ��-------------------------------------------------------------
////����: ͨ���豸���������豸���豸ָ�롣
////����: DevAlias,�豸����
////����: �豸ָ��
////-----------------------------------------------------------------------------
//struct DjyDevice *__dev_Alias2Dev(u32 DevAlias)
//{
//    return (struct DjyDevice *)(DevAlias-1);
//};

////----��ȡ�豸����-------------------------------------------------------------
////����: ͨ���豸ָ������豸���豸����
////����: Dev,�豸ָ��
////����: �豸handle
////-----------------------------------------------------------------------------
//u32 __dev_Dev2Alias(struct DjyDevice * Dev)
//{
//    return (u32)Dev+1;
//};

////----��ȡ�豸���-----------------------------------------------------------
////����: ͨ���豸���������豸���豸�����
////����: DevAlias,�豸����
////����: �豸���
////---------------------------------------------------------------------------
//struct DjyDevice * __dev_Alias2Hdl(u32 DevAlias)
//{
//    return (struct DjyDevice *)(DevAlias-1);
//};
//
////----��ȡ�豸����-----------------------------------------------------------
////����: ͨ���豸��������豸���豸����
////����: Handle,�豸���
////����: �豸handle
////---------------------------------------------------------------------------
//u32 __dev_Hdl2Alias(struct DjyDevice * Handle)
//{
//    return (u32)Handle+1;
//};

//----����豸-----------------------------------------------------------------
//����: ���豸���뵽�ֵ��豸��β��,
//����: ParentDevice,������豸�ĸ��豸��NULL��ʾ�豸���ĸ����Ϊ���豸���
//      name�����豸�����֣���ָ����ַ����ڴ��������Ǿֲ�������Ҳ�����ǿ�
//      dReadMutex�����豸������ָ��
//      dWriteMutex��д�豸������ָ��
//      WriteFunc��д����ָ��
//      ReadFunc��������ָ��
//      Ctrl�����ƺ���ָ��
//      MultiplexAdd��֧�ֶ�·���õĺ���ָ��
//      tag,��Ӧ�豸������������һ��������ָ����豸�������ݽṹ
//����:�¼�����豸����Դ���ָ��.��Դ�����򷵻�NULL
//���������豸driver���ߵ��ã�������Ӧ�ó���ʹ�ã��ʲ�ʹ��djy_handle_t
//������struct  DjyDevice���Ͳ���
//-----------------------------------------------------------------------------
struct DjyDevice * Driver_DeviceCreate(  struct DjyDevice *ParentDevice,
                                            const char              *name,
                                            struct MutexLCB      *dReadMutex,
                                            struct MutexLCB      *dWriteMutex,
                                            fntDevWrite            WriteFunc ,
                                            fntDevRead             ReadFunc,
                                            fntDevCtrl             Ctrl ,
                                            fntDevMultiplexAdd     MultiplexAdd,
                                            ptu32_t                 tag)
{
    struct DjyDevice *new_device,*result;
    if(name == NULL)
        return NULL;        //�豸����û������
    if(strchr(name,'\\'))   //�����в��ܰ����ַ� \.
        return NULL;
    if(ParentDevice == NULL)
        ParentDevice = (struct DjyDevice *)s_ptDeviceRscTree;
    Lock_MutexPend(&s_tDevMutex,CN_TIMEOUT_FOREVER);
    if(OBJ_SearchChild(&ParentDevice->Node,name) != NULL)
    {
        Djy_SaveLastError(EN_DRV_HOMONYMY);
        printf("�豸����\n\r");
        result = NULL;
    }else
    {
        //�����豸���ƿ�����豸
        new_device = Mb_Malloc(s_ptDevicePool,0);
        if(new_device != NULL)
        {
            OBJ_AddChild(&ParentDevice->Node,&new_device->Node,
                                    sizeof(struct DjyDevice),RSC_DEVICE,name);
            new_device->MutexFlag = 0;
            if(dReadMutex != NULL)
            {
                new_device->dReadMutex = dReadMutex;
            }else
            {
                new_device->dReadMutex = Lock_MutexCreate(name);
                new_device->MutexFlag |= DEV_READ_MUTEX_SYS;
            }
            if(dWriteMutex != NULL)
            {
                new_device->dWriteMutex = dWriteMutex;
            }else
            {
                new_device->dWriteMutex = Lock_MutexCreate(name);
                new_device->MutexFlag |= DEV_WRITE_MUTEX_SYS;
            }
            if( (new_device->dReadMutex != NULL)
                    && (new_device->dWriteMutex != NULL))
            {
                if(WriteFunc != NULL)
                    new_device->dWrite = WriteFunc;
                else
                    new_device->dWrite =(fntDevWrite)NULL_func;
                if(ReadFunc != NULL)
                    new_device->dRead = ReadFunc;
                else
                    new_device->dRead = (fntDevRead)NULL_func;
                if(Ctrl != NULL)
                    new_device->dCtrl = Ctrl;
                else
                    new_device->dCtrl = (fntDevCtrl)NULL_func;
                if(MultiplexAdd != NULL)
                    new_device->dMultiplexAdd = MultiplexAdd;
                else
                    new_device->dMultiplexAdd = (fntDevMultiplexAdd)NULL;
                new_device->PrivateTag = tag;
                result = new_device;
            }
            else
            {
                if(new_device->MutexFlag & DEV_READ_MUTEX_SYS)
                    Lock_MutexDelete(new_device->dReadMutex);
                if(new_device->MutexFlag & DEV_WRITE_MUTEX_SYS)
                    Lock_MutexDelete(new_device->dWriteMutex);
                OBJ_Del(&new_device->Node);
                Mb_Free(s_ptDevicePool,new_device);
                Djy_SaveLastError(EN_MEM_TRIED);
                printf("�ڴ治��\n\r");
                result = NULL;
            }
        }else
        {
            Djy_SaveLastError(EN_MEM_TRIED);
            printf("�ڴ治��\n\r");
            result = NULL;
        }
    }
    Lock_MutexPost(&s_tDevMutex);
    return result;
}

////----�����豸-----------------------------------------------------------------
////����: �����豸��ʹ���豸���ܱ�ɾ��������Ӱ����豸�����������������豸��
////      Driver_OpenDeviceAlias������ȫ���еı�֤������ȷ��Driver_OpenDeviceAlias
////      ǰ���豸���ᱻж�ء�
////����: DevAlias��Ŀ���豸��������Driver_FindDevice����
////����: �豸״̬��true=��������false=δ������
////-----------------------------------------------------------------------------
//bool_t Driver_ProtectDevice(u32 DevAlias)
//{
//    struct DjyDevice *Dev;
//    if(DevAlias == 0)
//        return false;
//    Dev = __dev_Alias2Dev(DevAlias);
//    atom_uadd32(&(Dev->ProtectCounter),1);
//    return (Dev->ProtectCounter > 0);
//}
//
////----��������豸-------------------------------------------------------------
////����: Driver_LockDevice�����������
////����: DevAlias��Ŀ���豸��������Driver_FindDevice����
////����: �豸״̬��false=��������true=δ������
////-----------------------------------------------------------------------------
//bool_t Driver_UnProtectDevice(u32 DevAlias)
//{
//    struct DjyDevice *Dev;
//    if(DevAlias == 0)
//        return false;
//    Dev = __dev_Alias2Dev(DevAlias);
//    atom_usub32(&(Dev->ProtectCounter),1);
//    return (Dev->ProtectCounter == 0);
//}

//----ж���豸------------------------------------------------------------------
//����: ���豸���豸������ɾ��,���ͷ���ռ�õ��ڴ�.���豸�������������:
//      1.���豸û�����豸.
//      2.����ʹ�ø��豸���û���Ϊ0.
//����: handle,���ͷŵ��豸
//����: �ɹ��ͷŷ���true,���򷵻�false
//��Գƺ���dev_add_deviceһ�������������豸driver���ߵ��ã�������Ӧ�ó���
//ʹ�ã��ʲ�ʹ��struct  dev_handle������struct  DjyDevice���Ͳ���
//-----------------------------------------------------------------------------
bool_t Driver_DeleteDevice(struct DjyDevice * device)
{
    struct  Object *node;
    bool_t en_del = true;
    if(device == NULL)
        return false;
    if(device->OpenCounter > 0)
        return false;
    Lock_MutexPend(&s_tDevMutex,CN_TIMEOUT_FOREVER);
    if( Lock_MutexQuery(device->dReadMutex)
            || Lock_MutexQuery(device->dWriteMutex))     //����豸�Ƿ����
    {
        node = &device->Node;
        //�����������driverģ������,ɾ��֮
        if(device->MutexFlag & DEV_READ_MUTEX_SYS)
            Lock_MutexDelete(device->dReadMutex);
        if(device->MutexFlag & DEV_WRITE_MUTEX_SYS)
            Lock_MutexDelete(device->dWriteMutex);
        if(OBJ_Del(node) != NULL)
        {
            Mb_Free(s_ptDevicePool,device);
        }
        else
        {
            en_del = false;
        }
    }
    else
    {
        en_del = false;
    }

    Lock_MutexPost(&s_tDevMutex);
    return en_del;
}

//----�����豸-----------------------------------------------------------------
//����: ��ѯ�豸�������ڷ����豸ָ�룬���������豸���������򷵻�0
//����: name���豸���֣�����·�����������ذ���'dev\'��4���ַ�
//����: �豸�����򷵻��豸ָ�룬�������򷵻�NULL
//-----------------------------------------------------------------------------
struct DjyDevice *Driver_FindDevice(const char * name)
{
    struct DjyDevice     *Dev;
    if( ! Lock_MutexPend(&s_tDevMutex,0))    //���Ǳ����豸���Ļ�����
        return NULL;
    //���豸���������豸
    Dev = (struct DjyDevice *)OBJ_Search(s_ptDeviceRscTree,name);
    Lock_MutexPost(&s_tDevMutex);
    if(Dev != NULL)
//      return __dev_Dev2Alias(Dev);
        return Dev;
    else
        return NULL;
}

//----���Һ���豸-------------------------------------------------------------
//����: ĳ�豸�ĺ���豸�У���ѯ����Ϊscion_name���豸�Ƿ���ڣ������򷵻�����
//      ��ָ�룬�������򿪣��������򷵻�0.
//����: ancestor������ѯ�豸�������豸��
//      scion_name,�豸���ַ���,����·����,
//����: �豸�����򷵻��豸ָ�룬�������򷵻�NULL
//-----------------------------------------------------------------------------
struct DjyDevice *Driver_FindScionDevice(struct DjyDevice * ancestor,const char * scion_name)
{
    struct DjyDevice *Dev_scion;
    if(ancestor == NULL)
        return NULL;
    if( ! Lock_MutexPend(&s_tDevMutex,0))    //���Ǳ����豸���Ļ�����
        return NULL;
    //���豸���������豸
    Dev_scion = (struct DjyDevice*)OBJ_Search(&(ancestor->Node),
                                                scion_name);
    Lock_MutexPost(&s_tDevMutex);
    if(Dev_scion != NULL)
//      return __dev_Dev2Alias(Dev_scion);
        return Dev_scion;
    else
        return NULL;
}

//----���豸-----------------------------------------------------------------
//����: �����豸�����豸�����������豸��Դ�����ҵ�������nameƥ�����Դ��㣬
//      ���ظþ��ָ�롣���̻߳��ⷽʽ����:
//      A�߳���D_RDWRģʽ��,------�����̲߳����ٴ�
//      A�߳���D_RDONLYģʽ��,----�����߳�(����A)��������D_WRONLYģʽ��
//      D_WRONLYͬ��
//����: name,�豸���ַ���,����·�����������ذ���'dev\'��4���ַ�
//      flags,�豸��ģʽ,D_RDONLY,D_WRONLY,D_RDWR�е�һ��
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//          ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//����: �ɹ����豸(�������ȴ����)�����豸���,���򷵻�NULL.
//-----------------------------------------------------------------------------
struct DjyDevice * Driver_OpenDevice(const char *name,u32 flags,u32 timeout)
{
    struct DjyDevice     *Dev;
    if( ! Lock_MutexPend(&s_tDevMutex,timeout))    //���Ǳ����豸���Ļ�����
        return NULL;
    //���豸���������豸
    Dev = (struct DjyDevice *)OBJ_Search(s_ptDeviceRscTree,name);
    if(Dev != NULL)     //���û���ҵ�name�豸,���ؿ�
    {
        if(flags == O_RDONLY)
        {
            if(Lock_MutexPend(Dev->dReadMutex,timeout)==false)//��ȡ������,���Ǳ����豸��
                Dev = NULL;
            else
                if(Dev->OpenCounter != CN_LIMIT_UINT32)
                    Dev->OpenCounter++;
        }
        else if(flags == O_WRONLY)
        {
            if(Lock_MutexPend(Dev->dWriteMutex,timeout)==false)//��ȡ������,���Ǳ����豸��
                Dev = NULL;
            else
                if(Dev->OpenCounter != CN_LIMIT_UINT32)
                    Dev->OpenCounter++;
        }
        else if (flags == O_RDWR)
        {
            if(Lock_MutexPend(Dev->dReadMutex,timeout)!=false)//��ȡ������,���Ǳ����豸��
            {
                if(Lock_MutexPend(Dev->dWriteMutex,timeout)==false)//��ȡ������,���Ǳ����豸��
                {
                    Lock_MutexPost(Dev->dReadMutex);
                    Dev = NULL;
                }
                else
                    if(Dev->OpenCounter < (CN_LIMIT_UINT32-1) )
                        Dev->OpenCounter += 2;
            }
        }
    }
    Lock_MutexPost(&s_tDevMutex);
    return Dev;
}

//----�򿪺���豸-------------------------------------------------------------
//����: �򿪺���豸������ancestor�豸���������豸�����ҵ�������scion_nameƥ��
//      ����Դ���,�����豸��������̻߳��ⷽʽ����:
//      A�߳���D_RDWRģʽ��,------�����̲߳����ٴ�
//      A�߳���D_RDONLYģʽ��,----�����߳�(����A)��������D_WRONLYģʽ��
//      D_WRONLYͬ��
//����: ancestor�������豸�������豸��
//      scion_name,�豸���ַ���,����·����,
//      flags,�豸��ģʽ,D_RDONLY,D_WRONLY,D_RDWR�е�һ��
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//          ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//����: �ɹ����豸(�������������)�����豸���,���򷵻�NULL.
//-----------------------------------------------------------------------------
struct DjyDevice * Driver_OpenScionDevice(struct DjyDevice * ancestor,
                                const char *scion_name,u32 flags, u32 timeout)
{
    struct DjyDevice     *Dev;
    if(ancestor == NULL)
        return NULL;
    if( ! Lock_MutexPend(&s_tDevMutex,timeout))    //���Ǳ����豸���Ļ�����
        return NULL;
    //���豸���������豸
    Dev = (struct DjyDevice*)OBJ_Search(&(ancestor->Node),
                                                scion_name);
    if(Dev != NULL)     //���û���ҵ�name�豸,���ؿ�
    {
        if(flags == O_RDONLY)
        {
            if(Lock_MutexPend(Dev->dReadMutex,timeout)==false)//��ȡ������,���Ǳ����豸��
                Dev = NULL;
            else
                if(Dev->OpenCounter != CN_LIMIT_UINT32)
                    Dev->OpenCounter++;
        }
        else if(flags == D_WRONLY)
        {
            if(Lock_MutexPend(Dev->dWriteMutex,timeout)==false)//��ȡ������,���Ǳ����豸��
                Dev = NULL;
            else
                if(Dev->OpenCounter != CN_LIMIT_UINT32)
                    Dev->OpenCounter++;
        }
        else if (flags == O_RDWR)
        {
            if(Lock_MutexPend(Dev->dReadMutex,timeout)!=false)//��ȡ������,���Ǳ����豸��
            {
                if(Lock_MutexPend(Dev->dWriteMutex,timeout)==false)//��ȡ������,���Ǳ����豸��
                {
                    Lock_MutexPost(Dev->dReadMutex);
                    Dev = NULL;
                }
                else
                    if(Dev->OpenCounter < (CN_LIMIT_UINT32-1) )
                        Dev->OpenCounter += 2;
            }
            else
                Dev = NULL;
        }
    }

    Lock_MutexPost(&s_tDevMutex);
    return Dev;
}

//----���ٴ��豸-------------------------------------------------------------
//����: ���豸ָ����֪������£�Driver_OpenDeviceFast���������ַ����������豸��
//      �����豸��ִ���ٶȺܿ졣��������Driver_FindDevice����Driver_CloseDevice
//      �������ʹ�ã���ʵ�ֿ��ٴ��豸����ʱ�رգ�������ռ���豸��
//      ���̻߳��ⷽʽ����:
//      A�߳���D_RDWRģʽ��,------�����̲߳����ٴ�
//      A�߳���D_RDONLYģʽ��,----�����߳�(����A)��������D_WRONLYģʽ��
//      D_WRONLYͬ��
//����: Device,���򿪵��豸ָ�룬������������ȷ�����豸δ��ɾ����
//      flags,�豸��ģʽ,D_RDONLY,D_WRONLY,D_RDWR�е�һ��
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//          ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//����: �ɹ����豸�򷵻ؾ��,���򷵻�NULL.
//-----------------------------------------------------------------------------
struct DjyDevice * Driver_OpenDeviceFast(struct DjyDevice *Device,u32 flags,u32 timeout)
{
    struct DjyDevice     *Dev = Device;
    if( ! Lock_MutexPend(&s_tDevMutex,timeout))    //���Ǳ����豸���Ļ�����
        return NULL;
//  if (DevAlias == 0)
//      return NULL;
//  Dev = __dev_Alias2Dev(DevAlias);
    if(Dev != NULL)     //���û���ҵ�name�豸,���ؿ�
    {
        if(flags == O_RDONLY)
        {
            if(Lock_MutexPend(Dev->dReadMutex,timeout)==false)//��ȡ������,���Ǳ����豸��
                Dev = NULL;
            else
                if(Dev->OpenCounter != CN_LIMIT_UINT32)
                    Dev->OpenCounter++;
        }
        else if(flags == O_WRONLY)
        {
            if(Lock_MutexPend(Dev->dWriteMutex,timeout)==false)//��ȡ������,���Ǳ����豸��
                Dev = NULL;
            else
                if(Dev->OpenCounter != CN_LIMIT_UINT32)
                    Dev->OpenCounter++;
        }
        else if (flags == O_RDWR)
        {
            if(Lock_MutexPend(Dev->dReadMutex,timeout)!=false)//��ȡ������,���Ǳ����豸��
            {
                if(Lock_MutexPend(Dev->dWriteMutex,timeout)==false)//��ȡ������,���Ǳ����豸��
                {
                    Lock_MutexPost(Dev->dReadMutex);
                    Dev = NULL;
                }
                else
                    if(Dev->OpenCounter < (CN_LIMIT_UINT32-1) )
                        Dev->OpenCounter += 2;
            }
            else
            {
                Dev = NULL;
            }
        }
    }
    Lock_MutexPost(&s_tDevMutex);
    return Dev;
}

//----�ر��豸-----------------------------------------------------------------
//����: �ر��豸�������豸�ܻ�������������ֻ���豸ӵ����(�����豸���¼�)���ܹ�
//      ���豸�������ɻ��������ص�����ġ�
//      �豸�򿪺͹رչ�������:
//
//      1.A�̴߳�,D_RDWRģʽ
//      2.A�̹߳ر�,----ȫ���ر�
//
//      1.A�̴߳�,D_WRONLYģʽ
//      2.A�̴߳�,D_RDONLYģʽ
//      3.A�̹߳ر�,----ȫ���ر�
//
//      1.A�̴߳�,D_WRONLYģʽ
//      2.B�̴߳�,D_RDONLYģʽ
//      3.A�̹߳ر�,----ֻ�ر���"д"ģʽ,��ʱC�߳̿���дģʽ���豸,
//      4.B�̹߳ر�,----�����ر���"��"ģʽ,��ʱC�߳̿��ö�дģʽ���豸,
//
//����: Dev,���رյ��豸ָ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t Driver_CloseDevice(struct DjyDevice * Dev)
{
    if (Dev == NULL)
        return false;
    if(Lock_MutexGetOwner(Dev->dReadMutex) == Djy_MyEventId())
    {
        Lock_MutexPost(Dev->dReadMutex );
        if(Dev->OpenCounter != 0)
            Dev->OpenCounter--;
    }
    if(Lock_MutexGetOwner(Dev->dWriteMutex) == Djy_MyEventId())
    {
        Lock_MutexPost(Dev->dWriteMutex );
        if(Dev->OpenCounter != 0)
            Dev->OpenCounter--;
    }

    return true;
}

//----���豸����---------------------------------------------------------------
//����: ���豸��ȡ����.
//����: Dev,�豸ָ��
//      buf,���ڽ������ݵĻ����������������벻С��len��buf�������޷���顣
//      len,��ȡ�����������벻����buf������
//      offset,��ȡλ�����豸�е�ƫ�ƣ��������豸(���紮��)��˵��ͨ����0.
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//          ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//      ���ϲ�����driverģ��ֻ����ʵ���͸������豸���豸������Ҳ���Բ���������
//      �涨��
//����: -1��ʾ��������������ֵΪʵ�ʶ�ȡ�����ݳ���.
//-----------------------------------------------------------------------------
u32 Driver_ReadDevice(struct DjyDevice * Dev,u8 *buf,u32 len,u32 offset,u32 timeout)
{
    if (Dev == NULL)
        return -1;
    else
    {
        //ֻ���豸�ĳ����˲��ܲ����豸
//        if(Lock_MutexGetOwner(Dev->dReadMutex) == Djy_MyEventId() )
            return (Dev->dRead(Dev->PrivateTag,buf,len,offset,timeout));
//        else
//            return -1;
    }
}

//----д�豸����---------------------------------------------------------------
//����: ������д���豸.
//����: Dev,�豸ָ��
//      buf,���������ݵĻ����������������벻С��len��buf�������޷���顣
//      len,���͵����������벻����buf������
//      offset,д��λ�����豸�е�ƫ�ƣ��������豸(���紮��)��˵��ͨ����0.
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//          ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//      BlockOption������ѡ�ȡֵΪCN_BLOCK_BUFFER��CN_BLOCK_COMPLETE�������
//          �˶��������崦��
//      ���ϲ�����driverģ��ֻ����ʵ���͸������豸���豸������Ҳ���Բ���������
//      �涨��
//����: -1��ʾ��������������ֵΪʵ��д�����ݳ���.
//-----------------------------------------------------------------------------
u32 Driver_WriteDevice(struct DjyDevice * Dev,u8 *buf,
                  u32 len,u32 offset,bool_t BlockOption,u32 timeout)
{
    if (Dev == NULL)
        return -1;
    else
    {
//        if(Lock_MutexGetOwner(Dev->dWriteMutex) == Djy_MyEventId())
            return (Dev->dWrite(Dev->PrivateTag,buf,len,offset,
                                                BlockOption,timeout));
//        else
//            return -1;
    }
}

//----�����豸����-------------------------------------------------------------
//����: ���ڿ����豸.
//����: Dev,�豸ָ��
//      cmd,����ţ�driver.h��Ԥ������һЩ����Լ��û������ʹ�ù��򣬿ɲ���
//          ����CN_DRV_CTRL_START���帽�����롣
//      data1��data2,������������Ҫ�Ĳ��������豸�����߶���
//����: -1��ʾ��������������ֵ���豸�����߶���
//-----------------------------------------------------------------------------
u32 Driver_CtrlDevice(struct DjyDevice * Dev,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    if (Dev == NULL)
        return -1;
    else
    {
//        if( (Lock_MutexGetOwner(Dev->dReadMutex) == Djy_MyEventId())
//           || (Lock_MutexGetOwner(Dev->dWriteMutex) == Djy_MyEventId()) )
            return (Dev->dCtrl(Dev->PrivateTag,cmd,data1,data2));
//        else
//            return -1;
    }
}

//----�����û���ǩ-------------------------------------------------------------
//����: �����豸���ƿ��е�UserTag��Ա��ֵ
//����: Dev,�豸ָ��
//      tag,�����õ�ֵ
//����: ��
//-----------------------------------------------------------------------------
void Driver_SetUserTag(struct DjyDevice * Dev,ptu32_t tag)
{
    if (Dev == NULL)
        return;
    else
    {
        Dev->UserTag = tag;
    }
}

//----��ȡ�û���ǩ-------------------------------------------------------------
//����: ��ȡ�豸���ƿ��е�UserTag��Ա��ֵ
//����: Dev,�豸ָ��
//����: UserTag��Ա��ֵ
//-----------------------------------------------------------------------------
ptu32_t Driver_GetUserTag(struct DjyDevice * Dev)
{
    if (Dev == NULL)
        return 0;
    else
    {
        return Dev->UserTag;
    }
}

//----Multiplex���ƺ���--------------------------------------------------------
//����: �����豸��Multiplex���������ö�д������ֵ������devMultiplexAddǰ�����ȵ�
//      �ñ������������дˮƽ�������豸driver���ʱ��Ĭ��ֵ������Ĭ��ֵ�Ƕ��٣�
//      ��ο���Ӧ�豸driver��˵����

//      �豸�Ƿ�֧��������ֵ��Ҳ�Ǿ����豸driver���£�����֧�֣�������
//      CN_MULTIPLEX_ERROR��

//      ���dMultiplexAddΪ�գ��������豸��֧��Multiplex���ܣ�������Ҳ������
//      CN_MULTIPLEX_INVALID
//����: DevAlias,���������豸����,��Driver_FindDevice�������ء�
//      ReadLevel,������ˮƽ��==NULL��ʾ������ReadLevel��*ReadLevel=0��ʾֻ��ѯ
//      WriteLevel,д����ˮƽ��==NULL��ʾ������WriteLevel��*WriteLevel=0��ʾֻ��ѯ
//����: CN_MULTIPLEX_SUCCESS = �ɹ����ã�
//      CN_MULTIPLEX_INVALID = �豸��֧��Multiplex���ܡ�
//      CN_MULTIPLEX_ERROR   = �豸֧��Multiplex�����ܾ��˱��ε���
//-----------------------------------------------------------------------------
u32 Driver_MultiplexCtrl(struct DjyDevice * Dev,u32 *ReadLevel,u32 *WriteLevel)
{
//  struct DjyDevice *Dev;
    if (Dev == NULL)
        return CN_MULTIPLEX_INVALID;
    else
    {
//      Dev = __dev_Alias2Dev(DevAlias);
        if(Dev->dMultiplexAdd != NULL)
        {
            if(Dev->dCtrl(Dev->PrivateTag,CN_DRV_CTRL_SET_LEVEL,(ptu32_t)ReadLevel,
                               (ptu32_t)WriteLevel) == CN_LIMIT_UINT32)
                return CN_MULTIPLEX_ERROR;
            else
                return CN_MULTIPLEX_SUCCESS;
        }
        else
            return CN_MULTIPLEX_INVALID;
    }
}

//----Multiplex���ú���--------------------------------------------------------
//����: ��һ���豸���뵽һ����·���ü��У�ʹ���豸Index��
//
//����: MultiplexSets��Ŀ���·���ü�ָ��
//      Dev,һ�鱻�������豸Index�����ַ,����Driver_FindDevice�����õ���Ҳ����
//          open�����õ�
//      num,Dev�������豸����
//      SensingBit,����Ȥ�Ĳ�������Multiplex.h��CN_MULTIPLEX_SENSINGBIT_READ����
//          ����س�������.�ر�ע�⣬�����0�����MultiplexSets��ɾ������(driver
//          ������Multiplex_Del����)����31������Ȥ��bit��bit31���ڶ�����󴥷���
//          �ͣ�ȡֵ CN_MULTIPLEX_SENSINGBIT_AND �� CN_MULTIPLEX_SENSINGBIT_OR
//����: �����ɹ����豸����
//-----------------------------------------------------------------------------
u32 Driver_MultiplexAdd(struct MultiplexSetsCB *MultiplexSets,
                        struct DjyDevice **Dev,u32 num,u32 SensingBit)
{
    u32 n,result=0;
    if (Dev == NULL)
        return 0;
    else
    {
        for(n = 0; n < num; n++)
        {
//            Dev = __dev_Alias2Dev(DevAliases[n]);
            if(Dev[n]->dMultiplexAdd != NULL)
            {
                if( (Dev[n]->dMultiplexAdd(Dev[n]->PrivateTag,MultiplexSets,
                                        Dev[n],SensingBit)) )
                    result++;
            }
        }
        return result;
    }
}

//----Multiplex�������-----------------------------------------------------------
//����: ��һ���豸��һ����·���ü���ɾ����ʹ���豸Index��
//����: MultiplexSets��Ŀ���·���ü�ָ��
//      Dev,һ�鱻�������豸Index�����ַ,��Driver_FindDevice�����õ���Ҳ����
//          open�����õ���
//      num,Dev�������豸����
//����: ��
//-----------------------------------------------------------------------------
void Driver_MultiplexDel(struct MultiplexSetsCB *MultiplexSets,
                         struct DjyDevice **Dev,u32 num)
{
    Driver_MultiplexAdd(MultiplexSets,Dev,num,0);
}

