#include "hal_w25q64.h"

SGCC_SPI_TypeDef * SPI3x;
FLASH_STATUS_REG_TypeDef g_flash_reg;


FLASH_ID_TypeDef flash_id[] = {
	{"W25X25",      0xEF3011},
	{"W25Q32D",     0xEF4016},
	{"W25Q16D",  		0xEF4015},
	{"W25Q64D",  		0xEF4017},
};

uint8_t flash_read_status_reg(void)
{
  uint8_t result=0xff;
	hal_w25q64_start();	
  hal_w25q64_send_byte(FLASH_READ_STATUS_1_REG);
  result=hal_w25q64_rec_byte();
	hal_w25q64_stop();
	return result;
}


void flash_wait_ready(void)
{
	FLASH_STATUS_REG_TypeDef flash_reg;
	do{
			flash_reg.reg_val=flash_read_status_reg();
   }while(flash_reg.bits.BUSY);
}


void flash_write_status_reg(uint8_t reg_val)
{
	flash_wait_ready();
  flash_write_enable();
	hal_w25q64_start();	
	hal_w25q64_send_byte(FLASH_WRITE_STATUS_REG);
	hal_w25q64_send_byte(reg_val);
	hal_w25q64_stop();
	flash_write_disable();
}


void flash_write_enable(void)
{
	hal_w25q64_start();	
  hal_w25q64_send_byte(FLASH_WRITE_ENABLE);
	hal_w25q64_stop();
}

void flash_write_disable(void)
{
	hal_w25q64_start();	
  hal_w25q64_send_byte(FLASH_WRITE_DISABLE);
	hal_w25q64_stop();
}


void flash_protect(uint8_t protect_opt)
{
	flash_write_status_reg(protect_opt);
}

void flash_write_byte(uint32_t addr,uint8_t val)
{
	flash_wait_ready();
	flash_write_enable();
	hal_w25q64_start();	
  hal_w25q64_send_byte(FLASH_PAGE_PROGRAM);
	hal_w25q64_send_byte((addr&0xff0000)>>16);
	hal_w25q64_send_byte((addr&0xff00)>>8);
	hal_w25q64_send_byte(addr);
	hal_w25q64_send_byte(val);
	hal_w25q64_stop();
}

uint8_t flash_read_byte(uint32_t addr)
{
	uint8_t result;
	flash_wait_ready();
	hal_w25q64_start();	
  hal_w25q64_send_byte(FLASH_READ_DATA);
	hal_w25q64_send_byte((addr&0xff0000)>>16);
	hal_w25q64_send_byte((addr&0xff00)>>8);
	hal_w25q64_send_byte(addr);
	result=hal_w25q64_rec_byte();
	hal_w25q64_stop();
	return result;
}


void flash_write_bytes(uint32_t addr,uint8_t *buf, uint8_t len)
{
	uint8_t i;
	flash_wait_ready();
	flash_write_enable();
	hal_w25q64_start();	
    hal_w25q64_send_byte(FLASH_PAGE_PROGRAM);
	hal_w25q64_send_byte((addr&0xff0000)>>16);
	hal_w25q64_send_byte((addr&0xff00)>>8);
	hal_w25q64_send_byte(addr);
	for(i=0;i<len;i++)
	{
		hal_w25q64_send_byte((*buf+i));
  }
	hal_w25q64_stop();
}


void flash_read_bytes(uint32_t addr,uint8_t *buf,uint32_t len)
{
	uint16_t i;
	flash_wait_ready();
	hal_w25q64_start();	
  hal_w25q64_send_byte(FLASH_READ_DATA);
	hal_w25q64_send_byte((addr&0xff0000)>>16);
	hal_w25q64_send_byte((addr&0xff00)>>8);
	hal_w25q64_send_byte(addr);
	for(i=0;i<len;i++)
	{
		*(buf+i)=hal_w25q64_rec_byte();
  }
	hal_w25q64_stop();
}


void flash_erase(uint8_t cmd,u32 addr)
{
	uint32_t address;
//	va_list arg_ptr;
	flash_wait_ready();
	flash_write_enable();
	hal_w25q64_start();	
	hal_w25q64_send_byte(cmd);
	switch(cmd)
	{
		case FLASH_SECTOR_ERASE:
    case FLASH_BLOCK_32K_ERASE:
    case FLASH_BLOCK_64K_ERASE:{
//			                        va_start(arg_ptr,cmd);
//			                        addr=va_arg(arg_ptr,unsigned int);
//			                        va_end(arg_ptr);
			                        hal_w25q64_send_byte((addr&0xff0000)>>16);
	                                hal_w25q64_send_byte((addr&0xff00)>>8);
	                                hal_w25q64_send_byte(addr);
                                  }break;
	  case FLASH_CHIP_ERASE:break;
		default: break;
  }
	hal_w25q64_stop();	
}


uint32_t flash_read_jedec_id(void)
{
	uint8_t i;
	uint32_t jedec_id=0x00;
	flash_wait_ready();
	hal_w25q64_start();	
	hal_w25q64_send_byte(FLASH_READ_JEDEC_ID);
	for(i=3; i>0; i--)
	{								
		jedec_id <<=8;
		jedec_id |= hal_w25q64_rec_byte();
	} 
	hal_w25q64_stop();
  return jedec_id;
}


void flash_read_device_id(uint8_t *device_id)
{
	uint8_t i;
	flash_wait_ready();
	hal_w25q64_start();	
	hal_w25q64_send_byte(FLASH_READ_UNIQUE_ID);
	for(i=0;i<4;i++)
	{
		hal_w25q64_send_byte(0xFF);
	}
	for(i=8;i>0;i--)
	{
		device_id[i-1]=hal_w25q64_rec_byte();
	}
	hal_w25q64_stop();
}


void hal_w25q64_cs(uint8_t opt)
{
//	if(opt == ENABLE)
//	{
//	 //GPIO_WrDataOut(FLASH_WRITE_PROTECT_IO,GPIO_HIGH);
//	  GPIO_WrDataOut(FLASH_CS_IO,GPIO_LOW);
//  }else
//	{
//	  //GPIO_WrDataOut(FLASH_WRITE_PROTECT_IO,GPIO_LOW);
//	  GPIO_WrDataOut(FLASH_CS_IO,GPIO_HIGH);
//  }
}


void hal_w25q64_init(void)
{
//	SCU_EnPeriClk(SPI1);
//	GPIO6_InitPad();  //CS
//	GPIO7_InitPad();  //WP
//	GPIO_ConfigDir(FLASH_WRITE_PROTECT_IO,GPIO_OUTPUT);
//	GPIO_ConfigDir(FLASH_CS_IO,GPIO_OUTPUT);
//	GPIO_WrDataOut(FLASH_WRITE_PROTECT_IO,GPIO_HIGH);
//	hal_w25q64_cs(DISABLE);
//	SGCC_PINCTRL_P->IO_DS |=(1<<1);
//	SPI1_InitPad();
//	SPI_ResetReg(SGCC_SPI1_P);
}


void hal_w25q64_start(void)
{
//	SPI_MASTER initMaster;
//	initMaster.rxEn = SPI_CR1_R_EN;
//	initMaster.txEn = SPI_CR1_T_EN;
//	initMaster.sckDiv = 1;
//	initMaster.mode = SPI_CR1_SCK_MOD0;
//	initMaster.txFifoLvl = SPI_CR2_TTHD_8;
//	initMaster.rxFifoLvl = SPI_CR2_RTHD_0;
//	initMaster.DataNumEn = SPI_CR3_DNUM_DIS;
//	initMaster.DataNum = 0;
//	initMaster.waitNum = 0;
//	SPI_InitMaster(SGCC_SPI1_P, &initMaster ); 
//	SPI_StartTransferAsMaster(SGCC_SPI1_P);
//	hal_w25q64_cs(ENABLE);
	
	SPI_MASTER initMaster;
	
	SPI3_InitPad(DISABLE);
	SPI3x = (SGCC_SPI_TypeDef*)SGCC_SPI3_P;
  SPI_Reset_Reg(SPI3x);
    
	initMaster.rxEn = SPI_CR1_R_EN;
	initMaster.txEn = SPI_CR1_T_EN;
	initMaster.sckDiv = 10;
	initMaster.mode = SPI_CR1_SCK_MOD1;
	initMaster.txFifoLvl = SPI_CR2_TTHD_8;
	initMaster.rxFifoLvl = SPI_CR2_RTHD_0;
	initMaster.DataNumEn = SPI_CR3_DNUM_DIS;
	initMaster.DataNum = 0;
	initMaster.waitNum = 0;
	 
	SPI_Init_Master(SPI3x, &initMaster );
	SPI_Drive_SSN(SPI3x,ENABLE);
  SPI_Start_Transfer_As_Master(SPI3x);
}


void hal_w25q64_stop(void)
{
	hal_w25q64_cs(DISABLE);
//	SPI_StopTransferAsMaster(SGCC_SPI1_P);
	SPI_Stop_Transfer_As_Master(SPI3x);
}


void hal_w25q64_send_byte(uint8_t val)
{
//	SPI_WriteByte(SGCC_SPI1_P,val);
//	SPI_RdByte(SGCC_SPI1_P);
	SPI_Write_Byte(SPI3x,val);
	SPI_Read_Byte(SPI3x);
}


uint8_t hal_w25q64_rec_byte(void)
{
	uint8_t result;
	SPI_Write_Byte(SPI3x,0xff);
	result=SPI_Read_Byte(SPI3x);
//	SPI_WriteByte(SGCC_SPI1_P,0xff);
//	result=SPI_RdByte(SGCC_SPI1_P);
	return result;
}




