
//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
/*
 * app_main.c
 *
 *  Created on: 2014-5-28
 *      Author: Administrator
 */
#include "stdio.h"
#include "stdint.h"
#include "stddef.h"
#include "filesystems.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

/*led test*/
#include "SCM6XX_ALL.h"
#include "djyos.h"

ptu32_t ledTestTask(void)
{
    #ifndef mS
    #define mS 1000
    #endif

    extern void BoardLedInit(void);
    BoardLedInit();

    while(1)
    {
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(13,GPIO_HIGH);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(14,GPIO_HIGH);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(29,GPIO_HIGH);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(30,GPIO_HIGH);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(13,GPIO_LOW);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(14,GPIO_LOW);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(29,GPIO_LOW);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(30,GPIO_LOW);
        Djy_EventDelay(100*mS);
    }

    return 0;
}

void LED_Task(void)
{
  u16 evtt_led;
  evtt_led = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
          ledTestTask,NULL,1000,"Led_Test");
  //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
  if(evtt_led!=CN_EVTT_ID_INVALID)
      Djy_EventPop(evtt_led,NULL,0,NULL,0,0);
}

/*test timer*/
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
void timerTestTask(void)
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
    HardTimer_Ctrl(timerHandle,EN_TIMER_SETRELOAD,true);
    HardTimer_Ctrl(timerHandle,EN_TIMER_ENINT,true);
    HardTimer_Ctrl(timerHandle,EN_TIMER_STARTCOUNT,true);

    timerHandle =  HardTimer_Alloc(UsrTimerHandle1);
    timerFre = HardTimer_GetFreq(timerHandle);
    HardTimer_Ctrl(timerHandle, EN_TIMER_SETCYCLE,2*timerFre);
    HardTimer_Ctrl(timerHandle,EN_TIMER_SETINTPRO,false);
    HardTimer_Ctrl(timerHandle,EN_TIMER_SETRELOAD,true);
    HardTimer_Ctrl(timerHandle,EN_TIMER_ENINT,true);
    HardTimer_Ctrl(timerHandle,EN_TIMER_STARTCOUNT,true);
}

/*test Rtc*/
void Rtc_Test(void)
{
    extern bool_t djyos_ver(char *param);
    djyos_ver(0);

    extern ptu32_t ModuleInstall_CpuRtc(ptu32_t para);
    ModuleInstall_CpuRtc(0);
}

/*Test Wdt*/

void Wdt_Test(void)
{
    extern bool_t Wdt_Initial(u32 setcycle);
    Wdt_Initial(0);

    printk("wdt test begin\r\n");
}

//Uart4 test
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

u8 buf[128];
ptu32_t Uart4_TestTask(void)
{
    Uart4_Init();
    memset(buf,0,128);

    while(1)
    {
        int Bytes = 0;
        Bytes = read(fd,buf,128);
        if(Bytes != 0)
        {
            u8 loop;
            for(loop = 0;loop < Bytes;loop++)
            {
                printk("%c",buf[loop]);
            }
            memset(buf,0,128);
        }
    }

    return (ptu32_t)0;
}

void Uart4_TaskInit(void)
{
    u16 evtt_Uart4;
    evtt_Uart4 = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
            Uart4_TestTask,NULL,1024,"Uart4");
    //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
    if(evtt_Uart4!=CN_EVTT_ID_INVALID)
        Djy_EventPop(evtt_Uart4,NULL,0,NULL,0,0);
}

//nor flash test

u8 gbuf[256] = {0};

void W25q64_Test(void)
{
    extern s32 Flash_BlockErase(u32 BlkNo);
    extern s32 Flash_PageProgram(u32 PageNo, u8 *Data, u32 Flags);
    extern s32 Flash_PageRead(u32 PageNo, u8 *Data, u32 Flags);
    extern s32 ModuleInstall_NorFlash(const char *TargetFs,u32 bstart, u32 bend, u32 doformat);

    ModuleInstall_NorFlash("yaf2",0,128,0);
    u16 loop;

    //全片擦除
    u32 ereaseLoop = 0;
    for(ereaseLoop = 0;ereaseLoop < 128;ereaseLoop++)
    {
        Flash_BlockErase(ereaseLoop);
    }

    //检查全片擦除是否成功
    for(ereaseLoop = 0;ereaseLoop < 128*256;ereaseLoop++)
    {
        u8 tmp[256] = {0};
        memset(tmp,0,256);
        Flash_PageRead(ereaseLoop,tmp,0);
        u16 tmpLoop = 0;

        while(tmpLoop < 256)
        {
            if(tmp[tmpLoop] != 0xFF)
            {
                printk("Usr %lu page erease failed\r\n",ereaseLoop);
            }
            tmpLoop++;
        }
    }

    for(loop =0;loop < 128*128*2;loop++)
    {
        u16 loop2;
        for(loop2 = 0;loop2 < 256;loop2++)
        {
           gbuf[loop2] = loop2;
        }
        Flash_PageProgram(loop,gbuf,0);
        memset(gbuf,0,256);
        Flash_PageRead(loop,gbuf,0);
        u16 loop1;
        printf("\r\n第%d page:\r\n\r\n\r\n",loop);
        for(loop1 = 0;loop1 < 256;loop1++)
            printk("%d ",gbuf[loop1]);
        Djy_EventDelay(1000*1000);
    }
}

//文件系统调试

void FileSys_TestInit(void)
{
    extern s32 ModuleInstall_EFS(const char *target, u32 opt, void *config);
    ModuleInstall_EFS("efs",MS_INSTALLCREAT,64*1024);
//    extern s32 ModuleInstall_YAF2(const char *target, u32 opt, void *data);
//    ModuleInstall_YAF2("yaf2", MS_INSTALLCREAT, NULL);
    extern s32 ModuleInstall_NorFlash(const char *TargetFs,u32 bstart, u32 bend, u32 doformat);
    ModuleInstall_NorFlash("efs", 0, -1, 0);
}

//文件系统调试

void YaF2FileSystem_Test(void)
{
    u8 tmp[128] = {0};
    u8 loop;

    for(loop =0;loop < 128;loop++)
    {
        tmp[loop] = loop;
    }

    extern s32 ModuleInstall_EFS(const char *target, u32 opt, void *config);
    ModuleInstall_EFS("efs",MS_INSTALLCREAT,64*1024);

    extern s32 ModuleInstall_NorFlash(const char *TargetFs,u32 bstart, u32 bend, u32 doformat);
    ModuleInstall_NorFlash("efs", 0, -1, 1);

    FILE *fp = fopen("/efs/test.txt","w+");
    if(fp == NULL)
    {
        printf("Creat test.txt failed\r\n");
        return;
    }
    else
    {
        printf("Creat test.txt success\r\n");
    }
    u8 len = fwrite(tmp,128,1,fp);
    if(len == 1)
    {
        printf("write file success\r\n");
    }else{
        printf("write file failed\r\n");
    }
    fclose(fp);
    fp = fopen("/efs/test.txt","w+");
    if(fp == NULL)
    {
        printf("Creat test.txt failed\r\n");
        return;
    }
    else
    {
        printf("Creat test.txt success\r\n");
    }
    memset(tmp,0,128);
    len = fread(tmp,128,1,fp);
    if(len > 0)
    {
        for(loop = 0;loop < 128;loop++)
        {
            printf("%d ",buf[loop]);
        }
    }
    fclose(fp);
}

//tset
char IpSerAddr[16] = {"192.168.0.60"};
u16  Port = 8080;
#define CN_LEN_MAX (128)
int SerFd   = -1;
ptu32_t Client_Send_Task(void)
{
    struct in_addr  ipaddr;
    struct sockaddr_in address;
    int con = -1;
    u16 port;

    printf("Send Task is Running\r\n");
    extern bool_t TcpInit(void);
    TcpInit();
    inet_aton(IpSerAddr,&ipaddr);
    port = htons((u16)8080);

    SerFd = socket(AF_INET, SOCK_STREAM, 0);
    if(SerFd < 0)
    {
        printf("Create Socket Failed\r\n");
        return (ptu32_t)-1;
    }else{
        u32 timeo = 0xffffffff;  //DO THE TIME OUT SET
        setsockopt(SerFd, SOL_SOCKET, SO_SNDTIMEO, &timeo, sizeof(timeo));
        setsockopt(SerFd, SOL_SOCKET, SO_RCVTIMEO, &timeo, sizeof(timeo));
        address.sin_addr   = ipaddr;
        address.sin_family = AF_INET;
        address.sin_port   = port;
    }

    //connect->in fact socket s is relative to service
    con = connect(SerFd, (struct sockaddr*) &address, sizeof(address));
    while(con < 0)//until connect success
    {
        con = connect(SerFd, (struct sockaddr*) &address, sizeof(address));
    }
    ptu32_t Client_Rev_Task(void);
    u16 net_Rev;
    //启动接收任务
    net_Rev = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS - 1,0,0,
                    Client_Rev_Task,NULL,1000,"RevTask");
              //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
              if(net_Rev!=CN_EVTT_ID_INVALID)
                  Djy_EventPop(net_Rev,NULL,0,NULL,0,0);

    //now send and rev
    while(1)
    {
        send(SerFd, "hello world", sizeof("hello world"), 0);
        Djy_EventDelay(1000*1000);
    }

    return (ptu32_t)0;
}

ptu32_t Client_Rev_Task(void)
{
    int len = 0;
    u8  buf[CN_LEN_MAX];

    printf("Recieve Task is Running\r\n");

    while(1)
    {
        memset(buf,0,CN_LEN_MAX);
        len = recv(SerFd, buf,CN_LEN_MAX, 0);
        if(len > 0)
        {
            printf("rev:%s\r\n",buf);
        }
    }

    return (ptu32_t)0;
}

//net test
void netTest_Init(void)
{
    u16 net_Send;

    net_Send = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
            Client_Send_Task,NULL,1000,"sendTask");
    //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
    if(net_Send!=CN_EVTT_ID_INVALID)
        Djy_EventPop(net_Send,NULL,0,NULL,0,0);

}

//at24c08 test
u8 buf[128] = {0};
void At24c08_Test(void)
{
    u8 loop;
    //init
    for(loop = 0;loop < 128;loop++)
    {
        buf[loop] = loop;
    }
    extern bool_t IIC_ModuleInstall(u8 iic_port);
    IIC_ModuleInstall(0);
    extern bool_t AT24_ModuleInit(char *BusName);
    AT24_ModuleInit("IIC1");
    u32 startAddress = 0;
    extern u32 AT24_WriteBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum);
    AT24_WriteBytes(startAddress,buf,128);
    memset(buf,0,128);
    extern u32 AT24_ReadBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum);
    AT24_ReadBytes(startAddress,buf,128);
    printf("read dat\r\n");
    for(loop = 0;loop < 128;loop++)
    {
        printf("%d ",buf[loop]);
    }
}

void ebedFlashTest(void)
{
    #define CN_TEST_BLOCK_START (60)
    int loop;
    u32 start = CN_TEST_BLOCK_START*4096;
    u32 testCountBlock = 150;
    u32 addressBase = start;

    printf("test begin\r\n");
    extern UINT8 FLASH_Sector_Erase(UINT32 *eraseAdr);
    for(loop = testCountBlock;loop >= 0;loop--)
    {
        addressBase = start + loop *4096;
        u16 i;
        for(i=0;i < 4096/512;i++)
        {
            FLASH_Sector_Erase(addressBase);
            u16 tmpLoop;
            u32 *p = (u32*)addressBase;
            for(tmpLoop = 0;tmpLoop < 512/4;tmpLoop++)
            {
                if(p[tmpLoop] != 0xffffffff)
                {
                    printf("erease failed\r\n");
                }
            }
            addressBase += 512;
        }
    }
    printf("test end\r\n");

    printf("test again begin\r\n");
    extern Flash_BlockEarse(u32 BlockNo);
    for(loop = CN_TEST_BLOCK_START + testCountBlock;loop >= CN_TEST_BLOCK_START;loop--)
    {
        Flash_BlockEarse(loop);
    }
    printf("test again end\r\n");

    printf("test pages\r\n");
    u32 value = 0;
    u32 k;
    u8 tmpbuf[512] = {0};
    for(loop=0;loop<512/4;loop++)
    {
        tmpbuf[loop] = loop;
    }
    extern s32 Flash_PageProgram(u32 Page, u8 *Data, u32 Flags);
    for(loop = 0;loop < 8*120;loop++)
    {
        Flash_PageProgram(CN_TEST_BLOCK_START*8 + 512*loop,tmpbuf,0);
        u8 *tmpRead = (u8*)(CN_TEST_BLOCK_START*8 + 512*loop);
//        u16 k;
//        for(k=0;k<512;k++)
//        {
//            printf("%d ",tmpRead[k]);
//        }
    }

}

ptu32_t djy_main(void)
{
//      extern bool_t ModuleInstall_HardTimer(void);
//      ModuleInstall_HardTimer();
//      extern void SysPerformTest(void);
//      SysPerformTest();
//    netTest_Init();
//    LED_Task();
//    timerTestTask();
//    Rtc_Test();

//    Uart4_TaskInit();

//    W25q64_Test();
//    ebedFlashTest();
    At24c08_Test();

//    YaF2FileSystem_Test();
//    extern u32 Systime_GetTime(void);
	while(1)
	{
// 	    printf("cnt val:%d\r\n",Systime_GetTime());
//	    ledTestTask();
//		printf("hello world!\r\n");
		Djy_EventDelay(1000*1000);
	}
	return 0;
}
