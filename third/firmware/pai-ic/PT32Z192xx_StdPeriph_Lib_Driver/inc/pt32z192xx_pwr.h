/**
  ******************************************************************************
  * @file    pt32z192xx_pwr.h
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   This file contains all the functions prototypes for the PWR firmware 
  *          library.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PT32Z192XX_PWR_H
#define __PT32Z192XX_PWR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx.h"

/** @addtogroup STM32F0xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup PWR
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/** @defgroup PWR_Exported_Constants
  * @{
  */ 

/** @defgroup PWR_PVD_detection_level 
  * @brief    This parameters are only applicable for STM32F051 and STM32F072 devices
  * @{
  */ 

#define PWR_PVDLevel_0                  ANAMIX_VDDLVDCTRL_2V0
#define PWR_PVDLevel_1                  ANAMIX_VDDLVDCTRL_1V7
#define PWR_PVDLevel_2                  ANAMIX_VDDLVDCTRL_2V2
#define PWR_PVDLevel_3                  ANAMIX_VDDLVDCTRL_2V5
#define PWR_PVDLevel_4                  ANAMIX_VDDLVDCTRL_2V75
#define PWR_PVDLevel_5                  ANAMIX_VDDLVDCTRL_3V0
#define PWR_PVDLevel_6                  ANAMIX_VDDLVDCTRL_3V5
#define PWR_PVDLevel_7                  ANAMIX_VDDLVDCTRL_4V0 

#define IS_PWR_PVD_LEVEL(LEVEL) (((LEVEL) == PWR_PVDLevel_0) || ((LEVEL) == PWR_PVDLevel_1)|| \
                                 ((LEVEL) == PWR_PVDLevel_2) || ((LEVEL) == PWR_PVDLevel_3)|| \
                                 ((LEVEL) == PWR_PVDLevel_4) || ((LEVEL) == PWR_PVDLevel_5)|| \
                                 ((LEVEL) == PWR_PVDLevel_6) || ((LEVEL) == PWR_PVDLevel_7))
/**
  * @}
  */
/** @defgroup PWR_LVD_detection_level 
  * @brief    This parameters are only applicable for STM32F051 and STM32F072 devices
  * @{
  */ 

#define PWR_LVDLevel_0                  ANAMIX_LDOLVDCTRL_1V35
#define PWR_LVDLevel_1                  ANAMIX_LDOLVDCTRL_1V4
#define PWR_LVDLevel_2                  ANAMIX_LDOLVDCTRL_1V45

#define IS_PWR_LVD_LEVEL(LEVEL) (((LEVEL) == PWR_LVDLevel_0) || ((LEVEL) == PWR_LVDLevel_1)|| \
                                 ((LEVEL) == PWR_LVDLevel_7))
/**
  * @}
  */	
	

/** @defgroup PWR_WakeUp_Pins 
  * @{
  */

#define PWR_WakeUpPin_1                 PWR_CSR_EWUP1
#define PWR_WakeUpPin_2                 PWR_CSR_EWUP2
#define PWR_WakeUpPin_3                 PWR_CSR_EWUP3 /*!< only applicable for STM32F072 devices */
#define PWR_WakeUpPin_4                 PWR_CSR_EWUP4 /*!< only applicable for STM32F072 devices */
#define PWR_WakeUpPin_5                 PWR_CSR_EWUP5 /*!< only applicable for STM32F072 devices */
#define PWR_WakeUpPin_6                 PWR_CSR_EWUP6 /*!< only applicable for STM32F072 devices */
#define PWR_WakeUpPin_7                 PWR_CSR_EWUP7 /*!< only applicable for STM32F072 devices */
#define PWR_WakeUpPin_8                 PWR_CSR_EWUP8 /*!< only applicable for STM32F072 devices */
#define IS_PWR_WAKEUP_PIN(PIN) (((PIN) == PWR_WakeUpPin_1) || ((PIN) == PWR_WakeUpPin_2) || \
                                ((PIN) == PWR_WakeUpPin_3) || ((PIN) == PWR_WakeUpPin_4) || \
                                ((PIN) == PWR_WakeUpPin_5) || ((PIN) == PWR_WakeUpPin_6) || \
                                ((PIN) == PWR_WakeUpPin_7) || ((PIN) == PWR_WakeUpPin_8))
/**
  * @}
  */

 
/** @defgroup PWR_Regulator_state_is_Sleep_STOP_mode 
  * @{
  */

#define PWR_Regulator_ON                ((uint32_t)0x00000000)
#define PWR_Regulator_LowPower          PWR_CR_LPSDSR
#define IS_PWR_REGULATOR(REGULATOR) (((REGULATOR) == PWR_Regulator_ON) || \
                                     ((REGULATOR) == PWR_Regulator_LowPower))
/**
  * @}
  */

/** @defgroup PWR_SLEEP_mode_entry 
  * @{
  */

#define PWR_SLEEPEntry_WFI              ((uint8_t)0x01)
#define PWR_SLEEPEntry_WFE              ((uint8_t)0x02)
#define IS_PWR_SLEEP_ENTRY(ENTRY) (((ENTRY) == PWR_SLEEPEntry_WFI) || ((ENTRY) == PWR_SLEEPEntry_WFE))
 
/**
  * @}
  */

/** @defgroup PWR_STOP_mode_entry 
  * @{
  */

#define PWR_STANDBYEntry_WFI               ((uint8_t)0x01)
#define PWR_STANDBYEntry_WFE               ((uint8_t)0x02)
#define IS_PWR_STANDBY_ENTRY(ENTRY) (((ENTRY) == PWR_STANDBYEntry_WFI) || ((ENTRY) == PWR_STANDBYEntry_WFE))
 
/**
  * @}
  */

/** @defgroup PWR_Flag 
  * @{
  */

#define PWR_FLAG_WU                     PWR_CSR_WUF
#define PWR_FLAG_SB                     PWR_CSR_SBF
#define PWR_FLAG_PVDO                   PWR_CSR_PVDO /*!< Not applicable for STM32F030 devices */
#define PWR_FLAG_VREFINTRDY             PWR_CSR_VREFINTRDYF 

#define IS_PWR_GET_FLAG(FLAG) (((FLAG) == PWR_FLAG_WU) || ((FLAG) == PWR_FLAG_SB) || \
                               ((FLAG) == PWR_FLAG_PVDO) || ((FLAG) == PWR_FLAG_VREFINTRDY))

#define IS_PWR_CLEAR_FLAG(FLAG) (((FLAG) == PWR_FLAG_WU) || ((FLAG) == PWR_FLAG_SB))


#define  Set_SLEEPONEXIT    SetBit_BB((int32_t)&SCB->SCR, 1)
#define  Reset_SLEEPONEXIT  ResetBit_BB((int32_t)&SCB->SCR, 1)
#define  Set_SLEEPDEEP    SetBit_BB((int32_t)&SCB->SCR, 2)
#define  Reset_SLEEPDEEP  ResetBit_BB((int32_t)&SCB->SCR, 2)
#define  Set_SEVONPEND    SetBit_BB((int32_t)&SCB->SCR, 4)
#define  Reset_SEVONPEND  ResetBit_BB((int32_t)&SCB->SCR, 4)


typedef enum
{
  POWER_SLEEP_WFE = 0,         /* sleep then wait for event     */
  POWER_SLEEP_WFI              /* sleep then wait for interrupt */
}POWER_SLEEP_ENTRY_Enum;
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* Function used to set the PWR configuration to the default reset state ******/


/* Backup Domain Access function **********************************************/


/* PVD configuration functions ************************************************/


/* WakeUp pins configuration functions ****************************************/


/* Low Power modes configuration functions ************************************/


/* Flags management functions *************************************************/
void Power_Sleep(POWER_SLEEP_ENTRY_Enum SleepEntry);
void Power_DeepSleep(POWER_SLEEP_ENTRY_Enum SleepEntry);
void Power_UPF(void);
#ifdef __cplusplus
}
#endif

#endif /* __STM32F0XX_PWR_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
