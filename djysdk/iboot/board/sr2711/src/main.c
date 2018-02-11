#include "os.h"
#include "string.h"
#include "IAP.h"
#include <cfg/iboot_config.h>
#include "stddef.h"
#include "cpu_peri.h"
#include "uartctrl.h"
#include <osboot.h>
#include "IAP_Inner.h"


extern void HAL_ClrUpdateFlag(void);
extern s32 APP_Update(void);
extern void RunApp(void);
extern void CPU_Led_Ctrl(uint8_t idx, uint8_t flag);
extern bool_t HAL_CheckUpdateFlag(void);
extern tagIapVar pg_IapVar;

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
	   Djy_EventDelay(1000*mS);

	}
}




void djy_main(void)
{
    bool_t Ret;
    s32 result=-1;
    u16 LedEvtt;
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
		   result=APP_Update();
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
		   result=APP_Update();
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

