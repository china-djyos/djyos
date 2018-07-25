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
#ifndef __DEVICE_H__
#define __DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#include <object.h>
#include <objfile.h>
#include <multiplex.h>

// ���豸ģ�������룬��enum������enum_drv_no_error��ʼ������1.
enum _DRV_ERROR_CODE_
{
    EN_DRV_NO_ERROR = CN_DEV_NO_ERROR, //û�д���
    EN_DRV_HANDLE_ERROR, //�������
    EN_DRV_HOMONYMY, //�豸��������
    EN_DRV_DEV_DEL, //ɾ���豸ʧ��
};

#define CN_DEV_NAME_LIMIT   255 // �豸�����Ȳ��ܳ���255�ַ�

#ifndef O_DBC // �豸�ļ��Ĳ����߼���block after complete����ʾд������ʱ����������ݷ����꣬���ܷ��� (Ŀǰ��������UART)
 #define O_DBC                  0x00000004 // ��Ҫ��֤�������׼O_XXX���ͳ�ͻ
#else
 #error "O_DBC" duplicate definition!
#endif

#ifndef O_DRAW // ��������������ļ����ԣ�offset�����ӣ�
 #define O_DRAW                 0x00000008 // ��Ҫ��֤�������׼O_XXX���ͳ�ͻ
#else
 #error "O_DRAW" duplicate definition!
#endif

//�����豸��������
#define CN_DEV_CTRL_START           (CN_OBJ_CMD_USER+0)      // �����豸����Щ�ܿ��Ƶ�Դ���豸��Ҫ
#define CN_DEV_CTRL_STOP            (CN_OBJ_CMD_USER+1)      // �����豸����Щ�ܿ��Ƶ�Դ���豸��Ҫ
#define CN_DEV_CTRL_SHUTDOWN        (CN_OBJ_CMD_USER+2)      // �رյ�Դ,�ϵ�ǰ��Ҫ�ض�������豸��Ҫ
#define CN_DEV_CTRL_SLEEP           (CN_OBJ_CMD_USER+3)      // �豸����͹���״̬,���ڵ�Դ����
#define CN_DEV_CTRL_RESUME          (CN_OBJ_CMD_USER+4)      // �豸�ָ�,�ӵ͹���״̬����
#define CN_DEV_CTRL_CHECK           (CN_OBJ_CMD_USER+5)      // ����豸״̬
#define CN_DEV_CTRL_SET_FUNC        (CN_OBJ_CMD_USER+6)      // ����������/���/����ʱ�ص�����
#define CN_DEV_CTRL_BLOCK_BUFFER    (CN_OBJ_CMD_USER+7)      // Write����������Ƿ��͵�������
#define CN_DEV_CTRL_BLOCK_COPLETE   (CN_OBJ_CMD_USER+8)      // Write����������Ǵ������
#define CN_DEV_CTRL_USER            (CN_OBJ_CMD_USER+256)    //�Զ�����豸���������0x100��ʼ
//�û������������������0x80��ʼ������:
//#define CN_XXX_CTRL_MYCMD1        (CN_DEV_CTRL_USER+0)
//#define CN_XXX_CTRL_MYCMD2        (CN_DEV_CTRL_USER+1)

//�����豸ʱ��ͨ����Щָ�룬��ӵ����豸�����ṩ�ĺ�����

//���豸�����豸����򿪼��ɹ������ÿռ���
typedef s32 (*fntDevOpen)(tagOFile *of, u32 mode, u32 timeout);
//�ر��豸������豸����Ҫ�رղ������ÿռ���
typedef s32 (*fntDevClose)(tagOFile *of);
//����ֵ:�ɹ�д����ֽ�����
//�Դ���Ϊ�����������dev_WriteDeviceʱBlockOption = CN_BLOCK_BUFFER���򷵻�ֵ��
//ʾ��������ʱ�ɹ�д�뵽�豸��������������������ȷ���Ƿ��Ѿ��������ڴ����ȥ�ˡ�
//���BlockOption = CN_BLOCK_COMPLETE������ȷ��������ɲŷ��ء�
//������Զ�485ͨ���ر����ã���ΪRS485��Ҫ�����շ��л���
typedef s32 (*fntDevWrite)(tagOFile *of, u8 *data, u32 size, u32 offset, u32 timeout);
//����ֵ:�ɹ���ȡ���ֽ���
typedef s32 (*fntDevRead) (tagOFile *of, u8 *data, u32 size, u32 offset, u32 timeout);
//����ֵ:�յ���֧�ֵ��������-1��0��ʾ�ɹ�ִ�У���������ֵ�ĺ����Զ�
typedef s32 (*fntDevCntl) (tagOFile *of, u32 cmd, ptu32_t data1, ptu32_t data2);

s32            DevOpen(const char *name, s32 flags,u32 timeout);
s32            DevClose(s32 handle);
s32            DevRead(s32 handle, void *buf, u32 len, u32 offset, u32 timeout);
s32            DevWrite(s32 handle, void *buf, u32 len, u32 offset, u32 timeout);
s32            DevCntl(s32 handle, u32 cmd, ptu32_t data1, ptu32_t data2);

s32            israw(tagOFile *pOF);
s32            isbc(u32 flags);
const char    *dev_nameo(struct Object *devo);
struct Object *dev_grpaddo(char *name);
s32            dev_grpadd(char *name);
s32            dev_grpdelo(struct Object *grp);
s32            dev_grpdel(char *name);
struct Object *dev_addo(struct Object *grp, const char *name,
                        fntDevOpen dopen, fntDevClose dclose,
                        fntDevWrite dwrite, fntDevRead dread,
                        fntDevCntl dcntl, ptu32_t dtag);
s32            dev_add(const char *grp, const char *name, fntDevOpen dopen, fntDevClose dclose,
                       fntDevWrite dwrite, fntDevRead dread,
                       fntDevCntl dcntl, ptu32_t dtag);
s32            dev_delo(struct Object *devo);
s32            dev_del(const char *grp, const char *name);
ptu32_t        devfileutag(tagOFile *of);
ptu32_t        devfiledtag(tagOFile *of);

#ifdef __cplusplus
}
#endif
#endif //__DEVICE_H___
