/**
  ******************************************************************************
  * @file    system_pt32z192xx.c
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/04/28
  * @brief   
  *          
  *        
  *          
  *
  *
  *
  *
  *
  * 
  *=============================================================================
  *=============================================================================
  *        System Clock source                    | HSIRC
  *-----------------------------------------------------------------------------
  *        SYSCLK(Hz)                             | 24000000
  *-----------------------------------------------------------------------------
  *        HSCLK(Hz)                              | 24000000
  *-----------------------------------------------------------------------------
  *        HCLK Prescaler                         | 1
  *-----------------------------------------------------------------------------
  *        PCLK Prescaler                         | 1
  *-----------------------------------------------------------------------------
  *        HCLK Frequency(Hz)                     | 24000000
  *----------------------------------------------------------------------------
  *        PCLK Frequency(Hz)                     | 24000000
  *-----------------------------------------------------------------------------
  *        Flash Latency(WS)                      | 1
  *-----------------------------------------------------------------------------                                                                                               
  ******************************************************************************                 
  * @attention
  *
  *  
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS                                    
  * @{                                                   
  */                                                     
                                                         
/** @addtogroup pt32z192xx_system                         
  * @{                                                   
  */                                                     
                                                         
/** @addtogroup PT32F0xx_System_Private_Includes        
  * @{                                                   
  */                                                     
                                                         
#include "pt32z192xx.h"                                   
                                                         
/**                                                      
  * @}                                                   
  */                                                     
                                                         
/** @addtogroup PT32F0xx_System_Private_TypesDefinitions
  * @{                                                   
  */                                                     
                                                         
/**                                                      
  * @}                                                   
  */                                                     
                                                         
/** @addtogroup PT32F0xx_System_Private_Defines         
  * @{                                                   
  */    
	
/**                                                      
  * @}                                                   
  */ 
                                                        
/** @addtogroup PT32F0xx_System_Private_Macros          
  * @{                                                   
  */ 

//<<< Use Configuration Wizard in Context Menu >>>

/*--------------------- Clock Configuration ----------------------------------
//
//    <h> High Speed Internal RC Oscillator Configuration (24M)
//         <q0>    IOSC_24M Enable
//                <i> Default = ENABLE
//    </h>
//
//    <h> High Speed External Crystal Oscillator Configuration (4~25M)
//         <q1>    EOSC_CLK Enable
//                <i> Default = DISABLE
//         <o2>    EOSC_CLK_frequency (MHz)
//                <4-25>
//    </h>
//
//    <h> Low Speed External RC Oscillator Configuration (32.768K)
//         <q3>     EOSC_32K_Enable
//                <i> Defaul = ENABLE
//    </h> PLL Clock Configuration
//      <e4> PLL_CLK_Enable
//                <i> Default = ENABLE
//                <i> Output up to 160M high speed clock
//                <i> 2MHz<PLL Clock Source/PLL DIV PRE<30MHz
//         <o5> PLL Clock Source
//                <0=> Internal RC_24M
//                <1=> External Clock
//         <o6> PLL DIV LOOP (1~255)
//                <1-255:1>
//         <o7> PLL DIV OUT (1~63)
//                <1-63:1>
//         <o8> PLL DIV PRE (1~63)
//                <1-63:1>
//    </e>
//
//    <h> SystemCoreClock Configuration 
//      <o9>    SystemCoreClock Source
//                <0=> Internal RC_24M
//                <1=> External Clock
//                <2=> PLL Clock
//                <i> Default SystemCoreClock source = Internal RC_24M
//      <q10>    PCLK Divider Enable
//                <i> Default = DISABLE
//      <o11>    PCLK Divider (1~7)
//                <0-7:1> 
//                <i> pclk= SystemCoreClock/Divider+1
//    </h>
//
//    <h> FLASH Configuration
//      <o12>   Wait state
//                <0=> 0 WS
//                <1=> 1 WS
//                <2=> 2 WS
//                <3=> 3 WS
//                <4=> 4 WS
//                <5=> 5 WS
//                <6=> 6 WS
//                <7=> 7 WS
//                <i> 0 WS:  1 KHz  <=  HCLK < 20 MHz
//                <i> 1 WS: 20 MHz  <=  HCLK < 40 MHz
//                <i> 2 WS: 40 MHz  <=  HCLK < 60 MHz
//                <i> 3 WS: 60 MHz  <=  HCLK < 80 MHz
//                <i> 4 WS: 80 MHz  <=  HCLK < 100 MHz
//                <i> 5 WS: 100 MHz <=  HCLK < 120 MHz
//                <i> 6 WS: 120 MHz <=  HCLK < 140 MHz
//                <i> 7 WS: 140 MHz <=  HCLK < 160 MHz
//    </h>
*/

#define IOSC_24M_ENABLE               (1)     /*!< 0: DISABLE,  1: ENABLE                                        */
#define EOSC_CLK_ENABLE               (0)     /*!< 0: DISABLE,  1: ENABLE                                        */  
#define EOSC_CLK_frequency            (24)     /*!< External crystal frequency                                        */ 
#define EOSC_32K_ENABLE               (0)     /*!< 0: DISABLE,  1: ENABLE                                        */      
#define PLL_CLK_ENABLE                (1)     /*!< 0: DISABLE,  1: ENABLE                                        */     
#define PLL_CLK_SRC                   (0)     /*!< 0: IOCS_24M, 1: EXT_CLK                                       */
#define PLL_DIV_LOOP                  (160)    /*!< 1~255                                                         */
#define PLL_DIV_OUT                   (1)     /*!< 1~63                                                          */
#define PLL_DIV_PRE                   (24)     /*!< 1~63                                                          */
#define HCLK_SRC                      (2)     /*!< 0: IOCS_24M, 1: EXT_CLK,  2:PLL_CLK                           */
#define PCLK_DIV_ENABLE               (0)     /*!< 0: DISABLE,  1: ENABLE                                        */
#define PCLK_DIV                      (0)     /*!< 1~7                                                           */  
#define WAIT_STATE                    (7)     /*!< 0: 0 WS, 1: 1 WS, 2: 2 WS, 3:3WS, 4:4WS, 5:5WS, 6:6WS, 7:7WS  */

                                                 
/** @addtogroup PT32F0xx_System_Private_Variables       
  * @{                                                   
  */     
uint32_t SystemCoreClock = 160000000;
/**
  * @}
  */

/** @addtogroup STM32F0xx_System_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */
	
/** @addtogroup STM32F0xx_System_Private_Functions
  * @{
  */
#define  IOSC_24M_VALUE   24000000UL
#define  EOSC_32K_VALUE   32768UL
#define  EOSC_25M_VALUE   24000000UL;         //外部crystal频率
int  VCO_VALUE = IOSC_24M_VALUE;

/**
  * @brief         
  * @param  None
  * @retval None
  */	

// Delay in millisec
void Wait(unsigned int delay)
{
	// Default clock 24MHz, 1 ms ~ 24000 cycles
	// Each loop takes ~ 10 cycles
	volatile unsigned int loop = 2400 * delay;

	while(loop > 0)
		loop--;
}


/**
  * @brief  
  *         
  *         
  * @param  None
  * @retval None
  */

void SystemInit (void)
{
#ifdef PROTO_192
	while(GetBit_BB(0x40010000,4));
#endif

	// Wait for a while (~500ms) for circuit to stabilize
	Wait(500);

#if (EOSC_CLK_ENABLE == 1)
	
	 ww(0x40015018,5);  //PD0/PD1:crystal
	
	 CMSDK_ANAMIX ->  EOSC_32M_CON |=1 ; //enable crystal

	Wait(50);
	
//check crystal ready 
	
//	SetBit_BB(0x4002100C, 6);    //PLL_CLK_SRC: eosc_clk
//	
//	CMSDK_ANAMIX ->PLL_CONFIG = ((1<<16) | (100<<8) | (24));
//	
//	CMSDK_ANAMIX ->PLL_CON= 0xd21;
//	
//	while(!(CMSDK_ANAMIX ->PLL_CON & 0x80000000));	
//	
//	CMSDK_ANAMIX ->PLL_CONFIG = 0;
//	
//	CMSDK_ANAMIX ->PLL_CON= 0;	
	
#else
	
		CMSDK_ANAMIX ->  EOSC_32M_CON &=0xFFFFFFFE ; //disable crystal

#endif	


#if (EOSC_32K_ENABLE == 1)
	
   CMSDK_GPIO2->ALTFUNCSET |= (1<<28); //PC14/PC15
   CMSDK_GPIO2->ALTFUNCSET |= (1<<30);
	 
	 CMSDK_ANAMIX -> EOSC_32K_CON |=1 ; //enable eosc_32k
	 
		
#else

		CMSDK_ANAMIX -> EOSC_32K_CON &=0xFFFFFFFE ; //disable eosc_32k  	
		
#endif	
	
#if (PLL_CLK_ENABLE == 1) 
	
	#if (PLL_CLK_SRC == 0 )
   #if (IOSC_24M_ENABLE == 0)
		 #error "PLL_CLK clock source issue: IOSC_24M has not been enabled"	
	 #else 
	    ResetBit_BB(0x4002100C, 6);  //PLL_CLK_SRC: iosc_24m
			VCO_VALUE = IOSC_24M_VALUE;
	 #endif
	#endif

	#if (PLL_CLK_SRC == 1 )
	   #if (EOSC_CLK_ENABLE == 0)
		 #error "PLL_CLK clock source issue: EOSC_CLK has not been enabled"
		#else			 
	   SetBit_BB(0x4002100C, 6);    //PLL_CLK_SRC: eosc_clk
		 VCO_VALUE =EOSC_25M_VALUE;
		#endif
	#endif
 

	CMSDK_ANAMIX ->PLL_CONFIG = ((PLL_DIV_OUT<<16) | (PLL_DIV_LOOP<<8) | (PLL_DIV_PRE));
	
	CMSDK_ANAMIX ->PLL_CON= 0xD11;

	
	 #if (60000000<(VCO_VALUE*PLL_DIV_LOOP/PLL_DIV_PRE)<120000000)      //设置VCO档位
				CMSDK_ANAMIX->PLL_CON |= (0x01<<4);
			 	#elif (100000000<(VCO_VALUE*PLL_DIV_LOOP/PLL_DIV_PRE)<200000000)
				CMSDK_ANAMIX->PLL_CON |= (0x02<<4);
			  #elif (160000000<(VCO_VALUE*PLL_DIV_LOOP/PLL_DIV_PRE)<280000000)
				CMSDK_ANAMIX->PLL_CON |= (0x03<<4);
			  #elif (220000000<(VCO_VALUE*PLL_DIV_LOOP/PLL_DIV_PRE)<360000000)
				CMSDK_ANAMIX->PLL_CON |= (0x04<<4);
				#elif (160000000<(VCO_VALUE*PLL_DIV_LOOP/PLL_DIV_PRE)<440000000)
				CMSDK_ANAMIX->PLL_CON |= (0x05<<4);
			  #elif (380000000<(VCO_VALUE*PLL_DIV_LOOP/PLL_DIV_PRE)<520000000)
				CMSDK_ANAMIX->PLL_CON |= (0x06<<4);
			  #elif (460000000<(VCO_VALUE*PLL_DIV_LOOP/PLL_DIV_PRE)<600000000)
				CMSDK_ANAMIX->PLL_CON |= (0x07<<4);
        #endif	
	while(!(CMSDK_ANAMIX ->PLL_CON & 0x80000000));
		
#else

	CMSDK_ANAMIX ->PLL_CON= 0;  	    	
		
#endif

	CMSDK_FLASH -> ACR = ((0xA5A50000) | (WAIT_STATE<<5)|(0x10)) ;  //wait state

#if (HCLK_SRC == 0 )
	 
#if (IOSC_24M_ENABLE == 0)
#error "System clock source issue: IOSC_24M has not been enabled"
#endif
	
#endif

#if (HCLK_SRC == 1 )
	 
#if (EOSC_CLK_ENABLE == 0)
#error "System clock source issue: EOSC_CLK has not been enabled"
 
#endif
 
#endif

#if (HCLK_SRC == 2 )
	 
#if (PLL_CLK_ENABLE == 0)
 
	 #error "System clock source issue: PLL_CLK_ENABLE has not been enabled"

#endif
 
#endif

	Wait(50);
	 
	CMSDK_SYSCON ->SYSCLK = ((CMSDK_SYSCON ->SYSCLK & 0xFFFC) | 0) ;    //HCLK_SROUCE

	Wait(50);
	 
#if (PCLK_DIV_ENABLE == 1) 

	SetBit_BB(0x4002100C, 10);    //PCLK DIV ENABLE
	 CMSDK_SYSCON ->SYSCLK = ((CMSDK_SYSCON ->SYSCLK & 0xFC7F) | (PCLK_DIV<<7)) ;  

#else
	
 	ResetBit_BB(0x4002100C, 10); 
	
	#endif
 	 
	Wait(50);
	#if (IOSC_24M_ENABLE == 1)
		
		CMSDK_ANAMIX ->	IOSC_24M_CON |= 0x1;    		    //enable osc 24M
			
	#else
	
				CMSDK_ANAMIX ->	IOSC_24M_CON &= 0xFFFFFFFE;  //disable osc 24M
	
	#endif		 
}


void SysClk_Update (void)
{
    CMSDK_SYSCON ->SYSCLK = ((CMSDK_SYSCON ->SYSCLK & 0xFFFC) | 2) ;    //HCLK_SROUCE
}


//<<< end of configuration section >>>
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
