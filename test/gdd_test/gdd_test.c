//---------------d--------------------------------------------------------------
//GDD测试:
//
//
//------------------------------------------------------
extern void Hello_Word(void);
extern void GDD_Demo_DrawText(void);
extern void GDD_Demo_Listbox(void);
extern void GDD_Demo_MoveWindow(void);
extern void GDD_Demo_Timer(void);
extern void GDD_Demo_ZigBee(void);
extern void GDD_Demo_Progressbar(void);

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <gkernel.h>
#include <gdd.h>
#include <gdd_widget.h>
#include "systime.h"
//#include    "..gui/gdd/gdd_private.h"
#include <widget/gdd_button.h>

//#include "arm32_stdint.h"


static u16 test_id;
static const struct Test_tab
{
    const char *text;
    void (* GuiTestFun)(void);

}test_tab[]={
        {"HelloWord"   ,Hello_Word          },
        {"DrawText"    ,GDD_Demo_DrawText   },
        {"Listbox"     ,GDD_Demo_Listbox    },
        {"MoveWindow"  ,GDD_Demo_MoveWindow },
        {"Timer"       ,GDD_Demo_Timer      },
        {"ZigBee"      ,GDD_Demo_ZigBee     },
        {"Progressbar" ,GDD_Demo_Progressbar},
        {"Exit"        ,NULL                },
};


static ptu32_t HmiCreate(struct WindowMsg *pMsg)
{
    RECT rc0,rc;
    HWND hwnd;
    u16 num,j,k;
    u16 m,n;//最小行列数

    hwnd=pMsg->hwnd;
    num = sizeof(test_tab)/sizeof(struct Test_tab);

    m=0;n=0;//n行 m列
    while(m*n<num)
    {
        if(m==n)
            m++;
        else
            n++;
    }

    GDD_GetClientRect(hwnd,&rc0);
    GDD_SetRect(&rc,GDD_RectW(&rc0)/(m*3+1),GDD_RectH(&rc0)/(n*3+1),\
            (2*GDD_RectW(&rc0))/(m*3+1),(2*GDD_RectH(&rc0))/(n*3+1));
    GDD_OffsetRect(&rc,0, GDD_RectH(&rc)/3);
    num = 0;
    for(j=0;j<n;j++)
    {
        rc0  = rc;
        for(k=0;k<m;k++)
        {
            if(num == (sizeof(test_tab)/sizeof(struct Test_tab)))
                return true;
            Widget_CreateButton(test_tab[num].text,
                    WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,//按钮风格
                    rc.left,rc.top,GDD_RectW(&rc),GDD_RectH(&rc),    //按钮位置和大小
                    hwnd,num,NULL,NULL);    //按钮所属的父窗口，ID,附加数据
            num++;
            GDD_OffsetRect(&rc,(GDD_RectW(&rc)*3)/2,0);
        }
        rc  = rc0;
        GDD_OffsetRect(&rc,0,3*GDD_RectH(&rc)/2);
    }
    return true;
}

static ptu32_t HmiNotify(struct WindowMsg *pMsg)
{
    HWND hwnd;
    u16 event,id;
    hwnd =pMsg->hwnd;
    event =HI16(pMsg->Param1);
    id =LO16(pMsg->Param1);

    if(event==MSG_BTN_UP)
    {
        test_id = id;
        GDD_PostMessage(hwnd,MSG_CLOSE,0,0);
    }
    return true;
}


//消息处理函数表
static struct MsgProcTable s_gTestMsgTable[] =
{
    {MSG_CREATE,HmiCreate},
    {MSG_NOTIFY,HmiNotify},
};

static struct MsgTableLink  s_gTestMsgLink;
static void CreateTestGui(void)
{
    s_gTestMsgLink.MsgNum = sizeof(s_gTestMsgTable) / sizeof(struct MsgProcTable);
    s_gTestMsgLink.myTable = (struct MsgProcTable *)&s_gTestMsgTable;
    GDD_CreateGuiApp("Gdd Test", &s_gTestMsgLink, 0x800, CN_WINBUF_PARENT,WS_BORDER|WS_DLGFRAME|WS_CAPTION|WS_SYSMENU);
    GDD_WaitGuiAppExit("Gdd Test");
}

void gdd_test(void)
{
    HWND Desktop;
    while(1)
    {
        CreateTestGui();
        if((test_id<sizeof(test_tab)/sizeof(struct Test_tab))
             &&test_tab[test_id].GuiTestFun!=NULL)
            test_tab[test_id].GuiTestFun();
        else
            break;
    }
    Desktop = GDD_GetDesktopWindow();
    GK_FillWin(Desktop->pGkWin,CN_COLOR_BLUE,0);
    GDD_UpdateDisplay(hwnd);
//  GDD_PostMessage(Desktop, MSG_SYNC_DISPLAY, 0, 0);

}

