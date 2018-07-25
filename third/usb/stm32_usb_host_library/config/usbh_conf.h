/**
  ******************************************************************************
  * @file    USB_Host/MSC_Standalone/Inc/usbh_conf.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    22-April-2016
  * @brief   General low level driver configuration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBH_CONF_H
#define __USBH_CONF_H

/* Includes ------------------------------------------------------------------*/
// #include "stm32f7xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Exported types ------------------------------------------------------------*/
#define USBH_MAX_NUM_ENDPOINTS                    3 //
#define USBH_MAX_NUM_INTERFACES                   10 //
#define USBH_MAX_NUM_CONFIGURATION                1
#define USBH_MAX_NUM_SUPPORTED_CLASS              5 // ֧��CLASS����
#define USBH_KEEP_CFG_DESCRIPTOR                  0
#define USBH_MAX_SIZE_CONFIGURATION               0x200
#define USBH_MAX_DATA_BUFFER                      0x200
#define USBH_DEBUG_LEVEL                          3
#define USBH_USE_OS                               0

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* CMSIS OS macros */   
#if (USBH_USE_OS == 1)
  #include "cmsis_os.h"
  #define   USBH_PROCESS_PRIO    osPriorityNormal
#endif

/* Memory management macros */   
#define USBH_malloc                               malloc
#define USBH_free                                 free
#define USBH_memset                               memset
#define USBH_memcpy                               memcpy
#define USBH_PRINT                                printf

/* DEBUG macros */   
// TODO: ����궨�����bug
#if (USBH_DEBUG_LEVEL > 0)
#define USBH_UsrLog(...)                           do{USBH_PRINT(__VA_ARGS__);}while(0)
#else
#define USBH_UsrLog(...)   
#endif 
                            
                            
#if (USBH_DEBUG_LEVEL > 1)
#define USBH_ErrLog(...)                           do{USBH_PRINT("USB : error : ") ;\
                                                                     USBH_PRINT(__VA_ARGS__);\
                                                                     USBH_PRINT("\r\n");}while(0)
#else
#define USBH_ErrLog(...)   
#endif 
                                                      
#if (USBH_DEBUG_LEVEL > 2)                         
#define USBH_DbgLog(...)                           do{USBH_PRINT("USB : info : ") ;\
                                                                     USBH_PRINT(__VA_ARGS__);\
                                                                     USBH_PRINT("\r\n");}while(0)
#else
#define USBH_DbgLog(...)                         
#endif

/* Exported functions ------------------------------------------------------- */

// SOF = 125us when HS(or FS in HS)
#define SOF_CYCLE                            125
// timeout = 50s,
#define MSC_TEST_UNIT_READY_TIMEOUT            400000 // 30000000/125

#endif /* __USBH_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
