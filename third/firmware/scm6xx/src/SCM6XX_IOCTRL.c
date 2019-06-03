/***************************************************************************
�������ܽŷ����ļ�
ƽ̨��SCM6XX
ʱ�䣺2018-1-15
���ߣ�������                                                 
****************************************************************************/
#include "SCM6XX_IOCTRL.h"


/********************************************************
 Function:		MII0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪMII0  
********************************************************/
void MII0_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_MII0_GPIO3133;
	  SGCC_IOCTRL_P->IO_CTRL2 &= ~(IO_CTRL2_MII0_GPIO4249 | IO_CTRL2_RMII0_GPIO3439) ;
}

/********************************************************
 Function:		RMII0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪRMII0  
********************************************************/
void RMII0_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_RMII0_GPIO28;
	  SGCC_IOCTRL_P->IO_CTRL2 &= ~(IO_CTRL2_MII0_GPIO4249 | IO_CTRL2_RMII0_GPIO3439);
}

/********************************************************
 Function:		MII1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪMII1  
********************************************************/
void MII1_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL2 &= ~(IO_CTRL2_RMII1_GPIO5863 | IO_CTRL2_MII1_GPIO5457);
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~(IO_CTRL3_RMII1_GPIO7071 |IO_CTRL3_MII1_SPI3_GPIO6669 | IO_CTRL3_MII1_GPIO6465);
}

/********************************************************
 Function:		RMII1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪRMII1  
********************************************************/
void RMII1_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL2 &= ~(IO_CTRL2_RMII1_GPIO5863 | IO_CTRL2_MII1_GPIO5457);
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_RMII1_GPIO7071;
}

/********************************************************
 Function:		OTG_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪOTG 
********************************************************/
void OTG_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_DRVVBUS_GPIO25;
	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_OTG_EN;
}

/********************************************************
 Function:		SRAM_EnPad                                                                                                                                                                            
 input:      opt -- ENABLE/DISABLE              
 output:     None                                        
 Returns:    None                                                                                      
 Description: SRAM�ܽŹ���ʹ��/��ʹ��  
********************************************************/
void SRAM_EnPad(FunctionalState opt)
{	  
	  if(opt == ENABLE)
	  {
	      SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_SRAM_EN;    	
	  }
	  else
	  {
	  	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_SRAM_EN;    
	  }
}

/********************************************************
 Function:		MCO_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪMCO 
********************************************************/
void MCO_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_MCO_GPIO111;
	  
}

/********************************************************
 Function:		PWM0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪPWM0 
********************************************************/
void PWM0_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_PWM0_GPIO29;
	  
}

/********************************************************
 Function:		PWM1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪPWM1 
********************************************************/
void PWM1_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_PWM1_GPIO30;
	  
}

/********************************************************
 Function:		PWM2_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪPWM2 
********************************************************/
void PWM2_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_PWM2_GPIO76;
	  
}

/********************************************************
 Function:		PWM3_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪPWM3 
********************************************************/
void PWM3_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_PWM3_GPIO77;
	  
}

/********************************************************
 Function:		UART0_EnIrda                                                                                                                                                                            
 input:      opt -- ENABLE/DISABLE              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ����/���������⹦�� 
********************************************************/
void UART0_EnIrda(FunctionalState opt)
{	  
	  if(opt == ENABLE)
	  {
	     	SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_IRDA_EN;
	  }
	  else
	  {
	  	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_IRDA_EN;
	  }
	  
}

/********************************************************
 Function:		UART0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪUART0 
********************************************************/
void UART0_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_UART0_GPIO7475;
}

/********************************************************
 Function:		UART1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪUART1 
********************************************************/
void UART1_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_UART1_GPIO2122;
}

/********************************************************
 Function:		UART2_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪUART2
********************************************************/
void UART2_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_UART2_GPIO;
	  
}

/********************************************************
 Function:		UART3_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪUART3 
********************************************************/
void UART3_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_UART3_GPIO2324;
	  
}

/********************************************************
 Function:		UART4_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪUART4 
********************************************************/
void UART4_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_UART4_GPIO2627;
	  
}

/********************************************************
 Function:		UART5_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪUART5 
********************************************************/
void UART5_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_UART5_GPIO9495;
	  
}

/********************************************************
 Function:		UART6_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪUART6 
********************************************************/
void UART6_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_UART6_GPIO8485;
	  
}

/********************************************************
 Function:		UART7_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪUART7 
********************************************************/
void UART7_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_UART7_CAN1_GPIO8889;
	  
}

/********************************************************
 Function:		SPI0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪSPI0 
********************************************************/
void SPI0_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_SPI0_SCI0_GPIO7879;
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_SPI0_GPIO8081;
}

/********************************************************
 Function:		SPI1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪSPI1 
********************************************************/
void SPI1_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL2 &= ~IO_CTRL2_SPI1_SCI1_GPIO5053;
	  
}

/********************************************************
 Function:		SPI2_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪSPI2 
********************************************************/
void SPI2_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_SPI2_GPIO9093;
	  
}

/********************************************************
 Function:		SPI3_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪSPI3 
********************************************************/
void SPI3_InitPad(FunctionalState opt)
{	  
	  if(opt == 0)
	  {
	      SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_SPI3_GPIO1720;
	  }
	  else if(opt == 1)
	  {
	  	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_MII1_SPI3_GPIO6669;
	  	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_MII1_SPI3_GPIO6669_SPI3;
	  }
	  
}

/********************************************************
 Function:		I2C0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪI2C0 
********************************************************/
void I2C0_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_I2C0_GPIO8687;
}

/********************************************************
 Function:		I2C1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪI2C1 
********************************************************/
void I2C1_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~(IO_CTRL3_I2C1_SDA_SCI0_GPIO73 | IO_CTRL3_I2C1_SCL_GPIO72);
}

/********************************************************
 Function:		I2C2_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪI2C2 
********************************************************/
void I2C2_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL2 &= ~IO_CTRL2_I2C2_GPIO4041;
}

/********************************************************
 Function:		SCI0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪSCI0 
********************************************************/
void SCI0_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~(IO_CTRL3_SPI0_SCI0_GPIO7879 | IO_CTRL3_I2C1_SDA_SCI0_GPIO73);
	  SGCC_IOCTRL_P->IO_CTRL3 |= (IO_CTRL3_SPI0_SCI0_GPIO7879_SCI0 | IO_CTRL3_I2C1_SDA_SCI0_GPIO73_SCI0);
}


/********************************************************
 Function:		SCI1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪSCI1 
********************************************************/
void SCI1_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL2 &= ~IO_CTRL2_SPI1_SCI1_GPIO5053;
	  SGCC_IOCTRL_P->IO_CTRL2 |= IO_CTRL2_SPI1_SCI1_GPIO5053_SCI1;
}


/********************************************************
 Function:		CAN0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪCAN0_TX/CAN0_RX
********************************************************/
void CAN0_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_GPIO1516_CAN;
}

/********************************************************
 Function:		CAN1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪCAN1_TX/CAN1_RX
********************************************************/
void CAN1_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_UART7_CAN1_GPIO8889;
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_UART7_CAN1_GPIO8889_CAN1;
}

/********************************************************
 Function:		EXTAL0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪEXTAL0
********************************************************/
void EXTAL0_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_EXTAL0_CLKIN_GPIO112;
}

/********************************************************
 Function:		XTAL0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪXTAL0
********************************************************/
void XTAL0_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_XTAL0_GPIO113;
}

/********************************************************
 Function:		EXTAL32_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪEXTAL32
********************************************************/
void EXTAL32_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_EXTAL32_32KCLKIN_GPIO110;
}

/********************************************************
 Function:		XTAL32_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪXTAL32
********************************************************/
void XTAL32_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_XTAL32_GPIO109;
}

/********************************************************
 Function:		CLKIN_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪCLKIN
********************************************************/
void CLKIN_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_EXTAL0_CLKIN_GPIO112;
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_EXTAL0_CLKIN_GPIO112_CLKIN;
}

/********************************************************
 Function:		CLKIN32K_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����Ϊ32KCLKIN
********************************************************/
void CLKIN32K_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_EXTAL32_32KCLKIN_GPIO110;
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_EXTAL32_32KCLKIN_GPIO110_32KCLKIN;
}

/********************************************************
 Function:		JTAG_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪJTAG
********************************************************/
void JTAG_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_JTAG_GPIO346;
}

/********************************************************
 Function:		GPIO346_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO3/4/6
********************************************************/
void GPIO346_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_JTAG_GPIO346;
}

/********************************************************
 Function:		GPIO1516_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO15~16
********************************************************/
void GPIO1516_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_GPIO1516_CAN;
}

/********************************************************
 Function:		GPIO1720_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO17~20
********************************************************/
void GPIO1720_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_SPI3_GPIO1720;
}


/********************************************************
 Function:		GPIO2122_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO21/22
********************************************************/
void GPIO2122_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_UART1_GPIO2122;
}

/********************************************************
 Function:		GPIO2324_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO23/24
********************************************************/
void GPIO2324_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_UART3_GPIO2324;
}

/********************************************************
 Function:		GPIO25_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO25
********************************************************/
void GPIO25_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_DRVVBUS_GPIO25;
}

/********************************************************
 Function:		GPIO2627_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO26/GPIO27
********************************************************/
void GPIO2627_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_UART4_GPIO2627;
}


/********************************************************
 Function:		GPIO28_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO28
********************************************************/
void GPIO28_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_RMII0_GPIO28;
}

/********************************************************
 Function:		GPIO29_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO29
********************************************************/
void GPIO29_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_PWM0_GPIO29;
}


/********************************************************
 Function:		GPIO30_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO30 
********************************************************/
void GPIO30_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_PWM1_GPIO30;
}

/********************************************************
 Function:		GPIO3133_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO31/GPIO32/GPIO33 
********************************************************/
void GPIO3133_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_MII0_GPIO3133;
}

/********************************************************
 Function:		GPIO3439_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO34~39
********************************************************/
void GPIO3439_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL2 |= IO_CTRL2_RMII0_GPIO3439;
}

/********************************************************
 Function:		GPIO4041_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO40~41 
********************************************************/
void GPIO4041_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL2 |= IO_CTRL2_I2C2_GPIO4041;
}

/********************************************************
 Function:		GPIO4249_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO42~49 
********************************************************/
void GPIO4249_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL2 |= IO_CTRL2_MII0_GPIO4249;
}

/********************************************************
 Function:		GPIO5053_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO50/GPIO51/GPIO52/GPIO53 
********************************************************/
void GPIO5053_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL2 |= IO_CTRL2_SPI1_SCI1_GPIO5053;
}

/********************************************************
 Function:		GPIO5457_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO54~57
********************************************************/
void GPIO5457_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL2 |= IO_CTRL2_MII1_GPIO5457;
}


/********************************************************
 Function:		GPIO5863_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO58~63
********************************************************/
void GPIO5863_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL2 |= IO_CTRL2_RMII1_GPIO5863;
}

/********************************************************
 Function:		GPIO6465_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO64~65
********************************************************/
void GPIO6465_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_MII1_GPIO6465;
}

/********************************************************
 Function:		GPIO6669_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO66~GPIO69
********************************************************/
void GPIO6669_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_MII1_SPI3_GPIO6669;
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_MII1_SPI3_GPIO6669_GPIO;
}

/********************************************************
 Function:		GPIO7071_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO70/GPIO71
********************************************************/
void GPIO7071_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_RMII1_GPIO7071;
}

/********************************************************
 Function:		GPIO72_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO72
********************************************************/
void GPIO72_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_I2C1_SCL_GPIO72_GPIO;
}

/********************************************************
 Function:		GPIO73_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO73
********************************************************/
void GPIO73_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_I2C1_SDA_SCI0_GPIO73;
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_I2C1_SDA_SCI0_GPIO73_GPIO;
}

/********************************************************
 Function:		GPIO7475_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO74/GPIO75
********************************************************/
void GPIO7475_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_UART0_GPIO7475;
}

/********************************************************
 Function:		GPIO76_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO76
********************************************************/
void GPIO76_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_PWM2_GPIO76;
}

/********************************************************
 Function:		GPIO77_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO77
********************************************************/
void GPIO77_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_PWM3_GPIO77;
}

/********************************************************
 Function:		GPIO7879_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO78~79
********************************************************/
void GPIO7879_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_SPI0_SCI0_GPIO7879;
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_SPI0_SCI0_GPIO7879_GPIO;
}

/********************************************************
 Function:		GPIO8081_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO80~81
********************************************************/
void GPIO8283_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_UART2_GPIO_GPIO8283;
}

/********************************************************
 Function:		GPIO8081_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO80~81
********************************************************/
void GPIO8081_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_SPI0_GPIO8081;
}

/********************************************************
 Function:		GPIO8485_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO84~85
********************************************************/
void GPIO8485_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_UART6_GPIO8485;
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_UART6_GPIO8485_GPIO;
}

/********************************************************
 Function:		GPIO8687_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO86~87
********************************************************/
void GPIO8687_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_I2C0_GPIO8687;
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_I2C0_GPIO8687_GPIO;
}

/********************************************************
 Function:		GPIO8889_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO88~89
********************************************************/
void GPIO8889_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 &= ~IO_CTRL3_UART7_CAN1_GPIO8889;
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_UART7_CAN1_GPIO8889_GPIO;
}

/********************************************************
 Function:		GPIO9093_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO90~93
********************************************************/
void GPIO9093_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_SPI2_GPIO9093;
}

/********************************************************
 Function:		GPIO9495_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO94~95
********************************************************/
void GPIO9495_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL3 |= IO_CTRL3_UART5_GPIO9495;
}

/********************************************************
 Function:		GPIO109_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO109
********************************************************/
void GPIO109_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_XTAL32_GPIO109;
}

/********************************************************
 Function:		GPIO110_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO110 
********************************************************/
void GPIO110_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_EXTAL32_32KCLKIN_GPIO110;
}

/********************************************************
 Function:		GPIO111_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO111 
********************************************************/
void GPIO111_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_MCO_GPIO111;
}

/********************************************************
 Function:		GPIO112_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO112 
********************************************************/
void GPIO112_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_EXTAL0_CLKIN_GPIO112;
}

/********************************************************
 Function:		GPIO113_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ��عܽ�����ΪGPIO113 
********************************************************/
void GPIO113_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_XTAL0_GPIO113;
}

/********************************************************
 Function:		ANA_EnTest                                                                                                                                                                            
 input:      opt: ENABLE/DISABLE              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ����ģ���źŲ���ʹ��
********************************************************/
void ANA_EnTest(FunctionalState opt)
{	  
	  if(opt == ENABLE)
	  {
	      SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_ANA_TEST_EN;
	  }
	  else
	  {
	  	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_ANA_TEST_EN;
	  }
}

/********************************************************
 Function:		ADC_EnTest                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ADC����ʹ��
********************************************************/
void ADC_EnTest(FunctionalState opt)
{	  
	  if(opt == ENABLE)
	  {
	      SGCC_IOCTRL_P->IO_CTRL1 |= IO_CTRL1_ADC_TEST_EN;
	  }
	  else
	  {
	  	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_ADC_TEST_EN;
	  }
}

/********************************************************
 Function:		SPI0_ConfigDS                                                                                                                                                                            
 input:      ds - IO_DS_SPI0_4P5/IO_DS_SPI0_9/IO_DS_SPI0_13P5/IO_DS_SPI0_18              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ����SPI0��عܽŵ����������С
********************************************************/
void SPI0_ConfigDS(UINT32 SPI0_Ds)
{
    SGCC_IOCTRL_P->IO_DS &= ~IO_DS_SPI0;    	  
	  SGCC_IOCTRL_P->IO_DS |= SPI0_Ds;
}

/********************************************************
 Function:		SPI1_ConfigDS                                                                                                                                                                            
 input:      ds - IO_DS_SPI1_4P5/IO_DS_SPI1_9/IO_DS_SPI1_13P5/IO_DS_SPI1_18              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ����SPI0��عܽŵ����������С
********************************************************/
void SPI1_ConfigDS(UINT32 SPI1_Ds)
{
    SGCC_IOCTRL_P->IO_DS &= ~IO_DS_SPI1;    	  
	  SGCC_IOCTRL_P->IO_DS |= SPI1_Ds;
}

/********************************************************
 Function:		SPI2_ConfigDS                                                                                                                                                                            
 input:      ds - IO_DS_SPI2_4P5/IO_DS_SPI2_9/IO_DS_SPI2_13P5/IO_DS_SPI2_18              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ����SPI2��عܽŵ����������С
********************************************************/
void SPI2_ConfigDS(UINT32 SPI2_Ds)
{
    SGCC_IOCTRL_P->IO_DS &= ~IO_DS_SPI2;    	  
	  SGCC_IOCTRL_P->IO_DS |= SPI2_Ds;
}

/********************************************************
 Function:		SPI3_ConfigDS                                                                                                                                                                            
 input:      ds - IO_DS_SPI3_4P5/IO_DS_SPI3_9/IO_DS_SPI3_13P5/IO_DS_SPI3_18              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ����SPI3��عܽŵ����������С
********************************************************/
void SPI3_ConfigDS(UINT32 SPI3_Ds)
{
    SGCC_IOCTRL_P->IO_DS &= ~IO_DS2_SPI3;    	  
	SGCC_IOCTRL_P->IO_DS |= SPI3_Ds;
}

/********************************************************
 Function:		MII0_ConfigDS                                                                                                                                                                            
 input:      ds - IO_DS_MII0_4P5/IO_DS_MII0_9/IO_DS_MII0_13P5/IO_DS_MII0_18              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ����MII0/RMII0��عܽŵ����������С
********************************************************/
void MII0_ConfigDS(UINT32 MII0_Ds)
{
    SGCC_IOCTRL_P->IO_DS &= ~IO_DS_MII0;    	  
	  SGCC_IOCTRL_P->IO_DS |= MII0_Ds;
}

/********************************************************
 Function:		MII1_ConfigDS                                                                                                                                                                            
 input:      ds - IO_DS_MII1_4P5/IO_DS_MII1_9/IO_DS_MII1_13P5/IO_DS_MII1_18              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ����MII1/RMII1��عܽŵ����������С
********************************************************/
void MII1_ConfigDS(UINT32 MII1_Ds)
{
    SGCC_IOCTRL_P->IO_DS &= ~IO_DS_MII1;    	  
	  SGCC_IOCTRL_P->IO_DS |= MII1_Ds;
}

/********************************************************
 Function:		IO_ConfigPullUp                                                                                                                                                                            
 input:      ioNo - P_GPIO0~P_UART5_RX
             opt - ENABLE/DISABLE            
 output:     None                                        
 Returns:    None                                                                                      
 Description: ����IO�ܽŵ���������ʹ��/��ʹ��
********************************************************/
void IO_Configure_PullUp(UINT8 IO_No, FunctionalState opt)
{
    if(IO_No < 32)
    {
    	  if(opt == ENABLE)
            SGCC_IOCTRL_P->IO_PU1 |= (1 << IO_No);
        else
        	  SGCC_IOCTRL_P->IO_PU1 &= ~(1 << IO_No);    	
    }
    else if(IO_No < 64)
    {
    	  if(opt == ENABLE)
            SGCC_IOCTRL_P->IO_PU2 |= (1 << (IO_No - 32));
        else
        	  SGCC_IOCTRL_P->IO_PU2 &= ~(1 << (IO_No - 32)); 
        	  
    }
    else if(IO_No < 96)
    {
    	  if(opt == ENABLE)
            SGCC_IOCTRL_P->IO_PU3 |= (1 << (IO_No - 64));
        else
        	  SGCC_IOCTRL_P->IO_PU3 &= ~(1 << (IO_No - 64)); 
    }
    else
    {
    	  if(opt == ENABLE)
            SGCC_IOCTRL_P->IO_PU4 |= (1 << (IO_No - 96));
        else
        	  SGCC_IOCTRL_P->IO_PU4 &= ~(1 << (IO_No - 96)); 
    }
}

/********************************************************
 Function:		IO_ConfigPullDown                                                                                                                                                                            
 input:      ioNo - P_GPIO0~P_UART5_RX
             opt - ENABLE/DISABLE            
 output:     None                                        
 Returns:    None                                                                                      
 Description: ����IO�ܽŵ���������ʹ��/��ʹ��
********************************************************/
void IO_Configure_PullDown(UINT8 IO_No, FunctionalState opt)
{
    if(IO_No < 32)
    {
    	  if(opt == ENABLE)
            SGCC_IOCTRL_P->IO_PD1 |= (1 << IO_No);
        else
        	  SGCC_IOCTRL_P->IO_PD1 &= ~(1 << IO_No);    	
    }
    else if(IO_No < 64)
    {
    	  if(opt == ENABLE)
            SGCC_IOCTRL_P->IO_PD2 |= (1 << (IO_No - 32));
        else
        	  SGCC_IOCTRL_P->IO_PD2 &= ~(1 << (IO_No - 32)); 
        	  
    }
    else if(IO_No < 96)
    {
    	  if(opt == ENABLE)
            SGCC_IOCTRL_P->IO_PD3 |= (1 << (IO_No - 64));
        else
        	  SGCC_IOCTRL_P->IO_PD3 &= ~(1 << (IO_No - 64)); 
    }
    else
    {
    	  if(opt == ENABLE)
            SGCC_IOCTRL_P->IO_PD4 |= (1 << (IO_No - 96));
        else
        	  SGCC_IOCTRL_P->IO_PD4 &= ~(1 << (IO_No - 96)); 
    }
}
