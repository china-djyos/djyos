
#include <stdint.h>
#include    "stdlib.h"
#include    "stdio.h"
#include    "djyos.h"
#include    "gdd.h"
#include <gdd_widget.h>

/*============================================================================*/
#define ID_CLOSE    0x1000

#define ID_START_1  0x1100
#define ID_START_2  0x1101
#define ID_START_3  0x1102
#define ID_START_4  0x1103

#define ID_CLR_1    0x1200
#define ID_CLR_2    0x1201
#define ID_CLR_3    0x1202
#define ID_CLR_4    0x1203

HWND g_ptMainHwnd;
static struct   {
    u32 text_color;
    u32 bd_color;
    u32 bk_color;
}DrawText_Color_Tbl[6]={

         {RGB(255,0,0),RGB(255,0,255),RGB(88,88,88)},
         {RGB(0,255,0),RGB(0,255,255),RGB(0,88,0)},
         {RGB(0,160,255),RGB(255,160,255),RGB(0,0,88)},
         {RGB(255,255,0),RGB(255,0,0),RGB(88,0,0)},
};

static  int cfg_idx=0;
static  int bBorder=TRUE;
static  int bBKGND=TRUE;
static  int bColor=TRUE;

const int timer_interval[]={
        50,
        200,
        500,
        1000,
};

static  int timer_count[8];
static  int timer_run[8];
//创建主窗口
static ptu32_t HmiCreate(struct WindowMsg *pMsg)
{
    HWND hwnd;
    RECT rc0;
    struct WinTimer *Timer;
    u32 i,x,y;
    hwnd=pMsg->hwnd;

    cfg_idx =0;
    bBorder=TRUE;
    bBKGND=TRUE;
    bColor=TRUE;

    for(i=0;i<8;i++)
    {
        timer_count[i] =0;
        timer_run[i]    =FALSE;
    }
    GDD_GetClientRect(hwnd,&rc0);
    Widget_CreateButton("关闭",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,GDD_RectW(&rc0)-64,GDD_RectH(&rc0)-28,60,24,hwnd,ID_CLOSE,NULL,NULL);
    GDD_ClientToScreen(hwnd,(POINT *)&rc0,2);
    y=20+rc0.top;
    x=128+8;
    Widget_CreateButton("run1",WS_CHILD|BS_HOLD|WS_VISIBLE,x,y+0*28,45,24,hwnd,ID_START_1,NULL,NULL);
    Widget_CreateButton("run2",WS_CHILD|BS_HOLD|WS_VISIBLE,x,y+1*28,45,24,hwnd,ID_START_2,NULL,NULL);
    Widget_CreateButton("run3",WS_CHILD|BS_HOLD|WS_VISIBLE,x,y+2*28,45,24,hwnd,ID_START_3,NULL,NULL);
    Widget_CreateButton("run4",WS_CHILD|BS_HOLD|WS_VISIBLE,x,y+3*28,45,24,hwnd,ID_START_4,NULL,NULL);

    x=128+8+49;
    Widget_CreateButton("reset1",WS_CHILD|WS_VISIBLE,x,y+0*28,45,24,hwnd,ID_CLR_1,NULL,NULL);
    Widget_CreateButton("reset2",WS_CHILD|WS_VISIBLE,x,y+1*28,45,24,hwnd,ID_CLR_2,NULL,NULL);
    Widget_CreateButton("reset3",WS_CHILD|WS_VISIBLE,x,y+2*28,45,24,hwnd,ID_CLR_3,NULL,NULL);
    Widget_CreateButton("reset4",WS_CHILD|WS_VISIBLE,x,y+3*28,45,24,hwnd,ID_CLR_4,NULL,NULL);

    GDD_CreateTimer(hwnd,0,timer_interval[0]);
    GDD_CreateTimer(hwnd,1,timer_interval[1]);
    GDD_CreateTimer(hwnd,2,timer_interval[2]);
    GDD_CreateTimer(hwnd,3,timer_interval[3]);
    Timer = GDD_CreateTimer(hwnd,7,500);
    GDD_StartTimer(Timer);
    return true;
}
//定时器处理函数
static ptu32_t HmiTimer(struct WindowMsg *pMsg)
{
    u32 i;
    HWND hwnd;
    hwnd =pMsg->hwnd;
    i =pMsg->Param1&0xFF;
    timer_count[i]++;
    GDD_InvalidateWindow(hwnd,FALSE);

    return true;
}
//子控件通知消息处理函数
static ptu32_t HmiNotify(struct WindowMsg *pMsg)
{
    HWND hwnd;

    hwnd =pMsg->hwnd;
    u16 event,id;
    u32 i;
    event =HI16(pMsg->Param1);
    id =LO16(pMsg->Param1);

    switch(event)
    {
        case MSG_BTN_DOWN:  //按钮按下
            if(id>=ID_START_1 && id<=ID_START_4)
            {
                i =id&0xF;
                timer_run[i] =TRUE;
                GDD_ResetTimer(GDD_FindTimer(hwnd,i),timer_interval[i]);
            }

            if(id>=ID_CLR_1 && id<=ID_CLR_4)
            {
                i =id&0xF;
                timer_count[i] =0;
                GDD_InvalidateWindow(hwnd,TRUE);
            }
            break;
            ////

        case MSG_BTN_UP:    //按钮弹起
            if(id==ID_CLOSE)
            {
                GDD_PostMessage(hwnd,MSG_CLOSE,0,0);
            }

            if(id>=ID_START_1 && id<=ID_START_4)
            {
                i =id&0xF;
                timer_run[i] =FALSE;
                GDD_StopTimer(GDD_FindTimer(hwnd,i));
            }
            break;
        default: break;
    }
    return true;
}
//背景擦出消息处理函数
static ptu32_t HmiErasebkgnd(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT rc;
    hwnd =pMsg->hwnd;
    hdc =(HDC)pMsg->Param1;

    GDD_GetClientRect(hwnd,&rc);
    GDD_SetFillColor(hdc,RGB(100,100,100));
    GDD_FillRect(hdc,&rc);

    return true;
}
//绘图消息处理函数
static ptu32_t HmiPaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc;
    char wbuf[32];
    u32 i;

    hwnd =pMsg->hwnd;
    hdc =GDD_BeginPaint(hwnd);
    GDD_SetRect(&rc,4,20,128,24);
    for(i=0;i<4;i++)
    {
        GDD_SetTextColor(hdc,DrawText_Color_Tbl[i].text_color);
        GDD_SetDrawColor(hdc,DrawText_Color_Tbl[i].bd_color);
        GDD_SetFillColor(hdc,DrawText_Color_Tbl[i].bk_color);
        sprintf(wbuf," time %d: %d",i+1,timer_count[i]);

        if(timer_run[i]!=FALSE)
        {
            GDD_DrawText(hdc,wbuf,-1,&rc,DT_LEFT|DT_VCENTER|DT_BORDER|DT_BKGND);
        }
        else
        {
            if(timer_count[7]&0x01)
            {
                GDD_DrawText(hdc,wbuf,-1,&rc,DT_LEFT|DT_VCENTER|DT_BORDER|DT_BKGND);

            }
            else
            {
                GDD_DrawText(hdc,wbuf,-1,&rc,DT_LEFT|DT_VCENTER|DT_BKGND);
            }
        }

        GDD_OffsetRect(&rc,0,28);

    }
    GDD_EndPaint(hwnd,hdc);
    return true;
}
//消息处理函数表
static struct MsgProcTable s_gTimerMsgTable[] =
{
    {MSG_CREATE,HmiCreate},         //主窗口创建消息
    {MSG_TIMER,HmiTimer},           //定时器消息
    {MSG_NOTIFY,HmiNotify},         //子控件发来的通知消息
    {MSG_ERASEBKGND,HmiErasebkgnd}, //背景擦除
    {MSG_PAINT,HmiPaint},           //绘制消息
};

static struct MsgTableLink  s_gTimerDemoMsgLink;
/*============================================================================*/

void    GDD_Demo_Timer(void)
{
    s_gTimerDemoMsgLink.MsgNum = sizeof(s_gTimerMsgTable) / sizeof(struct MsgProcTable);
    s_gTimerDemoMsgLink.myTable = (struct MsgProcTable *)&s_gTimerMsgTable;
    GDD_CreateGuiApp("Timer", &s_gTimerDemoMsgLink, 0x800, CN_WINBUF_PARENT,WS_BORDER|WS_DLGFRAME|WS_CAPTION|WS_SYSMENU);
    GDD_WaitGuiAppExit("Timer");
}
