#include <cpu_peri_gpio.h>
#include <djyfs/file.h>
#include <djyos.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <systime.h>
#include "../../../../../djysrc/bsp/arch/arm/arm32_stdint.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <gkernel.h>
#include <gdd.h>
#include <gdd_widget.h>
// =============================================================================
// ����: LED TEST
// ����: ��
// ����: ��
// =============================================================================
ptu32_t Led_Test(void)
{
    for(;;)
    {
        GPIO_SettoHigh(GPIO_B, PIN1);
        GPIO_SettoLow(GPIO_B, PIN0);
        Djy_EventDelay(250*mS);
        GPIO_SettoLow(GPIO_B, PIN1);
        GPIO_SettoHigh(GPIO_B, PIN0);
        Djy_EventDelay(250*mS);
    }
    return 0;
}
// =============================================================================
// ����:led��ʼ��
// ����:��
// ����: ��
// =============================================================================
void led_init(void)
{
    u16 evtt_led;
    GPIO_PowerOn(GPIO_B);
    GPIO_CfgPinFunc(GPIO_B,PIN0|PIN1 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);

    evtt_led = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
                            Led_Test,NULL,1000,"Led_Test");
    //�¼���������������Ϊ0,�����shell����,�������ɼ�shell�����в���
    if(evtt_led!=CN_EVTT_ID_INVALID)
        Djy_EventPop(evtt_led,NULL,0,NULL,0,0);

}


// =============================================================================
// ����: ������
// ����:��
// ����: ��
// =============================================================================

bool_t fs_test(void)
{
    char buf[100];
    char *p="hello test!!";
    FILE *pfile;
    pfile = fopen("/yaffs2/test.txt","w+");
    if(pfile)
        fwrite(p,strlen(p),1,pfile);
    fclose(pfile);

    pfile=NULL;
    pfile = fopen("/yaffs2/test.txt","r");
    if(pfile)
        fread(buf,strlen(p),1,pfile);
    fclose(pfile);
    if(0== strncasecmp(p, buf,strlen(p)))
        return true;
    return false;
}





#define ID_CLOSE    0x1000
static ptu32_t HmiCreate(struct WindowMsg *pMsg)
{

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
        PostMessage(hwnd,MSG_CLOSE,0,0);
    }
    return true;
}
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
//���ڹر���Ϣ������
static ptu32_t HmiClose(struct WindowMsg *pMsg)
{
    printf("win2_close.\r\n");
    return true;
}
//��Ϣ��������
static struct MsgProcTable s_gHelloMsgTable[] =
{
    {MSG_CREATE,HmiCreate},         //��ʱ����Ϣ
    {MSG_NOTIFY,HmiNotify},         //�ӿؼ�������֪ͨ��Ϣ
    {MSG_PAINT,HmiPaint},           //������Ϣ
    {MSG_CLOSE,HmiClose}      //���ڹر���Ϣ
};

static struct MsgTableLink  s_gHelloDemoMsgLink;

void Hello_demo(void)
{
    s_gHelloDemoMsgLink.MsgNum = sizeof(s_gHelloMsgTable) / sizeof(struct MsgProcTable);
    s_gHelloDemoMsgLink.myTable = (struct MsgProcTable *)&s_gHelloMsgTable;
    GDD_CreateGuiApp("hello World", &s_gHelloDemoMsgLink, 0x800, CN_WINBUF_PARENT);
    GDD_WaitGuiAppExit("hello World");
}
// =============================================================================
// ����:led��ʼ��
// ����:��
// ����: ��
// =============================================================================

void djy_main(void)
{

//	fs_test();
	printf("Run Mode:App.\r\n");
	Hello_demo();
    for(;;)
    {
    	Djy_EventDelay(1000*mS);
    }

}











