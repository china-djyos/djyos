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
#include <include/list.h>
#include <fcntl.h>

// fnObjOps ��������ֵ��
#define CN_OBJ_CMD_UNSUPPORT    (-1)    //��object��֧�ִ�����
#define CN_OBJ_CMD_EXECUTED     (0)     //���ִ�У����طǲ���ֵ����������OpsArgs1���أ�
#define CN_OBJ_CMD_TRUE         (1)     //�������ȷ�����ڷ��ز���ֵ�����
#define CN_OBJ_CMD_FALSE        (2)     //����ش��󣨶��ڷ��ز���ֵ�����
//opsTarget�����ͣ������ֿ��ܣ�struct obj*����struct objhandle*�������������˵��
//ϰ���÷���OpsArgs1��������ִ�н����ָ��
typedef s32 (*fnObjOps)(void *opsTarget, u32 opscmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3);
//��ͬ����������壺
//-----------------CN_OBJ_CMD_OPEN---------------------------------
//���ܣ����ļ���Ŀ¼��
// opsTarget��struct Object *�����򿪵�path���Ѿ���Object�����е����һ��Object
// OpsArgs1��struct objhandle *�����ؽ����
// OpsArgs2��u64*����32λ��open���õ� oflag(O_CREAT��) ��������32λ���� cmd == O_CREAT
//         ʱ���ã���ʾ�ļ�����Ȩ�ޣ� S_ISUID ϵ�г�����stat.h�ж���
// OpsArgs3��path��opsTarget��Ĳ��֣�������NULL��
// ���أ�CN_OBJ_CMD_UNSUPPORT �� CN_OBJ_CMD_EXECUTED
//-----------------CN_OBJ_CMD_READ---------------------------------
//���ܣ����ļ�
// opsTarget��struct objhandle * ���ͣ�����ȡ���ļ�ָ��
// OpsArgs1��ssize_t *�����ؽ����ʵ�ʶ��õ����ݳ��ȡ�
// OpsArgs2���������ݵ� buf ָ�롣
// OpsArgs3��buf �ĳߴ硣
// ���أ�CN_OBJ_CMD_UNSUPPORT �� CN_OBJ_CMD_EXECUTED
//----------------CN_OBJ_CMD_WRITE---------------------------------
//���ܣ�д�ļ�
// opsTarget��struct objhandle * ���ͣ���д����ļ�ָ��
// OpsArgs1��ssize_t *�����ؽ����ʵ��д������ݳ��ȡ�
// OpsArgs2��д�����ݵ� buf ָ�롣
// OpsArgs3��д�����ݵĳ��ȡ�
// ���أ�CN_OBJ_CMD_UNSUPPORT �� CN_OBJ_CMD_EXECUTED
//----------------CN_OBJ_CMD_CLOSE---------------------------------
//���ܣ��ر��ļ���Ŀ¼��
// opsTarget��struct objhandle * ���ͣ����رյ��ļ�ָ��
// OpsArgs1�����á�
// OpsArgs2�����á�
// OpsArgs3�����á�
// ���أ�CN_OBJ_CMD_TRUE �� CN_OBJ_CMD_FALSE �� CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_SHOW----------------------------------
//���ܣ���ʾ�ļ���Ŀ¼�����ݣ�ע�⣬Ŀ¼�����ݣ�������Ŀ¼�б�������Ŀ¼��˵����
//      Ҳ�������������ݣ���CN_OBJ_CMD_SHOWĿ¼��ʵ���߾���
// opsTarget��struct Object * ���ͣ�����ʾ��objectָ��
// OpsArgs1�����á�
// OpsArgs2����ʾ�������ɾ���ʵ���߶��塣
// OpsArgs3��path��opsTarget��Ĳ��֣�������NULL��
// ���أ�CN_OBJ_CMD_SUCCESS �� CN_OBJ_CMD_FALSE �� CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_SEEK----------------------------------
//���ܣ��ƶ��ļ�ָ��
// opsTarget��struct objhandle * ���ͣ����������ļ�ָ��
// OpsArgs1��off_t *�����ؽ�����ɹ����ض�λ���λ�ã���ʧ�ܣ�-1����
// OpsArgs2��off_t *��Ŀ��ƫ������
// OpsArgs3��ƫ�Ʋο���SEEK_SET��SEEK_CUR��SEEK_END ֮һ��
// ���أ�CN_OBJ_CMD_UNSUPPORT �� CN_OBJ_CMD_EXECUTED
//----------------CN_OBJ_CMD_TELL----------------------------------
//���ܣ���ȡ�ļ�ָ��λ��
// opsTarget��struct objhandle *�����������ļ�ָ��
// OpsArgs1��off_t *���ļ�ָ�뵱ǰλ��ָ��
// OpsArgs2�����á�
// OpsArgs3�����á�
// ���أ�CN_OBJ_CMD_UNSUPPORT �� CN_OBJ_CMD_EXECUTED
//----------------CN_OBJ_CMD_DELETE----------------------------------
//���ܣ�ɾ���ļ�
// opsTarget��struct Object* ���ͣ��������Ķ���ָ��
// OpsArgs1�����á�
// OpsArgs2�����á�
// OpsArgs3��path��opsTarget��Ĳ��֣�������NULL��
// ���أ�CN_OBJ_CMD_SUCCESS �� CN_OBJ_CMD_FALSE �� CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_STAT----------------------------------
//���ܣ�ȡ�ļ�״̬
// opsTarget��struct Object *���������Ķ���ָ��
// OpsArgs1��struct stat *�������������Ľṹָ��
// OpsArgs2�����á�
// OpsArgs3��path��opsTarget��Ĳ��֣�������NULL��
// ���أ�CN_OBJ_CMD_SUCCESS �� CN_OBJ_CMD_FALSE �� CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_TRUNCATE----------------------------------
//���ܣ��ض��ļ�
// opsTarget��struct objhandle *�����������ļ�ָ��
// OpsArgs1������
// OpsArgs2��s64*���ضϳߴ�
// OpsArgs3�����á�
// ���أ�CN_OBJ_CMD_SUCCESS �� CN_OBJ_CMD_FALSE �� CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_READDIR----------------------------------
//���ܣ���ȡĿ¼
// opsTarget��struct Object* ���ͣ��������Ķ���ָ��
// OpsArgs1��DIR *�������������Ľṹָ��
// OpsArgs2�����á�
// OpsArgs3������
// ���أ�CN_OBJ_CMD_SUCCESS �� CN_OBJ_CMD_FALSE �� CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_SYNC----------------------------------
//���ܣ�ͬ���ļ������ļ�������ˢ�µ��ļ��
// opsTarget��struct objhandle *�����������ļ�ָ��
// OpsArgs1������
// OpsArgs2������
// OpsArgs3�����á�
// ���أ�CN_OBJ_CMD_SUCCESS �� CN_OBJ_CMD_FALSE �� CN_OBJ_CMD_UNSUPPORT

//�����������壬�������ܳ���CN_OBJ_CMD_USER����������ϵͳ��Ҫ���±��롣
//16bit��fnObjOps����cmd��32λ��������16~23λ��ʾ����������Ĳ������ȡ�
//���������������ں�̬ copy ����ʱʹ�ã��û�����������ʱ������ġ�24~31bit������־
#define CN_TARGET_IS_OBJ      (1<<31)               //fnObjOps ������ opsTarget ������object
#define CN_TARGET_IS_HDL      (0<<31)               //fnObjOps ������ opsTarget ������ handle

#define CN_OBJ_CMD_MSK        0xffff                // cmdռ�õ�16bit
#define CN_OBJ_CMD_OPEN       (CN_TARGET_IS_OBJ|0)  // ���ļ���
#define CN_OBJ_CMD_READ       (CN_TARGET_IS_HDL|1)  // ���ļ���
#define CN_OBJ_CMD_WRITE      (CN_TARGET_IS_HDL|2)  // д�ļ���
#define CN_OBJ_CMD_CLOSE      (CN_TARGET_IS_HDL|3)  // �ر��ļ�
#define CN_OBJ_CMD_SHOW       (CN_TARGET_IS_OBJ|4)  // ��ʾ�ļ���Ŀ¼���������������ļ�ϵͳ
#define CN_OBJ_CMD_SEEK       (CN_TARGET_IS_HDL|5)  // �ض�λ�ļ�
#define CN_OBJ_CMD_TELL       (CN_TARGET_IS_HDL|6)  // ȡ�ļ�ָ��λ��
#define CN_OBJ_CMD_DELETE     (CN_TARGET_IS_OBJ|7)  // ɾ���ļ���
#define CN_OBJ_CMD_STAT       (CN_TARGET_IS_OBJ|8)  // ȡ�ļ�״̬
#define CN_OBJ_CMD_TRUNCATE   (CN_TARGET_IS_HDL|9)  // �ж��ļ�
#define CN_OBJ_CMD_READDIR    (CN_TARGET_IS_OBJ|10) // ��Ŀ¼
#define CN_OBJ_CMD_SYNC       (CN_TARGET_IS_HDL|11) // ͬ���ļ�
#define CN_OBJ_FCNTL          (CN_TARGET_IS_HDL|12) // fcntl ����������
#define CN_OBJ_IOCTL          (CN_TARGET_IS_HDL|13) // ioctl ����������
#define CN_OBJ_RENAME         (CN_TARGET_IS_OBJ|14) // �����ļ����ƻ�λ��

#define CN_OBJ_CMD_USER       (CN_TARGET_IS_HDL|0x1000) // �û��Զ���������ʼֵ�����û�д������������ͣ���ͬģ����Զ����ظ���ֵ


//Ȩ�޹�����stat.h�� S_IRUSR ���Ѿ������ˡ�
//union __rights{
//    struct __bit{
//        u32 r:1; // �ɶ���
//        u32 w:1; // ��д��
//        u32 x:1; // ��ִ�У�
//        u32 res:12; // ������
//        u32 muses:16
//    }b;
//    u32 a;
//};
//
#define CN_OBJ_TEMPORARY    0x80000000
struct __ObjBitFlag
{
    u32 inuse:24;           // �������ü���
    u32 temporary:1;        //�Ƿ���ʱ����
};
struct Object
{
    char *name;             // ���������������ļ�ϵͳΪ�ļ�����Ŀ¼���������豸��
                            //�豸��������gui���Ǵ�������
    ptu32_t ObjPrivate;     // ����˽�����ݣ�������һ������Ҳ����ָ����������Ľṹ��
    fnObjOps ObjOps;        // ���󷽷���������Ĳ�����
    struct __ObjBitFlag BitFlag;    //��������λ��־
    list_t handles;         // �������������󱻴򿪣�ϵͳ����û�����һ�������
                            // ��������ݽṹ���������
    struct Object *prev,*next, *parent, *child; // �����ϵ��������������
};

struct objhandle;
ptu32_t OBJ_GetPrivate(struct Object *ob);
void OBJ_SetPrivate(struct Object *ob, ptu32_t Private);
fnObjOps OBJ_GetOps(struct Object *ob);
s32 OBJ_SetOps(struct Object *ob, fnObjOps ops);
char *OBJ_GetName(struct Object *ob);
char *obj_rename(struct Object *ob,char *NewName);
struct Object *OBJ_GetParent(struct Object *ob);
struct Object *OBJ_GetChild(struct Object *ob);
struct Object *OBJ_GetPrev(struct Object *ob);
struct Object *OBJ_GetNext(struct Object *ob);
struct Object *OBJ_GetHead(struct Object *ob);
s32 OBJ_GetLevel(struct Object *ob);
s32 OBJ_IsHead(struct Object *ob);
s32 OBJ_IsLast(struct Object *ob);
s32 OBJ_IsOnDuty(struct Object *ob);
struct objhandle* obj_TraveFile(struct objhandle *Current, struct Object *Object);
s32 OBJ_LinkHandle(struct objhandle *hdl, struct Object *ob);
bool_t OBJ_Lock(void);
void OBJ_Unlock(void);

//u32 obj_InuseUp(struct Object *ob);
//void obj_InuseUpRange(struct Object *start, struct Object *end);
void OBJ_DutyUp(struct Object *Obj);
//u32 obj_InuseDown(struct Object *ob);
//void obj_InuseDownRange(struct Object *start, struct Object *end);
void OBJ_DutyDown(struct Object *Obj);

s32 OBJ_Delete(struct Object *ob);
struct Object *OBJ_Detach(struct Object *branch);
s32 OBJ_CheckName(const char *name);
struct Object *OBJ_MatchPath(const char *match, char **left);
struct Object *OBJ_BuildTempPath(struct Object *begin, fnObjOps ops,
                            ptu32_t ObjPrivate, char *path);
s32 OBJ_ReleaseTempPath(struct Object *start);
struct Object *OBJ_GetCurrent(void);
void OBJ_SetCurrent(struct Object *ob);
struct Object *OBJ_GetRoot(void);
struct Object *OBJ_NewPrev(struct Object *loc, fnObjOps ops,
                        ptu32_t ObjPrivate, const char *name);
struct Object *OBJ_NewNext(struct Object *loc, fnObjOps ops,
                        ptu32_t ObjPrivate, const char *name);
struct Object *OBJ_NewChild(struct Object *parent, fnObjOps ops,
                         ptu32_t ObjPrivate, const char *name);
struct Object *OBJ_NewHead(struct Object *loc,fnObjOps ops,
                           ptu32_t ObjPrivate, const char *name);
s32 obj_MoveToLast(struct Object *ob);
s32 OBJ_MoveToHead(struct Object *ob);
struct Object *OBJ_InsertToChild(struct Object *loc, struct Object *child);
s32 OBJ_InsertToNext(struct Object *loc, struct Object *next);
s32 OBJ_InsertToPrev(struct Object *loc, struct Object *prev);
s32 OBJ_ChildMoveToOrev(struct Object *parent);
s32 OBJ_ChildMoveToNext(struct Object *parent);
struct Object *OBJ_GetTwig(struct Object *ob);
struct Object *OBJ_ForeachChild(struct Object *parent, struct Object *child);
struct Object * OBJ_ForeachScion(struct Object *ancester, struct Object *scion);
struct Object *OBJ_SearchSibling(struct Object *ob, const char *name);
struct Object *OBJ_SearchChild(struct Object *parent, const char *name);
struct Object *OBJ_SearchScion(struct Object *ancester, const char *name);
struct Object *OBJ_SearchPath(struct Object *start, const char *path);
s32 OBJ_SetMultiplexEvent(struct Object *ob, u32 events);
s32 OBJ_ClrMultiplexEvent(struct Object *ob, u32 events);
s32 issocketactive(s32 Fd, s32 mode);
struct objhandle* OBJ_ForeachHandle(struct objhandle *Current, struct Object *Object);
s32 OBJ_CurWorkPathLen(void);
s32 OBJ_CurWorkPath(char *Buf, u32 BufSize);
s32 OBJ_SetPwd(const char *Path);

#ifdef __cplusplus
}
#endif

#endif //__OBJECT_H__

