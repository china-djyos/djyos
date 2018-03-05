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
//�ļ�����: ��������ӡ��ƶ���ɾ����������Դ�ڵ�ķ���
//����˵��:
//�޶���ʷ:
//4. ����:20130220
//   ����:  ������.
//   �°汾�ţ�v1.3.0
//   �޸�˵��:
//      1.������rsc�������ܵ������������⣬bug������ë���ύ�ġ�
//3. ����:20090518
//   ����:  ������.
//   �°汾�ţ�v1.2.0
//   �޸�˵��:
//      1.�ڸ����ϣ��ֿ�����������Դ����������Ҳ��Ӧ�ص�����
//      2.����rsc_get_root������ԭrsc_get_root����Ϊrsc_get_tree���Ҳ��ټ�ְ
//      3.������rsc_get_root��ȡ��1.1.0�汾��"�޸�3"
//      4.��ȫ��NULL�������
//2. ����:20090131
//   ����:  ������.
//   �°汾�ţ�v1.1.0
//   �޸�˵��:
//      1.���˸��ܸ��ڵ㣬�����ģ���ʼ������
//      2.���ź��������������ʰ�ȫ
//      3.�Ľ��˱���������������NULL���������������
//      4.������һЩbug
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stddef.h"
#include "string.h"
#include "int.h"
#include "object.h"
#include "lock.h"
//-------------��Դ���㷨˵��----------------------
//��ͬ���ڵ�����˫��ѭ������
//ÿ���ڵ��childָ��ֱ��ָ��Ľڵ��Ϊ�ýڵ�����ڶ��еĶ���ͷ������previous�ڵ�
//�Ƕ���β��
//���нڵ��Parentָ���ָ�򸸽ڵ�

//�ر���ʾ����Դ���еĲ������ʰ�ȫ����Դ����ģ�鸺����Դ�ڵ�����Ĳ�������
//          ������ʹ���߸���
//��������޸Ķ���ʹ��ԭ�Ӳ�����֤������걸��
//������Ķ�������ʹ���κΰ�ȫ���ƣ�����Դ����ģ������֤��Դ�����İ�ȫ��

//
//
//
#define ROOT_FILENAME       "/"
#define SYS_FILENAME        "sys"

//static struct Object s_tRscRoot;
static struct Object *s_ptRscRoot;
//static struct MutexLCB s_tMutexRscRoot;

//
// ���ڵ�
//
struct VCommon g_tRootCom =
{
    .Context = NULL,
    .Name = ROOT_FILENAME,
};
//
// ϵͳ�ڵ�
//
struct VCommon g_tSysCom =
{
    .Context = NULL,
    .Name = SYS_FILENAME,
};

//-----------------------------------------------------------------------------
//����:
//����:
//����: ��NULL -- �ɹ�; NULL -- ʧ��;
//��ע: ��RSC������ʹ�õĺ����ӿ�,ע���ʱϵͳ����δ����
//-----------------------------------------------------------------------------
struct Object *RootObjInit(void)
{

    // object��ϵ����ʼ��
    g_tRootCom.Obj.Type = RSC_RSCNODE;
    g_tRootCom.Obj.Size = sizeof(struct VCommon) + sizeof(ROOT_FILENAME);//+"/"�ַ����ռ�
    g_tRootCom.Obj.Name = g_tRootCom.Name;
    g_tRootCom.Obj.Inuse = 0;
    g_tRootCom.Obj.Child = g_tRootCom.Obj.Parent = NULL;
    g_tRootCom.Obj.Previous = g_tRootCom.Obj.Next = &g_tRootCom.Obj;// ԭRSC�߼�


    // "/sys"��ϵ��ʼ��
    g_tSysCom.Obj.Type = RSC_RSCNODE;
    g_tSysCom.Obj.Size = sizeof(struct VCommon) + sizeof(SYS_FILENAME);// +"sys"�ַ����ռ�
    g_tSysCom.Obj.Name = g_tSysCom.Name;
    g_tSysCom.Obj.Inuse = 0;
    g_tSysCom.Obj.Parent = &g_tRootCom.Obj;
    g_tRootCom.Obj.Child = &g_tSysCom.Obj;
    g_tSysCom.Obj.Child = NULL;
    g_tSysCom.Obj.Previous = g_tSysCom.Obj.Next = &g_tSysCom.Obj;// ԭRSC�߼�

    return (&g_tSysCom.Obj);// ϵͳ�е��������������"/sys"��
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע: �ڲ����ã������ţ�Ϊ�˽����ݽṹ����
//-----------------------------------------------------------------------------
struct Object *__RootObj(void)
{
    return (&(g_tRootCom.Obj));
}
//----��ʼ����Դ����ģ��-------------------------------------------------------
//����: ������Դ�ڵ���ڣ������������ѳ��������ڶ�������������ִ��������ʼ����
//      ���ܵ��ã���һ�������������ʼ��ִ��ǰ����
//����: ��
//����: 1
//------------------------------------------------------------------------------
ptu32_t ModuleInstall_OBJ(ptu32_t Para)
{
    Para = Para;        //�����������澯
#if 0
    s_tRscRoot.Name = "resouce queue";
    s_tRscRoot.Child = NULL;
    s_tRscRoot.Next =&s_tRscRoot;
    s_tRscRoot.Previous =&s_tRscRoot;
    s_tRscRoot.Parent = NULL;
    s_tRscRoot.Size = sizeof(s_tRscRoot);
    s_tRscRoot.Type = RSC_RSCNODE;
    s_ptRscRoot = &s_tRscRoot;
#else
    s_ptRscRoot = RootObjInit();
#endif
    return 1;
}
//ptu32_t Rsc2_ModuleInit(ptu32_t para)
//{
//    para = para;        //�����������澯
//    Lock_MutexCreate_s(&s_tMutexRscRoot,"mutex for resouce queue");
//    return 1;
//}
//----��Ӹ��ڵ�----------------------------------------------------------------
//����: ����Դ�������һ�����ڵ�(�ع���ģ�飬���ʱ����δ��ʼ����)
//����: Obj,����ӵĽڵ�ָ��
//      size���½ڵ������صĳߴ磬�ֽ���
//      name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľڵ�
//------------------------------------------------------------------------------
struct Object *__Lock_RscAddLockTree(struct Object *Obj,
                                     u16 Size, const char *Name)
{
    struct Object *root_node;

    if(Obj == NULL)
        return NULL;

    Obj->Parent = s_ptRscRoot;
    Obj->Child = NULL;
    Obj->Size = Size;
    Obj->Type = RSC_RSCNODE;
    Obj->Name = (char*)Name;
    if(s_ptRscRoot->Child == NULL)
    {
        s_ptRscRoot->Child = Obj;
        Obj->Next = Obj;
        Obj->Previous = Obj;
    }
    else
    {
        root_node = s_ptRscRoot->Child;
        Obj->Next = root_node;
        Obj->Previous = root_node->Previous;
        root_node->Previous->Next = Obj;
        root_node->Previous = Obj;
    }
    return Obj;
}

//----��ʼ���ڵ�----------------------------------------------------------------
//����: ��һ���ڵ������ָ�����㣬������ڵ㸺��size������ָ��
//����: Obj,����ӵĽڵ�ָ��
//����: ��
//------------------------------------------------------------------------------
void OBJ_Clean(struct Object *Obj)
{
    if(Obj)
    {
        Obj->Child = NULL;
        Obj->Parent = NULL;
        Obj->Next = NULL;
        Obj->Previous = NULL;
    }
}

//----��Ӹ��ڵ�----------------------------------------------------------------
//����: ����Դ�������һ�����ĸ��ڵ�
//����: Obj,����ӵĽڵ�ָ��
//      Size���½ڵ������صĳߴ磬�ֽ���
//      RscType, ��Դ����,�������ֲ�ͬ��Դ,ԭ����,��Դ�����ݽṹ��ͬ,���;Ͳ�ͬ
//      Name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľڵ�
//��ע: �޸��߼�,��ԭ�����ص����µ��齨(/dev��֧����),ȫ�������½���/SYS��
//------------------------------------------------------------------------------
struct Object *OBJ_AddTree(struct Object *Obj, u16 Size,
                           u16 RscType, const char *Name)
{
    struct Object *root_node;
    atom_low_t low_atom;

    if(Obj == NULL)
        return NULL;

    if(OBJ_SearchTree(Name) != NULL)
        return NULL;

    Obj->Parent = s_ptRscRoot;
    Obj->Child=NULL;
    Obj->Size = Size;
    Obj->Type = RscType;
    Obj->Name = (char *)Name;
    low_atom = Int_LowAtomStart();
    //��Դ���������ٴ���һ���ź����ڵ㣬�����ж��Ƿ��
    root_node = s_ptRscRoot->Child;
    Obj->Next = root_node;
    Obj->Previous = root_node->Previous;
    root_node->Previous->Next = Obj;
    root_node->Previous = Obj;
    Int_LowAtomEnd(low_atom);
    return Obj;
}

//----����ڵ�------------------------------------------------------------------
//����: ����Դ�����в���һ���ڵ㣬�½ڵ���ԭ�ڵ��previousλ��
//����  Obj,�ڴ˽ڵ�ǰ�����ڵ�
//      NewObj,��������½ڵ�
//      Size���½ڵ������صĳߴ磬�ֽ���
//      RscType, ��Դ����,�������ֲ�ͬ��Դ,ԭ����,��Դ�����ݽṹ��ͬ,���;Ͳ�ͬ
//      Name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľڵ�
//------------------------------------------------------------------------------
struct Object *OBJ_AddToPrevious(struct Object *Obj, struct Object *NewObj,
                                 u16 Size, u16 RscType, const char *Name)
{
    atom_low_t low_atom;

    if((Obj == NULL) || (NewObj == NULL))
        return NULL;

    low_atom = Int_LowAtomStart();
    NewObj->Next = Obj;
    NewObj->Previous = Obj->Previous;
    NewObj->Parent = Obj->Parent;
    NewObj->Child = NULL;
    NewObj->Size = Size;
    NewObj->Type = RscType;
    NewObj->Name = (char *)Name;
    Obj->Previous->Next = NewObj;
    Obj->Previous = NewObj;
    Int_LowAtomEnd(low_atom);
    return NewObj;
}

//----���ӽڵ�------------------------------------------------------------------
//����: ����Դ����������һ���ڵ㣬�½ڵ���ԭ�ڵ��nextλ��
//����  Obj,�ڴ˽ڵ�������ڵ�
//      NewObj,��������½ڵ�
//      Size���½ڵ������صĳߴ磬�ֽ���
//      RscType, ��Դ����,�������ֲ�ͬ��Դ,ԭ����,��Դ�����ݽṹ��ͬ,���;Ͳ�ͬ
//      Name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľڵ�
//------------------------------------------------------------------------------
struct Object *OBJ_AddToNext(struct Object *Obj, struct Object *NewObj,
                             u16 Size, u16 RscType, const char *Name)
{
    atom_low_t   low_atom;

    if((Obj == NULL) || (NewObj == NULL))
        return NULL;

    low_atom = Int_LowAtomStart();
    NewObj->Previous = Obj;
    NewObj->Next = Obj->Next;
    NewObj->Parent = Obj->Parent;
    NewObj->Child = NULL;
    NewObj->Size = Size;
    NewObj->Type = RscType;
    NewObj->Name = (char *)Name;
    Obj->Next->Previous = NewObj;
    Obj->Next = NewObj;
    Int_LowAtomEnd(low_atom);
    return NewObj;
}

//----�����ӽڵ�----------------------------------------------------------------
//����: ��ָ���ڵ�����һ���ӽڵ�,�½ڵ㽫�ڶ���βλ��
//����  Parent,�½ڵ�ĸ��ڵ�
//      Child,��������½ڵ�
//      Size���½ڵ������صĳߴ磬�ֽ���
//      RscType, ��Դ����,�������ֲ�ͬ��Դ,ԭ����,��Դ�����ݽṹ��ͬ,���;Ͳ�ͬ
//      Name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľڵ�
//------------------------------------------------------------------------------
struct Object *OBJ_AddChild(struct Object *Parent, struct Object *Child,
                            u16 Size, u16 RscType, const char *Name)
{
    atom_low_t low_atom;
    struct Object *p;

    if((Parent == NULL) || (Child == NULL))
        return NULL;

    low_atom = Int_LowAtomStart();
    Child->Size = Size;
    Child->Type = RscType;
    Child->Name = (char *)Name;
    Child->Child = NULL;
    if(Parent->Child == NULL)
    {
        Parent->Child = Child;
        Child->Parent = Parent;
        Child->Next = Child;
        Child->Previous = Child;
    }
    else
    {
        p = Parent->Child;
        Child->Next = p;
        Child->Previous = p->Previous;
        Child->Parent = Parent;
        p->Previous->Next = Child;
        p->Previous = Child;
    }
    Child->Inuse = 0;
    Int_LowAtomEnd(low_atom);
    return Child;
}

//----����һ��ͷ�ڵ�---------------------------------------------------------
//����: ��ָ���ڵ�����һ���ӽڵ�,�½ڵ㽫�ڶ���ͷλ��
//����  Parent,�½ڵ�ĸ��׽ڵ�
//      Child,��������½ڵ�
//      Size���½ڵ������صĳߴ磬�ֽ���
//      RscType, ��Դ����,�������ֲ�ͬ��Դ,ԭ����,��Դ�����ݽṹ��ͬ,���;Ͳ�ͬ
//      Name����Դ���֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//����: �¼���Ľڵ�
//------------------------------------------------------------------------------
struct Object *OBJ_AddChildHead(struct Object *Parent, struct  Object *Child,
                                u16 Size, u16 RscType, const char *Name)
{
    atom_low_t low_atom;
    struct Object *p;

    if((Parent == NULL) || (Child == NULL))
        return NULL;
    low_atom = Int_LowAtomStart();
    Child->Size = Size;
    Child->Type = RscType;
    Child->Name = (char *)Name;
    Child->Child = NULL;
    if(Parent->Child == NULL)
    {
        Parent->Child = Child;
        Child->Parent = Parent;
        Child->Next = Child;
        Child->Previous = Child;
    }
    else
    {
        p = Parent->Child;
        Child->Next = p;
        Child->Previous = p->Previous;
        Child->Parent = Parent;
        p->Previous->Next = Child;
        p->Previous = Child;
        Parent->Child = Parent->Child->Previous;
    }
    Int_LowAtomEnd(low_atom);
    return Child;
}

//----�����ӽڵ�---------------------------------------------------------------
//���ܣ���һ��������뵽��һ��������ӽڵ�λ�ã��½ڵ㽫λ�ڶ���ͷλ�ã�ע�⣬
//      �½ڵ�������ӽڵ㡣
//������Parent�����ڵ�
//      New����������ӽڵ㣬�������¼��ڵ㡣
//���أ��½ڵ�ָ��
//-----------------------------------------------------------------------------
struct Object *OBJ_InsertChild(struct Object *Parent, struct Object *New)
{
    struct Object *p;
    atom_low_t low_atom;
    if( (Parent == NULL) || (New == NULL))
        return NULL;
    New->Parent = Parent;
    low_atom = Int_LowAtomStart();
    if(Parent->Child == NULL)
    {
        Parent->Child = New;
        New->Next = New;
        New->Previous = New;
    }
    else
    {
        p = Parent->Child;
        New->Next = p;
        New->Previous = p->Previous;
        p->Previous->Next = New;
        p->Previous = New;
        Parent->Child = New;
    }
    Int_LowAtomEnd(low_atom);
    return New;
}

//----�滻�ڵ�-----------------------------------------------------------------
//����: ��һ���½ڵ��滻����Դ�����е�ԭ�нڵ㣬�½ڵ�ԭ�����벻�Ƕ����еĽڵ�
//����: Old�����滻�Ľڵ�
//      New���½ڵ�
//����: ��
//-----------------------------------------------------------------------------
bool_t OBJ_Displace(struct Object *Old, struct Object *New)
{
    struct Object *temp1, *temp2;
    atom_low_t low_atom;

    if((NULL == Old) || (NULL == New))
        return FALSE;

    low_atom = Int_LowAtomStart();
    temp1 = Old->Child;
    if(temp1 != NULL)   //��oldnode�����ӽڵ�ĸ�ָ��ָ��newnode
    {
        temp2 = temp1;
        do
        {
            temp2->Parent = New;
            temp2 = temp2->Next;
        }while(temp2 != temp1);
    }
    New->Child = temp1;
    New->Next = Old->Next;
    New->Previous = Old->Previous;
    New->Parent = Old->Parent;
    Old->Next->Previous = New;
    Old->Previous->Next = New;
    if(Old->Parent->Child == Old) //����Ƕ���ͷ�ڵ�
        Old->Parent->Child = New; //���ڵ���ӽڵ�ָ�����ͷ�ڵ�
    Int_LowAtomEnd(low_atom);

    return TRUE;
}

//----ɾ��һ����֦-------------------------------------------------------------
//����: ��һ����֦����Դ������ɾ��
//����: Branch����ɾ���ķ�֧��
//����: ��ɾ����ָ֧�룬NULL��ʾ��֧������
//-----------------------------------------------------------------------------
struct Object *OBJ_DelBranch(struct Object *Branch)
{
    struct Object *result;
    atom_low_t low_atom;

    if(Branch == NULL)
        return NULL;

    low_atom = Int_LowAtomStart();
    if(Branch->Next == Branch)   //˵���ýڵ�û���ֵܽڵ�.
    {
        Branch->Parent->Child = NULL;
    }
    else
    {
        if(Branch->Parent->Child == Branch)
        {   //˵���ýڵ��Ƕ���ͷ�ڵ�,��Ҫ�ı����ͷ�ڵ�
            Branch->Parent->Child = Branch->Next;
        }
        Branch->Previous->Next = Branch->Next;
        Branch->Next->Previous = Branch->Previous;
    }
    result = Branch;
    Int_LowAtomEnd(low_atom);

    return result;
}
//---ɾ��һ���ڵ�---------------------------------------------------------------
//����: ��һ���ڵ����Դ�����жϿ��ڵ�,�ýڵ㲻�����ӽڵ�
//����: Obj,��ɾ���Ľڵ�,��ýڵ����ӽڵ���ɾ��
//����: ���ر�ɾ���ڵ�ָ�룬�����򷵻�NULL
//------------------------------------------------------------------------------
struct Object *OBJ_Del(struct Object *Obj)
{
    atom_low_t low_atom;
    struct Object *result;

    if(Obj == NULL)
        return NULL;

    low_atom = Int_LowAtomStart();
    if(Obj->Child != NULL) //�ӽڵ�ǿ�,������
        result = NULL;
    else
    {
        if(Obj->Next == Obj)   //˵���ýڵ�û���ֵܽڵ�.
        {
            Obj->Parent->Child = NULL;
        }
        else
        {
            if(Obj->Parent->Child == Obj)
            {   //˵���ýڵ��Ƕ���ͷ�ڵ�,��Ҫ�ı����ͷ�ڵ�
                Obj->Parent->Child = Obj->Next;
            }
            Obj->Previous->Next = Obj->Next;
            Obj->Next->Previous = Obj->Previous;
        }
        result = Obj;
    }
    Int_LowAtomEnd(low_atom);

    return result;
}

//---�ƶ�һ����---------------------------------------------------------------
//����: �ƶ�һ��������֦����Ľڵ������Ϊ������
//������Parent���¸��ڵ�
//      Obj����������ӽڵ㣬�������¼��ڵ㡣
//����: TRUE = �ɹ�ִ�У�FALSE = ʧ��
//-----------------------------------------------------------------------------
bool_t OBJ_MoveToTree(struct Object *Parent,struct  Object *Obj)
{
    atom_low_t low_atom;
    struct Object *p;
    if( (Parent == NULL) || (Obj == NULL))
        return false;
    low_atom = Int_LowAtomStart();
    OBJ_DelBranch(Obj);
    Obj->Parent = Parent;
    if(Parent->Child == NULL)
    {
        Parent->Child = Obj;
        Obj->Next = Obj;
        Obj->Previous = Obj;
    }
    else
    {
        p = Parent->Child;
        Obj->Next = p;
        Obj->Previous = p->Previous;
        p->Previous->Next = Obj;
        p->Previous = Obj;
        Parent->Child = Obj;
    }
    Int_LowAtomEnd(low_atom);
    return true;
}

//----�ƶ��ڵ㵽���---------------------------------------------------------
//����: ��nextָ�뷽���ƶ���Դ�����е�һ���ڵ㣬��ͬ������ͷ��ǰһ���ڵ�λ��
//����: Obj,���ƶ��Ľڵ�ָ��
//����: ��
//-----------------------------------------------------------------------------
bool_t OBJ_MoveToLast(struct Object *Obj)
{
    struct Object *eldest;
    atom_low_t low_atom;

    if(Obj == NULL)
        return FALSE;

    low_atom = Int_LowAtomStart();
    if(Obj->Parent != NULL)    //���ڵ㲻���ƶ�
    {
        eldest = Obj->Parent->Child;
        if(eldest == Obj)
        //���node��ͷ�ڵ�,��ֱ���ƶ����ڵ����ָ�뵽��һ���ڵ�Ϳ�����.
            Obj->Parent->Child = Obj->Next;
        //���´�������ȡ���ڵ�
        Obj->Next->Previous = Obj->Previous;
        Obj->Previous->Next = Obj->Next;
        //���°�node�������βλ��,������ѭ������,ͷ�ڵ��ǰ�����β�ڵ�.
        Obj->Next = eldest;
        Obj->Previous = eldest->Previous;
        eldest->Previous->Next = Obj;
        eldest->Previous = Obj;
    }
    Int_LowAtomEnd(low_atom);

    return TRUE;
}

//----�ƶ��ڵ��Ϊ����ͷ-------------------------------------------------------
//����: ��previousָ�뷽���ƶ���Դ�����е�һ���ڵ㣬��Ϊͬ������ͷ
//����: Obj,���ƶ��Ľڵ�ָ��
//����: TRUE = �ɹ�ִ�У�FALSE = ʧ��
//------------------------------------------------------------------------------
bool_t OBJ_MoveToHead(struct Object *Obj)
{
    atom_low_t low_atom;

    if(Obj == NULL)
        return FALSE;
    low_atom = Int_LowAtomStart();
    if(Obj->Parent)    //���ڵ㲻���ƶ�
    {
        OBJ_MoveToLast(Obj);
        Obj->Parent->Child = Obj;
    }
    Int_LowAtomEnd(low_atom);
    return TRUE;
}

//----�ƶ��ڵ㵽ĳ�ڵ�nextλ��-------------------------------------------------
//����: �ƶ���Դ�����е�һ���ڵ�NewNext����һ���ڵ�Obj��nextλ�ã����ƶ��Ľڵ�
//      �������ӽڵ�
//����: Loc��Ŀ��λ�ã�Obj�ƶ������ڵ����
//      Obj,Ŀ��ڵ�,������ԭ������Object������
//����: TRUE = �ɹ�ִ�У�FALSE = ʧ��
//------------------------------------------------------------------------------
bool_t OBJ_MoveToNext(struct Object *Loc, struct Object *Obj)
{
    atom_low_t low_atom;

    if((Loc == NULL) || (Obj == NULL) || (Loc == Obj))
        return FALSE;
    low_atom = Int_LowAtomStart();
    if(Loc->Parent != Obj->Parent)
    {
        OBJ_DelBranch(Obj);
    }
    else
    {
        //���´�������ȡ���ڵ�
        Obj->Next->Previous = Obj->Previous;
        Obj->Previous->Next = Obj->Next;
    }
    Obj->Previous = Loc;
    Obj->Next = Loc->Next;
    Loc->Next->Previous = Obj;
    Loc->Next = Obj;
    Obj->Parent = Loc->Parent;
    Int_LowAtomEnd(low_atom);

    return TRUE;
}

//----�ƶ��ڵ㵽ĳ�ڵ�previousλ��---------------------------------------------
//����: �ƶ���Դ�����е�һ���ڵ�NewPre����һ���ڵ�Obj��previousλ�ã����ƶ��Ľڵ�
//      �������ӽڵ�
//����: Loc��Ŀ��λ�ã�Obj�ƶ������ڵ�ǰ��
//      Obj,Ŀ��ڵ㣬��ԭ������Object�����С�
//����: TRUE = �ɹ�ִ�У�FALSE = ʧ��
//------------------------------------------------------------------------------
bool_t OBJ_MoveToPrevious(struct Object *Loc, struct Object *Obj)
{
    atom_low_t low_atom;

    if((Loc == NULL) || (Obj == NULL) || (Loc == Obj))
        return FALSE;

    low_atom = Int_LowAtomStart();
    //���´�������ȡ���ڵ�
    if(Loc->Parent != Obj->Parent)
    {
        OBJ_DelBranch(Obj);
    }
    else
    {
        Obj->Next->Previous = Obj->Previous;
        Obj->Previous->Next = Obj->Next;
    }
    Obj->Next = Loc;
    Obj->Previous = Loc->Previous;
    Loc->Previous->Next = Obj;
    Loc->Previous = Obj;
    Obj->Parent = Loc->Parent;
    Int_LowAtomEnd(low_atom);

    return TRUE;
}

//----����ͷλ�ú���-----------------------------------------------------------
//����: Parent���ӽڵ�����λ�ò���,����ͷ��previous�����ƶ�һ��
//����: Parent,���ڵ�ָ��
//����: TURE = �ɹ�ִ�У�FALSE = ʧ��
//------------------------------------------------------------------------------
bool_t OBJ_RoundPrevious(struct Object *Parent)
{
    atom_low_t low_atom;

    if(Parent == NULL)
        return FALSE;

    low_atom = Int_LowAtomStart();
    if(Parent->Child != NULL)
    {
        Parent->Child = Parent->Child->Previous;
    }
    Int_LowAtomEnd(low_atom);

    return TRUE;
}

//----����ͷλ��ǰ��-----------------------------------------------------------
//����: Parent���ӽڵ�����λ�ò���,����ͷ��next�����ƶ�һ��
//����: Parent,���ڵ�ָ��
//����: TRUE = �ɹ�ִ�У�FALSE = ʧ��
//------------------------------------------------------------------------------
bool_t OBJ_RoundNext(struct Object *Parent)
{
    atom_low_t low_atom;

    if(Parent == NULL)
        return FALSE;

    low_atom = Int_LowAtomStart();
    if(Parent->Child != NULL)
    {
        Parent->Child = Parent->Child->Next;
    }
    Int_LowAtomEnd(low_atom);

    return TRUE;
}

//----��Դ����-----------------------------------------------------------------
//����: �޸���Դ��
//����: Obj,���޸ĵ���Դ�ڵ�ָ��
//      NewName,������
//����: TRUE = �ɹ�ִ�У�FALSE = ʧ��
//-----------------------------------------------------------------------------
bool_t OBJ_Rename(struct Object *Obj, const char *NewName)
{
    if((Obj == NULL) || (NewName == NULL))
        return FALSE;

    Obj->Name = (char *)NewName;  //�����ж��������ַ����ĳ��ȣ���Դ���������ǿմ�

    return TRUE;
}

//----��ȡ�����ڵ�-------------------------------------------------------------
//����: ����ָ���ڵ����ڵ����ĸ��ڵ�ָ��
//����: Obj,Ŀ��ڵ㡣
//����: ���ڵ�ָ��
//----------------------------------------------------------------------------
struct Object *OBJ_GetTree(struct Object *Obj)
{
    atom_low_t low_atom;
    struct Object *node = Obj;

    if(node == NULL)    //Ŀ��ڵ��
        return NULL;

    low_atom = Int_LowAtomStart();
    while(node->Parent != s_ptRscRoot)
    {
        if(NULL != node->Parent)
            node = node->Parent;
        else
        {
            node = NULL;
            break;
        }
    }
    Int_LowAtomEnd(low_atom);
    return node;
}

//----��ȡ��Դ���ڵ�-----------------------------------------------------------
//����: ����������Դϵͳ�ĸ��ڵ�ָ��
//����: �ޡ�
//����: ��Դ��ָ��
//----------------------------------------------------------------------------
struct Object *OBJ_SysRoot(void)
{
    return s_ptRscRoot;
}

//----��ȡ�ڵ�����---------------------------------------------------------------
//����: ����ĳ�ڵ������,
//����: Obj,����������Դ�ڵ�
//����: �ýڵ�����֣�Ӧ����һ��char *��ָ��
//ע��: �ýڵ����û�����֣���˷��صļ�ʹ�ǳɹ�Ҳ����ΪNULL����Ϊȷʵû������
//      �ɹ�ʧ�ܲ�Ӧ�þݴ����ж�
//-----------------------------------------------------------------------------
char *OBJ_Name(struct Object *Obj)
{
    char *name;

    if(Obj == NULL)
        return NULL;

    name = Obj->Name;

    return name;
}

//----��ȡ�ڵ�ߴ�---------------------------------------------------------------
//����: ����ĳ�ڵ�����ݽṹ�ߴ�,�����ڵ㱾��,
//����: Obj,����������Դ�ڵ�
//����: �ýڵ�ĳߴ�
//-----------------------------------------------------------------------------
u16 OBJ_Size(struct Object *Obj)
{
    if(Obj == NULL)
        return 0;

    return Obj->Size;
}

//----��ȡ�ڵ�����---------------------------------------------------------------
//����: ����ĳ�ڵ������,
//����: Obj,����������Դ�ڵ�
//����: �ýڵ������
//-----------------------------------------------------------------------------
u16 OBJ_Type(struct Object *Obj)
{
    if(Obj == NULL)
        return 0;

    return Obj->Type;
}

//----��ȡ���ڵ�---------------------------------------------------------------
//����: ����ĳ�ڵ�ĸ��ڵ�,
//����: Obj,��Ҫ���ҵ���֦������һ���ӽڵ�
//����: ���ڵ�.
//-----------------------------------------------------------------------------
struct Object *OBJ_Parent(struct Object *Obj)
{
    struct Object *node;

    if(Obj == NULL)
        return NULL;

    node = Obj->Parent;

    return node;
}

//----��ȡ�ӽڵ�--------------------------------------------------------------
//����: �����ӽڵ㣬�򸸽ڵ��childָ��ָ���ӽڵ�Ķ���ͷ��ʵ�ʷ����ӽڵ�Ķ���ͷ
//����: Obj,��Ҫ���ҵ���֦�ĸ��ڵ�
//����: �ӽڵ����ͷ
//-----------------------------------------------------------------------------
struct Object *OBJ_Child(struct Object *Obj)
{
    struct Object *node;

    if(Obj == NULL)
        return NULL;

    node = Obj->Child;

    return node;
}

//----��ȡǰһ���ڵ�-----------------------------------------------------------
//����: ����Obj�ڵ��previous�ڵ�
//����: Obj,��Ҫ���ҵ���֦���ֽڵ��е�һ���ڵ�
//����: previous�ڵ�.
//-----------------------------------------------------------------------------
struct Object *OBJ_Previous(struct Object *Obj)
{
    struct Object *node;

    if(Obj == NULL)
        return NULL;

    node = Obj->Previous;

    return node;
}

//----��ȡ�ܽڵ�--------------------------------------------------------------
//����: ����Obj�ڵ��next�ڵ�
//����: Obj,��Ҫ���ҵ���֦���ֽڵ��е�һ���ڵ�
//����: next�ڵ�.
//-----------------------------------------------------------------------------
struct Object *OBJ_Next(struct Object *Obj)
{
    struct Object *node;

    if(Obj == NULL)
        return NULL;

    node = Obj->Next;

    return node;
}

//----��ȡ����ͷ---------------------------------------------------------------
//����: ����Obj�ڵ����ڶ��еĶ���ͷ
//����: Obj,��Ҫ���ҵ���֦������ڵ�
//����: ����ͷָ��.
//-----------------------------------------------------------------------------
struct Object *OBJ_GetHead(struct Object *Obj)
{
    struct Object *node;

    if(Obj == NULL)
        return NULL;

    node = Obj->Parent->Child;

    return node;
}

//----��ȡ��֦һ��ĩ�ҽڵ�-----------------------------------------------------
//����: ����ĳ��֦��һ��ĩ�ҽڵ�,ĩ�ҽڵ���ָû���ӽڵ�Ľڵ�.
//����: Obj,��Ҫ��������֦�ĸ��ڵ�
//����: ��֦��һ��ĩ�ҽڵ�.��û���ӽڵ�ʱ������NULL
//��ע: ����Ҫɾ��������֦ʱ,������������,���rsc_del_node����,�������ñ�����,
//      ���ѷ��صĽڵ�ɾ��,ֱ������������NULL.��Ҫɾ��һ���ļ��л���ɾ��һ����
//      ���Ӵ��ڵ�gui����ʱ,����Ҫ�õ�ɾ��������֦�Ĳ���.
//-----------------------------------------------------------------------------
struct Object *OBJ_GetTwig(struct Object *Obj)
{
    struct Object *result = NULL, *current;

    if(Obj == NULL)
        return NULL;

    current = Obj;
    while(current->Child != NULL)
    {
        current = current->Child;
    }
    if(current == Obj)
        result = NULL;
    else
        result = current;

    return result;
}

//----ȡ�ڵ�ļ���-----------------------------------------------------
//����: �鿴ĳ�ڵ��Ǹ��ڵ�ĵڼ����ӽڵ㣬
//����: Obj,����Ľڵ�
//����: �ӽڵ㼶����0������ڵ㡣cn_limit_uint32�������
//-----------------------------------------------------------------------------
u32 OBJ_GetLevel(struct Object *Obj)
{
    u32 n = 0;
    struct Object *pl_node = Obj;

    if(pl_node == NULL)
        return CN_LIMIT_UINT32;

    while(pl_node->Parent != s_ptRscRoot)
    {
        if(NULL != pl_node->Parent)
            pl_node = pl_node->Parent;
        else
        {
            n = CN_LIMIT_UINT32;
            break;
        }
        n++;
    }

    return n;
}

//----�����ӽڵ�--------------------------------------------------------------
//����: �ӵ�ǰ�ڵ㿪ʼ,��ȡ��һ���ڵ��ָ��,ֱ��������ȫ���ӽڵ�
//����: Parent,��Ҫ��������֦�����Ƚڵ�
//      Child,��ǰ����λ��,��ʼλ��Ӧ����Ϊ��parent��ȣ���������ȫ���ӽڵ㡣
//����: ��ǰ����λ�õ���һ���ڵ�ָ��,����Ѿ��������,�򷵻�NULL.
//------------------------------------------------------------------------------
struct Object *OBJ_TraveChild(struct Object *Parent, struct Object *Child)
{
    if((Parent == NULL) || (Child == NULL))
        return NULL;

    if(Child == Parent)
    {
        Child = Parent->Child;
    }
    else
    {
        Child = Child->Next;
        if(Child == Parent->Child)
            Child = NULL;
    }

    return Child;
}

//----����һ����֦--------------------------------------------------------------
//����: �ӵ�ǰ�ڵ㿪ʼ,��ȡ��һ���ڵ��ָ��,��������·��,ֱ��������������֦.����
//      ·��Ϊ:��ǰ�ڵ���ӽڵ�,����ӽڵ�Ϊ��������next�ڵ�,next�ڵ���Ϊ������
//      �����ڵ��next�ڵ�,ֱ���������.
//      ������ֻ��������ڵ㣬������ancestor_node����
//����: Ancestor,��Ҫ��������֦�����Ƚڵ�
//      Current,��ǰ����λ��,
//����: ��ǰ����λ�õ���һ���ڵ�ָ��,����Ѿ��������,�򷵻�NULL.
//��ע: ����Ҫ����Դ������ĳһ����֦�������������еĽڵ���һ����ĳ�ֲ���ʱ,��
//      �������ñ�����,��һ�ε���current_node = parent_node,���current_node
//      = �ϴη���ֵ��ֱ�����ؿ�.
//      �������������ӡ������....��˳������������ֱϵ��������ϵ��ȷ����������
//      �ڵ㶼�ܹ����ʵ�������Է���˳��������Ҫ�󣬲���ʹ�ñ�����
//------------------------------------------------------------------------------
struct Object *OBJ_TraveScion(struct Object *Ancestor, struct Object *Current)
{
    struct  Object *result = NULL,*current_copy;
    bool_t up = FALSE;

    if((Ancestor == NULL) || (Current == NULL))
        return NULL;

    if((Current != Ancestor) || Ancestor->Child)
    {
        current_copy = Current;
        do
        {
            if((up == FALSE) && (current_copy->Child != NULL))
            {   //�ӽڵ�ǿ�,�����ӽڵ�
                result = current_copy->Child;
                break;
            }
            else if(current_copy->Next != current_copy->Parent->Child)
            {   //�ӽڵ��,�����ڵ㲻�Ƕ���β�ڵ�,����next�ڵ�
                result = current_copy->Next;
                break;
            }
            else
            {   //���ӽڵ�,�ұ��ڵ��Ѿ��Ƕ���β�ڵ�,��Ҫ�жϵ�ǰ���ڵ��next�ڵ�
                current_copy = current_copy->Parent;
                up = TRUE;
            }
        }while(current_copy != Ancestor);
    }

    return result;
}

//----���ֵܽڵ���������Դ-----------------------------------------------------
//����: ��ĳһ����Դ�ڵ�������ֵܽڵ��������������ֵ���Դ
//����: Brother,�ֵܽڵ�������һ����Դָ��.
//      Name,��Ҫ��������Դ��
//����: �����������Դ������Դ�ڵ�ָ��,���򷵻�NULL
//-----------------------------------------------------------------------------
struct Object *OBJ_SearchSibling(struct Object *Brother, const char *Name)
{
    struct Object *temp, *result = NULL;

    if((Brother == NULL) || (Name == NULL))
        return NULL;

    temp = Brother;

    do
    {
        if(strcmp(temp->Name, Name) == 0)
        {
            result = temp;
            break;
        }
        temp = temp->Next;
    }while (temp != Brother);

    return result;
}

//----������Դ��---------------------------------------------------------------
//����: ����Դ���ڵ��ֱ���ӽڵ��в�����Ϊname����Դ��
//����: ��Դ����
//����: ��Դ���ڵ�ָ�롣NULL = û�ҵ�
//-----------------------------------------------------------------------------
struct Object *OBJ_SearchTree(const char *Name)
{
    struct Object *temp, *result = NULL;

    if(Name == NULL)
        return NULL;

    temp = s_ptRscRoot->Child;

    do
    {
        if(strcmp(temp->Name, Name)==0)
        {
            result = temp;
            break;
        }
        temp = temp->Next;
    }while (temp != s_ptRscRoot->Child);

    return result;
}

//----���ӽڵ���������Դ-----------------------------------------------------
//����: ��ĳһ����Դ�ڵ�������ӽڵ��������������ֵ���Դ
//����: Parent,���ڵ���Դָ��.
//      Name,��Ҫ��������Դ��,����Դ��,������·��.
//����: �����������Դ������Դ�ڵ�ָ��,���򷵻�NULL
//-----------------------------------------------------------------------------
struct Object *OBJ_SearchChild(struct Object *Parent, const char *Name)
{
    struct Object *temp, *result = NULL;

    if((Name == NULL) || (Parent == NULL))
        return NULL;

    temp = Parent->Child;
    if(temp == NULL)
        result = NULL;
    else
    {
        do
        {
            if(strcmp(temp->Name, Name)==0)
            {
                result = temp;
                break;
            }
            temp = temp->Next;
        }while (temp != Parent->Child);
    }

    return result;
}

//----�ں����������Դ��-------------------------------------------------------
//����: ��rsc_search_layer����,����ancestor_node�����к���ڵ�,ֱ���ҵ�һ������
//      ƥ����Դ��
//����: Ancestor,��֦�ĸ��ڵ�
//      Name,��Ҫ��������Դ��,����Դ��,������·��.
//����: �����������Դ������Դ�ڵ�ָ��,���򷵻�NULL
//------------------------------------------------------------------------------
struct Object *OBJ_SearchScion(struct Object *Ancestor, const char *Name)
{
    struct Object *current, *temp, *result = NULL;

    if((Ancestor == NULL) || (Name == NULL))
        return NULL;

    current = Ancestor;
    //��rsc_trave_scion���Ѿ����ź����������˴����뱣��
    while((temp = OBJ_TraveScion(Ancestor, current)) != NULL)
    {
        if(strcmp(temp->Name, Name) == 0)
        {
            result = temp;
            break;
        }
        current = temp;
    }
    return result;
}

//----��·��������Դ��---------------------------------------------------------
//����: ��rsc_search_scion����,��ͬ���ǣ�path�ǰ���·�������ַ�����ִ�о�ȷ������
//����: Start,��֦�ĸ��ڵ�
//      Path,����·��������Դ��
//����: �����������Դ������Դ�ڵ�ָ��,���򷵻�NULL
//-----------------------------------------------------------------------------
struct Object *OBJ_Search(struct Object *Start, const char *Path)
{
    const char *path_name;
    char *dest_name;
    bool_t match;
    u32 i;
    struct Object *current, *result = NULL;

    if((Path == NULL) || (Start == NULL))
        return NULL;

    current = Start->Child;
    if(current == NULL)
    {
        return NULL;
    }
    path_name = Path;
    while(1)
    {
        dest_name = current->Name;
        match = FALSE;
        for(i=0;i<CN_RSC_NAME_LIMIT;i++)   //��Դ����������
        {
            if((path_name[i] == 0)||((path_name[i]=='\\')&&(path_name[i+1]==0)))
            {   //�Ѿ��ﵽ�ַ�������,�����
                result = current;
                break;
            }else if(path_name[i] == '\\')
            {   //�ַ������,��path_nameδ����
                match = TRUE;
                break;
            }
            if(path_name[i] != dest_name[i])
            {   //�ַ��������
                match = FALSE;
                break;
            }
        }
        if(result != NULL)  //����ƥ���ҵ���path�ַ���ĩ,���ҽ���
            break;

        if(match != TRUE)
        {   //���ڵ����ֲ�ƥ��,������һ���ڵ�
            current = current->Next;
            if(current == current->Parent->Child)
            //���������нڵ��Ѿ�������,û���ҵ�ƥ���.
                break;
        }
        else
        {   //���ڵ�����ƥ��,���ǻ�û��path������,���������ӽڵ�
            path_name += i+1;
            current = current->Child;
            if(current == NULL)
            //������Դ���Ѿ���������,��path_name��δ����
                break;
        }
    }

    return result;
}

//----�Ƿ����ͷ-------------------------------------------------------------
//����: �жϸ����ڵ��Ƿ��ֵܽڵ��еĶ���ͷ�ڵ�
//����: Obj�������Խڵ�
//����: TRUE=�Ƕ���ͷ��FALSE=���Ƕ���ͷ
//-----------------------------------------------------------------------------
bool_t OBJ_IsHead(struct Object *Obj)
{
    if(NULL == Obj)
        return (FALSE);

    if(Obj == s_ptRscRoot)
        return TRUE;

    if((Obj) && (Obj  == Obj->Parent->Child))
        return TRUE;
    else
        return FALSE;
}

//----�Ƿ����β---------------------------------------------------------------
//����: �жϸ����ڵ��Ƿ��ֵܽڵ��е����һ���ڵ�
//����: Obj�������Խڵ�
//����: TRUE=�����һ���ڵ㣬FALSE=����
//-----------------------------------------------------------------------------
bool_t OBJ_IsLast(struct Object *Obj)
{
    if(NULL == Obj)
        return (FALSE);

    if(Obj == s_ptRscRoot)
        return TRUE;

    if(Obj->Next  == Obj->Parent->Child)
        return TRUE;
    else
        return FALSE;
}

//----ȡ�ڵ�˳��-------------------------------------------------------------
//����: ȡ�����ڵ����ֵܽڵ��е�λ�ã�head=0����һ����1������֮
//����: Obj�������Խڵ�
//����: �ڵ�λ�ú�
//-----------------------------------------------------------------------------
u32 OBJ_Sequencing(struct Object *Obj)
{
    u32 ranking;
    struct Object *temp;

    if(NULL == Obj)
        return (-1);

    if(Obj == s_ptRscRoot)
        return (0);
    ranking = 0;
    temp = Obj->Parent->Child;
    while(temp != Obj)
    {
        ranking +=1;
        temp = temp->Next;
    }
    return (ranking);
}

