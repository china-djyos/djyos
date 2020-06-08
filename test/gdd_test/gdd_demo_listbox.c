
#include <stdint.h>
#include    "stdlib.h"
#include    "stdio.h"
#include    "djyos.h"
#include    "gdd.h"
#include <gdd_widget.h>

/*============================================================================*/
#define ID_CLOSE    0x1000
#define ID_LEFT     0x1100
#define ID_RIGHT    0x1101


#define ID_LISTBOX1 0x1111
#define ID_LISTBOX2 0x1112


static HWND hwndLB1=NULL;
static HWND hwndLB2=NULL;

//创建主窗口
static ptu32_t HmiCreate(struct WindowMsg *pMsg)
{
    HWND hwnd;
    struct WinTimer *timer;
    RECT rc,rc0;
    hwnd=pMsg->hwnd;

    GDD_GetClientRect(hwnd,&rc0);
    Widget_CreateButton("关闭",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,GDD_RectW(&rc0)-64,GDD_RectH(&rc0)-10,60,24,hwnd,ID_CLOSE,NULL,NULL);

    GDD_SetRect(&rc,0,10,107,190);
    GDD_ClientToScreen(hwnd,(POINT*)&rc,2);
    GDD_ScreenToWindow(hwnd,(POINT*)&rc,2);
    hwndLB1=Widget_CreateListBox("列表框1",WS_CHILD|WS_BORDER|WS_VISIBLE|WS_CAPTION,
                            rc.left,rc.top,GDD_RectW(&rc),GDD_RectH(&rc),
                            hwnd,ID_LISTBOX1,NULL,NULL);
    Widget_CreateButton("-->",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,rc.right-50,rc.bottom+4,50,20,hwnd,ID_RIGHT,NULL,NULL);

    GDD_OffsetRect(&rc,GDD_RectW(&rc)+8,0);
    hwndLB2=Widget_CreateListBox("列表框2",WS_CHILD|WS_BORDER|WS_VISIBLE|WS_CAPTION,
                        rc.left,rc.top,GDD_RectW(&rc),GDD_RectH(&rc),
                        hwnd,ID_LISTBOX2,NULL,NULL);
    Widget_CreateButton("<--",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,rc.left,rc.bottom+4,50,20,hwnd,ID_LEFT,NULL,NULL);


    GDD_SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,0,(u32)"ListItem-0");
    GDD_SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,1,(u32)"ListItem-1");
    GDD_SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,2,(u32)"ListItem-2");
    GDD_SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,3,(u32)"ListItem-3");
    GDD_SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,4,(u32)"ListItem-4");
    GDD_SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,5,(u32)"ListItem-5");
    GDD_SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,6,(u32)"ListItem-6");
    GDD_SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,7,(u32)"ListItem-7");
    GDD_SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,8,(u32)"ListItem-8");
    GDD_SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,9,(u32)"ListItem-9");
    GDD_SendMessage(hwndLB1,MSG_ListBox_SETCURSEL,3,0);

    GDD_SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,0,(u32)"ListItem-10");
    GDD_SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,1,(u32)"ListItem-11");
    GDD_SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,2,(u32)"ListItem-12");
    GDD_SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,3,(u32)"ListItem-13");
    GDD_SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,4,(u32)"ListItem-14");
    GDD_SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,5,(u32)"ListItem-15");
    GDD_SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,6,(u32)"ListItem-16");
    GDD_SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,7,(u32)"ListItem-17");
    GDD_SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,8,(u32)"ListItem-18");
    GDD_SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,9,(u32)"ListItem-19");
    GDD_SendMessage(hwndLB2,MSG_ListBox_SETCURSEL,3,0);

    timer = GDD_CreateTimer(hwnd,1,3000);
    GDD_StartTimer(timer);
    timer = GDD_CreateTimer(hwnd,2,100);
    GDD_StartTimer(timer);

    return true;
}
//定时器处理函数
static ptu32_t HmiTimer(struct WindowMsg *pMsg)
{

    switch(pMsg->Param1)
    {
        case    1: break;

        case    2: break;
        default:break;
    }
    return true;
}

//子控件通知消息处理函数
static ptu32_t HmiNotify(struct WindowMsg *pMsg)
{
    HWND hwnd;
    u16 event,id;
    hwnd =pMsg->hwnd;
    event =HI16(pMsg->Param1);
    id =LO16(pMsg->Param1);

    switch (id)
    {
        case ID_CLOSE:
            if(event==MSG_BTN_UP )
                GDD_PostMessage(hwnd,MSG_CLOSE,0,0);
            break;
        case ID_RIGHT://-->
            if(event==MSG_BTN_UP )
                {
                    char *buf;
                    int i_1,i_2,n;
                    i_1 =GDD_SendMessage(hwndLB1,MSG_ListBox_GETCURSEL,0,0);//获取当前选中项
                    i_2 =GDD_SendMessage(hwndLB2,MSG_ListBox_GETCURSEL,0,0);

                    if(i_1>=0)
                    {
                        buf =(char*)malloc(GDD_SendMessage(hwndLB1,MSG_ListBox_GETTEXTLEN,i_1,0));
                        if(buf!=NULL)
                        {
                            GDD_SendMessage(hwndLB1,MSG_ListBox_GETTEXT,i_1,(u32)buf);//获取字符串
                            GDD_SendMessage(hwndLB1,MSG_ListBox_DELSTRING,i_1,0);     //删除一个字符
                            n=GDD_SendMessage(hwndLB1,MSG_ListBox_GETCOUNT,0,0);//所有项个数
                            if((n-1)<i_1)   i_1--;
                            GDD_SendMessage(hwndLB1,MSG_ListBox_SETCURSEL,i_1,0);     //设置当前选项



                            GDD_SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,i_2,(u32)buf);//添加一个字符索引为
                            GDD_SendMessage(hwndLB2,MSG_ListBox_SETCURSEL,i_2+1,0);     //设置当前选项
                            free(buf);
                        }
                     }
                }
                    break;
        case ID_LEFT://<--
                if(event==MSG_BTN_UP)
                {
                    char *buf;
                    int i_1,i_2,n;
                    i_1 =GDD_SendMessage(hwndLB1,MSG_ListBox_GETCURSEL,0,0);//获取当前选中项
                    i_2 =GDD_SendMessage(hwndLB2,MSG_ListBox_GETCURSEL,0,0);

                    if(i_2>=0)
                    {
                        buf =(char*)malloc(GDD_SendMessage(hwndLB2,MSG_ListBox_GETTEXTLEN,i_2,0));
                        if(buf!=NULL)
                        {
                            GDD_SendMessage(hwndLB2,MSG_ListBox_GETTEXT,i_2,(u32)buf);//获取字符串
                            GDD_SendMessage(hwndLB2,MSG_ListBox_DELSTRING,i_2,0);     //删除一个字符
                            n=GDD_SendMessage(hwndLB2,MSG_ListBox_GETCOUNT,0,0);//所有项个数
                            if((n-1)<i_2)   i_2--;
                            GDD_SendMessage(hwndLB2,MSG_ListBox_SETCURSEL,i_2,0);     //设置当前选项



                            GDD_SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,i_1,(u32)buf);//添加一个字符索引为
                            GDD_SendMessage(hwndLB1,MSG_ListBox_SETCURSEL,i_1+1,0);     //设置当前选项
                            free(buf);
                        }
                     }
                }

                    break;
        case ID_LISTBOX1:
             if(event==LBN_SELCHANGE )
                  printf("listbox1 sel change.\r\n");
                    break;
        case ID_LISTBOX2:
             if(event==LBN_SELCHANGE )
            printf("listbox2 sel change.\r\n");
                    break;
        default:    break;
    }

    return true;
}
//绘图消息处理函数

static ptu32_t HmiPaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc;
    hwnd =pMsg->hwnd;
    hdc =GDD_BeginPaint(hwnd);

    GDD_GetClientRect(hwnd,&rc);
    GDD_SetFillColor(hdc,RGB(200,200,200));
    GDD_FillRect(hdc,&rc);
    GDD_EndPaint(hwnd,hdc);

    return true;
}
//消息处理函数表
static struct MsgProcTable s_gListboxMsgTable[] =
{
    {MSG_CREATE,HmiCreate},         //主窗口创建消息
    {MSG_TIMER,HmiTimer},           //定时器消息
    {MSG_NOTIFY,HmiNotify},         //子控件发来的通知消息
    {MSG_PAINT,HmiPaint},           //绘制消息
};

static struct MsgTableLink  s_gListboxDemoMsgLink;

/*========================================================================================*/
void    GDD_Demo_Listbox(void)
{
    s_gListboxDemoMsgLink.MsgNum = sizeof(s_gListboxMsgTable) / sizeof(struct MsgProcTable);
    s_gListboxDemoMsgLink.myTable = (struct MsgProcTable *)&s_gListboxMsgTable;
    GDD_CreateGuiApp("Listbox", &s_gListboxDemoMsgLink, 0x800, CN_WINBUF_PARENT,WS_BORDER|WS_DLGFRAME|WS_CAPTION|WS_SYSMENU);
    GDD_WaitGuiAppExit("Listbox");
}
