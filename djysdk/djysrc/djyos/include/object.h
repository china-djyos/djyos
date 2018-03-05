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
#ifndef __rsc_h__
#define __rsc_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CN_RSC_NAME_LIMIT   255

typedef struct Object
{
    struct Object *Next,*Previous,*Parent,*Child;
    u16  Size;      //����node�����ݽṹ�ĳߴ磬���ڵ���
    u16  Type;      //��������
    u32  Inuse;     //ʹ�ü���
    char *Name;     //��Դ��,�������ļ�ϵͳΪ�ļ�����Ŀ¼��,�����豸���豸��
                    //����gui���Ǵ�����.
}TagObject;

//
// �ļ�ϵͳʹ��
//
struct VCommon
{
    struct Object Obj;
    void *Context; // ָ��struct FileContext��struct VMount
    char Name[];
};

//�ļ�ϵͳ������
#define OB_MOUNT                (0x0800) // ��װ����
#define OB_FILE                 (0x8000) // �ļ�����
#define OB_REG                  (0x9000) // �ļ�
#define OB_DIR                  (0xA000) // Ŀ¼
#define OB_LINK                 (0xC000) // ����

//������Դ����
#define RSC_RSCNODE             0       //���ڵ�
#define RSC_RSCV_TYPE           1       //��������
#define RSC_DEVICE              2       //�豸
#define RSC_SEMP                3       //�ź���
#define RSC_MUTEX               4       //������
#define RSC_MSGQ                5       //��Ϣ����
#define RSC_MEMPOOL             6       //�ڴ��
#define RSC_SOFT_TIMER          7       //��ʱ��
#define RSC_SOCKET              8       //socket
#define RSC_NET_CARD            9        //����
#define RSC_SPIBUS              10       //spi����
#define RSC_SPI_DEVICE          11       //spi����
#define RSC_IICBUS              12       //i2c����
#define RSC_IIC_DEVICE          13       //i2c����
#define RSC_STDIN_OUT           14       //��׼���������豸
#define RSC_SHELL_CMD           15       //shell ����
#define RSC_DISPLAY             16       //��ʾ��
#define RSC_GKWIN               17       //�ں˴���(��ʾ)
#define RSC_FILE                18       //�ļ����ļ���
#define RSC_FILE_SYS            19       //�ļ�ϵͳ(ָ�����ļ�ϵͳ)
#define RSC_PTT                 20       //�ļ�ϵͳ����
#define RSC_FFS_FLASH_CHIP      21       //����flash�ļ�ϵͳ��flashоƬ
#define RSC_EFS_NOR_CHIP        22       //����easy nor�ļ�ϵͳ��flashоƬ
#define RSC_CHARSET             23       //�ַ���
#define RSC_FONT                24       //����


#define RSCTYPE_USER_BASE       0x80
#define RSCTYPE_USER            (RSCTYPE_USER_BASE + 0)

ptu32_t ModuleInstall_OBJ(ptu32_t Para);
//ptu32_t Rsc2_ModuleInit(ptu32_t para);
void OBJ_Clean(struct Object *Obj);
struct Object *OBJ_AddTree(struct Object *Obj, u16 Size, u16 RscType, const char *Name);
struct Object *OBJ_AddToPrevious(struct Object *Obj,struct Object *NewObj,
                                u16 Size, u16 RscType, const char *Name);
struct Object *OBJ_AddToNext(struct Object *Obj,struct Object *NewObj,
                             u16 Size, u16 RscType, const char *Name);
struct Object *OBJ_AddChild(struct Object *Parent, struct Object *Child,
                            u16 Size, u16 RscType, const char *Name);
struct Object *OBJ_AddChildHead(struct Object *Parent, struct Object *Child,
                                u16 Size, u16 RscType, const char *Name);
struct Object *OBJ_InsertChild(struct Object *Parent, struct Object *New);
bool_t OBJ_Displace(struct Object *Old, struct Object *New);
struct Object *OBJ_DelBranch(struct Object *Branch);
struct Object *OBJ_Del(struct  Object *Obj);
bool_t OBJ_MoveToTree(struct Object *Parent,struct Object *Node);
bool_t OBJ_MoveToLast(struct Object *Obj);
bool_t OBJ_MoveToHead(struct Object *Obj);
bool_t OBJ_MoveToNext(struct Object *Loc, struct Object *Obj);
bool_t OBJ_MoveToPrevious(struct Object *Loc, struct Object *Obj);
bool_t OBJ_RoundPrevious(struct Object *Parent);
bool_t OBJ_RoundNext(struct Object *Parent);
bool_t OBJ_Rename(struct Object *Obj, const char *NewName);
struct Object *OBJ_GetTree(struct Object *Obj);
struct Object *OBJ_SearchTree(const char *Name);
struct Object *OBJ_SysRoot(void);
char *OBJ_Name(struct Object *Obj);
u16 OBJ_Size(struct Object *Obj);
u16 OBJ_Type(struct Object *Obj);
struct Object *OBJ_Parent(struct Object *Obj);
struct Object *OBJ_Child(struct Object *Obj);
struct Object *OBJ_Previous(struct Object *Obj);
struct Object *OBJ_Next(struct Object *Obj);
struct Object *OBJ_GetHead(struct Object *Obj);
struct Object *OBJ_GetTwig(struct Object *Obj);
u32 OBJ_GetLevel(struct Object *Obj);
struct Object *OBJ_TraveChild(struct Object *Parent, struct Object *Child);
struct Object *OBJ_TraveScion(struct Object *Ancestor, struct Object *Current);
struct Object *OBJ_SearchSibling(struct Object *Brother, const char *Name);
struct Object *OBJ_SearchChild(struct Object *Parent, const char *Name);
struct Object *OBJ_SearchScion(struct Object *Ancestor, const char *Name);
struct Object *OBJ_Search(struct Object *Start, const char *Path);
u32 OBJ_Sequencing(struct Object *Obj);
bool_t OBJ_IsLast(struct Object *Obj);
bool_t OBJ_IsHead(struct Object *Obj);
struct Object *__RootObj(void);
#ifdef __cplusplus
}
#endif

#endif //__rsc_h__

