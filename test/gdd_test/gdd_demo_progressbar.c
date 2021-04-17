
#include <stdint.h>
#include    "stdlib.h"
#include    "stdio.h"
#include    "djyos.h"
#include    "gdd.h"
#include <gdd_widget.h>
/*============================================================================*/
#define ID_CLOSE    0x1000
#define ID_SHOWTEXT 0x1100
#define ID_ORG      0x1101

HWND g_ptMainHwnd;

#define ID_PROGBAR1 0x1111
#define ID_PROGBAR2 0x1112

static int prog1_val=0;
static int prog1_inc=1;

static int prog2_val=0;
static int prog2_inc=1;

static PROGRESSBAR_DATA pb1;
static PROGRESSBAR_DATA pb2;
static char text_buf[128];

static  int  text_align_idx=0;
static  const u32 text_align[9]={
        DT_LEFT|DT_TOP,
        DT_LEFT|DT_VCENTER,
        DT_LEFT|DT_BOTTOM,

        DT_CENTER|DT_TOP,
        DT_CENTER|DT_VCENTER,
        DT_CENTER|DT_BOTTOM,

        DT_RIGHT|DT_TOP,
        DT_RIGHT|DT_VCENTER,
        DT_RIGHT|DT_BOTTOM,

};

//创建窗口信息
static ptu32_t HmiCreate(struct WindowMsg *pMsg)
{
    HWND hwnd;
    RECT rc0;
    HDC hdc;
    struct WinTimer *timer;
    hwnd =pMsg->hwnd;
    text_align_idx =0;

    hdc=GDD_BeginPaint(hwnd);
    GDD_GetClientRect(hwnd,&rc0);
    GDD_SetFillColor(hdc,RGB(250,250,250));
    GDD_FillRect(hdc,&rc0);

    prog1_val=0;
    prog2_val=0;

    prog1_inc=1;
    prog2_inc=1;

    pb1.Flag    =PBF_SHOWTEXT|PBF_ORG_LEFT;
    pb1.Range   =100;
    pb1.Pos     =prog1_val;
    pb1.FGColor =RGB(0,160,0);
    pb1.BGColor =RGB(10,10,10);
    pb1.TextColor =RGB(240,240,240);
    pb1.DrawTextFlag =DT_VCENTER|DT_CENTER;


    pb2.Flag    =PBF_SHOWTEXT|PBF_ORG_BOTTOM;
    pb2.Range   =100;
    pb2.Pos     =prog1_val;
    pb2.FGColor =RGB(200,0,0);
    pb2.BGColor =RGB(10,10,200);
    pb2.TextColor =RGB(1,1,1);
    pb2.DrawTextFlag =DT_VCENTER|DT_CENTER;

    GDD_GetClientRect(hwnd,&rc0);
    Widget_CreateButton("关闭",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,GDD_RectW(&rc0)-64,GDD_RectH(&rc0)-28,60,24,hwnd,ID_CLOSE,NULL,NULL);

    Widget_CreateProgressBar("水平进度条1",WS_CHILD|PBS_HOR|WS_VISIBLE,8,36,128,28,hwnd,ID_PROGBAR1,(ptu32_t)&pb1,NULL);
    Widget_CreateProgressBar("垂直进度条2",WS_CHILD|PBS_VER|WS_VISIBLE,160,36,28,128,hwnd,ID_PROGBAR2,(ptu32_t)&pb2,NULL);

    timer = GDD_CreateTimer(hwnd,1,3000);
    GDD_StartTimer(timer);
    timer = GDD_CreateTimer(hwnd,2,100);
    GDD_StartTimer(timer);

    return true;
}
//定时消息处理函数
static ptu32_t HmiTimer(struct WindowMsg *pMsg)
{
    HWND hwnd;
    hwnd =pMsg->hwnd;
    switch(pMsg->Param1)
    {
        case    1:
                {
                    HWND wnd;

                    text_align_idx++;
                    if(text_align_idx>=9)
                    {
                        text_align_idx=0;
                    }
                    pb1.DrawTextFlag =text_align[text_align_idx];
                    pb2.DrawTextFlag =text_align[text_align_idx];

                    wnd =GDD_GetDlgItem(hwnd,ID_PROGBAR1);
                    GDD_SendMessage(wnd,MSG_ProcessBar_SETDATA,(u32)&pb1,0);

                    wnd =GDD_GetDlgItem(hwnd,ID_PROGBAR2);
                    GDD_SendMessage(wnd,MSG_ProcessBar_SETDATA,(u32)&pb2,0);
                }
                break;
        case    2:
                {
                    HWND wnd;

                    if(prog1_val<=0)
                    {
                        prog1_val =0;
                        prog1_inc =1;
                    }
                    if(prog1_val>=100)
                    {
                        prog1_val =100;
                        prog1_inc =-1;
                    }
                    wnd =GDD_GetDlgItem(hwnd,ID_PROGBAR1);
                    pb1.Pos =prog1_val;
                    GDD_SendMessage(wnd,MSG_ProcessBar_SETDATA,(u32)&pb1,0);

                    printf(text_buf,"进度:%d...",pb1.Pos);
//                  SetWindowText(wnd,text_buf,1000);
                    prog1_val += prog1_inc;

                    if(prog2_val<=0)
                    {
                        prog2_val =0;
                        prog2_inc =1;
                    }
                    if(prog2_val>=100)
                    {
                        prog2_val =100;
                        prog2_inc =-1;
                    }
                    wnd =GDD_GetDlgItem(hwnd,ID_PROGBAR2);
                    pb2.Pos =prog2_val;
                    GDD_SendMessage(wnd,MSG_ProcessBar_SETDATA,(u32)&pb2,0);

                    printf(text_buf,"%d%%",pb2.Pos);
//                  SetWindowText(wnd,text_buf,1000);
                    prog2_val += prog2_inc;


                    GDD_InvalidateWindow(hwnd,FALSE);
                }
                break;
    }

    return true;
}
//子控件发送消息处理函数
static ptu32_t HmiNotify(struct WindowMsg *pMsg)
{
    HWND hwnd;
    u16 event,id;

    hwnd =pMsg->hwnd;
    event =HI16(pMsg->Param1);
    id =LO16(pMsg->Param1);
    switch(id)
    {
        case ID_CLOSE:
            if(event==MSG_BTN_UP)
            {
                GDD_PostMessage(hwnd,MSG_CLOSE,0,0);
            }
            break;


        case ID_SHOWTEXT:
            if(event==CBN_SELECTED)//复选框选中
            {
                pb1.Flag |= PBF_SHOWTEXT;
                pb2.Flag |= PBF_SHOWTEXT;
            }

            if(event==CBN_UNSELECTED)//复选框未选中
            {
                pb1.Flag &= ~PBF_SHOWTEXT;
                pb2.Flag &= ~PBF_SHOWTEXT;
            }
            break;
            ////
        case    ID_ORG:
             if(event==CBN_SELECTED)
             {
                pb1.Flag &= ~PBF_ORG_RIGHT;
                pb2.Flag &= ~PBF_ORG_BOTTOM;
             }

             if(event==CBN_UNSELECTED)
             {
                 pb1.Flag |= PBF_ORG_RIGHT;
                 pb2.Flag |= PBF_ORG_BOTTOM;
             }
             break;
    }
    return true;
}
//背景擦出消息处理函数
static ptu32_t HmiErasebkgnd(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc0;
    hwnd =pMsg->hwnd;
    hdc =(HDC)pMsg->Param1;
    GDD_GetClientRect(hwnd,&rc0);
    GDD_SetFillColor(hdc,RGB(200,200,200));
    GDD_FillRect(hdc,&rc0);
    return true;
}

//消息处理函数表
static struct MsgProcTable s_gProcessbarMsgTable[] =
{
    {MSG_CREATE,HmiCreate},         //主窗口创建消息
    {MSG_TIMER,HmiTimer},           //定时器消息
    {MSG_NOTIFY,HmiNotify},         //子控件发来的通知消息
    {MSG_ERASEBKGND,HmiErasebkgnd} //窗口背景擦除消息
};

static struct MsgTableLink  s_gProcessbarDemoMsgLink;
/*========================================================================================*/
void    GDD_Demo_Progressbar(void)
{
    s_gProcessbarDemoMsgLink.MsgNum = sizeof(s_gProcessbarMsgTable) / sizeof(struct MsgProcTable);
    s_gProcessbarDemoMsgLink.myTable = (struct MsgProcTable *)&s_gProcessbarMsgTable;
    GDD_CreateGuiApp("processbar", &s_gProcessbarDemoMsgLink, 0x800, CN_WINBUF_PARENT,WS_BORDER|WS_DLGFRAME|WS_CAPTION|WS_SYSMENU);
    GDD_WaitGuiAppExit("processbar");
}
