
#include <stdint.h>
#include    "stdlib.h"
#include    "stdio.h"
#include    "djyos.h"
#include    "gdd.h"
#include <gdd_widget.h>


/*============================================================================*/
#define ID_CLOSE    0x1000
#define ID_MOVE_1   0x1100
#define ID_MOVE_2   0x1101

static  HWND hwnd_BTN1=NULL;
static  HWND hwnd_BTN2=NULL;
static  int bMoveMainWin;

static  int x1_inc,y1_inc;
static  int x2_inc,y2_inc;

//开始创建窗口消息处理函数
static ptu32_t HmiCreateMove(struct WindowMsg  *pMsg)
{
    HWND hwnd;
    RECT rc;
    struct WinTimer *timer;
    hwnd =pMsg->hwnd;

    x1_inc =2;
    y1_inc =1;
    x2_inc =2;
    y2_inc =1;
    bMoveMainWin=FALSE;
    GDD_GetClientRect(hwnd,&rc);
    hwnd_BTN1 =Widget_CreateButton("关闭1",WS_CHILD|WS_BORDER|WS_VISIBLE,GDD_RectW(&rc)-64,GDD_RectH(&rc)-28,60,24,hwnd,ID_CLOSE,NULL,NULL);
    hwnd_BTN2 =Widget_CreateButton("关闭2",WS_CHILD|WS_BORDER|WS_VISIBLE,10,34,100,50,hwnd,ID_CLOSE,NULL,NULL);

    Widget_CreateButton("移动主窗口",WS_CHILD|BS_HOLD|WS_VISIBLE|WS_BORDER,4,GDD_RectH(&rc)-28,100,54,hwnd,ID_MOVE_1,NULL,NULL);
    timer = GDD_CreateTimer(hwnd,0,200);
    GDD_StartTimer(timer);
    timer = GDD_CreateTimer(hwnd,1,150);
    GDD_StartTimer(timer);
    timer = GDD_CreateTimer(hwnd,2,100);
    GDD_StartTimer(timer);

    return true;
}

//定时器消息处理函数
static ptu32_t HmiTimerMove(struct WindowMsg  *pMsg)
{
    HWND hwnd;
    RECT rc,rc0;
    hwnd =pMsg->hwnd;
        switch(pMsg->Param1)
        {
        case 0:
            {
                static int x_inc=2;
                static int y_inc=1;

                if(bMoveMainWin==FALSE)
                {
                    break;
                }

                GDD_GetWindowRect(hwnd,&rc);

                if(rc.left > 200)
                {
                    x_inc =-3;
                }

                if(rc.left <- 100)
                {
                    x_inc =3;
                }

                if(rc.top > 200)
                {
                    y_inc =-2;
                }

                if(rc.top < -150)
                {
                    y_inc =2;
                }

                rc.left+=x_inc;
                rc.top+=y_inc;
                GDD_MoveWindow(hwnd,rc.left,rc.top);//相对父窗口客户区的位置
            }
            break;
        case 1:
        {

            GDD_GetClientRect(hwnd,&rc0);
            GDD_ClientToScreen(hwnd,(POINT*)&rc0,2);

            GDD_GetWindowRect(hwnd_BTN1,&rc);

            if((rc.right) >= (rc0.right))
            {
                x1_inc =-4;
            }

            if(rc.left <= rc0.left)
            {
                x1_inc =4;
            }

            if((rc.bottom) >= (rc0.bottom))
            {
                y1_inc =-5;
            }

            if(rc.top <= rc0.top)
            {
                y1_inc =5;
            }

            rc.left +=x1_inc;
            rc.top +=y1_inc;
//            GDD_ScreenToWindow(hwnd,(POINT*)&rc,2);
            GDD_ScreenToClient(hwnd,(POINT*)&rc,2);
            GDD_MoveWindow(hwnd_BTN1,rc.left,rc.top);//相对父窗口客户区的位置
        }break;

        case 2:
        {
            GDD_GetClientRect(hwnd,&rc0);
            GDD_ClientToScreen(hwnd,(POINT*)&rc0,2);//相对屏幕坐标

            GDD_GetWindowRect(hwnd_BTN2,&rc);//相对屏幕坐标
            if((rc.right) >= (rc0.right))
            {
                x2_inc =-7;
            }

            if(rc.left <= rc0.left)
            {
                x2_inc =7;
            }

            if((rc.bottom) >= (rc0.bottom))
            {
                y2_inc =-4;
            }

            if(rc.top <= rc0.top)
            {
                y2_inc =4;
            }

            rc.left +=x2_inc;
            rc.top +=y2_inc;
//            GDD_ScreenToWindow(hwnd,(POINT*)&rc,2);
            GDD_ScreenToClient(hwnd,(POINT*)&rc,2);//屏幕坐标转换为客户区
            GDD_MoveWindow(hwnd_BTN2,rc.left,rc.top);//相对父窗口客户区的位置
        }
        break;
        default:
            break;
        }

        GDD_InvalidateWindow(hwnd,TRUE);

    return true;
}
//子控件发来的消息处理函数
static ptu32_t HmiNotifyMove(struct WindowMsg  *pMsg)
{
    HWND hwnd;
    u16 event,id;

    hwnd =pMsg->hwnd;
    event =HI16(pMsg->Param1);
    id =LO16(pMsg->Param1);
    switch(event)
    {
        case MSG_BTN_DOWN:  //按钮按下
            if(id==ID_MOVE_1)
            {
                bMoveMainWin=TRUE;
            }
            break;
        case MSG_BTN_UP:    //按钮弹起
            if(id==ID_CLOSE)
            {
                GDD_PostMessage(hwnd,MSG_CLOSE,0,0);
            }

            if(id==ID_MOVE_1)
            {
                bMoveMainWin=FALSE;
            }
            break;
        default:
            break;
    }
    return true;
}
//绘制消息处函数
static ptu32_t HmiPaintMove(struct WindowMsg  *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc,rc0;
    char wbuf[32];

    hwnd =pMsg->hwnd;
    hdc =GDD_BeginPaint(hwnd);

    GDD_GetClientRect(hwnd,&rc0);
    GDD_SetTextColor(hdc,RGB(0,240,0));
    GDD_SetFillColor(hdc,RGB(40,40,40));
    GDD_SetDrawColor(hdc,RGB(0,240,0));

    GDD_GetWindowRect(hwnd,&rc);
    sprintf(wbuf,"当前主窗口位置: %d,%d",rc.left,rc.top);
    GDD_DrawText(hdc,wbuf,-1,&rc0,DT_LEFT|DT_VCENTER|DT_BORDER|DT_BKGND);
    GDD_EndPaint(hwnd,hdc);
    return true;
}

//消息处理函数表
static struct MsgProcTable s_gMoveWinMsgTable[] =
{
    {MSG_CREATE,HmiCreateMove},         //主窗口创建消息
    {MSG_TIMER,HmiTimerMove},           //定时器消息
    {MSG_NOTIFY,HmiNotifyMove},         //子控件发来的通知消息
    {MSG_PAINT,HmiPaintMove},           //绘制消息
};

static struct MsgTableLink  s_gMoveWinDemoMsgLink;

/*============================================================================*/

void    GDD_Demo_MoveWindow(void)
{
    s_gMoveWinDemoMsgLink.MsgNum = sizeof(s_gMoveWinMsgTable) / sizeof(struct MsgProcTable);
    s_gMoveWinDemoMsgLink.myTable = (struct MsgProcTable *)&s_gMoveWinMsgTable;
    GDD_CreateGuiApp("move window", &s_gMoveWinDemoMsgLink, 0x800, CN_WINBUF_PARENT,WS_BORDER|WS_DLGFRAME|WS_CAPTION|WS_SYSMENU);
    GDD_WaitGuiAppExit("move window");
}
