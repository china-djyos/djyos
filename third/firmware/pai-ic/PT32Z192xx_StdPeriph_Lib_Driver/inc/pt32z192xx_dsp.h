/**
  ******************************************************************************
  * @file    pt32z192xx_DSP.h
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   This file contains all the functions prototypes for the DSP firmware 
  *          library.
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
#ifndef __PT32Z192XX_DSP_H
#define __PT32Z192XX_DSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/*!< Includes ----------------------------------------------------------------*/
#include "pt32z192xx.h"

/** @addtogroup PT32Z192xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup DSP
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  DSP_BATCH Compute Init structure definition
  */
  
typedef struct
{

  uint8_t  BATCH_OFFSET;                      
	
	uint16_t BATCH_SLWR;
	
  uint16_t BATCH_SAWR;   
	
  uint8_t  BATCH_DEF_LENERROR;                
	
  uint8_t  BATCH_DEF_DIRERROR;                      
	
}DSP_BATCHInitTypeDef;


typedef struct
{

  uint8_t  ROTATE_ANGLE;                      
	
	int16_t  ROTATE_X0;
	
  int16_t  ROTATE_Y0;   
	
  int16_t  ROTATE_X1;                
	
  int16_t  ROTATE_Y1;                      
	
}DSP_ROTATEInitTypeDef;


typedef struct
{

  uint32_t  PIXEL_ADDR1;                      
	
  uint32_t  PIXEL_ADDR2;  
	
  uint16_t  PIXEL_WIDTH;    
	
  uint16_t  PIXEL_HIGH;                  
	
  int16_t   PIXEL_CXOFFSET;  
	
  int16_t   PIXEL_CYOFFSET; 

  uint8_t   PIXEL_ANGLE; 	
	
}DSP_PIXELInitTypeDef;


/* Exported constants --------------------------------------------------------*/

/** @defgroup 
  * @{
  */

#define CONV_LINA 1
#define CONV_LINB 2
#define CONV_LINC 3
#define CONV_LIND 4
#define CONV_LINE 5
#define CONV_LINF 6
#define CONV_LING 7
#define CONV_LINH 8
#define CONV_LINI 9
#define CONV_LINJ 10
#define CONV_LINK 11
#define CONV_LINL 12
#define CONV_LINM 13


#define IS_DSP_ABSVALUE(VALUE)                       ((VALUE)<=0x7FFFFFFF)

#define IS_DSP_SQRTVALUE(VALUE)                      ((VALUE)<=0xFFFFFFFF)

#define IS_DSP_ATANXVALUE(VALUE)                     ((VALUE)<=0x7FFFFFFF)
#define IS_DSP_ATANYVALUE(VALUE)                     ((VALUE)<=0x7FFFFFFF)

#define IS_DSP_GETBIT1VALUE(VALUE)                   ((VALUE)<=0xFFFFFFFF)

#define IS_DSP_BATCH_OFFSETVALUE(VALUE)              ((VALUE)<=0x000000FF)
#define IS_DSP_BATCH_SLWRVALUE(VALUE)                ((VALUE)<=0x000003FF)
#define IS_DSP_BATCH_SAWRVALUE(VALUE)                ((VALUE)<=0x000003FF)
#define IS_DSP_BATCH_DEF_LENERRORVALUE(VALUE)        ((VALUE)<=0x000000FF)
#define IS_DSP_BATCH_DEF_DIRERRORVALUE(VALUE)        ((VALUE)<=0x000000FF)
#define IS_DSP_BATCH_LENVALUE(VALUE)                 ((VALUE)<=0x000000FF)
#define IS_DSP_BATCH_A1VALUE(VALUE)                  ((VALUE)<=0x000000FF)
#define IS_DSP_BATCH_A2VALUE(VALUE)                  ((VALUE)<=0x000000FF)

#define IS_DSP_ROTATE_ANGLEVALUE(VALUE)              ((VALUE)<=0x000000FF)
#define IS_DSP_ROTATE_X0VALUE(VALUE)                 ((VALUE)<=0x00007FFF)
#define IS_DSP_ROTATE_Y0VALUE(VALUE)                 ((VALUE)<=0x00007FFF)
#define IS_DSP_ROTATE_X1VALUE(VALUE)                 ((VALUE)<=0x00007FFF)
#define IS_DSP_ROTATE_Y1VALUE(VALUE)                 ((VALUE)<=0x00007FFF)

#define IS_DSP_CONV_COREVALUE(VALUE)                 ((VALUE)<=0x000000FF)

#define IS_DSP_CONV_LINE(LINE)                    (((LINE) == CONV_LINA)   || \
                                                   ((LINE) == CONV_LINB)   || \
                                                   ((LINE) == CONV_LINC)   || \
                                                   ((LINE) == CONV_LIND)  || \
                                                   ((LINE) == CONV_LINE)  || \
																							     ((LINE) == CONV_LINF)  || \
																							     ((LINE) == CONV_LING)  || \
																								   ((LINE) == CONV_LINH)  || \
																								   ((LINE) == CONV_LINI)  || \
																								   ((LINE) == CONV_LINJ)  || \
																								   ((LINE) == CONV_LINK)  || \
																								   ((LINE) == CONV_LINL)  || \
																								   ((LINE) == CONV_LINM))

#define IS_DSP_PIXELBIT_DATABYTE(BYTE)            ((BYTE)<=0x000000FF)
#define IS_DSP_PIXELBIT_ADDRVALUE(ADDR)           ((ADDR)<=0x000000FF)

#define IS_DSP_CAPBIT_ADDRVALUE(ADDR)             ((0x20000000<=ADDR) && (ADDR<=0x20020000))
#define IS_DSP_CAPBIT_NUM(NUM)                    ((NUM)<=0x0000FFFF)
#define IS_DSP_CAPBIT_LOCATION(LOCATION)          ((LOCATION)<=0x0000FFFF)


#define IS_DSP_PIXEL_ADDR1(ADDR)                  ((0x20000000<=ADDR) && (ADDR<=0x20020000))
#define IS_DSP_PIXEL_ADDR2(ADDR)                  ((0x20000000<=ADDR) && (ADDR<=0x20020000))
#define IS_DSP_PIXEL_WIDTH(WIDTH)                 ((WIDTH)<=0x0000FFFF)
#define IS_DSP_PIXEL_HIGH(HIGH)                   ((HIGH)<=0x0000FFFF)
#define IS_DSP_PIXEL_CXOFFSET(CXOFFSET)           ((CXOFFSET)<=0x00007FFF)
#define IS_DSP_PIXEL_CYOFFSET(CYOFFSET)           ((CYOFFSET)<=0x00007FFF)
#define IS_DSP_PIXEL_ANGLE(ANGLE)                 ((ANGLE)<=0x000000FF)

#define IS_DSP_MAXMIN_CALC_SADDR(SADDR)           ((0x20000000<=SADDR) && (SADDR<=0x20020000))
#define IS_DSP_MAXMIN_CALC_EADDR(EADDR)           ((0x20000000<=EADDR) && (EADDR<=0x20020000))



/**
  * @}
  */

/** @defgroup DSP_PolynomialSize
  * @brief    
  * @{
  */
	
uint32_t DSP_ABS_CALC(int32_t abs_value);	
uint16_t DSP_SQRT_CALC(uint32_t sqrt_value);
void DSP_SQRT_RST(void);
uint8_t DSP_ATAN_CALC(int32_t atan_x , int32_t atan_y);
void DSP_GETBIT1_CALC(uint32_t getbit1_data);
uint8_t DSP_GETBIT1_RESULT(void);
void DSP_Batchcompute_Init(DSP_BATCHInitTypeDef* DSP_BATCHInitStructure);
void DSP_Batchcompute_LENConfig(uint8_t test_len , uint8_t line_len);
void DSP_Batchcompute_A1Config(uint8_t test_A1 , uint8_t line_A1);
void DSP_Batchcompute_A2Config(uint8_t test_A2 , uint8_t line_A2);
uint8_t DSP_Batchcompute_Result(void);
uint8_t DSP_ROTATE_CALC(DSP_ROTATEInitTypeDef* DSP_ROTATEInitStructure);
void DSP_CONVCore_Sel(uint8_t core_sel);
void DSP_CONVCMD(FunctionalState NewState);
void DSP_ResultDiv_CMD(FunctionalState NewState);
uint32_t DSP_CONV_Result(void);
void DSP_CONVData_Config(uint8_t LINESEL,uint16_t def_conv_iW,unsigned char *buf);
void DSP_PIXELBIT1_DATA0(uint8_t byte0,uint8_t byte1,uint8_t byte2,uint8_t byte3);
void DSP_PIXELBIT1_DATA1(uint8_t byte4,uint8_t byte5,uint8_t byte6,uint8_t byte7);
void DSP_PIXELBIT2_DATA0(uint8_t byte0,uint8_t byte1,uint8_t byte2,uint8_t byte3);
void DSP_PIXELBIT2_DATA1(uint8_t byte4,uint8_t byte5,uint8_t byte6,uint8_t byte7);
void DSP_PIXELBIT_CFG(uint8_t addr_a0,uint8_t addr_a1,uint8_t addr_b0,uint8_t addr_b1);
uint8_t DSP_PIXELCMP_Result(void);
void DSP_CAPBIT_SADDR(uint32_t addr);
void DSP_CAPBIT_Config(uint16_t capbit_num ,uint16_t capbit_location);
uint32_t Get_CAPBIT_RESULT(void);
void DSP_PIXELComparison_Init(DSP_PIXELInitTypeDef* DSP_PIXELInitStructure);
void DSP_PIXELComparison_CMD(FunctionalState NewState);
uint16_t Get_DSP_PIXEL_SAMECOUNT(void);
uint16_t Get_DSP_PIXEL_ALLCOUNT(void);
uint16_t Get_DSP_PIXEL_ARRY1_ZEROCOUNT(void);
uint16_t Get_DSP_PIXEL_ARRY2_ZEROCOUNT(void);
uint16_t Get_DSP_PIXEL_EVEN_SAMECOUNT(void);
uint16_t Get_DSP_PIXEL_EVEN_ALLCOUNT(void);
uint16_t Get_DSP_PIXEL_EVEN_ARRY1_ZEROCOUNT(void);
uint16_t Get_DSP_PIXEL_EVEN_ARRY2_ZEROCOUNT(void);
uint16_t Get_DSP_PIXEL_ALLZEROCOUNT(void);
uint16_t Get_DSP_PIXEL_EVEN_ALLZEROCOUNT(void);
void DSP_MAXMIN_CALC_SADDR(uint32_t Addr);
void DSP_MAXMIN_CALC_EADDR(uint32_t Addr);
uint32_t Get_DSP_MAXMIN_CALC_MINValue(void);
uint32_t Get_DSP_MAXMIN_CALC_MINIndex(void);
uint32_t Get_DSP_MAXMIN_CALC_MAXValue(void);
uint32_t Get_DSP_MAXMIN_CALC_MAXIndex(void);

	



/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Configuration of the DSP computation unit **********************************/

/* DSP computation ************************************************************/


/* Independent register (IDR) access (write/read) *****************************/


#ifdef __cplusplus
}
#endif

#endif /* __STM32F0XX_DSP_H */

/**
  * @}
  */

/**
  * @}
  */


