//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
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
// =============================================================================

// 文件名     ：max98357.c
// 模块描述: 语音DAC max98357模块驱动
// 模块版本: V1.10
// 创建人员: zhb
// 创建时间: 10/27.2017
// =============================================================================
#include "stdint.h"
#include <string.h>
#include <stdlib.h>
#include "cpu_peri.h"
#include "os.h"
#include "board-config.h"
#include <stm32f7xx_hal_dma.h>
#include <stm32f7xx_hal_dma_ex.h>
#include <stm32f7xx_hal.h>
#include <stm32f7xx_hal_i2s.h>
#include "shell.h"
#include "msgqueue.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_MAX98357(void);
//    ModuleInstall_MAX98357();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"max98357"//IIS接口的音频功放
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                         //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                      //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                        //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                      //初始化时机，可选值：early，medium，later, pre-main。
                                        //表示初始化时间，分别是早期、中期、后期
//dependence:"int","Message","shell"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件将强制选中，
                                        //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"                 //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件不会被强制选中，
                                        //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                        //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_MAX98357 == false )
//#warning  " max98357  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_MAX98357    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


#define CN_WAVE_FORMAT_PCM     1
#define CN_WAVE_FORMAT_ADPCM   2

#define CN_CHANNEL_SINGLE      1
#define CN_CHANNEL_DOUBLE      2

#define CN_RIFF_FLAG          0x52494646

#pragma pack(1)
typedef  struct
{
     uint32_t riff;                  /* = "RIFF" 0x46464952*/
     uint32_t size_8;                /* 从下个地址开始到文件尾的总字节数 */
     uint32_t wave;                  /* = "WAVE" 0x45564157*/
     uint32_t fmt;                   /* = "fmt " 0x20746d66*/
     uint32_t fmtSize;               /* 下一个结构体的大小(一般为16) */
     uint16_t wFormatTag;            /* 编码方式,一般为1 */
     uint16_t wChannels;             /* 通道数，单声道为1，立体声为2 */
     uint32_t dwSamplesPerSec;       /* 采样率 */
     uint32_t dwAvgBytesPerSec;      /* 每秒字节数(= 采样率 × 每个采样点字节数) */
     uint16_t wBlockAlign;           /* 每个采样点字节数(=量化比特数/8*通道数) */
     uint16_t wBitsPerSample;        /* 量化比特数(每个采样需要的bit数) */
     uint32_t data;                  /* = "data" 0x61746164*/
     uint32_t datasize;              /* 纯数据长度 */
 } WavHead;




#define CN_I2S_DMA_BUF_LEN             4096
#define CN_SINGLE_READ_LEN             2048
 /* DMA Handle declaration */
static DMA_HandleTypeDef     DmaHandle;
static I2S_HandleTypeDef  I2SHandle;
static WavHead gs_wavhead;
static FILE *Fp;
volatile static uint32_t gs_WaveFileLen=0;
static uint8_t gs_buf1[CN_I2S_DMA_BUF_LEN];
static uint8_t gs_buf2[CN_I2S_DMA_BUF_LEN];
static uint8_t gs_srcbuf[CN_SINGLE_READ_LEN];

void I2S_DMA_TX_CallBack2();
void I2S_DMA_TX_CallBack1();

ptu32_t MAX98357_Shell_Module_Install(void);
static bool_t audiop(char *param);
static bool_t audios(char *param);

u32 fillnum=CN_SINGLE_READ_LEN;            //填充进缓冲区的数据大小

static struct MsgQueue * gs_ptMax98357MsgQ;//中断处理函数中发送的消息
#define CN_WAVE_READ_STATCK_LEN     2048
static uint8_t gs_WaveReadStack[CN_WAVE_READ_STATCK_LEN];
static bool_t b_ReadCompleteFlag=true;    //传输完成标志位
static bool_t b_DMACloseFlag=true;    //传输完成标志位
static bool_t b_FileCloseFlag=true;    //传输完成标志位



/*******************************************************************************
功能:I2S的DMA功能使能
参数:
返回值:
说明：
*********************************************************************************/
void I2S_DMA_Enable(void)
{
    u32 tempreg=0;
    tempreg=MAX98357_SPIX->CR2;
    tempreg|=1<<1;
    MAX98357_SPIX->CR2=tempreg;
}



/*******************************************************************************
功能:I2S初始化配置
参数:
返回值:
说明：
*********************************************************************************/
void I2S_ModeConfig(void)
{
    HAL_I2S_DeInit(&I2SHandle);
    I2SHandle.Instance = MAX98357_SPIX;
    I2SHandle.Init.Mode = I2S_MODE_MASTER_TX;
    I2SHandle.Init.Standard = I2S_STANDARD_PHILIPS;
    I2SHandle.Init.DataFormat = I2S_DATAFORMAT_16B;
    I2SHandle.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
    I2SHandle.Init.AudioFreq = I2S_AUDIOFREQ_16K;
    I2SHandle.Init.CPOL = I2S_CPOL_HIGH;
    I2SHandle.Init.ClockSource = I2S_CLOCK_PLL;
//    I2S_GPIO_Init();    //在board.c中已初始化
    __HAL_I2S_DISABLE(&I2SHandle);
    if(HAL_I2S_Init(&I2SHandle) != HAL_OK)
    {
        printf("I2S3 init failed.\r\n");
    }
    I2S_DMA_Enable();
    __HAL_I2S_ENABLE(&I2SHandle);
}
/*******************************************************************************
功能:DMA初始化配置及中断使能
参数:
返回值:
说明：
*********************************************************************************/
void I2Sx_TX_DMA_Init(void)
{
      __HAL_RCC_DMA1_CLK_ENABLE();
      __HAL_LINKDMA(&I2SHandle,hdmatx,DmaHandle);
     /*##-2- Select the DMA functional Parameters ###############################*/
     /* 配置 DMA Stream */
     //通道0 MAX98357_SPIX_TX通道
      DmaHandle.Init.Channel = DMA_CHANNEL_0;                  /* DMA_CHANNEL_0                    */
      DmaHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;         /* M2M transfer mode                */
      DmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;             /* Peripheral increment mode Disable */
      DmaHandle.Init.MemInc = DMA_MINC_ENABLE;                 /* Memory increment mode Enable     */
      DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /* Peripheral data alignment : 16bit */
      DmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    /* memory data alignment : 16bit     */
      DmaHandle.Init.Mode = DMA_CIRCULAR;                         /* Normal DMA mode                  */
      DmaHandle.Init.Priority = DMA_PRIORITY_VERY_HIGH;              /* priority level : high            */
      DmaHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;           /* FIFO mode disabled               */
      DmaHandle.Init.MemBurst = DMA_MBURST_SINGLE;              /* Memory burst                     */
      DmaHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;           /* Peripheral burst                 */

      /*##-3- Select the DMA instance to be used for the transfer : DMAX_StreamX #*/
      DmaHandle.Instance = MAX98357_DMAX_StreamX;

      HAL_DMA_DeInit(&DmaHandle);
      HAL_DMA_Init(&DmaHandle);

      HAL_DMA_RegisterCallback(&DmaHandle,HAL_DMA_XFER_M1CPLT_CB_ID,I2S_DMA_TX_CallBack2);
      HAL_DMA_RegisterCallback(&DmaHandle,HAL_DMA_XFER_CPLT_CB_ID,I2S_DMA_TX_CallBack1);

      HAL_DMAEx_MultiBufferStart(&DmaHandle,(u32)gs_buf1,(u32)&(MAX98357_SPIX->DR),(u32)gs_buf2,CN_SINGLE_READ_LEN);
      __HAL_DMA_DISABLE(&DmaHandle);
      DJY_DelayUs(100);

      extern void DMA_IntEnable(DMA_Stream_TypeDef *DMA_Streamx,u8 SrcInt);
      DMA_IntEnable(MAX98357_DMAX_StreamX,DMA_INT_TCIE);    //使能传输完成中断
      Int_Register(MAX98357_CN_INT_LINE_DMAX_StreamX);
      Int_IsrConnect(MAX98357_CN_INT_LINE_DMAX_StreamX,(uint32_t(*)(ptu32_t))HAL_DMA_IRQHandler);
      Int_SetIsrPara(MAX98357_CN_INT_LINE_DMAX_StreamX,&DmaHandle);
      Int_SettoAsynSignal(MAX98357_CN_INT_LINE_DMAX_StreamX);
      Int_ClearLine(MAX98357_CN_INT_LINE_DMAX_StreamX);
      Int_RestoreAsynLine(MAX98357_CN_INT_LINE_DMAX_StreamX);
 }



/*******************************************************************************
功能:填充缓冲区
参数:
返回值:
说明：
*********************************************************************************/
u32 wav_buffill(u8 *buf,u16 size,u8 bits)
{
    u32 bread;
    u16 i;

    bread=fread(gs_srcbuf,size,1,Fp);
    if(bread<size)
    {
        memset(gs_srcbuf+bread,0,size-bread);
    }
    for(i=0;i<CN_SINGLE_READ_LEN;i++)
    {
        buf[2*i+1]=gs_srcbuf[i];
    }
    gs_WaveFileLen-=size;

    return bread;
}



// =============================================================================
// 功能：DMA使能
// 参数:
// 返回:
// =============================================================================
void I2S_PlayStart(void)
{
    __HAL_DMA_ENABLE(&DmaHandle);
}
// =============================================================================
// 功能：DMA停止
// 参数:
// 返回:
// =============================================================================
void I2S_PlayStop(void)
{
    uint8_t dat;
    dat=2;
    MsgQ_Send(gs_ptMax98357MsgQ,&dat,1,0,CN_MSGQ_PRIO_NORMAL);
//  b_ReadCompleteFlag=true;
//  __HAL_DMA_DISABLE(&DmaHandle);
}

// =============================================================================
// 功能：传输完成中断触发的中断处理函数
// 参数:
// 返回:
// 说明：1是buf1的，2是buf2的，清中断标志位，中断线，发送消息来填充缓冲区
// =============================================================================
void I2S_DMA_TX_CallBack1()
{
    uint8_t dat;

    if(b_DMACloseFlag==false)
    {
        DMA_ClearIntFlag(MAX98357_DMAX_StreamX);
        Int_ClearLine(MAX98357_CN_INT_LINE_DMAX_StreamX);
        if(b_ReadCompleteFlag && b_FileCloseFlag)
        {
            b_DMACloseFlag=true;
            DmaHandle.Instance->CR  &= 0xfff7ffff;
            memset(gs_buf1,0,CN_I2S_DMA_BUF_LEN);
            memset(gs_buf2,0,CN_I2S_DMA_BUF_LEN);
        }
        else
        {
            dat=0;
            MsgQ_Send(gs_ptMax98357MsgQ,&dat,1,0,CN_MSGQ_PRIO_NORMAL);
        }
    }
}

void I2S_DMA_TX_CallBack2()
{
    uint8_t dat;

    if(b_DMACloseFlag==false)
    {
        DMA_ClearIntFlag(MAX98357_DMAX_StreamX);
        Int_ClearLine(MAX98357_CN_INT_LINE_DMAX_StreamX);
        if(b_ReadCompleteFlag && b_FileCloseFlag)
        {
            b_DMACloseFlag=true;
            DmaHandle.Instance->CR  &= 0xfff7ffff;
            memset(gs_buf2,0,CN_I2S_DMA_BUF_LEN);
            memset(gs_buf1,0,CN_I2S_DMA_BUF_LEN);
        }
        else
        {
            dat=1;
            MsgQ_Send(gs_ptMax98357MsgQ,&dat,1,0,CN_MSGQ_PRIO_NORMAL);
        }
    }
}
// =============================================================================
// 功能：音频输出的主函数
// 参数:想播放的文件，如“00.wav”
// 返回:
// =============================================================================
bool_t Audio_StartPlay(const char *filename)
{
    char *Path;
    u8 FileNameLen,WaveHeadLen;
    s32 Ret;
    u8 num=0;

    if(b_FileCloseFlag!=true)
    {
        I2S_PlayStop();
    }
    while((b_FileCloseFlag!=true) || (b_ReadCompleteFlag!=true)||(b_DMACloseFlag!=true))
    {
        num++;
        DJY_EventDelay( 1 * mS );
    }

    DJY_EventDelay( 1 * mS );

    struct stat FileInfo = {0};
    if(filename==NULL)
        return false;
    printf("当前播放文件->%s.\r\n",filename);
    FileNameLen=strlen(filename);
    Path=malloc(FileNameLen);
    if(Path==NULL)
        return false;
    strcpy(Path,filename);
    Fp=fopen(Path,"r");
    if(Fp==NULL)
    {
        printf("音频文件%s打开失败.\r\n",filename);
        fclose(Fp);
        return false;
    }
    Ret=stat(Path,&FileInfo);
    if(Ret)
    {
        printf("%s cannot stat.\r\n",Path);
        return false;
    }
    gs_WaveFileLen=FileInfo.st_size;
    //读取WAVE文件头
    WaveHeadLen=sizeof(gs_wavhead);
    Ret=fread(&gs_wavhead,WaveHeadLen,1,Fp);
    if(Ret!=WaveHeadLen)
    {
        printf("音频文件%s头信息读取失败.\r\n",filename);
        return false;
    }
    gs_WaveFileLen-=WaveHeadLen;

    memset(gs_buf1,0,CN_I2S_DMA_BUF_LEN);
    memset(gs_buf2,0,CN_I2S_DMA_BUF_LEN);

    b_ReadCompleteFlag=false;
    b_DMACloseFlag=false;
    b_FileCloseFlag=false;

    DMA_ClearIntFlag(MAX98357_DMAX_StreamX);
    Int_ClearLine(MAX98357_CN_INT_LINE_DMAX_StreamX);
    DmaHandle.Instance->FCR &=0x7f;
    fillnum=CN_SINGLE_READ_LEN;
    I2S_PlayStart();
    return true;
}


// =============================================================================
// 功能：另开一个线程用来填充缓冲区，接收从中断处理函数中发来的消息
// 参数:
// 返回:
// =============================================================================
ptu32_t __WaveReadHandle(void)
{
    uint8_t dat;
    while(1)
    {
        MsgQ_Receive(gs_ptMax98357MsgQ, &dat, 1, CN_TIMEOUT_FOREVER);
        {
            if(dat==2)//此处不关文件，只改完成标志，再跑一轮到dat==0，1时让他们关，然后在中断里判断完成标志且文件标志，再停DMA
            {
                b_ReadCompleteFlag=true;
//              fclose(Fp);
//              b_FileCloseFlag=true;
            }
            if(dat==0)
            {
                memset(gs_buf1,0,CN_I2S_DMA_BUF_LEN);
                fillnum=wav_buffill(gs_buf1,CN_SINGLE_READ_LEN,16);
                if(fillnum<CN_SINGLE_READ_LEN && Fp!=NULL)
                {
                    b_ReadCompleteFlag=true;
                }
                if(b_ReadCompleteFlag)
                {
                    fclose(Fp);
                    b_FileCloseFlag=true;
                }
            }
            if(dat==1)
            {
                memset(gs_buf2,0,CN_I2S_DMA_BUF_LEN);
                fillnum=wav_buffill(gs_buf2,CN_SINGLE_READ_LEN,16);
                if(fillnum<CN_SINGLE_READ_LEN && Fp!=NULL)
                {
                    b_ReadCompleteFlag=true;
                }
                if(b_ReadCompleteFlag)
                {
                    fclose(Fp);
                    b_FileCloseFlag=true;
                }
            }
        }
    }
}
// =============================================================================
// 功能：初始化函数
// 参数:
// 返回:
// =============================================================================
ptu32_t ModuleInstall_MAX98357(void)
{
    uint16_t evtt_id,event_id;
    I2S_ModeConfig();
    I2Sx_TX_DMA_Init();
    MAX98357_Shell_Module_Install();
    gs_ptMax98357MsgQ=MsgQ_Create(1,1,CN_MSGQ_TYPE_FIFO);
    if(gs_ptMax98357MsgQ==NULL)
        return false;
    evtt_id = DJY_EvttRegist(EN_CORRELATIVE,10,0,0,__WaveReadHandle,
            gs_WaveReadStack,sizeof(gs_WaveReadStack),"wave read function");
    if(evtt_id!=CN_EVTT_ID_INVALID)
    {
       event_id=DJY_EventPop(evtt_id,NULL,0,NULL,0,0);
    }
    else
    {
        DJY_EvttUnregist(evtt_id);
        printf("wave read evtt pop failed.\r\n");
    }
    return 1;
}

/*******************************************************************************
功能:CAN控制器操作shell模块
参数:无.
返回值:1。
*********************************************************************************/
//static bool_t Sh_AudioStart(char *param)
bool_t audiop(char *param)
{
    bool_t ret;
    if(param==NULL)
    {
        printf("%s para invaild.\r\n",__FUNCTION__);
        return false;
    }
    ret=Audio_StartPlay(param);
    return ret;
}

/*******************************************************************************
功能:CAN控制器操作shell模块
参数:无.
返回值:1。
*********************************************************************************/
//static bool_t Sh_AudioStop(char *param)
bool_t audios(char *param)
{
    I2S_PlayStop();
    return true;
}


#if 0
/*******************************************************************************
功能:测试用的程序
参数:无.
返回值:1。
*********************************************************************************/
//void max98357_text()
//{
//}


//board_config.h********************************

//#define MAX98357_DMAX_StreamX               DMA1_Stream4
//#define MAX98357_SPIX                       SPI2
//#define MAX98357_CN_INT_LINE_DMAX_StreamX   CN_INT_LINE_DMA1_Stream4

//board.c***************************************

//static const Pin AUDIO_I2S2_pin[]={
//  {GPIO_I,PIN0,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_H,GPIO_PUPD_PU,AF5}, //I2S3 FSYNC
//  {GPIO_I,PIN1,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},  //I2S3 CLK
//  {GPIO_I,PIN3,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},  //I2S3 SD
//  {GPIO_H,PIN12,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll}, //SD_MODE
//};
//
//
//
//void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s)
//{
//   RCC_PeriphCLKInitTypeDef RCC_ExCLKInitStruct;
//     GPIO_InitTypeDef  GPIO_InitStruct;
//     RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
//     if(hi2s->Instance==SPI2)
//     {
//       __HAL_RCC_SPI2_CLK_ENABLE();
//       PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
//     PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
//     PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLP_DIV2;
//     PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
//     PeriphClkInitStruct.PLLI2S.PLLI2SQ = 2;
//     PeriphClkInitStruct.PLLI2SDivQ = 1;
//     PeriphClkInitStruct.I2sClockSelection = RCC_I2SCLKSOURCE_PLLI2S;
//     if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
//     {
//         printf("I2S CLK init failed.\r\n");
//     }
//
//     }
//}
//
//void HAL_I2S_MspDeInit(I2S_HandleTypeDef* hi2s)
//{
//
//  if(hi2s->Instance==SPI2)
//  {
//    /* Peripheral clock disable */
//    __HAL_RCC_SPI2_CLK_DISABLE();
//    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_0);
//    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_1);
//    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_3);
//    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_12);
//  }
//}
#endif
ADD_TO_ROUTINE_SHELL(audiop,audiop,"播放音频文件");
ADD_TO_ROUTINE_SHELL(audios,audios,"停止播放音频文件");
