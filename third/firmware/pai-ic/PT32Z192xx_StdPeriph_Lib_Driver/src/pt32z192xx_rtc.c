
#include "pt32z192xx_rtc.h"

void RTC_Init(CMSDK_RTC_TypeDef* RTCx, RTC_InitTypeDef* RTC_Init)
{
  /* Check the parameters */
  assert_param(IS_RTC_ALL_PERIPH(RTCx));
  assert_param(IS_RTC_CLOCK(RTC_Init->SourceClock));
  assert_param(IS_RTC_MatchValue(RTC_Init->MatchValue));
  assert_param(IS_RTC_Loadvalue(RTC_Init->Loadvalue));  

  RTCx->CR |= ((RTC_Init->SourceClock)<<1);
  RTCx->MR = RTC_Init->MatchValue;
  RTCx->LR = RTC_Init->Loadvalue;

}


void RTC_BaseStructInit(RTC_InitTypeDef* RTC_BaseInit)
{
  /* Set the default configuration */
  RTC_BaseInit->MatchValue = 0xFFFFFFFF;
  RTC_BaseInit->Loadvalue = 0x0000;
	RTC_BaseInit->SourceClock = External_32K;
}


void RTC_Cmd(CMSDK_RTC_TypeDef* RTCx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RTC_ALL_PERIPH(RTCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));	

  if (NewState == ENABLE)
  {

    RTCx->CR |= 0x01;
  }

}


void RTC_ITConfig(CMSDK_RTC_TypeDef* RTCx, uint32_t RTC_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RTC_ALL_PERIPH(RTCx));
  assert_param(IS_RTC_INT(RTC_IT));	
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	

    if (NewState != DISABLE)
    {
      /* Enable the interrupt sources */
      RTCx->IR |= RTC_IT;
    }
    else
    {
      /* Disable the interrupt sources */
      RTCx->IR &= ~RTC_IT;
	
	  }
}

FlagStatus RTC_GetFlagStatus(CMSDK_RTC_TypeDef* RTCx, uint32_t RTC_FLAG_Status)
{
  /* Check the parameters */
  assert_param(IS_RTC_ALL_PERIPH(RTCx));
  assert_param(IS_RTC_RISR_FLAG(RTC_FLAG_Status));	

  if ((RTCx->RISR & RTC_FLAG_Status) != 0)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

FlagStatus RTC_GetFlagINT(CMSDK_RTC_TypeDef* RTCx, uint32_t RTC_FLAG_INT)
{
  /* Check the parameters */
  assert_param(IS_RTC_ALL_PERIPH(RTCx));
  assert_param(IS_RTC_IR_FLAG(RTC_FLAG_INT));	

  if ((RTCx->RISR & RTC_FLAG_INT) != 0)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}


void RTC_ClearFlag(CMSDK_RTC_TypeDef* RTCx, uint32_t RTC_FLAG)
{
  /* Check the parameters */
  assert_param(IS_RTC_ALL_PERIPH(RTCx));
  assert_param(IS_RTC_IR_FLAG(RTC_FLAG));
	
  /* Clear the flags */
  RTCx->ICR = RTC_FLAG;
}


void RTC_Load(CMSDK_RTC_TypeDef* RTCx, uint32_t Loadvalue)
{
	assert_param(IS_RTC_ALL_PERIPH(RTCx));
	assert_param(IS_RTC_Loadvalue(Loadvalue)); 
	
	RTCx->LR =Loadvalue;
	
	
}


void RTC_Match(CMSDK_RTC_TypeDef* RTCx, uint32_t Matchvalue)
{
	assert_param(IS_RTC_ALL_PERIPH(RTCx));
	assert_param(IS_RTC_Loadvalue(Matchvalue)); 
	
	RTCx->MR =Matchvalue;

}
