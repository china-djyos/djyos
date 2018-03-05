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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��max98357.c
// ģ������: ����DAC max98357ģ������
// ģ��汾: V1.10
// ������Ա: zhb
// ����ʱ��: 10/27.2017
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

u32 fillnum=CN_SINGLE_READ_LEN;            //���������������ݴ�С

static struct MsgQueue * gs_ptMax98357MsgQ;//�жϴ������з��͵���Ϣ
#define CN_WAVE_READ_STATCK_LEN     2048
static uint8_t gs_WaveReadStack[CN_WAVE_READ_STATCK_LEN];
static bool_t b_ReadCompleteFlag=false;    //������ɱ�־λ

/*******************************************************************************
����:I2S��DMA����ʹ��
����:
����ֵ:
˵����
*********************************************************************************/
void I2S_DMA_Enable(void)
{
    u32 tempreg=0;
    tempreg=SPI3->CR2;
	tempreg|=1<<1;
	SPI3->CR2=tempreg;
}
/*******************************************************************************
����:I2S��ʼ������
����:
����ֵ:
˵����
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
//    I2S_GPIO_Init();    //��board.c���ѳ�ʼ��
    __HAL_I2S_DISABLE(&I2SHandle);
    if(HAL_I2S_Init(&I2SHandle) != HAL_OK)
    {
    	printf("I2S3 init failed.\r\n");
    }
    I2S_DMA_Enable();
    __HAL_I2S_ENABLE(&I2SHandle);
}
/*******************************************************************************
����:DMA��ʼ�����ü��ж�ʹ��
����:
����ֵ:
˵����
*********************************************************************************/
void I2Sx_TX_DMA_Init(void)
{
	  __HAL_RCC_DMA1_CLK_ENABLE();
	  __HAL_LINKDMA(&I2SHandle,hdmatx,DmaHandle);
	 /*##-2- Select the DMA functional Parameters ###############################*/
	 /* ���� DMA Stream */
	 //ͨ��0 SPIx_TXͨ��
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
      DMA_IntEnable(DMA1_Stream5,DMA_INT_TCIE);    //ʹ�ܴ�������ж�
	  Int_Register(CN_INT_LINE_DMA1_Stream5);
	  Int_IsrConnect(CN_INT_LINE_DMA1_Stream5,(uint32_t(*)(ptu32_t))HAL_DMA_IRQHandler);
	  Int_SetIsrPara(CN_INT_LINE_DMA1_Stream5,&DmaHandle);
	  Int_SettoAsynSignal(CN_INT_LINE_DMA1_Stream5);
	  Int_ClearLine(CN_INT_LINE_DMA1_Stream5);
	  Int_RestoreAsynLine(CN_INT_LINE_DMA1_Stream5);
 }


/*******************************************************************************
����:��仺����
����:
����ֵ:
˵����
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
// ���ܣ�DMAʹ��
// ����:
// ����:
// =============================================================================
void I2S_PlayStart(void)
{
	__HAL_DMA_ENABLE(&DmaHandle);
}
// =============================================================================
// ���ܣ�DMAֹͣ
// ����:
// ����:
// =============================================================================
void I2S_PlayStop(void)
{
	__HAL_DMA_DISABLE(&DmaHandle);
}

// =============================================================================
// ���ܣ���������жϴ������жϴ�����
// ����:
// ����:
// ˵����1��buf1�ģ�2��buf2�ģ����жϱ�־λ���ж��ߣ�������Ϣ����仺����
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
// ���ܣ���Ƶ�����������
// ����:�벥�ŵ��ļ����硰00.wav��
// ����:
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
	printf("��ǰ�����ļ�->%s.\r\n",filename);
	FileNameLen=strlen(filename);
	Path=malloc(5+FileNameLen);
	if(Path==NULL)
		return false;
	strcpy(Path,"/efs/");
	strcpy(Path+5,filename);
	Fp=fopen(Path,"r");
	if(Fp==NULL)
	{
		printf("��Ƶ�ļ�%s��ʧ��.\r\n",filename);
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
	//��ȡWAVE�ļ�ͷ
	WaveHeadLen=sizeof(gs_wavhead);
	Ret=fread(&gs_wavhead,WaveHeadLen,1,Fp);
	if(Ret!=WaveHeadLen)
	{
		printf("��Ƶ�ļ�%sͷ��Ϣ��ȡʧ��.\r\n",filename);
		return false;
	}
	gs_WaveFileLen-=WaveHeadLen;

	memset(gs_buf1,0,CN_I2S_DMA_BUF_LEN);
	memset(gs_buf2,0,CN_I2S_DMA_BUF_LEN);

	Ret=fread(gs_srcbuf,CN_SINGLE_READ_LEN,1,Fp);
	if(Ret!=CN_SINGLE_READ_LEN)
	{
		printf("��Ƶ�ļ�%s��Ϣ��ȡʧ��.\r\n",filename);
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
		printf("��Ƶ�ļ�%s��Ϣ��ȡʧ��.\r\n",filename);
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
// ���ܣ���һ���߳�������仺���������մ��жϴ������з�������Ϣ
// ����:
// ����:
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
// ���ܣ���ʼ������
// ����:
// ����:
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
	{"audiop",(bool_t(*)(char*))Sh_AudioStart,    "������Ƶ�ļ�",          NULL},
	{"audios",(bool_t(*)(char*))Sh_AudioStop,    "ֹͣ������Ƶ�ļ�",         NULL},
};
//**************************************************************************
#define CN_MAX98357_SHELL_NUM  sizeof(shell_cmd_max98357_table)/sizeof(struct ShellCmdTab)
static struct ShellCmdRsc tg_max98357_shell_cmd_rsc[CN_MAX98357_SHELL_NUM];

/*******************************************************************************
����:CAN����������shellģ��
����:��.
����ֵ:1��
*********************************************************************************/
ptu32_t MAX98357_Shell_Module_Install(void)
{
	Sh_InstallCmd(shell_cmd_max98357_table,tg_max98357_shell_cmd_rsc,CN_MAX98357_SHELL_NUM);
	return 1;
}


/*******************************************************************************
����:CAN����������shellģ��
����:��.
����ֵ:1��
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
����:CAN����������shellģ��
����:��.
����ֵ:1��
*********************************************************************************/
static bool_t Sh_AudioStop(char *param)
{
	I2S_PlayStop();
	return true;
}



/*******************************************************************************
����:�����õĳ���
����:��.
����ֵ:1��
*********************************************************************************/
//void text()
//{
//	ModuleInstall_MAX98357();
//	Audio_StartPlay("04.wav");
//}



















