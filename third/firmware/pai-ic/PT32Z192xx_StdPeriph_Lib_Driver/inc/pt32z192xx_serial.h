/**
  ******************************************************************************
  * @file    pt32z192xx_serial.h
  * @author  应用开发团队
  * @version 
  * @date    
  * @brief   This file contains all the functions prototypes for the ADC firmware 
  *          library
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
#ifndef __PT32Z192XX_SERIAL_H
#define __PT32Z192XX_SERIAL_H

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx.h"

/* Exported functions --------------------------------------------------------------------------------------*/

uint16_t SERIAL_GetChar(void);
uint16_t SERIAL_PutChar(uint8_t ch);

#endif /* __HT32F52341_SERIAL_H ----------------------------------------------------------------------------*/
