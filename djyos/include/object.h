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
#include <list.h>
#include <fcntl.h>

// fnObjOps ��������ֵ��
#define CN_OBJ_CMD_UNSUPPORT    (-1)    //��object��֧�ִ�����
#define CN_OBJ_CMD_EXECUTED     (0)     //���ִ�У����طǲ���ֵ����������OpsArgs1���أ�
#define CN_OBJ_CMD_TRUE         (1)     //�������ȷ�����ڷ��ز���ֵ�����
#define CN_OBJ_CMD_FALSE        (2)     //����ش��󣨶��ڷ��ز���ֵ�����
//oipsTarget�����ͣ������ֿ��ܣ�struct obj*����struct objhandle*�������������˵��
//ϰ���÷���OpsArgs1��������ִ�н����ָ��
typedef s32 (*fnObjOps)(void *opsTarget, u32 opscmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3);
//��ͬ����������壺
//-----------------CN_OBJ_CMD_OPEN---------------------------------
//���ܣ����ļ���Ŀ¼��
// opsTarget��struct obj *�����򿪵�path���Ѿ���Object�����е����һ��Object
// OpsArgs1��struct objhandle *�����ؽ����
// OpsArgs2��u64*����32λ��open���õ�oflag��������32λ���� cmd == O_CREAT ʱ���ã�
//        ��ʾ�ļ�����Ȩ�ޣ� S_ISUID ϵ�г�����stat.h�ж���
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
// ���أ�CN_OBJ_CMD_SUCCESS �� CN_OBJ_CMD_FALSE �� CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_SHOW----------------------------------
//���ܣ���ʾ�ļ���Ŀ¼�����ݣ�ע�⣬Ŀ¼�����ݣ�������Ŀ¼�б�������Ŀ¼��˵����
//      Ҳ�������������ݣ���CN_OBJ_CMD_SHOWĿ¼��ʵ���߾���
// opsTarget��struct obj * ���ͣ�����ʾ��objectָ��
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
// opsTarget��struct obj* ���ͣ��������Ķ���ָ��
// OpsArgs1�����á�
// OpsArgs2�����á�
// OpsArgs3��path��opsTarget��Ĳ��֣�������NULL��
// ���أ�CN_OBJ_CMD_SUCCESS �� CN_OBJ_CMD_FALSE �� CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_STAT----------------------------------
//���ܣ�ȡ�ļ�״̬
// opsTarget��struct obj *���������Ķ���ָ��
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
// opsTarget��struct obj* ���ͣ��������Ķ���ָ��
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
//16bit��fnObjOps���Ȳ���cmd��32λ��������16~23λ��ʾ����������Ĳ������ȡ�
//���������������ں�̬ copy ����ʱʹ�ã��û�����������ʱ������ġ�24~31bit������־
#define CN_TARGET_IS_OBJ      (1<<31)               //fnObjOps ������ opsTarget �����Ƕ���
#define CN_TARGET_IS_HDL      (0<<31)               //fnObjOps ������ opsTarget ������ handle

#define CN_OBJ_CMD_MSK        0xffff                // cmdռ�õ�16bit
#define CN_OBJ_CMD_OPEN       (CN_TARGET_IS_OBJ|0)  // ���ļ���
#define CN_OBJ_CMD_READ       (CN_TARGET_IS_HDL|1)  // ���ļ���
#define CN_OBJ_CMD_WRITE      (CN_TARGET_IS_HDL|2)  // д�ļ���
#define CN_OBJ_CMD_CLOSE      (CN_TARGET_IS_HDL|3)  // �ر��ļ�
#define CN_OBJ_CMD_SHOW       (CN_TARGET_IS_OBJ|4)  // ��ʾ�ļ���Ŀ¼��
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
struct obj
{
    char *name;             // ���������������ļ�ϵͳΪ�ļ�����Ŀ¼���������豸��
                            //�豸��������gui���Ǵ�������
    ptu32_t ObjPrivate;     // ����˽�����ݣ�������һ������Ҳ����ָ����������Ľṹ��
    fnObjOps ops;           // ���󷽷���������Ĳ�����
    u32 inuse;              // ���û�ͬʱ���ü���
//  union __rights rights;  // ����Ȩ�޹���stat.h�� S_IRUSR ��
    list_t handles;         // �������������󱻴򿪣�ϵͳ����û�����һ�������
                            // ��������ݽṹ���������
    struct obj *prev,*next, *parent, *child; // �����ϵ��������������
//  struct obj *seton;      // �����ϵ��ĳ���µĶ��󼯺�(��)������ĳ���ɶ���֮�ϣ�
//                          //�γ�һ���ջ����������������ļ�ϵͳ��mount��ĳ����ԭ
//                          //�ڵ������ﱸ�ݣ��Ա�unmountʱ�ָ��ɶ���-1����Ϊ�ر�
//                          //�ô�����ʾ�ö���֮�ϲ��ɽ����µĶ��󼯺ϣ��ࣩ��
//  struct obj *set;        // �����ϵ�����ϵ㣨�ࣩ�������������Ƕ��󼯺ϣ��ࣩ��
//                          //��ָ���Լ���������ָ�����ļ��ϵ㣨�ࣩ��
};

struct objhandle;
ptu32_t obj_GetPrivate(struct obj *ob);
void obj_SetPrivate(struct obj *ob, ptu32_t Private);
fnObjOps obj_GetOps(struct obj *ob);
s32 obj_SetOps(struct obj *ob, fnObjOps ops);
const char *obj_name(struct obj *ob);
struct obj *obj_parent(struct obj *ob);
struct obj *obj_child(struct obj *ob);
struct obj *obj_prev(struct obj *ob);
struct obj *obj_next(struct obj *ob);
struct obj *obj_head(struct obj *ob);
s32 obj_level(struct obj *ob);
s32 obj_ishead(struct obj *ob);
s32 obj_islast(struct obj *ob);
s32 obj_isonduty(struct obj *ob);
struct objhandle* obj_TraveFile(struct objhandle *Current, struct obj *Object);
s32 obj_LinkHandle(struct objhandle *hdl, struct obj *ob);
bool_t obj_lock(void);
void obj_unlock(void);

u32 obj_InuseUp(struct obj *ob);
void obj_InuseUpRange(struct obj *start, struct obj *end);
void obj_InuseUpFullPath(struct obj *Obj);
u32 obj_InuseDown(struct obj *ob);
void obj_InuseDownRange(struct obj *start, struct obj *end);
void obj_InuseDownFullPath(struct obj *Obj);

s32 obj_Delete(struct obj *ob);
struct obj *obj_detach(struct obj *branch);
s32 obj_checkname(const char *name);
struct obj *obj_matchpath(const char *match, char **left);
struct obj *obj_buildpath(struct obj *begin, fnObjOps ops,
                            ptu32_t ObjPrivate, char *path);
s32 obj_releasepath(struct obj *start);
struct obj *obj_current(void);
void obj_setcurrent(struct obj *ob);
struct obj *obj_root(void);
struct obj *obj_newprev(struct obj *loc, fnObjOps ops,
                        ptu32_t ObjPrivate, const char *name);
struct obj *obj_newnext(struct obj *loc, fnObjOps ops,
                        ptu32_t ObjPrivate, const char *name);
struct obj *obj_newchild(struct obj *parent, fnObjOps ops,
                         ptu32_t ObjPrivate, const char *name);
struct obj *obj_newhead(struct obj *loc,fnObjOps ops,
                           ptu32_t ObjPrivate, const char *name);
s32 obj_move2last(struct obj *ob);
s32 obj_move2head(struct obj *ob);
s32 obj_insert2next(struct obj *loc, struct obj *next);
s32 obj_insert2prev(struct obj *loc, struct obj *prev);
s32 obj_child_move2prev(struct obj *parent);
s32 obj_child_move2next(struct obj *parent);
struct obj *obj_foreach_child(struct obj *parent, struct obj *child);
struct obj *obj_foreach_scion(struct obj *ancester, struct obj *scion);
struct obj *obj_search_sibling(struct obj *ob, const char *name);
struct obj *obj_search_child(struct obj *parent, const char *name);
struct obj *obj_search_scion(struct obj *ancester, const char *name);
struct obj *obj_search_path(struct obj *start, const char *path);
s32 obj_SetMultiplexEvent(struct obj *ob, u32 events);
s32 obj_ClrMultiplexEvent(struct obj *ob, u32 events);
s32 issocketactive(s32 Fd, s32 mode);

#ifdef __cplusplus
}
#endif

#endif //__OBJECT_H__

