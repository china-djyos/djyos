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
//文件描述: GDD内核私有头文件
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-11-10
//   作者:  LiuWei.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __GDD_PRIVATE_H__
#define __GDD_PRIVATE_H__

#if __cplusplus
extern "C" {
#endif

#include    "stddef.h"
#include    "stdio.h"
#include    "stdlib.h"
#include    "string.h"
#include    "math.h"
#include    "list.h"
#include    "systime.h"
#include    "lock.h"
#include    "djyos.h"
#include    "hmi-input.h"
#include    "gkernel.h"

#include    "gdd.h"

/*============================================================================*/

#define DEF_BORDER_SIZE     1   //默认的边框大小
#define DEF_DLGFRAME_SIZE   3   //默认的对话边框大小
#define DEF_CAPTION_SIZE    28  //默认的标题栏大小


//#define CN_DEF_FILL_COLOR           RGB(255,255,255)   //默认窗口填充颜色
//#define CN_DEF_DRAW_COLOR           RGB(0, 0, 0)  //默认窗口绘制颜色
//#define CN_DEF_TEXT_COLOR           RGB(0, 0, 0)  //默认窗口文本颜色

#define CN_DEF_FILL_COLOR           RGB(100, 20, 100)   //默认窗口填充颜色
#define CN_DEF_DRAW_COLOR           RGB(255, 255, 255)  //默认窗口绘制颜色
#define CN_DEF_TEXT_COLOR           RGB(255, 255, 255)  //默认窗口文本颜色


#define WINDOW_BORDER_COLOR         RGB(200,200,200)    //窗口边框颜色
#define WINDOW_DLGFRAME_COLOR1      RGB(70,70,100)      //窗口对话边框颜色1
#define WINDOW_DLGFRAME_COLOR2      RGB(70,70,220)      //窗口对话边框颜色2
#define WINDOW_DLGFRAME_COLOR3      RGB(70,70,90)       //窗口对话边框颜色3
#define WINDOW_CAPTION_TEXT_COLOR   RGB(255,255,0)    //窗口标题栏文字颜色


struct WinMsgQueueCB;

/*============================================================================*/

#define HDC2DC(hdc) (struct DC*)(hdc)
#define DC2HDC(dc)  (HDC)dc


/*============================================================================*/

#define DC_TYPE_PAINT   0
#define DC_TYPE_CLIENT  1
#define DC_TYPE_WINDOW  2
#define DC_TYPE_CAPTION 3

struct DC
{
    struct GkWinObj *pGkWin;
    struct FontObj *pFontDef;   //默认字体,这个值在创建DC时指定,后续将不可更改
    struct FontObj *pFont;      //当前字体,这个值在DC使用过程中,可以被更改
    struct Charset *pCharset;   //当前字符集
    struct Rectangle DrawArea;  //本DC的可绘制区，相对于窗口坐标
    HWND hwnd;                  //DC所属窗口
    u32 DCType;                 //DC类型
    s32 CurX,CurY;              //当前位置
    u32 DrawColor;              //绘制颜色
    u32 BGColor;              //背景颜色
    u32 TextColor;              //字体颜色
    u32 SyncTime;               //同步时间
    struct RopGroup RopCode;    //光栅码

};

#define WF_MAINWIN      (1<<0)  //这是个主窗口，有自己的消息队列和消息循环
                                //无此标志的窗口，使用父窗口的消息队列，由父窗口消息循环处理
#define WF_ERASEBKGND   (1<<1)  //擦除背景

struct WINDOW
{
    //窗口（一般是控件）的私有消息处理函数表。
    //本成员是实现窗口消息继承机制的关键
    struct MsgTableLink **MyMsgTableLink;
//    struct MsgTableLink *MyMsgTableLink;
//  list_t MsgProcFuncTable;
    char    *Text;
    struct MutexLCB *mutex_lock;        //窗口互斥锁，子窗口使用所属主窗口的锁
    struct WinMsgQueueCB *pMsgQ;        //窗口消息队列，子窗口使用所属主窗口的消息队列
    struct GkWinObj *pGkWin;   //继承GkWin
    ptu32_t PrivateData;      //窗口私有数据

    u16     EventID;        //窗口所属事件ID
    u16     WinId;          //窗口ID

    u8      BorderSize;     //边框大小，所有子窗口共享所属主窗口的。
    u8      CaptionSize;    //标题栏大小
    u16     Flag;           //窗口标记，例如： WF_ERASEBKGND
    u32     Style;          //窗口风格,高16位系统使用，例如：WS_CAPTION
                            //0~7 bit=控件类型，8~15 bit = 控件风格
//  RECT    CliRect;        //窗口客户区(使用屏幕坐标)
    RECT    CliRect;        //窗口客户区(改为窗口左上角为原点的坐标)
    u32     DrawColor;      //绘制颜色，创建子窗口时，将继承
    u32     BGColor;      //背景颜色，创建子窗口时，将继承
    u32     TextColor;      //字体颜色，创建子窗口时，将继承

    list_t  list_timer;         //窗口定时器链表
    list_t  node_msg_close;     //窗口CLOSE消息链表节点
    list_t  node_msg_ncpaint;   //窗口NCPAINT消息链表节点
    list_t  node_msg_paint;     //窗口PAINT消息链表节点
};

struct WinTimer
{
//    u32 Tag ;
    HWND hwnd;              //定时器所属窗口.
    u16 Id;                 //定时器Id.
//    u16 Flag;               //定时器标记，参看TMR_START等定义
    u32 Interval;           //定时间隔(单位:mS).
    s64 Alarm;             //下一次到点时间（uS）.
    list_t  node_sys;       //系统定时器链表节点.
    list_t  node_hwnd;      //所属窗口的定时器链表节点.
    list_t  node_msg_timer; //TIMER消息节点,当产生MSG_TIMER消息时,该节点插入到
                            //所属窗口的消息队列中的list_timer上.
};

#define CN_HMIINPUT_TIMER_ID     0xff01         //HMI输入扫描定时器
/*============================================================================*/
void    GDD_Init(void);

bool_t  __HWND_Lock(HWND hwnd);
void    __HWND_Unlock(HWND hwnd);
bool_t  __GDD_Lock(void);
void    __GDD_Unlock(void);
bool_t GDD_AdoptWin(HWND Hwnd ,HWND NewParent);
void    GDD_InitDC(HDC pdc,struct GkWinObj *gk_win,HWND hwnd,s32 dc_type);


bool_t  __GDD_PostMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd,u32 msg,u32 param1,ptu32_t param2);
void    __GDD_PostTimerMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd,struct WinTimer *ptmr);

struct WinMsgQueueCB*   __GUI_CreateMsgQ(s32 size);
void    __GUI_DeleteMsg(HWND hwnd);
void    __GUI_DeleteMsgQ(struct WinMsgQueueCB *pMsgQ);
void    __GDD_RemoveWindowTimer(HWND hwnd);
void    __GDD_DeleteMainWindowData(HWND hwnd);
void __GDD_DeleteChildWindowData(HWND hwnd);

struct WinMsgQueueCB *__GDD_GetWindowMsgQ(HWND hwnd);
bool_t __GDD_WinMsgProc(struct WindowMsg *pMsg);
u32         __GDD_GetWindowEvent(HWND hwnd);
ptu32_t GDD_GK_MessageLoop(void);

/*============================================================================*/


#if __cplusplus
}
#endif
#endif  /*__WDD_PRIVATE_H__*/
