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
//����ģ��:���豸����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�ṩ���豸������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "errno.h"
#include "object.h"
#include "multiplex.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CN_DEV_NAME_LIMIT   255     //�豸�����Ȳ��ܳ���255�ַ�
struct DjyDevice;
struct  Object;

#define D_RDONLY    1   //ֻ��ģʽ
#define D_WRONLY    2   //ֻдģʽ
#define D_RDWR      3   //��дģʽ

//���豸ģ�������룬��enum������enum_drv_no_error��ʼ������1.
enum _DRV_ERROR_CODE_
{
    EN_DRV_NO_ERROR = CN_DRV_NO_ERROR,  //û�д���
    EN_DRV_HANDLE_ERROR,                //�������
    EN_DRV_HOMONYMY,                    //�豸��������
    EN_DRV_DEV_DEL,                   //ɾ���豸ʧ��
};

//�����豸��������
#define CN_DRV_CTRL_START           0   //�����豸����Щ�ܿ��Ƶ�Դ���豸��Ҫ
#define CN_DRV_CTRL_SHUTDOWN        1   //�رյ�Դ,�ϵ�ǰ��Ҫ�ض�������豸��Ҫ
#define CN_DRV_CTRL_SLEEP           2   //�豸����͹���״̬,���ڵ�Դ����
#define CN_DRV_CTRL_RESUME          3   //�豸�ָ�,�ӵ͹���״̬����
#define CN_DRV_CTRL_CHECK           4   //����豸״̬
#define CN_DRV_CTRL_SET_FUNC        5   //����������/���/����ʱ�ص�����
#define CN_DRV_CTRL_SET_LEVEL       6   //�����豸�¼�����д����ˮƽ

#define CN_DRV_CTRL_USER            0x80   //�Զ�����豸���������0x100��ʼ
//�û������������������0x80��ʼ������:
//#define CN_XXX_CTRL_MYCMD1        (CN_DRV_CTRL_USER+0)
//#define CN_XXX_CTRL_MYCMD2        (CN_DRV_CTRL_USER+1)

//����dev_Write���������(block_option����)���ﵽ�����󣬺������ء�
#define CN_BLOCK_BUFFER         0       //dev_WriteDevice����������Ƿ��͵�������
#define CN_BLOCK_COMPLETE       1       //dev_WriteDevice����������Ǵ������

//�����豸ʱ��ͨ����Щָ�룬��ӵ����豸�����ṩ�ĺ�����
//����ֵ:�ɹ�д����ֽ�����
//�Դ���Ϊ�����������dev_WriteDeviceʱBlockOption = CN_BLOCK_BUFFER���򷵻�ֵ��
//ʾ��������ʱ�ɹ�д�뵽�豸��������������������ȷ���Ƿ��Ѿ��������ڴ����ȥ�ˡ�
//���BlockOption = CN_BLOCK_COMPLETE������ȷ��������ɲŷ��ء�
//������Զ�485ͨ���ر����ã���ΪRS485��Ҫ�����շ��л���
typedef u32 (*fntDevWrite)(ptu32_t PrivateTag,u8 *buf,
                           u32 len,u32 offset,bool_t BlockOption,u32 timeout);
//����ֵ:�ɹ���ȡ���ֽ���
typedef u32 (*fntDevRead) (ptu32_t PrivateTag,u8 *buf,
                                   u32 len,u32 offset,u32 timeout);
//����ֵ:�յ���֧�ֵ��������-1��0��ʾ�ɹ�ִ�У���������ֵ�ĺ����Զ�
typedef u32 (*fntDevCtrl) (ptu32_t PrivateTag,u32 cmd,
                                     ptu32_t data1,ptu32_t data2);
//����ֵ:true=�ɹ�ִ�У�false=ʧ�ܡ�
//SendsingBit��Ϊ0��ʾ��MultiplexSets��ɾ�����豸
typedef bool_t (*fntDevMultiplexAdd) (ptu32_t PrivateTag,
                                       struct MultiplexSetsCB *MultiplexSets,
                                       struct DjyDevice *Dev,
                                       u32 SensingBit);
#define DEV_READ_MUTEX_SYS      1       //bit0,1=���豸�Ļ�������ϵͳ����ġ�
#define DEV_WRITE_MUTEX_SYS     2       //bit1,1=д�豸�Ļ�������ϵͳ����ġ�
//ע�⣬���ṩdev_open��dev_close���������������Щ�豸��Ҫ��openʱ����dev_open
//���һЩ���ù����ģ�����dev_Ctrl������CN_DRV_CTRL_START���ͬ��Ҳ���ṩ
//dev_close�ӿ�
struct DjyDevice
{
    struct  Object Node;
    fntDevWrite  dWrite;
    fntDevRead   dRead;
    fntDevCtrl   dCtrl;
    fntDevMultiplexAdd dMultiplexAdd;   //���豸driver��֧�ֶ�·���ã����ÿա�
    struct MutexLCB *dReadMutex;        //������,�����豸��ռʽ������
    struct MutexLCB *dWriteMutex;       //������,�����豸��ռʽд����
    u32    MutexFlag;                   //��־���������û��ṩ��,����ϵͳ����ġ�
                                        //��DEV_READ_MUTEX_SYS�Ķ���
//  u32 ProtectCounter;                 //ɾ���������� 0 ��ʾ���豸����ɾ��
    u32 OpenCounter;                    //�򿪼��������� 0 ��ʾ���豸���򿪣���
                                        //��ɾ��.��д��ʽ�򿪶������㣬��������
                                        //��ʽ��+1����д����ʽ��Ҳ+1������д��
                                        //��ʽ����+2
    ptu32_t PrivateTag;                 //���豸���������ݣ��ڴ����豸ʱ����������
    ptu32_t UserTag;                    //�û��������ݣ��û�ʹ�ù���������
};

ptu32_t ModuleInstall_Driver(ptu32_t para);
struct DjyDevice * Driver_DeviceCreate(  struct DjyDevice *         ParentDevice,
                                const char              *name,
                                struct MutexLCB      *dReadMutex,
                                struct MutexLCB      *dWriteMutex,
                                fntDevWrite            WriteFunc ,
                                fntDevRead             ReadFunc,
                                fntDevCtrl             Ctrl ,
                                fntDevMultiplexAdd     MultiplexAdd,
                                ptu32_t                 tag);
bool_t Driver_ProtectDevice(u32 DevAlias);
bool_t Driver_UnProtectDevice(u32 DevAlias);
bool_t Driver_DeleteDevice(struct DjyDevice * handle);
struct DjyDevice *Driver_FindDevice(const char * name);
struct DjyDevice *Driver_FindScionDevice(struct DjyDevice * ancestor,const char * scion_name);
struct DjyDevice * Driver_OpenDevice(const char *name,u32 flags,u32 timeout);
struct DjyDevice * Driver_OpenScionDevice(struct DjyDevice * ancestor,
                                const char *scion_name,u32 flags, u32 timeout);
struct DjyDevice * Driver_OpenDeviceFast(struct DjyDevice *Device,u32 flags,u32 timeout);
bool_t Driver_CloseDevice(struct DjyDevice * handle);
u32 Driver_ReadDevice(struct DjyDevice * handle,u8 *buf,u32 len,u32 offset,u32 timeout);
u32 Driver_WriteDevice(struct DjyDevice * handle,u8 *buf,
                  u32 len,u32 offset,bool_t BlockOption,u32 timeout);
u32 Driver_CtrlDevice(struct DjyDevice * handle,u32 cmd,ptu32_t data1,ptu32_t data2);
u32 Driver_MultiplexCtrl(struct DjyDevice * Dev,u32 *ReadLevel,u32 *WriteLevel);
u32 Driver_MultiplexAdd(struct MultiplexSetsCB *MultiplexSets,
                        struct DjyDevice **Dev,u32 num,u32 SensingBit);
void Driver_MultiplexDel(struct MultiplexSetsCB *MultiplexSets,
                         struct DjyDevice **Dev,u32 num);
void Driver_SetUserTag(struct DjyDevice * Dev,ptu32_t tag);
ptu32_t Driver_GetUserTag(struct DjyDevice * Dev);

#ifdef __cplusplus
}
#endif
#endif //__DRIVER_H___
