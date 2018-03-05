// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：max11410.c
// 模块描述: AD芯片max11410的驱动
// 模块版本: V1.00
// 创建人员: czz
// 创建时间: 9/8.2017
// =============================================================================

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "os.h"
#include "spibus.h"
#include "cpu_peri.h"
#include "max11410.h"
#include "math.h"
#include "shell.h"

static struct SPI_Device s_Max_Dev;
static u32 s_Max_Timeout = CN_TIMEOUT_FOREVER;
#define Max_SPI_SPEED      (400*1000)
static bool_t sMaxInited = false;
static bool_t Max11410_config = true;

#define ID_Date  0xf02

const static double A =(0.0039083);
const static double B =(-0.0000005775);
__attribute__((weak))  const u32 Max11410_ref = (4000);
__attribute__((weak))  const bool_t Max11410_RxAadjust = false;
struct Max11410_CfgTab config;
static struct SemaphoreLCB Max_Semp;         //芯片互斥访问保护


__attribute__((weak)) bool_t Max11410_Init(struct Max11410_CfgTab *config)
{
    Max11410_config = false;
    return false;
}
// =============================================================================
// 函数功能：与芯片数据交互
// 输入参数：无
// 返回值  ：
// =============================================================================
static u32 Max11410_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen,u32 RecvOff)
{
    struct SPI_DataFrame data;
    s32 result;
    if(rlen != 0)
        *sdata |=0x80;
    else
        *sdata &=~0x80;
    data.RecvBuf = rdata;
    data.RecvLen = rlen;
    data.RecvOff = RecvOff;
    data.SendBuf = sdata;
    data.SendLen = slen;
    SPI_CsActive(&s_Max_Dev,s_Max_Timeout);
    result = SPI_Transfer(&s_Max_Dev,&data,true,s_Max_Timeout);
    SPI_CsInactive(&s_Max_Dev);
    if(result != CN_SPI_EXIT_NOERR)
        return 0;
    return 1;
}


//=============================================================================
//功能：获取芯片ID
//参数：参数：
//返回：ID
//=============================================================================
static u32 Max11410_GetID(void)
{
    u8 addr = Max11410_PART_ID;
    u8 rdid[3];
    u32 ID;
    Max11410_TxRx(&addr,1,rdid,3,1);
    ID = (rdid[0]<<16)|(rdid[1]<<8)|(rdid[2]);
    return ID;
}

//=============================================================================
//功能：获取芯片ID
//参数：参数：
//返回：ID
//=============================================================================
static u8 Max11410_GetuC(void)
{
    u8 addr = Max11410_uCADDR;
    u8 uCaddr;
    Max11410_TxRx(&addr,1,&uCaddr,1,1);
    return uCaddr;
}
//=============================================================================
//功能：设置Max11410急促安琪为默认初始值
//参数：参数：
//返回：true = 成功初始化，false = 初始化失败
//=============================================================================
static bool_t Max11410_Config(struct Max11410_CfgTab *pt_config)
{
    u8 i;

    if(ID_Date != Max11410_GetID())
    {
        printf("MAX11410 ID Error!!\n\r");
        return false;
    }
    for(i=0;i<pt_config->Reg16BitNum;i++)
    {
        Max11410_TxRx((u8*)&pt_config->pt_Reg16Bit[i],sizeof(struct Init_Reg16Bit),NULL,0,0);
    }
    for(i=0;i<pt_config->Reg8BitNum;i++)
    {
        Max11410_TxRx((u8*)&pt_config->pt_Reg8Bit[i],sizeof(struct Init_Reg8Bit),NULL,0,0);
    }
    Djy_DelayUs(10*mS);
    if(0 != Max11410_GetuC())
    {
        sMaxInited = true;
        return true;
    }
    return false;
}

// =============================================================================
// 函数功能：读温度(注意：函数类型为float，必须声名，否则返回值出错)
// 输入参数：无
// 返回值  ：温度值，为float类型；TEMP_ERROR，表示出错
// =============================================================================
float Max11410_TemperatureGet(u8 temp_x)
{
    float temp;
    u8 RegMatrix;
    u8 Data[3] = {0x00,0x00,0x00};
    u32 DATA;
    float RX,Rxadjust;
    double num;

    if((!sMaxInited))
    {
        printf("MA11410 Init Error !!  \n\r");
        return TEMP_ERROR;
    }
    if((temp_x>4))
    {
        printf("MA11410 Channel Error !!  \n\r");
        if(!Max11410_Config(&config))
            return TEMP_ERROR;
    }
    if((false == Lock_SempPend(&Max_Semp,25*mS)))
        return -1;

    RegMatrix = Max11410_DATA0+temp_x-1;
    Max11410_TxRx(&RegMatrix,1,Data,3,1);
    DATA = ((Data[0]<<16)|(Data[1]<<8)|(Data[2]));

    if((DATA != 0xfffffff)&&(DATA != 0x0))
    {
        RX = ((float)DATA/0xffffff)*Max11410_ref;
        if(Max11410_RxAadjust)
        {
            RegMatrix = Max11410_DATA0+temp_x-1+4;
            Max11410_TxRx(&RegMatrix,1,Data,3,1);
            DATA = ((Data[0]<<16)|(Data[1]<<8)|(Data[2]));
            Rxadjust = ((float)DATA/0xffffff)*Max11410_ref;
            RX = (RX- Rxadjust)+1000;
//            printf("RX%d=%.4f aj=%.4f  ",temp_x,RX,Rxadjust);
        }
        num = sqrt(A*A-4*B*((1000-RX)/1000));
        temp = (num-A)/(2*B);
    }
    else
    {
        temp =  TEMP_ERROR;
    }
    temp = ((-50<temp) && (temp<100)) ? temp : (TEMP_ERROR);
    Lock_SempPost(&Max_Semp);
    return temp;

}
//=============================================================================
//功能： shell 相关
//参数：
//返回：
//=============================================================================
extern char *Sh_GetWord(char *buf,char **next);
static bool_t Max11410_shell(char *param)
{
    ptu32_t ch;
    char *word,*next_param;
    float temp;

    word = Sh_GetWord(param,&next_param);

    if(0==strcmp(word,"info"))
    {
        if(sMaxInited)
            printf("pt 初始化完成 !!\n\r" );
        else
            printf("pt 初始化失败 !!\n\r" );
        if(Max11410_RxAadjust)
            printf("pt 工作在校准状态 !! \n\r" );
        else
            printf("pt 工作在非校准状态 !!\n\r" );
        printf("uC Addr = 0x%x \n\r",Max11410_GetuC());
        if(!Max11410_config)
            printf("没有找到配置信息!!\n\r" );

    }
    else if(0==strcmp(word,"get"))
    {
        word = Sh_GetWord(next_param,&next_param);
        ch = strtoul(word, (char **)NULL, 0);
        temp =  Max11410_TemperatureGet(ch);
        if (temp == TEMP_ERROR)
            printf("通道%d 温度读取失败   !!  \n\r",ch);
        else
            printf("temp%d = %.4f °C !!  \n\r",ch,temp);
    }
    else
    {
        printf("输入'help pt' 获取帮助 .\n\r");
    }

    return true;
}

/*******************************************************************************
功能:
参数:无.
返回值:1。
*********************************************************************************/
static struct ShellCmdTab const shell_cmd_max11410_table[]=
{
    {
            "pt",
            Max11410_shell,
            "测温度芯片相关命令",
            "pt info      获取max11410的工作状态  . \n\r"
            "pt get x  获取“x”通道的温度值 x = 1,2... \n\r"
    }
};

#define CN_MAX11410_SHELL_NUM  sizeof(shell_cmd_max11410_table)/sizeof(struct ShellCmdTab)
static struct ShellCmdRsc tg_max11410_shell_cmd_rsc[CN_MAX11410_SHELL_NUM];

/*******************************************************************************
功能:
参数:无.
返回值:1。
*********************************************************************************/
static ptu32_t Shell_Module_Install(void)
{
    Sh_InstallCmd(shell_cmd_max11410_table,tg_max11410_shell_cmd_rsc,CN_MAX11410_SHELL_NUM);
    return 1;
}

//=============================================================================
//功能：初始化Max11410芯片，若采用连续读模式，则配置其模式为连续采样
//参数：参数：
//     BusName,总结名称，如“SPI1”
//返回：true = 成功初始化，false = 初始化失败
//=============================================================================
bool_t ModuleInstall_Max11410(char *BusName)
{

    if(NULL == Lock_SempCreate_s(&Max_Semp,1,1,CN_BLOCK_FIFO,"MAX11410_semp"))
        return false;

    if(Max11410_Init(&config) ==false)
        return false;

    Shell_Module_Install();
    s_Max_Dev.AutoCs = false;
    s_Max_Dev.CharLen = 8;
    s_Max_Dev.Cs = 0;
    s_Max_Dev.Freq = Max_SPI_SPEED;
    s_Max_Dev.Mode = SPI_MODE_0;
    s_Max_Dev.ShiftDir = SPI_SHIFT_MSB;

    if(NULL != SPI_DevAdd_s(&s_Max_Dev,BusName,"MAX11410"))
    {
        SPI_BusCtrl(&s_Max_Dev,CN_SPI_SET_POLL,0,0);

        if(Max11410_Config(&config))
        {
            sMaxInited = true;
        }
    }
    return sMaxInited;
}

