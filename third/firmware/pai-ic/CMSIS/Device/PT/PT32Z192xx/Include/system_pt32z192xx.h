/**
  ******************************************************************************
  * @file    system_pt32z192xx.h
  * @author  应用开发团队
  * @version 
  * @date    
  * @brief   CMSIS Cortex-M0 Device Peripheral Access Layer System Header File.
  ******************************************************************************
  * @attention
  *
  * 
  *
  *
  * 
  * 
  *
  *      
  *
  * 
  * 
  *
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
  
/**
  * @brief Define to prevent recursive inclusion
  */
#ifndef __SYSTEM_PT32Z192XX_H
#define __SYSTEM_PT32Z192XX_H

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup PT32F0xx_System_Includes
  * @{
  */

/**
  * @}
  */


/** @addtogroup P32F0xx_System_Exported_types
  * @{
  */

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */

/**
  * @}
  */

/** @addtogroup PT32F0xx_System_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @addtogroup PT32F0xx_System_Exported_Macros
  * @{
  */

#define EOSC_TIMEOUT 0xFFFFFFFF

#define  IOSC_RC24M_VALUE     24000000UL
#define  EOSC_CRYSTAL_VALUE   24000000UL
#define  I0SC_32K_VALUE       32768UL

/**
  * @}
  */

/** @addtogroup PT32F0xx_System_Exported_Functions
  * @{
  */
  
extern void SystemInit(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_PT32Z192XX_H */

/**
  * @}
  */
  
/**
  * @}
  */  
/************************ (C) COPYRIGHT  *****END OF FILE****/
