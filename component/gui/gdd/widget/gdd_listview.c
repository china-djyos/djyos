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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

//所属模块: GDD
//作者:  LiuWei.
//版本：V1.0.0
//文件描述: 表格控件实现
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2016-02-15
//   作者:  LiuWei.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include    "gdd.h"
#include    "../gdd_private.h"
#include  <widget/gdd_listview.h>

typedef struct  __LVCOLUM
{
    const char *pszText;
    u32 Width;
}LVCOLUM;

typedef struct  __LVROW
{
    const char *pszText;
    u32 Height;
}LVROW;

typedef struct  tagNMLISTVIEW
{
    u16 ColumnIdx;
    u16 RowIdx;
}NMLISTVIEW;

/*=========================================================================================*/

#define LV_DEF_TEXTCOLOR    RGB(0,0,0)
#define LV_DEF_BACKCOLOR    RGB(240,240,240)
#define LV_DEF_BORDERCOLOR  RGB(100,100,100)

typedef struct  __list_item
{
    u32 Format;
    const char *pszText;
    u32 TextColor,BackColor,BorderColor;
    const void *pData;

}list_cell_t;

typedef struct  __list_col_cel
{
    list_t  node;
    const char *Text;
    u32 Width;

}list_col_cel_t;

typedef struct  __list_row_cel
{
    list_t  node;
    const char *Text;
    u32 Height;
    list_cell_t *cell_item;

}list_row_cel_t;


typedef struct  __listview{

    u16 col_num;    //列数量
    u16 row_num;    //行数量
    u16 col_height; //列表头高度
    u16 row_width;  //行表头宽度
    u16 first_col_idx;  //第一个列索引
    u16 first_row_idx;  //第一个行索引
    list_cell_t *sel_cell;  //当前选口中的单元
    list_t list_col_hdr;    //列表头链表
    list_t list_row_hdr;    //列表行链表

}listview_t;


////????
typedef struct  __LVINFO{

    u16 col_count;
    u16 row_count;
}LVINFO;


/*==================================================================*/

list_t* Widget_FindIndexNode(list_t *list,s32 idx)
{
    s32 i;
    list_t *n;

    i=0;
    n=list->next;
    while(1)
    {
        if(i==idx) break;
        if(n==list) break;
        n=n->next;
        i++;
    }

    return n;
}

static  void __Widget_ListViewReset(listview_t *pLV)
{

    pLV->col_num =0;
    pLV->row_num =0;
    pLV->col_height =20;
    pLV->row_width  =56;
    pLV->first_col_idx =0;
    pLV->first_row_idx =0;
    pLV->sel_cell =NULL;
    dListInit(&pLV->list_col_hdr);
    dListInit(&pLV->list_row_hdr);
}


static bool_t __Widget_ListViewCreate(struct WindowMsg *pMsg)
{
    listview_t *pLV;
    HWND hwnd;
    hwnd =pMsg->hwnd;
    pLV =(listview_t*)malloc(sizeof(listview_t));
    if(pLV==NULL)
    {
        return false;
    }
    memset(pLV, 0, sizeof(listview_t));
    __Widget_ListViewReset(pLV);
    GDD_SetWindowPrivateData(hwnd,(ptu32_t)pLV);
    return true;
}


static bool_t __Widget_ListViewAddCol(struct WindowMsg *pMsg)
{
    listview_t *pLV;
    HWND hwnd;
    u16 idx;
    LVCOLUM *lv_col;
    list_t *n;
    list_col_cel_t *col;
    hwnd =pMsg->hwnd;
    pLV =(listview_t*)GDD_GetWindowPrivateData(hwnd);
    idx=(u16)pMsg->Param1;
    lv_col=(LVCOLUM*)pMsg->Param2;
    col =(list_col_cel_t*)malloc(sizeof(list_col_cel_t));
    if(col!=NULL)
    {
        memset(col, 0, sizeof(list_col_cel_t));
        dListInit(&col->node);
        col->Text =lv_col->pszText;
        col->Width =lv_col->Width;

        n =Widget_FindIndexNode(&pLV->list_col_hdr,idx);
        dListInsertBefore(n,&col->node);

        pLV->col_num++;
        return true;
    }
    return false;
}



static bool_t __Widget_ListViewAddRow(struct WindowMsg *pMsg)
{
    listview_t *pLV;
    HWND hwnd;
    u16 idx;
    LVROW *lv_row;
    list_t *n;
    list_row_cel_t *row;
    s32 i;
    hwnd =pMsg->hwnd;
    pLV =(listview_t*)GDD_GetWindowPrivateData(hwnd);
    idx=(u16)pMsg->Param1;
    lv_row=(LVROW*)pMsg->Param2;
    row =(list_row_cel_t*)malloc(sizeof(list_row_cel_t));
    if(row!=NULL)
    {
        memset(row, 0, sizeof(list_row_cel_t));
        dListInit(&row->node);
        row->Text =lv_row->pszText;
        row->Height =lv_row->Height;

        n =Widget_FindIndexNode(&pLV->list_row_hdr,idx);
        dListInsertBefore(n,&row->node);
        pLV->row_num++;
        ////
        row->cell_item =(list_cell_t*)malloc(sizeof(list_cell_t)*pLV->col_num);
        //todo:加空指针判断
        memset(row->cell_item, 0, (sizeof(list_cell_t)*pLV->col_num));

        for(i=0;i<pLV->col_num;i++)
        {
            row->cell_item[i].Format =LVF_DEFAULT;
            row->cell_item[i].pszText =NULL;
            row->cell_item[i].TextColor =LV_DEF_TEXTCOLOR;
            row->cell_item[i].BackColor =LV_DEF_BACKCOLOR;
            row->cell_item[i].BorderColor =LV_DEF_BORDERCOLOR;
            row->cell_item[i].pData =NULL;
        }

        return true;
    }
    return false;
}


static bool_t __Widget_ListViewSetFirstColIdx(struct WindowMsg *pMsg)
{
    listview_t *pLV;
    HWND hwnd;
    u16 idx;
    hwnd =pMsg->hwnd;
    pLV =(listview_t*)GDD_GetWindowPrivateData(hwnd);
    idx=(u16)pMsg->Param1;
    if(idx>pLV->col_num)
    {
        idx =pLV->col_num-1;
    }
    pLV->first_col_idx =idx;
    GDD_InvalidateWindow(hwnd,false);
    return true;
}


static bool_t Widget_ListViewSetFirstRowIdx(struct WindowMsg *pMsg)
{
    listview_t *pLV;
    HWND hwnd;
    u16 idx;
    hwnd =pMsg->hwnd;
    pLV =(listview_t*)GDD_GetWindowPrivateData(hwnd);
    idx=(u16)pMsg->Param1;
    if(idx>pLV->row_num)
    {
        idx =pLV->row_num-1;
    }
    pLV->first_row_idx =idx;
    GDD_InvalidateWindow(hwnd,false);
    return true;
}

static void __Widget_ListViewSetFirstRowIdx(listview_t *pLV,u32 idx)
{
    if(idx>pLV->row_num)
    {
        idx =pLV->row_num-1;
    }
    pLV->first_row_idx =idx;
}
static bool_t __Widget_ListViewDeleteRow(struct WindowMsg *pMsg)
{
    listview_t *pLV;
    HWND hwnd;
    u16 idx;
    hwnd =pMsg->hwnd;
    pLV =(listview_t*)GDD_GetWindowPrivateData(hwnd);
    idx=(u16)pMsg->Param1;

    list_t *n;
    list_row_cel_t *row;

    n =Widget_FindIndexNode(&pLV->list_row_hdr,idx);
    if(n!=&pLV->list_row_hdr)
    {
        row =(list_row_cel_t*)dListEntry(n,list_row_cel_t,node);

        __Widget_ListViewSetFirstRowIdx(pLV,idx+1);
        dListRemove(&row->node);
        free(row->cell_item);
        free(row);
        pLV->row_num --;
        return true;
    }
    return false;
}


static bool_t Widget_ListViewDeleteAllRows(struct WindowMsg *pMsg)
{
    listview_t *pLV;
    HWND hwnd;
    hwnd =pMsg->hwnd;
    pLV =(listview_t*)GDD_GetWindowPrivateData(hwnd);
    list_t *lst,*n;
    lst =&pLV->list_row_hdr;
    n = lst->next;
    while(n!=lst)
    {
        list_row_cel_t *row;
        row =(list_row_cel_t*)dListEntry(n,list_row_cel_t,node);
        n =n->next;
        __Widget_ListViewSetFirstRowIdx(pLV,0);
        dListRemove(&row->node);
        free(row->cell_item);
        free(row);
    }
    pLV->row_num =0;
    return true;
}


static void __Widget_ListViewDeleteAllRows(listview_t *pLV)
{
    list_t *lst,*n;
    lst =&pLV->list_row_hdr;
    n = lst->next;

    while(n!=lst)
    {
        list_row_cel_t *row;
        row =(list_row_cel_t*)dListEntry(n,list_row_cel_t,node);
        n =n->next;
        __Widget_ListViewSetFirstRowIdx(pLV,0);
        dListRemove(&row->node);
        free(row->cell_item);
        free(row);
    }
    pLV->row_num =0;
}


static  bool_t __Widget_ListViewClose(struct WindowMsg *pMsg)
{
    listview_t *pLV;
    //注：这里不能从pMsg->hwnd中取PrivateData指针，因为系统执行close后，hwnd已经
    //无效。
    pLV = (listview_t *)pMsg->Param2;
    list_t *lst,*n;
    //destroy row
    __Widget_ListViewDeleteAllRows(pLV);

    //destroy col
    lst =&pLV->list_col_hdr;
    n =lst->next;
    while(n!=lst)
    {
        list_col_cel_t *col;

        col =(list_col_cel_t*)dListEntry(n,list_col_cel_t,node);
        n =n->next;

        free(col);

    }
    return true;
}

static bool_t __Widget_ListViewSetCell(struct WindowMsg *pMsg)
{

    listview_t *pLV;
    HWND hwnd;
    u16 idx;
    LVCELL *new;
    list_t *n;
    list_row_cel_t *row;
    list_cell_t *cell;
    u16 col_idx,row_idx;
    hwnd =pMsg->hwnd;
    pLV =(listview_t*)GDD_GetWindowPrivateData(hwnd);
    idx=(u16)pMsg->Param1;
    new=(LVCELL*)pMsg->Param2;
    row_idx =(idx>>16)&0xFFFF;
    col_idx =idx&0xFFFF;

    n =Widget_FindIndexNode(&pLV->list_row_hdr,row_idx);
    if(n==&pLV->list_row_hdr)
    {
        return false;
    }

    row =(list_row_cel_t*)dListEntry(n,list_row_cel_t,node);

    cell = &row->cell_item[col_idx];

    if(new->Mask&LVCF_FORMAT)
    {
        cell->Format =new->Format;
    }

    if(new->Mask&LVCF_TEXT)
    {
        cell->pszText =new->pszText;
    }

    if(new->Mask&LVCF_TEXTCOLOR)
    {
        cell->TextColor =new->TextColor;
    }

    if(new->Mask&LVCF_BACKCOLOR)
    {
        cell->BackColor =new->BackColor;
    }

    if(new->Mask&LVCF_BORDERCOLOR)
    {
        cell->BorderColor =new->BorderColor;
    }

    if(new->Mask&LVCF_DATA)
    {
        cell->pData =new->pData;
    }

    return true;
}


static bool_t __Widget_ListViewGetCell(struct WindowMsg *pMsg)
{
    listview_t *pLV;
    HWND hwnd;
    u16 idx;
    LVCELL *out;
    list_t *n;
    list_row_cel_t *row;
    list_cell_t *cell;
    u16 col_idx,row_idx;
    hwnd =pMsg->hwnd;
    pLV =(listview_t*)GDD_GetWindowPrivateData(hwnd);

    idx=(u16)pMsg->Param1;
    out=(LVCELL*)pMsg->Param2;

    row_idx =(idx>>16)&0xFFFF;
    col_idx =idx&0xFFFF;

    n =Widget_FindIndexNode(&pLV->list_row_hdr,row_idx);

    row =(list_row_cel_t*)dListEntry(n,list_row_cel_t,node);

    cell = &row->cell_item[col_idx];

    if(out->Mask&LVCF_FORMAT)
    {
        out->Format =cell->Format;
    }

    if(out->Mask&LVCF_TEXT)
    {
        out->pszText =cell->pszText;
    }

    if(out->Mask&LVCF_TEXTCOLOR)
    {
        out->TextColor =cell->TextColor;
    }

    if(out->Mask&LVCF_BACKCOLOR)
    {
        out->BackColor =cell->BackColor;
    }

    if(out->Mask&LVCF_BORDERCOLOR)
    {
        out->BorderColor =cell->BorderColor;
    }

    if(out->Mask&LVCF_DATA)
    {
        out->pData =cell->pData;
    }

    return true;
}



static bool_t  __Widget_ListViewLbuttonDown(struct WindowMsg *pMsg)
{
    listview_t *pLV;
    HWND hwnd;
    s32 pos_x,pos_y;
    hwnd =pMsg->hwnd;
    pLV =(listview_t*)GDD_GetWindowPrivateData(hwnd);
    pos_x=LO16(pMsg->Param2);
    pos_y=HI16(pMsg->Param2);

    POINT pt;
    RECT rc;
    s32 i,y,idx_row;
    list_t *lst_row,*n_row;
    list_t *lst_col,*n_col;
    list_col_cel_t *col;
    list_row_cel_t *row;

    pt.x =pos_x;
    pt.y =pos_y;

    idx_row = pLV->first_row_idx;
    y =pLV->col_height;

    lst_row =&pLV->list_row_hdr;
    n_row =Widget_FindIndexNode(lst_row,idx_row);
    while(n_row!=lst_row)
    {

        row =(list_row_cel_t*)dListEntry(n_row,list_row_cel_t,node);

        i =pLV->first_col_idx;

        lst_col =&pLV->list_col_hdr;
        n_col =Widget_FindIndexNode(lst_col,i);
        while(n_col!=lst_col)
        {
            col =(list_col_cel_t*)dListEntry(n_col,list_col_cel_t,node);

            GDD_SetRect(&rc,pLV->row_width,y,col->Width,row->Height);
            if(GDD_PtInRect(&rc,&pt))
            {
                list_cell_t *cell;

                cell =&row->cell_item[i];
                pLV->sel_cell =cell;
                GDD_PostMessage(GDD_GetWindowParent(hwnd),MSG_NOTIFY,(LVN_CLICK<<16)|(hwnd->WinId),(ptu32_t)hwnd);

            }

            //下一个cell
            n_col =n_col->next;
            rc.left += GDD_RectW(&rc);
            i++;

        }

        //下一个Row.
        n_row =n_row->next;
        y += row->Height;
        idx_row++;

        GDD_InvalidateWindow(hwnd,false);
    }

    return false;
}


static bool_t __Widget_ListViewPaint(struct WindowMsg *pMsg)
{
    listview_t *pLV;
    HWND hwnd;
    HDC hdc;
    hwnd =pMsg->hwnd;
    pLV =(listview_t*)GDD_GetWindowPrivateData(hwnd);
    hdc =GDD_BeginPaint(hwnd);
    if(hdc == NULL)
        return false;
    RECT rc,rc0;
    list_t *lst,*n;
    list_col_cel_t *col;
    list_row_cel_t *row;
    s32 x,y;
    ////

    GDD_SetTextColor(hdc,RGB(0,0,0));
    GDD_SetBackGroundColor(hdc,RGB(220,220,220));
    GDD_SetDrawColor(hdc,RGB(160,160,200));

    //draw org hdr
    GDD_SetRect(&rc,0,0,pLV->row_width,pLV->col_height);
    rc0 =rc;

    GDD_DrawRect(hdc,&rc0);
    GDD_InflateRect(&rc0,-1,-1);
    GDD_Fill3DRect(hdc,&rc0,RGB(220,220,220),RGB(130,130,130));

    //GDD_GradientFillRect(hdc,&rc,MapRGB(hdc,240,240,240),MapRGB(hdc,160,160,180),TRUE);
    //GDD_DrawText(hdc," ",0,&rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_BORDER);

    //// 绘制列标签
    GDD_OffsetRect(&rc,GDD_RectW(&rc)-1,0);

    lst =&pLV->list_col_hdr;
    n =Widget_FindIndexNode(lst,pLV->first_col_idx);
    while(n!=lst)
    {
        col =(list_col_cel_t*)dListEntry(n,list_col_cel_t,node);
        rc.right =rc.left+col->Width;

        GDD_GradientFillRect(hdc,&rc,RGB(240,240,240),RGB(160,160,180),CN_FILLRECT_MODE_UD);
        GDD_DrawText(hdc,col->Text,-1,&rc,DT_VCENTER|DT_CENTER|DT_BORDER);
        GDD_OffsetRect(&rc,GDD_RectW(&rc)-1,0);

        n =n->next;

    }

    //// 绘制列表行
    x =0;
    y =pLV->col_height-1;

    lst =&pLV->list_row_hdr;
    n =Widget_FindIndexNode(lst,pLV->first_row_idx);
    while(n!=lst)
    {
        RECT rc0;

        row =(list_row_cel_t*)dListEntry(n,list_row_cel_t,node);

        //绘制行标签
        GDD_SetRect(&rc,x,y,pLV->row_width,row->Height);
        GDD_SetTextColor(hdc,RGB(0,0,0));
        GDD_SetBackGroundColor(hdc,RGB(160,160,160));
        GDD_SetDrawColor(hdc,RGB(160,160,200));

        rc0 =rc;
        GDD_InflateRect(&rc0,-1,-1);
        GDD_Fill3DRect(hdc,&rc0,RGB(220,220,220),RGB(130,130,130));
        GDD_DrawText(hdc,row->Text,-1,&rc,DT_VCENTER|DT_CENTER|DT_BORDER);

        //绘制行所有的成员组
        if(1)
        {
            s32 i;
            u32 dt;

            list_t *lst,*n;
            list_col_cel_t *col;
            list_cell_t *cell;


            GDD_OffsetRect(&rc0,GDD_RectW(&rc)-1,0);

            i =pLV->first_col_idx;
            lst =&pLV->list_col_hdr;
            n =Widget_FindIndexNode(lst,i);
            while(n!=lst)
            {
                col =(list_col_cel_t*)dListEntry(n,list_col_cel_t,node);

                rc0.right =rc0.left+col->Width;

                cell =&row->cell_item[i];

                GDD_SetTextColor(hdc,cell->TextColor);
                GDD_SetBackGroundColor(hdc,cell->BackColor);
                GDD_SetDrawColor(hdc,RGB(160,160,200));

                dt =DT_BKGND|DT_BORDER;

                switch(cell->Format&LVF_ALIGN_V_MASK)
                {
                    case LVF_BOTTOM:
                        dt |= DT_BOTTOM;
                        break;

                    case LVF_VCENTER:
                        dt |= DT_VCENTER;
                        break;

                    case LVF_TOP:
                        dt |= DT_TOP;
                        break;
                }

                switch(cell->Format&LVF_ALIGN_H_MASK)
                {
                    case LVF_LEFT:
                        dt |= DT_LEFT;
                        break;

                    case LVF_CENTER:
                        dt |= DT_CENTER;
                        break;

                    case LVF_RIGHT:
                        dt |= DT_RIGHT;
                        break;
                }

                if(cell->pszText!=NULL)
                {
                    GDD_DrawText(hdc,cell->pszText,-1,&rc0,dt);
                }
                else
                {
                    //GDD_DrawText(hdc,L" ",0,&rc0,dt);
                }

                if(cell==pLV->sel_cell)
                {
                    RECT rc;
//                    GDD_CopyRect(&rc,&rc0);
                    rc = rc0;
                    GDD_InflateRect(&rc,-1,-1);
                    GDD_SetDrawColor(hdc,RGB(0,0,0));
                    GDD_DrawRect(hdc,&rc);
                }

                GDD_OffsetRect(&rc0,GDD_RectW(&rc0)-1,0);

                //Next row item;
                n =n->next;
                i++;
            }

        }

        //下一行
        //GDD_OffsetRect(&rc,0,rc.h-1);
        y += GDD_RectH(&rc)-1;
        n =n->next;

    }
    GDD_EndPaint(hwnd,hdc);

    return true;
}


//默认复选框消息处理函数表，处理用户函数表中没有处理的消息。
static struct MsgProcTable s_gListViewMsgProcTable[] =
{
    {MSG_CREATE,__Widget_ListViewCreate},
    {LVM_INSERTCOLUMN,__Widget_ListViewAddCol},
    {LVM_INSERTROW,__Widget_ListViewAddRow},
    {LVM_SETCELL,__Widget_ListViewSetCell},
    {LVM_GETCELL,__Widget_ListViewGetCell},
    {LVM_DELETEROW,__Widget_ListViewDeleteRow},
    {LVM_DELETEALLROWS,Widget_ListViewDeleteAllRows},
    {LVM_SETFIRSTCOLUMN,__Widget_ListViewSetFirstColIdx},
    {LVM_SETFIRSTROW,Widget_ListViewSetFirstRowIdx},
    {MSG_LBUTTON_DOWN,__Widget_ListViewLbuttonDown},
    {MSG_PAINT,__Widget_ListViewPaint},
    {MSG_CLOSE, __Widget_ListViewClose},
};

static struct MsgTableLink  s_gListViewMsgLink;

HWND Widget_CreateListView(  const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,ptu32_t pdata,
                    struct MsgTableLink *UserMsgTableLink)
{
    HWND pGddWin;
    if(hParent == NULL)
        hParent = GDD_GetDesktopWindow(NULL);
    //加锁后，GDD_GetMessage函数将不能立即取出消息，确保 GDD_AddProcFuncTable 函数
    //完成后，即消息处理函数表完整后再取出消息处理。
    if(__GDD_Lock())
    {
        s_gListViewMsgLink.MsgNum = sizeof(s_gListViewMsgProcTable) / sizeof(struct MsgProcTable);
        s_gListViewMsgLink.myTable = (struct MsgProcTable *)&s_gListViewMsgProcTable;
        pGddWin=GDD_CreateWindow(Text,&s_gListViewMsgLink, x,y,w,h,CN_WINBUF_PARENT,
                                 WS_CAN_FOCUS|Style,CN_SYS_PF_DISPLAY, CN_COLOR_WHITE,WinId,pdata,hParent);
        if(UserMsgTableLink != NULL)
            GDD_AddProcFuncTable(pGddWin,UserMsgTableLink);
        __GDD_Unlock();
        return pGddWin;
    }
    else
        return NULL;
}



/*==================================================================*/



