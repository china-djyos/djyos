#include <cpu_peri_gpio.h>
#include <djyfs/file.h>
#include <djyos.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <systime.h>
#include "../../../../../djysrc/bsp/arch/arm/arm32_stdint.h"

// =============================================================================
// 功能: LED TEST
// 参数: 无
// 返回: 无
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
// 功能:led初始化
// 参数:无
// 返回: 无
// =============================================================================
void led_init(void)
{
    u16 evtt_led;
    GPIO_PowerOn(GPIO_B);
    GPIO_CfgPinFunc(GPIO_B,PIN0|PIN1 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);

    evtt_led = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
                            Led_Test,NULL,1000,"Led_Test");
    //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
    if(evtt_led!=CN_EVTT_ID_INVALID)
        Djy_EventPop(evtt_led,NULL,0,NULL,0,0);

}


// =============================================================================
// 功能: 主函数
// 参数:无
// 返回: 无
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

#define ID_DELETE_1     0x1300
#define ID_DELETE_2     0x1301
#define ID_DELETE_3     0x1302
#define ID_DELETE_4     0x1303

#define ID_DELETE_ALL   0x1304
#define ID_ADD          0x1305
#define ID_SUB          0x1306

#define ID_RESET_1     0x1400
#define ID_RESET_2     0x1401
#define ID_RESET_3     0x1402
#define ID_RESET_4     0x1403

#define ID_CREATE_1     0x1500
#define ID_CREATE_2     0x1501
#define ID_CREATE_3     0x1502
#define ID_CREATE_4     0x1503



#define IdBase   7
HWND g_ptMainHwnd;
static struct   {
    u32 text_color;
    u32 bd_color;
    u32 bk_color;
}DrawText_Color_Tbl[6]={

         {RGB(255,0,0),RGB(255,0,0),RGB(0,0,0)},
         {RGB(0,255,0),RGB(0,255,0),RGB(0,0,0)},
         {RGB(0,160,255),RGB(0,160,255),RGB(0,0,0)},
         {RGB(255,255,0),RGB(255,255,0),RGB(0,0,0)},
         {RGB(0,0,255),RGB(0,0,255),RGB(0,255,0)},
};

static  int cfg_idx=0;
static  int bBorder=TRUE;
static  int bBKGND=TRUE;
static  int bColor=TRUE;

const int timer_interval[]={
        50,
        100,
        200,
        400,
        500,
        1000,
};

static  int timer_count[8]={0,0,0,0,0,0,0,0};

static  int timer = 10;



//创建主窗口
static ptu32_t HmiCreate(struct WindowMsg *pMsg)
{
    HWND hwnd;
    RECT rc0;
    u32  x,y;
    hwnd=pMsg->hwnd;

    cfg_idx =0;
    bBorder=TRUE;
    bBKGND=TRUE;
    bColor=TRUE;

    GetClientRect(hwnd,&rc0);
    CreateButton("Close",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,RectW(&rc0)-164,RectH(&rc0)-128,160,80,hwnd,ID_CLOSE,NULL,NULL);
    ClientToScreen(hwnd,(POINT *)&rc0,2);
    y=20+rc0.top;
    x=128+8;
    CreateButton("start 1",WS_CHILD|BS_HOLD|WS_VISIBLE,x,y+0*80,100,50,hwnd,ID_START_1,NULL,NULL);
    CreateButton("Start 2",WS_CHILD|BS_HOLD|WS_VISIBLE,x,y+1*80,100,50,hwnd,ID_START_2,NULL,NULL);
    CreateButton("Start 3",WS_CHILD|BS_HOLD|WS_VISIBLE,x,y+2*80,100,50,hwnd,ID_START_3,NULL,NULL);
    CreateButton("Start 4",WS_CHILD|BS_HOLD|WS_VISIBLE,x,y+3*80,100,50,hwnd,ID_START_4,NULL,NULL);
    CreateButton("+",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+4*80,100,50,hwnd,ID_ADD,NULL,NULL);

    x=128+8+300;
    CreateButton("Clear 1",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+0*80,100,50,hwnd,ID_CLR_1,NULL,NULL);
    CreateButton("Clear 2",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+1*80,100,50,hwnd,ID_CLR_2,NULL,NULL);
    CreateButton("Clear 3",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+2*80,100,50,hwnd,ID_CLR_3,NULL,NULL);
    CreateButton("Clear 4",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+3*80,100,50,hwnd,ID_CLR_4,NULL,NULL);
    CreateButton("-",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+4*80,100,50,hwnd,ID_SUB,NULL,NULL);

    x=128+8+300+120;
    CreateButton("Reset 1",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+0*80,100,50,hwnd,ID_RESET_1,NULL,NULL);
    CreateButton("Reset 2",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+1*80,100,50,hwnd,ID_RESET_2,NULL,NULL);
    CreateButton("Reset 3",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+2*80,100,50,hwnd,ID_RESET_3,NULL,NULL);
    CreateButton("Reset 4",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+3*80,100,50,hwnd,ID_RESET_4,NULL,NULL);

    x=128+8+300+120+120;
    CreateButton("Delete 1",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+0*80,100,50,hwnd,ID_DELETE_1,NULL,NULL);
    CreateButton("Delete 2",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+1*80,100,50,hwnd,ID_DELETE_2,NULL,NULL);
    CreateButton("Delete 3",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+2*80,100,50,hwnd,ID_DELETE_3,NULL,NULL);
    CreateButton("Delete 4",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+3*80,100,50,hwnd,ID_DELETE_4,NULL,NULL);
    CreateButton("Delete ALL",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+4*80,100,50,hwnd,ID_DELETE_ALL,NULL,NULL);

    x=128+8+300+120+120+120;
    CreateButton("Create 1",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+0*80,100,50,hwnd,ID_CREATE_1,NULL,NULL);
    CreateButton("Create 2",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+1*80,100,50,hwnd,ID_CREATE_2,NULL,NULL);
    CreateButton("Create 3",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+2*80,100,50,hwnd,ID_CREATE_3,NULL,NULL);
    CreateButton("Create 4",WS_CHILD|BS_NORMAL|WS_VISIBLE,x,y+3*80,100,50,hwnd,ID_CREATE_4,NULL,NULL);

    GDD_CreateTimer(hwnd,0,timer_interval[0]);
    GDD_CreateTimer(hwnd,1,timer_interval[1]);
    GDD_CreateTimer(hwnd,2,timer_interval[2]);
    GDD_CreateTimer(hwnd,3,timer_interval[3]);
    return true;
}
//定时器处理函数
static ptu32_t HmiTimer(struct WindowMsg *pMsg)
{
    u32 i,y;
    HWND hwnd;
    HDC  hdc;
    RECT rc,rc0;
    char wbuf[32];

    hwnd =pMsg->hwnd;
    i =pMsg->Param1&0xFF;
    timer_count[i]++;

    hwnd =pMsg->hwnd;
    hdc =BeginPaint(hwnd);
    GetClientRect(hwnd,&rc0);
    ClientToScreen(hwnd,(POINT *)&rc0,2);
    y=rc0.top;
    SetRect(&rc,128+8+135,y,130,24);
    OffsetRect(&rc,0,80*i);

    SetTextColor(hdc,DrawText_Color_Tbl[i].text_color);
    SetDrawColor(hdc,DrawText_Color_Tbl[i].bd_color);
    SetFillColor(hdc,DrawText_Color_Tbl[i].bk_color);

    sprintf(wbuf," 定时器%d: %d",i+1,timer_count[i]);
    DrawText(hdc,wbuf,-1,&rc,DT_LEFT|DT_VCENTER|DT_BORDER|DT_BKGND);
    EndPaint(hwnd,hdc);
    return true;
}
//子控件通知消息处理函数
static ptu32_t HmiNotify(struct WindowMsg *pMsg)
{
    HWND hwnd;
    struct WinTimer *ptmr;
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
                ptmr=GDD_FindTimer(hwnd,i);
                GDD_StartTimer(ptmr);
            }                    break;

        case MSG_BTN_UP:    //按钮弹起
            if(id==ID_CLOSE)
            {
                PostMessage(hwnd,MSG_CLOSE,0,0);
            }

            if(id>=ID_START_1 && id<=ID_START_4)
            {
                i =id&0xF;
                ptmr=GDD_FindTimer(hwnd,i);
                GDD_StopTimer(ptmr);
            }
            if(id>=ID_CLR_1 && id<=ID_CLR_4)
            {
                i =id&0xF;
                timer_count[i]=0;
                InvalidateWindow(hwnd,FALSE);
            }

            if(id>=ID_DELETE_1 && id<=ID_DELETE_4)
            {
                i =id&0xF;
                ptmr=GDD_FindTimer(hwnd,i);
                GDD_DeleteTimer(ptmr);
            }

            extern void __RemoveWindowTimer(HWND hwnd);
            if(id==ID_DELETE_ALL)
                __RemoveWindowTimer(hwnd);

            if(id>=ID_RESET_1 && id<=ID_RESET_4)
            {
                i =id&0xF;
                ptmr=GDD_FindTimer(hwnd,i);
                GDD_ResetTimer(ptmr,timer);
            }
            if(id>=ID_CREATE_1 && id<=ID_CREATE_4)
            {
                i =id&0xF;
                GDD_CreateTimer(hwnd,i,timer);
            }
            if((id==ID_ADD)||(id==ID_SUB))
            {
                if((id==ID_SUB))
                    timer-=10;
                else
                    timer+=10;
                u32 y;
                HDC  hdc;
                RECT rc,rc0;
                char wbuf[32];

                hwnd =pMsg->hwnd;
                hdc =BeginPaint(hwnd);
                GetClientRect(hwnd,&rc0);
                ClientToScreen(hwnd,(POINT *)&rc0,2);
                y=rc0.top;
                SetRect(&rc,128+8+135,y,130,24);
                OffsetRect(&rc,0,80*4);

                SetTextColor(hdc,DrawText_Color_Tbl[4].text_color);
                SetDrawColor(hdc,DrawText_Color_Tbl[4].bd_color);
                SetFillColor(hdc,DrawText_Color_Tbl[4].bk_color);

                sprintf(wbuf," 定时间隔:%d",timer);
                DrawText(hdc,wbuf,-1,&rc,DT_LEFT|DT_VCENTER|DT_BORDER|DT_BKGND);
                EndPaint(hwnd,hdc);
            }
            break;
        default: break;
    }
    return true;
}


//绘图消息处理函数
static ptu32_t HmiPaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc,rc0;
    char wbuf[32];
    u32 i,y;

    hwnd =pMsg->hwnd;
    hdc =BeginPaint(hwnd);
    GetClientRect(hwnd,&rc0);
    ClientToScreen(hwnd,(POINT *)&rc0,2);
    y=rc0.top;
    SetRect(&rc,128+8+135,y,130,24);
    for(i=0;i<4;i++)
    {
        SetTextColor(hdc,DrawText_Color_Tbl[i].text_color);
        SetDrawColor(hdc,DrawText_Color_Tbl[i].bd_color);
        SetFillColor(hdc,DrawText_Color_Tbl[i].bk_color);
        sprintf(wbuf," 定时器%d: %d",i+1,timer_count[i]);
        DrawText(hdc,wbuf,-1,&rc,DT_LEFT|DT_VCENTER|DT_BORDER|DT_BKGND);
        OffsetRect(&rc,0,80);
    }

    SetTextColor(hdc,DrawText_Color_Tbl[4].text_color);
    SetDrawColor(hdc,DrawText_Color_Tbl[4].bd_color);
    SetFillColor(hdc,DrawText_Color_Tbl[4].bk_color);

    sprintf(wbuf," 定时间隔:%d",timer);
    DrawText(hdc,wbuf,-1,&rc,DT_LEFT|DT_VCENTER|DT_BORDER|DT_BKGND);
    EndPaint(hwnd,hdc);

    return true;
}
//消息处理函数表
static struct MsgProcTable s_gTimerMsgTable[] =
{
    {MSG_CREATE,HmiCreate},         //主窗口创建消息
    {MSG_TIMER,HmiTimer},           //定时器消息
    {MSG_NOTIFY,HmiNotify},         //子控件发来的通知消息
    {MSG_PAINT,HmiPaint},           //绘制消息
};

static struct MsgTableLink  s_gTimerDemoMsgLink;
/*============================================================================*/

void    GDD_Demo(void)
{
    s_gTimerDemoMsgLink.MsgNum = sizeof(s_gTimerMsgTable) / sizeof(struct MsgProcTable);
    s_gTimerDemoMsgLink.myTable = (struct MsgProcTable *)&s_gTimerMsgTable;
    GDD_CreateGuiApp("Demo", &s_gTimerDemoMsgLink, 0x800, CN_WINBUF_PARENT);
    GDD_WaitGuiAppExit("Demo");
}


// =============================================================================
// 功能:led初始化
// 参数:无
// 返回: 无
// =============================================================================

void djy_main(void)
{

//	fs_test();
	printf("Run Mode:App.\r\n");
	GDD_Demo();
    for(;;)
    {
        printf("Run Mode:App.\r\n");
    	Djy_EventDelay(1000*mS);
    }

}











