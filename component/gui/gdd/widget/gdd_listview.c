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
//�ļ�����: ���ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2016-02-15
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include    "gdd.h"
#include    "../gdd_private.h"
#include    <gdd_widget.h>

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

    u16 col_num;    //������
    u16 row_num;    //������
    u16 col_height; //�б�ͷ�߶�
    u16 row_width;  //�б�ͷ���
    u16 first_col_idx;  //��һ��������
    u16 first_row_idx;  //��һ��������
    list_cell_t *sel_cell;  //��ǰѡ���еĵ�Ԫ
    list_t list_col_hdr;    //�б�ͷ����
    list_t list_row_hdr;    //�б�������

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
        //todo:�ӿ�ָ���ж�
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
    //ע�����ﲻ�ܴ�pMsg->hwnd��ȡPrivateDataָ�룬��Ϊϵͳִ��close��hwnd�Ѿ�
    //��Ч��
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

            //��һ��cell
            n_col =n_col->next;
            rc.left += GDD_RectW(&rc);
            i++;

        }

        //��һ��Row.
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
    GDD_SetFillColor(hdc,RGB(220,220,220));
    GDD_SetDrawColor(hdc,RGB(160,160,200));

    //draw org hdr
    GDD_SetRect(&rc,0,0,pLV->row_width,pLV->col_height);
    rc0 =rc;

    GDD_DrawRect(hdc,&rc0);
    GDD_InflateRect(&rc0,-1,-1);
    GDD_Fill3DRect(hdc,&rc0,RGB(220,220,220),RGB(130,130,130));

    //GDD_GradientFillRect(hdc,&rc,MapRGB(hdc,240,240,240),MapRGB(hdc,160,160,180),TRUE);
    //GDD_DrawText(hdc," ",0,&rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_BORDER);

    //// �����б�ǩ
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

    //// �����б���
    x =0;
    y =pLV->col_height-1;

    lst =&pLV->list_row_hdr;
    n =Widget_FindIndexNode(lst,pLV->first_row_idx);
    while(n!=lst)
    {
        RECT rc0;

        row =(list_row_cel_t*)dListEntry(n,list_row_cel_t,node);

        //�����б�ǩ
        GDD_SetRect(&rc,x,y,pLV->row_width,row->Height);
        GDD_SetTextColor(hdc,RGB(0,0,0));
        GDD_SetFillColor(hdc,RGB(160,160,160));
        GDD_SetDrawColor(hdc,RGB(160,160,200));

        rc0 =rc;
        GDD_InflateRect(&rc0,-1,-1);
        GDD_Fill3DRect(hdc,&rc0,RGB(220,220,220),RGB(130,130,130));
        GDD_DrawText(hdc,row->Text,-1,&rc,DT_VCENTER|DT_CENTER|DT_BORDER);

        //���������еĳ�Ա��
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
                GDD_SetFillColor(hdc,cell->BackColor);
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
                    GDD_CopyRect(&rc,&rc0);
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

        //��һ��
        //GDD_OffsetRect(&rc,0,rc.h-1);
        y += GDD_RectH(&rc)-1;
        n =n->next;

    }
    GDD_EndPaint(hwnd,hdc);

    return true;
}


//Ĭ�ϸ�ѡ����Ϣ�������������û���������û�д������Ϣ��
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
    s_gListViewMsgLink.MsgNum = sizeof(s_gListViewMsgProcTable) / sizeof(struct MsgProcTable);
    s_gListViewMsgLink.myTable = (struct MsgProcTable *)&s_gListViewMsgProcTable;
    pGddWin=GDD_CreateWindow(Text,WS_CHILD | WS_CAN_FOCUS|Style,x,y,w,h,hParent,WinId,
                            CN_WINBUF_PARENT,pdata,&s_gListViewMsgLink);
    if(UserMsgTableLink != NULL)
        GDD_AddProcFuncTable(pGddWin,UserMsgTableLink);
    return pGddWin;
}



/*==================================================================*/



