
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

//����������
static ptu32_t HmiCreate(struct WindowMsg *pMsg)
{
    HWND hwnd;
    struct WinTimer *timer;
    RECT rc,rc0;
    hwnd=pMsg->hwnd;

    GetClientRect(hwnd,&rc0);
    CreateButton("�ر�",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,RectW(&rc0)-64,RectH(&rc0)-10,60,24,hwnd,ID_CLOSE,NULL,NULL);

    SetRect(&rc,0,10,107,190);
    ClientToScreen(hwnd,(POINT*)&rc,2);
    ScreenToWindow(hwnd,(POINT*)&rc,2);
    hwndLB1=CreateListBox("�б��1",WS_CHILD|WS_BORDER|WS_VISIBLE|WS_CAPTION,
                            rc.left,rc.top,RectW(&rc),RectH(&rc),
                            hwnd,ID_LISTBOX1,NULL,NULL);
    CreateButton("-->",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,rc.right-50,rc.bottom+4,50,20,hwnd,ID_RIGHT,NULL,NULL);

    OffsetRect(&rc,RectW(&rc)+8,0);
    hwndLB2=CreateListBox("�б��2",WS_CHILD|WS_BORDER|WS_VISIBLE|WS_CAPTION,
                        rc.left,rc.top,RectW(&rc),RectH(&rc),
                        hwnd,ID_LISTBOX2,NULL,NULL);
    CreateButton("<--",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,rc.left,rc.bottom+4,50,20,hwnd,ID_LEFT,NULL,NULL);


    SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,0,(u32)"ListItem-0");
    SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,1,(u32)"ListItem-1");
    SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,2,(u32)"ListItem-2");
    SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,3,(u32)"ListItem-3");
    SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,4,(u32)"ListItem-4");
    SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,5,(u32)"ListItem-5");
    SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,6,(u32)"ListItem-6");
    SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,7,(u32)"ListItem-7");
    SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,8,(u32)"ListItem-8");
    SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,9,(u32)"ListItem-9");
    SendMessage(hwndLB1,MSG_ListBox_SETCURSEL,3,0);

    SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,0,(u32)"ListItem-10");
    SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,1,(u32)"ListItem-11");
    SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,2,(u32)"ListItem-12");
    SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,3,(u32)"ListItem-13");
    SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,4,(u32)"ListItem-14");
    SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,5,(u32)"ListItem-15");
    SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,6,(u32)"ListItem-16");
    SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,7,(u32)"ListItem-17");
    SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,8,(u32)"ListItem-18");
    SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,9,(u32)"ListItem-19");
    SendMessage(hwndLB2,MSG_ListBox_SETCURSEL,3,0);

    timer = GDD_CreateTimer(hwnd,1,3000);
    GDD_StartTimer(timer);
    timer = GDD_CreateTimer(hwnd,2,100);
    GDD_StartTimer(timer);

    return true;
}
//��ʱ��������
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

//�ӿؼ�֪ͨ��Ϣ������
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
                PostMessage(hwnd,MSG_CLOSE,0,0);
            break;
        case ID_RIGHT://-->
            if(event==MSG_BTN_UP )
                {
                    char *buf;
                    int i_1,i_2,n;
                    i_1 =SendMessage(hwndLB1,MSG_ListBox_GETCURSEL,0,0);//��ȡ��ǰѡ����
                    i_2 =SendMessage(hwndLB2,MSG_ListBox_GETCURSEL,0,0);

                    if(i_1>=0)
                    {
                        buf =(char*)malloc(SendMessage(hwndLB1,MSG_ListBox_GETTEXTLEN,i_1,0));
                        if(buf!=NULL)
                        {
                            SendMessage(hwndLB1,MSG_ListBox_GETTEXT,i_1,(u32)buf);//��ȡ�ַ���
                            SendMessage(hwndLB1,MSG_ListBox_DELSTRING,i_1,0);     //ɾ��һ���ַ�
                            n=SendMessage(hwndLB1,MSG_ListBox_GETCOUNT,0,0);//���������
                            if((n-1)<i_1)   i_1--;
                            SendMessage(hwndLB1,MSG_ListBox_SETCURSEL,i_1,0);     //���õ�ǰѡ��



                            SendMessage(hwndLB2,MSG_ListBox_ADDSTRING,i_2,(u32)buf);//���һ���ַ�����Ϊ
                            SendMessage(hwndLB2,MSG_ListBox_SETCURSEL,i_2+1,0);     //���õ�ǰѡ��
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
                    i_1 =SendMessage(hwndLB1,MSG_ListBox_GETCURSEL,0,0);//��ȡ��ǰѡ����
                    i_2 =SendMessage(hwndLB2,MSG_ListBox_GETCURSEL,0,0);

                    if(i_2>=0)
                    {
                        buf =(char*)malloc(SendMessage(hwndLB2,MSG_ListBox_GETTEXTLEN,i_2,0));
                        if(buf!=NULL)
                        {
                            SendMessage(hwndLB2,MSG_ListBox_GETTEXT,i_2,(u32)buf);//��ȡ�ַ���
                            SendMessage(hwndLB2,MSG_ListBox_DELSTRING,i_2,0);     //ɾ��һ���ַ�
                            n=SendMessage(hwndLB2,MSG_ListBox_GETCOUNT,0,0);//���������
                            if((n-1)<i_2)   i_2--;
                            SendMessage(hwndLB2,MSG_ListBox_SETCURSEL,i_2,0);     //���õ�ǰѡ��



                            SendMessage(hwndLB1,MSG_ListBox_ADDSTRING,i_1,(u32)buf);//���һ���ַ�����Ϊ
                            SendMessage(hwndLB1,MSG_ListBox_SETCURSEL,i_1+1,0);     //���õ�ǰѡ��
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
//��ͼ��Ϣ������

static ptu32_t HmiPaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc;
    hwnd =pMsg->hwnd;
    hdc =BeginPaint(hwnd);

    GetClientRect(hwnd,&rc);
    SetFillColor(hdc,RGB(200,200,200));
    FillRect(hdc,&rc);
    EndPaint(hwnd,hdc);

    return true;
}
//��Ϣ��������
static struct MsgProcTable s_gListboxMsgTable[] =
{
    {MSG_CREATE,HmiCreate},         //�����ڴ�����Ϣ
    {MSG_TIMER,HmiTimer},           //��ʱ����Ϣ
    {MSG_NOTIFY,HmiNotify},         //�ӿؼ�������֪ͨ��Ϣ
    {MSG_PAINT,HmiPaint},           //������Ϣ
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
