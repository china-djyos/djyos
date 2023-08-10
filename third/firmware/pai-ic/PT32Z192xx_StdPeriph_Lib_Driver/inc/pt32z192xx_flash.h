/**
  ******************************************************************************
  * @file    pt32z192xx_flash.h
  * @author  应用开发团队
  * @version
  * @date
  * @brief
  *
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PT32Z192XX_FLASH_H
#define __PT32Z192XX_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx.h"

/** @addtogroup PT32Z192xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  FLASH Status
  */
typedef enum
{
  FLASH_WRITE_END=1,
  FLASH_ERASE_END,
  FLASH_BUSY,
  FLASH_CMD_ERR,
  FLASH_ADDR_ERR,
  FLASH_CACHE_HIT,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;

typedef enum
{
  PASSED,
  FAILED
}TestStatus;

/* Exported constants --------------------------------------------------------*/

/** @defgroup FLASH_Exported_Constants
  * @{
  */

/** @defgroup FLASH_Latency
  * @{
  */
#define FLASH_Latency_0                FLASH_ACR_LATENCY_0   /*!< FLASH Zero Latency cycle */
#define FLASH_Latency_1                FLASH_ACR_LATENCY_1   /*!< FLASH One Latency cycle */
#define FLASH_Latency_2                FLASH_ACR_LATENCY_2   /*!< FLASH Two Latency cycle */
#define FLASH_Latency_3                FLASH_ACR_LATENCY_3   /*!< FLASH Three Latency cycle */
#define FLASH_Latency_4                FLASH_ACR_LATENCY_4   /*!< FLASH Four Latency cycle */
#define FLASH_Latency_5                FLASH_ACR_LATENCY_5   /*!< FLASH Five Latency cycle */
#define FLASH_Latency_6                FLASH_ACR_LATENCY_6   /*!< FLASH Six Latency cycle */
#define FLASH_Latency_7                FLASH_ACR_LATENCY_7   /*!< FLASH Seven Latency cycle */


#define IS_FLASH_LATENCY(LATENCY) (((LATENCY) == FLASH_Latency_0)  || \
                                   ((LATENCY) == FLASH_Latency_1)  || \
                                   ((LATENCY) == FLASH_Latency_2)  || \
                                   ((LATENCY) == FLASH_Latency_3)  || \
                                   ((LATENCY) == FLASH_Latency_4)  || \
                                                                     ((LATENCY) == FLASH_Latency_5)  || \
                                                                     ((LATENCY) == FLASH_Latency_6)  || \
                                                                     ((LATENCY) == FLASH_Latency_7))

/**
  * @}
  */

/** @defgroup FLASH_Interrupts
  * @{
  */

#define FLASH_IT_WREND                   FLASH_IER_WREND      /*!< FLASH 写命令完成中断使能 */
#define FLASH_IT_CMDER                   FLASH_IER_CMDER      /*!< FLASH操作命令错误中断使能 */
#define FLASH_IT_ADER                    FLASH_IER_ADER       /*!< FLASH地址错误中断使能 */
#define FLASH_IT_CACHE_HIT               FLASH_IER_CACHE_HIT  /*!< Flash 访问总次数计数饱和中断使能 */



#define IS_FLASH_IT(IT)                ((((IT) & (uint32_t)0xFFFFEBFF) == 0x00000000) && (((IT) != 0x00000000)))
/**
  * @}
  */

/** @defgroup FLASH_Address
  * @{
  */
#if defined(PT32Z192Ex)     /*512K devices */
#define FLASH_PAGE_SIZE    ((uint16_t)0x200)     /*!< 512B */

#define MAINCODE_START_ADDR  ((uint32_t)0x00000000)
#define MAINCODE_END_ADDR    ((uint32_t)0x00080000)




#define IS_FLASH_PROGRAM_ADDRESS(ADDRESS)   ((ADDRESS) <= MAINCODE_END_ADDR)

#endif /* STM32F031 */
/**
  * @}
  */

/** @defgroup FLASH_Flags
  * @{
  */

#define FLASH_FLAG_WREND                 FLASH_ISR_WREND       /*!< FLASH 写命令完成标志 */
#define FLASH_FLAG_CMDER                 FLASH_ISR_CMDER       /*!< FLASH操作命令错误标志*/
#define FLASH_FLAG_ACCER                 FLASH_ISR_ACCER       /*!< FLASH存取错误标志*/
#define FLASH_FLAG_ADER                  FLASH_ISR_ADER        /*!< FLASH地址错误标志 */
#define FLASH_FLAG_SEC                   FLASH_ISR_SEC         /*!< FLASH 1个数据位错误被纠正标志位 */
#define FLASH_FLAG_DEC                   FLASH_ISR_DEC         /*!< FLASH 2个数据位错误被纠正标志位 */
#define FLASH_FLAG_CACHE_HIT             FLASH_ISR_CACHE_HIT   /*!< Flash 访问总次数计数饱和 ≥0xFF00_0000*/

#define IS_FLASH_GET_FLAG(FLAG)  (((FLAG) == FLASH_FLAG_WREND) ||((FLAG) == FLASH_FLAG_CMDER) ||((FLAG) == FLASH_FLAG_ACCER) || \
                                  ((FLAG) == FLASH_FLAG_ADER)  ||((FLAG) == FLASH_FLAG_CACHE_HIT)  ||((FLAG) == FLASH_FLAG_DEC)  ||((FLAG) == FLASH_FLAG_SEC))

#define IS_FLASH_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFFFFFF80) == 0x00000000) && ((FLAG) != 0x00000000))
/**
  * @}
  */

/** @defgroup FLASH_Timeout_definition
  * @{
  */
#define FLASH_ER_PRG_TIMEOUT         ((uint32_t)0x000B0000)

/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/**
  * @brief  FLASH Operation Commands.
  */
#define FLASH_OPCMD_PRO   ((uint32_t)0x00000000)
#define FLASH_OPMOD_RE    ((uint32_t)0x00000000)
#define FLASH_OPCMD_PERA   FLASH_ACR_PERA
#define FLASH_OPCMD_MERA   FLASH_ACR_MERA
#define FLASH_OPMOD_WR    FLASH_ACR_MODE
#define FLASH_OPSTR_STRT  FLASH_ACR_START

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  FLASH memory functions that can be executed from FLASH.
  */
/* FLASH Interface configuration functions ************************************/
void FLASH_SetLatency(uint32_t FLASH_Latency);

/* FLASH Memory Programming functions *****************************************/
FLASH_Status FLASH_ErasePage(uint32_t Page_Address);
FLASH_Status FLASH_EraseAllPages(void);
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data0,uint32_t Data1);

/* FLASH Memory Reading functions *****************************************/
uint32_t FLASH_ReadWord(uint32_t Address);
uint16_t FLASH_ReadHalfWord(uint32_t Address);
uint8_t FLASH_ReadByte(uint32_t Address);


/* FLASH Interrupts and flags management functions **********************************/
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState);
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG);
void FLASH_ClearFlag(uint32_t FLASH_FLAG);
FLASH_Status FLASH_GetStatus(void);
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout);
void FlashCache_Cmd(FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif /* __PT32Z192XX_FLASH_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) *****END OF FILE****/
