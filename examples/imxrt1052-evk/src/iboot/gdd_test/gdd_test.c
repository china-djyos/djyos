//---------------d--------------------------------------------------------------
//GDD����:
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
#include <gui/gdd/gdd_private.h>
#include <widget/gdd_button.h>

#include "../../../../djysrc/bsp/arch/arm/arm32_stdint.h"


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


static ptu32_t HmiPaint(struct WindowMsg *pMsg)
{
    RECT rc0;
    HWND hwnd;
    hwnd=pMsg->hwnd;
    HDC hdc;
    hdc =BeginPaint(hwnd);  //��ʼ��ͼ����û�ͼ������(hdc)��
    GetClientRect(hwnd,&rc0);           //��ô��ڿͻ�������
    SetFillColor(hdc,RGB(255,255,255)); //�������ɫ��
    FillRect(hdc,&rc0);                 //�����Ρ�
    SetTextColor(hdc,RGB(255,0,0));     //����������ɫ��
    TextOut(hdc,10,20,"Hello World!",-1);  //�ڿͻ���10,20��λ��������֡�
    EndPaint(hwnd,hdc); //�������Ʋ��ͷŻ�ͼ�����ġ�
    return true;

}






static ptu32_t HmiCreate(struct WindowMsg *pMsg)
{
//    RECT rc0,rc;
//    HWND hwnd;
//    u16 num,j,k;
//    u16 m,n;//��С������
//
//    hwnd=pMsg->hwnd;
//    num = sizeof(test_tab)/sizeof(struct Test_tab);
//
//    m=0;n=0;//n�� m��
//    while(m*n<num)
//    {
//        if(m==n)
//            m++;
//        else
//            n++;
//    }
//
//    GetClientRect(hwnd,&rc0);
//    SetRect(&rc,RectW(&rc0)/(m*3+1),RectH(&rc0)/(n*3+1),\
//            (2*RectW(&rc0))/(m*3+1),(2*RectH(&rc0))/(n*3+1));
//    num = 0;
//    for(j=0;j<n;j++)
//    {
//        rc0  = rc;
//        for(k=0;k<m;k++)
//        {
//            if(num == (sizeof(test_tab)/sizeof(struct Test_tab)))
//                return true;
//            CreateButton(test_tab[num].text,
//                    WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,//��ť���
//                    rc.left,rc.top,RectW(&rc),RectH(&rc),    //��ťλ�úʹ�С
//                    hwnd,num,NULL,NULL);    //��ť�����ĸ����ڣ�ID,��������
//            num++;
//            OffsetRect(&rc,(RectW(&rc)*3)/2,0);
//        }
//        rc  = rc0;
//        OffsetRect(&rc,0,3*RectH(&rc)/2);
//    }
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
        PostMessage(hwnd,MSG_CLOSE,0,0);
    }
    return true;
}

//���ڹر���Ϣ������
static ptu32_t HmiClose(struct WindowMsg *pMsg)
{
    return true;
}
//��Ϣ��������
static struct MsgProcTable s_gTestMsgTable[] =
{
    {MSG_CREATE,HmiCreate},
    {MSG_NOTIFY,HmiNotify},
    {MSG_CLOSE,HmiClose},
    {MSG_PAINT,HmiPaint}          //������Ϣ
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
    Desktop = GetDesktopWindow();
    GK_FillWin(Desktop->pGkWin,CN_COLOR_BLUE,0);
    GK_SyncShow(CN_TIMEOUT_FOREVER);

}

