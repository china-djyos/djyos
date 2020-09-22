/*
 * cpu_peri_qspi.c
 *
 *  Created on: 2020年5月6日
 *      Author: CK
 */


// =============================================================================
#include "string.h"
#include "djyos.h"
#include "cpu_peri.h"
#include <board.h>
#include <dbug.h>
// =============================================================================
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。


//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern s32 ModuleInstall_QSPI(void);
//    ModuleInstall_QSPI();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip qspi"    //片内sdcard驱动
//parent:"none"                         //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                         //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                      //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                        //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                      //初始化时机，可选值：early，medium，later, pre-main。
                                        //表示初始化时间，分别是早期、中期、后期
//dependence:none"                      //该组件的依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件将强制选中，
                                        //如果依赖多个组件，则依次列出
//weakdependence:"none"                 //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件不会被强制选中，
                                        //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                          //该组件的互斥组件名（可以是none，表示无互斥组件），
                                        //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_SDCARD == false )
//#warning  " cpu_onchip_uart  组件参数未配置，使用默认配置"
//%$#@target = header    //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_SDCARD    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,255,
#define CFG_QSPI_CLOCK_PRESCALER    2   //设置QSPI的时钟为AHB时钟的1/(CFG_QSPI_CLOCK_PRESCALER + 1)
//%$#@num,0,31,
#define CFG_QSPI_FIFO_THRESHOLD_LEVEL    4   //设置QSPI的FIFO的阈值为CFG_QSPI_FIFO_THRESHOLD_LEVEL
#define CFG_QSPI_FLASH_SIZE              24  //Flash 中的字节数 = 2^[FSIZE+1]
//%$#@num,1,8,
#define CFG_QSPI_CHIP_SELECT_HIGH_TIME   4  //片选高电平时间
//%$#@enum,1,2
#define CFG_QSPI_FLASH_ID    1   //该QSPI支持控制两个flash，这里选择，接的是哪个flash。
#define CFG_QSPI_DDR_HOLD    1    //1：使用模拟延迟来延迟数据输出,2：数据输出延迟 1/4 个 QUADSPI 输出时钟周期。
//%$#@enum,0,3
#define CFG_QSPI_CK_MODE     0   //时钟模式0或者3
#define CFG_QSPI_ALTERNATE_BYTES_MODE     0   //交替字节模式 0:无交替字节,1:单线传输交替字节 ,2:双线传输交替字节 ,3:四线传输交替字节
#define CFG_QSPI_ALTERNATE_BYTES_SIZE     0   //交替字节长度 0:8 位交替字节,1:16 位交替字节 ,2:24 位交替字节 ,3:32 位交替字节
//%$#@enum,true,false
#define CFG_QSPI_DUAL_FLASH_MODE    false   //是否开启双闪存模式。
#define CFG_QSPI_SAMPLE_SHIFT       true    //是否采样移位半个周期。
#define CFG_QSPI_SIOO               false    //是否仅发送指令一次模式。
#define CFG_QSPI_DDR_ENABLE         false    //是否使能DDR。
//%$#@num,0,4294967295,
#define CFG_QSPI_TIMEOUT           5000  //QSPI执行的超时时间
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
// ============================================================================



#define     REG     0

QSPI_HandleTypeDef QSPI_Handler;


#define     QSPI_SR_TCF     1 << 1      //传输完成标志
#define     QSPI_SR_FTF     1 << 2      //FIFO阈值标志
#define     QSPI_SR_BUSY    1 << 5      //忙
//-----------------------------------------------------------------------------
//功能: 等待状态标志
//参数: flag： 等待的标志位，state：等待的状态；time：等待次数
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t QSPI_WaitFlag(u32 flag, u8 state, u32 time)
{
    u8 now_state=0;
    while(time)
    {
        now_state = (QUADSPI->SR & flag) ? 1 : 0;
        if(now_state == state)
            break;
        time--;
    }
    if(time)
        return true;
    else
        return false;
}

void QSPI_HandingErrorState(void)
{
    u8 fifo_data = 0;
    u8 read_fifo;
    __IO u32 *data_reg = &QUADSPI->DR;

    if(QSPI_Handler.State == HAL_QSPI_STATE_ERROR)
    {
        notice_printf("QSPI", " handing qspid error state.\r\n");
        while(QSPI_WaitFlag(QSPI_SR_BUSY, 0, 0xffff) == false)
        {
            while(QSPI_WaitFlag(QSPI_SR_FTF, 0, 0xffff) == false)
            {
                fifo_data = (QUADSPI->SR >> 8);
                if(fifo_data > 0x20)
                    printf(" >20 ");
                while(fifo_data > 0)
                {
                    read_fifo = *(__IO u8 *)data_reg;
                    fifo_data --;
                }

            }
            printf(" .");
        }
        QSPI_Handler.State = HAL_QSPI_STATE_READY;
    }
}


//-----------------------------------------------------------------------------
//功能: 命令发送
//参数: cmd：发送的命令，addr：目标地址，mode：模式，dmcycle：空指令周期数
//  mode[1:0]:指令模式;00,无指令;01,单线传输指令;10,双线传输指令;11,四线传输指令
//  mode[3:2]:地址模式;00,无地址;01,单线传输地址;10,双线传输地址;11,四线传输地址
//  mode[5:4]:地址长度;00,8位地址;01,16位地址;10,24位地址;11,32位地址.
//  mode[7:6]:数据模式;00,无数据;01,单线传输数据;10,双线传输数据;11,四线传输数据.
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
#if REG
bool_t QSPI_SendCMD(u8 cmd,u32 addr,u8 mode,u8 dmcycle)
#else
bool_t QSPI_Send_CMD(u32 instruction,u32 addr,u32 dummy_cycles,u32 instruction_mode,u32 addr_mode,u32 addr_size,u32 data_mode)
#endif
{
#if REG
    u32 reg;

    if(QSPI_WaitFlag(QSPI_SR_BUSY, 0 , 0XFFFF))
    {
        reg = 0 << 31;      //禁止DDR模式
        reg |= CFG_QSPI_SIOO << 28;     //每次都发送指令
        reg |= 0 << 26;     //间接写模式
        reg |= ((u32)mode>>6) << 24;     //设置数据模式
        reg |= (u32)dmcycle << 18;       //设置空指令周期数
        reg |= ((u32)mode>>4 & 0x03) << 12;     //设置地址长度
        reg |= ((u32)mode>>2 & 0x03) << 10;     //设置地址模式
        reg |= ((u32)mode>>0 & 0x03) << 8;      //设置指令模式
        reg |= ((u32)cmd & 0xff);        //设置指令
        QUADSPI->CCR = reg;

        if(mode & 0x0c)
        {
            QUADSPI->AR = addr;     //设置地址寄存器
        }
        if((mode & 0xc0) == 0)
        {
            if(QSPI_WaitFlag(QSPI_SR_TCF, 1, 0xFFFF))   //等待传输完成
            {
                QUADSPI->FCR |= QSPI_SR_TCF;    //清除TCF标志位
            }
        }
        return  true;
    }

    return  false;
#else
    QSPI_CommandTypeDef Cmdhandler;
    HAL_StatusTypeDef ret = HAL_ERROR;
    u8 i = 0;

    Cmdhandler.Instruction=instruction;     //指令
    Cmdhandler.Address=addr;             //地址
    Cmdhandler.DummyCycles=dummy_cycles;     //空周期数
    Cmdhandler.InstructionMode=instruction_mode; //指令模式
    Cmdhandler.AddressMode=addr_mode;         //地址模式
    Cmdhandler.AddressSize=addr_size;         //地址长度
    Cmdhandler.DataMode=data_mode;               //数据模式
    if(CFG_QSPI_SIOO)
        Cmdhandler.SIOOMode=QSPI_SIOO_INST_ONLY_FIRST_CMD;   //仅为第一条命令发送指令
    else
        Cmdhandler.SIOOMode=QSPI_SIOO_INST_EVERY_CMD;   //每次都发送指令

    switch(CFG_QSPI_ALTERNATE_BYTES_MODE)
    {
        case 0:
            Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_NONE; //无交替字节
            break;
        case 1:
            Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_1_LINE; //单线传输交替字节
            break;
        case 2:
            Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_2_LINES; //双线传输交替字节
            break;
        case 3:
            Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_4_LINES; //四线传输交替字节
            break;
        default:
            error_printf("QSPI","Alternate byte mode error.\r\n");
           break;
    }

    switch(CFG_QSPI_ALTERNATE_BYTES_SIZE)
    {
        case 0:
            Cmdhandler.AlternateBytesSize=QSPI_ALTERNATE_BYTES_8_BITS; //8位替字节
            break;
        case 1:
            Cmdhandler.AlternateBytesSize=QSPI_ALTERNATE_BYTES_16_BITS; //16位替字节
            break;
        case 2:
            Cmdhandler.AlternateBytesSize=QSPI_ALTERNATE_BYTES_24_BITS; //24位替字节
            break;
        case 3:
            Cmdhandler.AlternateBytesSize=QSPI_ALTERNATE_BYTES_32_BITS; //32位替字节
            break;
        default:
            error_printf("QSPI","Alternate byte size error.\r\n");
           break;
    }

    if(CFG_QSPI_DDR_ENABLE)
        Cmdhandler.DdrMode=QSPI_DDR_MODE_ENABLE;               //打开DDR
    else
        Cmdhandler.DdrMode=QSPI_DDR_MODE_DISABLE;               //关闭DDR
    if(CFG_QSPI_DDR_HOLD == 1)
        Cmdhandler.DdrHoldHalfCycle=QSPI_DDR_HHC_ANALOG_DELAY;
    else
        Cmdhandler.DdrHoldHalfCycle=QSPI_DDR_HHC_HALF_CLK_DELAY;

    ret = HAL_QSPI_Command(&QSPI_Handler,&Cmdhandler,CFG_QSPI_TIMEOUT);
    if(ret != HAL_OK)
    {
        while(i++ < 3)
        {
            if(ret != HAL_OK)
            {
                QSPI_HandingErrorState();
            }
            else
                break;
            ret = HAL_QSPI_Command(&QSPI_Handler,&Cmdhandler,CFG_QSPI_TIMEOUT);
        }
    }

    if(ret == HAL_OK)
        return  true;
    else
    {
        error_printf("qspi", "cmd send fail\r\n");
        return  false;
    }
#endif
}

//-----------------------------------------------------------------------------
//功能: QSPI接收指定长度的数据
//参数: buf： 接收数据缓冲区，len：数据长度
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t QSPI_Receive(u8 *buf,u32 len)
{
#if REG
    u32 reg = QUADSPI->CCR;
//    u32 addr_reg = QUADSPI->AR;
    bool_t sta = false;

    QUADSPI->DLR = len - 1;     //设置数据传输长度
    reg &= ~(3 << 26);          //清除原来的功能模式
    reg |= 1 << 26;             //设置为间接读功能模式
    QUADSPI->FCR |= QSPI_SR_TCF;    //清除TCF标志位
    QUADSPI->CCR = reg;
//    QUADSPI->AR = addr_reg;
    while(len)
    {
        sta = QSPI_WaitFlag(QSPI_SR_TCF | QSPI_SR_FTF, 1, 0xFFFF);
        if(sta)
        {
            *buf++ =  *(u8 *)QUADSPI->DR;
            len --;
        }
        else
            break;
    }
    if(sta)
    {
        QUADSPI->CR |= 1 << 1;
        sta = QSPI_WaitFlag(QSPI_SR_TCF, 1, 0xFFFF);
        if(sta)
        {
            QUADSPI->FCR |= QSPI_SR_TCF;    //清除TCF标志位
            sta = QSPI_WaitFlag(QSPI_SR_BUSY, 0, 0xFFFF);
        }
    }
    return sta;
#else

    u8 i = 0;
    HAL_StatusTypeDef ret = HAL_ERROR;
    __IO u32 *data_reg = &QUADSPI->DR;

    QSPI_Handler.Instance->DLR=len-1;       //配置数据长度

    ret = HAL_QSPI_Receive(&QSPI_Handler, buf, CFG_QSPI_TIMEOUT);
    if(ret != HAL_OK)
    {
        while(i++ < 3)
        {
            if(ret != HAL_OK)
            {
                QSPI_HandingErrorState();
            }
            else
                break;
            ret = HAL_QSPI_Receive(&QSPI_Handler, buf, CFG_QSPI_TIMEOUT);
        }
    }

    if(ret == HAL_OK)
    {
        return true;
    }
    else
    {
        error_printf("qspi", "receive error\r\n");
        return false;
    }
#endif
}

//-----------------------------------------------------------------------------
//功能: QSPI发送指定长度的数据
//参数: buf： 接收数据缓冲区，len：数据长度
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
u8 QSPI_Transmit(u8 *buf,u32 len)
{
#if REG
    u32 reg = QUADSPI->CCR;
//    u32 addr_reg = QUADSPI->AR;
    bool_t sta = false;

    QUADSPI->DLR = len - 1;     //设置数据传输长度
    reg &= ~(3 << 26);          //清除原来的功能模式
    reg |= 0 << 26;             //设置为间接写功能模式
    QUADSPI->FCR |= QSPI_SR_TCF;    //清除TCF标志位
    QUADSPI->CCR = reg;
//    QUADSPI->AR = addr_reg;
    while(len)
    {
        sta = QSPI_WaitFlag(QSPI_SR_FTF, 1, 0xFFFF);
        if(sta)
        {
            *(u8 *)QUADSPI->DR = *buf++;
            len --;
        }
        else
            break;
    }
    if(sta)
    {
        QUADSPI->CR |= 1 << 1;
        sta = QSPI_WaitFlag(QSPI_SR_TCF, 1, 0xFFFF);
        if(sta)
        {
            QUADSPI->FCR |= QSPI_SR_TCF;    //清除TCF标志位
            sta = QSPI_WaitFlag(QSPI_SR_BUSY, 0, 0xFFFF);
        }
    }
    return sta;
#else
    u8 i = 0;
    HAL_StatusTypeDef ret = HAL_ERROR;

    QSPI_Handler.Instance->DLR=len-1;

    ret = HAL_QSPI_Transmit(&QSPI_Handler,buf,CFG_QSPI_TIMEOUT);
    if(ret != HAL_OK)
    {
        while(i++ < 3)
        {
            if(ret != HAL_OK)
            {
                QSPI_HandingErrorState();
            }
            else
                break;
            ret = HAL_QSPI_Transmit(&QSPI_Handler,buf,CFG_QSPI_TIMEOUT);
        }
    }

    if(ret == HAL_OK)
    {
        return true;
    }
    else
    {
        error_printf("qspi", "transmit error\r\n");
        return false;
    }
#endif
}
//-----------------------------------------------------------------------------
//功能: QSPI初始化
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t QSPI_Init(void)
{
#if REG
    u32 reg = 0;

    QSPI_GpioInit();
    if(QSPI_WaitFlag(QSPI_SR_BUSY, 0 , 0XFFFF))
    {
        reg= CFG_QSPI_CLOCK_PRESCALER << 24;   //设置QSPI的时钟为AHB时钟的1/3,216/3 = 72Mhz = 13.8ns
        reg|= CFG_QSPI_FIFO_THRESHOLD_LEVEL << 8;   //FIFO的阈值为4
        if(CFG_QSPI_FLASH_ID == 1)
            reg|= 0 << 7;   //选择FLASH1
        else
            reg|= 1 << 7;   //选择FLASH2

        if(CFG_QSPI_DUAL_FLASH_MODE)
            reg|= 1 << 6;   //使能双闪存模式
        else
            reg|= 0 << 6;   //禁止双闪存模式

        if(CFG_QSPI_SAMPLE_SHIFT)
            reg|= 1 << 4;   //采样移位半个周期
        else
            reg|= 0 << 4;   //采样不发生移位
        QUADSPI->CR = reg;

        reg = CFG_QSPI_FLASH_SIZE << 16; //设置flash大小为 2^25 = 32M
        reg|= (CFG_QSPI_CHIP_SELECT_HIGH_TIME - 1) << 8;   //设置片选高电平时间为4个时钟13.8ns*4=55.2ns，看手册
        if(CFG_QSPI_CK_MODE)
            reg|= 1 << 0;   //nCS 为高电平（片选释放）时， CLK 必须保持高电平。这称为模式 3
        else
            reg|= 0 << 0;
        QUADSPI->DCR = reg;
        QUADSPI->CR |= 1 << 0;  //使能QSPI
    }
    else
        return false;

    return true;
#else
    QSPI_Handler.Instance=QUADSPI;
    QSPI_Handler.Init.ClockPrescaler=CFG_QSPI_CLOCK_PRESCALER;     //设置QSPI的时钟,216/(CFG_QSPI_CLOCK_PRESCALER+1),如果CFG_QSPI_CLOCK_PRESCALER=2的话。clk = 72Mhz = 13.8ns
    QSPI_Handler.Init.FifoThreshold=CFG_QSPI_FIFO_THRESHOLD_LEVEL;      //FIFO的阈值为

    if(CFG_QSPI_SAMPLE_SHIFT)
        QSPI_Handler.Init.SampleShifting=QSPI_SAMPLE_SHIFTING_HALFCYCLE;    //采样移位半个周期
    else
        QSPI_Handler.Init.SampleShifting=QSPI_SAMPLE_SHIFTING_NONE;    //采样不发生移位

    QSPI_Handler.Init.FlashSize=CFG_QSPI_FLASH_SIZE; //设置flash大小为 2^(CFG_QSPI_FLASH_SIZE + 1)

    switch(CFG_QSPI_CHIP_SELECT_HIGH_TIME)
    {
        case 1:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;   //设置片选高电平时间为1个时钟13.8ns*1
            break;
        case 2:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_2_CYCLE;   //设置片选高电平时间为2个时钟13.8ns*2
            break;
        case 3:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_3_CYCLE;   //设置片选高电平时间为3个时钟13.8ns*3
            break;
        case 4:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_4_CYCLE; //设置片选高电平时间为4个时钟13.8ns*4
            break;
        case 5:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE; //设置片选高电平时间为4个时钟13.8ns*5
            break;
        case 6:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_6_CYCLE; //设置片选高电平时间为4个时钟13.8ns*6
            break;
        case 7:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_7_CYCLE; //设置片选高电平时间为4个时钟13.8ns*7
            break;
        case 8:
            QSPI_Handler.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_8_CYCLE; //设置片选高电平时间为4个时钟13.8ns*8
            break;
        default:
            error_printf("QSPI","Chip select high time error.\r\n");
           break;
    }
    if(CFG_QSPI_CK_MODE)
        QSPI_Handler.Init.ClockMode=QSPI_CLOCK_MODE_3;  //模式 3
    else
        QSPI_Handler.Init.ClockMode=QSPI_CLOCK_MODE_0;  //模式 0

    if(CFG_QSPI_FLASH_ID == 1)
        QSPI_Handler.Init.FlashID=QSPI_FLASH_ID_1;  //选择FLASH1
    else
        QSPI_Handler.Init.FlashID=QSPI_FLASH_ID_2;

    if(CFG_QSPI_DUAL_FLASH_MODE)
        QSPI_Handler.Init.DualFlash=QSPI_DUALFLASH_ENABLE;
    else
        QSPI_Handler.Init.DualFlash=QSPI_DUALFLASH_DISABLE; //禁止双闪存模式

    if(HAL_QSPI_Init(&QSPI_Handler)==HAL_OK)
        return true;
    else
        return false;
#endif

}

//-----------------------------------------------------------------------------
//功能: QSPI注销
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t QSPI_DeInit(void)
{
    if(HAL_QSPI_DeInit(&QSPI_Handler) == HAL_OK)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------------
//功能: 安装QSPI
//参数: 无
//返回: -1 -- 失败; 0 -- 成功;
//备注:
//-----------------------------------------------------------------------------
s32 ModuleInstall_QSPI(void)
{
    RCC->AHB3ENR|=1<<1;  //使能QSPI时钟
    if(false == QSPI_Init())
        return (-1);
    return (0);
}
