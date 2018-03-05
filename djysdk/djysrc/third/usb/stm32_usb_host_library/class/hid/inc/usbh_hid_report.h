/**
  ******************************************************************************
  * @file    usbh_hid_usage.c 
  * @author  MCD Application Team
  * @version V3.2.2
  * @date    07-July-2015
  * @brief   This file contain the USAGE page codes              
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
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
/* Define to prevent recursive  ----------------------------------------------*/
#ifndef _USDH_HID_REPORT_H_
#define _USDH_HID_REPORT_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>


#define HID_ITEM_MAIN                    0
#define HID_MAIN_INPUT                    (u8)0x80
#define HID_MAIN_OUTPUT                    (u8)0x90
#define HID_MAIN_FEATURE                (u8)0xB0
#define HID_MAIN_COLLECTION                (u8)0xA0
#define HID_MAIN_END_COLLECTION            (u8)0xC0
                                        
#define HID_ITEM_GLOBAL                    1
#define HID_GLOBAL_USAGE_PAGE            (u8)0x00
#define HID_GLOBAL_LOGICAL_MIN            (u8)0x10
#define HID_GLOBAL_LOGICAL_MAX            (u8)0x20
#define HID_GLOBAL_PHYSICAL_MIN            (u8)0x30
#define HID_GLOBAL_PHYSICAL_MAX            (u8)0x40
#define HID_GLOBAL_UNIT_EXPONENT        (u8)0x50
#define HID_GLOBAL_UNIT                    (u8)0x60
#define HID_GLOBAL_REPORT_SIZE            (u8)0x70
#define HID_GLOBAL_REPORT_ID            (u8)0x80
#define HID_GLOBAL_REPORT_COUNT            (u8)0x90
#define HID_GLOBAL_PUSH                    (u8)0xA0
#define HID_GLOBAL_POP                    (u8)0xB0
                                        
#define HID_ITEM_LOCAL                    2
#define HID_LOCAL_USAGE                    (u8)0x00
#define HID_LOCAL_USAGE_MIN                (u8)0x10
#define HID_LOCAL_USAGE_MAX                (u8)0x20
#define HID_LOCAL_DESIGNATOR_INDEX        (u8)0x30
#define HID_LOCAL_DESIGNATOR_MIN        (u8)0x40
#define HID_LOCAL_DESIGNATOR_MAX        (u8)0x50
#define HID_LOCAL_STRING_INDEX            (u8)0x70
#define HID_LOCAL_STRING_MIN            (u8)0x80
#define HID_LOCAL_STRING_MAX            (u8)0x90
#define HID_LOCAL_DELIMITER                (u8)0xA0

#define HID_ITEM_RESERVED                3

#define HID_ITEM_SIZE(x)                (((u8)x) & 0x3)
#define HID_ITEM_TYPE(x)                ((((u8)x) & 0xC)>> 2)
#define HID_ITEM_TAG(x)                    ((((u8)x) & 0xF0))
#ifdef __cplusplus
}
#endif

#endif /* _USDH_HID_REPORT_H_ */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
