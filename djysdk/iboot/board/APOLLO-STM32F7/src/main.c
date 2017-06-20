#include <cpu_peri_gpio.h>
#include <djyos.h>
#include <stdint.h>
#include <stdio.h>
#include <systime.h>
#include "shell.h"
#include "../../../../../djysrc/bsp/arch/arm/arm32_stdint.h"
#include "hmi-input.h"


// =============================================================================
// 功能: LED TEST
// 参数:无
// 返回: 无
// =============================================================================
ptu32_t Led_Test(void)
{
	for(;;)
	{
		GPIO_SettoHigh(GPIO_B, PIN1);
		GPIO_SettoLow(GPIO_B, PIN0);
		Djy_EventDelay(500*mS);
		GPIO_SettoLow(GPIO_B, PIN1);
		GPIO_SettoHigh(GPIO_B, PIN0);
		Djy_EventDelay(500*mS);
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
static u8 debug_test=false;

static bool_t __debugOn(char *param)
{
    debug_test = true;
    return true;
}

static bool_t __debugOff(char *param)
{
    debug_test = false;
    return true;
}

static struct ShellCmdTab  gEthRawDebug[] =
{
    {
        "debugon",
        __debugOn,
        "usage:debugon",
        NULL
    },
    {
        "debugoff",
        __debugOff,
        "usage:debugoff",
        NULL
    }
};

#define CN_ETHCAN_DEBUGNUM  ((sizeof(gEthRawDebug))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gEthRawDebugCmdRsc[CN_ETHCAN_DEBUGNUM];

static bool_t shell_test(void)
{
    bool_t result;

    result = Sh_InstallCmd(gEthRawDebug,gEthRawDebugCmdRsc,CN_ETHCAN_DEBUGNUM);

    return result;
}

static const char* KeyName;

ptu32_t Key_Test(void)
{
    tpInputMsgQ pMsgQ;
    struct InputDeviceMsg msg;
    struct KeyBoardMsg * KeyBoard;
    u8 key=0;//低4位表示key的案件状泰

    pMsgQ=HmiIn_CreatInputMsgQ(20,"Key_msg");
    HmiIn_SetFocus((char*)KeyName,pMsgQ);
    while(1)
    {
        if(HmiIn_ReadMsg(pMsgQ, &msg, CN_TIMEOUT_FOREVER) != false)
        {
            if (msg.input_type == EN_HMIIN_KEYBOARD)
            {
                KeyBoard = (struct KeyBoardMsg*) (&msg.input_data);

                if(KeyBoard->key_value[1]==0)
                    key|=1<<(KeyBoard->key_value[0]-1);
                else
                    key&=~(1<<(KeyBoard->key_value[0]-1));
            }
            printf("key flag:%d %d %d %d     \r",(key)&1,(key>>1)&1,(key>>2)&1,(key>>3)&1);

        }
    }
}

void Key_Init(const char* name)
{
    u16 evtt_key;
    KeyName=name;
    evtt_key = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
                Key_Test,NULL,1000,"Key_Test");

    if(evtt_key!=CN_EVTT_ID_INVALID)
        Djy_EventPop(evtt_key,NULL,0,NULL,0,0);

}



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




void djy_main(void)
{
	led_init();
	printf("Run Mode:Iboot.\r\n");
//	fs_test();

//	iic_test( );

    for(;;)
    {
    	Djy_EventDelay(1000*mS);
    }


}


