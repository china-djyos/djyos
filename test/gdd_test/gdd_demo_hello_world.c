
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
   //����һ����ť��
    Widget_CreateButton("�ر�",     //��ť��
                 WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,    //��ť���
                 GDD_RectW(&rc0)/2,GDD_RectH(&rc0)/2-28,60,48,    //��ťλ�úʹ�С
                 hwnd,ID_CLOSE,NULL,NULL);   //��ť�����ĸ����ڣ�ID,��������
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
    hdc =GDD_BeginPaint(hwnd);  //��ʼ��ͼ����û�ͼ������(hdc)��
    GDD_GetClientRect(hwnd,&rc0);           //��ô��ڿͻ�������
    GDD_SetFillColor(hdc,RGB(255,255,255)); //�������ɫ��
    GDD_FillRect(hdc,&rc0);                 //�����Ρ�
    GDD_SetTextColor(hdc,RGB(255,0,0));     //����������ɫ��
    GDD_TextOut(hdc,100,100,"Hello World!",-1);  //�ڿͻ���10,20��λ��������֡�
    GDD_EndPaint(hwnd,hdc); //�������Ʋ��ͷŻ�ͼ�����ġ�
    return true;

}
//���ڹر���Ϣ������
static ptu32_t HmiClose(struct WindowMsg *pMsg)
{
    printf("win2_close.\r\n");
    return true;
}
//��Ϣ��������
static struct MsgProcTable s_gHelloMsgTable[] =
{
    {MSG_CREATE,HmiCreate},         //������Ϣ
    {MSG_NOTIFY,HmiNotify},         //�ӿؼ�������֪ͨ��Ϣ
    {MSG_PAINT,HmiPaint},           //������Ϣ
    {MSG_CLOSE,HmiClose}      //���ڹر���Ϣ
};

static struct MsgTableLink  s_gHelloDemoMsgLink;

void Hello_Word(void)
{
    s_gHelloDemoMsgLink.MsgNum = sizeof(s_gHelloMsgTable) / sizeof(struct MsgProcTable);
    s_gHelloDemoMsgLink.myTable = (struct MsgProcTable *)&s_gHelloMsgTable;
    GDD_CreateGuiApp("hello World", &s_gHelloDemoMsgLink, 0x800, CN_WINBUF_PARENT,WS_BORDER|WS_DLGFRAME|WS_CAPTION|WS_SYSMENU);
    GDD_WaitGuiAppExit("hello World");
}







