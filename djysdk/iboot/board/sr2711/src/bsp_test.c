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
static void Sh_CAN_Set(void);
static void Sh_CAN_Clr(void);
static void Sh_Fs_Set(void);
static void Sh_Fs_Clr(void);
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
			"canset",
			(bool_t(*)(char*))Sh_CAN_Set,
			"使能CAN测试",
			"COMMAND:canset+enter"
	},
	{
			"canclr",
			(bool_t(*)(char*))Sh_CAN_Clr,
			"禁能CAN测试",
			"COMMAND:canclr+enter"
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

void Sh_CAN_Set(void)
{
	s_bCanFlag=true;
}
void Sh_CAN_Clr(void)
{
	s_bCanFlag=false;
}

void Sh_Fs_Set(void)
{
	s_bFsFlag=true;
}
void Sh_Fs_Clr(void)
{
	s_bFsFlag=false;
}


#if 0

#define CRC_TEST_START_ADDR   0x8082000
#define CRC_TEST_LEN          (1024*600+50)

static u8 buf[1024];
extern u32 Crc32(char *buf, int len);
extern void init_crc_table(void);
extern unsigned int crc32(unsigned int crc,unsigned char *buffer, unsigned int size);
extern u32 CRC32( u32 crc,char *buf, int len,bool_t lastflg,bool_t startflg);
static void CRC_Test(void)
{
   u32 crc1,crc2,crc3,i,ret;
   init_crc_table();
   memcpy(buf,(u8 *)CRC_TEST_START_ADDR,1024);
   ret=crc32(0,buf,1024);
   for(i=1;i<600;i++)
   {
	   memcpy(buf,(u8 *)(CRC_TEST_START_ADDR+1024*i),1024);
	   ret=crc32(ret,buf,1024);
   }
   memcpy(buf,(u8 *)(CRC_TEST_START_ADDR+1024*600),50);
   ret=crc32(ret,buf,50);
   crc1=ret;
   printf("crc1:0x%08x.\r\n",crc1);
   crc2=Crc32((u8 *)CRC_TEST_START_ADDR,CRC_TEST_LEN);
   printf("crc2:0x%08x.\r\n",crc2);
   memcpy(buf,(u8 *)CRC_TEST_START_ADDR,1024);
   ret=CRC32(0,buf,1024,false,true);
   for(i=1;i<600;i++)
   {
  	   memcpy(buf,(u8 *)(CRC_TEST_START_ADDR+1024*i),1024);
  	   ret=CRC32(ret,buf,1024,false,false);
   }
   memcpy(buf,(u8 *)(CRC_TEST_START_ADDR+1024*600),50);
   ret=CRC32(ret,buf,50,true,false);
   crc3=ret;
   printf("crc3:0x%08x.\r\n",crc3);


}

#endif



//**************************************************************************
//KIKO Test
//**************************************************************************
#if 1
static void KIKO_Test(void)
{
	u8 kival=0;
	//u8 dwKoValue=0x7;
    u8 dwKoValue;
	//KI test
	KI_Read(&kival);
	printf("ki value:%x.\r\n",kival);
	//KO test
//	KO_StartRly(0);
//	Djy_EventDelay(50*mS);
	dwKoValue=0x7;
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
#define CN_RS232_BUF_LEN   100
static u8 rs232_sendbuf[CN_RS232_BUF_LEN];
static u8 rs232_rcvbuf[CN_RS232_BUF_LEN];
#define CN_RS232_TIMEOUT    200*mS
static void RS232_Test(void)
{
	struct DjyDevice *RS232_dev;
	u32 rlen=0,i;

	for(i=0;i<CN_RS232_BUF_LEN;i++)
	{
		rs232_sendbuf[i]=i;
	}
	//如若测试UART4/UART5可直接将函数参数改为"UART4"、"UART5"
	RS232_dev=Driver_OpenDevice("UART3",2,CN_TIMEOUT_FOREVER);
	if(RS232_dev==NULL)
	{
		printf("UART3 open failed.\r\n");
		return;
	}
	//此处可修改UART的波特率、是否使用DMA等属性，调用Driver_CtrlDevice函数即可，
	//具体属性可参见driver.c、uart.c及cpu_peri_uart.c
	Driver_CtrlDevice(RS232_dev,CN_UART_SET_BAUD,115200,0);
	Driver_CtrlDevice(RS232_dev,CN_UART_DMA_UNUSED,0,0);
	Driver_CtrlDevice(RS232_dev,CN_UART_START,0,0);

	//发送数据
	Driver_WriteDevice(RS232_dev,rs232_sendbuf,CN_RS232_BUF_LEN,\
	    					  0,CN_BLOCK_COMPLETE,CN_RS232_TIMEOUT);
	Djy_EventDelay(500*mS);
	//接收数据
	rlen=Driver_ReadDevice(RS232_dev,rs232_rcvbuf,CN_RS232_BUF_LEN,\
			0,CN_RS232_TIMEOUT);
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
#define CN_RS485_BUF_LEN   100
static u8 rs485_sendbuf[CN_RS485_BUF_LEN];
static u8 rs485_rcvbuf[CN_RS485_BUF_LEN];
static void RS485_Test(void)
{
	struct DjyDevice  *RS485_dev;
	u8 i;
	u32 rlen=0;

	for(i=0;i<CN_RS485_BUF_LEN;i++)
	{
		rs485_sendbuf[i]=i;
	}
	//如若测试UART7可直接将函数参数改为"UART7"
	RS485_dev=Driver_OpenDevice("UART4",2,CN_TIMEOUT_FOREVER);

	if(RS485_dev==NULL)
	{
		printf("RS485(UART4)dev open failed.\r\n");
		return;
	}

	//设置波特率
	Driver_CtrlDevice(RS485_dev,CN_UART_SET_BAUD,2400,0);
	 // 启动串口
    Driver_CtrlDevice(RS485_dev,CN_UART_START,0,0);

    //注意如果是串口6(UART6，即接的各种传感器那一路，则必须加下面一行代码，其他串口不需要)
    extern void Board_SetCsSend(u8 SerialNo,u8 CsNo);
    Board_SetCsSend(CN_UART6,0);   //第二个参数可以是0~4

    Driver_WriteDevice(RS485_dev,rs485_sendbuf,CN_RS485_BUF_LEN,
    					  0,CN_BLOCK_COMPLETE,CN_TIMEOUT_FOREVER);
    //注意如果是串口6(UART6，即接的各种传感器那一路，则必须加下面一行代码，其他串口不需要)
    extern void Board_SetCsRecv(u8 SerialNo,u8 CsNo);
    Board_SetCsRecv(CN_UART6,0); //第二个参数可以是0~4

    Driver_CtrlDevice(RS485_dev,CN_UART_HALF_DUPLEX_RECV,0,0);

    Djy_EventDelay(500*mS);

    //收
    rlen=Driver_ReadDevice(RS485_dev,rs485_rcvbuf,50,0,10000);

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
	for(i=0;i<100;i++)
	{
		len=CAN_WriteData(0,can_snd_buf,13);
		if(len==13)
		{
			gs_SndCnt++;
		}
		Djy_EventDelay(5*mS);
	}
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
#if 0
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
#if 0
uint8_t fram_wbuf[256];
uint8_t fram_rbuf[256];
void FRAM_test(void)
{
	uint32_t i,ret,j;
	for(i=0;i<256;i++)
	{
		fram_wbuf[i]=i;
	}
	for(i=0;i<256;i++)
	{
		ret=FRAM_Write_Data(256*i,fram_wbuf,256);
		if(ret!=256)
			while(1);
	}
	for(i=0;i<256;i++)
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


//**************************************************************************
//SPI NOR Flash(AT45DB321)测试
//**************************************************************************
#if 1
#define CN_WR_SIZE       64
static u8 at45_flash_wbuf[64];
static u8 at45_flash_rbuf[64];

static void AT45DB321_Test(void)
{
	u32 ret,w_addr,r_addr,i,j;
	for(i=0;i<16384;i++)
	{
		w_addr=CN_WR_SIZE*i;
		r_addr=w_addr;
		for(j=0;j<CN_WR_SIZE;j++)
		{
			at45_flash_wbuf[j]=j;
		}
		//每次读写不能超过页大小  一页为64bytes
		extern u32 AT45_FLASH_Write(u32 Address,u8 *data,u32 data_len);
	    ret=AT45_FLASH_Write(w_addr, at45_flash_wbuf, CN_WR_SIZE);
	    if(!ret)
	    {
	    	printf("AT45DB321 flash write error.\r\n");
	    	return;
	    }
	    else
	    {
	    	extern u32 AT45_FLASH_Read(u32 Address,u8 *data,u32 data_len);
	    	ret=AT45_FLASH_Read(r_addr,at45_flash_rbuf,CN_WR_SIZE);
	    	if(!ret)
	    	{
	    		printf("AT45DB321 flash read error.\r\n");
	    	    return;
	    	}
	    	for(j=0;j<CN_WR_SIZE;j++)
	    	{
	    		if(at45_flash_rbuf[j]!=at45_flash_wbuf[j])
	    		{
	    			printf("AT45DB321 flash test error,cnt:0x%08x.\r\n",i);
	    			while(1);
	    		}
	    	}
	    }
	    Djy_EventDelay(5000);
	    if(i%50==0)
	    {
	    	printf("Success Cnt:0x%08x.\r\n",i);
	    }
	}
	printf("AT45DB321 flash test success.\r\n");
}
#endif




static void NAND_Test(void)
{

}
static void AUDIO_Test(void)
{

}
static void Fs_Test(void)
{

}

extern uint32_t  ADC_GetVDD5Volt(void);
extern uint32_t AD_GetVDD12Volt(void);
extern uint32_t ADC_GetBatVolt(void);
static void ADC_Test(void)
{

}



void HardDrv_Test(void)
{
	BSP_Test_Shell_Module_Install();
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
		  AT45DB321_Test();
	  }
	  if(s_bAudioFlag)
	  {
		  AUDIO_Test();
	  }
	  if(s_bCanFlag)
	  {
		  CAN_Test();
	  }
	  if(s_bFsFlag)
	  {
		  Fs_Test();
	  }
	  Djy_EventDelay(5000*mS);
	}

}
