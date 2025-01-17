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
//作者:  zhb.
//版本：V1.0.0
//文件描述: 虚拟键盘控件实现
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2016-10-08
//   作者:  zhb.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//---------------------------------

#include  "gdd.h"
#include    "../gdd_private.h"
#include "font.h"
#include  <widget/gdd_keyboard.h>
#include  <widget/gdd_button.h>
#include "dbug.h"
static u8 gs_KeyBoardRowsDef=4;
static u8 gs_KeyBoardColsDef=4;
static u8 gs_KeySpaceDef=0;
static u8 gs_KeyVtimeLimitDef=10;
//static char *gs_KeyValSetsDef="123C456Ｘ789O000.R";

static u8 gs_KeyBoardRows;
static u8 gs_KeyBoardCols;
static u8 gs_KeySpace;
static u8 gs_KeyWidth,gs_KeyHeight;

static bool_t gs_bKeyDownFlag=false;
static char gs_CurKeyDown;
static s32 gs_KeyBoardId=0;





static HWND key_hwnd[16];


#define ID_KEY_0        0x6000
#define ID_KEY_1        0x6001
#define ID_KEY_2        0x6002
#define ID_KEY_3        0x6003
#define ID_KEY_4        0x6004
#define ID_KEY_5        0x6005
#define ID_KEY_6        0x6006
#define ID_KEY_7        0x6007
#define ID_KEY_8        0x6008
#define ID_KEY_9        0x6009
#define ID_KEY_10       0x600A
#define ID_KEY_11       0x600B
#define ID_KEY_12       0x600C
#define ID_KEY_13       0x600D
#define ID_KEY_14       0x600E
#define ID_KEY_15       0x600F



//-----------------------------------------------------------------------
//功能：虚拟键盘获得当前按下的键值
//参数：空
//返回：按下的键值ASCII值.
//-----------------------------------------------------------------------------
static u32 __Widget_VirKeyBoardRead(void)
{
      return (u32)gs_CurKeyDown;
}

//---------------------------------------------------------------------------
//功能：虚拟按键创建消息响应函数
//参数：pMsg，窗口消息指针
//返回：成功返回true,失败则返回false。
//-----------------------------------------------------------------------------
static bool_t __Widget_VirKeyBoardCreate(struct WindowMsg *pMsg)
{
    HWND hwnd;
    VirKeyBoard *pVKB;
    struct KeyBoardPrivate *pkbp;
    char *name;
    char buf[5];
    if(pMsg==NULL)
        return false;
    hwnd=pMsg->hwnd;
    if(hwnd==NULL)
        return false;
    if(pMsg->Param1==0)
    {
       pVKB=(VirKeyBoard *)malloc(sizeof(VirKeyBoard));
       if(pVKB==NULL)
           return false;
       memset(pVKB, 0, sizeof(VirKeyBoard));
       pkbp=(struct KeyBoardPrivate *)malloc(sizeof(struct KeyBoardPrivate));
       if(pkbp==NULL)
           return false;
       memset(pkbp, 0, sizeof(struct KeyBoardPrivate));
       pVKB->cols=gs_KeyBoardRowsDef;
       pVKB->rows=gs_KeyBoardColsDef;
       pVKB->keyspace=gs_KeySpaceDef;
       pVKB->id=gs_KeyBoardId;
       pkbp->key_bak=0;
       pkbp->key_now=0;
       pkbp->vtime_count=0;
       pkbp->vtime_limit=gs_KeyVtimeLimitDef;
       pkbp->read_keyboard=__Widget_VirKeyBoardRead;
       pVKB->pKBP=pkbp;

       GDD_SetWindowPrivateData(hwnd,(ptu32_t)pVKB);
       itoa(gs_KeyBoardId,buf,10);
       name="vir single touch keyboard";
       Keyboard_InstallDevice(name, pkbp);
       gs_KeyBoardId++;
    }
    else
    {

    }
    return true;
}
//----------------------------------------------------------------------------
//功能：虚拟键盘控件的MSG_PAINT消息响应函数
//参数：pMsg，窗口消息指针
//返回：成功返回true,失败则返回false。
//-----------------------------------------------------------------------------
static  bool_t __Widget_VirKeyBoardPaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT rc;
    u8 width,height;
    u8 LeftStart,HighStart;
    VBK vbk;
    if(pMsg==NULL)
        return false;
    hwnd=pMsg->hwnd;
    if(hwnd==NULL)
        return false;
    hdc =GDD_BeginPaint(hwnd);
    if(hdc==NULL)
        return false;
    vbk=(VBK)pMsg->Param1;
    if(vbk==NULL)
    {
        gs_KeyBoardRows=gs_KeyBoardRowsDef;
        gs_KeyBoardCols=gs_KeyBoardColsDef;
        gs_KeySpace=gs_KeySpaceDef;
    }
    else
    {
        gs_KeyBoardRows=vbk->rows;
        gs_KeyBoardCols=vbk->cols;
        gs_KeySpace=vbk->keyspace;
    }

    GDD_GetClientRect(hwnd,&rc);
    GDD_SetBackGroundColor(hdc,RGB(255,255,255));
    GDD_FillRectEx(hdc,&rc,hdc->BGColor);
    width=GDD_RectW(&rc);
    height=GDD_RectH(&rc);
    gs_KeyWidth=(width-(gs_KeyBoardCols+1)*gs_KeySpace)/gs_KeyBoardCols;
    gs_KeyHeight=(height-(gs_KeyBoardRows+1)*gs_KeySpace)/gs_KeyBoardRows;
    LeftStart=gs_KeySpace;
    HighStart=gs_KeySpace;
    //
    key_hwnd[0]=Widget_CreateButton("1",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,\
        LeftStart,HighStart,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_0,(ptu32_t)"1",NULL);

    key_hwnd[1]=Widget_CreateButton("2",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,\
        LeftStart+gs_KeyWidth,HighStart,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_1,(ptu32_t)"2",NULL);

    key_hwnd[2]=Widget_CreateButton("3",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,\
        LeftStart+2*gs_KeyWidth,HighStart,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_2,(ptu32_t)"3",NULL);

    key_hwnd[3]=Widget_CreateButton("x",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,\
        LeftStart+3*gs_KeyWidth,HighStart,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_3,(ptu32_t)"x",NULL);



    key_hwnd[4]=Widget_CreateButton("4",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart,
        HighStart+gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_4,(ptu32_t)"4",NULL);

    key_hwnd[5]=Widget_CreateButton("5",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+gs_KeyWidth,
        HighStart+gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_5,(ptu32_t)"5",NULL);

    key_hwnd[6]=Widget_CreateButton("6",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL, LeftStart+2*gs_KeyWidth,
        HighStart+gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_6,(ptu32_t)"6",NULL);

    key_hwnd[7]=Widget_CreateButton("√",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+3*gs_KeyWidth,
        HighStart+gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_7,(ptu32_t)"√",NULL);



    key_hwnd[8]=Widget_CreateButton("7",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart,
                 HighStart+2*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_8,(ptu32_t)"7",NULL);

    key_hwnd[9]=Widget_CreateButton("8",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+gs_KeyWidth,
                 HighStart+2*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_9,(ptu32_t)"8",NULL);

    key_hwnd[10]=Widget_CreateButton("9",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+2*gs_KeyWidth,
                 HighStart+2*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_10,(ptu32_t)"9",NULL);

    key_hwnd[11]=Widget_CreateButton("↑",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+3*gs_KeyWidth,
                 HighStart+2*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_11,(ptu32_t)"↑",NULL);



    key_hwnd[12]=Widget_CreateButton("0",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart,
                     HighStart+3*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_12,(ptu32_t)"0",NULL);

    key_hwnd[13]=Widget_CreateButton(".",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+gs_KeyWidth,
                 HighStart+3*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_13,(ptu32_t)".",NULL);

    key_hwnd[14]=Widget_CreateButton("R",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+2*gs_KeyWidth,
                 HighStart+3*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_14,(ptu32_t)"R",NULL);

    key_hwnd[15]=Widget_CreateButton("↓",BS_NORMAL|WS_BORDER|WS_VISIBLE|BS_SIMPLE | WS_UNFILL,LeftStart+3*gs_KeyWidth,
                 HighStart+3*gs_KeyHeight,gs_KeyWidth-1,gs_KeyHeight-1,hwnd,ID_KEY_15,(ptu32_t)"↓",NULL);

    if(key_hwnd[15]==NULL)
    {
        debug_printf("gdd","Create virtual keyboard failed.\r\n");
        return false;
    }

    GDD_EndPaint(hwnd,hdc);
    return true;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//-----------------------------------------------------------------------------
//功能：判断某个窗口是否能接受虚拟键盘输入，只有TextBox、RICHTEXTBOX、LISTBOX、LISTVIEW
//等可以接受虚拟键盘输入.
//参数：hwnd，窗口句柄.
//返回：是则返回true,否则返回false.
//-----------------------------------------------------------------------------
static bool_t __Widget_IsKeyBoradInputValidControl(HWND hwnd)
{
      return false;
}
#pragma GCC diagnostic pop

//-------------------------------------------------------------------------
//功能：虚拟键盘控件的MSG_KEY_DOWN消息响应函数
//参数：pMsg，窗口消息指针
//返回：成功返回true,失败则返回false。
//-----------------------------------------------------------------------------
static bool_t __Widget_VirKeyBoardKeyDown(struct WindowMsg *pMsg)
{
     u8 val;
     bool_t ret;
     HWND focus_hwnd=NULL;
     HWND hwnd;
     if(pMsg==NULL)
         return false;
     hwnd =pMsg->hwnd;
     if(hwnd==NULL)
         return false;
    //按下的键值
    val=pMsg->Param1;
    //查找当前焦点窗口
    focus_hwnd=GDD_GetFocusWindow();
    if(focus_hwnd==NULL)
        return false;
    ret=__Widget_IsKeyBoradInputValidControl(focus_hwnd);
    if(ret)
    {
         GDD_SendMessage( focus_hwnd, MSG_KEY_DOWN, val,  0);
         return true;
    }
    else
        return false;

}
//----------------------------------------------------------------------------
//功能：虚拟键盘控件的MSG_KEY_UP消息响应函数
//参数：pMsg，窗口消息指针
//返回：成功返回true,失败则返回false。
//-----------------------------------------------------------------------------
static bool_t __Widget_VirKeyBoardKeyUp(struct WindowMsg *pMsg)
{
    u8 val;
    bool_t ret;
    HWND focus_hwnd=NULL;
    HWND hwnd;
    if(pMsg==NULL)
         return false;
    hwnd =pMsg->hwnd;
    if(hwnd==NULL)
         return false;
    //按下的键值
    val=pMsg->Param1;
    //查找当前焦点窗口
    focus_hwnd=GDD_GetFocusWindow();
    if(focus_hwnd==NULL)
        return false;
    ret=__Widget_IsKeyBoradInputValidControl(focus_hwnd);
    if(ret)
    {
         GDD_SendMessage( focus_hwnd, MSG_KEY_UP, val,  0);
         return true;
    }
    else
        return false;
}

//---------------------------------------------------------------------------
//功能：获取虚拟键盘按下的键值.
//参数：x，按下点的x方向坐标;
//      y,按下点的y方向坐标;
//返回：按下的字符.
//-----------------------------------------------------------------------------
static char  __Widget_GetKeyDownVal(u16 x,u16 y)
{
    u8 i=0,j=0,idx=0;
    RECT *rect;
    POINT *pt;
    bool_t ret=false;
    HWND hwnd;
    char KeyVal;
    char *pdata;
    rect=malloc(sizeof(RECT));
    if(rect==NULL)
        return '\0';
    pt=malloc(sizeof(POINT));
    if(pt==NULL)
        return '\0';
    pt->x=x;
    pt->y=y;

    for(i=0;i<gs_KeyBoardRows;i++)
    {
        for(j=0;j<gs_KeyBoardCols;j++)
        {
           rect->left=(j*gs_KeyWidth)+(j+1)*gs_KeySpace;
           rect->top=(i*gs_KeyHeight)+(i+1)*gs_KeySpace;
           rect->right=rect->left+gs_KeyWidth;
           rect->bottom=rect->top+gs_KeyHeight;
           ret=GDD_PtInRect(rect,pt);
           if(ret==true)
               break;
        }
    }

    idx=i*gs_KeyBoardCols+j;
    hwnd=key_hwnd[idx];
    if(hwnd!=NULL)
    {
        pdata=(char *)GDD_GetWindowPrivateData(hwnd);
        KeyVal=*pdata;
        return KeyVal;
    }
    else
        return 0;

}

//------------------------------------------------------------------------------
//功能：虚拟键盘控件的MSG_TOUCH_DOWN消息响应函数
//参数：pMsg，窗口消息指针。
//返回：成功返回true,失败则返回false。
//-----------------------------------------------------------------------------
static bool_t __Widget_VirKeyBoardTouchDown(struct WindowMsg *pMsg)
{
     HWND hwnd;
     u32 loc;;
     u16 x,y;
     char keyval;
     if(pMsg==NULL)
         return false;
     hwnd =pMsg->hwnd;
     if(hwnd==NULL)
         return false;
     loc=pMsg->Param2;   //获取触摸点信息
     x=loc;
     y=loc>>16;
     //获取到哪个按键被按下
     keyval=__Widget_GetKeyDownVal(x,y);
     gs_bKeyDownFlag=true;
     gs_CurKeyDown=keyval;
     return true;
}

//----虚拟按键按下-------------------------------------------------------------
//功能：虚拟键盘控件的MSG_TOUCH_UP消息响应函数
//参数：pMsg，窗口消息指针。
//返回：成功返回true,失败则返回false。
//-----------------------------------------------------------------------------
static bool_t __Widget_VirKeyBoardTouchUp(struct WindowMsg *pMsg)
{
     HWND hwnd;
     u32 loc;;
     u16 x,y;
     char keyval;
     if(pMsg==NULL)
         return false;
     hwnd =pMsg->hwnd;
     if(hwnd==NULL)
         return false;
     if(!gs_bKeyDownFlag)
         return false;
     loc=pMsg->Param2;   //获取触摸点信息
     x=loc;
     y=loc>>16;
     //获取到哪个按键被按下
     keyval=__Widget_GetKeyDownVal(x,y);
     gs_bKeyDownFlag=false;
     gs_CurKeyDown=keyval;
     return true;
}

//-----------------------------------------------------------------
//功能：虚拟键盘控件MSG_NOTIFY消息响应函数
 //参数：pMsg，窗口消息指针。
 //返回：成功返回true,失败则返回false。
//-----------------------------------------------------------------------------
static bool_t __Widget_VirKeyBoardNotifyHandle(struct WindowMsg *pMsg)
{
     HWND hwnd,Hwnd;
     u16 event,id;
     char *pdata;
     char keyval;
     if(pMsg==NULL)
         return false;
     hwnd =pMsg->hwnd;
     if(hwnd==NULL)
         return false;
     event=HI16(pMsg->Param1);
     id=LO16(pMsg->Param1);

     id&=0xff;

     Hwnd=key_hwnd[id];
     pdata=(char *)GDD_GetWindowPrivateData(Hwnd);

     keyval=*pdata;



     if(event==MSG_BTN_UP)
     {
        GDD_PostMessage( hwnd, MSG_KEY_UP, (u32) keyval, 0);
     }
     else if(event==MSG_BTN_DOWN)
     {
        GDD_PostMessage( hwnd, MSG_KEY_DOWN, (u32) keyval, 0);
     }
     else
     {
        //  todo
     }
     return true;
}



//默认虚拟键盘消息处理函数表，处理用户函数表中没有处理的消息。
static struct MsgProcTable s_gVirKeyBoardMsgProcTable[] =
{
    {MSG_KEY_DOWN,__Widget_VirKeyBoardKeyDown},
    {MSG_KEY_UP, __Widget_VirKeyBoardKeyUp},
    {MSG_CREATE,__Widget_VirKeyBoardCreate},
    {MSG_PAINT,__Widget_VirKeyBoardPaint},
    {MSG_TOUCH_DOWN,__Widget_VirKeyBoardTouchDown},
    {MSG_TOUCH_UP,__Widget_VirKeyBoardTouchUp},
    {MSG_NOTIFY,__Widget_VirKeyBoardNotifyHandle}
};

static struct MsgTableLink  s_gVirKeyBoardMsgLink;
// =============================================================================
// 函数功能: 虚拟键盘控件创建函数。
// 输入参数: Text:虚拟键盘窗口Text;
//           Style:虚拟键盘风格，参见gdd.h;
//           x:虚拟键盘起始位置x方向坐标(单位：像素);
//           y:虚拟键盘起始位置y方向坐标(单位：像素);
//           w:虚拟键盘宽度(单位：像素);
//           h:虚拟键盘高度(单位：像素);
//           hParent:虚拟键盘父窗口句柄;
//           WinId:虚拟键盘控件Id;
//           pdata:虚拟键盘控件私有数据结构;
//           UserMsgTableLink:虚拟键盘控件用户消息列表结构指针。
// 输出参数: 无。
// 返回值  :成功则返回文本框句柄，失败则返回NULL。
// =============================================================================
HWND Widget_CreateVirKeyBoard(const char *Text,u32 Style,
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
        s_gVirKeyBoardMsgLink.MsgNum = sizeof(s_gVirKeyBoardMsgProcTable) / sizeof(struct MsgProcTable);
        s_gVirKeyBoardMsgLink.myTable = (struct MsgProcTable *)&s_gVirKeyBoardMsgProcTable;
        pGddWin=GDD_CreateWindow(Text,&s_gVirKeyBoardMsgLink, x,y,w,h,CN_WINBUF_PARENT,
                                 WS_CAN_FOCUS|Style,CN_SYS_PF_DISPLAY, CN_COLOR_WHITE,WinId,pdata,hParent);
        if(UserMsgTableLink != NULL)
            GDD_AddProcFuncTable(pGddWin,UserMsgTableLink);
        __GDD_Unlock();
        return pGddWin;
    }
    else
        return NULL;
}
