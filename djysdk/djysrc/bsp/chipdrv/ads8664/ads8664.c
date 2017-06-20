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
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
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
// 文件名     ：ads8664.c
// 模块描述: AD采样芯片ads8664驱动
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 8/24.2016
// =============================================================================
#include <harddrv_ad.h>
#include "stdint.h"
#include "stddef.h"
#include "endian.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "ads8664.h"


extern const Pin ADCS[];

// =============================================================================
#define ADS8664_CS           (0)
//
#define WRITE    1
#define READ     0

extern s32 SPI_Read(void);
extern s32 SPI_Write(uint32_t dwNpcs, uint16_t wData);

// =============================================================================
// 功能：ADS8664对应的SPI控制器初始化
// 参数：port, SPI控制器编号，在sr5333-v2.0中ads8664采用的是SPI2;
// 返回：空.
// =============================================================================
void ADS8664_SpiInit(uint8_t port)
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
// 功能：ADS8664对应的SPI控制器初始化
// 参数：addr, Program Register地址;
//       data，仅对写指令有效，为写进行寄存器的值.
// 返回：
// =============================================================================
static bool_t __ADS8664_SpiTrans(uint16_t *wBuf, uint32_t wlen, uint16_t *rBuf,uint32_t rlen)
{
    uint32_t i;
    s32 ret;
    bool_t result = false;
    Spi *Reg;
   	Reg = (Spi *)SPI_BASE;
    PIO_Clear(ADCS);
	for(i = 0; i < wlen; i++)
	{
    	ret=SPI_Write(ADS8664_CS,wBuf[i]);
		if(ret==-1)
	      	goto SPI_EXIT;
    	ret=SPI_Read();
	    if(ret==-1)
	      	goto SPI_EXIT;
	}
	for(i = 0; i < rlen; i ++)
	{
    	ret=SPI_Write(ADS8664_CS,0x0000);
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

 void ADS8664_WriteCommandReg(uint16_t command)//写ADS8664命令寄存器
{
	uint16_t tmp=0;
    __ADS8664_SpiTrans(&command,1,&tmp,1);
}

 void ADS8664_EnterAutoRstMode(void)    //进入自动扫描模式
{
    ADS8664_WriteCommandReg(AUTO_RST);
}

 void ADS8664_EnterRstMode(void)
{
    ADS8664_WriteCommandReg(RST);
}

// =============================================================================
// 功能：Program Register写操作
// 参数：addr, Program Register地址;
//       data，仅对写指令有效，为写进行寄存器的值.
// 返回：
// =============================================================================
static bool_t ADS8664_WRProgReg(uint8_t addr,uint8_t data)
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
	ret=__ADS8664_SpiTrans(&w_dat,1,&r_dat,1);
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
bool_t ADS8664_SetChInRange(uint8_t ch,uint8_t range)
{
	bool_t result;
	result=ADS8664_WRProgReg(ch,range);
	return result;
}

// =============================================================================
// 功能：获取连续自动采样通道数据
// 参数：seq
// 返回：
// =============================================================================
bool_t ADS8664_GetRstModeData(uint8_t seq,uint16_t *pdata,uint8_t chnum)
{
	bool_t ret;
	uint8_t i;
	uint16_t bData = 0x0000;
	uint16_t temp=0;
	ret=ADS8664_WRProgReg(AUTO_SEQ_EN,seq);
	if(!ret)
		return false;
	ADS8664_EnterAutoRstMode();
	for (i = 0; i < chnum; i ++)
	{
		ret=__ADS8664_SpiTrans(&bData,1,pdata,1);
		if(ret==false)
			return ret;
		temp=*pdata;
		temp=temp>>4;
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
uint16_t ADS8664_GetManModeData(uint16_t ch)
{
	uint16_t dat=0;
	bool_t ret=true;
	uint16_t bData = 0x0000;
	//设置指定通道为手动模式
	ADS8664_WriteCommandReg(ch);
	ret=__ADS8664_SpiTrans(&bData,1,&dat,1);
	if(ret==false)
		return 0;
	dat=dat>>4;
	return dat;
}
