// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：ADS8668.c
// 模块描述: ADS8688采集模块的底层驱动，调用了SPI接口通信函数读取AD采集值
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 05/10.2017
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "endian.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "shell.h"
#include "ads8688.h"
#include "harddrv_ad.h"


extern const Pin ADCS[];
// =============================================================================
#define ADS8688_CS           (0)
#define WRITE                 1
#define READ                  0

extern s32 SPI_Read(void);
extern s32 SPI_Write(uint32_t dwNpcs, uint16_t wData);

// =============================================================================
// 功能：ADS8688对应的SPI控制器初始化
// 参数：port, SPI控制器编号，在sr5333-v2.0中ADS8688采用的是SPI2;
// 返回：空.
// =============================================================================
void ADS8688_SpiInit(uint8_t port)
{
	//已经在board.c里面初始化了SPI2的驱动
	 Spi *Reg;
	 Reg = (Spi *)SPI_BASE;
	 //配置SPI使用GPIO引脚
	 RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // 使能SPI2
	 RCC->APB1RSTR |= RCC_APB1RSTR_SPI2RST; // 复位SPI2
	 RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST; // 停止复位SPI2

	 Reg->CR1 |=SPI_CR1_BR_0;//波特率设置  108M/4=27MHz
	 Reg->CR1 &= ~SPI_CR1_CPOL;                      // 空闲模式下SCK为1 CPOL=0
	 Reg->CR1 |= SPI_CR1_CPHA;                      // 数据采样从第2个时间边沿开始,CPHA=1
	 Reg->CR1 &= ~SPI_CR1_RXONLY;//全双工通讯模式
	 Reg->CR1 &= ~SPI_CR1_LSBFIRST;//数据帧格式
	 Reg->CR1 &= ~ SPI_CR1_SSM;         // 禁止软件nss管理
	 Reg->CR1 |= SPI_CR1_SSI;  //SSM
	 Reg->CR1 |= SPI_CR1_MSTR;       // SPI主机

	 Reg->CR2 |=SPI_CR2_DS_0|SPI_CR2_DS_1|SPI_CR2_DS_2|SPI_CR2_DS_3 ;//数据帧格式为16bit
	 Reg->CR2 |=SPI_CR2_SSOE;//SS 输出使能
	 Reg->CR2 |=SPI_CR2_NSSP;//生成 NSS 脉冲
	 Reg->CR2 &=~SPI_CR2_FRXTH;//16字节fifo
	 Reg->I2SCFGR &= (uint16_t)(~SPI_I2SCFGR_I2SMOD);// 选择SPI模式
	 Reg->CR1 |= SPI_CR1_SPE;
}

// =============================================================================
// 功能：ADS8688对应的SPI控制器初始化
// 参数：addr, Program Register地址;
//       data，仅对写指令有效，为写进行寄存器的值.
// 返回：
// =============================================================================
static bool_t __ADS8688_SpiTrans(uint16_t *wBuf, uint32_t wlen, uint16_t *rBuf,uint32_t rlen)
{
    uint32_t i;
    s32 ret;
    bool_t result = false;
    Spi *Reg;
   	Reg = (Spi *)SPI_BASE;
    PIO_Clear(ADCS);
	for(i = 0; i < wlen; i++)
	{
    	ret=SPI_Write(ADS8688_CS,wBuf[i]);
		if(ret==-1)
	      	goto SPI_EXIT;
    	ret=SPI_Read();
	    if(ret==-1)
	      	goto SPI_EXIT;
	}
	for(i = 0; i < rlen; i ++)
	{
    	ret=SPI_Write(ADS8688_CS,0x0000);
		if(ret==-1)
	      	goto SPI_EXIT;
    	 ret = SPI_Read();
		 if(ret==-1)
		    goto SPI_EXIT;

	     rBuf[i] = (uint16_t)(ret & 0xFFFF);
	}
  	result = true;
SPI_EXIT:
    PIO_Set(ADCS);
	return result;
}

// =============================================================================
void ADS8688_WriteCmdReg(uint16_t command)//写ADS8688命令寄存器
{
	uint16_t tmp=0;
    __ADS8688_SpiTrans(&command,1,&tmp,1);
}

void ADS8688_EnterAutoRstMode(void)    //进入自动扫描模式
{
    ADS8688_WriteCmdReg(AUTO_RST);
}

void ADS8688_EnterRstMode(void)
{
    ADS8688_WriteCmdReg(RST);
}

// =============================================================================
// 功能：Program Register写操作
// 参数：addr, Program Register地址;
//       data，仅对写指令有效，为写进行寄存器的值.
// 返回：
// =============================================================================
static bool_t ADS8668_WRProgReg(uint8_t addr,uint8_t data)
{
	uint16_t w_dat=0x0000;
	uint16_t r_dat=0x0000;
	bool_t ret=true;
	uint8_t temp=0x00;
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
// 功能：设置通道输入范围
// 参数：seq
// 返回：
// =============================================================================
bool_t ADS8688_SetChInRange(uint8_t ch,uint8_t range)
{
	bool_t result;
	result=ADS8668_WRProgReg(ch,range);
	return result;
}


// =============================================================================
// 功能：读取扫描通道序列的AD转换数据
// 参数：seq
// 返回：
// =============================================================================
bool_t ADS8688_GetRstModeData(uint8_t seq,uint16_t *pdata,uint8_t chnum)
{
	uint8_t i;
	uint16_t bData = 0x0000;
	bool_t ret=true;
	uint16_t temp=0;
	ret=ADS8664_WRProgReg(AUTO_SEQ_EN,seq);
	if(!ret)
		return false;
	ADS8688_EnterRstMode();
	for (i = 0; i < chnum; i ++)
	{
		ret=__ADS8688_SpiTrans(&bData,1,pdata,1);
		if(ret==false)
			return ret;
		temp=*pdata;
		*pdata=temp;
		pdata++;
	}
	return true;
}

// =============================================================================
// 功能：读取手动通道的AD转换数据
// 参数：无。
// 返回：
// =============================================================================
uint16_t ADS8688_GetManChModeData(uint16_t ch)
{
	uint16_t dat=0;
	bool_t ret=true;
	uint16_t bData = 0x0000;
	//设置指定通道为手动模式
	ADS8688_WriteCmdReg(ch);
	ret=ADS8688_SpiTrans(&bData,1,&dat,1);
	if(ret==false)
		return 0;
	return dat;
}


