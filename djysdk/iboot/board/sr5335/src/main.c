#include "os.h"
#include "string.h"
#include "IAP.h"
#include <cfg/iboot_config.h>
#include "stddef.h"
#include "cpu_peri.h"
#include "uartctrl.h"
#include <osboot.h>
#include <IAP_Inner.h>

extern bool_t HAL_ClrUpdateFlag(void);
extern bool_t HAL_CheckUpdateFlag(void);
extern tagIapVar pg_IapVar;
extern void RunApp(void);
extern s32 IAP_Update(char *pDevPath);

extern const char *g_AppUpdatePath;

void AD_Test(void)
{
	 u64 starttime=0;
	 u64 endtime=0;
	 u32 ctime=0;
	 atom_low_t atom;
	 extern sint8_t AD_GetTemp(void);
	 starttime=DjyGetSysTime();
	 AD_GetTemp();
	 endtime=DjyGetSysTime();
	 ctime=endtime-starttime;
	 printf("Temp time:%d.\r\n",ctime);
	 extern u32 AD_GetVolt(void);
	 atom = Int_LowAtomStart();
	 starttime=DjyGetSysTime();
	 AD_GetVolt();
	 endtime=DjyGetSysTime();
	 ctime=endtime-starttime;
	 Int_LowAtomEnd(atom);
	 printf("Volt time:%d.\r\n",ctime);
}

static void Led_Run(void)
{
	bool_t ret=true;
	printf("Led Thread Run...\r\n");
	while(1)
	{
	   if(ret)
	   {
		   CPU_Led_Ctrl(1,1);
		   ret=false;
	   }
	   else
	   {
		   CPU_Led_Ctrl(1,0);
		   ret=true;
	   }
	   Djy_EventDelay(1000*mS);
	}
}

void djy_main(void)
{
    bool_t ret=true;
    bool_t Ret;
    s32 result=2;
    u16 LedEvtt,HardTestEvtt;
    static bool_t flag=false;
    extern void Sh_GetStatus(char *param);
    extern void Sh_GetRunMode(char *param);
    Sh_GetRunMode(NULL);
    Sh_GetStatus(NULL);

    LedEvtt = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0, (ptu32_t(*)(void))Led_Run,\
    	                                    NULL, 0x400, "Led Monitor Task");
   if(LedEvtt==CN_EVTT_ID_INVALID)
   {
		printf("Led Monitor Task register failed.\r\n");
   }
   else
   {
		Djy_EventPop(LedEvtt, NULL, 0, 0, 0, 0);
   }
   #if 0
	HardTestEvtt=Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0, (ptu32_t(*)(void))HardDrv_Test,\
			   NULL, 0x4000, "HardDrv Test Task");
	if(HardTestEvtt==CN_EVTT_ID_INVALID)
	{
		 printf("HardDrv Test Task register failed.\r\n");
	}
	else
	{
		 Djy_EventPop(HardTestEvtt, NULL, 0, 0, 0, 0);
	}
   #endif



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
		   result=IAP_Update(g_AppUpdatePath);
		   if(result!=-1)
		   {
			   HAL_ClrUpdateFlag();
			   printf("Update APP success.Now going on running APP in 5 seconds.\r\n");
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
		   result=IAP_Update(g_AppUpdatePath);
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

