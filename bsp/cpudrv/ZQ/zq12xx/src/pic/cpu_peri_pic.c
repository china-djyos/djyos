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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

// 文件名     ：
// 模块描述:
// 模块版本: V1.00
// 创建人员:
// 创建时间:
// =============================================================================
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "os.h"

typedef struct _PIC_TYPE_
{
    struct _PIC_TYPE_ *Next;
    u32 IntLine;
    u32 IntSub;
    u32 *IntEn;
    u32 *IntSts;
    u32 (*isr)(ptu32_t);
}tagPic;

tagPic *sPicHead = NULL;

static void Pic_NodeInsert(tagPic *node)
{
    tagPic *temp = sPicHead;
    while(temp->Next != NULL)
    {
        temp = temp->Next;
    }
    temp->Next = node;
    node->Next = NULL;
}

static tagPic* Pic_FindLineNode(tagPic *pic,u8 intline)
{
    tagPic *temp = pic;
    while(temp != NULL)
    {
        if(temp->IntLine == intline)
        {
            break;
        }
        else
        {
            temp = temp->Next;
        }
    }
    return temp;
}
static tagPic* Pic_FindSubInt(tagPic *pic,u8 intline,u8 subint)
{
    tagPic *temp = pic;
    while(temp != NULL)
    {
        if((temp->IntLine == intline) && \
                (temp->IntSub == subint))
        {
            break;
        }
        else
        {
            temp = temp->Next;
        }
    }
    return temp;
}

static bool_t Pic_IsrHandle(u8 intline)
{
    tagPic *pic;

    pic = sPicHead;
    while(1)
    {
        pic = Pic_FindLineNode(pic,intline);
        if(pic == NULL)
        {
            break;
        }

        if(*(pic->IntSts) & (1<<pic->IntSub))
        {
            pic->isr(pic->IntSub);
        }
        pic = pic->Next;
    }

    return true;
}
bool_t Pic_IntRegister(u8 intline,u8 subint,u32* EnAddr,\
                        u32* StsAddr,u32 (*isr)(ptu32_t))
{
    bool_t Ret = false;
    tagPic *pic,*temp;

    if(NULL == isr)
    {
        return false;
    }

    temp = Pic_FindLineNode(sPicHead,intline);
    if(NULL == temp)
    {
        Int_Register(intline);
        Int_SetClearType(intline,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(intline,Pic_IsrHandle);
        Int_SettoAsynSignal(intline);
        Int_ClearLine(intline);
        Int_RestoreAsynLine(intline);
    }

    pic = malloc(sizeof(tagPic));
    if(NULL != pic)
    {
        pic->IntLine = intline;
        pic->IntSub  = subint;
        pic->IntEn   = EnAddr;
        pic->IntSts  = StsAddr;
        pic->isr     = isr;
        pic->Next    = NULL;

        if(sPicHead == NULL)
        {
            sPicHead = pic;
        }
        else
        {
            Pic_NodeInsert(pic);
        }
        Ret = true;
    }
    return true;
}

bool_t Pic_IntEnable(u8 intline,u8 subint)
{
    bool_t Ret = false;
    tagPic *pic;

    pic = Pic_FindSubInt(sPicHead,intline,subint);
    if(NULL != pic)
    {
        *(pic->IntEn) |= 1<<subint;
        Ret = true;
    }
    return Ret;
}

bool_t Pic_IntDisable(u8 intline,u8 subint)
{
    bool_t Ret = false;
    tagPic *pic;

    pic = Pic_FindSubInt(sPicHead,intline,subint);
    if(NULL != pic)
    {
        *(pic->IntEn) &= ~(1<<subint);
        Ret = true;
    }
    return Ret;
}


