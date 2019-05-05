/***************************************************************************
描述：analog control  routine
平台：
时间：2017-8-10
作者：张彦欣                                                 
****************************************************************************/
#include "SCM6XX_ANA_CTRL.h"

/*-----------------------------------------------------------------------------
  Function:		ADC_EnableChnInt                                                                                                                                                                                 
  input:      opt -- ENABLE/DISABLE
  output:     None
  Returns:    None                                                                                      
  Description:enable or disable channel-change interrupt                                   
 *-----------------------------------------------------------------------------*/
void ADC_Enable_Chn_Int(FunctionalState opt)
{
    if(opt == ENABLE)
        SGCC_ANACTL_P->ANA_CFG_REG |= ANA_CFG_REG_ADC_CHN_INTEN;
    else
        SGCC_ANACTL_P->ANA_CFG_REG &= ~ANA_CFG_REG_ADC_CHN_INTEN;   
}

/*-----------------------------------------------------------------------------
  Function:		ADC_EnableCalibInt                                                                                                                                                                                 
  input:      opt -- ENABLE/DISABLE
  output:     None
  Returns:    None                                                                                      
  Description:enable or disable channel calibration interrupt                                   
 *-----------------------------------------------------------------------------*/
void ADC_Enable_Calib_Int(FunctionalState opt)
{
    if(opt == ENABLE)
        SGCC_ANACTL_P->ANA_CFG_REG |= ANA_CFG_REG_CALIB_INTEN;
    else
        SGCC_ANACTL_P->ANA_CFG_REG &= ~ANA_CFG_REG_CALIB_INTEN;   
}

/*-----------------------------------------------------------------------------
  Function:		ADC_EnableSampInt                                                                                                                                                                                 
  input:      opt -- ENABLE/DISABLE
  output:     None
  Returns:    None                                                                                      
  Description:enable or disable ADC sampling interrupt                                   
 *-----------------------------------------------------------------------------*/
void ADC_Enable_Samp_Int(FunctionalState opt)
{
    if(opt == ENABLE)
        SGCC_ANACTL_P->ANA_CFG_REG |= ANA_CFG_REG_ADC_INTEN;
    else
        SGCC_ANACTL_P->ANA_CFG_REG &= ~ANA_CFG_REG_ADC_INTEN;   
}


/*-----------------------------------------------------------------------------
  Function:		ADC_GetFlagSts                                                                                                                                                                                 
  input:      flag-ANA_STAT_REG_CHS/ANA_STAT_REG_CALIBRE/ANA_STAT_REG_SMP_PULSE/ANA_STAT_REG_CONVSTART/ANA_STAT_REG_RSTN/ANA_STAT_REG_CS/ANA_STAT_REG_CHN_UPDATED/ANA_STAT_REG_CALIB_FAIL
                  /ANA_STAT_REG_ADC_CHN_INT/ANA_STAT_REG_CALIB_INT/ANA_STAT_REG_ADC11_INT~ANA_STAT_REG_ADC0_INT
  Returns:    None                                                                                      
  Description:get status                                    
 *-----------------------------------------------------------------------------*/
UINT8 ADC_Get_Flag_Status (UINT32 ADC_Flag)
{
    if(SGCC_ANACTL_P->ANA_STAT_REG & ADC_Flag)
        return 1;
    else
        return 0;
}

/*-----------------------------------------------------------------------------
  Function:		ADC_ClrIntSts                                                                                                                                                                                 
  input:      flag-ANA_STAT_REG_ADC_CHN_INT/ANA_STAT_REG_CALIB_INT/ANA_STAT_REG_ADC11_INT~ANA_STAT_REG_ADC0_INT
  Returns:    None                                                                                      
  Description:clear interrupt status                                    
 *-----------------------------------------------------------------------------*/
void ADC_Clear_Int_Status(UINT32 ADC_Int_Status)
{
    SGCC_ANACTL_P->ANA_STAT_REG = ADC_Int_Status;
}

/*-----------------------------------------------------------------------------
  Function:		ADC_GetRdySts                                                                                                                                                                                 
  input:      flag-ADC_RDY_REG_CH0~ADC_RDY_REG_CH11
  Returns:    None                                                                                      
  Description:get ADC data ready status                                    
 *-----------------------------------------------------------------------------*/
UINT8 ADC_Get_Ready_Status (UINT32 ADC_Ready_Flag)
{
    if(SGCC_ANACTL_P->ADC_RDY_REG & ADC_Ready_Flag)
        return 1;
    else
        return 0;
}

/*-----------------------------------------------------------------------------
  Function:		ADC_ConfigTrim                                                                                                                                                                                 
  input:      calibA0 - ADC_TRIM_CALIB_A0_M3~ADC_TRIM_CALIB_A0_M1/ADC_TRIM_CALIB_A0_0/ADC_TRIM_CALIB_A0_P1~ADC_TRIM_CALIB_A0_P4
              calibA1- ADC_TRIM_CALIB_A1_M3~ADC_TRIM_CALIB_A1_M1/ADC_TRIM_CALIB_A1_0/ADC_TRIM_CALIB_A1_P1~ADC_TRIM_CALIB_A1_P4
              cdacTrim - ADC_TRIM_TRIM_(n), n can be 0x0~0x7F
              ib - ADC_TRIM_IB_SMALLI/ADC_TRIM_IB_BIGI
  output:     None
  Returns:    None                                                                                      
  Description:configure ADC trim value                                
 *-----------------------------------------------------------------------------*/
void ADC_ConfigTrim(UINT32 calibA0, UINT32 calibA1, UINT32 cdacTrim, UINT32 ib)
{
	UINT32 temp;
	  
    temp = calibA0 | calibA1 | cdacTrim | ib;
    SGCC_ANACTL_P->ADC_TRIM_REG = temp;
}



/*-----------------------------------------------------------------------------
  Function:		ADC_Config                                                                                                                                                                                 
  input:      adcPara
typedef struct
{
    UINT32 smpWidth; //ADC_CON1_SMP_WIDTH_(n)
    UINT32 adcMode;  //ADC_CON1_ADC_MODE_0~ADC_CON1_ADC_MODE_3
    UINT32 trigEn;   //ENABLE/DISABLE
    UINT32 avgTime; //ADC_CON1_AVGTIMES_1/ADC_CON1_AVGTIMES_2/ADC_CON1_AVGTIMES_4/ADC_CON1_AVGTIMES_8
    UINT32 chSel; //ADC_CON3_MCHS_(n)
    UINT32 chCombined; //ADC_CON3_CHNCOMBINED_1/ADC_CON3_CHNCOMBINED_0
} ADC_PARA ;  
  Returns:    None                                                                                      
  Description:configure ADC work in auto mode                                    
 *-----------------------------------------------------------------------------*/
void ADC_Config(ADC_PARA 	ADC_Para)
{
    UINT32 temp;
    
    //step 1
    //disable ADC and auto-calibration
    ADC_Enable(DISABLE);
    ADC_Enable_Auto_Cal(DISABLE);
    
    //clear all adc interrupt
    ADC_Clear_Int_Status(ANA_STAT_REG_ADC_ALLINT);
    
    
    //step 2
    //enable interrupt
    ADC_Enable_Samp_Int(ENABLE);
    
    
    //step 3
    temp = ADC_Para.smpWidth | ADC_Para.adcMode ;
    
    if(ADC_Para.trigEn == ENABLE)
    {
        temp |= ADC_CON1_TRIG_EN ;
    }
    else
    {
        temp &= ~ADC_CON1_TRIG_EN ;
    }
    
    if(ADC_Para.adcMode == ADC_CON1_ADC_MODE_0)
    {
        temp |= ADC_Para.avgTime;
    }
    
    //config ADC_CON1
    SGCC_ANACTL_P->ADC_CON1_REG = temp;
    
    //step 4, configure ADC_CON2:CS_B==0, RST_N==1，CALIBRE==0
    SGCC_ANACTL_P->ADC_CON2_REG = ADC_CON2_CS_LOW | ADC_CON2_RSTN_HIGH | ADC_CON2_CALIBRE_0;
    
    //step 5, config ADC_CON3
    SGCC_ANACTL_P->ADC_CON3_REG = (ADC_Para.chSel);  
    if((ADC_Para.adcMode == ADC_CON1_ADC_MODE_2) || (ADC_Para.adcMode == ADC_CON1_ADC_MODE_3))
    {
        SGCC_ANACTL_P->ADC_CON3_REG |= ADC_Para.chCombined;
    }
    
    //step 6, enable ADC
    SGCC_ANACTL_P->ADC_CON4_REG |= ADC_CON4_ADCEN;       
}

/*-----------------------------------------------------------------------------
  Function:		ADC_Reset                                                                                                                                                                                 
  input:      opt  - ADC_CON2_RSTN_LOW / ADC_CON2_RSTN_HIGH
  output:     None
  Returns:    None                                                                                      
  Description:reset ADC or not                                   
 *-----------------------------------------------------------------------------*/
void ADC_Reset(FunctionalState opt)
{
    SGCC_ANACTL_P->ADC_CON2_REG &= ~ADC_CON2_RSTN;
    SGCC_ANACTL_P->ADC_CON2_REG |= opt;
   
}

/*-----------------------------------------------------------------------------
  Function:		ADC_SetCs                                                                                                                                                                                 
  input:      opt  - ADC_CON2_CS_LOW / ADC_CON2_CS_HIGH
  output:     None
  Returns:    None                                                                                      
  Description:set cs high or low                                   
 *-----------------------------------------------------------------------------*/
void ADC_Set_Cs(FunctionalState opt)
{
    SGCC_ANACTL_P->ADC_CON2_REG &= ~ADC_CON2_CS;  
    SGCC_ANACTL_P->ADC_CON2_REG |= opt;
   
}


/*-----------------------------------------------------------------------------
  Function:		ADC_SelectCh                                                                                                                                                                                 
  input:      chNo - ADC_CON3_MCHS_(n)
  output:     None
  Returns:    None                                                                                      
  Description:select ADC channel                                   
 *-----------------------------------------------------------------------------*/
void ADC_Select_Ch(UINT8 chNo)
{
    SGCC_ANACTL_P->ADC_CON3_REG = chNo;  
}

/*-----------------------------------------------------------------------------
  Function:		ADC_GetData                                                                                                                                                                                 
  input:      chNo - 0~11
  output:     None
  Returns:    None                                                                                      
  Description:read ADC data                                    
 *-----------------------------------------------------------------------------*/
UINT16 ADC_Get_Data(UINT8 chNo)
{
    UINT16 *ptr;
    ptr = (UINT16*)(SGCC_ANA_BASE + 0x80 + 4 * chNo);
 
    return (*ptr); 
}


/*-----------------------------------------------------------------------------
  Function:		ADC_Enable                                                                                                                                                                                 
  input:      opt -- ENABLE/DISABLE
  output:     None
  Returns:    None                                                                                      
  Description:disable ADC                                    
 *-----------------------------------------------------------------------------*/
void ADC_Enable(FunctionalState opt)
{
    if(opt == ENABLE)
        SGCC_ANACTL_P->ADC_CON4_REG |= ADC_CON4_ADCEN;
    else
        SGCC_ANACTL_P->ADC_CON4_REG &= ~ADC_CON4_ADCEN;   
}

/*-----------------------------------------------------------------------------
  Function:		ADC_EnableAutoCal                                                                                                                                                                                 
  input:      opt -- ENABLE/DISABLE
  output:     None
  Returns:    None                                                                                      
  Description:enable or disable ADC auto calibration                                  
 *-----------------------------------------------------------------------------*/
void ADC_Enable_Auto_Cal(FunctionalState opt)
{
    if(opt == ENABLE)
        SGCC_ANACTL_P->ADC_CON4_REG |= ADC_CON4_CALIB_EN;
    else
        SGCC_ANACTL_P->ADC_CON4_REG &= ~ADC_CON4_CALIB_EN;   
}



