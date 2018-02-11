/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include "stdint.h"
#include "systime.h"
#include "gkernel.h"
#include "time.h"
#include "wdt_soft.h"
#include "exp.h"
#include "djyos.h"
#include "kiko.h"
#include "uartctrl.h"
#include "driver.h"
#include "cpu_peri.h"
#include <sys/socket.h>
#include "cpu_peri.h"
#include <driver/flash/flash.h>
#include <string.h>
#include <stdlib.h>
#include <spibus.h>
#include <stddef.h>
//#include <stm32f7xx_hal_mmc.h>

struct FlashChip *gpNOR;

//**************************************************************************
extern struct IIC_Device *IIC_DevFind(const char *DevName);
extern s32  IIC_Write(struct IIC_Device *DelDev, u32 addr,u8 *buf,u32 len,
                bool_t block_option,u32 timeout);
extern s32  IIC_Read(struct IIC_Device *DelDev,u32 addr,u8 *buf,u32 len,u32 timeout);
extern void PWM_Set(u8 idx,uint32_t cycle,uint32_t duty);

//**************************************************************************
static void Sh_KIKO_Set(void);
static void Sh_KIKO_Clr(void);
static void Sh_RS232_Set(void);
static void Sh_RS232_Clr(void);
static void Sh_RS485_Set(void);
static void Sh_RS485_Clr(void);
static void Sh_SDRAM_Set(void);
static void Sh_SDRAM_Clr(void);
static void Sh_NAND_Set(void);
static void Sh_NAND_Clr(void);
static void Sh_NOR_Set(void);
static void Sh_NOR_Clr(void);
static void Sh_AUDIO_Set(void);
static void Sh_AUDIO_Clr(void);
void Sh_CAN_TX_ENABLE(void);
void Sh_CAN_RX_ENABLE(void);
void Sh_CAN_TX_DISABLE(void);
void Sh_CAN_RX_DISABLE(void);
static void Sh_Fs_Set(void);
static void Sh_Fs_Clr(void);

static void sh_ya_clr(void);
static void sh_fa_clr(void);
//**************************************************************************
static bool_t s_bKIKOFlag=false;
static bool_t s_bRS232Flag=false;
static bool_t s_bRS485Flag=false;
static bool_t s_bSdramFlag=false;
static bool_t s_bNandFlag=false;
static bool_t s_bNorFlag=false;
static bool_t s_bAudioFlag=false;
static bool_t s_bCanFlag=false;
static bool_t s_bFsFlag=false;
static char *param;
static bool_t s_bCanTxFlag=false;
static bool_t s_bCanRxFlag=false;
static uint8_t byCanNo=0;

struct ShellCmdTab const shell_cmd_bsp_test_table[]=
{
	{
			"kikoset",
			(bool_t(*)(char*))Sh_KIKO_Set,
			"使能KIKO测试",
			"COMMAND:kikoset+enter"
	},
	{
			"kikoclr",
			(bool_t(*)(char*))Sh_KIKO_Clr,
			"禁能KIKO测试",
			"COMMAND:kikoclr+enter"
	},
	{
			"232set",
			(bool_t(*)(char*))Sh_RS232_Set,
			"使能UART测试",
			"COMMAND:232set+enter"
	},
	{
			"232clr",
			(bool_t(*)(char*))Sh_RS232_Clr,
			"禁能UART测试",
			"COMMAND:232clr+enter"
	},

	{
			"485set",
			(bool_t(*)(char*))Sh_RS485_Set,
			"使能UART测试",
			"COMMAND:485set+enter"
	},
	{
			"485clr",
			(bool_t(*)(char*))Sh_RS485_Clr,
			"禁能UART测试",
			"COMMAND:485clr+enter"
	},
	{
			"sdramset",
			(bool_t(*)(char*))Sh_SDRAM_Set,
			"使能SDRAM测试",
			"COMMAND:sdramset+enter"
	},
	{
			"sdramclr",
			(bool_t(*)(char*))Sh_SDRAM_Clr,
			"禁能SDRAM测试",
			"COMMAND:sdramclr+enter"
	},

	{
			"nandset",
			(bool_t(*)(char*))Sh_NAND_Set,
			"使能nandflash测试",
			"COMMAND:nandset+enter"
	},
	{
			"nandclr",
			(bool_t(*)(char*))Sh_NAND_Clr,
			"禁能nandflash测试",
			"COMMAND:nandclr+enter"
	},
	{
			"norset",
			(bool_t(*)(char*))Sh_NOR_Set,
			"使能norflashad45db321测试",
			"COMMAND:norset+enter"
	},
	{
			"norclr",
			(bool_t(*)(char*))Sh_NOR_Clr,
			"禁能norflashad45db321测试",
			"COMMAND:norclr+enter"
	},
	{
			"audioset",
			(bool_t(*)(char*))Sh_AUDIO_Set,
			"使能音频测试",
			"COMMAND:audioset+enter"
	},
	{
			"audioclr",
			(bool_t(*)(char*))Sh_AUDIO_Clr,
			"禁能音频测试",
			"COMMAND:audioclr+enter"
	},

	{
			"fsset",
			(bool_t(*)(char*))Sh_Fs_Set,
			"使能文件系统测试",
			"COMMAND:fsset+enter"
	},
	{
			"fsclr",
			(bool_t(*)(char*))Sh_Fs_Clr,
			"禁能文件系统测试",
			"COMMAND:fsclr+enter"
	},
	{
			"yaffs2",
			(bool_t(*)(char*))sh_ya_clr,
			"选择yaffs2文件系统",
			"COMMAND:yaffs2+enter"
	},
	{
			"fat",
			(bool_t(*)(char*))sh_fa_clr,
			"选择fat文件系统",
			"COMMAND:fat+enter"
	},

	{
		"cantx",
		(bool_t(*)(char*))Sh_CAN_TX_ENABLE,
		"使能CAN TX测试",
		"COMMAND:cantx+enter"
	},
	{
		"canrx",
		(bool_t(*)(char*))Sh_CAN_RX_ENABLE,
		"使能CAN RX测试",
		"COMMAND:canrx+enter"
	},
	{
		"cantxn",
		(bool_t(*)(char*))Sh_CAN_TX_DISABLE,
		"禁能CAN TX测试",
		"COMMAND:cantxn+enter"
	},
	{
		"canrxn",
		(bool_t(*)(char*))Sh_CAN_RX_DISABLE,
		"禁能CAN RX测试",
		"COMMAND:canrxn+enter"
	},


};
//**************************************************************************
#define CN_BSP_TEST_SHELL_NUM  sizeof(shell_cmd_bsp_test_table)/sizeof(struct ShellCmdTab)
static struct ShellCmdRsc tg_bsp_test_shell_cmd_rsc[CN_BSP_TEST_SHELL_NUM];


/*******************************************************************************
功能:CAN控制器操作shell模块
参数:无.
返回值:1。
*********************************************************************************/
ptu32_t BSP_Test_Shell_Module_Install(void)
{
	Sh_InstallCmd(shell_cmd_bsp_test_table,tg_bsp_test_shell_cmd_rsc,CN_BSP_TEST_SHELL_NUM);
	return 1;
}


//**************************************************************************
void Sh_KIKO_Set(void)
{
	s_bKIKOFlag=true;
}
void Sh_KIKO_Clr(void)
{
	s_bKIKOFlag=false;
}


void Sh_RS232_Set(void)
{
	s_bRS232Flag=true;
}
void Sh_RS232_Clr(void)
{
	s_bRS232Flag=false;
}

void Sh_RS485_Set(void)
{
	s_bRS485Flag=true;
}
void Sh_RS485_Clr(void)
{
	s_bRS485Flag=false;
}


void Sh_SDRAM_Set(void)
{
	s_bSdramFlag=true;
}
void Sh_SDRAM_Clr(void)
{
	s_bSdramFlag=false;
}

void Sh_NAND_Set(void)
{
	s_bNandFlag=true;
}
void Sh_NAND_Clr(void)
{
	s_bNandFlag=false;
}

void Sh_NOR_Set(void)
{
	s_bNorFlag=true;
}
void Sh_NOR_Clr(void)
{
	s_bNorFlag=false;
}

void Sh_AUDIO_Set(void)
{
	s_bAudioFlag=true;
}
void Sh_AUDIO_Clr(void)
{
	s_bAudioFlag=false;
}


//**************************************************************************
void Sh_CAN_TX_ENABLE(void)
{
	s_bCanTxFlag=true;
}

void Sh_CAN_RX_ENABLE(void)
{
	s_bCanRxFlag=true;
}


void Sh_CAN_TX_DISABLE(void)
{
	s_bCanTxFlag=false;
}

void Sh_CAN_RX_DISABLE(void)
{
	s_bCanRxFlag=false;
}





void Sh_Fs_Set(void)
{
	s_bFsFlag=true;
}
void Sh_Fs_Clr(void)
{
	s_bFsFlag=false;
}
void sh_ya_clr(void)
{
	param="/yaffs2";
	printf("param = %s",param);
}
void sh_fa_clr(void)
{
	param="/fat";
	printf("param = %s",param);
}

//**************************************************************************
//KIKO Test
//**************************************************************************
#if 1
static void KIKO_Test(void)
{
	u8 kival=0;
	//u8 dwKoValue=0x7;
    u8 dwKoValue=0x0;
	//KI test
	KI_Read(&kival);
	printf("ki value:%x.\r\n",kival);
	//KO test
//	KO_StartRly(0);
	Djy_EventDelay(50*mS);
	KO_Write(dwKoValue);
	//led ko test
	//Led_KO_Ctrl(1,1);
	//Led_KO_Ctrl(2,0);
}
#endif

//**************************************************************************
//RS23_Test
//**************************************************************************
#if 1
struct DjyDevice *RS232_dev;
#define CN_RS232_BUF_LEN   128
#define CN_RS232_TIMEOUT    4000*mS
u8 rs232_sendbuf[CN_RS232_BUF_LEN];
u8 rs232_rcvbuf[CN_RS232_BUF_LEN];
static void RS232_Test(void)
{
	u32 rlen=0,i;
	Driver_CtrlDevice(RS232_dev,CN_UART_HALF_DUPLEX_SEND,0,0);
	//发送数据
	Driver_WriteDevice(RS232_dev,rs232_sendbuf,CN_RS232_BUF_LEN,\
	    					  0,CN_BLOCK_COMPLETE,CN_TIMEOUT_FOREVER);
	Driver_CtrlDevice(RS232_dev,CN_UART_HALF_DUPLEX_RECV,0,0);
	Djy_EventDelay(500*mS);
	//接收数据
	memset(rs232_rcvbuf, 0, CN_RS232_BUF_LEN);
	rlen=Driver_ReadDevice(RS232_dev,rs232_rcvbuf,CN_RS232_BUF_LEN,\
			0,CN_RS232_TIMEOUT);
	Driver_CtrlDevice(RS232_dev,CN_UART_DMA_UNUSED,0,0);
	if(rlen!=0)
	{
		printf("Recv data........\r\n");
		for(i=0;i<rlen;i++)
		{
		   printf("%x ",rs232_rcvbuf[i]);
		}
		printf("\r\n");
	}
    return;
}
#endif
//**************************************************************************
//RS485 Test
//**************************************************************************
#if 1
#define CN_RS485_BUF_LEN   30
static u8 rs485_sendbuf[CN_RS485_BUF_LEN];
static u8 rs485_rcvbuf[CN_RS485_BUF_LEN];
struct DjyDevice  *RS485_dev;
static void RS485_Test(void)
{
	u8 i;
	u32 rlen=0;
	static u8 q=1;
	for(i=0;i<CN_RS485_BUF_LEN;i++)
	{
		rs485_sendbuf[i]=i;
	}
	//如若测试UART7可直接将函数参数改为"UART7"
	RS485_dev=Driver_OpenDevice("UART6",2,CN_TIMEOUT_FOREVER);

	if(RS485_dev==NULL)
	{
		printf("RS485(UART6)dev open failed.\r\n");
		return;
	}

	//设置波特率
	Driver_CtrlDevice(RS485_dev,CN_UART_SET_BAUD,9600,0);

	if(q==1)
	{
//		Driver_CtrlDevice(RS485_dev,CN_UART_DMA_USED,0,0);
		q--;
	}
	// 启动串口
    Driver_CtrlDevice(RS485_dev,CN_UART_START,0,0);
    Driver_CtrlDevice(RS485_dev,CN_UART_HALF_DUPLEX_SEND,0,0);

    Driver_WriteDevice(RS485_dev,rs485_sendbuf,CN_RS485_BUF_LEN,
    					  0,CN_BLOCK_COMPLETE,CN_TIMEOUT_FOREVER);

    Driver_CtrlDevice(RS485_dev,CN_UART_HALF_DUPLEX_RECV,0,0);

    Djy_EventDelay(500*mS);
//
    //收
    rlen=Driver_ReadDevice(RS485_dev,rs485_rcvbuf,CN_RS485_BUF_LEN,0,500*mS);

    if(rlen!=0)
    {
    	printf("Recv data........\r\n");
    	for(i=0;i<rlen;i++)
    	{
           printf("%x ",rs485_rcvbuf[i]);
    	}
    	printf("\r\n");
    }
}
#endif


//**************************************************************************
//CAN Test
//**************************************************************************
#if 1
static u8 can_snd_buf[13]={0x88,0x11,0x011,0x11,0x11,0x55,0x55,0x55,0x55,0xff,\
		0x00,0xff,0x00};
static u32 gs_SndCnt=0,gs_RcvCnt=0;
static u8 can_rcv_buf[13];

static void CAN_Test(void)
{
	u32 len=0,i;

	len=CAN_WriteData(0,can_snd_buf,13);
	if(len==13)
	{
		gs_SndCnt++;
	}
	if((gs_SndCnt%10==0)&&(gs_SndCnt!=0))
	{
		printf("Snd CAN data cnt %d.\r\n",gs_SndCnt);
	}
	Djy_EventDelay(10*mS);

	len=CAN_ReadData(0,can_rcv_buf,13,NULL);
	if(len!=0)
	{
		gs_RcvCnt++;
		printf("Recv data........\r\n");
		for(i=0;i<len;i++)
		{
		   printf("%x ",can_rcv_buf[i]);
		}
		printf("\r\n");
	}
}
#endif

//**************************************************************************
//SDRAM_Test
//**************************************************************************
#if 1
#define BUFFER_SIZE         ((uint32_t)0x1000)
#define WRITE_READ_ADDR     ((uint32_t)0x0800)

/* Read/Write Buffers */
uint32_t aTxBuffer[BUFFER_SIZE];
uint32_t aRxBuffer[BUFFER_SIZE];

#define SDRAM_BANK_ADDR       ((u32)(0xC0000000))    //SDRAM起始地址
/* Counter index */
uint32_t uwIndex = 0;
/* Status variables */
__IO uint32_t uwWriteReadStatus = 0;
/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

void SDRAM_Test(void)
{
	/*##-2- SDRAM memory read/write access #####################################*/

	  /* Fill the buffer to write */
   Fill_Buffer(aTxBuffer, BUFFER_SIZE, 0xA244250F);
	  /* Write data to the SDRAM memory */
   for (uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
   {
	 *(__IO uint32_t*) (SDRAM_BANK_ADDR + WRITE_READ_ADDR + 4*uwIndex) = aTxBuffer[uwIndex];
   }
	   /* Read back data from the SDRAM memory */
   for (uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
   {
	   aRxBuffer[uwIndex] = *(__IO uint32_t*) (SDRAM_BANK_ADDR + WRITE_READ_ADDR + 4*uwIndex);
   }
   /*##-3- Checking data integrity ############################################*/

     for (uwIndex = 0; (uwIndex < BUFFER_SIZE) && (uwWriteReadStatus == 0); uwIndex++)
     {
       if (aRxBuffer[uwIndex] != aTxBuffer[uwIndex])
       {
         uwWriteReadStatus++;
       }
     }
     if (uwWriteReadStatus != 0)
     {
       printf("SDRAM test occur error.\r\n");
     }
     else
     {
    	 printf("SDRAM test ok.\r\n");
     }
}
#endif

//**************************************************************************
//**************************************************************************
//EEPROM测试
#if 1
#define CN_PAGE_NUM        256
#define CN_PAGE_SIZE      64
static u8 eeprom_wbuf[CN_PAGE_SIZE];
static u8 eeprom_rbuf[CN_PAGE_SIZE];

static void AT24_Test(void)
{
	u32 ret,w_addr,r_addr,i;
	u8 j;
	//一共256页
	for(i=0;i<CN_PAGE_NUM;i++)
	{
		w_addr=CN_PAGE_SIZE*i;
		r_addr=w_addr;
		for(j=0;j<CN_PAGE_SIZE;j++)
		{
			eeprom_wbuf[j]=j;
		}
		//每次读写不能超过页大小  一页为64bytes
		extern u32 AT24_WriteBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum);
	    ret=AT24_WriteBytes(w_addr, eeprom_wbuf, CN_PAGE_SIZE);
	    if(!ret)
	    {
	    	printf("EEPROM write error.\r\n");
	    	return;
	    }
	    else
	    {
	    	extern u32 AT24_ReadBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum);
	    	ret=AT24_ReadBytes(r_addr,eeprom_rbuf,CN_PAGE_SIZE);
	    	if(!ret)
	    	{
	    		printf("EEPROM read error.\r\n");
	    	    return;
	    	}
	    	for(j=0;j<CN_PAGE_SIZE;j++)
	    	{
	    		if(eeprom_rbuf[j]!=eeprom_wbuf[j])
	    		{
	    			printf("EEPROM test error.\r\n");
	    		}
	    	}
	    }
	}
	printf("EEPROM test success.\r\n");
}

#endif

//**************************************************************************
//PWM_Test
//**************************************************************************
#if 0
static void PWM_test(void)
{
	PWM_Set(0,1000,100);

//	PWM_Set(1,1000,75);
	PWM_Set(1,1000,90);

}
#endif


//**************************************************************************
//FRAM(at24cl128b)Test
//**************************************************************************
#if 1
uint8_t fram_wbuf[256];
uint8_t fram_rbuf[256];
#define CN_FRAM_LEN    (0x2000)
void FRAM_test(void)
{
	uint32_t i,ret,j;
	for(i=0;i<256;i++)
	{
		fram_wbuf[i]=i;
	}
	for(i=0;i<(CN_FRAM_LEN/256);i++)
	{
		ret=FRAM_Write_Data(256*i,fram_wbuf,256);
		if(ret!=256)
		{
			while(1);
		}
	}
	for(i=0;i<(CN_FRAM_LEN/256);i++)
	{
		FRAM_Read_Data(256*i,fram_rbuf,256);
		for(j=0;j<256;j++)
		{
			if(fram_rbuf[j]!=fram_wbuf[j])
			{
				printf("fram test error.\r\n");
			}
		}

	}
	printf("fram test ok.\r\n");

}
#endif


////**************************************************************************
////SPI NOR Flash(AT45DB321)测试
////**************************************************************************
//#if 1
//#define CN_WR_SIZE       64
//static u8 at45_flash_wbuf[64];
//static u8 at45_flash_rbuf[64];
//
//static void AT45DB321_Test(void)
//{
//	u32 ret,w_addr,r_addr,i,j;
//	for(i=0;i<16384;i++)
//	{
//		w_addr=CN_WR_SIZE*i;
//		r_addr=w_addr;
//		for(j=0;j<CN_WR_SIZE;j++)
//		{
//			at45_flash_wbuf[j]=j;
//		}
//		//每次读写不能超过页大小  一页为64bytes
//		extern u32 AT45_FLASH_Write(u32 Address,u8 *data,u32 data_len);
//	    ret=AT45_FLASH_Write(w_addr, at45_flash_wbuf, CN_WR_SIZE);
//	    if(!ret)
//	    {
//	    	printf("AT45DB321 flash write error.\r\n");
//	    	return;
//	    }
//	    else
//	    {
//	    	extern u32 AT45_FLASH_Read(u32 Address,u8 *data,u32 data_len);
//	    	ret=AT45_FLASH_Read(r_addr,at45_flash_rbuf,CN_WR_SIZE);
//	    	if(!ret)
//	    	{
//	    		printf("AT45DB321 flash read error.\r\n");
//	    	    return;
//	    	}
//	    	for(j=0;j<CN_WR_SIZE;j++)
//	    	{
//	    		if(at45_flash_rbuf[j]!=at45_flash_wbuf[j])
//	    		{
//	    			printf("AT45DB321 flash test error,cnt:0x%08x.\r\n",i);
//	    			while(1);
//	    		}
//	    	}
//	    }
//	    Djy_EventDelay(5000);
//	    if(i%50==0)
//	    {
//	    	printf("Success Cnt:0x%08x.\r\n",i);
//	    }
//	}
//	printf("AT45DB321 flash test success.\r\n");
//}
//#endif

//**************************************************************************
//SPI NOR Flash(AT45DB321)测试
//**************************************************************************

//static u32 NOR_Test(void)
//{
//	u16 *buf,*check;
//	u32 i,j;
//	u32 block,page;
//	s32 reg;
//
////	if((!ModuleInstall_SPI(CN_SPI4))&&(!ModuleInstall_AT45DB321E(1, "SPI4")))
////		while(1);
//	buf = (u16*)malloc(gpNOR->dwPageBytes);
//	if(!buf)
//	{
//		printf("\r\nTSET : error : memory out.\r\n");
//		return 0;
//	}
//	check=(u16*)malloc(gpNOR->dwPageBytes);
//	if(!check)
//	{
//		printf("\r\nTSET : error　:　memory out.\r\n");
//		return 0;
//	}
//	memset((u8*)check, 0, gpNOR->dwPageBytes);
//	for(i=0 ; i < (gpNOR->dwPageBytes>>1) ; i++)
//	{
//		buf[i]=i;
//	}
//	for(block=0; block<=1023; block++)
//	{
//		extern s32 _AT45_BlockErase(u32 dwBlock);
//		_AT45_BlockErase(block);
//		page=8*block;
//		for(i=0; i<8; i++)
//		{
//			extern s32 _AT45_PageRead(u32 dwPage, u8 *pBuf, u32 dwUnUse);
//			reg=_AT45_PageRead(page+i,(u8*)check,0);
//			if(reg != gpNOR->dwPageBytes)
//			{
//				printf("\r\nTEST : error : read failed.\r\n");
//			}
//			for(j=0; j<(gpNOR->dwPageBytes); j++)
//			{
//				if(0xffff != check[i])
//				{
//					printf("\r\nTEST : error : data was not erased, <%d> <%d> <%04x>.\r\n", i, j, check[j]);
//				}
//			}
//		}
//
//		for(i=0; i<8; i++)
//		{
//			extern s32 _AT45_PageWrite(u32 dwPage, u8 *pBuf, u32 dwUnUse);
//			reg = _AT45_PageWrite(page+i,(u8*)buf,0);
//			if(reg != gpNOR->dwPageBytes)
//			{
//				printf("\r\nTEST : error : write failed.\r\n");
//			}
//			reg = _AT45_PageRead(page+i,(u8*)check,0);
//			if(reg != gpNOR->dwPageBytes)
//			{
//				printf("\r\nTEST : error : read failed.\r\n");
//			}
//			for(j=0; j<(gpNOR->dwPageBytes>>1); j++)
//			{
//				if(buf[j] != check[j])
//				{
//					printf("\r\nTEST : error : data was not erased, <%d> <%d> <%04x> <%04x>.\r\n", i, j, check[j], buf[j]);
//				}
//			}
//		}
//		if((block%100==0)||(block==1023))
//		printf("\r\nTest success <%d>.\r\n",block);
//	}
//	return 1;
//}
//
//**************************************************************************
//SPI NAND Flash测试
//**************************************************************************

struct NandDescr *s_ptNandInfo;

void BadMarkFunctionCheck(struct FlashChip *Chip)
{
    u32 i;
    u32 Blks;

    if(NULL == Chip)
        while(1);

    Blks = Chip->Descr.Nand.BlksPerLUN * Chip->Descr.Nand.LUNs;

    for(i = 0; i < Blks; i++)
    {
        if(Chip->Ops.Special.Nand.MrkBad(i))
            while(1);
    }

    for(i = 0; i < Blks; i++)
    {
        if(-1 != Chip->Ops.Special.Nand.ChkBlk(i))
            while(1);
    }
}
void ContinuityTest(struct FlashChip *Chip)
{
    u32 i, j, Len;
    u32 Blks;
    u8 *Temp;

    if(NULL == Chip)
        while(1);

    Blks = Chip->Descr.Nand.BlksPerLUN * Chip->Descr.Nand.LUNs;

    for(i = Chip->Descr.Nand.ReservedBlks; i < Blks; i++)
    {
        if(0 != Chip->Ops.ErsBlk(i))
        {
            /* 擦除失败 */
//          TraceDrv(NAND_TRACE_ERROR, "block %d cannot be erased!\r\n", i);
            while(1);
        }
    }

    // 擦除后的校验
    Len = (Chip->Descr.Nand.BytesPerPage + Chip->Descr.Nand.OOB_Size);
    Temp = (u8*)malloc(Len);
    if(NULL == Temp)
        while(1);// 内存不足
    for(i = (Chip->Descr.Nand.ReservedBlks * Chip->Descr.Nand.PagesPerBlk); i < (Blks * Chip->Descr.Nand.PagesPerBlk); i++)
    {
        if(Len != Chip->Ops.RdPage(i, Temp, SPARE_REQ | NO_ECC))
            while(1);
        for(j = 0; j < Len; j++)
        {
            if(0xFF != Temp[j])
                while(1);
        }
    }
}
static void NAND_Test(void)
{
    struct FlashChip *Chip;

    u32 i;
    u8 *Buf;
    u32 TestPages;
    u32 TestBlocks;
    u32 ErrorCount = 0;

//    if(ModuleInstall_NAND("nand", 0, 0))//安装nand设备
//        while(1);
//    extern void ChipRawTest(void);
//    ChipRawTest();


    Chip = Container(s_ptNandInfo, struct FlashChip, Descr);
    Buf = Chip->Buf;

   for(TestBlocks = 0; TestBlocks < s_ptNandInfo->BlksPerLUN; TestBlocks++)
    {
        if(Chip->Ops.ErsBlk(TestBlocks))
            while(1);
    }
    for(TestBlocks = 0; TestBlocks < s_ptNandInfo->BlksPerLUN; TestBlocks++)
    {
        Chip->Ops.ErsBlk(TestBlocks);
        if(TestBlocks%200==0)
        {
        	printf("\r\nTest block (%d) succeed",TestBlocks);
        }

        for(TestPages = 0; TestPages < s_ptNandInfo->PagesPerBlk; TestPages++)
        {
            u32 CurPage = TestPages + (TestBlocks * s_ptNandInfo->PagesPerBlk);

            //测试擦除一页
            memset(Buf, 0x00, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
            Chip->Ops.RdPage(CurPage, Buf, NO_ECC | SPARE_REQ);
            for(i=0; i<(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size); i++)
            {
                if(0xFF != Buf[i])
                {
                    printf("\r\n擦除失败:Block = %d , Page = %d\r\n",TestBlocks,CurPage);
                    ErrorCount++;
                }
            }

            //测试写、读一页
            for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
                Buf[i] = (u8)i;
            Chip->Ops.WrPage(CurPage, Buf, HW_ECC | SPARE_REQ);
            memset(Buf, 0x00, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
            Chip->Ops.RdPage(CurPage, Buf, HW_ECC | SPARE_REQ);

            for(i=0; i<(s_ptNandInfo->BytesPerPage); i++)
            {
                if(((u8)i) != Buf[i])
                {
                    printf("读写失败:Block = %d ,"\
                             " Page = %d\n\r",TestBlocks,CurPage);
                    ErrorCount++;
                }
            }
//            Buf[1] = 0;
//            Chip->Ops.WrPage(CurPage, Buf, NO_ECC| SPARE_REQ);
//            memset(Buf, 0x55, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
//            Chip->Ops.RdPage(CurPage, Buf, HW_ECC | SPARE_REQ);
//            for(i=0; i<(s_ptNandInfo->BytesPerPage); i++)
//            {
//                if(((u8)i) != Buf[i])
//                {
//                    printf("NO ECC");
//                    printf("\r\n纠错失败:Block = %d ,"\
//                                 " Page = %d i=%d\r\n",TestBlocks,CurPage,i);
//                    ErrorCount++;
//                }
//            }
        }
    }
    printf("\n\rBad Mark Function Check ~ing Please wait...\n\r");
    BadMarkFunctionCheck(Chip);
    ContinuityTest(Chip);
    if(ErrorCount)
        printf("Test End ErrorCount = %d \n\r",ErrorCount);
    else
        printf("Test End No Error\n\r");
}
//**************************************************************************
//file_test  测试
//**************************************************************************
#if 0
static void file_test(void)  //char *para
{

	char *Path;
	extern const unsigned char small_file[1024];
	unsigned char *ConfigFileSource = (unsigned char *)small_file;
	extern int mkdir (const char *filename, mode_t mode);
	mkdir("/yaffs2/123",0);
	unsigned char *ConfigFileName = "small_file_00000000";
	u32 ConfigFileSize = sizeof(small_file);
	char *TestFileName = ConfigFileName;
	u8 *Buffer;
	u8 RootLen = strlen("/yaffs2/123");
	u8 FileLen = strlen(TestFileName);
	u8 PathLen = RootLen + FileLen + 2;
	u32 i_1 = 1, i_2 = 1, i_3 = 1, i_4 = 1, i_5 = 1, i_6 = 1, i_7 = 1, i_8 = 1;
	u32 Ret;
	u32 j=0;
	u32 DataLen = ConfigFileSize;
	unsigned char *TestFile = ConfigFileSource;
	FILE *Fp0;
	u32 RetLen, i;
	Buffer =(u8*) malloc(DataLen);
	if(NULL == Buffer)
	{
		printf("error: memory run out.\r\n");
		while(1);
	}
	Path = malloc(PathLen+1);
	if(NULL == Path)
	{
		printf("error: memory run out.\r\n");
		while(1);
	}

//	//	一个文件大小1G时能创建多少个文件
	memset(Path, 0, PathLen+1);
	Path[0] = '/';
	memcpy(Path+1, "/yaffs2/123", RootLen);
	Path[RootLen+1] = '/';
	strcpy(Path+RootLen+2, TestFileName);
	memset(Buffer, 0, DataLen);
	for(j=0 ; j<10; j++)
	{
		Fp0 = fopen(Path, "w+");
		printf("打开文件 \"%s\"  GB = %d.\r\n", Path,j);
		if(NULL == Fp0)
		{
			printf("打开文件 \"%s\" 失败  GB = %d.\r\n", Path,j);
			goto END;
		}
		RetLen = fwrite(TestFile, 1, DataLen, Fp0);
		if(DataLen != RetLen)
		{
			printf("cannot write \"%s\"  j = %d  .\r\n", Path,j);
		}
		Ret = fflush(Fp0);
		if(Ret)
		{
			printf("cannot flush \"%s\"  j = %d.\r\n", Path,j);
		}
		Ret = fseek(Fp0, 0, V_SEEK_SET);
		if(Ret)
		{
			printf("cannot seek \"%s\"  j = %d.\r\n", Path,j);

		}
		RetLen = fread(Buffer, 1, DataLen, Fp0);
		if(DataLen == RetLen)
		{
			for(i = 0; i < DataLen; i++)
			{
				if(TestFile[i] != Buffer[i])
				{
					printf("cannot working the \"%s\" TestFile=%d,Buffer=%d,i=%d. \r\n",Path,TestFile[i],Buffer[i],i);
				}
			}
		}	//				printf("cannot read \"%s\"\r\n", Path);
		else
		{
			printf("cannot read \"%s\"  j = %d  \r\n", Path,j);

		}
		memset(Buffer, 0, DataLen);
		Ret = fclose(Fp0);
		if(Ret)
		{
			printf("cannot close \"%s\"  j = %d.\r\n", Path,j);
		}
		remove(Path);

END:
		 if (Path[PathLen - 1] != '9')
		 {
			 Path[PathLen - 1] = ('0' + i_1++);
			 if (10 == i_1)
		      i_1 = 1;
		 }
		 else if (Path[PathLen - 2] != '9')
		 {
			 Path[PathLen - 2] = ('0' + i_2++);
			 Path[PathLen - 1] = '0';
			 if (10 == i_2)
			     i_2 = 1;
			 }
		 else if (Path[PathLen - 3] != '9')
		 {
			 Path[PathLen - 3] = ('0' + i_3++);
			 Path[PathLen - 2] = '0';
			 Path[PathLen - 1] = '0';
			 if (10 == i_3)
			      i_3 = 1;
		 }
		 else if (Path[PathLen - 4] != '9')
		 {
			 Path[PathLen - 4] = ('0' + i_4++);
			 Path[PathLen - 3] = '0';
			 Path[PathLen - 2] = '0';
			 Path[PathLen - 1] = '0';
			 if (10 == i_4)
				 i_4 = 1;
		 }
		 else if (Path[PathLen - 5] != '9')
		 {
			 Path[PathLen - 5] = ('0' + i_5++);
			 Path[PathLen - 4] = '0';
			 Path[PathLen - 3] = '0';
			 Path[PathLen - 2] = '0';
			 Path[PathLen - 1] = '0';
			 if (10 == i_5)
				 i_5 = 1;
		 }
		 else if (Path[PathLen - 6] != '9')
		 {
			 Path[PathLen - 6] = ('0' + i_6++);
			 Path[PathLen - 5] = '0';
			 Path[PathLen - 4] = '0';
			 Path[PathLen - 3] = '0';
			 Path[PathLen - 2] = '0';
			 Path[PathLen - 1] = '0';
			 if (10 == i_6)
				 i_6 = 1;
		 }
		 else if (Path[PathLen - 7] != '9')
		 {
			 Path[PathLen - 7] = ('0' + i_7++);
			 Path[PathLen - 6] = '0';
			 Path[PathLen - 5] = '0';
			 Path[PathLen - 4] = '0';
			 Path[PathLen - 3] = '0';
			 Path[PathLen - 2] = '0';
			 Path[PathLen - 1] = '0';
			 if (10 == i_7)
				 i_7 = 1;
		 }
		 else if (Path[PathLen - 8] != '9')
		 {
			 Path[PathLen - 8] = ('0' + i_8++);
			 Path[PathLen - 7] = '0';
			 Path[PathLen - 6] = '0';
			 Path[PathLen - 5] = '0';
			 Path[PathLen - 4] = '0';
			 Path[PathLen - 3] = '0';
			 Path[PathLen - 2] = '0';
			 Path[PathLen - 1] = '0';
			 if (10 == i_8)
				 i_8 = 1;
		 }
	}

	free(Buffer);
	free(Path);
}
#endif
//static void AUDIO_Test(void)
//{
//
//}
//static void Fs_Test(void)
//{
//
//}

//static void SDram_Test(void)
//{
//	char *SD;
////	struct HeapCB *sdram;
////	extern struct HeapCB *M_FindHeap(const char *HeapName);
////	extern void __M_FreeHeap(void * pl_mem,struct HeapCB *Heap);
////	extern void *__M_MallocHeap(ptu32_t size,struct HeapCB *Heap, u32 timeout);
////	sdram =(struct HeapCB *)M_FindHeap("extram");
////	SD = __M_MallocHeap(100,sdram,0);
////	__M_FreeHeap(SD,sdram);
//
//	extern void *yaffsfs_malloc(size_t size);
//	extern void yaffsfs_free(void *ptr);
//	extern void yaffsfs_OSInitialisation(void);
//	yaffsfs_OSInitialisation();
//	SD=yaffsfs_malloc(100);
//	yaffsfs_free(SD);
//}

void HardDrv_Test(void)
{
	u16 i;
	BSP_Test_Shell_Module_Install();

	extern bool_t pppStart(void);
//	for(i=0;i<CN_RS232_BUF_LEN;i++)
//	{
//		rs232_sendbuf[i]=i;
//	}
//	//如若测试UART4/UART5可直接将函数参数改为"UART4"、"UART5"
//	RS232_dev=Driver_OpenDevice("UART6",2,CN_TIMEOUT_FOREVER);
//	if(RS232_dev==NULL)
//	{
//		printf("UART3 open failed.\r\n");
//		return;
//	}
//	//此处可修改UART的波特率、是否使用DMA等属性，调用Driver_CtrlDevice函数即可，
//	//具体属性可参见driver.c、uart.c及cpu_peri_uart.c
//	Driver_CtrlDevice(RS232_dev,CN_UART_SET_BAUD,2400,0);
//	Driver_CtrlDevice(RS232_dev,CN_UART_DMA_USED,0,0);
//	Driver_CtrlDevice(RS232_dev,CN_UART_START,0,0);
//	for(i=0;i<CN_RS485_BUF_LEN;i++)
//	{
//		rs485_sendbuf[i]=i;
//	}
//	//如若测试UART7可直接将函数参数改为"UART7"
//	RS485_dev=Driver_OpenDevice("UART7",2,CN_TIMEOUT_FOREVER);
//
//	if(RS485_dev==NULL)
//	{
//		printf("RS485(UART6)dev open failed.\r\n");
//		return;
//	}
//
//	//设置波特率
//	Driver_CtrlDevice(RS485_dev,CN_UART_SET_BAUD,9600,0);
//
//	Driver_CtrlDevice(RS485_dev,CN_UART_DMA_UNUSED,0,0);
//	 // 启动串口
//    Driver_CtrlDevice(RS485_dev,CN_UART_START,0,0);
	while(1)
	{
      if(s_bKIKOFlag)
      {
    	  KIKO_Test();
      }
      if(s_bRS232Flag)
      {
    	  RS232_Test();
      }
      if(s_bRS485Flag)
      {
    	  RS485_Test();
      }
      if(s_bSdramFlag)
	  {
		  SDRAM_Test();
	  }
	  if(s_bNandFlag)
	  {
		  NAND_Test();
	  }
	  if(s_bNorFlag)
	  {
//		  AT45DB321_Test();
//		  NOR_Test();
		  FRAM_test();
	  }
	  if(s_bAudioFlag)
	  {
//		  SDram_Test();
		  AT24_Test();
	  }
	  if(s_bCanFlag)
	  {
		  CAN_Test();
	  }
	  if(s_bFsFlag)
	  {
//		  LOCAL_Test_c();
//		  ModuleInstall_FAT("/dev/emmc", 1);
//		  file_test(param);

//		  file_test();
	  }

	  Djy_EventDelay(1000*mS);
	}

}
