/**
  ******************************************************************************
  * @file    pt32z192xx_DSP.c
  * @author  MCD Application Team
  * @version
  * @date   
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of DSP computation unit peripheral:
  *            + Configuration of the DSP computation unit
  *            + DSP computation of one/many 32-bit data
  *            + DSP Independent register (IDR) access
  *
  *  @verbatim
 ===============================================================================
                     ##### How to use this driver #####
 ===============================================================================
    [..]
    
         (+) Enable DSP AHB clock using RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DSP, ENABLE)
             function
         (+) If required, select the reverse operation on input data 
             using DSP_ReverseInputDataSelect()  
         (+) If required, enable the reverse operation on output data
             using DSP_ReverseOutputDataCmd(Enable)
         (+) use DSP_CalcDSP() function to compute the DSP of a 32-bit data
             or use DSP_CalcBlockDSP() function to compute the DSP if a 32-bit 
             data buffer
            (@) To compute the DSP of a new data use DSP_ResetDR() to reset
                 the DSP computation unit before starting the computation
                 otherwise you can get wrong DSP values.
      
     @endverbatim
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

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx_dsp.h"

/** @addtogroup PT32F0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup DSP 
  * @brief DSP driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup DSP_Private_Functions
  * @{
  */

/** @defgroup DSP_Group1 Configuration of the DSP computation unit functions
 *  @brief   Configuration of the DSP computation unit functions 
 *
@verbatim
 ===============================================================================
                     ##### DSP configuration functions #####
 ===============================================================================

@endverbatim
  * @{
  */



/**
  * @brief  Computes An Absolute value.
  * @param  abs_value:  
  *          This parameter range can be: -2147483648<=abs_value<=2147483647.
  * @retval None
  */
uint32_t DSP_ABS_CALC(int32_t abs_value)
{
  /* Check the parameters */
  assert_param(IS_DSP_ABSVALUE(abs_value));

  DSP->ABSCALC = abs_value;  
	
  return DSP->ABSCALC;
}

/**
  * @brief  Computes An Sqrt value.
  * @param  sqrt_value:  
  *          This parameter range can be: 0<=sqrt_value<=0xFFFFFFFF.
  * @retval None
  */
uint16_t DSP_SQRT_CALC(uint32_t sqrt_value)
{
  /* Check the parameters */
  assert_param(IS_DSP_SQRTVALUE(sqrt_value));

  DSP->SQRTCALC = sqrt_value;  
	
  return DSP->SQRTCALC;
}

/**
  * @brief  Reset Sqrt Register.
  * @retval None
  */
void DSP_SQRT_RST(void)
{

  DSP->DSP_STAUS = 0x1;     

}

/**
  * @brief  Computes An Arc-Tangent value.
  * @param  atan_x:  The x coordinate values
  *          This parameter range can be: -2147483648<=atan_x<=2147483647.
  * @param  atan_y:  The y coordinate values
  *          This parameter range can be: -2147483648<=atan_y<=2147483647.
  * @retval None
  */
uint8_t DSP_ATAN_CALC(int32_t atan_x , int32_t atan_y)
{
  /* Check the parameters */
  assert_param(IS_DSP_ATANXVALUE(atan_x));
  assert_param(IS_DSP_ATANYVALUE(atan_y));	

  DSP->ATAN_X =  atan_x;  
  DSP->ATAN_Y =  atan_y; 
	
  return DSP->ATAN_RESULT;
}


/**
  * @brief  Computes An GETBIT1 value.
  * @param  getbit1_data: 
  *          This parameter range can be: 0<=getbit1_data<=0xFFFFFFFF.
  * @retval None
  */
void DSP_GETBIT1_CALC(uint32_t getbit1_data)
{
  /* Check the parameters */
  assert_param(IS_DSP_GETBIT1VALUE(getbit1_data));

	DSP->GET_BIT1 =  getbit1_data; 

}

/**
  * @brief  Get GETBIT1 Calculate Result.
  * @retval None
  */
uint8_t DSP_GETBIT1_RESULT(void)
{

   return DSP->GET_BIT1; 

}

/**
  * @brief  Initialize Batch Compute Parameter .
  * @param  
  * @retval None
  */
void DSP_Batchcompute_Init(DSP_BATCHInitTypeDef* DSP_BATCHInitStructure)
{
  /* Check the parameters */
  assert_param(IS_DSP_BATCH_OFFSETVALUE(DSP_BATCHInitStructure->BATCH_OFFSET));
  assert_param(IS_DSP_BATCH_SLWRVALUE(DSP_BATCHInitStructure->BATCH_SLWR));
  assert_param(IS_DSP_BATCH_SAWRVALUE(DSP_BATCHInitStructure->BATCH_SAWR));
  assert_param(IS_DSP_BATCH_DEF_LENERRORVALUE(DSP_BATCHInitStructure->BATCH_DEF_LENERROR));
  assert_param(IS_DSP_BATCH_DEF_DIRERRORVALUE(DSP_BATCHInitStructure->BATCH_DEF_DIRERROR));	

	DSP->BATCH_OFFSET =  DSP_BATCHInitStructure->BATCH_OFFSET;   
	DSP->SLWR         =  DSP_BATCHInitStructure->BATCH_SLWR;            
	DSP->SAWR         =  DSP_BATCHInitStructure->BATCH_SAWR;            
	DSP->DEF_LENERROR =  DSP_BATCHInitStructure->BATCH_DEF_LENERROR;  
	DSP->DEF_DIRERROR =  DSP_BATCHInitStructure->BATCH_DEF_DIRERROR;  

}

/**
  * @brief  Config Batch Compute Len Parameter.
  * @param  test_len:  
  *          This parameter range can be: 0<=test_len<=0xFF.
  * @param  line_len:  
  *          This parameter range can be: 0<=line_len<=0xFF.
  * @retval None
  */
void DSP_Batchcompute_LENConfig(uint8_t test_len , uint8_t line_len)
{
  /* Check the parameters */
  assert_param(IS_DSP_BATCH_LENVALUE(test_len));
  assert_param(IS_DSP_BATCH_LENVALUE(line_len));	

  DSP->BATCH_LEN =  ((test_len)|(line_len<<8));  

}

/**
  * @brief  Config Batch Compute A1 Parameter.
  * @param  test_A1:  
  *          This parameter range can be: 0<=test_A1<=0xFF.
  * @param  line_A1:  
  *          This parameter range can be: 0<=line_A1<=0xFF.
  * @retval None
  */
void DSP_Batchcompute_A1Config(uint8_t test_A1 , uint8_t line_A1)
{
  /* Check the parameters */
  assert_param(IS_DSP_BATCH_A1VALUE(test_A1));
  assert_param(IS_DSP_BATCH_A1VALUE(line_A1));	

  DSP->BATCH_A1 =  ((test_A1)|(line_A1<<8));  

}

/**
  * @brief  Config Batch Compute A2 Parameter.
  * @param  test_A2:  
  *          This parameter range can be: 0<=test_A2<=0xFF.
  * @param  line_A2:  
  *          This parameter range can be: 0<=line_A2<=0xFF.
  * @retval None
  */
void DSP_Batchcompute_A2Config(uint8_t test_A2 , uint8_t line_A2)
{
  /* Check the parameters */
  assert_param(IS_DSP_BATCH_A2VALUE(test_A2));
  assert_param(IS_DSP_BATCH_A2VALUE(line_A2));	

  DSP->BATCH_A2 =  ((test_A2)|(line_A2<<8));  

}

/**
  * @brief  Get Batch Calculate Result.
  * @retval None
  */
uint8_t DSP_Batchcompute_Result(void)
{

   return DSP ->PAIRSIMILARITY; 

}


/**
  * @brief  Computes Point coordinate rotation .
  * @param  
  * @retval None
  */
uint8_t DSP_ROTATE_CALC(DSP_ROTATEInitTypeDef* DSP_ROTATEInitStructure)
{
  /* Check the parameters */
  assert_param(IS_DSP_ROTATE_ANGLEVALUE(DSP_ROTATEInitStructure->ROTATE_ANGLE));
  assert_param(IS_DSP_ROTATE_X0VALUE(DSP_ROTATEInitStructure->ROTATE_X0));
  assert_param(IS_DSP_ROTATE_Y0VALUE(DSP_ROTATEInitStructure->ROTATE_Y0));
  assert_param(IS_DSP_ROTATE_X1VALUE(DSP_ROTATEInitStructure->ROTATE_X0));
  assert_param(IS_DSP_ROTATE_Y1VALUE(DSP_ROTATEInitStructure->ROTATE_Y1));	

	DSP->ROTATE_ANGLE =  DSP_ROTATEInitStructure->ROTATE_ANGLE;   
	
	DSP->ROTATE_XY0   =  (((DSP_ROTATEInitStructure->ROTATE_X0)<<16) | (DSP_ROTATEInitStructure->ROTATE_Y0)) ;            

	DSP->ROTATE_XY1   =  (((DSP_ROTATEInitStructure->ROTATE_X1)<<16) | (DSP_ROTATEInitStructure->ROTATE_Y1)) ;          

  /* return result */	
  return DSP->ROTATE_XY;    
}


/**
  * @brief  Select Convolution Core.
  * @param  core_sel: 
  *          This parameter range can be: 0<=core_sel<=0xFF.
  * @retval None
  */
void DSP_CONVCore_Sel(uint8_t core_sel)
{
  /* Check the parameters */
  assert_param(IS_DSP_CONV_COREVALUE(core_sel));

	DSP -> CONV_CORE_SEL = core_sel;

}

/**
  * @brief  Start Convolution .
  * @param  NewState: new state of the DSP Convolution. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DSP_CONVCMD(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

	DSP->CONV_CTL &= (uint32_t)~((uint32_t)DSP_CONV_CTL_EN);	
	
  if (NewState != DISABLE)
  {
    /* Enable the DSP Convolution  */
    DSP->CONV_CTL = DSP_CONV_CTL_EN;
  }
  else
  {
    /* Disable the DSP Convolution */
    DSP->CONV_CTL = 0;
  }

}

/**
  * @brief  Convolution Result Div Enable or Disable .
  * @param  NewState: new state of the DSP Convolution Div 112211. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DSP_ResultDiv_CMD(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

	DSP->CONV_CTL &= (uint32_t)~((uint32_t)DSP_CONV_CTL_DIVEN);		
	
  if (NewState != DISABLE)
  {
    /* Enable the DSP CONV Result Div */
    DSP->CONV_CTL = DSP_CONV_CTL_DIVEN;
  }
  else
  {
    /* Disable the DSP CONV Result Div */
	DSP->CONV_CTL &= (uint32_t)~((uint32_t)DSP_CONV_CTL_DIVEN);	
  }

}


/**
  * @brief  Get Convolution Result.
  * @retval None
  */
uint32_t DSP_CONV_Result(void)
{

   return DSP ->CONV_OUT; 

}


/**
  * @brief  Config Convolution Line(x) data.
  * @param  
  * @retval None
  */

void DSP_CONVData_Config(uint8_t LINESEL,uint16_t def_conv_iW,unsigned char *buf)
{
  /* Check the parameters */
  assert_param(IS_DSP_CONV_LINE(LINESEL));
	
  switch (LINESEL)
	{	
	case CONV_LINA :	
	DSP->CONV_A1_A4  = *(buf) | (*(buf + def_conv_iW))<<8 | (*(buf + def_conv_iW*2)) <<16 | (*(buf + def_conv_iW *3))<<24;
	DSP->CONV_A5_A8  = *(buf +def_conv_iW*4) | (*(buf + def_conv_iW*5))<<8 | (*(buf + def_conv_iW*6))<<16 | (*(buf + def_conv_iW*7))<<24;
	DSP->CONV_A9_A12 = *(buf +def_conv_iW*8) | (*(buf + def_conv_iW*9))<<8 | (*(buf + def_conv_iW*10))<<16 | (*(buf + def_conv_iW *11))<<24;
	DSP->CONV_A13    = *(buf +def_conv_iW*12);
	break;
	
	case CONV_LINB :	
	DSP->CONV_B1_B4  = *(buf) | (*(buf + def_conv_iW))<<8 | (*(buf + def_conv_iW*2)) <<16 | (*(buf + def_conv_iW *3))<<24;
	DSP->CONV_B5_B8  = *(buf +def_conv_iW*4) | (*(buf + def_conv_iW*5))<<8 | (*(buf + def_conv_iW*6))<<16 | (*(buf + def_conv_iW*7))<<24;
	DSP->CONV_B9_B12 = *(buf +def_conv_iW*8) | (*(buf + def_conv_iW*9))<<8 | (*(buf + def_conv_iW*10))<<16 | (*(buf + def_conv_iW *11))<<24;
	DSP->CONV_B13    = *(buf +def_conv_iW*12);	
	break;
	
	case CONV_LINC :	
	DSP->CONV_C1_C4  = *(buf) | (*(buf + def_conv_iW))<<8 | (*(buf + def_conv_iW*2)) <<16 | (*(buf + def_conv_iW *3))<<24;
	DSP->CONV_C5_C8  = *(buf +def_conv_iW*4) | (*(buf + def_conv_iW*5))<<8 | (*(buf + def_conv_iW*6))<<16 | (*(buf + def_conv_iW*7))<<24;
	DSP->CONV_C9_C12 = *(buf +def_conv_iW*8) | (*(buf + def_conv_iW*9))<<8 | (*(buf + def_conv_iW*10))<<16 | (*(buf + def_conv_iW *11))<<24;
	DSP->CONV_C13    = *(buf +def_conv_iW*12);	
  break;
	
	case CONV_LIND :	
	DSP->CONV_D1_D4  = *(buf) | (*(buf + def_conv_iW))<<8 | (*(buf + def_conv_iW*2)) <<16 | (*(buf + def_conv_iW *3))<<24;
	DSP->CONV_D5_D8  = *(buf +def_conv_iW*4) | (*(buf + def_conv_iW*5))<<8 | (*(buf + def_conv_iW*6))<<16 | (*(buf + def_conv_iW*7))<<24;
	DSP->CONV_D9_D12 = *(buf +def_conv_iW*8) | (*(buf + def_conv_iW*9))<<8 | (*(buf + def_conv_iW*10))<<16 | (*(buf + def_conv_iW *11))<<24;
	DSP->CONV_D13    = *(buf +def_conv_iW*12);
  break;
	
	case CONV_LINE :	
	DSP->CONV_E1_E4  = *(buf) | (*(buf + def_conv_iW))<<8 | (*(buf + def_conv_iW*2)) <<16 | (*(buf + def_conv_iW *3))<<24;
	DSP->CONV_E5_E8  = *(buf +def_conv_iW*4) | (*(buf + def_conv_iW*5))<<8 | (*(buf + def_conv_iW*6))<<16 | (*(buf + def_conv_iW*7))<<24;
	DSP->CONV_E9_E12 = *(buf +def_conv_iW*8) | (*(buf + def_conv_iW*9))<<8 | (*(buf + def_conv_iW*10))<<16 | (*(buf + def_conv_iW *11))<<24;
	DSP->CONV_E13    = *(buf +def_conv_iW*12);
  break;
	
	case CONV_LINF :	
	DSP->CONV_F1_F4  = *(buf) | (*(buf + def_conv_iW))<<8 | (*(buf + def_conv_iW*2)) <<16 | (*(buf + def_conv_iW *3))<<24;
	DSP->CONV_F5_F8  = *(buf +def_conv_iW*4) | (*(buf + def_conv_iW*5))<<8 | (*(buf + def_conv_iW*6))<<16 | (*(buf + def_conv_iW*7))<<24;
	DSP->CONV_F9_F12 = *(buf +def_conv_iW*8) | (*(buf + def_conv_iW*9))<<8 | (*(buf + def_conv_iW*10))<<16 | (*(buf + def_conv_iW *11))<<24;
	DSP->CONV_F13    = *(buf +def_conv_iW*12);
  break;
	
	case CONV_LING :	
	DSP->CONV_G1_G4  = *(buf) | (*(buf + def_conv_iW))<<8 | (*(buf + def_conv_iW*2)) <<16 | (*(buf + def_conv_iW *3))<<24;
	DSP->CONV_G5_G8  = *(buf +def_conv_iW*4) | (*(buf + def_conv_iW*5))<<8 | (*(buf + def_conv_iW*6))<<16 | (*(buf + def_conv_iW*7))<<24;
	DSP->CONV_G9_G12 = *(buf +def_conv_iW*8) | (*(buf + def_conv_iW*9))<<8 | (*(buf + def_conv_iW*10))<<16 | (*(buf + def_conv_iW *11))<<24;
	DSP->CONV_G13    = *(buf +def_conv_iW*12);
  break;
	
	case CONV_LINH :	
	DSP->CONV_H1_H4  = *(buf) | (*(buf + def_conv_iW))<<8 | (*(buf + def_conv_iW*2)) <<16 | (*(buf + def_conv_iW *3))<<24;
	DSP->CONV_H5_H8  = *(buf +def_conv_iW*4) | (*(buf + def_conv_iW*5))<<8 | (*(buf + def_conv_iW*6))<<16 | (*(buf + def_conv_iW*7))<<24;
	DSP->CONV_H9_H12 = *(buf +def_conv_iW*8) | (*(buf + def_conv_iW*9))<<8 | (*(buf + def_conv_iW*10))<<16 | (*(buf + def_conv_iW *11))<<24;
	DSP->CONV_H13    = *(buf +def_conv_iW*12);
  break;
	
	case CONV_LINI :	
	DSP->CONV_I1_I4  = *(buf) | (*(buf + def_conv_iW))<<8 | (*(buf + def_conv_iW*2)) <<16 | (*(buf + def_conv_iW *3))<<24;
	DSP->CONV_I5_I8  = *(buf +def_conv_iW*4) | (*(buf + def_conv_iW*5))<<8 | (*(buf + def_conv_iW*6))<<16 | (*(buf + def_conv_iW*7))<<24;
	DSP->CONV_I9_I12 = *(buf +def_conv_iW*8) | (*(buf + def_conv_iW*9))<<8 | (*(buf + def_conv_iW*10))<<16 | (*(buf + def_conv_iW *11))<<24;
	DSP->CONV_I13    = *(buf +def_conv_iW*12);
  break;
	
	case CONV_LINJ :	
	DSP->CONV_J1_J4  = *(buf) | (*(buf + def_conv_iW))<<8 | (*(buf + def_conv_iW*2)) <<16 | (*(buf + def_conv_iW *3))<<24;
	DSP->CONV_J5_J8  = *(buf +def_conv_iW*4) | (*(buf + def_conv_iW*5))<<8 | (*(buf + def_conv_iW*6))<<16 | (*(buf + def_conv_iW*7))<<24;
	DSP->CONV_J9_J12 = *(buf +def_conv_iW*8) | (*(buf + def_conv_iW*9))<<8 | (*(buf + def_conv_iW*10))<<16 | (*(buf + def_conv_iW *11))<<24;
	DSP->CONV_J13    = *(buf +def_conv_iW*12);
  break;
	
	case CONV_LINK :	
	DSP->CONV_K1_K4  = *(buf) | (*(buf + def_conv_iW))<<8 | (*(buf + def_conv_iW*2)) <<16 | (*(buf + def_conv_iW *3))<<24;
	DSP->CONV_K5_K8  = *(buf +def_conv_iW*4) | (*(buf + def_conv_iW*5))<<8 | (*(buf + def_conv_iW*6))<<16 | (*(buf + def_conv_iW*7))<<24;
	DSP->CONV_K9_K12 = *(buf +def_conv_iW*8) | (*(buf + def_conv_iW*9))<<8 | (*(buf + def_conv_iW*10))<<16 | (*(buf + def_conv_iW *11))<<24;
	DSP->CONV_K13    = *(buf +def_conv_iW*12);
	break;
	
	case CONV_LINL :	
	DSP->CONV_L1_L4  = *(buf) | (*(buf + def_conv_iW))<<8 | (*(buf + def_conv_iW*2)) <<16 | (*(buf + def_conv_iW *3))<<24;
	DSP->CONV_L5_L8  = *(buf +def_conv_iW*4) | (*(buf + def_conv_iW*5))<<8 | (*(buf + def_conv_iW*6))<<16 | (*(buf + def_conv_iW*7))<<24;
	DSP->CONV_L9_L12 = *(buf +def_conv_iW*8) | (*(buf + def_conv_iW*9))<<8 | (*(buf + def_conv_iW*10))<<16 | (*(buf + def_conv_iW *11))<<24;
	DSP->CONV_L13    = *(buf +def_conv_iW*12);
  break;
	
	case CONV_LINM :	
	DSP->CONV_M1_M4  = *(buf) | (*(buf + def_conv_iW))<<8 | (*(buf + def_conv_iW*2)) <<16 | (*(buf + def_conv_iW *3))<<24;
	DSP->CONV_M5_M8  = *(buf +def_conv_iW*4) | (*(buf + def_conv_iW*5))<<8 | (*(buf + def_conv_iW*6))<<16 | (*(buf + def_conv_iW*7))<<24;
	DSP->CONV_M9_M12 = *(buf +def_conv_iW*8) | (*(buf + def_conv_iW*9))<<8 | (*(buf + def_conv_iW*10))<<16 | (*(buf + def_conv_iW *11))<<24;
	DSP->CONV_M13    = *(buf +def_conv_iW*12);
	break;	
	
  }
}

/**
  * @brief  Config PIXEL1BIT CMP Data0.
  * @param  bytex(0~3): 
  *          This parameter range can be: 0<=bytex<=0xFF.
  * @retval None
  */
void DSP_PIXELBIT1_DATA0(uint8_t byte0,uint8_t byte1,uint8_t byte2,uint8_t byte3)
{
  /* Check the parameters */
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte0));
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte1));	
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte2));	
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte3));	
	
  DSP->PIXEL1_DATA0 = ((byte0<<24) | (byte1<<16) | (byte2<<8) | (byte3)); 
	
}

/**
  * @brief  Config PIXEL1BIT CMP Data1.
  * @param  bytex(4~7): 
  *          This parameter range can be: 0<=bytex<=0xFF.
  * @retval None
  */
void DSP_PIXELBIT1_DATA1(uint8_t byte4,uint8_t byte5,uint8_t byte6,uint8_t byte7)
{
  /* Check the parameters */
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte4));
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte5));	
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte6));	
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte7));	
	
  DSP->PIXEL1_DATA1 = ((byte4<<24) | (byte5<<16) | (byte6<<8) | (byte7)); 
	
}


/**
  * @brief  Config PIXEL2BIT CMP Data0.
  * @param  bytex(0~3): 
  *          This parameter range can be: 0<=bytex<=0xFF.
  * @retval None
  */
void DSP_PIXELBIT2_DATA0(uint8_t byte0,uint8_t byte1,uint8_t byte2,uint8_t byte3)
{
  /* Check the parameters */
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte0));
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte1));	
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte2));	
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte3));	
	
  DSP->PIXEL2_DATA0 = ((byte0<<24) | (byte1<<16) | (byte2<<8) | (byte3)); 
	
}

/**
  * @brief  Config PIXELBIT CMP Data1.
  * @param  bytex(4~7): 
  *          This parameter range can be: 0<=bytex<=0xFF.
  * @retval None
  */
void DSP_PIXELBIT2_DATA1(uint8_t byte4,uint8_t byte5,uint8_t byte6,uint8_t byte7)
{
  /* Check the parameters */
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte4));
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte5));	
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte6));	
  assert_param(IS_DSP_PIXELBIT_DATABYTE(byte7));	
	
  DSP->PIXEL2_DATA1 = ((byte4<<24) | (byte5<<16) | (byte6<<8) | (byte7)); 
	
}


/**
  * @brief  Config PIXELBIT CMP ADDR.
  * @param  addrx: 
  *          This parameter range can be: 0<=addrx<=0xFF.
  * @retval None
  */

void DSP_PIXELBIT_CFG(uint8_t addr_a0,uint8_t addr_a1,uint8_t addr_b0,uint8_t addr_b1)
{
	
  /* Check the parameters */
  assert_param(IS_DSP_PIXELBIT_ADDRVALUE(addr_a0));
  assert_param(IS_DSP_PIXELBIT_ADDRVALUE(addr_a1));	
  assert_param(IS_DSP_PIXELBIT_ADDRVALUE(addr_b0));	
  assert_param(IS_DSP_PIXELBIT_ADDRVALUE(addr_b1));		
	
  DSP->PIXEL_CFG=  ((addr_a0<<24) | (addr_a1<<16) | (addr_b0<<8) | (addr_b1));    
	
}

/**
  * @brief  Get PIXEL_CMP Result.
  * @retval None
  */
uint8_t DSP_PIXELCMP_Result(void)
{

   return DSP ->PIXEL_CMP_RESULT; 

}

/**
  * @brief  Config CAPBIT Start ADDR.
  * @param  addr: 
  *          This parameter range can be: 0<=addrx<=0xFFFFFFFF.
  * @retval None
  */

void DSP_CAPBIT_SADDR(uint32_t addr)
{
  /* Check the parameters */
  assert_param(IS_DSP_CAPBIT_ADDRVALUE(addr));	
	
	DSP->CAPBIT_ADDR_CFG=  addr;  
}


/**
  * @brief  Config CAPBIT Num & Location.
  * @param  addr: 
  *          This parameter range can be: 0<=addrx<=0xFFFFFFFF.
  * @retval None
  */

void DSP_CAPBIT_Config(uint16_t capbit_num ,uint16_t capbit_location)
{
  /* Check the parameters */
  assert_param(IS_DSP_CAPBIT_NUM(capbit_num));
  assert_param(IS_DSP_CAPBIT_LOCATION(capbit_location));
	
	DSP->CAPBIT_CFG=  capbit_num <<16 | capbit_location;  
	
}

/**
  * @brief  Get CAPBIT Result.
  * @retval None
  */

uint32_t Get_CAPBIT_RESULT(void)
{
    return DSP->CAPBIT_READ_RESULT ;  
}



/**
  * @brief  Initialize PIXEL Comparison Parameter .
  * @param  
  * @retval None
  */
void DSP_PIXELComparison_Init(DSP_PIXELInitTypeDef* DSP_PIXELInitStructure)
{
  /* Check the parameters */
  assert_param(IS_DSP_PIXEL_ADDR1(DSP_PIXELInitStructure->PIXEL_ADDR1));
  assert_param(IS_DSP_PIXEL_ADDR2(DSP_PIXELInitStructure->PIXEL_ADDR2));
  assert_param(IS_DSP_PIXEL_WIDTH(DSP_PIXELInitStructure->PIXEL_WIDTH));
  assert_param(IS_DSP_PIXEL_HIGH(DSP_PIXELInitStructure->PIXEL_HIGH));
  assert_param(IS_DSP_PIXEL_CXOFFSET(DSP_PIXELInitStructure->PIXEL_CXOFFSET));	
  assert_param(IS_DSP_PIXEL_CYOFFSET(DSP_PIXELInitStructure->PIXEL_CYOFFSET));		
  assert_param(IS_DSP_PIXEL_ANGLE(DSP_PIXELInitStructure->PIXEL_ANGLE));		
	

	DSP->PIXEL_CMP_ADDR1 =  DSP_PIXELInitStructure->PIXEL_ADDR1;   
	DSP->PIXEL_CMP_ADDR2 =  DSP_PIXELInitStructure->PIXEL_ADDR2;            
	DSP->PIXEL_CMP_HW    =  (((DSP_PIXELInitStructure->PIXEL_HIGH) <<16) | (DSP_PIXELInitStructure->PIXEL_WIDTH));            
	DSP->PIXEL_CMP_XY    =  (((DSP_PIXELInitStructure->PIXEL_CXOFFSET) <<16) | (DSP_PIXELInitStructure->PIXEL_CYOFFSET));  ;  
	DSP->PIXEL_CMP_ANGLE =  DSP_PIXELInitStructure->PIXEL_ANGLE;  

}



/**
  * @brief  Start PIXEL Comparison .
  * @param  NewState: new state of the DSP PIXEL Comparison. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DSP_PIXELComparison_CMD(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

	
  if (NewState != DISABLE)
  {
    /* Enable the PIXEL Comparison */
    DSP->PIXEL_CMP_CTL = DSP_PPIXEL_CMP_CTL_EN;
  }
  else
  {
    /* Disable the PIXEL Comparison */
	  DSP->PIXEL_CMP_CTL = 0;	
  }

}


/**
  * @brief  Get PIXEL SAME COUNT Result.
  * @retval None
  */

uint16_t Get_DSP_PIXEL_SAMECOUNT(void)
{
    return DSP->PIXEL_RESULT ;  
}



/**
  * @brief  Get PIXEL ALL Intersect COUNT Result.
  * @retval None
  */

uint16_t Get_DSP_PIXEL_ALLCOUNT(void)
{
    return ((DSP->PIXEL_RESULT)>>16) ;  
}


/**
  * @brief  Get PIXEL ARRY1 ZERO COUNT  Result.
  * @retval None
  */

uint16_t Get_DSP_PIXEL_ARRY1_ZEROCOUNT(void)
{
    return ((DSP->PIXEL_ZERO_COUNTER0)>>16) ;  
}


/**
  * @brief  Get PIXEL ARRY2 ZERO COUNT  Result.
  * @retval None
  */

uint16_t Get_DSP_PIXEL_ARRY2_ZEROCOUNT(void)
{
    return (DSP->PIXEL_ZERO_COUNTER0) ;  
}


/**
  * @brief  Get PIXEL EVEN Line&Column SAME COUNT Result.
  * @retval None
  */

uint16_t Get_DSP_PIXEL_EVEN_SAMECOUNT(void)
{
    return DSP->PIXEL_RESULT_EVEN ;  
}

/**
  * @brief  Get PIXEL EVEN Line&Column ALL Intersect COUNT Result.
  * @retval None
  */

uint16_t Get_DSP_PIXEL_EVEN_ALLCOUNT(void)
{
    return ((DSP->PIXEL_RESULT_EVEN)>>16) ;  
}


/**
  * @brief  Get PIXEL EVEN Line&Column ARRY1 ZERO COUNT  Result.
  * @retval None
  */

uint16_t Get_DSP_PIXEL_EVEN_ARRY1_ZEROCOUNT(void)
{
    return ((DSP->PIXEL_ZERO_COUNTER0_EVEN)>>16) ;  
}


/**
  * @brief  Get PIXEL EVEN Line&Column ARRY2 ZERO COUNT  Result.
  * @retval None
  */

uint16_t Get_DSP_PIXEL_EVEN_ARRY2_ZEROCOUNT(void)
{
    return (DSP->PIXEL_ZERO_COUNTER0_EVEN) ;  
}


/**
  * @brief  Get PIXEL ALL Intersect ZERO COUNT Result.
  * @retval None
  */

uint16_t Get_DSP_PIXEL_ALLZEROCOUNT(void)
{
    return ((DSP->PIXEL_ZERO_COUNTER1)>>16) ;  
}



/**
  * @brief  Get PIXEL EVEN Line&Column ALL Intersect ZERO COUNT Result.
  * @retval None
  */

uint16_t Get_DSP_PIXEL_EVEN_ALLZEROCOUNT(void)
{
    return (DSP->PIXEL_ZERO_COUNTER1) ;  
}


/**
  * @brief  Config MAX&MIN  Calculate Start Addr.
  * @param  Addr: 
  *          This parameter range can be: 0x20000000<=Addr<=0x20020000 (SRAM RANGE).
  * @retval None
  */
void DSP_MAXMIN_CALC_SADDR(uint32_t Addr)
{
  /* Check the parameters */
  assert_param(IS_DSP_MAXMIN_CALC_SADDR(Addr));
	
	
  DSP->COMP_SADDR = Addr; 
	
}

/**
  * @brief  Config MAX&MIN Calculate End Addr.
  * @param  Addr: 
  *          This parameter range can be: 0x20000000<=Addr<=0x20020000 (SRAM RANGE).
  * @retval None
  */
void DSP_MAXMIN_CALC_EADDR(uint32_t Addr)
{
  /* Check the parameters */
  assert_param(IS_DSP_MAXMIN_CALC_EADDR(Addr));
	
	
  DSP->COMP_EADDR = Addr; 
	
}

/**
  * @brief  Get  MAX&MIN Calculate Min Value.
  * @retval None
  */

uint32_t Get_DSP_MAXMIN_CALC_MINValue(void)
{
    return  DSP->COMP_MIN ;  
}


/**
  * @brief  Get  MAX&MIN Calculate Min Index.
  * @retval None
  */

uint32_t Get_DSP_MAXMIN_CALC_MINIndex(void)
{
    return  DSP->COMP_MIN_INDEX ;  
}

/**
  * @brief  Get  MAX&MIN Calculate Max Value.
  * @retval None
  */

uint32_t Get_DSP_MAXMIN_CALC_MAXValue(void)
{
    return  DSP->COMP_MAX ;  
}


/**
  * @brief  Get  MAX&MIN Calculate MAX Index.
  * @retval None
  */

uint32_t Get_DSP_MAXMIN_CALC_MAXIndex(void)
{
    return  DSP->COMP_MAX_INDEX ;  
}



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

/************************ (C) *****END OF FILE****/
