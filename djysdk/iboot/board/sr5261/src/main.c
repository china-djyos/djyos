#include "os.h"
#include "string.h"
#include "IAP.h"
#include <cfg/iboot_config.h>
#include "stddef.h"
#include "cpu_peri.h"
#include "uartctrl.h"
#include <osboot.h>
#include "IAP_Inner.h"
#include <shell.h>


extern void HAL_ClrUpdateFlag(void);
extern s32 APP_Update(void);
extern void RunApp(void);
extern void CPU_Led_Ctrl(uint8_t idx, uint8_t flag);
extern bool_t HAL_CheckUpdateFlag(void);
extern tagIapVar pg_IapVar;
extern void HardDrv_Test(void);




















static u8 can_snd_buf[13]={0x88,0x11,0x011,0x11,0x11,0x55,0x55,0x55,0x55,0xff,\
		0x00,0xff,0x00};
static u32 gs_SndCnt=0,gs_RcvCnt=0;
static u8 can_rcv_buf[13];



#define TX
//#define RX

static void CAN_Test(void)
{
	u32 len=0,i;
//#ifdef TX
	len=CAN_WriteData(0,can_snd_buf,13);
	if(len==13)
	{
		printf("Snd data success.\r\n");
		gs_SndCnt++;
	}
	else
	{
		printf("Snd data failed.\r\n");
	}
//#endif
	Djy_EventDelay(10*mS);

//#ifdef RX
	len=CAN_ReadData(0,can_rcv_buf,13,NULL);
	if(len==13)
	{
		gs_RcvCnt++;
		printf("Recv data...");
		for(i=0;i<len;i++)
		{
		   printf("%x ",can_rcv_buf[i]);
		}
		printf("\r\n");
	}
	else
	{
		printf("Recv data failed.\r\n");
	}
//#endif
}





static void Led_Run(void)
{
	bool_t ret=true;
	printf("Led Thread Run...\r\n");
	while(1)
	{
	   if(ret)
	   {
		   CPU_Led_Ctrl(0,1);
		   ret=false;
	   }
	   else
	   {
		   CPU_Led_Ctrl(0,0);
		   ret=true;
	   }

//	   CAN_Test(1);
	   Djy_EventDelay(1000*mS);

	}
}




void djy_main(void)
{
    bool_t Ret;
    s32 result=-1;
    u16 LedEvtt,BspTestEvtt,CanTestEvtt;
    static bool_t flag=false;
    extern void Sh_GetStatus(char *param);
    extern void Sh_GetRunMode(char *param);
    Sh_GetRunMode(NULL);
    Sh_GetStatus(NULL);

//    CAN_Test_Shell_Module_Install();


    LedEvtt = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0, (ptu32_t(*)(void))Led_Run,\
                                    NULL, 0x1000, "Led Monitor Task");
   	if(LedEvtt==CN_EVTT_ID_INVALID)
   	{
   	    printf("Led Monitor Task register failed.\r\n");
   	}
   	else
   	{
   	    Djy_EventPop(LedEvtt, NULL, 0, 0, 0, 0);
   	}
  
   	CanTestEvtt=Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0, (ptu32_t(*)(void))CAN_Test,\
            NULL, 0x1000, "Led Monitor Task");
	if(LedEvtt==CN_EVTT_ID_INVALID)
	{
	printf("Led Monitor Task register failed.\r\n");
	}
	else
	{
	Djy_EventPop(LedEvtt, NULL, 0, 0, 0, 0);
	}




//   	BspTestEvtt= Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0, (ptu32_t(*)(void))HardDrv_Test,\
//            NULL, 0x4000, "BSP Test Task");
//	if(BspTestEvtt==CN_EVTT_ID_INVALID)
//	{
//	   printf("BSP Test Task register failed.\r\n");
//	}
//	else
//	{
//	   Djy_EventPop(BspTestEvtt, NULL, 0, 0, 0, 0);
//	}



    while(1)
    {
	   Ret=HAL_CheckUpdateFlag();
	   if(Ret)
	   {
		   if(!flag)
		   {
			  printf("APP need update.\r\n");
			  flag=true;
		   }
//		   result=APP_Update();
		   if(result!=-1)
		   {
			   HAL_ClrUpdateFlag();
			   printf("\r\nUpdate APP success.Now going on running APP in 5 seconds.\r\n");
			   Djy_EventDelay(5000*mS);
			   RunApp();
		   }
		   else
		   {
			   printf("Update APP Failed.\r\n");
		   }
	   }
	   else if(pg_IapVar.IbootStatus==EN_CRC_ERR)
	   {
//		   result=APP_Update();
		   if(result!=-1)
		   {
			   printf("\r\nUpdate APP success.Now going on running APP in 5 seconds.\r\n");
			   Djy_EventDelay(5000*mS);
			   RunApp();
		   }
		   else
		   {
			   printf("Update APP Failed.\r\n");
		   }
	   }
	   else
	   {

	   }
	   Djy_EventDelay(5000*mS);
    }
}

