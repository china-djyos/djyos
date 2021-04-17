#include <stdint.h>
#include <stddef.h>
#include <gkernel.h>
#include <gdd.h>
#include <gdd_widget.h>

/*============================================================================*/
#define ID_CLOSE    0x1000
#define ID_BORDER   0x1100
#define ID_BKGND    0x1101
#define ID_COLOR    0x1102

static const struct   {
    const char *text;
    u32 flag;
}DrawText_Cfg_Tbl[]={
        {"正文左对齐+正文顶端对齐"           ,DT_LEFT|DT_TOP},
        {"正文左对齐+正文垂直居中"           ,DT_LEFT|DT_VCENTER},
        {"正文左对齐+正文底部对齐"           ,DT_LEFT|DT_BOTTOM},

        {"水平居中+正文顶端对齐"             ,DT_CENTER|DT_TOP},
        {"水平居中+正文垂直居中"             ,DT_CENTER|DT_VCENTER},
        {"水平居中+正文底部对齐"             ,DT_CENTER|DT_BOTTOM},

        {"水平右对齐+正文顶端对齐"            ,DT_RIGHT|DT_TOP},
        {"水平右对齐+正文垂直居中"            ,DT_RIGHT|DT_VCENTER},
        {"水平右对齐+正文底部对齐"            ,DT_RIGHT|DT_BOTTOM},

        {"多行显示1\r\n正文左对齐+正文顶端对齐"  ,DT_LEFT|DT_TOP},
        {"多行显示2\r\n正文左对齐+正文垂直居中"  ,DT_LEFT|DT_VCENTER},
        {"多行显示3\r\n正文左对齐+正文底部对齐"  ,DT_LEFT|DT_BOTTOM},

        {"多行显示4\r\n水平居中+正文顶端对齐"   ,DT_CENTER|DT_TOP},
        {"多行显示5\r\n水平居中+正文垂直居中"   ,DT_CENTER|DT_VCENTER},
        {"多行显示6\r\n水平右对齐+正文底部对齐"  ,DT_CENTER|DT_BOTTOM},

        {"多行显示7\r\n水平右对齐+正文顶端对齐"  ,DT_RIGHT|DT_TOP},
        {"多行显示8\r\n水平右对齐+垂直居中"     ,DT_RIGHT|DT_VCENTER},
        {"多行显示9\r\n水平右对齐+正文底部对齐"  ,DT_RIGHT|DT_BOTTOM},

        {NULL,0},
};

static const struct   {
    u32 text_color;
    u32 bd_color;
    u32 bk_color;
}DrawText_Color_Tbl[8]={

        {RGB(255,255,255),RGB(255,255,255),RGB(200,0,240)},
        {RGB(1,1,1),RGB(0,0,0),RGB(255,255,255)},
        {RGB(255,0,0),RGB(255,0,0),RGB(0,0,0)},
        {RGB(0,255,0),RGB(0,255,0),RGB(0,0,0)},
        {RGB(0,100,255),RGB(0,100,255),RGB(0,0,0)},
        {RGB(1,1,1),RGB(0,0,0),RGB(240,200,0)},
        {RGB(255,255,0),RGB(0,0,0),RGB(0,100,100)},
        {RGB(255,0,255),RGB(0,0,0),RGB(100,0,100)},
};

static  s8 cfg_idx=0;
static  s8 color_idx=0;
static  s8 bBorder=FALSE;
static  s8 bBKGND=FALSE;
static  s8 bColor=FALSE;
//static  ptu32_t ButtonL_Up(struct WindowMsg *pMsg)
//


#include <stdio.h>
//创建主窗口
static ptu32_t HmiCreate(struct WindowMsg *pMsg)
{
    HWND hwnd;
    RECT rc0;
    u32 i;
    struct WinTimer *timer;
    hwnd =pMsg->hwnd;
    cfg_idx =0;

    GDD_GetClientRect(hwnd,&rc0);
    Widget_CreateButton("关闭",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,GDD_RectW(&rc0)-64,GDD_RectH(&rc0)+4,60,24,hwnd,ID_CLOSE,NULL,NULL);
    i=GDD_RectH(&rc0)-(3*30);
    Widget_CreateButton("绘制边框",WS_CHILD|BS_HOLD|WS_BORDER|WS_VISIBLE,4,i+0*32,128,24,hwnd,ID_BORDER,NULL,NULL);
    Widget_CreateButton("绘制背景",WS_CHILD|BS_HOLD|WS_BORDER|WS_VISIBLE,4,i+1*32,128,24,hwnd,ID_BKGND,NULL,NULL);
    Widget_CreateButton("改变颜色",WS_CHILD|BS_HOLD|WS_BORDER|WS_VISIBLE,4,i+2*32,128,24,hwnd,ID_COLOR,NULL,NULL);
    timer = GDD_CreateTimer(hwnd,1,3000);
    GDD_StartTimer(timer);
    timer = GDD_CreateTimer(hwnd,2,1000);
    GDD_StartTimer(timer);
    return true;
}

//子控件发送的通知
static ptu32_t HmiNotify(struct WindowMsg *pMsg)
{
        HWND hwnd;
        u16 event,id;

        hwnd =pMsg->hwnd;

        event =HI16(pMsg->Param1);
        id =LO16(pMsg->Param1);

        switch (id)
        {
            case ID_CLOSE://关闭
                    if(event==MSG_BTN_UP)
                        {
                            GDD_PostMessage(hwnd,MSG_CLOSE,0,0);
                        }
                        GDD_InvalidateWindow(hwnd,FALSE);
                    break;
            case ID_BORDER ://绘制边框
                    if(event==CBN_SELECTED)
                        {
                            bBorder =TRUE;
                        }
                        if(event==CBN_UNSELECTED)
                        {
                            bBorder =FALSE;
                        }
                        GDD_InvalidateWindow(hwnd,FALSE);
                    break;
            case ID_BKGND://绘制背景
                    if(event==CBN_SELECTED)
                        {
                            bBKGND =TRUE;
                        }
                        if(event==CBN_UNSELECTED)
                        {
                            bBKGND =FALSE;
                        }
                        GDD_InvalidateWindow(hwnd,FALSE);
                    break;
            case ID_COLOR://改变颜色
                if(event==CBN_SELECTED)
                    {
                        bColor =TRUE;
                    }
                    if(event==CBN_UNSELECTED)
                    {
                        bColor =FALSE;
                    }
                    GDD_InvalidateWindow(hwnd,FALSE);
                    break;
            default:break;
        }
    return true;
}
//绘图消息
static ptu32_t HmiPaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc0,rc;
    u32 i;
    hwnd =pMsg->hwnd;

    hdc = GDD_BeginPaint(hwnd);
    GDD_GetClientRect(hwnd,&rc0);

    GDD_SetRect(&rc,2,10,GDD_RectW(&rc0)-2*2,90);
    GDD_SetFillColor(hdc,RGB(000,200,200));
    GDD_FillRect(hdc,&rc);

    GDD_SetTextColor(hdc,DrawText_Color_Tbl[color_idx].text_color);
    GDD_SetDrawColor(hdc,DrawText_Color_Tbl[color_idx].bd_color);
    GDD_SetFillColor(hdc,DrawText_Color_Tbl[color_idx].bk_color);

    i=DrawText_Cfg_Tbl[cfg_idx].flag;
    if(bBorder)
    {
        i |= DT_BORDER;
    }
    if(bBKGND)
    {
        i |= DT_BKGND;
    }
    GDD_DrawText(hdc,DrawText_Cfg_Tbl[cfg_idx].text,-1,&rc,i);
    GDD_EndPaint(hwnd,hdc);

    return true;
}
//窗口背景擦除

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

//定时器发送消息
static  u32 HmiTimer(struct WindowMsg *pMsg)
{

    HWND hwnd;
    hwnd =pMsg->hwnd;


    switch(pMsg->Param1)
       {
           case    1:
                   cfg_idx++;
                   if(DrawText_Cfg_Tbl[cfg_idx].text==NULL)
                   {
                       cfg_idx=0;
                   }
                   GDD_InvalidateWindow(hwnd,true);
                   break;
                   /////
           case    2:
                   if(bColor!=FALSE)
                   {
                       color_idx++;
                       if(color_idx>=8)
                       {
                           color_idx=0;
                       }
                       GDD_InvalidateWindow(hwnd,true);
                   }
                   break;
           default:break;
       }
     return  0;
}


//消息处理函数表
static struct MsgProcTable s_gDrawTextMsgTable[] =
{
    {MSG_CREATE,HmiCreate},         //主窗口创建消息
    {MSG_TIMER,HmiTimer},           //定时器消息
    {MSG_NOTIFY,HmiNotify},         //子控件发来的通知消息
    {MSG_PAINT,HmiPaint},           //绘制消息
    {MSG_ERASEBKGND,HmiErasebkgnd}, ///窗口背景擦除; Param1:绘图上下文;
};
static struct MsgTableLink  s_gDrawTextDemoMsgLink;

void    GDD_Demo_DrawText(void)
{
    s_gDrawTextDemoMsgLink.MsgNum = sizeof(s_gDrawTextMsgTable) / sizeof(struct MsgProcTable);
    s_gDrawTextDemoMsgLink.myTable = (struct MsgProcTable *)&s_gDrawTextMsgTable;
    GDD_CreateGuiApp("DrawText", &s_gDrawTextDemoMsgLink, 0x1000, CN_WINBUF_PARENT,WS_BORDER|WS_DLGFRAME|WS_CAPTION|WS_SYSMENU);
    GDD_WaitGuiAppExit("DrawText");
}
