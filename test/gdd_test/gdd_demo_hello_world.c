
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <gkernel.h>
#include <gdd.h>
#include <gdd_widget.h>

static HWND hwnd;
static RECT rc0;
#define ID_CLOSE    0x1000
static ptu32_t HmiCreate(struct WindowMsg *pMsg)
{

    hwnd=pMsg->hwnd;

    GDD_GetClientRect(hwnd,&rc0);
   //创建一个按钮。
    Widget_CreateButton("关闭",     //按钮名
                 WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,    //按钮风格
                 GDD_RectW(&rc0)/2,GDD_RectH(&rc0)/2-28,60,48,    //按钮位置和大小
                 hwnd,ID_CLOSE,NULL,NULL);   //按钮所属的父窗口，ID,附加数据
    return true;
}

static ptu32_t HmiNotify(struct WindowMsg *pMsg)
{
    HWND hwnd;
    u16 event,id;
    hwnd =pMsg->hwnd;

    event =HI16(pMsg->Param1);
    id =LO16(pMsg->Param1);

    if(event==MSG_BTN_UP && id==ID_CLOSE)
    {
        GDD_PostMessage(hwnd,MSG_CLOSE,0,0);
    }
    return true;
}
static ptu32_t HmiPaint(struct WindowMsg *pMsg)
{
    RECT rc0;
    HWND hwnd;
    hwnd=pMsg->hwnd;
    HDC hdc;
    hdc =GDD_BeginPaint(hwnd);  //开始绘图并获得绘图上下文(hdc)。
    GDD_GetClientRect(hwnd,&rc0);           //获得窗口客户区矩形
    GDD_SetFillColor(hdc,RGB(255,255,255)); //设置填充色。
    GDD_FillRect(hdc,&rc0);                 //填充矩形。
    GDD_SetTextColor(hdc,RGB(255,0,0));     //设置文字颜色。
    GDD_TextOut(hdc,100,100,"Hello World!",-1);  //在客户区10,20的位置输出文字。
    GDD_EndPaint(hwnd,hdc); //结束绘制并释放绘图上下文。
    return true;

}
//窗口关闭消息处理函数
static ptu32_t HmiClose(struct WindowMsg *pMsg)
{
    printf("win2_close.\r\n");
    return true;
}
//消息处理函数表
static struct MsgProcTable s_gHelloMsgTable[] =
{
    {MSG_CREATE,HmiCreate},         //创建消息
    {MSG_NOTIFY,HmiNotify},         //子控件发来的通知消息
    {MSG_PAINT,HmiPaint},           //绘制消息
    {MSG_CLOSE,HmiClose}      //窗口关闭消息
};

static struct MsgTableLink  s_gHelloDemoMsgLink;

void Hello_Word(void)
{
    s_gHelloDemoMsgLink.MsgNum = sizeof(s_gHelloMsgTable) / sizeof(struct MsgProcTable);
    s_gHelloDemoMsgLink.myTable = (struct MsgProcTable *)&s_gHelloMsgTable;
    GDD_CreateGuiApp("hello World", &s_gHelloDemoMsgLink, 0x800, CN_WINBUF_PARENT,WS_BORDER|WS_DLGFRAME|WS_CAPTION|WS_SYSMENU);
    GDD_WaitGuiAppExit("hello World");
}







