//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

//所属模块: GDD
//作者:  LiuWei.
//版本：V1.0.0
//文件描述: 列表框控件实现
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2015-03-25
//   作者:  LiuWei.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include    "gdd.h"
#include    <gui/gdd/gdd_private.h>
#include    <gdd_widget.h>

/*============================================================================*/

typedef struct{

    list_t node;
    u32 Flag;
    u32 Data;
    char szText[4];
}LISTBOX_ITEM;

typedef struct{
    list_t list;
    s32 ItemNum;
    s32 TopIndex;
    s32 CurSel;
    s32 ItemHeight;
    s32 xpos,ypos;
}LISTBOX_DATA;
//---------------创建列表-------------------------------------------------------
//功能：略
//参数：pMsg，消息指针
//返回：固定true
//-----------------------------------------------------------------------------
static bool_t __Widget_ListBoxCreate(struct WindowMsg *pMsg)
{
     HWND hwnd;
     LISTBOX_DATA *pLB;
     hwnd =pMsg->hwnd;
     if(pMsg->Param1==0)
     {
         pLB =(LISTBOX_DATA*)malloc(sizeof(LISTBOX_DATA));
         if(pLB==NULL)
              return false;
         memset(pLB, 0, sizeof(LISTBOX_DATA));
         pMsg->Param1=(ptu32_t)pLB;
         dListInit(&pLB->list);
         pLB->ItemNum =0;
         pLB->TopIndex =0;//表头是链表第几项
         pLB->CurSel =-1;
         pLB->ItemHeight =20;//行高
     }
     GDD_SetWindowPrivateData(hwnd,(ptu32_t)pMsg->Param1);
     return true;
}

//-----------------------------------------------------------------------
//功能：获取列表项
//参数：pMsg，消息指针
//返回：列表项
//-----------------------------------------------------------------------------

static  LISTBOX_ITEM*   __Widget_ListBoxGetItem(LISTBOX_DATA *pLB,u32 idx)
{
    LISTBOX_ITEM *item=NULL;
    list_t *n;
    u32 i;

    if(idx<pLB->ItemNum)
    {
        i=0;
        n =pLB->list.next;
        while(1)
        {
            if(n==&pLB->list)
            {
                break;
            }
            if(i==idx)
            {
                item =(LISTBOX_ITEM*)dListEntry(n,LISTBOX_ITEM,node);
                break;
            }

            i++;
            n =n->next;

        }

    }
    return item;
}
//添加字符串
static  bool_t  __Widget_ListBoxAddString(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    u32 i;
    char *text;
    list_t *n;
    LISTBOX_ITEM *item;
    u32 idx;
    hwnd =pMsg->hwnd;
    idx=pMsg->Param1;
    text=(char*)pMsg->Param2;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    if(pLB->ItemNum >= (s32)0x7FFFFFFE)
    {
        return (s32)-1;
    }

    i=strlen(text)+1;
    item =(LISTBOX_ITEM*)malloc(sizeof(LISTBOX_ITEM)+i);
    if(item == NULL)
    {
        return (s32)-1;
    }
    memset(item, 0, sizeof(LISTBOX_ITEM) + i);
    strcpy(item->szText,text);
    item->Flag =0;
    item->Data =0;

    i=0;
    n=pLB->list.next;
    while(1)
    {
        if(n == &pLB->list)
        {
            break;
        }
        n =n->next;

        if(i == idx)
        {
            break;
        }
        i++;
    }

    dListInsertBefore(n,&item->node);

    pLB->ItemNum++;

    if(pLB->CurSel<0)
    {
        pLB->CurSel=0;
    }

    if(pLB->TopIndex<0)
    {
        pLB->TopIndex=0;
    }
    GDD_InvalidateWindow(hwnd,false);
    return true;
}


//删除字符串
static  bool_t __Widget_ListBoxDelString(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    LISTBOX_ITEM *item;
    hwnd =pMsg->hwnd;
    s32 idx;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    idx=pMsg->Param1;

    if(pLB->ItemNum>0)
    {
        if(idx>=pLB->ItemNum)
        {
            idx=pLB->ItemNum-1;
        }

        item =__Widget_ListBoxGetItem(pLB,idx);
        if(item!=NULL)
        {
            dListRemove(&item->node);
            free(item);
        }

        pLB->ItemNum--;

        if(pLB->CurSel>=pLB->ItemNum)
        {
            pLB->CurSel=pLB->ItemNum-1;
        }

        if(pLB->TopIndex>=pLB->ItemNum)
        {
            pLB->TopIndex=pLB->ItemNum-1;
        }

        GDD_InvalidateWindow(hwnd,false);
        return true;
    }
    return false;
}


//重置内容
static  bool_t __Widget_ListBoxResetContent(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    list_t *n,*next;
    LISTBOX_ITEM *item;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);

    n=pLB->list.next;
    while(1)
    {
        if(n==&pLB->list)
        {
            break;
        }
        next =n->next;

        item =(LISTBOX_ITEM*)dListEntry(n,LISTBOX_ITEM,node);
        free(item);
        n=next;
    }

    pLB->ItemNum =0;
    pLB->TopIndex =0;
    pLB->CurSel =-1;
    GDD_InvalidateWindow(hwnd,false);
    return true;
}

//设置当前选中项
static  u32 Widget_ListBoxSetCurSel(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    u32 idx;
    idx=pMsg->Param1;

    if(pLB->ItemNum>0)
    {
        if(idx>=pLB->ItemNum)
        {
            idx=pLB->ItemNum-1;
        }

        if(pLB->CurSel!=idx)
        {
            pLB->CurSel=idx;
//          GDD_InvalidateWindow(hwnd,false);   //父窗口消息处理可能导致按钮被删除，
//                                          //InvalidateWindow不能在SendMessage之后调用
            GDD_PostMessage(GDD_GetWindowParent(hwnd),MSG_NOTIFY,(LBN_SELCHANGE<<16)|((u16)hwnd->WinId),(ptu32_t)hwnd);
        }
    }
    GDD_InvalidateWindow(hwnd,false);   //父窗口消息处理可能导致按钮被删除，
                                    //InvalidateWindow不能在SendMessage之后调用
    return pLB->CurSel;
}
//
static ptu32_t __Widget_ListBoxSetCurSel(HWND hwnd,LISTBOX_DATA *pLB,u32 idx)
{
    if(pLB->ItemNum>0)
    {
        if(idx>=pLB->ItemNum)
        {
            idx=pLB->ItemNum-1;
        }

        if(pLB->CurSel!=idx)
        {
            pLB->CurSel=idx;
            GDD_PostMessage(GDD_GetWindowParent(hwnd),MSG_NOTIFY,(LBN_SELCHANGE<<16)|((u16)hwnd->WinId),(ptu32_t)hwnd);
        }
    }
    return pLB->CurSel;
}

//获取当前选项
static ptu32_t __Widget_ListBoxGetCurSel(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    return pLB->CurSel;
}


//获取列表数
static ptu32_t __Widget_ListBoxGetCount(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    return pLB->ItemNum;
}

//设置表头指针
static  ptu32_t Widget_ListBoxSetTopIndex(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    s32 idx;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    idx=pMsg->Param1;
    if(idx<0)
    {
        idx =pLB->ItemNum-1;
    }

    if(idx<pLB->ItemNum)
    {
        pLB->TopIndex=idx;
    }
    GDD_InvalidateWindow(hwnd,false);
    return pLB->TopIndex;
}

//设置头指针
static ptu32_t __Widget_ListBoxSetTopIndex(LISTBOX_DATA *pLB,u32 idx)
{
    if(idx<pLB->ItemNum)
    {
        pLB->TopIndex=idx;
    }
    return pLB->TopIndex;
}
//设置头指针
static ptu32_t __Widget_ListBoxGetTopIndex(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    return pLB->TopIndex;
}

//设置项的行高
static  bool_t __Widget_ListBoxSetItemHeight(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    pLB->ItemHeight=pMsg->Param1;
    GDD_InvalidateWindow(hwnd,false);
    return true;
}
//取的项的行高
static ptu32_t __Widget_ListBoxGetItemHeight(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    return pLB->ItemHeight;
}

//设置项的数据
static  bool_t __Widget_ListBoxSetItemData(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    s32 idx;
    u32 data;
    LISTBOX_ITEM *item;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    idx=pMsg->Param1;
    data=pMsg->Param2;
    item =__Widget_ListBoxGetItem(pLB,idx);
    if(item!=NULL)
    {
        item->Data =data;
        return true;
    }
    return false;
}
//获取项的数据
static  ptu32_t __Widget_ListBoxGetItemData(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    s32 idx;
    LISTBOX_ITEM *item;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    idx=pMsg->Param1;
    item=__Widget_ListBoxGetItem(pLB,idx);
    if(item!=NULL)
    {
        return item->Data;
    }
    return 0;
}
//窗口客户区绘制
static  bool_t __Widget_ListBoxPaint(struct WindowMsg *pMsg)
{
    HDC hdc;
    HWND hwnd;
    LISTBOX_DATA *pLB;
    RECT rc,rc0;
    list_t *n;
    LISTBOX_ITEM *item;
    s32 i;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);

    hdc =GDD_BeginPaint(hwnd);
    if(hdc == NULL)
        return false;
    GDD_GetClientRect(hwnd,&rc0);
    if(GDD_GetWindowStyle(hwnd)&LBS_FLAT)
    {
        GDD_SetFillColor(hdc,RGB(200,200,200));
        GDD_FillRect(hdc,&rc0);
    }
    else
    {
        GDD_GradientFillRect(hdc,&rc0,
                        RGB(240,240,240),
                        RGB(100,100,100),
                        CN_FILLRECT_MODE_UD);
    }

    i=0;
    n=pLB->list.next;
    while(1)
    {
        if(n==&pLB->list)
        {   //已经是最后一个表项.
            break;
        }

        if(i==pLB->TopIndex)
        {   //丛TopIndex开始绘制.
            break;
        }

        i++;
        n=n->next;
    }

    if(n!=&pLB->list)
    {
        GDD_SetRect(&rc,0,0,GDD_RectW(&rc0),pLB->ItemHeight);
        while(1)
        {   //循环绘制表项.
            if(n == &pLB->list)
            {   //已经是最后一个表项.
                break;
            }

            if(rc.top >= rc0.bottom)
            {   //已经不在窗口范围内.
                break;
            }

            item =(LISTBOX_ITEM*)dListEntry(n,LISTBOX_ITEM,node);
            if(i == pLB->CurSel)
            {   //为当前选中项目.
                GDD_SetDrawColor(hdc,RGB(255,100,255));
                GDD_SetFillColor(hdc,RGB(128,0,160));
                GDD_SetTextColor(hdc,RGB(0,255,0));
                GDD_DrawText(hdc,item->szText,-1,&rc,DT_LEFT|DT_VCENTER|DT_BORDER|DT_BKGND);
            }
            else
            {
                GDD_SetTextColor(hdc,RGB(1,1,1));
                GDD_DrawText(hdc,item->szText,-1,&rc,DT_LEFT|DT_VCENTER);
            }

            GDD_OffsetRect(&rc,0,GDD_RectH(&rc));
            i++;
            n =n->next;

        }
    }

    GDD_EndPaint(hwnd,hdc);
    return true;
}

//列表框左键按下
static  bool_t    __Widget_ListBoxLbuttonDown(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    hwnd =pMsg->hwnd;
    s32 x,y,i;
    RECT rc,rc0;
    LISTBOX_ITEM *item;
    POINT pt;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    x=(s16)LO16(pMsg->Param2);
    y=(s16)HI16(pMsg->Param2);
    pt.x =x;
    pt.y =y;
    GDD_ScreenToClient(hwnd,&pt,1);
    pLB->xpos =x;
    pLB->ypos =y;
    GDD_GetClientRect(hwnd,&rc0);
    GDD_SetRect(&rc,0,0,GDD_RectW(&rc0),pLB->ItemHeight);
    item =__Widget_ListBoxGetItem(pLB,pLB->TopIndex);
    if(item!=NULL)
    {
        i=pLB->TopIndex;
        while(i<pLB->ItemNum)
        {
            if(rc.top>rc0.bottom)
            {
                break;
            }

            if(GDD_PtInRect(&rc,&pt))
            {
                __Widget_ListBoxSetCurSel(hwnd,pLB,i);
                break;
            }

            GDD_OffsetRect(&rc,0,GDD_RectH(&rc));
            i++;
        }
    }
    GDD_InvalidateWindow(hwnd,false);
    return true;
}
//列表框移动
static  bool_t    __Widget_ListBoxMouseMove(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    s32 x,y,i;
    x=(s16)LO16(pMsg->Param2);
    y=(s16)HI16(pMsg->Param2);

    if((y-pLB->ypos)>pLB->ItemHeight)
    {
        pLB->xpos =x;
        pLB->ypos =y;

        i=pLB->TopIndex;
        if(i>0)
        {
            i--;
        }

        __Widget_ListBoxSetTopIndex(pLB,i);
        return true;
    }

    if((y-pLB->ypos)<(-pLB->ItemHeight))
    {
        pLB->xpos =x;
        pLB->ypos =y;

        i=pLB->TopIndex;
        if(i<pLB->ItemNum)
        {
            i++;
        }
        __Widget_ListBoxSetTopIndex(pLB,i);
        GDD_InvalidateWindow(hwnd,false);
        return true;
    }

    return false;

}

//获取文本长度
static  ptu32_t __Widget_ListBoxGetTextLen(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    LISTBOX_ITEM *item;
    u32 idx;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    idx=pMsg->Param1;
    item =__Widget_ListBoxGetItem(pLB,idx);
    return strlen(item->szText)+1;

}

//获取文本
static  bool_t    __Widget_ListBoxGetText(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    LISTBOX_ITEM *item;
    char *buf;
    u32 idx;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    idx=pMsg->Param1;
    buf=(char*)pMsg->Param2;
    if(buf!=NULL)
    {
        item =__Widget_ListBoxGetItem(pLB,idx);
        if(item!=NULL)
        {
            strcpy(buf,item->szText);
            return true;
        }
        buf[0]='\0';
    }
    return false;
}

//默认进度条消息处理函数表，处理用户函数表中没有处理的消息。
static struct MsgProcTable s_gListBoxMsgProcTable[] =
{
    {MSG_LBUTTON_DOWN,__Widget_ListBoxLbuttonDown},
    {MSG_MOUSE_MOVE,__Widget_ListBoxMouseMove},
    {MSG_ListBox_ADDSTRING,__Widget_ListBoxAddString},
    {MSG_ListBox_DELSTRING,__Widget_ListBoxDelString},
    {MSG_ListBox_RESETCONTENT,__Widget_ListBoxResetContent},
    {MSG_ListBox_GETCOUNT,__Widget_ListBoxGetCount},
    {MSG_ListBox_SETCURSEL,__Widget_ListBoxSetCurSel},
    {MSG_ListBox_GETCURSEL,__Widget_ListBoxGetCurSel},
    {MSG_ListBox_SETTOPINDEX,__Widget_ListBoxSetTopIndex},
    {MSG_ListBox_GETTOPINDEX,__Widget_ListBoxGetTopIndex},
    {MSG_ListBox_SETITEMHEIGHT,__Widget_ListBoxSetItemHeight},
    {MSG_ListBox_GETITEMHEIGHT,__Widget_ListBoxGetItemHeight},
    {MSG_ListBox_SETITEMDATA,__Widget_ListBoxSetItemData},
    {MSG_ListBox_GETITEMDATA,__Widget_ListBoxGetItemData},
    {MSG_ListBox_GETTEXTLEN,__Widget_ListBoxGetTextLen},
    {MSG_ListBox_GETTEXT,__Widget_ListBoxGetText},
    {MSG_CREATE,__Widget_ListBoxCreate},
    {MSG_PAINT,__Widget_ListBoxPaint}
};


static struct MsgTableLink  s_gListBoxMsgLink;

HWND Widget_CreateListBox(  const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,ptu32_t pdata,
                    struct MsgTableLink *UserMsgTableLink)
{
    HWND pGddWin;
    s_gListBoxMsgLink.MsgNum = sizeof(s_gListBoxMsgProcTable) / sizeof(struct MsgProcTable);
    s_gListBoxMsgLink.myTable = (struct MsgProcTable *)&s_gListBoxMsgProcTable;
    pGddWin=GDD_CreateWindow(Text,WS_CHILD | WS_CAN_FOCUS|Style,x,y,w,h,hParent,WinId,
                            CN_WINBUF_PARENT,pdata,&s_gListBoxMsgLink);
    if(UserMsgTableLink != NULL)
        GDD_AddProcFuncTable(pGddWin,UserMsgTableLink);
    return pGddWin;
}

/*============================================================================*/
