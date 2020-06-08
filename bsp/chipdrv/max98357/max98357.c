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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include "shell.h"
#include "msgqueue.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern ptu32_t ModuleInstall_MAX98357(void);
//    ModuleInstall_MAX98357();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"max98357"//IIS�ӿڵ���Ƶ����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                      //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"int","Message","shell"//������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//weakdependence:"none"                 //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                        //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_MAX98357 == false )
//#warning  " max98357  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_MAX98357    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
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
     uint32_t size_8;                /* ���¸���ַ��ʼ���ļ�β�����ֽ��� */
     uint32_t wave;                  /* = "WAVE" 0x45564157*/
     uint32_t fmt;                   /* = "fmt " 0x20746d66*/
     uint32_t fmtSize;               /* ��һ���ṹ��Ĵ�С(һ��Ϊ16) */
     uint16_t wFormatTag;            /* ���뷽ʽ,һ��Ϊ1 */
     uint16_t wChannels;             /* ͨ������������Ϊ1��������Ϊ2 */
     uint32_t dwSamplesPerSec;       /* ������ */
     uint32_t dwAvgBytesPerSec;      /* ÿ���ֽ���(= ������ �� ÿ���������ֽ���) */
     uint16_t wBlockAlign;           /* ÿ���������ֽ���(=����������/8*ͨ����) */
     uint16_t wBitsPerSample;        /* ����������(ÿ��������Ҫ��bit��) */
     uint32_t data;                  /* = "data" 0x61746164*/
     uint32_t datasize;              /* �����ݳ��� */
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

u32 fillnum=CN_SINGLE_READ_LEN;            //���������������ݴ�С

static struct MsgQueue * gs_ptMax98357MsgQ;//�жϴ������з��͵���Ϣ
#define CN_WAVE_READ_STATCK_LEN     2048
static uint8_t gs_WaveReadStack[CN_WAVE_READ_STATCK_LEN];
static bool_t b_ReadCompleteFlag=true;    //������ɱ�־λ
static bool_t b_DMACloseFlag=true;    //������ɱ�־λ
static bool_t b_FileCloseFlag=true;    //������ɱ�־λ



/*******************************************************************************
����:I2S��DMA����ʹ��
����:
����ֵ:
˵����
*********************************************************************************/
void I2S_DMA_Enable(void)
{
    u32 tempreg=0;
    tempreg=MAX98357_SPIX->CR2;
    tempreg|=1<<1;
    MAX98357_SPIX->CR2=tempreg;
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
    I2SHandle.Instance = MAX98357_SPIX;
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
     //ͨ��0 MAX98357_SPIX_TXͨ��
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
      DMA_IntEnable(MAX98357_DMAX_StreamX,DMA_INT_TCIE);    //ʹ�ܴ�������ж�
      Int_Register(MAX98357_CN_INT_LINE_DMAX_StreamX);
      Int_IsrConnect(MAX98357_CN_INT_LINE_DMAX_StreamX,(uint32_t(*)(ptu32_t))HAL_DMA_IRQHandler);
      Int_SetIsrPara(MAX98357_CN_INT_LINE_DMAX_StreamX,&DmaHandle);
      Int_SettoAsynSignal(MAX98357_CN_INT_LINE_DMAX_StreamX);
      Int_ClearLine(MAX98357_CN_INT_LINE_DMAX_StreamX);
      Int_RestoreAsynLine(MAX98357_CN_INT_LINE_DMAX_StreamX);
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
    uint8_t dat;
    dat=2;
    MsgQ_Send(gs_ptMax98357MsgQ,&dat,1,0,CN_MSGQ_PRIO_NORMAL);
//  b_ReadCompleteFlag=true;
//  __HAL_DMA_DISABLE(&DmaHandle);
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
// ���ܣ���Ƶ�����������
// ����:�벥�ŵ��ļ����硰00.wav��
// ����:
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
    printf("��ǰ�����ļ�->%s.\r\n",filename);
    FileNameLen=strlen(filename);
    Path=malloc(FileNameLen);
    if(Path==NULL)
        return false;
    strcpy(Path,filename);
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
            if(dat==2)//�˴������ļ���ֻ����ɱ�־������һ�ֵ�dat==0��1ʱ�����ǹأ�Ȼ�����ж����ж���ɱ�־���ļ���־����ͣDMA
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
����:CAN����������shellģ��
����:��.
����ֵ:1��
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
����:CAN����������shellģ��
����:��.
����ֵ:1��
*********************************************************************************/
//static bool_t Sh_AudioStop(char *param)
bool_t audios(char *param)
{
    I2S_PlayStop();
    return true;
}


#if 0
/*******************************************************************************
����:�����õĳ���
����:��.
����ֵ:1��
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
ADD_TO_ROUTINE_SHELL(audiop,audiop,"������Ƶ�ļ�");
ADD_TO_ROUTINE_SHELL(audios,audios,"ֹͣ������Ƶ�ļ�");
