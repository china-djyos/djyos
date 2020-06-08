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
#ifndef __DEVICE_H__
#define __DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <fcntl.h>
#include <errno.h>
#include <object.h>
#include <objhandle.h>
#include <multiplex.h>
#include <stdarg.h>
// �豸ģ�������룬��enum������enum_drv_no_error��ʼ������1.
enum _DRV_ERROR_CODE_
{
    EN_DRV_NO_ERROR = CN_DEV_NO_ERROR, //û�д���
    EN_DRV_HANDLE_ERROR, //�������
    EN_DRV_HOMONYMY, //�豸��������
    EN_DRV_DEV_DEL, //ɾ���豸ʧ��
};

#define CN_DEV_NAME_LIMIT   255 // �豸�����Ȳ��ܳ���255�ַ�

//�����豸�������Ϊ����ɴ�����ݣ�
#define CN_DEV_CTRL_START           (F_DSTART)              // �����豸����Щ�ܿ��Ƶ�Դ���豸��Ҫ
#define CN_DEV_CTRL_STOP            (F_DSTOP)               // �����豸����Щ�ܿ��Ƶ�Դ���豸��Ҫ
#define CN_DEV_CTRL_SHUTDOWN        (F_DSHUTDOWN)           // �رյ�Դ,�ϵ�ǰ��Ҫ�ض�������豸��Ҫ
#define CN_DEV_CTRL_SLEEP           (F_DSLEEP)              // �豸����͹���״̬,���ڵ�Դ����
#define CN_DEV_CTRL_RESUME          (F_DRESUME)             // �豸�ָ�,�ӵ͹���״̬����
#define CN_DEV_CTRL_CHECK           (F_DCHECK)              // ����豸״̬
#define CN_DEV_CTRL_SET_FUNC        (F_DHOOK)               // ����������/���/����ʱ�ص�����
#define CN_DEV_CTRL_BLOCK_BUFFER    (F_DBLOCK_BUFFER)       // Write����������Ƿ��͵�������
#define CN_DEV_CTRL_BLOCK_COPLETE   (F_DBLOCK_COPLETE)      // Write����������Ǵ������

//#define CN_DEV_CTRL_USER            (CN_OBJ_CMD_USER+256)    //�Զ�����豸���������0x100��ʼ
//�û������������������0x80��ʼ������:
//#define CN_XXX_CTRL_MYCMD1        (CN_DEV_CTRL_USER+0)
//#define CN_XXX_CTRL_MYCMD2        (CN_DEV_CTRL_USER+1)

//�����豸ʱ��ͨ����Щָ�룬��ӵ����豸�����ṩ�ĺ�����
//���豸�����豸����򿪼��ɹ������ÿռ���
typedef s32 (*fnDevOpen)(struct objhandle *hdl, u32 mode, u32 timeout);
//�ر��豸������豸����Ҫ�رղ������ÿռ���
typedef s32 (*fnDevClose)(struct objhandle *hdl);
//����ֵ:�ɹ�д����ֽ�����
//�Դ���Ϊ�����������dev_WriteDeviceʱBlockOption = CN_BLOCK_BUFFER���򷵻�ֵ��
//ʾ��������ʱ�ɹ�д�뵽�豸��������������������ȷ���Ƿ��Ѿ��������ڴ����ȥ�ˡ�
//���BlockOption = CN_BLOCK_COMPLETE������ȷ��������ɲŷ��ء�
typedef s32 (*fnDevWrite)(struct objhandle *hdl, u8 *data, u32 size, u32 offset, u32 timeout);
//����ֵ:�ɹ���ȡ���ֽ���
typedef s32 (*fnDevRead) (struct objhandle *hdl, u8 *data, u32 size, u32 offset, u32 timeout);
//����ֵ: CN_OBJ_CMD_UNSUPPORT �� CN_OBJ_CMD_EXECUTED
typedef s32 (*fnDevCntl) (struct objhandle *hdl, u32 cmd, va_list *args);


s32 handle_IsBlockComplete(u32 flags);
const char *Device_GetName(struct Object *devo);
//ptu32_t dev_GetDrvTagFromObj(struct obj *devo);
struct Object *Device_Create(const char *name, fnDevOpen dopen, fnDevClose dclose,
                        fnDevWrite dwrite, fnDevRead dread, fnDevCntl dcntl,
                        ptu32_t DrvTag);
s32 dev_Delete(struct Object *dev);
s32 Device_DeleteByName(const char *name);
void Device_SetDrvTag(s32 fd,ptu32_t DrvTag);
ptu32_t Device_SetUserTag(s32 fd,ptu32_t UserTag);
ptu32_t Device_GetUserTag(s32 fd);
ptu32_t Device_GetDrvTag(s32 fd);

s32 Device_Open(const char *name, s32 flags,u32 timeout);
s32 Device_Close(s32 fd);
s32 Device_Read(s32 fd, void *buf, u32 len, u32 offset, u32 timeout);
s32 Device_Write(s32 fd, void *buf, u32 len, u32 offset, u32 timeout);
s32 Device_Ctrl(s32 fd, u32 cmd, ptu32_t data1, ptu32_t data2);
#ifdef __cplusplus
}
#endif
#endif //__DEVICE_H___
