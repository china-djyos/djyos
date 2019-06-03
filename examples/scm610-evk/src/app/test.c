
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "stdio.h"
#include "stdint.h"
#include "stddef.h"
#include "SCM6XX_ALL.h"
#include "arm32_stdint.h"
#include "djyos.h"

//----Use This File To Test For Driver-----------------------------------------
//功能:
//参数:
//返回:
//说明：
//-----------------------------------------------------------------------------

//Use this to test led

#ifndef mS
#define mS 1000
#endif

ptu32_t ledTestTask(void)
{
    extern void BoardLedInit(void);
    BoardLedInit();

    while(1)
    {
        Djy_EventDelay(1000*mS);
        GPIO_Write_Out(13,GPIO_HIGH);
        Djy_EventDelay(1000*mS);
        GPIO_Write_Out(14,GPIO_HIGH);
        Djy_EventDelay(1000*mS);
        GPIO_Write_Out(29,GPIO_HIGH);
        Djy_EventDelay(1000*mS);
        GPIO_Write_Out(30,GPIO_HIGH);
        Djy_EventDelay(1000*mS);
        GPIO_Write_Out(13,GPIO_LOW);
        Djy_EventDelay(1000*mS);
        GPIO_Write_Out(14,GPIO_LOW);
        Djy_EventDelay(1000*mS);
        GPIO_Write_Out(29,GPIO_LOW);
        Djy_EventDelay(1000*mS);
        GPIO_Write_Out(30,GPIO_LOW);
        Djy_EventDelay(1000*mS);
    }

    return 0;
}

void test_Led(void)
{
    u16 evtt_led;
    evtt_led = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
            ledTestTask,NULL,1000,"Led_Test");
    //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
    if(evtt_led!=CN_EVTT_ID_INVALID)
        Djy_EventPop(evtt_led,NULL,0,NULL,0,0);
}


//Use this to test timer

#include "timer_hard.h"

u32 UsrTimerHandle0(ptu32_t TimerHandle)
{
    printk("timer 0 Usr\r\n");
    return 0;
}

u32 UsrTimerHandle1(ptu32_t TimerHandle)
{
    printk("timer 1 Usr\r\n");
    return 0;
}

ptu32_t timerHandle1;

void test_Timer(void)
{
    ptu32_t timerHandle;
    u32 timerFre;

    extern bool_t ModuleInstall_HardTimer(void);
    ModuleInstall_HardTimer();

    timerHandle  =  HardTimer_Alloc(UsrTimerHandle0);
    timerHandle1 = timerHandle;
    if(timerHandle == 0)
    {
        return;
    }

    timerFre = HardTimer_GetFreq(timerHandle);
    HardTimer_Ctrl(timerHandle, EN_TIMER_SETCYCLE,1*timerFre);
    HardTimer_Ctrl(timerHandle,EN_TIMER_SETINTPRO,false);
    HardTimer_Ctrl(timerHandle,EN_TIMER_SETRELOAD,false);
    HardTimer_Ctrl(timerHandle,EN_TIMER_ENINT,true);
    HardTimer_Ctrl(timerHandle,EN_TIMER_STARTCOUNT,true);

    timerHandle =  HardTimer_Alloc(UsrTimerHandle1);
    timerFre = HardTimer_GetFreq(timerHandle);
    HardTimer_Ctrl(timerHandle, EN_TIMER_SETCYCLE,2*timerFre);
    HardTimer_Ctrl(timerHandle,EN_TIMER_SETINTPRO,false);
    HardTimer_Ctrl(timerHandle,EN_TIMER_SETRELOAD,false);
    HardTimer_Ctrl(timerHandle,EN_TIMER_ENINT,true);
    HardTimer_Ctrl(timerHandle,EN_TIMER_STARTCOUNT,true);
}


//Use this to test Rtc

/*test Rtc*/

void test_Rtc(void)
{
    extern bool_t djyos_ver(char *param);
    djyos_ver(0);

    extern ptu32_t ModuleInstall_CpuRtc(ptu32_t para);
    ModuleInstall_CpuRtc(0);
}


//Use this to test WatchDog

void test_Wdt(void)
{
    extern bool_t Wdt_Initial(u32 setcycle);
    Wdt_Initial(0);

    printk("wdt test begin\r\n");
}

//Use this to Uart4

#include "fcntl.h"
int fd;
void Uart4_Init(void)
{
    fd =  open("/dev/UART4",O_RDWR);
    if(fd < 0)
    {
        printf("open Uart4 Failed\r\n");
        return;
    }
}

ptu32_t Uart4_TestTask(void)
{
    u8 buf[128];
    u32 bytes = 0;
    Uart4_Init();
    memset(buf,0,128);

    while(1)
    {
        bytes = read(fd,buf,128);
        if(bytes > 0)
        {
            u8 loop;
            for(loop = 0;loop < bytes;loop++)
            {
                printk("%c",buf[loop]);
            }
            memset(buf,0,128);
        }
    }

    return (ptu32_t)0;
}

bool_t test_Uart(void)
{
    bool_t retValue = false;

    u16 evtt_Uart4;
    evtt_Uart4 = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
            Uart4_TestTask,NULL,1024,"Uart4");
    //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
    if(evtt_Uart4!=CN_EVTT_ID_INVALID)
    {
        Djy_EventPop(evtt_Uart4,NULL,0,NULL,0,0);
        retValue = true;
    }


    return retValue;
}


//test i2c At24c08

void test_At24c08(void)
{
    u8 tmpLoop  = 0;
    u8 tmpBuf[128] = {0};

    //init
    for(tmpLoop = 0;tmpLoop < 128;tmpLoop++)
    {
        tmpBuf[tmpLoop] = tmpLoop;
    }
//    extern bool_t IIC_ModuleInstall(u8 iic_port);
//    IIC_ModuleInstall(0);
    extern bool_t AT24_ModuleInit(char *BusName);
    AT24_ModuleInit("IIC1");
    u32 tmpStartAddr = 0;
    extern u32 AT24_WriteBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum);
    AT24_WriteBytes(tmpStartAddr,tmpBuf,128);
    memset(tmpBuf,0,128);
    extern u32 AT24_ReadBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum);
    AT24_ReadBytes(tmpStartAddr,tmpBuf,128);
    printf("now we can read datas\r\n");
    for(tmpLoop = 0;tmpLoop < 128;tmpLoop++)
    {
        printf("%d ",tmpBuf[tmpLoop]);
    }
}


//Use this to test net and here to creat a cilent and service model
//and then Mcu to communication and pc



//computer ip
static u8  gc_ServiceAddr[16] = {"192.168.0.60"};
//app sevice port
static u16 gPort     = 8080;
static int ComSocket = -1;

ptu32_t MCU_SendTask(void)
{
    struct in_addr  ipaddr;
    struct sockaddr_in address;
    int con = -1;
    u16 port;
    u32 timeout;

    printf("Send Task is Running\r\n");
//Use tcp
//    extern bool_t TcpInit(void);
//    TcpInit();
    inet_aton(gc_ServiceAddr,&ipaddr);
    port = htons((u16)gPort);

    ComSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(ComSocket < 0)
    {
        printf("Create communicate socket failed\r\n");
        return (ptu32_t)-1;
    }else{
        //wait forever
        timeout = 0xffffffff;
        setsockopt(ComSocket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
        setsockopt(ComSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        address.sin_addr   = ipaddr;
        address.sin_family = AF_INET;
        address.sin_port   = port;
    }

    //connect->in fact socket  is relative to service
    con = connect(ComSocket, (struct sockaddr*) &address, sizeof(address));
    while(con < 0)//Until connect success
    {
        con = connect(ComSocket, (struct sockaddr*) &address, sizeof(address));
    }
    ptu32_t MCU_RevTask(void);
    u16 net_Rev;
    //启动接收任务
    net_Rev = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS - 1,0,0,
              MCU_RevTask,NULL,2048,"RevTask");
              //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
              if(net_Rev!=CN_EVTT_ID_INVALID)
                  Djy_EventPop(net_Rev,NULL,0,NULL,0,0);

    //now send and rev
    static int s_MassegeCnt = 0;
    while(1)
    {
        u8 Msg[32] = {0};
        memset(Msg,0,sizeof(Msg));
        sprintf(Msg,"%s%d","hello world",s_MassegeCnt++);
        send(ComSocket, Msg, strlen(Msg), 0);
        Djy_EventDelay(1000*1000);
    }

    return (ptu32_t)0;
}

ptu32_t MCU_RevTask(void)
{
    int tmpLen = 0;
    u8  tmpBuf[128];

    printf("Rev Task is running\r\n");

    while(1)
    {
        memset(tmpBuf,0,sizeof(tmpBuf));
        tmpLen = recv(ComSocket, tmpBuf,sizeof(tmpBuf), 0);
        if(tmpLen > 0)
        {
            printf("rev:%s\r\n",tmpBuf);
        }
    }

    return (ptu32_t)0;
}

//net test

void TCB_TestTask(void)
{
    u16 net_Send;

    net_Send = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
            MCU_SendTask,NULL,2048,"sendTask");
    //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
    if(net_Send!=CN_EVTT_ID_INVALID)
        Djy_EventPop(net_Send,NULL,0,NULL,0,0);

}

















