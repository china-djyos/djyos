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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
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
#include <wave.h>
#include "shell.h"
#include "msgqueue.h"

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
static bool_t Sh_AudioStart(char *param);
static bool_t Sh_AudioStop(char *param);

u32 fillnum=CN_SINGLE_READ_LEN;            //填充进缓冲区的数据大小

static struct MsgQueue * gs_ptMax98357MsgQ;//中断处理函数中发送的消息
#define CN_WAVE_READ_STATCK_LEN     2048
static uint8_t gs_WaveReadStack[CN_WAVE_READ_STATCK_LEN];
static bool_t b_ReadCompleteFlag=false;    //传输完成标志位

/*******************************************************************************
功能:I2S的DMA功能使能
参数:
返回值:
说明：
*********************************************************************************/
void I2S_DMA_Enable(void)
{
    u32 tempreg=0;
    tempreg=SPI3->CR2;
	tempreg|=1<<1;
	SPI3->CR2=tempreg;
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
	I2SHandle.Instance = SPI3;
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
	 //通道0 SPIx_TX通道
	  DmaHandle.Init.Channel = DMA_CHANNEL_0;                  /* DMA_CHANNEL_0                    */
	  DmaHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;         /* M2M transfer mode                */
	  DmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;             /* Peripheral increment mode Disable */
	  DmaHandle.Init.MemInc = DMA_MINC_ENABLE;                 /* Memory increment mode Enable     */
	  DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /* Peripheral data alignment : 16bit */
	  DmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    /* memory data alignment : 16bit     */
	  DmaHandle.Init.Mode = DMA_CIRCULAR;                         /* Normal DMA mode                  */
	  DmaHandle.Init.Priority = DMA_PRIORITY_HIGH;              /* priority level : high            */
	  DmaHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;           /* FIFO mode disabled               */
	  DmaHandle.Init.MemBurst = DMA_MBURST_SINGLE;              /* Memory burst                     */
	  DmaHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;           /* Peripheral burst                 */

	  /*##-3- Select the DMA instance to be used for the transfer : DMA1_Stream5 #*/
	  DmaHandle.Instance = DMA1_Stream5;

	  HAL_DMA_DeInit(&DmaHandle);
	  HAL_DMA_Init(&DmaHandle);

	  HAL_DMA_RegisterCallback(&DmaHandle,HAL_DMA_XFER_M1CPLT_CB_ID,I2S_DMA_TX_CallBack2);
	  HAL_DMA_RegisterCallback(&DmaHandle,HAL_DMA_XFER_CPLT_CB_ID,I2S_DMA_TX_CallBack1);

	  HAL_DMAEx_MultiBufferStart(&DmaHandle,(u32)gs_buf1,(u32)&(SPI3->DR),(u32)gs_buf2,CN_SINGLE_READ_LEN);
	  __HAL_DMA_DISABLE(&DmaHandle);
	  Djy_DelayUs(100);

      extern void DMA_IntEnable(DMA_Stream_TypeDef *DMA_Streamx,u8 SrcInt);
      DMA_IntEnable(DMA1_Stream5,DMA_INT_TCIE);    //使能传输完成中断
	  Int_Register(CN_INT_LINE_DMA1_Stream5);
	  Int_IsrConnect(CN_INT_LINE_DMA1_Stream5,(uint32_t(*)(ptu32_t))HAL_DMA_IRQHandler);
	  Int_SetIsrPara(CN_INT_LINE_DMA1_Stream5,&DmaHandle);
	  Int_SettoAsynSignal(CN_INT_LINE_DMA1_Stream5);
	  Int_ClearLine(CN_INT_LINE_DMA1_Stream5);
	  Int_RestoreAsynLine(CN_INT_LINE_DMA1_Stream5);
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
		for(i=bread;i<size-bread;i++)gs_srcbuf[i]=0;
	}
	for(i=0;i<CN_SINGLE_READ_LEN;i++)
	{
		buf[2*i]=0;
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
	__HAL_DMA_DISABLE(&DmaHandle);
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
	DMA1->HIFCR |= 1<<9;
	DMA1->HIFCR |= 1<<6;
	DMA1->HIFCR |= 1<<11;
	DMA_ClearIntFlag(DMA1_Stream5);
	Int_ClearLine(CN_INT_LINE_DMA1_Stream5);
	if(b_ReadCompleteFlag)
	{
		__HAL_DMA_DISABLE(&DmaHandle);
	}
	else
	{
		dat=0;
		MsgQ_Send(gs_ptMax98357MsgQ,&dat,1,0,CN_MSGQ_PRIO_NORMAL);
	}
}
void I2S_DMA_TX_CallBack2()
{
	uint8_t dat;

	DMA1->HIFCR |= 1<<9;
	DMA1->HIFCR |= 1<<6;
	DMA1->HIFCR |= 1<<11;
	DMA_ClearIntFlag(DMA1_Stream5);
	Int_ClearLine(CN_INT_LINE_DMA1_Stream5);
	if(b_ReadCompleteFlag)
	{
		__HAL_DMA_DISABLE(&DmaHandle);
	}
	else
	{
		dat=1;
		MsgQ_Send(gs_ptMax98357MsgQ,&dat,1,0,CN_MSGQ_PRIO_NORMAL);
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
	uint32_t i;
	b_ReadCompleteFlag=false;

	struct stat FileInfo = {0};
	if(filename==NULL)
		return false;
	printf("当前播放文件->%s.\r\n",filename);
	FileNameLen=strlen(filename);
	Path=malloc(5+FileNameLen);
	if(Path==NULL)
		return false;
	strcpy(Path,"/efs/");
	strcpy(Path+5,filename);
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

	Ret=fread(gs_srcbuf,CN_SINGLE_READ_LEN,1,Fp);
	if(Ret!=CN_SINGLE_READ_LEN)
	{
		printf("音频文件%s信息读取失败.\r\n",filename);
		return false;
	}
	for(i=0;i<CN_SINGLE_READ_LEN;i++)
	{
		gs_buf1[2*i]=0;
		gs_buf1[2*i+1]=gs_srcbuf[i];
	}
	gs_WaveFileLen-=CN_SINGLE_READ_LEN;
	Ret=fread(gs_srcbuf,CN_SINGLE_READ_LEN,1,Fp);
	if(Ret!=CN_SINGLE_READ_LEN)
	{
		printf("音频文件%s信息读取失败.\r\n",filename);
		return false;
	}
	for(i=0;i<CN_SINGLE_READ_LEN;i++)
	{
		gs_buf2[2*i]=0;
		gs_buf2[2*i+1]=gs_srcbuf[i];
	}
	gs_WaveFileLen-=CN_SINGLE_READ_LEN;
	DMA1->HIFCR |= 1<<10;
	DMA1->HIFCR |= 1<<11;
	DmaHandle.Instance->FCR |=0<<7;
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
			if(dat==0)
			{
				memset(gs_buf1,0,CN_I2S_DMA_BUF_LEN);
				fillnum=wav_buffill(gs_buf1,CN_SINGLE_READ_LEN,16);
				if(fillnum<CN_SINGLE_READ_LEN && Fp!=NULL)
				{
					b_ReadCompleteFlag=true;

				}
			}
			else
			{
				memset(gs_buf2,0,CN_I2S_DMA_BUF_LEN);
				fillnum=wav_buffill(gs_buf2,CN_SINGLE_READ_LEN,16);
				if(fillnum<CN_SINGLE_READ_LEN && Fp!=NULL)
				{
					b_ReadCompleteFlag=true;
				}
			}
			if(b_ReadCompleteFlag)
			{
				fclose(Fp);
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
    evtt_id = Djy_EvttRegist(EN_CORRELATIVE,100,0,0,__WaveReadHandle,
    		gs_WaveReadStack,sizeof(gs_WaveReadStack),"wave read function");
	if(evtt_id!=CN_EVTT_ID_INVALID)
	{
	   event_id=Djy_EventPop(evtt_id,NULL,0,NULL,0,0);
	}
	else
	{
		Djy_EvttUnregist(evtt_id);
		printf("wave read evtt pop failed.\r\n");
	}
	return 1;
}

//**************************************************************************
struct ShellCmdTab const shell_cmd_max98357_table[]=
{
	{"audiop",(bool_t(*)(char*))Sh_AudioStart,    "播放音频文件",          NULL},
	{"audios",(bool_t(*)(char*))Sh_AudioStop,    "停止播放音频文件",         NULL},
};
//**************************************************************************
#define CN_MAX98357_SHELL_NUM  sizeof(shell_cmd_max98357_table)/sizeof(struct ShellCmdTab)
static struct ShellCmdRsc tg_max98357_shell_cmd_rsc[CN_MAX98357_SHELL_NUM];

/*******************************************************************************
功能:CAN控制器操作shell模块
参数:无.
返回值:1。
*********************************************************************************/
ptu32_t MAX98357_Shell_Module_Install(void)
{
	Sh_InstallCmd(shell_cmd_max98357_table,tg_max98357_shell_cmd_rsc,CN_MAX98357_SHELL_NUM);
	return 1;
}


/*******************************************************************************
功能:CAN控制器操作shell模块
参数:无.
返回值:1。
*********************************************************************************/
static bool_t Sh_AudioStart(char *param)
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
static bool_t Sh_AudioStop(char *param)
{
	I2S_PlayStop();
	return true;
}



/*******************************************************************************
功能:测试用的程序
参数:无.
返回值:1。
*********************************************************************************/
//void text()
//{
//	ModuleInstall_MAX98357();
//	Audio_StartPlay("04.wav");
//}



















