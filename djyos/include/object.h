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
//����ģ��: ��Դ����
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��������ӡ��ƶ���ɾ����������Դ���ķ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __OBJECT_H__
#define __OBJECT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <list.h>
#include <stdio.h>

#define CN_OBJ_NAME_LIMIT   255

//Target����cmd��ͬ�������ֿ��ܣ�struct Object*����struct ObjectPort*����ʵ��
//ʱʹ��ǿ������ת����
//struct Object*��CN_OBJ_CMD_OPEN��CN_OBJ_CMD_SHOW��CN_OBJ_CMD_READDIR��
//                CN_OBJ_CMD_DELETE��CN_OBJ_CMD_STAT
//struct ObjectPort*����������

//����4�����������õ������para2������Ӧ�Է�����
//1��ʹ��va_list *��2���ýṹָ��
typedef ptu32_t (*fnObjOps)(u32 dwCMD, ptu32_t context, ptu32_t args, ...);
//��ͬ����������壺
//-----------------CN_OBJ_CMD_OPEN---------------------------------
//���ܣ����ļ���Ŀ¼��
// target��struct Object *�����򿪵�path���Ѿ���Object�����е����һ��Object
// para1��path��target��Ĳ��֣�������NULL��
// para2��u64*����32λ��open���õ�oflag��������32λ���� cmd == O_CREAT ʱ���ã�
//        ��ʾ�ļ�����Ȩ�ޣ� S_ISUID ϵ�г�����stat.h�ж���
//-----------------CN_OBJ_CMD_READ---------------------------------
//���ܣ����ļ�
// target��struct ObjectPort * ���ͣ�����ȡ���ļ�ָ��
// para1���������ݵ� buf ָ�롣
// para2��buf �ĳߴ硣
//----------------CN_OBJ_CMD_WRITE---------------------------------
//���ܣ�д�ļ�
// target��struct ObjectPort * ���ͣ���д����ļ�ָ��
// para1��д�����ݵ� buf ָ�롣
// para2��д�����ݵĳ��ȡ�
//----------------CN_OBJ_CMD_CLOSE---------------------------------
//���ܣ��ر��ļ���Ŀ¼��
// target��struct ObjectPort * ���ͣ����رյ��ļ�ָ��
// para1�����á�
// para2�����á�
//----------------CN_OBJ_CMD_SHOW----------------------------------
//���ܣ���ʾ�ļ���Ŀ¼�����ݣ�ע�⣬Ŀ¼�����ݣ�������Ŀ¼�б�������Ŀ¼��˵����
//      Ҳ�������������ݣ���CN_OBJ_CMD_SHOWĿ¼��ʵ���߾���
// target��struct Object * ���ͣ�����ʾ��objectָ��
// para1����ʾ������
// para2�����á�
//----------------CN_OBJ_CMD_SEEK----------------------------------
//���ܣ��ƶ��ļ�ָ��
// target��struct ObjectPort * ���ͣ����������ļ�ָ��
// para1��off_t *��Ŀ��ƫ������
// para2��ƫ�Ʋο���SEEK_SET��SEEK_CUR��SEEK_END ֮һ��

//�����������壬�������ܳ���CN_OBJ_CMD_USER����������ϵͳ��Ҫ���±��롣
//16bit��fnObjOps���Ȳ���cmd��32λ�������и�16λ��ʾ����������Ĳ������ȡ�
//���������������ں�̬ copy ����ʱʹ�ã��û�����������ʱ������ġ�
#define CN_OBJ_CMD_OPEN             0   // ���ļ���(���أ�������struct ObjectPort ָ�룻����NULL��)
#define CN_OBJ_CMD_READ             1   // ���ļ���(���أ�������������������-1��)
#define CN_OBJ_CMD_WRITE            2   // д�ļ������أ�д���������������-1
#define CN_OBJ_CMD_CLOSE            3   // �ر��ļ������أ�������0���ļ����ʹ���-2����������-1
#define CN_OBJ_CMD_SHOW             4   // ���أ�������0������-1
#define CN_OBJ_CMD_CRTL             5   // �����ļ������أ��ɿ���������嶨��
#define CN_OBJ_CMD_SEEK             6   // �ض�λ�ļ���(���أ�������0������-1)
#define CN_OBJ_CMD_TELL             7   // ��(���أ�������0������-1)
#define CN_OBJ_CMD_DELETE           8   // ɾ���ļ���(���أ�������0������-1)
#define CN_OBJ_CMD_STAT             9   // ���أ�������0������-1
#define CN_OBJ_CMD_TRUNCATE         10  // ���أ�������0������-1
#define CN_OBJ_CMD_READDIR          11  // ���أ�������0������-1����ɣ�1
#define CN_OBJ_CMD_SYNC             12  // ���أ�������0������-1����ɣ�1
#define CN_OBJ_CMD_MULTIPLEX_ADD    13  // ���أ�������0������-1����ɣ�1
#define CN_OBJ_CMD_FILE_SPECIAL     14  // ���أ��ο� CN_FILE_STATICDATA ��
#define CN_OBJ_CMD_FSTAT            15  // ���أ�������0������-1

#define CN_OBJ_FCNTL_START          0x1000 // fcntl ��������ʼ����
#define CN_OBJ_IOCTL_START          0x2000 // ioctl ��������ʼ����
#define CN_OBJ_CMD_USER             0xF000 // �û��Զ���������ʼֵ�����û�д������������ͣ���ͬģ����Զ����ظ���ֵ

//���飨��ǿ�ƣ���������֧�ֵ�����ʱ fnObjOps ��������ֵ��
#define CN_OBJ_CMD_UNSUPPORT        (-1)


#define GROUP_POINT                 (1)
#define TEMPORARY_POINT             (2)
#define DIR_POINT                   (3)
#define REG_POINT                   (4)

typedef fnObjOps    tagObjOps;

struct Object *OBJ_NewSanityChild(struct Object*, fnObjOps, ptu32_t, const char*);
s32            OBJ_Del(struct Object*);
s32            OBJ_SetOps(struct Object*, fnObjOps);
s32            OBJ_SetRepresent(struct Object*, ptu32_t);
void           OBJ_SetCurrent(struct Object*);
struct Object *OBJ_AddToPrevious(struct Object*, fnObjOps, ptu32_t, const char*);
struct Object *OBJ_AddToNext(struct Object*, fnObjOps, ptu32_t, const char*);
struct Object *OBJ_AddChild(struct Object*, fnObjOps, ptu32_t, const char*);
struct Object *OBJ_AddChildHead(struct Object*,fnObjOps, ptu32_t,const char*);
struct Object *OBJ_InsertChild(struct Object*, struct Object*);
struct Object *OBJ_TakeOutBranch(struct Object*);
s32            OBJ_MoveToBranch(struct Object*,struct Object*);
s32            OBJ_MoveToLast(struct Object*);
s32            OBJ_MoveToHead(struct Object*);
s32            OBJ_MoveToNext(struct Object*, struct Object*);
s32            OBJ_MoveToPrevious(struct Object*, struct Object*);
s32            OBJ_RoundPrevious(struct Object*);
s32            OBJ_RoundNext(struct Object*);
s32            OBJ_Rename(struct Object*, const char*);
fnObjOps       OBJ_Ops(struct Object*);
ptu32_t        OBJ_Represent(struct Object*);
list_t        *OBJ_PortList(struct Object*);
const char    *OBJ_Name(struct Object*);
struct Object *OBJ_Parent(struct Object*);
struct Object *OBJ_Child(struct Object*);
struct Object *OBJ_Previous(struct Object*);
struct Object *OBJ_Next(struct Object*);
struct Object *OBJ_Head(struct Object*);
struct Object *OBJ_Twig(struct Object*);
u32            OBJ_Level(struct Object*);
bool_t         OBJ_IsParentChild(struct Object*, struct Object*);
bool_t         OBJ_IsLast(struct Object*);
bool_t         OBJ_IsHead(struct Object*);
struct Object *OBJ_TraveChild(struct Object*, struct Object*);
struct Object *OBJ_TraveScion(struct Object*, struct Object*);
struct Object *OBJ_SearchSibling(struct Object*, const char*);
struct Object *OBJ_SearchChild(struct Object*, const char*);
struct Object *OBJ_SearchScion(struct Object*, const char*);
struct Object *OBJ_Search(struct Object*, const char*);
u32            OBJ_Sequencing(struct Object*);
struct Object *OBJ_NewSanityChild(struct Object*, fnObjOps, ptu32_t, const char*);
struct Object *OBJ_Root(void);
struct Object *OBJ_Current(void);
s32            OBJ_IsRoot(struct Object*);
struct Object *OBJ_MatchPath(const char*, char**, char**);
struct Object *OBJ_BufferPath(struct Object*, char*);
void           OBJ_UnBufferPath(struct Object*);
s32            OBJ_NameSantiy(const char*);
struct Object *OBJ_Restore(struct Object*);
struct Object *OBJ_Replace(struct Object*);

s32            __OBJ_Type(struct Object*);
void           __OBJ_Free(struct Object*);
s32            __OBJ_Del(struct Object*);
void           __OBJ_Init(struct Object*);
const char    *__OBJ_Name(struct Object*);
fnObjOps       __OBJ_Ops(struct Object*);
ptu32_t        __OBJ_Represent(struct Object*);
struct Object *__OBJ_Parent(struct Object*);
struct Object *__OBJ_Child(struct Object*);
struct Object *__OBJ_Prev(struct Object *pObj);
struct Object *__OBJ_Next(struct Object *pObj);
s32            __OBJ_Link(struct Object*, list_t*);
s32            __OBJ_LinksCheck(struct Object*, u32);
s32            __OBJ_Protect(struct Object*);
s32            __OBJ_Forbid(struct Object*);
s32            __OBJ_IsProtected(struct Object*);
s32            __OBJ_Release(struct Object*);
s32            __OBJ_NameSantiy(const char*);
void           __OBJ_SetRepresent(struct Object*, ptu32_t);
void           __OBJ_SetName(struct Object*, const char*);
void           __OBJ_SetOps(struct Object*, fnObjOps);
s32            __OBJ_SetMountPoint(struct Object*);
s32            __OBJ_ClearMountPoint(struct Object*);
s32            __OBJ_InitSystem(void);

#ifdef __cplusplus
}
#endif

#endif //__OBJECT_H__

