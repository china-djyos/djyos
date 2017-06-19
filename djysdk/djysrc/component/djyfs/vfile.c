//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//
//
//
#include <string.h>
#include <stdlib.h>
#include <systime.h>
#include <pool.h>
#include <djyos.h>
#include "mount.h"
#include "vfile.h"

//
// ���ر���
//
extern char g_cPathSegment[257]; // ./fs_module.c�ж���;
//-----------------------------------------------------------------------------
//����: ·��������ʼ��
//����: PathContext -- ·��������������;
//      Path -- ·��;
//����: ��NULL -- �ɹ�; NULL -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
void __PathInit(struct PathWalk *PathContext, const char *Path)
{
    struct VCommon *FileCom;
    struct VMount *Mount;

    PathContext->PrivateVHost = NULL;
    PathContext->Private = NULL;
    PathContext->FileOps = NULL;

    if(Path[0] == '/')// ����·��
    {
        PathContext->End = g_tSysRoot.Obj;
        PathContext->VOps = g_tSysRoot.VOps;
    }
    else// �����������·��
    {
        PathContext->End = g_tSysPWD.Obj;
        PathContext->VOps = g_tSysPWD.VOps;
    }

    PathContext->Path = PathContext->UncoveredPath = (char*)Path;

    // ��ȡ�ļ�ϵͳ�ĸ���
    if(PathContext->End->Type & OB_MOUNT)
    {
        FileCom = Container(PathContext->End, struct VCommon, Obj);
        Mount = (struct VMount*)(FileCom->Context);
        PathContext->Root = Mount->Name;
    }
    else
    {
        PathContext->Root = g_tSysPWD.Path; // �龰: ��ǰ����·�����õ��˾����ļ�ϵͳ�ڲ���Ŀ¼
        PathContext->FileOps = g_tSysPWD.FileOps;
    }
}
//-----------------------------------------------------------------------------
//����: �����⻺���м����ļ�
//����: PathContext -- ·��������������;
//      Open -- 1��ʾ��Open�߼���ʹ��
//����: 1 -- �ҵ�Ŀ���ļ�; 0 -- δ�ҵ�Ŀ���ļ�;
//��ע:
//-----------------------------------------------------------------------------
u8 __VPathLookup(struct PathWalk *PathContext, u8 Open)
{
    struct Object *Current, *Parent;
    char *PathCur = PathContext->Path;
    char *PathNext;
    struct VMount *Mount;
    struct VCommon *FileCom;
    u8 Length = 0;

    Current = PathContext->End;
    while(1)
    {
        while(*PathCur == '/')
            PathCur++; // ���˶����'/'

        if(*(PathCur) == '\0')
            break; // ����·������

        Parent = Current;


        // �ҵ���һ���ļ���Ŀ¼������PathCurָ���ļ�������ʼ��PathNextָ���ļ�������������һ���ַ���
        PathNext = PathCur;
        while(1)
        {
            if((*PathNext == '/') || (*PathNext == '\0'))
                break;
            PathNext++;
        }

        if(PathCur[0] == '.' && PathCur[1] == '.')
        {
            // �����ǰ�ڵ�ĸ�Ϊ��㣬��ǰ�ڵ㲻��.�ݴ�������丸Ϊ�յĻ���current�ڵ㲻��.
            if(Current->Parent)
                Current = Current->Parent;
        }
        else if(PathCur[0] != '.')
        {
            // �����ַ�������Ҫ�ַ�����ʽ������Ϊ�˽�ʡ�ռ䣬����ԭ�ƻ�ֱ����ԭ�ַ����ϸģ���ʵ�ֲ���,��Ϊconst
            if(*PathNext != '\0')
            {
                Length = PathNext - PathCur;
                memcpy(g_cPathSegment, PathCur, Length);
                g_cPathSegment[Length] = '\0';
                Current = OBJ_SearchChild(Current, g_cPathSegment);
            }
            else
                Current = OBJ_SearchChild(Current, PathCur);
        }
        // �Ȳ�����û���ҵ����ȶԸ��ڵ�����жϡ���Ϊ·�����е����λ�ã�������ڵ��ǹ��ص㣬
        // ��ô�Ϳ��ǵ���������һ���ļ�ϵͳ�ˡ�
        if(Parent->Type & OB_MOUNT)
        {
            PathContext->Path = PathCur;// �ļ�ϵͳ�ľ���·��
            FileCom = Container(Parent, struct VCommon, Obj);
            Mount = (struct VMount*)(FileCom->Context);
            PathContext->VOps = Mount->VOps;
            PathContext->FileOps = Mount->Private;
            PathContext->Root = Mount->Name;// �ļ�ϵͳ�ĸ���
        }

        if(NULL == Current)
            break;// �����жϣ�·���ϳ��ֲ�����(����)�Ľڵ�

        PathCur = PathNext;
    }

    if(NULL != Current)
    {
        PathContext->UncoveredPath = NULL;
        PathContext->End = Current;

        // �龰����Ŀ�����ļ�ϵͳ�Ĺ��ص�,�˴��൱�ڴ�һ���ļ�ϵͳ�ĸ�Ŀ¼,
        // ��ô��Ҫ��ȡ���������
        if((Current->Type & OB_MOUNT) && Open)
        {
            PathContext->Path = NULL;
            FileCom = Container(Current, struct VCommon, Obj);
            Mount = (struct VMount*)(FileCom->Context);
            PathContext->VOps = Mount->VOps;
            PathContext->FileOps = Mount->Private;
            PathContext->Root = Mount->Name;// �ļ�ϵͳ�ĸ���
        }

        return (1);// �ڻ������ҵ���Ŀ��
    }
    else
    {
        PathContext->UncoveredPath = PathCur;
        PathContext->End = Parent;

        return (0);// �ڻ�����δ�ҵ�Ŀ¼
    }
}

//-----------------------------------------------------------------------------
//����: �����⻺���в���·��ȱʧ��Ŀ¼��
//����: Path -- �����⻺����ȱʧ��·��;
//      Obj -- ·�����;
//����: ��NULL -- �ɹ�; NULL -- ʧ��,�ڴ治���·���ڵ��Ѵ���.
//��ע: û�д���////�����龰����Ϊǰ�ڵĺ������Ա�֤������������龰��
//-----------------------------------------------------------------------------
struct VCommon *__CoverPath(const char *Path, struct Object *Obj)
{
    struct VCommon *FileCom = NULL;
    struct Object *Parent = Obj;
    struct Object *Child;
    char *PathCur = (char *)Path;
    char *PathNext, *Slash;
    u8 Last = 0;
    u16 Length;

    while(1)
    {
        while(*PathCur == '/')
            PathCur++;// ���˶����'/'

        PathNext = PathCur;

        while(1)
        {
            if('\0' == *PathNext)
            {
                Last = 1;
                break;
            }

            if('/' == *PathNext)// �ж�·��'/'�����Ƿ��ֱ�ӽ�����
            {
                Slash = PathNext;
                Slash++;
                while(1)
                {
                    if('/' == *Slash)
                    {
                        Slash++;
                        continue;
                    }
                    else if('\0' == *Slash)
                        Last = 1;
                    break;// '/'����������Ƿ�'/'��'\0'�ַ�
                }
                break;// '/'����������Ƿ�'/'��'\0'�ַ�
            }

            PathNext++;
        }

        Length = PathNext - PathCur;// ��ǰ�ļ�������

        if(PathCur[0] == '.' && PathCur[1] == '.')// �����ǰ�ڵ�ĸ�Ϊ��㣬��ǰ�ڵ㲻��
        {
            if(Parent->Parent)
                Parent = Parent->Parent;// �ݴ�������丸Ϊ�յĻ���Parent�ڵ㲻��
        }
        else if(PathCur[0] != '.')
        {
            if(*PathNext != '\0')
            {
                Length = PathNext - PathCur;
                memcpy(g_cPathSegment, PathCur, Length);
                g_cPathSegment[Length] = '\0';
                Child = OBJ_SearchChild(Parent, g_cPathSegment);
            }
            else
                Child = OBJ_SearchChild(Parent, PathCur);


            if(NULL == Child)
            {
                Length += (sizeof(struct VCommon) + 1);// 1��'\0'

                FileCom = (struct VCommon*)(malloc(Length));
                if(NULL == FileCom)
                    return (NULL);

                if(*PathNext != '\0')
                    strcpy(FileCom->Name, g_cPathSegment);
                else
                    strcpy(FileCom->Name, PathCur);

                FileCom->Context = NULL;

                Child = OBJ_AddChild(Parent, &(FileCom->Obj), Length, (u16)OB_FILE, FileCom->Name);
            }

            Parent = Child;
        }

        if(Last)
            return ((NULL == Parent) ? NULL : Container(Parent, struct VCommon, Obj)); // todo
        else
            PathCur = PathNext;
    }
}

//-----------------------------------------------------------------------------
//����: ���ļ�
//����: Path -- �ļ�·��;
//      Mode -- �ļ�����ģʽ;
//����: ��NULL -- �ɹ�; NULL -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
struct VFile *__OpenInner(const char *Path, u32 Mode, u8 Lock)
{
    struct PathWalk PContext;
    struct VFile *Fp;
    u8 IsFound;
    s32 Ret = -1;

    if(Lock)
        Lock_MutexPend(g_ptVFMutex, CN_TIMEOUT_FOREVER);// ������

    // ��ʼ��PContext
    __PathInit(&PContext, Path);

    IsFound = __VPathLookup(&PContext, 1);// �����м���

    if(PContext.VOps && PContext.VOps->VOpen)// ϵͳ�ļ���֧�ָú���
        Ret = PContext.VOps->VOpen(&PContext, Mode, IsFound);

    if(Lock)
        Lock_MutexPost(g_ptVFMutex);// ������

    if(Ret < 0)
    {// ��������
        Fp = NULL;
    }
    else
    {// �ɹ���
        Fp = AllocVFile();
        if(NULL != Fp)
        {
            if(PContext.PrivateVHost) // ��ֹ�ܷ�
                *(PContext.PrivateVHost) = Fp;
            Fp->Private = PContext.Private; // FileContext
            Fp->VOps = PContext.VOps;
        }
    }

    return (Fp);
}

//-----------------------------------------------------------------------------
//����: ���ļ�,���ⷽʽ
//����: Path -- �ļ�·��;
//      Mode -- �ļ�����ģʽ;
//����: ��NULL -- �ɹ�; NULL -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
struct VFile *__Open(const char *Path, u32 Mode)
{
    return (__OpenInner(Path, Mode, 1));
}

//-----------------------------------------------------------------------------
//����: ���ļ�,�ǻ��ⷽʽ
//����: Path -- �ļ�·��;
//      Mode -- �ļ�����ģʽ;
//����: ��NULL -- �ɹ�; NULL -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
struct VFile *__OpenUnlock(const char *Path, u32 Mode)
{
    return (__OpenInner(Path, Mode, 0));
}

//-----------------------------------------------------------------------------
//����: �����ļ������ṹ��
//����:
//����: ��NULL -- �ɹ�; NULL -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
struct VFile *AllocVFile(void)
{
    struct VFile *Fp = NULL;
    struct VFilePool *Pool = g_ptVFilePool;

    Lock_MutexPend(g_ptVFilePoolMutex, CN_TIMEOUT_FOREVER);
    while(Pool)
    {
        Fp = (struct VFile*)Mb_Malloc(Pool->Management, 0);
        if(Fp)
        {
            Pool->Used++;// ��ȡ�ɹ�,������һ;
            break;
        }

        // ��ǰ�ڴ�������꣬������һ��������չ�ڴ��;
        if(NULL == Pool->Next)
            Pool = __ExtendVFilePool();
        else
            Pool = Pool->Next;
    }
    Lock_MutexPost(g_ptVFilePoolMutex);

    return (Fp);
}
//-----------------------------------------------------------------------------
//����: �ͷ��ļ������ṹ��
//����: File -- �ļ������ṹ��;
//����: ��NULL -- �ɹ�; NULL -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
void FreeVFile(struct VFile *File)
{
    struct VFilePool *Pool = PoolOf(File);

    Lock_MutexPend(g_ptVFilePoolMutex, CN_TIMEOUT_FOREVER);

    Mb_Free(Pool->Management, File);

    if((!(--Pool->Used)) && g_ptVFilePool->Next)// ɾ��Pool�������ٱ���һ��Pool����ɾ��
    {
        if(Pool != g_ptVFilePool)
        {
            struct VFilePool *PrePool = g_ptVFilePool;
            while(PrePool)
            {
                if(PrePool->Next == Pool)
                {
                    PrePool->Next = Pool->Next;
                    break;
                }
                PrePool = PrePool->Next;
            }
        }
        else
            g_ptVFilePool = Pool->Next;

        Mb_DeletePool(Pool->Management);// ע���ڴ��
        free(Pool->Space);// �ͷſռ�
        free(Pool);
    }

    Lock_MutexPost(g_ptVFilePoolMutex);
}
//-----------------------------------------------------------------------------
//����: ��չ�ļ��������ṹ��ռ�.�ļ��������ռ��ɶ������һ����ڴ����ɣ����
//      djyos�ļ���������ǿ��Զ�̬��ӵģ�����Ҫ��̬���á�
//����:
//����: ��NULL -- �ɹ�; NULL -- ʧ��;
//��ע: ���ñ���������ע�⻥�⴦��
//-----------------------------------------------------------------------------
struct VFilePool *__ExtendVFilePool(void)
{
    struct VFilePool *New, *Next, *Cur;
    u16 ID = 0;

    New = (struct VFilePool*)malloc(sizeof(struct VFilePool));
    if(NULL == New)
        return (NULL);

    New->Space = (struct VFile*)malloc(VFILE_POOL_SIZE);
    if(NULL == New->Space)
    {
        free(New);
        return (NULL);
    }

    memset(New->Space, 0, VFILE_POOL_SIZE);// ���ݳ�ʼ��

    New->Management = (struct MemCellPool*)Mb_CreatePool(New->Space,
                       VFILE_NUM, VFILE_SIZE, 0, 0, "vfile pool");
    New->Used = 0;

    // ����VFilePool��������,�����ǰ�ID�ŵĴ�С��������.
    if((g_ptVFilePool) && (0 == g_ptVFilePool->ID))
    {
        Cur = g_ptVFilePool;
        while(1)
        {
            ID++;
            Next = Cur->Next;
            if(Next == NULL || Next->ID != ID)
                break;
        }
        New->ID = ID;
        New->Next = Next;
        Cur->Next = New;
    }
    else
    {
        New->ID = ID;
        New->Next = g_ptVFilePool;
        g_ptVFilePool = New;
    }

    return (New);
}


//-----------------------------------------------------------------------------
//����: ��ȡ�ļ������ṹ�����ڵĿռ�.
//����: Fp -- �ļ������ṹ��;
//����: �ļ������ṹ�����ڵĿռ�;
//��ע:
//-----------------------------------------------------------------------------
struct VFilePool *PoolOf(struct VFile *Fp)
{
    struct VFilePool *Pool = g_ptVFilePool;

    // ��ȡFp����VFile�ؿռ�
    while(Pool)
    {
        if((Pool->Space <= Fp) && (Fp < (Pool->Space + VFILE_NUM)))
            break;// �ҵ�����ַ������Pool��
        Pool = Pool->Next;
    }

    return(Pool);
}

//-----------------------------------------------------------------------------
//����: �����ļ�������
//����: BufRet -- ����������;
//      Force -- 1-ǿ������, 0 - ��ǿ��;
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
const u32 gc_CONFIG_FS __attribute__ ((weak));
#define BUF_SIZE            (512)
s32 AllocFileBuf(struct FileBuf **BufRet, u8 Force)
{
    struct FileBuf *Buf = NULL;

    if((gc_CONFIG_FS & FS_BUFFERED) || (Force))
    {
        Buf = (struct FileBuf*)malloc(sizeof(struct FileBuf));
        if(!Buf)
            return (-1);

        Buf->Start = Buf->Current = (u8*)malloc(BUF_SIZE);
        if(!Buf->Start)
        {
            free(Buf);
            return (-1);
        }

        Buf->End = Buf->Start + BUF_SIZE;
        Buf->Status = BUF_CLEAN;
    }

    *BufRet =  Buf;
    return (0);
}
//-----------------------------------------------------------------------------
//����: �ͷ��ļ�������.
//����: Buf -- �ļ�������;
//����:
//��ע:
//-----------------------------------------------------------------------------
void FreeFileBuf(struct FileBuf *Buf)
{
    if(Buf)
    {
        free(Buf->Start);
        free(Buf);
    }
}

//-----------------------------------------------------------------------------
//����: ·��ת��
//����: RelPath -- ���·��;
//      Root -- RelPath·�������;
//����:
//��ע: �ϼ�ϵͳ��֤�˻��⣬���ﲻ���ǻ���
//-----------------------------------------------------------------------------
char *PathCopy(const char *Root, const char *RelPath, char (*Rebuild)(char *Path))
{
    u8 Length ;

    if(Root) // û�еĻ������Ǹ����·��
    {
        Length = strlen(Root);
        memcpy(g_cPathSegment, Root, Length);
        if(RelPath && ('/' != RelPath[0]))
            g_cPathSegment[Length++] = '/';
    }

    if(RelPath)
        strcpy((g_cPathSegment+Length), RelPath); // ƴ��������·��
    else
        g_cPathSegment[Length] = '\0'; // ֻ�и�Ŀ¼

    if(Rebuild)
        Rebuild(g_cPathSegment);

    return(g_cPathSegment);
}

//-----------------------------------------------------------------------------
//����: �����ļ�������
//����:
//����: !NULL -- �ɹ�; NULL -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
struct FileContext *AllocContext(void)
{
    struct FileContext *FileCt;

    FileCt = malloc(sizeof(struct FileContext));
    if(!FileCt)
        return (NULL);

    memset(FileCt, 0, sizeof(struct FileContext));

    return (FileCt);
}

//-----------------------------------------------------------------------------
//����: �ͷ��ļ�������.
//����: FileCt -- �ļ�������;
//����:
//��ע:
//-----------------------------------------------------------------------------
void FreeContext(struct FileContext * FileCt)
{
    if(FileCt)
        free(FileCt);
}
//-----------------------------------------------------------------------------
//����: ��Member������г�ԱMemInList֮��
//����: ListMem  --
//      New -- �¼���ĳ�Ա
//����:
//��ע:
//-----------------------------------------------------------------------------
void __ListInsert(struct FileContextList *MemInList, struct FileContextList *New)
{
    New->Pre = MemInList;
    New->Next = MemInList->Next;
    if(MemInList->Next)
        MemInList->Next->Pre = New;
    MemInList->Next = New;
}
//-----------------------------------------------------------------------------
//����: ���ļ�
//����: PathContext  -- ·��������;
//      Mode -- �ļ�����ģʽ;
//      IsFound -- �ļ��Ƿ��ѻ���;1:�ѻ���;0:δ����;
//����: 1 -- �´�һ���ļ�(δ�򿪹�); 2 -- ��һ���ļ�(�Ѵ򿪹�);
//      -1 -- �ļ����ʹ���(�����������); -2 -- �ļ��޷���; -3 -- ��������;
//��ע: ���������Ƿ�ע���飬
//-----------------------------------------------------------------------------
s32 FOpen(struct PathWalk *PathContext, u32 Mode, u8 IsFound)
{
    struct FileContext *FileCt;
    struct VCommon *FileCom;
    struct FileOperations *FileOps;
    struct VMount *Mount;
    char *PathTemp;
    u32 Length;
    s32 Ret;
    struct Object *End = PathContext->End;

    if(!(End->Type & OB_FILE))// ����У��
        return (-1);

    FileOps = (struct FileOperations *)__FileOps(PathContext);

   if(IsFound)// �ļ��ڻ����з����ˣ����ܱ��򿪹���������ʱ�ļ�
   {
        struct FileContext **ExFileCt;// �Ѵ��ڵ������ļ���
        u16 TypeCheck = End->Type & (~OB_FILE);

        FileCom = Container(End, struct VCommon, Obj);

        if((Mode & M_TYPE) &&
            TypeCheck &&
            !(((Mode & M_REG) && (TypeCheck & OB_REG)) ||
              ((Mode & M_DIR) && (TypeCheck & OB_DIR)) ||
              ((Mode & M_LINK) && (TypeCheck & OB_LINK))))
            return (-1); // Ŀ��������Mode��һ��

        FileCt = FileOps->FileOpen(PathContext->Path, Mode, PathContext->Root);
        if(NULL == FileCt)
            return (-2); // �ļ��޷���

        if(End->Type & OB_MOUNT)// ��ǰ�ڵ��ǰ�װ��
        {
            Mount = (struct VMount*)(FileCom->Context);

            while(Mount->Property & MOUNT_ON_MOUNT)// ���ڰ�װ�㸲��,��ʱѰ�ҵ����ļ���������
                Mount = (struct VMount*)Mount->Context;

            ExFileCt = (struct FileContext**)&(Mount->Context);
        }
        else
            ExFileCt = (struct FileContext**)&(FileCom->Context);// ��ͨ�ļ�

        FileCt->CHost = FileCom;
        if(*ExFileCt)
            __ListInsert(&((*ExFileCt)->List), &(FileCt->List));
        else
            *ExFileCt = FileCt; // ���������龰��ϵͳ����,��Ϊ���ֽڵ㽨��ʱ������������

        Ret = 2;
   }
   else
   {
       PathTemp = PathContext->UncoveredPath;
       Length = strlen(PathTemp);
       if(PathTemp[Length-1] == '/')
       {
           if((Mode & M_TYPE) && (M_DIR != (Mode & M_TYPE)))
               return (-1);// ģʽ��·��������,������M_REG

           Mode |= M_DIR;// ģʽ��δ����ģʽ�����ｫ������Ϊ��ΪĿ¼
       }

       FileCt = FileOps->FileOpen(PathContext->Path, Mode, PathContext->Root);// �������豸�е�Ŀ���ļ�
       if(NULL == FileCt)
           return (-2);// Ŀ���ļ��޷���

       FileCom = __CoverPath(PathContext->UncoveredPath, End);// ���ڴ��н���·��
       if(FileCom)
       {
           switch(FileCt->Property & P_TYPE) //todo:
           {
               case P_REG: FileCom->Obj.Type |= OB_REG; break;
               case P_DIR: FileCom->Obj.Type |= OB_DIR; break;
               case P_LINK: FileCom->Obj.Type |= OB_LINK; break;
               default :
                   FileOps->FileClose(FileCt);
                   return (-3);// �������͵�Ӧ���Ǵ���ġ�
           }
           FileCom->Context = (void*)FileCt;
           FileCt->CHost = FileCom;
           Ret = 1;
       }
       else
       {
           FileOps->FileClose(FileCt);// ��������,�ر��Ѵ򿪵�Ŀ���ļ�
           Ret = -3; // �ڴ治��(����·��ʧ��)
       }
   }

   if(Ret > 0)
   {
       PathContext->Private = FileCt;
       PathContext->PrivateVHost = &(FileCt->FHost);
   }

    return (Ret);
}
//-----------------------------------------------------------------------------
//����: ����Ա�Ӷ�����ɾ���������ض��е�ͷ�����������֮��Ķ��У�
//����: Mem -- �豻ɾ���Ķ��г�Ա
//����:
//��ע:
//-----------------------------------------------------------------------------
struct FileContextList *__ListDel(struct FileContextList *Mem)
{
    struct FileContextList *Head; // Listͷ
    struct FileContextList *PreMem = Mem->Pre;
    struct FileContextList *NextMem = Mem->Next;

    // ��ԱMem�޳�������
    if(PreMem)
        PreMem->Next = NextMem;

    if(NextMem)
        NextMem->Pre = PreMem;

    // ��ȡ����ͷ
    if(PreMem)
    {
        Head = PreMem;
        while(Head->Pre)
            Head = Head->Pre; // ��ȡ����ͷ
    }
    else
    {
        Head = NextMem; // ����ɾ�����Ƕ���ͷ�����Ժ����ĳ�Ա�ͳ�Ϊ�˶���ͷ����NULL
    }

    return (Head);
}
//-----------------------------------------------------------------------------
//����: �ر��ļ�
//����: File -- �ļ�������;
//����: 0 -- �ɹ�;
//      -1 -- ��������; -2 -- �ļ����ʹ���; ����;
//��ע:
//-----------------------------------------------------------------------------
s32 FClose(void *File)
{
    struct FileContext *FileCt;
    struct VCommon *FileCom;
    struct FileOperations *FileOps;
    s32 Ret;
    u32 BufferedDataLen;
    struct FileContextList *Head;
    struct FileContext *FileCtHead = NULL;

    if(NULL == File)
        return (-1);

    FileCt = (struct FileContext*)File;
    FileCom = FileCt->CHost;

    if(!(FileCom->Obj.Type & OB_FILE))// ����У��
        return (-2);

    FileOps = (struct FileOperations*)(FileCt->FileOps);
    if((NULL == FileOps) || (NULL == FileOps->FileClose))
        return (-2); // ����δע��

    // �ļ��ر�ʱ,����������д��
    if((FileCom->Obj.Type & OB_REG) && FileCt->Buf)
    {
        if(BUF_DIRTY == FileCt->Buf->Status)
        {
            BufferedDataLen = FileCt->Buf->Current - FileCt->Buf->Start;
            if(BufferedDataLen !=
                    FileOps->FileWrite(FileCt->Buf->Start, 1, BufferedDataLen, FileCt))
            {
                return (-2); // �ļ�д��ʧ��
            }

            FileCt->Buf->Status = BUF_CLEAN;
            FileCt->Buf->Current = FileCt->Buf->Start;
        }
    }
    Lock_MutexPend(g_ptVFMutex,CN_TIMEOUT_FOREVER);// ������

#if 0 // ��ʱ�������������
    if(FileCom->Obj.Type & OB_DIR)
        __FlushDir(FileCt->FHost);
#endif

    Head = __ListDel(&(FileCt->List)); // �Ƚ���Ա��������������ɾ��
    if(Head)
        FileCtHead = Container(Head, struct FileContext, List); // ����VCommon

    Ret = FileOps->FileClose(FileCt); // �ر������豸�е��ļ�
    if(Ret)
    {
        // �ļ�δ�رճɹ������������ؽ�������,�Ҷ���ͷ�������������ɾ�������˱仯
        if(Head)
        {
            __ListInsert(Head, &(FileCt->List));
            if(FileCom->Obj.Type & OB_MOUNT)
            {
                struct VMount *Mount = (struct VMount*)FileCom->Context;

                while(Mount->Property & MOUNT_ON_MOUNT)
                    Mount = (struct VMount*)Mount->Context;

                Mount->Context = FileCtHead;// ��ǰĿ¼�ǹ��ص㣬ֻ����������ΪNULL
            }
            else
            {
                FileCom->Context = FileCtHead; // ������������ΪNULL,�ٿ����Ƿ�ɾ��
            }
        }
    }
    else // �ɹ��ر�
    {
        // ɾ���ļ����ڴ�ڵ㻺��;
        // �ļ��ڵ㻺�治�ܱ�ɾ�������������������ӽڵ㣬����������, ������(SysRoot��SysPWD�����ص�)
        if(FileCom->Obj.Type & OB_MOUNT)
        {
            struct VMount *Mount = (struct VMount*)FileCom->Context;

            while(Mount->Property & MOUNT_ON_MOUNT)
                Mount = (struct VMount*)Mount->Context;

            Mount->Context = FileCtHead;// ��ǰĿ¼�ǹ��ص㣬ֻ����������ΪNULL
        }
        else
        {
            FileCom->Context = FileCtHead; // ������������ΪNULL,�ٿ����Ƿ�ɾ��
            if(NULL == FileCtHead) // �ļ����������Ѿ�ȫ�����ر�
            {
                while(!FileCom->Obj.Inuse) // �ļ�δ������
                {
                    struct Object *Parent;
                    if(OBJ_Del(&FileCom->Obj)) // ע����ǰ�ڵ�
                    {
                        Parent = FileCom->Obj.Parent;
                        free(FileCom); // ɾ���ļ��Ľڵ㻺��
                        if(Parent)
                        {
                            FileCom = Container(Parent, struct VCommon, Obj);
                            if(NULL == FileCom->Context)
                                continue; // ���ļ����������£������������ɾ��
                        }
                    }

                    break; // �����ӽڵ㣬�˳�
                }
            }
        }
    }

    Lock_MutexPost(g_ptVFMutex);// ������
    return (Ret);
}
//-----------------------------------------------------------------------------
//����: д�ļ�
//����: Buf -- ���ݻ���;
//      size -- ��Ҫд�ĵ�λ��;
//      nmemb -- ��λ��;
//      Private -- �ļ�������;
//����: 0 -- ʧ��; ��0 -- �ɹ�;
//��ע:
//-----------------------------------------------------------------------------
s32 FWrite(const void *Buf, u32 Size, u32 Nmemb, void *Private)
{
    struct FileContext *FileCt;
    struct VCommon *FileCom;
    struct FileOperations *FileOps;
    u32 Length, LengthLef, BufLeft, BufUsed, BufSize;
    u8 *DataPtr = (u8*)Buf;

    if(NULL == Private)
        return (0);// ��������

    FileCt = (struct FileContext*)Private;
    FileCom = (struct VCommon*)(FileCt->CHost);

    if((FileCom->Obj.Type != OB_REG) || !(FileCt->Property & P_WRITE))
        return (0);// �ļ����ͺ����Դ���

    FileOps = (struct FileOperations *)(FileCt->FileOps);
    if((NULL == FileOps) || (NULL == FileOps->FileWrite))
        return (0);// ����δע��

    LengthLef = Length = Size *Nmemb;
    while(FileCt->Buf) // �������߼�
    {
        BufLeft = FileCt->Buf->End - FileCt->Buf->Current;
        BufUsed = FileCt->Buf->Current - FileCt->Buf->Start;
        if(BufLeft >= LengthLef) // ����ռ��㹻
        {
            memcpy(FileCt->Buf->Current, DataPtr, LengthLef);
            FileCt->Buf->Current += LengthLef;
            FileCt->Buf->Status = BUF_DIRTY;
            return (Length);
        }
        else if(BUF_DIRTY == FileCt->Buf->Status)// ����ռ䲻��,�Ƚ�����������ˢ��
        {
            if(BufLeft) // ����д�����ݶ����߼�
            {
                memcpy(FileCt->Buf->Current, DataPtr, BufLeft);
                LengthLef -= BufLeft;
                DataPtr += BufLeft;
            }

            BufSize = FileCt->Buf->End - FileCt->Buf->Start;
            if((BufSize != FileOps->FileWrite(FileCt->Buf->Start, 1, BufSize, Private)))
                return (0);// д�����

            FileCt->Buf->Current = FileCt->Buf->Start;
            FileCt->Buf->Status = BUF_CLEAN;
            continue;// ��������ˢ���,�ٴ��ж�
        }

        break;
    }

    // �޻����߼�
    return (FileOps->FileWrite(DataPtr, 1, Length, Private));

}
//-----------------------------------------------------------------------------
//����: ���ļ�
//����: Buf -- ���ݻ���;
//      size -- ��Ҫ���ĵ�λ��;
//      nmemb -- ��λ��;
//      Private -- �ļ�������;
//����: 0 -- ʧ��; ��0 -- �ɹ�;
//��ע: ��Ϊ��д�������ǵ�������ģ���ǰ�����߼�ֻ֧��д���Զ��𲻵�����Ч����
//-----------------------------------------------------------------------------
s32 FRead(void *Buf, u32 Size, u32 Nmemb, void *Private)
{
    struct FileContext *FileCt;
    struct VCommon *FileCom;
    struct FileOperations *FileOps;
    u32 Length;

    if(NULL == Private)
        return (0);// ��������

    FileCt = (struct FileContext*)Private;
    FileCom = (struct VCommon*)(FileCt->CHost);

    if((FileCom->Obj.Type != OB_REG) || !(FileCt->Property & P_READ))
        return (0);// �ļ����ͺ����Դ���

    FileOps = (struct FileOperations *)(FileCt->FileOps);
    if((NULL == FileOps) || (NULL == FileOps->FileRead))
        return (0);// ����δע��

    if(FileCt->Buf)
    {
        if(BUF_DIRTY == FileCt->Buf->Status)
        {
            // ����FileWrite�Ƿ�ע�������, ��Ϊֻ��д�Ż����BUF_DIRTY, ����дʱ���˼��
            Length = FileCt->Buf->Current - FileCt->Buf->Start;
            if(Length != FileOps->FileWrite(FileCt->Buf->Start, 1, Length, FileCt))
                return (0);// д�����
            FileCt->Buf->Current = FileCt->Buf->Start;
            FileCt->Buf->Status = BUF_CLEAN;
        }

    }

    Length = Size * Nmemb;
    return (FileOps->FileRead(Buf, 1, Length, Private));
}
//-----------------------------------------------------------------------------
//����: ����Ŀ¼����(Ŀ¼�°������ļ���)���ļ�ϵͳ
//����:
//����: 0 -- �ɹ�; -1 -- ʧ��
//��ע: ʹ������Ҫ����
//-----------------------------------------------------------------------------
s32 __CacheDir(struct VFile *Fp)
{
    struct FileContext *FileCt;
    struct VCommon *FileCom;
    struct Object *Child, *DirObj;
    struct FileOperations *FileOps;
    s32 Ret;
    struct Dirent Content;

    FileCt = (struct FileContext *)Fp->Private;
    FileCom = FileCt->CHost;
    DirObj = &(FileCom->Obj);

    Child = OBJ_Child(DirObj);
    if((NULL != Child) && (0 == (Child->Type & OB_FILE)))
        return (0); // �����������ļ�ϵͳ���͵�����

    FileOps = (struct FileOperations *)FileCt->FileOps;

    if(NULL == FileOps->DirRead)
        return (-1); // ��֧�ָù���

    do
    {
        Ret = FileOps->DirRead(FileCt, &Content);
        if(Ret)
            break; // ����
        FileCom = __CoverPath(Content.Name, DirObj);
        if(FileCom)
        {
            switch(Content.Property & P_TYPE)
            {
                case P_REG: FileCom->Obj.Type |= OB_REG; break;
                case P_DIR: FileCom->Obj.Type |= OB_DIR; break;
                case P_LINK: FileCom->Obj.Type |= OB_LINK; break;
            }
        }
    }while(1);

    return (0);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
struct Object *__FileObj(struct VFile *Fp)
{
    struct FileContext *FileCt;
    struct VCommon *FileCom;

    FileCt = (struct FileContext *)Fp->Private;
    FileCom = FileCt->CHost;
    return (&(FileCom->Obj));
}
//-----------------------------------------------------------------------------
//����: ˢ��Ŀ¼����(Ŀ¼�°������ļ���)���ļ�ϵͳ
//����:
//����:
//��ע: ʹ������Ҫ����,��Ӧ__CacheDir,�������__CacheDir�������޷����յĽڵ�
//-----------------------------------------------------------------------------
void __FlushDir(struct VFile *Fp)
{
    struct VCommon *FileCom;
    struct Object *Obj, *ObjNext, *Tail;
    u8 AllChecked = 0;

    Obj = __FileObj(Fp);

    Obj = OBJ_Child(Obj); // ȡ��һ������
    if((Obj && (0 == (Obj->Type & OB_FILE))) || (NULL == Obj))
        return ; // �����������ļ�ϵͳ���͵����� �� û������

    // ������������,ɾ����ʱ���������
    Tail = OBJ_Previous(Obj); // ��ȡ����β�ڵ�

    while(1)
    {
        ObjNext = OBJ_Next(Obj);

        FileCom = Container(Obj, struct VCommon, Obj);
        if((NULL == FileCom->Context) && (0 == Obj->Inuse))
        {
            if(OBJ_Del(Obj))
                free(FileCom);
        }

        if(AllChecked)
            break;

        Obj = ObjNext;

        if(Tail == Obj)
            AllChecked = 1; // ����β�������������ȡ��
    }
}
//-----------------------------------------------------------------------------
//����: �ӵ�ǰ�ļ��ڵ�ĸ��̳в�������
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
void *__InheritFileOps(struct Object *FileObj)
{
    struct Object *Parent;
    struct VMount *Mount;
    struct VCommon *FileCom;

    Parent = FileObj;
    while(Parent)
    {
        if(Parent->Type & OB_MOUNT)
        {
             FileCom = Container(Parent, struct VCommon, Obj);
             Mount = (struct VMount*)(FileCom->Context);
             return (Mount->Private);
        }
        Parent = OBJ_Parent(Parent);
    }

    return (NULL);
}
//-----------------------------------------------------------------------------
//����: ��ȡ�ļ��Ĳ�������
//����: PathCt -- �ļ�·������ʱ��������
//����:
//��ע:
//----------------------------------------------------------------------------
void *__FileOps(struct PathWalk *PathCt)
{
    if(NULL != PathCt->FileOps)
        return (PathCt->FileOps);
    else
        return (__InheritFileOps(PathCt->End)); // �龰����ǰ����·������ĳ���ļ�ϵͳ�ڲ�
}
//-----------------------------------------------------------------------------
//����: ��Ŀ¼��
//����: Dir -- Ŀ¼������;
//
//����: 1 -- ����,ȫ������;
//      -1 -- ��������; -2 -- ������֧��; -3 -- ��������;
//      0 -- �ɹ���ȡ����;
//��ע:
//-----------------------------------------------------------------------------
 s32 ReadDir(struct VFile *Dir, struct Dirent *DirentInfo)
{
    struct FileContext *FileCt;
    struct VCommon *FileCom;
    struct Object *Child, *DirObj;
    struct FileOperations *FileOps;

    if((NULL == DirentInfo) || (NULL == Dir))
        return (-1);

    FileCt = (struct FileContext *)Dir->Private;
    FileCom = FileCt->CHost;
    DirObj = &(FileCom->Obj);

    Child = OBJ_Child(DirObj);
    if((NULL != Child) && (0 == (Child->Type & OB_FILE)))
        return (-3); // �����������ļ�ϵͳ���͵�����

    FileOps = (struct FileOperations *)FileCt->FileOps;

    if((NULL == FileOps) || (NULL == FileOps->DirRead))
        return (-2); // ��֧�ָù���

    return (FileOps->DirRead(FileCt, DirentInfo));
}
 //-----------------------------------------------------------------------------
 //����: �ж��Ƿ���ͨ�ļ�(Ŀ¼���ļ���)
 //����:
 //����:
 //��ע:
 //-----------------------------------------------------------------------------
 bool_t IS_FILE(struct VFile *File)
 {
     struct FileContext *FileCt;
     struct VCommon *FileCom;
     if(NULL == File)
         return (FALSE);

     FileCt = (struct FileContext*)File->Private;
     FileCom = FileCt->CHost;
     if(FileCom && (FileCom->Obj.Type & OB_FILE))
         return (TRUE);

     return (FALSE);

 }
 //-----------------------------------------------------------------------------
 //����: �ж��Ƿ����豸�ļ�
 //����:
 //����:
 //��ע:
 //-----------------------------------------------------------------------------
 bool_t IS_DEV(struct VFile *File)
  {
     if(IS_FILE(File))
         return (FALSE);
     return (TRUE);

  }
