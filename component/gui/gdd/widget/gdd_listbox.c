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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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

//����ģ��: GDD
//����:  LiuWei.
//�汾��V1.0.0
//�ļ�����: �б��ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2015-03-25
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
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
//---------------�����б�-------------------------------------------------------
//���ܣ���
//������pMsg����Ϣָ��
//���أ��̶�true
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
         pLB->TopIndex =0;//��ͷ������ڼ���
         pLB->CurSel =-1;
         pLB->ItemHeight =20;//�и�
     }
     GDD_SetWindowPrivateData(hwnd,(ptu32_t)pMsg->Param1);
     return true;
}

//-----------------------------------------------------------------------
//���ܣ���ȡ�б���
//������pMsg����Ϣָ��
//���أ��б���
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
//����ַ���
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


//ɾ���ַ���
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


//��������
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

//���õ�ǰѡ����
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
//          GDD_InvalidateWindow(hwnd,false);   //��������Ϣ������ܵ��°�ť��ɾ����
//                                          //InvalidateWindow������SendMessage֮�����
            GDD_PostMessage(GDD_GetWindowParent(hwnd),MSG_NOTIFY,(LBN_SELCHANGE<<16)|((u16)hwnd->WinId),(ptu32_t)hwnd);
        }
    }
    GDD_InvalidateWindow(hwnd,false);   //��������Ϣ������ܵ��°�ť��ɾ����
                                    //InvalidateWindow������SendMessage֮�����
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

//��ȡ��ǰѡ��
static ptu32_t __Widget_ListBoxGetCurSel(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    return pLB->CurSel;
}


//��ȡ�б���
static ptu32_t __Widget_ListBoxGetCount(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    return pLB->ItemNum;
}

//���ñ�ͷָ��
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

//����ͷָ��
static ptu32_t __Widget_ListBoxSetTopIndex(LISTBOX_DATA *pLB,u32 idx)
{
    if(idx<pLB->ItemNum)
    {
        pLB->TopIndex=idx;
    }
    return pLB->TopIndex;
}
//����ͷָ��
static ptu32_t __Widget_ListBoxGetTopIndex(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    return pLB->TopIndex;
}

//��������и�
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
//ȡ������и�
static ptu32_t __Widget_ListBoxGetItemHeight(struct WindowMsg *pMsg)
{
    HWND hwnd;
    LISTBOX_DATA *pLB;
    hwnd =pMsg->hwnd;
    pLB=(LISTBOX_DATA*)GDD_GetWindowPrivateData(hwnd);
    return pLB->ItemHeight;
}

//�����������
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
//��ȡ�������
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
//���ڿͻ�������
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
        {   //�Ѿ������һ������.
            break;
        }

        if(i==pLB->TopIndex)
        {   //��TopIndex��ʼ����.
            break;
        }

        i++;
        n=n->next;
    }

    if(n!=&pLB->list)
    {
        GDD_SetRect(&rc,0,0,GDD_RectW(&rc0),pLB->ItemHeight);
        while(1)
        {   //ѭ�����Ʊ���.
            if(n == &pLB->list)
            {   //�Ѿ������һ������.
                break;
            }

            if(rc.top >= rc0.bottom)
            {   //�Ѿ����ڴ��ڷ�Χ��.
                break;
            }

            item =(LISTBOX_ITEM*)dListEntry(n,LISTBOX_ITEM,node);
            if(i == pLB->CurSel)
            {   //Ϊ��ǰѡ����Ŀ.
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

//�б���������
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
//�б���ƶ�
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

//��ȡ�ı�����
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

//��ȡ�ı�
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

//Ĭ�Ͻ�������Ϣ�������������û���������û�д������Ϣ��
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
