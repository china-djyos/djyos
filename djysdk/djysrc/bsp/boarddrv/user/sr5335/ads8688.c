// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��ADS8668.c
// ģ������: ADS8688�ɼ�ģ��ĵײ�������������SPI�ӿ�ͨ�ź�����ȡAD�ɼ�ֵ
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 05/10.2017
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "endian.h"
#include "djyos.h"
#include "cpu_peri.h"
//#include "Drv_SR5333.h"
#include "shell.h"


extern const Pin ADCS[];
extern const Pin ADRST[];

//Command Register
#define NO_OP              0x0000
#define STDBY              0x8200
#define PWR_DN             0x8300
#define RST                0x8500
#define AUTO_RST           0xA000
#define MAN_Ch_0           0xC000
#define MAN_Ch_1           0xC400
#define MAN_Ch_2           0xC800
#define MAN_Ch_3           0xCC00

#define MAN_Ch_4           0xD000
#define MAN_Ch_5           0xD400
#define MAN_Ch_6           0xD800
#define MAN_Ch_7           0xDC00

#define MAN_AUX            0xE000
//Program Register
#define AUTO_SEQ_EN                  0x01

#define Channel_0_Input_Range        0x05
#define Channel_1_Input_Range        0x06
#define Channel_2_Input_Range        0x07
#define Channel_3_Input_Range        0x08
#define Channel_4_Input_Range        0x09
#define Channel_5_Input_Range        0x0A
#define Channel_6_Input_Range        0x0B
#define Channel_7_Input_Range        0x0C


// =============================================================================
#define ADS8688_CS           (0)
//
#define WRITE                 1
#define READ                  0
#define CN_CP_SAM_RATE_BSP       10


#define CN_CHANNEL_NUM           9
#define CN_DEBUG_SAMP_CNT        20
static u16 AD_Buf[CN_CHANNEL_NUM];
#define CN_SPI_TIMEOUT  (200*1000)

static u16 gs_cp1_buf[CN_CP_SAM_RATE_BSP];
static u16 gs_cp2_buf[CN_CP_SAM_RATE_BSP];
//void Sh_AD_Stat(void);
//void Sh_AD_Val(void);

//���±������ڵ�����֤
//static uint16_t ad_buf[5*CN_DEBUG_SAMP_CNT];
//static uint16_t ad_cpbuf[CN_CP_SAM_RATE_BSP*CN_DEBUG_SAMP_CNT];
//static u8 gs_AdDebugcnt=0;
//static uint16_t gs_AdDebugCpcnt=0;
//static bool_t gs_AdDebugFlag=false;
static u64 gs_adcpcnt=0;    //����ͳ��CPͨ���ɼ��ܴ���
static u64 gs_adcnt=0;      //����ͳ��Ӧ�ò��ȡ5��ADͨ�����ܴ���

//struct ShellCmdTab const shell_cmd_ad_table[]=
//{
//	{
//			"adstat",
//			Sh_AD_Stat,
//			"AD��������ͳ��",
//			"COMMAND:adstat+enter"
//	}
//};

//#define CN_AD_SHELL_NUM  sizeof(shell_cmd_ad_table)/sizeof(struct ShellCmdTab)
//static struct ShellCmdRsc tg_ad_shell_cmd_rsc[CN_AD_SHELL_NUM];

/*******************************************************************************
����:AD��������shellģ��
����:��.
����ֵ:1��
*********************************************************************************/
//ptu32_t AD_Shell_Module_Install(void)
//{
//	Sh_InstallCmd(shell_cmd_ad_table,tg_ad_shell_cmd_rsc,CN_AD_SHELL_NUM);
//	return 1;
//}
//#define CN_AD_PRINT_PREFIX  "   "
/*******************************************************************************
����:��ȡAD����ͳ������,����5��ͨ�������ܴ����Լ�CPͨ���������ܴ���.
����:�ޡ�
���:�ޡ�
*******************************************************************************/
//void Sh_AD_Stat(void)
//{
//   uint32_t data[2];//used to print the s64 type
//   printf("AD Stat:\r\n");
//   memcpy(data,&gs_adcnt,sizeof(data));
//   printf("%sAD Cnt(total)    :0x%08x(H) 0x%08x(L) \r\n",CN_AD_PRINT_PREFIX,data[1],data[0]);
//   memcpy(data,&gs_adcpcnt,sizeof(data));
//   printf("%s AD CP Cnt(total):0x%08x(H) 0x%08x(L) \r\n",CN_AD_PRINT_PREFIX,data[1],data[0]);
//   printf("The End.\r\n");
//
//}

/*******************************************************************************
����:��ȡCAN����ͨ��ͳ������
����:�ޡ�
���:�ޡ�
*******************************************************************************/
//void Sh_AD_Val(void)
//{
//	u8 i,j;
//	uint16_t val;
//	gs_AdDebugFlag=true;
//	Djy_EventDelay(50*mS);
//	printf("AD val:\r\n");
//	for(i=0;i<CN_DEBUG_SAMP_CNT;i++)
//	{
//	   printf("%sAD1:0x%x,AD2:0x%x,AD3:0x%x,AD4:0x%x,AD5:0x%x.\r\n",CN_AD_PRINT_PREFIX,
//		 ad_buf[5*i],ad_buf[5*i+1],ad_buf[5*i+2],ad_buf[5*i+3],ad_buf[5*i+4]);
//	   printf("CP Channel:\r\n");
//	   for(j=0;j<CN_CP_SAM_RATE_BSP;j++)
//	   {
//		  val=ad_cpbuf[CN_DEBUG_SAMP_CNT*i+j];
//		  printf("0x%x,",val);
//	   }
//	   printf("\r\n");
//	}
//	printf("The End.\r\n");
//	gs_AdDebugFlag=false;
//	gs_AdDebugcnt=0;
//	gs_AdDebugCpcnt=0;
//	memset(ad_buf,0,10*CN_DEBUG_SAMP_CNT);
//	memset(ad_cpbuf,0,2*CN_CP_SAM_RATE_BSP*CN_DEBUG_SAMP_CNT);
//}

// =============================================================================
// ���ܣ���SPI���ռĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ
// ���أ�����������
// =============================================================================
static s32 __SPI_Read(void)
{
	u32 time = CN_SPI_TIMEOUT;
	Spi *Reg;
	Reg = (Spi *)SPI2_BASE;
	while ( ( (Reg->SR & SPI_SR_RXNE) == 0 ) && (time > 0))
	{
		time --;
		Djy_DelayUs(1);
	}

	if(time == 0)
		return -1;
	return (s32)(Reg->DR & 0xFFFF);
}

// =============================================================================
// ���ܣ�дSPI���ͼĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ��dwNpcs,Ƭѡ��wData��д����
// ���أ���
// =============================================================================
static s32 __SPI_Write(uint32_t dwNpcs, uint16_t wData )
{
	u32 time = CN_SPI_TIMEOUT;
	Spi *Reg;
	Reg = (Spi *)SPI2_BASE;
	 /* Send data */
	 while ( ( (Reg->SR & SPI_SR_TXE) == 0 ) && (time > 0) )
	 {
	        time --;
	        Djy_DelayUs(1);
	 }
	 if(time == 0)
		return -1;
	    *((volatile uint16_t *)&Reg->DR) = wData;

	 return 0;
}

// =============================================================================
// ���ܣ�ADS8688��Ӧ��SPI��������ʼ��
// ������port, SPI��������ţ���sr5333-v2.0��ADS8688���õ���SPI2;
// ���أ���.
// =============================================================================
static void __ADS8688_SpiInit(u8 port)
{
	//�Ѿ���board.c�����ʼ����SPI2������
	 Spi *Reg;
	 Reg = (Spi *)SPI2_BASE;
	 //����SPIʹ��GPIO����
	 RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // ʹ��SPI2
	 RCC->APB1RSTR |= RCC_APB1RSTR_SPI2RST; // ��λSPI2
	 RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST; // ֹͣ��λSPI2

	 Reg->CR1 |=SPI_CR1_BR_0;//����������  108M/4=27MHz
	 Reg->CR1 &= ~SPI_CR1_CPOL;                      // ����ģʽ��SCKΪ1 CPOL=0
	 Reg->CR1 |= SPI_CR1_CPHA;                      // ���ݲ����ӵ�2��ʱ����ؿ�ʼ,CPHA=1
	 Reg->CR1 &= ~SPI_CR1_RXONLY;//ȫ˫��ͨѶģʽ
	 Reg->CR1 &= ~SPI_CR1_LSBFIRST;//����֡��ʽ
	 Reg->CR1 &= ~ SPI_CR1_SSM;         // ��ֹ���nss����
	 Reg->CR1 |= SPI_CR1_SSI;  //SSM
	 Reg->CR1 |= SPI_CR1_MSTR;       // SPI����

	 Reg->CR2 |=SPI_CR2_DS_0|SPI_CR2_DS_1|SPI_CR2_DS_2|SPI_CR2_DS_3 ;//����֡��ʽΪ16bit
	 Reg->CR2 |=SPI_CR2_SSOE;//SS ���ʹ��
	 Reg->CR2 |=SPI_CR2_NSSP;//���� NSS ����
	 Reg->CR2 &=~SPI_CR2_FRXTH;//16�ֽ�fifo
	 Reg->I2SCFGR &= (u16)(~SPI_I2SCFGR_I2SMOD);// ѡ��SPIģʽ

	 Reg->CR1 |= SPI_CR1_SPE;

}

// =============================================================================
// ���ܣ�ADS8688��Ӧ��SPI��������ʼ��
// ������addr, Program Register��ַ;
//       data������дָ����Ч��Ϊд���мĴ�����ֵ.
// ���أ�
// =============================================================================
static bool_t __ADS8688_SpiTrans(u16 *wBuf, u32 wlen, u16 *rBuf,u32 rlen)
{
    u32 i;
    s32 ret;
    bool_t result = false;
    Spi *Reg;
   	Reg = (Spi *)SPI2_BASE;
//   	Reg->CR1&=~SPI_CR1_CPHA;
    PIO_Clear(ADCS);
	for(i = 0; i < wlen; i++)
	{
    	ret=__SPI_Write(ADS8688_CS,wBuf[i]);
		if(ret==-1)
	      	goto SPI_EXIT;
    	ret=__SPI_Read();
	    if(ret==-1)
	      	goto SPI_EXIT;
	}
//	Reg->CR1|=SPI_CR1_CPHA;
//	Reg->CR1&=~SPI_CR1_CPHA;
	for(i = 0; i < rlen; i ++)
	{
    	ret=__SPI_Write(ADS8688_CS,0x0000);
		if(ret==-1)
	      	goto SPI_EXIT;
    	 ret = __SPI_Read();
		 if(ret==-1)
		    goto SPI_EXIT;

	     rBuf[i] = (u16)(ret & 0xFFFF);
	}
//	Reg->CR1|=SPI_CR1_CPHA;
  	result = true;
SPI_EXIT:
    PIO_Set(ADCS);
	return result;
}

// =============================================================================

static void __ADS8688_WriteCommandReg(u16 command)//дADS8688����Ĵ���
{
	u16 tmp=0;
    __ADS8688_SpiTrans(&command,1,&tmp,1);
}

static void __ADS8688_EnterAutoRstMode(void)    //�����Զ�ɨ��ģʽ
{
    __ADS8688_WriteCommandReg(AUTO_RST);
}

static void __ADS8688_EnterRstMode(void)
{
    __ADS8688_WriteCommandReg(RST);
}

// =============================================================================
// ���ܣ�Program Registerд����
// ������addr, Program Register��ַ;
//       data������дָ����Ч��Ϊд���мĴ�����ֵ.
// ���أ�
// =============================================================================
static bool_t __ADS8688_WriteReadProgramReg(u8 addr,u8 data)
{
	u16 w_dat=0x0000;
	u16 r_dat=0x0000;
	bool_t ret=true;
	u8 temp=0x00;
	temp=addr<<1;
	temp=temp|WRITE;
	w_dat=temp;
	w_dat=w_dat<<8;
	w_dat=w_dat|data;
	ret=__ADS8688_SpiTrans(&w_dat,1,&r_dat,1);
	if(ret==false)
		return ret;
	r_dat=r_dat>>8;
	if(r_dat!=data)
		return false;
	return true;
}

// =============================================================================
// ���ܣ������Զ�ɨ������ͨ��
// ������seq
// ���أ�
// =============================================================================
static bool_t __ADS8688_SetAutoScanSequence(u8 seq)
{
	bool_t result;
	result=__ADS8688_WriteReadProgramReg(AUTO_SEQ_EN,seq);
	return result;
}
// =============================================================================
// ���ܣ�����ͨ�����뷶Χ
// ������seq
// ���أ�
// =============================================================================
static bool_t __ADS8688_SetChannelInputRange(u8 ch,u8 range)
{
	bool_t result;
	result=__ADS8688_WriteReadProgramReg(ch,range);
	return result;
}

// =============================================================================
// ���ܣ�����ָ��ͨ��Ϊ�ֶ�ģʽ
// ������seq
// ���أ�
// =============================================================================
static bool_t __ADS8688_MAN_Ch_Mode(u16 ch)
{
	__ADS8688_WriteCommandReg(ch);
	return true;
}
// =============================================================================
// ���ܣ���ȡɨ��ͨ�����е�ADת������
// ������seq
// ���أ�
// =============================================================================
static bool_t __ADS8688_GetRstModeData(u16 *pdata,u8 chnum)
{
	u8 i;
	u16 bData = 0x0000;
	bool_t ret=true;
	u16 temp=0;
	for (i = 0; i < chnum; i ++)
	{
		ret=__ADS8688_SpiTrans(&bData,1,pdata,1);
		if(ret==false)
			return ret;
		temp=*pdata;
//		temp=temp>>4;
		*pdata=temp;
		pdata++;
	}
	return true;
}

// =============================================================================
// ���ܣ���ȡ�ֶ�ͨ����ADת������
// �������ޡ�
// ���أ�
// =============================================================================
u16 __ADS8688_GetManChModeData(void)
{
	u16 dat=0;
	bool_t ret=true;
	u16 bData = 0x0000;
	ret=__ADS8688_SpiTrans(&bData,1,&dat,1);
	if(ret==false)
		return 0;
//	dat=dat>>4;
	return dat;
}
// =============================================================================
// ���ܣ���ʼ��ADS8688�������������úͳ�ʼ�����ã��ӿڵĳ�ʼ����
// ��������
// ���أ���
// =============================================================================
bool_t ADS8688_Init(u8 spi_port)
{
    u8 i;
    bool_t ret=true;
    __ADS8688_SpiInit(spi_port);
    for(i=0;i<CN_CHANNEL_NUM;i++)
    {
    	AD_Buf[i]=0;
    }

    for(i=0;i<CN_CP_SAM_RATE_BSP;i++)
    {
    	gs_cp1_buf[i]=0;
    	gs_cp2_buf[i]=0;
    }

    PIO_Set(ADRST);
    PIO_Set(ADCS);
    __ADS8688_EnterRstMode();

    //Channel 0~5 AUTO SEQ ENANBEL
    ret=__ADS8688_WriteReadProgramReg(AUTO_SEQ_EN,0x03F);
    if(ret==false)
    	goto EXIT;
    //AD0~AD5����ͨ��������ΧΪ+-2.5*4.096V  LSB:5mv
    //Channel 6~7��������Χ����Ϊ0~2.5Vref    LSB:2.5mv
    ret=__ADS8688_SetChannelInputRange(Channel_6_Input_Range,0x5);
    if(ret==false)
    	goto EXIT;
    ret=__ADS8688_SetChannelInputRange(Channel_7_Input_Range,0x5);
	if(ret==false)
		goto EXIT;

//    AD_Shell_Module_Install();
//    memset(ad_buf,0,10*CN_DEBUG_SAMP_CNT);
//    memset(ad_cpbuf,0,2*CN_CP_SAM_RATE_BSP*CN_DEBUG_SAMP_CNT);
    printf("AD8688 Init success.\r\n");
    return ret;

    EXIT:
	printf("ADS8688 Init failed.\r\n");
    return false;

}

// =============================================================================
// ���ܣ���һ���������ҵ����ֵ.
// �������ޡ�
// ���أ�
// =============================================================================
static u16 __GetMax(u16 *buf,u8 num)
{
	u8 i,cnt;
	u16 max=0;
	for(i=0;i<num;i++)
	{
		if(*(buf+i)>max)
		{
			max=*(buf+i);
			cnt=i;
		}
	}
	buf[cnt]=0;
	return max;
}
// =============================================================================
// ���ܣ�����CPͨ���ɼ��ķ�ֵ
// �������ޡ�
// ���أ�
// =============================================================================
static u16 __GetPwmPeak(u8 index)
{
	u8 i;
	u16 ver=0;
	u16 tmp[3];
	u16 *buf;
	if(index==0)
		buf=gs_cp1_buf;
	else
		buf=gs_cp2_buf;
	for(i=0;i<3;i++)
	{
		tmp[i]=__GetMax(buf,CN_CP_SAM_RATE_BSP);
	}
	ver=(tmp[0]+tmp[1]+tmp[2])/3;
    return ver;
}


// =============================================================================
// ���ܣ���ȡADS8688�Ĳɼ�ֵ������������ת����ת���������ٶ�ȡADת����ֵ
// ��������
// ���أ���
// =============================================================================
bool_t ADS8688_GetValue(uint16_t *pAinBuf)
{
    u8 i;
	bool_t ret=true;
	u16 dat;
    if(pAinBuf == NULL)
    {
    	printf("%s para invalid.\r\n",__FUNCTION__);
    	return false;
    }

	//����ΪAUTO_RSTģʽ�²�����0~5
	ret=__ADS8688_SetAutoScanSequence(0x3F);
	if(ret==false)
		return false;
	__ADS8688_EnterAutoRstMode();
	ret=__ADS8688_GetRstModeData(AD_Buf,6);
	if(ret==false)
		return false;

	__ADS8688_MAN_Ch_Mode(MAN_Ch_6);
	dat=__ADS8688_GetManChModeData();
	gs_cp1_buf[CN_CP_SAM_RATE_BSP-1]=dat;
	dat=__GetPwmPeak(0);
	AD_Buf[6]=dat;

	__ADS8688_MAN_Ch_Mode(MAN_Ch_7);
	dat=__ADS8688_GetManChModeData();
	gs_cp2_buf[CN_CP_SAM_RATE_BSP-1]=dat;
	dat=__GetPwmPeak(1);
	AD_Buf[7]=dat;

	//�ɼ�����ͨ��AUX
	//����ͨ��������Χ0~Vref (0~4.096)  LSB=0.625mV.
	__ADS8688_MAN_Ch_Mode(MAN_AUX);
	AD_Buf[8]=__ADS8688_GetManChModeData();
	for(i = 0; i < CN_CHANNEL_NUM; i++)
	{
		*(pAinBuf+i)=AD_Buf[i];
	}

//	if(gs_AdDebugFlag)
//	{
//		if(gs_AdDebugcnt<CN_DEBUG_SAMP_CNT)
//		{
//			for(i = 0; i < 5; i++)
//			{
//				ad_buf[5*gs_AdDebugcnt+i]=AD_Buf[i];
//			}
//			gs_AdDebugcnt++;
//		}
//	}

    return true;
}

// =============================================================================
// ���ܣ���ȡADS8688�Ĳɼ�ֵ������������ת����ת���������ٶ�ȡADת����ֵ
// ��������
// ���أ���
// =============================================================================
bool_t AD_GetCPChannel(u8 num)
{
	u16 dat;
	//ֻ����CPͨ��
	__ADS8688_MAN_Ch_Mode(MAN_Ch_6);
	dat=__ADS8688_GetManChModeData();
	gs_cp1_buf[num]=dat;

	__ADS8688_MAN_Ch_Mode(MAN_Ch_7);
	dat=__ADS8688_GetManChModeData();
	gs_cp2_buf[num]=dat;

	gs_adcpcnt++;

//	if(gs_AdDebugFlag)
//	{
//		if(gs_AdDebugCpcnt<CN_DEBUG_SAMP_CNT*CN_CP_SAM_RATE_BSP)
//		ad_cpbuf[gs_AdDebugCpcnt]=dat;
//		gs_AdDebugCpcnt++;
//	}
	return true;
}


bool_t AD_GetALLChannel(uint16_t *pAinBuf)
{
     ADS8688_GetValue(pAinBuf);
     gs_adcnt++;
     return true;
}

